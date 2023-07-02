//
//  deteils/ofx_vision_index_sequence.hpp
//
//  Created by 2bit on 2023/07/03.
//

#ifndef ofx_vision_index_sequence_hpp
#define ofx_vision_index_sequence_hpp

#include <type_traits>
#include <tuple>

namespace ofx {
    namespace Vision {
        template <std::size_t ... indices>
        struct index_sequence {};
        
        template <std::size_t n, std::size_t ... indices>
        struct index_sequence_helper
            : index_sequence_helper<n - 1U, n - 1U, indices ...>
        {};
        
        template <std::size_t ... indices>
        struct index_sequence_helper<0U, indices ... >
        { using type = index_sequence<indices ... >; };
        
        template <std::size_t n>
        using make_index_sequence = typename index_sequence_helper<n>::type;
    };
};

#endif /* ofx_vision_index_sequence_hpp */
