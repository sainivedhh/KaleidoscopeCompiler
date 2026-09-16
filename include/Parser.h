#pragma once

#include "AST.h"
#include "Lexer.h"
#include <memory>
#include <map>

class Parser {
    Lexer& lex;
    int CurTok;
    std::map<char, int> BinopPrecedence;

    int getNextToken() {
        return CurTok = lex.getNextToken();
    }

    int GetTokPrecedence();

    std::unique_ptr<ExprAST> ParseNumberExpr();
    std::unique_ptr<ExprAST> ParseParenExpr();
    std::unique_ptr<ExprAST> ParseIdentifierExpr();
    std::unique_ptr<ExprAST> ParseIfExpr();
    std::unique_ptr<ExprAST> ParseForExpr();
    std::unique_ptr<ExprAST> ParsePrimary();
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
    
public:
    Parser(Lexer& lexer) : lex(lexer) {
        // Install standard binary operators.
        // 1 is lowest precedence.
        BinopPrecedence['<'] = 10;
        BinopPrecedence['+'] = 20;
        BinopPrecedence['-'] = 20;
        BinopPrecedence['*'] = 40; // highest.
        
        getNextToken(); // Prime the pump
    }

    std::unique_ptr<ExprAST> ParseExpression();
    std::unique_ptr<PrototypeAST> ParsePrototype();
    std::unique_ptr<FunctionAST> ParseDefinition();
    std::unique_ptr<FunctionAST> ParseTopLevelExpr();
    std::unique_ptr<PrototypeAST> ParseExtern();
};
