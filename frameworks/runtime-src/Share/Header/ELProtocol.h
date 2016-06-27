//
//  ELProtocol.h
//  simple-son
//
//  Created by ff on 15/10/22.
//
//

#ifndef simple_son_ELProtocol_h
#define simple_son_ELProtocol_h

//typedef unsigned long long int uint64_t;
#define GOLD_STAGE_COUNT 18
enum E2LProtocol
{
    e2l_login,                              //  在账号密码输入界面上，点击登录按钮后，调用此协议
    e2l_enter_game,                         //  选择关卡进入游戏
    e2l_sel_gold,                           //  选择黄金矿工某关
    e2l_gold_diff,                          //  选择黄金矿工难度
    e2l_gold_start_diff,                    //  黄金矿工使用某难度
    e2l_gold_info,                          //  点击介绍按钮
    e2l_gold_stage_box,                     //  开启黄金矿工关卡宝藏
    e2l_gold_start_runner,                  //  开启命运转盘
    e2l_gold_fate_box,                      //  开启黄金矿工命运转盘宝藏
    e2l_gold_start_game,                    //  开始黄金矿工某关
    e2l_gold_back_diff,                     //  返回难度选择
    e2l_gold_back_stage,                    //  返回黄金矿工关卡选择
    e2l_gold_open_stage_box,
    e2l_gold_open_fate_box,
    e2l_gold_open_fate_roll,
    e2l_gold_result,                        //  黄金矿工关卡结果确认
    e2l_gold_gain_obj,
    
    e2l_remove_out_fish,                    //  删除超出屏幕的鱼
    e2l_tide_switch,                        //  完成潮汐推浪
//    e2l_click_emit_fish,                    //  点击测试创建鱼
    e2l_change_cannon,                      //  换炮
    e2l_change_cannon_multi,                //  加减倍率
    e2l_cannon_fire,                        //  请求开炮
    e2l_fish_dead,                          //  鱼死亡动画放完
    e2l_fish_light_dead,                    //  电死的鱼转死亡状态
    e2l_choose_cannon,                      //  选炮
//    e2l_auto_fire,                          //  自动开炮
    
    e2l_use_skill,                          //  使用技能
    e2l_fish_all_remove,                    //  显示层没有鱼了
    
    e2l_niuniu_splash_over,                  //  进入牛牛游戏前的效果播放完
    e2l_niuniu_ask_showHand,                   //  请求牛牛结果
    e2l_niuniu_end,                         //  牛牛游戏结束
    
    e2l_mary_splash_over,                   //  进入玛丽游戏前的效果播放完
    e2l_mary_init,                          //  请求获得玛丽的局数据
    e2l_mary_exit,                          //  退出玛丽界面
    
    e2l_cannon_unlock_level,                      //  请求解锁炮台倍数
    e2l_charge_unlock_level,                      //  充值并解锁炮台倍数请求
    e2l_cannon_hide_unlock_level,                 //  退出解锁炮台倍数
    
    e2l_show_settings,                      //  进入设定界面
    e2l_exit_settings,                      //  退出设定界面
    
    e2l_boss_task_bounds,                   //  领取通关奖励
    e2l_take_task_bounds,                   //  领取任务奖励
    e2l_show_task_bounds,                   //  请求任务奖励界面
    e2l_hide_task_bounds,                   //  退出任务奖励界面
    
    e2l_show_lucky_draw,                    //  请求进入幸运抽奖界面
    e2l_end_lucky_draw,                     //  请求退出幸运抽奖界面
    e2l_exit_lucky_draw,                    //  关闭幸运抽奖界面
    e2l_update_lucky_draw,                  //  请求更新到某档抽奖界面
    e2l_show_lottery,                       //  请求进入最终抽奖界面
    e2l_end_lottery,                        //  请求退出最终抽奖界面
//    e2l_choose_lottery_card,                //
    
    e2l_next_stage,                         //  要进入下一关
    e2l_quit_stage,                         //  退出关卡
    
    e2l_show_diary,                         //  显示捕鱼日志
    e2l_show_fish,                          //  显示鱼的倍数界面
    e2l_show_cannon_setting,                //  显示换炮界面
    e2l_cannon_upgrade_vip,                 //  换炮请求升级VIP
    
    e2l_receive_drop_item,                  //  获得掉落物品
    e2l_take_relife,                        //  领取游戏内救济金
    
    e2l_unlock_to_require,                  //  解锁炮台到要求的倍率
    e2l_hide_multi_not_enough,              //  关闭倍率提示窗口
    
    e2l_show_hall_recharge,                 //  显示大厅的金币充值界面
    e2l_show_vip_intro,                     //  显示VIP介绍界面
    e2l_show_vip_gift,                      //  显示VIP礼包界面
    e2l_show_vip,                           //  点击VIP礼包，由vip等级决定显示什么界面
    e2l_recharge,                           //  充值
    e2l_check_auto_fire,                    //  检查自动发炮功能是否可用
    e2l_close_recharge,                     //  关闭充值界面
    e2l_show_game_recharge,                 //  显示游戏内充值界面
    
    e2l_show_benefits,                      //  显示救济金界面
    e2l_cash_benefit_code,                  //  兑救济金码
    e2l_link_to_netgame,                    //  跳转网页
    
    e2l_show_share,                         //  显示分享界面
    e2l_share_count_bound,                  //  领取分享次数奖励
    e2l_share,                              //  分享
    e2l_max
};

