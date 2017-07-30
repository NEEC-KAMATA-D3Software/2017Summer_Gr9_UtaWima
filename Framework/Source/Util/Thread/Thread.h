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
		* @brief	コンストラクタ
		*/
		Thread() ;

		/**
		* @brief	コンストラクタ
		*/
		Thread(std::shared_ptr<IRunnable> runnbale);

		/**
		* @brief	コンストラクタ
		*/
		Thread(std::function<void()> action);


		/**
		* @brief	デストラクタ
		*/
		~Thread() ;

		/**
		* @brief	スレッドの開始
		*/
		void start();

		/**
		* @brief	スレッドの終了を待つ
		*/
		void join();

		/**
		* @brief	スレッドの破棄
		*/
		void detach();

		/**
		* @brief				スレッドのスリープ
		* @param millsecond	スリープ時間（ミリ秒）
		*/
		void sleep(unsigned int millsecond);

		/**
		* @brief	IDの取得
		* @return	ID
		*/
		std::thread::id getId() const;
		
		/**
		* @brief	スレッドの実行
		*/
		virtual void run() override;

	private:
		std::thread m_Thread;
		//!Runメソッドで呼ばれる関数ポインタ
		std::shared_ptr<IRunnable> m_RunFunc = nullptr;
	};
}