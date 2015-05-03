#include "general.hpp"

// Attempting to perform a parse operation. If it throws a ParseError, it
// automatically backs up to its position before the parse operation and
// rethrows the ParseError.
template <typename ReturnType,
          typename ParserType,
          typename FunctionType>
ReturnType parsical::tryParse(ParseStream<ParserType>& stream, FunctionType fn) throw(ParseError) {
    int pos = stream.pos();
    try {
        return fn(stream);
    } catch (ParseError& e) {
        stream.stepBack(stream.pos() - pos);
        throw e;
    }
}

// Getting a vector of whatever the ParserType is based on some predicate.
// If the end of the stream is reached, it just returns all recorded values.
template <typename ParserType,
          typename FunctionType>
std::vector<ParserType> parsical::takeWhile(ParseStream<ParserType>& stream, FunctionType fn) {
    std::vector<ParserType> ret;

    while (!stream.eof() && fn(stream.peek()))
        ret.push_back(stream.get());

    return ret;
}

// The inerse of takeWhile - so long as a predicate is not true, it will
// take a new value.
template <typename ParserType,
          typename FunctionType>
std::vector<ParserType> parsical::takeUntil(ParseStream<ParserType>& stream, FunctionType fn) {
    std::vector<ParserType> ret;

    while (!stream.eof() && !fn(stream.peek()))
        ret.push_back(stream.get());

    return ret;
}

// Ignoring characters in a stream while a predicate is true. It stops when
// the predicate fails to be true for a character, or when the end of the
// stream is reached.
template <typename ParserType,
          typename FunctionType>
void parsical::dropWhile(ParseStream<ParserType>& stream, FunctionType fn) {
    while (!stream.eof() && fn(stream.peek()))
        stream.get();
}

// The inerse of dropWhile - so long as a predicate is not true, it will
// drop new values.
template <typename ParserType,
          typename FunctionType>
void parsical::dropUntil(ParseStream<ParserType>& stream, FunctionType fn) {
    while (!stream.eof() && !fn(stream.peek()))
        stream.get();
}