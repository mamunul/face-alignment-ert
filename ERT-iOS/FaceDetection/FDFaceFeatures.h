//
//  FDFaceFeatures.h
//  FDFaceMasking
//
//  Copyright © 2016 IPvision Canada Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreImage/CoreImage.h>
/**
    @class         FDFaceFeatures
 
    @abstract      Data structure for face features provided by face detector(FDFaceDetector)
 
    @superclass    Superclass: NSObject
 */
@interface FDFaceFeatures : NSObject
/**
    This object is given by CIDetector when any face is detected which contains face bounds,
    eye,mouth position and face angle
 */
@property (readwrite) CIFaceFeature* detectedFaceFeature;
/**
    This is the size(height,width in pixel) of the image in which image CIDetector will search
    for any available face
 */
@property (readwrite) CGSize ImageSize;
@end
