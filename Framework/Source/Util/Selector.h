#pragma once



namespace util {

	class Selector
	{
	public:
		/**
		* @brief			コンストラクタ
		* @param selectNum  セレクト数
		*/
		Selector(const int selectNum);
		~Selector();

		/**
		* @brief		初期化
		*/
		void init();
		
		/**
		* @brief		次に進める
		*/
		void enter();

		/**
		* @brief		前に戻す
		*/
		void back();

		/**
		* @brief		現在のセレクトを取得
		*/
		const int getSelectNum();

	private:
		//!セレクト数
		const int m_SelectNum;
		//!セレクトをバックさせるときに使うオフセット
		const int m_DownOffst;
		//!現在のセレクト
		int m_CurrentSelect;
	};
}