//
//  TipLayer.cpp
//  buyu_simple
//
//  Created by ff on 16/4/7.
//
//

#include "TipLayer.h"
#include "utils/GameUtils.h"
#include "../node/TipEffect.h"

#include "ELProtocol.h"
#include "EventDef.h"
#include "sound/SoundManager.h"
#include "utils/StringData.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

TipLayer::TipLayer()
{
    
}

TipLayer::~TipLayer()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(addTaskTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(addLevelupTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(addUnlockTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(addKillBossTipListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(disableSkillListener);
    
}

bool TipLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    addTaskTipListener = EventListenerCustom::create(UI_COMP_SUB_TASK, CC_CALLBACK_0(TipLayer::addTaskTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addTaskTipListener, -1);
    addLevelupTipListener = EventListenerCustom::create(UI_LEVELUP_TIP, CC_CALLBACK_0(TipLayer::addLeveupTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addLevelupTipListener, -1);
    addUnlockTipListener = EventListenerCustom::create(UI_UNLOCK_TIP, CC_CALLBACK_0(TipLayer::addUnlockTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addUnlockTipListener, -1);
    addKillBossTipListener = EventListenerCustom::create(UI_KILLBOSS_TIP, CC_CALLBACK_0(TipLayer::addKillBossTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addKillBossTipListener, -1);
    disableSkillListener = EventListenerCustom::create(UI_DISABLE_SKILL_TIP, CC_CALLBACK_0(TipLayer::addDisableSkillTip, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(disableSkillListener, -1);
    return true;
}

void TipLayer::addTaskTip()
{
    TipEffect *tip = TipEffect::create();
    tip->setupView("common/tip_effect/taskover.png");
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
    AUDIO_PLAY("win", AUDIO_TYPE::EFFECT_TYPE);
}

void TipLayer::addLeveupTip()
{
    TipEffect *tip = TipEffect::create();
    tip->setupView("common/tip_effect/levelup.png");
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
    AUDIO_PLAY("level_up", AUDIO_TYPE::EFFECT_TYPE);
}

void TipLayer::addUnlockTip()
{
    TipEffect *tip = TipEffect::create();
    tip->setupView("common/tip_effect/gunupgrade.png");
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
    AUDIO_PLAY("win", AUDIO_TYPE::EFFECT_TYPE);
}

void TipLayer::addKillBossTip()
{
    TipEffect *tip = TipEffect::create();
    tip->setupView("common/tip_effect/customspass.png");
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
    AUDIO_PLAY("win", AUDIO_TYPE::EFFECT_TYPE);
}

void TipLayer::addDisableSkillTip()
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("tide_forbid_skill"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    tip->setupView();
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addLackGoldTip()
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("gold_lack"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    tip->setupView();
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addShareErrorTip()
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("share_error"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    tip->setupView();
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addQQUninstallTip()
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("qq_share_error"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    tip->setupView();
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addBenefitCodeUsed()
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("exchange_err_used"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    tip->setupView();
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addBenefitCodeError()
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("exchange_err_code"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    tip->setupView();
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addBombNeedUnlockTip()
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("skill_need_unlock"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    tip->setupView();
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addRayNeedVipTip(int vipLevel, DelayLabelTip::fp &f)
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1((SSTRING("ray_need_vip")+Convert2String(vipLevel)).c_str());
    tip->setText2("");
    tip->setDelayDuration(1.5f);
//    DelayLabelTip::fp f = std::bind(&TipLayer::addRechargeOkTip, this);
    tip->setupView(&f);
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addAutoFireNeedVipTip(int vipLevel, DelayLabelTip::fp &f)
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1((SSTRING("autofire_need_vip")+Convert2String(vipLevel)).c_str());
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    tip->setupView(&f);
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addSkillNeedDiamondTip(DelayLabelTip::fp &f)
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("skill_need_diamond"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    //    DelayLabelTip::fp f = std::bind(&TipLayer::addRechargeOkTip, this);
    tip->setupView(&f);
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addUnlockNeedDiamondTip(DelayLabelTip::fp &f)
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("unlock_need_diamond"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    //    DelayLabelTip::fp f = std::bind(&TipLayer::addRechargeOkTip, this);
    tip->setupView(&f);
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}

void TipLayer::addRechargeOkTip()
{
    DelayLabelTip *tip = DelayLabelTip::create();
    tip->setText1(SSTRING("recharge_success"));
    tip->setText2("");
    tip->setDelayDuration(1.5f);
    tip->setupView();
    tip->setPosition(Vec2(GameUtils::winSize.width/2, GameUtils::winSize.height/2));
    addChild(tip);
}
