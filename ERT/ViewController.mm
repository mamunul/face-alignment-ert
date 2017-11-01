//
//  ViewController.m
//  ERT
//
//  Created by Mamunul on 10/30/17.
//  Copyright © 2017 Mamunul. All rights reserved.
//

#import "ERT.hpp"
#import "ViewController.h"


@implementation ViewController

- (void)viewDidLoad {
	[super viewDidLoad];

	// Do any additional setup after loading the view.
	

	[self startTraining];
	

}

-(void)startTraining{

	
	std::vector<std::string> trainDataName;
	extern std::string modelPath;
	extern std::string dataPath;
	extern std::string folderPath;
	
	dataPath = "";
	modelPath = "/Users/mamunul/Downloads/Shape_Prediction_Database/esr/";
	folderPath = "/Users/mamunul/Downloads/Shape_Prediction_Database/";
	
	
	trainDataName.push_back("helen/trainset");
	trainDataName.push_back("lfpw/trainset");
	trainDataName.push_back("afw");


	TrainModel(trainDataName, folderPath);



}

- (void)setRepresentedObject:(id)representedObject {
	[super setRepresentedObject:representedObject];

	// Update the view, if already loaded.
}





@end
