module Parser where

import Lexer


type Ident = String
          
data Program = Program ClassDeclarations deriving (Show)
        
data ClassDeclarations = CDs ClassDeclaration ClassDeclarations | CDEps deriving (Show)

data ClassDeclaration = CD Ident ClassMembers deriving (Show)

data ClassMembers = CMs ClassMember ClassMembers | CMEps deriving (Show)

data ClassMember = CMMainMethod MainMethod | CMFieldMethod FieldMethod deriving (Show) 

data FieldMethod = FM Type Ident FieldMethod' deriving (Show) 
data FieldMethod' = FM' Parameters Block | FM'Semi deriving (Show) 

data MainMethod = MainMethod Ident Ident Block deriving (Show)

data Parameters = Pas Parameter Parameters' | PasEps deriving (Show) 
data Parameters' = Pas' Parameter Parameters' | Pas'Eps deriving (Show) 
data Parameter = Pa Type Ident deriving (Show) 

data Type = Ty BasicType Brackets deriving (Show) 

data BasicType = T_Int Token | T_Boolean Token | T_Void Token | T_Ident Token deriving (Show) 

data Brackets = Br Brackets | BrEps deriving (Show) 


data Statement = SBl Block | SIf IfStatement | SEm EmptyStatement | SEx ExpressionStatement | SWh WhileStatement | SRe ReturnStatement deriving (Show) 
    
data Block = Bl BlockStatements deriving (Show) 
data BlockStatements = BSs BlockStatement BlockStatements | BSsEps deriving (Show) 
data BlockStatement = BS1 Statement | BS2 LocalVariableDeclarationStatement deriving (Show) 

data LocalVariableDeclarationStatement = LVDS Type Ident LVDS' deriving (Show) 
data LVDS' = LVDS' Expression | LVDS'Eps deriving (Show) 

data IfStatement = If Expression Statement IfStatement' deriving (Show) 
data IfStatement' = If' Statement | If'Eps deriving (Show) 

data EmptyStatement = EmS deriving (Show) 

data ExpressionStatement = ES Expression deriving (Show) 

data WhileStatement = WS Expression Statement deriving (Show) 

data ReturnStatement = RS | RSE Expression deriving (Show) 




-- data Expression = Expr String deriving (Show)

data Expression = Expr AssignmentExpression deriving (Show)

data AssignmentExpression = AE LogicalOrExpression AssignmentExpression' deriving (Show)
data AssignmentExpression' = AE' AssignmentExpression | AE'Eps deriving (Show)

data LogicalOrExpression = LOE LogicalAndExpression LogicalOrExpression' deriving (Show)
data LogicalOrExpression' = LOE' LogicalAndExpression LogicalOrExpression' | LOE'Eps deriving (Show)

data LogicalAndExpression = LAE EqualityExpression LogicalAndExpression' deriving (Show)
data LogicalAndExpression' = LAE' EqualityExpression LogicalAndExpression' | LAE'Eps deriving (Show)

data EqualityExpression = EqE RelationalExpression EqualityExpression' deriving (Show)
data EqualityExpression' = EqE'Eq RelationalExpression EqualityExpression'
                         | EqE'Neq RelationalExpression EqualityExpression'
                         | EqE'Eps deriving (Show)

data RelationalExpression = RE AdditiveExpression RelationalExpression' deriving (Show)
data RelationalExpression' = RE'Lt AdditiveExpression RelationalExpression'
                           | RE'LtEq AdditiveExpression RelationalExpression'
                           | RE'Gt AdditiveExpression RelationalExpression'
                           | RE'GtEq AdditiveExpression RelationalExpression'
                           | RE'Eps deriving (Show)

data AdditiveExpression = AdE MultiplicativeExpression AdditiveExpression' deriving (Show)
data AdditiveExpression' = AdE'Plus MultiplicativeExpression AdditiveExpression'
                         | AdE'Minus MultiplicativeExpression AdditiveExpression'
                         | AdE'Eps deriving (Show)
                         
