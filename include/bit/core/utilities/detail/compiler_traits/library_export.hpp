/*****************************************************************************
 * \file
 * \brief This header provides an interface for exporting symbols for
 *        dynamic libraries
 *
 * \note This is an internal header file, included by other library headers.
 *       Do not attempt to use it directly.
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
#ifndef BIT_CORE_UTILITIES_DETAIL_COMPILER_TRAITS_LIBRARY_EXPORT_HPP
#define BIT_CORE_UTILITIES_DETAIL_COMPILER_TRAITS_LIBRARY_EXPORT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

//! \def BIT_API_EXPORT
//! \brief Expands to compiler/platform specific symbol export statement
//!
//! \def BIT_API_IMPORT
//! \brief Expands to compiler/platform specific symbol import statement
//!
//! \def BIT_API_LOCAL
//! \brief Decrease visibility to just the library (usable only in linux)
#ifdef STATIC_BUILD
# define BIT_API_EXPORT /* not used */
# define BIT_API_IMPORT /* not used */
# define BIT_API_LOCAL  /* not used */
#else
# define BIT_API_EXPORT BIT_COMPILER_SYMBOL_EXPORT
# define BIT_API_IMPORT BIT_COMPILER_SYMBOL_IMPORT
# define BIT_API_LOCAL  BIT_COMPILER_SYMBOL_VISIBLE
#endif

//! \def BIT_API
//!
//! \brief Expands to export on DLL build, import otherwise
#ifdef BUILD_DLL
# define BIT_API BIT_API_EXPORT // Export the symbols to the library
#else
# define BIT_API BIT_API_IMPORT // Import symbols from the library
#endif

//! \def BIT_IMPEXP
//!
//! \brief DLL-building import/export symbol.
//!
//! Expands to compiler and platform specific import or export symbol if
//! building or reading from DLL, otherwise expands to nothing if statically
//! linking.
#if defined(STATIC_BUILD) || defined(NO_DLL)
# define BIT_IMPEXP
#elif defined(BUILD_DLL)
# define BIT_IMPEXP BIT_API_EXPORT
#else
# define BIT_IMPEXP BIT_API_IMPORT
#endif

//! \def BIT_CAPI
//!
//! \brief Symbol to denote the calling conventions for a C DLL
#ifndef BIT_CAPI
# if defined(__GNUC__) || (defined(_MSC_VER) && (_MSC_VER >= 800))
#   define BIT_CAPI BIT_CDECL
# else
#   define BIT_CAPI BIT_STDCALL
# endif
#endif

//! \def BIT_EXPORT_TYPE( type )
//!
//! \brief Expands to the type along with the import/export symbol
#ifndef BIT_EXPORT_TYPE
# define BIT_EXPORT_TYPE( type ) BIT_IMPEXP type
#endif

//! \def BIT_PROTOTYPE( type, name, args, attributes )
//!
//! \brief Expands into a function prototype containing attributes,
//!        return type, name, and arguments.
#ifndef BIT_PROTOTYPE
# define BIT_PROTOTYPE( type, name, args, attributes ) attributes type name args
#endif

//! \def BIT_EXPORT_CA( type, name, args, attributes )
//!
//! \brief Expands into an export statement for a C function that contains
//!        attributes. Note that this must be in an extern "C" statement.
#ifndef BIT_EXPORT_CA
# define BIT_EXPORT_CA( type, name, args, attributes ) \
    BIT_PROTOTYPE(                                     \
      BIT_IMPEXP type,                                 \
      (BIT_API name),                                  \
      args,                                            \
      extern attributes                                \
    )
#endif

//! \def BIT_EXPORT_C( type, name, args, attributes )
//!
//! \brief Expands into an export statement for a C function prototype.
//!        Note that this must be in an extern "C" statement.
#ifndef BIT_EXPORT_C
# define BIT_EXPORT_C( type, name, args ) BIT_EXPORT_CA( type, name, args, BIT_EMPTY )
#endif

//! \def BIT_EXPORT_A( type, name, args, attributes )
//!
//! \brief Expands into an export statement for a C++ function prototype
//!        containing function attributes.
//!
//! Note that this does not belong in an extern "C" statement, and should
//! not be in a class body/method signature.
#ifndef BIT_EXPORT_A
# define BIT_EXPORT_A( type, name, args, attributes ) \
    BIT_PROTOTYPE(                                    \
      BIT_IMPEXP type,                                \
      name,                                           \
      args,                                           \
      attributes                                      \
    )
#endif

//! \def BIT_EXPORT( type, name, args )
//!
//! \brief Expands into an export statement for a C++ function prototype.
//!
//! Note that this does not belong in an extern "C" statement, and should
//! not be in a class body/method signature.
#ifndef BIT_EXPORT
# define BIT_EXPORT( type, name, args ) BIT_EXPORT_A(type, name, args, BIT_EMPTY )
#endif

#endif /* BIT_CORE_UTILITIES_DETAIL_COMPILER_TRAITS_LIBRARY_EXPORT_HPP */
