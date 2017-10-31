//
//  ViewController.m
//  FDFaceMasking
//
//  Copyright © 2016 IPvision Canada Inc. All rights reserved.
//


#import "FDFaceDetector.h"


@interface FDFaceDetector() {
    CIDetector *detector;
    NSMutableArray* delegates;
}
@end

@implementation FDFaceDetector

- (id)initWithContext:(CIContext*)context {
    self = [super init];
	if (self) {
        delegates = [[NSMutableArray alloc] init];
        // if iOS version is more than 9.0, CIDetectorNumberOfAngles will be used.
        if ([[[UIDevice currentDevice] systemVersion] compare:@"9.0" options:NSNumericSearch] == NSOrderedDescending) {
            NSArray *value = [NSArray arrayWithObjects: CIDetectorAccuracyHigh,[NSNumber numberWithFloat:0.30],[NSNumber numberWithInt:7],[NSNumber numberWithBool:YES], nil];
            NSArray *key = [NSArray arrayWithObjects:CIDetectorAccuracy,CIDetectorMinFeatureSize,CIDetectorNumberOfAngles, CIDetectorTracking, nil ];
            detector = [CIDetector detectorOfType:CIDetectorTypeFace context:context options:[NSDictionary dictionaryWithObjects:value forKeys:key ]];
        }
        else {
            NSArray *value = [NSArray arrayWithObjects: CIDetectorAccuracyHigh,[NSNumber numberWithFloat:0.30],[NSNumber numberWithBool:YES], nil];
            NSArray *key = [NSArray arrayWithObjects:CIDetectorAccuracy,CIDetectorMinFeatureSize, CIDetectorTracking, nil ];
            detector = [CIDetector detectorOfType:CIDetectorTypeFace context:context options:[NSDictionary dictionaryWithObjects:value forKeys:key ]];
        }
	}
    return self;
}

- (void) registerDelegate:(id<FDFaceDetectionDelegate>)delegate {
    [delegates addObject:delegate];
}

- (NSArray *)detectFaceInSampleBuffer:(CameraData )cameraData{
    
    
    CIImage *ciImage = [CIImage imageWithCVPixelBuffer:cameraData.processedData];
    CGSize imageSize =  ciImage.extent.size;
    
    NSArray *featureArray = [detector featuresInImage:ciImage options:nil];
    NSMutableArray *faceFeaturesArray = [[NSMutableArray alloc] init];
    
    for (CIFaceFeature *feature in featureArray) {
        
        FDFaceFeatures *faceFeatures = [[FDFaceFeatures alloc] init];
        faceFeatures.detectedFaceFeature = feature;
        faceFeatures.ImageSize = imageSize;
        [faceFeaturesArray addObject:faceFeatures];
    }
    ciImage = nil;
    for (id<FDFaceDetectionDelegate> delegate in delegates) {
        [delegate onDetectionSuccess:faceFeaturesArray OnCameraData:cameraData];
    }
    
	return faceFeaturesArray;
}

@end
