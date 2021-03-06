#pragma once
/**
* @file	    Screen.h
* @brief	ウィンドウのサイズ
* @dital	フレームワークでの画像サイズ取得用
* @authro	高須優輝
* @date	2017/3/19
*/



namespace framework {
	struct Screen
	{
		//!ウィンドウの幅
		static int WINDOW_WIDTH;
		//!ウィンドウの高さ
		static int WINDOW_HEIGHT;
		//!ウィンドウの幅の半分
		static int WINDOW_WIDTH_HALF;
		//!ウィンドウの高さの半分
		static int WINDOW_HEIGHT_HALF;
		//!視野角
		static float PERSPECTIVE;
		//!描画距離
		static float FAR_;

		//!UIの表示領域の幅
		static int UI_WIDTH;
		//!UIの表示領域の幅
		static int UI_HEIGHT;


		//!解像度の幅
		static int PIXEL_WIDTH;
		//!解像度の高さ
		static int PIXEL_HEIGHT;
		//!解像度のの幅の半分
		static int PIXEL_WIDTH_HALF;
		//!解像度の高さの半分
		static int PIXEL_HEIGHT_HALF;
	};
}