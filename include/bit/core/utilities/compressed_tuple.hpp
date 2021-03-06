/*****************************************************************************
 * \file
 * \brief This header contains a utility for compressing tuples to leverage
 *        EBO
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
#ifndef BIT_CORE_UTILITIES_COMPRESSED_TUPLE_HPP
#define BIT_CORE_UTILITIES_COMPRESSED_TUPLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "compressed_pair.hpp"
#include "in_place.hpp"
#include "tuple_utilities.hpp"

#include "../traits/composition/size_constant.hpp"
#include "../traits/relationships/is_one_of.hpp"
#include "../traits/relationships/nth_type.hpp"

#include <type_traits> // std::is_empty, std::is_final, etc
#include <utility>     // std::forward

namespace bit {
  namespace core {
    namespace detail {

      //=======================================================================
      // class : compressed_tuple_storage
      //=======================================================================

      template<std::size_t Idx, typename T, bool IsEBO>
      class compressed_tuple_storage;

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T>
      class compressed_tuple_storage<Idx,T,true>
        : private T
      {
        static_assert( !std::is_reference<T>::value, "compressed_tuple cannot store references" );

        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        constexpr compressed_tuple_storage() = default;

        constexpr compressed_tuple_storage( const compressed_tuple_storage& other ) = default;

        constexpr compressed_tuple_storage( compressed_tuple_storage&& other ) = default;

        //---------------------------------------------------------------------

        template<typename Tuple, std::size_t...Idxs>
        constexpr compressed_tuple_storage( std::piecewise_construct_t,
                                            Tuple&& tuple,
                                            std::index_sequence<Idxs...> );

        template<typename...Args>
        constexpr compressed_tuple_storage( in_place_t,
                                            Args&&...args );

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        constexpr T& do_get( size_constant<Idx> ) &;
        constexpr const T& do_get( size_constant<Idx> ) const &;
        constexpr T&& do_get( size_constant<Idx> ) &&;
        constexpr const T&& do_get( size_constant<Idx> ) const &&;

        //---------------------------------------------------------------------
        // Modifiers
        //---------------------------------------------------------------------
      public:

        template<typename U>
        void do_assign( size_constant<Idx>, U&& val );
      };

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T>
      class compressed_tuple_storage<Idx,T,false>
      {
        static_assert( !std::is_reference<T>::value, "compressed_tuple cannot store references" );

        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        constexpr compressed_tuple_storage() = default;

        constexpr compressed_tuple_storage( const compressed_tuple_storage& other ) = default;

        constexpr compressed_tuple_storage( compressed_tuple_storage&& other ) = default;

        //---------------------------------------------------------------------

        template<typename Tuple, std::size_t...Idxs>
        constexpr compressed_tuple_storage( std::piecewise_construct_t,
                                            Tuple&& tuple,
                                            std::index_sequence<Idxs...> );

        template<typename...Args>
        constexpr compressed_tuple_storage( in_place_t,
                                            Args&&...args );

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        constexpr T& do_get( size_constant<Idx> ) &;
        constexpr const T& do_get( size_constant<Idx> ) const &;
        constexpr T&& do_get( size_constant<Idx> ) &&;
        constexpr const T&& do_get( size_constant<Idx> ) const &&;

        //---------------------------------------------------------------------
        // Modifiers
        //---------------------------------------------------------------------
      public:

        template<typename U>
        void do_assign( size_constant<Idx>, U&& val );

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        T m_current;
      };

      //=======================================================================
      // class : compressed_tuple_impl
      //=======================================================================

      template<std::size_t Idx, typename...Ts>
      class compressed_tuple_impl;

      //-----------------------------------------------------------------------

      template<std::size_t Idx, typename T0, typename...Ts>
      class compressed_tuple_impl<Idx, T0, Ts...>
        : compressed_tuple_storage<Idx,T0,std::is_empty<T0>::value && !std::is_final<T0>::value && !is_one_of<T0,Ts...>::value>,
          compressed_tuple_impl<Idx+1,Ts...>
      {
        using storage_type = compressed_tuple_storage<Idx,T0,std::is_empty<T0>::value && !std::is_final<T0>::value>;
        using base_type = compressed_tuple_impl<Idx+1,Ts...>;

        //---------------------------------------------------------------------
        // Constructors / Assignment
        //---------------------------------------------------------------------
      public:

        constexpr compressed_tuple_impl() noexcept;
        constexpr compressed_tuple_impl( const compressed_tuple_impl& other ) noexcept = default;
        constexpr compressed_tuple_impl( compressed_tuple_impl&& other ) noexcept = default;

        template<typename Arg0, typename...Args>
        constexpr compressed_tuple_impl( in_place_t,
                                         Arg0&& arg0, Args&&...args );

        template<typename Tuple0, typename...Tuples>
        constexpr compressed_tuple_impl( std::piecewise_construct_t,
                                         Tuple0&& tuple0, Tuples&&...tuples );

        //---------------------------------------------------------------------

        compressed_tuple_impl& operator=( const compressed_tuple_impl& other ) noexcept = default;
        compressed_tuple_impl& operator=( compressed_tuple_impl&& other ) noexcept = default;

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        using storage_type::do_get;
        using base_type::do_get;

        //---------------------------------------------------------------------
        // Modifiers
        //---------------------------------------------------------------------
      public:

        constexpr void swap( compressed_tuple_impl& other ) noexcept;

        template<typename Arg0, typename...Args>
        void assign( Arg0&& arg0, Args&&...args );
      };

      //-----------------------------------------------------------------------

      template<std::size_t Idx>
      class compressed_tuple_impl<Idx>
      {
        //---------------------------------------------------------------------
        // Constructors / Assignment
        //---------------------------------------------------------------------
      public:

        constexpr compressed_tuple_impl() noexcept;
        constexpr compressed_tuple_impl( const compressed_tuple_impl& other ) noexcept = default;
        constexpr compressed_tuple_impl( compressed_tuple_impl&& other ) noexcept = default;

        constexpr compressed_tuple_impl( in_place_t );

        constexpr compressed_tuple_impl( std::piecewise_construct_t );

        //---------------------------------------------------------------------

        compressed_tuple_impl& operator=( const compressed_tuple_impl& other ) noexcept = default;
        compressed_tuple_impl& operator=( compressed_tuple_impl&& other ) noexcept = default;

        //---------------------------------------------------------------------
        // Observers
        //---------------------------------------------------------------------
      public:

        void do_get( size_constant<Idx> ) noexcept;

        //---------------------------------------------------------------------
        // Modifiers
        //---------------------------------------------------------------------
      public:

        constexpr void swap( compressed_tuple_impl& ) noexcept;

        void assign() noexcept;

      };
    } // namespace detail

    //=========================================================================
    // class : compressed_tuple
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A tuple that leverages EBO to compress types
    ///
    /// \tparam Types the types to compress
    ///////////////////////////////////////////////////////////////////////////
    template<typename...Types>
    class compressed_tuple : detail::compressed_tuple_impl<0,Types...>
    {
      using base_type = detail::compressed_tuple_impl<0,Types...>;

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      // (1)
      /// \brief Default constructs this compressed_tuple by default
      ///        constructing each elements
      ///
      /// \note This overload is only enabled if all elements are trivially
      ///       constructible
      constexpr compressed_tuple() = default;

      // (2)
      /// \brief Constructs a compressed_tuple by copy-constructing each
      ///        element
      ///
      /// \param args the arguments to copy
      explicit constexpr compressed_tuple( const Types&...args );

      // (3)
      /// \brief Value-constructs a compressed_tuple by value-constructin each
      ///        element
      ///
      /// \param args the arguments to forward
      template<typename... UTypes>
      explicit constexpr compressed_tuple( UTypes&&...args );

      // (4)
      /// \brief Copy-constructs a compressed_tuple from another tuple
      ///
      /// \param other the other tuple to copy
      template<typename...UTypes>
      constexpr compressed_tuple( const compressed_tuple<UTypes...>& other );

      // (5)
      /// \brief Move-constructs a compressed_tuple from another tuple
      ///
      /// \param other the other tuple to move
      template<typename...UTypes>
      constexpr compressed_tuple( compressed_tuple<UTypes...>&& other );

      // (6)
      /// \brief Copy-constructs a compressed_tuple from a compressed_pair
      ///
      /// \param p the pair to copy
      template<typename U1, typename U2 >
      constexpr compressed_tuple( const compressed_pair<U1,U2>& p );

      // (7)
      /// \brief Move-constructs a compressed_tuple from a compressed_pair
      ///
      /// \param p the pair to move
      template<typename U1, typename U2 >
      constexpr compressed_tuple( compressed_pair<U1,U2>&& p );

      // (8)
      /// \brief Copy-constructs a compressed_tuple from an existing tuple
      ///
      /// \param other the other tuple to copy
      constexpr compressed_tuple( const compressed_tuple& other ) = default;

      // (9)
      /// \brief Move-constructs a compressed_tuple from an existing tuple
      ///
      /// \param other the other tuple to move
      constexpr compressed_tuple( compressed_tuple&& other ) = default;

      // (10)
      /// \brief Piecewise-constructs each of the elements of a compressed_tuple
      ///
      /// \param tuples the tuples to forward for constructing the underlying
      ///               types
      template<typename...Tuples, typename = std::enable_if_t<(sizeof...(Tuples)==sizeof...(Types))>>
      constexpr compressed_tuple( std::piecewise_construct_t, Tuples&&...tuples );

      //-----------------------------------------------------------------------

      /// \brief Copy-assigns a tuple from another one
      ///
      /// \param other the other tuple to copy
      /// \return reference to \c (*this)
      compressed_tuple& operator=( const compressed_tuple& other ) = default;

      /// \brief Move-assigns a tuple from another one
      ///
      /// \param other the other tuple to move
      /// \return reference to \c (*this)
      compressed_tuple& operator=( compressed_tuple&& other ) = default;

      /// \brief Copy-assigns a tuple from another one
      ///
      /// \param other the other tuple to copy
      /// \return reference to \c (*this)
      template<typename... UTypes>
      compressed_tuple& operator=( const compressed_tuple<UTypes...>& other );

      /// \brief Move-assigns a tuple from another one
      ///
      /// \param other the other tuple to move
      /// \return reference to \c (*this)
      template<typename... UTypes>
      compressed_tuple& operator=( compressed_tuple<UTypes...>&& other );

      /// \brief Copy-assigns a compressed_pair to this tuple
      ///
      /// \param p the pair to copy
      /// \return reference to \c (*this)
      template<typename U1, typename U2>
      compressed_tuple& operator=( const compressed_tuple<U1,U2>& p );

      /// \brief Move-assigns a compressed_pair to this tuple
      ///
      /// \param p the other tuple to copy
      /// \return reference to \c (*this)
      template<typename U1, typename U2>
      compressed_tuple& operator=( compressed_tuple<U1,U2>&& p );

      //-----------------------------------------------------------------------
      // Modifiers
      //-----------------------------------------------------------------------
    public:

      /// \brief Swaps this tuple with the \p other tuple
      ///
      /// \param other the other tuple to swap
      constexpr void swap( compressed_tuple& other );

      //-----------------------------------------------------------------------
      // Private Constructors
      //-----------------------------------------------------------------------
    private:

      /// \brief Constructs a compressed_tuple from a tuple
      ///
      /// \param tuple the tuple to construct
      template<typename Tuple,std::size_t...Idxs>
      constexpr compressed_tuple( std::index_sequence<Idxs...>, Tuple&& tuple );

      //-----------------------------------------------------------------------
      // Private Observers
      //-----------------------------------------------------------------------
    private:

      using base_type::do_get;

      //-----------------------------------------------------------------------
      // Private Modifiers
      //-----------------------------------------------------------------------
    private:

      /// \brief Assigns this tuple to each element recursively
      ///
      /// \param tuple the tuple to construct
      template<typename Tuple, std::size_t...Idxs>
      void assign_tuple( std::index_sequence<Idxs...>, Tuple&& tuple );

      //-----------------------------------------------------------------------
      // Friends
      //-----------------------------------------------------------------------
    private:

      template<std::size_t I, typename...UTypes>
      friend constexpr tuple_element_t<I,compressed_tuple<UTypes...>>&
        get( compressed_tuple<UTypes...>& tuple ) noexcept;

      template<std::size_t I, typename...UTypes>
      friend constexpr tuple_element_t<I,compressed_tuple<UTypes...>>&&
        get( compressed_tuple<UTypes...>&& tuple ) noexcept;

      template<std::size_t I, typename...UTypes>
      friend constexpr const tuple_element_t<I,compressed_tuple<UTypes...>>&
        get( const compressed_tuple<UTypes...>& tuple ) noexcept;

      template<std::size_t I, typename...UTypes>
      friend constexpr const tuple_element_t<I,compressed_tuple<UTypes...>>&&
        get( const compressed_tuple<UTypes...>&& tuple ) noexcept;

      template<typename...UTypes>
      friend constexpr bool operator==( const compressed_tuple<UTypes...>& lhs,
                                        const compressed_tuple<UTypes...>& rhs ) noexcept;

      template<typename...UTypes>
      friend constexpr bool operator<( const compressed_tuple<UTypes...>& lhs,
                                       const compressed_tuple<UTypes...>& rhs ) noexcept;
    };

    //=========================================================================
    // non-member functions : class : compressed_tuple
    //=========================================================================

    //-------------------------------------------------------------------------
    // Comparisons
    //-------------------------------------------------------------------------

    template<typename...Types>
    constexpr bool operator==( const compressed_tuple<Types...>& lhs,
                               const compressed_tuple<Types...>& rhs ) noexcept;

    template<typename...Types>
    constexpr bool operator!=( const compressed_tuple<Types...>& lhs,
                               const compressed_tuple<Types...>& rhs ) noexcept;

    template<typename...Types>
    constexpr bool operator<( const compressed_tuple<Types...>& lhs,
                              const compressed_tuple<Types...>& rhs ) noexcept;

    template<typename...Types>
    constexpr bool operator<=( const compressed_tuple<Types...>& lhs,
                               const compressed_tuple<Types...>& rhs ) noexcept;

    template<typename...Types>
    constexpr bool operator>( const compressed_tuple<Types...>& lhs,
                              const compressed_tuple<Types...>& rhs ) noexcept;

    template<typename...Types>
    constexpr bool operator>=( const compressed_tuple<Types...>& lhs,
                               const compressed_tuple<Types...>& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    template<typename...Types>
    constexpr void swap( compressed_tuple<Types...>& lhs,
                         compressed_tuple<Types...>& rhs );

    //-------------------------------------------------------------------------

    template<typename...Args>
    constexpr compressed_tuple<std::decay_t<Args>...> make_compressed_tuple( Args&&...args );

    //-------------------------------------------------------------------------

    template<std::size_t I, typename...UTypes>
    constexpr tuple_element_t<I,compressed_tuple<UTypes...>>&
      get( compressed_tuple<UTypes...>& tuple ) noexcept;

    template<std::size_t I, typename...UTypes>
    constexpr tuple_element_t<I,compressed_tuple<UTypes...>>&&
      get( compressed_tuple<UTypes...>&& tuple ) noexcept;

    template<std::size_t I, typename...UTypes>
    constexpr const tuple_element_t<I,compressed_tuple<UTypes...>>&
      get( const compressed_tuple<UTypes...>& tuple ) noexcept;

    template<std::size_t I, typename...UTypes>
    constexpr const tuple_element_t<I,compressed_tuple<UTypes...>>&&
      get( const compressed_tuple<UTypes...>&& tuple ) noexcept;

    //-------------------------------------------------------------------------
    // Trait extensions
    //-------------------------------------------------------------------------

    template<typename...Ts>
    struct tuple_size<compressed_tuple<Ts...>> : size_constant<sizeof...(Ts)>{};

    template<size_t Idx, typename...Ts>
    struct tuple_element<Idx,compressed_tuple<Ts...>> : nth_type<Idx,Ts...>{};

  } // namespace core
} // namespace bit

#include "detail/compressed_tuple.inl"

#endif /* BIT_CORE_UTILITIES_COMPRESSED_TUPLE_HPP */
