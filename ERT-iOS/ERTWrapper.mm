//
//  ERTWrapper.m
//  ERT
//
//  Created by Mamunul on 10/30/17.
//  Copyright © 2017 Mamunul. All rights reserved.
//

#import "ERTWrapper.h"


@interface ERTWrapper (){

	dlib::shape_predictor sp;

}

@end

@implementation ERTWrapper

- (instancetype)init
{
	self = [super init];
	if (self) {
		NSString *modelFileName = [[NSBundle mainBundle] pathForResource:@"shape_predictor_68_face_landmarks" ofType:@"dat"];
		std::string modelFileNameCString = [modelFileName UTF8String];
		
		dlib::deserialize(modelFileNameCString) >> sp;
	}
	return self;
}

-(dlib::full_object_detection)predictShape:(cv::Mat) image Face:(FDFaceFeatures *)faceFeature  Size:(CGSize) size{
	
	const int width = faceFeature.ImageSize.width;
	const int height = faceFeature.ImageSize.height;
	
	const CGRect rect = faceFeature.detectedFaceFeature.bounds;
	
	const int rectW = rect.size.width;
	const int rectH = rect.size.height;
	const int originX = rect.origin.x;
	const int originY = height - rect.origin.y - rectH;
	const dlib::rectangle oneFaceRect(originX , originY, originX + rectW, originY + rectH);
	
	dlib::full_object_detection shape = sp(dlib::cv_image<unsigned char>(image), oneFaceRect);
	
	cv::line(image, cv::Point2d(originX,originY), cv::Point2d(originX,originY + rectH), cv::Scalar(255,255,0));
	cv::line(image, cv::Point2d(originX,originY + rectH), cv::Point2d(originX + rectW,originY + rectH), cv::Scalar(255,255,0));
	cv::line(image, cv::Point2d(originX + rectW,originY + rectH), cv::Point2d(originX + rectW,originY), cv::Scalar(255,255,0));
	cv::line(image, cv::Point2d(originX + rectW,originY), cv::Point2d(originX,originY), cv::Scalar(255,255,0));

	for(int i = 0; i< shape.num_parts();i++)
	{
		const dlib::point p = shape.part(i);
		cv::circle(image,cv::Point2d(p.x(),p.y()),3,cv::Scalar(255,0,0),-1,8,0);
	}


	return shape;
}

@end
