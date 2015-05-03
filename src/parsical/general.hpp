// Name: parsical/general.hpp
//
// Description:
//   This file contains functions to run on template-agnostic parsers.

#ifndef _PARSICAL_GENERAL_HPP_
#define _PARSICAL_GENERAL_HPP_

//////////////
// Includes //
#include <vector>

#include "parsestream.hpp"
#include "parseerror.hpp"

//////////
// Code //

namespace parsical {
    // Attempting to perform a parse operation. If it throws a ParseError, it
    // automatically backs up to its position before the parse operation and
    // rethrows the ParseError.
    template <typename ReturnType,
              typename ParserType,
              typename FunctionType>
    ReturnType tryParse(ParseStream<ParserType>&, FunctionType) throw(ParseError);

    // Getting a vector of whatever the ParserType is based on some predicate.
    // If the end of the stream is reached, it just returns all recorded values.
    template <typename ParserType,
              typename FunctionType>
    std::vector<ParserType> takeWhile(ParseStream<ParserType>&, FunctionType);

    // The inverse of takeWhile - so long as a predicate is not true, it will
    // take a new value.
    template <typename ParserType,
              typename FunctionType>
    std::vector<ParserType> takeUntil(ParseStream<ParserType>&, FunctionType);

    // Ignoring characters in a stream while a predicate is true. It stops when
    // the predicate fails to be true for a character, or when the end of the
    // stream is reached.
    template <typename ParserType,
              typename FunctionType>
    void dropWhile(ParseStream<ParserType>&, FunctionType);

    // The inverse of dropWhile - so long as a predicate is not true, it will
    // drop new values.
    template <typename ParserType,
              typename FunctionType>
    void dropUntil(ParseStream<ParserType>&, FunctionType);

    // Given a set of possible values, it attempts to match the next value in
    // the ParseStream. If it succeeds, it returns that value. If it fails it
    // returns a new value and backs up to its previous position.
    template <typename ParserType>
    ParserType oneOf(ParseStream<ParserType>&, const std::set<ParserType>&) throw(ParseError);

    // Given a set of possible values, it attempts to match the next value in
    // the ParseStream. If it is not in the set, it succeeds, and it returns
    // that value. If it fails it returns a new value and backs up to its
    // previous position.
    template <typename ParserType>
    ParserType noneOf(ParseStream<ParserType>&, const std::set<ParserType>&) throw(ParseError);

    // Attempting to match many of a function on a parser.
    template <typename ReturnType,
              typename ParserType,
              typename FunctionType>
    std::vector<ReturnType> many(ParseStream<ParserType>&, FunctionType) throw(ParseError);
}

#include "general.tpp"

#endif
