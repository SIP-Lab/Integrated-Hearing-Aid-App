//
//  AudioSettingsController.swift
//  SPP_Integrated_App
//
//  Created by Akshay Chitale on 6/25/17.
//  Copyright © 2017 UT Dallas. All rights reserved.
//

import Foundation

/// Globally availible audio settings, set in `AppDelegate.swift` on app launch and for use in all view controllers.
var audioController: AudioSettingsController!

/// A controller for starting and stopping the audio, as well as changing the audio settings.
///
/// This class serves as an interface for Swift code to interact with the underlying Objective-C and C audio code.
class AudioSettingsController {
    // Later have private var for IosAudioController, for now is global
    
    /// Keeps track of "Low", "Medium", and "High" gains. This variable can be used to store other gains arrays as well.
    ///
    /// The element at `self.currentGains` is the current gains.
//    private var gainsTable: Dictionary<String, [Float]>
//    
//    /// The current array of gains.
//    public private(set) var currentGains: String { // Private setter, public getter
//        get {
//            if(self.autoGains) {
//                // Must be "Low", "Medium". or "High"
//                switch(iosAudio.settings.pointee.currentGains) {
//                case 0:
//                    return "Low"
//                case 1:
//                    return "Medium"
//                case 2:
//                    return "High"
//                default:
//                    return self._currentGains
//                }
//            }
//            else {
//                return self._currentGains
//            }
//        }
//        set {
//            self._currentGains = newValue
//        }
//    }
    
    
//    private var _currentGains: String;
//
//    /// The default array of gains, where every gain is 1.0.
//    let defaultGains: [Float] = [1, 1, 1, 1, 1]
    

    /// The definition of the "Low" audio level, in dB SPL.
//    var lowDef: Int {
//        return Int(iosAudio.settings.pointee.lowDef);
//    }
//
//    /// The definition of the "Medium" audio level, in dB SPL.
//    var mediumDef: Int {
//        return Int(iosAudio.settings.pointee.mediumDef);
//    }
//
//    /// The definition of the "High" audio level, in dB SPL.
//    var highDef: Int {
//        return Int(iosAudio.settings.pointee.highDef);
//    }
    
    /// The name of the file from which the current gains were read.
    ///
    /// This variable is for public use. This class does not read or mutate this variable.
    //var gainsFile: String = "Default"
    
    
    /// Initializes the `AudioSettingsController` with "Low", "Medium", and "High" gains and default settings.
    init() {
        iosAudio = IosAudioController()
        // Is global for now, make local later
//        gainsTable = Dictionary<String, [Float]>()
//        gainsTable["Low"] = defaultGains
//        gainsTable["Medium"] = defaultGains
//        gainsTable["High"] = defaultGains
//        self._currentGains = "Medium"
//        setGains()
    }
    
    /// Initializes the `AudioSettingsController` with an array of current gains.
    ///
    /// This init first calls the default constructor, which initializes the "Low", "Medium", and "High" gains to `self.defaultGains`.
    ///
    /// - Parameters:
    ///   - gains: The array of gains to set, of length `self.numGains`.
    ///   - currentGains: The name of the array of gains to set.
//    convenience init(gains: [Float], currentGains: String) {
//        self.init()
////        let set: Bool = update(gains: gains, currentGains: currentGains, setOnly: false)
////        if !set {
////            print("Default gains set")
////        }
//    }
    
    deinit {
        // Deallocate memory
        iosAudio.destructSettings()
    }
    
    
    /// Sets the gains in `iosAudio` to the current gains.
//    private func setGains() {
//        // Is a mutable pointer, but we know that Obj-C and C code only get gains
//        iosAudio.settings.pointee.lowGains      = UnsafeMutablePointer<Float>(mutating: gainsTable["Low"]!)
//        iosAudio.settings.pointee.mediumGains   = UnsafeMutablePointer<Float>(mutating: gainsTable["Medium"]!)
//        iosAudio.settings.pointee.highGains     = UnsafeMutablePointer<Float>(mutating: gainsTable["High"]!)
//        iosAudio.settings.pointee.gains         = UnsafeMutablePointer<Float>(mutating: gainsTable[currentGains]!)
//    }
    
    
    /// Starts the audio.
    ///
    /// This function is safe to use whether or not the audio is currently stopped.
    func start() {
        if !playAudio {
            iosAudio.start()
            iosAudio.settings.pointee.playAudio = 1
        }
    }
    
    /// Stops the audio.
    ///
    /// This function is safe to use whether or not the audio is currently playing.
    func stop() {
        if playAudio {
            iosAudio.stop()
            iosAudio.settings.pointee.playAudio = 0
        }
    }
    
    
    /// The number of gains an array of gains must have.
//    var numGains: Int {
//        return Int(iosAudio.settings.pointee.numGains)
//    }
    
