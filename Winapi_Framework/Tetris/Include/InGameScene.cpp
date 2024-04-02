#include "InGameScene.h"
#include "Map.h"

CInGameScene::CInGameScene()
{
}

CInGameScene::~CInGameScene()
{
}

bool CInGameScene::Init()
{
	if (!CScene::Init())
		return false;

	CLayer*  pLayer = FindLayer("Default");
	CMap* pMap = new CMap(100, 100);
	pLayer->AddObject(pMap);

	return true;
}