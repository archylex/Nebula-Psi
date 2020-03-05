#include "Gyroscope.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#define CLASS_NAME "com/errantbyte/jni/Gyroscope"

bool Gyroscope::startProcess() {
    JniMethodInfo method;
    JniHelper::getStaticMethodInfo(method, CLASS_NAME, "startProcess", "()Z");    
    bool res = method.env->CallStaticBooleanMethod(method.classID, method.methodID);
    method.env->DeleteLocalRef(method.classID);

    return res;
}

Vec2 Gyroscope::getCoordinates() {
    JniMethodInfo method;
    JniHelper::getStaticMethodInfo(method, CLASS_NAME, "getCoordinates", "()[F");
    jfloatArray jcoords = (jfloatArray) method.env->CallStaticObjectMethod(method.classID, method.methodID);
    jfloat* coord = method.env->GetFloatArrayElements(jcoords, 0);
    method.env->DeleteLocalRef(method.classID);

    return Vec2(coord[0], coord[1]);
}

void Gyroscope::stopProcess()
{
    JniMethodInfo method;
    JniHelper::getStaticMethodInfo(method, CLASS_NAME, "stopProcess", "()V");
    method.env->CallStaticVoidMethod(method.classID, method.methodID);
    method.env->DeleteLocalRef(method.classID);
}

#endif

