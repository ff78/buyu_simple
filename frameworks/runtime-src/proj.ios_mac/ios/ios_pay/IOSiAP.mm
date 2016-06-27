#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
//#include "IPConfig.h"

#import "IOSiAP.h"


//#define PAY_REQ_ORDER_ADDR "http://cz.buyubisai.com/jz_appstore_rechargeV2.aspx"
//#define PAY_QUERY_ORDER "http://cz.buyubisai.com/jz_appstore_validateV2.aspx"

@interface iAPProductsRequestDelegate : NSObject<SKProductsRequestDelegate>
@property (nonatomic, assign) IOSiAP *iosiap;
@end

@implementation iAPProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request
     didReceiveResponse:(SKProductsResponse *)response
{
    // release old
    if (_iosiap->skProducts) {
        [(NSArray *)(_iosiap->skProducts) release];
    }
    // record new product
    _iosiap->skProducts = [response.products retain];
    
    for (int index = 0; index < [response.products count]; index++) {
        SKProduct *skProduct = [response.products objectAtIndex:index];
        
        // check is valid
        bool isValid = true;
        for (NSString *invalidIdentifier in response.invalidProductIdentifiers) {
            NSLog(@"invalidIdentifier:%@", invalidIdentifier);
            if ([skProduct.productIdentifier isEqualToString:invalidIdentifier]) {
                isValid = false;
                break;
            }
        }
        
        IOSProduct *iosProduct = new IOSProduct;
        iosProduct->productIdentifier = std::string([skProduct.productIdentifier UTF8String]);
        iosProduct->localizedTitle = std::string([skProduct.localizedTitle UTF8String]);
        iosProduct->localizedDescription = std::string([skProduct.localizedDescription UTF8String]);
        
        // locale price to string
        NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
        [formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [formatter setLocale:skProduct.priceLocale];
        NSString *priceStr = [formatter stringFromNumber:skProduct.price];
        [formatter release];
        iosProduct->localizedPrice = std::string([priceStr UTF8String]);
        
        iosProduct->index = index;
        iosProduct->isValid = isValid;
        _iosiap->iOSProducts.push_back(iosProduct);
    }
}

- (void)requestDidFinish:(SKRequest *)request
{
    _iosiap->delegate->onRequestProductsFinish();
    [request.delegate release];
    [request release];
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    NSLog(@"%@", error);
    _iosiap->delegate->onRequestProductsError([error code]);
}

@end

@interface iAPTransactionObserver :NSObject<SKPaymentTransactionObserver>
@property (nonatomic, assign) IOSiAP *iosiap;
@property (nonatomic, strong) NSMutableData *responseData;
@end

@implementation iAPTransactionObserver

//购买结果回调，Apple会根据Apple服务器的返回状态调用
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions) {
        std::string identifier([transaction.payment.productIdentifier UTF8String]);
        IOSiAPPaymentEvent event;
        
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchasing:
                event = IOSIAP_PAYMENT_PURCHASING;
                return;
            case SKPaymentTransactionStatePurchased:
            {
                event = IOSIAP_PAYMENT_PURCHAED;
                NSString* productIdentifier = transaction.payment.productIdentifier;
                NSString* receipt = nil;
                if ([[UIDevice currentDevice].systemVersion floatValue] >= 7.0) {
                    NSURLRequest* appstoreRequest = [NSURLRequest requestWithURL:[[NSBundle mainBundle]appStoreReceiptURL]];
                    
                    NSError *error = nil;
                    
                    NSData * receiptData = [NSURLConnection sendSynchronousRequest:appstoreRequest returningResponse:nil error:&error];
                    
                    receipt = [receiptData base64EncodedStringWithOptions:NSDataBase64EncodingEndLineWithLineFeed];
                } else {
                    NSData * receiptData = transaction.transactionReceipt;
                    
                    receipt = [receiptData base64Encoding];
                }

                
                _iosiap->delegate->onValidSuccess(_iosiap->productId);
//                if ([productIdentifier length] >0) {
//                    if (_iosiap->orderId == "") {
//                        break;
//                    }
//                    //
//                    NSString *server = [NSString stringWithUTF8String:IPConfig::getInstance()->getLogonServerInfo().validateAddr.c_str()];
//
//                    NSURL* url = [NSURL URLWithString:server];
//                    
//                    NSMutableURLRequest *mulRequest = [[NSMutableURLRequest alloc]initWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10];
//                    [mulRequest setHTTPMethod:@"POST"];
//                
//                    [mulRequest setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
//                        
//                    NSString* userId = [NSString stringWithCString:_iosiap->userId.c_str() encoding:[NSString defaultCStringEncoding]];
////                    NSString* token = [NSString stringWithCString:_iosiap->token.c_str() encoding:[NSString defaultCStringEncoding]];
//                    NSString* orderId = [NSString stringWithCString:_iosiap->orderId.c_str() encoding:[NSString defaultCStringEncoding]];
//                    NSDictionary *json = @{
//                                            @"userId" : userId,
//                                            @"orderId" : orderId,
//                                            @"sign" : receipt
//                                           };
//
//                    //    NSData --> NSDictionary
//                    // NSDictionary --> NSData
//                    NSData *data = [NSJSONSerialization dataWithJSONObject:json options:NSJSONWritingPrettyPrinted error:nil];
//                    mulRequest.HTTPBody = data;
//
//                    //异步代理模式，现在换成同步卡住的模式
////                    NSURLConnection* connection = [[NSURLConnection alloc] initWithRequest:mulRequest delegate:self];
//
//                    //如果请求订单时发生错误或网络中断，则不断重试
//                    bool btrue = false;
//                    
//                    while (!btrue) {
//                        NSData* data = [NSURLConnection sendSynchronousRequest:mulRequest returningResponse:nil error:nil];
//                        
//                        if(data == nil) continue;
//                        
//                        NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableLeaves error:nil];
//                        
//                        int success = [dict[@"ret"] intValue];
//                        if (success == 0) {
//                            IAP_RESULT result;
//                            result.status = 0;
//                            result.money = [dict[@"Gold"] intValue];
//                            result.phoneFee = [dict[@"PhoneFee"] intValue];
//                            result.vip = [dict[@"VIP"] intValue];
//                            result.Treature = [dict[@"Treature"] intValue];
//                            result.RechargeAll = [dict[@"RechargeAll"] intValue];
////                            auto mJson = MakeJson::create();
////                            auto str = mJson->getIAPResultJson(result);
//
//                            _iosiap->delegate->onValidSuccess(result);
//                            btrue = true;
//                        }else{
//                            //错误号502：游戏服务器在等待Apple服务器返回；需要重发
//                            //其他错误，说明验证出错，不要重发
////                            int errsno = [dict[@"errno"] intValue];
//                            
////                            NSLog(@"验证错误号：%d",errsno);
//                            NSLog(@"错误信息：%@",dict[@"reason"]);
////                            if (errsno != 502) {
//                                _iosiap->delegate->onValidFail();
//                                btrue = true;
////                            }
//                            
//                        }
//                        
//                    }
//                }
            }
                break;
            case SKPaymentTransactionStateFailed:
            {
                NSString* receipt = @"";
                if (transaction.error.code != SKErrorPaymentCancelled) {
                    event = IOSIAP_PAYMENT_FAILED;
                    receipt = @"fail";
                }else{
                    event = IOSIAP_PAYMENT_CANCEL;
                    receipt = @"cancel";
                }
//                //付款失败，要把失败和取消传给服务器，标志订单状态
//                NSLog(@"==ios payment error:%@", transaction.error);
//                
//                NSString* productIdentifier = transaction.payment.productIdentifier;
//                
//                if ([productIdentifier length] >0) {
//                    //
//                    NSString *server = [NSString stringWithUTF8String:IPConfig::getInstance()->getLogonServerInfo().validateAddr.c_str()];
//                    
//                    NSURL* url = [NSURL URLWithString:server];
//                    
//                    NSMutableURLRequest *mulRequest = [[NSMutableURLRequest alloc]initWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10];
//                    [mulRequest setHTTPMethod:@"POST"];
//                    
//                    [mulRequest setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
//                    
//                    NSString* userId = [NSString stringWithCString:_iosiap->userId.c_str() encoding:[NSString defaultCStringEncoding]];
////                    NSString* token = [NSString stringWithCString:_iosiap->token.c_str() encoding:[NSString defaultCStringEncoding]];
//                    NSString* orderId = [NSString stringWithCString:_iosiap->orderId.c_str() encoding:[NSString defaultCStringEncoding]];
//                    NSDictionary *json = @{
//                                           @"userId" : userId,
//                                           @"orderId" : orderId,
//                                           @"sign" : receipt
//                                           };
//                    
//                    //    NSData --> NSDictionary
//                    // NSDictionary --> NSData
//                    NSData *data = [NSJSONSerialization dataWithJSONObject:json options:NSJSONWritingPrettyPrinted error:nil];
//                    mulRequest.HTTPBody = data;
//                    
//                    //如果请求订单时发生错误或网络中断，则不断重试
//                    bool btrue = false;
//                    
//                    while (!btrue) {
//                        NSData* data = [NSURLConnection sendSynchronousRequest:mulRequest returningResponse:nil error:nil];
//                        
//                        if(data == nil) continue;
//                        
//                        NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableLeaves error:nil];
//                        
//                        int success = [dict[@"ret"] intValue];
//                        if (success == 0) {
//                            btrue = true;
//                        }else{
//                            //错误号502：游戏服务器在等待Apple服务器返回；需要重发
//                            //其他错误，说明验证出错，不要重发
////                            int errsno = [dict[@"errno"] intValue];
//                            
////                            NSLog(@"验证错误号：%d",errsno);
//                            NSLog(@"错误信息：%@",dict[@"reason"]);
////                            if (errsno != 502) {
//                                btrue = true;
////                            }
//                            
//                        }
//                        
//                    }
//                }
            }
                break;
            case SKPaymentTransactionStateRestored:
                // NOTE: consumble payment is NOT restorable
                event = IOSIAP_PAYMENT_RESTORED;
                break;
        }
        //付费成功后需要验证，由验证成功回调处理订单
        if (event != IOSIAP_PAYMENT_PURCHAED) {
            _iosiap->delegate->onPaymentEvent(identifier, event, transaction.payment.quantity);
        }
        
        //PURCHASING说明APPLE还在处理，不要结束transaction
        if (event != IOSIAP_PAYMENT_PURCHASING) {
            [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        }
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions) {
        std::string identifier([transaction.payment.productIdentifier UTF8String]);
        _iosiap->delegate->onPaymentEvent(identifier, IOSIAP_PAYMENT_REMOVED, transaction.payment.quantity);
    }
}
//
//-(void) connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
//{
//    NSLog(@"接收到服务器的响应");
//    self.responseData = [NSMutableData data];
//}
//
//-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
//{
//    NSLog(@"接收到服务器的数据");
//    //拼接数据
//    [self.responseData appendData:data];
//    NSLog(@"%lu---%@--",self.responseData.length,[NSThread currentThread]);
//}
//
//-(void) connectionDidFinishLoading:(NSURLConnection *)connection
//{
//    NSLog(@"");
//
//    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:self.responseData options:NSJSONReadingMutableLeaves error:nil];
//
//    NSString *error = dict[@"error"];
//    if (error) {
//
//    }else{
//        NSString *data = dict[@"result"];
//
//        _iosiap->delegate->onValidSuccess(data.UTF8String);
//    }
//}
//
//-(void) connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
//{
//    NSLog(@"");
//    _iosiap->delegate->onRequestProductsError(error.code);
//}

