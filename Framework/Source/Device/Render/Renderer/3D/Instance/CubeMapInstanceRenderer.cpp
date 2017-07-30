#include"CubeMapInstanceRenderer.h"
#include<Source\Device\Render\Shader\StandardMaterial.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Math\Transform.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Device\Camera\Camera.h>
#include<Source\Application\Screen\Screen.h>

namespace framework {

	CubeMapInstanceRenderer::CubeMapInstanceRenderer(std::string modelName, std::vector <std::string> textureNames)
	{
		m_pAlwaysBuffer = util::createConstantBuffer(sizeof(AlwaysBuffer));
		m_pSometimeBuffer = util::createConstantBuffer(sizeof(SometimeBuffer));
		m_ModelName = modelName;
		m_TextureNames = textureNames;

		m_ViewMat = std::make_unique<util::Mat4[]>(6);
	}

	CubeMapInstanceRenderer::~CubeMapInstanceRenderer()
	{
	}

	void CubeMapInstanceRenderer::init()
	{
		m_Model = ResourceManager::getInstance()->getModel(m_ModelName);
		//マテリアル取得はハードコーディングで固定のものを取得するようにしておく
		m_pMaterial = ResourceManager::getInstance()->getMaterial("RigidInstanceEnvironment");
		m_Textures.reserve(m_TextureNames.size());
		for (auto& name : m_TextureNames) {
			m_Textures.emplace_back(ResourceManager::getInstance()->getTexture2D(name));
		}

		//入力レイアウト作成/////////////
		//TODO　レンダラインスタンスごとにIAオブジェクトが重複するので対策が必要
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "SV_Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",		0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3),D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "BINORMAL",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3) * 2,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TANGENT",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3) * 3,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXTURE"	   ,0,DXGI_FORMAT_R32G32_FLOAT   ,0,sizeof(util::Vec3) * 4,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		m_pInputLayout = util::createIA(layout, _countof(layout), m_pMaterial->getVertexByte()->GetBufferPointer(), m_pMaterial->getVertexByte()->GetBufferSize());

		auto sampler = util::createSamplerState(util::SamplerType::Clamp);
		m_pMaterial->setSamplerState(0, 1, &sampler.p);

		m_pShaderResource = std::make_unique<ID3D11ShaderResourceView*[]>(m_Textures.size());
		util::foreach(m_Textures.size(), [&](int i) {
			m_pShaderResource[i] = *(m_Textures[i]->getReource());
		});

		util::Transform trans;
		m_pCenterPosition = &trans;
		m_pCenterPosition->m_Position = util::Vec3(0, 0, 0);
		m_Projection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(Screen::PERSPECTIVE),
			Screen::WINDOW_WIDTH / (float)Screen::WINDOW_HEIGHT,//floatし忘れ注意
			1.0f,
			Screen::FAR_);
	}

	void CubeMapInstanceRenderer::setPipeline(int drawNum)
	{
		//パイプラインにシェーダーセット
		m_pMaterial->active();

		cameraUpdate();

		util::updateConstantBuffer<AlwaysBuffer>(m_pAlwaysBuffer.p, m_AlwaysBuffer);

		SometimeBuffer sb;

		sb.proj = XMMatrixTranspose(m_Projection.toXMMatrix());

		util::updateConstantBuffer<SometimeBuffer>(m_pSometimeBuffer.p, sb);

		//頂点バッファセット
		UINT strides[1] = { sizeof(RigidVertexLayout) };
		UINT offsets[1] = { 0 };
		util::getContext()->IASetVertexBuffers(0, 1, &m_Model->mesh.vertexBuffer.p, strides, offsets);
		util::getContext()->IASetIndexBuffer(m_Model->mesh.indexBuffer.p, DXGI_FORMAT_R32_UINT, 0);

		//	m_pMaterial->setTexture(0,1, m_Model.texture->getReource());

		util::getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//定数バッファにセットするための変数
		ID3D11Buffer* mediationBuffer[2];

		//順番厳守
		mediationBuffer[0] = m_pSometimeBuffer.p;
		mediationBuffer[1] = m_pAlwaysBuffer.p;

		m_pMaterial->setConstantBuffer(mediationBuffer, 2);

		util::getContext()->IASetInputLayout(m_pInputLayout.p);

		util::getContext()->DrawInstanced(m_Model->drawNum, drawNum, 0, 0);

		m_pMaterial->deActive();
	}

	void CubeMapInstanceRenderer::cameraUpdate()
	{
		Camera camera;
		camera.setEyePosition(m_pCenterPosition->m_Position);
		//+X
		camera.setLookAt(m_pCenterPosition->m_Position + util::Vec3(1,0,0));
		m_ViewMat[0] = camera.toViewMatrix();
		//-X
		camera.setLookAt(m_pCenterPosition->m_Position + util::Vec3(-1, 0, 0));
		m_ViewMat[1] = camera.toViewMatrix();
		//+Y
		camera.setLookAt(m_pCenterPosition->m_Position + util::Vec3(0, 1, 0));
		m_ViewMat[2] = camera.toViewMatrix();
		//-Y
		camera.setLookAt(m_pCenterPosition->m_Position + util::Vec3(0, -1, 0));
		m_ViewMat[3] = camera.toViewMatrix();
		//+Z
		camera.setLookAt(m_pCenterPosition->m_Position + util::Vec3(0, 0, 1));
		m_ViewMat[4] = camera.toViewMatrix();
		//-Z
		camera.setLookAt(m_pCenterPosition->m_Position + util::Vec3(0, 0, -1));
		m_ViewMat[5] = camera.toViewMatrix();

		util::foreach(6, [&](int i) {
			m_AlwaysBuffer.view[i] = m_ViewMat[i].toXMMatrix();
		});
	}


	void CubeMapInstanceRenderer::setCenterTransform(util::Transform * centerPosition)
	{
		m_pCenterPosition = centerPosition;
	}

	void CubeMapInstanceRenderer::instanceAction(int instanceNum, util::Transform * trans)
	{
		m_AlwaysBuffer.world[instanceNum] = trans->toMatrix().toXMMatrix();
		m_AlwaysBuffer.rotate[instanceNum] = trans->toRotateMatrix().toXMMatrix();
	}

}