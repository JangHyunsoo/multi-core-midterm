#pragma once
#include "Scene.h"
class CInGameScene :
    public CScene
{
private:
    friend class CSceneManager;
private:
    CInGameScene();
public:
    virtual bool Init(); 
};

