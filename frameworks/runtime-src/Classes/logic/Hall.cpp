//
//  Hall.cpp
//  buyu_simple
//
//  Created by ff on 15/12/9.
//
//

#include "Hall.h"
#include "ClientLogic.h"
#include "GameCore.h"


void Hall::enterGame(E2L_ENTERGAME &info)
{
    GameCore::instance()->setCurrStage(info.stage);
    ClientLogic::instance()->ChangeState(GAME_STATE_GAME);
}