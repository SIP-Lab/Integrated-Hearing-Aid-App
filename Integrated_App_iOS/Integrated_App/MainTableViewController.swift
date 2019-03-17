//
//  MainTableViewController.swift
//  SPP_Integrated_App
//
//  Created by Akshay Chitale on 6/12/17.
//  Copyright © 2017 UT Dallas. All rights reserved.
//

import UIKit
import Foundation
//var mainTableViewController: MainTableViewController!
/// The view controller for the root view of the app.
class MainTableViewController: UITableViewController {
    
    // Outlets to UI components
    @IBOutlet weak var switchSaveOutput: UISwitch!
    @IBOutlet weak var switchAutomaticGain: UISwitch!
    @IBOutlet weak var segmentedControlLMH: UISegmentedControl!
    @IBOutlet weak var labelLMHDesc: UILabel!
   // @IBOutlet weak var labelEqualizerDetail: UILabel!
    @IBOutlet weak var switchOutputType: UISwitch!
    @IBOutlet weak var switchCompression: UISwitch!
    @IBOutlet weak var labelAmplification: UILabel!
    @IBOutlet weak var sliderAmplification: UISlider!
    @IBOutlet weak var labelStartStop: UILabel!
    @IBOutlet weak var noise_label: UILabel!
    @IBOutlet weak var speech_label: UILabel!
    @IBOutlet weak var quiet_label: UILabel!
    
    var refreshVad: Timer!
    // local vars
    /// The green color for the start button text.
    var ratingText: String!
    let startColor: UIColor = UIColor(red: 56.0/256, green: 214.0/256, blue: 116.0/256, alpha: 1.0) // Same as green on switch
    var path: URL = MainTableViewController.documentsDirectory.appendingPathComponent("Ratings.txt")
    override func viewDidLoad() {
        super.viewDidLoad()
        
        let bounds = self.navigationController!.navigationBar.bounds
        let myView: UIView = UIView()
        let title: UILabel = UILabel(frame: CGRect(x:-135, y:-10, width:300, height:30))

        title.text = "Signal and Image Processing Lab"

        let image: UIImage = UIImage(named: "utd_logo.png")!
        let myImageView: UIImageView = UIImageView(image: image)
        myImageView.frame = CGRect(x:-170, y:-10, width:30, height:30)
       // myImageView.align
        let height: CGFloat = 50 //whatever height you want
        
        self.navigationController!.navigationBar.frame = CGRect(x: 0, y: 0, width: bounds.width, height: height)
        myView.addSubview(title)
        myView.addSubview(myImageView)
        
        self.navigationItem.titleView = myView

      //  self.navigationItem.titleView.pos
        
        ratingText = ""
        noise_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0 )
        speech_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0 )
        quiet_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0 )

        tableView.beginUpdates()
        tableView.endUpdates()
        
        // Output Type switch
        switchOutputType.isOn = audioController.noiseReductionOutputType
        switchOutputType.isEnabled = audioController.playAudio
        switchCompression.isOn = audioController.compressionOutputType;
        switchCompression.isEnabled = audioController.playAudio
        
        // Amplification slider
        sliderAmplification.value = ampMap(audioController.amplification, sliderToAmp: false)
        sliderAmplification.isEnabled = false
        labelAmplification.text = "Amplification: \(String(format: "%0.2f", audioController.amplification))x"
        
        // Set start/stop colors
        startStop(start: audioController.playAudio, setMic: true) // Show stopped or started
        
//                    DispatchQueue.main.async (execute: {
//                        self.changeLabel(vadOutput: audioController.classLabel)
//                    })
        
