#include "InGameScene.h"
#include "Map.h"
#include "FpsUI.h"

CInGameScene::CInGameScene()
{
}

CInGameScene::~CInGameScene()
{
}

bool CInGameScene::Init(HDC hDC)
{
	if (!CScene::Init(hDC))
		return false;

	CLayer*  pLayer = FindLayer("Default");
	CMap* pMap = new CMap(hDC, 100, 100);
	pMap->Init();
	pLayer->AddObject(pMap);

	pLayer = FindLayer("UI");
	CFpsUI* pFpsUI = new CFpsUI();
	pLayer->AddObject(pFpsUI);

	return true;
}