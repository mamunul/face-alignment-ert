//
//  ERT.hpp
//  ERT
//
//  Created by Mamunul on 11/1/17.
//  Copyright © 2017 Mamunul. All rights reserved.
//

#ifndef ERT_hpp
#define ERT_hpp

#import <opencv2/opencv.hpp>
#include <stdio.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <dlib/opencv/cv_image.h>
#include <iostream>
#include <vector>


struct Params{
	
	double bagging_overlap;
	int max_numtrees;
	int max_depth;
	int landmark_num;// to be decided
	int initial_num;
	
	int max_numstage;
	double max_radio_radius[10];
	int max_numfeats[10]; // number of pixel pairs
	int max_numthreshs;
};
extern Params global_params;
extern std::string modelPath;
extern std::string dataPath;
extern std::string folderPath;

class BoundingBox{
public:
	double start_x;
	double start_y;
	double width;
	double height;
	double centroid_x;
	double centroid_y;
	BoundingBox(){
		start_x = 0;
		start_y = 0;
		width = 0;
		height = 0;
		centroid_x = 0;
		centroid_y = 0;
	};
};

void TrainModel(std::vector<std::string> trainDataName,std::vector<std::string> testDataName,std::string _folderPath);

#endif /* ERT_hpp */
