//
//  create_req_res_header.inl
//
//  Created by 2bit on 2023/07/01.
//

            Request *createRequest() const;
            ResultType createResult(void *result) const;

            template <typename ... Detectors>
            friend struct MultipleDetector;
