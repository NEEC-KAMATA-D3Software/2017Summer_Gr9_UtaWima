#include "SkinningInstanceRenderer.h"
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
#include<Source\Resource\Motion\Motion.h>
#include<Source\Component\Effect\DirectionalLightComponent.h>
#include<Source\Entity\Entity.h>
#include<Source\Resource\Texture\BoneTexture.h>
#include<Source\Util\Thread\Thread.h>
#include<Source\Resource\ResourceManager.h>
#include<iostream>

namespace framework {
	//std::unique_ptr<BoneTexture> SkinningInstanceRenderer::m_pBoneTexure = std::make_unique<BoneTexture>(BONE_NUM , MAX_MOTION_FRAME);

	SkinningInstanceRenderer::SkinningInstanceRenderer(std::shared_ptr<Model> model, std::vector<std::string> textureNames, std::vector <std::string> motionNames)
	{
		m_ConstantData.pAlwaysGPU = util::createConstantBuffer(sizeof(AlwaysBuffer));
		m_ConstantData.pSometimeGPU = util::createConstantBuffer(sizeof(SometimeBuffer));
		m_ConstantData.pMaterialGPU = util::createConstantBuffer(sizeof(MaterialBuffer));
		m_ConstantData.pSkinningGPU = util::createConstantBuffer(sizeof(SkinningBuffer));
		m_ResourceData.pModel = model;
		m_LoadName.textureNames = textureNames;
		m_LoadName.motionNames = motionNames;
	}

	SkinningInstanceRenderer::~SkinningInstanceRenderer()
	{
	}

