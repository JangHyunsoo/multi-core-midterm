#pragma once
#include "Ref.h"

class CTexture :
	public CRef
{
private:
	friend class CResourceManager;

private:
	CTexture();
	~CTexture();

private:
	HDC m_hMemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	BITMAP m_tInfo;
public:
	HDC GetDC() const {
		return m_hMemDC;
	}

public:
	bool LoadTexture(HINSTANCE hInst, HDC hDC, const string& strKey, const wchar_t* pFileName, const string& strPathKey = TEXTURE_PATH);

};
