//
//  LuckyDrawLayer.h
//  buyu_simple
//
//  Created by ff on 16/5/3.
//
//

#ifndef LuckyDrawLayer_h
#define LuckyDrawLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class LuckyDrawLayer : public cocos2d::LayerColor
{
public:
    LuckyDrawLayer();
    ~LuckyDrawLayer();
    
    bool init();
    
    void setupView();
    
    void enterLuckyDraw(cocos2d::EventCustom *event);
    void updateLuckyDraw(cocos2d::EventCustom *event);
    
    void clickLevel(cocos2d::Ref *pSender);
    void clickClose(cocos2d::Ref *pSender);
    
    void showConfimDialog();
    void closeConfirmDialog();
    void removeConfirm();
    
    void showBossDialog();
    void removeBossDialog();
    
    void enterLottery();
    
    CREATE_FUNC(LuckyDrawLayer);
private:
    cocos2d::Node* root;
    cocostudio::timeline::ActionTimeline* uiAction;
    
    cocos2d::Node* confirmDialog;
    cocos2d::LayerColor* confirmLayer;
    
    cocos2d::Node* bossDialog;
    cocos2d::LayerColor* bossLayer;
    
    cocos2d::Sprite* card[6];
    cocos2d::Sprite* activeArea;
    
    cocos2d::ui::Button* levelBtn[6];
    
    int level;
    std::string nextTitle;
    int score;
    int maxScore;
    
    cocos2d::EventListenerCustom *updateListener;
};

#endif /* LuckyDrawLayer_h */