    /// The current sampling frequency.
    var fs: Int {
        return Int(iosAudio.settings.pointee.fs)
    }
    
    /// The current frame size, a.k.a. the window size.
    var frameSize: Int {
        return Int(iosAudio.settings.pointee.frameSize)
    }
    
    /// The current step size, a.k.a. the overlap size.
    var stepSize: Int {
        return Int(iosAudio.settings.pointee.stepSize)
    }
    
    /// The current microphone status. Is using audio input from the microphone if true, or from a file if false.
    var micStatus: Bool {
        return iosAudio.settings.pointee.micStatus != 0 ? true : false
    }
    
    /// Whether audio is currently playing.
    var playAudio: Bool {
        return iosAudio.settings.pointee.playAudio != 0 ? true : false
    }
    
    var classLabel: Int {
        return Int(iosAudio.settings.pointee.classLabel)
    }
    
    /// The current output type. Is outputting processed audio if true, or unprocessed audio if false.
    var noiseReductionOutputType: Bool {
        return iosAudio.settings.pointee.noiseReductionOutputType != 0 ? true : false
    }
    
    var compressionOutputType: Bool {
        return iosAudio.settings.pointee.compressionOutputType != 0 ? true : false
    }
    
    /// The current fileName of the file for audio input
//    var fileName: String {
//        return String(describing: iosAudio.settings.pointee.fileName)
//    }
    
    /// The current array of gains in use, of length `self.numGains`.
//    var gains: [Float] {
//        // Gains from here are set only, because AudioSettingsController stores more info than struct Settings
//        // Update functions and constructors guarantee gains exist
//        return gainsTable[currentGains]!
//    }
    
    /// The current "Low", "Medium", and "High" gains arrays.
    /// - "Low" gains at index 0.
    /// - "Medium" gains at index 1.
    /// - "High" gains at index 2.
//    var LMHGains: [[Float]] {
//        // LMH known to exist by constructor, return list of gains lists
//        return [gainsTable["Low"]!, gainsTable["Medium"]!, gainsTable["High"]!]
//    }
    
    /// The current amplification of the processed audio output.
    var amplification: Float {
        return Float(iosAudio.settings.pointee.amplification)
    }
    
    /// Whether the gains are being automatically selected. If false, gains are manually selected with `self.currentGains`.
//    var autoGains: Bool {
//        return iosAudio.settings.pointee.autoGains != 0 ? true : false
//    }
    
    /// The audio level, in dB SPL.
    var dbpower: Float {
        return iosAudio.settings.pointee.dbpower
    }
    
    var processTime: Float {
        return iosAudio.settings.pointee.processTime
    }
    
    /// The calibration constant for audio level measuring, in dB.
    var calibration: Float {
        return iosAudio.settings.pointee.calibration
    }
    
    /// The interval to wait before updating the audio level, in seconds.
    var dbUpdateInterval: Float {
        return iosAudio.settings.pointee.dbUpdateInterval
    }
    
    var noiseEstimateTime: Float {
        return iosAudio.settings.pointee.noiseEstimateTime
    }
    
    /// The position of the boundary between the low and medium ranges. This variable is only used if SPLBUFFER_MODE_RANGE is specified in `newSPLBuffer()` (`SpeechProcessing.c`).
    ///
    /// This value should be between 0 and 1, with 0 signifying a boundary at `self.lowDef` and 1 signifying a boundary at `self.mediumDef`. The boundary is equal to `alpha*self.lowDef + (1 - alpha)*self.mediumDef`.
//    var alpha: Float {
//        return iosAudio.settings.pointee.alpha
//    }
//
//    /// The position of the boundary between the medium and high ranges. This variable is only used if SPLBUFFER_MODE_RANGE is specified in `newSPLBuffer()` (`SpeechProcessing.c`).
//    ///
//    /// This value should be between 0 and 1, with 0 signifying a boundary at `self.mediumDef` and 1 signifying a boundary at `self.highDef`. The boundary is equal to `beta*self.lowDef + (1 - beta)*self.mediumDef`.
//    var beta: Float {
//        return iosAudio.settings.pointee.beta
//    }
    
    
    /// Updates the sampling frequency.
    ///
    /// - Parameter fs: The new sampling frequency, in Hertz.
    func update(fs: Int) {
        iosAudio.settings.pointee.fs = Int32(fs)
    }
    
