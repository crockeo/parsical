//////////////
// Includes //
#include <functional>
#include <iostream>

#include "catch.hpp"

#include "../parsical.hpp"

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
    //parsical::FileParser p("res/testfile.txt", 5);
    //std::vector<char> values { 'a', 'b', 'c', 'd', 'e', 'f', 'g', '\n' };

    //testParser(p, values);
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

    REQUIRE_THROWS(parsical::tryParse<std::vector<char>>(p, [](parsical::ParseStream<char>& stream) -> std::vector<char> {
        return parsical::manyOne<char>(stream, [](parsical::ParseStream<char>& stream) -> char {
            char c = stream.get();
            throw parsical::ParseError("Did the thing a second time.");
            return c;
        });
    }));

    REQUIRE(p.pos() == 0);
}

// Attempting to perform a takeWhile on a parser.
TEST_CASE("takeWhile") {
    parsical::StringParser p("aaabcdeeeef");

    std::vector<char> testValues { 'a', 'a', 'a' };
    REQUIRE(takeWhile(p, [](char c) -> bool { return c == 'a'; }) == testValues);

    REQUIRE(p.get() == 'b');
    REQUIRE(p.get() == 'c');
    REQUIRE(p.get() == 'd');

    std::vector<char> testValues2 { 'e', 'e', 'e', 'e', 'f' };
    REQUIRE(takeWhile(p, [](char c) -> bool { return c == 'e' || c == 'f'; }) == testValues2);
}

// Attemping to perform a takeUntil on a parser.
TEST_CASE("takeUntil") {
    parsical::StringParser p("aaabcdeeeef");

    std::vector<char> testValues { 'a', 'a', 'a', 'b', 'c', 'd' };
    REQUIRE(takeUntil(p, [](char c) -> bool { return c == 'e'; }) == testValues);
}

// Attempting to perform a dropWhile on a parser.
TEST_CASE("dropWhile") {
    parsical::StringParser p("aaabcdeeeef");

    dropWhile(p, [](char c) -> bool { return c == 'a'; });
    
    REQUIRE(p.get() == 'b');
    REQUIRE(p.get() == 'c');
    REQUIRE(p.get() == 'd');

    dropWhile(p, [](char c) -> bool { return c == 'e' || c == 'f'; });

    REQUIRE(p.eof());
}

// Attempting to perform a dropUntil on a parser.
TEST_CASE("dropUntil") {
    parsical::StringParser p("aaabcdeeeef");

    dropUntil(p, [](char c) -> bool { return c == 'e'; });

    REQUIRE(p.get() == 'e');
    REQUIRE(p.get() == 'e');
    REQUIRE(p.get() == 'e');
    REQUIRE(p.get() == 'e');
    REQUIRE(p.get() == 'f');
}

// Attempting to perform oneOf and noneOf operations on a parser.
TEST_CASE("oneOf & noneOf") {
    parsical::StringParser p("aaabcdeeeef");
    std::set<char> set { 'a', 'e' };

    REQUIRE(oneOf(p, set) == 'a');
    REQUIRE(oneOf(p, set) == 'a');
    REQUIRE_THROWS(noneOf(p, set));
    REQUIRE(oneOf(p, set) == 'a');

    REQUIRE(noneOf(p, set) == 'b');
    REQUIRE(noneOf(p, set) == 'c');
    REQUIRE(noneOf(p, set) == 'd');
}

// Attempting to perform a many. First on a new function, and then by attempting
// to bind an already-made parsing function.
TEST_CASE("many") {
    parsical::StringParser p("aaabcdeeeef");

    std::vector<int> test0 { };
    REQUIRE(parsical::many<int>(p, [](parsical::ParseStream<char>& c) -> int {
        throw parsical::ParseError("nothing");
    }) == test0);

    std::vector<int> test1 { 97, 97, 97 };
    REQUIRE(parsical::many<int>(p, [](parsical::ParseStream<char>& c) -> int {
        if (c.peek() != 'a')
            throw parsical::ParseError("DONE");
        return static_cast<int>(c.get());
    }) == test1);

    REQUIRE(p.get() == 'b');
    REQUIRE(p.get() == 'c');
    REQUIRE(p.get() == 'd');

    std::set<char> set { 'e', 'f' };
    std::vector<char> test2 { 'e', 'e', 'e', 'e', 'f' };
    REQUIRE(parsical::many<char>(p, std::bind(parsical::oneOf<char>, std::placeholders::_1, set)) == test2);
}

