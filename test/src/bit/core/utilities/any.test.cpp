/*****************************************************************************
 * \file
 * \brief Test cases for bit::core::any
 *****************************************************************************/

#include <bit/core/utilities/any.hpp>
#include <bit/core/utilities/in_place.hpp>

#include <string>
#include <utility>
#include <typeindex>

#include <catch2/catch.hpp>

using namespace std::literals::string_literals;

namespace {

  // A testing string that is sufficiently long enough to avoid string SBO.
  // This is used so that MSAN & LSAN can validate memory issues
  const auto string_value = // NOLINT
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
    "Cras consequat et augue auctor porttitor."s;

  struct large_object
  {
    large_object() noexcept : buffer{}{};
    large_object( std::string value ) : value{ std::move(value) }, buffer{}{}
    large_object( large_object&& other ) = default;
    large_object( const large_object& other ) = default;

    large_object& operator=( large_object&& other ) = default;
    large_object& operator=( const large_object& other ) = default;

    std::string value;
    // a buffer bigger than any's internal buffer
    char buffer[sizeof(bit::core::any)];
  };

  // TODO(bitwize): If this (fragile) assertion doesn't hold, unit tests will
  //   need to be run against fixed-sized types to avoid implementation
  //   variance.
  static_assert( sizeof(std::string) <= (sizeof(void*)*4),
                 "sanity check: std::string is used for checking small-buffer "
                 "for 'any' type");

} // anonymous namespace

//=============================================================================
// class : any
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor / Assignment
//-----------------------------------------------------------------------------

TEST_CASE("any::any()","[ctor]")
{
  auto a = bit::core::any{};

  SECTION("Does not contain a value")
  {
    REQUIRE_FALSE( a.has_value() );
  }

  SECTION("Type is typeid(void)")
  {
    REQUIRE( a.type() == typeid(void) );
  }
}

TEST_CASE("any::any( any&& )","[ctor]")
{
  SECTION("Source contains value")
  {
    auto original = bit::core::any{::string_value};
    auto& original_type = original.type();

    auto moved = std::move(original);

    SECTION("Moved result contains a value")
    {
      REQUIRE( moved.has_value() );
    }
    SECTION("Type is same as original")
    {
      REQUIRE( original_type == moved.type() );
    }
    SECTION("Contains same value")
    {
      using namespace bit::core::casts;

      REQUIRE( any_cast<std::string>(moved) == ::string_value );
    }
  }
  SECTION("Source does not contain value")
  {
    auto original = bit::core::any{};

    auto moved = std::move(original);

    SECTION("Moved result does not contain a value")
    {
      REQUIRE_FALSE( moved.has_value() );
    }
    SECTION("Type is typeid(void)")
    {
      REQUIRE( moved.type() == typeid(void) );
    }
  }
}

TEST_CASE("any::any( const any& )","[ctor]")
{
  SECTION("Source contains value")
  {
    auto original = bit::core::any{::string_value};
    auto& original_type = original.type();

    auto copy = original;

    SECTION("Moved result contains a value")
    {
      REQUIRE( copy.has_value() );
    }
    SECTION("Type is same as original")
    {
      REQUIRE( original_type == copy.type() );
    }
    SECTION("Contains same value")
    {
      using namespace bit::core::casts;

      REQUIRE( any_cast<std::string>(copy) == ::string_value );
    }
  }
  SECTION("Source does not contain value")
  {
    auto original = bit::core::any{};

    auto copy = original; // NOLINT

    SECTION("Moved result does not contain a value")
    {
      REQUIRE_FALSE( copy.has_value() );
    }
    SECTION("Type is typeid(void)")
    {
      REQUIRE( copy.type() == typeid(void) );
    }
  }
}

TEST_CASE("any::any( ValueType&& )","[ctor]")
{
  const char value[] = "Hello world";
  auto a = bit::core::any{value};

  SECTION("Constructs an any with a C-string value")
  {
    SECTION("Contains a value")
    {
      REQUIRE( a.has_value() );
    }
    SECTION("Type is decayed to typeid(const char*)")
    {
      REQUIRE( a.type() == typeid(const char*) );
    }
    SECTION("Value is same as original input")
    {
      using namespace bit::core::casts;

      REQUIRE( std::string{value} == any_cast<const char*>(a) );
    }
  }
}

TEST_CASE("any::any( in_place_type_t<ValueType>, Args&&... )","[ctor]")
{
  const auto value    = ::string_value;
  const auto expected = std::string{value,2,7};

  auto a = bit::core::any{ bit::core::in_place_type<std::string>, value, 2, 7 };

  SECTION("Constructs an any with by calling the underlying T constructor")
  {
    SECTION("Contains a value")
    {
      REQUIRE( a.has_value() );
    }
    SECTION("Type is the same as specified")
    {
      REQUIRE( a.type() == typeid(std::string) );
    }
    SECTION("Value is same as original input")
    {
      using namespace bit::core::casts;

      REQUIRE( expected == any_cast<std::string>(a) );
    }
  }
}

