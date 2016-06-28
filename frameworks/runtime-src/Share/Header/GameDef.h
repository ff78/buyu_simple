//
//  GameDef.h
//  buyu_simple
//
//  Created by ff on 15/12/10.
//
//

#ifndef buyu_simple_GameDef_h
#define buyu_simple_GameDef_h
#include "cocos2d.h"

static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);
static cocos2d::Size pathResolutionSize = cocos2d::Size(1074, 740);

#define MAX_SMALL_PATH				208
#define MAX_BIG_PATH                130
#define MAX_HUGE_PATH               62
#define MAX_SPECIAL_PATH            24
#define MAX_SCENE1_PATH             241
#define MAX_SCENE2_PATH             215
#define MAX_SCENE3_PATH             243

#define MAX_FISHARRAY_PATH          8

#define MAX_FISHARRAY_STYLE         7

#define MAX_GOLD_CHALLENGE_TIMES    3
#define GOLD_CHALLENGE_FEE1         0
#define GOLD_CHALLENGE_FEE2         1000
#define GOLD_CHALLENGE_FEE3         5000

#define MAX_GOLD_STAGE              18

#define MAX_SCENE_PROB_STYLE        4
//#define REBATE_TIME_LIMIT           3
//#define FIRST_FORCE_TIME            12
//#define NORMAL_FORCE_TIME           15
//#define PROB_DOWN_TIME              4
//#define PROB_UP_TIME                2
#define REBATE_TIME_LIMIT           600
#define FIRST_FORCE_TIME            7200
#define NORMAL_FORCE_TIME           14400
#define PROB_DOWN_TIME              1200
#define PROB_UP_TIME                600

#define ENERGY_TIME_LIMIT           30
#define ENERGY_THRES                5000
enum GOLD_ITEM_TYPE
{
    GOLD_ITEM_BOMB_TYPE = 1,
    GOLD_ITEM_GLOVES_TYPE,
    GOLD_ITEM_GRASS_TYPE,
    GOLD_ITEM_CLOCK_TYPE,
    GOLD_ITEM_DIAMOND_TYPE,
    GOLD_ITEM_ALCHEMY_TYPE,
    GOLD_ITEM_BEAUTY_TYPE,
    GOLD_ITEM_WINE_TYPE,
    GOLD_ITEM_SKULL_TYPE,
    GOLD_ITEM_WAND_TYPE,
    GOLD_ITEM_KEY_TYPE,
    GOLD_ITEM_MAX_TYPE
};

//// 鱼路径类型
enum FISH_PATH_TYPE
{
    SMALL_FISH_PATH = 0,
    BIG_FISH_PATH,
    HUGE_FISH_PATH,
    SPECIAL_FISH_PATH,
    SCENE1_FISH_PATH,
    SCENE2_FISH_PATH,
    SCENE3_FISH_PATH,
    
    FISH_ARRAY_PATH,
    
    FISH_ARRAY_PATH_EX1,
    FISH_ARRAY_PATH_EX2,
    FISH_ARRAY_PATH_EX3,
    FISH_SLASH_PATH,
    MAXTYPE_FISH_PATH,
};