enum L2EProtocol
{
    l2e_load_login,
    l2e_load_login_update,
    l2e_login,
    l2e_login_result,
    l2e_to_hall,                           // 逻辑层通知引擎层进入大厅
    l2e_enter_game,
    l2e_show_gold_diff,                     //  显示黄金矿工难度选择
    l2e_update_gold_diff,
    l2e_show_gold_runner,                   //  显示黄金矿工赛前转盘
    l2e_update_gold_runner,
    l2e_start_gold_roll,
    l2e_show_gold_info,                     //  显示黄金矿工帮助
    l2e_show_sel_gold,                      //  显示选择黄金矿工关卡
    l2e_update_sel_gold,                    //  更新选择黄金矿工关卡
    l2e_load_gold,                          //  加载黄金矿工关卡
    l2e_enter_goldgame,                     //  进入黄金矿工游戏
    l2e_result_goldgame,                    //  黄金矿工结果
    l2e_open_box,                           //  开启宝箱
    
    l2e_create_fish,                        //  创建鱼
    l2e_schedule_tide,                      //  开启潮汐开始倒计时
    l2e_tide_come,                          //  潮汐来临提示并清场
    l2e_tide_switch,                        //  推场景
    
    l2e_boss_come,                          //  BOSS来袭
    
    l2e_update_cannon,                      //  更新鱼炮
    l2e_cannon_fire,                        //  开炮
    l2e_create_cannon,                      //  创建炮
    l2e_cast_net,                           //  撒网
    l2e_change_cannon,                      //  换炮
    l2e_change_cannon_mul,                  //  改倍率
    
    l2e_start_energy_cannon,                //  开启能量炮
    l2e_end_energy_cannon,                  //  关闭能量炮
    
    l2e_ready_ray,                          //  准备发射激光炮
    l2e_end_ray,                            //  激光炮发射完成
    l2e_ready_plane,                        //  飞机轰炸准备
    l2e_end_plane,                          //  飞机轰炸完成
    
    l2e_niuniu_splash,                      //  进入牛牛游戏前的动画和效果
    l2e_enter_niuniu,                       //  进入牛牛游戏界面
    l2e_niuniu_showhand,                    //  通知牛牛结果
    l2e_hide_niuniu,                        //  退出牛牛界面
    
    l2e_mary_splash,                        //  进入马莉游戏前的动画和效果
    l2e_enter_mary,                         //  进入马莉机界面
    l2e_mary_init,                          //  返回马莉一局的牌型
    l2e_hide_mary,                          //  退出马莉界面
    
    l2e_cannon_max_level,                   //  通知界面已解锁最大倍率
    l2e_cannon_update_level,                //  通知界面更新解锁倍率显示
    l2e_cannon_unlock_level,                //  解锁倍率炮的返回
    l2e_cannon_show_unlock_dialog,          //  显示解锁倍率炮对话界面
    l2e_cannon_hide_unlock_dialog,          //  退出解锁倍率炮对话界面
    l2e_cannon_unlock_need_diamond,         //  显示解锁时钻石不足提示
    l2e_charge_unlock_level,                //  充值并解锁倍率成功返回
    l2e_hide_cannon_unlock,                 //  隐藏解锁倍率按钮
    
    l2e_update_gold,                        //  更新金币数量
    l2e_update_diamond,                     //  更新钻石数量
    l2e_update_energy,                      //  更新能量槽
    l2e_update_headInfo,                    //  更新经验、等级、vip、称号显示
    l2e_update_skill,                       //  更新技能数量
    l2e_use_skill,                          //  使用技能
    l2e_skill_charge,                       //  技能剩余次数和钻石都不足，需要调出充值对话框
    l2e_charge_use_skill,                   //  通过充钻石，使用技能
    l2e_levelup_tip,                        //  升级提示
    
    l2e_show_setting,                       //  显示设置界面
    l2e_exit_setting,                       //  退出设置界面
    
    l2e_init_boss_task,                     //  初始化通关任务界面
    l2e_update_boss_task,                   //  更新通关任务界面
    l2e_complete_boss_task,                 //  完成通关任务调出界面
    l2e_hide_task,                          //  隐藏通关任务界面
    
    l2e_init_task,                          //  初始化任务界面
    l2e_complete_sub_task,                  //  完成子任务给奖励
    l2e_update_task,                        //  更新任务界面
    l2e_show_task_bound,                    //  弹出领取任务奖励界面
    l2e_take_task_bound,                    //  领取任务奖励
    l2e_hide_task_bound,                    //  退出任务奖励界面
    
    l2e_update_big_star,                    //  更新大鱼星数与宝箱状态
    l2e_show_lucky_draw,                    //  进入幸运抽奖界面
    l2e_end_lucky_draw,                     //  退出幸运抽奖界面
    l2e_exit_lucky_draw,                    //  关闭幸运抽奖界面
    l2e_update_lucky_draw,                  //  更新某档抽奖数据
    l2e_show_lottery,                       //  进入最终抽奖界面
    l2e_end_lottery,                        //  退出最终抽奖界面
    
    l2e_tide_come_disable_skill,            //  潮汐来临前禁用技能
    
    l2e_enter_load_scene,                   //  进入关卡加载界面
    l2e_update_load_stage,                  //  更新关卡加载进度
    
    l2e_show_diary,                         //  显示捕鱼日志
    l2e_show_fish,                          //  显示鱼的倍数界面
    l2e_show_cannon_setting,                //  显示换炮界面
    l2e_update_cannon_setting,              //  更新换炮界面
    l2e_drop_item,                          //  掉落物品
    
