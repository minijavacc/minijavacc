#include "token.h"

#include <string>

using namespace cmpl;

// created using following table: http://introcs.cs.princeton.edu/java/11precedence/
// the order of the initialization entries has to be the same as the enum in token.h!
const OperatorSeperatorKeywordContainer Token::tokenAttribues[99] = {
  [T_O_EQUAL]                = {binaryOperator, "=", 0, right}, 
  
  [T_O_PIPE_PIPE]            = {binaryOperator, "||", 1, left}, 
  
  [T_O_AND_AND]              = {binaryOperator, "&&", 2, left}, 
  
  [T_O_EXCLM_EQUAL]          = {binaryOperator, "!=", 3, left}, 
  [T_O_EQUAL_EQUAL]          = {binaryOperator, "==", 3, left}, 
  
  [T_O_LESS]                 = {binaryOperator, "<", 4, left}, 
  [T_O_MORE]                 = {binaryOperator, ">", 4, left}, 
  [T_O_LESS_EQUAL]           = {binaryOperator, "<=", 4, left}, 
  [T_O_MORE_EQUAL]           = {binaryOperator, ">=", 4, left}, 
  
  [T_O_PLUS]                 = {binaryOperator, "+", 5, left}, 
  [T_O_MINUS]                = {binaryOperator, "-", 5, left}, 
  
  [T_O_SLASH]                = {binaryOperator, "/", 6, left}, 
  [T_O_STAR]                 = {binaryOperator, "*", 6, left}, 
  [T_O_PERCENT]              = {binaryOperator, "%", 6, left}, 
  
  [T_O_EXCLM]                = {unaryOperator, "!", 7, right}, 
  
  // operators that are not available in MiniJava
  [T_O_PLUS_EQUAL]           = {unusedOperator, "+=", 0, right}, 
  [T_O_MINUS_EQUAL]          = {unusedOperator, "-=", 0, right}, 
  [T_O_STAR_EQUAL]           = {unusedOperator, "*=", 0, right}, 
  [T_O_SLASH_EQUAL]          = {unusedOperator, "/=", 0, right},
  [T_O_LESS_LESS_EQUAL]      = {unusedOperator, "<<=", 0, right},
  [T_O_MORE_MORE_EQUAL]      = {unusedOperator, ">>=", 0, right},
  [T_O_MORE_MORE_MORE_EQUAL] = {unusedOperator, ">>>=", 0, right}, 
  [T_O_PERCENT_EQUAL]        = {unusedOperator, "%=", 0, right}, 
  [T_O_AND_EQUAL]            = {unusedOperator, "&=", 0, right}, 
  [T_O_CARET_EQUAL]          = {unusedOperator, "^=", 0, right}, 
  [T_O_PIPE_EQUAL]           = {unusedOperator, "|=", 0, right}, 
  [T_O_PIPE]                 = {unusedOperator, "|", 0, left}, 
  [T_O_CARET]                = {unusedOperator, "^", 0, left}, 
  [T_O_AND]                  = {unusedOperator, "&", 0, left}, 
  [T_O_LESS_LESS]            = {unusedOperator, "<<", 0, left}, 
  [T_O_MORE_MORE_MORE]       = {unusedOperator, ">>>", 0, left}, 
  [T_O_MORE_MORE]            = {unusedOperator, ">>", 0, left}, 
  [T_O_TILDE]                = {unusedOperator, "~", 0, right}, 
  [T_O_PLUS_PLUS]            = {unusedOperator, "++", 0, right},
  [T_O_MINUS_MINUS]          = {unusedOperator, "--", 0, right},
  [T_O_QUESTM]               = {unusedOperator, "?", 0, right},
  
  [T_O_LPAREN]               = {seperator, "(", 0, none}, 
  [T_O_RPAREN]               = {seperator, ")", 0, none}, 
  [T_O_KOMMA]                = {seperator, ",", 0, none}, 
  [T_O_DOT]                  = {seperator, ".", 0, none}, 
  [T_O_COLON]                = {seperator, ":", 0, none}, 
  [T_O_SEMICOLON]            = {seperator, ";", 0, none}, 
  [T_O_LBRACK]               = {seperator, "[", 0, none}, 
  [T_O_RBRACK]               = {seperator, "]", 0, none}, 
  [T_O_LBRACE]               = {seperator, "{", 0, none}, 
  [T_O_RBRACE]               = {seperator, "}", 0, none}, 
  
  [T_K_ABSTRACT]             = {keyword, "abstract", 0, none}, 
  [T_K_ASSERT]               = {keyword, "assert", 0, none}, 
  [T_K_BOOLEAN]              = {keyword, "boolean", 0, none}, 
  [T_K_BREAK]                = {keyword, "break", 0, none}, 
  [T_K_BYTE]                 = {keyword, "byte", 0, none}, 
  [T_K_CASE]                 = {keyword, "case", 0, none}, 
  [T_K_CATCH]                = {keyword, "catch", 0, none}, 
  [T_K_CHAR]                 = {keyword, "char", 0, none}, 
  [T_K_CLASS]                = {keyword, "class", 0, none}, 
  [T_K_CONST]                = {keyword, "const", 0, none}, 
  [T_K_CONTINUE]             = {keyword, "continue", 0, none}, 
  [T_K_DEFAULT]              = {keyword, "default", 0, none}, 
  [T_K_DOUBLE]               = {keyword, "double", 0, none}, 
  [T_K_DO]                   = {keyword, "do", 0, none}, 
  [T_K_ELSE]                 = {keyword, "else", 0, none}, 
  [T_K_ENUM]                 = {keyword, "enum", 0, none}, 
  [T_K_EXTENDS]              = {keyword, "extends", 0, none}, 
  [T_K_FALSE]                = {keyword, "false", 0, none}, 
  [T_K_FINALLY]              = {keyword, "finally", 0, none}, 
  [T_K_FINAL]                = {keyword, "final", 0, none}, 
  [T_K_FLOAT]                = {keyword, "float", 0, none}, 
  [T_K_FOR]                  = {keyword, "for", 0, none}, 
  [T_K_GOTO]                 = {keyword, "goto", 0, none}, 
  [T_K_IF]                   = {keyword, "if", 0, none}, 
  [T_K_IMPLEMENTS]           = {keyword, "implements", 0, none}, 
  [T_K_IMPORT]               = {keyword, "import", 0, none}, 
  [T_K_INSTANCEOF]           = {keyword, "instanceof", 0, none}, 
  [T_K_INTERFACE]            = {keyword, "interface", 0, none}, 
  [T_K_INT]                  = {keyword, "int", 0, none}, 
  [T_K_LONG]                 = {keyword, "long", 0, none}, 
  [T_K_NATIVE]               = {keyword, "native", 0, none}, 
  [T_K_NEW]                  = {keyword, "new", 0, none}, 
  [T_K_NULL]                 = {keyword, "null", 0, none}, 
  [T_K_PACKAGE]              = {keyword, "package", 0, none}, 
  [T_K_PRIVATE]              = {keyword, "private", 0, none}, 
  [T_K_PROTECTED]            = {keyword, "protected", 0, none}, 
  [T_K_PUBLIC]               = {keyword, "public", 0, none}, 
  [T_K_RETURN]               = {keyword, "return", 0, none}, 
  [T_K_SHORT]                = {keyword, "short", 0, none}, 
  [T_K_STATIC]               = {keyword, "static", 0, none}, 
  [T_K_STRICTFP]             = {keyword, "strictfp", 0, none}, 
  [T_K_SUPER]                = {keyword, "super", 0, none}, 
  [T_K_SWITCH]               = {keyword, "switch", 0, none}, 
  [T_K_SYNCHRONIZED]         = {keyword, "synchronized", 0, none}, 
  [T_K_THIS]                 = {keyword, "this", 0, none}, 
  [T_K_THROWS]               = {keyword, "throws", 0, none}, 
  [T_K_THROW]                = {keyword, "throw", 0, none}, 
  [T_K_TRANSIENT]            = {keyword, "transient", 0, none}, 
  [T_K_TRUE]                 = {keyword, "true", 0, none}, 
  [T_K_TRY]                  = {keyword, "try", 0, none}, 
  [T_K_VOID]                 = {keyword, "void", 0, none}, 
  [T_K_VOLATILE]             = {keyword, "volatile", 0, none}, 
  [T_K_WHILE]                = {keyword, "while", 0, none} 
};


std::string OperatorSeperatorKeywordToken::getStringValue()
{
  int tt = this->type;
	return tokenAttribues[tt].stringRepresentation;
}

std::string IdentifierToken::getStringValue()
{
  // lookup in stringtable
  std::string identifierString = stringTable.lookupIdentifier(id);
	return "identifier " + identifierString;
}

std::string IntegerLiteralToken::getStringValue()
{
  return "integer literal " + value;
}
