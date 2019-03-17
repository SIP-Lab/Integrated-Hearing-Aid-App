//
//  NoiseReductionSettingsTableViewController.swift
//  SPP_Integrated_App
//
//  Created by Akshay Chitale on 6/29/17.
//  Modified by Tahsin Ahmed Chowdhury on 10/15/17
//  Copyright © 2017 UT Dallas. All rights reserved.
//

import UIKit
import Foundation

/// The view controller for setting the advanced settings.
class NoiseReductionSettingsTableViewController: UITableViewController {
    
    // Outlets to UI Components
    @IBOutlet weak var labelSamplingFrequency: UILabel!
    @IBOutlet weak var labelWindowSize: UILabel!
    @IBOutlet weak var labelStepSize: UILabel!
    @IBOutlet weak var textFieldCalibration: UITextField!
    @IBOutlet weak var labelSBL: UILabel!
    @IBOutlet weak var labelCurrentGains: UILabel!
    @IBOutlet weak var textFieldUpdateInterval: UITextField!
    
    @IBOutlet weak var textFieldNoiseEstimateTime: UITextField!
    // local vars
    var refresh: Timer!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Only allowed to set if audio not playing
        let notPlaying = !audioController.playAudio

        textFieldUpdateInterval.isEnabled = notPlaying
        textFieldNoiseEstimateTime.isEnabled = notPlaying
        // Sampling Frequency
        let samplingFreq: Int = audioController.fs
        labelSamplingFrequency.text = "\(samplingFreq) Hz"
        
        // Frame (Window) size
        let frame: Float = 2.0 * Float(audioController.stepSize)/Float(audioController.fs)
        labelWindowSize.text = String(format: "%.2f", 1000.0 * frame)
        
        // Step size
        let step: Float = Float(audioController.stepSize)/Float(audioController.fs)
        labelStepSize.text = String(format: "%.2f", 1000.0 * step)
        
        // Calibration and SPL
        textFieldCalibration.text = String(format: "%.2f", audioController.calibration)
        refreshSPL()
        // Timer to refresh SPL
        refresh = Timer.scheduledTimer(timeInterval: TimeInterval(audioController.dbUpdateInterval),
                                       target: self,
                                       selector: #selector(self.refreshSPL),
                                       userInfo: nil,
                                       repeats: true)
        
        // Show update interval
        textFieldUpdateInterval.text = String(format: "%.2f", audioController.dbUpdateInterval)
        textFieldNoiseEstimateTime.text = String(format: "%.2f", audioController.noiseEstimateTime)
        
        // Recognize taps to exit keyboard
        let tapToExitKeyboard: UITapGestureRecognizer =
            UITapGestureRecognizer(target: self,
                                   action: #selector(self.dismissKeyboard))
        view.addGestureRecognizer(tapToExitKeyboard)
    }
    
    deinit {
        // Remove the notification
        NotificationCenter.default.removeObserver(self)
    }
    
    /* deprecated
    @IBAction func textFieldWindowSizeEditingDidEnd(_ sender: UITextField) {
        let frame: Float = Float(audioController.frameSize)/Float(audioController.fs)
        if let text: String = sender.text {
            if let newWindowSize: Float = Float(text) {
                if newWindowSize <= 0 || newWindowSize != round(newWindowSize) {
                    sender.text = String(format: "%.2f", 1000.0 * frame)
                    showErrorAlert(intRequired: true)
                }
                audioController.update(frameSize: Float(newWindowSize))
                sender.text = String(format: "%.2f", newWindowSize)
            }
            else {
                sender.text = String(format: "%.2f", 1000.0 * frame)
                showErrorAlert(intRequired: true)
            }
        }
        else {
            sender.text = String(format: "%.2f", 1000.0 * frame)
            showErrorAlert(intRequired: true)
        }
    }
     */
    @IBAction func textFieldNoiseEstimateTimeEditDidChanged(_ sender: UITextField) {
        let noiseEstimateTime = audioController.noiseEstimateTime
        if let text: String = sender.text {
            if let newEstimateTime: Float = Float(text) {
                if newEstimateTime < 0 {
                    sender.text = String(format: "%.2f", noiseEstimateTime)
                    showErrorAlert()
                }
                else {
                    audioController.update(noiseEstimateTime: newEstimateTime)
                    sender.text = String(format: "%.2f", newEstimateTime)
                }
            }
            else {
                sender.text = String(format: "%.2f", noiseEstimateTime)
                showErrorAlert()
            }
        }
        else {
            sender.text = String(format: "%.2f", noiseEstimateTime)
            showErrorAlert()
        }
    }
    
    @IBAction func textFieldCalibrationEditingDidEnd(_ sender: UITextField) {
        let calibration: Float = audioController.calibration
        if let text: String = sender.text {
            if let newCalibration: Float = Float(text) {
                audioController.update(calibration: newCalibration)
                sender.text = String(format: "%.2f", newCalibration)
            }
            else {
                sender.text = String(format: "%.2f", calibration)
                showErrorAlert(posRequired: false)
            }
        }
        else {
            sender.text = String(format: "%.2f", calibration)
            showErrorAlert(posRequired: false)
        }
    }
    
    @IBAction func textFieldUpdateIntervalEditingDidEnd(_ sender: UITextField) {
        let dbUpdateInterval = audioController.dbUpdateInterval
        if let text: String = sender.text {
            if let newUpdateInterval: Float = Float(text) {
                if newUpdateInterval < 0 {
                    sender.text = String(format: "%.2f", dbUpdateInterval)
                    showErrorAlert()
                }
                else {
                    audioController.update(dbUpdateInterval: newUpdateInterval)
                    sender.text = String(format: "%.2f", newUpdateInterval)
                }
            }
            else {
                sender.text = String(format: "%.2f", dbUpdateInterval)
                showErrorAlert()
            }
        }
        else {
            sender.text = String(format: "%.2f", dbUpdateInterval)
            showErrorAlert()
        }
    }
    
    /// Dismisses the keyboard when the screen is tapped after editing a field.
    func dismissKeyboard() {
        view.endEditing(true) // End editing on tap
    }
    
    /// Refreshes the values of the audio level and current gains.
    func refreshSPL() {
        labelSBL.text = "\(String(format:"%.2f", audioController.dbpower)) dB SPL"
        labelCurrentGains.text = "\(String(format:"%.2f", audioController.processTime)) ms"
    }
    
    /// Shows an error alert if an error occurs when entering a number.
    ///
    /// - Parameter intRequired: Whether or not an integer is required in the field.
    func showErrorAlert(intRequired: Bool = false, posRequired:Bool = true) {
        let posstr: String = posRequired ? "positive" : "valid"
        let message: String = intRequired ? "Please enter only \(posstr) integer numbers." : "Please enter only \(posstr) numbers."
        let alert: UIAlertController = UIAlertController(title: "Improper Value Entered",
                                                         message: message,
                                                         preferredStyle: .alert)
        let ok = UIAlertAction(title: "OK", style: .default, handler: {
            UIAlertAction in
        })
        alert.addAction(ok)
        self.present(alert, animated: true, completion: nil)
    }

    
}
