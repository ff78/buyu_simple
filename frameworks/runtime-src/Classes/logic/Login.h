//
//  Login.h
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#ifndef __buyu_simple__Login__
#define __buyu_simple__Login__

#include "utils/GameUtils.h"
#include "Load.h"
#include "ELProtocol.h"

//登陆逻辑处理
class Login : public Singleton<Login>, public Load
{
public:
    Login();
    ~Login();
    
    typedef void (Login::*LoadFunc)();
    //加载过程逻辑处理
    void load();
    //登陆是否成功判断
    bool isLoginSuc(E2L_LOGIN &val);
protected:
    virtual void onLoadFinish();
private:
    void load1();
//private:
//    std::vector<LoadFunc> m_vecLoadFunc;
//    int m_nStep;
};


#endif /* defined(__buyu_simple__Login__) */
