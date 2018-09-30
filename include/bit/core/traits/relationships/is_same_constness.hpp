/*****************************************************************************
 * \file
 * \brief This header defines a type-trait for detecting if a type has the same
 *        constness
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
#ifndef BIT_CORE_TRAITS_RELATIONSHIPS_IS_SAME_CONSTNESS_HPP
#define BIT_CORE_TRAITS_RELATIONSHIPS_IS_SAME_CONSTNESS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../composition/bool_constant.hpp"

#include <type_traits> // std::is_const

namespace bit {
  namespace core {

    /// \brief Type-trait to determine if \c T and \c U have the same const-
    ///        qualification
    ///
    /// The result is aliased as \c ::value
    template<typename T, typename U>
    struct is_same_constness : bool_constant<
      (std::is_const<T>::value == std::is_const<U>::value)
    >{};

    /// \brief Helper utility to extract is_same_constness::value
    template<typename T, typename U>
    constexpr bool is_same_constness_v = is_same_constness<T,U>::value;

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_TRAITS_RELATIONSHIPS_IS_SAME_CONSTNESS_HPP */
