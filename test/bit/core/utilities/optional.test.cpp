/*****************************************************************************
 * \file
 * \brief Test cases for bit::core::optional
 *****************************************************************************/

#include <bit/core/utilities/optional.hpp>

#include <catch2/catch.hpp>

namespace {

  class ctor_test
  {
  public:
    explicit ctor_test( bool& is_called )
    {
      is_called = true;
    }
  };

  class copy_ctor_test
  {
  public:
    explicit copy_ctor_test( bool& is_called )
      : m_is_called( is_called )
    {

    }

    explicit copy_ctor_test( const copy_ctor_test& other )
        : m_is_called( other.m_is_called )
    {
      m_is_called = true;
    }

  private:
    bool& m_is_called;
  };

  class move_ctor_test
  {
  public:
    explicit move_ctor_test( bool& is_called )
      : m_is_called( is_called )
    {

    }

    explicit move_ctor_test( move_ctor_test&& other )
        : m_is_called( other.m_is_called )
    {
      m_is_called = true;
    }

  private:
    bool& m_is_called;
  };

  class copy_assign_test
  {
  public:
    explicit copy_assign_test( bool& is_called )
      : m_is_called( is_called )
    {

    }

    copy_assign_test& operator=( const copy_assign_test& )
    {
      m_is_called = true;
      return (*this);
    }

  private:
    bool& m_is_called;
  };

  class move_assign_test
  {
  public:
    explicit move_assign_test( bool& is_called )
      : m_is_called( is_called )
    {

    }

    move_assign_test& operator=( move_assign_test&& )
    {
      m_is_called = true;
      return (*this);
    }

  private:
    bool& m_is_called;
  };

  class dtor_test
  {
  public:
    explicit dtor_test( bool& is_called )
      : m_is_called( is_called )
    {

    }

    ~dtor_test()
    {
      m_is_called = true;
    }

  private:
    bool& m_is_called;
  };
} // anonymous namespace

//=============================================================================
// class : optional
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors / Destructor / Assignment
//-----------------------------------------------------------------------------

