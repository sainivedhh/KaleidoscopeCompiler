#include "Lexer.h"
#include <iostream>
#include <vector>
#include <string>

// A simple test macro
#define ASSERT_EQ(actual, expected, msg) \
    if ((actual) != (expected)) { \
        std::cerr << "FAIL: " << msg << " | Expected: " << (expected) << ", Actual: " << (actual) << "\n"; \
        return false; \
    }

bool runTest(const std::string& input, const std::vector<int>& expectedTokens) {
    Lexer lexer(input);
    std::cout << "Testing: '" << input << "'\n";
    
    for (size_t i = 0; i < expectedTokens.size(); ++i) {
        int tok = lexer.getNextToken();
        if (tok != expectedTokens[i]) {
            std::cerr << "FAIL: Mismatch at token index " << i << ".\n";
            std::cerr << "Expected: " << expectedTokens[i] << ", Actual: " << tok << "\n";
            return false;
        }
    }
    
    // Ensure we hit EOF
    int finalTok = lexer.getNextToken();
    if (finalTok != tok_eof) {
        std::cerr << "FAIL: Expected EOF, got " << finalTok << "\n";
        return false;
    }

    std::cout << "PASS\n\n";
    return true;
}

int main() {
    int passed = 0;
    int failed = 0;

    auto test = [&](const std::string& input, const std::vector<int>& expected) {
        if (runTest(input, expected)) passed++; else failed++;
    };

    // Test simple math expression
    test("4 + 5", {tok_number, '+', tok_number});
    
    // Test function definition
    test("def foo(x) x * 2", {
        tok_def, tok_identifier, '(', tok_identifier, ')', 
        tok_identifier, '*', tok_number
    });

    // Test control flow keywords
    test("if x < 1 then x else 0", {
        tok_if, tok_identifier, '<', tok_number, 
        tok_then, tok_identifier, tok_else, tok_number
    });

    // Test comments
    test("def bar() # this is a comment\n 123", {
        tok_def, tok_identifier, '(', ')', tok_number
    });

    std::cout << "Tests Passed: " << passed << ", Failed: " << failed << "\n";
    return failed == 0 ? 0 : 1;
}
