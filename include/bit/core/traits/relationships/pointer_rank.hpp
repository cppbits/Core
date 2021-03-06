/*****************************************************************************
 * \file
 * \brief This header defines the type-trait for determining the level of
 *        indirection
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
#ifndef BIT_CORE_TRAITS_RELATIONSHIPS_POINTER_RANK_HPP
#define BIT_CORE_TRAITS_RELATIONSHIPS_POINTER_RANK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../composition/size_constant.hpp"

#include <type_traits> // std::decay_t
#include <cstddef>     // std::size_t

namespace bit {
  namespace core {

    namespace detail {

      template<typename T, std::size_t Count>
      struct pointer_rank : size_constant<Count>{};

      template<typename T, std::size_t Count>
      struct pointer_rank<T*,Count> : pointer_rank<std::decay_t<T>,Count+1>{};
    }

    /// \brief Determines the level of indirection of a given type T
    ///
    /// This effectively counts the number of pointer types in a given type T
    ///
    /// The result is stored in \c ::value
    template<typename T>
    using pointer_rank = detail::pointer_rank<T,0>;

    /// \brief Helper utility to extract pointer_rank::value
    template<typename T>
    constexpr std::size_t pointer_rank_v = pointer_rank< std::decay_t<T> >::value;
  } // namespace core
} // namespace bit



#endif /* BIT_CORE_TRAITS_RELATIONSHIPS_POINTER_RANK_HPP */
