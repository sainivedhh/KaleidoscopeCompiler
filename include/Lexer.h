#pragma once

#include <string>
#include <string_view>

// The lexer returns tokens [0-255] if it is an unknown character (like '+', '-', etc.), 
// otherwise one of these for known things.
enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,

    // control flow
    tok_if = -6,
    tok_then = -7,
    tok_else = -8,
    tok_for = -9,
    tok_in = -10
};

class Lexer {
public:
    // Initialize the lexer with a string input
    Lexer(std::string_view Input);

    // getNextToken - Return the next token from standard input.
    int getNextToken();

    // Accessors for token specific values
    const std::string& getIdentifierStr() const { return IdentifierStr; }
    double getNumVal() const { return NumVal; }

private:
    std::string_view InputString;
    size_t CurrentPos = 0;
    int LastChar = ' ';

    // State populated by getNextToken
    std::string IdentifierStr; // Filled in if tok_identifier
    double NumVal = 0.0;       // Filled in if tok_number

    // Helper to read the next char from the input string
    int advance();
};
