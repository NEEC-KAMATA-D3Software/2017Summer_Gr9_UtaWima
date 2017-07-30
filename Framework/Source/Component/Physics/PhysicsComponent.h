#pragma once
#include<Source\Component\Component.h>
#include<Source\State\State.h>
#include<list>
#include<deque>

namespace framework {

	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent();

		~PhysicsComponent();

		/**
		* @brief		���̃R���|�[���g���A�N�e�B�u�ɂ���
		*/
		virtual void active();

		/**
		* @brief		���̃R���|�[���g���f�B�A�N�e�B�u�ɂ���
		*/
		virtual void deActive();


		void init()override;

		/**
		* @brief			�t���O�̍X�V�݂̂��s��
		*/
		void collisionUpdate();

		/**
		* @brief			��������ƃq�b�g���Ă��邩�ǂ���
		*/
		bool isHit();

		/**
		* @brief			�w�肵���I�u�W�F�N�g�ƃq�b�g���Ă��邩�ǂ���
		* @param entity		�������Ă��邩�m���߂����I�u�W�F�N�g
		*/
		bool isHit(std::weak_ptr<Entity> entity);

		void hitCheck(std::weak_ptr<PhysicsComponent> other);

		/**
		* @brief			�q�b�g�I�u�W�F�N�g�ɑ΂��Ẵf���Q�[�g
		* @param hitFlag	���̃t���O��true�̎���hit���Ă���̂ɑ΂��čs���� false�͂��̋t
		* @param action		���삷��֐��|�C���^
		*/
		void hitObjAction(bool hitFlag,std::function<void(std::weak_ptr<Entity>)> action);

		/**
		* @brief			������s���R�[���o�b�N
		*/
		virtual bool hitCheckCallBack(std::weak_ptr<PhysicsComponent> other) = 0;

		/**
		* @brief			�q�b�g�R���e�i�ɔ�A�N�e�B�u�Ȃ��̂���������폜����
		*/
		void checkDelete();

	protected:
		/**
		* @brief			�����̌^���`�F�b�N
		*/
		template<typename T>
		bool isMold(std::weak_ptr<PhysicsComponent> other,std::shared_ptr<T>* result) {
			if (other.expired()) {
				return false;
			}
			auto circleComp = std::dynamic_pointer_cast<T>(other.lock());
			*result = circleComp;
			//�`�F�b�N�������肪�����Ɠ����łȂ���Ώ������Ȃ�
			if (!circleComp.get()) {
				return false;
			}
			return true;
		}

	protected:
		using HitPair = std::pair<std::weak_ptr<PhysicsComponent>, bool>;
		using HitContainer = std::list<HitPair>;

		HitContainer m_HitContainer;
	private:
		//�{�̂Ƒ���
		using CallBackContainer = std::deque<std::pair<std::weak_ptr<Entity>, std::weak_ptr<Entity>>>;
		
		CallBackContainer onCollitionContainer;
		CallBackContainer onStayContainer;
		CallBackContainer onExitContainer;
	};



}