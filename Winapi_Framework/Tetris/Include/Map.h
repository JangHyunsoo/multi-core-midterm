#pragma once
#include "StaticObj.h"
#include "Flag.h"
#include "Texture.h"
#include <vector>

using Array2D = std::vector<std::vector<TILE_TYPE>>;

class CMap :
    public CStaticObj
{
public:
    CMap();
    CMap(int width, int height);
    CMap(const CMap& obj);
    ~CMap();
private:
    const int m_iHeight = 100;
    const int m_iWidth = 100;
    const int m_iTileSize = 16;
    Array2D m_2DMap;
    CTexture* m_pAir;
    CTexture* m_pLand;
    CTexture* m_pStone;
    CTexture* m_pWater;
public:
    void SetCell(int x, int y, TILE_TYPE bValue);
    TILE_TYPE GetCell(int x, int y);
private:
    bool IsMap(int x, int y); 
    CTexture* getTile(TILE_TYPE tile_type);

public:
    virtual bool Init();
    virtual void Input(float fDeltaTime);
    virtual void Render(HDC hDC, float fDeltaTime);
    virtual CMap* Clone();
private:
    void InitTexture();
    void GenerateMap();
    void SetupRandomMap();
    void CalCellular();
    int CountSurround(int x, int y);
};