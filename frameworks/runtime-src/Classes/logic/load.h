//
//  load.h
//  buyu_simple
//
//  Created by ff on 15/11/20.
//
//

#ifndef __buyu_simple__load__
#define __buyu_simple__load__

#include <vector>

class Load
{
public:
    Load();
    ~Load();
    
    typedef void (Load::*LoadFunc)();
protected:
    int load();
    void addLoadFunc(LoadFunc pFunc)
    {
        m_vecLoadFunc.push_back(pFunc);
    }
    virtual void onLoadFinish();
protected:
    std::vector<LoadFunc> m_vecLoadFunc;
    int m_nStep;
};

#endif /* defined(__buyu_simple__load__) */
