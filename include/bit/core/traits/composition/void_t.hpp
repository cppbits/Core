/*****************************************************************************
 * \file
 * \brief This header defines utilities for the void_t idiom
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
#ifndef BIT_CORE_TRAITS_COMPOSITION_VOID_T_HPP
#define BIT_CORE_TRAITS_COMPOSITION_VOID_T_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "type_identity.hpp"

namespace bit {
  namespace core {
    namespace detail {
      /// \brief struct helper for void_t
      template<typename...>
      struct voidify : type_identity<void>{};
    } // namespace detail

    /// \brief Utility metafunction that maps a sequence of any types to the
    ///        type void
    ///
    /// This is used for determining validity of expressions using SFINAE
    template<typename...Types>
    using void_t = typename detail::voidify<Types...>::type;

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_TRAITS_COMPOSITION_VOID_T_HPP */