TEST_CASE("optional::optional()", "[ctor]")
{
  auto optional = bit::core::optional<int>();

  SECTION("Has no value")
  {
    REQUIRE_FALSE( static_cast<bool>(optional) );
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::optional( nullopt_t )","[ctor]")
{
  auto optional = bit::core::optional<int>( bit::core::nullopt );

  SECTION("Has no value")
  {
    REQUIRE_FALSE( static_cast<bool>(optional) );
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::optional( const optional& )","[ctor]")
{
  SECTION("Copying a null optional")
  {
    auto original  = bit::core::optional<int>();
    auto optional  = original;

    SECTION("Has no value")
    {
      REQUIRE_FALSE( static_cast<bool>(optional) );
    }
  }

  SECTION("Copying a non-null optional")
  {
    auto value = 42;
    auto original  = bit::core::optional<int>{ value };
    auto optional  = original;

    SECTION("Has a value")
    {
      REQUIRE( static_cast<bool>(optional) );
    }

    SECTION("Value is the same as original")
    {
      REQUIRE( optional.value() == value );
    }
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::optional( optional&& )","[ctor]")
{
  SECTION("Copying a null optional")
  {
    auto original  = bit::core::optional<int>();
    auto optional  = std::move(original);

    SECTION("Has no value")
    {
      REQUIRE_FALSE( static_cast<bool>(optional) );
    }
  }

  SECTION("Copying a non-null optional")
  {
    auto value = 42;
    auto original  = bit::core::optional<int>( value );
    auto optional  = std::move(original);

    SECTION("Has a value")
    {
      REQUIRE( static_cast<bool>(optional) );
    }

    SECTION("Value is the same as original")
    {
      REQUIRE( optional.value() == value );
    }
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::optional( const value_type& )","[ctor]")
{
  auto value    = 42;
  auto optional = bit::core::optional<int>(value);

  SECTION("Has a value")
  {
    REQUIRE( static_cast<bool>(optional) );
  }

  SECTION("Value is the same as original")
  {
    REQUIRE( optional.value() == value );
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::optional( value_type&& )","[ctor]")
{
  auto value          = 42;
  auto expectedValue  = 42;
  auto optional       = bit::core::optional<int>( std::move(value) );

  SECTION("Has a value")
  {
    REQUIRE( static_cast<bool>(optional) );
  }

  SECTION("Value is the same as original")
  {
    REQUIRE( optional.value() == expectedValue );
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::~optional()","[dtor]")
{
  bool is_called = false;
  auto dtor = dtor_test(is_called);
  {
    auto optional = bit::core::optional<dtor_test>(dtor);
  }

  SECTION("Destructor gets called")
  {
    REQUIRE( is_called );
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::operator=( nullopt_t )","[assignment]")
{
  SECTION("Assigning over non-null value")
  {
    auto is_called = false;
    auto dtor = dtor_test(is_called);
    auto optional = bit::core::optional<dtor_test>(dtor);
    optional = bit::core::nullopt;

    SECTION("Calls destructor on previous value")
    {
      REQUIRE( is_called );
    }

    SECTION("Converts to null")
    {
      REQUIRE_FALSE( static_cast<bool>(optional) );
    }
  }

  SECTION("Assigning over null value")
  {
    auto optional = bit::core::optional<int>( bit::core::nullopt );

    SECTION("Converts to null")
    {
      REQUIRE_FALSE( static_cast<bool>(optional) );
    }
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::operator=( const optional& )","[assignment]")
{
  SECTION("Constructs unconstructed optional")
  {
    // TODO(bitwizeshift): Add unit tests
  }

  SECTION("Copy-assigns constructed optional")
  {
    // TODO(bitwizeshift): Add unit tests
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::operator=( optional&& )","[assignment]")
{
  SECTION("Constructs unconstructed optional")
  {
    // TODO(bitwizeshift): Add unit tests
  }

  SECTION("Destructs previous optional before constructing new one")
  {
    // TODO(bitwizeshift): Add unit tests
  }
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::operator=( U&& )","[assignment]")
{
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

TEST_CASE("optional::operator->()","[observers]")
{
  // TODO(bitwizeshift): Add unit tests
}

TEST_CASE("optional::operator->() const","[observers]")
{
  // TODO(bitwizeshift): Add unit tests
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::operator*() &","[observers]")
{
  // TODO(bitwizeshift): Add unit tests
}

TEST_CASE("optional::operator*() &&","[observers]")
{
  // TODO(bitwizeshift): Add unit tests
}

TEST_CASE("optional::operator*() const &","[observers]")
{
  // TODO(bitwizeshift): Add unit tests
}

TEST_CASE("optional::operator*() const &&","[observers]")
{
  // TODO(bitwizeshift): Add unit tests
}

//-----------------------------------------------------------------------------

TEST_CASE("optional::operator bool()","[observers]")
{
  SECTION("Optional does not contain a value")
  {
    bit::core::optional<int> op;

    SECTION("Is implicitly false")
    {
      REQUIRE_FALSE(op);
    }
  }
  SECTION("Optional contains a value")
  {
    bit::core::optional<int> op = 42;

    SECTION("Is implicitly true")
    {
      REQUIRE(op);
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value() &","[observers]")
{
  SECTION("Optional does not contain value")
  {
    SECTION("Throws bad_optional_access")
    {
      // Arrange
      bit::core::optional<int> op;

      // Act & Assert
      REQUIRE_THROWS_AS( op.value(), bit::core::bad_optional_access );
    }
  }
  SECTION("Optional contains a value")
  {
    SECTION("Returns value")
    {
      int value = 42;
      bit::core::optional<int> op = value;

      REQUIRE( op.value() == value );
    }

    SECTION("Returns a value by lvalue-reference")
    {
      int value = 42;
      bit::core::optional<int> op = value;

      REQUIRE( (std::is_same<int&,decltype(op.value())>::value) );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value() const &","[observers]")
{
  SECTION("Optional does not contain value")
  {
    SECTION("Throws bad_optional_access")
    {
      bit::core::optional<int> const op;

      REQUIRE_THROWS_AS( op.value(), bit::core::bad_optional_access );
    }
  }
  SECTION("Optional contains a value")
  {
    SECTION("Returns value")
    {
      int value = 42;
      bit::core::optional<int> const op = value;

      REQUIRE( op.value() == value );
    }

    SECTION("Returns a value by const lvalue-reference")
    {
      int value = 42;
      bit::core::optional<int> const op = value;

      // Act & Assert
      REQUIRE( (std::is_same<int const&,decltype(op.value())>::value) );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value() &&","[observers]")
{
  SECTION("Optional does not contain value")
  {
    SECTION("Throws bad_optional_access")
    {
      bit::core::optional<int> op;

      REQUIRE_THROWS_AS( std::move(op).value(), bit::core::bad_optional_access ); // NOLINT
    }
  }
  SECTION("Optional contains a value")
  {
    SECTION("Returns value")
    {
      int value = 42;
      bit::core::optional<int> op = value;

      REQUIRE( std::move(op).value() == value ); // NOLINT
    }

    SECTION("Returns a value by rvalue-reference")
    {
      int value = 42;
      bit::core::optional<int> op = value;

      REQUIRE( (std::is_same<int&&,decltype(std::move(op).value())>::value) );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value() const &&","[observers]")
{
  SECTION("Optional does not contain value")
  {
    SECTION("Throws bad_optional_access")
    {
      // Arrange
      bit::core::optional<int> const op;

      // Act & Assert
      REQUIRE_THROWS_AS( std::move(op).value(), bit::core::bad_optional_access ); // NOLINT
    }
  }
  SECTION("Optional contains a value")
  {
    SECTION("Returns value")
    {
      // Arrange
      int value = 42;
      bit::core::optional<int> const op = value;

      // Act & Assert
      REQUIRE( std::move(op).value() == value ); // NOLINT
    }

    SECTION("Returns a value by const rvalue-reference")
    {
      // Arrange
      int value = 42;
      bit::core::optional<int> const op = value;

      // Act & Assert
      REQUIRE( (std::is_same<int const&&,decltype(std::move(op).value())>::value) );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value_or( U&& ) const &","[observers]")
{
  SECTION("Optional does not contain a value")
  {
    auto optional = bit::core::optional<int>();

    REQUIRE( optional.value_or(42) == 42 );
  }

  SECTION("Optional contains a value")
  {
    auto optional = bit::core::optional<int>(32);

    REQUIRE( optional.value_or(42) == 32 );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value_or( U&& ) &&","[observers]")
{
  SECTION("Optional does not contain a value")
  {
    auto optional = bit::core::optional<int>();

    REQUIRE( std::move(optional).value_or(42) == 42 ); // NOLINT
  }

  SECTION("Optional contains a value")
  {
    auto optional = bit::core::optional<int>(32);

    REQUIRE( std::move(optional).value_or(42) == 32 ); // NOLINT
  }
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------

TEST_CASE("optional::swap( optional<T>& )","[modifiers]")
{
  SECTION("Both optionals are null")
  {
    auto op1 = bit::core::optional<int>();
    auto op2 = bit::core::optional<int>();

    op1.swap(op2);

    SECTION("Values swapped")
    {
      SECTION("op1 is null")
      {
        REQUIRE( op1 == bit::core::nullopt );
      }
      SECTION("op2 is null")
      {
        REQUIRE( op2 == bit::core::nullopt );
      }
    }
  }

  SECTION("Both optionals are non-null")
  {
    auto value1 = 32;
    auto value2 = 64;
    auto op1 = bit::core::optional<int>(value1);
    auto op2 = bit::core::optional<int>(value2);

    op1.swap(op2);

    SECTION("Values swapped")
    {
      SECTION("op1 contains op2's value")
      {
        REQUIRE( op1.value() == value2 );
      }
      SECTION("op2 contains op1's value")
      {
        REQUIRE( op2.value() == value1 );
      }
    }

    SECTION("Optionals are non-null")
    {
      SECTION("op1 is non-null")
      {
        REQUIRE( static_cast<bool>(op1) );
      }
      SECTION("op2 is non-null")
      {
        REQUIRE( static_cast<bool>(op2) );
      }
    }
  }

  SECTION("(*this) optional is null")
  {
    auto value1 = 32;
    auto value2 = bit::core::nullopt;
    auto op1 = bit::core::optional<int>(value1);
    auto op2 = bit::core::optional<int>(value2);

    op1.swap(op2);

    SECTION("Values swapped")
    {
      SECTION("op1 contains op2's value")
      {
        REQUIRE( !op1.has_value() );
      }
      SECTION("op2 contains op1's value")
      {
        REQUIRE( op2 == value1 );
      }
    }
  }

  SECTION("other optional is null")
  {
    // Arrange
    auto value1 = bit::core::nullopt;
    auto value2 = 32;
    auto op1 = bit::core::optional<int>(value1);
    auto op2 = bit::core::optional<int>(value2);

    // Act
    op1.swap(op2);

    SECTION("Values swapped")
    {
      SECTION("op1 contains op2's value")
      {
        REQUIRE( op1 == value2 );
      }
      SECTION("op2 contains op1's value")
      {
        REQUIRE( !op2.has_value() );
      }
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::emplace( Args&&... )","[modifiers]")
{
  SECTION("Original optional is null")
  {
    // TODO(bitwizeshift): Add unit tests
  }

  SECTION("Original optional is non-null")
  {
    // TODO(bitwizeshift): Add unit tests
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::emplace( std::initializer_list<U>, Args&&... )","[modifiers]")
{
  SECTION("Original optional is null")
  {
    // TODO(bitwizeshift): Add unit tests
  }

  SECTION("Original optional is non-null")
  {
    // TODO(bitwizeshift): Add unit tests
  }
}