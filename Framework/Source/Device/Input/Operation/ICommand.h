#pragma once

/**
* @file	Command.h
* @brief	コマンドインターフェース
* @authro	高須優輝
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