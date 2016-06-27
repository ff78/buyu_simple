//
//  GoldGame.cpp
//  buyu_simple
//
//  Created by ff on 15/12/21.
//
//

#include "GoldGame.h"
#include "ClientLogic.h"
#include "GameDef.h"


GoldGame::GoldGame()
{
    gameData = BlackGoldGame::instance();
}

GoldGame::~GoldGame()
{
    
}

void GoldGame::initLoadFunc()
{
    addLoadFunc(Load::LoadFunc(&GoldGame::load1));
//    addLoadFunc(Load::LoadFunc(&GoldGame::initGrids));
    addLoadFunc(Load::LoadFunc(&GoldGame::genGameMap));
    addLoadFunc(Load::LoadFunc(&GoldGame::useAllItems));
}

void GoldGame::load()
{

    L2E_LOAD_GOLD_UPDATE info;
    info.eProtocol = l2e_load_gold;
    info.isOver = false;
    info.step = 0;
    info.count = (int)m_vecLoadFunc.size();
    int result = Load::load();
    if (result != -1) {
        info.step = result;
        ClientLogic::instance()->pass2Engine(&info);
    }else{
        onLoadFinish();
    }
}

void GoldGame::onLoadFinish()
{
    Load::onLoadFinish();
    ClientLogic::instance()->ChangeState(GAME_STATE_GOLDEN_GAME);
    changeGameState(GOLD_GAME_NORMAL);
}

void GoldGame::load1()
{
    
}

//生成游戏地图，摆放物品
void GoldGame::genGameMap()
{
    gameData->readFile(sceneId, GOLD_MAP_CONFIG_FILE);
    pliers = BlackPliers::instance();
    pliers->changeState(IDLE);
}

//进入关卡前使用所有关卡道具
void GoldGame::useAllItems()
{
    gameData->useAllItems();
}


void GoldGame::logic(float dt)
{
    switch (currState) {
        case GOLD_GAME_NORMAL:
            gameData->updateGameTime(dt);
            if (gameData->getGameTime() <= 0) {
                if (gameData->getMissionType() == 1) {
                    if (gameData->getStageMoney() < gameData->getMissionGold()) {
                        changeGameState(GOLD_GAME_FAILED);
                    }else{
                        changeGameState(GOLD_GAME_SUCCESS);
                    }
                }else{
                    if (gameData->getKeyCount() < gameData->getMissionKeyCount()) {
                        changeGameState(GOLD_GAME_FAILED);
                    }else{
                        changeGameState(GOLD_GAME_SUCCESS);
                    }
                }
            }else if (gameData->getAliveCount() <= 0){
                if (gameData->getMissionType() == 1) {
                    if (gameData->getStageMoney() < gameData->getMissionGold()) {
                        changeGameState(GOLD_GAME_FAILED);
                    }else{
                        changeGameState(GOLD_GAME_SUCCESS);
                    }
                }else{
                    if (gameData->getKeyCount() < gameData->getMissionKeyCount()) {
                        changeGameState(GOLD_GAME_FAILED);
                    }else{
                        changeGameState(GOLD_GAME_SUCCESS);
                    }
                }
            }
            break;
            
        default:
            break;
    }
//    gameData->updateGameTime(dt);
}

void GoldGame::changeGameState(GOLD_GAME_STATES nextState)
{
    if(currState == nextState)
        return;
    exitGameState(currState);
    lastState	= currState;
    currState		= nextState;
    enterGameState(nextState);
}

void GoldGame::enterGameState(GOLD_GAME_STATES nextState)
{
    switch (nextState) {
        case GOLD_GAME_SHOW_MISSION:
            
            break;
            
        case GOLD_GAME_NORMAL:
            
            break;
            
        case GOLD_GAME_GET:
            break;
            
        case GOLD_GAME_SUCCESS:
        {
            L2E_GOLD_RESULT info;
            info.eProtocol = l2e_result_goldgame;
            info.success = true;
            info.moneyResult = gameData->getStageMoney();
            ClientLogic::instance()->pass2Engine(&info);            
        }
            break;
            
        case GOLD_GAME_FAILED:
        {
            L2E_GOLD_RESULT info;
            info.eProtocol = l2e_result_goldgame;
            info.success = false;
            info.moneyResult = gameData->getStageMoney();
            ClientLogic::instance()->pass2Engine(&info);
        }
            break;
        default:
            break;
    }
}

void GoldGame::exitGameState(GOLD_GAME_STATES state)
{
    
}

void GoldGame::requestDiff(E2L_GOLD_DIFF &info_in)
{
    L2E_GOLD_DIFF info;
    info.eProtocol = l2e_update_gold_diff;
    info.diff = info_in.diff;
    info.timeLimit = gameData->getChallengeLimit();
    info.validCount = gameData->getChanllengeCount();
    info.fee = gameData->resChallengeFee();
    info.errCode = 0;
    ClientLogic::instance()->pass2Engine(&info);

}

void GoldGame::selDiff(E2L_GOLD_SEL_DIFF &info_in)
{
    int code = gameData->canChallenge();
    if(code == 0)
    {
        gameData->setTempChallengeCount(gameData->getTempChallengeCount()+1);
//        gameData->selDiff2Challenge(info_in.diff);
        ClientLogic::instance()->ChangeState(GAME_STATE_SEL_GOLDEN);
    }else{
        L2E_GOLD_DIFF info;
        info.eProtocol = l2e_update_gold_diff;
        info.errCode = code;
        ClientLogic::instance()->pass2Engine(&info);
    }
}

