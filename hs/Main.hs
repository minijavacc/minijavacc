{-# LANGUAGE ScopedTypeVariables #-}

module Main where

import Lexer
import Parser
import ParserTests

import System.IO
import System.Environment
import Data.List
import Control.Exception as X
import System.Exit

doSomething "--lextest" f = do
    contents <- readFile f
    let tokenList = tokenize contents in
        putStr (intercalate "\n" $ (map show tokenList) ++ ["EOF\n"])
        
doSomething "--parsetest" f = do
    contents <- readFile f
    let p = parse $ tokenize contents
    X.catch (doParse contents) printErr
    -- putStr (show p)
    return ()

doParse contents = do
    let p = parse $ tokenize contents
    hPutStr stdout ( case (show p) of
        "error" -> "an error occurred\n"
        s -> ""
        )


printErr :: SomeException -> IO ()
printErr e = do
    hPutStrLn stderr $ "an error occurred: " ++ show e
    exitWith (ExitFailure 1)
                
           
-- doTest :: IO ()
-- doTest = do
--     let res = testProgram
--     putStrLn $ show res
--     return ()
--
-- doTestWithCatch :: IO ()
-- doTestWithCatch = X.catch doTest printErr
    

        
        
    
readContents o f = do
    contents <- getContents
    return contents

-- Main entry
main = do
    [o, f] <- getArgs
    X.catch (doSomething o f) printErr