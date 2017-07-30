#pragma once
#include<fbxsdk.h>
#include <fbxsdk/scene/geometry/fbxmesh.h>
#include<string>
#include<vector>
#include<functional>
#include<Source\Util\Type.h>
#include<assert.h>
#include<unordered_map>
#include<memory>

/**
* @file	    FbxLoader.h
* @brief	FBX�ǂݍ��݃N���X
* @authro	���{�D�P
* @date	2017/3/7
*/

namespace framework {
	struct BoneData;
}

namespace util {

	struct ElementMode
	{
		ElementMode(FbxGeometryElement::EMappingMode _mappingMode, 
			FbxGeometryElement::EReferenceMode _referenceMode)
		{
			mappingMode = _mappingMode;
			referenceMode = _referenceMode;
		}
		FbxGeometryElement::EMappingMode mappingMode;
		FbxGeometryElement::EReferenceMode referenceMode;
	};

	struct ElementArray 
	{
		FbxLayerElementArrayTemplate<int>& indexArray;
		FbxLayerElementArrayTemplate<FbxVector2>& directArray;
	};

	struct MotionData {
		//!�A�j���[�V�����̊J�n����
		FbxTime startTime;
		//!�A�j���[�V�����̏I������
		FbxTime endTime;
	};

	struct BoneAndWeight
	{
		//���_�̓{�[��id�ƃE�F�C�g��1:1�Ŏ���
		int	  boneID;
		float weight;
		int index;
	};

	struct VertecxWeithgt
	{
		VertecxWeithgt() {
		}

		VertecxWeithgt(int vertexSize) {
			weightList.resize(vertexSize);
		}

		//���_���ƂɊi�[���� �C���f�b�N�X���Ɋi�[
		std::vector<BoneAndWeight> weightList;
	};

	//struct IndexAndCluster {
	//	IndexAndCluster() {
	//	}

	//	IndexAndCluster(std::pair<int, FbxCluster*>&  other)
	//	{
	//		indexCluster.first = other.first;
	//		indexCluster.second = other.second;
	//	}

	//	IndexAndCluster(IndexAndCluster& other)
	//	{
	//		indexCluster.first = other.indexCluster.first;
	//		indexCluster.second = other.indexCluster.second;
	//	}

	//	bool operator<(const IndexAndCluster& right)const {
	//		return indexCluster.second->GetUniqueID() < right.indexCluster.second->GetUniqueID();
	//	}

	//	std::pair<int, FbxCluster*> indexCluster;
	//};

	class FbxLoader
	{
	public:
		FbxLoader();
		~FbxLoader();

		/**
		* @brief		  �ǂݍ���
		* @param filePath �ǂݍ��ރt�@�C���̃p�X
		*/
		void loadFBX(const std::string& filePath );

		/**
		* @brief		  ���_�f�[�^���o��
		*/
		std::vector<std::vector<util::Vec3>> getVertex();

		/**
		* @brief		  �C���f�N�X���o��
		*/
		std::vector<std::vector<int>> getIndex();

		/**
		* @brief		  �@�����o��
		* @dital		  �@�����C���[�̐��͂P�����z�肵�Ȃ�
		* @dital		  �@���͒��_�̏��ԂŊi�[����Ă��邱�Ƃ�z�肵�Ă���
		*/
		std::vector<std::vector<util::Vec3>> getNormal();

		/**
		* @brief		  �e�N�X�`�����W���o��
		*/
		std::vector<std::vector<util::Vec2>> getUV();

		/**
		* @brief		  �o�C�m�[�}���擾
		*/
		std::vector<std::vector<util::Vec3>> getBinormal();

		/**
		* @brief		  �^���W�F���g�擾
		*/
		std::vector<std::vector<util::Vec3>> getTangent();

		/**
		* @brief		  �R���g���[���|�C���g�擾
		* @param		  �C���f�b�N�X���X�g
		*/
		std::vector<std::vector<util::Vec3>> getControlPoint(const std::vector<int>& indexList);

		/**
		* @brief		  �{�[�������p���s��擾
		*/
		std::vector<util::Mat4> getBoneInitMatrix();

		/**
		* @brief		  �e�������{�[����ID��weight���擾
		* @detile		  ���b�V���̐����̒��_���f�[�^�ŕԂ�
		*/
		std::vector<VertecxWeithgt> getBoneWeight();

		/**
		* @brief		  �A�j���[�V�����s��擾
		*/
		std::vector<std::vector<std::pair<std::string, util::Mat4>>> getMotionMatrix();

		/**
		* @brief		  ���b�V���̐��擾
		*/
		int getMeshCount();

	private:

		/**
		* @brief		  ���b�V�������邩�m�F���Ď��o��
		* @param node	   ���b�V�������o�������m�[�h
		* @return		  ���b�V�������o�����FbxMesh* ���o���Ȃ���΁@nullpter
		*/
		FbxMesh* getMesh(FbxNode* node);

		/**
		* @brief		 �X�P���g�������邩�m�F���Ď��o��
		* @param node	  �X�P���g���������o�������m�[�h
		* @return		  �X�P���g�������o�����FbxMesh* ���o���Ȃ���΁@nullpter
		*/
		FbxSkeleton* getSkeleton(FbxNode* node);

		/**
		* @brief		  ���b�V�����ċN�I�Ɏ��o��
		* @param node	  ���[�g�m�[�h
		* @param action	  ���o�����߂̊֐��|�C���^(���� : FbxMesh* mesh)
		* @param result	  ���o�����f�[�^���i�[����ϐ�
		*/
		void eachMesh(FbxNode* node, std::function<void(FbxMesh* mesh)> action);

		/**
		* @brief		  �X�P���g�����ċN�I�Ɏ��o��
		* @param node	  ���[�g�m�[�h
		* @param action	  ���o�����߂̊֐��|�C���^(���� : FbxMesh* mesh)
		* @param result	  ���o�����f�[�^���i�[����ϐ�
		*/
		//void eachSkeleton(FbxCluster * rootCluster, std::function<void(FbxSkeleton* FbxCluster)> action);

		/**
		* @brief		  �v�f���ǂ̂悤�ɂ��Ă��邩�擾
		* @param mesh	  ���b�V��
		* @return		  �擾�������[�h
		*/
		template<typename T>
		ElementMode getModes(T* element) {
			auto&& mapping = element->GetMappingMode();
			auto&& reference = element->GetReferenceMode();
			return ElementMode(std::move(mapping), std::move(reference));
		}

		/**
		* @brief		  ���[�V�����̊J�n���ԂƏI�����Ԃ̃f�[�^�擾
		*/
		MotionData getMotionInfo();

		/**
		* @brief		  ���ׂẴ{�[���擾
		* @detail		  
		*/
		std::vector<std::vector<std::pair<int, FbxCluster*>>> getAllCluster();

		/**
		* @brief		  �A�j���[�V�����̒P�ʎ��Ԏ擾
		*/
		FbxTime getPeriod();



	private:
		FbxManager* m_pFbxManager;
		FbxImporter* m_pImporter;
		FbxScene * m_pScene;
		FbxNode* m_pRootNode;
	};



}