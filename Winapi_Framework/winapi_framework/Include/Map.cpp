#include "Map.h"
#include "ResourceManager.h"
#include "Flag.h"
#include "Function.h"
#include "Input.h"
#include "PerlinNoise.h"
#include <random>
#include <omp.h>
#include <fstream>
#include "Player.h"

CMap::CMap() {

}

CMap::CMap(HDC hDC, int width = 300, int height = 300) : m_iWidth(width), m_iHeight(height)
{
	m_2DMap = Array2D(m_iHeight, std::vector<TILE_TYPE>(m_iWidth, TILE_TYPE::LAND));
	m_hMemDC = CreateCompatibleDC(hDC);
	m_hMemBitmap = CreateCompatibleBitmap(hDC, m_iWidth * m_iTileSize, m_iHeight * m_iTileSize);
	SelectObject(m_hMemDC, m_hMemBitmap);
}

CMap::CMap(const CMap& obj) :
	CStaticObj(obj), m_iWidth(obj.m_iWidth), m_iHeight(obj.m_iHeight)
{
	m_2DMap = Array2D(obj.m_iHeight, std::vector<TILE_TYPE>(obj.m_iWidth, TILE_TYPE::LAND));
	m_hMemDC = obj.m_hMemDC;
	m_hMemBitmap = obj.m_hMemBitmap;
	SelectObject(m_hMemDC, m_hMemBitmap);
}


CMap::~CMap()
{
	DeleteObject(m_hMemBitmap);
	DeleteDC(m_hMemDC);
}

void CMap::SetCell(int x, int y, TILE_TYPE bValue)
{
	if (IsMap(x, y)) {
		m_2DMap[y][x] = bValue;
	}
}

TILE_TYPE CMap::GetCell(int x, int y)
{
	return m_2DMap[y][x];
}

bool CMap::Init()
{
	SetPos(0, 0);
	InitTexture();
	InitParallel();
	GenerateMap();
	return true;
}

void CMap::InitTexture() {
	m_pWhite = CreateSolidBrush(RGB(255, 255, 255));
	m_pAir = CreateSolidBrush(RGB(0, 0, 0));
	m_pLand = CreateSolidBrush(RGB(0, 255, 100));
	m_pStone = CreateSolidBrush(RGB(222, 222, 222));
	m_pWater = CreateSolidBrush(RGB(0, 100, 255));
}

HBRUSH& CMap::getTile(TILE_TYPE tile_type) {
	switch (tile_type)
	{
	case TILE_TYPE::AIR:
		return m_pAir;
	case TILE_TYPE::LAND:
		return m_pLand;
	case TILE_TYPE::STONE:
		return m_pStone;
	case TILE_TYPE::WATER:
		return m_pWater;
	}
}

void CMap::DrawMap()
{
	for (int x = 0; x < m_iWidth; x++) {
		for (int y = 0; y < m_iHeight; y++) {
			int dx = m_tPos.x + x * m_iTileSize;
			int dy = m_tPos.y + y * m_iTileSize;
			SelectObject(m_hMemDC, getTile(m_2DMap[y][x]));
			Rectangle(m_hMemDC, dx, dy, dx + m_iTileSize, dy + m_iTileSize);
		}
	}

	SelectObject(m_hMemDC, m_pAir);
}



bool CMap::IsMap(int x, int y)
{
	return x < m_iWidth && y < m_iHeight && x >= 0 && y >= 0;
}

void CMap::Render(HDC hDC, float fDeltaTime)
{	
	BitBlt(hDC, 0, 0,m_iWidth * m_iTileSize , m_iHeight * m_iTileSize, m_hMemDC, 0, 0, SRCCOPY);
}