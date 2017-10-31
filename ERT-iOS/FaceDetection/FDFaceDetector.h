//
//  FDFaceDetector.h
//  FDFaceMasking
//
//  Copyright © 2016 IPvision Canada Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreImage/CoreImage.h>
#import <UIKit/UIKit.h>

#import "FDFaceDetectionDelegate.h"
#import "FDFaceFeatures.h"

/*!
 
    @class          FDFaceDetector
 
    @abstract       FDFaceDetector class is used to detect face using CIDetector
                    of CoreImage framework and
 
    @discussion     To get detected faces FDFaceDetectionDelegate has to be implemented.
                    CIDetector is initialized with the options CIDetectorAccuracy = High,
                    CIDetectorMinFeatureSize = 0.3, CIDetectorNumberOfAngles = 7
                    (supported on >= ios9.0), CIDetectorTracking = YES
    @superclass     Superclass: NSObject
 
 */
@interface FDFaceDetector : NSObject

/*!
    @method         initWithContext:
    @absrtact       Initilize a object with CIContext provided
 
    @param          context
                    Instance of CIContext
 
    @return         An initialized object of FDFaceDetection
 
 */
- (id)initWithContext:(CIContext*)context;

/*!
    @abstract       Detect face in the sampleBuffer provided in arguments
 
    @discussion     argument CMSampleBufferRef converted to CIImage which is used to identify
                    for face detection. If any face is detected then delegate method([delegate
                    onDetectionSuccess:faceFeaturesArray OnImageBuffer:sampleBuffer) is fired.
                    Delegate have to implement to get the detected face features
 
    @param          sampleBuffer
                    CMSampleBufferRef type data which is provided from
                    AVCaptureVideoDataOutputSampleBufferDelegate method
 */
- (NSArray *)detectFaceInSampleBuffer:(CameraData )cameraData;
/*!
    @method         registerDelegate:
    @abstract       Set FDFaceDetectionDelegate
 
    @param          delegate
                    Instance which will use this Delegate
 */
- (void)registerDelegate:(id<FDFaceDetectionDelegate>) delegate;
@end

