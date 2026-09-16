#include "Parser.h"
#include <iostream>

#define ASSERT_EQ(actual, expected, msg) \
    if ((actual) != (expected)) { \
        std::cerr << "FAIL: " << msg << "\nExpected:\n" << (expected) << "\nActual:\n" << (actual) << "\n"; \
        return false; \
    }

bool runTest(const std::string& input, const std::string& expectedDump) {
    Lexer lexer(input);
    Parser parser(lexer);
    
    std::cout << "Testing: '" << input << "'\n";
    auto expr = parser.ParseExpression();
    if (!expr) {
        std::cerr << "FAIL: Failed to parse expression.\n";
        return false;
    }
    
    std::string actualDump = expr->dump();
    ASSERT_EQ(actualDump, expectedDump, "AST structure mismatch");
    std::cout << "PASS\n\n";
    return true;
}

int main() {
    int passed = 0;
    int failed = 0;

    auto test = [&](const std::string& input, const std::string& expected) {
        if (runTest(input, expected)) passed++; else failed++;
    };

    // Test simple binary op
    test("4 + 5", 
"BinaryExpr(+)\n\
  NumberExpr(4)\n\
  NumberExpr(5)");

    // Test operator precedence: * binds tighter than +
    test("1 + 2 * 3", 
"BinaryExpr(+)\n\
  NumberExpr(1)\n\
  BinaryExpr(*)\n\
    NumberExpr(2)\n\
    NumberExpr(3)");

    // Test function call
    test("foo(x, 2)", 
"CallExpr(foo)\n\
  VariableExpr(x)\n\
  NumberExpr(2)");

    std::cout << "Tests Passed: " << passed << ", Failed: " << failed << "\n";
    return failed == 0 ? 0 : 1;
}
