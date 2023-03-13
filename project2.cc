/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdlib>
#include "lexer.h"
#include "parser.h"

using namespace std;

vector<string> LHS;
vector< vector<string> > RHS;



// read grammar
void ReadGrammar()
{
    Parser myParser;
    myParser.ParseProgram(LHS, RHS);

    for (int i = 0; i < LHS.size(); ++i)
    {
        cout << LHS[i] << " : ";
        for (auto s : RHS[i])
            cout << s << ' ';
        cout << endl;
    }


    /*
    Token tmp = lexer.GetToken();
    while(tmp.token_type != END_OF_FILE)
    {
        tmp.Print();
        tmp = lexer.GetToken();
    }

    cout << "0\n";
    */
}

// Task 1

void printTerminalsAndNoneTerminals()
{
    for (int i = 0; i < LHS.size(); ++i)
    {
        for (int j = 0; j < RHS[i].size(); ++j)
        {
            if(isTerminal(RHS[i][j]))
             cout << RHS[i][j] << " ";
            
        }
    }
    
    for (int i = 0; i < LHS.size(); ++i)
    cout << LHS[i] << " ";

}

// Task 1 helper function
bool isTerminal(string symbol)
{
    for(int i = 0; i < LHS.size(); ++i)
    {
        if (symbol == LHS[i])
            return false;

    }
    return true;
}

// Task 2
void RemoveUselessSymbols()
{
    cout << "2\n";
}

// Task 3
void CalculateFirstSets()
{
    cout << "3\n";
}

// Task 4
void CalculateFollowSets()
{
    cout << "4\n";
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
    cout << "5\n";
}
    
int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);
    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file

    switch (task) {
        case 1: printTerminalsAndNoneTerminals();
            break;

        case 2: RemoveUselessSymbols();
            break;

        case 3: CalculateFirstSets();
            break;

        case 4: CalculateFollowSets();
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

