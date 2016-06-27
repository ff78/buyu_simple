//
//  GameUtils.cpp
//  simple-son
//
//  Created by ff on 15/10/21.
//
//

#include "GameUtils.h"

cocos2d::Size GameUtils::winSize;

std::vector<std::string> GameUtils::splitStr(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;
    std::string::size_type size = str.size();
    
    for (std::string::size_type i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

void GameUtils::DealWithRectString(std::string strTemp, cocos2d::Rect &rect)
{
    /************************************************************************/
    // 处理范围字符串
    // 1. 判断字符串的格式是否正确
    int nTemp = static_cast<int>(count(strTemp.begin(), strTemp.end(), ','));
    if (nTemp != 3)
        return;
    
    // 2. 替换字符串中的"(" , ")" , ","为空的字符
    replace(strTemp.begin(), strTemp.end(), '(', ' ');
    replace(strTemp.begin(), strTemp.end(), ')', ' ');
    replace(strTemp.begin(), strTemp.end(), ',', ' ');
    
    // 3. 解析该字符串并输出到Rect中去
    float fTemp[4] = { 0 };
    std::istringstream stream(strTemp);
    for (int nIndex = 0; nIndex < 4; ++nIndex)
    {
        stream >> fTemp[nIndex];
    }
    
    rect = cocos2d::Rect(fTemp[0], fTemp[1], fTemp[2], fTemp[3]);
}

/**
 * 获取指定长度的随机数向量
 *
 * @param resVec std::vector<int> & 随机数向量
 * @param arrayLen int 向量长度
 */
void GameUtils::getRandArray(std::vector<int> &resVec, int arrayLen) {
    resVec.clear();
    for (int i = 0; i < arrayLen; i++) {
        resVec.push_back(i + 1);
    }
    int temp1, temp2;
    for (int i = 0; i < arrayLen / 2; i++) {
        temp1 = resVec[i];
        temp2 = abs(rand())%arrayLen;
        resVec[i] = resVec[temp2];
        resVec[temp2] = temp1;
    }
}
