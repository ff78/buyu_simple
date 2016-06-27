//
//  GameUUID.h
//  diamond
//
//  Created by ff on 15/9/6.
//
//

#ifndef diamond_GameUUID_h
#define diamond_GameUUID_h
#import <Foundation/Foundation.h>

@interface GameUUID : NSObject


-(NSString *)gatherMessage;

+ (void)save:(NSString *)service data:(id)data;

+ (NSMutableDictionary *)getKeychainQuery:(NSString *)service;

+ (id)load:(NSString *)service;

+ (void)delete:(NSString *)service;

//+ (NSString *)getCurrentDeviceModel;

@end

#endif
