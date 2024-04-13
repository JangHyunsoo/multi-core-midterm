#include "Map.h"
#include "ResourceManager.h"
#include "Flag.h"
#include "Function.h"
#include "Input.h"
#include "PerlinNoise.h"
#include <random>
#include <omp.h>

CMap::CMap() {

}

CMap::CMap(HDC hDC, int width, int height) : m_iWidth(300), m_iHeight(300)
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
	m_hMemDC = CreateCompatibleDC(NULL);
	m_hMemBitmap = CreateCompatibleBitmap(NULL, m_iWidth * m_iTileSize, m_iHeight * m_iTileSize);
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
	GenerateMapWithPerlin();
	//GenerateMap();
	//DrawMap();
	return true;
}

void CMap::InitTexture() {
	m_pWhite = CreateSolidBrush(RGB(255, 255, 255));
	m_pAir = CreateSolidBrush(RGB(0, 0, 0));
	m_pLand = CreateSolidBrush(RGB(0, 255, 100));
	m_pStone = CreateSolidBrush(RGB(222, 222, 222));
	m_pWater = CreateSolidBrush(RGB(0, 100, 255));
}

void CMap::GenerateMapWithPerlin() {
	PerlinNoise pn;
	std::vector<std::vector<double>> perlin_value(m_iHeight, vector<double>(m_iWidth, 0));
	double mean = 0.0f;
	double count = 0.0f;

	for (unsigned int i = 0; i < m_iHeight; ++i) {     // y
		for (unsigned int j = 0; j < m_iWidth; ++j) {  // x
			double x = (double)j / 30.0;
			double y = (double)i / 30.0;

			// Typical Perlin noise
			double n = pn.noise(10 * x, 10 * y, 0.2);

			// Wood like structure
			n = 20 * pn.noise(x, y, 0.8);
			n = n - floor(n);

			// Map the values to the [0, 255] interval, for simplicity we use 
			// tones of grey
			perlin_value[i][j] = n;
			count += 1.0f;
			mean += n;
		}
	}

	mean /= count;


	for (unsigned int i = 0; i < m_iHeight; ++i) {     // y
		for (unsigned int j = 0; j < m_iWidth; ++j) {  // x
			if (perlin_value[i][j] > mean + 0.1) {
				m_2DMap[i][j] = TILE_TYPE::LAND;
			}
			else {
				m_2DMap[i][j] = TILE_TYPE::WATER;
			}
			//int value = (int)(perlin_value[i][j] * 250);
			//int dx = j * m_iTileSize;
			//int dy = i * m_iTileSize;
			//SelectObject(m_hMemDC, CreateSolidBrush(RGB(value, value, value)));
			//Rectangle(m_hMemDC, dx, dy, dx + m_iTileSize, dy + m_iTileSize);
		}
	}

	DrawMap();
}

void CMap::GenerateMap() {
	SetupRandomMap();
}

void CMap::SetupRandomMap() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	int count = 0;

	for (int y = 3; y < m_iHeight - 3; y++)
	{
		for (int x = 3; x < m_iWidth - 3; x++)
		{
			if(dis(gen) > 0.4f)
			{
				m_2DMap[y][x] = TILE_TYPE::AIR;
				count++;
			}
		}
	}
}

void CMap::CalCellular() {
	for (int y = 3; y < m_iHeight - 3; y++)
	{
		for (int x = 3; x < m_iWidth - 3; x++)
		{
			int counter = CountSurround(x, y);
			if (counter > 4) {
				m_2DMap[y][x] = TILE_TYPE::LAND;
			}
			else if (counter < 4) {
				m_2DMap[y][x] = TILE_TYPE::AIR;
			}
		}
	}
}

void CMap::CalCellularParallel() {
	#pragma omp parallel num_threads(8)
	{
		int n = m_iHeight - 6;
		int thread_count = omp_get_num_threads();
		double d_thread_count = (double)thread_count;
		int thread_id = omp_get_thread_num();
		double d_thread_id = (double)thread_id;

		double d_start_height = d_thread_id * n / d_thread_count;
		int start_height = (int)d_start_height + 3;
		double d_end_height = (thread_count == thread_id - 1) ? n : d_start_height + n / d_thread_count;
		int end_height = (int)d_end_height + 3;
		for (int y = start_height; y < end_height; y++)
		{
			for (int x = 3; x < m_iWidth - 3; x++)
			{
				int counter = CountSurround(x, y);
				if (counter > 4) {
					m_2DMap[y][x] = TILE_TYPE::LAND;
				}
				else if (counter < 4) {
					m_2DMap[y][x] = TILE_TYPE::AIR;
				}
			}
		}
	}

	DrawMap();
}


int CMap::CountSurround(int x, int y) {
	int counter = 0;
	for (int dy = -1; dy <= 1; dy++)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			if (m_2DMap[y + dy][x + dx] == TILE_TYPE::LAND)
			{
				counter++;
			}
		}
	}
	return counter;
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

void CMap::Input(float fDeltaTime)
{
	if (GET_SINGE(CInput)->KeyDown("Activate")) {
		CalCellularParallel();
	}
}

void CMap::Render(HDC hDC, float fDeltaTime)
{	
	BitBlt(hDC, 0, 0,m_iWidth * m_iTileSize, m_iHeight * m_iTileSize, m_hMemDC, 0, 0, SRCCOPY);
}

CMap* CMap::Clone()
{
	return new CMap(*this);
}