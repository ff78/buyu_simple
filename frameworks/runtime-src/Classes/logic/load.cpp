//
//  load.cpp
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#include "load.h"

Load::Load()
{
    m_nStep = 0;
    m_vecLoadFunc.clear();    
}

Load::~Load()
{
    m_nStep = 0;
    m_vecLoadFunc.clear();
}

void Load::onLoadFinish()
{
    m_nStep = 0;
    m_vecLoadFunc.clear();
}

int Load::load()
{
    if ( m_vecLoadFunc.size()>0 && m_nStep<(int)m_vecLoadFunc.size() )
    {
        LoadFunc p = m_vecLoadFunc[m_nStep++];
        (this->*p)();
        return m_nStep;
    }
    return -1;
}
