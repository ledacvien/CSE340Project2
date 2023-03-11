// Dac Vien Le

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "parser.h"

using namespace std;

// First(Grammar)  = {ID}
// First(RuleList) = {ID}
// First(Rule)     = {ID}
// First(IdList)   = {ID}
// First(RHS)      = {ID, epsilon}

// Follow(Grammar) = {$}
// Follow(RuleList)= {$, HASH}
// Follow(Rule)    = {$, ID, HASH}
// Follow(IdList)  = {STAR}
// Follow(RHS)     = {STAR}

void Parser::syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

void Parser::parse_Grammar()
{
	parse_RuleList();
	expect(HASH);
}

void Parser::parse_RuleList()
{
	parse_Rule();

	Token t = lexer.peek(1);
	if (t.token_type == ID)
	{
		parse_RuleList();
	}
	else if (t.token_type == HASH || t.token_type == END_OF_FILE)
		return;
	else
		syntax_error();
}

void Parser::parse_Rule()
{
	LHS.push_back(expect(ID).lexeme);
	expect(ARROW);
	parse_RHS();
	RHS.push_back(idlist);
	expect(STAR);
}

void Parser::parse_IdList()
{
	idlist.push_back(expect(ID).lexeme);

	Token t = lexer.peek(1);
	if (t.token_type == ID)
		parse_IdList();
	else if (t.token_type == STAR)
		return;
	else
		syntax_error();
}

void Parser::parse_RHS()
{
	idlist.clear();
	Token t = lexer.peek(1);
    if (t.token_type == ID)                 // First(RHS) and First(IdList)
        parse_IdList();
    else if (t.token_type == STAR)          // Follow(RHS)
        return;
    else
        syntax_error();
}

void Parser::ParseProgram(vector<string> &LHS, vector< vector<string> > &RHS)
{
	parse_Grammar();
	expect(END_OF_FILE);
	LHS = this->LHS;
	RHS = this->RHS;
}