#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Type.h>
#include<string>
#include"IRenderer.h"
/**
* @file	    RigidRenderer.h
* @brief	剛体描画クラス
* @authro	高須優輝
* @date	2017/03/7
*/

namespace util {
	struct Transform;
}

namespace framework {

	class IMaterial;
	class Texture2D;


	class RigidRenderer : public IRenderer
	{
	public:
		//更新頻度の高いバッファ
		struct AlwaysBuffer {
			XMMATRIX view;
			XMMATRIX world;
		};

		//更新頻度の低いバッファ
		struct SometimeBuffer
		{
			XMMATRIX proj;
		};

		/**
		* @brief			コンストラクタ
		* @param material		描画に使うマテリアルオブジェクト
		*/
		RigidRenderer(const std::string& modelName);
		~RigidRenderer();

		void init()override;

		/**
		* @brief			描画コールバック
		* @param trans		描画に使うトランスフォーム
		*/
		void draw(util::Transform * trans)override;

	private:
		/**
		* @brief			定数バッファの更新
		*/
		void constBufferUpdate();

	private:
		Model m_Model;
		std::shared_ptr<Texture2D> texture;
		CComPtr<ID3D11InputLayout> m_pInputLayout;
		//!更新頻度が高いバッファ
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer;
		//!更新頻度の低いバッファ
		CComPtr<ID3D11Buffer> m_pSometimeBuffer;
		std::shared_ptr<IMaterial> m_pMaterial;

	};



}