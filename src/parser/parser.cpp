#include "parser.h"

using namespace cmpl;


/************************ helper functions **************/


// if token array of lexer is empty (method returns false) throw semanticError for now,
// later depends on implementation of lexer running in parallel (to avoid polling)
inline void Parser::nextToken()
{
  if(!lexer.getNextToken(currentToken)) {
    throw SemanticError();
  }
}

inline std::unique_ptr<IdentifierToken> Parser::getIdentifierFromCurrent() {

  IdentifierToken* id_t;
  if (id_t = dynamic_cast<IdentifierToken*>(currentToken.get())) {
    return std::unique_ptr<IdentifierToken>(id_t);
  } else {
    throw SemanticError();
  }
}

inline std::unique_ptr<IdentifierToken> Parser::getIdentifierFromNext() {
  nextToken();
  return std::move(getIdentifierFromCurrent());
}

// Checks whether next token is of specified type T, trows exception in case that not
template<typename T>
inline void Parser::assureCurrentTokenTypeIs()
{
  if (!isCurrentTokenOfType<T>())
  {
    throw SemanticError();
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
  if (!isCurrentTokenOSKTokenOfType(tokenType))
  {
    throw SemanticError();
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
  return ((osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(currentToken.get()))
          && (osk_t->type != tokenType));
}

// combines isCurrentTokenOfType with checking for specialized sub-type of operator/seperator tokens
inline bool Parser::isCurrentTokenOSKTokenOfCategory(const TokenCategory& tokenCategory, TokenType& tokenTypeOut) {
  OperatorSeperatorKeywordToken* osk_t = dynamic_cast<OperatorSeperatorKeywordToken*>(currentToken.get());
  if (!osk_t)
  {
    return false;
  }
  
  tokenTypeOut = osk_t->type;
  
  return (Token::tokenAttribues[osk_t->type] == binaryOperator);
}

// combines isNextTokenOfType with checking for specialized sub-type of operator/seperator tokens
inline bool Parser::isNextTokenOSKTokenOfType(const TokenType& tokenType) {
  nextToken();
  return isCurrentTokenOSKTokenOfType(tokenType);
}


/************************ end helper functions **************/


void Parser::run()
{
  // each parseNONTERMINAL() function requests their token at the beginning
  ast = parseProgram();
}

std::unique_ptr<Program> Parser::parseProgram()
{
  std::vector<std::unique_ptr<ClassDeclaration>> classes;
  
  // multile classes
  while(lexer.hasNextToken()) {
    assureNextIsOSKTokenWithType(T_K_CLASS);
    classes.push_back(std::move(parseClassDeclaration()));
  }
  
  return std::make_unique<Program>(classes);
}

/* start: current = "class" */
std::unique_ptr<ClassDeclaration> Parser::parseClassDeclaration()
{
  std::vector<std::unique_ptr<ClassMember>> classMembers;
  std::unique_ptr<IdentifierToken> ID;
  
  ID = getIdentifierFromNext();
  assureNextIsOSKTokenWithType(T_O_LBRACE);
  // multile class members
  while(!isNextTokenOSKTokenOfType(T_O_RBRACE)) {
    classMembers.push_back(std::move(parseClassMember()));
  }

  return std::make_unique<ClassDeclaration>(ID, classMembers);
}

std::unique_ptr<ClassMember> Parser::parseClassMember()
{
  assureNextIsOSKTokenWithType(T_K_PUBLIC);
  
  if(isNextTokenOSKTokenOfType(T_K_STATIC)) {
    // MainMethod
    std::unique_ptr<IdentifierToken> ID;
    std::unique_ptr<IdentifierToken> parameterID;
    std::unique_ptr<Block> block;
    
    assureNextIsOSKTokenWithType(T_K_VOID);
    
    ID = getIdentifierFromNext();
    
    assureNextIsOSKTokenWithType(T_O_LPAREN);
    assureNextTokenTypeIs<IdentifierToken>();
    //TODO String check
    assureNextIsOSKTokenWithType(T_O_LBRACK);
    assureNextIsOSKTokenWithType(T_O_RBRACK);
    parameterID = getIdentifierFromNext();
    assureNextIsOSKTokenWithType(T_O_RPAREN);
    nextToken();
    block = parseBlock();
    return std::make_unique<MainMethod>(ID, parameterID, block);
  } else {
    std::unique_ptr<Type> type = parseType();
    std::unique_ptr<IdentifierToken> ID = getIdentifierFromNext();
    if(isNextTokenOSKTokenOfType(T_O_SEMICOLON)) {
      // Field
      return std::make_unique<Field>(type, ID);
    } else if(isCurrentTokenOSKTokenOfType(T_O_LPAREN)) {
      // Method
      std::vector<std::unique_ptr<Parameter>> parameters;
      std::unique_ptr<Block> block;
      do {
        nextToken();
        parameters.push_back(std::move(parseParameter()));
      } while(isCurrentTokenOSKTokenOfType(T_O_COMMA));
      
      return std::make_unique<Method>(type, ID, parameters, block);
    } else {
      throw SemanticError();
    }
  }
}

std::unique_ptr<Type> Parser::parseType()
{
  std::unique_ptr<BasicType> type = parseBasicType();
  int arrayDepth = 0;
  
  while(isCurrentTokenOSKTokenOfType(T_O_LBRACK)) {
    assureNextIsOSKTokenWithType(T_O_RBRACK);
    ++arrayDepth;
    nextToken();
  }
  
  return std::make_unique<Type>(type, arrayDepth);
}

std::unique_ptr<BasicType> Parser::parseBasicType()
{
  if(isCurrentTokenOSKTokenOfType(T_K_BOOLEAN)) {
    return std::make_unique<TypeBoolean>();
  } else if(isCurrentTokenOSKTokenOfType(T_K_INT)) {
    return std::make_unique<TypeInt>();
  } else if(isCurrentTokenOSKTokenOfType(T_K_VOID)) {
    return std::make_unique<TypeVoid>();
  } else {
    std::unique_ptr<IdentifierToken> ID = getIdentifierFromCurrent();
    return std::make_unique<UserType>(ID);
  }
}

std::unique_ptr<Parameter> Parser::parseParameter()
{
  std::unique_ptr<Type> type = parseType();
  std::unique_ptr<IdentifierToken> ID = getIdentifierFromCurrent();
  
  return std::make_unique<Parameter>(type, ID);
}

std::unique_ptr<Block> Parser::parseBlock()
{
  std::vector<std::unique_ptr<Statement>> statements;
  
  assureCurrentIsOSKTokenWithType(T_O_LBRACK);
  
  while(!isNextTokenOSKTokenOfType(T_O_LBRACK)) {
    statements.push_back(std::move(parseStatement()));
  }
  
  return std::make_unique<Block>(statements);
}


std::unique_ptr<Statement> Parser::parseStatement()
{
  if(isCurrentTokenOSKTokenOfType(T_O_LBRACK)) {
    // Block
    return parseBlock();
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
    return std::make_unique<EmptyStatement>();
  } else  {
    // expression/LocVarDecl
    //TODO
    return nullptr;
  }
}

/* start: current = "if" */
std::unique_ptr<Statement> Parser::parseIfElseStatement()
{
  std::unique_ptr<Expression> expression;
  std::unique_ptr<Statement>  ifStatement;
  
  assureNextIsOSKTokenWithType(T_O_LPAREN);
  nextToken();
  expression = parseExpression();
  assureCurrentIsOSKTokenWithType(T_O_RPAREN);
  nextToken();
  ifStatement = parseStatement();
  if(!isCurrentTokenOSKTokenOfType(T_K_ELSE)) {
    return std::make_unique<IfStatement>(expression, ifStatement);
  } else {
    // else
    std::unique_ptr<Statement> elseStatement = parseStatement();
    return std::make_unique<IfElseStatement>(expression, ifStatement, elseStatement);
  }
};

/* start: current = "while" */
std::unique_ptr<Statement> Parser::parseWhileStatement()
{
  std::unique_ptr<Expression> expression;
  std::unique_ptr<Statement>  statement;
  
  assureNextIsOSKTokenWithType(T_O_LPAREN);
  nextToken();
  expression = parseExpression();
  assureCurrentIsOSKTokenWithType(T_O_RPAREN);
  nextToken();
  statement = parseStatement();
  
  return std::make_unique<WhileStatement>(expression, statement);
};

/* start: current = "return" */
std::unique_ptr<Statement> Parser::parseReturnStatement()
{
  if(isNextTokenOSKTokenOfType(T_O_SEMICOLON)) {
    return std::make_unique<ReturnStatement>();
  } else {
    std::unique_ptr<Expression> expression = parseExpression();
    return std::make_unique<ReturnExpressionStatement>(expression);
  }
};

std::unique_ptr<Expression> Parser::parseExpression(unsigned int minPrecedence)
{
  std::unique_ptr<Expression> rightNode;
  std::unique_ptr<Expression> node;
  
  TokenType tokenType;
  unsigned int currentPrecedence;
  
  // check for "(" determining subexpression
  if(isCurrentTokenOSKTokenOfType(T_O_LPAREN))
  {
    // parse subexpression
    nextToken();
    node = parseExpression();
    
    // and check for closing parensis
    assureNextIsOSKTokenWithType(T_O_RPAREN);
  }
  // otherwise it has to be an UnaryExpression
  else
  {
    node = parseUnaryExpression(); //TODO
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
    rightNode = parseExpression(currentPrecedence); //TODO
//    rightNode = precedenceClimbingParseExpression(currentPrecedence); //TODO
    
    // check which binary operator was used and create AST element
    switch (tokenType)
    {
      case T_O_EQUAL:
      {
        node = std::make_unique<AssignmentExpression>(node, rightNode);
        break;
      }
      
      case T_O_PIPE_PIPE:
      {
        node = std::make_unique<LogicalOrExpression>(node, rightNode);
        break;
      }
      
      case T_O_AND_AND:
      {
        node = std::make_unique<LogicalAndExpression>(node, rightNode);
        break;
      }
      
      case T_O_EQUAL_EQUAL:
      {
        std::unique_ptr<EqualityOp> op = std::make_unique<Equals>();
        node = std::make_unique<EqualityExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_EXCLM_EQUAL:
      {
        std::unique_ptr<EqualityOp> op = std::make_unique<NotEquals>();
        node = std::make_unique<EqualityExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_LESS:
      {
        std::unique_ptr<RelationalOp> op = std::make_unique<LessThan>();
        node = std::make_unique<RelationalExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_LESS_EQUAL:
      {
        std::unique_ptr<RelationalOp> op = std::make_unique<LessThanOrEqual>();
        node = std::make_unique<RelationalExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_MORE:
      {
        std::unique_ptr<RelationalOp> op = std::make_unique<GreaterThan>();
        node = std::make_unique<RelationalExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_MORE_EQUAL:
      {
        std::unique_ptr<RelationalOp> op = std::make_unique<GreaterThanOrEqual>();
        node = std::make_unique<RelationalExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_PLUS:
      {
        std::unique_ptr<AddOp> op = std::make_unique<Add>();
        node = std::make_unique<AdditiveExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_MINUS:
      {
        std::unique_ptr<AddOp> op = std::make_unique<Subtract>();
        node = std::make_unique<AdditiveExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_STAR:
      {
        std::unique_ptr<MultOp> op = std::make_unique<Multiply>();
        node = std::make_unique<MultiplicativeExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_SLASH:
      {
        std::unique_ptr<MultOp> op = std::make_unique<Divide>();
        node = std::make_unique<MultiplicativeExpression>(op, node, rightNode);
        break;
      }
      
      case T_O_PERCENT:
      {
        std::unique_ptr<MultOp> op = std::make_unique<Modulo>();
        node = std::make_unique<MultiplicativeExpression>(op, node, rightNode);
        break;
      }
      
      default:
        throw std::logic_error("ERROR: non-binary Token appeared in precedence climbing. this should never happen!");
    }
  }
  
  // return whole AST subtree
  return std::move(node);
}


std::unique_ptr<UnaryExpression> Parser::parseUnaryExpression()
{
  return nullptr; //TODO
}

void Parser::getAST(std::unique_ptr<Node> &n)
{
  n = std::move(ast);
}
