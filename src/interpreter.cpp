#include "interpreter.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
#include <map>


using namespace std;

int PRIORITY[] = {
        -1,
        -1, -1,
        -1, -1,
        -1, -1,
        -1, 0, -1,
        -1, -1,
        -1, -1,
        1,
        2,
        3,
        4,
        5,
        6, 6,
        7, 7,
        7, 7,
        8, 8,
        9, 9,
        10, 10, 10
};


string OPERTEXT[] = {
        "deref",
        "if", "then",
        "else", "endif",
        "while", "endwhile",
        "goto", ":=", ":",
        "(", ")",
        "[", "]",
        "or",
        "and",
        "|",
        "^",
        "&",
        "==", "!=",
        "<<", ">>",
        "<=", "<",
        ">=", ">",
        "+", "-",
        "*", "/", "%"
};

std::map<string, int> Var;
std::map<string, int> Labels;
std::map<string, vector<int> > ArrayTabel;

ArrayElem::ArrayElem(string name1, int index1) : Lexem(ARRAYS)
{
        this->name = name1;
        this->index = index1;
}

string ArrayElem::getName()
{
        return name;
}

int ArrayElem::getValue()
{
        return ArrayTabel[name][index];
}

int ArrayElem::setValue(int value)
{
        return ArrayTabel[name][index] = value;
}

void ArrayElem::print()
{

}

Goto::Goto(OPERATOR opertype) : Oper(opertype)
{
        row = UNDEFINED;
        op = opertype;
}


void Goto::setRow(int row)
{
        Goto::row = row;
}

int Goto::getRow()
{
        return row;
}

void Goto::print()
{
        cout << "[<row " << row << ">" << OPERTEXT[this->getType()] << "] ";
}

Variable::Variable()
{
        name = "";
}

int Variable::getValue()
{
        return Var[name];
}

void Variable::setValue(int value)
{
        Var[name] = value;
}

Variable::Variable(string name) : Lexem(VARIA)
{
        this->name = name;
}

string Variable::getName()
{
        return name;
}

bool Variable::inLableTable()
{
        return (Labels.find(this->name) != Labels.end());
}

void Variable::print()
{
        if(inLableTable())
                cout << " LABEL " << name << "=" << Labels[name];
        else
                cout << name << "(" << Var[name] << ")";
}

Lexem::Lexem()
{

}

Number::Number()
{
        value = 0;
}

Oper::Oper()
{

}

Number::Number(int value) : Lexem(NUMBER)
{
        this->value = value;
}

int Number::getValue() const
{
    return value;
}

void Number::print()
{
        cout << value;
}

void print(std::vector<Lexem *> vect)
{
        for(int i = 0; i < vect.size(); i++)
        {
                cout << " ";
                vect[i]->print();
        }
        cout << endl;
}

void clen(std::vector<Lexem *> vect)
{
        for(int i = 0; i < vect.size(); i++)
        {
                delete vect[i];
        }
}

void clear(std::vector<std::vector<Lexem *>> vect)
{
        for(int i = 0; i < vect.size(); i++)
        {
                for(int j = 0; j < vect[i].size(); j++)
                {
                        delete vect[i][j];
                }
        }
}

Oper::Oper(string ch) : Lexem(OPER)
{
        for(int i = 0; i < sizeof(OPERTEXT) / sizeof(string); i++)
        {
                if(OPERTEXT[i] == ch)
                {
                        opertype = (OPERATOR)i;
                }
        }
}

Oper::Oper(OPERATOR opertype)
{
        this->opertype = opertype;
}

OPERATOR Oper::getType()
{
    return opertype;
}


int Oper::getPriority()
{
    return PRIORITY[opertype];
}

int Oper::getValue(const Number & left, const Number & right)
{
        cerr << "LOG : " << endl;
        switch(opertype)
        {
                case PLUS:
                {
                        return left.getValue() + right.getValue();
                }
                case MINUS:
                {
                        return left.getValue() - right.getValue();
                }
                case MULTIPLY:
                {
                        return left.getValue() * right.getValue();
                }
                case ASSIGN:
                {
                      return right.getValue();
                }
                case OR:
                {
                        return left.getValue() || right.getValue();
                }
                case AND:
                {
                        return left.getValue() && right.getValue();
                }
                case BITOR:
                {
                        return left.getValue() | right.getValue();
                }
                case XOR:
                {
                        return left.getValue() ^ right.getValue();
                }
                case BITAND:
                {
                        return left.getValue() & right.getValue();
                }
                case EQ:
                {
                        return left.getValue() == right.getValue();
                }
                case NEQ:
                {
                        return left.getValue() != right.getValue();
                }
                case LEQ:
                {
                        return left.getValue() <= right.getValue();
                }
                case LT:
                {
                        return left.getValue() < right.getValue();
                }
                case GEQ:
                {
                        return left.getValue() >= right.getValue();
                }
                case GT:
                {
                        return left.getValue() > right.getValue();
                }
                case SHL:
                {
                        return left.getValue() << right.getValue();
                }
                case SHR:
                {
                        return left.getValue() >> right.getValue();
                }
                case DIV:
                {
                        return left.getValue() / right.getValue();
                }
                case MOD:
                {
                        return left.getValue() % right.getValue();
                }
        }
        return 0;
}

