/*****************************************************************************
 * \file
 * \brief This header defines the type 'monotype'
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
#ifndef BIT_CORE_UTILITIES_MONOSTATE_HPP
#define BIT_CORE_UTILITIES_MONOSTATE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "hash.hpp" // hash_t

#include <cstddef> // std::size_t

namespace bit {
  namespace core {

    //=========================================================================
    // class : monostate
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Unit type intended for use as a well-behaved empty alternative
    ///        in \c variant.
    ///
    /// In particular, a variant of non-default-constructible types may list
    /// monostate as its first alternative: this makes the variant itself
    /// default-constructible.
    ///////////////////////////////////////////////////////////////////////////
    struct monostate{};

    //=========================================================================
    // non-member functions : class : monostate
    //=========================================================================

    //-------------------------------------------------------------------------
    // Comparison
    //-------------------------------------------------------------------------

    constexpr bool operator<(monostate, monostate) noexcept;
    constexpr bool operator>(monostate, monostate) noexcept;
    constexpr bool operator<=(monostate, monostate) noexcept;
    constexpr bool operator>=(monostate, monostate) noexcept;
    constexpr bool operator==(monostate, monostate) noexcept;
    constexpr bool operator!=(monostate, monostate) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Hashes the monostate
    ///
    /// \return 0
    constexpr hash_t hash_value( const monostate& );

  } // namespace core
} // namespace bit

#include "detail/monostate.inl"

#endif /* BIT_CORE_UTILITIES_MONOSTATE_HPP */
