#include "parser.h"

using namespace cmpl;


/************************ helper functions **************/

inline void Parser::error(const std::string &err)
{
  throw SemanticError(std::to_string(currentToken->line) + " " + std::to_string(currentToken->column) + ": " + err);
}

// if token array of lexer is empty (method returns false) throw semanticError for now,
// later depends on implementation of lexer running in parallel (to avoid polling)
inline void Parser::nextToken()
{
  if(!lexer.getNextToken(currentToken)) {
    error("Unexpectedly ran out of tokens");
  }
  //std::cout<< "next token: " << currentToken->getStringValue() << "\n";
}

inline StringIdentifier Parser::getIdentifierFromCurrent() {
  IdentifierToken* id_t;
  if (id_t = dynamic_cast<IdentifierToken*>(currentToken.get())) {
    return id_t->id;
  } else {
    error("Expected identifier, current token is " + currentToken->getStringValue());
      return 0;
  }
}

inline StringIdentifier Parser::getIdentifierFromNext() {
  nextToken();
  return std::move(getIdentifierFromCurrent());
}

// Checks whether next token is of specified type T, trows exception in case that not
template<typename T>
inline void Parser::assureCurrentTokenTypeIs()
{
  if (!isCurrentTokenOfType<T>())
  {
    error(currentToken->getStringValue() + ", but expected a " + *typeid(T).name());
  }
}

// Checks whether next token is of specified type T, trows exception in case that not
template<typename T>
inline void Parser::assureNextTokenTypeIs()
{
  nextToken();
  assureCurrentTokenTypeIs<T>();
}

// combines assureNextTokenTypeIs with checking for specialized sub-type of operator/seperator tokens
inline void Parser::assureCurrentIsOSKTokenWithType(const TokenType& tokenType)
{
  if(!isCurrentTokenOfType<OperatorSeperatorKeywordToken>()) {
    error("expected " + Token::tokenAttribues[tokenType].stringRepresentation);
  } else if(!isCurrentTokenOSKTokenOfType(tokenType)) {
    OperatorSeperatorKeywordToken* osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(currentToken.get());
    error("expected " + Token::tokenAttribues[tokenType].stringRepresentation + " got " + Token::tokenAttribues[osk_t->type].stringRepresentation);
  }
}

// combines assureNextTokenTypeIs with checking for specialized sub-type of operator/seperator tokens
inline void Parser::assureNextIsOSKTokenWithType(const TokenType& tokenType)
{
  nextToken();
  assureCurrentIsOSKTokenWithType(tokenType);
}


// Checks whether current token is of specified type T
template<typename T>
inline bool Parser::isCurrentTokenOfType() {
  return dynamic_cast<T*>(currentToken.get());
}

// Checks whether next token is of specified type T
template<typename T>
inline bool Parser::isNextTokenOfType() {
  nextToken();
  return isCurrentTokenOfType<T>();
}

// combines isCurrentTokenOfType with checking for specialized sub-type of operator/seperator tokens
inline bool Parser::isCurrentTokenOSKTokenOfType(const TokenType& tokenType) {
  OperatorSeperatorKeywordToken* osk_t;
  return (osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(currentToken.get()))
          && (osk_t->type == tokenType);
}

// combines isCurrentTokenOfType with checking for specialized sub-type of operator/seperator tokens
inline bool Parser::isCurrentTokenOSKTokenOfCategory(const TokenCategory& tokenCategory, TokenType& tokenTypeOut) {
  OperatorSeperatorKeywordToken* osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(currentToken.get());
  if (!osk_t)
  {
    return false;
  }
  
  tokenTypeOut = osk_t->type;
  
  return (Token::tokenAttribues[osk_t->type].category == binaryOperator);
}

// combines isNextTokenOfType with checking for specialized sub-type of operator/seperator tokens
inline bool Parser::isNextTokenOSKTokenOfType(const TokenType& tokenType) {
  nextToken();
  return isCurrentTokenOSKTokenOfType(tokenType);
}


/************************ end helper functions **************/


void Parser::run()
{
  ast = parseProgram();
}

