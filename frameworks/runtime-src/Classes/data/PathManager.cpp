//
//  PathManager.cpp
//  buyu_simple
//
//  Created by ff on 15/12/11.
//
//

#include "PathManager.h"

USING_NS_CC;

PathManager::PathManager()
{
    loadOver = false;
}

void PathManager::createScene1CirclePath1()
{
    //! 速度
    const float fSpeed = 1.0f;
    
    //! 坐标
    Vec2 fish_pos[50];
    
    
    //右边往左边
    Vec2 center;
    float fCenterOffset = 300.0f;
    //按实时屏幕宽度
    center.x = GameUtils::winSize.width + fCenterOffset;
    center.y = GameUtils::winSize.height/2;
    //! 默认屏幕宽度 1074
//    center.x = 1074.0f + fCenterOffset;
//    center.y = 370.0f;
    
    //! 半径
    float fRadius = 300.0f;
    MathAide::BuildCircle(center.x, center.y, fRadius, fish_pos, 50);
    float init_x[2], init_y[2];
    MovePoints mp;
    for (int i = 0; i < 50; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = fish_pos[i].x - 1574;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths1.push_back(mp);
    }
    
    fRadius = 230.0f;
    MathAide::BuildCircle(center.x, center.y, fRadius, fish_pos, 40);
    for (int i = 0; i < 40; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = fish_pos[i].x - 1574;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths1.push_back(mp);
    }
    
    fRadius = 150.0f;
    MathAide::BuildCircle(center.x, center.y, fRadius, fish_pos, 30);
    for (int i = 0; i < 30; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = fish_pos[i].x - 1574;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths1.push_back(mp);
    }
    
    //! 中心大鱼
    init_x[0] = center.x;
    init_y[0] = center.y;
    init_x[1] = init_x[0] - 1574;
    init_y[1] = init_y[0];
    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
    circlePaths1.push_back(mp);
    
    
    //! 左边一个圈,往右边游
    fRadius = 300.0f;
    center.x = -1 * fCenterOffset;
    center.y = GameUtils::winSize.height/2;
//    center.y = 370.0f;
    MathAide::BuildCircle(center.x, center.y, fRadius, fish_pos, 50);
    for (int i = 0; i < 50; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = fish_pos[i].x + 1574;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths1.push_back(mp);
    }
    
    fRadius = 230.0f;
    MathAide::BuildCircle(center.x, center.y, fRadius, fish_pos, 40);
    for (int i = 0; i < 40; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = fish_pos[i].x + 1574;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths1.push_back(mp);
    }
    
    
    fRadius = 150.0f;
    MathAide::BuildCircle(center.x, center.y, fRadius, fish_pos, 30);
    for (int i = 0; i < 30; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = fish_pos[i].x + 1574;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths1.push_back(mp);
    }
    
    //! 中心大鱼
    init_x[0] = center.x;
    init_y[0] = center.y;
    init_x[1] = init_x[0] + 1574;
    init_y[1] = init_y[0];
    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
    circlePaths1.push_back(mp);
}

