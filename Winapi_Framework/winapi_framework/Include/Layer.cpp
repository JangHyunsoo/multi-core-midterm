#include "Layer.h"

CLayer::CLayer() : 
	m_iZOrder(0),
	m_strTag(""),
	m_pScene(NULL),
	m_bEnable(true),
	m_bLife(true)
{
}

CLayer::~CLayer()
{
	auto iterEnd = m_ObjList.end();

	for (auto iter = m_ObjList.begin(); iter != iterEnd; ++iter) {
		CObj::EraseObj(*iter);
		SAFE_RELEASE((*iter));
	}
	m_ObjList.clear();
}

void CLayer::AddObject(CObj* pObj)
{
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	pObj->AddRef();

	m_ObjList.push_back(pObj);
}

void CLayer::Input(float fDeltaTime)
{
	auto iterEnd = m_ObjList.end();

	for (auto iter = m_ObjList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->Input(fDeltaTime);

		if (!(*iter)->GetLife()) {
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else {
			++iter;
		}
	}
}

int CLayer::Update(float fDeltaTime)
{
	auto iterEnd = m_ObjList.end();

	for (auto iter = m_ObjList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLife()) {
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else {
			++iter;
		}
	}
	return 0;
}

int CLayer::LateUpdate(float fDeltaTime)
{
	auto iterEnd = m_ObjList.end();

	for (auto iter = m_ObjList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fDeltaTime);

		if (!(*iter)->GetLife()) {
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else {
			++iter;
		}
	}
	return 0;
}

void CLayer::Collision(float fDeltaTime)
{
	auto iterEnd = m_ObjList.end();

	for (auto iter = m_ObjList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->Collision(fDeltaTime);

		if (!(*iter)->GetLife()) {
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else {
			++iter;
		}
	}
}

void CLayer::Render(HDC hDC, float fDeltaTime)
{
	auto iterEnd = m_ObjList.end();

	for (auto iter = m_ObjList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->Render(hDC, fDeltaTime);

		if (!(*iter)->GetLife()) {
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else {
			++iter;
		}
	}
}