/* start: current = "class" */
std::shared_ptr<Program> Parser::parseProgram()
{
  std::vector<std::shared_ptr<ClassDeclaration>> classes;
  
  // multiple classes
  while(lexer.hasNextToken()) {
    assureNextIsOSKTokenWithType(T_K_CLASS);
    classes.push_back(std::move(parseClassDeclaration()));
  }
  
  return std::make_shared<Program>(classes);
}

/* start: current = "class" */
std::shared_ptr<ClassDeclaration> Parser::parseClassDeclaration()
{
  std::vector<std::shared_ptr<ClassMember>> classMembers;
  StringIdentifier ID;
  
  ID = getIdentifierFromNext();
  assureNextIsOSKTokenWithType(T_O_LBRACE);
  nextToken();
  // multiple class members
  while(!isCurrentTokenOSKTokenOfType(T_O_RBRACE)) {
    classMembers.push_back(std::move(parseClassMember()));
  }

  return std::make_shared<ClassDeclaration>(ID, classMembers);
}

/* start: current = "public" */
std::shared_ptr<ClassMember> Parser::parseClassMember()
{
  assureCurrentIsOSKTokenWithType(T_K_PUBLIC);
  
  if(isNextTokenOSKTokenOfType(T_K_STATIC)) {
    // MainMethod
    StringIdentifier ID;
    StringIdentifier parameterID;
    std::shared_ptr<Block> block;
    
    assureNextIsOSKTokenWithType(T_K_VOID);
    
    ID = getIdentifierFromNext();
    /* if(StringTable::lookupIdentifier(ID) != "main") {
      error("Only method allowed to be static is main method!");
    }*/
    assureNextIsOSKTokenWithType(T_O_LPAREN);
    StringIdentifier stringID = getIdentifierFromNext();
    if(StringTable::lookupIdentifier(stringID) != "String") {
      error("Main method needs String as parameter type!");
    }
    assureNextIsOSKTokenWithType(T_O_LBRACK);
    assureNextIsOSKTokenWithType(T_O_RBRACK);
    parameterID = getIdentifierFromNext();
    assureNextIsOSKTokenWithType(T_O_RPAREN);
    nextToken();
    block = parseBlock();
    return std::make_shared<MainMethod>(ID, parameterID, block);
  } else {
    std::shared_ptr<Type> type = parseType();
    StringIdentifier ID = getIdentifierFromCurrent();
    if(isNextTokenOSKTokenOfType(T_O_SEMICOLON)) {
      // Field
      nextToken();
      return std::make_shared<Field>(type, ID);
    } else if(isCurrentTokenOSKTokenOfType(T_O_LPAREN)) {
      // Method
      std::vector<std::shared_ptr<Parameter>> parameters;
      std::shared_ptr<Block> block;
      nextToken(); // (
      bool continuous = false;
      while(!isCurrentTokenOSKTokenOfType(T_O_RPAREN)) {
        if(continuous) {
          assureCurrentIsOSKTokenWithType(T_O_COMMA);
          nextToken(); // ,
        }
        parameters.push_back(std::move(parseParameter()));
        continuous = true;
      }
      nextToken();
      block = parseBlock();
      return std::make_shared<Method>(type, ID, parameters, block);
    } else {
      error("Neither Field nor Method definition");
    }
  }
    return nullptr;
}

std::shared_ptr<Type> Parser::parseType()
{
  std::shared_ptr<BasicType> type = parseBasicType();
  int arrayDepth = 0;
  
  while(isCurrentTokenOSKTokenOfType(T_O_LBRACK)) {
    assureNextIsOSKTokenWithType(T_O_RBRACK);
    ++arrayDepth;
    nextToken();
  }
  
  return std::make_shared<Type>(type, arrayDepth);
}

