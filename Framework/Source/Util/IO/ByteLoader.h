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
			//�ǂݍ��ݐ�p���o�C�i���ǂݍ��ݎw��
			:fin(filePath, std::ios::in)
		{
			//�J���Ȃ���Όx�����o��
			std::string&& error = filePath + "���J���܂���";
			if (!fin) {
				WinFunc::OutLog(std::move(error));
				assert(false && "�o�C�g�R�[�h���J���܂���");
			}
		}


		/**
		* @brief				���ׂẴt�@�C����ǂݍ���
		* @return				�o�C�g�R�[�hlist
		*/
		std::unique_ptr<unsigned char> allLoad(int *size) {
		
			using namespace std;
			//���݈ʒu��24�o�C�g���炷
			fin.seekg(24);
			
			//!3�o�C�g
			unsigned long sizeByte[2] = {};
			char c;
			//���炩���߂Q�ǂ�
			fin.get(c);
			sizeByte[0] = static_cast<unsigned long>(c);
			fin.get(c);
			sizeByte[1] = static_cast<unsigned long>(c);

			fin.get(c);

			const int byte3 = 16;
			const int byte120 = byte3 * 4;
			unsigned long bufSize = static_cast<unsigned long>(c) * byte120 + sizeByte[1] * byte3 * byte3 + sizeByte[0];
			//�擪�ɖ߂��Ă���
			fin.seekg(0,ios::beg);

			return nullptr;
		}

		/**
		* @brief				���ׂẴt�@�C����ǂݍ���
		* @return				�o�C�g�R�[�hvector
		*/
		std::vector<double> allLoadV() {

			std::vector<double> result;
			//�t�@�C���̍Ō�܂œǂݍ���
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
			//�Ƃ���
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
			assert(fp != NULL && "�ǂݍ��ݎ��s");
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