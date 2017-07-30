#pragma once
#include"Component.h"
#include<Source\Util\Type.h>

namespace component {

	class BoxRenderClientComponent : public framework::UpdateComponent
	{
	public:
		BoxRenderClientComponent();
		~BoxRenderClientComponent();

		void init()override;

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);

		/**
		* @brief		�F�擾
		*/
		const util::Vec3 getColor();

		void setColor(util::Vec3& color);

		util::Vec3 getOffset();

		// UpdateComponent ����Čp������܂���
		virtual void update() override;

		util::Transform* getTransform();

	private:
		//!�{�b�N�X�̐F
		util::Vec3 m_Color;
		util::Vec3 m_Offset;
		util::Vec3 m_Scale = util::Vec3(1, 1, 1);
		util::Vec3 m_Angle = util::Vec3(0, 0, 0);


		util::Transform m_Transform;
	};

}