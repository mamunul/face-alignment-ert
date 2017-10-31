//
//  FDFaceDetectionDelegate.h
//  FDFaceMasking
//
//  Copyright © 2016 IPvision Canada Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "FDFaceDetectionDelegate.h"

typedef struct {
    long numberOfPixels;
    BOOL shouldApplyHistogram;
    CVPixelBufferRef processedData;
    unsigned char* rawyBuffer;
    unsigned char* rawuvBuffer;
}CameraData;
/*!
    @protocol       FDFaceDetectionDelegate
    @abstract       Interface for face detection delegates
    @discussion     Whoever wants to be notified on successful face detection,
                    have to implement this interface and register to FDFaceDetector.
 */
@protocol FDFaceDetectionDelegate <NSObject>

/*!
    @method         onDetectionSuccess:OnImageBuffer:
    @abstract       Called on successful face detection
    @param          features
                    Collection of detected CIFaceFeature
    @param          sampleBuffer
                    Camera preview image buffer
    @discussion     This method is always called immediately after faces are detected
                    on sampleBuffer image data. Note that, there can be no face in the
                    image data. In that case features will be empty.
 */
@required
-(void)onDetectionSuccess:(NSArray *)features
            OnCameraData:(CameraData )sampleBuffer;

@end
