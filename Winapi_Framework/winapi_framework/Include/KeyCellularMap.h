#pragma once
#include "CellularMap.h"
class KeyCellularMap :
    public CellularMap
{
public:
    KeyCellularMap(HDC hDC, int width, int height, bool parallel, int seed = -1) : CellularMap(hDC, width, height, parallel, seed) {}
    virtual void Input(float fDeltaTime)
    {
        if (GET_SINGE(CInput)->KeyDown("Activate")) {
            Clock::GetInst()->start();
            CalCellular();
            DrawMap();
            Clock::GetInst()->end();
            Clock::GetInst()->message();
        }
    }
};

