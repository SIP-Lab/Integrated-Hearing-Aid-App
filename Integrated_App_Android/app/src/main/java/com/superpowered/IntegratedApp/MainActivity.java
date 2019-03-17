package com.superpowered.IntegratedApp;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.media.AudioManager;
import android.os.Build;
import android.os.Environment;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Html;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Button;
import android.widget.Toast;
import android.widget.ToggleButton;

import java.io.File;
import java.util.ArrayList;
import java.util.Date;

import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.List;

import static android.graphics.Color.argb;
import static com.superpowered.IntegratedApp.R.*;
import static java.lang.StrictMath.sqrt;

public class MainActivity extends AppCompatActivity {


    SeekBar amplifyBar;
    ToggleButton startStopButton;
    String samplerateString = null, buffersizeString = null;
    TextView amplificationLabel, noiseLabel, speechLabel, quietLabel;
    Switch noiseReductionSwitch, compressionSwitch, saveIOSwitch;

    private ActivityInference activityInference;
    private MovingAverageBuffer averageBuffer, timeBuffer;
    private int CNNUpdate = 62;

    String audiofileIn,
            folderName = Environment.getExternalStorageDirectory().toString() + "/IntegratedApp_Android/",
            audiofileOut;

    int quiet = argb(64,0,26,153),
            noise = argb(64,153,0,77),
            speech = argb(64,0,153,77),
            white = argb(255,255,255,255);

    SharedPreferences prefs;
    SharedPreferences.Editor prefEdit;
    public static final String appPreferences = "appPrefs";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(layout.activity_main);
        //getSupportActionBar().setTitle(Html.fromHtml("<small>Integrated App: VAD and Noise Classifier</small>"));
        getSupportActionBar().setTitle(" Signal and Image Processing Lab");
        getSupportActionBar().setDisplayShowHomeEnabled(true);
        getSupportActionBar().setDisplayUseLogoEnabled(true);
        getSupportActionBar().setLogo(mipmap.ic_utd_logo);

