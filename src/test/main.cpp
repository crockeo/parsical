//////////////
// Includes //
#include "catch.hpp"

#include "../parsical/parsestream.hpp"
#include "../parsical/parseerror.hpp"

//////////
// Code //

// Testing out the string parser for a couple of functions.
TEST_CASE("String Parser") {
    parsical::StringParser p("abcdefg");
    
    for (char c = 'a'; c <= 'g'; c++)
        REQUIRE(p.get() == c);
    REQUIRE(p.eof());
    REQUIRE_THROWS(p.get());

    REQUIRE_THROWS(p.stepBack(8));
    p.stepBack(7);
    REQUIRE_THROWS(p.unget());

    REQUIRE(p.peek() == 'a');
    REQUIRE(p.peek() == 'a');
}