void PathManager::createScene1CirclePath2()
{
    //! 最大的圆半径
    float fRadius_Big = 300;
    float fRadius_Small = 100;
    
    //! 速度
    const float fSpeed = 1.0f;
    
    //! 坐标
    Vec2 fish_pos[100];
    
    //! 圆心
    Vec2 center_big;
    center_big.x = -1 * fRadius_Big;
    center_big.y = GameUtils::winSize.height/2;
//    center_big.y = 370.0f;
    
    //! 小圆圆心
    Vec2 center_small_1;
    center_small_1.x = center_big.x + 110;
    center_small_1.y = center_big.y + 110;
    
    Vec2 center_small_2;
    center_small_2.x = center_big.x + 110;
    center_small_2.y = center_big.y - 110;
    
    Vec2 center_small_3;
    center_small_3.x = center_big.x - 110;
    center_small_3.y = center_big.y + 110;
    
    Vec2 center_small_4;
    center_small_4.x = center_big.x - 110;
    center_small_4.y= center_big.y - 110;
    
    //! 一个大圈向右游
    MathAide::BuildCircle(center_big.x, center_big.y, fRadius_Big, fish_pos, 100);
    float init_x[2], init_y[2];
    MovePoints mp;
    for (int i = 0; i < 100; i++)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = GameUtils::winSize.width + 2 * fRadius_Big;
//        init_x[1] = 1074.0f + 2 * fRadius_Big;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths2.push_back(mp);
    }
    
    //! 圈中心创建一条鱼
    init_x[0] = center_big.x;
    init_y[0] = center_big.y;
    init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//    init_x[1] = 1074.0f + 2 * fRadius_Small ;
    init_y[1] = init_y[0];
    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
    circlePaths2.push_back(mp);
    
    //! 四个小圆
    MathAide::BuildCircle(center_small_1.x, center_small_1.y, fRadius_Small, fish_pos,20);
    for (int i = 0; i < 20; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//        init_x[1] = 1074.0f + 2 * fRadius_Small ;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths2.push_back(mp);
    }
    
    //! 圈中心创建一条鱼
    init_x[0] = center_small_1.x;
    init_y[0] = center_small_1.y;
    init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//    init_x[1] = 1074.0f + 2 * fRadius_Small ;
    init_y[1] = init_y[0];
    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
    circlePaths2.push_back(mp);
    
    
    MathAide::BuildCircle(center_small_2.x, center_small_2.y, fRadius_Small, fish_pos,20);
    for (int i = 0; i < 20; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//        init_x[1] = 1074.0f + 2 * fRadius_Small ;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths2.push_back(mp);
    }
    
    //! 圈中心创建一条鱼
    init_x[0] = center_small_2.x;
    init_y[0] = center_small_2.y;
    init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//    init_x[1] = 1074.0f + 2 * fRadius_Small ;
    init_y[1] = init_y[0];
    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
    circlePaths2.push_back(mp);
    
    MathAide::BuildCircle(center_small_3.x, center_small_3.y, fRadius_Small, fish_pos,20);
    for (int i = 0; i < 20; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//        init_x[1] = 1074.0f + 2 * fRadius_Small ;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths2.push_back(mp);
    }
    
    init_x[0] = center_small_3.x;
    init_y[0] = center_small_3.y;
    init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//    init_x[1] = 1074.0f + 2 * fRadius_Small ;
    init_y[1] = init_y[0];
    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
    circlePaths2.push_back(mp);
    
    MathAide::BuildCircle(center_small_4.x, center_small_4.y, fRadius_Small, fish_pos,20);
    for (int i = 0; i < 20; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//        init_x[1] = 1074.0f + 2 * fRadius_Small ;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths2.push_back(mp);
    }
    
    init_x[0] = center_small_4.x;
    init_y[0] = center_small_4.y;
    init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//    init_x[1] = 1074.0f + 2 * fRadius_Small ;
    init_y[1] = init_y[0];
    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
    circlePaths2.push_back(mp);
    
    //! 同心一个小圆
    MathAide::BuildCircle(center_big.x, center_big.y, fRadius_Small, fish_pos, 20);
    for (int i = 0; i < 20; ++i)
    {
        init_x[0] = fish_pos[i].x;
        init_y[0] = fish_pos[i].y;
        init_x[1] = GameUtils::winSize.width + 2 * fRadius_Small ;
//        init_x[1] = 1074.0f + 2 * fRadius_Small ;
        init_y[1] = init_y[0];
        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
        circlePaths2.push_back(mp);
    }
}