    l2e_update_relife,                      //  更新游戏内救济领取按钮
    l2e_cannon_multi_not_enough,            //  炮倍率不足关卡最低倍率要求
    l2e_hide_multi_not_enough,              //  退出最低倍率提示
    
    l2e_lack_gold,                          //  发炮金币不足
    
    l2e_show_hall_recharge,                 //  显示大厅金币充值界面
    l2e_show_vip_info,                      //  显示vip介绍界面
    l2e_show_vip_gift,                      //  显示vip礼包界面
    l2e_ray_need_vip,                       //  显示激光炮需要4级vip提示
    l2e_bomb_need_unlock,                   //  显示炸弹需要解锁炮提示
    l2e_auto_fire_need_vip,                 //  显示自动发炮需要vip提示
    l2e_check_auto_fire,                    //  可以自动发炮
    l2e_update_hall_money,                  //  更新大厅界面的金币和钻石
    l2e_recharge_ok,                        //  充值成功
    l2e_close_recharge,                     //  关闭充值界面
    
    l2e_show_benefits,                      //  显示救济金界面
    l2e_cash_benefit_code,                  //  救济金兑奖结果
    
    l2e_show_share,                         //  显示分享界面
    l2e_share_count_bound,                  //  领取分享次数奖励
    l2e_update_share,                       //  更新分享界面
    l2e_share_result,                       //  分享结果
    l2e_max
};


typedef struct __E2L_LOGIN
{
    E2LProtocol     eProtocol;
    std::string     account;
    std::string     password;
}E2L_LOGIN;

typedef struct __E2L_ENTERGAME
{
    E2LProtocol     eProtocol;
    int             stage;
}E2L_ENTERGAME;

typedef struct __E2L_OPEN_BOX
{
    E2LProtocol     eProtocol;
}E2L_OPEN_BOX;

typedef struct __L2E_START_ROLL {
    L2EProtocol     eProtocol;
    int             itemType;
}L2E_START_ROLL;

typedef struct __L2E_OPEN_BOX {
    L2EProtocol     eProtocol;
    int             money;
    int             itemType;
    int             errorCode;
}L2E_OPEN_BOX;

typedef struct __L2E_GOLD_DIFF
{
    L2EProtocol     eProtocol;
    int             diff;
    int             timeLimit;
    int             validCount;
    int             fee;
    int             errCode;
}L2E_GOLD_DIFF;

typedef struct __E2L_GOLD_DIFF
{
    E2LProtocol     eProtocol;
    int             diff;
}E2L_GOLD_DIFF;

typedef struct __E2L_GOLD_SEL_DIFF
{
    E2LProtocol     eProtocol;
    int             diff;
}E2L_GOLD_SEL_DIFF;

typedef struct __E2L_GOLD_BACK
{
    E2LProtocol     eProtocol;
}E2L_GOLD_BACK;

typedef struct __E2L_GOLD_START_ROLL
{
    E2LProtocol     eProtocol;
}E2L_GOLD_START_ROLL;

typedef struct __E2L_GOLD_START_GAME
{
    E2LProtocol     eProtocol;
}E2L_GOLD_START_GAME;

typedef struct __E2L_GOLD_GAIN_OBJ
{
    E2LProtocol     eProtocol;
}E2L_GOLD_GAIN_OBJ;

typedef struct __E2L_GOLD_RESULT
{
    E2LProtocol     eProtocol;
}E2L_GOLD_RESULT;

typedef struct __L2E_GOLD_RESULT
{
    L2EProtocol     eProtocol;
    int             moneyResult;
    bool            success;
}L2E_GOLD_RESULT;

typedef struct __L2E_UPDATE_SELGOLD
{
    L2EProtocol     eProtocol;
    int             lastStage;
    int             boxState;
    bool            hasFocus;
    int             errorCode;
}L2E_UPDATE_SELGOLD;

typedef struct __L2E_UPDATE_GOLD_RUNNER
{
    L2EProtocol     eProtocol;
    int             keyCount;
    int             boxState;
    bool            hasRolled;
    int             errorCode;
}L2E_UPDATE_GOLD_RUNNER;

typedef struct __E2L_SEL_GOLD
{
    E2LProtocol     eProtocol;
    int             selStage;
}E2L_SEL_GOLD;

typedef struct __L2E_LOAD_GOLD_UPDATE
{
    L2EProtocol     eProtocol;
    bool isOver;
    int step;
    int count;
}L2E_LOAD_GOLD_UPDATE;

typedef struct __L2E_ENTER_GOLD_GAME
{
    L2EProtocol     eProtocol;
    int weaponId;
    int missionType;
    int missionGold;
    int missionKeyCount;
    int timeLimit;
}L2E_ENTER_GOLD_GAME;

typedef struct __L2E_ENTERGAME
{
    L2EProtocol     eProtocol;
    int             scnenId;
}L2E_ENTERGAME;

typedef struct __LOAD_LOGIN
{
    L2EProtocol eProtocol;
}LOAD_LOGIN;

typedef struct __LOAD_LOGIN_UPDATE
{
    L2EProtocol eProtocol;
    bool isOver;
    int step;
    int count;
}LOAD_LOGIN_UPDATE;

typedef struct __L2E_UPDATE_LOAD_STAGE
{
    L2EProtocol eProtocol;
    bool isOver;
    int step;
    int count;
}L2E_UPDATE_LOAD_STAGE;

