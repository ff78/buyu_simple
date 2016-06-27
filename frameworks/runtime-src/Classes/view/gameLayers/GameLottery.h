//
//  GameLottery.h
//  buyu_simple
//
//  Created by ff on 16/4/29.
//
//

#ifndef GameLottery_h
#define GameLottery_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

enum Lottery_State
{
    SHOW_CARD,
    SHUFFLE_CARD,
    CLICK_CARD,
    SHOWHAND,
    RECEIVE_CHIPS
};

class GameLottery : public cocos2d::LayerColor
{
public:
    GameLottery();
    ~GameLottery();
    
    bool init();
    
    void setupView(cocos2d::EventCustom *event);
    
    void clickStartBtn(cocos2d::Ref* pSender);
    void clickEndBtn(cocos2d::Ref* pSender);
    bool clickCardBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void clickCardEnd(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void startShuffel();
    void endShuffel();
    void openCard(int index);
    void showNextHand(int index);
    void endLottery();
    void exitLottery();
    
    void update(float dt);
    
    void changeGameState(Lottery_State nextState);
    
    CREATE_FUNC(GameLottery);
public:
    cocos2d::Node* root;
    cocostudio::timeline::ActionTimeline* uiAction;
    cocostudio::timeline::ActionTimeline* cardAction;
    
    cocos2d::ui::Button* startBtn;
    cocos2d::ui::Button* endBtn;
    cocos2d::ui::TextAtlas* countdownLabel;
    
    cocos2d::Sprite* card[6];
    cocos2d::Sprite* cardframeSpt;
    cocos2d::Sprite* cardLightSpt;
    
    int nextHand;
    int selFocus;
    int resultHand[6];
    int originBoundtypes[6];
    int originBoundIds[6];
    int originBoundCount[6];
    bool canClickCard;
    
    Lottery_State currState;
    float countdownDuration;
};

#endif /* GameLottery_h */
