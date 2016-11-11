module Parser where

import Lexer


type Ident = String
          
data Ref = Ref Ident deriving (Show)

data Type = Type BasicType Int deriving (Show)

data BasicType = TypeInt | TypeBoolean | TypeVoid | UserType Ident deriving (Show)


data Program = Program [ClassDeclaration] deriving (Show)

data ClassDeclaration = ClassDeclaration Ident [ClassMember] deriving (Show)
    
data ClassMember = Field Type
                 | Method Type [Parameter] Statement
                 | MainMethod Ident Statement deriving (Show)

data Parameter = Parameter Ident Type deriving (Show)

data Statement = Block [Statement]
               | IfStatement Expression Statement | IfElseStatement Expression Statement Statement 
               | ExpressionStatement Expression 
               | WhileStatement Expression Statement
               | ReturnExpressionStatement Expression | ReturnStatement
               | EmptyStatement
               | LocalVariableDeclaration Type
               | LocalVariableExpressionDeclaration Type Expression deriving (Show)
               
               
               

data EqualityOp = NotEquals | Equals deriving (Show)

data RelationalOp = LessThan | LessThanOrEqual | GreaterThan | GreaterThanOrEqual deriving (Show)

data AddOp = Add | Subtract deriving (Show)

data MultOp = Multiply | Divide | Modulo deriving (Show)

data UnaryOp = Negate | Minus | MethodInvocation Ident [Expression] | FieldAccess Ident | ArrayAccess Expression deriving (Show)

data Expression = AssignmentExpression Expression Expression
                | LogicalOrExpression Expression Expression
                | LogicalAndExpression Expression Expression
                | EqualityExpression EqualityOp Expression Expression
                | RelationalExpression RelationalOp Expression Expression
                | AdditiveExpression AddOp Expression Expression
                | MultiplicativeExpression MultOp Expression Expression
                | CallExpression Ident [Expression]
                | UnaryExpression UnaryOp Expression
                | CNull
                | CFalse
                | CTrue
                | CIntegerLiteral Int
                | CRef Ident
                | CThis
                | NewObject Ident
                | NewArray Int BasicType Expression deriving (Show)
    





-- Implementation

