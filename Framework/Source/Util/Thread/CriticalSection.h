#pragma once
#include <mutex>


/**
* @file	CriticalSection.h
* @brief	スレッドに資源を専有させるためのクラス
* @authro	高須優輝
* @date	2017/03/29
*/

namespace util {

	class CriticalSection
	{
	public:
		CriticalSection() {
			mtx.lock();
		};
		~CriticalSection() {
			mtx.unlock();
		};

	private:
		std::mutex mtx;
	};

}