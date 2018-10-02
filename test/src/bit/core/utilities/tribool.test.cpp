/*****************************************************************************
 * \file
 * \brief todo: fill in documentation
 *****************************************************************************/

#include <bit/core/utilities/tribool.hpp>

#include <catch2/catch.hpp>

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

TEST_CASE("tribool::tribool()")
{
  auto t = bit::core::tribool();

  SECTION("Constructor creates indeterminate tribool")
  {
    REQUIRE( bit::core::indeterminate( t ) );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("tribool::tribool( bool t )")
{
  SECTION("Constructor accepting true constructs true tribool")
  {
    bit::core::tribool t = true;

    REQUIRE( t == true );
  }

  SECTION("Constructor accepting false constructs false tribool")
  {
    bit::core::tribool t = false;

    REQUIRE( t == false );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("tribool::tribool( indeterminate_t )")
{
  SECTION("Constructor accepting indeterminate constructs indeterminate tribool")
  {
    bit::core::tribool t = bit::core::indeterminate;

    REQUIRE( (t == bit::core::indeterminate) );
  }
}

//----------------------------------------------------------------------------
// Boolean Operators
//----------------------------------------------------------------------------

TEST_CASE("tribool::operator bool()")
{
  SECTION("false returns false")
  {
    auto result = bit::core::tribool(false);

    REQUIRE_FALSE( bool(result) );
  }

  SECTION("indeterminate returns false")
  {
    auto result = bit::core::tribool(bit::core::indeterminate);

    REQUIRE_FALSE( bool(result) );
  }

  SECTION("true returns true")
  {
    auto result = bit::core::tribool(true);

    REQUIRE( bool(result) );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("tribool::operator !()")
{
  SECTION("false returns true")
  {
    auto result = bit::core::tribool(false);

    REQUIRE( !result );
  }

  SECTION("indeterminate returns false")
  {
    auto result = bit::core::tribool(bit::core::indeterminate);

    REQUIRE_FALSE( !result );
  }

  SECTION("true returns false")
  {
    auto result = bit::core::tribool(true);

    REQUIRE_FALSE( !result );
  }
}

//----------------------------------------------------------------------------
// Binary Boolean Operators
//----------------------------------------------------------------------------

TEST_CASE("tribool operator && ( const tribool&, const tribool& )")
{
  SECTION("false && false returns false")
  {
    auto result = bit::core::tribool(false) && bit::core::tribool(false);

    REQUIRE( result == false );
  }

  SECTION("false && true returns false")
  {
    auto result = bit::core::tribool(false) && bit::core::tribool(true);

    REQUIRE( result == false );
  }

  SECTION("true && true returns true")
  {
    auto result = bit::core::tribool(true) && bit::core::tribool(true);

    REQUIRE( result == true );
  }

  SECTION("false && indeterminate returns indeterminate")
  {
    auto result = bit::core::tribool(false) && bit::core::tribool(bit::core::indeterminate);

    REQUIRE( (result == bit::core::indeterminate) );
  }

  SECTION("true && indeterminate returns indeterminate")
  {
    auto result = bit::core::tribool(true) && bit::core::tribool(bit::core::indeterminate);

    REQUIRE( (result == bit::core::indeterminate) );
  }

  SECTION("indeterminate && indeterminate returns indeterminate")
  {
    auto result = bit::core::tribool(bit::core::indeterminate) && bit::core::tribool(bit::core::indeterminate);

    REQUIRE( (result == bit::core::indeterminate) );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("tribool operator || ( const tribool&, const tribool& )")
{
  SECTION("false || false returns false")
  {
    auto result = bit::core::tribool(false) || bit::core::tribool(false);

    REQUIRE( result == false );
  }

  SECTION("false || true returns true")
  {
    auto result = bit::core::tribool(false) || bit::core::tribool(true);

    REQUIRE( result == true );
  }

  SECTION("true || true returns true")
  {
    auto result = bit::core::tribool(true) || bit::core::tribool(true);

    REQUIRE( result == true );
  }

  SECTION("false || indeterminate returns indeterminate")
  {
    auto result = bit::core::tribool(false) || bit::core::tribool(bit::core::indeterminate);

    REQUIRE( (result == bit::core::indeterminate) );
  }

  SECTION("true || indeterminate returns true")
  {
    auto result = bit::core::tribool(true) || bit::core::tribool(bit::core::indeterminate);

    REQUIRE( (result == true) );
  }

  SECTION("indeterminate || indeterminate returns indeterminate")
  {
    auto result = bit::core::tribool(bit::core::indeterminate) || bit::core::tribool(bit::core::indeterminate);

    REQUIRE( (result == bit::core::indeterminate) );
  }
}
