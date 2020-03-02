#include <jni.h>
#include <android/bitmap.h>
#include <cstdio>
#include <malloc.h>
#include <cstring>
#include "include/jpeglib.h"

typedef unsigned char byte;

int generateJPEG(byte *data, unsigned int w, unsigned int h, int quality, const char *name,
                 bool optimize) {

    int nComponent = 3; // 说明是三通道 RGB  如果是1 就是只有灰色通道

    struct jpeg_compress_struct jcs;

    struct jpeg_error_mgr jem;
    jcs.err = jpeg_std_error(&jem);

    // 初始化
    jpeg_create_compress(&jcs);

    FILE *f = fopen(name, "wb");
    if (f == nullptr) {
        return 0;
    }

    // 指定图像输出目标
    jpeg_stdio_dest(&jcs, f);

    jcs.image_width = w;
    jcs.image_height = h;
    jcs.optimize_coding = optimize;
    jcs.input_components = nComponent;
    jcs.in_color_space = JCS_RGB;

    // 设置参数
    jpeg_set_defaults(&jcs);
    jpeg_set_quality(&jcs, quality, true); // 压缩的成都

    // 开始压缩
    jpeg_start_compress(&jcs, TRUE);

    JSAMPROW row_pointer[1];
    int row_stride;

    row_stride = jcs.image_width * nComponent; // 算出一行多长

    while (jcs.next_scanline < jcs.image_height) {
        row_pointer[0] = &data[jcs.next_scanline * row_stride];

        // 塞入数据
        jpeg_write_scanlines(&jcs, row_pointer, 1);
    }

    // 完成和销毁
    jpeg_finish_compress(&jcs);
    jpeg_destroy_compress(&jcs);
    fclose(f);

    return 0;
}

const char *jstringToString(JNIEnv *env, jstring jstr) {
    char *ret;
    const char *tempStr = env->GetStringUTFChars(jstr, nullptr);
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
    uint32_t h;
    uint32_t w;
    int32_t formet;
    w = info.width;
    h = info.height;
    formet = info.format;
    int color;
    byte b;
    byte g;
    byte r;

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

