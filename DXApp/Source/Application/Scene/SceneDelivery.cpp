#include"SceneDelivery.h"


std::unique_ptr<SceneDelivery> SceneDelivery::m_pInstance = std::unique_ptr<SceneDelivery>(new SceneDelivery());

SceneDelivery::SceneDelivery()
{
}

SceneDelivery::~SceneDelivery()
{
}

void SceneDelivery::addPlayer(int playerID)
{
	//æ“ª‚É’Ç‰Á‚µ‚Ä‚¢‚­
	m_RankData.insert(m_RankData.begin(), playerID);
}

void SceneDelivery::clear()
{
	m_RankData.clear();
}

std::vector<int> SceneDelivery::getData()
{
	return m_RankData;
}

SceneDelivery * SceneDelivery::getInstance()
{
	return m_pInstance.get();
}
