#include "general.hpp"

// Attempting to perform a parse operation. If it throws a ParseError, it
// automatically backs up to its position before the parse operation and
// rethrows the ParseError.
template <typename ReturnType,
          typename ParserType,
          typename FunctionType>
ReturnType parsical::tryParse(parsical::ParseStream<ParserType>& stream, FunctionType fn) throw(ParseError) {
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
std::vector<ParserType> parsical::takeWhile(parsical::ParseStream<ParserType>& stream, FunctionType fn) {
    std::vector<ParserType> ret;

    while (!stream.eof() && fn(stream.peek()))
        ret.push_back(stream.get());

    return ret;
}

// The inerse of takeWhile - so long as a predicate is not true, it will
// take a new value.
template <typename ParserType,
          typename FunctionType>
std::vector<ParserType> parsical::takeUntil(parsical::ParseStream<ParserType>& stream, FunctionType fn) {
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
void parsical::dropWhile(parsical::ParseStream<ParserType>& stream, FunctionType fn) {
    while (!stream.eof() && fn(stream.peek()))
        stream.get();
}

// The inerse of dropWhile - so long as a predicate is not true, it will
// drop new values.
template <typename ParserType,
          typename FunctionType>
void parsical::dropUntil(parsical::ParseStream<ParserType>& stream, FunctionType fn) {
    while (!stream.eof() && !fn(stream.peek()))
        stream.get();
}

// Given a set of possible values, it attempts to match the next value in
// the parsical::ParseStream. If it succeeds, it returns that value. If it fails it
// returns a new value and backs up to its previous position.
template <typename ParserType>
ParserType parsical::oneOf(parsical::ParseStream<ParserType>& stream, const std::set<ParserType>& set) throw(parsical::ParseError) {
    if (set.find(stream.peek()) == set.end())
        throw parsical::ParseError("Value is not in the set of appropriate values.");
    return stream.get();
}

// Given a set of possible values, it attempts to match the next value in
// the parsical::ParseStream. If it is not in the set, it succeeds, and it returns
// that value. If it fails it returns a new value and backs up to its
// previous position.
template <typename ParserType>
ParserType parsical::noneOf(parsical::ParseStream<ParserType>& stream, const std::set<ParserType>& set) throw(parsical::ParseError) {
    if (set.find(stream.peek()) != set.end())
        throw parsical::ParseError("Value is in the set of inappropriate values.");
    return stream.get();
}

// Attempting to match many of a function on a parser.
template <typename ReturnType,
          typename ParserType,
          typename FunctionType>
std::vector<ReturnType> parsical::many(parsical::ParseStream<ParserType>& stream, FunctionType fn) throw(parsical::ParseError) {
    std::vector<ReturnType> values;

    bool good = true;
    while (good) {
        try {
            values.push_back(fn(stream));
        } catch (parsical::ParseError& e) { good = false; }
    }

    return values;
}

// Attempting to match many of a function on a parser. Will fail if no parses
// succeed.
template <typename ReturnType,
          typename ParserType,
          typename FunctionType>
std::vector<ReturnType> parsical::manyOne(parsical::ParseStream<ParserType>& stream, FunctionType fn) throw(parsical::ParseError) {
    std::vector<ReturnType> values = parsical::many<ReturnType>(stream, fn);
    if (values.size() == 0)
        throw parsical::ParseError("manyOne: no parses succeeded.");
    return values;
}
