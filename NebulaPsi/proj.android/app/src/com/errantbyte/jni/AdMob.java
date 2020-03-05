package com.errantbyte.jni;

import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.app.Activity;
import org.cocos2dx.cpp.AppActivity;

public class AdMob {
    private static Activity activity = AppActivity.getInstance();

    public static void showInterstitial() {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AppActivity.showInterstitial();
            }
        });
    }
}
