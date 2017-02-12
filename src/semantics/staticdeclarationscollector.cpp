#include "staticdeclarationscollector.h"

#include "../structures/stringtable.h"

#include <iostream>
#include "../structures/types.h"

using namespace cmpl;

// helper
inline void StaticDeclarationsCollector::errorMultipleNames(const std::string &err, StringIdentifier ID) {
  throw CollectorError(("staticdeclarationscollector: cannot have multiple " + err + " with the same name " + StringTable::lookupIdentifier(ID)).c_str());
}


void StaticDeclarationsCollector::dispatch(std::shared_ptr<Program> n) {
  for(auto const& c: n->classDeclarations) {
    c->accept(shared_from_this());
    classes.emplace(c->ID,c);
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<ClassDeclaration> n) {
  currentClassDeclaration = n;
  
  if(classes.count(n->ID)>0) {
    errorMultipleNames("class declarations", n->ID);
  } else {
    for (auto const& c : n->classMembers) {
      c->accept(shared_from_this());
    }
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<Field> n) {
  if (currentClassDeclaration->fields.count(n->ID) > 0) {
    errorMultipleNames("fields", n->ID);
  } else {
    currentClassDeclaration->fields.emplace(n->ID, n);
  }
};


void StaticDeclarationsCollector::dispatch(std::shared_ptr<MainMethod> n) {
  if (currentClassDeclaration->methods.count(n->ID) > 0) {
    errorMultipleNames("methods", n->ID);
  } else {
    /* TODO add identifier of main into current methods, so that name conflicts can be noticed
    std::vector<std::shared_ptr<Parameter>> parameters;
    std::vector<std::shared_ptr<BlockStatement>> statements;
    std::shared_ptr<Block> block = std::make_shared<Block>(statements);
    currentClassDeclaration->methods.emplace(std::make_shared<Method>(fakeType, n->ID, parameters, block));
    */
    
    mainMethod = n;
    
    // collect local variables
    n->block->accept(shared_from_this());
    
    mainMethod = nullptr;
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<Method> n) {
  if (currentClassDeclaration->methods.count(n->ID) > 0) {
    errorMultipleNames("methods", n->ID);
    return;
  }
  
  currentClassDeclaration->methods.emplace(n->ID, n);
  
  // collect parameters
  currentMethod = n;
  
  for (auto const& p : n->parameters) {
    p->accept(shared_from_this());
  }
  
  // collect local variables
  n->block->accept(shared_from_this());
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<Parameter> n) {
  if (currentMethod->parameterMap.count(n->ID) > 0) {
    errorMultipleNames("parameters", n->ID);
  } else {
    currentMethod->parameterMap.emplace(n->ID, n);
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<Block> n) {
  for (auto const& s: n->statements) {
    s->accept(shared_from_this());
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<LocalVariableDeclaration> n) {
  if (mainMethod) {
    mainMethod->localVariables.push_back(n);
  } else {
    currentMethod->localVariables.push_back(n);
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<LocalVariableExpressionDeclaration> n) {
  if (mainMethod) {
    mainMethod->localVariables.push_back(n);
  } else {
    currentMethod->localVariables.push_back(n);
  }
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<IfStatement> n) {
  n->ifStatement->accept(shared_from_this());
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<WhileStatement> n) {
  n->statement->accept(shared_from_this());
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<IfElseStatement> n) {
  n->ifStatement->accept(shared_from_this());
  n->elseStatement->accept(shared_from_this());
};

void StaticDeclarationsCollector::dispatch(std::shared_ptr<Type> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<FakeType> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<NullType> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<UserType> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<TypeInt> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<TypeBoolean> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<TypeVoid> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<ExpressionStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<EmptyStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<ReturnStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<ReturnExpressionStatement> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<MethodInvocation> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<ArrayAccess> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<FieldAccess> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<AssignmentExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LogicalOrExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LogicalAndExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<EqualityExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<RelationalExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<AdditiveExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<MultiplicativeExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CallExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<UnaryLeftExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<UnaryRightExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CNull> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CThis> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CTrue> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CFalse> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CRef> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<CIntegerLiteral> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<NewObject> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<NewArray> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<SLCPrintlnExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<SLCWriteExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<SLCFlushExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<SLCReadExpression> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Equals> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<NotEquals> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LessThan> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<LessThanOrEqual> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<GreaterThan> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<GreaterThanOrEqual> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Add> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Subtract> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Multiply> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Divide> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Modulo> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Negate> n) { };
void StaticDeclarationsCollector::dispatch(std::shared_ptr<Minus> n) { };
