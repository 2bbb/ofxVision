//
//  detect_impl.inl
//
//  Created by 2bit on 2023/07/01.
//

        Target::ResultType Target::detect(const ofPixels &pix) {
            CGImageRef cgImage = ofPixelsToCGImageRef(pix);
            return detectWithCIImage([CIImage imageWithCGImage:cgImage]);
        }

        Target::ResultType Target::detect(const ofBaseHasPixels &pix) {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage([CIImage imageWithCGImage:cgImage]);
        }

        Target::ResultType Target::detect(IOSurfaceRef surface) {
            return detectWithCIImage([CIImage imageWithIOSurface:surface]);
        }

        Target::ResultType Target::detect(CVPixelBufferRef pix) {
            return detectWithCIImage([CIImage imageWithCVPixelBuffer:pix]);
        }
