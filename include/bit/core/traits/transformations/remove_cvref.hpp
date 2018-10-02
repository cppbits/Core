/*****************************************************************************
 * \file
 * \brief This header contains utility metafunctions for normalizing characters
 *        to just the 'char' type
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
#ifndef BIT_CORE_TRAITS_TRANSFORMATION_REMOVE_CVREF_HPP
#define BIT_CORE_TRAITS_TRANSFORMATION_REMOVE_CVREF_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../composition/type_identity.hpp"

#include <type_traits>

namespace bit {
  namespace core {

    /// \brief Removes any CV and reference qualifiers
    ///
    /// The result is aliased as \c ::type
    template<typename T>
    struct remove_cvref : type_identity<std::remove_cv_t<std::remove_reference_t<T>>>{};

    template<typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_TRAITS_TRANSFORMATION_REMOVE_CVREF_HPP */
