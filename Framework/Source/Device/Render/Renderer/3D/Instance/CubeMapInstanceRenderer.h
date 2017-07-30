#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<list>
#include<vector>
#include<Source\Device\Render\Renderer\3D\Instance\IInstanceRenderer.h>

/**
* @file	    CubeMapRenderer.h
* @brief	剛体インスタンスをキューブマップに描画するクラス
* @authro	高須優輝
* @date	2017/03/13
*/

#define VIEW_NUM 6

namespace util {
	struct Transform;
}

namespace framework {
	class IInstanceRenderer;
	class IMaterial;
	class Camera;

	class CubeMapInstanceRenderer : public IInstanceRenderer
	{
	public:

		//更新頻度の高いバッファ
		struct AlwaysBuffer
		{
			XMMATRIX view[VIEW_NUM];
			//!最大描画数はシェーダーに記述されている100
			XMMATRIX world[MAX_DRAW_NUM];
			//!法線回転用の行列
			XMMATRIX rotate[MAX_DRAW_NUM];
		};

		//あまり更新しないバッファ
		struct SometimeBuffer
		{
			XMMATRIX proj;
		};

		CubeMapInstanceRenderer(std::string modelName, std::vector <std::string> textureNames);
		~CubeMapInstanceRenderer();

		/**
		* @brief					初期化
		*/
		void init();

		/**
		* @brief					キューブマップの中心座標をセット
		* @param centerPosition		センターポジションの入ったトランスフォーム
		*/
		void setCenterTransform(util::Transform* centerPosition);

	private:

		// IInstanceRenderer を介して継承されました
		virtual void instanceAction(int instanceNum, util::Transform * trans) override;

		/**
		* @brief			パイプラインの設定
		*/
		void setPipeline(int drawNum);

		/**
		* @brief			キューブマップ用カメラのアップデート
		*/
		void cameraUpdate();

	private:
		std::string m_ModelName;
		std::vector <std::string> m_TextureNames;
		//!シェーダーにテクスチャを配列で渡すための変数
		std::unique_ptr<ID3D11ShaderResourceView*[]> m_pShaderResource;
		std::shared_ptr<Model> m_Model;
		std::vector<std::shared_ptr<Texture2D>> m_Textures;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
		//!更新頻度が高いバッファ
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer;
		//!更新頻度の低いバッファ
		CComPtr<ID3D11Buffer> m_pSometimeBuffer;
		std::shared_ptr<IMaterial> m_pMaterial;
		//!更新用変数
		AlwaysBuffer m_AlwaysBuffer;
		//!キューブマップ用のカメラ
		std::unique_ptr<util::Mat4[]> m_ViewMat;
		//キューブマップの中心ポジション
		util::Transform* m_pCenterPosition;
		util::Mat4 m_Projection;

	};



}