data MultiplicativeExpression = ME UnaryExpression MultiplicativeExpression' deriving (Show)
data MultiplicativeExpression' = ME'Mul UnaryExpression MultiplicativeExpression'
                               | ME'Div UnaryExpression MultiplicativeExpression'
                               | ME'Mod UnaryExpression MultiplicativeExpression'
                               | ME'Eps deriving (Show)
                               
data UnaryExpression = UEPost PostfixExpression | UENeg UnaryExpression | UEMinus UnaryExpression deriving (Show)

data PostfixExpression = PfE PrimaryExpression PostfixOps deriving (Show)
data PostfixOps = PfOs PostfixOp PostfixOps | PfOsEps deriving (Show)
data PostfixOp = PfOMI MethodInvocation | PfOFA FieldAccess | PfOAA ArrayAccess deriving (Show)

data MethodInvocation = MI Ident Arguments deriving (Show)

data FieldAccess = FA Ident deriving (Show)

data ArrayAccess = AA Expression deriving (Show)

data Arguments = Arguments Expression Arguments' | ArgumentsEps deriving (Show)
data Arguments' = Arguments' Expression Arguments' | Arguments'Eps deriving (Show)

data PrimaryExpression = PENull Token
                       | PEFalse Token
                       | PETrue Token
                       | PEInteger Token
                       | PEIdent Token
                       | PEIdentArg Token Arguments
                       | PEThis Token
                       | PEExpr Expression
                       | PENewObj NewObjectExpression
                       | PENewArr NewArrayExpression deriving (Show)
                       
data NewObjectExpression = NewObjectExpression Ident deriving (Show)

data NewArrayExpression = NewArrayExpression BasicType Expression Brackets deriving (Show)




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


parseClassDeclarations :: [Token] -> Maybe (ClassDeclarations, [Token])
parseClassDeclarations ts@((Token T_KEYWORD "class" _):tss) = do
    (cd, rest1) <- parseClassDeclaration ts
    (cds, rest2) <- parseClassDeclarations rest1
    return (CDs cd cds, rest2)
    
parseClassDeclarations ts = Just (CDEps, ts)


