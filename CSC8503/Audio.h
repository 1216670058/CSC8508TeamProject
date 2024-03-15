#pragma once
//irrKlang
//https://www.ambiera.com/irrklang/license.html
#include <stdio.h>
#include <irrKlang.h>
#include <conio.h>

#include "Window.h"
#include "Assets.h"
#include "GameWorld.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

using namespace irrklang;
using namespace NCL;
using namespace CSC8503;

class Audio
{
public:
    Audio(GameWorld* world);
    ~Audio();
    void Update();
    void playbutton();

private:
    GameWorld* world;
    //Audio
    ISoundEngine* soundEngine = nullptr;

    ISoundSource* menubgmsource = nullptr;
    ISoundSource* playingbgmsource = nullptr;
    ISoundSource* trainsource = nullptr;

    ISound* menubgm = nullptr;
    ISound* playingbgm = nullptr;
    ISound* trainbgm = nullptr;
    ISound* button = nullptr;
};
