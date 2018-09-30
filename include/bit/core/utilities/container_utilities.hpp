/*****************************************************************************
 * \file
 * \brief This file contains various utilities (mostly functional) for
 *        handling containers of different types.
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
#ifndef BIT_CORE_UTILITIES_CONTAINER_UTILITIES_HPP
#define BIT_CORE_UTILITIES_CONTAINER_UTILITIES_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../traits/composition/type_identity.hpp"

#include "assert.hpp" // BIT_ASSERT

#include <initializer_list> // std::initializer_list
#include <utility>          // std::forward
#include <stdexcept>

namespace bit {
  namespace core {
    namespace detail {

      template<typename Ptr>
      struct type_of_member_ptr;

      template<typename T, typename U>
      struct type_of_member_ptr<U T::*> : type_identity<U>{};

      template<typename Ptr>
      using type_of_member_ptr_t = typename type_of_member_ptr<Ptr>::type;

    } // namespace detail

    //=========================================================================
    // trait : underlying_container_type
    //=========================================================================

    /// \brief Gets the underlying container type of any container adapter
    ///
    /// The result is aliased as \c ::type
    ///
    /// If \c T is not a container adapter, \c ::type is not defined to allow
    /// for SFINAE
    ///
    /// \tparam T the container adapter type to extract the underlying type from
    template<typename T>
    class underlying_container_type
    {
    private:
      struct I : private T
      {
        using type = detail::type_of_member_ptr_t<decltype(&I::c)>;
      };
    public:
      using type = typename I::type;
    };

    //-------------------------------------------------------------------------
    // Aliases
    //-------------------------------------------------------------------------

    /// \brief Convenience alias for extracting
    ///        \c underlying_container_type<T>::type
    template<typename T>
    using underlying_container_type_t = typename underlying_container_type<T>::type;

    //=========================================================================
    // non-member functions
    //=========================================================================

    /// \brief Accesses the element at \p n with bounds-checking
    ///
    /// \throw std::out_of_range if \c n >= N
    ///
    /// \param array the array to check
    /// \param n the index to retrieve
    /// \return reference to the entry
    template<typename T, std::size_t N>
    T& at( T(&array)[N], std::size_t n );

    /// \brief Accesses the element at \p n with bounds-checking
    ///
    /// \throw std::out_of_range if \c n >= N
    ///
    /// \param il the initializer list to extract from
    /// \param n the index to retrieve
    /// \return reference to the entry
    template<typename T>
    T& at( std::initializer_list<T> il, std::size_t n );

    /// \{
    /// \brief Accesses the element from the container with the given \p key
    ///
    /// \param c the container to check
    /// \param key the key into the container
    /// \return reference to the entry
    template<typename C, typename Key>
    auto at( C& c, Key&& key ) -> decltype(c.at( std::forward<Key>(key) ))
    {
      return c.at( std::forward<Key>(key) );
    }
    template<typename C, typename Key>
    auto at( const C& c, Key&& key ) -> decltype(c.at( std::forward<Key>(key) ))
    {
      return c.at( std::forward<Key>(key) );
    }
    /// \}

    //-------------------------------------------------------------------------

    /// \brief Gets a pointer to contiguous data from a specified \p array
    ///
    /// \param array the array
    /// \return pointer to the data
    template<typename T, std::size_t N>
    constexpr T* data( T(&array)[N] ) noexcept;

    /// \brief Gets a pointer to contiguous data from a specified initializer
    ///        list
    ///
    /// \param il the initializer list
    /// \return pointer to the data
    template<typename T>
    constexpr const T* data( std::initializer_list<T> il ) noexcept;

    /// \{
    /// \brief Gets a pointer to contiguous data in a container by delegating
    ///        to the container
    ///
    /// \param c the container
    /// \return pointer to the data
    template<typename C>
    constexpr auto data( C& c ) -> decltype(c.data())
    {
      return c.data();
    }
    template<typename C>
    constexpr auto data( const C& c ) -> decltype(c.data())
    {
      return c.data();
    }
    /// \}

    //-------------------------------------------------------------------------

    /// \brief Gets a reference to the first entry in the given \p array
    ///
    /// \param array the array
    /// \return reference to the first entry
    template<typename T, std::size_t N>
    constexpr T& front( T(&array)[N] ) noexcept;

    /// \brief Gets a reference to the first entry in the given initializer list
    ///
    /// \param il the initializer list
    /// \return reference to the first entry
    template<typename T>
    constexpr T& front( std::initializer_list<T> il ) noexcept;

    /// \{
    /// \brief Gets a reference to the first entry in the given container
    ///        by delegating the call to the container
    ///
    /// \param c the container
    /// \return reference to the first entry
    template<typename C>
    constexpr auto front( C& c ) noexcept
      -> decltype(c.front())
    {
      return c.front();
    }
    template<typename C>
    constexpr auto front( const C& c ) noexcept
      -> decltype(c.front())
    {
      return c.front();
    }
    /// \}

    //-------------------------------------------------------------------------

    /// \brief Gets a reference to the last entry in the given \p array
    ///
    /// \param array the array
    /// \return reference to the last entry
    template<typename T, std::size_t N>
    constexpr T& back( T(&array)[N] ) noexcept;

    /// \brief Gets a reference to the last entry in the given initializer list
    ///
    /// \param il the initializer list
    /// \return reference to the last entry
    template<typename T>
    constexpr T& back( std::initializer_list<T> il ) noexcept;

    /// \{
    /// \brief Gets a reference to the last entry in the given container
    ///        by delegating the call to the container
    ///
    /// \param c the container
    /// \return reference to the last entry
    template<typename C>
    constexpr auto back( C& c ) noexcept
      -> decltype( c.back() )
    {
      return c.back();
    }
    template<typename C>
    constexpr auto back( const C& c ) noexcept
      -> decltype( c.back() )
    {
      return c.back();
    }
    /// \}

    //-------------------------------------------------------------------------

    /// \brief Determines the size of a given array
    ///
    /// \param array the array
    /// \return the size of the array
    template<typename T, std::size_t N>
    constexpr std::size_t size( T(&array)[N] ) noexcept;

    /// \brief Determines the size of the specified container by delegating
    ///        to the container's implementation
    ///
    /// \param c the container
    /// \return the size of the container
    template<typename C>
    inline constexpr auto size( const C& c ) noexcept
      -> decltype(c.size())
    {
      return c.size();
    }

    //-------------------------------------------------------------------------

    /// \brief Determines if the array is empty
    ///
    /// This overload will always return \c false, since arrays of size 0 are
    /// illegal in C++.
    ///
    /// \param array the array
    /// \return \c false
    template<typename T, std::size_t N>
    constexpr bool empty( T(&array)[N] ) noexcept;

    /// \brief Determines if the initializer list is empty
    ///
    /// \param il the initializer list
    /// \return \c true if the initializer list is empty
    template<typename T>
    constexpr bool empty( std::initializer_list<T> il ) noexcept;

    /// \brief Determines if the container is empty by delegating to the
    ///        container's implementation
    ///
    /// \param c the container
    /// \return \c true if the container is empty
    template<typename C>
    inline constexpr auto empty( const C& c ) noexcept
      -> decltype(c.empty())
    {
      return c.empty();
    }

    //-------------------------------------------------------------------------

    /// \{
    /// \brief Gets the underlying container of a given container adapter
    ///
    /// \note This function does not participate in overload resolution if
    ///       \p container is not a container adapter
    ///
    /// \param container reference to the container adapter
    /// \return reference to the underlying container
    template<typename C>
    underlying_container_type_t<C>& get_underlying_container( C& container );
    template<typename C>
    const underlying_container_type_t<C>& get_underlying_container( const C& container );
    template<typename C>
    underlying_container_type_t<C>&& get_underlying_container( C&& container );
    template<typename C>
    const underlying_container_type_t<C>&& get_underlying_container( const C&& container );
    /// \}

  } // namespace core
} // namespace bit

#include "detail/container_utilities.inl"

#endif /* BIT_CORE_UTILITIES_CONTAINER_UTILITIES_HPP */