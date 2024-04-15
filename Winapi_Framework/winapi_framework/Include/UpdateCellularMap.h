#pragma once
#include "CellularMap.h"
class UpdateCellularMap :
    public CellularMap
{
private:
	float m_fCurTime = 0.0f;
public:
	UpdateCellularMap(HDC hDC, int width, int height, bool parallel, int seed = -1) : CellularMap(hDC, width, height, parallel, seed) {}

	virtual int Update(float fDeltaTime) {
		m_fCurTime += fDeltaTime;

		if (m_fCurTime >= 1.0f) {
			m_fCurTime = 0;
			CalCellular();
			DrawMap();
		}
		return 0;
	}
};

