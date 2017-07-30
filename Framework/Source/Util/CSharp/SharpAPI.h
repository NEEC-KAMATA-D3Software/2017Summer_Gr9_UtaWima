#pragma once


namespace util {

	class SharpAPI
	{
	public:
		/**
		* @brief		API初期化
		*/
		static void initAPI();

		/**
		* @brief		API終了処理
		*/
		static void endAPI();

	private:
		//インスタンス生成禁止
		SharpAPI();
	};
}