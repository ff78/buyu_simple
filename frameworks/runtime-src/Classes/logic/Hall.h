//
//  Hall.h
//  buyu_simple
//
//  Created by ff on 15/12/9.
//
//

#ifndef __buyu_simple__Hall__
#define __buyu_simple__Hall__

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "ELProtocol.h"

class Hall : public Singleton<Hall>
{
public:
    void enterGame(E2L_ENTERGAME &info);
};

#endif /* defined(__buyu_simple__Hall__) */