        if (Build.VERSION.SDK_INT >= 17) {
            AudioManager audioManager = (AudioManager) this.getSystemService(Context.AUDIO_SERVICE);
            Log.d("Sampling Rate", audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE));
            Log.d("Frame Size", audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER));

            samplerateString = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
            buffersizeString = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
        }


        initializeIds();
        //load the Settings structure which pass user data from between jni and java
        System.loadLibrary("FrequencyDomain");
        loadSettings();

        prefs = PreferenceManager.getDefaultSharedPreferences(this);
        prefEdit = prefs.edit();

        disableButtons();
        setLabelColor(-1);


        File folder = new File(folderName);
        if (!folder.exists()) {
            folder.mkdirs();
        }

        amplifyBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                float ampValue = getConvertedValue(progress,true);
                updateSettingsAmplification(ampValue);
                amplificationLabel.setText(String.format("  Amplification: %.2fx", ampValue));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
        destroySettings();
    }

    public void disableButtons() {
        noiseReductionSwitch.setEnabled(false);
        compressionSwitch.setEnabled(false);
        amplifyBar.setEnabled(false);
        saveIOSwitch.setEnabled(true);
    }

    public void enableButtons() {
        noiseReductionSwitch.setEnabled(true);
        compressionSwitch.setEnabled(true);
        amplifyBar.setEnabled(true);
        saveIOSwitch.setEnabled(false);
    }

    public float getConvertedValue(int intVal, boolean sliderToAmp){
        //float floatVal = 0.0f;
        float mappedValue = (1.0f * intVal)/(amplifyBar.getMax()/2.0f);
        if(sliderToAmp) {
            return mappedValue * mappedValue;
        } else {
            return (float) sqrt(mappedValue);
        }
    }

    public void setUserDefaults(){

        if(prefs.getBoolean("Initialized", false)) {
//            prefEdit.putInt("Sampling Frequency", samplingRate.getProgress());
//            prefEdit.putString("Frame Size", frameSize.getText().toString());
//            prefEdit.putString("Decision Rate", decisionRate.getText().toString());
//            prefEdit.putInt("Quiet Adjustment", quietAdjustment.getProgress());
//            prefEdit.putBoolean("Play Audio", playAudioSwitch.isChecked());
            prefEdit.apply();
        }
        else {
            prefEdit.putBoolean("Initialized", true);
            prefEdit.putInt("Sampling Frequency", 8);
            prefEdit.putString("Frame Size", "10.00");
            prefEdit.putString("Decision Rate", "1.0");
            prefEdit.putInt("Quiet Adjustment", 90);
            prefEdit.putBoolean("Play Audio", false);
            prefEdit.apply();
        }

    }
    public void loadUserDefaults(){
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        if(prefs.getBoolean("Initialized", false)) {

        }
    }


    public void initializeIds(){

        noiseLabel = (TextView) findViewById(id.noiseLabel);
        speechLabel = (TextView) findViewById(id.speechLabel);
        quietLabel = (TextView) findViewById(id.quietLabel);
        startStopButton = (ToggleButton)findViewById(id.startStopButton);
        amplifyBar = (SeekBar) findViewById(id.amplificationBar);
        amplificationLabel = (TextView) findViewById(id.amplification);
        noiseReductionSwitch = (Switch) findViewById(id.noiseReductionSwitch);
        compressionSwitch = (Switch) findViewById(id.compressionSwitch);
        saveIOSwitch = (Switch) findViewById(id.saveIOSwitch);

        activityInference = new ActivityInference(getApplicationContext());
        averageBuffer     = new MovingAverageBuffer(13);
        timeBuffer        = new MovingAverageBuffer(Integer.parseInt(samplerateString)/Integer.parseInt(buffersizeString));

    }

    public void onStartStopClick(View view){
        if (startStopButton.isChecked()){

            enableButtons();
            if(saveIOSwitch.isChecked()){
                String dateFormat = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss").format(new Date());
                audiofileIn = folderName+"Input_"+ dateFormat + ".wav";
                audiofileOut = folderName+"Output_"+ dateFormat + ".wav";
                Log.d("Filename In",audiofileIn);
                Log.d("Filename Out",audiofileOut);
            }
            setAudioPlay(1);
            if (Build.VERSION.SDK_INT >= 17) {
                //AudioManager audioManager = (AudioManager) this.getSystemService(Context.AUDIO_SERVICE);
                samplerateString = Integer.toString(getFs());//audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
                buffersizeString = Integer.toString(getStepSize());//audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
            }

            if (samplerateString == null) samplerateString = "44100";
            if (buffersizeString == null) buffersizeString = "512";

            System.loadLibrary("FrequencyDomain");
            FrequencyDomain(Integer.parseInt(samplerateString),
                    Integer.parseInt(buffersizeString),
                    saveIOSwitch.isChecked(),
                    audiofileIn, audiofileOut
            );

            handler.postDelayed(r,(long)getGuiUpdateRate());
            handler.postDelayed(classify, CNNUpdate);
        }else{
            disableButtons();
            setAudioPlay(0);
            setLabelColor(-1);
            System.loadLibrary("FrequencyDomain");
            StopAudio(audiofileIn, audiofileOut);
            handler.removeCallbacks(r);
            handler.removeCallbacks(classify);
        }
    }

    public void setLabelColor(int detectedClass){
        switch (detectedClass) {
            case 0:
                quietLabel.setBackgroundColor(quiet);
                noiseLabel.setBackgroundColor(white);
                speechLabel.setBackgroundColor(white);
                break;
            case 1:
                quietLabel.setBackgroundColor(white);
                noiseLabel.setBackgroundColor(noise);
                speechLabel.setBackgroundColor(white);
                break;
            case 2:
                quietLabel.setBackgroundColor(white);
                noiseLabel.setBackgroundColor(white);
                speechLabel.setBackgroundColor(speech);
                break;
            default:
                quietLabel.setBackgroundColor(white);
                noiseLabel.setBackgroundColor(white);
                speechLabel.setBackgroundColor(white);
                break;
        }
    }



    public void onSettingsClick(View view) {
        Intent intent = null;
        switch (view.getId()){
            case id.noiseReductionSettings:
                intent = new Intent(this,NoiseReductionSettings.class);
                break;
            case id.compressionSettings:
                intent = new Intent(this,CompressionSettings.class);
                break;
        }

        if (null!=intent) startActivity(intent);
    }





    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public void onBackPressed() {
        //super.onBackPressed();
        // dont call **super**, if u want disable back button in current screen.
    } //Pressing backbutton from MainActivity is disabled.
      // Close app using other buttons (home or overview)
      // uncomment super.onBackPressed() if you want to enable.
      // If someone intends to close app using backbutton,
      // it is recommended to close from task manager as well

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }
//        if(id == R.id.home) {
//            finish();
//            return true;
//        }

        return super.onOptionsItemSelected(item);
    }

    Handler handler = new Handler();

    final Runnable r = new Runnable() {
        @Override
        public void run() {
            //setLabelColor(getDetectedClass()+1);
            setLabelColor((getdbPower() < 60.0f) ? 0 : getDetectedClass() + 1);
            handler.postDelayed(this, (long)getGuiUpdateRate());
        }
    };

    final Runnable classify = new Runnable() {
        @Override
        public void run() {
            long startTime = System.currentTimeMillis();
            float prob[] = activityInference.getActivityProb(GetMelImage());
            averageBuffer.addDatum((float) (prob[1]+0.15));
            timeBuffer.addDatum((System.currentTimeMillis() - startTime));
            setDetectedClass(averageBuffer.movingAverage);
            handler.postDelayed(this,CNNUpdate);
        }
    };

    public void noiseReductionSelect(View view){
        noiseReductionOn(noiseReductionSwitch.isChecked());
        updateSettingsAmplification(getConvertedValue(amplifyBar.getProgress(), false));
    }

    public void compressionSelect(View view){
        compressionOn(compressionSwitch.isChecked());
        updateSettingsAmplification(getConvertedValue(amplifyBar.getProgress(), false));
    }

    public static String getExtensionOfFile(String name)
    {
        String fileExtension="";

        // If fileName do not contain "." or starts with "." then it is not a valid file
        if(name.contains(".") && name.lastIndexOf(".")!= 0)
        {
            fileExtension=name.substring(name.lastIndexOf(".")+1);
        }

        return fileExtension;
    }


    private native void FrequencyDomain(int samplerate,
                                        int buffersize,
                                        boolean storeAudioFlag,
                                        String fileIn, String fileOut);
    private native void StopAudio(String fileIn, String fileOut);
    private native int getFs();
    private native void setAudioPlay(int on);
    private native int getDetectedClass();
    private native float getGuiUpdateRate();
    private native void noiseReductionOn(boolean on);
    private native void compressionOn(boolean on);
    private native int getStepSize();
    private native void loadSettings();
    private native void destroySettings();
    private native void updateSettingsAmplification(float ampValue);
    private native float[] GetMelImage();
    private native void setDetectedClass(float detectedClass);
    private native float getdbPower();
}
