{-# LANGUAGE ScopedTypeVariables #-}

module Main where

import Lexer
import Parser
import ParserTests

import System.IO
import System.Environment
import Data.List
import Control.Exception as X

doSomething "--lextest" f = do
    contents <- readFile f
    let tokenList = tokenize contents in
        putStr (intercalate "\n" $ (map show tokenList) ++ ["EOF\n"])
        
doSomething "--parsetest" f = do
    contents <- readFile f
    let p = parse $ tokenize contents
    return ()

check f = doSomething "--parsetest" f


printErr :: SomeException -> IO ()
printErr e =  do
        case fromException e of
                Just (x :: PatternMatchFail) -> putStrLn "I caught the exception" >> print x
                nothing -> return ()
                
           
doTest :: IO ()     
doTest = do
    let res = testProgram
    return ()
    

        
        
    
readContents o f = do
    contents <- getContents
    return contents

-- Main entry
main = do
    [o, f] <- getArgs
    X.catch (doSomething o f) printErr