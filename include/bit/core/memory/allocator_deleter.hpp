/*****************************************************************************
 * \file
 * \brief This header contains the definition of a deleter for allocators
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
#ifndef BIT_CORE_MEMORY_ALLOCATOR_DESTRUCTOR_HPP
#define BIT_CORE_MEMORY_ALLOCATOR_DESTRUCTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <memory> // std::allocator_traits

namespace bit {
  namespace core {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A deleter for use with standard allocators
    ///
    /// This holds onto the allocator by reference
    ///
    /// \tparam Allocator the allocator to delete
    ///////////////////////////////////////////////////////////////////////////
    template<typename Allocator>
    class allocator_deleter
    {
      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using alloc_traits = std::allocator_traits<Allocator>;
      using pointer   = typename alloc_traits::pointer;
      using size_type = typename alloc_traits::size_type;

      //-----------------------------------------------------------------------
      // Constructor
      //-----------------------------------------------------------------------
    public:

      allocator_deleter( Allocator& alloc, size_type size ) noexcept;

      //-----------------------------------------------------------------------
      // Modifiers
      //-----------------------------------------------------------------------
    public:

      void operator()( pointer p ) noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      Allocator& m_allocator;
      size_type  m_size;
    };

  } // namespace core
} // namespace bit

#include "detail/allocator_deleter.inl"

#endif /* BIT_CORE_MEMORY_ALLOCATOR_DESTRUCTOR_HPP */
