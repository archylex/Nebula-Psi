package com.errantbyte.jni;

import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.content.Context;
import org.cocos2dx.lib.Cocos2dxActivity;

public class CheckNet {
    
    private static CheckNet mInstance;
    private static boolean isConnected;
    private static ConnectivityManager cm;
    private static NetworkInfo activeNetwork;
    
    private static CheckNet getInstance() {

        if (mInstance == null) {
            mInstance = new CheckNet();
        }

        mInstance.isConnected = true;

        return mInstance;
    }

    public static boolean getNetCon() {

        if (mInstance == null)
            mInstance = getInstance();

        mInstance.cm = (ConnectivityManager)Cocos2dxActivity.getContext().getSystemService(Context.CONNECTIVITY_SERVICE);

        activeNetwork = mInstance.cm.getActiveNetworkInfo();
        mInstance.isConnected = mInstance.activeNetwork != null && mInstance.activeNetwork.isConnectedOrConnecting();  

        return mInstance.isConnected;
    }
    
}


