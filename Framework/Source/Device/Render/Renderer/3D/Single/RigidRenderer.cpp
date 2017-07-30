#include"RigidRenderer.h"
#include<Source\Device\Render\Shader\StandardMaterial.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Math\Transform.h>

namespace framework {

	RigidRenderer::RigidRenderer(const std::string& modelName)
	{
		m_pAlwaysBuffer = util::createConstantBuffer(sizeof(AlwaysBuffer));
		m_pSometimeBuffer = util::createConstantBuffer(sizeof(SometimeBuffer));
		//m_Model = ResourceManager::getInstance()->getModel(modelName);
		//マテリアル取得はハードコーディングで固定のものを取得するようにしておく
		m_pMaterial = ResourceManager::getInstance()->getMaterial("TestMaterial");

		//入力レイアウト作成/////////////
		//TODO　レンダラインスタンスごとにIAオブジェクトが重複するので対策が必要
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "SV_Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",		0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3),D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "BINORMAL",		0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3) * 2,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TANGENT",		0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3) * 3,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXTURE"	   ,0,DXGI_FORMAT_R32G32_FLOAT   ,0,sizeof(util::Vec3) * 4,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		m_pInputLayout = util::createIA(layout,_countof(layout), m_pMaterial->getVertexByte()->GetBufferPointer(), m_pMaterial->getVertexByte()->GetBufferSize());

		auto sampler = util::createSamplerState(util::SamplerType::Wrap);
		m_pMaterial->setSamplerState(0,1,&sampler.p);
	}

	RigidRenderer::~RigidRenderer()
	{
	}

	void RigidRenderer::init()
	{

	}

	void RigidRenderer::draw(util::Transform * trans)
	{
		//パイプラインにシェーダーセット
		m_pMaterial->active();

		//!更新用変数
		AlwaysBuffer ab;
		ab.world = trans->toMatrix().toXMMatrix();

		//TODO 後でカメラクラスに移植////////
		util::Vec4 eyePos = { 0.0f,5.0f,-10.0f,1.0f };
		util::Vec4 lookPos = { 0.0f,0.0f,0.0f,1.0f };
		util::Vec4 upDirection = { 0.0f,1.0f,0.0f,1.0f };

		util::Mat4 viewMat = XMMatrixLookAtLH(
			XMLoadFloat4(&eyePos),
			XMLoadFloat4(&lookPos),
			XMLoadFloat4(&upDirection));
		////////////////////////////////////

		ab.view = XMMatrixTranspose(viewMat.toXMMatrix());

		util::updateConstantBuffer<AlwaysBuffer>(m_pAlwaysBuffer.p, ab);

		SometimeBuffer sb;
		util::Mat4 projection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(48.0f),
			1920.0f / 1080.0f,//floarし忘れ注意
			1.0f,
			1000.0f);

		sb.proj = XMMatrixTranspose(projection.toXMMatrix());

		util::updateConstantBuffer<SometimeBuffer>(m_pSometimeBuffer.p, sb);

		//頂点バッファセット
		UINT strides[1] = { sizeof(RigidVertexLayout) };
		UINT offsets[1] = { 0 };
		util::getContext()->IASetVertexBuffers(0, 1, &m_Model.mesh.vertexBuffer.p, strides, offsets);
		util::getContext()->IASetIndexBuffer(m_Model.mesh.indexBuffer.p, DXGI_FORMAT_R32_UINT,0);

	//	m_pMaterial->setTexture(0,1, m_Model.texture->getReource());

		util::getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		constBufferUpdate();

		util::getContext()->IASetInputLayout(m_pInputLayout.p);
		
		util::getContext()->DrawIndexed(m_Model.drawNum,0,0);

		m_pMaterial->deActive();
	}

	void RigidRenderer::constBufferUpdate()
	{

		//定数バッファにセットするための変数
		ID3D11Buffer* mediationBuffer[2];

		//順番厳守
		mediationBuffer[0] = m_pSometimeBuffer.p;
		mediationBuffer[1] = m_pAlwaysBuffer.p;

		m_pMaterial->setConstantBuffer(mediationBuffer, 2);
	}

}