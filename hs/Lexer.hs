module Lexer where
    
import qualified Data.Char as Char
import Data.List

data TokenType = T_EXCL
               | T_EXCL_EQUALS 
               | T_LPAREN
               | T_RPAREN
               | T_STAR
               | T_STAR_EQUALS
               | T_PLUS_PLUS
               | T_PLUS_EQUALS
               | T_PLUS
               | T_COMMA
               | T_MINUS
               | T_MINUS_MINUS
               | T_MINUS_EQUALS
               | T_DOT
               | T_SLASH
               | T_SLASH_EQUALS
               | T_COLON
               | T_SEMICOLON
               | T_LANGLE
               | T_LANGLE_LANGLE
               | T_LANGLE_EQUALS
               | T_LANGLE_LANGLE_EQUALS
               | T_RANGLE
               | T_RANGLE_RANGLE
               | T_RANGLE_RANGLE_RANGLE
               | T_RANGLE_EQUALS
               | T_RANGLE_RANGLE_EQUALS
               | T_RANGLE_RANGLE_RANGLE_EQUALS
               | T_EQUALS
               | T_EQUALS_EQUALS
               | T_QUESTION
               | T_PERCENT
               | T_PERCENT_EQUALS
               | T_AMPERSAND
               | T_AMPERSAND_AMPERSAND
               | T_AMPERSAND_EQUALS
               | T_LBRACKET
               | T_RBRACKET
               | T_CARET
               | T_CARET_EQUALS
               | T_LBRACE
               | T_RBRACE
               | T_TILDE
               | T_PIPE
               | T_PIPE_PIPE
               | T_PIPE_EQUALS
               | T_IDENT
               | T_KEYWORD
               | T_INTEGER_LITERAL deriving (Show)

data Token = Token TokenType String (Int, Int)
instance Show Token where
    show (Token T_IDENT s p) = "identifier " ++ s
    show (Token T_KEYWORD s p) = s
    show (Token T_INTEGER_LITERAL s p) = "integer literal " ++ s
    show (Token t s p) = s
              
    



-- Tokenize function
tokenize :: String -> [Token]
tokenize s = start s (1, 1)

-- Skip whitespace
start (' ':xs)  (r, c) = start xs (r, c+1)
start ('\t':xs) (r, c) = start xs (r, c+1)
start ('\n':xs) (r, c) = start xs (r+1, c)

-- Operators and separators
start ('!':xs) (r, c) = excl xs (r, c)
start ('(':xs) (r, c) = lparen xs (r, c)
start (')':xs) (r, c) = rparen xs (r, c)
start ('*':xs) (r, c) = star xs (r, c)
start ('+':xs) (r, c) = plus xs (r, c)
start (',':xs) (r, c) = comma xs (r, c)
start ('-':xs) (r, c) = minus xs (r, c)
start ('.':xs) (r, c) = dot xs (r, c)
start ('/':xs) (r, c) = slash xs (r, c)
start (':':xs) (r, c) = colon xs (r, c)
start (';':xs) (r, c) = semicolon xs  (r, c)
start ('<':xs) (r, c) = langle xs (r, c)
start ('>':xs) (r, c) = rangle xs (r, c)
start ('=':xs) (r, c) = equals xs (r, c)
start ('?':xs) (r, c) = question xs  (r, c)
start ('%':xs) (r, c) = percent xs  (r, c)
start ('&':xs) (r, c) = ampersand xs  (r, c)
start ('[':xs) (r, c) = lbracket xs  (r, c)
start (']':xs) (r, c) = rbracket xs  (r, c)
start ('^':xs) (r, c) = caret xs  (r, c)
start ('{':xs) (r, c) = lbrace xs  (r, c)
start ('}':xs) (r, c) = rbrace xs  (r, c)
start ('~':xs) (r, c) = tilde xs  (r, c)
start ('|':xs) (r, c) = pipe xs  (r, c)

-- Identifiers and keywords (alpha)
start (x:xs) (r, c)
    | Char.isAlpha x || x == '_' = alpha [x] xs (r, c)
    
-- Integer literals
start ('0':xs) p@(r, c) = zeroToken:(start xs (r, c+1))
    where zeroToken = Token T_INTEGER_LITERAL "0" p
start (x:xs) (r, c)
    | Char.isDigit x = digit [x] xs (r, c)

-- End of string
start [] (r, c) = []



-- Operator and separator states
excl ('=':xs) p@(r, c) = (exclEqualsToken p):(start xs (r, c+2))
excl s p@(r, c) = (exclToken p):(start s (r, c+1))
    
lparen s p@(r, c) = (lparenToken p):(start s (r, c+1))
    
rparen s p@(r, c) = (rparenToken p):(start s (r, c+1))
    
star ('=':xs) p@(r, c) = (starEqualsToken p):(start xs (r, c+2))
star s p@(r, c) = (starToken p):(start s (r, c+1))
    
