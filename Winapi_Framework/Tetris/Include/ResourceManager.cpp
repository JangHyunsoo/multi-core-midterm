#include "ResourceManager.h"
#include "Texture.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager() {

}

CResourceManager::~CResourceManager() {
	SAFE_RELEASE(m_pBackBuffer);
	Safe_Release_Map(m_mapTexture);
}

CTexture* CResourceManager::GetBackBuffer() const
{
	m_pBackBuffer->AddRef();
	return m_pBackBuffer;
}

bool CResourceManager::Init(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;

	m_pBackBuffer = LoadTexture("BackBuffer", L"BackBuffer.bmp");

	return true;
}

CTexture* CResourceManager::LoadTexture(const string& strKey, const wchar_t* pFileName, const string& strPathKey)
{
	CTexture* pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, pFileName, strPathKey)) {
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

CTexture* CResourceManager::FindTexture(const string& strKey)
{
	unordered_map<string, class CTexture*>::iterator iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end()) return NULL;

	iter->second->AddRef();

	return iter->second;
}
