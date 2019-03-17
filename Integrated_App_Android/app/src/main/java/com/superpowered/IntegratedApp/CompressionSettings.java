package com.superpowered.IntegratedApp;

import android.content.SharedPreferences;
import android.os.SystemClock;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.SeekBar;
import android.widget.TextView;

public class CompressionSettings extends AppCompatActivity {

    SeekBar  b1ThSlider, b2ThSlider, b3ThSlider, b4ThSlider, b5ThSlider;
    TextView b1ThLabel,  b2ThLabel,  b3ThLabel,  b4ThLabel,  b5ThLabel;
    SeekBar  b1CrSlider, b2CrSlider, b3CrSlider, b4CrSlider, b5CrSlider;
    TextView b1CrLabel,  b2CrLabel,  b3CrLabel,  b4CrLabel,  b5CrLabel;
    SeekBar  b1AtSlider, b2AtSlider, b3AtSlider, b4AtSlider, b5AtSlider;
    TextView b1AtLabel,  b2AtLabel,  b3AtLabel,  b4AtLabel,  b5AtLabel;
    SeekBar  b1RtSlider, b2RtSlider, b3RtSlider, b4RtSlider, b5RtSlider;
    TextView b1RtLabel,  b2RtLabel,  b3RtLabel,  b4RtLabel,  b5RtLabel;

