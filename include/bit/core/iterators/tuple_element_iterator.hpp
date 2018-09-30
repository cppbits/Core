/*****************************************************************************
 * \file
 * \brief This header contains an implementation of the tuple_element_iterator,
 *        an adapter that wraps around iterators that iterate tuples to only
 *        return specific tuple elements.
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
#ifndef BIT_CORE_ITERATORS_DETAIL_TUPLE_ELEMENT_ITERATOR_HPP
#define BIT_CORE_ITERATORS_DETAIL_TUPLE_ELEMENT_ITERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../utilities/tuple_utilities.hpp"

#include <iterator>    // std::iterator_traits, std::iterator
#include <type_traits> // std::common_type
#include <tuple>       // std::tuple
#include <cstddef>     // std::size_t

namespace bit {
  namespace core {

    //////////////////////////////////////////////////////////////////////////
    ///
    //////////////////////////////////////////////////////////////////////////
    template<std::size_t N,typename Iterator>
    class tuple_element_iterator
    {
      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

      using value_type        = std::tuple_element_t<N,typename std::iterator_traits<Iterator>::value_type>;
      using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
      using pointer           = value_type*;
      using reference         = value_type&;
      using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

      //----------------------------------------------------------------------
      // Constructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Constructs a tuple_element_iterator from an underlying iterator
      ///
      /// \param it the iterator
      constexpr explicit tuple_element_iterator( Iterator it );

      /// \brief Copy constructs a tuple_element_iterator from another iterator
      ///
      /// \param other the other iterator to copy
      constexpr tuple_element_iterator( const tuple_element_iterator& other ) = default;

      /// \brief Move constructs a tuple_element_iterator from another iterator
      ///
      /// \param other the other iterator to move
      constexpr tuple_element_iterator( tuple_element_iterator&& other ) = default;

      /// \brief Copy assigns a tuple_element_iterator from another iterator
      ///
      /// \param other the other iterator to copy
      /// \return reference to \c (*this)
      tuple_element_iterator&
        operator=( const tuple_element_iterator& other ) = default;

      /// \brief Move assigns a tuple_element_iterator from another iterator
      ///
      /// \param other the other iterator to move
      /// \return reference to \c (*this)
      tuple_element_iterator&
        operator=( tuple_element_iterator&& other ) = default;

      //----------------------------------------------------------------------
      // Iteration
      //----------------------------------------------------------------------
    public:

      constexpr tuple_element_iterator& operator++() noexcept;
      constexpr tuple_element_iterator operator++(int) noexcept;

      constexpr tuple_element_iterator& operator--() noexcept;
      constexpr tuple_element_iterator operator--(int) noexcept;

      //----------------------------------------------------------------------
      // Random Access
      //----------------------------------------------------------------------
    public:

      constexpr tuple_element_iterator& operator+=( difference_type n ) noexcept;
      constexpr tuple_element_iterator& operator-=( difference_type n ) noexcept;
      constexpr difference_type operator-( const tuple_element_iterator& rhs ) const noexcept;

      //----------------------------------------------------------------------
      // Observers
      //----------------------------------------------------------------------
    public:

      constexpr pointer operator->() const noexcept;
      constexpr reference operator*() const noexcept;

      //----------------------------------------------------------------------
      // Comparison
      //----------------------------------------------------------------------
    public:

      constexpr bool operator == ( const tuple_element_iterator& rhs ) const noexcept;
      constexpr bool operator != ( const tuple_element_iterator& rhs ) const noexcept;
      constexpr bool operator <  ( const tuple_element_iterator& rhs ) const noexcept;
      constexpr bool operator <= ( const tuple_element_iterator& rhs ) const noexcept;
      constexpr bool operator >  ( const tuple_element_iterator& rhs ) const noexcept;
      constexpr bool operator >= ( const tuple_element_iterator& rhs ) const noexcept;

      //----------------------------------------------------------------------
      // Private Member Types
      //----------------------------------------------------------------------
    private:

      Iterator m_iter; ///< The underlying iterator
    };

    //-------------------------------------------------------------------------
    // Utility
    //-------------------------------------------------------------------------

    /// \brief Utility function to type-deduce and make an iterator for
    ///        iterating specific tuple elements
    ///
    /// \param it the iterator
    /// \return the tuple element iterator
    template<std::size_t N,typename Iterator>
    constexpr tuple_element_iterator<N,Iterator>
      make_tuple_element_iterator( Iterator it );

    /// \brief Convenience function for making a tuple iterator that only
    ///        iterates the first tuple element, which is the key in
    ///        associative containers
    ///
    /// \param it the iterator
    /// \return the tuple element iterator
    template<typename Iterator>
    constexpr tuple_element_iterator<0,Iterator>
      make_key_iterator( Iterator it );

    /// \brief Convenience function for making a tuple iterator that only
    ///        iterates the second tuple element, which is the value in
    ///        associative containers
    ///
    /// \param it the iterator
    /// \return the tuple element iterator
    template<typename Iterator>
    constexpr tuple_element_iterator<1,Iterator>
      make_value_iterator( Iterator it );

  } // namespace core
} // namespace bit

#include "detail/tuple_element_iterator.inl"

#endif /* BIT_CORE_ITERATORS_DETAIL_TUPLE_ELEMENT_ITERATOR_HPP */
