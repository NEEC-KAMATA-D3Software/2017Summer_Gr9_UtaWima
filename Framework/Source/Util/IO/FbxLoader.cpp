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
		assert(m_pFbxManager != nullptr && "�}�l�[�W���쐬���s");

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

		//�ǂݍ��݂Ɏ��s�����ꍇ
		if (isFaild) {
			std::string error = { filePath + "�ǂݍ��ݎ��s" };
			assert(false && error.c_str());
		}

		m_pImporter->Import(m_pScene);

		//���[�g�m�[�h���擾���Ă���
		m_pRootNode = m_pScene->GetRootNode();
		//���炩���ߎO�p�`�����Ă���
		FbxGeometryConverter converter(m_pFbxManager);
		converter.Triangulate(m_pScene, true);

#ifdef _MDEBUG
		//���[�g�t���[���̖��O�̕\��
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

			//!���������郁�b�V���Ɋ܂܂�钸�_�̐�
			const int vertexCount = mesh->GetControlPointsCount();
			FbxVector4 * vertex = mesh->GetControlPoints();
			auto node = mesh->GetNode();
			//!�ړ���
			auto trans = node->LclTranslation.Get();
			//!��]��
			auto rotate = node->LclRotation.Get();
			//!�g��k��
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
				//!���_�̈ꎞ�I�i�[��
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

			//!���������郁�b�V���Ɋ܂܂��C���f�b�N�X�̐�
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

			//!�@���Z�b�g
			FbxGeometryElementNormal* normal = mesh->GetElementNormal(0);

			auto re = normal->GetReferenceMode();
			assert(re == FbxGeometryElement::eDirect && "�@�������_���Ɋi�[����Ă��܂���");
			auto mapMode = normal->GetMappingMode();
			//!���������郁�b�V���Ɋ܂܂��C���f�b�N�X�̐�

			//!�@���̐�
			const int normalNum = normal->GetDirectArray().GetCount();

			for (int i = 0; i != normalNum; ++i) {
				util::Vec3 normalBuffer;
				normalBuffer.x = (float)normal->GetDirectArray().GetAt(i)[0];
				normalBuffer.y = (float)normal->GetDirectArray().GetAt(i)[1];
				normalBuffer.z = (float)normal->GetDirectArray().GetAt(i)[2];
				normalList.emplace_back(normalBuffer);
			}

			//���_���ƂɊi�[����Ă����ꍇ�C���f�b�N�X�œW�J����
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
			//!UV�̐�
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
		//���b�V�����@�{�[����
		std::vector<util::Mat4> boneInitMatList;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			
			//�X�L���͂P�������Ȃ��Ƒz��
			FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
			if (!skin)return;

			//! �{�[���̐�
			const auto culsterCount = skin->GetClusterCount();

			util::foreach(culsterCount, [&](int j) {
				auto cluster = skin->GetCluster(j);


				//!�e������_�̐�
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
		//���b�V������
		std::vector<VertecxWeithgt> vertexWeightByMesh;

		int boneCount = 0;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			//���_�̐����z����m��
			VertecxWeithgt vertexWight(mesh->GetControlPointsCount());

			//�X�L���͏�ɂP��z��
			FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
			if (!skin)return;

			//! �{�[���̐�
			const auto culsterCount = skin->GetClusterCount();

			util::foreach(culsterCount, [&](int j) {

				auto cluster = skin->GetCluster(j);

				//!�e������_�̐�
				int impactNum = cluster->GetControlPointIndicesCount();
				if (impactNum == 0)return;

				int* pointArray = cluster->GetControlPointIndices();
				auto* weightArray = cluster->GetControlPointWeights();

				util::foreach(impactNum, [&](int i) {
					vertexWight.weightList[pointArray[i]].index = pointArray[i];
					vertexWight.weightList[pointArray[i]].boneID = boneCount;
					vertexWight.weightList[pointArray[i]].weight = (float)weightArray[i];
				});

				//�e�������{�[�������ō\������
				boneCount += 1;
			});
			vertexWeightByMesh.emplace_back(vertexWight);
		});

		return vertexWeightByMesh;
	}


	std::vector<std::vector<std::pair<std::string,util::Mat4>>> FbxLoader::getMotionMatrix()
	{
		//�t���[�����@�{�[����
		std::vector<std::vector<std::pair<std::string, util::Mat4>>> boneInitMatListByFrame;

		//�X�^�[�g���ԂƏI�����Ԏ擾
		auto motionInfo = getMotionInfo();
		//�P�ʎ��Ԏ擾
		auto period = getPeriod();
		int frameNum = motionInfo.endTime.Get() / period.Get();

		boneInitMatListByFrame.resize(frameNum);

		int boneCount = 0;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {

			auto count = mesh->GetDeformerCount();

			//�X�L���͂P�������Ȃ��Ƒz��
			FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
			if (!skin)return;

			//! �{�[���̐�
			const auto culsterCount = skin->GetClusterCount();

			util::foreach(culsterCount, [&](int j) {
				auto cluster = skin->GetCluster(j);

				//!�e������_�̐�
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

		//FBX�Ɋi�[����Ă��郂�[�V�������ꗗ�擾
		m_pScene->FillAnimStackNameArray(motionNames);

		assert(motionNames.Size() != 0 && "�A�j���[�V�������i�[����Ă��܂���");

		auto takeInfo = m_pScene->GetTakeInfo(motionNames[0]->Buffer());

		MotionData result;

		result.startTime = takeInfo->mLocalTimeSpan.GetStart();

		result.endTime = takeInfo->mLocalTimeSpan.GetStop();

		return result;
	}

	std::vector<std::vector<std::pair<int, FbxCluster*>>> FbxLoader::getAllCluster()
	{
		//!���b�V�����ƂɊi�[
		std::vector<std::vector<std::pair<int, FbxCluster*>>> clusterListByMesh;

		eachMesh(m_pRootNode, [&](FbxMesh* mesh) {
			std::vector<std::pair<int, FbxCluster*>> clusterList;
			const auto skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);

			util::foreach(skinCount, [&](int i) {
				FbxSkin* skin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));
				if (!skin)return;

				//! �{�[���̐�
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

		//���b�V������ꂽ�ꍇ�̂ݎ��o������������
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

	//	//���b�V������ꂽ�ꍇ�̂ݎ��o������������
	//	if (mesh != nullptr) {
	//		action(mesh);
	//	}

	//	for (int i = 0, end = node->GetChildCount(); i < end; ++i) {
	//		eachSkeleton(node->GetChild(i), action);
	//	}
	//}






}