//
//  detect_impl.inl
//
//  Created by 2bit on 2023/07/01.
//

        TargetRequest::ResultType TargetRequest::detect(const ofBaseHasPixels &pix) {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage(handler_impl, settings, [CIImage imageWithCGImage:cgImage]);
        }

        TargetRequest::ResultType TargetRequest::detect(IOSurfaceRef surface) {
            return detectWithCIImage(handler_impl, settings, [CIImage imageWithIOSurface:surface]);
        }

        TargetRequest::ResultType TargetRequest::detect(CVPixelBufferRef pix) {
            return detectWithCIImage(handler_impl, settings, [CIImage imageWithCVPixelBuffer:pix]);
        }
