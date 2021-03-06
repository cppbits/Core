/*****************************************************************************
 * \file
 * \brief This header defines a type-trait for determining whether a type is
 *        implicitly convertible to another type
 *****************************************************************************/

/*
  The MIT License (MIT)

  CppBits Core Library.
  https://github.com/cppbits/Core

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef BIT_CORE_TRAITS_PROPERTIES_IS_IMPLICITLY_CONVERTIBLE_HPP
#define BIT_CORE_TRAITS_PROPERTIES_IS_IMPLICITLY_CONVERTIBLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../composition/bool_constant.hpp"
#include "../composition/void_t.hpp"

#include <type_traits> // std::declval

namespace bit {
  namespace core {
    namespace detail {

      template<typename T>
      struct is_implicitly_convertible_dummy
      {
        static void test( T );
      };

      template<typename From, typename To, typename = void>
      struct is_implicitly_convertible_impl : false_type{};

      template<typename From, typename To>
      struct is_implicitly_convertible_impl<From,To,void_t<
        decltype( is_implicitly_convertible_dummy<To>::test( std::declval<From>() ) )
      >> : true_type{};
    } // namespace detail

    /// \brief Type trait to determine if From is implicitly convertible to
    ///        To
    ///
    /// A type is considered implicitly convertible if the following expression
    /// is well-formed:
    ///
    /// \code
    /// To test(){ return std::declval<From>(); }
    /// \endcode
    ///
    /// For the purpose of the above illustration, std::declval is not
    /// considered ODR-used.
    ///
    /// The result is aliased as \c ::value
    template<typename From, typename To>
    struct is_implicitly_convertible
      : detail::is_implicitly_convertible_impl<From,To>{};

    /// \brief Helper utility to extract is_implicitly_convertible::type
    template<typename From, typename To>
    constexpr bool is_implicitly_convertible_v = is_implicitly_convertible<From,To>::value;

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_TRAITS_PROPERTIES_IS_IMPLICITLY_CONVERTIBLE_HPP */
