//
//  ERTWrapper.h
//  ERT
//
//  Created by Mamunul on 10/30/17.
//  Copyright © 2017 Mamunul. All rights reserved.
//

#import <opencv2/opencv.hpp>
#include <dlib/opencv/cv_image.h>
#import <Foundation/Foundation.h>
#import "FaceDetection/FDFaceFeatures.h"
#import <CoreGraphics/CoreGraphics.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <iostream>

@interface ERTWrapper : NSObject

-(dlib::full_object_detection)predictShape:(cv::Mat) imageBuffer Face:(FDFaceFeatures *)faceFeature  Size:(CGSize) size;
@end
