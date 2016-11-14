{-# LANGUAGE ScopedTypeVariables #-}

module Main where

import Lexer
import Parser
import ParserTests

import System.IO
import System.Environment
import Data.List
import System.Exit

doSomething "--lextest" f = do
    contents <- readFile f
    let tokenList = tokenize contents in
        putStr (intercalate "\n" $ (map show tokenList) ++ ["EOF\n"])
        
doSomething "--parsetest" f = do
    contents <- readFile f
    let p = parse $ tokenize contents
    case p of
        Just prog -> exitWith ExitSuccess
        Nothing -> do
            hPutStrLn stderr "error"
            exitWith (ExitFailure 1)
        
    
readContents o f = do
    contents <- getContents
    return contents

-- Main entry
main = do
    [o, f] <- getArgs
    doSomething o f