plus ('+':xs) p@(r, c) = (plusPlusToken p):(start xs (r, c+2))
plus ('=':xs) p@(r, c) = (plusEqualsToken p):(start xs (r, c+2))
plus s p@(r, c) = (plusToken p):(start s (r, c+1))
    
comma s p@(r, c) = (commaToken p):(start s (r, c+1))
    
minus ('-':xs) p@(r, c) = (minusMinusToken p):(start xs (r, c+2))
minus ('=':xs) p@(r, c) = (minusEqualsToken p):(start xs (r, c+2))
minus s p@(r, c) = (minusToken p):(start s (r, c+1))
    
dot s p@(r, c) = (dotToken p):(start s (r, c+1))

slash ('*':xs) p@(r, c) = comment xs (r, c+2) -- start of comment
slash ('=':xs) p@(r, c) = (slashEqualsToken p):(start xs (r, c+2))
slash s p@(r, c) = (slashToken p):(start s (r, c+1))

colon s p@(r, c) = (colonToken p):(start s (r, c+1))

semicolon s p@(r, c) = (semicolonToken p):(start s (r, c+1))

langle ('<':xs) p@(r, c) = langleLangle xs (r, c)
langle ('=':xs) p@(r, c) = (langleEqualsToken p):(start xs (r, c+2))
langle s p@(r, c) = (langleToken p):(start s (r, c+1))

langleLangle ('=':xs) p@(r, c) = (langleLangleEqualsToken p):(start xs (r, c+3))
langleLangle s p@(r, c) = (langleLangleToken p):(start s (r, c+2))

rangle ('>':xs) p@(r, c) = rangleRangle xs (r, c)
rangle ('=':xs) p@(r, c) = (rangleEqualsToken p):(start xs (r, c+2))
rangle s p@(r, c) = (rangleToken p):(start s (r, c+1))

rangleRangle ('>':xs) p@(r, c) = rangleRangleRangle xs (r, c)
rangleRangle ('=':xs) p@(r, c) = (rangleRangleEqualsToken p):(start xs (r, c+3))
rangleRangle s p@(r, c) = (rangleRangleToken p):(start s (r, c+2))

rangleRangleRangle ('=':xs) p@(r, c) = (rangleRangleRangleEqualsToken p):(start xs (r, c+4))
rangleRangleRangle s p@(r, c) = (rangleRangleRangleToken p):(start s (r, c+3))

equals ('=':xs) p@(r, c) = (equalsEqualsToken p):(start xs (r, c+2))
equals s p@(r, c) = (equalsToken p):(start s (r, c+1))

question s p@(r, c) = (questionToken p):(start s (r, c+1))

percent ('=':xs) p@(r, c) = (percentEqualsToken p):(start xs (r, c+2))
percent s p@(r, c) = (percentToken p):(start s (r, c+1))

ampersand ('=':xs) p@(r, c) = (ampersandEqualsToken p):(start xs (r, c+2))
ampersand ('&':xs) p@(r, c) = (ampersandAmpersandToken p):(start xs (r, c+2))
ampersand s p@(r, c) = (ampersandToken p):(start s (r, c+1))

lbracket s p@(r, c) = (lbracketToken p):(start s (r, c+1))

rbracket s p@(r, c) = (rbracketToken p):(start s (r, c+1))

caret ('=':xs) p@(r, c) = (caretEqualsToken p):(start xs (r, c+2))
caret s p@(r, c) = (caretToken p):(start s (r, c+1))

lbrace s p@(r, c) = (lbraceToken p):(start s (r, c+1))

rbrace s p@(r, c) = (rbraceToken p):(start s (r, c+1))

tilde s p@(r, c) = (tildeToken p):(start s (r, c+1))

pipe ('=':xs) p@(r, c) = (pipeEqualsToken p):(start xs (r, c+2))
pipe ('|':xs) p@(r, c) = (pipePipeToken p):(start xs (r, c+2))
pipe s p@(r, c) = (Token T_IDENT (show p) p):(start s (r, c+1))



-- Comment states
comment :: String -> (Int, Int) -> [Token]
comment ('*':xs) (r, c) = commentStar xs (r, c+1)
comment (x:xs) (r, c) = comment xs (r, c+1)
comment [] (r, c) = error $ "error: missing end of comment at (" ++ (show r) ++ ", " ++ (show c) ++ ")"
commentStar ('/':xs) (r, c) = start xs (r, c+1) -- end of comment
commentStar s (r, c) = comment s (r, c)



-- Alphanumeric states
alpha :: String -> String -> (Int, Int) -> [Token]
alpha as (x:xs) (r, c)
    | Char.isAlphaNum x || x == '_' = alpha (as ++ [x]) xs (r, c)
alpha as s p@(r, c) = (alphaToken as):(start s (r, c+(length as)))
    where alphaToken alpha
            | (isKeyword alpha) = Token T_KEYWORD alpha p
            | otherwise = Token T_IDENT alpha p
            

-- Integer literal states
digit :: String -> String -> (Int, Int) -> [Token]
digit ds (x:xs) (r, c)
    | Char.isDigit x = digit (ds ++ [x]) xs (r, c)
