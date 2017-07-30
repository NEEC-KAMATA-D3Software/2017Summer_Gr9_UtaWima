#pragma once
#include"AntWeak.h"
#include<string>

namespace util {

	class ColorPalette
	{
	public:
		ColorPalette(const std::string& title);
		ColorPalette(const std::string& title, util::Vec2& size);
		~ColorPalette();

		/**
		* @brief		UI�̃J���[��ύX
		*/
		void setUIColor(util::Vec4 rgba);

		/**
		* @brief		rgba��ݒ�
		*/
		void setValue(util::Vec4 rgba);

		/**
		* @brief		�J���[�擾
		*/
		util::Vec3 getValue();

		/**
		* @brief		rgba��ݒ�
		*/
		void draw();

	private:
		util::AntWeak m_AntWeak;
		//!�\�����镶����
		std::string m_String;
	};



}