TEST_CASE("any::any( in_place_type_t<ValueType>, std::initializer_list<U>, Args&&... )","[ctor]")
{
  // TODO(bitwize): test initializer_list constructor
}

//-----------------------------------------------------------------------------

TEST_CASE("any::operator=( any&& )","[assignment]")
{
  SECTION("Destination contains a value")
  {
    auto destination = bit::core::any{5};

    SECTION("Source does not contain a value")
    {
      auto source = bit::core::any{};

      destination = std::move(source);

      SECTION("Destination does not contain a value")
      {
        REQUIRE_FALSE( destination.has_value() );
      }
    }
    SECTION("Source contains a value")
    {
      const auto value = ::string_value;
      auto source = bit::core::any{value};

      destination = std::move(source);

      SECTION("Destination contains a value")
      {
        REQUIRE( destination.has_value() );
      }
      SECTION("Destination contains source's value")
      {
        using namespace bit::core::casts;

        REQUIRE( value == any_cast<std::string>(destination) );
      }
      SECTION("Destination changes to source's type")
      {
        using namespace bit::core::casts;

        REQUIRE( destination.type() == typeid(std::string) );
      }
    }
  }
  SECTION("Destination does not contain a value")
  {
    auto destination = bit::core::any{};

    SECTION("Source does not contain a value")
    {
      auto source = bit::core::any{};

      destination = std::move(source);

      SECTION("Destination does not contain a value")
      {
        REQUIRE_FALSE( destination.has_value() );
      }
    }
    SECTION("Source contains a value")
    {
      const auto value = ::string_value;
      auto source = bit::core::any{value};

      destination = std::move(source);

      SECTION("Destination contains a value")
      {
        REQUIRE( destination.has_value() );
      }
      SECTION("Destination contains source's value")
      {
        using namespace bit::core::casts;

        REQUIRE( value == any_cast<std::string>(destination) );
      }
      SECTION("Destination changes to source's type")
      {
        using namespace bit::core::casts;

        REQUIRE( destination.type() == typeid(std::string) );
      }
    }
  }
}

TEST_CASE("any::operator=( const any& )","[assignment]")
{
  SECTION("Destination contains a value")
  {
    auto destination = bit::core::any{5};

    SECTION("Source does not contain a value")
    {
      auto source = bit::core::any{};

      destination = source;

      SECTION("Destination does not contain a value")
      {
        REQUIRE_FALSE( destination.has_value() );
      }
    }
    SECTION("Source contains a value")
    {
      const auto value = ::string_value;
      auto source = bit::core::any{value};

      destination = source;

      SECTION("Destination contains a value")
      {
        REQUIRE( destination.has_value() );
      }
      SECTION("Destination contains source's value")
      {
        using namespace bit::core::casts;

        REQUIRE( value == any_cast<std::string>(destination) );
      }
      SECTION("Destination changes to source's type")
      {
        using namespace bit::core::casts;

        REQUIRE( destination.type() == typeid(std::string) );
      }
    }
  }
  SECTION("Destination does not contain a value")
  {
    auto destination = bit::core::any{};

    SECTION("Source does not contain a value")
    {
      auto source = bit::core::any{};

      destination = source;

      SECTION("Destination does not contain a value")
      {
        REQUIRE_FALSE( destination.has_value() );
      }
    }
    SECTION("Source contains a value")
    {
      const auto value = ::string_value;
      auto source = bit::core::any{value};

      destination = source;

      SECTION("Destination contains a value")
      {
        REQUIRE( destination.has_value() );
      }
      SECTION("Destination contains source's value")
      {
        using namespace bit::core::casts;

        REQUIRE( value == any_cast<std::string>(destination) );
      }
      SECTION("Destination changes to source's type")
      {
        using namespace bit::core::casts;

        REQUIRE( destination.type() == typeid(std::string) );
      }
    }
  }
}

TEST_CASE("any::operator=( ValueType&& )","[assignment]")
{
  SECTION("Destination contains a value")
  {
    const auto value = ::string_value;
    auto original = bit::core::any{5};

    original = value;

    SECTION("Destination contains a value")
    {
      REQUIRE( original.has_value() );
    }
    SECTION("Destination contains source's value")
    {
      using namespace bit::core::casts;

      REQUIRE( value == any_cast<std::string>(original) );
    }
    SECTION("Destination changes to source's type")
    {
      using namespace bit::core::casts;

      REQUIRE( original.type() == typeid(std::string) );
    }
  }
  SECTION("Destination does not contain a value")
  {
    auto original = bit::core::any{};

    const auto value = ::string_value;

    original = value;

    SECTION("Destination contains a value")
    {
      REQUIRE( original.has_value() );
    }
    SECTION("Destination contains source's value")
    {
      using namespace bit::core::casts;

      REQUIRE( value == any_cast<std::string>(original) );
    }
    SECTION("Destination changes to source's type")
    {
      using namespace bit::core::casts;

      REQUIRE( original.type() == typeid(std::string) );
    }
  }
}