//// 新的鱼类型
enum FISH_TYPE_EX
{
    FISH_TYPE_EX_1 = 0,    /// 小黄鱼
    FISH_TYPE_EX_2,        /// 小青鱼
    FISH_TYPE_EX_3,        /// 斑马鱼
    FISH_TYPE_EX_4,        /// 气泡鱼
    FISH_TYPE_EX_5,        /// 海扁鱼
    FISH_TYPE_EX_6,        /// 小金鱼
    FISH_TYPE_EX_7,        /// 刷子鱼
    FISH_TYPE_EX_8,        /// 蓝梦鱼
    FISH_TYPE_EX_9,        /// 灯笼鱼
    FISH_TYPE_EX_10,       /// 乌龟
    FISH_TYPE_EX_11,       /// 蝴蝶鱼
    FISH_TYPE_EX_12,       /// 神仙鱼
    FISH_TYPE_EX_13,       /// 蓝尾鱼
    FISH_TYPE_EX_14,       /// 尖嘴鱼
    FISH_TYPE_EX_15,       /// 魔鬼鱼
    FISH_TYPE_EX_16,       /// 银鲨鱼
    FISH_TYPE_EX_17,       /// 金鲨鱼
    FISH_TYPE_EX_18,       /// 银鲸鱼      ---- 小boss
    FISH_TYPE_EX_19,       /// 黄鲸鱼      ---- 小boss
    FISH_TYPE_EX_20,       /// 金蝉        ---- 大boss
    FISH_TYPE_EX_21,       /// 李逵
    FISH_TYPE_EX_22,       /// 大圣
    FISH_TYPE_EX_23,       /// 美人鱼
    FISH_TYPE_EX_24,       /// 财神     ---- 大boss
    FISH_TYPE_EX_25,	   /// 蛇头     ---- 特殊鱼
    FISH_TYPE_EX_26,       /// 蛇身
    FISH_TYPE_EX_27,	   /// 蛇尾
    FISH_TYPE_EX_28,	   /// 三人鱼
    FISH_TYPE_EX_29,	   /// 四人鱼
    FISH_TYPE_EX_30,       /// 冰冻球
    FISH_TYPE_EX_31,       /// 红宝石
    FISH_TYPE_EX_32,       /// 炸弹
    FISH_TYPE_EX_33,       /// 话费券
    FISH_TYPE_EX_34,       /// 宝箱
    FISH_TYPE_EX_35,       /// 元宝
    FISH_TYPE_EX_36,       /// 蝎子王
    FISH_TYPE_EX_37,       //   一网打尽
    FISH_TYPE_EX_38,       //   同类炸弹
    FISH_TYPE_EX_39,       //   牛牛
    FISH_TYPE_EX_40,       //   闪电鱼
    FISH_TYPE_EX_41,       //   马莉鱼
    FISH_TYPE_EX_42,       //   海豚
    FISH_TYPE_EX_43,       //   双头龟
    FISH_TYPE_EX_44,       //   金龙
    FISH_TYPE_EX_45,       //   美人鱼
    FISH_TYPE_EX_46,       //   双头鲸
    FISH_TYPE_EX_47,       //   食梦鱼
    FISH_TYPE_EX_48,       //   绿蟾
    FISH_TYPE_EX_49,       //   定海神针
    FISH_TYPE_EX_50,       //   蓝色妖姬
    FISH_TYPE_EX_51,       //   海之魂
    FISH_TYPE_EX_52,       //   三头鲸
    FISH_TYPE_EX_53,       //   四头鲸
    FISH_TYPE_EX_54,       //   追捕者
    FISH_TYPE_EX_55,       //   玄武
    FISH_TYPE_EX_56,       //   导弹鱼
    FISH_TYPE_EX_57,       //   红色妖姬
    FISH_TYPE_EX_58,       //   追光者
    FISH_TYPE_EX_59,       //   斩首行动
    FISH_TYPE_EX_60,       //   食人鱼
    FISH_TYPE_EX_61,       //   暗杀者
    FISH_TYPE_EX_62,       //   杀戮者
    FISH_TYPE_EX_63,       //   火麒麟
    FISH_TYPE_EX_64,       //   蓝诺
    FISH_TYPE_EX_65,       //   财运之堡
    FISH_TYPE_EX_66,       //   凤舞九天
    FISH_TYPE_EX_67,       //   螃蟹宝藏
    FISH_TYPE_EX_68,       //   大海神龟
    FISH_TYPE_EX_69,       //   深海霸王
    FISH_TYPE_EX_70,       //   龙凤凤舞
    FISH_TYPE_EX_71,       //   墨之鳞
    FISH_TYPE_EX_72,       //   财之魂
    FISH_TYPE_EX_73,       //   金凤报喜
    FISH_TYPE_EX_74,       //   三头龙
    FISH_TYPE_EX_75,       //   预言章鱼
    FISH_TYPE_EX_76,       //   幽海龟
    FISH_TYPE_EX_77,       //   财神到
    FISH_TYPE_EX_78,       //   恶魔令
    
