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
* @brief	FBX読み込みクラス
* @authro	高須優輝
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
		//!アニメーションの開始時間
		FbxTime startTime;
		//!アニメーションの終了時間
		FbxTime endTime;
	};

	struct BoneAndWeight
	{
		//頂点はボーンidとウェイトを1:1で持つ
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

		//頂点ごとに格納する インデックス順に格納
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
		* @brief		  読み込み
		* @param filePath 読み込むファイルのパス
		*/
		void loadFBX(const std::string& filePath );

		/**
		* @brief		  頂点データ取り出し
		*/
		std::vector<std::vector<util::Vec3>> getVertex();

		/**
		* @brief		  インデクス取り出し
		*/
		std::vector<std::vector<int>> getIndex();

		/**
		* @brief		  法線取り出し
		* @dital		  法線レイヤーの数は１つしか想定しない
		* @dital		  法線は頂点の順番で格納されていることを想定している
		*/
		std::vector<std::vector<util::Vec3>> getNormal();

		/**
		* @brief		  テクスチャ座標取り出し
		*/
		std::vector<std::vector<util::Vec2>> getUV();

		/**
		* @brief		  バイノーマル取得
		*/
		std::vector<std::vector<util::Vec3>> getBinormal();

		/**
		* @brief		  タンジェント取得
		*/
		std::vector<std::vector<util::Vec3>> getTangent();

		/**
		* @brief		  コントロールポイント取得
		* @param		  インデックスリスト
		*/
		std::vector<std::vector<util::Vec3>> getControlPoint(const std::vector<int>& indexList);

		/**
		* @brief		  ボーン初期姿勢行列取得
		*/
		std::vector<util::Mat4> getBoneInitMatrix();

		/**
		* @brief		  影響されるボーンのIDとweightを取得
		* @detile		  メッシュの数分の頂点分データで返る
		*/
		std::vector<VertecxWeithgt> getBoneWeight();

		/**
		* @brief		  アニメーション行列取得
		*/
		std::vector<std::vector<std::pair<std::string, util::Mat4>>> getMotionMatrix();

		/**
		* @brief		  メッシュの数取得
		*/
		int getMeshCount();

	private:

		/**
		* @brief		  メッシュがあるか確認して取り出す
		* @param node	   メッシュを取り出したいノード
		* @return		  メッシュが取り出せればFbxMesh* 取り出せなければ　nullpter
		*/
		FbxMesh* getMesh(FbxNode* node);

		/**
		* @brief		 スケルトンがあるか確認して取り出す
		* @param node	  スケルトンュを取り出したいノード
		* @return		  スケルトンが取り出せればFbxMesh* 取り出せなければ　nullpter
		*/
		FbxSkeleton* getSkeleton(FbxNode* node);

		/**
		* @brief		  メッシュを再起的に取り出す
		* @param node	  ルートノード
		* @param action	  取り出すための関数ポインタ(引数 : FbxMesh* mesh)
		* @param result	  取り出したデータを格納する変数
		*/
		void eachMesh(FbxNode* node, std::function<void(FbxMesh* mesh)> action);

		/**
		* @brief		  スケルトンを再起的に取り出す
		* @param node	  ルートノード
		* @param action	  取り出すための関数ポインタ(引数 : FbxMesh* mesh)
		* @param result	  取り出したデータを格納する変数
		*/
		//void eachSkeleton(FbxCluster * rootCluster, std::function<void(FbxSkeleton* FbxCluster)> action);

		/**
		* @brief		  要素がどのようについているか取得
		* @param mesh	  メッシュ
		* @return		  取得したモード
		*/
		template<typename T>
		ElementMode getModes(T* element) {
			auto&& mapping = element->GetMappingMode();
			auto&& reference = element->GetReferenceMode();
			return ElementMode(std::move(mapping), std::move(reference));
		}

		/**
		* @brief		  モーションの開始時間と終了時間のデータ取得
		*/
		MotionData getMotionInfo();

		/**
		* @brief		  すべてのボーン取得
		* @detail		  
		*/
		std::vector<std::vector<std::pair<int, FbxCluster*>>> getAllCluster();

		/**
		* @brief		  アニメーションの単位時間取得
		*/
		FbxTime getPeriod();



	private:
		FbxManager* m_pFbxManager;
		FbxImporter* m_pImporter;
		FbxScene * m_pScene;
		FbxNode* m_pRootNode;
	};



}