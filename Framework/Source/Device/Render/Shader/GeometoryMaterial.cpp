#include"GeometoryMaterial.h"

namespace framework {

	GeometoryMaterial::GeometoryMaterial(ShaderData<ShaderType::Vertex> vs, CComPtr<ID3D11GeometryShader> gs, CComPtr<ID3D11PixelShader> ps)
		:StandardMaterial(vs,ps),
		m_pGeometryShader(gs)
	{

	}

	GeometoryMaterial::~GeometoryMaterial()
	{
	}
	void GeometoryMaterial::active()
	{
		//vs,psに加えgsもセットする
		StandardMaterial::active();
		util::getContext()->GSSetShader(m_pGeometryShader.p, NULL, 0);
	}
	void GeometoryMaterial::deActive()
	{
		//vs,psに加えgsもセットする
		StandardMaterial::deActive();
		util::getContext()->GSSetShader(NULL, NULL, 0);
	}
	void GeometoryMaterial::setConstantBuffer(ID3D11Buffer * const * buffer, UINT bufferNum)
	{
		//vs,psに加えgsも定数バッファをセットする
		StandardMaterial::setConstantBuffer(buffer, bufferNum);
		util::getContext()->GSSetConstantBuffers(0, bufferNum, buffer);
	}
}