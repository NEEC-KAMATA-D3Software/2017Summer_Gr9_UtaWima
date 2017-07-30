#pragma once

#include<D3DX11.h>
#include<atlbase.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<memory>
#include<Source\Util\Type.h>
#include<string>
#include<Source\Util\IO\FbxLoader.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Util\Template\Template.h>
#include<Source\Util\Thread\CriticalSection.h>


#define BONE_NUM 64

namespace framework {

	enum class ModelType
	{
		rigid,
		skin,
	};

	//剛体メッシュよう頂点データ
	struct RigidVertexLayout
	{
		//!頂点座標
		util::Vec3 position;
		//!法線
		util::Vec3 normal;
		//!バイノーマル
		util::Vec3 binormal;
		//!バイノーマル
		util::Vec3 tangent;
		//!テクスチャ座標
		util::Vec2 uv;
	};

	//剛体メッシュ
	struct Mesh
	{
		CComPtr<ID3D11Buffer> vertexBuffer;
		CComPtr<ID3D11Buffer> indexBuffer;
	};

	struct Model {
		Mesh mesh;
		int drawNum;
		ModelType type = ModelType::rigid;
	};

	struct RigidModel : public Model
	{
		/**
		* @brief	基底コンストラクタ
		*/
		RigidModel() {
		}
		/**
		* @brief	コンストラクタ
		* @detail	読み込み
		*/
		RigidModel(const std::string& filePath) {
			util::CriticalSection c;
			util::FbxLoader loader;
			loader.loadFBX(filePath);

			auto meshCount = loader.getMeshCount();
			auto vertex = loader.getVertex();
			auto uv = loader.getUV();
			auto normal = loader.getNormal();
			auto index = loader.getIndex();
			auto binroaml = loader.getBinormal();
			auto tangent = loader.getTangent();

			std::vector<int> totalIndex;
			util::foreach(meshCount, [&](int mesh) {
				util::foreach(index[mesh].size(), [&](int i) {
					totalIndex.emplace_back(index[mesh][i]);
				});
			});

			std::vector<RigidVertexLayout> data;
			util::foreach(meshCount, [&](int mesh) {
				//インデックス順のものを頂点順に変換
				for (int i = 0, end = index[mesh].size(); i != end; ++i) {
					RigidVertexLayout layout;

					layout.position = vertex[mesh][index[mesh][i]];
					layout.normal = normal[mesh][i];
					layout.binormal = binroaml[mesh][i];
					layout.tangent = tangent[mesh][i];
					layout.uv = uv[mesh][i];

					data.emplace_back(layout);
				}
			});

			this->mesh.vertexBuffer = util::createVertexBuffer(sizeof(RigidVertexLayout) * totalIndex.size(), &data[0]);
			this->mesh.indexBuffer = util::createIndexBuffer(sizeof(int) * totalIndex.size(), &totalIndex[0]);
			this->drawNum = totalIndex.size();
		}
	};

	//スキンメッシュよう頂点データ(仮)
	struct SkinningVertexLayout
	{
		//!頂点座標
		util::Vec3 position;
		//!法線
		util::Vec3 normal;
		//!バイノーマル
		util::Vec3 binormal;
		//!バイノーマル
		util::Vec3 tangent;
		//!テクスチャ座標
		util::Vec2 uv;
		//!ボーンのid
		int boneID[4] = { 0 };
		//!ボーンの影響度
		float weights[4] = { 0 };
		//!メッシュ番号
		int meshID;
	};


	struct SkinningModel : public Model
	{
		struct BoneAndWeight {
			int boneID[4];
			float wight[4];
		};

		/**
		* @brief	基底コンストラクタ
		*/
		SkinningModel() {
		}
		/**
		* @brief	コンストラクタ
		* @detail	読み込み
		*/
		SkinningModel(const std::string& filePath) {
			util::CriticalSection c;
			util::FbxLoader loader;
			loader.loadFBX(filePath);

			auto meshCount = loader.getMeshCount();
			auto vertex = loader.getVertex();
			auto uv = loader.getUV();
			auto normal = loader.getNormal();
			auto index = loader.getIndex();
			auto binroaml = loader.getBinormal();
			auto tangent = loader.getTangent();
			auto weightList = loader.getBoneWeight();

			std::vector<int> totalIndex;
			util::foreach(meshCount, [&](int mesh) {
				util::foreach(index[mesh].size(), [&](int i) {
					totalIndex.emplace_back(index[mesh][i]);
				});
			});

			std::vector<SkinningVertexLayout> data;
			util::foreach(meshCount, [&](int mesh) {

				//インデックス順のものを頂点順に変換
				for (int i = 0, end = index[mesh].size(); i != end; ++i) {
					SkinningVertexLayout layout;
					layout.position = vertex[mesh][index[mesh][i]];
					layout.normal = normal[mesh][i];
					layout.binormal = binroaml[mesh][i];
					layout.tangent = tangent[mesh][i];
					layout.uv = uv[mesh][i];
					layout.meshID = mesh;

					if (weightList.size() > mesh) {
						auto weights = util::vectorCopyif<util::BoneAndWeight>(weightList[mesh].weightList, [&](util::BoneAndWeight obj) {
							return obj.index == index[mesh][i];
						});

						util::foreach(weights.size(), [&](int i) {
							layout.boneID[i] = weights[i].boneID;
							layout.weights[i] = weights[i].weight;
						});
					}

					data.emplace_back(layout);
				}
			});


			this->mesh.vertexBuffer = util::createVertexBuffer(sizeof(SkinningVertexLayout) * totalIndex.size(), &data[0]);
			this->mesh.indexBuffer = util::createIndexBuffer(sizeof(int) * totalIndex.size(), &totalIndex[0]);
			this->drawNum = totalIndex.size();
		}
	};


}