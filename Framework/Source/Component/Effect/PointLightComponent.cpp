#include"PointLightComponent.h"
#include<Source\Entity\Entity.h>
#include<Source\Util\WrapFunc.h>

namespace component {

	LightList PointLightComponent::m_PointLightList;

	std::shared_ptr<PointLightElement*> PointLightComponent::m_Result = nullptr;

	PointLightComponent::PointLightComponent()
	{
	}

	PointLightComponent::~PointLightComponent()
	{
	}

	void PointLightComponent::active()
	{
		UpdateComponent::active();
		auto key = m_Entity.lock()->getName();
		//�L�[�����łɓo�^����Ă���Ώ������Ȃ�
		if (m_PointLightList.find(key) != m_PointLightList.end())return;
		m_PointLightList[key] = &m_LightAttribute;

		//TODO ���������ɕ��S���傫���̂ŏꍇ�ɂ���Ă͕��@��ς���K�v������
		//�A�N�e�B�u�ɂ��z��̃T�C�Y���ς��
		m_Result = std::shared_ptr<PointLightElement*>(new PointLightElement*[m_PointLightList.size()], std::default_delete<PointLightElement*[]>());
	}

	void PointLightComponent::deActive()
	{
		UpdateComponent::deActive();
		auto itr = m_PointLightList.find(m_Entity.lock()->getName());
		if (itr == m_PointLightList.end())return;
		m_PointLightList.erase(itr);


		//�f�B�A�N�e�B�u�ɂ��z��̃T�C�Y���ς��
		m_Result = std::shared_ptr<PointLightElement*>(new PointLightElement*[m_PointLightList.size()], std::default_delete<PointLightElement*[]>());
	}

	const int PointLightComponent::getTotalLightNum()
	{
		return m_PointLightList.size();
	}

	std::shared_ptr<PointLightElement*> PointLightComponent::getLightArray()
	{
		int i = 0;
		for (auto& element : m_PointLightList) {
			m_Result.get()[i] = element.second;
			i++;
		}
		return m_Result;
	}

	void PointLightComponent::update()
	{
		//�|�W�V�����͖��t���[���X�V����
		m_LightAttribute.position = m_Entity.lock()->getTransform()->m_Position;
	}

	void PointLightComponent::remove()
	{
		if (m_PointLightList.empty())return;
		m_PointLightList.clear();
	}

	//0 power
	//1 near
	//2 far
	//3 ~ 5 color
	void PointLightComponent::setParam(const std::vector<std::string>& param)
	{
		m_LightAttribute.position = m_Entity.lock()->getTransform()->m_Position;
		m_LightAttribute.power = std::atof(param[0].c_str());
		m_LightAttribute.near_ = std::atof(param[1].c_str());
		m_LightAttribute.far_ = std::atof(param[2].c_str());
		m_LightAttribute.color = util::atof<util::Vec3>(param,3).reault;
	}

	//void PointLightComponent::setPower(const util::Vec3& power)
	//{
	//	m_LightAttribute.power = power.x;
	//	m_LightAttribute.near_ = power.y;
	//	m_LightAttribute.far_  = power.z;
	//}

	PointLightElement*  PointLightComponent::getAttrib()
	{
		return &m_LightAttribute;
	}

}