//-----------------------------------------------------------------------------
// Modifiers
//-----------------------------------------------------------------------------

TEST_CASE("any::swap( any& )","[modifiers]")
{
  SECTION("lhs contains a value")
  {
    const auto lhs_value = 42;

    auto lhs = bit::core::any{lhs_value};

    SECTION("rhs contains a value")
    {
      const auto rhs_value = "Hello World";

      auto rhs = bit::core::any{ bit::core::in_place_type<std::string>, rhs_value };

      lhs.swap(rhs);

      SECTION("lhs contains rhs's old value")
      {
        using namespace bit::core::casts;

        REQUIRE( any_cast<std::string>(lhs) == rhs_value );
      }
      SECTION("rhs contains lhs's old value")
      {
        using namespace bit::core::casts;

        REQUIRE( any_cast<int>(rhs) == lhs_value );
      }
    }
    SECTION("rhs does not contain a value")
    {
      auto rhs = bit::core::any{};

      lhs.swap(rhs);

      SECTION("lhs no longer contains a value")
      {
        REQUIRE_FALSE( lhs.has_value() );
      }
      SECTION("rhs contains lhs's old value")
      {
        using namespace bit::core::casts;

        REQUIRE( any_cast<int>(rhs) == lhs_value );
      }
    }
  }
  SECTION("lhs does not contain a value")
  {
    auto lhs = bit::core::any{};

    SECTION("rhs contains a value")
    {
      const auto rhs_value = 42;
      auto rhs = bit::core::any{rhs_value};

      lhs.swap(rhs);

      SECTION("lhs contains rhs's old value")
      {
        using namespace bit::core::casts;

        REQUIRE( any_cast<int>(lhs) == rhs_value );
      }
      SECTION("rhs no longer contains a value")
      {
        REQUIRE_FALSE( rhs.has_value() );
      }
    }
    SECTION("rhs does not contain a value")
    {
      auto rhs = bit::core::any{};

      lhs.swap(rhs);

      SECTION("lhs doesn't contain a value")
      {
        REQUIRE_FALSE( lhs.has_value() );
      }
      SECTION("rhs doesn't contain a value")
      {
        REQUIRE_FALSE( rhs.has_value() );
      }
    }
  }
}

TEST_CASE("any::emplace( Args&&... )","[modifiers]")
{
  SECTION("lhs contains a value")
  {
    auto lhs = bit::core::any{42};

    SECTION("new value fits in small buffer")
    {
      const auto lhs_value = ::string_value;

      lhs.emplace<std::string>(lhs_value);

      SECTION("Contains a value after")
      {
        using namespace bit::core::casts;

        REQUIRE( any_cast<std::string>(lhs) == lhs_value );
      }
      SECTION("Changes to the emplaced type")
      {
        REQUIRE( lhs.type() == typeid(std::string) );
      }
    }
    SECTION("new value does not fit in small buffer")
    {
      const auto lhs_value = ::string_value;

      lhs.emplace<::large_object>(lhs_value);

      SECTION("Contains a value after")
      {
        using namespace bit::core::casts;

        REQUIRE( any_cast<::large_object>(lhs).value == lhs_value );
      }
      SECTION("Changes to the emplaced type")
      {
        REQUIRE( lhs.type() == typeid(::large_object) );
      }
    }
  }
  SECTION("lhs doesn't contain a value")
  {
    SECTION("new value fits in small buffer")
    {
      const auto lhs_value = ::string_value;
      auto lhs = bit::core::any{};

      lhs.emplace<std::string>( lhs_value );

      SECTION("Contains a value after")
      {
        using namespace bit::core::casts;

        REQUIRE( any_cast<std::string>(lhs) == lhs_value );
      }
    }
    SECTION("new value does not fit in small buffer")
    {
      const auto lhs_value = ::string_value;
      auto lhs = bit::core::any{};

      lhs.emplace<::large_object>( lhs_value );

      SECTION("Contains a value after")
      {
        using namespace bit::core::casts;

        REQUIRE( any_cast<::large_object>(lhs).value == lhs_value );
      }
    }
  }
}

TEST_CASE("any::emplace( std::initializer_list<U>, Args&&... )","[modifiers]")
{
  // TODO(bitwize): test initializer_list emplace
}

TEST_CASE("any::reset()","[modifiers]")
{
  SECTION("Source contains a value")
  {
    auto source = bit::core::any{ bit::core::in_place_type<std::string>, "Hello World" };

    source.reset();

    SECTION("Reset clears value")
    {
      REQUIRE_FALSE( source.has_value() );
    }
  }
  SECTION("Source does not contain a value")
  {
    auto source = bit::core::any{};

    source.reset();

    SECTION("Source still contains no value")
    {
      REQUIRE_FALSE( source.has_value() );
    }
  }
}