void Oper::print()
{
        cout << OPERTEXT[opertype];
}

LEXEM_TYPE Lexem::getype()
{
        return type;
}

int assign(Lexem *left, Lexem *right)
{
        int num = 0;
        if(right->getype() == NUMBER)
        {
                num = static_cast<Number *>(right)->getValue();
        }
        if(right->getype() == VARIA)
        {
                num = static_cast<Variable *>(right)->getValue();
        }
        static_cast<Variable *>(left)->setValue(num);
        cout << "NUM " << num << endl;
        return num;
}


void joinGotoAndLabel(Variable *lexemvar, std::stack<Oper *> &stack)
{
        if(stack.top()->getType() == GOTO)
        {
                Goto *lexemgoto = (Goto *)stack.top();
                lexemgoto->setRow(Labels[lexemvar->getName()]);
        }
}

void initLabels(std::vector<Lexem *> &infix, int row)
{
        for(int i = 1; i < (int)infix.size(); i++)
        {
                if(infix[i - 1]->getype() == VARIA && infix[i]->getype() == OPER)
                {
                        cerr << "Log :  Variable + oper" << endl;
                        if(static_cast<Oper *>(infix[i])->getType() == COLON)
                        {
                                cerr << "Log : COLON" << endl;
                                Labels[static_cast<Variable *>(infix[i - 1])->getName()] = row;
                                delete infix[i - 1];
                                delete infix[i];
                                infix[i - 1] = nullptr;
                                infix[i] = nullptr;
                                i++;
                        }
                }
        }
}


void initJumps(std::vector<std::vector<Lexem *>> infix)
{
        std::stack<Goto *> stackIfElse, stackWhile;
        for(int row = 0; row < (int)infix.size(); row++)
        {
                for(int i = 0; i < (int)infix[row].size(); i++)
                {
                        if(infix[row][i] == nullptr)
                        {
                                continue;
                        }
			if(infix[row][i]->getype() == OPER)
                        {
                                Oper *lexemoper = static_cast<Oper *>(infix[row][i]);
                                if(lexemoper->getType() == IF)
                                {
                                        stackIfElse.push((Goto *)lexemoper);
                                }
                                else if(lexemoper->getType() == ELSE)
                                {
                                        stackIfElse.top()->setRow(row + 1);
                                        stackIfElse.pop();
                                        stackIfElse.push((Goto *)lexemoper);
                                }
                                else if(lexemoper->getType() == ENDIF)
                                {
                                        stackIfElse.top()->setRow(row + 1);
                                        stackIfElse.pop();
                                }
                                else if(lexemoper->getType() == WHILE)
                                {
                                        Goto *lexemgoto = (Goto *)lexemoper;
                                        lexemgoto->setRow(row);
                                        stackWhile.push(lexemgoto);
                                }
                                else if(lexemoper->getType() == ENDWHILE)
                                {
                                        Goto *lexemgoto = (Goto *)lexemoper;
                                        lexemgoto->setRow(stackWhile.top()->getRow());
                                        stackWhile.top()->setRow(row + 1);
                                        stackWhile.pop();
                                }
                        }
                }
        }
}

bool isNum(char ch)
{
        return (ch >= '0' && ch <= '9');
}

bool isVaria(char ch)
{
        return ((ch >= 'a' && ch <= 'z') ||(ch >= 'A' && ch <= 'Z'));
}

Lexem* oper(string &codeline, int i, int &next)
{
        int n = sizeof(OPERTEXT) / sizeof(string);
        for(int op = 0; op < n; op++)
        {
                string subcodeline = codeline.substr(i, OPERTEXT[op].size());
                if(OPERTEXT[op] == subcodeline)
                {
                        next = i + OPERTEXT[op].size();
                        if(op == GOTO || op == IF || op == ELSE || op == WHILE || op == ENDWHILE)
                        {
                                return new Goto((OPERATOR)op);
                        }
                        return new Oper(OPERTEXT[op]);
                }
        }
        return nullptr;
}

Lexem* Num(string &codeline, int i, int &next)
{
        int k;
        for(k = 0; k < codeline.size(); k++)
        {
                if(!isNum(codeline[i + k]))
                {
                        break;
                }
        }
        if(k != 0)
        {
                //number = number * 10 + codeline[k] - '0';
                string subcodeline = codeline.substr(i, k);
                next = i + k;
                return new Number(stoi(subcodeline));
        }
        return nullptr;
}

Lexem* var(string &codeline, int i, int &next)
{
        string name;
        int k;
        for(k = 0; k < codeline.size(); k++)
        {
                if(!isVaria(codeline[i + k]))
                {
                        break;
                }
        }
        if(k != 0)
        {
                name = codeline.substr(i, k);
                next = i + k;
                return new Variable(name);
        }
        return nullptr;
}

