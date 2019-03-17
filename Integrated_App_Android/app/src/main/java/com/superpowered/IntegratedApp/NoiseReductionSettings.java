package com.superpowered.IntegratedApp;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.text.DecimalFormat;

public class NoiseReductionSettings extends AppCompatActivity {

    TextView samplingFrequencyLabel, windowSizeLabel, overlapSizeLabel, audioLevelSpl, processingTime;
    EditText guiUpdateRate, noiseUpdateRate, splCalibration;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_noise_reduction_settings);
        getSupportActionBar().setTitle("Noise Reduction Settings");

        //Initializing IDs for Noise Reduction and Audio Settings
        initalizeIDs();
        enableIDs();
        // Load values for Noise Reduction and Audio Settings
        System.loadLibrary("FrequencyDomain");
        loadIDValues();
        //This updates the spl calibration setting
        splCalibration.setOnKeyListener(new View.OnKeyListener() {
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                // If the event is a key-down event on the "enter" button
                if ((event.getAction() == KeyEvent.ACTION_DOWN) &&
                        (keyCode == KeyEvent.KEYCODE_ENTER)) {
                    // Perform action on key press
                    updateSettingsSPLCalibration(Float.parseFloat(splCalibration.getText().toString()));
                    return true;
                }
                return false;
            }
        });

        //This updates the spl guiUpdateRate setting
        guiUpdateRate.setOnKeyListener(new View.OnKeyListener() {
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                // If the event is a key-down event on the "enter" button
                if ((event.getAction() == KeyEvent.ACTION_DOWN) &&
                        (keyCode == KeyEvent.KEYCODE_ENTER)) {
                    // Perform action on key press
                    updateSettingsGUIUpdateRate(Float.parseFloat(guiUpdateRate.getText().toString()));
                    return true;
                }
                return false;
            }
        });

        //This updates the noise estimation update rate setting
        noiseUpdateRate.setOnKeyListener(new View.OnKeyListener() {
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                // If the event is a key-down event on the "enter" button
                if ((event.getAction() == KeyEvent.ACTION_DOWN) &&
                        (keyCode == KeyEvent.KEYCODE_ENTER)) {
                    // Perform action on key press
                    updateSettingsNoiseUpdateRate(Float.parseFloat(noiseUpdateRate.getText().toString()));
                    return true;
                }
                return false;
            }
        });
        //run the handler when it is play audio is on.
        if(getPlayAudioStatus()!=0) {
            handler.postDelayed(r, (long) (getGuiUpdateRate()));
            disableIDs();
        }
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        handler.removeCallbacks(r);
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

    Handler handler = new Handler();

    final Runnable r = new Runnable() {
        @Override
        public void run() {

            audioLevelSpl.setText(String.format("%.0f dB SPL  ",getdbPower()));
            processingTime.setText(String.format("%.2f ms  ", getExecutionTime()));
            handler.postDelayed(this, (long)(getGuiUpdateRate()*1000.0f));
        }
    };

    public void initalizeIDs() {
        samplingFrequencyLabel = (TextView) findViewById(R.id.fsLabel);
        windowSizeLabel = (TextView) findViewById(R.id.windowSizeLabel);
        overlapSizeLabel = (TextView) findViewById(R.id.overlapSizeLabel);
        audioLevelSpl = (TextView)findViewById(R.id.audioLevel);
        processingTime = (TextView) findViewById(R.id.frameProcTime);
        guiUpdateRate = (EditText) findViewById(R.id.guiUpdateRate);
        noiseUpdateRate = (EditText) findViewById(R.id.noiseUpdateRate);
        splCalibration = (EditText) findViewById(R.id.splCalibration);
    }

    public void loadIDValues(){
        samplingFrequencyLabel.setText(getFs()+" Hz  ");
        windowSizeLabel.setText(String.format("%.2f  ", getWindowSize()));
        overlapSizeLabel.setText(String.format("%.2f  ", getOverlapSize()));
        guiUpdateRate.setText(String.format("%.2f", getGuiUpdateRate()));
        noiseUpdateRate.setText(String.format("%.2f", getNoiseUpdateRate()));
        splCalibration.setText(String.format("%.2f", getSPLCalibration()));
        audioLevelSpl.setText(String.format("%.2f dB SPL  ",getdbPower()));
        processingTime.setText(String.format("%.2f ms  ", getExecutionTime()));
    }

    public void enableIDs(){
        splCalibration.setEnabled(true);
        guiUpdateRate.setEnabled(true);
        noiseUpdateRate.setEnabled(true);
    }

    public void disableIDs(){
        splCalibration.setEnabled(false);
        guiUpdateRate.setEnabled(false);
        noiseUpdateRate.setEnabled(false);
    }

    private native int getFs();
    private native int getPlayAudioStatus();
    private native float getdbPower();
    private native float getWindowSize();
    private native float getOverlapSize();
    private native float getGuiUpdateRate();
    private native float getNoiseUpdateRate();
    private native float getSPLCalibration();
    private native float getExecutionTime();
    private native float updateSettingsSPLCalibration(float calibration);
    private native float updateSettingsGUIUpdateRate(float guiUpdateRate);
    private native float updateSettingsNoiseUpdateRate(float noiseUpdateRate);
}
