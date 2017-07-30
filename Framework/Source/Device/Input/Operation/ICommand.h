#pragma once

/**
* @file	Command.h
* @brief	�R�}���h�C���^�[�t�F�[�X
* @authro	���{�D�P
* @date	2017/04/14
*/

namespace framework {

	class ICommand
	{
	public:
		~ICommand() {}

		virtual void execute() = 0;

	private:

	};
}