@end

IOSiAP* IOSiAP::instance = nullptr;
IOSiAP* IOSiAP::getInstance()
{
    if (instance == nullptr) {
        instance = new IOSiAP();
    }
    
    return instance;
}
IOSiAP::IOSiAP():
skProducts(nullptr),
delegate(nullptr)
{
    skTransactionObserver = [[iAPTransactionObserver alloc] init];
    ((iAPTransactionObserver *)skTransactionObserver).iosiap = this;
    [[SKPaymentQueue defaultQueue] addTransactionObserver:(iAPTransactionObserver *)skTransactionObserver];
}

IOSiAP::~IOSiAP()
{
    if (skProducts) {
        [(NSArray *)(skProducts) release];
    }
    
    std::vector <IOSProduct *>::iterator iterator;
    for (iterator = iOSProducts.begin(); iterator != iOSProducts.end(); iterator++) {
        IOSProduct *iosProduct = *iterator;
        delete iosProduct;
    }
    
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:(iAPTransactionObserver *)skTransactionObserver];
    [(iAPTransactionObserver *)skTransactionObserver release];
}

IOSProduct *IOSiAP::iOSProductByIdentifier(std::string &identifier)
{
    std::vector <IOSProduct *>::iterator iterator;
    for (iterator = iOSProducts.begin(); iterator != iOSProducts.end(); iterator++)
    {
        IOSProduct *iosProduct = *iterator;
        if (iosProduct->productIdentifier == identifier)
        {
            return iosProduct;
        }
    }

    return nullptr;
}

