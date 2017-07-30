#pragma once
#include"Component.h"
#include<memory>
#include<list>
#include<utility>

/**
* @file		CubeMapCreateOwnerComponent.h
* @brief	Renderer�N���C�A���g���������ăL���[�u�}�b�v���쐬����
* @author	���{�D�P
* @date	2017/3/19
*/

namespace util {
	struct Transform;
}

namespace framework {
	class CubeMapInstanceRenderer;
	class Entity;
	class CubeRenderTrget;
	class CubeDepthTarget;
}

namespace component {
	class RenderClientComponent;

	using EntityAndComponent = std::pair<std::weak_ptr<framework::Entity>, std::weak_ptr<RenderClientComponent>>;
	using EntityList = std::list<EntityAndComponent>;

	class CubeMapCreateComponent : public framework::Component
	{
	public:
		CubeMapCreateComponent();
		~CubeMapCreateComponent();
		/**
		* @brief		������
		* @Detail		���ׂẴR���|�[�l���g����ĂɌĂ΂��
		*/
		virtual void init()override;

		virtual void draw();

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param)override;

		/**
		* @brief		�L���[�u�}�b�v�擾
		*/
		std::shared_ptr<framework::CubeRenderTrget> getCubMap();

	private:
		/**
		* @brief		�L���ȏ�Ԃ̃G���e�B�e�B�[��I�ʂ��ĕԂ�
		*/
		std::list<util::Transform*> findActiveObject();

		/**
		* @brief		�G���e�B�e�B�[�̏�Ԃ��m�F���ĂǍ쐬�����ԂȂ�true��ς���
		*/
		bool isCreate(EntityAndComponent object);

	private:
		std::shared_ptr<framework::CubeRenderTrget> m_pCubeTarget;
		std::shared_ptr<framework::CubeDepthTarget> m_pCubeDepth;
		std::unique_ptr<framework::CubeMapInstanceRenderer> m_pCubMapRenderer;
		EntityList m_EntityList;
		

	};




}
