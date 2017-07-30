#pragma once
#include"IInstanceRenderer.h"
#include<vector>
#include<memory>
#include<D3DX11.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<Source\Application\Screen\Screen.h>

/**
* @file	    CubeInstanceRenderer.h
* @brief	キューブをインスタンス描画するクラス
* @authro	高須優輝
* @date	2017/04/04
*/

namespace util {
	struct Transform;
}

namespace framework {

	class IMaterial;

	class CubeInstanceRenderer : public IInstanceRenderer
	{
	public:

		//更新頻度の高いバッファ
		struct AlwaysBuffer
		{
			XMMATRIX view;
			//!最大描画数はシェーダーに記述されている100
			XMMATRIX world[MAX_DRAW_NUM];
			//!法線回転用の行列
			XMMATRIX rotate[MAX_DRAW_NUM];
		};

		//更新頻度の高いバッファ
		struct AlwaysBuffer2
		{
			util::Vec4 color[MAX_DRAW_NUM];
		};

		//あまり更新しないバッファ
		struct SometimeBuffer
		{
			XMMATRIX proj;
		};

		struct BoxLayout {
			util::Vec3 position;
			util::Vec3 normal;
		};

		CubeInstanceRenderer();
		~CubeInstanceRenderer();

		/**
		* @brief					初期化
		*/
		void init();

		/**
		* @brief					描画をワイヤーフレームにする
		*/
		void onWire();

		/**
		* @brief					描画をポリゴンにする
		*/
		void onPolygon();

		void setColor(std::vector<util::Vec3> colors);

	private:
		// IInstanceRenderer を介して継承されました
		virtual void instanceAction(int instanceNum, util::Transform * trans) override;

		// IInstanceRenderer を介して継承されました
		virtual void setPipeline(int drawNum) override;

	private:
		RigidModel m_Model;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
		//!更新頻度が高いバッファ
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer;
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer2;
		//!更新頻度の低いバッファ
		CComPtr<ID3D11Buffer> m_pSometimeBuffer;
		std::shared_ptr<IMaterial> m_pMaterial;
		//!更新用変数
		AlwaysBuffer m_AlwaysBuffer;
		AlwaysBuffer2 m_AlwaysBuffer2;

		std::vector<util::Vec3> m_Colors;

		bool isWire = true;

		util::Mat4 m_Projection;

	};

}