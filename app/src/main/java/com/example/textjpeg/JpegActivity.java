package com.example.textjpeg;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class JpegActivity extends AppCompatActivity {

    private TextView textView;
    private String rsc = "";
    private TextView logView;
    private CheckBox checkBox;
    private SeekBar seekBar;

    private boolean useOptimize = false;
    private int quality = 50;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jpeg);
        textView = findViewById(R.id.sample_text);
        logView = findViewById(R.id.log_text);
        checkBox = findViewById(R.id.checkbox);
        checkBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                useOptimize = isChecked;
            }
        });
        seekBar = findViewById(R.id.seekBar);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                quality = progress;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
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
        int i = Libjpeg.compressPic(bitmap, bitmap.getWidth(), bitmap.getWidth(), quality, useOptimize, s);
        showLog("结果" + i);
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
