#include"AntWeak.h"
#include<AntTweakBar.h>
#include<Source\Util\Render\DXFunc.h>
#include<assert.h>
#include<Source\Application\Screen\Screen.h>

namespace util {

	AntWeak::AntWeak(const std::string & title)
		:AntWeak(title,util::Vec2(256,256))
	{
	}

	AntWeak::AntWeak(const std::string& title, util::Vec2& size)
	{
		m_BarInstance = TwNewBar(title.c_str());
		int barSize[2] = { size.x, size.y };
		TwSetParam(m_BarInstance, NULL, "size", TW_PARAM_INT32, 2, barSize);
	}

	AntWeak::~AntWeak()
	{
		TwDeleteBar(m_BarInstance);
	}

	void AntWeak::apiInit(TwGraphAPI module)
	{
		TwWindowSize(framework::Screen::WINDOW_WIDTH, framework::Screen::WINDOW_HEIGHT);
		assert(TwInit(module, util::getDevice()) && "TwÇÃèâä˙âªÇ…é∏îsÇµÇ‹ÇµÇΩ");
	}

	void AntWeak::apiEnd()
	{
		TwTerminate();
	}

	void AntWeak::setLabel(const std::string & title)
	{
		TwDefine(title.c_str());
	}

	void AntWeak::draw()
	{
		TwDraw();
	}

}
