#pragma once

#include<D3DX11.h>
#include<atlbase.h>
namespace framework {

	enum class ShaderType
	{
		Vertex,
		Geometry,
		Pix,
		Hal,
		Domain,
	};

	template<ShaderType T>
	struct ShaderData;

	template<>
	struct ShaderData<ShaderType::Vertex>
	{
		//!パイプラインにセットすための変数
		CComPtr<ID3D11VertexShader>		shaderAccess;
		//!バイトデータ(入力レイアウトなどで利用)
		CComPtr<ID3DBlob>				byteData;
	};

	template<>
	struct ShaderData<ShaderType::Pix>
	{
		//!パイプラインにセットすための変数
		CComPtr<ID3D11PixelShader>		shaderAccess;
		//!バイトデータ(入力レイアウトなどで利用)
		CComPtr<ID3DBlob>				byteData;
	};

	template<>
	struct ShaderData<ShaderType::Geometry>
	{
		//!パイプラインにセットすための変数
		CComPtr<ID3D11GeometryShader>		shaderAccess;
		//!バイトデータ(入力レイアウトなどで利用)
		CComPtr<ID3DBlob>				byteData;
	};
}