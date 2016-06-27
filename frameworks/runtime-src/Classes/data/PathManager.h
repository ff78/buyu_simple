//
//  PathManager.h
//  buyu_simple
//
//  Created by ff on 15/12/11.
//
//

#ifndef __buyu_simple__PathManager__
#define __buyu_simple__PathManager__

#include "cocos2d.h"
#include "utils/GameUtils.h"
#include "GameDef.h"
#include "utils/MathAide.h"


class PathManager : public Singleton<PathManager>
{
public:
    PathManager();
    bool initPath(const std::string filePath);
    
    const MovePoints& getPaths(FISH_PATH_TYPE type, int index) const;
    
    void clear();
private:
//    void createScene1Paths();
//    void createScene2Paths();
//    void createScene3Paths();
    
    void createScene1CirclePath1();
    void createScene1CirclePath2();
    void createScene1CirclePath3();
    void createSlashPath();
    
    std::vector<MovePoints> smallPaths;
    std::vector<MovePoints> bigPaths;
    std::vector<MovePoints> hugePaths;
    std::vector<MovePoints> specialPaths;
    
    std::vector<MovePoints> scene1Paths;
    std::vector<MovePoints> scene2Paths;
    std::vector<MovePoints> scene3Paths;
    std::vector<MovePoints> circlePaths1;//7鱼阵路径
    std::vector<MovePoints> circlePaths2;//5鱼阵路径
    std::vector<MovePoints> circlePaths3;//1鱼阵路径
    
    std::vector<MovePoints> slashPaths;//8鱼阵路径两条斜线
    
    std::vector<MovePoints> fishArrayPaths;
    
    CC_SYNTHESIZE_READONLY(bool, loadOver, LoadOver);
};


#endif /* defined(__buyu_simple__PathManager__) */
