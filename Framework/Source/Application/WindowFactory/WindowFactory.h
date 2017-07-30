#pragma once
#include<Windows.h>
#include<string>
#include<Source\Util\Type.h>
#include"../../Util/Type.h"

namespace framework {

	using Proc = LRESULT (CALLBACK *)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	class WindowFactory
	{
	public:
		WindowFactory(HINSTANCE hInstance);
		~WindowFactory();

		HWND createWnd(LPCSTR className, LPCSTR title, util::Vec2 size);
		HWND createWnd(LPCSTR className, LPCSTR title, util::Vec2 pos, util::Vec2 size);

		//現在の設定でクラスを登録する
		void regist();

		//タイトル名セット
		//TEXTマクロを使って利用するとよい
		void setClassName(LPCSTR className);

		void setProc(Proc proc);

		void setStyle(UINT style);

		void setMenuName(LPSTR menuName);

		void setMenuName(UINT nemeName);

		//アイコンセット
		void setIcon();

		//カーソルをセット
		void setCursor();

		//ウィンドウクライアント領域取得
		util::Vec2 getWindowSize();

	private:
		/**
		* @brief		ウィンドウの幅を考慮したクライアント領域の計算
		*/
		util::Vec2 clientSize(HWND hwnd,util::Vec2& size);

	private:
		WNDCLASS m_WndClass;
	};

}