    /// Updates the frame size, a.k.a. the window size.
    ///
    /// - Parameter frameSize: The new frame size, in milliseconds.
    func update(frameSize: Float) {
        iosAudio.settings.pointee.frameSize = Int32(frameSize * Float(fs) / 1000.0)
    }
    
    /// Updates the step size, a.k.a. the overlap size.
    ///
    /// - Parameter stepSize: The new step size, in milliseconds.
    func update(stepSize: Float) {
        iosAudio.settings.pointee.stepSize = Int32(stepSize * Float(fs) / 1000.0)
    }
    func update(classLabel: Int) {
        iosAudio.settings.pointee.classLabel = Int32(classLabel)
    }
    /// Updates the microphone status.
    ///
    /// - Parameter micStatus: If true, use microphone audio input. If false, use audio input from file.
    func update(micStatus: Bool) {
        iosAudio.settings.pointee.micStatus = Int32(micStatus ? 1 : 0)
    }
    
    /// Updates the noise reduction output type.
    ///
    /// - Parameter noiseReductionOutputType: If true, play processed audio. If false, play unprocessed audio.
    func update(noiseReductionOutputType: Bool) {
        iosAudio.settings.pointee.noiseReductionOutputType = Int32(noiseReductionOutputType ? 1 : 0)
    }
    
    func update(compressionOutputType: Bool) {
        iosAudio.settings.pointee.compressionOutputType = Int32(compressionOutputType ? 1 : 0)
    }
    
    func update(doSaveFile: Bool) {
        iosAudio.settings.pointee.doSaveFile = Int32(doSaveFile ? 1 : 0)
    }
    
    /// Updates the fileName of the audio to play.
    ///
    /// - Parameter fileName: The name of the audio file, without the `.caf` extension.
//    func update(fileName: String) {
//        // Safe to just point, since inner code never mutates C string
//        iosAudio.settings.pointee.fileName = UnsafePointer<Int8>(fileName)
//    }
    
    /// Updates the current array of gains.
    ///
    /// - Parameter gains: The new array of gains, of length `self.numGains`.
    /// - Returns: Whether the update was successful.
//    func update(gains: [Float]) -> Bool {
//        // Make sure gains array has correct number of gains
//        if gains.count != numGains {
//            return false
//        }
//        gainsTable[currentGains]! = gains
//        setGains()
//        return true
//    }
    
    /// Updates whether the gains are automatically selected
    ///
    /// - Parameter autoGains: If true, automatically select the gains based on audio level. If false, manually select the gains.
//    func update(autoGains: Bool) {
//        iosAudio.settings.pointee.autoGains = Int32(autoGains ? 1 : 0)
//    }
    
    /// Updates the calibration constant for audio level calculation
    ///
    /// - Parameter calibration: The new calibration constant for audio level measuring, in dB.
    func update(calibration: Float) {
        iosAudio.settings.pointee.calibration = calibration
    }
    
    /// Updates the interval to wait before updating the audio level.
    ///
    /// - Parameter dbUpdateInterval: The new interval to wait, in seconds.
    func update(dbUpdateInterval: Float) {
        iosAudio.settings.pointee.dbUpdateInterval = dbUpdateInterval
    }
    
    func update(noiseEstimateTime: Float) {
        iosAudio.settings.pointee.noiseEstimateTime = noiseEstimateTime
        iosAudio.settings.pointee.noiseEstimationFrame = noiseEstimateTime*Float(fs)/Float(stepSize)
    }
    
    
    /// Updates the boundary between the low and medium ranges. This variable is only used if SPLBUFFER_MODE_RANGE is specified in `newSPLBuffer()` (`SpeechProcessing.c`).
    ///
    /// - Parameter alpha: The new weight of `self.lowDef` in the boundary calculation.
    func update(alpha: Float) {
        iosAudio.settings.pointee.alpha = alpha
    }
//
//    /// Updates the boundary between the medium and high ranges. This variable is only used if SPLBUFFER_MODE_RANGE is specified in `newSPLBuffer()` (`SpeechProcessing.c`).
//    ///
//    /// - Parameter beta: The new weight of `self.mediumDef` in the boundary calculation.
    func update(beta: Float) {
        iosAudio.settings.pointee.beta = beta
    }
    
    /// Updates which array of gains to use.
    ///
    /// - Parameter currentGains: The name of the array of gains to be used.
    /// - Returns: Whether the update was successful.
//    func update(currentGains: String) -> Bool {
//        // Make sure requested gains table exists
//        if gainsTable[currentGains] == nil {
//            return false
//        }
//        self.currentGains = currentGains
//        setGains()
//        return true
//    }
    
