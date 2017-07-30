#include"FbxLoader.h"
#include<assert.h>
#include<Source\Util\Win\WinFunc.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Thread\CriticalSection.h>

namespace util {

	FbxLoader::FbxLoader()
	{
		m_pFbxManager = FbxManager::Create();
		assert(m_pFbxManager != nullptr && "マネージャ作成失敗");

		m_pImporter = FbxImporter::Create(m_pFbxManager, "Importer");
		m_pScene = FbxScene::Create(m_pFbxManager, "Scene");
	}

	FbxLoader::~FbxLoader()
	{
		m_pFbxManager->Destroy();
	}

	void FbxLoader::loadFBX(const std::string & filePath)
	{
		bool isFaild = !m_pImporter->Initialize(filePath.c_str());

		//読み込みに失敗した場合
		if (isFaild) {
			std::string error = { filePath + "読み込み失敗" };
			assert(false && error.c_str());
		}

		m_pImporter->Import(m_pScene);

		//ルートノードを取得しておく
		m_pRootNode = m_pScene->GetRootNode();
		//あらかじめ三角形化しておく
		FbxGeometryConverter converter(m_pFbxManager);
		converter.Triangulate(m_pScene, true);

#ifdef _MDEBUG
		//ルートフレームの名前の表示
		std::string str1 = "RootMeshName : ";
		std::string str2 = m_pRootNode->GetName();

		util::WinFunc::OutLog(str1 + str2);
#endif
	}

