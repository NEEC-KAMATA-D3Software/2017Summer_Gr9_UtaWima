#include"ColorPalette.h"

namespace util {
	ColorPalette::ColorPalette(const std::string & title)
		:ColorPalette(title,util::Vec2(500,500))
	{
	}
	ColorPalette::ColorPalette(const std::string& title ,util::Vec2& size)
		:m_AntWeak(title,size)
		, m_String("Color")
	{
		//!ARGB
		std::vector<int> uiColor = { 150,0,0,0};
		m_AntWeak.setParam(m_String.c_str(), uiColor, ETwParamValueType::TW_PARAM_INT32);
		std::vector<int> value = { 0};
		m_AntWeak.addVarRw(m_String.c_str(), value,TwType::TW_TYPE_COLOR32);
	}

	ColorPalette::~ColorPalette()
	{
	}

	void ColorPalette::setUIColor(util::Vec4 rgba)
	{
		//!ARGB
		std::vector<float> test = { rgba.w,rgba.x,rgba.y,rgba.z };
		m_AntWeak.setParam(m_String.c_str(), test, ETwParamValueType::TW_PARAM_INT32);
	}

	void ColorPalette::setValue(util::Vec4 rgba)
	{
	}

	util::Vec3 ColorPalette::getValue()
	{
		return util::Vec3();
	}

	void ColorPalette::draw()
	{
		m_AntWeak.draw();
	}

}