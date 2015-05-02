//////////////
// Includes //
#include "catch.hpp"

#include "../parsical/parsestream.hpp"
#include "../parsical/parseerror.hpp"
#include "../parsical/general.hpp"

//////////
// Code //

////
// parsestream.hpp

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

////
// general.hpp

// Attempting to perform a tryparse.
TEST_CASE("tryParse") {
    parsical::StringParser p("abcdefg");

    REQUIRE(p.pos() == 0);

    REQUIRE_THROWS(parsical::tryParse<char>(p, [](parsical::ParseStream<char>& stream) -> char {
        char c = stream.get();
        throw parsical::ParseError("Did the thing.");
        return c;
    }));

    REQUIRE(p.pos() == 0);
}
