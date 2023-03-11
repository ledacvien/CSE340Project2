// Dac Vien Le

#ifndef __PARSER_H__
#define __PARSER_H__

#include <cstring>
#include <vector>
#include "lexer.h"

class Parser {
  private:
    LexicalAnalyzer lexer;

    std::vector<std::string> LHS, idlist;
    std::vector< std::vector<std::string> > RHS;

    void syntax_error();
    Token expect(TokenType expected_type);
  
    void parse_Grammar();
    void parse_RuleList();
    void parse_Rule();
    void parse_IdList();
    void parse_RHS();

  public:
    void ParseProgram(std::vector<std::string> &LHS, std::vector< std::vector<std::string>> &RHS);
};

#endif