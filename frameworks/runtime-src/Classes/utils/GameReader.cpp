//
//  GameReader.cpp
//  soul
//
//  Created by ff on 14-9-11.
//
//

#include "GameReader.h"

USING_NS_CC;
using namespace rapidjson;
using namespace cocostudio;

std::map<std::string, rapidjson::Document*> GameReader::docPool;
std::map<std::string, rapidjson::Document> GameReader::filePool;

bool GameReader::readData(std::string filePath,rapidjson::Document* _doc)
{
    bool ret = false;
    
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(filePath);
    
    _doc->Parse < 0 > (contentStr.c_str());
    ret = !_doc->HasParseError();
    
    return ret;
}

rapidjson::Document* GameReader::getDocInstance(std::string filePath)
{
    auto dataItr = docPool.find(filePath);
    if(dataItr == docPool.end())
    {
        rapidjson::Document* _doc = new rapidjson::Document();
        
        if(readData(filePath, _doc))
        {
            docPool.insert(std::make_pair(filePath, _doc));
            return docPool[filePath];
        }else{
            return NULL;
        }
    }else{
        return (dataItr->second);
    }
}

