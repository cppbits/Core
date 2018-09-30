/*****************************************************************************
 * \file
 * \brief This header contains a type trait for determining if a type is
 *        a member function pointer
 *****************************************************************************/
#ifndef BIT_CORE_TRAITS_PROPERTIES_IS_MEMBER_FUNCTION_POINTER_HPP
#define BIT_CORE_TRAITS_PROPERTIES_IS_MEMBER_FUNCTION_POINTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../composition/conjunction.hpp"
#include "../composition/bool_constant.hpp" // true_type, false_type

namespace bit {
  namespace core {

    template<typename T>
    struct is_member_function_pointer : false_type{};

    template<typename R, typename T, typename...ArgTypes>
    struct is_member_function_pointer<R(T::*)(ArgTypes...)> : true_type{};

    template<typename R, typename T, typename...ArgTypes>
    struct is_member_function_pointer<R(T::*)(ArgTypes...) const> : true_type{};

    template<typename R, typename T, typename...ArgTypes>
    struct is_member_function_pointer<R(T::*)(ArgTypes...) volatile>
      : true_type{};

    template<typename R, typename T, typename...ArgTypes>
    struct is_member_function_pointer<R(T::*)(ArgTypes...) const volatile>
      : true_type{};

    template<typename T>
    constexpr bool is_member_function_pointer_v
      = is_member_function_pointer<T>::value;

    //-------------------------------------------------------------------------

    template<typename...Ts>
    struct are_member_function_pointers
      : conjunction<is_member_function_pointer<Ts>...>{};

    template<typename...Ts>
    constexpr bool are_member_function_pointers_v
      = are_member_function_pointers<Ts...>::value;

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_TRAITS_PROPERTIES_IS_MEMBER_FUNCTION_POINTER_HPP */
