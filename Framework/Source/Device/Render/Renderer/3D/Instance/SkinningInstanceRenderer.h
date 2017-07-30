#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<list>
#include<vector>
#include<Source\Device\Render\Renderer\3D\Instance\IInstanceRenderer.h>
#include<Source\Resource\Texture\CubeTexture.h>
#include<memory>
#include<Source\Component\Animation\AnimatorComponent.h>

/**
* @file	    SkinningInstanceRenderer.h
* @brief	剛体インスタンス描画クラス
* @detail   テクスチャのレジスタ0~9までとし、10番目にはキューブテクスチャが入る
* @authro	高須優輝
* @date	2017/03/13
*/

namespace util {
	struct Transform;
}

namespace framework {

	struct MaterialParam;
	class IMaterial;
	class Texture2D;
	class CubeRenderTrget;
	class CubeDepthTarget;
	class Motion;
	class BoneTexture;

	
	class SkinningInstanceRenderer : public IInstanceRenderer
	{
	public:
		/**
		* @brief		コストラクタ
		* @param model     描画するモデル
		* @param textureNames  テクスチャの名前の入ったvector
		*/
		SkinningInstanceRenderer(std::shared_ptr<Model> model, std::vector <std::string> textureNames, std::vector <std::string> motionNames);
		~SkinningInstanceRenderer();

		//更新頻度の高いバッファ
		struct AlwaysBuffer
		{
			XMMATRIX view;
			//!最大描画数はシェーダーに記述されている100
			XMMATRIX world[MAX_DRAW_NUM];
			//!法線回転用の行列
			XMMATRIX rotate[MAX_DRAW_NUM];
			//!シャドウマッピング用行列
			XMMATRIX shadowViewMat;
			util::Vec3 eyePos;
			int damy;
		};

		//更新頻度の高いバッファ
		struct SkinningBuffer
		{
			//!ボーン行列 メッシュの数　ボーンの数
			//XMMATRIX boneMatrix[BONE_NUM];
			util::Vec4 drawData[MAX_DRAW_NUM];//16の倍数にするために２足す
		};

		struct MaterialBuffer {
			float height;
			float specular;
			//!ライティングするかどうか
			float isNotLighting;
			float damy;
		};

		//あまり更新しないバッファ
		struct SometimeBuffer
		{
			XMMATRIX proj;
		};

		//定数バッファ系データ
		struct ConstantData
		{
			//!AlwaysのGPUに渡すためのバッファ
			CComPtr<ID3D11Buffer> pAlwaysGPU;
			//!MaterialのGPUに渡すためのバッファ
			CComPtr<ID3D11Buffer> pMaterialGPU;
			//!SometimeのGPUに渡すためのバッファ
			CComPtr<ID3D11Buffer> pSometimeGPU;
			//!SkinningのGPUに渡すためのバッファ
			CComPtr<ID3D11Buffer> pSkinningGPU;

			//!定数元データ
			AlwaysBuffer alwaysBuffer;
			//!定数元データ
			MaterialBuffer materialBuffer;
			//!定数元データ
			SkinningBuffer skinningBuffer;
		};

		//リソース読み込み　割り当て用の名前
		struct LoadNames {
			std::vector <std::string> textureNames;
			std::vector <std::string> motionNames;
		};

		struct ResourceData
		{
			//!アニメーションデータ
			std::unique_ptr<BoneTexture> pBoneTexure;
			//!モデルデータ
			std::shared_ptr<Model> pModel;
			//!テクスチャデータ
			std::vector<std::shared_ptr<Texture2D>> pTextures;
		};

		/**
		* @brief					初期化
		*/
		void init();

		void setCubMap(const std::vector<std::shared_ptr<framework::CubeRenderTrget>>& cubMap);

		/**
		* @brief				１つのキューブマップをすべてに反映させる
		* @param instanceNum　　描画するインスタンスの数
		* @param cubMap			反映させたいキューブテクスチャ
		*/
		void setCubMap(int instanceNum, const std::shared_ptr<framework::CubeTexture>& cubMap);

		/**
		* @brief				マテリアルのパラメータセット
		* @param param			マテリアルパラメータ
		*/
		void setMaterialParam(MaterialParam& param);

		/**
		* @brief				インスタンスごとのアニメーターセット
		* @param animators		描画するオブジェクトのインスタンスごとアニメーター
		*/
		void setAnimator(std::vector<std::weak_ptr<component::AnimatorComponent>> animators);


	private:
		/**
		* @brief				インスタンスごとに情報を取り出すためのコールバック
		* @param instanceNum	何番目のインスタンスか
		* @param trans			そのループ描画するトランスフォーム
		*/
		virtual void instanceAction(int instanceNum, util::Transform*  trans);
		/**
		* @brief			パイプラインの設定
		*/
		void setPipeline(int drawNum)override;

		/**
		* @brief			テクスチャセット
		*/
		void setTexture();

		/**
		* @brief			テクスチャにボーン行列を書き込み
		* @deteil			リソースの読み込まれる順番の関係でコンストラクタではできない
		*/
		void writeBoneTexture(std::weak_ptr<component::AnimatorComponent>);

		/**
		* @brief			アニメーションのフレームをセット
		*/
		void frameSet(const int instanceID);

		void clear()override;

	private:
		//!読み込み　割り当てに使う名前
		LoadNames m_LoadName;
		//!定数バッファ系データ
		ConstantData m_ConstantData;
		//!リソースデータ
		ResourceData m_ResourceData;
		//!シェーダーにテクスチャを配列で渡すための変数
		std::unique_ptr<ID3D11ShaderResourceView*[]> m_pShaderResource;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
	
		std::shared_ptr<IMaterial> m_pMaterial;
		std::pair<int, std::unique_ptr<ID3D11ShaderResourceView*[]>> m_pCubMaps;
		util::Mat4 m_Projection;

		//!インスタンスごとアニメーター
		std::vector<std::weak_ptr<component::AnimatorComponent>> m_Animators;

		int m_TextureNum = 0;
	};




}
