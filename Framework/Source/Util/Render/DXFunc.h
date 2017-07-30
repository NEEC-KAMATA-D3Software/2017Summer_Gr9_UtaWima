#pragma once

#include"../../Device/Render/DirectXInstance.h"
#include<string>
#include"../../Device/Render/Shader/ShaderData.h"
#include<d3d11shader.h>
#include<assert.h>
#include<iostream>
#include"../../Device/Render/Shader/ShaderData.h"
#include<Source\Util\Type.h>
#include<functional>

/**
* @file	DXFunc.h
* @brief	よく使う関数を簡易に使えるようにラップした関数まとめ
* @authro	高須優輝
* @date	2017/02/23
*/
namespace util {

	enum class SamplerType
	{
		Clamp,
		Wrap,
	};

	/**
	* @brief		デバイス取得
	*/
	ID3D11Device * getDevice();
	/**
	* @brief		コンテキスト取得
	*/
	ID3D11DeviceContext *getContext();
	/**
	* @brief		スワップチェイン取得
	*/
	IDXGISwapChain *getSwapChain();

	/**
	* @brief		バックバッファ取得
	* @return		バックバッファーへアクセスするための参照
	*/
	CComPtr<ID3D11Texture2D> getBackBuffer();

	void setSingleViewPort(int width, int height);

	/**
	* @brief				頂点バッファを作成する
	* @param bufferSize		バッファーのサイズ
	* @param data			頂点バッファ初期データ
	* @return				頂点バッファ
	*/
	CComPtr<ID3D11Buffer> createVertexBuffer(UINT bufferSize, void* data);

	/**
	* @brief				インデックスバッファを作成する
	* @param bufferSize		バッファーのサイズ
	* @param data			インデックスバッファ初期データ
	* @return				インデックスバッファ
	*/
	CComPtr<ID3D11Buffer> createIndexBuffer(UINT bufferSize, void* data);

	/**
	* @brief				定数バッファバッファを作成する
	* @param bufferSize		バッファーのサイズ
	* @param data			データの初期値(なくてもよい)
	* @return				定数バッファ
	* @param data			バッファーの初期値(なくても良い)
	*/
	CComPtr<ID3D11Buffer> createConstantBuffer(UINT bufferSize, void* data = nullptr);


	/**
	* @brief				シェーダーをコンパイルする
	* @param fileName		シェーダファイルまでのパス
	* @param entryName		シェーダーのエントリーポイントの名前
	* @param compile		コンパイルの種類
	* @return				シェーダのバイトコード
	*/
	CComPtr<ID3DBlob> compileShader(std::string fileName, std::string entryName, std::string compileVersion, const std::string & includePath = "");

	/**
	* @brief				入力レイアウトを作成する
	* @param layout			作成するレイアウト
	* @param layoutCount	レイアウトの数
	* @param vertexBlob		頂点シェーダのバイナリデータ
	* @return				IAインターフェース
	*/
	CComPtr<ID3D11InputLayout> createIA(const D3D11_INPUT_ELEMENT_DESC* layout, int layoutCount, ID3DBlob* vertexBlob);

	/**
	* @brief				入力レイアウトを作成する
	* @param layout			作成するレイアウト
	* @param layoutCount	レイアウトの数
	* @param data			バイトデータ
	* @param size			バイトデータのサイズ
	* @return				IAインターフェース
	*/
	CComPtr<ID3D11InputLayout> createIA(const D3D11_INPUT_ELEMENT_DESC* layout, int layoutCount, void* data, int size);

	/**
	* @brief					よく使う設定でラスタライザを作成する
	* @param activeMultiSample	マルチサンプルを有効にするかどうか true = 有効 false = 無効
	* @param isCullFace			フェースをカリングするかどうか     true = カリング有効 fakse = カリング無効
	* @return					ラスタライザインターフェース
	*/
	CComPtr<ID3D11RasterizerState> createRasterizerSate(bool activeMultiSample,bool isCullFace = true);

