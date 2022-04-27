#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
#include <map>

using namespace std;

enum LEXEM_TYPE {
        NUMBER,
        OPER,
        VARIA,
        ARRAYS
};

enum OPERATOR {
        DEREF,
        IF, THEN,
        ELSE, ENDIF,
        WHILE, ENDWHILE,
        GOTO, ASSIGN, COLON,
        LBRACKET, RBRACKET,
        LQBRACKET, RQBRACKET,
        OR,
        AND,
        BITOR,
        XOR,
        BITAND,
        EQ, NEQ,
        LEQ, LT,
        GEQ, GT,
        SHL, SHR,
        PLUS, MINUS,
        MULTIPLY, DIV, MOD

};

extern std::map<string, int> Var;
extern std::map<string, int> Labels;
extern std::map<string, vector<int> > ArrayTabel;


int evaluatePostfix(std::vector<Lexem *> poliz, int row);

std::vector<Lexem *> buildPostfix(std::vector<Lexem *> infix);

std::vector<Lexem *> parseLexem(string &codeline);

Lexem* var(string &codeline, int i, int &next);

Lexem* Num(string &codeline, int i, int &next);

Lexem* oper(string &codeline, int i, int &next);

bool isNum(char ch);

bool isVaria(char ch);

void initJumps(std::vector<std::vector<Lexem *>> infix);

void initLabels(std::vector<Lexem *> &infix, int row);

void joinGotoAndLabel(Variable *lexemvar, std::stack<Oper *> &stack);

int assign(Lexem *left, Lexem *right);

void clear(std::vector<std::vector<Lexem *>> vect);

void print(std::vector<Lexem *> vect);

class Lexem {
	public:
	Lexem();
	Lexem(LEXEM_TYPE typ) : type(typ);
	LEXEM_TYPE getype();
	virtual void print();
};

class Number : public Lexem {
public:
	Number();
	Number(int num = 0);
	int getValue() const;
	void print();
};

class Oper : public Lexem {
public:
	Oper();
	Oper(string ch);
	OPERATOR getType();
	Oper(OPERATOR opertype);
	int getPriority();
	int getValue(const Number &left, const Number &right);
	void print();
};

class Variable : public Lexem {
	public:
	Variable();
	Variable(string name);
	int getValue();
	void setValue(int value);
	string getName();
	bool inLableTable();
	void print();
};

class Goto : public Oper {
public:
	Goto(OPERATOR opertype);
	void setRow(int row);
	int getRow();
	void print();
};

class ArrayElem : public Lexem
{
public:
        ArrayElem(std::string);
        string getName();
        int getValue();
        int setValue(int);
};
