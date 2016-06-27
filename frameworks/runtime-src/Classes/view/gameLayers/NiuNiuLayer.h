//
//  NiuNiuLayer.hpp
//  buyu_simple
//
//  Created by ff on 16/3/18.
//
//

#ifndef NiuNiuLayer_hpp
#define NiuNiuLayer_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ELProtocol.h"
#include "GameDef.h"


const int resultResName[NIU_RESULT_MAX]={0,1,7,10};
const std::string suitName[NIU_RESULT_MAX]={"spade.png", "red_heat.png",  "titoni.png", "block.png"};
const std::string rankName[13]={"1","2","3","4","5","6","7","8","9","X","J","Q","K"};
class NiuNiuLayer : public cocos2d::LayerColor
{
public:
    NiuNiuLayer();
    ~NiuNiuLayer();
    
    void setupView(cocos2d::EventCustom* event);
    
    bool init();
    CREATE_FUNC(NiuNiuLayer);
    
    void clickStartDeal(Ref* pSender);
    void clickCountBtn(Ref* pSender);
    
    void endDeal();
    
    bool onTouchCardBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCard(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void showHand(cocos2d::EventCustom *event);
    void endShowHand(int result);
    void startClick();
    void endShowResult();
    void removeSelf();
    
    void onFrameEvent(cocostudio::timeline::Frame *frame);
    
    void update(float dt);
private:
    cocos2d::LayerColor *backLayer;
    cocos2d::Node *dialogNode;
    cocostudio::timeline::ActionTimeline *dialogAction;
    
    cocos2d::Sprite *root;
    cocos2d::Sprite *clockSpt;
    cocos2d::ui::TextAtlas *ClickTimer;
    cocos2d::ui::TextAtlas *MultiLabel;
    cocos2d::ui::TextAtlas *PressCount;
    cocos2d::ui::TextAtlas *MoneyResult;
    
    cocos2d::ui::Button *startDealBtn;
    cocos2d::ui::Button *clickBtn;
    
    cocos2d::EventListenerCustom *showhandListener;
    
    int clickCount;
    int multi;
    int cardResult;
    int clickResult;
    float clickDuration;
    
    std::string cardSuitRes[5];
    
};

#endif /* NiuNiuLayer_hpp */
