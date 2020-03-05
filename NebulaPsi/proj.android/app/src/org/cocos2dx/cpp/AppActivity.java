package org.cocos2dx.cpp;

import android.os.Bundle;
import android.app.Activity;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.widget.Toast;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.MobileAds;

public class AppActivity extends Cocos2dxActivity {
    private static Activity instance;
    private static InterstitialAd interstitialAd;

    private static final String AD_UNIT_ID = "ca-app-pub-3143297239120863/8558516338";
    private static final String APP_ID = "ca-app-pub-3143297239120863~9707946477";
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        instance = this;
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);

        // initialize the AdMob app
        MobileAds.initialize(this, APP_ID);

        interstitialAd = new InterstitialAd(this);
 
        // set the ad unit ID
        interstitialAd.setAdUnitId(AD_UNIT_ID);
 
        interstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                startAd();                
            }
        });
        
        startAd();
    }

    public static void startAd() {
        if (!interstitialAd.isLoading() && !interstitialAd.isLoaded()) {
            AdRequest adRequest = new AdRequest.Builder().build();
            interstitialAd.loadAd(adRequest);
        }
    }

    public static void showInterstitial() {
        if (interstitialAd != null && interstitialAd.isLoaded()) {
            interstitialAd.show();
        } else {
            //Toast.makeText(instance.getBaseContext(), "Ad did not load", Toast.LENGTH_SHORT).show();
            AppActivity.startAd();
        }        
    }

    public static Activity getInstance() {
        return instance;
    }

}
