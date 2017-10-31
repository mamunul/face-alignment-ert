//
//  ERTWrapper.m
//  ERT
//
//  Created by Mamunul on 10/30/17.
//  Copyright © 2017 Mamunul. All rights reserved.
//

#import "ERTWrapper.h"
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <iostream>

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

-(void)predictShape:(unsigned char*) processedyImageBuffer{

	dlib::rectangle oneFaceRect ;
	dlib::array2d<dlib::bgr_pixel> img;
	 img.set_size(height, width);
	
	 dlib::full_object_detection shape = sp(img, oneFaceRect);


}

@end