typedef struct __L2E_ENTER_LOAD_SCENE
{
    L2EProtocol eProtocol;
}L2E_ENTER_LOAD_SCENE;

typedef struct __L2E_TO_HALL
{
    L2EProtocol                     eProtocol;
    int                             diamond;
    uint64_t                        gold;
    int                             level;
    int                             exp;
    int                             maxExp;
    int                             maxUnlockStage;
    int                             maxStage;
    int                             currStage;
    std::string                     levelBossRes[36];
    int                             levelBossType[36];
    int                             levelBoundGold[36];
    int                             levelBoundDiamond[36];
}L2E_TO_HALL;

typedef struct __L2E_LOGIN_RESULTE
{
    L2EProtocol eProtocol;
    bool success;
}L2E_LOGIN_RESULTE;

typedef struct __L2E_CREATE_FISH
{
    L2EProtocol     eProtocol;
    int             fishId;
}L2E_CREATE_FISH;

typedef struct __E2L_REMOVE_OUT_FISH
{
    E2LProtocol     eProtocol;
    int             fishId;
}E2L_REMOVE_OUT_FISH;

typedef struct __L2E_SCHEDULE_TIDE
{
    L2EProtocol     eProtocol;
    float           startInterval;
}L2E_SCHEDULE_TIDE;

typedef struct __L2E_TIDE_COME_TIP
{
    L2EProtocol     eProtocol;
}L2E_TIDE_COME_TIP;

typedef struct __L2E_BOSS_COME_TIP
{
    L2EProtocol     eProtocol;
    int             bossType;
    std::string     bossRes;
}L2E_BOSS_COME_TIP;


typedef struct __L2E_TIDE_SWITCH
{
    L2EProtocol     eProtocol;
    int             sceneId;
}L2E_TIDE_SWITCH;

typedef struct __E2L_TIDE_SWITCH
{
    E2LProtocol     eProtocol;
}E2L_TIDE_SWITCH;

typedef struct __L2E_UPDATE_CANNON
{
    L2EProtocol     eProtocol;
    int             cannonType;
    int             multiple;
}L2E_UPDATE_CANNON;

typedef struct __E2L_CHANGE_CANNON
{
    E2LProtocol     eProtocol;
    int             cannonType;
}E2L_CHANGE_CANNON;

typedef struct __L2E_CHANGE_CANNON
{
    L2EProtocol     eProtocol;
    int             cannonType;
}L2E_CHANGE_CANNON;

typedef struct __E2L_CHANGE_CANNON_MULTIPLY
{
    E2LProtocol     eProtocol;
    int             val;
}E2L_CHANGE_CANNON_MULTIPLY;

typedef struct __L2E_CHANGE_CANNON_MULTIPLY
{
    L2EProtocol     eProtocol;
    int             multiply;
    int             bulletCount;
}L2E_CHANGE_CANNON_MULTIPLY;

typedef struct __E2L_CANNON_FIRE
{
    E2LProtocol     eProtocol;
    float           angle;
    int             traceFishId;
    float           posX;
    float           posY;
}E2L_CANNON_FIRE;

typedef struct __L2E_CANNON_FIRE
{
    L2EProtocol     eProtocol;
    int             bulletId;

}L2E_CANNON_FIRE;

typedef struct __L2E_CREATE_CANNON
{
    L2EProtocol     eProtocol;
    int             cannonType;
    int             multiple;
    int             bulletCount;
    bool            isHero;
    int             angle;
    int             chairId;
}L2E_CREATE_CANNON;

typedef struct __L2E_CAST_NET
{
    L2EProtocol     eProtocol;
    float           posX;
    float           posY;
    int             bulletId;
    float           cannonType;
    float           bulletCount;
}L2E_CAST_NET;

typedef struct __E2L_FISH_DEAD
{
    E2LProtocol     eProtocol;
    int             fishId;
}E2L_FISH_DEAD;

typedef struct __E2L_FISH_LIGHT_DEAD
{
    E2LProtocol     eProtocol;
    int             fishId;
}E2L_FISH_LIGHT_DEAD;

typedef struct __E2L_CHOOSE_CANNON
{
    E2LProtocol     eProtocol;
    int             cannonType;
}E2L_CHOOSE_CANNON;

typedef struct __L2E_START_ENERGY
{
    L2EProtocol     eProtocol;
}L2E_START_ENERGY;

typedef struct __L2E_END_ENERGY
{
    L2EProtocol     eProtocol;
    int             cannonType;
}L2E_END_ENERGY;

typedef struct __E2L_USE_SKILL
{
    E2LProtocol     eProtocol;
    int             skillId;
}E2L_USE_SKILL;

typedef struct __L2E_READY_RAY
{
    L2EProtocol     eProtocol;
}L2E_READY_RAY;

typedef struct __L2E_END_RAY
{
    L2EProtocol     eProtocol;
}L2E_END_RAY;

typedef struct __L2E_READY_PLANE
{
    L2EProtocol     eProtocol;
}L2E_READY_PLANE;

typedef struct __L2E_END_PLANE
{
    L2EProtocol     eProtocol;
}L2E_END_PLANE;

typedef struct __E2L_FISH_ALL_REMOVE
{
    E2LProtocol     eProtocol;
}E2L_FISH_ALL_REMOVE;

