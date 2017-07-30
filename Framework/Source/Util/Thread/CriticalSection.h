#pragma once
#include <mutex>


/**
* @file	CriticalSection.h
* @brief	�X���b�h�Ɏ������L�����邽�߂̃N���X
* @authro	���{�D�P
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