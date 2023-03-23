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



bool isNonTerminal(string s)
{
    for (auto t : non_terminal)
        if (t == s)
            return true;
    return false;
}

// read grammar
void ReadGrammar()
{
    Parser myParser;
    myParser.ParseProgram(LHS, RHS);

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

}

//Jennifer's version for task 1
// Task 1
void printTerminalsAndNoneTerminals()
{
    
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


vector<bool> simplify;
unordered_map<string, bool> reachable;

void PrintUsefulGrammar()
{
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

// Task 2
bool RemoveUselessSymbols()
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

    simplify.resize(LHS.size());

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

    for (int i = 0; i < LHS.size(); ++i)
        if (simplify[i] == false || reachable[LHS[i]] == false)
            return true;
    return false;
}

// A = A U (B - {#})
bool unionExcludeEpsilon(unordered_set<string>& a, unordered_set<string>& b)
{
    int n = a.size();
    unordered_set<string> c;
    c.insert(b.begin(), b.end());

    if (c.find("#") != c.end())
        c.erase(c.find("#"));

    a.insert(c.begin(), c.end());

    return ( n != a.size() );
}

// A = A U {#}
bool unionEpsilon(unordered_set<string>& a)
{
    int n = a.size();
    a.insert("#");
    return ( n != a.size() );
}

bool isEpsilonIn(unordered_set<string> a)
{
    return ( a.find("#") != a.end() );
}


unordered_map<string, unordered_set<string>> FIRST;
unordered_map<string, unordered_set<string>> FOLLOW;

// Task 3
void CalculateFirstSets()
{
    //cout << "3\n";
    for (int i = 0; i < LHS.size(); ++i)
        if (RHS[i].empty())
            RHS[i].push_back("#");
    
    for (auto s : terminal)
        FIRST[s].insert(s);
    FIRST["#"].insert("#");

    bool changed = true;
    while(changed)
    {
        changed = false;
        bool tmp;

        // Applying rule III. If A->Ba. Then First(A) = First(A) U (First(B)-{#})
        for (int i = 0; i < LHS.size(); ++i)
        {
            tmp = unionExcludeEpsilon(FIRST[LHS[i]], FIRST[RHS[i][0]]);
            if (!changed)
                changed = tmp;
        }

        // Applying rule IV. If A->CCCBa && {#} is in C. Then First(A) = First(A) U (First(B)-{#})
        for (int i = 0; i < LHS.size(); ++i)
        {
            for (int j = 1; j < RHS[i].size(); ++j)
                if ( isEpsilonIn(FIRST[ RHS[i][j-1] ]) )
                {
                    tmp = unionExcludeEpsilon(FIRST[LHS[i]], FIRST[RHS[i][j]]);
                    if (!changed)
                        changed = tmp;
                }
                else
                    break;
        }

        // Applying rule V. If A->BCDEF && {#} is in BCDEF. Then First(A) = First(A) U {#}
        for (int i = 0; i < LHS.size(); ++i)
        {
            tmp = true;
            for (auto s : RHS[i])
                if (!isEpsilonIn(FIRST[s]))
                {
                    tmp = false;
                    break;     
                }

            if (tmp){
                tmp = unionEpsilon(FIRST[LHS[i]]);
                if (!changed)
                    changed = tmp;
            }
        }
    }
}

void PrintFirstSet()
{
    terminal.insert(terminal.begin(), "#");
    for (auto s : non_terminal)
    {
        cout << "FIRST(" << s << ") = { ";
        int count = 0;
        for (auto t : terminal)
        {
            if (FIRST[s].find(t) != FIRST[s].end()){
                cout << t;
                count++;

                if (count < FIRST[s].size())
                    cout << ", ";
                else 
                    cout << " ";
            }
        }
        cout << "}\n";
    }
}

// Task 4
void CalculateFollowSets()
{
    //cout << "4\n";
    CalculateFirstSets();

    // FOLLOW[S] = $
    FOLLOW[LHS[0]].insert("$");

    // Apply rule IV, V.
    for (int i = 0; i < LHS.size(); ++i)
    {
        // Rule IV. If A-> a B A1 A2... Ak  add First(A1) - # to Follow(B)
        for (int j = 0; j < RHS[i].size()-1; ++j)
        {
            string s = RHS[i][j];
            string t = RHS[i][j+1];
            if (isNonTerminal(s))
                unionExcludeEpsilon(FOLLOW[s], FIRST[t]);
        }

        // Rule V. If A -> a B A1 A2 A3...Ai...Ai+1... Ak
        //         If # is in First(A1), First(A2), ..., First(Ai)
        //  Follow(B) = Follow(B) U [First(Ai+1) - #]
        for (int j = 0; j < RHS[i].size(); ++j)
        {
            string s = RHS[i][j];
            if (isNonTerminal(s))
            {
                int k = j+1;
                while(k < RHS[i].size() && isEpsilonIn(FIRST[RHS[i][k]]) ) k++;
                if (k < RHS[i].size())
                    unionExcludeEpsilon(FOLLOW[s], FIRST[RHS[i][k]]);
            }
        }
    }

    bool changed = true;
    while(changed)
    {
        changed = false;
        bool tmp;

        // Apply rule II
        //      If A -> aB. FOLLOW(B) = FOLLOW(A) U FOLLOW(B)
        for (int i = 0; i < LHS.size(); ++i)
        {
            string s = RHS[i].back();
            if (isNonTerminal(s))
                tmp = unionExcludeEpsilon(FOLLOW[s], FOLLOW[LHS[i]]);
            if (!changed)
                changed = tmp;
        }

        // Apply rule III
        //      If A -> a B A1 A2 A3 ... Ak and # is in First(A1), First(A2), ..., First(Ak)
        //      FOLLOW(B) = FOLLOW(B) U FOLLOW(A)
        for (int i = 0; i < LHS.size(); ++i)
        {
            for (int j = 0; j < RHS[i].size(); ++j)
            {
                if (!isNonTerminal(RHS[i][j]))
                    continue;

                int k = j+1;
                while (k < RHS[i].size() && isEpsilonIn(FIRST[RHS[i][k]]) ) k++;
                
                if (k == RHS[i].size())
                    tmp = unionExcludeEpsilon(FOLLOW[RHS[i][j]], FOLLOW[LHS[i]]);
                if (!changed)
                    changed = tmp;
            }
        }
    }
}

void PrintFollowSet()
{
    terminal.insert(terminal.begin(), "$");
    for (auto s : non_terminal)
    {
        cout << "FOLLOW(" << s << ") = { ";
        int count = 0;
        for (auto t : terminal)
        {
            if (FOLLOW[s].find(t) != FOLLOW[s].end()){
                cout << t;
                count++;

                if (count < FOLLOW[s].size())
                    cout << ", ";
                else 
                    cout << " ";
            }
        }
        cout << "}\n";
    }
}



bool isIntersect(unordered_set<string> a, unordered_set<string> b)
{
    for (auto i : a)
    {
        for (auto j : b)
            if (i == j)
                return true;
    }
    return false;
}


// Task 5
void CheckIfGrammarHasPredictiveParser()
{
    //cout << "5\n";
    if (RemoveUselessSymbols())
    {
        cout << "NO";
        return;
    }

    CalculateFirstSets();
    CalculateFollowSets();

    bool condition1 = true;
    bool condition2 = true;

    for (int i = 0; i < LHS.size(); ++i)
    {
        if (simplify[i] && reachable[LHS[i]])
        {
            if (RHS[i][0] == "#")
                condition2 = !isIntersect(FIRST[LHS[i]], FOLLOW[LHS[i]]);

            if (condition2 == false)
                break;

            if (RHS[i][0] != "#")
                for (auto s : RHS[i])
                {
                    for (int k = i+1; k < LHS.size(); ++k)
                    {
                        if (simplify[k] && reachable[LHS[k]] && LHS[k] == LHS[i])
                        {
                            for (auto t : RHS[k])
                                if (s != t && isIntersect(FIRST[s], FIRST[t]))
                                {
                                    condition1 = false;
                                    break;
                                }
                        }
                        if (condition1 == false)
                            break;
                    }
                    if (condition1 == false)
                        break;
                }

            if (condition1 == false)
                break;
        }
    }

    if (condition1 && condition2)
        cout << "YES";
    else
        cout << "NO";
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
                PrintUsefulGrammar();
            break;

        case 3: CalculateFirstSets();
                PrintFirstSet();
            break;

        case 4: CalculateFollowSets();
                PrintFollowSet();
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

