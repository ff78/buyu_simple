package org.cocos2dx.lua;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.telephony.TelephonyManager;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxLuaJavaBridge;

import java.net.URI;

/**
 * Created by ff on 16/6/29.
 */
public class AndroidHelper {
    private static AndroidHelper mInstance = null;
    private String mDeviceId = null;
    //// 主Activity
    private Cocos2dxActivity mMainActivity = null;

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
//        IAppPay.init(mainActivity, IAppPay.LANDSCAPE, PayConfig.IAppPayAppId);
    }

    //deviceId
    public static String getDeviceID() {
        System.out.println("getDevice()");
        AndroidHelper mHelper = getInstance();
        TelephonyManager tm = (TelephonyManager) mHelper.mMainActivity.getSystemService(Context.TELEPHONY_SERVICE);

        getInstance().mDeviceId = tm.getDeviceId();
        System.out.println("mDeviceId: "+getInstance().mDeviceId);

        //// 如果IMEI没有获得，直接获得手机的Mac地址
        if (null == getInstance().mDeviceId || getInstance().mDeviceId.length() == 0) {

            WifiManager wm = (WifiManager) mHelper.mMainActivity.getSystemService(Context.WIFI_SERVICE);
            getInstance().mDeviceId = wm.getConnectionInfo().getMacAddress();
        }

        return getInstance().mDeviceId;
//        if (null == luaFuncName || "".equals(luaFuncName)) {
//
//        } else {
//            Cocos2dxLuaJavaBridge.callLuaGlobalFunctionWithString(luaFuncName, getInstance().mDeviceId);
//        }
    }

    public static void openURL(final String url) {
        Intent i =  new  Intent(Intent.ACTION_VIEW);
        i.setData(Uri.parse(url));
        getInstance().mMainActivity.startActivity(i);
    }

    public static void pay(int orderId) {

    }

    public static native void OnPayProcessResult(int orderId, int errNo);

}
