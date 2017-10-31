//
//  ViewController.m
//  ERT
//
//  Created by Mamunul on 10/30/17.
//  Copyright © 2017 Mamunul. All rights reserved.
//
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <iostream>


#import "ViewController.h"

using namespace dlib;
using namespace std;

@implementation ViewController

- (void)viewDidLoad {
	[super viewDidLoad];

	// Do any additional setup after loading the view.
	
	dlib::array<array2d<unsigned char> > images_train, images_test;
	std::vector<std::vector<full_object_detection> > faces_train, faces_test;
	
	load_image_dataset(images_train, faces_train, faces_directory+"/training_with_face_landmarks.xml");
	load_image_dataset(images_test, faces_test, faces_directory+"/testing_with_face_landmarks.xml");
	
	  shape_predictor_trainer trainer;
	
	 trainer.set_oversampling_amount(300);
	
	trainer.set_nu(0.05);
	trainer.set_tree_depth(2);
	
	
	// Tell the trainer to print status messages to the console so we can
	// see how long the training will take.
	trainer.be_verbose();
	
	// Now finally generate the shape model
	shape_predictor sp = trainer.train(images_train, faces_train);
	
	cout << "mean training error: "<<
	test_shape_predictor(sp, images_train, faces_train, get_interocular_distances(faces_train)) << endl;
	
	cout << "mean testing error:  "<<
	test_shape_predictor(sp, images_test, faces_test, get_interocular_distances(faces_test)) << endl;
	
	serialize("sp.dat") << sp;
}


- (void)setRepresentedObject:(id)representedObject {
	[super setRepresentedObject:representedObject];

	// Update the view, if already loaded.
}


@end