//                    DispatchQueue.global(qos: .userInitiated).async {
//                        //let image = self.loadOrGenerateAnImage()
//                        // Bounce back to the main thread to update the UI
//                        DispatchQueue.main.async {
//                            self.changeLabel(vadOutput: audioController.classLabel)
//                        }
//                    }
        
        refreshVad = Timer.scheduledTimer(timeInterval: TimeInterval(audioController.dbUpdateInterval),
                                       target: self,
                                       selector: #selector(self.refreshVADOut),
                                       userInfo: nil,
                                       repeats: true)
        let longPressToExitKeyboard: UILongPressGestureRecognizer =
            UILongPressGestureRecognizer(target: self,
                                   action: #selector(self.dismissKeyboard))
        view.addGestureRecognizer(longPressToExitKeyboard)
    }
    
    func dismissKeyboard() {
        view.endEditing(true) // End editing on tap
    }
    
    func refreshVADOut() {
        self.changeLabel(vadOutput: audioController.classLabel)
    }
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
    }
    
    
    deinit {
        // Remove the notification
        NotificationCenter.default.removeObserver(self)
    }
    
    static var documentsDirectory: URL {
        get {
            let paths = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)
            let documentsDirectory = paths[0]
            return documentsDirectory
        }
    }
    
    
    func showErrorAlert(_ message: String) {
        let alert: UIAlertController = UIAlertController(title: "Save File Error",
                                                         message: message,
                                                         preferredStyle: .alert)
        let ok = UIAlertAction(title: "OK", style: .default, handler: {
            UIAlertAction in
        })
        alert.addAction(ok)
        self.present(alert, animated: true, completion: nil)
    }
    
    @IBAction func saveOutputinFile(_ sender: UISwitch) {
        audioController.update(doSaveFile: sender.isOn)
    }
    
    // Audio Output switch
    @IBAction func switchOutputTypeValueChanged(_ sender: UISwitch) {
        audioController.update(noiseReductionOutputType: sender.isOn)
        if sender.isOn {
            labelAmplification.text = "Amplification: \(String(format: "%0.2f", audioController.amplification))x"
            sliderAmplification.value = ampMap(audioController.amplification, sliderToAmp: false)
        }
    }
    
    @IBAction func compressionOutputTypeValueChanded(_ sender: UISwitch) {
        audioController.update(compressionOutputType: sender.isOn)
        if sender.isOn {
            labelAmplification.text = "Amplification: \(String(format: "%0.2f", audioController.amplification))x"
            sliderAmplification.value = ampMap(audioController.amplification, sliderToAmp: false)
        }
    }
    // Amplification slider
    @IBAction func sliderAmplificationValueChanged(_ sender: UISlider) {
        audioController.update(amplification: ampMap(sender.value, sliderToAmp: true))
        labelAmplification.text = "Amplification: \(String(format: "%0.2f", audioController.amplification))x"
    }
    
    /// Starts or stops the audio.
    ///
    /// - Parameters:
    ///   - start: Starts the audio if true, or stops the audio if false.
    ///   - setMic: Enables the microphone input on start if true.
    func startStop(start: Bool, setMic: Bool) {
        // setMic true for playing back input, false for playing audio from file
        if start {
            labelStartStop.text = "Stop Audio"
            labelStartStop.textColor = UIColor.red
            audioController.update(micStatus: setMic)
            audioController.start()
            // Set button enables
            switchOutputType.isEnabled = true
            switchCompression.isEnabled = true
            switchSaveOutput.isEnabled = false
            sliderAmplification.isEnabled = true
            noise_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
            speech_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
            quiet_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0 )
        }
        else {
            labelStartStop.text = "Start Audio"
            labelStartStop.textColor = startColor
            audioController.stop()
            audioController.update(micStatus: false)
            // Set button enables
            switchOutputType.isEnabled = false
            switchCompression.isEnabled = false
            switchSaveOutput.isEnabled = true
            sliderAmplification.isEnabled = false
            audioController.update(classLabel: -1)
        }
    }
    
    func changeLabel(vadOutput: Int)
    {
        switch (vadOutput) {
            case 0:
                quiet_label.backgroundColor = UIColor( red:0, green:0.1, blue:0.6, alpha:0.25)
                noise_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
                speech_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
                break;
            case 1:
                quiet_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
                noise_label.backgroundColor = UIColor( red:0.6, green:0, blue:0.3, alpha:0.25)
                speech_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
                break;
            case 2:
                quiet_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
                noise_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
                speech_label.backgroundColor = UIColor( red:0, green:0.6, blue:0.3, alpha:0.25)
            break;
            
            default:
                quiet_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
                noise_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
                speech_label.backgroundColor = UIColor( red:0, green:0, blue:0, alpha:0)
            break;
        }
    }
    
    /// Shows an error alert if an error occurs when importing files.
    ///
    /// - Parameter message: The error message to show in the alert.
    func showImportErrorAlert(_ message: String) {
        let alert: UIAlertController = UIAlertController(title: "Import File Error",
                                                         message: message,
                                                         preferredStyle: .alert)
        let ok = UIAlertAction(title: "OK", style: .default, handler: {
            UIAlertAction in
        })
        alert.addAction(ok)
        self.present(alert, animated: true, completion: nil)
    }
        
    /// A function to map slider value to amplification value.
    ///
    /// - Parameters:
    ///   - value: The value to map.
    ///   - sliderToAmp: Map a slider position to an amplification if true, or map an amplification value to a slider position if false.
    /// - Returns: The mapped value.
    func ampMap(_ value: Float, sliderToAmp: Bool) -> Float {
        if sliderToAmp {
            return value * value
        }
        else {
            return sqrtf(value)
        }
    }
    
    // Hide and show rows
    override func tableView(_ tableView: UITableView, estimatedHeightForRowAt indexPath: IndexPath) -> CGFloat {
        return UITableViewAutomaticDimension
    }
    
    // Selecting row which functions as a button
    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if indexPath.section == 3 && indexPath.row == 3 {
            startStop(start: !audioController.playAudio, setMic: true) // If it's playing, stop it; if not, start it
            tableView.deselectRow(at: indexPath, animated: true)
        }
    }
    
}