	void SkinningInstanceRenderer::init()
	{
		//マテリアル取得はハードコーディングで固定のものを取得するようにしておく
		m_pMaterial = ResourceManager::getInstance()->getMaterial("SkinningInstance");

		//入力レイアウト作成/////////////
		//TODO　レンダラインスタンスごとにIAオブジェクトが重複するので対策が必要
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "SV_Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",		0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3),D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "BINORMAL",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3) * 2,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TANGENT",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3) * 3,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXTURE"	   ,0,DXGI_FORMAT_R32G32_FLOAT   ,0,sizeof(util::Vec3) * 4,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXTURE"	   ,1,DXGI_FORMAT_R32G32B32A32_UINT   ,0,sizeof(util::Vec2) + sizeof(util::Vec3) * 4,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXTURE"	   ,2,DXGI_FORMAT_R32G32B32A32_FLOAT  ,0,(sizeof(util::Vec2) + sizeof(util::Vec3) * 4) + sizeof(util::Vec4),D3D11_INPUT_PER_VERTEX_DATA,0 },

		};
		m_pInputLayout = util::createIA(layout, _countof(layout), m_pMaterial->getVertexByte()->GetBufferPointer(), m_pMaterial->getVertexByte()->GetBufferSize());

		auto sampler = util::createSamplerState(util::SamplerType::Clamp);
		m_pMaterial->setSamplerState(0, 1, &sampler.p);

		m_ResourceData.pTextures.reserve(m_LoadName.textureNames.size());
		for (auto& name : m_LoadName.textureNames) {
			m_ResourceData.pTextures.emplace_back(ResourceManager::getInstance()->getTexture2D(name));
		}
		m_TextureNum = m_ResourceData.pTextures.size();
		m_pShaderResource = std::make_unique<ID3D11ShaderResourceView*[]>(m_TextureNum);
		util::foreach(m_TextureNum, [&](int i) {
			m_pShaderResource[i] = *(m_ResourceData.pTextures[i]->getReource());
		});

		m_ConstantData.materialBuffer.specular = 1.0f;
		m_ConstantData.materialBuffer.height = 1.0f;

		m_Projection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(Screen::PERSPECTIVE),
			Screen::WINDOW_WIDTH / (float)Screen::WINDOW_HEIGHT,//floatし忘れ注意
			1.0f,
			Screen::FAR_);

		//!アニメーションの数
		const int AnimationNum = 10;

		//縦にアニメーションをつんでいく
		m_ResourceData.pBoneTexure = std::make_unique<BoneTexture>(BONE_NUM, MAX_MOTION_FRAME * AnimationNum);

		//ボーンテクスチャに書き込み
		//writeBoneTexture();
	}

	void SkinningInstanceRenderer::setCubMap(const std::vector<std::shared_ptr<framework::CubeRenderTrget>>& cubMap)
	{
		m_pCubMaps.first = cubMap.size();
		m_pCubMaps.second = std::make_unique<ID3D11ShaderResourceView*[]>(m_pCubMaps.first);

		util::foreach(m_pCubMaps.first, [&](int i) {
			m_pCubMaps.second[i] = cubMap[i]->getShaderView();
		});
	}

	void SkinningInstanceRenderer::setCubMap(int instanceNum, const std::shared_ptr<framework::CubeTexture>& cubMap)
	{
		m_pCubMaps.first = instanceNum;
		m_pCubMaps.second = std::make_unique<ID3D11ShaderResourceView*[]>(m_pCubMaps.first);

		util::foreach(m_pCubMaps.first, [&](int i) {
			m_pCubMaps.second[i] = cubMap->getShaderResourceView();
		});
	}

	void SkinningInstanceRenderer::setMaterialParam(MaterialParam & param)
	{
		m_ConstantData.materialBuffer.specular = param.specular;
		m_ConstantData.materialBuffer.height = param.height;
		m_ConstantData.materialBuffer.isNotLighting = param.isNotLighting;
	}

	void SkinningInstanceRenderer::setAnimator(std::vector<std::weak_ptr<component::AnimatorComponent>> animators)
	{
		m_Animators = animators;

		if (m_Animators.size() == 0)return;

		writeBoneTexture(m_Animators[0]);
	}

	void SkinningInstanceRenderer::instanceAction(int instanceNum, util::Transform * trans)
	{
		m_ConstantData.alwaysBuffer.world[instanceNum] = trans->toMatrix().toXMMatrix();
		m_ConstantData.alwaysBuffer.rotate[instanceNum] = trans->toRotateMatrix().toXMMatrix();

		frameSet(instanceNum);
	}

	void SkinningInstanceRenderer::setPipeline(int drawNum)
	{
		//パイプラインにシェーダーセット
		m_pMaterial->active();

		setTexture();

		m_ConstantData.alwaysBuffer.view = component::CameraComponent::getMainCamera().lock()->toViewMatrix().toXMMatrix();

		m_ConstantData.alwaysBuffer.eyePos = component::CameraComponent::getMainCamera().lock()->getGameObj().lock()->getTransform()->m_Position;

		//auto lightMat = component::DirectionalLightComponent::getTransform()->toMatrix();

		util::Mat4 shadowCoord = component::DirectionalLightComponent::getCamera().lock()->toViewMatrix().toXMMatrix();

		//	util::shadowViewMat(lightMat, m_Projection, util::Mat4(),&shadowCoord);

		m_ConstantData.alwaysBuffer.shadowViewMat = shadowCoord.toXMMatrix();

		SometimeBuffer sb;

		sb.proj = XMMatrixTranspose(m_Projection.toXMMatrix());

		util::updateConstantBuffer<AlwaysBuffer>(m_ConstantData.pAlwaysGPU.p, m_ConstantData.alwaysBuffer);

		util::updateConstantBuffer<SometimeBuffer>(m_ConstantData.pSometimeGPU.p, sb);

		util::updateConstantBuffer<MaterialBuffer>(m_ConstantData.pMaterialGPU.p, m_ConstantData.materialBuffer);

		util::updateConstantBuffer<SkinningBuffer>(m_ConstantData.pSkinningGPU.p, m_ConstantData.skinningBuffer);

		//頂点バッファセット
		UINT strides[1] = { sizeof(SkinningVertexLayout) };
		UINT offsets[1] = { 0 };
		util::getContext()->IASetVertexBuffers(0, 1, &m_ResourceData.pModel->mesh.vertexBuffer.p, strides, offsets);
		util::getContext()->IASetIndexBuffer(m_ResourceData.pModel->mesh.indexBuffer.p, DXGI_FORMAT_R32_UINT, 0);
		util::getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//定数バッファにセットするための変数
		ID3D11Buffer* mediationBuffer[4];

		//順番厳守
		mediationBuffer[0] = m_ConstantData.pSometimeGPU.p;
		mediationBuffer[1] = m_ConstantData.pAlwaysGPU.p;
		mediationBuffer[2] = m_ConstantData.pMaterialGPU.p;
		mediationBuffer[3] = m_ConstantData.pSkinningGPU.p;

		m_pMaterial->setConstantBuffer(mediationBuffer, 4);

		util::getContext()->IASetInputLayout(m_pInputLayout.p);

		util::getContext()->DrawInstanced(m_ResourceData.pModel->drawNum, drawNum, 0, 0);

		m_pMaterial->deActive();
	}

	void SkinningInstanceRenderer::setTexture()
	{
		m_pMaterial->setTexture(0, m_TextureNum, m_pShaderResource.get());

		auto shadowMap = ResourceManager::getInstance()->getTexture2D("ShadowMap");
		//firestはテクスチャの数
		m_pMaterial->setTexture(9, 1, shadowMap->getReource());//キューブマップは10番目にセットする

		m_pMaterial->setTexture(10, m_pCubMaps.first, m_pCubMaps.second.get());//キューブマップは10番目にセットする

		auto setView = m_ResourceData.pBoneTexure->getShaderView();
		m_pMaterial->setTexture(11, 1, &setView);
	}

	void SkinningInstanceRenderer::writeBoneTexture(std::weak_ptr<component::AnimatorComponent> animator)
	{
		//TODO ここでアニメーションをテクスチャに
		//TODOアニメーターの順番にする必要がある

		//auto& motionNames = m_LoadName.motionNames;

		std::vector<std::shared_ptr<Motion>> motionContainer;

	/*	for (auto name : motionNames) {
			motionContainer.emplace_back(ResourceManager::getInstance()->getMotion(name));
		}*/

		for (auto name : animator.lock()->getMotionNames()) {
			motionContainer.emplace_back(ResourceManager::getInstance()->getMotion(name));
		}

		m_ResourceData.pBoneTexure->begin();
		//モーションの種類分まわす
		util::foreach(motionContainer.size(), [&](int i) {

			//!マトリクスの行の数
			const int matrixY = 4;

			const int motionOffset = ((MAX_MOTION_FRAME * matrixY)) * i;

			util::foreach(MAX_MOTION_FRAME, [&](int frame) {

				const int frameOffset = (frame * matrixY) + motionOffset;

				auto motionMat = motionContainer[i]->getFrameMotion(frame);

				util::foreach(motionMat.size(), [&](int boneNum) {

					auto mat = XMMatrixTranspose(motionMat[boneNum].toXMMatrix());

					m_ResourceData.pBoneTexure->beginToBuffer(boneNum, frameOffset + 0, [&](float* r, float* g, float* b, float* a) {
						*r = mat._11;
						*g = mat._12;
						*b = mat._13;
						*a = mat._14;
					});

					m_ResourceData.pBoneTexure->beginToBuffer(boneNum, frameOffset + 1, [&](float* r, float* g, float* b, float* a) {
						*r = mat._21;
						*g = mat._22;
						*b = mat._23;
						*a = mat._24;
					});

					m_ResourceData.pBoneTexure->beginToBuffer(boneNum, frameOffset + 2, [&](float* r, float* g, float* b, float* a) {
						*r = mat._31;
						*g = mat._32;
						*b = mat._33;
						*a = mat._34;
					});

					m_ResourceData.pBoneTexure->beginToBuffer(boneNum, frameOffset + 3, [&](float* r, float* g, float* b, float* a) {
						*r = mat._41;
						*g = mat._42;
						*b = mat._43;
						*a = mat._44;
					});

				});
			});

		});
		m_ResourceData.pBoneTexure->end();
	}

	void SkinningInstanceRenderer::frameSet(const int instanceID)
	{
		//アニメータからフレームを取得
		if (m_Animators.size() < instanceID + 1) {
			assert(false && "数が足りません");
			m_ConstantData.skinningBuffer.drawData[instanceID] = util::Vec4();
			return;
		}

		if (m_Animators[instanceID].expired()) {
			assert(false && "アニメーターが接続されていません");
			m_ConstantData.skinningBuffer.drawData[instanceID] = util::Vec4();
			return;
		}

		
		m_ConstantData.skinningBuffer.drawData[instanceID].x = m_Animators[instanceID].lock()->getFrame();
		m_ConstantData.skinningBuffer.drawData[instanceID].y = m_Animators[instanceID].lock()->getCurrentMotionID();
		//アにメーターフラグを立てる
		m_ConstantData.skinningBuffer.drawData[instanceID].z = 1;
	}

	void SkinningInstanceRenderer::clear()
	{
		m_ResourceData.pTextures.clear();
		m_Animators.clear();
		m_ResourceData.pBoneTexure.reset();
	}

}
