#include"BoxRenderClientComponent.h"
#include<Source\Util\WrapFunc.h>

namespace component {

	BoxRenderClientComponent::BoxRenderClientComponent()
	{
	}

	BoxRenderClientComponent::~BoxRenderClientComponent()
	{
	}

	void BoxRenderClientComponent::init()
	{
		m_Transform.m_Rotation = m_Angle;
		m_Transform.m_Scale = m_Scale;
	}

	void BoxRenderClientComponent::setParam(const std::vector<std::string>& param)
	{
		if (param.size() < 3)return;
		//パラメータをcharaからfloatに変えて保存
		m_Color = util::atof<util::Vec3>(param).reault;
		if (param.size() < 6)return;
		m_Offset = util::atof<util::Vec3>(param,3).reault;
		if (param.size() < 9)return;
		m_Scale = util::atof<util::Vec3>(param, 6).reault;
		if (param.size() < 12)return;
		m_Angle = util::atof<util::Vec3>(param, 9).reault;
	}

	const util::Vec3 BoxRenderClientComponent::getColor()
	{
		return m_Color;
	}

	void BoxRenderClientComponent::setColor(util::Vec3 & color)
	{
		m_Color = color;
	}

	util::Vec3 BoxRenderClientComponent::getOffset()
	{
		return m_Offset;
	}

	void BoxRenderClientComponent::update()
	{
		//ポジションのみ自分のエンティティーから受け継ぐ
		m_Transform.m_Position = m_Entity.lock()->getTransform()->m_Position + m_Offset;
	}

	util::Transform * BoxRenderClientComponent::getTransform()
	{
		return &m_Transform;
	}

}