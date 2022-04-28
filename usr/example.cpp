#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "interpreter.h"

using namespace std;

int main()
{
        std::string codeline;
        std::vector<std::vector<Lexem *>> infix;
        std::vector<std::vector<Lexem *>> postfix;
        int value = 0;
        while(std::getline(std::cin, codeline))
        {
                infix.push_back(parseLexem(codeline));
        }
        for(int row = 0; row < (int)infix.size(); ++row)
        {
                initLabels(infix[row], row);
        }
        cerr << "After initJu" << endl;
        initJumps(infix);
        for(const auto &infixs : std::infix)
        {
                postfix.push_back(buildPostfix(infixs));
                print(*postfix.rbegin());
        }
        int row = 0;
        while(0 <= row && row < postfix.size())
        {
                row = evaluatePostfix(postfix[row], row);
        }
	clear(infix);
        return 0;
}

