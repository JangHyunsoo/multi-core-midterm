#include "Texture.h"
#include "PathManager.h"

CTexture::CTexture() : 
	m_hMemDC(NULL),
	m_hBitmap(NULL),
	m_hOldBitmap(NULL),
	m_tInfo()
{
}

CTexture::~CTexture()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}

bool CTexture::LoadTexture(HINSTANCE hInst, HDC hDC, const string& strKey, const wchar_t* pFileName, const string& strPathKey)
{
	// make mem DC
	m_hMemDC = CreateCompatibleDC(hDC);

	// make total path
	const wchar_t* pPath = GET_SINGE(CPathManager)->FindPath(strPathKey);
	wstring strPath;
	
	if (pPath) 
		strPath = pPath;

	strPath += pFileName;

	m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	GetObject(m_hBitmap, sizeof(m_tInfo), &m_tInfo);

	return true;
}