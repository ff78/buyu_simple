#include "StringData.h"

#include "cocos2d.h"
USING_NS_CC;

using namespace std;

static string& replace_all(string& str, const string& old_value,const string& new_value)   
{   
	while(true)   {   
		string::size_type   pos(0);   
		if( (pos=str.find(old_value))!=string::npos   )   
			str.replace(pos,old_value.length(),new_value);   
		else   
			break;   
	}   
	return   str;   
}   

StringData* StringData::shared()
{
	static StringData sInstance;
	return &sInstance;
}

StringData::StringData()
{
	init();
}

StringData::~StringData()
{
}

//初始化
bool StringData::init()
{
	ValueMap vm = FileUtils::getInstance()->getValueMapFromFile("strings.plist");
	for (auto it = vm.begin(); it != vm.end(); ++it)
	{
		Value& vl = it->second;
		std::string s = vl.asString();
		replace_all(s, "\\n", "\n" );
		_dic[it->first] = s;
	}
	return true;
}

//根据键值得到String类型数据
const std::string& StringData::stringFromKey(const std::string& key)
{
	return _dic[key];
}
