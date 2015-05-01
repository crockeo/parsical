// Name: parsical/parsestream.hpp
//
// Description:
//   This contains a generic parse stream API with a couple of implementations
//   for different situations.

#ifndef _PARSICAL_PARSER_STREAM_HPP_
#define _PARSICAL_PARSER_STREAM_HPP_

//////////////
// Includes //
#include <istream>
#include <string>

#include "parseerror.hpp"

//////////
// Code //

namespace parsical {
    // The generic ParseStream interface.
    template <typename T>
    struct ParseStream {
        // Virtual destructor to preemptively eliminate any problems with
        // inherited deconstruction.
        virtual ~ParseStream() { }

        // Checking whether this ParseStream has reached its end.
        virtual bool eof() const noexcept = 0;

        // Peeking at the next value without consuming it.
        virtual T peek() const throw(ParseError) = 0;

        // Getting the current position in this ParseStream.
        virtual int pos() const noexcept = 0;

        // Consuming and returning a value.
        virtual T get() throw(ParseError) = 0;

        // Stepping back some interval.
        virtual void stepBack(int) throw(ParseError) = 0;

        // Un-getting a single character. It ought to be equivalent to
        // stepBack(1)
        virtual void unget() throw(ParseError) { stepBack(1); }
    };

    // A parser specifically desinged around parsing a string.
    class StringParser : public ParseStream<char> {
    private:
        std::string str;
        int p;

    public:
        // Constructing a StringParser from a given string.
        StringParser(std::string);

        // Checking whether this ParseStream has reached its end.
        virtual bool eof() const noexcept override;

        // Peeking at the next value without consuming it.
        virtual char peek() const throw(ParseError) override;

        // Getting the current position in this ParseStream.
        virtual int pos() const noexcept override;

        // Consuming and returning a value.
        virtual char get() throw(ParseError) override;

        // Stepping back some interval.
        virtual void stepBack(int) throw(ParseError) override;

        // Un-getting a single character. It ought to be equivalent to
        // stepBack(1)
        virtual void unget() throw(ParseError) override;
    };
}

#endif
