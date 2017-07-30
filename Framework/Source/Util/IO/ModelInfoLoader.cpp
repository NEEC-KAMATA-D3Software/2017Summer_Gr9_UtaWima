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

		//csvの説明欄削除
		data.erase(data.begin());

		assert(!data.empty() && "ModelInfoが不正です");

		m_Data = data[0];

		//モデルの名前
		m_ModelName = data[0][0];

		//モデルの名前をリストから削除
		data[0].erase(data[0].begin());
		//残ったテクスチャの名前をすべてコピー
		m_TextureName = data[0];

		//てくすちゃの枚数データ削除
		m_TextureName.erase(m_TextureName.begin());

		//パラメータ読み込み
		paramLoad(data);
		//モーションパス読み込み
		motionLoad(data);
		//モデルタイプ読み込み
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
		assert(m_TextureName.size() > location && "テクスチャの数を超えています");
		return m_TextureName[location] + ".png";
	}

	std::string ModelInfoLoader::getTextureName(int location)
	{
		assert(m_TextureName.size() > location && "テクスチャの数を超えています");
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
		//何も書かれていなければ剛体として生成
		if (tyep == "") return framework::ModelType::rigid;

		if (tyep == "rigid")return framework::ModelType::rigid;

		if (tyep == "skin")return framework::ModelType::skin;

		//どれにも当てはまらなければ剛体として生成
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

		//データを結合
		std::copy(paramData.begin(), paramData.end(), std::back_inserter(m_Data));
	}

	void ModelInfoLoader::typeLoad(std::vector<std::vector<std::string>>& param)
	{
		//サイズが
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