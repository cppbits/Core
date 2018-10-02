/*****************************************************************************
 * \file
 * \brief This header contains a type trait that determines a function
 *        signature from a function type
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
#ifndef BIT_CORE_TRAITS_TRANSFORMATIONS_FUNCTION_SIGNATURE_HPP
#define BIT_CORE_TRAITS_TRANSFORMATIONS_FUNCTION_SIGNATURE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../composition/type_identity.hpp"

#include <type_traits> // std::decay

namespace bit {
  namespace core {

    /// \brief Metafunction to extract the signature of a given function type
    ///
    /// The result is aliased as \c ::type
    ///
    /// \tparam fn the function to check
    template<typename Fn>
    struct function_signature;

    // Decay pointers/references

    template<typename Fn>
    struct function_signature<Fn*> : function_signature<std::decay_t<Fn>>{};

    template<typename Fn>
    struct function_signature<Fn&> : function_signature<std::decay_t<Fn>>{};

    template<typename Fn>
    struct function_signature<Fn&&> : function_signature<std::decay_t<Fn>>{};

    // Translate function values

    template<typename R, typename...ArgTypes>
    struct function_signature<R(ArgTypes...)> : type_identity<R(ArgTypes...)>{};

    template<typename R, typename...ArgTypes>
    struct function_signature<R(ArgTypes...) const> : type_identity<R(ArgTypes...)>{};

    template<typename R, typename...ArgTypes>
    struct function_signature<R(ArgTypes...) volatile> : type_identity<R(ArgTypes...)>{};

    template<typename R, typename...ArgTypes>
    struct function_signature<R(ArgTypes...) const volatile> : type_identity<R(ArgTypes...)>{};

    // Translate member function pointers

    template<typename C, typename R, typename...ArgTypes>
    struct function_signature<R(C::*)(ArgTypes...)> : type_identity<R(ArgTypes...)>{};

    template<typename C, typename R, typename...ArgTypes>
    struct function_signature<R(C::*)(ArgTypes...) const> : type_identity<R(ArgTypes...)>{};

    template<typename C, typename R, typename...ArgTypes>
    struct function_signature<R(C::*)(ArgTypes...) volatile> : type_identity<R(ArgTypes...)>{};

    template<typename C, typename R, typename...ArgTypes>
    struct function_signature<R(C::*)(ArgTypes...) const volatile> : type_identity<R(ArgTypes...)>{};

    /// \brief Helper utility to extract function_signature::type
    template<typename T>
    using function_signature_t = typename function_signature<T>::type;

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_TRAITS_TRANSFORMATIONS_FUNCTION_SIGNATURE_HPP */
