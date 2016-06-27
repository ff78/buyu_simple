//
//  ServerManager.h
//  buyu_simple
//
//  Created by ff on 16/5/27.
//
//

#ifndef ServerManager_h
#define ServerManager_h

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "../Share/Header/CSProtocol.h"
#include "../Share/Header/EventDef.h"

class ServerManager : public Singleton<ServerManager>
{
public:
    ServerManager();
    
    void processClientQuest(void *pMsg);
    void (ServerManager::*m_Client2ServerFunc[c2s_max])(void *pMsg);
private:
    void processRecharge(void *pMsg);
    void processCashBenefitCode(void *pMsg);
    void processLink2NetGame(void *pMsg);
    void processShare(void *pMsg);
};

#endif /* ServerManager_h */
