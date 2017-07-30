#pragma once
#include<Source\Util\Thread\IRunnable.h>
#include <thread>
#include<functional>
#include<memory>

namespace util {

	class ThreadAction : public IRunnable
	{
	public:
		ThreadAction(std::function<void()> action);
		~ThreadAction();

		void run();

	private:
		std::function<void()> m_Action;
	};


	class Thread : public IRunnable
	{
	public:
		/**
		* @brief	�R���X�g���N�^
		*/
		Thread() ;

		/**
		* @brief	�R���X�g���N�^
		*/
		Thread(std::shared_ptr<IRunnable> runnbale);

		/**
		* @brief	�R���X�g���N�^
		*/
		Thread(std::function<void()> action);


		/**
		* @brief	�f�X�g���N�^
		*/
		~Thread() ;

		/**
		* @brief	�X���b�h�̊J�n
		*/
		void start();

		/**
		* @brief	�X���b�h�̏I����҂�
		*/
		void join();

		/**
		* @brief	�X���b�h�̔j��
		*/
		void detach();

		/**
		* @brief				�X���b�h�̃X���[�v
		* @param millsecond	�X���[�v���ԁi�~���b�j
		*/
		void sleep(unsigned int millsecond);

		/**
		* @brief	ID�̎擾
		* @return	ID
		*/
		std::thread::id getId() const;
		
		/**
		* @brief	�X���b�h�̎��s
		*/
		virtual void run() override;

	private:
		std::thread m_Thread;
		//!Run���\�b�h�ŌĂ΂��֐��|�C���^
		std::shared_ptr<IRunnable> m_RunFunc = nullptr;
	};
}