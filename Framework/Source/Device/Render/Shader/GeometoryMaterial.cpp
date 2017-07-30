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
		//vs,ps�ɉ���gs���Z�b�g����
		StandardMaterial::active();
		util::getContext()->GSSetShader(m_pGeometryShader.p, NULL, 0);
	}
	void GeometoryMaterial::deActive()
	{
		//vs,ps�ɉ���gs���Z�b�g����
		StandardMaterial::deActive();
		util::getContext()->GSSetShader(NULL, NULL, 0);
	}
	void GeometoryMaterial::setConstantBuffer(ID3D11Buffer * const * buffer, UINT bufferNum)
	{
		//vs,ps�ɉ���gs���萔�o�b�t�@���Z�b�g����
		StandardMaterial::setConstantBuffer(buffer, bufferNum);
		util::getContext()->GSSetConstantBuffers(0, bufferNum, buffer);
	}
}