typedef struct __L2E_ENTER_NIUNIU {
    L2EProtocol     eProtocol;
    int             cannonMultiply;
    int             cards[5];
}L2E_ENTER_NIUNIU;

typedef struct __L2E_HIDE_NIUNIU {
    L2EProtocol     eProtocol;
}L2E_HIDE_NIUNIU;

typedef struct __E2L_NIUNIU_ASK_SHOWHAND
{
    E2LProtocol     eProtocol;
}E2L_NIUNIU_ASK_SHOWHAND;

typedef struct __L2E_NINIU_REQ_SHOWHAND
{
    L2EProtocol     eProtocol;
    int             result;
    int             cardType;
}L2E_NINIU_REQ_SHOWHAND;

typedef struct __E2L_NIUNIU_END
{
    E2LProtocol     eProtocol;
    int             result;
}E2L_NIUNIU_END;

typedef struct __L2E_ENTER_MARY {
    L2EProtocol                 eProtocol;
    int                         roundIndex;
    int                         roundTypeId;
    int                         centerIndex[4];
    int                         cardType;
    int                         stageScore;
    int                         stageDiamond;
    uint64_t                    totalScore;
    int                         cannonMulti;
}L2E_ENTER_MARY;

typedef struct __L2E_HIDE_MARY {
    L2EProtocol     eProtocol;
}L2E_HIDE_MARY;

typedef struct __E2L_MARY_INIT
{
    E2LProtocol     eProtocol;
}E2L_MARY_INIT;

typedef struct __E2L_MARY_EXIT
{
    E2LProtocol     eProtocol;
}E2L_MARY_EXIT;

typedef struct __L2E_MARY_INIT {
    L2EProtocol                 eProtocol;
    int                         roundIndex;
    int                         roundTypeId;
    int                         centerIndex[4];
    int                         cardType;
    int                         stageScore;
    int                         stageDiamond;
    uint64_t                    totalScore;
}L2E_MARY_INIT;

typedef struct __L2E_CANNON_MAX_LEVEL
{
    L2EProtocol     eProtocol;
}L2E_CANNON_MAX_LEVEL;

typedef struct __L2E_CANNON_UPDATE_LEVEL{
    L2EProtocol     eProtocol;
    int             totalDiamond;
    int             nextMultiply;
    int             needDiamond;
    int             boundGolds;
    bool            canUnlock;
}L2E_CANNON_UPDATE_LEVEL;

typedef struct __L2E_CANNON_UNLOCK_LEVEL
{
    L2EProtocol     eProtocol;
    int             errNo;
    int             totalDiamond;
    int             nextMultiply;
    int             needDiamond;
    int             boundGolds;
    bool            canUnlock;
//    int             cannonMultiply;
}L2E_CANNON_UNLOCK_LEVEL;

typedef struct __E2L_CANNON_UNLOCK_LEVEL
{
    E2LProtocol     eProtocol;
}E2L_CANNON_UNLOCK_LEVEL;

typedef struct __E2L_CANNON_HIDE_UNLOCK_LEVEL
{
    E2LProtocol     eProtocol;
}E2L_CANNON_HIDE_UNLOCK_LEVEL;

typedef struct __L2E_CANNON_SHOW_UNLOCK_DIALOG
{
    L2EProtocol     eProtocol;
    
    int             cardCount;
    int             nextMultiply[5];
    int             needDiamond[5];
    int             boundGolds[5];
    //    int             cannonMultiply;
}L2E_CANNON_SHOW_UNLOCK_DIALOG;

typedef struct __L2E_CANNON_HIDE_UNLOCK_DIALOG
{
    L2EProtocol     eProtocol;
}L2E_CANNON_HIDE_UNLOCK_DIALOG;

typedef struct __E2L_CHARGE_UNLOCK_LEVEL
{
    E2LProtocol     eProtocol;
//    int             chargeId;
}E2L_CHARGE_UNLOCK_LEVEL;

typedef struct __L2E_CHARGE_UNLOCK_LEVEL
{
    L2EProtocol     eProtocol;
    int             bound;
}L2E_CHARGE_UNLOCK_LEVEL;

typedef struct __L2E_HIDE_CANNON_UNLOCK
{
    L2EProtocol     eProtocol;
}L2E_HIDE_CANNON_UNLOCK;

typedef struct __L2E_UPDATE_GOLD
{
    L2EProtocol             eProtocol;
    uint64_t                gold;
}L2E_UPDATE_GOLD;

typedef struct __L2E_UPDATE_DIAMOND
{
    L2EProtocol     eProtocol;
    int             diamond;
}L2E_UPDATE_DIAMOND;

typedef struct __L2E_UPDATE_ENERGY
{
    L2EProtocol     eProtocol;
    int             currVal;
    int             maxVal;
}L2E_UPDATE_ENERGY;

typedef struct __L2E_UPDATE_SKILL
{
    L2EProtocol     eProtocol;
    int             skillCount[5];
    int             skillPrice[5];
}L2E_UPDATE_SKILL;

typedef struct __L2E_USE_SKILL
{
    L2EProtocol     eProtocol;
    int             useSkillId;
    float           cd;
}L2E_USE_SKILL;

typedef struct __L2E_UPDATE_HEADINFO {
    L2EProtocol     eProtocol;
    int             currExp;
    int             maxExp;
    std::string     title;
    int             vip;
    int             level;
}L2E_UPDATE_HEADINFO;

typedef struct __L2E_LEVELUP_TIP {
    L2EProtocol     eProtocol;
}L2E_LEVELUP_TIP;

