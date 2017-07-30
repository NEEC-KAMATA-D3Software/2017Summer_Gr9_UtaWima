#include"DirectionalLightComponent.h"
#include<list>
#include<algorithm>
#include<Source\Entity\Entity.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Component\CameraComponent.h>

namespace component {

	std::weak_ptr<DirectionalLightComponent> DirectionalLightComponent::m_DerectionalLight;

	DirectionalLightComponent::DirectionalLightComponent()
	{
		m_Color = util::Vec3(1,1,1);
	}

	DirectionalLightComponent::~DirectionalLightComponent()
	{
	}

	void DirectionalLightComponent::active()
	{
		haveDirectionalLightEntityLoop([&](framework::WeakEntity entity) {
			auto componentList = entity.lock()->getComponents<DirectionalLightComponent>();
			for (auto& light : componentList) {
				light.lock()->deActive();
			}
		});

		m_DerectionalLight = m_Entity.lock()->getComponent<DirectionalLightComponent>(this);
	}

	void DirectionalLightComponent::deActive()
	{
		m_DerectionalLight.reset();
	}

	void DirectionalLightComponent::componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)
	{
		auto camera = m_Entity.lock()->addComponent(std::make_shared<component::CameraComponent>());
		m_pCamera = std::dynamic_pointer_cast<CameraComponent>(camera.lock());
		m_pCamera.lock()->componentCreated(m_Entity);
		std::vector<std::string> param;
		param.reserve(2);
		param.emplace_back("none");
		param.emplace_back("1");
		m_pCamera.lock()->setParam(param);
		componentInitalizer->emplace_back(m_pCamera);
	}

	util::Vec3 DirectionalLightComponent::getLightPosition()
	{
		//�A�N�e�B�u�ȃ��C�g�R���|�[�l���g���Ȃ���ΐ^��ɐݒ�
		if (m_DerectionalLight.expired()) return util::Vec3(0,1,0);
		return m_DerectionalLight.lock()->getGameObj().lock()->getTransform()->m_Position;
	}

	util::Vec3 DirectionalLightComponent::getLightColor()
	{
		if (m_DerectionalLight.expired()) return util::Vec3(0, 0, 0);
		return m_DerectionalLight.lock()->getMColor();
	}

	std::weak_ptr<CameraComponent> DirectionalLightComponent::getCamera()
	{
		//�A�N�e�B�u�ȃ��C�g��������΋�̃J������n��
		if (m_DerectionalLight.expired())return std::weak_ptr<CameraComponent>();
		return m_DerectionalLight.lock()->m_pCamera;
	}

	//util::Transform * DirectionalLightComponent::getTransform()
	//{
	//	return m_DerectionalLight.lock()->getGameObj().lock()->getTransform();
	//}

	void DirectionalLightComponent::setParam(const std::vector<std::string>& param)
	{
		//rgb�������p�����[�^�Ƃ��ĂȂ���Ή������Ȃ�
		if (param.size() < 3) return;

		m_Color = util::atof<util::Vec3>(param).reault;
	}


	void DirectionalLightComponent::haveDirectionalLightEntityLoop(std::function<void(framework::WeakEntity)> entityAction)
	{
		haveComponentEntityLoop<DirectionalLightComponent>(entityAction);
	}

	util::Vec3 DirectionalLightComponent::getMColor()
	{
		return m_Color;
	}

}