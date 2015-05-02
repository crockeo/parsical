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
