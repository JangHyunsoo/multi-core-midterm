#include "InGameScene.h"
#include "Map.h"
#include "PerlinNoiseMap.h"
#include "CellularMap.h"
#include "FpsUI.h"
#include "Player.h"
#include "Core.h"
#include "SelectUI.h"
#include "MergeMap.h"
#include "KeyCellularMap.h"
#include "UpdateCellularMap.h"
#include "CellularGameMap.h"

CInGameScene::CInGameScene() : CScene()
{
	CreateLayer("Object", 1);
}

bool CInGameScene::Init()
{
	if (!CScene::Init())
		return false;


	auto alg = CCore::GetInst()->getAlg();

	CLayer* pLayer = FindLayer("Object");
	Player* pPlayerObj = Player::GetInst();
	pPlayerObj->Init();
	pLayer->AddObject(pPlayerObj);

	if (alg != ALGORITHM_TYPE::NONE) {
		pLayer = FindLayer("Default");
		CMap* pMap = nullptr;
		int map_size = CCore::GetInst()->getMapSize();
		switch (alg)
		{
		case ALGORITHM_TYPE::PERLIN:
			pMap = new PerlinNoiseMap(CCore::GetInst()->getDC(), map_size, map_size, CCore::GetInst()->IsParallel());
			break;
		case ALGORITHM_TYPE::CELLULAR_KEY:
			pMap = new KeyCellularMap(CCore::GetInst()->getDC(), map_size, map_size, CCore::GetInst()->IsParallel());
			break;
		case ALGORITHM_TYPE::CELLULAR_UPDATE:
			pMap = new UpdateCellularMap(CCore::GetInst()->getDC(), map_size, map_size, CCore::GetInst()->IsParallel());
			break;
		case ALGORITHM_TYPE::MERGE:
			pMap = new MergeMap(CCore::GetInst()->getDC(), map_size, map_size, CCore::GetInst()->IsParallel());
			break;
		case ALGORITHM_TYPE::GAME:
			pMap = new CellularGameMap(CCore::GetInst()->getDC(), map_size, map_size, CCore::GetInst()->IsParallel());
			break;
		default:
			break;
		}
		pMap->Init();
		pLayer->AddObject(pMap);
	}
	
	pLayer = FindLayer("UI");
	CFpsUI* pFpsUI = new CFpsUI();
	pLayer->AddObject(pFpsUI);

	SelectUI* pSelectUI = new SelectUI();
	pLayer->AddObject(pSelectUI);

	return true;
}