    SharedPreferences prefs;
    SharedPreferences.Editor prefEdit;
    float[] dataIn;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_compression_settings);
        getSupportActionBar().setTitle("Compression Settings");

        initializeIds();
        prefs = PreferenceManager.getDefaultSharedPreferences(this);
        prefEdit = prefs.edit();

        Update_Slider();
        Update_Label();



        b1CrSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionRationValue(b1CrSlider);
                b1CrLabel.setText(value+"  ");
                updateCompressionSettingsData(0, value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b1ThSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionThresholdValue(b1ThSlider);
                b1ThLabel.setText(value+"  ");
                updateCompressionSettingsData(1, value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b1AtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b1AtLabel.setText(b1AtSlider.getProgress()+"  ");
                updateCompressionSettingsData(2,b1AtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b1RtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b1RtLabel.setText(b1RtSlider.getProgress()+"  ");
                updateCompressionSettingsData(3,b1RtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b2CrSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionRationValue(b2CrSlider);
                b2CrLabel.setText(value+"  ");
                updateCompressionSettingsData(4, value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b2ThSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionThresholdValue(b2ThSlider);
                b2ThLabel.setText(value+"  ");
                updateCompressionSettingsData(5, value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b2AtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b2AtLabel.setText(b2AtSlider.getProgress()+"  ");
                updateCompressionSettingsData(6, b2AtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b2RtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b2RtLabel.setText(b2RtSlider.getProgress()+"  ");
                updateCompressionSettingsData(7,b2RtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b3CrSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionRationValue(b3CrSlider);
                b3CrLabel.setText(value+"  ");
                updateCompressionSettingsData(8,value);

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b3ThSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionThresholdValue(b3ThSlider);
                b3ThLabel.setText(value+"  ");
                updateCompressionSettingsData(9,value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b3AtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b3AtLabel.setText(b3AtSlider.getProgress()+"  ");
                updateCompressionSettingsData(10,b3AtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b3RtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b3RtLabel.setText(b3RtSlider.getProgress()+"  ");
                updateCompressionSettingsData(11,b3RtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b4CrSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionRationValue(b4CrSlider);
                b4CrLabel.setText(value+"  ");
                updateCompressionSettingsData(12,value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b4ThSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionThresholdValue(b4ThSlider);
                b4ThLabel.setText(value+"  ");
                updateCompressionSettingsData(13,value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b4AtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b4AtLabel.setText(b4AtSlider.getProgress()+"  ");
                updateCompressionSettingsData(14,b4AtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b4RtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b4RtLabel.setText(b4RtSlider.getProgress()+"  ");
                updateCompressionSettingsData(15,b4RtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b5CrSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionRationValue(b5CrSlider);
                b5CrLabel.setText(value+"  ");
                updateCompressionSettingsData(16, value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b5ThSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int value = adjustCompressionThresholdValue(b5ThSlider);
                b5ThLabel.setText(value+"  ");
                updateCompressionSettingsData(17, value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b5AtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b5AtLabel.setText(b5AtSlider.getProgress()+"  ");
                updateCompressionSettingsData(18, b5AtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        b5RtSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                b5RtLabel.setText(b5RtSlider.getProgress()+"  ");
                updateCompressionSettingsData(19, b5RtSlider.getProgress());
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

    }

    public void Update_Label() {
        b1CrLabel.setText(adjustCompressionRationValue(b1CrSlider)+"  ");
        b1ThLabel.setText(adjustCompressionThresholdValue(b1ThSlider)+"  ");
        b1AtLabel.setText(b1AtSlider.getProgress()+"  ");
        b1RtLabel.setText(b1RtSlider.getProgress()+"  ");

        b2CrLabel.setText(adjustCompressionRationValue(b2CrSlider)+"  ");
        b2ThLabel.setText(adjustCompressionThresholdValue(b2ThSlider)+"  ");
        b2AtLabel.setText(b2AtSlider.getProgress()+"  ");
        b2RtLabel.setText(b2RtSlider.getProgress()+"  ");

        b3CrLabel.setText(adjustCompressionRationValue(b3CrSlider)+"  ");
        b3ThLabel.setText(adjustCompressionThresholdValue(b3ThSlider)+"  ");
        b3AtLabel.setText(b3AtSlider.getProgress()+"  ");
        b3RtLabel.setText(b3RtSlider.getProgress()+"  ");

        b4CrLabel.setText(adjustCompressionRationValue(b4CrSlider)+"  ");
        b4ThLabel.setText(adjustCompressionThresholdValue(b4ThSlider)+"  ");
        b4AtLabel.setText(b4AtSlider.getProgress()+"  ");
        b4RtLabel.setText(b4RtSlider.getProgress()+"  ");

        b5CrLabel.setText(adjustCompressionRationValue(b5CrSlider)+"  ");
        b5ThLabel.setText(adjustCompressionThresholdValue(b5ThSlider)+"  ");
        b5AtLabel.setText(b5AtSlider.getProgress()+"  ");
        b5RtLabel.setText(b5RtSlider.getProgress()+"  ");
    }

    public void Update_Slider() {

        //No. of compression parameters are 5(no of bands) * 4(parameters per band) = 20;
        System.loadLibrary("FrequencyDomain");
        dataIn = getCompressionSettingsData();

        b1CrSlider.setProgress((int)dataIn[0]-1);
        b1ThSlider.setProgress((int)dataIn[1]+b1ThSlider.getMax());
        b1AtSlider.setProgress((int)dataIn[2]);
        b1RtSlider.setProgress((int)dataIn[3]);

        b2CrSlider.setProgress((int)dataIn[4]-1);
        b2ThSlider.setProgress((int)dataIn[5]+b2ThSlider.getMax());
        b2AtSlider.setProgress((int)dataIn[6]);
        b2RtSlider.setProgress((int)dataIn[7]);

        b3CrSlider.setProgress((int)dataIn[8]-1);
        b3ThSlider.setProgress((int)dataIn[9]+b3ThSlider.getMax());
        b3AtSlider.setProgress((int)dataIn[10]);
        b3RtSlider.setProgress((int)dataIn[11]);

        b4CrSlider.setProgress((int)dataIn[12]-1);
        b4ThSlider.setProgress((int)dataIn[13]+b4ThSlider.getMax());
        b4AtSlider.setProgress((int)dataIn[14]);
        b4RtSlider.setProgress((int)dataIn[15]);

        b5CrSlider.setProgress((int)dataIn[16]-1);
        b5ThSlider.setProgress((int)dataIn[17]+b5ThSlider.getMax());
        b5AtSlider.setProgress((int)dataIn[18]);
        b5RtSlider.setProgress((int)dataIn[19]);


    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        int id = item.getItemId();

        if (id == android.R.id.home) {
            onBackPressed();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }


    public int adjustCompressionThresholdValue(SeekBar seekBar){
        return (seekBar.getProgress()-seekBar.getMax());
    }

    public int adjustCompressionRationValue(SeekBar seekBar){
        return (seekBar.getProgress()+1);
    }

    public void initializeIds(){

        b1ThSlider = (SeekBar)findViewById(R.id.b1ThSlider); b1ThLabel = (TextView) findViewById(R.id.b1ThLabel);
        b1CrSlider = (SeekBar)findViewById(R.id.b1CrSlider); b1CrLabel = (TextView) findViewById(R.id.b1CrLabel);
        b1AtSlider = (SeekBar)findViewById(R.id.b1AtSlider); b1AtLabel = (TextView) findViewById(R.id.b1AtLabel);
        b1RtSlider = (SeekBar)findViewById(R.id.b1RtSlider); b1RtLabel = (TextView) findViewById(R.id.b1RtLabel);

        b2ThSlider = (SeekBar)findViewById(R.id.b2ThSlider); b2ThLabel = (TextView) findViewById(R.id.b2ThLabel);
        b2CrSlider = (SeekBar)findViewById(R.id.b2CrSlider); b2CrLabel = (TextView) findViewById(R.id.b2CrLabel);
        b2AtSlider = (SeekBar)findViewById(R.id.b2AtSlider); b2AtLabel = (TextView) findViewById(R.id.b2AtLabel);
        b2RtSlider = (SeekBar)findViewById(R.id.b2RtSlider); b2RtLabel = (TextView) findViewById(R.id.b2RtLabel);

        b3ThSlider = (SeekBar)findViewById(R.id.b3ThSlider); b3ThLabel = (TextView) findViewById(R.id.b3ThLabel);
        b3CrSlider = (SeekBar)findViewById(R.id.b3CrSlider); b3CrLabel = (TextView) findViewById(R.id.b3CrLabel);
        b3AtSlider = (SeekBar)findViewById(R.id.b3AtSlider); b3AtLabel = (TextView) findViewById(R.id.b3AtLabel);
        b3RtSlider = (SeekBar)findViewById(R.id.b3RtSlider); b3RtLabel = (TextView) findViewById(R.id.b3RtLabel);

        b4ThSlider = (SeekBar)findViewById(R.id.b4ThSlider); b4ThLabel = (TextView) findViewById(R.id.b4ThLabel);
        b4CrSlider = (SeekBar)findViewById(R.id.b4CrSlider); b4CrLabel = (TextView) findViewById(R.id.b4CrLabel);
        b4AtSlider = (SeekBar)findViewById(R.id.b4AtSlider); b4AtLabel = (TextView) findViewById(R.id.b4AtLabel);
        b4RtSlider = (SeekBar)findViewById(R.id.b4RtSlider); b4RtLabel = (TextView) findViewById(R.id.b4RtLabel);

        b5ThSlider = (SeekBar)findViewById(R.id.b5ThSlider); b5ThLabel = (TextView) findViewById(R.id.b5ThLabel);
        b5CrSlider = (SeekBar)findViewById(R.id.b5CrSlider); b5CrLabel = (TextView) findViewById(R.id.b5CrLabel);
        b5AtSlider = (SeekBar)findViewById(R.id.b5AtSlider); b5AtLabel = (TextView) findViewById(R.id.b5AtLabel);
        b5RtSlider = (SeekBar)findViewById(R.id.b5RtSlider); b5RtLabel = (TextView) findViewById(R.id.b5RtLabel);
    }

    private native float[] getCompressionSettingsData();
    private native void updateCompressionSettingsData(int index, int data);

}
