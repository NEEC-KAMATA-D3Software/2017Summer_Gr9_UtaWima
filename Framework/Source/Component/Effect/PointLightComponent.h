#pragma once
#include<Source\Component\UpdateComponent.h>
#include<Source\Util\Type.h>
#include<unordered_map>
#include<utility>
#include<string>

namespace component {

	struct PointLightElement {
		PointLightElement()
			:position(), color(), power(0),
			near_(0), far_(0)
		{
		}

		PointLightElement(util::Vec3 pos, util::Vec3 color_, float power_, float near_, float far_) {
			this->position = pos;
			this->color = color_;
			this->power = power_;
			this->near_ = near_;
			this->far_ = far_;
		}

		PointLightElement(const PointLightElement& other)
			:position(other.position), color(other.color), power(other.power),
			near_(other.near_), far_(other.far_)
		{
		}

		util::Vec3 position;
		util::Vec3 color;
		float power;//!�|�C���g���C�g�̋���
		float near_;//!�|�C���g���C�g�ɋ߂����̂ւ̉e���x
		float far_;//!�|�C���g���C�g�ɉ������̂ւ̉e���x
	};

	using LightList = std::unordered_map<std::string, PointLightElement*>;

	class PointLightComponent : public framework::UpdateComponent
	{
	public:
		PointLightComponent();
		~PointLightComponent();

		/**
		* @brief		�A�N�e�B�u�ɂ���
		*/
		virtual void active() override;

		/**
		* @brief		�f�B�A�N�e�B�u�ɂ���
		*/
		virtual void deActive() override;

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);

		///**
		//* @brief		���C�g�̃p���[���Z�b�g
		//* @param power  ������p�����[�^
		//*/
		//virtual void setPower(const util::Vec3& power);

		/**
		* @brief		�p�����[�^�擾
		*/
		virtual PointLightElement* getAttrib();

		/**
		* @brief		�S�̂̃|�C���g���C�g�̐��擾
		*/
		static const int getTotalLightNum();

		/**
		* @brief		�S�̂̃|�C���g���C�g�̎擾
		*/
		static std::shared_ptr<PointLightElement*> getLightArray();

		// UpdateComponent ����Čp������܂���
		virtual void update() override;

		/**
		* @brief		�J������
		*/
		virtual void remove()override;

	private:
		//!�S�̂̃|�C���g���C�g
		static LightList m_PointLightList;
		static std::shared_ptr<PointLightElement*> m_Result;

		//!�|�C���g���C�g�̗v�f
		PointLightElement m_LightAttribute;




	};


}