std::shared_ptr<BasicType> Parser::parseBasicType()
{
  if(isCurrentTokenOSKTokenOfType(T_K_BOOLEAN)) {
    nextToken();
    return std::make_shared<TypeBoolean>();
  } else if(isCurrentTokenOSKTokenOfType(T_K_INT)) {
    nextToken();
    return std::make_shared<TypeInt>();
  } else if(isCurrentTokenOSKTokenOfType(T_K_VOID)) {
    nextToken();
    return std::make_shared<TypeVoid>();
  } else {
    StringIdentifier ID = getIdentifierFromCurrent();
    nextToken();
    return std::make_shared<UserType>(ID);
  }
}

/* start: current = Type */
std::shared_ptr<Parameter> Parser::parseParameter()
{
  std::shared_ptr<Type> type = parseType();

  StringIdentifier ID = getIdentifierFromCurrent();
  nextToken();
  return std::make_shared<Parameter>(type, ID);
}

/* start: current = "{" */
std::shared_ptr<Block> Parser::parseBlock()
{
  std::vector<std::shared_ptr<BlockStatement>> statements;
  
  assureCurrentIsOSKTokenWithType(T_O_LBRACE);
  nextToken();
  while(!isCurrentTokenOSKTokenOfType(T_O_RBRACE)) {
    statements.push_back(std::move(parseBlockStatement()));
  }
  nextToken();
  
  return std::make_shared<Block>(statements);
}

std::shared_ptr<BlockStatement> Parser::parseBlockStatement()
{
    // check for possible LocVarDecl
    if(isCurrentTokenOSKTokenOfType(T_K_INT)     ||
       isCurrentTokenOSKTokenOfType(T_K_BOOLEAN) ||
       isCurrentTokenOSKTokenOfType(T_K_VOID)) {
      return parseLocalVarDecl();
    } else if(isCurrentTokenOfType<IdentifierToken>()) {
      auto token1 = std::move(currentToken);
      nextToken();
      if(isCurrentTokenOfType<IdentifierToken>()) {
        // var decl
        lexer.putBackToken(currentToken);
        currentToken = std::move(token1);
        return parseLocalVarDecl();
      } else if(isCurrentTokenOSKTokenOfType(T_O_LBRACK)) {
        // we need to go deeper
        auto token2 = std::move(currentToken);
        nextToken();
        
        if(isCurrentTokenOSKTokenOfType(T_O_RBRACK)) {
          // var decl
          lexer.putBackToken(currentToken);
          lexer.putBackToken(token2);
          currentToken = std::move(token1);
          return parseLocalVarDecl();
        } else {
          // expression
          lexer.putBackToken(currentToken);
          lexer.putBackToken(token2);
          currentToken = std::move(token1);
          return parseStatement();
        }
      } else {
        // expression
        lexer.putBackToken(currentToken);
        currentToken = std::move(token1);
        return parseStatement();
      }
    } else {
      // expression
      return parseStatement();
    }
}

/* start: current = Type */
std::shared_ptr<BlockStatement> Parser::parseLocalVarDecl()
{
  std::shared_ptr<Type> type;
  StringIdentifier ID;
  
  type = parseType();
  ID = getIdentifierFromCurrent();
  if(isNextTokenOSKTokenOfType(T_O_SEMICOLON)) {
    return std::make_shared<LocalVariableDeclaration>(type, ID);
  } else {
    assureCurrentIsOSKTokenWithType(T_O_EQUAL);
    nextToken();
    std::shared_ptr<Expression> expression = parseExpression();
    assureCurrentIsOSKTokenWithType(T_O_SEMICOLON);
    nextToken();
    
    return std::make_shared<LocalVariableExpressionDeclaration>(type, ID, expression);
  }
};

std::shared_ptr<Statement> Parser::parseStatement()
{
  if(isCurrentTokenOSKTokenOfType(T_O_LBRACE)) {
    // Block
    return parseBlock(); // for thrills
  } else if(isCurrentTokenOSKTokenOfType(T_K_IF)) {
    // if (else)
    return parseIfElseStatement();
  } else if(isCurrentTokenOSKTokenOfType(T_K_WHILE)) {
    // while
    return parseWhileStatement();
  } else if(isCurrentTokenOSKTokenOfType(T_K_RETURN)) {
    // return
    return parseReturnStatement();
  } else if(isCurrentTokenOSKTokenOfType(T_O_SEMICOLON)) {
    // empty
    nextToken();
    return std::make_shared<EmptyStatement>();
  } else {
    // probably an expression
    return parseExpressionStatement();
  }
}

