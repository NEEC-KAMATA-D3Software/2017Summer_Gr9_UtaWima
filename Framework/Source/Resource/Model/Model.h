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

	//���̃��b�V���悤���_�f�[�^
	struct RigidVertexLayout
	{
		//!���_���W
		util::Vec3 position;
		//!�@��
		util::Vec3 normal;
		//!�o�C�m�[�}��
		util::Vec3 binormal;
		//!�o�C�m�[�}��
		util::Vec3 tangent;
		//!�e�N�X�`�����W
		util::Vec2 uv;
	};

	//���̃��b�V��
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
		* @brief	���R���X�g���N�^
		*/
		RigidModel() {
		}
		/**
		* @brief	�R���X�g���N�^
		* @detail	�ǂݍ���
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
				//�C���f�b�N�X���̂��̂𒸓_���ɕϊ�
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

	//�X�L�����b�V���悤���_�f�[�^(��)
	struct SkinningVertexLayout
	{
		//!���_���W
		util::Vec3 position;
		//!�@��
		util::Vec3 normal;
		//!�o�C�m�[�}��
		util::Vec3 binormal;
		//!�o�C�m�[�}��
		util::Vec3 tangent;
		//!�e�N�X�`�����W
		util::Vec2 uv;
		//!�{�[����id
		int boneID[4] = { 0 };
		//!�{�[���̉e���x
		float weights[4] = { 0 };
		//!���b�V���ԍ�
		int meshID;
	};


	struct SkinningModel : public Model
	{
		struct BoneAndWeight {
			int boneID[4];
			float wight[4];
		};

		/**
		* @brief	���R���X�g���N�^
		*/
		SkinningModel() {
		}
		/**
		* @brief	�R���X�g���N�^
		* @detail	�ǂݍ���
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

				//�C���f�b�N�X���̂��̂𒸓_���ɕϊ�
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