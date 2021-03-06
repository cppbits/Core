/*****************************************************************************
 * \file
 * \brief TODO: Add description
 *****************************************************************************/
#ifndef BIT_CORE_TRAITS_PROPERTIES_IS_MEMBER_POINTER_HPP
#define BIT_CORE_TRAITS_PROPERTIES_IS_MEMBER_POINTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../composition/disjunction.hpp"
#include "../composition/conjunction.hpp"

#include "is_member_object_pointer.hpp"
#include "is_member_function_pointer.hpp"

namespace bit {
  namespace core {

    template<typename T>
    struct is_member_pointer : disjunction<is_member_object_pointer<T>,
                                           is_member_function_pointer<T>>{};

    template<typename T>
    constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

    //-------------------------------------------------------------------------

    template<typename...Ts>
    struct are_member_pointers : conjunction<is_member_pointer<Ts>...>{};

    template<typename...Ts>
    constexpr bool are_member_pointers_v = are_member_pointers<Ts...>::value;

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_TRAITS_PROPERTIES_IS_MEMBER_POINTER_HPP */