/* start: current = "if" */
std::shared_ptr<Statement> Parser::parseIfElseStatement()
{
  std::shared_ptr<Expression> expression;
  std::shared_ptr<Statement>  ifStatement;
  
  assureNextIsOSKTokenWithType(T_O_LPAREN);
  nextToken();
  expression = parseExpression();
  assureCurrentIsOSKTokenWithType(T_O_RPAREN);
  nextToken();
  ifStatement = parseStatement();
  if(!isCurrentTokenOSKTokenOfType(T_K_ELSE)) {
    return std::make_shared<IfStatement>(expression, ifStatement);
  } else {
    // else
    nextToken();
    std::shared_ptr<Statement> elseStatement = parseStatement();
    return std::make_shared<IfElseStatement>(expression, ifStatement, elseStatement);
  }
};

/* start: current = "while" */
std::shared_ptr<Statement> Parser::parseWhileStatement()
{
  std::shared_ptr<Expression> expression;
  std::shared_ptr<Statement>  statement;
  
  assureNextIsOSKTokenWithType(T_O_LPAREN);
  nextToken();
  expression = parseExpression();
  assureCurrentIsOSKTokenWithType(T_O_RPAREN);
  nextToken();
  statement = parseStatement();
  
  return std::make_shared<WhileStatement>(expression, statement);
};

/* start: current = "return" */
std::shared_ptr<Statement> Parser::parseReturnStatement()
{
  if(isNextTokenOSKTokenOfType(T_O_SEMICOLON)) {
    nextToken();
    return std::make_shared<ReturnStatement>();
  } else {
    std::shared_ptr<Expression> expression = parseExpression();
    assureCurrentIsOSKTokenWithType(T_O_SEMICOLON);
    nextToken();
    return std::make_shared<ReturnExpressionStatement>(expression);
  }
};

/* start: current = Expression */
std::shared_ptr<Statement> Parser::parseExpressionStatement()
{
  std::shared_ptr<Expression> expression = parseExpression();
  assureCurrentIsOSKTokenWithType(T_O_SEMICOLON);
  nextToken();

  return std::make_shared<ExpressionStatement>(expression);
};

