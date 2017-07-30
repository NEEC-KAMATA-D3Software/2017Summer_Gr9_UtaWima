#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<list>
#include<vector>
#include<Source\Device\Render\Renderer\3D\Instance\IInstanceRenderer.h>
#include<Source\Resource\Texture\CubeTexture.h>

/**
* @file	    RigidRenderer.h
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

	class RigidInstanceRenderer : public IInstanceRenderer
	{
	public:
		/**
		* @brief		コストラクタ
		* @param modelName     モデルのKey
		* @param textureNames  テクスチャの名前の入ったvector
		*/
		RigidInstanceRenderer(std::shared_ptr<Model> model, std::vector <std::string> textureNames);
		~RigidInstanceRenderer();

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
			float damy;
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

		/**
		* @brief					初期化
		*/
		void init() override;

		void setCubMap(const std::vector<std::shared_ptr<framework::CubeRenderTrget>>& cubMap)override;

		/**
		* @brief				１つのキューブマップをすべてに反映させる
		* @param instanceNum　　描画するインスタンスの数
		* @param cubMap			反映させたいキューブテクスチャ
		*/
		void setCubMap(int instanceNum, const std::shared_ptr<framework::CubeTexture>& cubMap)override;


		/**
		* @brief				マテリアルのパラメータセット
		* @param param			マテリアルパラメータ
		*/
		void setMaterialParam(MaterialParam& param)override;

		void clear();

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

		void setTexture();

	private:
		std::vector <std::string> m_TextureNames;
		//!シェーダーにテクスチャを配列で渡すための変数
		std::unique_ptr<ID3D11ShaderResourceView*[]> m_pShaderResource;
		std::shared_ptr<Model> m_Model;
		std::vector<std::shared_ptr<Texture2D>> m_Textures;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
		//!更新頻度が高いバッファ
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer;
		//!更新頻度の高いバッファ
		CComPtr<ID3D11Buffer> m_pMaterialBuffer;
		//!更新頻度の低いバッファ
		CComPtr<ID3D11Buffer> m_pSometimeBuffer;
		std::shared_ptr<IMaterial> m_pMaterial;
		//!更新用変数
		AlwaysBuffer m_AlwaysBuffer;
		MaterialBuffer m_MaterialBuffer;
		std::pair<int, std::unique_ptr<ID3D11ShaderResourceView*[]>> m_pCubMaps;
		util::Mat4 m_Projection;
		int m_TextureNum = 0;
	};




}
