#pragma once
#include "Map.h"
#include "Input.h"
#include <omp.h>

class CellularMap :
    public CMap
{
public:
	CellularMap(HDC hDC, int width, int height) : CMap(hDC, width, height) {}
	virtual void Input(float fDeltaTime)
    {
        if (GET_SINGE(CInput)->KeyDown("Activate")) {
            CalCellular();
        }
		if (GET_SINGE(CInput)->KeyDown("ActivateParallel")) {
			CalCellularParallel();
		}
    }
    virtual CMap* Clone() {
        return new CellularMap(*this);
    }
private:
	virtual void GenerateMap() {
		SetupRandomMap();
		DrawMap();
	}
    bool IsMap(int x, int y)
    {
        return x < m_iWidth && y < m_iHeight && x >= 0 && y >= 0;
    }
	void SetupRandomMap() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(0.0f, 1.0f);
		int count = 0;

		for (int y = 3; y < m_iHeight - 3; y++)
		{
			for (int x = 3; x < m_iWidth - 3; x++)
			{
				if (dis(gen) > 0.4f)
				{
					m_2DMap[y][x] = TILE_TYPE::AIR;
					count++;
				}
			}
		}
	}
	int CountSurround(int x, int y) {
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
	void CalCellular() {
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
	void CalCellularParallel() {
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
};

