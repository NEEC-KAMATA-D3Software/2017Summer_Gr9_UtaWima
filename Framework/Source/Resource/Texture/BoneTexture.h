#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<functional>

/**
* @file	    BoneTexture.h
* @brief	ボーンテクスチャクラス
* @authro	高須優輝
* @date	2017/05/12
*/


namespace framework {

	class BoneTexture
	{
	public:
		/**
		* @brief				コンストラクタ
		* @param boneNum		ボーンの数
		* @param frameNum		アニメーションのフレーム数
		*/
		BoneTexture(int boneNum, int frameNum);
		~BoneTexture();

		void copyTexture(BoneTexture* other,int insertSlot,int sourceSloat);

		/**
		* @brief				書き込み開始
		*/
		void begin();

		/**
		* @brief				書き込み終了
		*/
		void end();


		/**
		* @brief				マップ中のものをコピーする
		*/
		void begonToCopy(D3D11_MAPPED_SUBRESOURCE* result);

		/**
		* @brief				マップ中のものにコピーする
		*/
		void begonToSet(D3D11_MAPPED_SUBRESOURCE* data);

		/**
		* @brief				begin()が呼ばれている状態でのみ書き込みできる
		* @param instanceID		書き込むインスタンスID
		* @param action			書き込みアクション
		*/
		void beginToBuffer(int boneSloat, int frameSloat, std::function<void(float* r, float* g, float* b, float* a)> action);

		/**
		* @brief			シェーダービュー取得
		*/
		ID3D11ShaderResourceView* getShaderView();

		/**
		* @brief			サイズ取得
		*/
		util::Vec2 getSize();

		/**
		* @brief			ピクセルサイズ取得
		*/
		int getPixBufferSize();

	private:
		void createTexture2D(int boneNum, int frameNum);

		void createShaderResourceView();

		void writeSubResource(D3D11_MAPPED_SUBRESOURCE* pMapData, int x, int y, std::function<void(float* r, float* g, float* b, float* a)> action);

		void eachPix(std::function<void(int, int)> action);

		ID3D11Texture2D* getTexture2D();

	private:
		//!元データ
		CComPtr<ID3D11Texture2D> m_pTexture2D;

		//!シェーダービュー
		CComPtr<ID3D11ShaderResourceView> m_pShaderView;

		bool m_IsWriting = false;
		D3D11_MAPPED_SUBRESOURCE m_MappedResource;
		
		int m_Width, m_Height;
	};


}