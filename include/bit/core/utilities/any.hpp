/*****************************************************************************
 * \file any.hpp
 * \brief todo: fill in documentation
 *****************************************************************************/

/*
  The MIT License (MIT)

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
#ifndef BIT_CORE_UTILITIES_ANY_HPP
#define BIT_CORE_UTILITIES_ANY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../traits/composition/bool_constant.hpp"

#include "in_place.hpp"        // in_place
#include "aligned_storage.hpp" // aligned_storage
#include "assert.hpp"          // BIT_ASSERT
#include "compiler_traits.hpp" // BIT_UNUSED

#include <initializer_list> // std::initializer_list
#include <typeinfo>         // std::bad_cast
#include <type_traits>      // std::decay
#include <utility>          // std::forward
#include <cassert>


namespace bit {
  namespace core {

    class any;

    //=========================================================================
    // class : bad_any_cast
    //=========================================================================

    class bad_any_cast : public std::bad_cast
    {
      const char* what() const noexcept override;
    };

    //=========================================================================
    // class : any
    //=========================================================================

    // NOTE: These forward declarations are so that 'any' can friend this later
    class any;
    inline namespace casts {
      template<typename T>
      T* any_cast( any* ) noexcept;
      template<typename T>
      const T* any_cast( const any* ) noexcept;
    } // inline namespace casts

    class any
    {
      //-----------------------------------------------------------------------
      // Constructors / Destructor / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Constructs an any instance that does not contain any value
      any() noexcept;

      /// \brief Moves an any instance by moving the stored underlying value
      ///
      /// \post \p other is left valueless
      ///
      /// \param other the other instance to move
      any( any&& other ) noexcept;

      /// \brief Copies an any instance by copying the stored underlying value
      ///
      /// \param other the other instance to copy
      any( const any& other );

      /// \brief Constructs this any using \p value for the underlying instance
      ///
      /// \tparam ValueType
      /// \param value
      template<typename ValueType,
               typename=std::enable_if_t<!std::is_same<std::decay_t<ValueType>,any>::value &&
                                          std::is_copy_constructible<std::decay_t<ValueType>>::value>>
      any( ValueType&& value );

      template<typename ValueType, typename...Args,
               typename=std::enable_if_t<std::is_constructible<std::decay_t<ValueType>,Args...>::value &&
                                         std::is_copy_constructible<std::decay_t<ValueType>>::value>>
      explicit any( in_place_type_t<ValueType>, Args&&...args );

      template<typename ValueType, typename U, typename...Args,
               typename=std::enable_if_t<std::is_constructible<std::decay_t<ValueType>,std::initializer_list<U>,Args...>::value &&
                                         std::is_copy_constructible<std::decay_t<ValueType>>::value>>
      explicit any( in_place_type_t<ValueType>,
                    std::initializer_list<U> il,
                    Args&&...args );

      //-----------------------------------------------------------------------

      ~any();

      //-----------------------------------------------------------------------

      /// \brief Assigns the contents of \p other to this any
      ///
      /// \param other the other any to move
      /// \return reference to \c (*this)
      any& operator=( any&& other ) noexcept;

      /// \brief Assigns the contents of \p other to this any
      ///
      /// \param other the other any to copy
      /// \return reference to \c (*this)
      any& operator=( const any& other );

      /// \brief Assigns \p value to this any
      ///
      /// \param value the value to assign
      /// \return reference to \c (*this)
      template<typename ValueType,
               typename=std::enable_if_t<!std::is_same<std::decay_t<ValueType>,any>::value &&
                                          std::is_copy_constructible<std::decay_t<ValueType>>::value>>
      any& operator=( ValueType&& value );

      //-----------------------------------------------------------------------
      // Modifiers
      //-----------------------------------------------------------------------
    public:

      /// \{
      /// \brief Emplaces a \c ValueType into this any, destroying the previous
      ///        value if it contained one
      ///
      /// \tparam ValueType the type to construct
      /// \param args the arguments to forward to \c ValueType's constructor
      /// \return reference to the constructed value
      template<typename ValueType, typename...Args,
               typename=std::enable_if_t<std::is_constructible<std::decay_t<ValueType>,Args...>::value &&
                                         std::is_copy_constructible<std::decay_t<ValueType>>::value>>
      std::decay_t<ValueType>& emplace( Args&&...args );
      template<typename ValueType, typename U, typename...Args,
               typename=std::enable_if_t<std::is_constructible<std::decay_t<ValueType>,std::initializer_list<U>,Args...>::value &&
                                         std::is_copy_constructible<std::decay_t<ValueType>>::value>>
      std::decay_t<ValueType>& emplace( std::initializer_list<U> il,
                                        Args&&...args );
      /// \}

      /// \brief Destroys the underlying stored value, leaving this any
      ///        empty.
      void reset() noexcept;

      /// \brief Swaps the contents of \c this with \p other
      ///
      /// \post \p other contains the old contents of \c this, and \c this
      ///       contains the old contents of \p other
      ///
      /// \param other the other any to swap contents with
      void swap(any& other) noexcept;

      //-----------------------------------------------------------------------
      // Observers
      //-----------------------------------------------------------------------
    public:

      /// \brief Checks whether this any contains a value
      ///
      /// \return \c true if this contains a value
      bool has_value() const noexcept;

      /// \brief Gets the type_info for the underlying stored type, or
      ///        \c typeid(void) if \ref has_value() returns \c false
      ///
      /// \return the typeid of the stored type
      const std::type_info& type() const noexcept;

      //-----------------------------------------------------------------------
      // Private Static Members / Types
      //-----------------------------------------------------------------------
    private:

      // Internal buffer size + alignment
      static constexpr auto buffer_size  = 4u * sizeof(void*);
      static constexpr auto buffer_align = alignof(void*);

      // buffer (for internal storage)
      using internal_buffer = aligned_storage_t<buffer_size,buffer_align>;

      union storage
      {
        internal_buffer internal;
        void*           external;
      };

      //-----------------------------------------------------------------------

      // trait to determine if internal storage is required
      template<typename T>
      using requires_internal_storage = bool_constant<
        (sizeof(T) <= buffer_size) &&
        ((buffer_align % alignof(T)) == 0) &&
        std::is_nothrow_move_constructible<T>::value
      >;

      //-----------------------------------------------------------------------

      template<typename T>
      struct internal_storage_handler;

      template<typename T>
      struct external_storage_handler;

      template<typename T>
      using storage_handler = typename std::conditional<
        requires_internal_storage<T>::value,
        internal_storage_handler<T>,
        external_storage_handler<T>
      >::type;

      //-----------------------------------------------------------------------

      enum class operation
      {
        destroy, ///< Operation for calling the underlying's destructor
        copy,    ///< Operation for copying the underlying value
        move,    ///< Operation for moving the underlying value
        value,   ///< Operation for accessing the underlying value
        type,    ///< Operation for accessing the underlying type
      };

      //-----------------------------------------------------------------------

      using storage_handler_ptr = const void*(*)(operation, const storage*,const storage*);

      template<typename T>
      friend T* casts::any_cast( any* ) noexcept;
      template<typename T>
      friend const T* casts::any_cast( const any* ) noexcept;

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      storage             m_storage;
      storage_handler_ptr m_storage_handler;
    };

    //=========================================================================
    // non-member functions : class : any
    //=========================================================================

    //-------------------------------------------------------------------------
    // utilities
    //-------------------------------------------------------------------------

    /// \brief Swaps the contents of \p lhs and \p rhs
    void swap( any& lhs, any& rhs ) noexcept;

    //-------------------------------------------------------------------------
    // casts
    //-------------------------------------------------------------------------

    inline namespace casts {

      template<typename T>
      T any_cast( any& operand );
      template<typename T>
      T any_cast( any&& operand );
      template<typename T>
      T any_cast( const any& operand );

      template<typename T>
      T* any_cast( any* operand ) noexcept;
      template<typename T>
      const T* any_cast( const any* operand ) noexcept;

    } // inline namespace casts

  } // namespace core
} // namespace bit

#include "detail/any.inl"

#endif /* BIT_CORE_UTILITIES_ANY_HPP */
