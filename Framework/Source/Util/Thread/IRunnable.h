#pragma once

/**
* @file	IRunnable.h
* @brief	スレッド用インターフェース
* @authro	高須優輝
* @date	2017/03/18
*/

namespace util {

	class IRunnable
	{
	public:
		virtual ~IRunnable() {};

		virtual void run() = 0;
	};
}