std::vector<Lexem *> parseLexem(string &codeline)
{
        std::vector<Lexem *> infix;
        Lexem* set;
        for (int i = 0, next = 0; i < codeline.size();)
        {
                if(isspace(codeline[i]))
                {
                        i++;
                        continue;
                }
                set = oper(codeline, i, next);
                if(set != nullptr)
                {
                        infix.push_back(set);
                        i = next;
                        continue;
                }
                set = Num(codeline, i, next);
                if(set != nullptr)
                {
                        infix.push_back(set);
                        i = next;
                        continue;
                }
                set = var(codeline, i, next);
                if(set != nullptr)
                {
                        infix.push_back(set);
                        i = next;
                        continue;
                }
        }
        return infix;
}

std::vector<Lexem *> buildPostfix(std::vector<Lexem *> infix)
{
        std::stack<Oper *> operators;
        std::vector<Lexem *> posix;
        for(const auto &lexem: infix)
        {
                if(lexem == nullptr)
                {
                        continue;
                }
                if(lexem->getype() == VARIA)
                {
                        posix.push_back(lexem);
                }
                else if(lexem->getype() == NUMBER)
                {
                        posix.push_back(lexem);
                }
                else if(lexem->getype() == OPER)
                {
                        Oper *lex = static_cast<Oper *>(lexem);
                        if(lex->getType() == ENDIF || lex->getType() == THEN)
                        {
                                continue;
                        }
                        if(operators.empty())
                        {
                                operators.push(lex);
                        }
                        else if(lex->getType() == LBRACKET || lex->getType() == LQBRACKET)
                        {
                                operators.push(static_cast<Oper *>(lexem));
                        }
                        else if(lex->getType() == RBRACKET)
                        {
                                while(static_cast<Oper *>(operators.top())->getType() != LBRACKET)
                                {
                                        posix.push_back(operators.top());
                                        operators.pop();
                                }
                                operators.pop();
                        }
                        else if(lex->getType() == RQBRACKET)
                        {
                                while(static_cast<Oper *>(operators.top())->getType() != LQBRACKET)
                                {
                                        posix.push_back(operators.top());
                                        operators.pop();
                                }
                                posix.push_back(new Oper("deref"));
                                operators.pop();
                        }
                        else
                        {
                                while(!operators.empty() && operators.top()->getPriority() >= static_cast<Oper *>(lexem)->getPriority())
                                {
                                        posix.push_back(operators.top());
                                        operators.pop();
                                }
                                operators.push(static_cast<Oper *>(lexem));
                        }
                }
        }
        while(!operators.empty())
        {
                posix.push_back(operators.top());
                operators.pop();
        }
        return posix;
}

int evaluatePostfix(std::vector<Lexem *> poliz, int row)
{
        for(int i = 0; i < poliz.size(); i++)
        {
                poliz[i]->print();
        }
        std::stack<Lexem *> stack;
        std::vector<Lexem *> cleer;
        int valu = 0;
        for(int i = 0; i < poliz.size(); i++)
        {
                if(poliz[i]->getype() == VARIA)
                {
                        stack.push(poliz[i]);
                }
                else if(poliz[i]->getype() == NUMBER)
                {
                        stack.push(poliz[i]);
                }
                else if(poliz[i]->getype() == OPER)
                {
                        Oper *ptr = static_cast<Oper *>(poliz[i]);
                        if(ptr->getType() == GOTO || ptr->getType() == ELSE || ptr->getType() == ENDWHILE)
                        {
                                Goto *lexemgoto = (Goto *)poliz[i];
                                clen(cleer);
                                return lexemgoto->getRow();
                        }
                        else if(ptr->getType() == IF || ptr->getType() == WHILE)
                        {
                                Goto *lexemgoto = (Goto *)poliz[i];
                                Number *rvalue =  static_cast<Number *>(stack.top());
                                stack.pop();
                                if(!rvalue)
                                {
                                        clen(cleer);
                                        return lexemgoto->getRow();
                                }
                                continue;
                        }
                        Lexem *right_ptr = stack.top();
                        int right_value = static_cast<Number *>(right_ptr)->getValue();
                        stack.pop();
                        Lexem *left_ptr = stack.top();
                        int left_value =  static_cast<Number *>(left_ptr)->getValue();
                        stack.pop();
                        cout << endl;
                        if(ptr->getType() == DEREF)
                        {
                                stack.push(new ArrayElem(static_cast<Variable *>(left_ptr)->getName(), right_value));
                        }
                        if(ptr->getType() == ASSIGN)
                        {
                                stack.push(new Number(assign(left_ptr, right_ptr)));
                                valu =  static_cast<Number *>(stack.top())->getValue();
                                cout << "VAl" << valu << endl;
                        }
                        else
                        {
                                stack.push(new Number(ptr->getValue(left_value, right_value)));
                                cleer.push_back(stack.top());
                        }
                        valu = static_cast<Number *>(stack.top())->getValue();
                        cout << valu << endl;
                }
        }
        clen(cleer);
        return row + 1;
}