std::shared_ptr<Expression> Parser::parseExpression(unsigned int minPrecedence)
{
  std::shared_ptr<Expression> rightNode;
  std::shared_ptr<Expression> node;
  
  TokenType tokenType;
  unsigned int currentPrecedence;
  
  // check for "(" determining subexpression
  if(isCurrentTokenOSKTokenOfType(T_O_LPAREN))
  {
    // parse subexpression
    nextToken(); // (
    node = parseExpression();
    
    // and check for closing parensis
    assureCurrentIsOSKTokenWithType(T_O_RPAREN);
    nextToken();
  }
  // otherwise it has to be an UnaryExpression
  else
  {
    node = parseUnaryExpression();
  }
  
  // precedence climbing
  while (isCurrentTokenOSKTokenOfCategory(binaryOperator, tokenType) && 
    (Token::tokenAttribues[tokenType].precedence >= minPrecedence))
  {
    currentPrecedence = Token::tokenAttribues[tokenType].precedence;
    
    if (Token::tokenAttribues[tokenType].associativity == left)
    {
      currentPrecedence++;
    }
    // recursively get right side of binary operator
    auto token = std::move(currentToken);
    nextToken();
    rightNode = parseExpression(currentPrecedence); //TODO
    lexer.putBackToken(currentToken);
    currentToken = std::move(token);
    
    // check which binary operator was used and create AST element
    switch (tokenType)
    {
      case T_O_EQUAL:
      {
        node = std::make_shared<AssignmentExpression>(node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_PIPE_PIPE:
      {
        node = std::make_shared<LogicalOrExpression>(node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_AND_AND:
      {
        node = std::make_shared<LogicalAndExpression>(node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_EQUAL_EQUAL:
      {
        std::shared_ptr<EqualityOp> op = std::make_shared<Equals>();
        node = std::make_shared<EqualityExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_EXCLM_EQUAL:
      {
        std::shared_ptr<EqualityOp> op = std::make_shared<NotEquals>();
        node = std::make_shared<EqualityExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_LESS:
      {
        std::shared_ptr<RelationalOp> op = std::make_shared<LessThan>();
        node = std::make_shared<RelationalExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_LESS_EQUAL:
      {
        std::shared_ptr<RelationalOp> op = std::make_shared<LessThanOrEqual>();
        node = std::make_shared<RelationalExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_MORE:
      {
        std::shared_ptr<RelationalOp> op = std::make_shared<GreaterThan>();
        node = std::make_shared<RelationalExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_MORE_EQUAL:
      {
        std::shared_ptr<RelationalOp> op = std::make_shared<GreaterThanOrEqual>();
        node = std::make_shared<RelationalExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_PLUS:
      {
        std::shared_ptr<AddOp> op = std::make_shared<Add>();
        node = std::make_shared<AdditiveExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_MINUS:
      {
        std::shared_ptr<AddOp> op = std::make_shared<Subtract>();
        node = std::make_shared<AdditiveExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_STAR:
      {
        std::shared_ptr<MultOp> op = std::make_shared<Multiply>();
        node = std::make_shared<MultiplicativeExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_SLASH:
      {
        std::shared_ptr<MultOp> op = std::make_shared<Divide>();
        node = std::make_shared<MultiplicativeExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      case T_O_PERCENT:
      {
        std::shared_ptr<MultOp> op = std::make_shared<Modulo>();
        node = std::make_shared<MultiplicativeExpression>(op, node, rightNode);
        nextToken();
        break;
      }
      
      default:
        throw std::logic_error("ERROR: non-binary Token appeared in precedence climbing. this should never happen!");
    }
  }
  
  // return whole AST subtree
  return std::move(node);
}


std::shared_ptr<Expression> Parser::parseUnaryExpression()
{
  if(isCurrentTokenOSKTokenOfType(T_O_EXCLM)) {
    nextToken();
    std::shared_ptr<UnaryOp> op = std::make_shared<Negate>();
    std::shared_ptr<Expression> expression = parseUnaryExpression();
    return std::make_shared<UnaryLeftExpression>(op, expression);
  } else if(isCurrentTokenOSKTokenOfType(T_O_MINUS)) {
    nextToken();
    std::shared_ptr<UnaryOp> op = std::make_shared<Minus>();
    std::shared_ptr<Expression> expression = parseUnaryExpression();
    return std::make_shared<UnaryLeftExpression>(op, expression);
  } else {
    return parsePostfixExpression();
  }
}

std::shared_ptr<Expression> Parser::parsePostfixExpression()
{
  std::shared_ptr<Expression> expression = parsePrimaryExpression();
  
  while(isCurrentTokenOSKTokenOfType(T_O_DOT) || isCurrentTokenOSKTokenOfType(T_O_LBRACK)) {
    // post fix expression
    std::shared_ptr<UnaryOp> op;

    if(isCurrentTokenOSKTokenOfType(T_O_DOT)) {
      // method invocation / field access
      StringIdentifier ID = getIdentifierFromNext();
      
      if(isNextTokenOSKTokenOfType(T_O_LPAREN)) {
        // method invocation
        std::vector<std::shared_ptr<Expression>> arguments;
        nextToken(); // (
        
        bool continuous = false;
        while(!isCurrentTokenOSKTokenOfType(T_O_RPAREN)) {
          if(continuous) {
            assureCurrentIsOSKTokenWithType(T_O_COMMA);
            nextToken(); // ,
          }
          arguments.push_back(std::move(parseExpression()));
          continuous = true;
        }
        nextToken(); // )
        
        op = std::make_shared<MethodInvocation>(ID, arguments);
      } else {
        // field access
        op = std::make_shared<FieldAccess>(ID);
      }
    } else { // T_O_LBRACK
      // array access
      nextToken(); // [
      std::shared_ptr<Expression> accessExpression = parseExpression();
      assureCurrentIsOSKTokenWithType(T_O_RBRACK);
      nextToken(); // ]
      
      op = std::make_shared<ArrayAccess>(accessExpression);
    }
    expression = std::make_shared<UnaryRightExpression>(expression, op);
  }
  
  // no more postfix expressions
  return expression;
}

std::shared_ptr<Expression> Parser::parsePrimaryExpression()
{
  if(isCurrentTokenOSKTokenOfType(T_K_NULL)) {
    // null
    nextToken();
    return std::make_shared<CNull>();
  } else if(isCurrentTokenOSKTokenOfType(T_K_FALSE)) {
    // false
    nextToken();
    return std::make_shared<CFalse>();
  } else if(isCurrentTokenOSKTokenOfType(T_K_TRUE)) {
    // true
    nextToken();
    return std::make_shared<CTrue>();
  } else if(isCurrentTokenOSKTokenOfType(T_K_THIS)) {
    // this
    nextToken();
    return std::make_shared<CThis>();
  } else if(isCurrentTokenOfType<IntegerLiteralToken>()) {
    // integer literal
    std::string integer = dynamic_cast<IntegerLiteralToken*>(currentToken.get())->value;
    nextToken();
    return std::make_shared<CIntegerLiteral>(integer);
  } else if(isCurrentTokenOSKTokenOfType(T_O_LPAREN)) {
    // nested "real" expression
    nextToken(); // (
    std::shared_ptr<Expression> expression = std::move(parseExpression());
    assureCurrentIsOSKTokenWithType(T_O_RPAREN);
    nextToken(); // )
    return expression;
  } else if(isCurrentTokenOSKTokenOfType(T_K_NEW)) {
    // new array/object
    nextToken();
    auto token = std::move(currentToken); // ignore for now
    nextToken();
    
    if(isCurrentTokenOSKTokenOfType(T_O_LPAREN)) {
      // new object
      currentToken = std::move(token);
      StringIdentifier ID = getIdentifierFromCurrent();
      // ( has already been read
      assureNextIsOSKTokenWithType(T_O_RPAREN);
      nextToken();
      return std::make_shared<NewObject>(ID);
    } else if(isCurrentTokenOSKTokenOfType(T_O_LBRACK)) {
      // new array
      int arrayDepth = 0;
      currentToken = std::move(token);
      std::shared_ptr<BasicType> type = parseBasicType();
      // [ has already been read
      std::shared_ptr<Expression> expression = parseExpression();
      assureCurrentIsOSKTokenWithType(T_O_RBRACK);
      nextToken();
      
      // possible brackets []
      while (isCurrentTokenOSKTokenOfType(T_O_LBRACK)) {
        token = std::move(currentToken);
        nextToken();
        if(isCurrentTokenOSKTokenOfType(T_O_RBRACK)) {
          ++arrayDepth;
          nextToken();
        } else {
          lexer.putBackToken(currentToken);
          currentToken = std::move(token);
          break;
        }
      }
      
      return std::make_shared<NewArray>(type, expression, arrayDepth);
    } else {
      error("undefined use of new");
    }
  } else {
    // ID / method call
    StringIdentifier ID = getIdentifierFromCurrent();
    if(isNextTokenOSKTokenOfType(T_O_LPAREN)) {
      // method call
      std::vector<std::shared_ptr<Expression>> arguments;
      nextToken();
      
      bool continuous = false;
      while(!isCurrentTokenOSKTokenOfType(T_O_RPAREN)) {
        if(continuous) {
          assureCurrentIsOSKTokenWithType(T_O_COMMA);
          nextToken();
        }
        arguments.push_back(std::move(parseExpression()));
        continuous = true;
      }
      nextToken(); // )
      return std::make_shared<CallExpression>(ID, arguments);
    } else {
      // ID
      return std::make_shared<CRef>(ID);
    }
  }
    
    return nullptr;
}

std::shared_ptr<Node> Parser::getAST()
{
  return ast;
}