parseClassDeclaration :: [Token] -> Maybe (ClassDeclaration, [Token])
parseClassDeclaration ((Token T_KEYWORD "class" _)
                      :(Token T_IDENT ident _)
                      :(Token T_LBRACE _ _)
                      :ts) = do
                          (cms, (Token T_RBRACE _ _):ts') <- parseClassMembers ts
                          return (CD ident cms, ts')
                          
parseClassDeclaration _ = Nothing

        
parseClassMembers :: [Token] -> Maybe (ClassMembers, [Token])
parseClassMembers ts@((Token T_RBRACE _ _):tss) = Just (CMEps, ts)
parseClassMembers ts = do
    (cm, ts') <- parseClassMember ts
    (cms, ts'') <- parseClassMembers ts'
    return (CMs cm cms, ts'')

        
parseClassMember :: [Token] -> Maybe (ClassMember, [Token])
parseClassMember ts@((Token T_KEYWORD "public" _):(Token T_KEYWORD "static" _):tss) = do
    (mm, ts') <- parseMainMethod ts
    return (CMMainMethod mm, ts')
    
parseClassMember ts = do
    (fm, ts') <- parseFieldMethod ts
    return (CMFieldMethod fm, ts')

        
parseMainMethod :: [Token] -> Maybe (MainMethod, [Token])
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
                    return (MainMethod na pa bl, ts')
                    
parseMainMethod _ = Nothing
        
        
parseFieldMethod :: [Token] -> Maybe (FieldMethod, [Token])
parseFieldMethod ((Token T_KEYWORD "public" _):ts) = do
    (ty, (Token T_IDENT na _):ts') <- parseType ts
    (fm', ts'') <- parseFieldMethod' ts'
    return (FM ty na fm', ts'')
        
parseFieldMethod _ = fail "parseFieldMethod failed"
        

parseFieldMethod' :: [Token] -> Maybe (FieldMethod', [Token])
parseFieldMethod' ((Token T_LPAREN _ _):ts) = do
        (pas, (Token T_RPAREN _ _):ts') <- parseParameters ts
        (bl, ts'') <- parseBlock ts'
        return (FM' pas bl, ts'')

parseFieldMethod' ((Token T_SEMICOLON _ _):ts) = Just (FM'Semi, ts)

parseFieldMethod' _ = Nothing


parseParameters :: [Token] -> Maybe (Parameters, [Token])
parseParameters ts@((Token T_KEYWORD na _):tss) = do
        (pa, ts') <- parseParameter ts
        (pas, ts'') <- parseParameters' ts'
        return (Pas pa pas, ts'')
        
parseParameters ts@((Token T_IDENT na _):tss) = do
        (pa, ts') <- parseParameter ts
        (pas, ts'') <- parseParameters' ts'
        return (Pas pa pas, ts'')
        
parseParameters ts = Just (PasEps, ts)


parseParameters' :: [Token] -> Maybe (Parameters', [Token])
parseParameters' ((Token T_COMMA _ _):ts) = do
        (pa, ts') <- parseParameter ts
        (pas, ts'') <- parseParameters' ts'
        return (Pas' pa pas, ts'')
        
parseParameters' ts = Just (Pas'Eps, ts)
        

parseParameter :: [Token] -> Maybe (Parameter, [Token])
parseParameter ts = do
        (ty, (Token T_IDENT na _):ts') <- parseType ts
        return (Pa ty na, ts')
        
        
parseType :: [Token] -> Maybe (Type, [Token])
parseType ts = do
        (bt, ts') <- parseBasicType ts
        (bs, ts'') <- parseBrackets ts'
        return (Ty bt bs, ts'')
        
        
parseBrackets :: [Token] -> Maybe (Brackets, [Token])
parseBrackets ((Token T_LBRACKET _ _):(Token T_RBRACKET _ _):ts) = do
        (bs, ts') <- parseBrackets ts
        return (Br bs, ts')
        
parseBrackets ts = Just (BrEps, ts)

parseBasicType :: [Token] -> Maybe (BasicType, [Token])
parseBasicType (tok@(Token T_KEYWORD "int" _):ts) = Just (T_Int tok, ts)
parseBasicType (tok@(Token T_KEYWORD "boolean" _):ts) = Just (T_Boolean tok, ts)
parseBasicType (tok@(Token T_KEYWORD "void" _):ts) = Just (T_Void tok, ts)
parseBasicType (tok@(Token T_IDENT _ _):ts) = Just (T_Ident tok, ts)
parseBasicType _ = Nothing


parseStatement :: [Token] -> Maybe (Statement, [Token])
parseStatement ts@((Token ty na _):_) = do
    let exprStmtRes = do
        (res, ts') <- parseExpressionStatement ts
        return (SEx res, ts')
    case (ty, na) of
        (T_LBRACE, _) -> do
            (res, ts') <- parseBlock ts
            return (SBl res, ts')
        (T_KEYWORD, "if") -> do
            (res, ts') <- parseIfStatement ts
            return (SIf res, ts')
        (T_SEMICOLON, _) -> do
            (res, ts') <- parseEmptyStatement ts
            return (SEm res, ts')
        (T_KEYWORD, "while") -> do
            (res, ts') <- parseWhileStatement ts
            return (SWh res, ts')
        (T_KEYWORD, "return") -> do
            (res, ts') <- parseReturnStatement ts
            return (SRe res, ts')
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
        
   
parseBlock :: [Token] -> Maybe (Block, [Token])     
parseBlock ((Token T_LBRACE _ _):ts) = do
    (bs, (Token T_RBRACE _ _):ts') <- parseBlockStatements ts
    return (Bl bs, ts')
    
parseBlock _ = Nothing
        
parseBlockStatements :: [Token] -> Maybe (BlockStatements, [Token])  
parseBlockStatements ts@((Token T_RBRACE _ _):_) = Just (BSsEps, ts)
        
parseBlockStatements ts = do
    (bs, ts') <- parseBlockStatement ts
    (bss, ts'') <- parseBlockStatements ts'
    return (BSs bs bss, ts'')
        
parseBlockStatement :: [Token] -> Maybe (BlockStatement, [Token])  
parseBlockStatement ts@((Token ty na _):(Token ty1 na1 _):(Token ty2 na2 _):_) = do
    let stmtRes = do
        (res, ts') <- parseStatement ts
        return (BS1 res, ts')
    let lvdsRes = do
        (res, ts') <- parseLocalVariableDeclarationStatement ts
        return (BS2 res, ts')
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

parseLocalVariableDeclarationStatement :: [Token] -> Maybe (LocalVariableDeclarationStatement, [Token])
parseLocalVariableDeclarationStatement ts = do
        (ty, (Token T_IDENT na _):ts') <- parseType ts
        (lvds', (Token T_SEMICOLON _ _):ts'') <- parseLVDS' ts'
        return (LVDS ty na lvds', ts'')
       
parseLVDS' :: [Token] -> Maybe (LVDS', [Token]) 
parseLVDS' ((Token T_EQUALS _ _):ts) = do
        (ex, ts') <- parseExpression ts 
        return (LVDS' ex, ts')
        
parseLVDS' ts@((Token T_SEMICOLON _ _):_) = Just (LVDS'Eps, ts)

parseLVDS' _ = Nothing
            
parseEmptyStatement :: [Token] -> Maybe (EmptyStatement, [Token]) 
parseEmptyStatement ((Token T_SEMICOLON _ _):ts) = Just (EmS, ts)

parseEmptyStatement _ = Nothing

parseWhileStatement :: [Token] -> Maybe (WhileStatement, [Token]) 
parseWhileStatement ((Token T_KEYWORD "while" _):(Token T_LPAREN _ _):ts) = do
        (ex, (Token T_RPAREN _ _):ts') <- parseExpression ts
        (s, ts'') <- parseStatement ts'
        return (WS ex s, ts'')
        
parseWhileStatement _ = Nothing
        
parseIfStatement :: [Token] -> Maybe (IfStatement, [Token]) 
parseIfStatement ((Token T_KEYWORD "if" _):(Token T_LPAREN _ _):ts) = do
        (ex, (Token T_RPAREN _ _):ts') <- parseExpression ts
        (s, ts'') <- parseStatement ts'
        (if', ts''') <- parseIfStatement' ts''
        return (If ex s if', ts''')
            
parseIfStatement _ = Nothing
            
parseIfStatement' :: [Token] -> Maybe (IfStatement', [Token]) 
parseIfStatement' ((Token T_KEYWORD "else" _):ts) = do
        (s, ts') <- parseStatement ts
        return (If' s, ts')

parseIfStatement' ts = Just (If'Eps, ts)

parseExpressionStatement :: [Token] -> Maybe (ExpressionStatement, [Token]) 
parseExpressionStatement ts = do
        (e, (Token T_SEMICOLON _ _):ts') <- parseExpression ts
        return (ES e, ts')
        
parseReturnStatement :: [Token] -> Maybe (ReturnStatement, [Token])
parseReturnStatement ((Token T_KEYWORD "return" _):(Token T_SEMICOLON _ _):ts) = Just (RS, ts)

parseReturnStatement ((Token T_KEYWORD "return" _):ts) = do
        (ex, (Token T_SEMICOLON _ _):ts') <- parseExpression ts
        return (RSE ex, ts')
        
parseReturnStatement _ = Nothing



-- Expressions
--------------
            
parseExpression :: [Token] -> Maybe (Expression, [Token])
parseExpression ts = do
    (ae, ts') <- parseAssignmentExpression ts
    return (Expr ae, ts')



parseAssignmentExpression :: [Token] -> Maybe (AssignmentExpression, [Token])
parseAssignmentExpression ts = do
        (oe, ts') <- parseLogicalOrExpression ts
        (ae, ts'') <- parseAssignmentExpression' ts'
        return (AE oe ae, ts'')

parseAssignmentExpression' :: [Token] -> Maybe (AssignmentExpression', [Token])
parseAssignmentExpression' ((Token T_EQUALS _ _):ts) = do
        (ae, ts') <- parseAssignmentExpression ts
        return (AE' ae, ts')
        
parseAssignmentExpression' ts = Just (AE'Eps, ts)



parseLogicalOrExpression :: [Token] -> Maybe (LogicalOrExpression, [Token])
parseLogicalOrExpression ts = do
        (ae, ts') <- parseLogicalAndExpression ts
        (oe', ts'') <- parseLogicalOrExpression' ts'
        return (LOE ae oe', ts'')

parseLogicalOrExpression' :: [Token] -> Maybe (LogicalOrExpression', [Token])
parseLogicalOrExpression' ((Token T_PIPE_PIPE _ _):ts) = do
        (ae, ts') <- parseLogicalAndExpression ts
        (oe', ts'') <- parseLogicalOrExpression' ts'
        return (LOE' ae oe', ts'')
        
parseLogicalOrExpression' ts = Just (LOE'Eps, ts)
       
       
parseLogicalAndExpression :: [Token] -> Maybe (LogicalAndExpression, [Token])
parseLogicalAndExpression ts = do
        (ee, ts') <- parseEqualityExpression ts
        (ae', ts'') <- parseLogicalAndExpression' ts'
        return (LAE ee ae', ts'')
        
parseLogicalAndExpression' :: [Token] -> Maybe (LogicalAndExpression', [Token])
parseLogicalAndExpression' ((Token T_AMPERSAND_AMPERSAND _ _):ts) = do
        (ee, ts') <- parseEqualityExpression ts
        (ae', ts'') <- parseLogicalAndExpression' ts'
        return (LAE' ee ae', ts'')
        
parseLogicalAndExpression' ts = Just (LAE'Eps, ts)



parseEqualityExpression :: [Token] -> Maybe (EqualityExpression, [Token])
parseEqualityExpression ts = do
        (re, ts') <- parseRelationalExpression ts
        (ee', ts'') <- parseEqualityExpression' ts'
        return (EqE re ee', ts'')

parseEqualityExpression' :: [Token] -> Maybe (EqualityExpression', [Token])
parseEqualityExpression' ((Token T_EQUALS_EQUALS _ _):ts) = do
        (re, ts') <- parseRelationalExpression ts
        (ee', ts'') <- parseEqualityExpression' ts'
        return (EqE'Eq re ee', ts'')
        
parseEqualityExpression' ((Token T_EXCL_EQUALS _ _):ts) = do
        (re, ts') <- parseRelationalExpression ts
        (ee', ts'') <- parseEqualityExpression' ts'
        return (EqE'Neq re ee', ts'')
        
parseEqualityExpression' ts = Just (EqE'Eps, ts)



parseRelationalExpression :: [Token] -> Maybe (RelationalExpression, [Token])
parseRelationalExpression ts = do
        (ae, ts') <- parseAdditiveExpression ts
        (re', ts'') <- parseRelationalExpression' ts'
        return (RE ae re', ts'')

parseRelationalExpression' :: [Token] -> Maybe (RelationalExpression', [Token])
parseRelationalExpression' ((Token T_LANGLE _ _):ts) = do
        (ae, ts') <- parseAdditiveExpression ts
        (re', ts'') <- parseRelationalExpression' ts'
        return (RE'Lt ae re', ts'')
        
parseRelationalExpression' ((Token T_LANGLE_EQUALS _ _):ts) = do
        (ae, ts') <- parseAdditiveExpression ts
        (re', ts'') <- parseRelationalExpression' ts'
        return (RE'LtEq ae re', ts'')
        
parseRelationalExpression' ((Token T_RANGLE _ _):ts) = do
        (ae, ts') <- parseAdditiveExpression ts
        (re', ts'') <- parseRelationalExpression' ts'
        return (RE'Gt ae re', ts'')
        
parseRelationalExpression' ((Token T_RANGLE_EQUALS _ _):ts) = do
        (ae, ts') <- parseAdditiveExpression ts
        (re', ts'') <- parseRelationalExpression' ts'
        return (RE'GtEq ae re', ts'')
        
parseRelationalExpression' ts = Just (RE'Eps, ts)



parseAdditiveExpression :: [Token] -> Maybe (AdditiveExpression, [Token])
parseAdditiveExpression ts = do
        (me, ts') <- parseMultiplicativeExpression ts
        (ae', ts'') <- parseAdditiveExpression' ts'
        return (AdE me ae', ts'')

parseAdditiveExpression' :: [Token] -> Maybe (AdditiveExpression', [Token])
parseAdditiveExpression' ((Token T_PLUS _ _):ts) = do
        (me, ts') <- parseMultiplicativeExpression ts
        (ae', ts'') <- parseAdditiveExpression' ts'
        return (AdE'Plus me ae', ts'')
        
parseAdditiveExpression' ((Token T_MINUS _ _):ts) = do
        (me, ts') <- parseMultiplicativeExpression ts
        (ae', ts'') <- parseAdditiveExpression' ts'
        return (AdE'Minus me ae', ts'')
        
parseAdditiveExpression' ts = Just (AdE'Eps, ts)



parseMultiplicativeExpression :: [Token] -> Maybe (MultiplicativeExpression, [Token])
parseMultiplicativeExpression ts = do
        (ue, ts') <- parseUnaryExpression ts
        (me', ts'') <- parseMultiplicativeExpression' ts'
        return (ME ue me', ts'')
        
parseMultiplicativeExpression' :: [Token] -> Maybe (MultiplicativeExpression', [Token])
parseMultiplicativeExpression' ((Token T_STAR _ _):ts) = do
        (ue, ts') <- parseUnaryExpression ts
        (me', ts'') <- parseMultiplicativeExpression' ts'
        return (ME'Mul ue me', ts'')
        
parseMultiplicativeExpression' ((Token T_SLASH _ _):ts) = do
        (ue, ts') <- parseUnaryExpression ts
        (me', ts'') <- parseMultiplicativeExpression' ts'
        return (ME'Div ue me', ts'')
        
parseMultiplicativeExpression' ((Token T_PERCENT _ _):ts) = do
        (ue, ts') <- parseUnaryExpression ts
        (me', ts'') <- parseMultiplicativeExpression' ts'
        return (ME'Mod ue me', ts'')
        
parseMultiplicativeExpression' ts = Just (ME'Eps, ts)



parseUnaryExpression :: [Token] -> Maybe (UnaryExpression, [Token])
parseUnaryExpression ((Token T_EXCL _ _):ts) = do
        (ue, ts') <- parseUnaryExpression ts
        return (UENeg ue, ts')
        
parseUnaryExpression ((Token T_MINUS _ _):ts) = do
        (ue, ts') <- parseUnaryExpression ts
        return (UEMinus ue, ts')
        
parseUnaryExpression ts = do
        (pe, ts') <- parsePostfixExpression ts
        return (UEPost pe, ts')
        
        
        
parsePostfixExpression :: [Token] -> Maybe (PostfixExpression, [Token])
parsePostfixExpression ts = do
        (pe, ts') <- parsePrimaryExpression ts
        (ops, ts'') <- parsePostfixOps ts'
        return (PfE pe ops, ts'')
        
parsePostfixOps :: [Token] -> Maybe (PostfixOps, [Token])
parsePostfixOps ts@((Token T_DOT _ _):tss) = do
        (pfo, ts') <- parsePostfixOp ts
        (ops, ts'') <- parsePostfixOps ts'
        return (PfOs pfo ops, ts'')
        
parsePostfixOps ts@((Token T_LBRACKET _ _):tss) = do
        (pfo, ts') <- parsePostfixOp ts
        (ops, ts'') <- parsePostfixOps ts'
        return (PfOs pfo ops, ts'')
        
parsePostfixOps ts = Just (PfOsEps, ts)

parsePostfixOp :: [Token] -> Maybe (PostfixOp, [Token])
parsePostfixOp ts@((Token T_DOT _ _):(Token T_IDENT _ _):(Token T_LPAREN _ _):tss) = do
        (mi, ts') <- parseMethodInvocation ts
        return (PfOMI mi, ts')
        
parsePostfixOp ts@((Token T_DOT _ _):(Token T_IDENT _ _):tss) = do
        (fa, ts') <- parseFieldAccess ts
        return (PfOFA fa, ts')
        
parsePostfixOp ts@((Token T_LBRACKET _ _):tss) = do
        (aa, ts') <- parseArrayAccess ts
        return (PfOAA aa, ts')
        
parsePostfixOp _ = Nothing
        


parseMethodInvocation :: [Token] -> Maybe (MethodInvocation, [Token])
parseMethodInvocation ((Token T_DOT _ _):(Token T_IDENT na _):(Token T_LPAREN _ _):ts) = do
        (args, (Token T_RPAREN _ _):ts') <- parseArguments ts
        return (MI na args, ts')
        
parseMethodInvocation _ = Nothing

parseFieldAccess :: [Token] -> Maybe (FieldAccess, [Token])
parseFieldAccess ((Token T_DOT _ _):(Token T_IDENT na _):ts) = Just (FA na, ts)

parseFieldAccess _ = Nothing

parseArrayAccess :: [Token] -> Maybe (ArrayAccess, [Token])
parseArrayAccess ((Token T_LBRACKET _ _):ts) = do
        (ex, (Token T_RBRACKET _ _):ts') <- parseExpression ts
        return (AA ex, ts')
        
parseArrayAccess _ = Nothing
        
        
        
parseArguments :: [Token] -> Maybe (Arguments, [Token])
parseArguments ts@((Token ty na _):tss) = do
    let res = do 
        (ex, ts') <- parseExpression ts
        (args', ts'') <- parseArguments' ts'
        return (Arguments ex args', ts'')
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
      (_, _) -> return (ArgumentsEps, ts)
      
parseArguments _ = Nothing

parseArguments' :: [Token] -> Maybe (Arguments', [Token])
parseArguments' ((Token T_COMMA _ _):ts) = do
        (ex, ts') <- parseExpression ts
        (args', ts'') <- parseArguments' ts'
        return (Arguments' ex args', ts'')
        
parseArguments' ts = Just (Arguments'Eps, ts)



parsePrimaryExpression :: [Token] -> Maybe (PrimaryExpression, [Token])
parsePrimaryExpression (tok@(Token T_KEYWORD "null" _):ts) = Just (PENull tok, ts)
parsePrimaryExpression (tok@(Token T_KEYWORD "true" _):ts) = Just (PETrue tok, ts)
parsePrimaryExpression (tok@(Token T_KEYWORD "false" _):ts) = Just (PEFalse tok, ts)
parsePrimaryExpression (tok@(Token T_KEYWORD "this" _):ts) = Just (PEThis tok, ts)
parsePrimaryExpression (tok@(Token T_INTEGER_LITERAL _ _):ts) = Just (PEInteger tok, ts)

parsePrimaryExpression (tok@(Token T_IDENT _ _):(Token T_LPAREN _ _):ts) = do
        (args, (Token T_RPAREN _ _):ts') <- parseArguments ts
        return (PEIdentArg tok args, ts')
        
parsePrimaryExpression (tok@(Token T_IDENT _ _):ts) = Just (PEIdent tok, ts)

parsePrimaryExpression ((Token T_LPAREN _ _):ts) = do
        (ex, (Token T_RPAREN _ _):ts') <- parseExpression ts
        return (PEExpr ex, ts')
        
parsePrimaryExpression ts@((Token T_KEYWORD "new" _):(Token T_IDENT _ _):(Token T_LPAREN _ _):_) = do
        (obj, ts') <- parseNewObjectExpression ts
        return (PENewObj obj, ts')
        
parsePrimaryExpression ts = do
        (arr, ts') <- parseNewArrayExpression ts
        return (PENewArr arr, ts')
        
        
        
parseNewObjectExpression :: [Token] -> Maybe (NewObjectExpression, [Token])
parseNewObjectExpression ((Token T_KEYWORD "new" _)
                         :(Token T_IDENT na _)
                         :(Token T_LPAREN _ _)
                         :(Token T_RPAREN _ _)
                         :ts) = Just (NewObjectExpression na, ts)
                         
parseNewObjectExpression _ = Nothing
                         
parseNewArrayExpression :: [Token] -> Maybe (NewArrayExpression, [Token])
parseNewArrayExpression ((Token T_KEYWORD "new" _):ts) = do
        (ty, (Token T_LBRACKET _ _):ts') <- parseBasicType ts
        (ex, (Token T_RBRACKET _ _):ts'') <- parseExpression ts'
        (br, ts''') <- parseBrackets ts''
        return (NewArrayExpression ty ex br, ts''')
        
parseNewArrayExpression _ = Nothing