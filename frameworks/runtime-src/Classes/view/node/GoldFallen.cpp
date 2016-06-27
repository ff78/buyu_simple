#include "GoldFallen.h"
#include "sound/SoundManager.h"

#define		Path_PNG_Gold0						"Hall/gold/gold_0.png"
#define		Path_PNG_Gold1						"Hall/gold/gold_1.png"
#define		Path_PNG_Gold2						"Hall/gold/gold_2.png"
#define		Path_PNG_Gold3						"Hall/gold/gold_3.png"
#define		Path_PNG_Gold4						"Hall/gold/gold_4.png"
#define		Path_PNG_Gold5						"Hall/gold/gold_5.png"
#define		Path_PNG_Gold6						"Hall/gold/gold_6.png"

#define		Gold_Jump_Min	0.5f
#define		Gold_Jump_Max	0.8f
#define		Gold_Offset		60

#ifndef _ZERO
#define _ZERO	0
#endif

//静态变量初始化
GoldFallen* GoldFallen::m_instance = NULL;

GoldFallen::GoldFallen()
{
}

bool GoldFallen::init()
{
	if (false == Node::init())
	{
		return false;
	}

	m_visibleSize = Director::getInstance()->getWinSize();
	//memset(m_Texture, NULL, sizeof(m_Texture));
	//CCLOG("屏幕大小：%f,%f\n", m_visibleSize.x, m_visibleSize.y);
	//从文件夹加载Max_Tex张金币图片


	m_Texture[0] = Path_PNG_Gold0;
	m_Texture[1] = Path_PNG_Gold1;
	m_Texture[2] = Path_PNG_Gold2;
	m_Texture[3] = Path_PNG_Gold3;
	m_Texture[4] = Path_PNG_Gold4;
	m_Texture[5] = Path_PNG_Gold5;
	m_Texture[6] = Path_PNG_Gold6;
	/*m_Texture[0] = TextureCache::sharedTextureCache()->addImage(Path_PNG_Gold0);
	m_Texture[1] = TextureCache::sharedTextureCache()->addImage(Path_PNG_Gold1);
	m_Texture[2] = TextureCache::sharedTextureCache()->addImage(Path_PNG_Gold2);
	m_Texture[3] = TextureCache::sharedTextureCache()->addImage(Path_PNG_Gold3);
	m_Texture[4] = TextureCache::sharedTextureCache()->addImage(Path_PNG_Gold4);
	m_Texture[5] = TextureCache::sharedTextureCache()->addImage(Path_PNG_Gold5);
	m_Texture[6] = TextureCache::sharedTextureCache()->addImage(Path_PNG_Gold6);*/

    this->setName("GoldFallen");
	scheduleUpdate();
	return true;
}

GoldFallen::~GoldFallen()
{
	/*for (size_t i = _ZERO; i < Max_Tex; i++)
	{
		delete m_Texture[i];
	}*/
   
}


GoldFallen* GoldFallen::GetInstance()
{


	if (NULL == m_instance)
	{
		m_instance = new GoldFallen();
		m_instance->init();
       	}

	return m_instance;
}

void GoldFallen::update(float deltaTime)
{

	//每帧生产一个金币
	if (m_bIsOpen)
	{
		/*	time += deltaTime;
			if (time > 0.01f)
			{*/
		if (++now_index > Max_Gold)
		{
			StopPlay();
		}
		CreatOne();
		//	time = 0;
		//}
	}

	//位置临时变量
	Vec3 v3;

	for (Vector<OneGold*>::iterator itor = m_list.begin(); itor != m_list.end(); /*++itor*/)
	{
		OneGold *index = (OneGold*)(*itor);

		index->v_y += (float)SpeedAdd * deltaTime;

		v3 = index->getPosition3D();
		v3 -= Vec3(index->v_x * deltaTime, deltaTime * index->v_y, deltaTime * index->v_z);
		
		index->setPosition3D(v3);

		index->time += deltaTime;

		if (index->time >= 0.1f)
		{
			index->time = 0;
			index->setTexture(m_Texture[random(_ZERO, Max_Tex - 1)]);
		}

		if (v3.y < -Gold_Offset )
		{
			if (false == index->jump)
			{
				v3.y = _ZERO;
				index->setPosition3D(v3);
				index->v_y = -index->v_y * random(Gold_Jump_Min, Gold_Jump_Max);
				index->jump = true;
				index->v_z = random(-1000, -500);
				index->v_x = random(-200, 200);
				++itor;
			}
			else
			{
				removeChild(index, true);
				itor = m_list.erase(itor);
			}

		}
		else
		{
			++itor;
		}
	}

}


//创建一枚金币
void GoldFallen::CreatOne()
{

	OneGold *g = OneGold::create(m_Texture[random(_ZERO, Max_Tex - 1)]);

 //   g->setTexture();
//	g->initWithFile(m_Texture[random(_ZERO, Max_Tex - 1)]);

	//g->initWithTexture(m_Texture[random(_ZERO, Max_Tex - 1)]);

	//g->setScale(0.5f);
	//起始位置
	g->setPosition(random(0.f, m_visibleSize.x), m_visibleSize.y + Gold_Offset);

	//g->jump = 2;
	//下落速度
	g->time = _ZERO;
	g->v_x = _ZERO;
	g->v_y = 200.f;
	g->v_z = _ZERO;
	g->jump = false;

	//g->setTextureRect(Rect(g->getTextureRect().origin.x, g->getTextureRect().origin.y, 65.f, 65.f));

	//g->setCameraMask((int)CameraFlag::USER1);
	// g.MaxSpeed = Random.Range(MaxSpeed1, MaxSpeed2);
	m_list.pushBack(g);
	//g->retain();
	addChild(g);
}


//开启动画
void GoldFallen::StartPlay()
{
    Scene* main = Director::getInstance()->getRunningScene();
    GoldFallen* This = GetInstance();
    
    if (!main->getChildByName("GoldFallen"))
    {
        main->addChild(GetInstance(), 999);
        This->scheduleUpdate();
    }
    
    GetInstance()->m_bIsOpen = true;
    AUDIO_PLAY("gold_down", AUDIO_TYPE::EFFECT_TYPE);
}


//停止生产金币
void GoldFallen::StopPlay()
{
//    AUDIO_PLAY("gold_down", AUDIO_TYPE::EFFECT_TYPE);
	GetInstance()->now_index = 0;
	GetInstance()->m_bIsOpen = false;
}


