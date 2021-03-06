/*****************************************************************************
 * \file
 * \brief This file contains the definition for delegates
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
#ifndef BIT_CORE_UTILITIES_DELEGATE_HPP
#define BIT_CORE_UTILITIES_DELEGATE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "assert.hpp"
#include "hash.hpp"   // hash_t
#include "invoke.hpp" // is_invocable
#include "types.hpp"  // function_t, member_function_t

#include <type_traits> // std::enable_if
#include <utility>     // std::forward, std::move, std::pair
#include <memory>      // std::addressof

namespace bit {
  namespace core {

    //=========================================================================
    // class : delegate
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief This class is for a lightweight way of managing function
    ///        callbacks without requiring heap allocations.
    ///
    /// The syntax for binding functions requires statically specifying the
    /// function address and, in the case of member functions, the type of the
    /// class. This is due to a limitation in the (pre-c++14) deduction system.
    ///
    /// \note Since delegates deal with statically typed member function
    ///       pointers, they cannot be used with lambdas. The
    ///       pointer-to-function must be able to be retrieved statically
    ///       (e.g. at constexpr time), which is something not possible until
    ///       C++17 with constexpr lambdas.
    ///       Additionally, since delegates are statically-bound, they
    ///       cannot bind to functors, since that requires additional storage
    ///       that is otherwise not possible.
    ///
    /// Example
    /// \code
    /// Delegate<void()> delegate;
    /// T       t;
    /// const T ct;
    ///
    /// delegate.bind<&foo>();           // bind function
    /// delegate.bind<&T::static_foo>(); // bind static member function
    /// delegate.bind<T,&T::foo>( &t );  // bind member functions
    /// delegate.bind<T,&T::foo>( &ct ); // bind const member function
    /// \endcode
    ///
    /// \tparam Fn signature and return type
    ///////////////////////////////////////////////////////////////////////////
    template<typename Fn> class delegate;

    template<typename R, typename...Types>
    class delegate<R(Types...)> final
    {
      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using return_type = R;

      //-----------------------------------------------------------------------
      // Constructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs an unbound delegate
      constexpr delegate() noexcept;

      /// \brief Move-constructs a delegate from an existing delegate
      ///
      /// \param other the other delegate to move
      constexpr delegate( delegate&& other ) noexcept = default;

      /// \brief Copy-constructs a delegate from an existing delegate
      ///
      /// \param other the other delegate to copy
      constexpr delegate( const delegate& other ) noexcept = default;

      //-----------------------------------------------------------------------

      /// \brief Move-assigns a delegate from an existing delegate
      ///
      /// \param other the other delegate to move
      /// \return reference to \c (*this)
      delegate& operator=( delegate&& other ) noexcept = default;

      /// \brief Copy-assigns a delgate from an existing delegate
      ///
      /// \param other the other delegate to move
      /// \return reference to \c (*this)
      delegate& operator=( const delegate& other ) = default;

      //-----------------------------------------------------------------------
      // Function Binding
      //-----------------------------------------------------------------------
    public:

      /// \brief Binds a function pointer to this delegate
      ///
      /// \tparam function the free-function type to bind
      /// \tparam function the function pointer for the delegate
      template<function_pointer<R(Types...)> Function>
      constexpr void bind() noexcept;

      /// \{
      /// \brief Binds a member function pointer to this delegate
      ///
      /// \tparam C The type of instance to bind
      /// \tparam MemberFunction The pointer to member function to bind
      /// \param instance the instance to call the member function on
      template <class C, member_function_pointer<C,R(Types...)> MemberFunction>
      constexpr void bind( C& instance ) noexcept;
      template <class C, member_function_pointer<const C,R(Types...)> MemberFunction>
      constexpr void bind( const C& instance ) noexcept;
      /// \}

      /// \brief Binds a const member function pointer to this delegate
      ///
      /// This overload exists to disambiguate 'bind' without specifying the
      /// CV qualifiers.
      ///
      /// \tparam C the type of instance to bind
      /// \tparam MemberFunction The pointer to member function to bind
      /// \param instance the instance to call the member function on
      template <class C, member_function_pointer<const C,R(Types...)> MemberFunction>
      constexpr void cbind( const C& instance ) noexcept;

      //-----------------------------------------------------------------------
      // Queries
      //-----------------------------------------------------------------------
    public:

      /// \brief Is this delegate bound?
      ///
      /// \return \c true if the delegate is bound
      constexpr bool is_bound() const noexcept;

      /// \brief Returns true if this delegate is bound
      constexpr explicit operator bool() const noexcept;

      //-----------------------------------------------------------------------
      // Invocation
      //-----------------------------------------------------------------------
    public:

      /// \brief Invokes the bound function
      ///
      /// \note It is undefined behavior to invoke this function without
      ///       binding the delegate first
      ///
      /// \param args the arguments for the invokation
      /// \return the return value for the invoked delegate
      /// \copydoc delegate::invoke( Args&&... )
      template<typename...Args,
               typename=std::enable_if_t<is_invocable<R(*)(Types...),Args...>::value>>
      constexpr return_type operator()( Args&&...args ) const;

      //-----------------------------------------------------------------------
      // Private Member Types
      //-----------------------------------------------------------------------
    private:

      using function_type = return_type(*)(void*, Types...);

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      void*         m_instance; ///< instance pointer (for member functions)
      function_type m_function; ///< function pointer

      template<typename Fn>
      friend bool operator==(const delegate<Fn>& lhs, const delegate<Fn>& rhs) noexcept;

      template<typename Fn>
      friend bool operator<(const delegate& lhs, const delegate& rhs) noexcept;

      template<typename Fn>
      friend hash_t hash_value( const delegate<Fn>& val ) noexcept;
    };

    //=========================================================================
    // non-member functions : class : delegate
    //=========================================================================

    //-------------------------------------------------------------------------
    // Comparison
    //-------------------------------------------------------------------------

    template<typename Fn>
    bool operator==(const delegate<Fn>& lhs, const delegate<Fn>& rhs) noexcept;
    template<typename Fn>
    bool operator!=(const delegate<Fn>& lhs, const delegate<Fn>& rhs) noexcept;
    template<typename Fn>
    bool operator<(const delegate<Fn>& lhs, const delegate<Fn>& rhs) noexcept;
    template<typename Fn>
    bool operator>(const delegate<Fn>& lhs, const delegate<Fn>& rhs) noexcept;
    template<typename Fn>
    bool operator<=(const delegate<Fn>& lhs, const delegate<Fn>& rhs) noexcept;
    template<typename Fn>
    bool operator>=(const delegate<Fn>& lhs, const delegate<Fn>& rhs) noexcept;

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Hashes the delegate \p fn
    ///
    /// \param fn the delegate to hash
    /// \return the hash result
    template<typename Fn>
    hash_t hash_value( const delegate<Fn>& fn ) noexcept;

  } // namespace core
}  // namespace bit

#include "detail/delegate.inl"

#endif /* BIT_CORE_UTILITIES_DELEGATE_HPP */
