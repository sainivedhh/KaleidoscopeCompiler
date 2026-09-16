#pragma once

#include <string>
#include <memory>
#include <vector>
#include <sstream>

namespace llvm {
class Value;
class Function;
}

// Helper for indentation
inline std::string indentStr(int indent) {
    return std::string(indent * 2, ' ');
}

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual std::string dump(int indent = 0) const = 0;
    virtual llvm::Value *codegen() = 0;
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
    
    std::string dump(int indent = 0) const override {
        std::ostringstream os;
        os << indentStr(indent) << "NumberExpr(" << Val << ")";
        return os.str();
    }
    
    llvm::Value *codegen() override;
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
    
    std::string dump(int indent = 0) const override {
        return indentStr(indent) + "VariableExpr(" + Name + ")";
    }
    
    llvm::Value *codegen() override;
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        
    std::string dump(int indent = 0) const override {
        std::ostringstream os;
        os << indentStr(indent) << "BinaryExpr(" << Op << ")\n";
        os << LHS->dump(indent + 1) << "\n";
        os << RHS->dump(indent + 1);
        return os.str();
    }
    
    llvm::Value *codegen() override;
};

/// IfExprAST - Expression class for if/then/else.
class IfExprAST : public ExprAST {
    std::unique_ptr<ExprAST> Cond, Then, Else;

public:
    IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then,
              std::unique_ptr<ExprAST> Else)
        : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}

    std::string dump(int indent = 0) const override {
        std::ostringstream os;
        os << indentStr(indent) << "IfExpr:\n";
        os << indentStr(indent + 1) << "Cond:\n" << Cond->dump(indent + 2) << "\n";
        os << indentStr(indent + 1) << "Then:\n" << Then->dump(indent + 2) << "\n";
        os << indentStr(indent + 1) << "Else:\n" << Else->dump(indent + 2);
        return os.str();
    }

    llvm::Value *codegen() override;
};

/// ForExprAST - Expression class for for/in.
class ForExprAST : public ExprAST {
    std::string VarName;
    std::unique_ptr<ExprAST> Start, End, Step, Body;

public:
    ForExprAST(const std::string &VarName, std::unique_ptr<ExprAST> Start,
               std::unique_ptr<ExprAST> End, std::unique_ptr<ExprAST> Step,
               std::unique_ptr<ExprAST> Body)
        : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
          Step(std::move(Step)), Body(std::move(Body)) {}

    std::string dump(int indent = 0) const override {
        std::ostringstream os;
        os << indentStr(indent) << "ForExpr(" << VarName << "):\n";
        os << indentStr(indent + 1) << "Start:\n" << Start->dump(indent + 2) << "\n";
        os << indentStr(indent + 1) << "End:\n" << End->dump(indent + 2) << "\n";
        if (Step)
            os << indentStr(indent + 1) << "Step:\n" << Step->dump(indent + 2) << "\n";
        os << indentStr(indent + 1) << "Body:\n" << Body->dump(indent + 2);
        return os.str();
    }

    llvm::Value *codegen() override;
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args)
        : Callee(Callee), Args(std::move(Args)) {}
        
    std::string dump(int indent = 0) const override {
        std::ostringstream os;
        os << indentStr(indent) << "CallExpr(" << Callee << ")";
        for (const auto& arg : Args) {
            os << "\n" << arg->dump(indent + 1);
        }
        return os.str();
    }
    
    llvm::Value *codegen() override;
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names.
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {}

    const std::string &getName() const { return Name; }
    
    std::string dump(int indent = 0) const {
        std::ostringstream os;
        os << indentStr(indent) << "Prototype(" << Name;
        for (const auto& arg : Args) {
            os << ", " << arg;
        }
        os << ")";
        return os.str();
    }
    
    llvm::Function *codegen();
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}
        
    std::string dump(int indent = 0) const {
        std::ostringstream os;
        os << indentStr(indent) << "Function:\n";
        os << Proto->dump(indent + 1) << "\n";
        if (Body) {
            os << Body->dump(indent + 1);
        } else {
            os << indentStr(indent + 1) << "(extern)";
        }
        return os.str();
    }
    
    llvm::Function *codegen();
};