// Attempting to perform a manyOne.
TEST_CASE("manyOne") {
    parsical::StringParser p("aaabcdeeeef");

    REQUIRE_THROWS(parsical::manyOne<int>(p, [](parsical::ParseStream<char>& c) -> int {
        throw parsical::ParseError("nothing");
    }));

    std::vector<int> test1 { 97, 97, 97 };
    REQUIRE(parsical::manyOne<int>(p, [](parsical::ParseStream<char>& c) -> int {
        if (c.peek() != 'a')
            throw parsical::ParseError("DONE");
        return static_cast<int>(c.get());
    }) == test1);

    REQUIRE(p.get() == 'b');
    REQUIRE(p.get() == 'c');
    REQUIRE(p.get() == 'd');

    std::set<char> set { 'e', 'f' };
    std::vector<char> test2 { 'e', 'e', 'e', 'e', 'f' };
    REQUIRE(parsical::manyOne<char>(p, std::bind(parsical::oneOf<char>, std::placeholders::_1, set)) == test2);
}

// Attempting to perform an option.
TEST_CASE("option") {
    parsical::StringParser p("aaabcdeeeef");

    // Creating a comparisong function.
    auto makeFn = [](char comp) -> std::function<char(parsical::ParseStream<char>&)> {
        return [=](parsical::ParseStream<char>& stream) -> char {
            char c = stream.get();
            if (c != comp)
                throw parsical::ParseError();
            return c;
        };
    };

    // Creating a series of functions.
    std::vector<std::function<char(parsical::ParseStream<char>&)>> fns {
        makeFn('a'),
        makeFn('b'),
        makeFn('c')
    };

    REQUIRE(parsical::option<char>(p, fns) == 'a');
    REQUIRE(parsical::option<char>(p, fns) == 'a');
    REQUIRE(parsical::option<char>(p, fns) == 'a');
    REQUIRE(parsical::option<char>(p, fns) == 'b');
    REQUIRE(parsical::option<char>(p, fns) == 'c');

    int pos = p.pos();
    REQUIRE_THROWS(parsical::option<char>(p, fns));
    REQUIRE(pos == p.pos());
}

////
// string.hpp

// Testing the parsical::str::string function.
TEST_CASE("string") {
    parsical::StringParser p("aaabcdeeeef");

    REQUIRE(parsical::str::string(p, "aaabcd") == "aaabcd");
    REQUIRE_THROWS(parsical::tryParse<std::string>(p, std::bind(parsical::str::string, std::placeholders::_1, "eef")));
    REQUIRE_THROWS(parsical::str::string(p, "eeeea"));
    REQUIRE(p.get() == 'f');
}

// Testing the str::takeWhile function.
TEST_CASE("str::takeWhile") {
    parsical::StringParser p("aaabcdeeeef");

    REQUIRE(parsical::str::takeWhile(p, [](char c) -> bool { return c == 'a'; }) == "aaa");
    REQUIRE(parsical::str::takeWhile(p, [](char c) -> bool { return c == 'a'; }) == "");
    REQUIRE(parsical::str::takeWhile(p, [](char c) -> bool {
        switch (c) {
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return true;
        default:
            return false;
        }
    }) == "bcdeeeef");
}

// Testing the str::takeUntil function.
TEST_CASE("str::takeUntil") {
    parsical::StringParser p("aaabcdeeeef");

    REQUIRE(parsical::str::takeUntil(p, [](char c) -> bool { return c == 'a'; }) == "");
    REQUIRE(parsical::str::takeUntil(p, [](char c) -> bool { return c == 'f'; }) == "aaabcdeeee");
    REQUIRE(parsical::str::takeUntil(p, [](char c) -> bool { return c == '\0'; }) == "f");
}

// Testing the parseBool function.
TEST_CASE("parseBool") {
    parsical::StringParser p("atruefalse");

    REQUIRE_THROWS(parsical::str::parseBool(p));
    p.get();

    REQUIRE(parsical::str::parseBool(p) == true);
    REQUIRE(parsical::str::parseBool(p) == false);
}
