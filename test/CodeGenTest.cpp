#include "Parser.h"
#include "CodeGen.h"
#include <iostream>
#include <llvm/Support/raw_ostream.h>

void runTest(const std::string& input) {
    Lexer lexer(input);
    Parser parser(lexer);
    
    std::cout << "Compiling: '" << input << "'\n";
    auto expr = parser.ParseTopLevelExpr();
    if (!expr) {
        std::cerr << "FAIL: Failed to parse expression.\n";
        return;
    }
    
    auto IR = expr->codegen();
    if (IR) {
        std::cout << "Generated LLVM IR:\n";
        IR->print(llvm::outs());
        std::cout << "\n\n";
    } else {
        std::cerr << "Error generating IR.\n";
    }
}

int main() {
    InitializeModule();
    
    runTest("4 + 5");
    runTest("def foo(a, b) a * a + 2 * a * b + b * b");
    runTest("foo(1, 2)");
    runTest("def fib(x) if x < 3 then 1 else fib(x-1)+fib(x-2)");
    runTest("extern putchard(char); def printstar(n) for i = 1, i < n, 1.0 in putchard(42)");
    
    // Constant folding test
    std::cout << "--- OPTIMIZATION TEST ---\n";
    runTest("def test_opt(x) (1+2+x)*(x+(1+2))");
    
    return 0;
}
