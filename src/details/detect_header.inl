//
//  detect_header.inl
//
//  Created by 2bit on 2023/07/01.
//

            ResultType detectWithCIImage(ofxVisionCIImage *image);
            ResultType detect(const ofPixels &pix);
            ResultType detect(const ofBaseHasPixels &pix);
            ResultType detect(IOSurfaceRef surface);
            ResultType detect(CVPixelBufferRef pix);
