//////////////
// Includes //
#include <iostream>

#include "catch.hpp"

#include "../parsical/parsestream.hpp"
#include "../parsical/parseerror.hpp"
#include "../parsical/general.hpp"

//////////
// Code //

////
// parsestream.hpp

// A generic testing operation for a given implementation of a ParseStream.
template <typename T>
void testParser(parsical::ParseStream<T>& p, std::vector<T> values) {
    // Consuming all of the characters that ought to be in the test file.
    for (T t: values)
        REQUIRE(p.get() == t);

    // Checking that the stream is indeed empty.
    REQUIRE(p.eof());

    // Showing that it should throw an error when trying to get / peek from an
    // already-finished stream.
    REQUIRE_THROWS(p.get());
    REQUIRE_THROWS(p.peek());

    // Should through when you step back more than possible.
    REQUIRE_THROWS(p.stepBack(values.size() + 1));

    // Stepping back a valid amount.
    p.stepBack(values.size());

    // Should throw when you try to unget when you're already at the first
    // position.
    REQUIRE_THROWS(p.unget());

    // Requiring that you're both at the beginning and that peek doesn't change
    // the internal state of the parser.
    REQUIRE(p.peek() == values.at(0));
    REQUIRE(p.peek() == values.at(0));
}

// Testing out the string parser for a couple of functions.
TEST_CASE("StringParser") {
    parsical::StringParser p("abcdefg");
    std::vector<char> values { 'a', 'b', 'c', 'd', 'e', 'f', 'g' };

    testParser(p, values);
}

// Testing out a file parser in a similar way.
TEST_CASE("FileParser") {
    parsical::FileParser p("res/testfile.txt", 5);
    std::vector<char> values { 'a', 'b', 'c', 'd', 'e', 'f', 'g', '\n' };

    testParser(p, values);
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
