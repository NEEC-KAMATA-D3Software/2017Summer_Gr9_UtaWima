#pragma once
#include<Windows.h>
#include<Source\Util\Timer\FPSTimer.h>
/**
* @file	    Application.h
* @brief	アプリケーションの基本クラス
* @authro	高須優輝
* @date	2017/2/20
*/

namespace framework {

	void loadMaterial();

	class Application
	{
	public:
		Application(HINSTANCE instance);
		~Application();

		virtual void init();

		int run();


		static bool m_Exit;
	private:
		/**
		* @brief	アップデートコールバック
		*/
		virtual void update() = 0;

		/**
		* @brief	描画コールバック
		*/
		virtual void draw() = 0;

	private:
		/**
		* @brief			メッセージ処理
		* @param msg		メッセージ取得格納するための変数
		* @return			メッセージ取得に成功したかどうか
		*/
		bool messageProcess(MSG* msg);

	protected:
		/**
		* @brief			フレームワーク内で利用する必須リソースを読み込む
		*/
		void importMaterial();

	protected:
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		util::FPSTimer m_FpsTimer;
	};

}

