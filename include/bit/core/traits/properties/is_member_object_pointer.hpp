/*****************************************************************************
 * \file
 * \brief TODO: Add description
 *****************************************************************************/
#ifndef BIT_CORE_TRAITS_PROPERTIES_IS_MEMBER_OBJECT_POINTER_HPP
#define BIT_CORE_TRAITS_PROPERTIES_IS_MEMBER_OBJECT_POINTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../composition/conjunction.hpp"
#include "../composition/bool_constant.hpp" // true_type, false_type

namespace bit {
  namespace core {

    template<typename T>
    struct is_member_object_pointer : false_type{};

    template<typename R, typename T>
    struct is_member_object_pointer<R T::*> : true_type{};

    template<typename R, typename T>
    struct is_member_object_pointer<R T::* const> : true_type{};

    template<typename R, typename T>
    struct is_member_object_pointer<R T::* volatile> : true_type{};

    template<typename R, typename T>
    struct is_member_object_pointer<R T::* const volatile> : true_type{};

    template<typename T>
    constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;

    //-------------------------------------------------------------------------

    template<typename...Ts>
    using are_member_object_pointers = conjunction<is_member_object_pointer<Ts>...>;

    template<typename...Ts>
    constexpr bool are_member_object_pointer_v = are_member_object_pointers<Ts...>::value;

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_TRAITS_PROPERTIES_IS_MEMBER_OBJECT_POINTER_HPP */
