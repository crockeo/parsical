// Name: parsical/general.hpp
//
// Description:
//   This file contains functions to run on template-agnostic parsers.

#ifndef _PARSICAL_GENERAL_HPP_
#define _PARSICAL_GENERAL_HPP_

//////////////
// Includes //
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
}

#include "general.tpp"

#endif
