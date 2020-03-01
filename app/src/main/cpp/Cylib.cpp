#include <jni.h>
#include <android/bitmap.h>
#include <stdlib.h>
#include <string.h>
#include "include/jpeglib.h"
#include "include/jconfig.h"

typedef unsigned char byte;

int generateJPEG(byte *data, int w, int h, int quality, const char *name, bool optimize) {

    struct jpeg_compress_struct jcs;

    jpeg_create_compress(&jcs);

    return 0;
}

const char *jstringToString(JNIEnv *env, jstring jstr) {
    char *ret;
    const char *tempStr = env->GetStringUTFChars(jstr, NULL);
    jsize len = env->GetStringUTFLength(jstr);
    if (len > 0) {
        ret = (char *) malloc(len + 1);
        memcpy(ret, tempStr, len);
        ret[len] = 0;
    }
    env->ReleaseStringUTFChars(jstr, tempStr);
    return ret;
}


jint compressPic(JNIEnv *jniEnv, jclass object, jobject bitmap, jint width,
                 jint high, jint quality, jboolean optimize, jstring dst) {

    AndroidBitmapInfo info;
    int *pixelsColor;

    // 解码Android Bitmap信息
    if (AndroidBitmap_getInfo(jniEnv, bitmap, &info) < 0) {
        return 1;
    }

    // bitmap转成数组
    if (AndroidBitmap_lockPixels(jniEnv, bitmap, (void **) (&pixelsColor)) < 0) {
        return 1;
    }

    // argb rgb
    uint32_t h, w;
    int32_t formet;
    h = info.width;
    w = info.height;
    formet = info.format;
    int color;
    byte b, g, r;

    byte *tempByte;
    byte *data = (byte *) malloc(h * w * 3);
    tempByte = data;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            //只处理 RGBA_8888
            if (formet == ANDROID_BITMAP_FORMAT_RGBA_8888) {
                color = (*(int *) (pixelsColor));
                // 这里取到的颜色对应的 A B G R  各占8位
                b = (color >> 16) & 0xFF;
                g = (color >> 8) & 0xFF;
                r = (color >> 0) & 0xFF;
                *data = r;
                *(data + 1) = g;
                *(data + 2) = b;

                data += 3;
                pixelsColor += 1;

            } else {
                // 暂时先
                return 1;
            }
        }
    }
    AndroidBitmap_unlockPixels(jniEnv, bitmap);

    const char *dstFileName = jstringToString(jniEnv, dst);

    int rst = generateJPEG(tempByte, w, h, quality, dstFileName, optimize);

    free(tempByte);

    return rst;
}

int registerNativeMethods(JNIEnv *env) {

    JNINativeMethod methods[] = {
            {"compress", "(Landroid/graphics/Bitmap;IIIZLjava/lang/String;)I", (void *) compressPic},
    };

    const char *className = "com/example/textjpeg/Libjpeg";

    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    if (env == NULL) {
        return -1;
    }

    // 动态注册
    if (!registerNativeMethods(env)) {
        return -1;
    }

    return JNI_VERSION_1_6;
}