typedef struct __E2L_SHOW_SETTING
{
    E2LProtocol     eProtocol;
}E2L_SHOW_SETTING;

typedef struct __E2L_EXIT_SETTING
{
    E2LProtocol     eProtocol;
}E2L_EXIT_SETTING;

typedef struct __L2E_SHOW_SETTING
{
    L2EProtocol     eProtocol;
}L2E_SHOW_SETTING;

typedef struct __L2E_EXIT_SETTING
{
    L2EProtocol     eProtocol;
}L2E_EXIT_SETTING;

typedef struct __L2E_COMPLETE_BOSS_TASK
{
    L2EProtocol     eProtocol;
    int             boundGold;
    int             boundDiamond;
}L2E_COMPLETE_BOSS_TASK;

typedef struct __L2E_UPDATE_BOSS_TASK
{
    L2EProtocol     eProtocol;
    int             targetState;
    int             bossCount;
    int             bossMaxCount;
    int             subBossCount;
    int             subBossMaxCount;
}L2E_UPDATE_BOSS_TASK;

typedef struct __L2E_INIT_BOSS_TASK
{
    L2EProtocol     eProtocol;
    int             boundGold;
    int             boundDiamond;
    int             bossType;
    std::string     bossRes;
    int             bossCount;
    int             bossMaxCount;
    int             subBossType;
    std::string     subBossRes;
    int             subBossCount;
    int             subBossMaxCount;
}L2E_INIT_BOSS_TASK;

typedef struct __E2L_TAKE_BOSS_BOUND
{
    E2LProtocol     eProtocol;
}E2L_TAKE_BOSS_BOUND;

typedef struct __L2E_INIT_TASK
{
    L2EProtocol     eProtocol;
    std::string     fishRes[3];
    int             fishType[3];
    int             fishCount[3];
    int             fishMaxCount[3];
}L2E_INIT_TASK;

typedef struct __L2E_COMPLETE_SUB_TASK {
    L2EProtocol     eProtocol;
    int             gold;
}L2E_COMPLETE_SUB_TASK;

typedef struct __L2E_UPDATE_TASK
{
    L2EProtocol     eProtocol;
    int             taskState;
    int             fishCount[3];
    int             fishMaxCount[3];
}L2E_UPDATE_TASK;

typedef struct __L2E_SHOW_TASK_BOUND
{
    L2EProtocol     eProtocol;
    int             taskState;
    int             boundGold;
    int             boundDiamond;
    std::string     fishRes[3];
    int             fishType[3];
    bool            complete[3];
    int             fishMaxCount[3];
    int             fishGold[3];
    
}L2E_SHOW_TASK_BOUND;

typedef struct __E2L_SHOW_TASK_BOUND
{
    E2LProtocol     eProtocol;
}E2L_SHOW_TASK_BOUND;

typedef struct __E2L_HIDE_TASK_BOUND
{
    E2LProtocol     eProtocol;
}E2L_HIDE_TASK_BOUND;

typedef struct __E2L_TAKE_TASK_BOUND
{
    E2LProtocol     eProtocol;
}E2L_TAKE_TASK_BOUND;

typedef struct __L2E_TAKE_TASK_BOUND
{
    L2EProtocol     eProtocol;
}L2E_TAKE_TASK_BOUND;

typedef struct __L2E_HIDE_TASK_BOUND
{
    L2EProtocol     eProtocol;
}L2E_HIDE_TASK_BOUND;

typedef struct __L2E_SKILL_CHARGE
{
    L2EProtocol     eProtocol;
    int             skillId;
}L2E_SKILL_CHARGE;

typedef struct __L2E_CHARGE_USE_SKILL
{
    L2EProtocol     eProtocol;
    int             useSkillId;
}L2E_CHARGE_USE_SKILL;

typedef struct __L2E_TIDE_COME_DISABLE_SKILL
{
    L2EProtocol     eProtocol;
}L2E_TIDE_COME_DISABLE_SKILL;

typedef struct __L2E_MARY_SPLASH
{
    L2EProtocol     eProtocol;
}L2E_MARY_SPLASH;

typedef struct __L2E_NIUNIU_SPLASH
{
    L2EProtocol     eProtocol;
}L2E_NIUNIU_SPLASH;

typedef struct __E2L_MARY_SPLASH_OVER
{
    E2LProtocol     eProtocol;
}E2L_MARY_SPLASH_OVER;

typedef struct __E2L_NIUNIU_SPLASH_OVER
{
    E2LProtocol     eProtocol;
}E2L_NIUNIU_SPLASH_OVER;

typedef struct __E2L_SHOW_LUCKY_DRAW
{
    E2LProtocol    eProtocol;
}E2L_SHOW_LUCKY_DRAW;

typedef struct __L2E_SHOW_LUCKY_DRAW
{
    L2EProtocol     eProtocol;
    int             level;
    int             killScore;
    int             maxScore;
    int             showType;
    int             boundtypes[6];
    int             boundIds[6];
    int             boundCount[6];
    std::string     nextTitle;
}L2E_SHOW_LUCKY_DRAW;

typedef struct __E2L_UPDATE_LUCKY_DRAW
{
    E2LProtocol     eProtocol;
    int             level;
}E2L_UPDATE_LUCKY_DRAW;