digit ds s p@(r, c) = intToken:(start s (r, c+(length ds)))
    where intToken = Token T_INTEGER_LITERAL ds p









exclEqualsToken p               = Token T_EXCL_EQUALS "!=" p
exclToken p                     = Token T_EXCL "!" p
lparenToken p                   = Token T_LPAREN "(" p
rparenToken p                   = Token T_RPAREN ")" p
starEqualsToken p               = Token T_STAR_EQUALS "*=" p
starToken p                     = Token T_STAR "*" p
plusPlusToken p                 = Token T_PLUS_PLUS "++" p
plusEqualsToken p               = Token T_PLUS_EQUALS "+=" p
plusToken p                     = Token T_PLUS "+" p
commaToken p                    = Token T_COMMA "," p
minusMinusToken p               = Token T_MINUS_MINUS "--" p
minusEqualsToken p              = Token T_MINUS_EQUALS "-=" p
minusToken p                    = Token T_MINUS "-" p
dotToken p                      = Token T_DOT "." p
slashToken p                    = Token T_SLASH "/" p
slashEqualsToken p              = Token T_SLASH_EQUALS "/=" p
colonToken p                    = Token T_COLON ":" p
semicolonToken p                = Token T_SEMICOLON ";" p
langleToken p                   = Token T_LANGLE "<" p
langleLangleToken p             = Token T_LANGLE_LANGLE "<<" p
langleEqualsToken p             = Token T_LANGLE_EQUALS "<=" p
langleLangleEqualsToken p       = Token T_LANGLE_LANGLE_EQUALS "<<=" p
rangleToken p                   = Token T_RANGLE ">" p
rangleRangleToken p             = Token T_RANGLE_RANGLE ">>" p
rangleRangleRangleToken p       = Token T_RANGLE_RANGLE_RANGLE ">>>" p
rangleEqualsToken p             = Token T_RANGLE_EQUALS ">=" p
rangleRangleEqualsToken p       = Token T_RANGLE_RANGLE_EQUALS ">>=" p
rangleRangleRangleEqualsToken p = Token T_RANGLE_RANGLE_RANGLE_EQUALS ">>>=" p
equalsToken p                   = Token T_EQUALS "=" p
equalsEqualsToken p             = Token T_EQUALS_EQUALS "=" p
questionToken p                 = Token T_QUESTION "?" p
percentToken p                  = Token T_PERCENT "%" p
percentEqualsToken p            = Token T_PERCENT_EQUALS "%=" p
ampersandToken p                = Token T_AMPERSAND "&" p
ampersandEqualsToken p          = Token T_AMPERSAND_EQUALS "&=" p
ampersandAmpersandToken p       = Token T_AMPERSAND_AMPERSAND "&&" p
lbracketToken p                 = Token T_LBRACKET "[" p
rbracketToken p                 = Token T_RBRACKET "]" p
caretToken p                    = Token T_CARET "^" p
caretEqualsToken p              = Token T_CARET_EQUALS "^=" p
lbraceToken p                   = Token T_LBRACE "{" p
rbraceToken p                   = Token T_RBRACE "}" p
tildeToken p                    = Token T_TILDE "~" p
pipeToken p                     = Token T_PIPE "|" p
pipePipeToken p                 = Token T_PIPE_PIPE "||" p
pipeEqualsToken p               = Token T_PIPE_EQUALS "|=" p






        
isKeyword "abstract" = True
isKeyword "assert" = True
isKeyword "boolean" = True
isKeyword "break" = True
isKeyword "byte" = True
isKeyword "case" = True
isKeyword "catch" = True
isKeyword "char" = True
isKeyword "class" = True
isKeyword "const" = True
isKeyword "continue" = True
isKeyword "default" = True
isKeyword "double" = True
isKeyword "do" = True
isKeyword "else" = True
isKeyword "enum" = True
isKeyword "extends" = True
isKeyword "false" = True
isKeyword "finally" = True
isKeyword "final" = True
isKeyword "float" = True
isKeyword "for" = True
isKeyword "goto" = True
isKeyword "if" = True
isKeyword "implements" = True
isKeyword "import" = True
isKeyword "instanceof" = True
isKeyword "interface" = True
isKeyword "int" = True
isKeyword "long" = True
isKeyword "native" = True
isKeyword "new" = True
isKeyword "null" = True
isKeyword "package" = True
isKeyword "private" = True
isKeyword "protected" = True
isKeyword "public" = True
isKeyword "return" = True
isKeyword "short" = True
isKeyword "static" = True
isKeyword "strictfp" = True
isKeyword "super" = True
isKeyword "switch" = True
isKeyword "synchronized" = True
isKeyword "this" = True
isKeyword "throws" = True
isKeyword "throw" = True
isKeyword "transient" = True
isKeyword "true" = True
isKeyword "try" = True
isKeyword "void" = True
isKeyword "volatile" = True
isKeyword "while" = True
isKeyword s = False
