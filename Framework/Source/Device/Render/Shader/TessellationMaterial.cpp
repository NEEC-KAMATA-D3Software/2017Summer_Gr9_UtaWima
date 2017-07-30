#include"TessellationMaterial.h"


namespace framework {

	TessellationMaterial::TessellationMaterial(ShaderData<ShaderType::Vertex> vs, CComPtr<ID3D11PixelShader> ps, CComPtr<ID3D11HullShader> hs, CComPtr<ID3D11DomainShader> ds)
		:StandardMaterial(vs,ps),
		m_pHullShader(hs),
		m_pDomainShader(ds)
	{
	}

	TessellationMaterial::~TessellationMaterial()
	{
	}

	void TessellationMaterial::active()
	{
		StandardMaterial::active();
		util::getContext()->HSSetShader(m_pHullShader.p, NULL, 0);
		util::getContext()->DSSetShader(m_pDomainShader.p, NULL, 0);
	}

	void TessellationMaterial::deActive()
	{
		StandardMaterial::deActive();
		util::getContext()->HSSetShader(NULL, NULL, 0);
		util::getContext()->DSSetShader(NULL, NULL, 0);
	}

	void TessellationMaterial::setConstantBuffer(ID3D11Buffer * const * buffer, UINT bufferNum)
	{
		StandardMaterial::setConstantBuffer(buffer,bufferNum);

		util::getContext()->HSSetConstantBuffers(0, bufferNum, buffer);
		util::getContext()->DSSetConstantBuffers(0, bufferNum, buffer);
	}

}