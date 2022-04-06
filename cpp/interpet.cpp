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
	VARIA
};

enum OPERATOR {
	GOTO, ASSIGN, COLON,
        LBRACKET, RBRACKET,
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

int PRIORITY[] = {
        -1, 0, -1,
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
	"goto", ":=", ":",
	"(", ")",
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

class Lexem
{
	LEXEM_TYPE type;
public:
	Lexem();
	Lexem(LEXEM_TYPE typ) : type(typ)
	{

	}
	LEXEM_TYPE getype();
};

class Number : public Lexem
{
	int value;
public:

	Number();
	Number(int num = 0);
	int getValue() const;
	void print();
};


class Oper : public Lexem
{
	OPERATOR opertype;
public:
	Oper();
	Oper(string ch);
	OPERATOR getType();
	Oper(OPERATOR opertype);
	int getPriority();
	int getValue(const Number &left, const Number &right);
	void print();
};


map<string, int> Var;
map<string, int> Labels;

class Variable : public Lexem
{
	string name;
public:
	Variable();
	Variable(string name);
	int getValue();
	void setValue(int value);
	string getName();
	bool inLableTable();
	void print();
};

class Goto : public Oper
{
	int row;
public:
	enum {UNDEFINED = -INT32_MAX};
	Goto(OPERATOR opertype);
	void setRow(int row);
	int getRow();
	void print();
};


Goto::Goto(OPERATOR opertype) : Oper(opertype)
{
	row = UNDEFINED;
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
	cout << name << Var[name] << "|";
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

Oper::Oper(string ch) : Lexem(OPER)
{
	if(ch == "+")
	{
		opertype = PLUS;
	}
	else if(ch == "-")
	{
		opertype = MINUS;
	}
	else if(ch == "*")
	{
		opertype = MULTIPLY;
	}
	else if(ch == "(")
	{
		opertype = LBRACKET;
	}
	else if(ch == ")")
	{
		opertype = RBRACKET;
	}
	else if(ch == ":=")
	{
		opertype = ASSIGN;
	}
	else if(ch == "or")
	{
		opertype = OR;
	}
	else if(ch == "goto")
	{
		opertype = GOTO;
	}
	else if(ch == ":")
	{
		opertype = COLON;
	}
	else if(ch == "and")
	{
		opertype = AND;
	}
	else if(ch == "|")
	{
		opertype = BITOR;
	}
	else if(ch == "^")
	{
		opertype = XOR;
	}
	else if(ch == "&")
	{
		opertype = BITAND;
	}
	else if(ch == "==")
	{
		opertype = EQ;
	}
	else if(ch == "!=")
	{
		opertype = NEQ;
	}
	else if(ch == "<=")
	{
		opertype = LEQ;
	}
	else if(ch == "<")
	{
		opertype = LT;
	}
	else if(ch == ">=")
	{
		opertype = GEQ;
	}
	else if(ch == ">")
	{
		opertype = GT;
	}
	else if(ch == "<<")
	{
		opertype = SHL;
	}
	else if(ch == ">>")
	{
		opertype = SHR;
	}
	else if(ch == "/")
	{
		opertype = DIV;
	}
	else if(ch == "%")
	{
		opertype = MOD;
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
			cout << "  23" << endl;
			if(static_cast<Oper *>(infix[i])->getType() == COLON)
			{
				cout << "ttt" << endl;
				Labels[((Variable *)infix[i - 1])->getName()] = row;
				delete infix[i - 1];
				delete infix[i];
				infix[i - 1] = nullptr;
				infix[i] = nullptr;
				i++;
			}
		}
		cout << "  24" << endl;
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

/*vector<Lexem *> parseLexem(std::string codeline)
{
	int number = 0;
	std::vector<Lexem *> infix;
	string name;
	for(int i = 0; i < codeline.size(); i++)
	{
		if(codeline[i] == '\t' || codeline[i] == ' ' || codeline[i] == '\n')
		{
			continue;
		}
		else if(codeline[i] >= '0' && codeline[i] <= '9')
		{
			number = number * 10 + codeline[i] - '0';
			if(codeline[i + 1] < '0' || codeline[i + 1] > '9')
			{
				infix.push_back(new Number(number));
				number = 0;
			}
		}
		else if(codeline[i] >= 'A' && codeline[i] <= 'Z' || codeline[i] >= 'a' && codeline[i] <= 'z')
		{
			name += codeline[i];
			if(!((codeline[i + 1] >= 'A' && codeline[i + 1] <= 'Z') || (codeline[i + 1] >= 'a' && codeline[i + 1] <= 'z')) || (codeline[i + 1] < '0' || codeline[i + 1] > '9'))
			{
				infix.push_back(new Variable(name));
				name = "";
			}
		}
		else
		{
			int n = sizeof(OPERTEXT) / sizeof(string);
			for(int op = 0; op < n; op++)
			{
				string subcodeline = codeline.substr(i, OPERTEXT[op].size());
				if(OPERTEXT[op] == subcodeline)
				{
					infix.push_back(new Oper(OPERTEXT[op]));
					break;
				}
			}
		}
	}
	return infix;
}
*/

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

			cout << "56" << endl;
			Variable *lexemvar = (Variable *)lexem;
			if(lexemvar->inLableTable())
			{
				cout << "join" << endl;
				joinGotoAndLabel(lexemvar, operators);
			}
			else
			{
				cout << "57" << endl;
				posix.push_back(lexem);
			}
			//posix.push_back(lexem);
		}
		else if(lexem->getype() == NUMBER)
		{
			posix.push_back(lexem);
			cout << "aaa" << endl;
		}
		else if(lexem->getype() == OPER)
		{
			if(operators.empty())
			{
				operators.push(static_cast<Oper *>(lexem));
			}
			else if(static_cast<Oper *>(lexem)->getType() == LBRACKET)
			{
				operators.push(static_cast<Oper *>(lexem));
			}
			else if(static_cast<Oper *>(lexem)->getType() == RBRACKET)
			{
				while(static_cast<Oper *>(operators.top())->getType() != LBRACKET)
				{
					posix.push_back(operators.top());
					operators.pop();
				}
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
	std::stack<Number *> stack;
	int valu = 0;
	Lexem *l, *r;
	for(int i = 0; i < poliz.size(); i++)
	{
		if(poliz[i]->getype() == VARIA)
		{
			stack.push(static_cast<Number *>(poliz[i]));
		}
		else if(poliz[i]->getype() == NUMBER)
		{
			stack.push(static_cast<Number *>(poliz[i]));
		}
		else if(poliz[i]->getype() == OPER)
		{
			if(static_cast<Oper *>(poliz[i])->getType() == GOTO)
			{
				Goto *lexemgoto = (Goto *)poliz[i];
				return lexemgoto->getRow();
			}
			r = stack.top();
			stack.pop();
			l = stack.top();
			stack.pop();
			if(static_cast<Oper *>(poliz[i])->getType() == ASSIGN)
			{
				cout << "assign" << endl;
				stack.push(new Number(assign(l, r)));
			}
			else
			{
				stack.push(new Number(static_cast<Oper *>(poliz[i])->getValue(static_cast<Number *>(l)->getValue(), static_cast<Number *>(r)->getValue())));
			}
			valu = stack.top()->getValue();
			cout << valu << endl;
		}
	}
	return row + 1;
}

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
	cout << "pp" << endl;
	for(const auto &infixs : infix)
	{
		postfix.push_back(buildPostfix(infixs));
	}
	cout << "PPPP" << endl;
	int row = 0;
	while(0 <= row && row < postfix.size())
	{
		row = evaluatePostfix(postfix[row], row);
	}
	return 0;
}
