#include "InGameScene.h"
#include "Map.h"
#include "PerlinNoiseMap.h"
#include "CellularMap.h"
#include "FpsUI.h"
#include "Player.h"

CInGameScene::CInGameScene() : CScene()
{
	CreateLayer("Object", 1);
}

bool CInGameScene::Init(HDC hDC)
{
	if (!CScene::Init(hDC))
		return false;

	CLayer* pLayer = FindLayer("Object");
	Player* pPlayerObj = Player::GetInst();
	pPlayerObj->Init();
	pLayer->AddObject(pPlayerObj);

	pLayer = FindLayer("Default");
	CMap* pMap = new PerlinNoiseMap(hDC, 1000, 1000, 100);
	pMap->Init();
	pLayer->AddObject(pMap);

	pLayer = FindLayer("UI");
	CFpsUI* pFpsUI = new CFpsUI();
	pLayer->AddObject(pFpsUI);

	return true;
}