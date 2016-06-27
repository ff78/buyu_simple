#pragma once
#include "cocos2d.h"
USING_NS_CC;

#define		Max_Gold	150
#define		Max_Tex		7

#ifndef GOLD_FALLEN
#define GOLD_FALLEN
#endif



class GoldFallen :
public Node
{
private:
    const int SpeedAdd = /*1500*/ 1500;
    //float time = 0.0f;
    Vec2 m_visibleSize;
    int	now_index = 0;					//当前掉落金币数量
    bool m_bIsOpen = false;
    
    static GoldFallen* m_instance;
protected:
    
    std::string	m_Texture[Max_Tex];
    //Texture2D *m_Texture[Max_Tex];
    struct OneGold :public Sprite
    {
        float v_x;
        float v_z;
        float v_y;						//下落速度
        float time;					    //下落翻转事件计时
        bool jump;						//是否弹起
        
        static OneGold* create(const std::string& filename)
        {
            OneGold *sprite = new (std::nothrow) OneGold();
            if (sprite && sprite->initWithFile(filename))
            {
                sprite->autorelease();
                return sprite;
            }
            CC_SAFE_DELETE(sprite);
            return nullptr;
        }
        
        static OneGold* create()
        {
            OneGold *sprite = new (std::nothrow) OneGold();
            if (sprite && sprite->init())
            {
                sprite->autorelease();
                return sprite;
            }
            CC_SAFE_DELETE(sprite);
            return nullptr;
        }
    };
    
    Vector<OneGold*> m_list;
    
    
protected:
    
    void CreatOne();
    
    virtual void update(float deltaTime);
    virtual bool init();
protected:
    
    GoldFallen();
    
public:
    
    ~GoldFallen();
    static GoldFallen* GetInstance();
    static void StartPlay();
    static void StopPlay();
};

#define GOLD_FALL_ANIMATION() GoldFallen::StartPlay();

