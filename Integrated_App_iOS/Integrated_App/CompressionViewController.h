//
//  CompressionViewController.h
//  NoiseReduction
//
//  Created by Tahsin Ahmed Chowdhury on 8/20/17.
//  Copyright © 2017 SIPLab. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CompressionViewController : UITableViewController

//band1
@property (weak, nonatomic) IBOutlet UISlider *b1ThSlider;
@property (weak, nonatomic) IBOutlet UILabel *b1ThLabel;

@property (weak, nonatomic) IBOutlet UISlider *b1CrSlider;
@property (weak, nonatomic) IBOutlet UILabel *b1CrLabel;

@property (weak, nonatomic) IBOutlet UISlider *b1AtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b1AtLabel;

@property (weak, nonatomic) IBOutlet UISlider *b1RtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b1RtLabel;

//band2
@property (weak, nonatomic) IBOutlet UISlider *b2ThSlider;
@property (weak, nonatomic) IBOutlet UILabel *b2ThLabel;

@property (weak, nonatomic) IBOutlet UISlider *b2CrSlider;
@property (weak, nonatomic) IBOutlet UILabel *b2CrLabel;

@property (weak, nonatomic) IBOutlet UISlider *b2AtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b2AtLabel;

@property (weak, nonatomic) IBOutlet UISlider *b2RtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b2RtLabel;

//band3
@property (weak, nonatomic) IBOutlet UISlider *b3ThSlider;
@property (weak, nonatomic) IBOutlet UILabel *b3ThLabel;

@property (weak, nonatomic) IBOutlet UISlider *b3CrSlider;
@property (weak, nonatomic) IBOutlet UILabel *b3CrLabel;

@property (weak, nonatomic) IBOutlet UISlider *b3AtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b3AtLabel;

@property (weak, nonatomic) IBOutlet UISlider *b3RtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b3RtLabel;

//band4
@property (weak, nonatomic) IBOutlet UISlider *b4ThSlider;
@property (weak, nonatomic) IBOutlet UILabel *b4ThLabel;

@property (weak, nonatomic) IBOutlet UISlider *b4CrSlider;
@property (weak, nonatomic) IBOutlet UILabel *b4CrLabel;

@property (weak, nonatomic) IBOutlet UISlider *b4AtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b4AtLabel;

@property (weak, nonatomic) IBOutlet UISlider *b4RtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b4RtLabel;

//band5
@property (weak, nonatomic) IBOutlet UISlider *b5ThSlider;
@property (weak, nonatomic) IBOutlet UILabel *b5ThLabel;

@property (weak, nonatomic) IBOutlet UISlider *b5CrSlider;
@property (weak, nonatomic) IBOutlet UILabel *b5CrLabel;

@property (weak, nonatomic) IBOutlet UISlider *b5AtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b5AtLabel;

@property (weak, nonatomic) IBOutlet UISlider *b5RtSlider;
@property (weak, nonatomic) IBOutlet UILabel *b5RtLabel;


@end
