#include "Scene.h"
#include "Layer.h"

CScene::CScene()
{
	CLayer* pLayer = CreateLayer("Default", 0);
	pLayer = CreateLayer("UI", INT_MAX);
}

CScene::~CScene()
{
	Safe_Delete_VecList(m_LayerList);
}

CLayer* CScene::CreateLayer(const string& strTag, int iZOrder)
{
	CLayer* pLayer = new CLayer;

	pLayer->SetTag(strTag);
	pLayer->SetZOrder(iZOrder);
	pLayer->SetScene(this);

	m_LayerList.push_back(pLayer);

	if (m_LayerList.size() >= 2) {
		m_LayerList.sort(CScene::LayerSort);
	}

	return pLayer;
}

CLayer* CScene::FindLayer(const string& strTag)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter) {
		if ((*iter)->GetTag() == strTag) 
			return *iter;
	}

	return NULL;
}

bool CScene::Init(HDC hDC)
{
	return true;
}

void CScene::Input(float fDeltaTime)
{
	auto iterEnd = m_LayerList.end();

	for (auto iter = m_LayerList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->Input(fDeltaTime);

		if (!(*iter)->GetLife()) {
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else {
			++iter;
		}
	}
}

int CScene::Update(float fDeltaTime)
{
	auto iterEnd = m_LayerList.end();

	for (auto iter = m_LayerList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLife()) {
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else {
			++iter;
		}
	}

	return 0;
}

int CScene::LateUpdate(float fDeltaTime)
{
	auto iterEnd = m_LayerList.end();

	for (auto iter = m_LayerList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fDeltaTime);

		if (!(*iter)->GetLife()) {
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else {
			++iter;
		}
	}
	return 0;
}

void CScene::Collision(float fDeltaTime)
{
	auto iterEnd = m_LayerList.end();

	for (auto iter = m_LayerList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->Collision(fDeltaTime);

		if (!(*iter)->GetLife()) {
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else {
			++iter;
		}
	}
}

void CScene::Render(HDC hDC, float fDeltaTime)
{
	auto iterEnd = m_LayerList.end();

	for (auto iter = m_LayerList.begin(); iter != iterEnd;) {
		if (!(*iter)->GetEnable()) {
			++iter;
			continue;
		}

		(*iter)->Render(hDC, fDeltaTime);

		if (!(*iter)->GetLife()) {
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else {
			++iter;
		}
	}
}

bool CScene::LayerSort(CLayer* A, CLayer* B)
{
	return A->GetZOrder() < B->GetZOrder();
}
