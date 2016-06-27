//
//  Login.cpp
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#include "Login.h"

#include "view/UIManager.h"
#include "ClientLogic.h"

Login::Login()
{
    addLoadFunc(Load::LoadFunc(&Login::load1));
}

Login::~Login()
{

}


void Login::onLoadFinish()
{
    Load::onLoadFinish();
    //加载完成，转入登陆状态
    ClientLogic::instance()->ChangeState(GAME_STATE_LOGIN);
}

void Login::load()
{
    LOAD_LOGIN_UPDATE info;
    info.eProtocol = l2e_load_login_update;
    info.isOver = false;
    info.step = 0;
    info.count = (int)m_vecLoadFunc.size();
    int result = Load::load();
    if (result != -1) {
        info.step = result;
        //更新加载界面
        ClientLogic::instance()->pass2Engine(&info);
    }else{
        onLoadFinish();
    }
    
}

bool Login::isLoginSuc(E2L_LOGIN &val)
{
    if(val.account == "" && val.password == "")
    {
        //登陆成功，转到大厅状态
        ClientLogic::instance()->ChangeState(GAME_STATE_HALL);
        return true;
    }
    
    L2E_LOGIN_RESULTE result;
    result.eProtocol = l2e_login_result;
    result.success = false;
    //登陆失败，发消息给显示层，反馈失败信息
    ClientLogic::instance()->pass2Engine(&result);
    return false;
}

void Login::load1()
{
    //todo 账号相关初始化
}