parse :: [Token] -> Maybe Program
parse ts = do
    (p, ts') <- parseProgram ts
    case ts' of
        [] -> Just p
        _ -> Nothing


parseProgram :: [Token] -> Maybe (Program, [Token])
parseProgram ts = do
    (decls, ts') <- parseClassDeclarations ts
    return (Program decls, ts')


parseClassDeclarations :: [Token] -> Maybe ([ClassDeclaration], [Token])
parseClassDeclarations ts@((Token T_KEYWORD "class" _):tss) = do
    (cd, rest1) <- parseClassDeclaration ts
    (cds, rest2) <- parseClassDeclarations rest1
    return (cd:cds, rest2)
    
parseClassDeclarations ts = Just ([], ts)


parseClassDeclaration :: [Token] -> Maybe (ClassDeclaration, [Token])
parseClassDeclaration ((Token T_KEYWORD "class" _)
                      :(Token T_IDENT ident _)
                      :(Token T_LBRACE _ _)
                      :ts) = do
                          (cms, (Token T_RBRACE _ _):ts') <- parseClassMembers ts
                          return (ClassDeclaration ident cms, ts')
                          
parseClassDeclaration _ = Nothing

        
parseClassMembers :: [Token] -> Maybe ([ClassMember], [Token])
parseClassMembers ts@((Token T_RBRACE _ _):tss) = Just ([], ts)
parseClassMembers ts = do
    (cm, ts') <- parseClassMember ts
    (cms, ts'') <- parseClassMembers ts'
    return (cm:cms, ts'')

        
parseClassMember :: [Token] -> Maybe (ClassMember, [Token])
parseClassMember ts@((Token T_KEYWORD "public" _):(Token T_KEYWORD "static" _):tss) = do
    (mm, ts') <- parseMainMethod ts
    return (mm, ts')
    
parseClassMember ts = do
    (fm, ts') <- parseFieldMethod ts
    return (fm, ts')

        
parseMainMethod :: [Token] -> Maybe (ClassMember, [Token])
parseMainMethod ((Token T_KEYWORD "public" _)
                :(Token T_KEYWORD "static" _)
                :(Token T_KEYWORD "void" _)
                :(Token T_IDENT na _)
                :(Token T_LPAREN _ _)
                :(Token T_IDENT "String" _)
                :(Token T_LBRACKET _ _)
                :(Token T_RBRACKET _ _)
                :(Token T_IDENT pa _)
                :(Token T_RPAREN _ _)
                :ts) = do
                    (bl, ts') <- parseBlock ts
                    return (MainMethod pa bl, ts')
                    
parseMainMethod _ = Nothing
        
        
parseFieldMethod :: [Token] -> Maybe (ClassMember, [Token])
parseFieldMethod ((Token T_KEYWORD "public" _):ts) = do
    (ty, (Token T_IDENT na _):ts') <- parseType ts
    (fm', ts'') <- parseFieldMethod' ty ts'
    return (fm', ts'')
        
parseFieldMethod _ = fail "parseFieldMethod failed"
        

parseFieldMethod' :: Type -> [Token] -> Maybe (ClassMember, [Token])
parseFieldMethod' ty ((Token T_LPAREN _ _):ts) = do
        (pas, (Token T_RPAREN _ _):ts') <- parseParameters ts
        (bl, ts'') <- parseBlock ts'
        return (Method ty pas bl, ts'')

parseFieldMethod' ty ((Token T_SEMICOLON _ _):ts) = Just (Field ty, ts)

parseFieldMethod' _ _ = Nothing


parseParameters :: [Token] -> Maybe ([Parameter], [Token])
parseParameters ts@((Token T_KEYWORD na _):tss) = do
        (pa, ts') <- parseParameter ts
        (pas, ts'') <- parseParameters' ts'
        return (pa:pas, ts'')
        
parseParameters ts@((Token T_IDENT na _):tss) = do
        (pa, ts') <- parseParameter ts
        (pas, ts'') <- parseParameters' ts'
        return (pa:pas, ts'')
        
parseParameters ts = Just ([], ts)


parseParameters' :: [Token] -> Maybe ([Parameter], [Token])
parseParameters' ((Token T_COMMA _ _):ts) = do
        (pa, ts') <- parseParameter ts
        (pas, ts'') <- parseParameters' ts'
        return (pa:pas, ts'')
        
parseParameters' ts = Just ([], ts)
        

parseParameter :: [Token] -> Maybe (Parameter, [Token])
parseParameter ts = do
        (ty, (Token T_IDENT na _):ts') <- parseType ts
        return (Parameter na ty, ts')
        
        
parseType :: [Token] -> Maybe (Type, [Token])
parseType ts = do
        (basic, ts') <- parseBasicType ts
        (dim, ts'') <- parseBrackets ts'
        return (Type basic dim, ts'')
        
        
parseBrackets :: [Token] -> Maybe (Int, [Token])
parseBrackets ((Token T_LBRACKET _ _):(Token T_RBRACKET _ _):ts) = do
        (d, ts') <- parseBrackets ts
        return (d+1, ts')
        
parseBrackets ts = Just (0, ts)

parseBasicType :: [Token] -> Maybe (BasicType, [Token])
parseBasicType (tok@(Token T_KEYWORD "int" _):ts) = Just (TypeInt, ts)
parseBasicType (tok@(Token T_KEYWORD "boolean" _):ts) = Just (TypeBoolean, ts)
parseBasicType (tok@(Token T_KEYWORD "void" _):ts) = Just (TypeVoid, ts)
parseBasicType (tok@(Token T_IDENT na _):ts) = Just (UserType na, ts)
parseBasicType _ = Nothing


parseStatement :: [Token] -> Maybe (Statement, [Token])
parseStatement ts@((Token ty na _):_) = do
    let exprStmtRes = do
        (res, ts') <- parseExpressionStatement ts
        return (res, ts')
    case (ty, na) of
        (T_LBRACE, _) -> do
            (res, ts') <- parseBlock ts
            return (res, ts')
        (T_KEYWORD, "if") -> do
            (res, ts') <- parseIfStatement ts
            return (res, ts')
        (T_SEMICOLON, _) -> do
            (res, ts') <- parseEmptyStatement ts
            return (res, ts')
        (T_KEYWORD, "while") -> do
            (res, ts') <- parseWhileStatement ts
            return (res, ts')
        (T_KEYWORD, "return") -> do
            (res, ts') <- parseReturnStatement ts
            return (res, ts')
        (T_EXCL, _) -> exprStmtRes
        (T_MINUS, _) -> exprStmtRes
        (T_LPAREN, _) -> exprStmtRes
        (T_KEYWORD, "null") -> exprStmtRes
        (T_KEYWORD, "false") -> exprStmtRes
        (T_KEYWORD, "true") -> exprStmtRes
        (T_KEYWORD, "this") -> exprStmtRes
        (T_KEYWORD, "new") -> exprStmtRes
        (T_INTEGER_LITERAL, _) -> exprStmtRes
        (T_IDENT, _) -> exprStmtRes
        (_, _) -> Nothing
        
parseStatement _ = Nothing
        
   
parseBlock :: [Token] -> Maybe (Statement, [Token])     
parseBlock ((Token T_LBRACE _ _):ts) = do
    (stmts, (Token T_RBRACE _ _):ts') <- parseBlockStatements ts
    return (Block stmts, ts')
    
parseBlock _ = Nothing
        
parseBlockStatements :: [Token] -> Maybe ([Statement], [Token])
parseBlockStatements ts@((Token T_RBRACE _ _):_) = Just ([], ts)
        
parseBlockStatements ts = do
    (stmt, ts') <- parseBlockStatement ts
    (stmts, ts'') <- parseBlockStatements ts'
    return (stmt:stmts, ts'')
        
parseBlockStatement :: [Token] -> Maybe (Statement, [Token])  
parseBlockStatement ts@((Token ty na _):(Token ty1 na1 _):(Token ty2 na2 _):_) = do
    let stmtRes = do
        (res, ts') <- parseStatement ts
        return (res, ts')
    let lvdsRes = do
        (res, ts') <- parseLocalVariableDeclarationStatement ts
        return (res, ts')
    case (ty, na) of
        (T_LBRACE, _) -> stmtRes
        (T_KEYWORD, "if") -> stmtRes
        (T_SEMICOLON, _) -> stmtRes
        (T_KEYWORD, "while") -> stmtRes
        (T_KEYWORD, "return") -> stmtRes
        (T_EXCL, _) -> stmtRes
        (T_MINUS, _) -> stmtRes
        (T_LPAREN, _) -> stmtRes
        (T_KEYWORD, "null") -> stmtRes
        (T_KEYWORD, "false") -> stmtRes
        (T_KEYWORD, "true") -> stmtRes
        (T_KEYWORD, "this") -> stmtRes
        (T_KEYWORD, "new") -> stmtRes
        (T_INTEGER_LITERAL, _) -> stmtRes
        (T_KEYWORD, "int") -> lvdsRes
        (T_KEYWORD, "boolean") -> lvdsRes
        (T_KEYWORD, "void") -> lvdsRes
        (T_IDENT, _) -> case (ty1, na1) of
            (T_IDENT, _) -> lvdsRes
            (T_LBRACKET, _) -> case (ty2, na2) of
                (T_RBRACKET, _) -> lvdsRes
                (_, _) -> stmtRes
            (_, _) -> stmtRes
        (_, _) -> fail "parseBlockStatement"
        
parseBlockStatement _ = Nothing

parseLocalVariableDeclarationStatement :: [Token] -> Maybe (Statement, [Token])
parseLocalVariableDeclarationStatement ts = do
        (ty, (Token T_IDENT na _):ts') <- parseType ts
        (lvds', (Token T_SEMICOLON _ _):ts'') <- parseLVDS' ty ts'
        return (lvds', ts'')
       
parseLVDS' :: Type -> [Token] -> Maybe (Statement, [Token]) 
parseLVDS' ty ((Token T_EQUALS _ _):ts) = do
        (ex, ts') <- parseExpression ts
        return (LocalVariableExpressionDeclaration ty ex, ts')
        
parseLVDS' ty ts@((Token T_SEMICOLON _ _):_) = Just (LocalVariableDeclaration ty, ts)

parseLVDS' _ _ = Nothing
            
parseEmptyStatement :: [Token] -> Maybe (Statement, [Token]) 
parseEmptyStatement ((Token T_SEMICOLON _ _):ts) = Just (EmptyStatement, ts)

parseEmptyStatement _ = Nothing

parseWhileStatement :: [Token] -> Maybe (Statement, [Token]) 
parseWhileStatement ((Token T_KEYWORD "while" _):(Token T_LPAREN _ _):ts) = do
        (ex, (Token T_RPAREN _ _):ts') <- parseExpression ts
        (s, ts'') <- parseStatement ts'
        return (WhileStatement ex s, ts'')
        
parseWhileStatement _ = Nothing
        
parseIfStatement :: [Token] -> Maybe (Statement, [Token]) 
parseIfStatement ((Token T_KEYWORD "if" _):(Token T_LPAREN _ _):ts) = do
        (ex, (Token T_RPAREN _ _):ts') <- parseExpression ts
        (s, ts'') <- parseStatement ts'
        (if', ts''') <- parseIfStatement' ex s ts''
        return (if', ts''')
            
parseIfStatement _ = Nothing
            
parseIfStatement' :: Expression -> Statement -> [Token] -> Maybe (Statement, [Token]) 
parseIfStatement' ex s1 ((Token T_KEYWORD "else" _):ts) = do
        (s2, ts') <- parseStatement ts
        return (IfElseStatement ex s1 s2, ts')

parseIfStatement' ex s ts = Just (IfStatement ex s, ts)

parseExpressionStatement :: [Token] -> Maybe (Statement, [Token]) 
parseExpressionStatement ts = do
        (e, (Token T_SEMICOLON _ _):ts') <- parseExpression ts
        return (ExpressionStatement e, ts')
        
parseReturnStatement :: [Token] -> Maybe (Statement, [Token])
parseReturnStatement ((Token T_KEYWORD "return" _):(Token T_SEMICOLON _ _):ts) = Just (ReturnStatement, ts)

parseReturnStatement ((Token T_KEYWORD "return" _):ts) = do
        (ex, (Token T_SEMICOLON _ _):ts') <- parseExpression ts
        return (ReturnExpressionStatement ex, ts')
        
parseReturnStatement _ = Nothing






-- Expressions
--------------
            
parseExpression :: [Token] -> Maybe (Expression, [Token])
parseExpression ts = do
    (ae, ts') <- parseAssignmentExpression ts
    return (ae, ts')



parseAssignmentExpression :: [Token] -> Maybe (Expression, [Token])
parseAssignmentExpression ts = do
        (ex1, ts') <- parseLogicalOrExpression ts
        (ex2, ts'') <- parseAssignmentExpression' ex1 ts'
        return (ex2, ts'')

parseAssignmentExpression' :: Expression -> [Token] -> Maybe (Expression, [Token])
parseAssignmentExpression' left ((Token T_EQUALS _ _):ts) = do
        (ex1, ts') <- parseLogicalOrExpression ts
        let ex = AssignmentExpression left ex1
        (ex2, ts'') <- parseAssignmentExpression' ex ts'
        return (ex2, ts'')

parseAssignmentExpression' left ts = Just (left, ts)



parseLogicalOrExpression :: [Token] -> Maybe (Expression, [Token])
parseLogicalOrExpression ts = do
        (ex1, ts') <- parseLogicalAndExpression ts
        (ex2, ts'') <- parseLogicalOrExpression' ex1 ts'
        return (ex2, ts'')

parseLogicalOrExpression' :: Expression -> [Token] -> Maybe (Expression, [Token])
parseLogicalOrExpression' left ((Token T_PIPE_PIPE _ _):ts) = do
        (ex1, ts') <- parseLogicalAndExpression ts
        let ex = LogicalOrExpression left ex1
        (ex2, ts'') <- parseLogicalOrExpression' ex ts'
        return (ex2, ts'')

parseLogicalOrExpression' left ts = Just (left, ts)


parseLogicalAndExpression :: [Token] -> Maybe (Expression, [Token])
parseLogicalAndExpression ts = do
        (ex1, ts') <- parseEqualityExpression ts
        (ex2, ts'') <- parseLogicalAndExpression' ex1 ts'
        return (ex2, ts'')

parseLogicalAndExpression' :: Expression -> [Token] -> Maybe (Expression, [Token])
parseLogicalAndExpression' left ((Token T_AMPERSAND_AMPERSAND _ _):ts) = do
        (ex1, ts') <- parseEqualityExpression ts
        let ex = LogicalAndExpression left ex1
        (ex2, ts'') <- parseLogicalAndExpression' ex ts'
        return (ex2, ts'')

parseLogicalAndExpression' left ts = Just (left, ts)



parseEqualityExpression :: [Token] -> Maybe (Expression, [Token])
parseEqualityExpression ts = do
        (ex1, ts') <- parseRelationalExpression ts
        (ex2, ts'') <- parseEqualityExpression' ex1 ts'
        return (ex2, ts'')

parseEqualityExpression' :: Expression ->  [Token] -> Maybe (Expression, [Token])
parseEqualityExpression' left ((Token T_EQUALS_EQUALS _ _):ts) = do
        (ex1, ts') <- parseRelationalExpression ts
        let ex = EqualityExpression Equals left ex1
        (ex2, ts'') <- parseEqualityExpression' ex ts'
        return (ex2, ts'')

parseEqualityExpression' left ((Token T_EXCL_EQUALS _ _):ts) = do
        (ex1, ts') <- parseRelationalExpression ts
        let ex = EqualityExpression NotEquals left ex1
        (ex2, ts'') <- parseEqualityExpression' ex ts'
        return (ex2, ts'')

parseEqualityExpression' left ts = Just (left, ts)



parseRelationalExpression :: [Token] -> Maybe (Expression, [Token])
parseRelationalExpression ts = do
        (ex1, ts') <- parseAdditiveExpression ts
        (ex2, ts'') <- parseRelationalExpression' ex1 ts'
        return (ex2, ts'')

parseRelationalExpression' :: Expression -> [Token] -> Maybe (Expression, [Token])
parseRelationalExpression' left ((Token T_LANGLE _ _):ts) = do
        (ex1, ts') <- parseAdditiveExpression ts
        let ex = RelationalExpression LessThan left ex1
        (ex2, ts'') <- parseRelationalExpression' ex ts'
        return (ex2, ts'')

parseRelationalExpression' left ((Token T_LANGLE_EQUALS _ _):ts) = do
        (ex1, ts') <- parseAdditiveExpression ts
        let ex = RelationalExpression LessThanOrEqual left ex1
        (ex2, ts'') <- parseRelationalExpression' ex ts'
        return (ex2, ts'')

parseRelationalExpression' left ((Token T_RANGLE _ _):ts) = do
        (ex1, ts') <- parseAdditiveExpression ts
        let ex = RelationalExpression LessThan left ex1
        (ex2, ts'') <- parseRelationalExpression' ex ts'
        return (ex2, ts'')

parseRelationalExpression' left ((Token T_RANGLE_EQUALS _ _):ts) = do
        (ex1, ts') <- parseAdditiveExpression ts
        let ex = RelationalExpression LessThanOrEqual left ex1
        (ex2, ts'') <- parseRelationalExpression' ex ts'
        return (ex2, ts'')

parseRelationalExpression' left ts = Just (left, ts)



parseAdditiveExpression :: [Token] -> Maybe (Expression, [Token])
parseAdditiveExpression ts = do
        (ex1, ts') <- parseMultiplicativeExpression ts
        (ex2, ts'') <- parseAdditiveExpression' ex1 ts'
        return (ex2, ts'')

parseAdditiveExpression' :: Expression -> [Token] -> Maybe (Expression, [Token])
parseAdditiveExpression' left ((Token T_PLUS _ _):ts) = do
        (ex1, ts') <- parseMultiplicativeExpression ts
        let ex = AdditiveExpression Add left ex1
        (ex2, ts'') <- parseAdditiveExpression' ex ts'
        return (ex2, ts'')

parseAdditiveExpression' left ((Token T_MINUS _ _):ts) = do
        (ex1, ts') <- parseMultiplicativeExpression ts
        let ex = AdditiveExpression Subtract left ex1
        (ex2, ts'') <- parseAdditiveExpression' ex ts'
        return (ex2, ts'')

parseAdditiveExpression' left ts = Just (left, ts)



parseMultiplicativeExpression :: [Token] -> Maybe (Expression, [Token])
parseMultiplicativeExpression ts = do
        (ex1, ts') <- parseUnaryExpression ts
        (ex2, ts'') <- parseMultiplicativeExpression' ex1 ts'
        return (ex2, ts'')

parseMultiplicativeExpression' :: Expression -> [Token] -> Maybe (Expression, [Token])
parseMultiplicativeExpression' left ((Token T_STAR _ _):ts) = do
        (ex1, ts') <- parseUnaryExpression ts
        let ex = MultiplicativeExpression Multiply left ex1
        (ex2, ts'') <- parseMultiplicativeExpression' ex ts'
        return (ex2, ts'')

parseMultiplicativeExpression' left ((Token T_SLASH _ _):ts) = do
        (ex1, ts') <- parseUnaryExpression ts
        let ex = MultiplicativeExpression Divide left ex1
        (ex2, ts'') <- parseMultiplicativeExpression' ex ts'
        return (ex2, ts'')

parseMultiplicativeExpression' left ((Token T_PERCENT _ _):ts) = do
        (ex1, ts') <- parseUnaryExpression ts
        let ex = MultiplicativeExpression Modulo left ex1
        (ex2, ts'') <- parseMultiplicativeExpression' ex ts'
        return (ex2, ts'')

parseMultiplicativeExpression' left ts = Just (left, ts)



parseUnaryExpression :: [Token] -> Maybe (Expression, [Token])
parseUnaryExpression ((Token T_EXCL _ _):ts) = do
        (ex, ts') <- parseUnaryExpression ts
        return (UnaryExpression Negate ex, ts')

parseUnaryExpression ((Token T_MINUS _ _):ts) = do
        (ex, ts') <- parseUnaryExpression ts
        return (UnaryExpression Minus ex, ts')

parseUnaryExpression ts = do
        (pe, ts') <- parsePostfixExpression ts
        return (pe, ts')



parsePostfixExpression :: [Token] -> Maybe (Expression, [Token])
parsePostfixExpression ts = do
        (ex1, ts') <- parsePrimaryExpression ts
        (ex2, ts'') <- parsePostfixOps ex1 ts'
        return (ex2, ts'')

parsePostfixOps :: Expression -> [Token] -> Maybe (Expression, [Token])
parsePostfixOps left ts@((Token T_DOT _ _):tss) = do
        (ex1, ts') <- parsePostfixOp left ts
        (ex2, ts'') <- parsePostfixOps ex1 ts'
        return (ex2, ts'')

parsePostfixOps left ts@((Token T_LBRACKET _ _):tss) = do
        (ex1, ts') <- parsePostfixOp left ts
        (ex2, ts'') <- parsePostfixOps ex1 ts'
        return (ex2, ts'')

parsePostfixOps left ts = Just (left, ts)

parsePostfixOp :: Expression -> [Token] -> Maybe (Expression, [Token])
parsePostfixOp left ts@((Token T_DOT _ _):(Token T_IDENT _ _):(Token T_LPAREN _ _):tss) = do
        (ex, ts') <- parseMethodInvocation left ts
        return (ex, ts')

parsePostfixOp left ts@((Token T_DOT _ _):(Token T_IDENT _ _):tss) = do
        (ex, ts') <- parseFieldAccess left ts
        return (ex, ts')

parsePostfixOp left ts@((Token T_LBRACKET _ _):tss) = do
        (ex, ts') <- parseArrayAccess left ts
        return (ex, ts')

parsePostfixOp _ _ = Nothing



parseMethodInvocation :: Expression -> [Token] -> Maybe (Expression, [Token])
parseMethodInvocation left ((Token T_DOT _ _):(Token T_IDENT na _):(Token T_LPAREN _ _):ts) = do
        (args, (Token T_RPAREN _ _):ts') <- parseArguments ts
        return (UnaryExpression (MethodInvocation na args) left, ts')

parseMethodInvocation _ _ = Nothing

parseFieldAccess :: Expression -> [Token] -> Maybe (Expression, [Token])
parseFieldAccess left ((Token T_DOT _ _):(Token T_IDENT na _):ts) = Just (UnaryExpression (FieldAccess na) left, ts)

parseFieldAccess _ _ = Nothing

parseArrayAccess :: Expression -> [Token] -> Maybe (Expression, [Token])
parseArrayAccess left ((Token T_LBRACKET _ _):ts) = do
        (ex, (Token T_RBRACKET _ _):ts') <- parseExpression ts
        return (UnaryExpression (ArrayAccess ex) left, ts')

parseArrayAccess _ _ = Nothing



parseArguments :: [Token] -> Maybe ([Expression], [Token])
parseArguments ts@((Token ty na _):tss) = do
    let res = do
        (arg, ts') <- parseExpression ts
        (args', ts'') <- parseArguments' ts'
        return (arg:args', ts'')
    case (ty, na) of
      (T_EXCL, _) -> res
      (T_MINUS, _) -> res
      (T_LPAREN, _) -> res
      (T_KEYWORD, "null") -> res
      (T_KEYWORD, "false") -> res
      (T_KEYWORD, "true") -> res
      (T_KEYWORD, "this") -> res
      (T_KEYWORD, "new") -> res
      (T_INTEGER_LITERAL, _) -> res
      (T_IDENT, _) -> res
      (_, _) -> return ([], ts)

parseArguments _ = Nothing

parseArguments' :: [Token] -> Maybe ([Expression], [Token])
parseArguments' ((Token T_COMMA _ _):ts) = do
        (arg, ts') <- parseExpression ts
        (args', ts'') <- parseArguments' ts'
        return (arg:args', ts'')

parseArguments' ts = Just ([], ts)



parsePrimaryExpression :: [Token] -> Maybe (Expression, [Token])
parsePrimaryExpression (tok@(Token T_KEYWORD "null" _):ts) = Just (CNull, ts)
parsePrimaryExpression (tok@(Token T_KEYWORD "true" _):ts) = Just (CTrue, ts)
parsePrimaryExpression (tok@(Token T_KEYWORD "false" _):ts) = Just (CFalse, ts)
parsePrimaryExpression (tok@(Token T_KEYWORD "this" _):ts) = Just (CThis, ts)
parsePrimaryExpression (tok@(Token T_INTEGER_LITERAL s _):ts) = Just (CIntegerLiteral (read s), ts)

parsePrimaryExpression (tok@(Token T_IDENT na _):(Token T_LPAREN _ _):ts) = do
        (args, (Token T_RPAREN _ _):ts') <- parseArguments ts
        return (CallExpression na args, ts')

parsePrimaryExpression (tok@(Token T_IDENT na _):ts) = Just (CRef na, ts)

parsePrimaryExpression ((Token T_LPAREN _ _):ts) = do
        (ex, (Token T_RPAREN _ _):ts') <- parseExpression ts
        return (ex, ts')

parsePrimaryExpression ts@((Token T_KEYWORD "new" _):(Token T_IDENT _ _):(Token T_LPAREN _ _):_) = do
        (ex, ts') <- parseNewObjectExpression ts
        return (ex, ts')

parsePrimaryExpression ts = do
        (ex, ts') <- parseNewArrayExpression ts
        return (ex, ts')



parseNewObjectExpression :: [Token] -> Maybe (Expression, [Token])
parseNewObjectExpression ((Token T_KEYWORD "new" _)
                         :(Token T_IDENT na _)
                         :(Token T_LPAREN _ _)
                         :(Token T_RPAREN _ _)
                         :ts) = Just (NewObject na, ts)

parseNewObjectExpression _ = Nothing

parseNewArrayExpression :: [Token] -> Maybe (Expression, [Token])
parseNewArrayExpression ((Token T_KEYWORD "new" _):ts) = do
        (ty, (Token T_LBRACKET _ _):ts') <- parseBasicType ts
        (ex, (Token T_RBRACKET _ _):ts'') <- parseExpression ts'
        (dim, ts''') <- parseBrackets ts''
        return (NewArray dim ty ex, ts''')

parseNewArrayExpression _ = Nothing