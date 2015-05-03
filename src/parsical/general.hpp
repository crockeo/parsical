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

    // The inerse of takeWhile - so long as a predicate is not true, it will
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

    // The inerse of dropWhile - so long as a predicate is not true, it will
    // drop new values.
    template <typename ParserType,
              typename FunctionType>
    void dropUntil(ParseStream<ParserType>&, FunctionType);
}

#include "general.tpp"

#endif
