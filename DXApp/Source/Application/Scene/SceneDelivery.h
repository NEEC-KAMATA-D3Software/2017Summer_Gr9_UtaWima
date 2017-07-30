#pragma once
#include<memory>
#include<vector>

class SceneDelivery
{
public:
	~SceneDelivery();


	void addPlayer(int playerID);

	void clear();

	std::vector<int> getData();

	static SceneDelivery* getInstance();

private:
	SceneDelivery();

private:
	static std::unique_ptr<SceneDelivery> m_pInstance;

	//!��ɓ����Ă���f�[�^�قǃ����L���O������
	std::vector<int> m_RankData;
};

