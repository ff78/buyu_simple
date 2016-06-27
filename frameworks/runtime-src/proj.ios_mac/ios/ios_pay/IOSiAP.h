//
//  IOSiAP.h
//  LongChengDaRen
//
//  Created by 白白 on 14-11-11.
//
//

#ifndef __iAP_JSBinding__IOSiAP__
#define __iAP_JSBinding__IOSiAP__

#include <iostream>
#include <vector>
//#include "MakeJson.h"

class IOSProduct
{
public:
    std::string productIdentifier;
    std::string localizedTitle;
    std::string localizedDescription;
    std::string localizedPrice;// has be localed, just display it on UI.
    bool isValid;
    int index;//internal use : index of skProducts
};

typedef enum {
    IOSIAP_PAYMENT_PURCHASING,// just notify, UI do nothing
    IOSIAP_PAYMENT_PURCHAED,// need unlock App Functionality
    IOSIAP_PAYMENT_FAILED,// remove waiting on UI, tall user payment was failed
    IOSIAP_PAYMENT_RESTORED,// need unlock App Functionality, consumble payment No need to care about this.
    IOSIAP_PAYMENT_REMOVED,// remove waiting on UI
    IOSIAP_PAYMENT_CANCEL
} IOSiAPPaymentEvent;

class IOSiAPDelegate
{
public:
    virtual ~IOSiAPDelegate() {}
    // for requestProduct
    virtual void onRequestProductsFinish(void) = 0;
    virtual void onRequestProductsError(int code) = 0;
    // for payment
    virtual void onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity) = 0;
    virtual void onValidSuccess(int productId) = 0;
    virtual void onValidFail() = 0;
};

class IOSiAP
{
public:
    static IOSiAP* getInstance();
    ~IOSiAP();
    void requestProducts(std::vector <std::string> &productIdentifiers);
    IOSProduct *iOSProductByIdentifier(std::string &identifier);
    void paymentWithProduct(IOSProduct *iosProduct, int quantity = 1);
//    bool requestOrderId(IOSProduct *iosProduct);
    
    IOSiAPDelegate *delegate;
    // ===  internal use for object-c class ===
    void *skProducts;// object-c SKProduct
    void *skTransactionObserver;// object-c TransactionObserver
    std::vector<IOSProduct *> iOSProducts;
    
//    std::string userId;
    std::string token;
    std::string receipt;
//    std::string orderId;
    int productId;
private:
    IOSiAP();
    static IOSiAP* instance;
};

#endif /* defined(__iAP_JSBinding__IOSiAP__) */
