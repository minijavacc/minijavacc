#include "token.h"

#include <string>

using namespace cmpl;

const std::string Token::stringRepresentations[99] = {
  [T_O_EXCLM_EQUAL]          = "!=",
  [T_O_EXCLM]                = "!",
  [T_O_LPAREN]               = "(",
  [T_O_RPAREN]               = ")",
  [T_O_STAR_EQUAL]           = "*=",
  [T_O_STAR]                 = "*",
  [T_O_PLUS_PLUS]            = "++",
  [T_O_PLUS_EQUAL]           = "+=",
  [T_O_PLUS]                 = "+",
  [T_O_KOMMA]                = ",",
  [T_O_MINUS_EQUAL]          = "-=",
  [T_O_MINUS_MINUS]          = "--",
  [T_O_MINUS]                = "-",
  [T_O_DOT]                  = ".",
  [T_O_SLASH_EQUAL]          = "/=",
  [T_O_SLASH]                = "/",
  [T_O_COLON]                = ":",
  [T_O_SEMICOLON]            = ";",
  [T_O_LESS_LESS_EQUAL]      = "<<=",
  [T_O_LESS_LESS]            = "<<",
  [T_O_LESS_EQUAL]           = "<=",
  [T_O_LESS]                 = "<",
  [T_O_EQUAL_EQUAL]          = "==",
  [T_O_EQUAL]                = "=",
  [T_O_MORE_EQUAL]           = ">=",
  [T_O_MORE_MORE_EQUAL]      = ">>=",
  [T_O_MORE_MORE_MORE_EQUAL] = ">>>=",
  [T_O_MORE_MORE_MORE]       = ">>>",
  [T_O_MORE_MORE]            = ">>",
  [T_O_MORE]                 = ">",
  [T_O_QUESTM]               = "?",
  [T_O_PERCENT_EQUAL]        = "%=",
  [T_O_PERCENT]              = "%",
  [T_O_AND_EQUAL]            = "&=",
  [T_O_AND_AND]              = "&&",
  [T_O_AND]                  = "&",
  [T_O_LBRACK]               = "[",
  [T_O_RBRACK]               = "]",
  [T_O_CARET_EQUAL]          = "^=",
  [T_O_CARET]                = "^",
  [T_O_LBRACE]               = "{",
  [T_O_RBRACE]               = "}",
  [T_O_TILDE]                = "~",
  [T_O_PIPE_EQUAL]           = "|=",
  [T_O_PIPE_PIPE]            = "||",
  [T_O_PIPE]                 = "|",

  [T_K_ABSTRACT] = "abstract",
  [T_K_ASSERT] = "assert",
  [T_K_BOOLEAN] = "boolean",
  [T_K_BREAK] = "break",
  [T_K_BYTE] = "byte",
  [T_K_CASE] = "case",
  [T_K_CATCH] = "catch",
  [T_K_CHAR] = "char",
  [T_K_CLASS] = "class",
  [T_K_CONST] = "const",
  [T_K_CONTINUE] = "continue",
  [T_K_DEFAULT] = "default",
  [T_K_DOUBLE] = "double",
  [T_K_DO] = "do",
  [T_K_ELSE] = "else",
  [T_K_ENUM] = "enum",
  [T_K_EXTENDS] = "extends",
  [T_K_FALSE] = "false",
  [T_K_FINALLY] = "finally",
  [T_K_FINAL] = "final",
  [T_K_FLOAT] = "float",
  [T_K_FOR] = "for",
  [T_K_GOTO] = "goto",
  [T_K_IF] = "if",
  [T_K_IMPLEMENTS] = "implements",
  [T_K_IMPORT] = "import",
  [T_K_INSTANCEOF] = "instanceof",
  [T_K_INTERFACE] = "interface",
  [T_K_INT] = "int",
  [T_K_LONG] = "long",
  [T_K_NATIVE] = "native",
  [T_K_NEW] = "new",
  [T_K_NULL] = "null",
  [T_K_PACKAGE] = "package",
  [T_K_PRIVATE] = "private",
  [T_K_PROTECTED] = "protected",
  [T_K_PUBLIC] = "public",
  [T_K_RETURN] = "return",
  [T_K_SHORT] = "short",
  [T_K_STATIC] = "static",
  [T_K_STRICTFP] = "strictfp",
  [T_K_SUPER] = "super",
  [T_K_SWITCH] = "switch",
  [T_K_SYNCHRONIZED] = "synchronized",
  [T_K_THIS] = "this",
  [T_K_THROWS] = "throws",
  [T_K_THROW] = "throw",
  [T_K_TRANSIENT] = "transient",
  [T_K_TRUE] = "true",
  [T_K_TRY] = "try",
  [T_K_VOID] = "void",
  [T_K_VOLATILE] = "volatile",
  [T_K_WHILE] = "while"
};


std::string OperatorSeperatorKeywordToken::getStringValue()
{
  int tt = this->type;
	return stringRepresentations[tt];
}

std::string IdentifierToken::getStringValue()
{
	return "identifier " + this->identifierString;
}

std::string IntegerLiteralToken::getStringValue()
{
  return "integer literal " + value;
}
