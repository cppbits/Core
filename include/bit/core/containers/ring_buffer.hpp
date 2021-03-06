/*****************************************************************************
 * \file
 * \brief This header contains the implementation for a circular buffer into
 *        non-owned memory
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
#ifndef BIT_CORE_CONTAINERS_RING_BUFFER_HPP
#define BIT_CORE_CONTAINERS_RING_BUFFER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../utilities/uninitialized_storage.hpp" // destroy_at
#include "../utilities/compiler_traits.hpp" // BIT_COMPILER_EXCEPTIONS_ENABLED

#include <iterator>    // std::bidirectional_iterator_tag, std::reverse_iterator
#include <algorithm>   // std::equal, std::lexicographical_compare
#include <cstddef>     // std::size_t
#include <type_traits> // std::add_pointer_t, etc

namespace bit {
  namespace core {

    //=========================================================================
    // class : detail::ring_buffer_iterator
    //=========================================================================

    namespace detail {

      /////////////////////////////////////////////////////////////////////////
      /// \brief An iterator for iterating the circular buffer
      ///
      /// \tparam Container type that will always be 'circular_buffer' (to break
      ///                   cyclic dependency that requires forward-declaration)
      /// \tparam T the underlying type
      /////////////////////////////////////////////////////////////////////////
      template<typename Container, typename T>
      class ring_buffer_iterator
      {
        //---------------------------------------------------------------------
        // Public Member Types
        //---------------------------------------------------------------------
      public:

        using value_type = T;
        using reference  = std::add_lvalue_reference_t<T>;
        using pointer    = std::add_pointer_t<T>;
        using size_type  = std::size_t;
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------
      public:

        ring_buffer_iterator( Container& container,
                              T* ptr,
                              bool compare_twice );

        ring_buffer_iterator( const ring_buffer_iterator& other ) noexcept = default;

        ring_buffer_iterator( ring_buffer_iterator&& other ) noexcept = default;

        //---------------------------------------------------------------------

        ring_buffer_iterator& operator=( const ring_buffer_iterator& other ) noexcept = default;

        ring_buffer_iterator& operator=( ring_buffer_iterator&& other ) noexcept = default;

        //---------------------------------------------------------------------
        // Iteration
        //---------------------------------------------------------------------
      public:

        ring_buffer_iterator& operator++() noexcept;
        ring_buffer_iterator operator++(int) noexcept;

        ring_buffer_iterator& operator--() noexcept;
        ring_buffer_iterator operator--(int) noexcept;

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        reference operator*() noexcept;
        pointer operator->() noexcept;

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        value_type* m_ptr;       ///< Pointer to the location in the buffer
        Container*  m_container; ///< The underlying container
        bool m_compare_twice; ///< Will this iterator be compared twice? (full buffer)

        template<typename C, typename U>
        friend bool operator==( const ring_buffer_iterator<C,U>& lhs,
                                const ring_buffer_iterator<C,U>& rhs ) noexcept;

      };

      template<typename C, typename T>
      bool operator==( const ring_buffer_iterator<C,T>& lhs,
                       const ring_buffer_iterator<C,T>& rhs ) noexcept;

      template<typename C, typename T>
      bool operator!=( const ring_buffer_iterator<C,T>& lhs,
                       const ring_buffer_iterator<C,T>& rhs ) noexcept;

    } // namespace detail

    //=========================================================================
    // class : ring_buffer
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This class is an implementation of a non-owning circular
    ///        buffer
    ///
    /// All emplace/push operations perform constructions inside the buffer.
    /// If the buffer is already filled, the destructor of the previous
    /// function is invoked prior to construction of the newly added item.
    ///
    /// \tparam T the underlying type of this buffer
    ///////////////////////////////////////////////////////////////////////////
    template<typename T>
    class ring_buffer
    {
      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using value_type      = T;
      using reference       = std::add_lvalue_reference_t<T>;
      using pointer         = std::add_pointer_t<T>;
      using const_reference = std::add_lvalue_reference_t<std::add_const_t<T>>;
      using const_pointer   = std::add_pointer_t<std::add_const_t<T>>;

      using size_type       = std::size_t;
      using difference_type = std::ptrdiff_t;

      using iterator       = detail::ring_buffer_iterator<ring_buffer,T>;
      using const_iterator = detail::ring_buffer_iterator<const ring_buffer,const T>;
      using reverse_iterator       = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

      //-----------------------------------------------------------------------
      // Constructor
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs a null circular_buffer
      ring_buffer() noexcept;

      /// \brief Constructs a null circular_buffer
      explicit ring_buffer( std::nullptr_t ) noexcept;

      /// \brief Constructs a circular buffer from an uninitialized buffer
      ///        that holds \p size \p T entries
      ///
      /// \param buffer a pointer to the buffer
      /// \param size the size of the buffer
      ring_buffer( void* buffer, size_type size ) noexcept;

      /// \brief Move-constructs a circular buffer from another buffer
      ///
      /// \param other the other circular_buffer to move
      ring_buffer( ring_buffer&& other ) noexcept;

      // Deleted copy constructor
      ring_buffer(const ring_buffer& ) = delete;

      //-----------------------------------------------------------------------

      /// \brief Destructs this circular_buffer, destructing all instances
      ~ring_buffer();

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a circular_buffer from another one
      ///
      /// \param other the other circular_buffer to move
      /// \return a reference to \c (*this)
      ring_buffer& operator=( ring_buffer other ) noexcept;

      //-----------------------------------------------------------------------
      // Modifiers
      //-----------------------------------------------------------------------
    public:

      /// \brief Invokes \p T's constructor with the given \p args, storing
      ///        the result at the end of the buffer
      ///
      /// \note If the buffer is full, this will destruct the entry currently
      ///       at the \c front of the buffer before construction
      ///
      /// \param args the arguments to forward to T
      template<typename...Args,
               typename=std::enable_if_t<std::is_constructible<T,Args...>::value>>
      reference emplace_back( Args&&...args );

      /// \brief Invokes \p T's constructor with the given \p args, storing
      ///        the result at the beginning of the bugger
      ///
      /// \note If the buffer is full, this will destruct the entry currently
      ///       at the \c end of the buffer before construction
      ///
      /// \param args the arguments to forward to T
      template<typename...Args,
               typename=std::enable_if_t<std::is_constructible<T,Args...>::value>>
      reference emplace_front( Args&&...args );

      //-----------------------------------------------------------------------

      /// \brief Constructs a \p T object by calling the copy-constructor, and
      ///        storing the result at the end of the buffer
      ///
      /// \note If the buffer is full, this will destruct the entry currently
      ///       at the \c front of the buffer before construction
      ///
      /// \param value the value to copy
      template<typename U=T,
               typename=std::enable_if_t<std::is_copy_constructible<U>::value>>
      void push_back( const T& value );

      /// \brief Constructs a \p T object by calling the move-constructor, and
      ///        storing the result at the end of the buffer
      ///
      /// \note If the buffer is full, this will destruct the entry currently
      ///       at the \c front of the buffer before construction
      ///
      /// \param value the value to move
      template<typename U=T,
               typename=std::enable_if_t<std::is_move_constructible<U>::value>>
      void push_back( T&& value );

      //-----------------------------------------------------------------------

      /// \brief Constructs a \p T object by calling the copy-constructor, and
      ///        storing the result at the front of the buffer
      ///
      /// \note If the buffer is full, this will destruct the entry currently
      ///       at the \c back of the buffer before construction
      ///
      /// \param value the value to copy
      template<typename U=T,
               typename=std::enable_if_t<std::is_copy_constructible<U>::value>>
      void push_front( const T& value );

      /// \brief Constructs a \p T object by calling the move-constructor, and
      ///        storing the result at the end of the buffer
      ///
      /// \note If the buffer is full, this will destruct the entry currently
      ///       at the \c back of the buffer before construction
      ///
      /// \param value the value to copy
      template<typename U=T,
               typename=std::enable_if_t<std::is_move_constructible<U>::value>>
      void push_front( T&& value );

      //-----------------------------------------------------------------------

      /// \brief Pops the entry at the front of the circular_buffer
      void pop_front();

      /// \brief Pops the entry at the back of the circular_buffer
      void pop_back();

      //-----------------------------------------------------------------------

      /// \brief Clears all entries from this circular_buffer
      void clear() noexcept;

      /// \brief Swaps this circular_buffer with another one
      ///
      /// \param other the other buffer to swap with
      void swap( ring_buffer& other ) noexcept;

      //-----------------------------------------------------------------------
      // Capacity
      //-----------------------------------------------------------------------
    public:

      /// \brief Returns whether this buffer is empty
      ///
      /// \return \c true if the buffer is empty
      bool empty() const noexcept;

      /// \brief Returns whether this buffer is full
      ///
      /// \return \c true if the buffer is full
      bool full() const noexcept;

      /// \brief Returns the number of elements in this circular buffer
      ///
      /// \return the number of elements in this buffer
      size_type size() const noexcept;

      /// \brief Returns the max size of this buffer
      ///
      /// \note This result is always the same as capacity
      /// \return the max number of elements this circular_buffer can contain
      size_type max_size() const noexcept;

      /// \brief Returns the capacity of this buffer
      ///
      /// \return the capacity of this circular_buffer
      size_type capacity() const noexcept;

      //-----------------------------------------------------------------------
      // Element Access
      //-----------------------------------------------------------------------
    public:

      /// \brief Returns a reference to the front element of this
      ///        circular_buffer
      ///
      /// \return reference to the front element of this circular_buffer
      reference front() noexcept;

      /// \copydoc front()
      const_reference front() const noexcept;

      //-----------------------------------------------------------------------

      /// \brief Returns a reference to the back element of this
      ///        circular_buffer
      ///
      /// \return reference to the front element of this circular_buffer
      reference back() noexcept;

      /// \copydoc back()
      const_reference back() const noexcept;

      //-----------------------------------------------------------------------

      /// \brief Gets a pointer to the underlying buffer
      ///
      /// \return the pointer
      pointer data() noexcept;

      /// \copydoc data()
      const_pointer data() const noexcept;

      //-----------------------------------------------------------------------
      // Iterators
      //-----------------------------------------------------------------------
    public:

      /// \brief Gets the iterator to the beginning of this range
      ///
      /// \return the begin iterator
      iterator begin() noexcept;

      /// \copydoc begin
      const_iterator begin() const noexcept;

      /// \copydoc begin
      const_iterator cbegin() const noexcept;

      /// \brief Gets the iterator to the end of this range
      ///
      /// \return the end iterator
      iterator end() noexcept;

      /// \copydoc end
      const_iterator end() const noexcept;

      /// \copydoc end
      const_iterator cend() const noexcept;

      //-----------------------------------------------------------------------

      /// \brief Gets the iterator to the beginning of the reverse range
      ///
      /// \return the reverse iterator
      reverse_iterator rbegin() noexcept;

      /// \copydoc rbegin()
      const_reverse_iterator rbegin() const noexcept;

      /// \copydoc rbegin()
      const_reverse_iterator crbegin() const noexcept;

      /// \brief Gets the iterator to the end of the reverse range
      ///
      /// \return the reverse iterator
      reverse_iterator rend() noexcept;

      /// \copydoc rend()
      const_reverse_iterator rend() const noexcept;

      /// \copydoc rend()
      const_reverse_iterator crend() const noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      T* m_buffer;            ///< Pointer to the underlying buffer
      T* m_begin;             ///< The beginning pointer
      T* m_end;               ///< The end pointer
      std::size_t m_capacity; ///< The size of the buffer
      std::size_t m_size;     ///< The total entries in the circular_buffer

      template<typename,typename> friend class detail::ring_buffer_iterator;

      //-----------------------------------------------------------------------
      // Private Member Functions
      //-----------------------------------------------------------------------
    private:

      T*& increment( T*& iter ) noexcept;
      const T*& increment( const T*& iter ) const noexcept;
      T*& decrement( T*& iter ) noexcept;
      const T*& decrement( const T*& iter ) const noexcept;
    };

    //=========================================================================
    // non-member functions : class : ring_buffer
    //=========================================================================

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<typename T>
    void swap( ring_buffer<T>& lhs, ring_buffer<T>& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Comparison
    //-------------------------------------------------------------------------

    template<typename T>
    bool operator==( const ring_buffer<T>& lhs,
                     const ring_buffer<T>& rhs ) noexcept;
    template<typename T>
    bool operator!=( const ring_buffer<T>& lhs,
                     const ring_buffer<T>& rhs ) noexcept;
    template<typename T>
    bool operator<( const ring_buffer<T>& lhs,
                    const ring_buffer<T>& rhs ) noexcept;
    template<typename T>
    bool operator>( const ring_buffer<T>& lhs,
                    const ring_buffer<T>& rhs ) noexcept;
    template<typename T>
    bool operator<=( const ring_buffer<T>& lhs,
                     const ring_buffer<T>& rhs ) noexcept;
    template<typename T>
    bool operator>=( const ring_buffer<T>& lhs,
                     const ring_buffer<T>& rhs ) noexcept;

  } // namespace core
} // namespace bit

#include "detail/ring_buffer.inl"

#endif /* BIT_CORE_CONTAINERS_RING_BUFFER_HPP */
