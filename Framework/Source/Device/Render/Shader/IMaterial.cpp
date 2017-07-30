#include "IMaterial.h"
#include<Source\Resource\Texture\Texture2D.h>


void framework::IMaterial::setTexture(int startSlot, int texNum,ID3D11ShaderResourceView* const* texture)
{
	util::getContext()->VSSetShaderResources(startSlot, texNum, texture);
	util::getContext()->PSSetShaderResources(startSlot,texNum,texture);
}

void framework::IMaterial::setSamplerState(int baindNum, int samplerNum, ID3D11SamplerState *const* sampler)
{
	util::getContext()->PSSetSamplers(baindNum, samplerNum, sampler);
}

ID3DBlob * framework::IMaterial::getVertexByte()
{
	return m_pVertexShader.byteData.p;
}
