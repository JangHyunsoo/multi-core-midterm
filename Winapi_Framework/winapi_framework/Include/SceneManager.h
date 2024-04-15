#pragma once

#include "Game.h"

class CSceneManager
{
private:
	class CScene* m_pScene;
	class CScene* m_pNextScene;
public:
	CScene* getScene() {
		return m_pScene;
	}
public:
	bool Init(HDC hDC);
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void Render(HDC hDC, float fDeltaTime);
public:
	template<typename T>
	T* CreateScene(HDC hDC, SCENE_CREATE sc) { // sc : next, current
		T* pScene = new T;

		if (!pScene->Init(hDC)) {
			SAFE_DELETE(pScene);
			return NULL;
		}

		switch (sc)
		{
		case SCENE_CREATE::SC_CURRENT:
			SAFE_DELETE(m_pScene);
			m_pScene = pScene;
			break;
		case SCENE_CREATE::SC_NEXT:
			SAFE_DELETE(m_pNextScene);
			m_pNextScene = pScene;
			break;
		default:
			break;
		}

		return pScene;
	}

private:
	void loadNextScene() {
		SAFE_DELETE(m_pScene);
		m_pScene = m_pNextScene;
		m_pNextScene = nullptr;
	}

	DECLARE_SINGLE(CSceneManager)
};

