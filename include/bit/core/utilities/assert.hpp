/*****************************************************************************
 * \file
 * \brief This header contains the definitions for both static and
 *        runtime assertions.
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
#ifndef BIT_CORE_UTILITIES_ASSERT_HPP
#define BIT_CORE_UTILITIES_ASSERT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "source_location.hpp" // source_location
#include "compiler_traits.hpp" // BIT_DEBUG, BIT_COMPILER_EXCEPTIONS_ENABLED
#include "macros.hpp"          // BIT_STRINGIZE

#include <cstdio>    // std::printf, stderr
#include <exception> // std::terminate

//=============================================================================
// Assertion Macros
//=============================================================================

// Assertions default to enabled in debug mode unless otherwise specified
#ifndef BIT_CORE_ASSERTIONS_ENABLED
# ifdef BIT_DEBUG
#   define BIT_CORE_ASSERTIONS_ENABLED 1
# endif
#endif

#ifdef BIT_ALWAYS_ASSERT
# error duplicate definition of BIT_ALWAYS_ASSERT
#endif

#ifdef BIT_ASSERT
# error duplicate definition of BIT_ASSERT
#endif

#ifdef BIT_INTERNAL_ASSERT_1
# error duplicate definition of BIT_INTERNAL_ASSERT_1
#endif

#ifdef BIT_INTERNAL_ASSERT_2
# error duplicate definition of BIT_INTERNAL_ASSERT_2
#endif

#define BIT_INTERNAL_ASSERT_1(condition) \
  ((BIT_LIKELY(condition)) \
    ? ((void)0) \
    : []( ::bit::core::source_location source ) \
      { \
        ::bit::core::detail::assert_internal( \
          "assertion failure: condition '" \
          BIT_STRINGIZE(condition) \
          "' failed.", \
          source \
        ); \
      }( BIT_MAKE_SOURCE_LOCATION() ) )

#define BIT_INTERNAL_ASSERT_2(condition,message) \
  ((BIT_LIKELY(condition)) \
    ? ((void)0) \
    : []( ::bit::core::source_location source ) \
      { \
        ::bit::core::detail::assert_internal( \
          "assertion failure: condition '" \
          BIT_STRINGIZE(condition) \
          "' failed with message \"" \
          message "\"", \
          source \
        ); \
      }( BIT_MAKE_SOURCE_LOCATION() ) )

//! \def BIT_ASSERT(condition, message)
//!
//! \brief A runtime assertion when \a condition fails, displaying \a message
//!        to the user.
//!
//! An assertion will report the error, the source location of the error,
//! and trigger a breakpoint (if the debugger is open). After the breakpoint
//! trigger, it invokes std::terminate.
//!
//! \note This assertion is always enabled, regardless of the state of
//!       BIT_CORE_ASSERTIONS_ENABLED
//!
//! \param condition the condition that, when false, triggers an assertion
//! \param message   the message for the failure [optional]
#define BIT_ALWAYS_ASSERT(...) \
  BIT_JOIN( BIT_INTERNAL_ASSERT_, BIT_COUNT_VA_ARGS(__VA_ARGS__) )(__VA_ARGS__)

//! \def BIT_ASSERT(condition, message, ...)
//!
//! \brief A runtime assertion when \a condition fails, displaying \a message
//!        to the user.
//!
//! This simply calls 'BIT_ALWAYS_ASSERT' when assertions are enabled
//!
//! \param condition the condition that, when false, triggers an assertion
//! \param message   the message for the failure [optional]
#if BIT_CORE_ASSERTIONS_ENABLED
# define BIT_ASSERT(...) BIT_ALWAYS_ASSERT(__VA_ARGS__)
#else
# define BIT_ASSERT(...) ((void)0)
#endif

//! \def BIT_ASSERT_OR_THROW(condition, exception, message)
//!
//! \brief An assertion that either throws the given exception, if exceptions
//!        are enabled, or calls the default assert
//!
//! \param condition the condition that, when false, triggers either an
//!                  assertion or throws an exception
//! \param exception the exception to throw
//! \param message   the message for the failure
#if BIT_COMPILER_EXCEPTIONS_ENABLED
# define BIT_ASSERT_OR_THROW(condition,exception,message) \
  ((BIT_LIKELY(condition)) ? ((void)0) : []{ throw exception{ message }; }())
#else
# define BIT_ASSERT_OR_THROW(condition,exception,message) \
  BIT_ASSERT(condition,message)
#endif

#include "detail/assert.inl"

#endif /* BIT_CORE_UTILITIES_ASSERT_HPP */
