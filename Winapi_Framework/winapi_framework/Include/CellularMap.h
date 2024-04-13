#pragma once
#include "Map.h"
#include "Input.h"
#include "Clock.h"
#include <omp.h>

class CellularMap :
    public CMap
{
public:
	CellularMap(HDC hDC, int width, int height) : CMap(hDC, width, height) {}
	virtual void Input(float fDeltaTime)
    {
        if (GET_SINGE(CInput)->KeyDown("Activate")) {
			Clock::GetInst()->start();
            CalCellular();
			Clock::GetInst()->end();
			Clock::GetInst()->message();
			DrawMapParallel();
        }
		if (GET_SINGE(CInput)->KeyDown("ActivateParallel")) {
			Clock::GetInst()->start();
			CalCellularParallel();
			Clock::GetInst()->end();
			Clock::GetInst()->message();
			DrawMapParallel();
		}
    }
    virtual CMap* Clone() {
        return new CellularMap(*this);
    }
private:
	virtual void GenerateMap() {
		SetupRandomMap();
		DrawMapParallel();
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

		for (int y = 0; y < m_iHeight; y++)
		{
			for (int x = 0; x < m_iWidth; x++)
			{
				if (dis(gen) > 0.4f)
				{
					m_2DMap[y][x] = TILE_TYPE::AIR;
					count++;
				}
			}
		}
	}
	inline int CountSurround(int x, int y) {
		int counter = 0;
		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				int cur_x = x + dx;
				int cur_y = y + dy;
				if (!IsMap(cur_x, cur_y)) {
					counter++;
				}
				else if (m_2DMap[cur_y][cur_x] == TILE_TYPE::LAND)
				{
					counter++;
				}
			}
		}
		return counter;
	}
	void CalCellular() {
		for (int y = 0; y < m_iHeight; y++)
		{
			for (int x = 0; x < m_iWidth; x++)
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
		int thread_count = ThreadManager::GetInst()->getThreadCount();
#pragma omp parallel num_threads(thread_count)
		{
			int n = m_iHeight;
			double d_thread_count = (double)thread_count;
			int thread_id = omp_get_thread_num();
			double d_thread_id = (double)thread_id;

			double d_start_height = d_thread_id * n / d_thread_count;
			int start_height = (int)d_start_height;
			double d_end_height = (thread_count == thread_id - 1) ? n : d_start_height + n / d_thread_count;
			int end_height = (int)d_end_height;
			for (int y = start_height; y < end_height - 1; y++)
			{
				for (int x = 0; x < m_iWidth; x++)
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
#pragma omp barrier
			int y = end_height - 1;
			for (int x = 0; x < m_iWidth; x++)
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
};

