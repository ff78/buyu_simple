//
//  GameUtils.h
//  simple-son
//
//  Created by ff on 15/10/21.
//
//

#ifndef __simple_son__GameUtils__
#define __simple_son__GameUtils__
#include "cocos2d.h"



template <class T>
std::string Convert2String(T value) {
    std::stringstream ss;
    ss<<value;
    return ss.str();
}

template <class T>
class Singleton
{
public:
    static T* instance()
    {
        //        if(!_instance)
        //        {
        //
        //        }
        //        return _instance;
        if( 0== _instance.get())
        {
            _instance.reset(new T());
        }
        //        unlockpt(_instance);
        return _instance.get();
        
    };
    
public:
    Singleton(void){};
    ~Singleton(void){};
    
private:
    Singleton(const Singleton&){};
    Singleton & operator= (const Singleton &){};
    
public:
    static std::auto_ptr<T> _instance;
    //    static T *_instance;
};

#define DECLARE_SINGLETON_CLASS( type ) \
friend class auto_ptr< type >;\
friend class Singleton< type >;

template <class T>
std::auto_ptr<T> Singleton<T>::_instance;

class Util : public Singleton<Util>{
    
    
public:
    
};


class GameUtils
{
public:
    static std::vector<std::string> splitStr(std::string str, std::string pattern);
    
    static void DealWithRectString(std::string strTemp, cocos2d::Rect &rect);
    
    static void getRandArray(std::vector<int> &resVec, int arrayLen);
    
    static cocos2d::Size winSize;
};

#endif /* defined(__simple_son__GameUtils__) */