void PathManager::createScene1CirclePath3()
{
    //! 默认屏幕宽度 1074
    Vec2 center;
    float fCenterOffset = 220.0f;
    center.x = GameUtils::winSize.width/2 + fCenterOffset;
    center.y = GameUtils::winSize.height/2;
//    center.x = 537.0f + fCenterOffset;
//    center.y = 370.0f;
    
    int fishCircleCount = 30;
    int circlePosCount = 300;
    
    //! 半径
    float fRadius = 300.0f;
    fishCircleCount = 30;
    
    for(int i = 0; i<30;i++)
    {
        //! 转大圈
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(center.x, center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 360.0f / 30.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
    
    fRadius = 250.0f;
    fishCircleCount = 25;
    for(int i = 0; i<25;i++)
    {
        //!
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(center.x, center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 360.0f / 25.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
    
    fRadius = 200.0f;
    fishCircleCount = 20;
    for(int i = 0; i<20;i++)
    {
        //! 转圈
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(center.x, center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 360.0f / 20.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
    
    fRadius = 150.0f;
    fishCircleCount = 15;
    for(int i = 0; i<15;i++)
    {
        //! 转圈
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(center.x, center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 360.0f / 15.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
    
    Vec2 left_center;
    fCenterOffset = 220.0f;
    left_center.x = GameUtils::winSize.width/2 - fCenterOffset;
    left_center.y = GameUtils::winSize.height/2;
//    left_center.x = 537.0f - fCenterOffset;
//    left_center.y = 370.0f;
    
    fRadius = 300.0f;
    fishCircleCount = 30;
    for(int i = 0; i<30;i++)
    {
        //! 转大圈
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(left_center.x, left_center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 360.0f / 30.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
    
    fRadius = 250.0f;
    fishCircleCount = 25;
    for(int i = 0; i<25;i++)
    {
        //!
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(left_center.x, left_center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 360.0f / 25.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
    
    fRadius = 200.0f;
    fishCircleCount = 20;
    for(int i = 0; i<20;i++)
    {
        //! 转圈
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(left_center.x, left_center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 360.0f / 20.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
    
    fRadius = 150.0f;
    fishCircleCount = 15;
    for(int i = 0; i<15;i++)
    {
        //! 转圈
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(left_center.x, left_center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 360.0f / 15.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
    
    
    //! 两处圆形创建大鱼
    fRadius = 0.0f;
    fishCircleCount = 1;
    for(int i = 0; i<1;i++)
    {
        //! 转圈
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(left_center.x, left_center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 12.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
    
    fRadius = 0.0f;
    fishCircleCount = 1;
    for(int i = 0; i<1;i++)
    {
        //! 转圈
        MovePoints mp;
        MovePoints mp2;
        MovePoints points;
        
        float init_x[2], init_y[2];
        MathAide::BuildCircle(center.x, center.y, fRadius, points, circlePosCount, 2 * M_PI * i/(float)fishCircleCount, 12.0f * i);
        
        MovePoints::iterator it = points.begin();
        while(it != points.end())
        {
            mp.push_back(*it);
            it++;
        }
        
        int index = (int)points.size();
        init_x[0] = points[index - 1].position.x;
        init_y[0] = points[index - 1].position.y;
        
        init_x[1] = 600 * sinf(2 * M_PI * points[index - 1].direction / 360.f) + init_x[0];
        init_y[1] = 600 * cosf(2 * M_PI * points[index - 1].direction / 360.f) + init_y[0];
        
        MathAide::BuildLinear(init_x, init_y, 2, mp2, 2.0f);
        
        it = mp2.begin() + 1;
        while(it != mp2.end())
        {
            mp.push_back(*it);
            it++;
        }
        circlePaths3.push_back(mp);
    }
}

void PathManager::createSlashPath()
{
    float init_x[2], init_y[2];
    
    init_x[0] = -107.0f;
    init_x[1] = 1181.0f;
    
    init_y[0] = -53.7f;
    init_y[1] = 793.7f;
    
    MovePoints mp;
    MathAide::BuildLinear(init_x, init_y, 2, mp, 1.0f);
    slashPaths.push_back(mp);
    
    init_y[0] = 793.7f;
    init_y[1] = -53.7f;
    mp.clear();
    MathAide::BuildLinear(init_x, init_y, 2, mp, 1.0f);
    slashPaths.push_back(mp);
}

//void PathManager::createScene3Paths()
//{
//    
//    const float fRadius = (GameUtils::winSize.height - 240) / 2.0f;
//    const float fSpeed = 3.0f;
//    Vec2 fish_pos[100];
//    Vec2 center;
//    
//    center.x = GameUtils::winSize.width + fRadius;
//    center.y = fRadius + 120;
//    MathAide::BuildCircle(center.x, center.y, fRadius, fish_pos, 50);
//    float init_x[2], init_y[2];
//    MovePoints mp;
//    for (int i = 0; i < 50; ++i)
//    {
//        init_x[0] = fish_pos[i].x;
//        init_y[0] = fish_pos[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene3Paths.push_back(mp);
//    }
//    
//    MathAide::BuildCircle(center.x, center.y, fRadius * 40 / 50, fish_pos, 40);
//    for (int i = 0; i < 40; ++i)
//    {
//        init_x[0] = fish_pos[i].x;
//        init_y[0] = fish_pos[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene3Paths.push_back(mp);
//    }
//    
//    MathAide::BuildCircle(center.x, center.y, fRadius * 30 / 50, fish_pos, 30);
//    for (int i = 0; i < 30; ++i)
//    {
//        init_x[0] = fish_pos[i].x;
//        init_y[0] = fish_pos[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene3Paths.push_back(mp);
//    }
//    
//    init_x[0] = center.x;
//    init_y[0] = center.y;
//    init_x[1] = -2 * fRadius;
//    init_y[1] = init_y[0];
//    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//    scene3Paths.push_back(mp);
//    
//    
//    center.x = -fRadius;
//    MathAide::BuildCircle(center.x, center.y, fRadius, fish_pos, 50);
//    for (int i = 0; i < 50; ++i)
//    {
//        init_x[0] = fish_pos[i].x;
//        init_y[0] = fish_pos[i].y;
//        init_x[1] =GameUtils::winSize.width + 2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene3Paths.push_back(mp);
//    }
//    
//    MathAide::BuildCircle(center.x, center.y, fRadius * 40 / 50, fish_pos, 40);
//    for (int i = 0; i < 40; ++i)
//    {
//        init_x[0] = fish_pos[i].x;
//        init_y[0] = fish_pos[i].y;
//        init_x[1] = GameUtils::winSize.width + 2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene3Paths.push_back(mp);
//    }
//    
//    MathAide::BuildCircle(center.x, center.y, fRadius * 30 / 50, fish_pos, 30);
//    for (int i = 0; i < 30; ++i)
//    {
//        init_x[0] = fish_pos[i].x;
//        init_y[0] = fish_pos[i].y;
//        init_x[1] = GameUtils::winSize.width + 2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene3Paths.push_back(mp);
//    }
//    
//    init_x[0] = center.x;
//    init_y[0] = center.y;
//    init_x[1] = GameUtils::winSize.width + 2 * fRadius;
//    init_y[1] = init_y[0];
//    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//    scene3Paths.push_back(mp);
//}
//
//void PathManager::createScene2Paths()
//{
//    float scaleX =  1;//designResolutionSize.width*1.0/DEF_SCREEN_WIDTH;
//    float scaleY =  1;//designResolutionSize.height*1.0/DEF_SCREEN_HEIGHT;
//    const float fStopExcursion = 180.f;
//    const float fHExcursion = 27.f / 2;
//    const float fSmallFishInterval = (designResolutionSize.width - fHExcursion * 2) / 100;
//    const float fSpeed = 6.f;
//    float fSmallFishHeight = 65.f;
//    MovePoints mp;
//    int fish_count = 0;
//    float init_x[2], init_y[2];
//    int small_height = (int)fSmallFishHeight * 3;
//    for (int i = 0; i < 200; ++i)
//    {
//        init_x[0] = init_x[1] = fHExcursion + (i % 100) * fSmallFishInterval;
//        int excursion = rand() % small_height;
//        if (i < 100)
//        {
//            init_y[0] = -65.f - excursion;
//            init_y[1] = designResolutionSize.height + 65.f;
//        }
//        else
//        {
//            init_y[0] =  designResolutionSize.height + 65.f + excursion;
//            init_y[1] = -65.f;
//        }
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene2Paths.push_back(mp);
//    }
//    for (int i = 0; i < 200; ++i)
//    {
//        MovePoints::iterator it = scene2Paths[i].begin();
//        while(it != scene2Paths[i].end())
//        {
//            MOVE_POINT& pos = *it;
//            if(i < 100)
//            {
//                if (pos.position.y >= fStopExcursion)
//                {
//                    scene2Paths[i].insert(it, 350, pos);
//                    break;
//                }
//            }
//            else
//            {
//                if (pos.position.y <  designResolutionSize.height - fStopExcursion)
//                {
//                    scene2Paths[i].insert(it, 350, pos);
//                    break;
//                }
//            }
//            it++;
//        }
//    }
//    
//    float big_fish_width= 600;
//    float big_fish_excursion[7];
//    for (int i = 0; i < 7; ++i)
//    {
//        big_fish_excursion[i] = big_fish_width * (i+1);
//    }
//    float yexcursoin = 250 / 2.0f;
//    
//    for (int i = 0; i < 14; ++i)
//    {
//        if (i < 7)
//        {
//            init_y[0] = init_y[1] =  designResolutionSize.height / 2 - yexcursoin;
//            init_x[0] = -big_fish_excursion[i % 7];
//            init_x[1] = designResolutionSize.width + big_fish_width;
//            MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed*2);
//            scene2Paths.push_back(mp);
//        }
//        else
//        {
//            init_y[0] = init_y[1] =  designResolutionSize.height / 2 + yexcursoin;
//            init_x[0] =designResolutionSize.width + big_fish_excursion[i % 7];
//            init_x[1] = -big_fish_width;
//            MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed*2);
//            scene2Paths.push_back(mp);
//        }
//    }
//    
//}
//
//void PathManager::createScene1Paths()
//{
//    float fRadius = (designResolutionSize.height - 240) / 2.0f;
//    float fSpeed = 3.0f;
//    float fRadian1 = (float)(45 * M_PI / 180);
//    float fRadian2 = (float)(135 * M_PI / 180);
//    Vec2 pt[100];
//    Vec2 center;
//    MovePoints mp;
//    center.x =designResolutionSize.width + fRadius;
//    center.y = fRadius+120;
//    MathAide::BuildCircle(center.x, center.y, fRadius, pt, 100);
//    float init_x[2], init_y[2];
//    for (int i = 0; i < 100; ++i)
//    {
//        init_x[0] = pt[i].x;
//        init_y[0] = pt[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene1Paths.push_back(mp);
//    }
//    
//    float fRadius1 = fRadius / 2.0f;
//    float fRadius2 = fRadius / 3.0f;
//    Vec2 center_small[4];
//    center_small[0].x = center.x + fRadius1 * cosf(-fRadian2);
//    center_small[0].y = center.y + fRadius1 * sinf(-fRadian2);
//    center_small[1].x = center.x + fRadius1 * cosf(-fRadian1);
//    center_small[1].y = center.y + fRadius1 * sinf(-fRadian1);
//    center_small[2].x = center.x + fRadius1 * cosf(fRadian2);
//    center_small[2].y = center.y + fRadius1 * sinf(fRadian2);
//    center_small[3].x = center.x + fRadius1 * cosf(fRadian1);
//    center_small[3].y = center.y + fRadius1 * sinf(fRadian1);
//    
//    MathAide::BuildCircle(center_small[0].x, center_small[0].y, fRadius2, pt, 30);
//    for (int i = 0; i < 30; ++i)
//    {
//        init_x[0] = pt[i].x;
//        init_y[0] = pt[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene1Paths.push_back(mp);
//    }
//    MathAide::BuildCircle(center_small[1].x, center_small[1].y, fRadius2, pt, 30);
//    for (int i = 0; i < 30; ++i)
//    {
//        init_x[0] = pt[i].x;
//        init_y[0] = pt[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene1Paths.push_back(mp);
//    }
//    
//    MathAide::BuildCircle(center_small[2].x, center_small[2].y, fRadius2, pt, 30);
//    for (int i = 0; i < 30; ++i)
//    {
//        init_x[0] = pt[i].x;
//        init_y[0] = pt[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene1Paths.push_back(mp);
//    }
//    
//    MathAide::BuildCircle(center_small[3].x, center_small[3].y, fRadius2, pt, 30);
//    for (int i = 0; i < 30; ++i)
//    {
//        init_x[0] = pt[i].x;
//        init_y[0] = pt[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene1Paths.push_back(mp);
//    }
//    
//    MathAide::BuildCircle(center.x, center.y, fRadius1 / 2, pt, 15);
//    for (int i = 0; i < 15; ++i) 
//    {
//        init_x[0] = pt[i].x;
//        init_y[0] = pt[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene1Paths.push_back(mp);
//    }
//    
//    for (int i = 0; i < 4; ++i) 
//    {
//        init_x[0] = center_small[i].x;
//        init_y[0] = center_small[i].y;
//        init_x[1] = -2 * fRadius;
//        init_y[1] = init_y[0];
//        MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//        scene1Paths.push_back(mp);
//    }
//    
//    init_x[0] = center.x;
//    init_y[0] = center.y;
//    init_x[1] = -2*fRadius;
//    init_y[1] = init_y[0];
//    MathAide::BuildLinear(init_x, init_y, 2, mp, fSpeed);
//    scene1Paths.push_back(mp);
//}

void PathManager::clear()
{
    smallPaths.clear();
    bigPaths.clear();
    hugePaths.clear();
    specialPaths.clear();
    
    scene1Paths.clear();
    scene2Paths.clear();
    scene3Paths.clear();
    circlePaths1.clear();
    circlePaths2.clear();
    circlePaths3.clear();
    slashPaths.clear();
    fishArrayPaths.clear();
}

const MovePoints& PathManager::getPaths(FISH_PATH_TYPE nType, int nID) const
{
    switch(nType % MAXTYPE_FISH_PATH)
    {
        case SMALL_FISH_PATH:
        {
            return smallPaths[nID % MAX_SMALL_PATH];
        }
        case BIG_FISH_PATH:
        {
            return bigPaths[nID % MAX_BIG_PATH];
        }
        case HUGE_FISH_PATH:
        {
            return hugePaths[nID % MAX_HUGE_PATH];
        }
        case SPECIAL_FISH_PATH:
        {
            return specialPaths[nID % MAX_SPECIAL_PATH];
        }
            //	case SCENE1_FISH_PATH:
            //		{
            //			return m_Scene1_Paths[nID % MAX_SCENE1_PATH];
            //		}
            //	case SCENE2_FISH_PATH:
            //		{
            //			return m_Scene2_Paths[nID % MAX_SCENE2_PATH];
            //		}
            //	case SCENE3_FISH_PATH:
            //		{
            //			return m_Scene3_Paths[nID % MAX_SCENE3_PATH];
            //		}
        case FISH_ARRAY_PATH_EX1:
        {
            return circlePaths1[nID];
        }
        case FISH_ARRAY_PATH_EX2:
        {
            return circlePaths2[nID];
        }
        case FISH_ARRAY_PATH_EX3:
        {
            return circlePaths3[nID];
        }
        case FISH_SLASH_PATH:
        {
            return slashPaths[nID];
        }
        case FISH_ARRAY_PATH:
        {
            return fishArrayPaths[nID % MAX_FISHARRAY_PATH];
        }
        default:
            break;
    }
    static MovePoints mp;
    return mp;
}

bool get_string_line(std::string& path, const std::string& all, int& start_pos)
{
    path.clear();
    int end_pos = 0;
    unsigned int n= (unsigned int)all.length();
    
    if((start_pos >= 0) && (start_pos <  n))
    {
        end_pos = (int)all.find_first_of( "\n ",start_pos);
        
        if((end_pos < 0) && (end_pos > n))
            end_pos = n;
        
        path += all.substr(start_pos, end_pos - start_pos);
        start_pos = end_pos + 1   ;
        
    } 
    return !path.empty();   
}

void ReadPath(const std::string& szPath, int nCount, std::vector<MovePoints>& pathVector)
{
    int x,y,staff;
    float angle;
    
    std::ostringstream ostr;
    
    pathVector.clear();
    std::string sData;
    std::string line;
    std::string fullpath;
    
    for (int i=0; i < nCount; i++)
    {
        MovePoints move_points;
        
        ostr.str("");
        
        ostr <<  szPath << i << ".dat";
        
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(ostr.str().c_str());
        sData = FileUtils::getInstance()->getStringFromFile(fullpath);
        
        if (sData.empty())
        {
            log("%s failed", fullpath.c_str());
            return;
        }
        
        int start_pos = 0;
        get_string_line(line, sData, start_pos);
        
        while(get_string_line(line, sData, start_pos))
        {
            std::sscanf(line.c_str(), "(%d,%d,%f,%d)", &x, &y, &angle, &staff);
            x *= GameUtils::winSize.width/pathResolutionSize.width ;
            y *= GameUtils::winSize.height/pathResolutionSize.height ;
            MOVE_POINT point;
            point.position = Vec2(x, y);
            point.direction = angle;
            move_points.push_back(point);
        }		
        pathVector.push_back(move_points);
    }
}

bool PathManager::initPath(const std::string filePath)
{
    clear();
    
    std::ostringstream ostr;
    
    ostr.str("");
    ostr <<  filePath << "small/";
    ReadPath(ostr.str().c_str(), MAX_SMALL_PATH, smallPaths);
    
    ostr.str("");
    ostr <<  filePath << "big/";
    ReadPath(ostr.str().c_str(), MAX_BIG_PATH, bigPaths);
    
    ostr.str("");
    ostr <<  filePath << "huge/";
    ReadPath(ostr.str().c_str(), MAX_HUGE_PATH, hugePaths);
    
    ostr.str("");
    ostr <<  filePath << "special/";
    ReadPath(ostr.str().c_str(), MAX_SPECIAL_PATH, specialPaths);
    
    ostr.str("");
    ostr<< filePath << "scene/";
    ReadPath(ostr.str(), MAX_FISHARRAY_PATH, fishArrayPaths);
    
    createScene1CirclePath1();
    createScene1CirclePath2();
    createScene1CirclePath3();
    createSlashPath();
    //	CreateScene1Paths();
    //	CreateScene2Paths();
    //	CreateScene3Paths();
    loadOver = true;
}