void IOSiAP::requestProducts(std::vector <std::string> &productIdentifiers)
{
    NSMutableSet *set = [NSMutableSet setWithCapacity:productIdentifiers.size()];
    std::vector <std::string>::iterator iterator;
    for (iterator = productIdentifiers.begin(); iterator != productIdentifiers.end(); iterator++) {
        [set addObject:[NSString stringWithUTF8String:(*iterator).c_str()]];
       
    }
    SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:set];
    iAPProductsRequestDelegate *delegate = [[iAPProductsRequestDelegate alloc] init];
    delegate.iosiap = this;
    productsRequest.delegate = delegate;
    [productsRequest start];
}

/*
bool IOSiAP::requestOrderId(IOSProduct *iosProduct)
{
    //向游戏服务器请求订单号
    NSString *server = [NSString stringWithUTF8String:IPConfig::getInstance()->getLogonServerInfo().rechargeAddr.c_str()];

    NSURL* url = [NSURL URLWithString:server];

    NSMutableURLRequest *mulRequest = [[NSMutableURLRequest alloc]initWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10];
    [mulRequest setHTTPMethod:@"POST"];

    [mulRequest setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];

    
//    iosProduct->productIdentifier
    NSString* userData = [NSString stringWithCString:userId.c_str() encoding:[NSString defaultCStringEncoding]];
//    NSString* tokenData = [NSString stringWithCString:token.c_str() encoding:[NSString defaultCStringEncoding]];
//    NSString* descript = [NSString stringWithCString:iosProduct->localizedDescription.c_str() encoding:[NSString defaultCStringEncoding]];
    NSNumber* prodId = [NSNumber numberWithInt:productId];
    NSDictionary *json = @{
                           @"userId" : userData,
                           @"productId" : prodId
                           };

    //    NSData --> NSDictionary
    // NSDictionary --> NSData
    NSData *data = [NSJSONSerialization dataWithJSONObject:json options:NSJSONWritingPrettyPrinted error:nil];
    mulRequest.HTTPBody = data;
    
    //如果请求订单时发生错误或网络中断，则重试10次
    bool btrue = false;
    int retryCount = 0;
    
    while (!btrue) {
        NSData* data = [NSURLConnection sendSynchronousRequest:mulRequest returningResponse:nil error:nil];
        
        if(data != nil)
        {
            NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableLeaves error:nil];
            
            int success = [dict[@"ret"] intValue];
            if (success == 0) {
                orderId = std::string([dict[@"orderId"] UTF8String]);
                btrue = true;
            }else{
                NSString *error = dict[@"reason"];
                NSLog(@"请求订单号错误：%@",error);
//                if (retryCount >= 10) {
                    return false;
//                }
            }
        }
        retryCount++;
            
    }
    
    return true;
}
*/ 
 
void IOSiAP::paymentWithProduct(IOSProduct *iosProduct, int quantity)
{
    SKProduct *skProduct = [(NSArray *)(skProducts) objectAtIndex:iosProduct->index];
    SKMutablePayment *payment = [SKMutablePayment paymentWithProduct:skProduct];
    payment.quantity = quantity;
    
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}