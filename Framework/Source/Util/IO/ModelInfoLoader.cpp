#include"ModelInfoLoader.h"
#include<assert.h>
#include<iterator>
#include<algorithm>

namespace util {

	ModelInfoLoader::ModelInfoLoader(const std::string& fileName)
		:m_Loader(fileName),
		tyep("")
	{
		auto data = m_Loader.load();

		//csv�̐������폜
		data.erase(data.begin());

		assert(!data.empty() && "ModelInfo���s���ł�");

		m_Data = data[0];

		//���f���̖��O
		m_ModelName = data[0][0];

		//���f���̖��O�����X�g����폜
		data[0].erase(data[0].begin());
		//�c�����e�N�X�`���̖��O�����ׂăR�s�[
		m_TextureName = data[0];

		//�Ă�������̖����f�[�^�폜
		m_TextureName.erase(m_TextureName.begin());

		//�p�����[�^�ǂݍ���
		paramLoad(data);
		//���[�V�����p�X�ǂݍ���
		motionLoad(data);
		//���f���^�C�v�ǂݍ���
		typeLoad(data);

		std::copy(m_MotionPath.begin(), m_MotionPath.end(),std::back_inserter(m_Data));
	}

	ModelInfoLoader::~ModelInfoLoader()
	{
	}

	std::string ModelInfoLoader::getModelNameToFBX()
	{
		return m_ModelName + ".fbx";
	}

	std::string ModelInfoLoader::getModelName()
	{
		return m_ModelName;
	}

	std::string ModelInfoLoader::getTextureNameToPng(int location)
	{
		assert(m_TextureName.size() > location && "�e�N�X�`���̐��𒴂��Ă��܂�");
		return m_TextureName[location] + ".png";
	}

	std::string ModelInfoLoader::getTextureName(int location)
	{
		assert(m_TextureName.size() > location && "�e�N�X�`���̐��𒴂��Ă��܂�");
		return m_TextureName[location];
	}

	int ModelInfoLoader::getTextureNum()
	{
		return m_TextureName.size();
	}

	std::vector<std::string> ModelInfoLoader::getVectorResource()
	{
		return m_Data;
	}

	framework::ModelType ModelInfoLoader::getType()
	{
		//����������Ă��Ȃ���΍��̂Ƃ��Đ���
		if (tyep == "") return framework::ModelType::rigid;

		if (tyep == "rigid")return framework::ModelType::rigid;

		if (tyep == "skin")return framework::ModelType::skin;

		//�ǂ�ɂ����Ă͂܂�Ȃ���΍��̂Ƃ��Đ���
		return framework::ModelType::rigid;
	}

	std::vector<std::string> ModelInfoLoader::getMotionPath()
	{
		return m_MotionPath;
	}

	void ModelInfoLoader::paramLoad(std::vector<std::vector<std::string>>& param)
	{
		if (param.size() < 4) return;
		if (param[3].size() == 0)return;
		if (param[3].empty())return;

		std::vector<std::string> paramData;
		paramData.emplace_back(param[2][0]);
		paramData.emplace_back(param[2][1]);
		paramData.emplace_back(param[2][2]);

		//�f�[�^������
		std::copy(paramData.begin(), paramData.end(), std::back_inserter(m_Data));
	}

	void ModelInfoLoader::typeLoad(std::vector<std::vector<std::string>>& param)
	{
		//�T�C�Y��
		if (param.size() < 4) return;
		if (param[3].empty())return;

		tyep = param[3][0];
	}

	void ModelInfoLoader::motionLoad(std::vector<std::vector<std::string>>& param)
	{
		if (param.size() < 2) return;
		if (param[1].size() == 0)return;
		if (param[1].empty())return;

		for (auto path : param[1]) {
			if (path == "")break;
			m_MotionPath.emplace_back(path);
		}
	}

}