	/**
	* @brief	よく使う設定でデプスステンシルステートを作成する
	* @return	デプスステンシルステートオブジェクトインターフェース
	* @dital	ステンシルテストは無効になっている
	*/
	CComPtr<ID3D11DepthStencilState> createDepthStencilState();

	/**
	* @brief				よく使う設定でブレンドテートを作成する
	* @param isBlendEnable  アルファブレンド有効にするかどうか true = 有効　false = 無効
	* @return				ブレンドステートオブジェクト
	* @dital				アルファブレンドが有効になっている場合レンダーターゲット0番目のみアルファブレンド有効
	*/
	CComPtr<ID3D11BlendState> createBlendState(bool isBlendEnable = true, D3D11_BLEND_DESC* desc = nullptr);

	/**
	* @brief					定数バッファの更新を行う
	* @param constantBuffer		書き換えを行うバッファ
	* @param bufferStruct		上書きするデータ
	*/
	template<typename T>
	void updateConstantBuffer(ID3D11Buffer* constantBuffer, const T& bufferStruct) {
		D3D11_MAPPED_SUBRESOURCE mapResource;
		HRESULT hr = getContext()->Map(
			constantBuffer,
			0,
			D3D11_MAP_WRITE_DISCARD,//書き込みアクセス
			0,
			&mapResource);

		if (FAILED(hr)) {
			assert(false && "定数バッファ書き込み失敗");
		}

		CopyMemory(mapResource.pData, &bufferStruct, sizeof(T));

		getContext()->Unmap(constantBuffer, 0);
	}

	/**
	* @brief					サンプラーステートオブジェクト作成
	* @param samplerType		境界線の設定
	*/
	CComPtr<ID3D11SamplerState> createSamplerState(const SamplerType& samplerType);

	/**
	* @brief					ブレンドステートのセット
	* @param sate				ブレンドステートオブジェクト
	*/
	void setBlendState(ID3D11BlendState* state);

	/**
	* @brief					シャドウマップ用行列作成
	*/
	void shadowViewMat(util::Mat4& lightView,util::Mat4& lightProj,util::Mat4& world, util::Mat4* result);

	/**
	* @brief					テクスチャに書き込み
	* @param  pTexture2D		書き込まれるテクスチャ
	* @param  textureNum		書き込まれるテクスチャのArray数
	*/
	void writeTexutre(ID3D11Texture2D* pTexture2D, std::function<void(D3D11_MAPPED_SUBRESOURCE*)> action, D3D11_MAP mapMode);

	/**
	* @brief					テクスチャに書き込み
	* @param  pTexture2D		書き込まれるテクスチャ
	* @param  location			何番目に書き込むかどうか
	* @param  pResource			書き込みをするテクスチャ
	* @detile　CPUアクセスフラグが書き込み不可能状態のもだけ可能 D3D11_MAP_DISCARDを指定した場合行使されない場合がある
	*/
	void writeArrayTexutre(ID3D11Texture2D* pTexture2D,int location , ID3D11Texture2D * pResource, D3D11_MAP mapMode);

	/**
	* @brief					テクスチャに書き込み
	* @param  pTexture2D		書き込まれるテクスチャ
	* @param  location			何番目に書き込むかどうか
	* @param  mappedResource	コピーするマップデータ
	* @detile　CPUアクセスフラグが書き込み不可能状態のもだけ可能 D3D11_MAP_DISCARDを指定した場合行使されない場合がある
	*/
	void writeArrayTexutre(ID3D11Texture2D* pTexture2D, int location, D3D11_MAPPED_SUBRESOURCE* mappedResource);

	CComPtr<ID3D11Texture2D> loadTexture2D(const std::string& file);

	/**
	* @brief				読み込みと描画
	* @param  filePath		描画するテクスチャのパス
	* @param  pos			描画位置
	* @param  deteil		描画が終わるたびに読み込んだテクスチャは破棄される
	*/
	void LoadAndDraw(const std::string& filePath,const util::Vec2& pos,const util::Vec2& scale);

	void SetMultiThreadFlag(bool isMultiThread);
}