#pragma once

/**
* @file	IRunnable.h
* @brief	�X���b�h�p�C���^�[�t�F�[�X
* @authro	���{�D�P
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