//
//  type_traits.hpp
//
//  Created by 2bit on 2023/07/03.
//

#ifndef type_traits_hpp
#define type_traits_hpp

#include <type_traits>
#include <tuple>

namespace ofx {
    namespace Vision {
        template <typename ... types>
        using last_of_t = decltype(std::get<sizeof...(types) - 1>(std::declval<std::tuple<types ...>>()));
        
        template <typename ...>
        struct holder {};
        
        template <typename testee, typename holder, typename ... types>
        struct is_constructible_without_last_one_arg_impl;
        
        template <typename testee, typename type, typename ... types>
        struct is_constructible_without_last_one_arg_impl<testee, holder<types ...>, type>
        : std::is_constructible<testee, types ...>
        {};
        
        template <typename testee, typename type, typename type2, typename ... others, typename ... types>
        struct is_constructible_without_last_one_arg_impl<testee, holder<types ...>, type, type2, others ...>
        : is_constructible_without_last_one_arg_impl<testee, holder<types ..., type>, type2, others ...>
        {};
        
        template <typename testee, typename ... types>
        using is_constructible_without_last_one_arg = is_constructible_without_last_one_arg_impl<testee, holder<>, types ...>;
        
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
        
        template <typename type, typename ... arguments, std::size_t ... indices>
        inline type create_without_last_arg_impl(std::tuple<arguments ...> && t,
                                                 index_sequence<indices ...> &&)
        {
            return type(std::get<indices>(t) ...);
        }
        
        template <typename type>
        auto ref(type &v) -> decltype(std::ref(v)) { return std::ref(v); };
        template <typename type>
        auto ref(type &&v) -> type { return std::move(v); };

        template <typename type, typename ... arguments>
        inline type create_without_last_arg(arguments && ... args)
        {
            return create_without_last_arg_impl<type>(std::make_tuple(ref(std::forward<arguments>(args)) ...),
                                                make_index_sequence<sizeof...(arguments)  - 1>());
        }
        
        template <typename ... types>
        last_of_t<types ...> get_last(types && ... args) {
            return std::move(std::get<sizeof...(types) - 1>(std::make_tuple(ref(std::forward<types>(args)) ...)));
        }
    };
};

#endif /* type_traits_h */
