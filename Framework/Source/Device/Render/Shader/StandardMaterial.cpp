#include"StandardMaterial.h"

namespace framework {


	StandardMaterial::StandardMaterial(ShaderData<ShaderType::Vertex> vs, CComPtr<ID3D11PixelShader> ps)
		:IMaterial(vs),
		m_pPixelShader(ps)
	{
	}

	StandardMaterial::~StandardMaterial()
	{
	}

	void StandardMaterial::active()
	{
		util::getContext()->VSSetShader(m_pVertexShader.shaderAccess.p,NULL,0);
		util::getContext()->PSSetShader(m_pPixelShader.p,NULL,0);
	}

	void StandardMaterial::deActive()
	{
		util::getContext()->VSSetShader(NULL, NULL, 0);
		util::getContext()->PSSetShader(NULL, NULL, 0);
	}

	void StandardMaterial::setConstantBuffer(ID3D11Buffer *const* buffer, UINT bufferNum)
	{
		util::getContext()->PSSetConstantBuffers(0, bufferNum, buffer);
		util::getContext()->VSSetConstantBuffers(0,bufferNum,buffer);
	}

}