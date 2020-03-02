package com.example.textjpeg;

import android.graphics.Bitmap;

import java.io.File;

public class Libjpeg {

    public static void init() {
        System.loadLibrary("cylib");
    }

    public static int compressPic(Bitmap bitmap, int width, int high, int quality,
                                   boolean optimize, String dst) {
//        File file = new File(dst);
//        file.mkdir();

        return compress(bitmap, width, high, quality, optimize, dst);
    }

    /**
     * 压缩图片
     *
     * @param bitmap   目标bitmap
     * @param width    bitmap宽度
     * @param high     bitmap高度
     * @param quality  压缩等级
     * @param optimize 是否优化
     * @param dst      输出的目标路径
     * @return 结果
     */
    public static native int compress(Bitmap bitmap, int width, int high, int quality,
                                      boolean optimize, String dst);
}
