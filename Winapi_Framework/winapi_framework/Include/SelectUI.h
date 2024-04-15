#pragma once
#include "StaticObj.h"
#include "Player.h"
#include "ThreadManager.h"
#include "Core.h"
#include <string>

class SelectUI :
    public CStaticObj
{
public:
	SelectUI() {}
	SelectUI(const SelectUI* obj) {}
	~SelectUI() {}
	virtual SelectUI* Clone() {
		return new SelectUI(*this);
	}
	virtual bool Init() { return true; }
	virtual void Input(float fDeltaTime) {
		if (CInput::GetInst()->KeyDown("ThreadCount")) {
			int thread_count = ThreadManager::GetInst()->getThreadCount();
			if (thread_count >= 16) {
				thread_count = 1;
			}
			ThreadManager::GetInst()->setThreadCount(thread_count * 2);
		}
		if (CInput::GetInst()->KeyDown("Parallel")) {
			CCore::GetInst()->toggleParallel();
		}
		if (CInput::GetInst()->KeyDown("Algorithm")) {
			CCore::GetInst()->nextAlg();
		}
		if (CInput::GetInst()->KeyDown("MapSize")) {
			CCore::GetInst()->nextMapSize();
		}
		if (CInput::GetInst()->KeyDown("Reset")) {
			CSceneManager::GetInst()->CreateScene<CInGameScene>(SCENE_CREATE::SC_NEXT);
		}
	}

	virtual void Render(HDC hDC, float fDeltaTime)
	{
		auto pos = Player::GetInst()->GetPos();

		wchar_t buffer1[20];
		swprintf_s(buffer1, L"Thread(1): %d", ThreadManager::GetInst()->getThreadCount());
		TextOut(hDC, pos.x - 340, pos.y - 320, (LPCWSTR)buffer1, wcslen(buffer1));

		const wchar_t* buffer2 = (CCore::GetInst()->IsParallel() ? L"Mode(2): Parallel" : L"Mode(2): Serial");
		TextOut(hDC, pos.x - 340, pos.y - 300, (LPCWSTR)buffer2, wcslen(buffer2));
		
		const wchar_t* buffer3 = toString(CCore::GetInst()->getAlg());
		TextOut(hDC, pos.x - 340, pos.y - 280, (LPCWSTR)buffer3, wcslen(buffer3));

		wchar_t buffer4[20];
		swprintf_s(buffer4, L"MapSize(4): %d", CCore::GetInst()->getMapSize());
		TextOut(hDC, pos.x - 340, pos.y - 260, (LPCWSTR)buffer4, wcslen(buffer4));
	}
private:
	const wchar_t* toString(ALGORITHM_TYPE at) {
		switch (at)
		{
		case ALGORITHM_TYPE::NONE:
			return L"Alg(3): NONE";
		case ALGORITHM_TYPE::PERLIN:
			return L"Alg(3): PERLIN";
		case ALGORITHM_TYPE::CELLULAR_KEY:
			return L"Alg(3): CELLULAR_KEY";
		case ALGORITHM_TYPE::CELLULAR_UPDATE:
			return L"Alg(3): CELLULAR_UPDATE";
		case ALGORITHM_TYPE::MERGE:
			return L"Alg(3): MERGE";
		case ALGORITHM_TYPE::GAME:
			return L"Alg(3): GAME";
		default:
			break;
		}
		return NULL;
	}
};

