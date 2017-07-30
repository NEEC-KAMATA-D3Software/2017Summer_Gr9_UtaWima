#include"RigidInstanceRenderer.h"
#include<Source\Device\Render\Shader\StandardMaterial.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Math\Transform.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Device\Camera\Camera.h>
#include<Source\Resource\Texture\CubeReanderTarget.h>
#include<Source\Component\CameraComponent.h>
#include<Source\Device\Input\Input.h>
#include<Source\Util\Math\Math.h>
#include<Source\Entity\Entity.h>
#include<Source\Application\Screen\Screen.h>
#include<Source\Util\Render\DXFunc.h>

namespace framework {


	RigidInstanceRenderer::RigidInstanceRenderer(std::shared_ptr<Model> model, std::vector <std::string> textureNames)
	{
		m_pAlwaysBuffer = util::createConstantBuffer(sizeof(AlwaysBuffer));
		m_pSometimeBuffer = util::createConstantBuffer(sizeof(SometimeBuffer));
		m_pMaterialBuffer = util::createConstantBuffer(sizeof(MaterialBuffer));
		m_Model = model;
		m_TextureNames = textureNames;
	}

	RigidInstanceRenderer::~RigidInstanceRenderer()
	{

	}

	void RigidInstanceRenderer::init()
	{
		//マテリアル取得はハードコーディングで固定のものを取得するようにしておく
		m_pMaterial = ResourceManager::getInstance()->getMaterial("RigidInstance");

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

		m_Textures.reserve(m_TextureNames.size());
		for (auto& name : m_TextureNames) {
			m_Textures.emplace_back(ResourceManager::getInstance()->getTexture2D(name));
		}
		m_TextureNum = m_Textures.size();
		m_pShaderResource = std::make_unique<ID3D11ShaderResourceView*[]>(m_TextureNum);
		util::foreach(m_Textures.size(), [&](int i) {
			m_pShaderResource[i] = *(m_Textures[i]->getReource());
		});

		m_MaterialBuffer.specular = 1.0f;
		m_MaterialBuffer.height = 1.0f;

		m_Projection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(Screen::PERSPECTIVE),
			Screen::WINDOW_WIDTH / (float)Screen::WINDOW_HEIGHT,//floatし忘れ注意
			1.0f,
			Screen::FAR_);
	}

	void RigidInstanceRenderer::setCubMap(const std::vector<std::shared_ptr<framework::CubeRenderTrget>>&  cubMap)
	{
		m_pCubMaps.first = cubMap.size();
		m_pCubMaps.second = std::make_unique<ID3D11ShaderResourceView*[]>(m_pCubMaps.first);
	
		util::foreach(m_pCubMaps.first, [&](int i) {
			m_pCubMaps.second[i] = cubMap[i]->getShaderView();
		});
	}

	void RigidInstanceRenderer::setCubMap(int instanceNum,const std::shared_ptr<framework::CubeTexture>& cubMap)
	{
		m_pCubMaps.first = instanceNum;
		m_pCubMaps.second = std::make_unique<ID3D11ShaderResourceView*[]>(m_pCubMaps.first);

		util::foreach(m_pCubMaps.first, [&](int i) {
			m_pCubMaps.second[i] = cubMap->getShaderResourceView();
		});
	}

	void RigidInstanceRenderer::setMaterialParam(MaterialParam & param)
	{
		m_MaterialBuffer.specular = param.specular;
		m_MaterialBuffer.height = param.height;
		m_MaterialBuffer.isNotLighting = param.isNotLighting;
	}

	void RigidInstanceRenderer::clear()
	{
		m_TextureNames.clear();
		m_Textures.clear();
		m_pShaderResource.reset();
	}

	void RigidInstanceRenderer::instanceAction(int instanceNum, util::Transform* trans)
	{
		m_AlwaysBuffer.world[instanceNum] = trans->toMatrix().toXMMatrix();
		m_AlwaysBuffer.rotate[instanceNum] = trans->toRotateMatrix().toXMMatrix();
	}

	void RigidInstanceRenderer::setPipeline(int drawNum)
	{
		//パイプラインにシェーダーセット
		m_pMaterial->active();

		setTexture();

		m_AlwaysBuffer.view = component::CameraComponent::getMainCamera().lock()->toViewMatrix().toXMMatrix();

		m_AlwaysBuffer.eyePos = component::CameraComponent::getMainCamera().lock()->getGameObj().lock()->getTransform()->m_Position;

	//	util::shadowViewMat(util::Mat4(), util::Mat4(), util::Mat4(),&a);
		//m_AlwaysBuffer.shadowViewMat = 

		util::updateConstantBuffer<AlwaysBuffer>(m_pAlwaysBuffer.p, m_AlwaysBuffer);

		SometimeBuffer sb;

		sb.proj = XMMatrixTranspose(m_Projection.toXMMatrix());

		util::updateConstantBuffer<SometimeBuffer>(m_pSometimeBuffer.p, sb);

		util::updateConstantBuffer<MaterialBuffer>(m_pMaterialBuffer.p, m_MaterialBuffer);

		//頂点バッファセット
		UINT strides[1] = { sizeof(RigidVertexLayout) };
		UINT offsets[1] = { 0 };
		util::getContext()->IASetVertexBuffers(0, 1, &m_Model->mesh.vertexBuffer.p, strides, offsets);
		util::getContext()->IASetIndexBuffer(m_Model->mesh.indexBuffer.p, DXGI_FORMAT_R32_UINT, 0);
		util::getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//定数バッファにセットするための変数
		ID3D11Buffer* mediationBuffer[3];

		//順番厳守
		mediationBuffer[0] = m_pSometimeBuffer.p;
		mediationBuffer[1] = m_pAlwaysBuffer.p;
		mediationBuffer[2] = m_pMaterialBuffer.p;

		m_pMaterial->setConstantBuffer(mediationBuffer, 3);

		util::getContext()->IASetInputLayout(m_pInputLayout.p);

		util::getContext()->DrawInstanced(m_Model->drawNum, drawNum, 0, 0);

		m_pMaterial->deActive();

	}

	void RigidInstanceRenderer::setTexture()
	{
		m_pMaterial->setTexture(0, m_TextureNum, m_pShaderResource.get());

		auto shadowMap = ResourceManager::getInstance()->getTexture2D("ShadowMap");
		//firestはテクスチャの数
		m_pMaterial->setTexture(9, 1, shadowMap->getReource());//キューブマップは10番目にセットする
		//firestはテクスチャの数
		m_pMaterial->setTexture(10, m_pCubMaps.first, m_pCubMaps.second.get());//キューブマップは10番目にセットする
	}


}