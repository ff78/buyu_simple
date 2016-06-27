//
//  GameUUID.m
//  diamond
//
//  Created by ff on 15/9/6.
//
//

#import "GameUUID.h"

@implementation GameUUID

-(NSString *)gatherMessage{
    NSString * const KEY_USERNAME_PASSWORD = @"com.jz.diamond.usernamepassword";
    NSString * const KEY_PASSWORD = @"com.jz.diamond.password";
    
    NSMutableDictionary *readUserPwd = (NSMutableDictionary *)[GameUUID load:KEY_USERNAME_PASSWORD];

    
    
    //采集用户设备信息
    
    NSUserDefaults *userDefaults=[NSUserDefaults standardUserDefaults];
    NSDate *loadDate=[NSDate date];
    NSDate *lastData=[userDefaults valueForKey:@"loadForDay"];
    NSDateFormatter *dateFormatter=[[NSDateFormatter alloc]init];
    [dateFormatter setDateFormat:@"YYYY/MM/dd"];

    NSString *lastd=[dateFormatter stringFromDate:lastData];
    NSString *nowd=[dateFormatter stringFromDate:loadDate];

    if (![lastd isEqualToString:nowd]) {
        [userDefaults setObject:loadDate forKey:@"loadForDay"];
//        //获取设备型号
//        NSString *deviceModel=[GameUUID getCurrentDeviceModel];
        //identifierForVendor 作为唯一标识
        NSString *identifierStr = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
        identifierStr = [identifierStr stringByReplacingOccurrencesOfString:@"-" withString:@""];
        
        NSMutableDictionary *usernamepasswordKVPairs = [NSMutableDictionary dictionary];
        [usernamepasswordKVPairs setObject:identifierStr forKey:KEY_PASSWORD];
        
//        NSMutableDictionary *readUserPwd = (NSMutableDictionary *)[GameUUID load:KEY_USERNAME_PASSWORD];
        if(readUserPwd==nil){
            //存
            [GameUUID save:KEY_USERNAME_PASSWORD data:usernamepasswordKVPairs];
            readUserPwd = (NSMutableDictionary *)[GameUUID load:KEY_USERNAME_PASSWORD];
        }
        
        //调用接口纪录登陆信息
//        NSString *string= [NSString stringWithFormat:@"%@cdpt/api/uselog?access_token=123&imeiid=%@&mtype=%@&device_platform=2",BaseURLString, [readUserPwd objectForKey:KEY_PASSWORD],deviceModel];
//        NSURL *url = [NSURL URLWithString:string];
//        NSURLRequest *request = [NSURLRequest requestWithURL:url];
//        NSLog(@"%@",string);
//        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
//        operation.responseSerializer = [AFJSONResponseSerializer serializer];
//        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
//        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
//        }];
//        
//        [operation start];
    }
    return [readUserPwd objectForKey:KEY_PASSWORD];
}



//存
+ (void)save:(NSString *)service data:(id)data {
    //Get search dictionary
    NSMutableDictionary *keychainQuery = [self getKeychainQuery:service];

    //Delete old item before add new item
    SecItemDelete((__bridge CFDictionaryRef)keychainQuery);
    
    //Add new object to search dictionary(Attention:the data format)
    [keychainQuery setObject:[NSKeyedArchiver archivedDataWithRootObject:data] forKey:(__bridge id)kSecValueData];
    
    //Add item to keychain with the search dictionary
    SecItemAdd((__bridge CFDictionaryRef)keychainQuery, NULL);
}

+ (NSMutableDictionary *)getKeychainQuery:(NSString *)service {
    return [NSMutableDictionary dictionaryWithObjectsAndKeys:
            (__bridge id)kSecClassGenericPassword,(__bridge id)kSecClass,
            service, (__bridge id)kSecAttrService,
            service, (__bridge id)kSecAttrAccount,
            (__bridge id)kSecAttrAccessibleAfterFirstUnlock,(__bridge id)kSecAttrAccessible,
            nil];
}

//取
+ (id)load:(NSString *)service {
    id ret = nil;
    NSMutableDictionary *keychainQuery = [self getKeychainQuery:service];
    
    //Configure the search setting
    
    //Since in our simple case we are expecting only a single attribute to be returned (the password) we can set the attribute kSecReturnData to kCFBooleanTrue
    [keychainQuery setObject:(__bridge id)kCFBooleanTrue forKey:(__bridge id)kSecReturnData];
    [keychainQuery setObject:(__bridge id)kSecMatchLimitOne forKey:(__bridge id)kSecMatchLimit];
    CFDataRef keyData = NULL;
    
    if (SecItemCopyMatching((__bridge CFDictionaryRef)keychainQuery, (CFTypeRef *)&keyData) == noErr) {
        @try {
            ret = [NSKeyedUnarchiver unarchiveObjectWithData:(__bridge NSData *)keyData];
        } @catch (NSException *e) {
            NSLog(@"Unarchive of %@ failed: %@", service, e);
        } @finally {
            
        }
    }
    
    if (keyData)
        CFRelease(keyData);
        return ret;
}

+ (void)delete:(NSString *)service {
    NSMutableDictionary *keychainQuery = [self getKeychainQuery:service];
    SecItemDelete((__bridge CFDictionaryRef)keychainQuery);
    
}

////获得设备型号
//+ (NSString *)getCurrentDeviceModel
//{
//    int mib[2];
//    size_t len;
//    char *machine;
//    mib[0] = CTL_HW;
//    mib[1] = HW_MACHINE;
//    sysctl(mib, 2, NULL, &len, NULL, 0);
//    machine = malloc(len);
//    
//    sysctl(mib, 2, machine, &len, NULL, 0);
//    NSString *platform = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
//    free(machine);
//    
//    return platform;
//}

@end