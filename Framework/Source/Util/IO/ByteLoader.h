#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<assert.h>
#include"../Win/WinFunc.h"
#include<list>
#include<vector>
#include<memory>

namespace util {

	class ByteLoader
	{
	public:
		ByteLoader(const std::string& filePath)
			//読み込み専用かつバイナリ読み込み指定
			:fin(filePath, std::ios::in)
		{
			//開けなければ警告を出す
			std::string&& error = filePath + "が開けません";
			if (!fin) {
				WinFunc::OutLog(std::move(error));
				assert(false && "バイトコードが開けません");
			}
		}


		/**
		* @brief				すべてのファイルを読み込む
		* @return				バイトコードlist
		*/
		std::unique_ptr<unsigned char> allLoad(int *size) {
		
			using namespace std;
			//現在位置を24バイトずらす
			fin.seekg(24);
			
			//!3バイト
			unsigned long sizeByte[2] = {};
			char c;
			//あらかじめ２つ読み
			fin.get(c);
			sizeByte[0] = static_cast<unsigned long>(c);
			fin.get(c);
			sizeByte[1] = static_cast<unsigned long>(c);

			fin.get(c);

			const int byte3 = 16;
			const int byte120 = byte3 * 4;
			unsigned long bufSize = static_cast<unsigned long>(c) * byte120 + sizeByte[1] * byte3 * byte3 + sizeByte[0];
			//先頭に戻しておく
			fin.seekg(0,ios::beg);

			return nullptr;
		}

		/**
		* @brief				すべてのファイルを読み込む
		* @return				バイトコードvector
		*/
		std::vector<double> allLoadV() {

			std::vector<double> result;
			//ファイルの最後まで読み込み
			while (!fin.eof())
			{
				double d;
				fin.read((char*)(&d), sizeof(double));
				result.emplace_back(d);
				WinFunc::OutLog(std::to_string(d));
			}

			return result;
		}

		~ByteLoader() {
			//とじる
			fin.close();
		}

	private:
		std::ifstream fin;
	};

	class Byte
	{
	public:
		Byte(const std::string& filePath) {
			_tfopen_s(&fp,filePath.c_str(),"rb");
			assert(fp != NULL && "読み込み失敗");
		}
		~Byte() {
			fclose(fp);
		}

		void test() {

		}

		std::unique_ptr<unsigned char> allLoad(int* size) {
			fseek(fp,24,SEEK_CUR);
			unsigned long sizeByte[2] = { (unsigned long)getc(fp), '0' };
			sizeByte[1] = (unsigned long)getc(fp);
			unsigned long bufSize = (unsigned long)getc(fp) * 16 * 16 * 16 * 16 + sizeByte[1] * 16 * 16 + sizeByte[0];
			fseek(fp, 0, SEEK_SET);

			std::unique_ptr<unsigned char> bufptr(new unsigned char[bufSize]);
			fread_s(bufptr.get(), bufSize, sizeof(unsigned char), bufSize, fp);

			*size = bufSize;
			return  bufptr;
		}

	private:
		FILE* fp;
	};

}