typedef struct __L2E_UPDATE_LUCKY_DRAW
{
    L2EProtocol     eProtocol;
    int             level;
    int             killScore;
    int             maxScore;
    int             showType;
    int             boundtypes[6];
    int             boundIds[6];
    int             boundCount[6];
    std::string     nextTitle;
}L2E_UPDATE_LUCKY_DRAW;

typedef struct __E2L_END_LUCKY_DRAW
{
    E2LProtocol    eProtocol;
}E2L_END_LUCKY_DRAW;
typedef struct __E2L_EXIT_LUCKY_DRAW
{
    E2LProtocol    eProtocol;
}E2L_EXIT_LUCKY_DRAW;

typedef struct __L2E_END_LUCKY_DRAW
{
    L2EProtocol     eProtocol;
}L2E_END_LUCKY_DRAW;
typedef struct __L2E_EXIT_LUCKY_DRAW
{
    L2EProtocol     eProtocol;
}L2E_EXIT_LUCKY_DRAW;

typedef struct __E2L_SHOW_LOTTERY
{
    E2LProtocol    eProtocol;
}E2L_SHOW_LOTTERY;

typedef struct __L2E_SHOW_LOTTERY
{
    L2EProtocol     eProtocol;
    int             level;
    int             boundtypes[6];
    int             boundIds[6];
    int             boundCount[6];
    
    int             result[6];
}L2E_SHOW_LOTTERY;

typedef struct __E2L_END_LOTTERY
{
    E2LProtocol    eProtocol;
    int             selFocus;
}E2L_END_LOTTERY;

typedef struct __L2E_END_LOTTERY
{
    L2EProtocol     eProtocol;
}L2E_END_LOTTERY;

typedef struct __L2E_UPDATE_BIG_STAR
{
    L2EProtocol     eProtocol;
    int             starCount;
    bool            boxEnable;
}L2E_UPDATE_BIG_STAR;

typedef struct __E2L_NEXT_STAGE
{
    E2LProtocol     eProtocol;
}E2L_NEXT_STAGE;

typedef struct __E2L_QUIT_STAGE
{
    E2LProtocol     eProtocol;
}E2L_QUIT_STAGE;

typedef struct __E2L_SHOW_DIARY
{
    E2LProtocol     eProtocol;
}E2L_SHOW_DIARY;

typedef struct __L2E_SHOW_DIARY
{
    L2EProtocol     eProtocol;
    int             fishTypeCount;
    int             killCount[100];
    std::string     killTitle[100];
    std::string     killRes[100];
    int             killFishType[100];
}L2E_SHOW_DIARY;

typedef struct __L2E_DROP_ITEM
{
    L2EProtocol     eProtocol;
    int             dropIncreaceId;
    int             dropType;
    int             dropId;
    int             dropCount;
    float           posX;
    float           posY;
}L2E_DROP_ITEM;

typedef struct __E2L_RECEIVE_DROP_ITEM
{
    E2LProtocol     eProtocol;
    int             dropIncreaceId;
}E2L_RECEIVE_DROP_ITEM;

typedef struct __E2L_SHOW_FISH
{
    E2LProtocol     eProtocol;
}E2L_SHOW_FISH;

typedef struct __L2E_SHOW_FISH
{
    L2EProtocol     eProtocol;
    int             fishTypeCount;
    int             fishType[100];
    std::string     fishRes[100];
    int             fishPrice[100];
}L2E_SHOW_FISH;

typedef struct __E2L_SHOW_CANNON_SETTING
{
    E2LProtocol     eProtocol;
}E2L_SHOW_CANNON_SETTING;

typedef struct __L2E_SHOW_CANNON_SETTING
{
    L2EProtocol     eProtocol;
    int             focus;
    bool            holdCannon[10];
}L2E_SHOW_CANNON_SETTING;

typedef struct __L2E_UPDATE_CANNON_SETTING
{
    L2EProtocol     eProtocol;
    bool            holdCannon[10];
}L2E_UPDATE_CANNON_SETTING;

typedef struct __E2L_CANNON_UPGRADE_VIP
{
    E2LProtocol     eProtocol;
}E2L_CANNON_UPGRADE_VIP;

typedef struct __L2E_UPDATE_RELIFE
{
    L2EProtocol     eProtocol;
    int             state;
    float           countDown;
    int             bound;
}L2E_UPDATE_RELIFE;

typedef struct __E2L_TAKE_RELIFE
{
    E2LProtocol     eProtocol;
}E2L_TAKE_RELIFE;

typedef struct __L2E_CANNON_MULTI_NOT_ENOUGH
{
    L2EProtocol     eProtocol;
    int             requireMultiply;
}L2E_CANNON_MULTI_NOT_ENOUGH;

typedef struct __E2L_UNLOCK_TO_REQUIRE
{
    E2LProtocol     eProtocol;
}E2L_UNLOCK_TO_REQUIRE;

typedef struct __E2L_HIDE_MULTI_NOT_ENOUGH
{
    E2LProtocol     eProtocol;
}E2L_HIDE_MULTI_NOT_ENOUGH;

typedef struct __L2E_HIDE_MULTI_NOT_ENOUGH
{
    L2EProtocol     eProtocol;
}L2E_HIDE_MULTI_NOT_ENOUGH;

typedef struct __L2E_LACK_GOLD
{
    L2EProtocol     eProtocol;
}L2E_LACK_GOLD;

typedef struct __E2L_SHOW_HALL_RECHARGE
{
    E2LProtocol     eProtocol;
    int             type;
}E2L_SHOW_HALL_RECHARGE;

