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
    
    bool isOnLine();
private:
    GameIOS();
    static GameIOS* instance;
};


#endif
