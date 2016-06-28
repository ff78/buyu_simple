//
//  JZVerManager.h
//  diamond
//
//  Created by ff on 15/9/2.
//
//

#ifndef diamond_JZVerManager_h
#define diamond_JZVerManager_h

#include <iostream>
#include <vector>

class GameIOS {
public:
    ~GameIOS();
    
    static GameIOS* getInstance();
    
    void openCommentUrl();
    
    void openCommentUrl1();
    
    void openCommentUrl2();
    
    bool isOnLine();
    
    std::string getTodayStr();
private:
    GameIOS();
    static GameIOS* instance;
};


#endif
