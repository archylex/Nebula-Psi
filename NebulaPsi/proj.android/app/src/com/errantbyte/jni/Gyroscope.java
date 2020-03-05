package com.errantbyte.jni;


import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.content.pm.PackageManager;
import android.os.Bundle;
import org.cocos2dx.lib.Cocos2dxActivity;

public class Gyroscope implements SensorEventListener {
    
    private static Gyroscope mInstance;
    private Sensor mSensor;
    private static boolean isGyro;
    private static float[] coords;

    private static Gyroscope getInstance() {

        if (mInstance == null) {
            mInstance = new Gyroscope();
            mInstance.isGyro = Cocos2dxActivity.getContext().getPackageManager().hasSystemFeature(PackageManager.FEATURE_SENSOR_GYROSCOPE);
            mInstance.coords = new float[2];
        }

        return mInstance;
    }

    public static boolean startProcess() {

        if (mInstance == null)
            mInstance = getInstance();

        if (mInstance.isGyro) {
            SensorManager mSensorManager = (SensorManager) Cocos2dxActivity.getContext().getSystemService(Context.SENSOR_SERVICE);
            mInstance.mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
            mSensorManager.registerListener(mInstance, mInstance.mSensor, SensorManager.SENSOR_DELAY_NORMAL);
        }

        return mInstance.isGyro;
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        mInstance.coords[0] = event.values[0];
        mInstance.coords[1] = event.values[1];
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    public static float[] getCoordinates() {
        return mInstance.coords;
    }

    public static void stopProcess() {
        SensorManager mSensorManager = (SensorManager) Cocos2dxActivity.getContext().getSystemService(Context.SENSOR_SERVICE);
        mSensorManager.unregisterListener(mInstance);
        mInstance = null;
    }
}


