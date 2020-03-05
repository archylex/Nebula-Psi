package com.errantbyte.jni;

import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.app.Activity;
import org.cocos2dx.cpp.AppActivity;
import android.widget.Toast;

public class PsiToast {
    private static Activity activity = AppActivity.getInstance();

    public static void showToast(final String message) {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(activity.getBaseContext(), message, Toast.LENGTH_LONG).show();
            }
        });
    }
}
