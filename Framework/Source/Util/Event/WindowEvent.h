#pragma once

#include"../Template/Event.h"
#include<memory>
#include<Windows.h>
#include<../DXApp/resource.h>

//windows�̃��j���[�p�̃C�x���g

namespace util {

	class WindowEvent : public util::Event<UINT>
	{
	public:
		~WindowEvent() {

		}

		static WindowEvent* getInstance() {
			return instance.get();
		}

	private:
		WindowEvent() {
		}

	private:
		static std::shared_ptr<WindowEvent> instance;

	};
}