typedef struct __L2E_SHOW_HALL_RECHARGE
{
    L2EProtocol     eProtocol;
    float           nextPercent;
    int             nextNeedMoney;
    bool            firstRecharge[12];
    int             vip;
    int             type;
}L2E_SHOW_HALL_RECHARGE;

typedef struct __E2L_SHOW_VIP_INTRO
{
    E2LProtocol     eProtocol;
}E2L_SHOW_VIP_INTRO;

typedef struct __L2E_SHOW_VIP_INTRO
{
    L2EProtocol     eProtocol;
    float           nextPercent;
    int             nextNeedMoney;
    int             vip;
}L2E_SHOW_VIP_INTRO;

typedef struct __E2L_SHOW_VIP_GIFT
{
    E2LProtocol     eProtocol;
}E2L_SHOW_VIP_GIFT;

typedef struct __L2E_SHOW_VIP_GIFT
{
    L2EProtocol     eProtocol;
}L2E_SHOW_VIP_GIFT;

typedef struct __E2L_SHOW_VIP
{
    E2LProtocol     eProtocol;
}E2L_SHOW_VIP;

typedef struct __E2L_RECHARGE
{
    E2LProtocol     eProtocol;
    int             rechargeId;
}E2L_RECHARGE;

typedef struct __E2L_SHOW_GAME_RECHARGE
{
    E2LProtocol     eProtocol;
    int             type;
}E2L_SHOW_GAME_RECHARGE;

typedef struct __L2E_RAY_NEED_VIP
{
    L2EProtocol     eProtocol;
    int             needVipLevel;
}L2E_RAY_NEED_VIP;

typedef struct __E2L_CHECK_AUTO_FIRE
{
    E2LProtocol     eProtocol;
}E2L_CHECK_AUTO_FIRE;

typedef struct __L2E_AUTO_FIRE_NEED_VIP
{
    L2EProtocol     eProtocol;
    int             needVipLevel;
}L2E_AUTO_FIRE_NEED_VIP;

typedef struct __L2E_CHECK_AUTO_FIRE
{
    L2EProtocol     eProtocol;
    int             errNo;
}L2E_CHECK_AUTO_FIRE;

typedef struct __L2E_BOMB_NEED_UNLOCK
{
    L2EProtocol     eProtocol;
}L2E_BOMB_NEED_UNLOCK;

typedef struct __L2E_UPDATE_HALL_MONEY
{
    L2EProtocol     eProtocol;
    uint64_t        gold;
    int             diamond;
}L2E_UPDATE_HALL_MONEY;

typedef struct __L2E_RECHARGE_OK
{
    L2EProtocol     eProtocol;
    float           nextPercent;
    int             nextNeedMoney;
    bool            firstRecharge[12];
    int             vip;
    int             type;
}L2E_RECHARGE_OK;

typedef struct __E2L_CLOSE_RECHARGE
{
    E2LProtocol     eProtocol;
}E2L_CLOSE_RECHARGE;

typedef struct __L2E_CLOSE_RECHARGE
{
    L2EProtocol     eProtocol;
}L2E_CLOSE_RECHARGE;

typedef struct __E2L_SHOW_BENEFITS
{
    E2LProtocol     eProtocol;
    int             type;
}E2L_SHOW_BENEFITS;

typedef struct __L2E_SHOW_BENEFITS
{
    L2EProtocol     eProtocol;
    bool            hasReceive1;
    bool            hasReceive2;
    int             type;
}L2E_SHOW_BENEFITS;

typedef struct __E2L_CASH_BENEFIT_CODE
{
    E2LProtocol     eProtocol;
    std::string     codeStr;
}E2L_CASH_BENEFIT_CODE;

typedef struct __L2E_CASH_BENEFIT_CODE
{
    L2EProtocol     eProtocol;
    bool            hasReceive1;
    bool            hasReceive2;
    int             errNo;
    int             type;
}L2E_CASH_BENEFIT_CODE;

typedef struct __E2L_LINK_TO_NETGAME
{
    E2LProtocol     eProtocol;
}E2L_LINK_TO_NETGAME;

typedef struct __E2L_SHOW_SHARE
{
    E2LProtocol     eProtocol;
}E2L_SHOW_SHARE;

typedef struct __L2E_SHOW_SHARE
{
    L2EProtocol     eProtocol;
    int             shareCount;
    int             countState[4];
}L2E_SHOW_SHARE;

typedef struct __E2L_SHARE_COUNT_BOUND
{
    E2LProtocol     eProtocol;
    int             index;
}E2L_SHARE_COUNT_BOUND;

typedef struct __L2E_SHARE_COUNT_BOUND
{
    L2EProtocol     eProtocol;
    int             shareCount;
    int             countState[4];
}L2E_SHARE_COUNT_BOUND;

typedef struct __E2L_SHARE
{
    E2LProtocol     eProtocol;
    int             channalId;
}E2L_SHARE;

typedef struct __L2E_SHARE_RESULT
{
    L2EProtocol     eProtocol;
    int             shareCount;
    int             countState[4];
    int             errNo;
}L2E_SHARE_RESULT;

typedef struct __L2E_UPDATE_SHARE
{
    L2EProtocol     eProtocol;
    int             shareCount;
    int             countState[4];
}L2E_UPDATE_SHARE;

typedef struct __L2E_UNLOCK_NEED_DIAMOND
{
    L2EProtocol     eProtocol;
}L2E_UNLOCK_NEED_DIAMOND;
#endif
