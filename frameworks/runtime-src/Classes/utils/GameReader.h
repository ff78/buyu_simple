//
//  GameReader.h
//  soul
//
//  Created by ff on 14-9-11.
//
//

#ifndef __soul__GameReader__
#define __soul__GameReader__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

class GameReader {
public:
    static std::map<std::string, rapidjson::Document*> docPool;
    
    static std::map<std::string, rapidjson::Document> filePool;
    
public:
    static bool readData(std::string filePath, rapidjson::Document* doc);
    
    static rapidjson::Document* getDocInstance(std::string filePath);
    
};

#endif /* defined(__soul__GameReader__) */
