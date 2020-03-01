package com.example.textjpeg;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class JpegActivity extends AppCompatActivity {

    private TextView textView;
    private String rsc = "";
    private TextView logView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jpeg);
        textView = findViewById(R.id.sample_text);
        logView = findViewById(R.id.log_text);
        Libjpeg.init();
    }

    public void openFile(View view) {
        ImgFileUtils.openFile(this, 1);
    }

    public void compress(View view) {
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;
        BitmapFactory.decodeFile(rsc, options);
        showLog("宽：" + options.outWidth);
        showLog("高：" + options.outHeight);

        options = new BitmapFactory.Options();
        options.inJustDecodeBounds = false;
        Bitmap bitmap = BitmapFactory.decodeFile(rsc, options);
        String s = ImgFileUtils.getImgPath(this);
        showLog("输出路径" + s);
        Libjpeg.compressPic(bitmap, bitmap.getWidth(), bitmap.getWidth(), 100, true, s);
    }

    private void showLog(String s) {
        String text = logView.getText().toString();
        logView.setText(text + "\n" + s);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == Activity.RESULT_OK) {
            if (requestCode == 1 && data != null) {
                Uri uri = data.getData();
                if (uri != null) {
                    textView.setText(ImgFileUtils.getRealPathFromUri(this, uri));
                    rsc = ImgFileUtils.getRealPathFromUri(this, uri);
                }
            }
        }
    }


}
