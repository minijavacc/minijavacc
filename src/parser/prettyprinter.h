#pragma once

#include "node.h"

namespace cmpl {
  
  class PrettyPrinter : public Dispatcher {
  private:
    const std::string indent = "   ";
    std::ostream& printStream;
    std::string indents;
    bool indentsPrinted;
    
  public:
    PrettyPrinter(std::ostream& printStream) : printStream(printStream), indentsPrinted(false) { };
    void print(const std::string &s);
    void println(const std::string &s);
    void addIndent();
    void removeIndent();
    
    std::ostream& getStream();
    
    virtual void dispatch(Type &n);
    virtual void dispatch(UserType &n);
    virtual void dispatch(TypeInt &n);
    virtual void dispatch(TypeBoolean &n);
    virtual void dispatch(TypeVoid &n);
    virtual void dispatch(Program &n);
    virtual void dispatch(ClassDeclaration &n);
    virtual void dispatch(Field &n);
    virtual void dispatch(Method &n);
    virtual void dispatch(MainMethod &n);
    virtual void dispatch(Parameter &n);
    virtual void dispatch(Block &n);
    virtual void dispatch(IfStatement &n);
    virtual void dispatch(IfElseStatement &n);
    virtual void dispatch(ExpressionStatement &n);
    virtual void dispatch(WhileStatement &n);
    virtual void dispatch(LocalVariableDeclaration &n);
    virtual void dispatch(LocalVariableExpressionDeclaration &n);
    virtual void dispatch(ReturnStatement &n);
    virtual void dispatch(ReturnExpressionStatement &n);
    virtual void dispatch(EmptyStatement &n);
    virtual void dispatch(MethodInvocation &n);
    virtual void dispatch(ArrayAccess &n);
    virtual void dispatch(FieldAccess &n);
    virtual void dispatch(AssignmentExpression &n);
    virtual void dispatch(LogicalOrExpression &n);
    virtual void dispatch(LogicalAndExpression &n);
    virtual void dispatch(EqualityExpression &n);
    virtual void dispatch(RelationalExpression &n);
    virtual void dispatch(AdditiveExpression &n);
    virtual void dispatch(MultiplicativeExpression &n);
    virtual void dispatch(CallExpression &n);
    virtual void dispatch(UnaryLeftExpression &n);
    virtual void dispatch(UnaryRightExpression &n);
    virtual void dispatch(CNull &n);
    virtual void dispatch(CThis &n);
    virtual void dispatch(CTrue &n);
    virtual void dispatch(CFalse &n);
    virtual void dispatch(CRef &n);
    virtual void dispatch(CIntegerLiteral &n);
    virtual void dispatch(NewObject &n);
    virtual void dispatch(NewArray &n);
    virtual void dispatch(Equals &n);
    virtual void dispatch(NotEquals &n);
    virtual void dispatch(LessThan &n);
    virtual void dispatch(LessThanOrEqual &n);
    virtual void dispatch(GreaterThan &n);
    virtual void dispatch(GreaterThanOrEqual &n);
    virtual void dispatch(Add &n);
    virtual void dispatch(Subtract &n);
    virtual void dispatch(Multiply &n);
    virtual void dispatch(Divide &n);
    virtual void dispatch(Modulo &n);
    virtual void dispatch(Negate &n);
    virtual void dispatch(Minus &n);
  };
}
