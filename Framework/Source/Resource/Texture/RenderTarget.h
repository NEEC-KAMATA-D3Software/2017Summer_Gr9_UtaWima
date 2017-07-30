
#pragma once

/**
* @file	RenderTarget.h
* @brief	描画先クラス マルチサンプルには対応しない
* @authro	高須優輝
* @date	2017/02/23
*/

#include<Source\Device\Render\DirectXInstance.h>
#include"ITarget.h"
#include<Source\Util\Type.h>

namespace framework {

	class SpriteRenderer;

	class RenderTarget : public ITarget
	{
	public:
		/**
		* @brief		空のテクスチャを生成してそれをもとにする
		* @param width	ターゲットの横幅
		* @param height	ターゲットの縦幅
		* @param format	テクスチャフォーマット
		*/
		RenderTarget(int width, int height, DXGI_FORMAT format);

		/**
		* @brief			既存のリソースをもとにビューを作成する　
		* @param resource	元リソースのポインタ
		* @detail			ただしそのリソースは内部でポインタ管理される
		*/
		RenderTarget(CComPtr<ID3D11Texture2D> resource, DXGI_FORMAT format);
		~RenderTarget();


		/**
		* @brief	  ビューの設定用変数を返す
		* @return	　ID3D11RenderTargetView*
		*/
		virtual ID3D11RenderTargetView * const* getView();

		/**
		* @brief				レンダーターゲットをクリアする
		* @param clearColor		クリア色
		*/
		void clear(const util::Vec4& clearColor);


		util::Vec2 getSize();


		void draw(const util::Vec2& drawPos);

		void draw(util::Transform* trans);

		void copy(const RenderTarget& other);

	protected:
		/**
		* @brief				継承先にResolve処理をしてもらうためのコンストラクタ
		* @param width			ターゲットの横幅
		* @param height			ターゲットの縦幅
		* @param format			テクスチャフォーマット
		* @param sampleCount	マルチサンプルの数
		*/
		RenderTarget(int width, int height, DXGI_FORMAT format, int sampleCount);

		/**
		* @brief				レンダラ作成
		*/
		virtual void createSpriteRenderer(int width, int height);

	private:
		void createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount = 1) override;
		void createView(DXGI_FORMAT format, int sampleCount = 1);
		void createShaderResourceView(DXGI_FORMAT format)override;

	protected:
		//描画先に設定するための変数
		CComPtr<ID3D11RenderTargetView> m_pView;
		util::Vec2 m_Size;
		std::unique_ptr<SpriteRenderer> m_pRenderer;

	};

}