/*****************************************************************************
 * \file
 * \brief This header defines a type-trait for determining if a given type is
 *        derived from all bases
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
#ifndef BIT_CORE_TRAITS_RELATIONSHIPS_IS_DERIVED_FROM_HPP
#define BIT_CORE_TRAITS_RELATIONSHIPS_IS_DERIVED_FROM_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../composition/conjunction.hpp"

#include <type_traits> // std::is_base_of

namespace bit {
  namespace core {

    /// \brief Type-trait to determine that \c Derived is derived from all
    ///        \c Bases
    ///
    /// The result is aliased as \c ::value
    ///
    /// \tparam Derived the derived type
    /// \tparam Bases the base types to check
    template<typename Derived, typename...Bases>
    struct is_derived_from_all : conjunction<std::is_base_of<Bases,Derived>...>{};

    /// \brief Helper utility to extract is_derived_from_all::value
    template<typename Derived, typename...Bases>
    constexpr bool is_derived_from_all_v = is_derived_from_all<Derived,Bases...>::value;

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_TRAITS_RELATIONSHIPS_IS_DERIVED_FROM_HPP */
