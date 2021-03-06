/*****************************************************************************
 * \file
 * \brief This file contains a custom implementation of the gsl::span;
 *        a mutable view of contiguous data.
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
#ifndef BIT_CORE_CONTAINERS_SPAN_HPP
#define BIT_CORE_CONTAINERS_SPAN_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../traits/composition/bool_constant.hpp"
#include "../traits/transformations/match_cv_qualifiers.hpp"
#include "../traits/relationships/is_same_decay.hpp"
#include "../traits/concepts/is_contiguous_container.hpp"
#include "../utilities/assert.hpp"          // BIT_ASSERT_OR_THROW
#include "../iterators/tagged_iterator.hpp" // tagged_iterator

#include <iterator>  // std::reverse_iterator
#include <algorithm> // std::min, std::max
#include <stdexcept> // std::out_of_range

namespace bit {
  namespace core {

    // Private Type Traits

    /// \brief The type used to represent dynamic-extents
    constexpr auto dynamic_extent = std::ptrdiff_t{-1};

    namespace detail {

      /////////////////////////////////////////////////////////////////////////
      /// \brief A type used to manage the extent.
      ///
      /// This is used in conjunction with storage_type to leverage
      /// empty base-class optimization.
      /////////////////////////////////////////////////////////////////////////
      template <std::ptrdiff_t Extent>
      class span_extent_type
      {
        //---------------------------------------------------------------------
        // Public Member Types
        //---------------------------------------------------------------------
      public:

        using index_type = std::ptrdiff_t;

        static_assert(Extent >= 0, "A fixed-size span must be >= 0 in size.");

        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        /// \brief Constructs an extent
        constexpr span_extent_type() noexcept {}

        /// \brief Constructs an extent from another extent
        ///
        /// \tparam Other the other extent value
        /// \param extent the other extent entry
        template <index_type Other>
        constexpr span_extent_type(span_extent_type<Other> extent)
        {
          static_assert( Other == Extent || Other == dynamic_extent,
                        "Mismatch between fixed-size extent and size of initializing data.");

          BIT_ASSERT(extent.size() == Extent,"Extent size must match for fixed size types");
        }

        constexpr span_extent_type( index_type size )
        {
          BIT_ASSERT( size == Extent,"Extent size must match for fixed size types");
        }

        //---------------------------------------------------------------------
        // Capacity
        //---------------------------------------------------------------------
      public:

        constexpr inline index_type size()
          const noexcept
        {
          return Extent;
        }
      };

      template <>
      class span_extent_type<dynamic_extent>
      {
        //---------------------------------------------------------------------
        // Public Member Types
        //---------------------------------------------------------------------
      public:

        using index_type = std::ptrdiff_t;

        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        template <index_type Other>
        explicit constexpr span_extent_type(span_extent_type<Other> extent)
          : m_size(extent.size())
        {

        }

        explicit constexpr span_extent_type(index_type size)
          : m_size(size)
        {
          BIT_ASSERT( size >= 0, "Span size must be greater than 0" );
        }

        //---------------------------------------------------------------------
        // Capacity
        //---------------------------------------------------------------------
      public:

        constexpr inline index_type size()
          const noexcept
        {
          return m_size;
        }

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        index_type m_size;
      };

      //=======================================================================
      // Type-Traits
      //=======================================================================

      template<std::ptrdiff_t To, std::ptrdiff_t From>
      struct is_allowed_extent_conversion
        : std::integral_constant<bool, From == To ||
                                       From == dynamic_extent ||
                                       To   == dynamic_extent>{};

      template <typename From, typename To>
      struct is_allowed_element_type_conversion
        : std::is_convertible<From (*)[], To (*)[]>{};


      template<typename T, typename = void>
      struct has_data_member_function : std::false_type{};

      template<typename T>
      struct has_data_member_function<T,void_t<decltype(std::declval<T>().data())>> : std::true_type{};

      template<typename T, typename = void>
      struct has_size_member_function : std::false_type{};

      template<typename T>
      struct has_size_member_function<T,void_t<decltype(std::declval<T>().size())>> : std::true_type{};

    }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A light-weight non-owning wrapper around contiguous memory.
    ///
    /// Overloads are provided for easy conversion from std::vector,
    /// std::array, references-to-arrays, strings, and simple pointer/size pairs.
    ///
    /// This helps to effectively reduce the number of available operations,
    /// while still exposing contiguous data to the client.
    ///
    /// This is based on Microsoft's implementation of the gsl::span
    ///
    /// \tparam T The type of this span
    /// \tparam Extent The maximum size of this span (default: unbounded)
    ///////////////////////////////////////////////////////////////////////////
    template<typename T, std::ptrdiff_t Extent = dynamic_extent>
    class span
    {
      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using value_type      = T;
      using pointer         = std::add_pointer_t<value_type>;
      using const_pointer   = std::add_pointer_t<std::add_const_t<value_type>>;
      using reference       = std::add_lvalue_reference_t<value_type>;
      using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;

      using size_type       = std::ptrdiff_t;
      using difference_type = std::ptrdiff_t;
      using index_type      = std::ptrdiff_t;

      using iterator               = tagged_iterator<T*,span<T,Extent>>;
      using const_iterator         = tagged_iterator<const T*,span<T,Extent>>;
      using reverse_iterator       = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

      //-----------------------------------------------------------------------
      // Public Static Members
      //-----------------------------------------------------------------------
    public:

      static constexpr size_type extent = Extent; ///< The extent of this view

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      template<typename U, std::ptrdiff_t OtherExtent>
      using enable_if_allowed_t = std::enable_if_t<
        detail::is_allowed_extent_conversion<OtherExtent, Extent>::value &&
        detail::is_allowed_element_type_conversion<U, value_type>::value
      >;

      template<typename U>
      using enable_if_container_t = std::enable_if_t<
        is_contiguous_container<std::decay_t<U>>::value &&
        !is_same_decay<std::decay_t<U>,span>::value
      >;

      /// \brief Constructs a span containing 0 entries
      constexpr span() noexcept;

      /// \brief Constructs a span containing 0 entries
      constexpr span( std::nullptr_t ) noexcept;

      /// \brief Constructs a span
      constexpr span( pointer first, pointer last ) noexcept;

      /// \brief Constructs a span by copying an existing one
      ///
      /// \param other the other span to copy
      constexpr span( const span& other ) noexcept = default;

      /// \brief Constructs a span by copying another span
      ///
      /// \param other the other span
#ifndef BIT_DOXYGEN_BUILD
      template<typename U, std::ptrdiff_t OtherExtent, typename = enable_if_allowed_t<U,OtherExtent>>
#else
      template<typename U, std::ptrdiff_t OtherExtent>
#endif
      constexpr span( const span<U,OtherExtent>& other );

      /// \brief Constructs a span from the specified \p ptr with
      ///        size \p count
      ///
      /// \param ptr   pointer to the data
      /// \param count the number of entries
      template<typename U>
      constexpr span( U* ptr, size_type count ) noexcept;

      /// \brief Constructs a span from a given array reference
      ///
      /// \param array a reference to a const T array of size N
      template<typename U, std::size_t N>
      constexpr /* IMPLICIT */ span( U (&array)[N] ) noexcept;

      /// \brief Constructs a span from a given contiguous container
      ///        that contains a data() member function and a size() member
      ///        function
      ///
      /// \param container the container to construct a span out of
      template<typename ContiguousContainer,
               typename = enable_if_container_t<ContiguousContainer>>
      constexpr /* IMPLICIT */ span( ContiguousContainer& container ) noexcept;
      template<typename ContiguousContainer,
        typename = enable_if_container_t<ContiguousContainer>>
      constexpr /* IMPLICIT */ span( const ContiguousContainer& container ) noexcept;

      //-----------------------------------------------------------------------

      /// \brief Assigns a span
      ///
      /// This will automatically resize the span to refer to at most N
      /// bytes of the supplied view
      ///
      /// \param other the span to copy
      /// \return reference to \c (*this)
      span& operator=( const span& other ) noexcept = default;

      //-----------------------------------------------------------------------
      // Capacity
      //-----------------------------------------------------------------------
    public:

      /// \brief Returns the size of the span
      ///
      /// \return the number of entries in the span
      constexpr size_type length() const noexcept;

      /// \brief Returns the size of the span
      ///
      /// \return the number of entries in the span
      constexpr size_type size() const noexcept;

      /// \brief Returns the max size of this span
      ///
      /// \return the max size of this span
      constexpr size_type max_size() const noexcept;

      /// \brief Returns the size of the span in bytes
      ///
      /// \return the number of bytes in the span
      constexpr size_type length_bytes() const noexcept;

      /// \brief Returns the size of the span in bytes
      ///
      /// \return the number of bytes in the span
      constexpr size_type size_bytes() const noexcept;

      /// \brief Returns whether the span is empty
      ///        (i.e. whether its size is 0).
      ///
      /// \return \c true if the span is empty
      constexpr bool empty() const noexcept;

      //-----------------------------------------------------------------------
      // Element Access
      //-----------------------------------------------------------------------
    public:

      /// \brief Gets the data of the current span
      ///
      /// \return the data this non-const span contains
      constexpr pointer data() noexcept;

      /// \copydoc span::data()
      constexpr const_pointer data() const noexcept;

      /// \brief Accesses the element at index \p pos
      ///
      /// \param pos the index to access
      /// \return const reference to the entry
      constexpr reference operator[]( index_type pos ) noexcept;

      /// \copydoc span::operator[]
      constexpr const_reference operator[]( index_type pos ) const noexcept;

      /// \brief Accesses the element at index \p pos
      ///
      /// \param pos the index to access
      /// \return const reference to the entry
      constexpr reference at( index_type pos );

      /// \copydoc span::at( index_type )
      constexpr const_reference at( index_type pos ) const;

      /// \brief Access the first entry of the span
      ///
      /// \note Undefined behavior if span is empty
      ///
      /// \return reference to the first entry of the span
      constexpr reference front() noexcept;

      /// \copydoc span::front()
      constexpr const_reference front() const noexcept;

      /// \brief References the last entry of the span
      ///
      /// \note Undefined behavior if span is empty
      ///
      /// \return reference to the last entry of the span
      constexpr reference back() noexcept;

      /// \copydoc span::back()
      constexpr const_reference back() const noexcept;

      //-----------------------------------------------------------------------
      // Modifiers
      //-----------------------------------------------------------------------
    public:

      /// \brief Swaps this span with \p other
      ///
      /// \param other the other span to swap with
      void swap( span& other ) noexcept;

      //-----------------------------------------------------------------------
      // Operations
      //-----------------------------------------------------------------------
    public:

      /// \brief Copies the sublist [pos, pos + rcount) to the data pointed
      ///        to by dest, where rcount is the smaller of count and size() - pos.
      ///
      /// \param dest pointer to the destination data
      /// \param count requested sublist length
      /// \param pos position of the first entry
      size_type copy( value_type* dest,
                      size_type count = dynamic_extent,
                      size_type pos = 0 ) const;

      /// \brief Returns a subview of this span
      ///
      /// \param offset the position of the first entry in the subview
      /// \param count  the length of the subview
      /// \return the created span subview
      constexpr span<T,dynamic_extent> subspan( size_type offset, size_type count = dynamic_extent ) const;

      /// \brief Returns a subview consisting of the first \p n entries
      ///        of the span
      ///
      /// \param n the number of entries from the beginning of the span
      /// \return the created span subview
      constexpr span<T,dynamic_extent> first( size_type n ) const;

      /// \brief Returns a subview consisting of the last \p n entries
      ///        of the span
      ///
      /// \param n the number of entries from the end of the span
      /// \return the created span subview
      constexpr span<T,dynamic_extent> last( size_type n ) const;

      //-----------------------------------------------------------------------
      // Iterators
      //-----------------------------------------------------------------------
    public:

      /// \brief Retrieves the begin iterator for this span
      ///
      /// \return the begin iterator
      constexpr iterator begin() const noexcept;
      constexpr const_iterator cbegin() const noexcept;

      /// \brief Retrieves the end iterator for this span
      ///
      /// \return the end iterator
      constexpr iterator end() const noexcept;
      constexpr const_iterator cend() const noexcept;

      //-----------------------------------------------------------------------

      /// \brief Retrieves the reverse begin iterator for this span
      ///
      /// \return the reverse begin iterator
      constexpr reverse_iterator rbegin() const noexcept;
      constexpr const_reverse_iterator crbegin() const noexcept;

      /// \brief Retrieves the reverse end iterator for this span
      ///
      /// \return the reverse end iterator
      constexpr reverse_iterator rend() const noexcept;
      constexpr const_reverse_iterator crend() const noexcept;


      //-----------------------------------------------------------------------
      // Private Member Types
      //-----------------------------------------------------------------------
    private:

      /////////////////////////////////////////////////////////////////////////
      /// \brief Internal class to use for storage of data
      ///
      /// This class uses ExtentType to leverage empty-base-class optimization
      /// for spans whose sizes are fixed
      /////////////////////////////////////////////////////////////////////////
      template<typename ExtentType>
      class storage_type : public ExtentType
      {
        //---------------------------------------------------------------------
        // Constructor
        //---------------------------------------------------------------------
      public:

        /// \brief Constructs a storage_type from a pointer to the data, and
        ///        an extent type
        ///
        /// \param data pointer to the data
        /// \param ext  the other extent to construct this storage_type from
        template<typename OtherExtentType>
        constexpr storage_type(pointer data, OtherExtentType ext)
          : ExtentType(ext),
            m_data(data)
        {
          BIT_ASSERT( (!data && ExtentType::size() == 0) || (data && ExtentType::size() >= 0), "span::storage_type: Invalid span size");
        }

        //---------------------------------------------------------------------
        // Accessors
        //---------------------------------------------------------------------
      public:

        /// \brief Retrieves the pointer to the data for a given span
        ///
        /// \return a pointer to the underlying data
        constexpr inline pointer data()
          const noexcept
        {
          return m_data;
        }

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        pointer m_data;
      };

      using extent_storage = storage_type<detail::span_extent_type<Extent>>;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      extent_storage m_storage; ///< The storage for this span

      template<typename,std::ptrdiff_t>
      friend class span;
    };

    //=========================================================================
    // non-member functions : class : span
    //=========================================================================

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Swaps the contents of \p lhs with \p rhs
    ///
    /// \param lhs the left span to swap
    /// \param rhs the right span to swap
    template<typename T, std::ptrdiff_t Extent>
    void swap( span<T,Extent>& lhs, span<T,Extent>& rhs ) noexcept;

    //-------------------------------------------------------------------------

    /// \{
    /// \brief Gets the \p Nth element from \p x
    ///
    /// \tparam N The entry to retrieve
    /// \param x the span to retrieve
    /// \return reference to the
    template<std::size_t N, typename T, std::ptrdiff_t Extent>
    constexpr T& get( span<T,Extent>& x ) noexcept;
    template<std::size_t N, typename T, std::ptrdiff_t Extent>
    constexpr const T& get( const span<T,Extent>& x ) noexcept;
    template<std::size_t N, typename T, std::ptrdiff_t Extent>
    constexpr T&& get( span<T,Extent>&& x ) noexcept;
    template<std::size_t N, typename T, std::ptrdiff_t Extent>
    constexpr const T&& get( const span<T,Extent>&& x ) noexcept;
    /// \}

    //-------------------------------------------------------------------------

    template<std::size_t N, typename T>
    constexpr T& get( span<T>& x );
    template<std::size_t N, typename T>
    constexpr const T& get( const span<T>& x );
    template<std::size_t N, typename T>
    constexpr T&& get( span<T>&& x );
    template<std::size_t N, typename T>
    constexpr const T&& get( const span<T>&& x );

    //-------------------------------------------------------------------------
    // Comparison Operators
    //-------------------------------------------------------------------------

    template<typename T, std::ptrdiff_t Extent>
    constexpr bool operator==( const span<T,Extent>& lhs,
                               const span<T,Extent>& rhs ) noexcept;
    template<typename T, std::ptrdiff_t Extent>
    constexpr bool operator!=( const span<T,Extent>& lhs,
                               const span<T,Extent>& rhs ) noexcept;
    template<typename T, std::ptrdiff_t Extent>
    constexpr bool operator<( const span<T,Extent>& lhs,
                              const span<T,Extent>& rhs ) noexcept;
    template<typename T, std::ptrdiff_t Extent>
    constexpr bool operator<=( const span<T,Extent>& lhs,
                               const span<T,Extent>& rhs ) noexcept;
    template<typename T, std::ptrdiff_t Extent>
    constexpr bool operator>( const span<T,Extent>& lhs,
                              const span<T,Extent>& rhs ) noexcept;
    template<typename T, std::ptrdiff_t Extent>
    constexpr bool operator>=( const span<T,Extent>& lhs,
                               const span<T,Extent>& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Type Traits
    //-------------------------------------------------------------------------

    /// \brief Is the given type a span ?
    ///
    /// \tparam T the type to check
    ///
    /// The result is aliased as \c ::value
    template<typename T>
    struct is_span : std::false_type{};

    template<typename T, std::ptrdiff_t N>
    struct is_span< span<T,N> > :  std::true_type{};

    /// \brief Is the given type a dynamic span?
    ///
    /// \tparam T the type to check
    ///
    /// The result is aliased as \c ::value
    template<typename T>
    struct is_dynamic_span : std::false_type{};

    template<typename T>
    struct is_dynamic_span< span<T,dynamic_extent> > : std::true_type{};

    inline namespace casts {

      // TODO: Add to_bytes and from_bytes
      // This has been removed because casting any T to stl::byte violates
      // strict aliasing under c++14, and causes undefined behaviour

    } // inline namespace casts
  } // namespace core
} // namespace bit

#include "detail/span.inl"

#endif /* BIT_CORE_CONTAINERS_SPAN_HPP */
