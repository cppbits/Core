/*****************************************************************************
 * \file
 * \brief todo: fill in documentation
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
#ifndef BIT_CORE_CONTAINERS_DETAIL_RING_BUFFER_STORAGE_HPP
#define BIT_CORE_CONTAINERS_DETAIL_RING_BUFFER_STORAGE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../../utilities/compressed_pair.hpp"

#include <tuple>
#include <utility>

namespace bit {
  namespace core {
    namespace detail {

      template<typename T, typename Allocator>
      class ring_buffer_storage
      {
        //---------------------------------------------------------------------
        // Constructors
        //---------------------------------------------------------------------
      public:

        ring_buffer_storage( ring_buffer_storage&& other ) noexcept = default;

        ring_buffer_storage( ring_buffer_storage&& other, const Allocator& alloc )
          : m_storage( std::piecewise_construct,
                       std::forward_as_tuple(std::move(other.m_storage.first())),
                       std::forward_as_tuple(alloc) )
        {

        }

        ring_buffer_storage( std::size_t n, const Allocator& alloc )
          : m_storage( std::piecewise_construct,
                       std::forward_as_tuple(),
                       std::forward_as_tuple(alloc) )
        {
          if( n == 0 ) return;

          using traits_type = std::allocator_traits<Allocator>;

          auto p = traits_type::allocate( m_storage.second(), n );
          // No RAII needed; allocate is the only thing that can throw at this point

          m_storage.first() = ring_buffer<T>( p, n );
        }

        ~ring_buffer_storage()
        {
          using traits_type = std::allocator_traits<Allocator>;
          auto& buffer    = m_storage.first();
          auto& allocator = m_storage.second();

          // Deconstruct all entries, then deallocate the memory
          if( buffer.data() != nullptr ) {
            buffer.clear();
            traits_type::deallocate( allocator, buffer.data(), buffer.capacity() );
          }
        }

        ring_buffer<T>& buffer() noexcept
        {
          return m_storage.first();
        }

        const ring_buffer<T>& buffer() const noexcept
        {
          return m_storage.first();
        }

        const Allocator& get_allocator() const noexcept
        {
          return m_storage.second();
        }

        void swap( ring_buffer_storage& other )
        {
          using std::swap;

          m_storage.swap(other.m_storage);
        }

        //---------------------------------------------------------------------
        // Private Member Types
        //---------------------------------------------------------------------
      private:

        using storage_type = compressed_pair<ring_buffer<T>,Allocator>;

        //---------------------------------------------------------------------
        // Private Members
        //---------------------------------------------------------------------
      private:

        storage_type m_storage;
      };

    } // namespace detail

  } // namespace core
} // namespace bit

#endif /* BIT_CORE_CONTAINERS_DETAIL_RING_BUFFER_STORAGE_HPP */
