
     
# 概述
   友盟社会化组件，可以让移动应用快速具备社会化分享、登录、评论、喜欢等功能，并提供实时、全面的社会化数据统计分析服务。   
   本指南将会手把手教你使用友盟社会化组件Cocos2d-x SDK，用5分钟为APP增加新浪微博、微信、QQ空间等国内外十几个主流平台的分享功能。
   该Cocos2d-x SDK目前支持iOS和Android平台。
 
# 前提   
   该Cocos2d-x分享组件需要依赖友盟社会化组件sdk，因此在您集成时必须将iOS或者Android平台的SDK集成到您的工程中,即压缩包中的Platforms目录下的Android和iOS两个文件夹。      
   如果你之前已经在友盟注册了应用，并获取到了Appkey，可以继续使用它。如果你尚未在友盟[友盟](http://www.umeng.com/)注册开发者账号，需要先注册，注册之后登录你的账号，点击**添加新应用**，填写完应用基本信息后，将进入"下载SDK并添加代码"页面，此页面即可得到Umeng Appkey。进入到"组件"一栏，选择左边的“社会化分享”，然后选择“设置”--“自定义设置”， 然后将你在各个平台获取到的app id和app secret填写到其中，并且保存。    

<b id=social_cocos2dx></b>
#  下载和拷贝Cocos2d-x所需文件 
   首先您需要下载友盟社会化组件 Cocos2d-x SDK (<a href="http://dev.umeng.com/social/cocos2d-x/sdk-download" target="_blank">下载链接</a>),然后将该sdk压缩包解压。
>压缩包目录说明：  
>**1. Cocos2dx**    
	实现cocos2d-x中跨平台分享功能,需拷贝到您项目的Classes文件夹中;    
>**2. Platforms**    
	原生的Android和iOS社会化组件SDK,需要您将库和资源拷贝到对应平台的项目中;    
>**3. doc**    
	Cocos2d-x SDK集成文档。   
	   
   **SDK中默认的添加的平台为新浪微博、腾讯微博、人人网、豆瓣这四个平台，其他的平台需要按[添加更多平台  ( 按需集成 )](#cocos2dx_integration_more_platforms)提供的方式手动添加。**
	
<b id=cocos2dx_integration></b>   
#  Cocos2d-x集成友盟分享组件
<b id=cocos2dx_integration_android></b>
##  Android平台集成 
###  拷贝所需的jar包和资源文件          
  解压Cocos2d-x SDK压缩包，进入到Platforms/Android/core文件夹下，将'libs'文件夹中的所有jar文件拷贝到工程中的libs目录中，并且将所有jar文件添加到编译路径(build path)中；
  将Platforms/Android/core/res目录下的所有文件夹拷贝到你的项目工程res目录下对应的文件夹中, 如图所示 :    
  <img src="http://dev.umeng.com/system/resources/W1siZiIsIjIwMTQvMDUvMjkvMTZfMTZfNDJfMjg1X2NvY29zMmRfeF9kaXIucG5nIl1d/cocos2d-x-dir.png" width="500" height="400" style="border:1px solid black">    
  如果您还需要其他的社交平台，您需要按照[添加更多平台  ( 按需集成 )](#cocos2dx_integration_more_platforms)这个章节的步骤来进行手动添加。    
  
###  拷贝类文件
   将Platforms/Android/controller目录下的com文件夹拷贝到您的Cocos2d-x项目Android平台的src目录下(<font color="red">所有文件的编码为utf-8，如有乱码，请开发者自行进行转码。</font>), 如图所示 :    
     <img src="http://dev.umeng.com/system/images/W1siZiIsIjIwMTQvMDQvMDkvMTVfNTdfMTVfODc2X2NvY29zMmR4X2NvbnRyb2xsZXIucG5nIl1d/cocos2dx-controller.png" width="400" height="160" style="border:1px solid black">        
   在jni/Android.mk中的LOCAL_SRC_FILES下添加如下配置 (注意格式,否则会编译出错) :    
```xml
../../Classes/Cocos2dx/Android/CCUMSocialController.cpp  \
../../Classes/Cocos2dx/ShareButton/UMShareButton.cpp \
../../Classes/Cocos2dx/Common/CCUMSocialSDK.cpp
```   
   
###  配置AndroidManifest.xml
```xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.umeng.example"
    android:versionCode="1"
    android:versionName="1.0" >

      <application
        android:icon="@drawable/icon"
        android:label="@string/app_name"
        android:name="com.umeng.social.CCApp">
       <activity
            android:name="com.umeng.soexample.UmengGame"
            android:screenOrientation="landscape"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
        <activity
            android:name=".WBShareActivity"
            android:configChanges="keyboardHidden|orientation"
            android:screenOrientation="portrait" >
            <intent-filter>
                <action android:name="com.sina.weibo.sdk.action.ACTION_SDK_REQ_ACTIVITY" />
                <category android:name="android.intent.category.DEFAULT" />
            </intent-filter>
        </activity>
        <activity
            android:name="com.sina.weibo.sdk.component.WeiboSdkBrowser"
            android:configChanges="keyboardHidden|orientation"
            android:windowSoftInputMode="adjustResize"
            android:exported="false" >
        </activity>
           <service android:name="com.sina.weibo.sdk.net.DownloadService"
            android:exported="false"></service>
        <!-- 腾讯SSO授权的Activity注册 -->
        <activity
            android:name="com.tencent.tauth.AuthActivity"
            android:launchMode="singleTask"
            android:noHistory="true" >
            <intent-filter>
                <action android:name="android.intent.action.VIEW" />

                <category android:name="android.intent.category.DEFAULT" />
                <category android:name="android.intent.category.BROWSABLE" />

                <data android:scheme="tencent100424468" /> <!-- 100498017 -->
            </intent-filter>
        </activity>
        <activity android:name="com.tencent.plus.ImageActivity" />
        <activity
            android:name="com.tencent.connect.common.AssistActivity"
            android:screenOrientation="portrait"
            android:theme="@android:style/Theme.Translucent.NoTitleBar" />
        <activity
            android:name="com.umeng.socialize.editorpage.ShareActivity"
            android:theme="@style/Theme.UMDefault"
            android:excludeFromRecents="true"
            />
        <!-- twitter -->
        <activity
            android:name="com.umeng.socialize.handler.TwitterWebActivity"
            android:configChanges=
                "keyboard|keyboardHidden|screenLayout|screenSize|orientation"
            android:theme="@android:style/Theme.Translucent.NoTitleBar"
            android:label="@string/app_name" />

        <meta-data
            android:name="UMENG_APPKEY"
            android:value="4eaee02c527015373b000003" >
        </meta-data>

        <!-- facebook相关 -->
        <!-- facebook -->
        <activity
            android:name="com.facebook.FacebookActivity"
            android:configChanges="keyboard|keyboardHidden|screenLayout|screenSize|orientation"
            android:label="@string/app_name"
            android:theme="@android:style/Theme.Translucent.NoTitleBar" />

        <provider
            android:name="com.facebook.FacebookContentProvider"
            android:authorities="com.facebook.app.FacebookContentProvider1444817025830662"
            android:exported="true" />

        <meta-data
            android:name="com.facebook.sdk.ApplicationId"
            android:value="@string/facebook_app_id" />
        <activity
            android:name="com.facebook.LoginActivity"
            android:label="@string/app_name"
            android:theme="@android:style/Theme.Translucent.NoTitleBar" />
        <!-- renren need -->
        <activity
            android:name="com.renn.rennsdk.oauth.OAuthActivity"
            android:configChanges="orientation|navigation|keyboardHidden" />
        <activity
            android:name="com.sina.weibo.sdk.component.WeiboSdkBrowser"
            android:configChanges="keyboardHidden|orientation"
            android:windowSoftInputMode="adjustResize"
            android:exported="false" >
        </activity>

        <service android:name="com.sina.weibo.sdk.net.DownloadService"
            android:exported="false"></service>
        <activity
            android:name=".yxapi.YXEntryActivity"
            android:configChanges="keyboardHidden|orientation|screenSize"
            android:exported="true"
            android:screenOrientation="portrait"
            android:theme="@android:style/Theme.Translucent.NoTitleBar" />
    </application>

    <uses-sdk android:minSdkVersion="8" />

    <!-- ###################声明SDK使用的相关权限###################### -->
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />  <!-- 检测网络状态 -->
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />     <!-- 获取mac地址作为用户的备用唯一标识 -->
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />      <!-- 获取用户手机的IMEI，用来唯一的标识用户。 -->
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" /><!-- 缓存资源优先存入SDcard -->
    <uses-permission android:name="android.permission.INTERNET" />              <!-- 允许应用程序联网，以便向我们的服务器端发送数据。 -->
    <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />  <!-- 用于评论模块分享位置信息 -->
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" /><!-- 用于评论模块分享位置信息 -->
</manifest>
```
   
###  添加Android平台的初始化代码
  在您的Cocos2dxActivity子类的onCreate方法下添加如下代码:    
```java         
// this为Cocos2dxActivity类型, 参数2为描述符,可随意修改.
CCUMSocialController.initSocialSDK(this, "com.umeng.social.share");
```   
###  覆写Cocos2dxActivity子类的onActivityResult方法
   在onActivityResult添加如下代码 : 
```java
// 授权回调    
CCUMSocialController.onActivityResult(requestCode, resultCode, data);    	  
super.onActivityResult(requestCode, resultCode, data);
```   
   完成以上步骤以后，并且您不需要集成分享到ios平台，您就可以到[在Cocos2d-x游戏中添加分享功能](#cocos2dx_integration_cocos2dx)章节添加分享代码到cocos2d-x游戏中。
### CCApp类
该类继承自Application，在该类中设置各个平台的appkey和appid
### 加入新浪微博的so文件
在jni中建立prebuild文件夹，放入对应的so文件，在Android.mk中写入这些文件的引用：
`include $(CLEAR_VARS)
LOCAL_MODULE := libweibosdkcore
LOCAL_SRC_FILES := prebuild/armeabi/libweibosdkcore.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libweibosdkcorev8
LOCAL_SRC_FILES := prebuild/arm64-v8a/libweibosdkcore.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libweibosdkcorev7
LOCAL_SRC_FILES := prebuild/armeabi-v7a/libweibosdkcore.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libweibosdkcoremips
LOCAL_SRC_FILES := prebuild/mips/libweibosdkcore.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libweibosdkcoremips64
LOCAL_SRC_FILES := prebuild/mips64/libweibosdkcore.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libweibosdkcorex86
LOCAL_SRC_FILES := prebuild/x86/libweibosdkcore.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libweibosdkcorex64
LOCAL_SRC_FILES := prebuild/x86_64/libweibosdkcore.so
include $(PREBUILT_SHARED_LIBRARY)`
<b id=cocos2dx_integration_ios></b>  
## iOS平台集成      

### 加入iOS SDK

解压SDK压缩包，将Platforms/iOS文件夹和Cocos2dx文件夹拖入工程目录，并删除Cocos2dx/Android文件夹.   


![alt text](http://dev.umeng.com/images/ios/social_drag_sdk.png "drag_sdk") 

确认勾选了“Copy items to destination's group folder”选项，并选择你要添加到的Target: 

![alt text](http://dev.umeng.com/images/ios/social_check_tip.png "check_tip") 

现在应该是这样的效果：

![alt text](http://dev.umeng.com/images/ios/social_project_structure.png "project_structure") 

UMSocial_Sdk_x.x.x 文件夹的目录结构

文件或文件夹|含义
---|---
libUMSocial_Sdk_x.x.a, libUMSocial_Sdk_Comment_x.x.a | 库文件
Header| 头文件
SocialSDKXib| xib文件
en.lproj，zh-Hans.lproj | 英文和中文语言文件

UMSocial_Sdk_Extra_Frameworks 文件夹的目录结构

文件或文件夹|含义
---|---
Wechat|微信
sinaSSO|新浪微博
TencentOpenAPI|QQ互联SDK
Facebook|Facebook SDK
Twitter|Twitter SDK
LaiWang|来往 SDK
YiXin|易信 SDK

###  添加下面的系统framework
Security.framework,libiconv.dylib,SystemConfiguration.framework,CoreGraphics.framework，libsqlite3.dylib，CoreTelephony.framework,libstdc++.dylib,libz.dylib,Social.framework,Accounts.framework。

###  按需设置各个平台的URL Scheme
<table border="1">
  <tr>
    <th> 平台 </th>
    <th>url scheme设置格式</th>
  </tr>
   <tr>
    <td>微信</td>
    <td>微信应用appId，例如“wxd9a39c7122aa6516”,微信详细集成步骤参考<a href="#social_wechat" style="text-decoration:none">微信集成方法</a></td>
  </tr>
   <tr>
    <td>微博</td>
    <td>在工程设置项,targets 一栏下,选中自己的 target,在 Info->URL Types 中添加 URL Schemes,格式为“wb”+新浪appkey，例如“wb126663232”</td>
  </tr>
   <tr>
    <td>分享到手机QQ、QQ空间</td>
    <td>“QQ”+腾讯QQ互联应用appId转换成十六进制（不足8位前面补0），例如“QQ05FA957C”。生成十六进制方法：在命令行输入<div>echo 'ibase=10;obase=16;您的腾讯QQ互联应用Id'|bc</div>，并在QQ互联后台的URL schema中填入此字符串保持一致，手机QQ详细集成步骤参考<a href="#social_qq" style="text-decoration:none">手机QQ集成方法</a></td>
  </tr>
  <tr>
    <td>单独登录到QQ、QQ空间（不分享）</td>
    <td>“tencent“+腾讯QQ互联应用Id，例如“tencent100308348”</td>
  </tr>
  <tr>
    <td>来往</td>
    <td>Identifier填“Laiwang”，URL Schemes填来往AppId.注意使用来往SDK后，Xcode工程other linker flags需要添加-ObjC参数</td>
  </tr>
  <tr>
    <td>易信</td>
    <td>易信Appkey，例如“yx35664bdff4db42c2b7be1e29390c1a06”</td>
  </tr>
  <tr>
    <td>Facebook</td>
    <td>默认使用iOS自带的Facebook分享framework，在iOS 6以上有效，若要使用我们提供的facebook分享需要设置“fb”+facebook AppID，例如“fb1440390216179601”，详细集成方法见[集成facebook](#social_facebook)</td>
  </tr>
</table> 

###  在AppDelegate实现系统回调方法
在Xcode工程中的ios文件夹下的AppController.mm文件，实现下面的系统回调

```
#import "UMSocial.h"
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    return [UMSocialSnsService handleOpenURL:url];
}
```
     
   然后您就可以到[在Cocos2d-x游戏中添加分享功能](#cocos2dx_integration_cocos2dx)章节添加分享代码到cocos2d-x游戏中。


<b id=cocos2dx_integration_cocos2dx></b>
###  在Cocos2d-x游戏中添加分享功能
   将所需的资源添加到对应的工程以后, 您就可以在cocos2d-x中使用该友盟社会化组件的分享、登录功能了。   
  首先将sdk压缩包下的根目录下的Cocos2dx文件夹拷贝到您的工程的Classes目录下，Cocos2dx包括：   
> * Android文件夹，Android平台的调用实现；   
> * iOS文件夹，iOS平台的调用实现；  
> * Common文件夹，针对Cocos2d-x的统一接口和类型定义 ；     
> * ShareButton文件夹，针对Cocos2d-x封装的分享按钮,点击按钮即可打开分享面板；    

   针对Android平台， 如果开发者需要使用facebook、易信、易信朋友圈、来往、来往动态、twitter、instagram平台则需要到[添加更多平台  ( 按需集成 )](#cocos2dx_integration_more_platforms)章节添加相应的平台。    
   友盟提供了三种方式来方便开发者使用分享功能，示例如下 :    
   ***方式一 :***     
   开发者可以通过UMShareButton类来快速的实现分享功能，UMShareButton是CCMenuItemImage的子类，当用户点击该按钮时将会打开友盟的分享面板。
   集成代码如下 :
```cpp
// 引入相关的头文件
#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "Cocos2dx/ShareButton/UMShareButton.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;
// ...... 代码省略

// HelloWorld为cocos2d::CCLayer的子类
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	// 创建分享按钮, 参数1为按钮正常情况下的图片, 参数2为按钮选中时的图片,参数3为友盟appkey, 参数4为分享回调
	UMShareButton *shareButton = UMShareButton::create("shareNormal.png","shareSelected.png", "你的友盟appkey", share_selector(shareCallback)) ;
	// 显示在友盟分享面板上的平台
    vector<int>* platforms = new vector<int>();
    platforms->push_back(SINA);
    platforms->push_back(RENREN) ;
    platforms->push_back(DOUBAN) ;
    platforms->push_back(QZONE) ;
    platforms->push_back(QQ) ;
    // 设置友盟分享面板上显示的平台
    shareButton->setPlatforms(platforms);
	// 设置文本分享内容
	shareButton->setShareContent("umeng social cocos2d-x sdk.") ;
	// 设置要分享的图片, 图片支持本地图片和url图片, 但是url图片必须以http://或者https://开头
	shareButton->setShareImage("/sdcard/header.jpeg") ;
	// 设置按钮的位置
	shareButton->setPosition(ccp(150, 180));
	// 然后开发者需要将该按钮添加到游戏场景中
    CCMenu* pMenu = CCMenu::create(shareButton, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
	
	// ********************** 设置平台信息 ***************************
	// CCUMSocialSDK *sdk = shareButton->getSocialSDK();
    // sdk->setQQAppIdAndAppKey("设置QQ的app id", "appkey");
    // sdk->setWeiXinAppInfo("设置微信和朋友圈的app id","app key");
    // sdk->setYiXinAppKey("设置易信和易信朋友圈的app id");
    // sdk->setLaiwangAppInfo("设置来往和来往动态的app id", 
    //					"设置来往和来往动态的app key", "我的应用名");
    // sdk->setFacebookAppId("你的facebook appid");
    // 设置用户点击一条图文分享时用户跳转到的目标页面, 一般为app主页或者下载页面
    // sdk->setTargetUrl("http://www.umeng.com/social");
    //     // 打开或者关闭log
    // sdk->setLogEnable(true) ;
    // **********************	END	***************************
    
    return true;
}
```     

***方式二 :***    
    开发者可以使用CCUMSocialSDK类来完成分享、授权、删除授权等操作， 实际上UMShareButton就是包装了CCUMSocialSDK以实现相关的功能。具体功能请参考CCUMSocialSDK中的注释。
    开发者可以自行定义某个按钮，然后在该按钮的点击事件中通过CCUMSocialSDK来进行相应的操作，示例代码如下 :   
```cpp
// 引入相关的头文件
#include "Cocos2dx/Common/CCUMSocialSDK.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;
// ...... 代码省略


// HelloWorld为cocos2d::CCLayer的子类, shareButtonClick为某个按钮点击事件的处理函数
void HelloWorld::shareButtonClick()
{
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
    // 设置友盟appkey,如果create中设置了不用调用该函数
    // sdk->setAppKey("4eaee02c527015373b000003");
    // **********************	设置平台信息	***************************
    // sdk->setQQAppIdAndAppKey("设置QQ的app id", "appkey");
    // sdk->setWeiXinAppInfo("设置微信和朋友圈的app id","app key");
    // sdk->setYiXinAppKey("设置易信和易信朋友圈的app id");
    // sdk->setLaiwangAppInfo("设置来往和来往动态的app id", 
    //				"设置来往和来往动态的app key", "我的应用名");
    // sdk->setFacebookAppId("你的facebook appid");
    //     // 打开或者关闭log
    // sdk->setLogEnable(true) ;
    // **********************	END	***************************
    
    // 设置用户点击一条图文分享时用户跳转到的目标页面, 一般为app主页或者下载页面
    sdk->setTargetUrl("http://www.umeng.com/social");
    // 设置友盟分享面板上显示的平台
    vector<int>* platforms = new vector<int>();
    platforms->push_back(SINA);
    platforms->push_back(RENREN) ;
    platforms->push_back(DOUBAN) ;
    platforms->push_back(QZONE) ;
    platforms->push_back(QQ) ;
	// 设置平台, 在调用分享、授权相关的函数前必须设置SDK支持的平台
	sdk->setPlatforms(platforms) ;

    // 打开分享面板, 注册分享回调, 参数1为分享面板上的平台, 参数2为要分享的文字内容，
    // 参数3为要分享的图片路径(android和IOS的图片地址格式不一致，因此分平台设置), 参数4为分享回调.
   #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    	sdk->openShare("要分享的文字内容", "/sdcard/image.png", share_selector(shareCallback));
	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    	sdk->openShare("要分享的文字内容","share.png", share_selector(shareCallback));
	#endif
}

```      

***方式三 :***    
    开发者也可以自行定义某个按钮，然后在该按钮的点击事件中通过CCUMSocialSDK的directShare函数来进行直接分享到某个平台的操作，该接口为api分享接口，不会弹出分享面板和内容编辑界面，用户授权之后直接分享。示例代码如下 :   
```cpp
// 引入相关的头文件
#include "Cocos2dx/Common/CCUMSocialSDK.h"
// 使用友盟命令空间
USING_NS_UM_SOCIAL;
// ...... 代码省略


// HelloWorld为cocos2d::CCLayer的子类, shareButtonClick为某个按钮点击事件的处理函数
void HelloWorld::shareButtonClick()
{
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
    // 设置用户点击一条图文分享时用户跳转到的目标页面, 一般为app主页或者下载页面
    sdk->setTargetUrl("http://www.umeng.com/social");   
    // **********************	设置平台信息	***************************
    // sdk->setQQAppIdAndAppKey("设置QQ的app id", "appkey");
    // sdk->setSinaAppKey("设置新浪appkey", "设置新浪appSecret", "设置新浪redirectURL");
    // sdk->setWeiXinAppInfo("设置微信和朋友圈的app id","app key");
    // sdk->setYiXinAppKey("设置易信和易信朋友圈的app id");
    // sdk->setLaiwangAppInfo("设置来往和来往动态的app id", 
    //				"设置来往和来往动态的app key", "我的应用名");
    // sdk->setFacebookAppId("你的facebook appid");
    //     // 打开或者关闭log
    // sdk->setLogEnable(true) ;
    // **********************	END	***************************

    // 直接分享，参数1为要分享到的目标平台, 参数2为要分享的文字内容, 
    // 参数3为要分享的图片路径(android和IOS的图片地址格式不一致，因此分平台设置), 参数4为分享回调.
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		sdk->directShare(SINA, "要分享的文字内容","/sdcard/image.png", share_selector(shareCallback));
	#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    	sdk->directShare(SINA, "要分享的文字内容","image.png", share_selector(shareCallback));
	#endif
```      

***特别说明 :***     
   使用CCUMSocialSDK对象设置各个平台的app id或者app key.CCUMSocialSDK对象可以通过CCUMSocialSDK::create("umeng appkey")函数获取，如果使用UMShareButton可以通过getSocialSDK()函数获取.          
>1.如果集成了QQ或者QQ空间平台,则必须通过CCUMSocialSDK类的setQQAppIdAndAppKey("appid","appkey")函数来设置QQ或者QQ空间的AppId、AppKey;           
>2.如果集成了微信或者微信朋友圈平台,则必须通过CCUMSocialSDK类的setWeiXinAppInfo("","")函数来设置微信或者朋友圈的App id和app key;   
>3.iOS:如果集成了新浪微博平台,则必须通过CCUMSocialSDK类的setSinaAppKey("","","")函数来设置微博的appkey和appSecret;      
>4.如果集成了易信或者易信朋友圈平台,则必须通过CCUMSocialSDK类的setYiXinAppKey("")函数来设置微信的App key;        
>5.如果集成了来往或者来往动态平台,则必须通过CCUMSocialSDK类的setLaiwangAppInfo("app id ", "app key", "app Name")来设置来往和来往动态的App id、App key、App Name(应用名).  
>6.iOS:如果集成了Facebook平台,则必须通过CCUMSocialSDK类的setFacebookAppId("")函数来设置Facebook的app id;   
>7.iOS:如果集成了Twitter平台,则必须通过CCUMSocialSDK类的openTwitterForiOS("","")函数来设置Twitter的appKey和appSecret;  
   在设置完平台的app id或者app key之后，需要调用CCUMSocialSDK对象的setPlatforms函数将所需的平台添加到sdk中，这样该平台才会在sdk得到支持。    
   最后,点击对应的按钮则会弹出如下界面 :    
  <img src="http://dev.umeng.com/system/images/W1siZiIsIjIwMTQvMDQvMDkvMTVfNTdfNTVfMjk5X2NvY29zMmR4X29wZW5TaGFyZS5wbmciXV0/cocos2dx-openShare.png" width="450" height="300" style="border:1px solid black">      
         
**分享回调的为如下形式 :**    
   请不要在该回调中更新UI，友盟社会化组件Cocos2d-x SDK不保证在分享回调中更新UI一定会被顺利执行。      
```cpp
/*
 * 分享回调, 该回调不是某个类的成员函数， 而是一个普通的函数, 具体使用参考HelloWorldScene的例子
* @param platform 要分享到的目标平台
 * @param stCode 返回码, 200代表分享成功, 100代表开始分享
 * @param errorMsg 分享失败时的错误信息,android平台没有错误信息
 */
void shareCallback(int platform, int stCode, string& errorMsg)
{
    if ( stCode == 100 ) 
    {
        CCLog("#### HelloWorld 开始分享");
    }
    else if ( stCode == 200 ) 
    {
        CCLog("#### HelloWorld 分享成功");
    }
    else 
    {
        CCLog("#### HelloWorld 分享出错");
    }

    CCLog("platform num is : %d.", platform);
}
```     
          


<b id=cocos2dx_integration_auth></b>
##  授权接口使用说明
###  授权接口说明   
   CCUMSocialSDK类中还提供了授权相关的接口，支持授权的平台有新浪微博、QQ空间、QQ、人人网、豆瓣、腾讯微博、facebook。授权接口使用说明如下 : 
```cpp
CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
// 对某个平台授权, 参数二为授权回调
sdk->authorize(RENREN, auth_selector(authCallback));

// 判断某个平台是否授权
 sdk->isAuthorized(RENREN);

// 删除某个平台的授权
 sdk->deleteAuthorization(RENREN, auth_selector(authCallback));
```     

###  授权回调说明
   授权回调类型定义在CCUMTypeDef.h中，当授权成功时, 会将授权信息返回给开发者, 开发者可以通过遍历map来获取数据；    
    当授权失败, 会返回一个字段的数据, key为"msg", 值为错误的信息。如果是删除授权, 也是返回一个字段的数据, key为"msg", 值为"deleteOauth"。开发者可以通过判断返回码和map中的数据来进行相应的处理。    
   请不要在该回调中更新UI，友盟社会化组件Cocos2d-x SDK不保证在分享回调中更新UI一定会被顺利执行。    
   授权回调函数示例如下 :   
```cpp
/*
 *授权回调
 * @param platform 要授权的平台
 * @param stCode 返回码, 200代表授权成功, 100代表开始授权, 0代表授权出错, -1代表取消授权
 * @param data 授权时返回的数据
 */
void authCallback(int platform, int stCode, map<string, string>& data)
{
    if ( stCode == 100 ) 
    {
        CCLog("#### 授权开始");
    }
    else if ( stCode == 200 ) 
    {
        CCLog("#### 授权完成");
    } else if ( stCode == 0 ) 
    {
        CCLog("#### 授权出错");
    } else if ( stCode == -1 ) 
    {
        CCLog("#### 取消授权");
    }
 
    // 输入授权数据, 如果授权失败,则会输出错误信息
    map<string,string>::iterator it = data.begin();
    for (; it != data.end(); ++it) {
        CCLog("#### data  %s -> %s." , it->first.c_str(), it->second.c_str());
    }
}
```   

<b id=cocos2dx_integration_more_platforms></b>
##  添加更多平台 ( 按需集成 )     
	为了减小集成友盟社会化组件SDK占用的内存空间，Android SDK中默认只添加了新浪微博、腾讯微博、人人网、豆瓣这几个平台。其他的平台则需要您手动添加到SDK中。     
	与Android不用的是，iOS SDK中默认链接了各个平台所需的framework，如果您不需要某个平台，可以将该平台移除。    
	
<b id=cocos2dx_platforms_weixin_integration></b> 
###  集成微信和微信朋友圈     

   <font color="red">注意，集成微信或者微信朋友圈之前，您必须到<a href="http://open.weixin.qq.com/" target="_blank">微信开放平台</a>中申请app id,app key,并且正确填写应用的信息，对于Android平台您必须确保包名和APP的签名是正确的，然后通过审核后即可分享成功，否则将无法跳转到微信或者微信朋友圈的分享界面。</font>       
	要集成微信或者微信朋友圈，你需要在Cocos2d-x代码中添加如下代码 :    
```java
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
    // 设置微信的app id和app key
    sdk->setWeiXinAppInfo("wx485ab7ca6a7e02d1",
			"f9cad1d153aaa97925a2e2b5fa63d59f");

    // 设置友盟SDK支持的平台, 记得加上微信和微信朋友圈
    vector<int>* platforms = new vector<int>();
    platforms->push_back(WEIXIN);
    platforms->push_back(WEIXIN_CIRCLE) ;
    // 其他你需要的平台
    
	// 将这些平台添加到SDK中
	sdk->setPlatforms(platforms) ;
```    

***Android平台***     
	如果是在Android平台上添加微信或者微信朋友圈平台，需要将Platforms/Android/sns_platforms/weixin/libs目录下的文件拷贝到您工程中的libs目录中。还需要您到压缩包中的Platforms/Android/sns_platforms/weixin/目录下将wxapi目录拷贝到您的工程根包下面。
	您的根包定义在AndroidManafest.xml的package属性中，例如我的package为com.umeng.game,那么wxapi就要放在com.umeng.game包下,因此其完整路径为com.umeng.game.wxapi.WXEntryActivity。此时，可能会在WXEntryActivity有错误，您重新import一下所需要的类即可。       

***iOS平台***     
	需要设置Xcode的URL scheme，添加“URL Schemes”为微信appId，例如"wxd9a39c7122aa6516"



<b id=cocos2dx_platforms_qq_integration></b> 
###  集成QQ和QQ空间
   <font color="red">注意，集成QQ或者QQ空间之前，您必须到<a href="http://open.qq.com/" target="_blank">腾讯开放平台</a>中申请app id,app key,并且正确填写应用的信息，审核通过后即可分享成功。然后将app id填写到AndroidManifext.xml对应的位置中。</font>              
	在Cocos2d-x游戏中通过如下代码添加QQ空间或者QQ的支持：   
```java
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
    // 设置QQ和QQ空间的app id和app key
	sdk->setQQAppIdAndAppKey("100424468", "c7394704798a158208a74ab60104f0ba");

    // 设置友盟SDK支持的平台, 记得加上QQ或者QQ空间
    vector<int>* platforms = new vector<int>();
    platforms->push_back(QQ);
    platforms->push_back(QZONE) ;
    // 其他你需要的平台
    
	// 将这些平台添加到SDK中
	sdk->setPlatforms(platforms) ;
```    

***Android平台***      
   	针对Android平台，需要将Platforms/Android/sns_platforms/qq_qzone/libs目录下的文件拷贝到您工程中的libs目录中。这样，您就成功集成QQ或者QQ空间了。       
   
***iOS平台***         
	需要设置Xcode的URL scheme，添加“URL Schemes”为“QQ”+腾讯QQ互联应用appId转换成十六进制（不足8位前面补0），例如“QQ05FA957C”。生成十六进制方法：在命令行输入<div>echo 'ibase=10;obase=16;您的腾讯QQ互联应用Id'|bc</div>，并在QQ互联后台的URL schema中填入此字符串保持一致。  
	若你只使用授权到qq或者QQ空间，需要设置Xcode的URL scheme的“URL Schemes”为"tencent"+腾讯开放平台appId。


<b id=cocos2dx_platforms_laiwang_integration></b> 
###  集成来往和来往动态     
   	通过CCUMSocialSDK类的setLaiwangAppId("")和setLaiwangAppKey()函数来分别设置来往和来往动态的App id、app key，代码如下 :
```java
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
    // 设置来往或者来往动态的app id和app key
		sdk->setLaiwangAppInfo("设置来往和来往动态的app id", "设置来往和来往动态的app key", "我的应用名");

    // 设置友盟SDK支持的平台, 记得加上来往或者来往动态
    vector<int>* platforms = new vector<int>();
    platforms->push_back(LAIWANG);
    platforms->push_back(LAIWANG_CIRCLE) ;
    // 其他你需要的平台
    
	// 将这些平台添加到SDK中
	sdk->setPlatforms(platforms) ;
```     

***Android平台***    
   添加所需jar和资源，将Platforms/Android/sns_platforms/laiwang文件夹下的libs、res目录下的文件拷贝到工程中对应的文件夹中。这样，您就成功集成来往或者来往动态了。        
   
***iOS平台***     
	针对于iOS平台，集成来往平台需要将Xcode的other linker flag设置为-all_load 。
	但是如果您不需要集成来往，可以在Xcode中把UMSocial_Sdk_Extra_Frameworks下把LaiWang文件夹删除。并把UmSocialControllerIOS.h 头文件中把宏CC_ShareToLaiWang设置为0。  
 	需要设置Xcode的URL scheme，添加“URL Schemes”为来往AppId，“Identifier”为“Laiwang”
   
<b id=cocos2dx_platforms_yixin_integration></b> 
###  集成易信和易信朋友圈
           
   首先到<a href="http://open.yixin.im/" target="_blank">易信开发平台</a>中获取app id, app key等信息。然后在Cocos2d-x代码中，通过CCUMSocialSDK类的setYiXinAppKey("")函数来设置微信的App key，
```java
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
    // 设置易信和易信朋友圈的app id和app key
	sdk->setYiXinAppKey("yx52dd5c14bbb3430b93e6f2dafcbcc68d");

    // 设置友盟SDK支持的平台, 记得加上易信和易信朋友圈
    vector<int>* platforms = new vector<int>();
    platforms->push_back(YIXIN);
    platforms->push_back(LYIXIN_CIRCLE) ;
    // 其他你需要的平台
    
	// 将这些平台添加到SDK中
	sdk->setPlatforms(platforms) ;
```       

***Android平台***    
    添加所需jar和资源，将SDK压缩包中Platforms/Android/sns_platforms/yixin文件夹下的libs、res目录下的文件拷贝到工程中对应的文件夹中。      

**易信精确回调使用说明**    
如果在您的项目中集成了易信或者易信朋友圈，并且您需要准确的分享回调 (建议添加)，则需要在AndroidManifest.xml中下注册下面的回调Activity。
```xml
<activity
   android:name=".yxapi.YXEntryActivity"
   android:theme="@android:style/Theme.Translucent.NoTitleBar"
   android:configChanges="keyboardHidden|orientation|screenSize"
   android:exported="true"
   android:screenOrientation="portrait" />
```    
> * 将SDK包中yixin目录下的yxapi文件夹拷贝到您的工程的包目录下，然后修改YXEntryActivity的完整路径即可。例如social_sdk_example的包名为com.umeng.soexample,
因此将yxapi文件夹拷贝到com.umeng.soexample下即可。最终YXEntryActivity的完整路径为com.umeng.soexample.yxapi.YXEntryActivity。     
* 其中分享回调接口SnsPostListener中的onComplete方法的第二个参数代表分享的状态码，当值为200时表示分享成功;其余的值则为分享失败。    

***iOS平台***    
	iOS平台集成易信不需要其他的设置。如果您不需要集成易信，可以在Xcode中把UMSocial_Sdk_Extra_Frameworks下把YiXin文件夹删除。并把UmSocialControllerIOS.h 头文件中把宏CC_ShareToYiXin设置为0。  
	需要设置Xcode的URL scheme，添加“URL Schemes”为易信AppId。
      

 <b id=cocos2dx_platforms_facebook_integration></b> 
###  集成Facebook       
 首先到<a href="https://developers.facebook.com/">facebook开发者网站</a>(需翻墙)上创建应用，然后填写应用签名、包名等信息，然后获取app id等信息,通过facebook审核以后即可进行分享。在Cocos2d-x代码中首先需要设置facebook的app id，并且将facebook添加到sdk中, 代码如下 :
```java
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
    // 设置facebook的app id
	sdk->setFacebookAppId("567261760019884");

    // 设置友盟SDK支持的平台, 记得加上facebook
    vector<int>* platforms = new vector<int>();
    platforms->push_back(FACEBOOK);
    // 其他你需要的平台
    
	// 将这些平台添加到SDK中
	sdk->setPlatforms(platforms) ;
```      

**Android平台**    
   针对于Android平台, 您需要添加所需jar和资源，将SDK压缩包中Platforms/Android/sns_platforms/facebook文件夹下的libs、res目录下的文件拷贝到工程中对应的文件夹中。最后在AndroidManifest.xml中注册activity:      
```xml
        <!-- 如果使用，则必须添加以下activity和facebook_app_id (不使用facebook可以去掉) -->
        <activity
            android:name="com.facebook.LoginActivity"
            android:label="@string/app_name"
            android:theme="@android:style/Theme.Translucent.NoTitleBar" />
```         
	这样就集成完成。      

**iOS平台**  
	iOS平台集成facebook不再需要其他的设置。但是如果您不需要集成Facebook，可以在Xcode中把UMSocial_Sdk_Extra_Frameworks下把Facebook文件夹删除。并把UmSocialControllerIOS.h 头文件中把宏CC_ShareToFacebook设置为0。

<b id=cocos2dx_platforms_instagram_integration></b> 
###  集成Instagram
 
 在Cocos2d-x代码中添加instagram到SDK:
 ```cpp
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
 	// 其他设置

    // 设置友盟SDK支持的平台, 记得加上Instagram
    vector<int>* platforms = new vector<int>();
    platforms->push_back(INSTAGRAM);
    // 其他你需要的平台
    
	// 将这些平台添加到SDK中
	sdk->setPlatforms(platforms) ;
```      
 
**Android平台**
	针对于Android平台，您还需要添加所需jar和资源，将SDK压缩包中Platforms/Android/sns_platforms/instagram文件夹下的libs、res目录下的文件拷贝到工程中对应的文件夹中。     

**iOS平台**  
	iOS平台集成Instagram不再需要其他的设置。   
	若不需要集成Instagram，可以在Xcode中把UMSocial_Sdk_Extra_Frameworks下把Instagram文件夹删除。并把UmSocialControllerIOS.h 头文件中把宏CC_ShareToInstagram设置为0。


<b id=cocos2dx_platforms_twitter_integration></b> 
###  集成Twitter
	在Cocos2d-x游戏代码中添加如下代码:
```cpp
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
 	// 其他设置

    // 设置友盟SDK支持的平台, 记得加上Twitter
    vector<int>* platforms = new vector<int>();
    platforms->push_back(TWITTER);
    // 其他你需要的平台
    
	// 将这些平台添加到SDK中
	sdk->setPlatforms(platforms) ;
```      

***Android平台***
	无需设置。 

***iOS平台***
	iOS平台集成Twitter不再需要其他的设置。    
	若不需要集成Twitter，可以在Xcode中把UMSocial_Sdk_Extra_Frameworks下把Twitter文件夹删除。并把UmSocialControllerIOS.h 头文件中把宏CC_ShareToTwitter设置为0。    
	
	
<b id=cocos2dx_platforms_sms_integration></b> 
###  集成短信分享
	在Cocos2d-x游戏代码中添加如下代码:
```cpp
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
 	// 其他设置

    // 设置友盟SDK支持的平台, 记得加上Twitter
    vector<int>* platforms = new vector<int>();
    platforms->push_back(SMS);
    // 其他你需要的平台
    
	// 将这些平台添加到SDK中
	sdk->setPlatforms(platforms) ;
```      

***Android平台***
	将SDK压缩包中Platforms/Android/sns_platforms/sms文件夹下的libs、res目录下的文件拷贝到工程中对应的文件夹中即可。     

***iOS平台***
	iOS平台集成短信不再需要其他的设置。     
	
<b id=cocos2dx_platforms_email_integration></b> 
###  集成邮件分享
	在Cocos2d-x游戏代码中添加如下代码:
```cpp
	// 获取一个CCUMSocialSDK实例
    CCUMSocialSDK *sdk = CCUMSocialSDK::create("你的友盟appkey");
 	// 其他设置

    // 设置友盟SDK支持的平台, 记得加上EMAIL
    vector<int>* platforms = new vector<int>();
    platforms->push_back(EMAIL);
    // 其他你需要的平台
    
	// 将这些平台添加到SDK中
	sdk->setPlatforms(platforms) ;
```      

***Android平台***
	将SDK压缩包中Platforms/Android/sns_platforms/email文件夹下的libs、res目录下的文件拷贝到工程中对应的文件夹中即可。        

***iOS平台***
	iOS平台集成邮件不再需要其他的设置。       


<b id=cocos2dx_sso_auth></b>
##  设置SSO授权
<b id=cocos2dx_sina_sso></b>
###  新浪微博的SSO授权
   要使用新浪微博SSO授权，首先您需要在新浪微博开放平台填写游戏的相关信息。如有问题，请<a href="http://dev.umeng.com/social/android/detail-share/sso#SSO_2_1" target="_blank">新浪微博SSO授权设置</a>。               
	针对于Android平台需要支持新浪微博SSO授权，首先您需要将压缩包下的platforms/Android/sns_platforms/sina/src/目录下的com文件夹拷贝到工程中的src目下，并且将libs目录下的SocialSDK_sina.jar拷贝到工程中的libs目录下。然后通过如下代码支持新浪微博的SSO授权 :	
```java
	CCUMSocialSDK *sdk = CCUMSocialSDK::create("4eaee02c527015373b000003");
	// 设置平台，按照您的需要添加平台
	vector<int>* platforms = new vector<int>();
	// 添加新浪平台
	platforms->push_back(SINA);
	// 添加其他平台 
	
	// 添加平台到SDK中
	sdk->setPlatforms(platforms);

	// 设置新浪微博SSO授权,参数2修改为你的授权回调地址
	sdk->setSsoAuthorization(SINA,"http://sns.whalecloud.com/sina2/callback");
	// iOS通过以下设置
	sdk->setSinaAppKey("app key", "app secret", "http://sns.whalecloud.com/sina2/callback");

```     
	这样新浪就支持SSO授权了。     
	
<b id=cocos2dx_tencent_sso></b>
###  腾讯微博的SSO授权
	针对于Android平台的腾讯微博SSO授权，您需要将压缩包内的Platforms/Android/sns_platforms/tencent_wb/目录下的jar包拷贝到您工程的libs目录下，并且添加到"build path"中。然后添加如下代码即可支持SSO授权 : 
```java
	CCUMSocialSDK *sdk = CCUMSocialSDK::create("4eaee02c527015373b000003");
	// 设置平台，按照您的需要添加平台
	vector<int>* platforms = new vector<int>();
	// 添加腾讯微博平台
	platforms->push_back(TENCENT_WEIBO);
	// 添加其他平台 
	
	// 添加平台到SDK中
	sdk->setPlatforms(platforms);

	// 设置腾讯微博SSO授权, 参数2修改为你的授权回调地址
	sdk->setSsoAuthorization(TENCENT_WEIBO,"http://sns.whalecloud.com/tencent2/callback");
```     

	这样腾讯微博就支持SSO授权了。   


<b id=cocos2dx_renren_sso></b>
###  人人网的SSO授权（仅支持Android)
	由于人人网iOS SSO SDK在横屏下有问题，不支持人人网iOS的SSO授权。  
	针对于Android平台的人人网SSO授权，您需要将压缩包内的Platforms/Android/sns_platforms/renren/目录下的jar包拷贝到您工程的libs目录下，并且添加到"build path"中。然后添加如下代码即可支持SSO授权 : 
```java
	CCUMSocialSDK *sdk = CCUMSocialSDK::create("4eaee02c527015373b000003");
	// 设置平台，按照您的需要添加平台
	vector<int>* platforms = new vector<int>();
	// 添加人人网平台
	platforms->push_back(RENREN);
	// 添加其他平台 
	
	// 添加平台到SDK中
	sdk->setPlatforms(platforms);

	// 设置人人网SSO授权的app id, app key, app secret
	sdk->setRenrenAppInfo("人人网的app id", "人人网的app key", "人人网的app secret");
	
	// 最后设置人人网支持SSO授权
	sdk->setSsoAuthorization(RENREN,NULL);
```        
	最后在AndroidManifest.xml中添加如下权限:
```xml
    <!-- renren sso Permission for Unit Test -->
    <uses-permission android:name="android.permission.GET_TASKS" />
    <uses-permission android:name="android.permission.SET_DEBUG_APP" />
    <uses-permission android:name="android.permission.SYSTEM_ALERT_WINDOW" />
    <uses-permission android:name="android.permission.GET_ACCOUNTS" />
    <uses-permission android:name="android.permission.USE_CREDENTIALS" />
    <uses-permission android:name="android.permission.MANAGE_ACCOUNTS" />
```    
	并且如下Activity注册 :
```xml
    <!-- 人人SSO授权所需的OAuthActivity -->
        <activity
            android:name="com.renn.rennsdk.oauth.OAuthActivity"
            android:configChanges="orientation|navigation|keyboardHidden" />
```          
	这样人人网就支持SSO授权了。   

<b id=cocos2dx_platform_sharecontent></b>
##  设置分平台的分享内容
	通过CCUMSocialSDK的directShare和openShare函数设置的分享内容都是各个平台通用的，即所有平台的分享内容都是一样的。
	在某些情况下，你需要某些平台的分享内容与其他平台的不一样，那么您可以使用如下接口来设置 : 
```java
CCUMSocialSDK *sdk = CCUMSocialSDK::create("4eaee02c527015373b000003");
// 设置新浪微博的分享内容
sdk->setPlatformShareContent(SINA, "sina share 内容",
			"assets/CloseNormal.png", "sina-title");
// 设置微信的分享内容
sdk->setPlatformShareContent(WEIXIN, "WEIXIN share 内容",
			"assets/CloseNormal.png", "WEIXIN-title",
			"http://blog.csdn.net/bboyfeiyu");
// 设置QQ的分享内容
sdk->setPlatformShareContent(QQ, "QQ share 内容", "assets/CloseNormal.png",
			"QQ-title", "http://blog.csdn.net/bboyfeiyu");
```    
	该接口有五个参数，其中前三个参数为必填参数，分别为平台、文本内容、图片地址；后两个参数为选填，分别为分享时的标题、点击该条分享时跳转到的目标url,这两个属性主要在QQ、QQ空间、微信、微信朋友圈、来往、易信这几个平台上使用。
      	

<b id=proguard></b>
##  Android混淆         
  为了保证引用友盟Social SDK jar文件以及腾讯jar文件被混淆，请在proguard.cfg文件中添加以下代码避免被混淆.。   
```text
-dontwarn com.google.android.maps.**
-dontwarn android.webkit.WebView
-dontwarn com.umeng.**
-dontwarn com.tencent.weibo.sdk.**
-dontwarn com.facebook.**

-libraryjars libs/SocialSDK_QQZone_2.jar

-keep enum com.facebook.**
-keepattributes Exceptions,InnerClasses,Signature
-keepattributes *Annotation*
-keepattributes SourceFile,LineNumberTable

-keep public interface com.facebook.**
-keep public interface com.tencent.**
-keep public interface com.umeng.socialize.**
-keep public interface com.umeng.socialize.sensor.**
-keep public interface com.umeng.scrshot.**

-keep public class com.umeng.socialize.* {*;}
-keep public class javax.**
-keep public class android.webkit.**

-keep class com.facebook.**
-keep class com.umeng.scrshot.**
-keep public class com.tencent.** {*;}
-keep class com.umeng.socialize.sensor.**

-keep class com.tencent.mm.sdk.openapi.WXMediaMessage {*;}

-keep class com.tencent.mm.sdk.openapi.** implements com.tencent.mm.sdk.openapi.WXMediaMessage$IMediaObject {*;}

-keep class im.yixin.sdk.api.YXMessage {*;}
-keep class im.yixin.sdk.api.** implements im.yixin.sdk.api.YXMessage$YXMessageData{*;}

-keep public class [your_pkg].R$*{
    public static final int *;
}                                 
```

混淆过程中遇到的问题,请联系我们.

<b id=support></b>
## 技术支持        

请发邮件至social-support@umeng.com。如果您出现的问题和SDK相关，请说明您使用的是Android的SDK或者是iOS的SDK，我们会尽快回复您。
