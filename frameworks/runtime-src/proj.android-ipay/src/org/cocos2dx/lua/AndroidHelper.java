package org.cocos2dx.lua;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.os.Looper;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.widget.Toast;
import com.iapppay.interfaces.callback.IPayResultCallback;
import com.iapppay.sdk.main.IAppPay;
import com.iapppay.sdk.main.IAppPayOrderUtils;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;

import java.text.SimpleDateFormat;


/**
 * Created by ff on 16/6/29.
 */

public class AndroidHelper implements IPayResultCallback {
    private static AndroidHelper mInstance = null;
    private String mDeviceId = null;
    //// 主Activity
    private Cocos2dxActivity mMainActivity = null;
    //// 主线程的Looper
    private Handler mMainLooper = new Handler(Looper.getMainLooper());
    //// 等待框
    private LoadingDialog mLoading = null;

    private int orderId;

    public static final String IAppPayAppId = "3005943193";

    //应用的私钥
    public static final String szPrivateKey = "MIICXgIBAAKBgQCYLfQTiY9hj1Mm/s1dPIeE1PxxMnJ0VCmI+fYaByaLPmRIErbX4j5lb/bA/uWRW/UN7OAp9C5jlNGLtAsAuFu1CgxmiDbkaYOIDKrVXvldAgDLAJriIJ/bmo1p/TTSqaLZiWl0Gp+I4/RD7pPbWCUtjmTI4gePQeNetfboL+ksVwIDAQABAoGAPKDiQv5lD1tIp5qQOvsoirVINdkBXHNDcP0VHV2H76XV0hkSm4PZlBQ9XLqwv27BfODIli5gZPAfY+dBEvJgJmzRgtyHxa/bdiHa+1s1C63LFcxhCUV4kQFwLB1kSaQXTCPl0Jksc0myIJoBbKJe6flEQeF8oP8DRGMPaAbo2yECQQDS8SbTwvbJ2jtVT1M2NCOl1XoPAfBxFqxYT9BXm+8+ZL9MiHgkh0NQTt2XCVCnOcnAxcJyLEyfIeB9p499IYVJAkEAuK+Eq6lhk85XghFkkSCdUfwFv0WMiSRyXKSSv6dyrlsuTlnpAk0YRxjRRTnNxtPa8qQw1bjXVkHXk5ft+zBEnwJBAJeDMTFPS3Xm81EuXgWD48r8j+RxryN7dqUkBvC2h1xYfvNY3nrWUSpAy3bt8Ol8X4bARQ8+9HYknhuDHFRznEkCQQCIDr5FnLZHJpdvOhMFm6pqkKlHPRyImDIsMA3wZZLmAkR98lznlhZRomXhgvEnDkJZyT405US2xjDkCeSHzgpvAkEAs9N73epW9kAX54snCDJj6wfGyLuYwGjBFTWdSEFseDxtIwKrB8kjj4TeA1MRkQ8UgMk1QYZiwLAfjw9rQS2zHg==";
    public static final String szPublicKey = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC1UZT9sTkPiYeybTkAZQclex+gNZygSPvapyBjwuYlfk5ABfYV7elVQflI1WwQgsg4bjwm7T2zv+59Ardao3Y/jo4/PEkiF0sfvKzIf5C83hZzzVuX6vCNXDryhwrfeMQPxnEgtPk8YCJvKCPUo3Ci6aqV+5uMJRm3yld5+Teq/QIDAQAB";
    //// 单例模型
    public static AndroidHelper getInstance() {

        if (null == mInstance) {
            mInstance = new AndroidHelper();
        }

        return mInstance;
    }
    /// 初始化支付接口
    public static void initIApp(Cocos2dxActivity mainActivity) {
        ////初始化主activity
        getInstance().mMainActivity = mainActivity;

        //爱贝初始化
        IAppPay.init(mainActivity, IAppPay.LANDSCAPE, IAppPayAppId);
    }

    //deviceId
    public static String getDeviceID() {
//        System.out.println("getDevice()");
        AndroidHelper mHelper = getInstance();
        TelephonyManager tm = (TelephonyManager) mHelper.mMainActivity.getSystemService(Context.TELEPHONY_SERVICE);

        getInstance().mDeviceId = tm.getDeviceId();
//        System.out.println("mDeviceId: "+getInstance().mDeviceId);

        //// 如果IMEI没有获得，直接获得手机的Mac地址
        if (null == getInstance().mDeviceId || getInstance().mDeviceId.length() == 0) {

            WifiManager wm = (WifiManager) mHelper.mMainActivity.getSystemService(Context.WIFI_SERVICE);
            getInstance().mDeviceId = wm.getConnectionInfo().getMacAddress();
        }

        return getInstance().mDeviceId;
    }

    public static void openURL(final String url) {
        Intent i =  new  Intent(Intent.ACTION_VIEW);
        i.setData(Uri.parse(url));
        getInstance().mMainActivity.startActivity(i);
    }

    public static void pay(int orderId, float price) {
//        System.out.println("order:" + orderId);
        getInstance().paying(orderId, price);
        getInstance().orderId = orderId;
    }

    public void paying(final int orderId, final float price)
    {
        mMainLooper.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (null == mLoading) mLoading = LoadingDialog.createDialog(mMainActivity);
                if (!mLoading.isShowing()) {
                    mLoading.setMessage("正在处理...");
                    mLoading.show();
                }


                startPay(getInstance().mMainActivity, getTransData(orderId,price));
            }
        }, 0);


    }

    private String getTransData(final int orderId, final float price) {
        IAppPayOrderUtils orderUtils = new IAppPayOrderUtils();
        orderUtils.setAppid(IAppPayAppId);
        orderUtils.setWaresid(orderId);
        orderUtils.setCporderid(getInstance().mDeviceId+buildCporderid());        ;
        orderUtils.setAppuserid(getInstance().mDeviceId);
        orderUtils.setPrice(price);
        return orderUtils.getTransdata(szPrivateKey);
    }
    public void startPay(Activity activity, String param) {
        IAppPay.startPay(activity, param, getInstance());
    }

    public static String buildCporderid(){
        long No = 0;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMddss");
        String nowdate = sdf.format(new java.util.Date());
        No = Long.parseLong(nowdate)*10000;
        return No+String.valueOf(Math.round(Math.random()*1000000));
    }

    public static native void OnPayProcessResult(int orderId, int errNo);

    @Override
    public void onPayResult(int resultCode, String signvalue, String resultInfo) {
        switch (resultCode) {
            case IAppPay.PAY_SUCCESS:
                boolean payState = IAppPayOrderUtils.checkPayResult(signvalue, szPublicKey);
                if(payState){
                    mLoading.dismiss();
                    OnPayProcessResult(orderId, 0);
                }
                break;
            case IAppPay.PAY_ING:
                Toast.makeText(getInstance().mMainActivity, "成功下单", Toast.LENGTH_LONG).show();
                break ;
            default:
                mLoading.dismiss();
                Toast.makeText(getInstance().mMainActivity, resultInfo, Toast.LENGTH_LONG).show();
                break;
        }
        Log.d("GoodsActivity", "requestCode:" + resultCode + ",signvalue:" + signvalue + ",resultInfo:" + resultInfo);
    }
}
