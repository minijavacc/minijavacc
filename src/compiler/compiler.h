#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace cmpl {

  class Compiler
  {
  public:
    static int echo(std::ifstream &file, std::string filename);
    static int lextest(std::ifstream &file, std::string filename);
    static int parsetest(std::ifstream &file, std::string filename);
    static int printast(std::ifstream &file, std::string filename);
    static int semcheck(std::ifstream &file, std::string filename);
    static int creategraph(std::ifstream &file, std::string filename);
    static int compilefirm(std::ifstream &file, std::string filename);
    static int compile(std::ifstream &file, std::string filename);
    static int unittest(std::ifstream &file, std::string filename);
  
    static std::string sourcePreview(std::ifstream &file, unsigned int line, unsigned int column);
  };

}
