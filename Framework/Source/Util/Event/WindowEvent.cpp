#include"WindowEvent.h"

namespace util {

	std::shared_ptr<WindowEvent> WindowEvent::instance = std::shared_ptr<WindowEvent>(new WindowEvent());

}