    /// Updates a specific array of gains.
    ///
    /// - Parameters:
    ///   - gains: The new array of gains, of length `self.numGains`.
    ///   - currentGains: The name of the array of gains to update.
    ///   - setOnly: A flag for whether to set gains without using them as the current gains after the update.
    /// - Returns: Whether the update was successful.
//    func update(gains: [Float], currentGains: String, setOnly: Bool) -> Bool {
//        if gains.count != numGains {
//            return false
//        }
//        if !setOnly {
//            self.currentGains = currentGains // only change value of gains if setOnly, do not change currentGains
//        }
//        gainsTable[currentGains] = gains
//        setGains()
//        return true
//    }
    
    /// Updates the amplification.
    ///
    /// - Parameter amplification: The new amplification value.
    func update(amplification: Float) {
        iosAudio.settings.pointee.amplification = amplification
    }
    
    /// Updates the "Low", "Medium", and "High" gains from a JSON Object
    ///
    /// The JSON object must have an integer attribute "numGains" that is equal to `self.numGains`, as well as a "gains" object attribute that contains "Low", "Medium", and "High" floating point array attributes of length `self.numGains`
    ///
    /// - Parameters:
    ///   - fromJSON: The JSON object from which to get the update information
    ///   - errorString: An inout string in which to store error information should one occur.
    /// - Returns: Whether the update was successful.
//    func update(fromJSON: [String: Any], errorString: inout String) -> Bool {
//        errorString = ""
//        var success: Bool = false;
//        // errorString if action fails followed by action
//        // success only set to true if all if statements succeed
//        errorString = "Required attribute \"numGains\" not provided"
//        if let numGains = fromJSON["numGains"] {
//
//        errorString = "Required attribute \"numGains\" is not an integer"
//        if let numGainsInt = numGains as? Int {
//
//        errorString = "Number of gains is not equal to \(self.numGains)"
//        if numGainsInt == self.numGains {
//
//        errorString = "Required attribute \"gains\" not provided"
//        if let gains = fromJSON["gains"] {
//
//        errorString = "Required attribute \"gains\" is not a dictionary of lists"
//        if let gainsDict = gains as? [String: Any] {
//
//        errorString = "Required gains \"Low\" not provided"
//        if let lowGains = gainsDict["Low"] {
//
//        errorString = "Required gains \"Low\" is not a list"
//        if let lowGainsList = lowGains as? [Any] {
//
//        errorString = "Required gains \"Low\" is not a list of floats"
//        if let lowGainsListFloat = lowGainsList as? [Float] {
//
//        errorString = "Requied gains \"Low\" does not contain \(numGainsInt) gains"
//        if lowGainsListFloat.count == numGainsInt {
//
//        errorString = "Required gains \"Medium\" not provided"
//        if let mediumGains = gainsDict["Medium"] {
//
//        errorString = "Required gains \"Medium\" is not a list"
//        if let mediumGainsList = mediumGains as? [Any] {
//
//        errorString = "Required gains \"Medium\" is not a list of floats"
//        if let mediumGainsListFloat = mediumGainsList as? [Float] {
//
//        errorString = "Requied gains \"Medium\" does not contain \(numGainsInt) gains"
//        if mediumGainsListFloat.count == numGainsInt {
//
//        errorString = "Required gains \"High\" not provided"
//        if let highGains = gainsDict["High"] {
//
//        errorString = "Required gains \"High\" is not a list"
//        if let highGainsList = highGains as? [Any] {
//
//        errorString = "Required gains \"High\" is not a list of floats"
//        if let highGainsListFloat = highGainsList as? [Float] {
//
//        errorString = "Requied gains \"High\" does not contain \(numGainsInt) gains"
//        if highGainsListFloat.count == numGainsInt {
//
//        // Everything valid in JSON file
//        _ = self.update(gains: lowGainsListFloat, currentGains: "Low", setOnly: true)
//        _ = self.update(gains: mediumGainsListFloat, currentGains: "Medium", setOnly: true)
//        _ = self.update(gains: highGainsListFloat, currentGains: "High", setOnly: true)
//        success = true
//
//        }}}}}}}}}}}}}}}}}
//
//        return success
//    }
    
    
    /// Resets gains to `self.defaultGains`.
    ///
    /// - Parameter all: If true, resets "Low", "Medium", and "High" gains to default instead of only resetting the current gains.
//    func setDefault(all: Bool) {
//        if all {
//            // Set all gains to default
//            gainsTable["Low"] = defaultGains
//            gainsTable["Medium"] = defaultGains
//            gainsTable["High"] = defaultGains
//        }
//        else {
//            // Set only current gains to default
//            gainsTable[currentGains] = defaultGains
//        }
//        setGains()
//    }
}