    MAX_FISH_TYPE_EX
};

//// 炮台类型
enum CANNON_TYPE_EX
{
    CANNON_TYPE_XINSHOU=0,   ///新手炮
    CANNON_TYPE_ZIYUAN,      ///紫苑炮
    CANNON_TYPE_HANGUANG,    ///寒光炮
    CANNON_TYPE_HAIYAN,      ///海炎炮
    CANNON_TYPE_SHENPAN,     ///审判炮
    CANNON_TYPE_DUYAO,       ///毒药炮
    CANNON_TYPE_BINGHUN,     ///冰魂炮
    CANNON_TYPE_NONGYUAN,    ///龙渊炮
    CANNON_TYPE_ANMIE,       ///黯灭炮
    CANNON_TYPE_NENGLIANG,   ///能量炮
    CANNON_TYPE_MAX_EX
};
enum NIU_CARD_TYPE
{
    NIU_0,
    NIU_1,
    NIU_2,
    NIU_3,
    NIU_4,
    NIU_5,
    NIU_6,
    NIU_7,
    NIU_8,
    NIU_9,
    NIU_10,
};
enum NIU_RESULT_EX
{
    NIU_RESULT_0,           //没牛
    NIU_RESULT_SMALL,       //牛1-牛6
    NIU_RESULT_BIG,         //牛7-牛9
    NIU_RESULT_NIU,          //牛牛
    NIU_RESULT_MAX
};

const int gNiuNiuSecond[] = {6, 12, 18, 30};
const int gNiuMaxCards = 52;


//// 炮轴和炮盖的索引
const int gCannonIndex[] = { 0, 0, 1, 3, 1, 2, 2, 4, 4, 3 };

