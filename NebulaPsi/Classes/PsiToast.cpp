#include "PsiToast.h"

#define CLASS_NAME "com/errantbyte/jni/PsiToast"
#define METHOD_NAME "showToast"
#define PARAM_CODE "(Ljava/lang/String;)V"

USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void PsiToast::showToast(const char* message) {
    JniMethodInfo method;
    JniHelper::getStaticMethodInfo(method, CLASS_NAME, METHOD_NAME, PARAM_CODE);
    jstring stringMessage = method.env->NewStringUTF(message);

    method.env->CallStaticVoidMethod(method.classID, method.methodID, stringMessage);
    method.env->DeleteLocalRef(stringMessage);
}
#endif

