#include "CheckNet.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#define CLASS_NAME "com/errantbyte/jni/CheckNet"

bool CheckNet::getNetCon() {
    JniMethodInfo method;
    JniHelper::getStaticMethodInfo(method, CLASS_NAME, "getNetCon", "()Z");
    bool res = method.env->CallStaticBooleanMethod(method.classID, method.methodID);
    method.env->DeleteLocalRef(method.classID);

    return res;
}


#endif