const int gCannonMultiply[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
const int gCannonBullet[] = {2, 2, 2, 3, 3, 3, 3, 4, 4, 4};

const float gSceneProb[] = {1.3, 1.0, 0.7, 0.5};
const float gSceneProbRate[] = {5, 30, 55, 10};
const float gRebateProb = 1.0;
const float gSceneLowProb = 0.5;
const float gSceneUpProb = 1.6;
const float gEnergyCanProb = 1.2;

typedef struct __CMD_CREATE_COIN
{
    cocos2d::Vec2   fishPos;
    cocos2d::Vec2   targetPos;
    int             baseScore;
    int             score;
    int             type;
    int             fishEffectType;
}CMD_CREATE_COIN;

//const int roundRollRate[24] = {
//     5,2,4,5,5,2,
//     7,4,4,3,4,3,
//     8,4,5,4,2,5,
//     4,3,3,5,4
//};
const int roundBtnTypeId[24] = {
    0,3,5,7,8,2,
    0,4,6,10,5,3,
    0,4,7,8,6,2,
    0,5,9,1,7,6
};
const int roundRollRate[24] = {
    5,3,4,5,6,3,
    5,4,4,3,4,3,
    5,4,5,6,4,3,
    5,4,3,3,5,4
};

const int centerRollRate[11] = {
    5,8,10,12,12,15,15,15,6,4
};

const float typeRollMulti[11] = {
    500,300,180,100,80,50,25,10,0.05,0.1
};

const int maryCardMulti[4] = {
    1,20,100,20
};

const int gExpMultiThres[] = {
    100, 600, 2000, 6000, 10000
};

const int gExpMulti[] = {
    1, 2, 3, 5, 8, 10
};

const std::string gTitles[] = {
    "渔夫", "见习水手", "水手", "水手长", "大副",
    "海底猎人", "海贼", "船长", "海贼王", "传说海贼王"
};

const int MAX_LEVEL = 99;


enum STAGE_TARGET_STATES
{
    STAGE_TARGET_INIT = 0,
    STAGE_TARGET_OPEN,
    STAGE_TARGET_BOUND,
    STAGE_TARGET_CLOSE
};

enum TASK_STATES
{
    TASK_INIT = 0,
    TASK_OPEN,
    TASK_BOUND,
    TASK_CLOSE
};

enum FishType
{
    SMALL_FISH = 0,
    TINY_BOSS_FISH = 1,
    BOSS_FISH = 2,
    GROUP_FISH = 3,
    SPECIAL_FISH = 4,
    SPECIAL_BOSS_FISH = 5
};

const int MAX_FISH_PER_BULLET = 5;

const float gCommonBulletRate[MAX_FISH_PER_BULLET] = {
    1, 0.6, 0.4, 0.3, 0.3
};

const int MAX_TASK_COUNT = 3;

const int gLotteryThes[] = {
    50, 40000, 200000, 400000, 800000, 2400000
};

const std::string gLotteryTitle[] = {
    "普通","青铜","白银","黄金","白金","钻石"
};

const int MAX_RECHARGE_ITEM = 12;

const int gShareCountThres[] = {
    1, 2, 5, 10
};

const int gShareCountBounds[] = {
    2000, 5000, 10000, 20000
};

const std::string xxteaKeyStr = "btzsy";
const std::string xxteaSignStr = "twgdh";

#define GOLD_OBJ_CONFIG_FILE "res/data/data_Gold_Obj.json"
#define GOLD_MAP_CONFIG_FILE "res/data/data_Gold_Map.json"
#define LOTTERY_CONFIG_FILE "res/data/data_Lottery_Config.json"
#define CANNON_LEVEL_CONFIG_FILE "res/data/data_Cannon_Level.json"
#define TEST_CREATE_FISH_CONFIG_FILE "res/data/data_Create_Fish1.json"
#define FISH_CONFIG_FILE "res/data/data_Fish_Config.json"
#define RELIFE_CONFIG_FILE "res/data/data_Relife.json"
#define STAGE_CONFIG_FILE "res/data/data_Stage_Config.json"
#define MISSION_CONFIG_FILE "res/data/data_Mission_Config.json"
#define LEVEL_CONFIG_FILE "res/data/data_Level_Config.json"
#define VIP_CONFIG_FILE "res/data/data_Vip_Config.json"
#define RECHARGE_CONFIG_FILE "res/data/data_Recharge_Config.json"
#define SKILL_CONFIG_FILE "res/data/data_Skill_Config.json"

#define BULLET_LASER_ANIM "animation/laser/Laser.csb"
#define CANNON_ENERGY_EFFECT "animation/power/Energy_Effect.csb"
#define CANNON_MULTIPLY_EFFECT "animation/mulriple/muleffect.csb"
#define TIP_EFFECT "common/tip_effect/TipEffect.csb"
#define LIGHT_EFFECT "animation/lightning/lightning.csb"
#define COIN1_ANIM "animation/coin1/coin1.csb"
#define SCORE_LABEL_ANIM "animation/coin2/score_label.csb"
#define TURNTABLE_ANIM "animation/turntable/turntable.csb"
#define TURN_CIRCLE_ANIM "game_scene/game_effect/game_effect/circle.csb"
#define RICH_TIP_ANIM "game_scene/game_effect/game_effect/rich.csb"
#define BLAST_TIP_EFFECT "game_scene/game_effect/game_effect/blast.csb"
#define SUN_LIGHT_EFFECT "game_scene/game_effect/sunlight/sunLight.csb"
#define LIGHT_DOT_ANIM "animation/lightning/lightningDot.csb"
#define ICE_SCREEN_EFFECT "game_scene/game_effect/game_effect/ice.csb"

#define ROPE_ARMATURE "armature/rope/rope.ExportJson"

#define BLAST_PLST "animation/blast.plist"

#define BUBBLE_PARTICLE "common/particle/yuchaoshuipaotexiao.plist"
#define NEXT_STAGE_UI "common/NextStage.csb"
#define COMMON_TIP_UI "common/CommonTip.csb"
#define LOTTERY_TIP_UI "common/LotteryTip.csb"
#define STAGE_COMP_TIP_UI "common/StageComplete.csb"
#define SERVICE_TIP_UI "hall_scene/game_hall/ServiceTip.csb"

#define BULLET_BOMB_ANIM "game_scene/right_menu/Plan_Bomb.csb"
#define AUTO_CIRCLE_RES "game_scene/right_menu/rm_green.png"
#define FIRE_CIRCLE_RES "game_scene/right_menu/rm_fire.png"
#define RAY_CIRCLE_ANIM "game_scene/right_menu/CannonLight.csb"
#define THREE_GROUP_ANIM "game_scene/fish/special_fish_alive/afish_101.csb"
#define FOUR_GROUP_ANIM "game_scene/fish/special_fish_alive/afish_102.csb"
#define ALL_NET_ANIM "game_scene/fish/special_fish_alive/afish_100.csb"
#define BOMB_TYPE_FISH_ANIM "game_scene/fish/special_fish_alive/afish_103.csb"
#define BOMB_RAID_ANIM "game_scene/right_menu/Raid.csb"

#define ENTER_MARY_UI "game_scene/game_effect/game_effect/EnterMary.csb"
#define CHANGE_WEAPON_UI "game_scene/gamer_ui/settings/WeaponShow.csb"
#define LOTTERY_UI "game_scene/gamer_ui/lucky_draw/Lottery.csb"
#define SETTINGS_UI "game_scene/gamer_ui/settings/SettingLayer.csb"
#define GAME_LEFT_MENU_UI "game_scene/gamer_ui/mianscence/LeftMenu.csb"
#define GAME_TOP_MENU_UI "game_scene/gamer_ui/mianscence/TopMenu.csb"
#define GAME_BOTTOM_MENU_UI "game_scene/gamer_ui/user/BottomBar.csb"
#define LUCKY_DRAW_UI "game_scene/gamer_ui/lucky_draw/LuckyCardLayer.csb"
#define SHOW_BOSS_UI "game_scene/gamer_ui/lucky_draw/BossShow.csb"
#define MARY_UI "game_scene/gamer_ui/mary/Mary.csb"
#define MARY_SCROLL_UI "game_scene/gamer_ui/mary/MaryScroll.csb"
#define NIUNIU_UI "game_scene/gamer_ui/niuniu/NiuDialog.csb"
#define SHOW_FISH_UI "game_scene/gamer_ui/settings/FishShow.csb"
#define FISH_CARD_UI "game_scene/gamer_ui/settings/FishCard.csb"
#define TASK_BOUND_UI "game_scene/gamer_ui/mianscence/TaskBoundLayer.csb"
#define UNLOCK_CANNON_UI "game_scene/gamer_ui/mianscence/UnlockCannon.csb"
#define BENEFITS_NORMAL_UI "hall_scene/benefits/BenefitsNormal.csb"
#define BENEFITS_ONLINE_UI "hall_scene/benefits/BenefitsNet.csb"
#define DIAMOND_RECHARGE_UI "hall_scene/recharge/DiamondRecharge.csb"
#define GOLD_RECHARGE_UI "hall_scene/recharge/GoldRecharge.csb"
#define DIALY_UI "hall_scene/journal/Journal.csb"
#define DIALY_CARD_UI "hall_scene/journal/JournalCard.csb"
#define GAME_LOADING_UI "hall_scene/loading/Loading.csb"
#define HALL_STAGE_UI "hall_scene/game_hall/HallStageLayer.csb"
#define HALL_TOP_UI "hall_scene/game_hall/HallTop.csb"
#define HALL_LEFT_UI "hall_scene/game_hall/HallLeft.csb"
#define HALL_RIGHT_UI "hall_scene/game_hall/HallRight.csb"
#define STAGE_CARD_UI "hall_scene/fish_room/StageCard.csb"
#define HALL_SOUND_TIP_UI "hall_scene/game_hall/SoundTip.csb"
#define HALL_SHARE_UI "hall_scene/freegold/FreeGold.csb"
#define HALL_VIP_GIFT_UI "hall_scene/recharge/VipGift.csb"
#define HALL_VIP_INTRO_UI "hall_scene/recharge/VipIntro.csb"
#define HALL_EVALUATE_UI "hall_scene/comment/Eveluate.csb"

#endif