	std::vector<std::vector<util::Vec3>> FbxLoader::getVertex()
	{
		std::vector<std::vector<util::Vec3>> vertexResult;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			std::vector<util::Vec3> vertexList;

			//!今処理するメッシュに含まれる頂点の数
			const int vertexCount = mesh->GetControlPointsCount();
			FbxVector4 * vertex = mesh->GetControlPoints();
			auto node = mesh->GetNode();
			//!移動量
			auto trans = node->LclTranslation.Get();
			//!回転量
			auto rotate = node->LclRotation.Get();
			//!拡大縮小
			auto scale = node->LclScaling.Get();

			util::Mat4 localWorld(
				scale[0], 0, 0, 0,
				0, scale[1], 0, 0,
				0, 0, scale[2], 0,
				trans[0], trans[1], trans[2], 1);

			util::Mat4 localRotate(
				1, 0, 0, 0,
				0, cos(rotate[0]), sin(rotate[1]), 0,
				0, -sin(rotate[1]), cos(rotate[0]), 0,
				0, 0, 0, 1);

			auto localMat = localWorld.toXMMatrix() *  localRotate.toXMMatrix();

			for (int i = 0; vertexCount > i; ++i) {
				//!頂点の一時的格納先
				util::Vec4 vertexBuffer;
				vertexBuffer.x = (float)vertex[i][0];
				vertexBuffer.y = (float)vertex[i][1];
				vertexBuffer.z = (float)vertex[i][2];
				vertexBuffer.w = 1;

				auto vec = XMVector4Transform(vertexBuffer.toXMVector(), localMat);
				vertexBuffer = util::Vec4(vec);
				util::Vec3 result;
				result.x = vertexBuffer.x;
				result.y = vertexBuffer.y;
				result.z = vertexBuffer.z;

				vertexList.emplace_back(result);
			}

			vertexResult.emplace_back(vertexList);

		});
		return vertexResult;
	}

	std::vector<std::vector<int>> FbxLoader::getIndex()
	{
		std::vector<std::vector<int>> indexResult;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {

			std::vector<int> indexList;

			//!今処理するメッシュに含まれるインデックスの数
			const int indexCount = mesh->GetPolygonVertexCount();
			int * index = mesh->GetPolygonVertices();
			for (int i = 0; indexCount > i; ++i) {
				indexList.emplace_back(index[i]);
			}

			indexResult.emplace_back(indexList);
		});
		return indexResult;
	}

	std::vector<std::vector<util::Vec3>> FbxLoader::getNormal()
	{
		std::vector<std::vector<util::Vec3>> normalResult;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {

			std::vector<util::Vec3> normalList;

			//!法線セット
			FbxGeometryElementNormal* normal = mesh->GetElementNormal(0);

			auto re = normal->GetReferenceMode();
			assert(re == FbxGeometryElement::eDirect && "法線が頂点順に格納されていません");
			auto mapMode = normal->GetMappingMode();
			//!今処理するメッシュに含まれるインデックスの数

			//!法線の数
			const int normalNum = normal->GetDirectArray().GetCount();

			for (int i = 0; i != normalNum; ++i) {
				util::Vec3 normalBuffer;
				normalBuffer.x = (float)normal->GetDirectArray().GetAt(i)[0];
				normalBuffer.y = (float)normal->GetDirectArray().GetAt(i)[1];
				normalBuffer.z = (float)normal->GetDirectArray().GetAt(i)[2];
				normalList.emplace_back(normalBuffer);
			}

			//頂点ごとに格納されていた場合インデックスで展開する
			if (mapMode == FbxGeometryElement::eByControlPoint) {
				std::vector<util::Vec3> normalListByIndex;
				const int indexCount = mesh->GetPolygonVertexCount();
				normalListByIndex.resize(indexCount);
				int * index = mesh->GetPolygonVertices();
				for (int i = 0; indexCount > i; ++i) {
					normalListByIndex[i] = normalList[index[i]];
				}
				normalList = normalListByIndex;
			}

			normalResult.emplace_back(normalList);
		});

		return normalResult;
	}

	std::vector<std::vector<util::Vec2>> FbxLoader::getUV()
	{
		std::vector<std::vector<util::Vec2>> uvResult;

		//eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
		//	FbxStringList uvsetName;
		//	mesh->GetUVSetNames(uvsetName);
		//	FbxArray<FbxVector2> uvsets;
		//	mesh->GetPolygonVertexUVs(uvsetName.GetStringAt(0), uvsets);
		//	for (int i = 0; i != uvsets.GetCount(); ++i) {
		//		util::Vec2 uv;
		//		uv.x = (float)uvsets.GetAt(i)[0];
		//		uv.y = 1.0f - (float)uvsets.GetAt(i)[1];
		//		uvList.emplace_back(uv);
		//	}
		//});

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {

			std::vector<util::Vec2> uvList;

			auto uvElement = mesh->GetElementUV();
			//!UVの数
			auto uvCount = uvElement->GetDirectArray().GetCount();
			auto& uvIndex = uvElement->GetIndexArray();
			auto indexCount = uvIndex.GetCount();

			for (int i = 0; i != indexCount; ++i) {
				util::Vec2 uv;
				auto j = uvIndex.GetAt(i);
				uv.x = (float)uvElement->GetDirectArray().GetAt(j)[0];
				uv.y = 1.0f - (float)uvElement->GetDirectArray().GetAt(j)[1];
				uvList.emplace_back(uv);
			}
			uvResult.emplace_back(uvList);
		});

		//eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
		//	std::vector<util::Vec2> uvList;

		//	auto uvElement = mesh->GetElementUV();
		//	auto&& mode = getModes(uvElement);
		//	auto& indexArray = uvElement->GetIndexArray();
		//	auto& directArray = uvElement->GetDirectArray();
		//	auto meshCount = getMeshCount();
		
		//	if (mode.mappingMode == FbxGeometryElement::eByControlPoint) {
		//		auto indexList = getIndex();

		//		util::foreach(meshCount, [&](int mesh) {
		//			for (auto index : indexList) {
		//				auto uvIndex = (mode.referenceMode == FbxGeometryElement::eDirect)
		//					? index[mesh] :
		//					indexArray.GetAt(index[mesh]);
		//				auto fbxUV = directArray.GetAt(uvIndex);
		//				util::Vec2 uv;
		//				uv.x = (float)fbxUV[0];
		//				uv.y = 1.0f - (float)fbxUV[1];
		//				uvList.emplace_back(uv);
		//			}
		//		
		//		})
		//	}
		//	else if (mode.mappingMode == FbxGeometryElement::eByPolygonVertex) {
		//		auto indexByPolygonVertex = 0;
		//		auto polygonCount = mesh->GetPolygonCount();
		//		for (int i = 0; i != polygonCount; ++i) {
		//			auto polygonSize = mesh->GetPolygonSize(i);
		//			for (int j = 0; j != polygonSize; ++j) {
		//				auto index = (mode.referenceMode == FbxGeometryElement::eDirect)
		//					? indexByPolygonVertex :
		//					indexArray.GetAt(indexByPolygonVertex);
		//				auto fbxUV = directArray.GetAt(index);
		//				util::Vec2 uv;
		//				uv.x = (float)fbxUV[0];
		//				uv.y = 1.0f - (float)fbxUV[1];
		//				uvList.emplace_back(uv);
		//				++indexByPolygonVertex;
		//			}
		//		}
		//	}
		//	uvResult.emplace_back(uvList);
		//});

		return uvResult;
	}

	std::vector<std::vector<util::Vec3>> FbxLoader::getBinormal()
	{
		std::vector<std::vector<util::Vec3>> binormalResult;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			std::vector<util::Vec3> binormalList;

			auto binormalElement = mesh->GetElementBinormal();
			const int binormalCount = binormalElement->GetDirectArray().GetCount();
			for (int i = 0; binormalCount > i; ++i) {

				util::Vec3 binormal;
				binormal.x = (float)binormalElement->GetDirectArray().GetAt(i)[0];
				binormal.y = (float)binormalElement->GetDirectArray().GetAt(i)[1];
				binormal.z = (float)binormalElement->GetDirectArray().GetAt(i)[2];
				binormalList.emplace_back(binormal);
			}
			binormalResult.emplace_back(binormalList);
		});
		return binormalResult;
	}

	std::vector<std::vector<util::Vec3>> FbxLoader::getTangent()
	{
		std::vector<std::vector<util::Vec3>> tangentResult;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			std::vector<util::Vec3> tangentList;

			auto tangentElement = mesh->GetElementTangent();
			const int tangentCount = tangentElement->GetDirectArray().GetCount();
			for (int i = 0; tangentCount > i; ++i) {

				util::Vec3 tangent;
				tangent.x = (float)tangentElement->GetDirectArray().GetAt(i)[0];
				tangent.y = (float)tangentElement->GetDirectArray().GetAt(i)[1];
				tangent.z = (float)tangentElement->GetDirectArray().GetAt(i)[2];
				tangentList.emplace_back(tangent);
			}
			tangentResult.emplace_back(tangentList);
		});
		return tangentResult;
	}

	std::vector<std::vector<util::Vec3>> FbxLoader::getControlPoint(const std::vector<int>& indexList)
	{
		std::vector<std::vector<util::Vec3>> controlResult;

		controlResult.reserve(indexList.size());

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			std::vector<util::Vec3> controlPointList;

			for (auto index : indexList) {
				auto controlPoint = mesh->GetControlPointAt(index);
				util::Vec3 point;
				point.x = controlPoint[0];
				point.y = controlPoint[1];
				point.z = controlPoint[2];
				controlPointList.emplace_back(point);
			}
			controlResult.emplace_back(controlPointList);
		});

		return controlResult;
	}

	std::vector<util::Mat4> FbxLoader::getBoneInitMatrix()
	{
		//メッシュ事　ボーン事
		std::vector<util::Mat4> boneInitMatList;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			
			//スキンは１こしかないと想定
			FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
			if (!skin)return;

			//! ボーンの数
			const auto culsterCount = skin->GetClusterCount();

			util::foreach(culsterCount, [&](int j) {
				auto cluster = skin->GetCluster(j);


				//!影響する点の数
				int impactNum = cluster->GetControlPointIndicesCount();
				if (impactNum == 0)return;

				FbxAMatrix initMat;
				cluster->GetTransformLinkMatrix(initMat);

				util::Mat4 motionMat(
					initMat.Get(0, 0), initMat.Get(0, 1), initMat.Get(0, 2), initMat.Get(0, 3),
					initMat.Get(1, 0), initMat.Get(1, 1), initMat.Get(1, 2), initMat.Get(1, 3),
					initMat.Get(2, 0), initMat.Get(2, 1), initMat.Get(2, 2), initMat.Get(2, 3),
					initMat.Get(3, 0), initMat.Get(3, 1), initMat.Get(3, 2), initMat.Get(3, 3)
				);

				boneInitMatList.emplace_back(motionMat);

			});
		});

		return boneInitMatList;
	}

	std::vector<VertecxWeithgt> FbxLoader::getBoneWeight()
	{
		//メッシュごと
		std::vector<VertecxWeithgt> vertexWeightByMesh;

		int boneCount = 0;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			//頂点の数分配列を確保
			VertecxWeithgt vertexWight(mesh->GetControlPointsCount());

			//スキンは常に１つを想定
			FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
			if (!skin)return;

			//! ボーンの数
			const auto culsterCount = skin->GetClusterCount();

			util::foreach(culsterCount, [&](int j) {

				auto cluster = skin->GetCluster(j);

				//!影響する点の数
				int impactNum = cluster->GetControlPointIndicesCount();
				if (impactNum == 0)return;

				int* pointArray = cluster->GetControlPointIndices();
				auto* weightArray = cluster->GetControlPointWeights();

				util::foreach(impactNum, [&](int i) {
					vertexWight.weightList[pointArray[i]].index = pointArray[i];
					vertexWight.weightList[pointArray[i]].boneID = boneCount;
					vertexWight.weightList[pointArray[i]].weight = (float)weightArray[i];
				});

				//影響を持つボーンだけで構成する
				boneCount += 1;
			});
			vertexWeightByMesh.emplace_back(vertexWight);
		});

		return vertexWeightByMesh;
	}


	std::vector<std::vector<std::pair<std::string,util::Mat4>>> FbxLoader::getMotionMatrix()
	{
		//フレーム事　ボーン事
		std::vector<std::vector<std::pair<std::string, util::Mat4>>> boneInitMatListByFrame;

		//スタート時間と終了時間取得
		auto motionInfo = getMotionInfo();
		//単位時間取得
		auto period = getPeriod();
		int frameNum = motionInfo.endTime.Get() / period.Get();

		boneInitMatListByFrame.resize(frameNum);

		int boneCount = 0;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {

			auto count = mesh->GetDeformerCount();

			//スキンは１こしかないと想定
			FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
			if (!skin)return;

			//! ボーンの数
			const auto culsterCount = skin->GetClusterCount();

			util::foreach(culsterCount, [&](int j) {
				auto cluster = skin->GetCluster(j);

				//!影響する点の数
				int impactNum = cluster->GetControlPointIndicesCount();
				if (impactNum == 0)return;

				FbxAMatrix initMat;
				cluster->GetTransformLinkMatrix(initMat);
				
				auto node = cluster->GetLink();
				util::foreach(frameNum, [&](int i) {
					FbxTime frame = i * period.Get();
					auto motionMatFbx = node->EvaluateGlobalTransform(frame);
					motionMatFbx = motionMatFbx * initMat.Inverse();
					
					util::Mat4 motionMat(
						motionMatFbx.Get(0, 0), motionMatFbx.Get(1, 0), motionMatFbx.Get(2, 0), motionMatFbx.Get(3, 0),
						motionMatFbx.Get(0, 1), motionMatFbx.Get(1, 1), motionMatFbx.Get(2, 1), motionMatFbx.Get(3, 1),
						motionMatFbx.Get(0, 2), motionMatFbx.Get(1, 2), motionMatFbx.Get(2, 2), motionMatFbx.Get(3, 2),
						motionMatFbx.Get(0, 3), motionMatFbx.Get(1, 3), motionMatFbx.Get(2, 3), motionMatFbx.Get(3, 3)
					);
					
					boneInitMatListByFrame[i].emplace_back(std::make_pair<std::string, util::Mat4 > (cluster->GetLink()->GetName(), motionMat.toXMMatrix()));
				});
				boneCount += 1;
			});
		});

		return boneInitMatListByFrame;
	}

	int FbxLoader::getMeshCount()
	{
		int meshCount = 0;
		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			meshCount += 1;
		});
		return meshCount;
	}

	MotionData FbxLoader::getMotionInfo()
	{
		FbxArray<FbxString*> motionNames;

		//FBXに格納されているモーション名一覧取得
		m_pScene->FillAnimStackNameArray(motionNames);

		assert(motionNames.Size() != 0 && "アニメーションが格納されていません");

		auto takeInfo = m_pScene->GetTakeInfo(motionNames[0]->Buffer());

		MotionData result;

		result.startTime = takeInfo->mLocalTimeSpan.GetStart();

		result.endTime = takeInfo->mLocalTimeSpan.GetStop();

		return result;
	}

	std::vector<std::vector<std::pair<int, FbxCluster*>>> FbxLoader::getAllCluster()
	{
		//!メッシュごとに格納
		std::vector<std::vector<std::pair<int, FbxCluster*>>> clusterListByMesh;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			std::vector<std::pair<int, FbxCluster*>> clusterList;
			const auto skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);

			util::foreach(skinCount, [&](int i) {
				FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));
				if (!skin)return;

				//! ボーンの数
				const auto culsterCount = skin->GetClusterCount();

				util::foreach(culsterCount, [&](int j) {
					clusterList.emplace_back(std::make_pair(j, skin->GetCluster(j)));
				});
			});
			clusterListByMesh.emplace_back(clusterList);
		});

		return clusterListByMesh;
	}

	FbxTime FbxLoader::getPeriod()
	{
		auto& timeSetting = m_pScene->GetGlobalSettings();
		auto mode = timeSetting.GetTimeMode();
		FbxTime period;
		period.SetTime(0, 0, 0, 1, 0, mode);

		return period;
	}

	FbxSkeleton * FbxLoader::getSkeleton(FbxNode * node)
	{
		auto attri = node->GetNodeAttribute();

		if (attri == nullptr)return nullptr;
		if (attri->GetAttributeType() != FbxNodeAttribute::eSkeleton)return nullptr;

		return node->GetSkeleton();
	}

	FbxMesh * FbxLoader::getMesh(FbxNode * node)
	{
		auto attri = node->GetNodeAttribute();

		if (attri == nullptr)return nullptr;
		if (attri->GetAttributeType() != FbxNodeAttribute::eMesh)return nullptr;

		return node->GetMesh();
	}

	void FbxLoader::eachMesh(FbxNode * node, std::function<void(FbxMesh*mesh)> action)
	{
		FbxMesh* mesh = getMesh(node);

		//メッシュが取れた場合のみ取り出し処理をする
		if (mesh != nullptr) {
			action(mesh);
		}

		for (int i = 0, end = node->GetChildCount(); i < end; ++i) {
			eachMesh(node->GetChild(i), action);
		}
	}

	//void FbxLoader::eachSkeleton(FbxCluster * rootCluster, std::function<void(FbxSkeleton* FbxCluster)> action)
	//{
	//	

	//	//メッシュが取れた場合のみ取り出し処理をする
	//	if (mesh != nullptr) {
	//		action(mesh);
	//	}

	//	for (int i = 0, end = node->GetChildCount(); i < end; ++i) {
	//		eachSkeleton(node->GetChild(i), action);
	//	}
	//}






}