void GoldGame::selStage(E2L_SEL_GOLD &info_in)
{
    if(gameData->canEnterStage(info_in.selStage))
    {
        sceneId = info_in.selStage;
        ClientLogic::instance()->ChangeState(GAME_STATE_GOLDEN_RUNNER);
    }else{
        L2E_UPDATE_SELGOLD info;
        info.eProtocol = l2e_update_sel_gold;
        info.hasFocus = gameData->getSucNextStage();
        info.errorCode = 1;
        ClientLogic::instance()->pass2Engine(&info);
    }
}

void GoldGame::openStageBox(E2L_OPEN_BOX &info_in)
{
    if (gameData->getStageBoxState() == 0) {
        L2E_OPEN_BOX info;
        info.eProtocol = l2e_open_box;
//        info.money = randStageMoney;
//        info.itemType = randStageItem;
        info.errorCode = 2;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    if (gameData->getStageBoxState() == 2) {
        L2E_OPEN_BOX info;
        info.eProtocol = l2e_open_box;
        //        info.money = randStageMoney;
        //        info.itemType = randStageItem;
        info.errorCode = 3;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    randStageMoney = abs(rand())%1000+200;
    randStageItem = abs(rand())%(GOLD_ITEM_MAX_TYPE-2)+1;
    
    L2E_OPEN_BOX info;
    info.eProtocol = l2e_open_box;
    info.money = randStageMoney;
    info.itemType = randStageItem;
    info.errorCode = 0;
    ClientLogic::instance()->pass2Engine(&info);
}
void GoldGame::startGame(E2L_GOLD_START_GAME &info)
{
    if (gameData->getTempChallengeCount() > gameData->getChanllengeCount()) {
        gameData->selDiff2Challenge();
    }
    
//    gameData->setHasRolled(false);
    initLoadFunc();
    ClientLogic::instance()->ChangeState(GAME_STATE_LOAD_GOLDEN);
}

void GoldGame::gameResult(E2L_GOLD_RESULT &info)
{
    if (currState == GOLD_GAME_FAILED) {
        ClientLogic::instance()->ChangeState(GAME_STATE_GOLDEN_DIFF);
    }else if (currState == GOLD_GAME_SUCCESS){
        gameData->setSucNextStage(gameData->openNextStage());
        gameData->setHasRolled(!gameData->getSucNextStage());
        ClientLogic::instance()->ChangeState(GAME_STATE_SEL_GOLDEN);
    }
}

void GoldGame::startRoll(E2L_GOLD_START_ROLL &info_in)
{
    gameData->setHasRolled(true);
    randRollItem = abs(rand())%(GOLD_ITEM_MAX_TYPE-2)+1;
    
    L2E_START_ROLL info;
    info.eProtocol = l2e_start_gold_roll;
    info.itemType = randRollItem;
    ClientLogic::instance()->pass2Engine(&info);
}
void GoldGame::openFateBox(E2L_OPEN_BOX &info_in)
{
    if (gameData->getFateBoxState() == 0 || gameData->getKeyCount() < 5) {
        L2E_OPEN_BOX info;
        info.eProtocol = l2e_open_box;
        //        info.money = randStageMoney;
        //        info.itemType = randStageItem;
        info.errorCode = 1;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    if (gameData->getFateBoxState() == 2) {
        L2E_OPEN_BOX info;
        info.eProtocol = l2e_open_box;
        //        info.money = randStageMoney;
        //        info.itemType = randStageItem;
        info.errorCode = 3;
        ClientLogic::instance()->pass2Engine(&info);
        return;
    }
    
    randFateMoney = abs(rand())%1000+200;
    randFateItem = abs(rand())%(GOLD_ITEM_MAX_TYPE-2)+1;

    
    L2E_OPEN_BOX info;
    info.eProtocol = l2e_open_box;
    info.money = randFateMoney;
    info.itemType = randFateItem;
    info.errorCode = 0;
    ClientLogic::instance()->pass2Engine(&info);
}

void GoldGame::getFromStageBox(E2L_OPEN_BOX &info_in)
{
    gameData->addMoney(randStageMoney);
    gameData->addItem((GOLD_ITEM_TYPE)randStageItem, false);
    gameData->setStageBoxState(2);
    gameData->setSucNextStage(gameData->openNextStage());
    gameData->setHasRolled(false);
    
    
    L2E_UPDATE_SELGOLD info;
    info.eProtocol = l2e_update_sel_gold;
    info.boxState = gameData->getStageBoxState();
    info.lastStage = gameData->getDiffStage();
    info.hasFocus = gameData->getSucNextStage();
    info.errorCode = 0;
    ClientLogic::instance()->pass2Engine(&info);
}


void GoldGame::getFromFateRoll(E2L_OPEN_BOX &info_in)
{
    gameData->addItem((GOLD_ITEM_TYPE)randRollItem, false);

    
//    L2E_UPDATE_GOLD_RUNNER info;
//    info.eProtocol = l2e_update_gold_runner;
//    info.boxState = gameData->getFateBoxState();
//    info.keyCount = gameData->getKeyCount();
//    info.errorCode = 0;
//    ClientLogic::instance()->pass2Engine(&info);
}

void GoldGame::getFromFateBox(E2L_OPEN_BOX &info_in)
{
    gameData->setFateBoxState(2);
    gameData->clearKey();
    gameData->addMoney(randFateMoney);
    gameData->addItem((GOLD_ITEM_TYPE)randFateItem, false);

    
    L2E_UPDATE_GOLD_RUNNER info;
    info.eProtocol = l2e_update_gold_runner;
    info.boxState = gameData->getFateBoxState();
    info.keyCount = gameData->getKeyCount();
    info.hasRolled = gameData->getHasRolled();
    info.errorCode = 0;
    ClientLogic::instance()->pass2Engine(&info);
}

void GoldGame::gainObj()
{
    if (currState == GOLD_GAME_NORMAL) {
        gameData->gainObj(pliers->getPullSerNo());
    }
    
}