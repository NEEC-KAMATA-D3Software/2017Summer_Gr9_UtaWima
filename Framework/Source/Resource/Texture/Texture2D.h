#pragma once

/**
* @file	Texture2D.h
* @brief	静的テクスチャクラス
* @authro	高須優輝
* @date	2017/02/23
*/

#include<string>
#include<D3DX11.h>
#include<atlbase.h>
#include<Source\Util\Type.h>
#include<Source\Util\Math\Transform.h>

namespace framework {

	class SpriteRenderer;

	class Texture2D
	{
	public:
		Texture2D(const std::string& filePath);
		Texture2D(int width,int height, CComPtr<ID3D11ShaderResourceView> resourceView);
		~Texture2D();

		ID3D11ShaderResourceView*const* getReource();

		const util::Vec2& getSize();

		void draw(const util::Vec2& pos);

		void draw(util::Transform* trans);

	private:
		void createRenderer();

	private:
		//!シェーダアクセス用
		CComPtr<ID3D11ShaderResourceView> m_ShaderView;
		util::Vec2 m_Size;
	};

}