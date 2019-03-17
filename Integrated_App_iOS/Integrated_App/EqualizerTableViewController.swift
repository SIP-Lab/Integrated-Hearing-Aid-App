//
//  EqualizerTableViewController.swift
//  SPP_Integrated_App
//
//  Created by Akshay Chitale on 7/5/17.
//  Copyright © 2017 UT Dallas. All rights reserved.
//

import UIKit
import Foundation

/// The view controller for the equalizer menu.
class EqualizerTableViewController: UITableViewController {
    
    // Outlets to UI Components
    @IBOutlet weak var tableViewCellDefault: UITableViewCell!
    @IBOutlet weak var labelCurrentGainsFile: UILabel!
    
    // local vars
    
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        
        // Notify when app enters foreground for updates
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(appActiveAction),
                                               name: NSNotification.Name.UIApplicationDidBecomeActive,
                                               object: UIApplication.shared)
        
        // Update labels and other UI
        labelCurrentGainsFile.text = audioController.gainsFile
        if audioController.gainsFile == "Default" {
            // checkmark by Default
            tableViewCellDefault.accessoryType = UITableViewCellAccessoryType.checkmark
        }
        else {
            // no checkmark
            tableViewCellDefault.accessoryType = UITableViewCellAccessoryType.none
        }
    }
    
    deinit {
        // Remove the notification
        NotificationCenter.default.removeObserver(self)
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
    
    /// Updates the gains file label when app becomes active.
    ///
    /// Selected to be called so that the imported file is shown.
    func appActiveAction(_ note: NSNotification) {
        if(self.isViewLoaded && self.view.window != nil) {
            // Show error from import if there is one pending
            if let importError = importedFileErrorString {
                if(importError != "") {
                    showImportErrorAlert(importError)
                    // Clear error string
                    importedFileErrorString = nil
                }
            }
            labelCurrentGainsFile.text = audioController.gainsFile
        }
    }
    
    // Selecting row which functions as a button
    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        // Reset to default buttons
        if indexPath.section == 0 && indexPath.row == 0 {
            // Set to default
            audioController.setDefault(all: true)
            audioController.gainsFile = "Default"
            labelCurrentGainsFile.text = audioController.gainsFile
            tableViewCellDefault.accessoryType = UITableViewCellAccessoryType.checkmark
            tableView.deselectRow(at: indexPath, animated: true)
        }
    }
}
