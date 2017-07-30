#include "Thread.h"

namespace util {
	Thread::Thread()
		:m_Thread()
	{
	}

	Thread::Thread(std::shared_ptr<IRunnable> runnbale)
		: m_RunFunc(runnbale)
	{
	}

	Thread::Thread(std::function<void()> action)
	{
		m_RunFunc = std::make_shared<ThreadAction>(action);
	}

	Thread::~Thread()
	{
	}

	void Thread::start()
	{
		m_Thread = std::thread(&Thread::run,this);
	}

	void Thread::join()
	{
		m_Thread.join();
	}

	void Thread::detach()
	{
		m_Thread.detach();
	}

	void Thread::sleep(unsigned int millsecond)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(millsecond));
	}

	std::thread::id Thread::getId() const
	{
		return m_Thread.get_id();
	}

	void Thread::run()
	{
		//コストラクタで初期化されてなければ何もしない
		if (m_RunFunc == nullptr)return;
		m_RunFunc->run();
	}


	ThreadAction::ThreadAction(std::function<void()> action)
		:m_Action(action)
	{
	}

	ThreadAction::~ThreadAction()
	{
	}

	void ThreadAction::run()
	{
		m_Action();
	}

}


