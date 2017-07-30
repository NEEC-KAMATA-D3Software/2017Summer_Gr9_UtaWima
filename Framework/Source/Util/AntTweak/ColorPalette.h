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
		* @brief		UIのカラーを変更
		*/
		void setUIColor(util::Vec4 rgba);

		/**
		* @brief		rgbaを設定
		*/
		void setValue(util::Vec4 rgba);

		/**
		* @brief		カラー取得
		*/
		util::Vec3 getValue();

		/**
		* @brief		rgbaを設定
		*/
		void draw();

	private:
		util::AntWeak m_AntWeak;
		//!表示する文字列
		std::string m_String;
	};



}