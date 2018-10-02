/*****************************************************************************
 * \file
 * \brief This header contains utilities for managing uninitialized storage
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
#ifndef BIT_CORE_UTILITY_UNINITIALIZED_STORAGE_HPP
#define BIT_CORE_UTILITY_UNINITIALIZED_STORAGE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "tuple_utilities.hpp" // adl::get

#include <iterator> // std::iterator_traits
#include <new>      // placement new
#include <memory>   // std::addressof
#include <utility>  // std::forward, std::index_sequence

namespace bit {
  namespace core {

    //-------------------------------------------------------------------------
    // Uninitialized Construction
    //-------------------------------------------------------------------------

    /// \brief Default constructs an instance of type \p T in the given memory
    ///
    /// \param ptr The memory location to construct into
    /// \return Pointer to the initialized memory (cast of \p ptr)
    template<typename T>
    T* uninitialized_default_construct_at( void* ptr );

    /// \brief Copy constructs an instance of type \p T in the given memory
    ///
    /// \param ptr   The memory location to construct into
    /// \param other The instance to copy
    /// \return Pointer to the initialized memory (cast of \p ptr)
    template<typename T>
    T* uninitialized_copy_at( void* ptr, const T& other );

    /// \brief Move constructs an instance of type \p T in the given memory
    ///
    /// \param ptr   The memory location to construct into
    /// \param other The instance to move
    /// \return Pointer to the initialized memory (cast of \p ptr)
    template<typename T>
    T* uninitialized_move_at( void* ptr, T&& other );

    /// \brief Constructs an instance of type \p T with the given \p args
    ///        at the memory location specified in \p ptr
    ///
    /// \param ptr     The memory location to construct into
    /// \param args... The arguments to supply to T's constructor
    /// \return Pointer to the initialized memory (cast of \p ptr)
    template<typename T, typename...Args>
    T* uninitialized_construct_at( void* ptr, Args&&...args );

    /// \brief Constructs an instance of type \p T with the given \p tuple
    ///        at the memory location specified in \p ptr.
    ///
    /// This forwards the arguments from the \p tuple to the constructor
    /// of T, as if by calling make_from_tuple
    ///
    /// \param ptr   The memory location to construct into
    /// \param tuple The tuple containing arguments to forward to T
    /// \return Pointer ot the initialized memory (cast of \p ptr)
    template<typename T, typename Tuple>
    T* uninitialized_tuple_construct_at( void* ptr, Tuple&& tuple );

    /// \brief Constructs an instance of type \p T with the given \p args
    ///        at the memory addresses in the given range [\p first, \p last )
    ///
    /// \param first the start of the range of the elements to copy
    /// \param last  the end of the range of the elements to copy
    /// \param copy  an instance to copy to each entry
    template<typename ForwardIterator>
    void uninitialized_construct( ForwardIterator first,
                                  ForwardIterator last );
    template<typename ForwardIterator>
    void uninitialized_construct( ForwardIterator first,
                                  ForwardIterator last,
                                  const typename std::iterator_traits<ForwardIterator>::value_type& copy );

    /// \{
    /// \brief Constructs an instance of type \p T with the given \p args
    ///        at the memory addresses in the given range [\p first, \p first + \p n )
    ///
    /// \param first the start of the range of the elements to copy
    /// \param n     the number of entries to construct
    /// \param copy  an instance to copy to each entry
    template<typename ForwardIterator, typename Size>
    ForwardIterator uninitialized_construct_n( ForwardIterator first,
                                               Size n );
    template<typename ForwardIterator, typename Size>
    ForwardIterator uninitialized_construct_n( ForwardIterator first,
                                               Size n,
                                               const typename std::iterator_traits<ForwardIterator>::value_type& copy );
    /// \}

    //-------------------------------------------------------------------------
    // Destruction
    //-------------------------------------------------------------------------

    /// \brief Calls the destructor of the object pointed to by p, as if by p->~T().
    ///
    /// \param p a pointer to the object to be destroyed
    template<typename T>
    void destroy_at( T* p );

    /// \brief Destroys the objects in the range \c [first, \c last)
    ///
    /// \param first the start of the range to destroy
    /// \param end   the end of the range to destroy
    template<typename ForwardIterator>
    void destroy( ForwardIterator first, ForwardIterator last );

    /// \brief Destroys the \p n objects in the range starting at \p first
    ///
    /// \param first the start of the range to destroy
    /// \param n     the number of entries to destroy
    template<typename ForwardIterator, typename Size>
    ForwardIterator destroy_n( ForwardIterator first, Size n );

  } // namespace core
} // namespace bit

#include "detail/uninitialized_storage.inl"

#endif /* BIT_CORE_UTILITY_UNINITIALIZED_STORAGE_HPP */
