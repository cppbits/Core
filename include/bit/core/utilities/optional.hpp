/*****************************************************************************
 * \file
 * \brief This header contains an implementation of c++17's \c std::optional
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
#ifndef BIT_CORE_UTILITIES_OPTIONAL_HPP
#define BIT_CORE_UTILITIES_OPTIONAL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "hash.hpp"     // hash_t
#include "in_place.hpp" // in_place_t
#include "invoke.hpp"   // invoke_result_t, invoke
#include "assert.hpp"   // BIT_ASSERT

#include "../traits/composition/conjunction.hpp"
#include "../traits/composition/disjunction.hpp"
#include "../traits/composition/negation.hpp"
#include "../traits/composition/sfinae.hpp"

#include <initializer_list> // std::initializer_list
#include <type_traits>      // std::enable_if_t
#include <utility>          // std::forward, std::move
#include <stdexcept>        // std::logic_error
#include <utility>          // std::forward, std::move
#include <new>              // placement new

namespace bit {
  namespace core {

    template<typename> class optional;

    //=========================================================================
    // trait : is_optional
    //=========================================================================

    template<typename T>
    struct is_optional : std::false_type{};

    template<typename T>
    struct is_optional<optional<T>> : std::true_type{};

    template<typename T>
    constexpr bool is_optional_v = is_optional<T>::value;

    //-------------------------------------------------------------------------
    // Optional SFINAE type traits
    //-------------------------------------------------------------------------

    namespace detail {

      template<typename T, typename U>
      using optional_is_convertible = conjunction<
        std::is_constructible<T, optional<U>&>,
        std::is_constructible<T, const optional<U>&>,
        std::is_constructible<T, optional<U>&&>,
        std::is_constructible<T, const optional<U>&&>,
        std::is_convertible<optional<U>&, T>,
        std::is_convertible<const optional<U>&, T>,
        std::is_convertible<optional<U>&&, T>,
        std::is_convertible<const optional<U>&&, T>
      >;

      template<typename T, typename U>
      using optional_is_copy_convertible = conjunction<
        optional_is_convertible<T,U>,
        std::is_constructible<T, const U&>
      >;

      template<typename T, typename U>
      using optional_is_move_convertible = conjunction<
        optional_is_convertible<T,U>,
        std::is_constructible<T, U&&>
      >;

      template<typename T, typename U>
      using optional_is_value_convertible = conjunction<
        std::is_constructible<T, U&&>,
        negation<std::is_same<std::decay_t<U>,in_place_t>>,
        negation<std::is_same<std::decay_t<U>,optional<T>>>
      >;

      template<typename T, typename U>
      using optional_is_convert_assignable = conjunction<
        optional_is_convertible<T,U>,
        std::is_assignable<T&, optional<U>&>,
        std::is_assignable<T&, const optional<U>&>,
        std::is_assignable<T&, optional<U>&&>,
        std::is_assignable<T&, const optional<U>&&>
      >;

      template<typename T, typename U>
      using optional_is_copy_convert_assignable = conjunction<
        optional_is_convert_assignable<T,U>,
        std::is_constructible<T, const U&>,
        std::is_assignable<T&, const U&>
      >;

      template<typename T, typename U>
      using optional_is_move_convert_assignable = conjunction<
        optional_is_convert_assignable<T,U>,
        std::is_constructible<T, U&&>,
        std::is_assignable<T&, U&&>
      >;

      template<typename T, typename U>
      using optional_is_value_assignable = conjunction<
        negation<std::is_same<std::decay_t<U>,optional<T>>>,
        std::is_constructible<T,U>,
        std::is_assignable<T,U>,
        disjunction<
          negation<std::is_same<std::decay_t<U>,T>>,
          negation<std::is_scalar<T>>
        >
      >;

    } // namespace detail

    //=========================================================================
    // class : bad_optional_access
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    /// \brief An exception thrown when an optional is attempted to be accessed
    ///        while not containing a value
    ///////////////////////////////////////////////////////////////////////////
    class bad_optional_access : public std::logic_error
    {
    public:
      bad_optional_access() : std::logic_error("bad_optional_access"){}
    };

    //=========================================================================
    // struct : nullopt_t
    //=========================================================================

    /// \brief This type represents null optional value
    struct nullopt_t
    {
      nullopt_t() = delete;
      constexpr explicit nullopt_t(int) noexcept{}
    };

    //-------------------------------------------------------------------------

    /// \brief Instance of a nullopt type
    constexpr nullopt_t nullopt = nullopt_t{0};

    //=========================================================================
    // class : optional
    //=========================================================================

    namespace detail {

      template<typename T, bool IsTrivial> class optional_base;

      template<typename T>
      class optional_base<T,true>
      {
        //---------------------------------------------------------------------
        // Constructors / Assignment
        //---------------------------------------------------------------------
      public:
        constexpr optional_base( nullopt_t ) noexcept;

        template<typename...Args>
        constexpr optional_base( in_place_t, Args&&...args )
          noexcept(std::is_nothrow_constructible<T,Args...>::value);

        optional_base( optional_base&& other ) = default;
        optional_base( const optional_base& other ) = default;

        //---------------------------------------------------------------------

        optional_base& operator=( optional_base&& other ) = default;
        optional_base& operator=( const optional_base& other ) = default;

        //---------------------------------------------------------------------
        // Protected Modifiers
        //---------------------------------------------------------------------
      protected:

        constexpr T* val() noexcept;
        constexpr const T* val() const noexcept;

        constexpr bool contains_value() const noexcept;

        //---------------------------------------------------------------------
        // Protected Observers
        //---------------------------------------------------------------------
      protected:

        template<typename...Args>
        void construct( Args&&...args );

        void destruct();

        //---------------------------------------------------------------------
        // Private Member Types
        //---------------------------------------------------------------------
      private:

        struct empty{};
        union storage_type {
          empty nothing;
          T     something;

          template<typename...Args>
          constexpr storage_type( in_place_t, Args&&...args )
            : something( std::forward<Args>(args)... ){}
          constexpr storage_type() : nothing(){}
        };

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        storage_type m_storage;
        bool         m_engaged;
      };

      template<typename T>
      class optional_base<T,false>
      {
        //---------------------------------------------------------------------
        // Protected Constructors / Destructor / Assignment
        //---------------------------------------------------------------------
      protected:

        optional_base( nullopt_t ) noexcept;

        template<typename...Args>
        optional_base( in_place_t, Args&&...args )
          noexcept(std::is_nothrow_constructible<T,Args...>::value);

        optional_base( optional_base&& other ) = default;
        optional_base( const optional_base& other ) = default;

        //---------------------------------------------------------------------

        ~optional_base() noexcept( std::is_nothrow_destructible<T>::value );

        //---------------------------------------------------------------------

        optional_base& operator=( optional_base&& other ) = default;
        optional_base& operator=( const optional_base& other ) = default;

        //---------------------------------------------------------------------
        // Protected Observers
        //---------------------------------------------------------------------
      protected:

        T* val() noexcept;
        const T* val() const noexcept;
        bool contains_value() const noexcept;

        //---------------------------------------------------------------------
        // Protected Modifiers
        //---------------------------------------------------------------------
      protected:

        template<typename...Args>
        void construct( Args&&...args );
        void destruct();

        //---------------------------------------------------------------------
        // Private Member Types
        //---------------------------------------------------------------------
      private:

        struct empty{};
        union storage_type {
          empty nothing;
          T     something;

          template<typename...Args>
          storage_type( in_place_t, Args&&...args )
            : something( std::forward<Args>(args)... ){}
          storage_type() : nothing(){}
          ~storage_type(){}
        };

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        storage_type m_storage;
        bool         m_engaged;
      };
    } // namespace detail

    ///////////////////////////////////////////////////////////////////////////
    /// \brief The class template optional manages an optional contained value,
    ///        i.e. a value that may or may not be present.
    ///
    /// A common use case for optional is the return value of a function that
    /// may fail. As opposed to other approaches, such as std::pair<T,bool>,
    /// optional handles expensive to construct objects well and is more
    /// readable, as the intent is expressed explicitly.
    ///
    /// Any instance of optional<T> at any given point in time either contains
    /// a value or does not contain a value.
    ///
    /// If an optional<T> contains a value, the value is guaranteed to be
    /// allocated as part of the optional object footprint, i.e. no dynamic
    /// memory allocation ever takes place. Thus, an optional object models an
    /// object, not a pointer, even though the operator*() and operator->()
    /// are defined.
    ///
    /// When an object of type optional<T> is contextually converted to bool,
    /// the conversion returns true if the object contains a value and false if
    /// it does not contain a value.
    ///
    /// The optional object contains a value in the following conditions:
    /// - The object is initialized with a value of type T
    /// - The object is assigned from another optional that contains a value.
    /// - The object does not contain a value in the following conditions:
    /// - The object is default-initialized.
    /// - The object is initialized with a value of nullopt_t or an optional
    ///   object that does not contain a value.
    /// - The object is assigned from a value of nullopt_t or from an optional
    ///   that does not contain a value
    ///
    /// \tparam T the underlying type
    ///////////////////////////////////////////////////////////////////////////
    template<typename T>
    class optional
      : detail::optional_base<T,std::is_trivially_destructible<T>::value>
    {
      static_assert( !std::is_void<T>::value,
                     "optional<void> is ill-formed");
      static_assert( !std::is_reference<T>::value,
                     "optional<T> is ill-formed");
      static_assert( !std::is_abstract<T>::value,
                     "optional of an abstract-type is ill-formed");

      using base_type = detail::optional_base<T,std::is_trivially_destructible<T>::value>;

      //-----------------------------------------------------------------------
      // Public Member Types
      //-----------------------------------------------------------------------
    public:

      using value_type = T; ///< The underlying type of this Optional

      //-----------------------------------------------------------------------
      // Constructor / Destructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Constructs an optional that does not contain a value
      constexpr optional() noexcept;
      constexpr optional( nullopt_t ) noexcept;
      /// \}

      /// \brief Copy constructs an optional
      ///
      /// If \p other contains a value, initializes the contained value as if
      /// direct-initializing (but not direct-list-initializing) an object of
      /// type T with the expression *other.
      ///
      /// If other does not contain a value, constructs an object that does
      /// not contain a value.
      ///
      /// \note This constructor is defined as deleted if std::is_copy_constructible_v<T> is false
      ///
      /// \param other the optional to copy
      optional( enable_overload_if_t<std::is_copy_constructible<T>::value,const optional&> other );
      optional( disable_overload_if_t<std::is_copy_constructible<T>::value,const optional&> other ) = delete;

      /// \brief Move constructs an optional
      ///
      /// If other contains a value, initializes the contained value as if
      /// direct-initializing (but not direct-list-initializing) an object
      /// of type T with the expression std::move(*other) and does not make
      /// other empty: a moved-from optional still contains a value, but the
      /// value itself is moved from.
      ///
      /// If other does not contain a value, constructs an object that does
      /// not contain a value.
      ///
      /// \note This constructor is defined as deleted if std::is_move_constructible_v<T> is false
      ///
      /// \param other the optional to move
      optional( enable_overload_if_t<std::is_move_constructible<T>::value,optional&&> other );
      optional( disable_overload_if_t<std::is_move_constructible<T>::value,optional&&> other ) = delete;

      /// \{
      /// \brief Converting copy constructor
      ///
      /// If other doesn't contain a value, constructs an optional object
      /// that does not contain a value. Otherwise, constructs an optional
      /// object that contains a value, initialized as if direct-initializing
      /// (but not direct-list-initializing) an object of type T with the
      /// expression *other.
      ///
      /// \note This constructor does not participate in overload resolution
      ///       unless the following conditions are met:
      ///       - std::is_constructible_v<T, const U&> is true
      ///       - T is not constructible or convertible from any expression
      ///         of type (possibly const) optional<U>, i.e., the
      ///         following 8 type traits are all false:
      ///         - std::is_constructible_v<T, std::optional<U>&>
      ///         - std::is_constructible_v<T, const std::optional<U>&>
      ///         - std::is_constructible_v<T, std::optional<U>&&>
      ///         - std::is_constructible_v<T, const std::optional<U>&&>
      ///         - std::is_convertible_v<std::optional<U>&, T>
      ///         - std::is_convertible_v<const std::optional<U>&, T>
      ///         - std::is_convertible_v<std::optional<U>&&, T>
      ///         - std::is_convertible_v<const std::optional<U>&&, T>
      ///
      /// \note This constructor is explicit if and only if
      ///       std::is_convertible_v<const U&, T> is false
      ///
      /// \param other the other type to convert
      template<typename U,
               std::enable_if_t<detail::optional_is_copy_convertible<T,U>::value && std::is_convertible<const U&, T>::value>* =nullptr>
      optional( const optional<U>& other );
      template<typename U,
               std::enable_if_t<detail::optional_is_copy_convertible<T,U>::value && !std::is_convertible<const U&, T>::value>* =nullptr>
      explicit optional( const optional<U>& other );
      /// \}

      /// \{
      /// \brief Converting move constructor
      ///
      /// If other doesn't contain a value, constructs an optional object
      /// that does not contain a value. Otherwise, constructs an optional
      /// object that contains a value, initialized as if direct-initializing
      /// (but not direct-list-initializing) an object of type T with the
      /// expression std::move(*other).
      ///
      /// \note This constructor does not participate in overload resolution
      ///       unless the following conditions are met:
      ///       - std::is_constructible_v<T, U&&> is true
      ///       - T is not constructible or convertible from any expression
      ///         of type (possibly const) optional<U>, i.e., the
      ///         following 8 type traits are all false:
      ///         - std::is_constructible_v<T, std::optional<U>&>
      ///         - std::is_constructible_v<T, const std::optional<U>&>
      ///         - std::is_constructible_v<T, std::optional<U>&&>
      ///         - std::is_constructible_v<T, const std::optional<U>&&>
      ///         - std::is_convertible_v<std::optional<U>&, T>
      ///         - std::is_convertible_v<const std::optional<U>&, T>
      ///         - std::is_convertible_v<std::optional<U>&&, T>
      ///         - std::is_convertible_v<const std::optional<U>&&, T>
      ///
      /// \note This constructor is explicit if and only if
      ///       std::is_convertible_v<U&&, T> is false
      ///
      /// \param other
      template<typename U,
               std::enable_if_t<detail::optional_is_move_convertible<T,U>::value && std::is_convertible<U&&, T>::value>* =nullptr>
      optional( optional<U>&& other );
      template<typename U,
               std::enable_if_t<detail::optional_is_move_convertible<T,U>::value && !std::is_convertible<U&&, T>::value>* =nullptr>
      explicit optional( optional<U>&& other );
      /// \}

      /// \brief Constructs an optional object that contains a value
      ///
      /// the value is initialized as if direct-initializing (but not
      /// direct-list-initializing) an object of type T from the arguments
      /// std::forward<Args>(args)...
      ///
      /// \param tag     The in_place tag
      /// \param args... the arguments to pass to T's constructor
      template<typename...Args,
               typename=std::enable_if_t<std::is_constructible<T,Args...>::value>>
      constexpr explicit optional( in_place_t tag, Args&&... args );

      /// \brief Constructs an optional object that contains a value
      ///
      /// The value is initialized as if direct-initializing (but not
      /// direct-list-initializing) an object of type T from the arguments
      /// std::forward<std::initializer_list<U>>(ilist), std::forward<Args>(args)...
      ///
      /// \param tag     The in_place tag
      /// \param ilist   An initializer list of entries to forward
      /// \param args... the arguments to pass to T's constructor
      template<typename U, typename...Args,
               typename=std::enable_if_t<std::is_constructible<T, std::initializer_list<U>&, Args...>::value>>
      constexpr explicit optional( in_place_t,
                                   std::initializer_list<U> ilist,
                                   Args&&...args );

      /// \{
      /// \brief Constructs an optional object that contains a value
      ///
      /// The value is initialized as if direct-initializing (but not
      /// direct-list-initializing) an object of type T with the expression
      /// value.
      ///
      /// \note This constructor is constexpr if the constructor of T
      ///       selected by direct-initialization is constexpr
      ///
      /// \note This constructor does not participate in overload
      ///       resolution unless std::is_constructible_v<T, U&&> is true
      ///       and std::decay_t<U> is neither std::in_place_t nor
      ///       optional<T>.
      ///
      /// \note This constructor is explicit if and only if
      ///       std::is_convertible_v<U&&, T> is false
      ///
      /// \param value the value to copy
      template<typename U=T,
               std::enable_if_t<detail::optional_is_value_convertible<T,U>::value && std::is_convertible<U&&, T>::value>* =nullptr>
      constexpr optional( U&& value );
      template<typename U=T,
               std::enable_if_t<detail::optional_is_value_convertible<T,U>::value && !std::is_convertible<U&&, T>::value>* =nullptr>
      explicit constexpr optional( U&& value );
      /// \}

      //-----------------------------------------------------------------------

      /// \brief Assigns this optional to be a null optional
      ///
      /// If \c (*this) had a value before the call, the contained value is
      /// destroyed.
      optional& operator=( nullopt_t );

      /// \brief Copy assigns the optional stored in \p other
      ///
      /// \param other the other optional to copy
      optional& operator=( enable_overload_if_t<std::is_copy_constructible<T>::value && std::is_copy_assignable<T>::value,const optional&> other );
      optional& operator=( disable_overload_if_t<std::is_copy_constructible<T>::value && std::is_copy_assignable<T>::value,const optional&> other ) = delete;

      /// \brief Move assigns the optional stored in \p other
      ///
      /// \note If \p other contained a value before the call, it is left as a
      ///       null optional
      ///
      /// \remark This assignment does not participate in overload resolution
      ///         unless U is
      ///
      /// \param other the other optional to move
      optional& operator=( enable_overload_if_t<std::is_move_constructible<T>::value && std::is_move_assignable<T>::value,optional&&> other );
      optional& operator=( disable_overload_if_t<std::is_move_constructible<T>::value && std::is_move_assignable<T>::value,optional&&> other ) = delete;

      /// \brief Perfect-forwarded assignment
      ///
      /// Depending on whether *this contains a value before the call, the
      /// contained value is either direct-initialized from std::forward<U>(value)
      /// or assigned from std::forward<U>(value).
      ///
      /// \note The function does not participate in overload resolution unless
      ///       - std::decay_t<U> is not optional<T>,
      ///       - std::is_constructible_v<T, U> is true
      ///       - std::is_assignable_v<T&, U> is true
      ///       - and at least one of the following is true:
      ///           - T is not a scalar type;
      ///           - std::decay_t<U> is not T.
      ///
      /// \param value to assign to the contained value
      /// \return reference to \c (*this)
      template<typename U=T,
               typename=std::enable_if_t<detail::optional_is_value_assignable<T,U>::value>>
      optional& operator=( U&& value );

      /// \brief Assigns the state of \p other
      ///
      /// If both *this and other do not contain a value, the function has no effect.
      ///
      /// If *this contains a value, but other does not, then the contained
      /// value is destroyed by calling its destructor. *this does not
      /// contain a value after the call.
      ///
      /// If other contains a value, then depending on whether *this contains a value,
      /// the contained value is either direct-initialized or assigned from *other.
      ///
      /// \note A moved-from optional still contains a value.
      ///
      /// \note This overload does not participate in overload resolution unless the
      ///       following conditions are met:
      ///       - std::is_constructible_v<T, const U&> and
      ///         std::is_assignable_v<T&, const U&> are both true.
      ///       - T is not constructible, convertible, or assignable from any expression
      ///         of type (possibly const) :optional<U>, i.e.,
      ///         the following 12 type traits are all false:
      ///         - std::is_constructible_v<T, std::optional<U>&>
      ///         - std::is_constructible_v<T, const std::optional<U>&>
      ///         - std::is_constructible_v<T, std::optional<U>&&>
      ///         - std::is_constructible_v<T, const std::optional<U>&&>
      ///         - std::is_convertible_v<std::optional<U>&, T>
      ///         - std::is_convertible_v<const std::optional<U>&, T>
      ///         - std::is_convertible_v<std::optional<U>&&, T>
      ///         - std::is_convertible_v<const std::optional<U>&&, T>
      ///         - std::is_assignable_v<T&, std::optional<U>&>
      ///         - std::is_assignable_v<T&, const std::optional<U>&>
      ///         - std::is_assignable_v<T&, std::optional<U>&&>
      ///         - std::is_assignable_v<T&, const std::optional<U>&&>.
      ///
      /// \param other another optional object whose contained value to assign
      /// \return reference to (*this)
      template<typename U>
      optional& operator=( std::enable_if_t<detail::optional_is_copy_convert_assignable<T,U>::value,const optional<U>&> other );

      /// \brief Assigns the state of \p other
      ///
      /// If both *this and other do not contain a value, the function has no effect.
      ///
      /// If *this contains a value, but other does not, then the contained
      /// value is destroyed by calling its destructor. *this does not
      /// contain a value after the call.
      ///
      /// If other contains a value, then depending on whether *this contains a value,
      /// the contained value is either direct-initialized or assigned from
      /// std::move(*other).
      ///
      /// \note A moved-from optional still contains a value.
      ///
      /// \note This overload does not participate in overload resolution unless the
      ///       following conditions are met:
      ///       - std::is_constructible_v<T, U&&> and
      ///         std::is_assignable_v<T&, U&&> are both true.
      ///       - T is not constructible, convertible, or assignable from any expression
      ///         of type (possibly const) :optional<U>, i.e.,
      ///         the following 12 type traits are all false:
      ///         - std::is_constructible_v<T, std::optional<U>&>
      ///         - std::is_constructible_v<T, const std::optional<U>&>
      ///         - std::is_constructible_v<T, std::optional<U>&&>
      ///         - std::is_constructible_v<T, const std::optional<U>&&>
      ///         - std::is_convertible_v<std::optional<U>&, T>
      ///         - std::is_convertible_v<const std::optional<U>&, T>
      ///         - std::is_convertible_v<std::optional<U>&&, T>
      ///         - std::is_convertible_v<const std::optional<U>&&, T>
      ///         - std::is_assignable_v<T&, std::optional<U>&>
      ///         - std::is_assignable_v<T&, const std::optional<U>&>
      ///         - std::is_assignable_v<T&, std::optional<U>&&>
      ///         - std::is_assignable_v<T&, const std::optional<U>&&>.
      ///
      /// \param other another optional object whose contained value to assign
      /// \return reference to (*this)
      template<typename U>
      optional& operator=( std::enable_if_t<detail::optional_is_move_convert_assignable<T,U>::value,optional<U>&&> other );

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------

      /// \{
      /// \brief Accesses the contained value
      ///
      /// \note The behavior is undefined if \c *this does not contain a value.
      ///
      /// \return a pointer to the contained value
      constexpr value_type* operator->() noexcept;
      constexpr value_type const* operator->() const noexcept;
      /// \}

      /// \{
      /// \brief Accesses the contained value
      ///
      /// \note The behaviour is undefined if \c *this does not contain a value
      ///
      /// \return a reference to the contained value
      constexpr value_type& operator*() & noexcept;
      constexpr value_type&& operator*() && noexcept;
      constexpr value_type const& operator*() const& noexcept;
      constexpr value_type const&& operator*() const&& noexcept;
      /// \}

      /// \brief Checks whether \c *this contains a value
      ///
      /// \return \c true if \c *this contains a value, \c false if \c *this
      ///         does not contain a value
      constexpr explicit operator bool() const noexcept;

      /// \brief Checks whether \c *this contains a value
      ///
      /// \return \c true if \c *this contains a value, \c false if \c *this
      ///         does not contain a value
      constexpr bool has_value() const noexcept;

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Returns the contained value.
      ///
      /// Equivalent to
      /// \code
      /// return bool(*this) ? *val : throw bad_optional_access();
      /// \endcode
      ///
      /// \throws bad_optional_access if \c *this does not contain a value.
      ///
      /// \return the value of \c *this
      constexpr value_type& value() &;
      constexpr value_type const& value() const &;
      /// \}

      /// \{
      /// \brief Returns the contained value.
      ///
      /// Equivalent to
      /// \code
      /// return bool(*this) ? std::move(*val) : throw bad_optional_access();
      /// \endcode
      ///
      /// \throws bad_optional_access if \c *this does not contain a value.
      ///
      /// \return the value of \c *this
      constexpr value_type&& value() &&;
      constexpr const value_type&& value() const &&;
      /// \}

      //-----------------------------------------------------------------------

      /// \{
      /// \brief Returns the contained value if \c *this has a value,
      ///        otherwise returns \p default_value.
      ///
      /// \param default_value the value to use in case \c *this is empty
      /// \return the value to use in case \c *this is empty
      template<typename U>
      constexpr value_type value_or( U&& default_value ) const &;
      template<typename U>
      constexpr value_type value_or( U&& default_value ) &&;
      /// \}

      //------------------------------------------------------------------------
      // Monadic Functionality
      //------------------------------------------------------------------------
    public:

      /// \brief Invokes the function \p fn with this optional as the argument
      ///
      /// If this optional is nullopt, this function returns nullopt
      /// \param fn the function to invoke with this
      /// \return The result of the function being called
      template<typename Fn,
               typename=std::enable_if_t<conjunction<is_invocable<Fn,const T&>,
                                         is_optional<invoke_result_t<Fn,const T&>>>::value>>
      invoke_result_t<Fn,const T&> flat_map( Fn&& fn ) const;

      /// \brief Invokes this function \p fn with this optional as the argument
      ///
      /// If this optional is nullopt, the result of this function is nullopt.
      /// Otherwise this function wraps the result and returns it as an optional
      ///
      /// \param fn the function to invoke with this
      /// \return The optional result of the function invoked
      template<typename Fn,
               typename=std::enable_if_t<is_invocable<Fn,const T&>::value>>
      optional<invoke_result_t<Fn,const T&>> map( Fn&& fn ) const;

      //------------------------------------------------------------------------

      /// \brief Returns \p u if this optional contains a value, nullopt
      ///        otherwise.
      ///
      /// \param U optional
      /// \param u the value to return as an optional
      /// \return an optional of \p u if this contains a value
      template<typename U>
      optional<std::decay_t<U>> and_then( U&& u ) const;

      /// \brief Returns \p u if this optional does not contain a value, nullopt
      ///        otherwise.
      ///
      /// \param u the value to return as an optional
      /// \return an optional of \p u if this does not contains a value
      template<typename U>
      optional<std::decay_t<U>> or_else( U&& u ) const;

      //-----------------------------------------------------------------------
      // Modifiers
      //-----------------------------------------------------------------------

      /// \brief Swaps the contents with those of other.
      ///
      /// \param other the optional object to exchange the contents with
      void swap( optional& other );

      /// \brief If \c *this contains a value, destroy that value as if by
      ///        \c value().T::~T().
      ///
      /// Otherwise, there are no effects. \c *this does not contain a value
      /// after this call.
      void reset() noexcept(std::is_nothrow_destructible<T>::value);

      /// \brief Constructs the contained value in-place.
      ///
      /// If \c *this already contains a value before the call, the contained
      /// value is destroyed by calling its destructor.
      ///
      /// \param args... the arguments to pass to the constructor
      template<typename...Args>
      void emplace( Args&&...args );

      /// \brief Constructs the contained value in-place.
      ///
      /// If \c *this already contains a value before the call, the contained
      /// value is destroyed by calling its destructor.
      ///
      /// \param ilist   the initializer list to pass to the constructor
      /// \param args... the arguments to pass to the constructor
      template<typename U,typename...Args >
      void emplace( std::initializer_list<U> ilist, Args&&...args );
    };

    //=========================================================================
    // non-member functions : class : optional
    //=========================================================================

    //-------------------------------------------------------------------------
    // Comparison
    //-------------------------------------------------------------------------

    template<typename T>
    constexpr bool operator==( const optional<T>& lhs, const optional<T>& rhs );
    template<typename T>
    constexpr bool operator!=( const optional<T>& lhs, const optional<T>& rhs );
    template<typename T>
    constexpr bool operator<( const optional<T>& lhs, const optional<T>& rhs );
    template<typename T>
    constexpr bool operator>( const optional<T>& lhs, const optional<T>& rhs );
    template<typename T>
    constexpr bool operator<=( const optional<T>& lhs, const optional<T>& rhs );
    template<typename T>
    constexpr bool operator>=( const optional<T>& lhs, const optional<T>& rhs );

    //-------------------------------------------------------------------------

    template<typename T>
    constexpr bool operator==( const optional<T>& opt, nullopt_t ) noexcept;
    template<typename T>
    constexpr bool operator==( nullopt_t, const optional<T>& opt ) noexcept;
    template<typename T>
    constexpr bool operator!=( const optional<T>& opt, nullopt_t ) noexcept;
    template<typename T>
    constexpr bool operator!=( nullopt_t, const optional<T>& opt ) noexcept;
    template<typename T>
    constexpr bool operator<( const optional<T>& opt, nullopt_t ) noexcept;
    template<typename T>
    constexpr bool operator<( nullopt_t, const optional<T>& opt ) noexcept;
    template<typename T>
    constexpr bool operator>( const optional<T>& opt, nullopt_t ) noexcept;
    template<typename T>
    constexpr bool operator>( nullopt_t, const optional<T>& opt ) noexcept;
    template<typename T>
    constexpr bool operator<=( const optional<T>& opt, nullopt_t ) noexcept;
    template<typename T>
    constexpr bool operator<=( nullopt_t, const optional<T>& opt ) noexcept;
    template<typename T>
    constexpr bool operator>=( const optional<T>& opt, nullopt_t ) noexcept;
    template<typename T>
    constexpr bool operator>=( nullopt_t, const optional<T>& opt ) noexcept;

    //-------------------------------------------------------------------------

    template<typename T>
    constexpr bool operator==( const optional<T>& opt, const T& value );
    template<typename T>
    constexpr bool operator==( const T& value, const optional<T>& opt );
    template<typename T>
    constexpr bool operator!=( const optional<T>& opt, const T& value );
    template<typename T>
    constexpr bool operator!=( const T& value, const optional<T>& opt );
    template<typename T>
    constexpr bool operator<( const optional<T>& opt, const T& value );
    template<typename T>
    constexpr bool operator<( const T& value, const optional<T>& opt );
    template<typename T>
    constexpr bool operator>( const optional<T>& opt, const T& value );
    template<typename T>
    constexpr bool operator>( const T& value, const optional<T>& opt );
    template<typename T>
    constexpr bool operator<=( const optional<T>& opt, const T& value );
    template<typename T>
    constexpr bool operator<=( const T& value, const optional<T>& opt );
    template<typename T>
    constexpr bool operator>=( const optional<T>& opt, const T& value );
    template<typename T>
    constexpr bool operator>=( const T& value, const optional<T>& opt );

    //-------------------------------------------------------------------------
    // Utilities
    //-------------------------------------------------------------------------

    /// \brief Creates an optional object from \p value
    ///
    /// \param value the value to construct the optional from
    template<typename T>
    constexpr optional<std::decay_t<T>> make_optional( T&& value );

    /// \brief Creates an optional object of type \p T from the given \p args
    ///
    /// \tparam T the type to construct
    /// \param args... the arguments to pass to \p T's constructor
    template<typename T, typename... Args >
    constexpr optional<T> make_optional( Args&&... args );

    /// \brief Creates an optional object of type \p T from the given
    ///        \p il \p args
    ///
    /// \tparam T the type to construct
    /// \param il      the initializer list to pass
    /// \param args... the arguments to pass to \p T's constructor
    template<typename T, typename U, typename... Args >
    constexpr optional<T> make_optional( std::initializer_list<U> il, Args&&... args );

    //-------------------------------------------------------------------------

    /// \brief Swaps \p lhs and \p rhs
    ///
    /// \param lhs the left optional to swap
    /// \param rhs the right optional to swap
    template<typename T>
    void swap( optional<T>& lhs, optional<T>& rhs );

    //-------------------------------------------------------------------------

    /// \brief Retrieves the hash from a given optional
    ///
    /// \param str the optional to retrieve the hash from
    /// \return the hash of the string
    template<typename T>
    constexpr hash_t hash_value( const optional<T>& s ) noexcept;

  } // namespace core
} // namespace bit

#include "detail/optional.inl"

#endif /* BIT_CORE_UTILITIES_OPTIONAL_HPP */
