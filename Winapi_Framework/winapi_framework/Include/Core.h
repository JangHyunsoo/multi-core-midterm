#pragma once
#include "Game.h"

class CCore
{
	DECLARE_SINGLE(CCore)
private:
	static bool m_bLoop;
private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HDC m_hDC;
	RESOLUTION m_tRS;
	const wchar_t* m_strName;
	bool m_bParallel;
	int m_iMapSize = 500;
	ALGORITHM_TYPE m_eAlg = ALGORITHM_TYPE::NONE;
public:
	bool Init(HINSTANCE hInst, const wchar_t* strName, int iWidth, int iHeight, int iThreadCount = 4, bool parallel = true);
	int Run();
	void toggleParallel() {
		m_bParallel = !m_bParallel;
	}
	bool IsParallel() {
		return m_bParallel;
	}
	void nextMapSize() {
		if (m_iMapSize > 1000) {
			m_iMapSize = 100;
		}
		m_iMapSize += 100;
	}
	int getMapSize() {
		return m_iMapSize;
	}
	ALGORITHM_TYPE getAlg() {
		return m_eAlg;
	}
	void nextAlg() {
		switch (m_eAlg)
		{
		case ALGORITHM_TYPE::NONE:
			m_eAlg = ALGORITHM_TYPE::PERLIN;
			m_iMapSize = 500;
			break;
		case ALGORITHM_TYPE::PERLIN:
			m_eAlg = ALGORITHM_TYPE::CELLULAR_KEY;
			m_iMapSize = 500;
			break;
		case ALGORITHM_TYPE::CELLULAR_KEY:
			m_eAlg = ALGORITHM_TYPE::CELLULAR_UPDATE;
			m_iMapSize = 300;
			break;
		case ALGORITHM_TYPE::CELLULAR_UPDATE:
			m_eAlg = ALGORITHM_TYPE::MERGE;
			m_iMapSize = 500;
			break;
		case ALGORITHM_TYPE::MERGE:
			m_eAlg = ALGORITHM_TYPE::GAME;
			m_iMapSize = 300;
			break;
		case ALGORITHM_TYPE::GAME:
			m_eAlg = ALGORITHM_TYPE::NONE;
			m_iMapSize = 300;
		default:
			break;
		}
	}
	HWND getWND() {
		return m_hWnd;
	}
	HDC getDC() {
		return m_hDC;
	}
private:
	void Logic();
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void Render(float fDeltaTime);

private:
	ATOM MyRegisterClass();
	BOOL Create();
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};