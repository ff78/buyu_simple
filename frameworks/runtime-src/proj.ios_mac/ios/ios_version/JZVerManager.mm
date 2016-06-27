//
//  JZVerManager.m
//  diamond
//
//  Created by ff on 15/9/2.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "JZVerManager.h"

#ifndef APP_URL
//    #define APP_URL @"http://itunes.apple.com/lookup?id=1062224929"
#define APP_URL @"http://yuyu.91bisai.com"


//    #define APP_URL @"http://itunes.apple.com/lookup?id=1037694328"
#endif
@interface JZVerManager : NSObject

@property(nonatomic, strong)NSString* trackViewURL;
@property(nonatomic,strong)NSString*  mVersion;
@property(nonatomic, strong)NSString* mLastVersion;

+(JZVerManager*) getInstance;

-(bool) isOnLine;

-(void) openCommentUrl;

@end

static JZVerManager* mInstance = nil;
@implementation JZVerManager

+(JZVerManager*) getInstance{
    
    if(mInstance == nil){
        
        mInstance = [[JZVerManager alloc] init];
    }
    return mInstance;
}



-(bool) isOnLine{
    //得到本地app的版本
    NSDictionary* pdict = [[NSBundle mainBundle] infoDictionary];
    NSString* appVersionForLocal = [pdict objectForKey:@"CFBundleShortVersionString"];
    
    self.mVersion = [[NSString alloc] initWithString:appVersionForLocal];
    
    if(self.mLastVersion == nil){
        NSString *URL = APP_URL;
        NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
        [request setURL:[NSURL URLWithString:URL]];
        [request setHTTPMethod:@"POST"];
        NSHTTPURLResponse *urlResponse = nil;
        NSError *error = nil;
        NSData *recervedData = [NSURLConnection sendSynchronousRequest:request returningResponse:&urlResponse error:&error];
        
        if(recervedData == nil) return false;
        
        NSDictionary *dic  = [NSJSONSerialization JSONObjectWithData:recervedData options:NSJSONReadingMutableContainers error:nil];//[results JSONValue];
        NSArray *infoArray  = [dic objectForKey:@"results"];
        
        if ([infoArray count]) {
            NSDictionary *releaseInfo   = [infoArray objectAtIndex:0];
            NSString     *lastVersion   = [releaseInfo objectForKey:@"version"];
            self.mLastVersion = [[NSString alloc] initWithString:lastVersion];
        }else{
            self.mLastVersion=@"0";
        }
    }
    
    double ver1 = atof([self.mVersion UTF8String]);
    double ver2 = atof([self.mLastVersion UTF8String]);
    
    NSLog(@"ver1=%lf,ver2=%lf",ver1,ver2);
    
    return ver1<=ver2;//[self.mVersion isEqualToString:self.mLastVersion];
}

-(void) openCommentUrl{
    
    if (self.trackViewURL != nil) {
        
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:self.trackViewURL]];
        
    }else{
        NSString *URL = APP_URL;
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:URL]];
//        NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
//        [request setURL:[NSURL URLWithString:URL]];
//        [request setHTTPMethod:@"POST"];
//        NSHTTPURLResponse *urlResponse = nil;
//        NSError *error = nil;
//        NSData *recervedData = [NSURLConnection sendSynchronousRequest:request returningResponse:&urlResponse error:&error];
//        
//        if(recervedData == nil) return ;
//        
//        NSDictionary *dic  = [NSJSONSerialization JSONObjectWithData:recervedData options:NSJSONReadingMutableContainers error:nil];
//        NSArray *infoArray  = [dic objectForKey:@"results"];
//        
//        if ([infoArray count]) {
//            NSDictionary *releaseInfo   = [infoArray objectAtIndex:0];
//            NSString     *lastVersion   = [releaseInfo objectForKey:@"version"];
//            self.mLastVersion = [[NSString alloc] initWithString:lastVersion];
//            self.trackViewURL = [releaseInfo objectForKey:@"trackViewUrl"];
//            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:self.trackViewURL]];
//        }
    }
}

@end

GameIOS* GameIOS::instance = nullptr;
GameIOS* GameIOS::getInstance()
{
    if (instance == nullptr) {
        instance = new GameIOS();
    }
    return instance;
}
GameIOS::GameIOS()
{
    
}

GameIOS::~GameIOS()
{
    delete this;
}

void GameIOS::openCommentUrl()
{
    [[JZVerManager getInstance] openCommentUrl];
}

bool GameIOS::isOnLine()
{
    return [[JZVerManager getInstance] isOnLine];
}