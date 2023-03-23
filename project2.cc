/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include <cstdlib>
#include "lexer.h"
#include "parser.h"

using namespace std;

bool isTerminal(string symbol);
bool notExist(vector<string> mixed, string t);
vector<string> LHS;
vector< vector<string> > RHS;
vector<string> terminal;
vector<string> non_terminal;
vector<string> mixed; 

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


// Task 1

void printTerminalsAndNoneTerminals()
{
    
    for (int i = 0; i < LHS.size(); ++i)
    {
        if(notExist(mixed, LHS[i]))
        {
            mixed.push_back(LHS[i]);
            //cout << LHS[i] << " ";
        }
        for (int j = 0; j < RHS[i].size(); ++j)
        {
            if(notExist(mixed,RHS[i][j]))
            {
                mixed.push_back(RHS[i][j]);
                //cout << RHS[i][j] << " ";  
            }
        }
    }

    for(int i = 0; i < mixed.size(); ++i)
    {
        if(isTerminal(mixed[i]))
            terminal.push_back(mixed[i]);
           
        else
            non_terminal.push_back(mixed[i]);
      
    }

    for(int i = 0; i < terminal.size(); ++i)
    {
        cout <<terminal[i] << " ";
    }

    for(int i = 0; i < non_terminal.size(); ++i)
    {
        cout <<non_terminal[i] << " ";
    }

    cout << "\n";
    
}

//task1 helper functions

bool isTerminal(string symbol)
{
    for(int i = 0; i < LHS.size(); ++i)
    {
        if (symbol == LHS[i])
            return false;
    }
    return true;
}

bool notExist(vector<string> mixed, string t)
{
    for( int i = 0; i < mixed.size(); ++i)
    {   
        if(mixed[i] == t)
            return false;
     }
     return true;
}
// Task 2
void RemoveUselessSymbols()
{
    //cout << "2\n";
    // insert symbols to unordered map
    unordered_map <string, bool> symbol;
    //symbol["#"] = true;
    // add nonterminal
    for (auto s : non_terminal)
        symbol[s] = false;
    // add terminal
    for (auto s : terminal)
        symbol[s] = true;

    vector<bool> simplify(LHS.size());

    // generating symbol
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int i = 0; i < LHS.size(); ++i)
        {
            // assume it is generating
            bool generating = true; 
            for (auto s : RHS[i])
                if (symbol[s] == false)     // if one right hand side is not generating
                {
                    generating = false;
                    break;
                }

            simplify[i] = generating;

            if (generating && symbol[LHS[i]] == false)
            {
                symbol[LHS[i]] = true;
                changed = true;
            }
        }
    }

    unordered_map<string, bool> reachable;
    unordered_map<string, bool>::iterator it;
    for (it = symbol.begin(); it != symbol.end(); ++it)
        reachable[it->first] = false;

    // reachable[S] = true;
    reachable[non_terminal[0]] = true;         
    // calculating reachable
    changed = true;
    while(changed)
    {
        changed = false;
        for (int i = 0; i < LHS.size(); ++i)
        {
            if (simplify[i] && reachable[LHS[i]])
                for (auto s : RHS[i])
                    if (reachable[s] == false)
                    {
                        reachable[s] = true;
                        changed = true;
                    }

        }
    }

    // print out final grammar
    for (int i = 0; i < LHS.size(); ++i)
        if (simplify[i] && reachable[LHS[i]])
        {
            cout << LHS[i] << " -> ";
            if (RHS[i].empty())
                cout << "#";
            else
                for (auto s : RHS[i])
                    cout << s << ' ';
            cout << endl;
        }
}


bool unionExcludeEpsilon(unordered_set<string> a, unordered_set<string> b)
{
    int n = a.size();

    a.insert(b.begin(), b.end());
    a.erase(a.find("#"));

    return ( n != a.size() );
}

bool unionEpsilon(unordered_set<string> a)
{
    int n = a.size();
    a.insert("#");
    return ( n != a.size() );
}

bool isEpsilonIn(unordered_set<string> a)
{
    return ( a.find("#") != a.end() );
}

// Task 3
void CalculateFirstSets()
{
    //cout << "3\n";
    //unordered_map<"string", unordered_set<string>> FIRST;
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

