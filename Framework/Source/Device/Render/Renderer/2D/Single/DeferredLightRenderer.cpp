#include"DeferredLightRenderer.h"
#include<Source\Component\Effect\DirectionalLightComponent.h>
#include<Source\Component\Effect\PointLightComponent.h>
#include<Source\Component\CameraComponent.h>
#include<Source\Entity\Entity.h>

namespace framework {


	DeferredLightRenderer::DeferredLightRenderer()
		:ISpriteRenderer("DeferredLighting")
	{
	}

	DeferredLightRenderer::~DeferredLightRenderer()
	{
	}

	void DeferredLightRenderer::begin()
	{
	}

	void DeferredLightRenderer::end()
	{
	}

	void DeferredLightRenderer::setPipeline()
	{
		DeferredAlways ab;

		ab.world = m_WoldMat.toXMMatrix();
		util::Vec3&& dPos = component::DirectionalLightComponent::getLightPosition();
		util::Vec3&& dColor = component::DirectionalLightComponent::getLightColor();
		ab.directionalLightPosition = util::Vec4(dPos.x, dPos.y, dPos.z,1);
		ab.directionalLightColor = util::Vec4(dColor.x, dColor.y, dColor.z,1);

		auto&& lightArray = component::PointLightComponent::getLightArray();
		ab.activeLightNum = component::PointLightComponent::getTotalLightNum();

		util::foreach(ab.activeLightNum, [&](int i) {
			util::Vec3 pPos = lightArray.get()[i]->position;
			ab.lightPos[i] = util::Vec4(pPos.x, pPos.y, pPos.z,1);
			util::Vec3 pColor = lightArray.get()[i]->color;
			ab.pointLightColor[i] = util::Vec4(pColor.x, pColor.y, pColor.z, 1);
			ab.attribu[i].x = lightArray.get()[i]->power;
			ab.attribu[i].y = lightArray.get()[i]->near_;
			ab.attribu[i].z = lightArray.get()[i]->far_;
		});

		ab.cameraPos = component::CameraComponent::getMainCamera().lock()->getGameObj().lock()->getTransform()->m_Position;

		util::updateConstantBuffer<DeferredAlways>(m_pAlwaysBuffer.p, ab);

		DeferredSometime sb;
		sb.projection = m_Projection.toXMMatrix();

		util::updateConstantBuffer<DeferredSometime>(m_pSometimeBuffer.p, sb);
	}


}