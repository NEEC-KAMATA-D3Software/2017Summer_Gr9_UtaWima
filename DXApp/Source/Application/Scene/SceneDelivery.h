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

	//!先に入っているデータほどランキングが高い
	std::vector<int> m_RankData;
};

