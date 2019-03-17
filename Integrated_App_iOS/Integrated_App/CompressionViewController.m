//
//  CompressionViewController.m
//  NoiseReduction
//
//  Created by Tahsin Ahmed Chowdhury on 8/20/17.
//  Copyright © 2017 SIPLab. All rights reserved.
//

#import "CompressionViewController.h"

#import "VAD_NR_Com.h"
@interface CompressionViewController ()

@end

@implementation CompressionViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self update_slider];
    
    
    [self update_label];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void) update_slider{
    
    _b1ThSlider.value = dataIn[1];
    _b1CrSlider.value = dataIn[0];
    _b1AtSlider.value = dataIn[2];
    _b1RtSlider.value = dataIn[3];
    
    _b2ThSlider.value = dataIn[5];
    _b2CrSlider.value = dataIn[4];
    _b2AtSlider.value = dataIn[6];
    _b2RtSlider.value = dataIn[7];
    
    _b3ThSlider.value = dataIn[9];
    _b3CrSlider.value = dataIn[8];
    _b3AtSlider.value = dataIn[10];
    _b3RtSlider.value = dataIn[11];
    
    _b4ThSlider.value = dataIn[13];
    _b4CrSlider.value = dataIn[12];
    _b4AtSlider.value = dataIn[14];
    _b4RtSlider.value = dataIn[15];
    
    _b5ThSlider.value = dataIn[17];
    _b5CrSlider.value = dataIn[16];
    _b5AtSlider.value = dataIn[18];
    _b5RtSlider.value = dataIn[19];
}

-(void) update_label{
    _b1ThLabel.text = [NSString stringWithFormat:@"%.0f", [_b1ThSlider value]];
    
    _b1CrLabel.text = [NSString stringWithFormat:@"%.0f", [_b1CrSlider value]];
    
    _b1AtLabel.text = [NSString stringWithFormat:@"%.0f", [_b1AtSlider value]];
    
    _b1RtLabel.text = [NSString stringWithFormat:@"%.0f", [_b1RtSlider value]];
    
    _b2ThLabel.text = [NSString stringWithFormat:@"%.0f", [_b2ThSlider value]];
    
    _b2CrLabel.text = [NSString stringWithFormat:@"%.0f", [_b2CrSlider value]];
    
    _b2AtLabel.text = [NSString stringWithFormat:@"%.0f", [_b2AtSlider value]];
    
    _b2RtLabel.text = [NSString stringWithFormat:@"%.0f", [_b2RtSlider value]];
    
    _b3ThLabel.text = [NSString stringWithFormat:@"%.0f", [_b3ThSlider value]];
    
    _b3CrLabel.text = [NSString stringWithFormat:@"%.0f", [_b3CrSlider value]];
    
    _b3AtLabel.text = [NSString stringWithFormat:@"%.0f", [_b3AtSlider value]];
    
    _b3RtLabel.text = [NSString stringWithFormat:@"%.0f", [_b3RtSlider value]];
    
    
    _b4ThLabel.text = [NSString stringWithFormat:@"%.0f", [_b4ThSlider value]];
    
    _b4CrLabel.text = [NSString stringWithFormat:@"%.0f", [_b4CrSlider value]];
    
    _b4AtLabel.text = [NSString stringWithFormat:@"%.0f", [_b4AtSlider value]];
    
    _b4RtLabel.text = [NSString stringWithFormat:@"%.0f", [_b4RtSlider value]];
    
    _b5ThLabel.text = [NSString stringWithFormat:@"%.0f", [_b5ThSlider value]];
    
    _b5CrLabel.text = [NSString stringWithFormat:@"%.0f", [_b5CrSlider value]];
    
    _b5AtLabel.text = [NSString stringWithFormat:@"%.0f", [_b5AtSlider value]];
    
    _b5RtLabel.text = [NSString stringWithFormat:@"%.0f", [_b5RtSlider value]];
    
    
}

- (IBAction)b1Th_Action:(UISlider*)sender {
    _b1ThLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[1] = [sender value];
    dRC1_not_empty = false;
}

- (IBAction)b1Cr_Action:(UISlider*)sender {
    _b1CrLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[0] = [sender value];
    dRC1_not_empty = false;
}

- (IBAction)b1At_Action:(UISlider*)sender {
    _b1AtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[2] = [sender value];
    dRC1_not_empty = false;
}

- (IBAction)b1Rt_Action:(UISlider*)sender {
    _b1RtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[3] = [sender value];
    dRC1_not_empty = false;
}

-(IBAction)b2Th_Action:(UISlider*)sender {
    _b2ThLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[5] = [sender value];
    dRC2_not_empty = false;
}

- (IBAction)b2Cr_Action:(UISlider*)sender {
    _b2CrLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[4] = [sender value];
    dRC2_not_empty = false;
}

- (IBAction)b2At_Action:(UISlider*)sender {
    _b2AtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[6] = [sender value];
    dRC2_not_empty = false;
}

- (IBAction)b2Rt_Action:(UISlider*)sender {
    _b2RtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[7] = [sender value];
    dRC2_not_empty = false;
}

-(IBAction)b3Th_Action:(UISlider*)sender {
    _b3ThLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[9] = [sender value];
    dRC3_not_empty = false;
}

- (IBAction)b3Cr_Action:(UISlider*)sender {
    _b3CrLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[8] = [sender value];
    dRC3_not_empty = false;
}

- (IBAction)b3At_Action:(UISlider*)sender {
    _b3AtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[10] = [sender value];
    dRC3_not_empty = false;
}

- (IBAction)b3Rt_Action:(UISlider*)sender {
    _b3RtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[11] = [sender value];
    dRC3_not_empty = false;
}

-(IBAction)b4Th_Action:(UISlider*)sender {
    _b4ThLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[13] = [sender value];
    dRC4_not_empty = false;
}

- (IBAction)b4Cr_Action:(UISlider*)sender {
    _b4CrLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[12] = [sender value];
    dRC4_not_empty = false;
}

- (IBAction)b4At_Action:(UISlider*)sender {
    _b4AtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[14] = [sender value];
    dRC4_not_empty = false;
}

- (IBAction)b4Rt_Action:(UISlider*)sender {
    _b4RtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[15] = [sender value];
    dRC4_not_empty = false;
}

-(IBAction)b5Th_Action:(UISlider*)sender {
    _b5ThLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[17] = [sender value];
    dRC5_not_empty = false;
}

- (IBAction)b5Cr_Action:(UISlider*)sender {
    _b5CrLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[16] = [sender value];
    dRC5_not_empty = false;
}

- (IBAction)b5At_Action:(UISlider*)sender {
    _b5AtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[18] = [sender value];
    dRC5_not_empty = false;
}

- (IBAction)b5Rt_Action:(UISlider*)sender {
    _b5RtLabel.text = [NSString stringWithFormat:@"%.0f", [sender value]];
    dataIn[19] = [sender value];
    dRC5_not_empty = false;
}


/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
