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

parse :: [Token] -> Program
parse ts = p
    where
        (p, []) = parseProgram ts


parseProgram ts = (Program decls, ts')
    where
        (decls, ts') = parseClassDeclarations ts


parseClassDeclarations ts@((Token T_KEYWORD "class" _):tss) = (CDs cd cds, ts'')
    where
        (cd, ts') = parseClassDeclaration ts
        (cds, ts'') = parseClassDeclarations ts'
parseClassDeclarations [] = (CDEps, [])


parseClassDeclaration ((Token T_KEYWORD "class" _)
                      :(Token T_IDENT ident _)
                      :(Token T_LBRACE _ _)
                      :ts) = (CD ident cms, ts')
    where
        (cms, (Token T_RBRACE _ _):ts') = parseClassMembers ts

        
parseClassMembers ts@((Token T_RBRACE _ _):tss) = (CMEps, ts)
parseClassMembers ts = (CMs cm cms, ts'')
    where
        (cm, ts') = parseClassMember ts
        (cms, ts'') = parseClassMembers ts'

        
parseClassMember :: [Token] -> (ClassMember, [Token])
parseClassMember ts@((Token T_KEYWORD "public" _):(Token T_KEYWORD "static" _):tss) = (CMMainMethod mm, ts')
    where
        (mm, ts') = parseMainMethod ts
parseClassMember ts = (CMFieldMethod fm, ts')
    where
        (fm, ts') = parseFieldMethod ts

        
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
                :ts) = (MainMethod na pa bl, ts')
                    where (bl, ts') = parseBlock ts
        
        
parseFieldMethod ((Token T_KEYWORD "public" _):ts) = (FM ty na fm', ts'')
    where
        (ty, (Token T_IDENT na _):ts') = parseType ts
        (fm', ts'') = parseFieldMethod' ts'
        
parseFieldMethod _ = error "Expected field or method starting with \'public\'"
        
parseFieldMethod' ((Token T_LPAREN _ _):ts) = (FM' pas bl, ts'')
    where
        (pas, (Token T_RPAREN _ _):ts') = parseParameters ts
        (bl, ts'') = parseBlock ts'

parseFieldMethod' ((Token T_SEMICOLON _ _):ts) = (FM'Semi, ts)

parseParameters ts@((Token T_KEYWORD na _):tss) = (Pas pa pas, ts'')
    where
        (pa, ts') = parseParameter ts
        (pas, ts'') = parseParameters' ts'
        
parseParameters ts@((Token T_IDENT na _):tss) = (Pas pa pas, ts'')
    where
        (pa, ts') = parseParameter ts
        (pas, ts'') = parseParameters' ts'
        
parseParameters ts = (PasEps, ts)

parseParameters' ((Token T_COMMA _ _):ts) = (Pas' pa pas, ts'')
    where
        (pa, ts') = parseParameter ts
        (pas, ts'') = parseParameters' ts'
        
parseParameters' ts = (Pas'Eps, ts)
        
parseParameter ts = (Pa ty na, ts')
    where
        (ty, (Token T_IDENT na _):ts') = parseType ts
        
parseType ts = (Ty bt bs, ts'')
    where
        (bt, ts') = parseBasicType ts
        (bs, ts'') = parseBrackets ts'
        
parseBrackets ((Token T_LBRACKET _ _):(Token T_RBRACKET _ _):ts) = (Br bs, ts')
    where
        (bs, ts') = parseBrackets ts
        
parseBrackets ts = (BrEps, ts)

parseBasicType (tok@(Token T_KEYWORD "int" _):ts) = (T_Int tok, ts)
parseBasicType (tok@(Token T_KEYWORD "boolean" _):ts) = (T_Boolean tok, ts)
parseBasicType (tok@(Token T_KEYWORD "void" _):ts) = (T_Void tok, ts)
parseBasicType (tok@(Token T_IDENT _ _):ts) = (T_Ident tok, ts)


        
parseStatement ts@((Token ty na _):_) = let
    exprStmtRes = (let (res, ts') = parseExpressionStatement ts in (SEx res, ts'))
    in case (ty, na) of
        (T_LBRACE, _) -> let (res, ts') = parseBlock ts in (SBl res, ts')
        (T_KEYWORD, "if") -> let (res, ts') = parseIfStatement ts in (SIf res, ts')
        (T_SEMICOLON, _) -> let (res, ts') = parseEmptyStatement ts in (SEm res, ts')
        (T_KEYWORD, "while") -> let (res, ts') = parseWhileStatement ts in (SWh res, ts')
        (T_KEYWORD, "return") -> let (res, ts') = parseReturnStatement ts in (SRe res, ts')
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
        (_, _) -> error "parseStatement"        
        
        
parseBlock ((Token T_LBRACE _ _):ts) = (Bl bs, ts')
    where
        (bs, (Token T_RBRACE _ _):ts') = parseBlockStatements ts
        
parseBlockStatements ts@((Token T_RBRACE _ _):_) = (BSsEps, ts)
        
parseBlockStatements ts = (BSs bs bss, ts'')
    where
        (bs, ts') = parseBlockStatement ts
        (bss, ts'') = parseBlockStatements ts'
        
parseBlockStatement ts@((Token ty na _):(Token ty1 na1 _):(Token ty2 na2 _):_) = let
    stmtRes = let (res, ts') = parseStatement ts in (BS1 res, ts')
    lvdsRes = let (res, ts') = parseLocalVariableDeclarationStatement ts in (BS2 res, ts')
    in case (ty, na) of
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
        (_, _) -> error "parseBlockStatement"

parseLocalVariableDeclarationStatement :: [Token] -> (LocalVariableDeclarationStatement, [Token])
parseLocalVariableDeclarationStatement ts = (LVDS ty na lvds', ts'')
    where
        (ty, (Token T_IDENT na _):ts') = parseType ts
        (lvds', (Token T_SEMICOLON _ _):ts'') = parseLVDS' ts'
       
parseLVDS' :: [Token] -> (LVDS', [Token]) 
parseLVDS' ((Token T_EQUALS _ _):ts) = let (ex, ts') = parseExpression ts in (LVDS' ex, ts')
parseLVDS' ts@((Token T_SEMICOLON _ _):_) = (LVDS'Eps, ts)
            
parseEmptyStatement :: [Token] -> (EmptyStatement, [Token]) 
parseEmptyStatement ((Token T_SEMICOLON _ _):ts) = (EmS, ts)

parseWhileStatement :: [Token] -> (WhileStatement, [Token]) 
parseWhileStatement ((Token T_KEYWORD "while" _):(Token T_LPAREN _ _):ts) = (WS ex s, ts'')
    where
        (ex, (Token T_RPAREN _ _):ts') = parseExpression ts
        (s, ts'') = parseStatement ts'
        
parseIfStatement :: [Token] -> (IfStatement, [Token]) 
parseIfStatement ((Token T_KEYWORD "if" _):(Token T_LPAREN _ _):ts) = (If ex s if', ts''')
    where
        (ex, (Token T_RPAREN _ _):ts') = parseExpression ts
        (s, ts'') = parseStatement ts'
        (if', ts''') = parseIfStatement' ts''
            
parseIfStatement' :: [Token] -> (IfStatement', [Token]) 
parseIfStatement' ((Token T_KEYWORD "else" _):ts) = (If' s, ts')
    where
        (s, ts') = parseStatement ts

parseIfStatement' ts = (If'Eps, ts)

parseExpressionStatement :: [Token] -> (ExpressionStatement, [Token]) 
parseExpressionStatement ts = (ES e, ts')
    where
        (e, (Token T_SEMICOLON _ _):ts') = parseExpression ts
        
parseReturnStatement :: [Token] -> (ReturnStatement, [Token])
parseReturnStatement ((Token T_KEYWORD "return" _):(Token T_SEMICOLON _ _):ts) = (RS, ts)
parseReturnStatement ((Token T_KEYWORD "return" _):ts) = (RSE ex, ts')
    where
        (ex, (Token T_SEMICOLON _ _):ts') = parseExpression ts



-- Expressions
--------------
            
parseExpression :: [Token] -> (Expression, [Token])
parseExpression ts = (let (ae, ts') = parseAssignmentExpression ts in (Expr ae, ts'))



parseAssignmentExpression ts = (AE oe ae, ts'')
    where
        (oe, ts') = parseLogicalOrExpression ts
        (ae, ts'') = parseAssignmentExpression' ts'
        
parseAssignmentExpression' ((Token T_EQUALS _ _):ts) = (AE' ae, ts')
    where
        (ae, ts') = parseAssignmentExpression ts
        
parseAssignmentExpression' ts = (AE'Eps, ts)



parseLogicalOrExpression ts = (LOE ae oe', ts'')
    where
        (ae, ts') = parseLogicalAndExpression ts
        (oe', ts'') = parseLogicalOrExpression' ts'
        
parseLogicalOrExpression' ((Token T_PIPE_PIPE _ _):ts) = (LOE' ae oe', ts'')
    where
        (ae, ts') = parseLogicalAndExpression ts
        (oe', ts'') = parseLogicalOrExpression' ts'
        
parseLogicalOrExpression' ts = (LOE'Eps, ts)
       
       
        
parseLogicalAndExpression ts = (LAE ee ae', ts'')
    where
        (ee, ts') = parseEqualityExpression ts
        (ae', ts'') = parseLogicalAndExpression' ts'
        
parseLogicalAndExpression' ((Token T_AMPERSAND_AMPERSAND _ _):ts) = (LAE' ee ae', ts'')
    where
        (ee, ts') = parseEqualityExpression ts
        (ae', ts'') = parseLogicalAndExpression' ts'
        
parseLogicalAndExpression' ts = (LAE'Eps, ts)



parseEqualityExpression ts = (EqE re ee', ts'')
    where
        (re, ts') = parseRelationalExpression ts
        (ee', ts'') = parseEqualityExpression' ts'
        
parseEqualityExpression' ((Token T_EQUALS_EQUALS _ _):ts) = (EqE'Eq re ee', ts'')
    where
        (re, ts') = parseRelationalExpression ts
        (ee', ts'') = parseEqualityExpression' ts'
        
parseEqualityExpression' ((Token T_EXCL_EQUALS _ _):ts) = (EqE'Neq re ee', ts'')
    where
        (re, ts') = parseRelationalExpression ts
        (ee', ts'') = parseEqualityExpression' ts'
        
parseEqualityExpression' ts = (EqE'Eps, ts)



parseRelationalExpression ts = (RE ae re', ts'')
    where
        (ae, ts') = parseAdditiveExpression ts
        (re', ts'') = parseRelationalExpression' ts'
        
parseRelationalExpression' ((Token T_LANGLE _ _):ts) = (RE'Lt ae re', ts'')
    where
        (ae, ts') = parseAdditiveExpression ts
        (re', ts'') = parseRelationalExpression' ts'
        
parseRelationalExpression' ((Token T_LANGLE_EQUALS _ _):ts) = (RE'LtEq ae re', ts'')
    where
        (ae, ts') = parseAdditiveExpression ts
        (re', ts'') = parseRelationalExpression' ts'
        
parseRelationalExpression' ((Token T_RANGLE _ _):ts) = (RE'Gt ae re', ts'')
    where
        (ae, ts') = parseAdditiveExpression ts
        (re', ts'') = parseRelationalExpression' ts'
        
parseRelationalExpression' ((Token T_RANGLE_EQUALS _ _):ts) = (RE'GtEq ae re', ts'')
    where
        (ae, ts') = parseAdditiveExpression ts
        (re', ts'') = parseRelationalExpression' ts'
        
parseRelationalExpression' ts = (RE'Eps, ts)



parseAdditiveExpression ts = (AdE me ae', ts'')
    where
        (me, ts') = parseMultiplicativeExpression ts
        (ae', ts'') = parseAdditiveExpression' ts'
        
parseAdditiveExpression' ((Token T_PLUS _ _):ts) = (AdE'Plus me ae', ts'')
    where
        (me, ts') = parseMultiplicativeExpression ts
        (ae', ts'') = parseAdditiveExpression' ts'
        
parseAdditiveExpression' ((Token T_MINUS _ _):ts) = (AdE'Minus me ae', ts'')
    where
        (me, ts') = parseMultiplicativeExpression ts
        (ae', ts'') = parseAdditiveExpression' ts'
        
parseAdditiveExpression' ts = (AdE'Eps, ts)



parseMultiplicativeExpression ts = (ME ue me', ts'')
    where
        (ue, ts') = parseUnaryExpression ts
        (me', ts'') = parseMultiplicativeExpression' ts'
        
parseMultiplicativeExpression' ((Token T_STAR _ _):ts) = (ME'Mul ue me', ts'')
    where
        (ue, ts') = parseUnaryExpression ts
        (me', ts'') = parseMultiplicativeExpression' ts'
        
parseMultiplicativeExpression' ((Token T_SLASH _ _):ts) = (ME'Div ue me', ts'')
    where
        (ue, ts') = parseUnaryExpression ts
        (me', ts'') = parseMultiplicativeExpression' ts'
        
parseMultiplicativeExpression' ((Token T_PERCENT _ _):ts) = (ME'Mod ue me', ts'')
    where
        (ue, ts') = parseUnaryExpression ts
        (me', ts'') = parseMultiplicativeExpression' ts'
        
parseMultiplicativeExpression' ts = (ME'Eps, ts)



parseUnaryExpression ((Token T_EXCL _ _):ts) = (UENeg ue, ts')
    where
        (ue, ts') = parseUnaryExpression ts
        
parseUnaryExpression ((Token T_MINUS _ _):ts) = (UEMinus ue, ts')
    where
        (ue, ts') = parseUnaryExpression ts
        
parseUnaryExpression ts = (UEPost pe, ts')
    where
        (pe, ts') = parsePostfixExpression ts
        
        
        
parsePostfixExpression ts = (PfE pe ops, ts'')
    where
        (pe, ts') = parsePrimaryExpression ts
        (ops, ts'') = parsePostfixOps ts'
        
parsePostfixOps ts@((Token T_DOT _ _):tss) = (PfOs pfo ops, ts'')
    where
        (pfo, ts') = parsePostfixOp ts
        (ops, ts'') = parsePostfixOps ts'
        
parsePostfixOps ts@((Token T_LBRACKET _ _):tss) = (PfOs pfo ops, ts'')
    where
        (pfo, ts') = parsePostfixOp ts
        (ops, ts'') = parsePostfixOps ts'
        
parsePostfixOps ts = (PfOsEps, ts)

parsePostfixOp ts@((Token T_DOT _ _):(Token T_IDENT _ _):(Token T_LPAREN _ _):tss) = (PfOMI mi, ts')
    where
        (mi, ts') = parseMethodInvocation ts
        
parsePostfixOp ts@((Token T_DOT _ _):(Token T_IDENT _ _):tss) = (PfOFA fa, ts')
    where
        (fa, ts') = parseFieldAccess ts
        
parsePostfixOp ts@((Token T_LBRACKET _ _):tss) = (PfOAA aa, ts')
    where
        (aa, ts') = parseArrayAccess ts
        


parseMethodInvocation ((Token T_DOT _ _):(Token T_IDENT na _):(Token T_LPAREN _ _):ts) = (MI na args, ts')
    where
        (args, (Token T_RPAREN _ _):ts') = parseArguments ts
        
parseFieldAccess ((Token T_DOT _ _):(Token T_IDENT na _):ts) = (FA na, ts)

parseArrayAccess ((Token T_LBRACKET _ _):ts) = (AA ex, ts')
    where
        (ex, (Token T_RBRACKET _ _):ts') = parseExpression ts
        
        
        
parseArguments ts@((Token ty na _):tss) = let
    res = let (ex, ts') = parseExpression ts
              (args', ts'') = parseArguments' ts'
              in (Arguments ex args', ts'')
              in case (ty, na) of
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
                  (_, _) -> (ArgumentsEps, ts)
                  
parseArguments' ((Token T_COMMA _ _):ts) = (Arguments' ex args', ts'')
    where
        (ex, ts') = parseExpression ts
        (args', ts'') = parseArguments' ts'
        
parseArguments' ts = (Arguments'Eps, ts)



parsePrimaryExpression (tok@(Token T_KEYWORD "null" _):ts) = (PENull tok, ts)
parsePrimaryExpression (tok@(Token T_KEYWORD "true" _):ts) = (PETrue tok, ts)
parsePrimaryExpression (tok@(Token T_KEYWORD "false" _):ts) = (PEFalse tok, ts)
parsePrimaryExpression (tok@(Token T_KEYWORD "this" _):ts) = (PEThis tok, ts)
parsePrimaryExpression (tok@(Token T_INTEGER_LITERAL _ _):ts) = (PEInteger tok, ts)

parsePrimaryExpression (tok@(Token T_IDENT _ _):(Token T_LPAREN _ _):ts) = (PEIdentArg tok args, ts')
    where
        (args, (Token T_RPAREN _ _):ts') = parseArguments ts
        
parsePrimaryExpression (tok@(Token T_IDENT _ _):ts) = (PEIdent tok, ts)

parsePrimaryExpression ((Token T_LPAREN _ _):ts) = (PEExpr ex, ts')
    where
        (ex, (Token T_RPAREN _ _):ts') = parseExpression ts
        
parsePrimaryExpression ts@((Token T_KEYWORD "new" _):(Token T_IDENT _ _):(Token T_LPAREN _ _):_) = (PENewObj obj, ts')
    where
        (obj, ts') = parseNewObjectExpression ts
        
parsePrimaryExpression ts = (PENewArr arr, ts')
    where
        (arr, ts') = parseNewArrayExpression ts
        
        
        
parseNewObjectExpression ((Token T_KEYWORD "new" _)
                         :(Token T_IDENT na _)
                         :(Token T_LPAREN _ _)
                         :(Token T_RPAREN _ _)
                         :ts) = (NewObjectExpression na, ts)
                         
parseNewArrayExpression ((Token T_KEYWORD "new" _):ts) = (NewArrayExpression ty ex br, ts''')
    where
        (ty, (Token T_LBRACKET _ _):ts') = parseBasicType ts
        (ex, (Token T_RBRACKET _ _):ts'') = parseExpression ts'
        (br, ts''') = parseBrackets ts''