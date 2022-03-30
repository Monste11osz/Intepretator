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
        LBRACKET, RBRACKET,
	ASSIGN,
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
        -1, -1,
        0,
	1, 1,
	2,
	3,
	4,
	5,
	6, 6,
	7, 7,
	8, 8,
	9, 9,
	10, 10, 10
};


string OPERTEXT[] = {
	"(", ")",
	"=",
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
};


class Oper : public Lexem
{
	OPERATOR opertype;
public:
	Oper();
	Oper(string ch);
	OPERATOR getType();
	int getPriority();
	int getValue(const Number &left, const Number &right);
};


map<string, int> Var;


class Variable : public Lexem
{
	string name;
public:
	Variable();
	Variable(string name);
	int getValue();
	void setValue(int value);
};

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
	else if(ch == "=")
	{
		opertype = ASSIGN;
	}
	else if(ch == "or")
	{
		opertype = OR;
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


LEXEM_TYPE Lexem::getype()
{
	return type;
}


int assign(Lexem *left, Lexem *right)
{
	int num;
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


vector<Lexem *> parseLexem(std::string codeline)
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


std::vector<Lexem *> buildPostfix(std::vector<Lexem *> infix)
{
	std::stack<Oper *> operators;
	std::vector<Lexem *> posix;
	for(int i = 0; i < infix.size(); i++)
	{
		if(infix[i]->getype() == NUMBER || infix[i]->getype() == VARIA)
		{
			//cout << i << "  " << infix[i] << "Num or Var" << endl;
			posix.push_back(infix[i]);
		}
		else if(infix[i]->getype() == OPER)
		{
			if(operators.empty())
			{
				operators.push(static_cast<Oper *>(infix[i]));

			}
			else if(static_cast<Oper *>(infix[i])->getType() == LBRACKET)
			{
				operators.push(static_cast<Oper *>(infix[i]));
			}
			else if(static_cast<Oper *>(infix[i])->getType() == RBRACKET)
			{
				while(static_cast<Oper *>(operators.top())->getType() != LBRACKET)
				{
					posix.push_back(operators.top());
					operators.pop();
				}
				operators.pop();
			}
			else if(!operators.empty() && operators.top()->getPriority() >= static_cast<Oper *>(infix[i])->getPriority())
			{
		 		posix.push_back(operators.top());
				operators.pop();
				operators.push(static_cast<Oper *>(infix[i]));
			}
			else
			{
				operators.push(static_cast<Oper *>(infix[i]));
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


int evaluatePostfix(std::vector<Lexem *> poliz)
{
	std::stack<Number *> stack;
	int valu = 0;
	Lexem *l, *r;
	for(int i = 0; i < poliz.size(); i++)
	{
		if(poliz[i]->getype() == NUMBER || poliz[i]->getype() == VARIA)
		{
			stack.push(static_cast<Number *>(poliz[i]));
		}
		else if(poliz[i]->getype() == OPER)
		{
			//static_cast<Number *>(r) = stack.top()->getValue();
			r = stack.top();
			stack.pop();
			//static_cast<Number *>(l) = stack.top()->getValue();
			l = stack.top();
			stack.pop();
			if(static_cast<Oper *>(poliz[i])->getType() == ASSIGN)
			{
				stack.push(new Number(assign(l, r)));
			}
			else
			{
				stack.push(new Number(static_cast<Oper *>(poliz[i])->getValue(static_cast<Number *>(l)->getValue(), static_cast<Number *>(r)->getValue())));
			}
		}
	}
	valu = stack.top()->getValue();
	stack.pop();
	return valu;
}

int main()
{
	std::string codeline;
	std::vector<Lexem *> infix;
	std::vector<Lexem *> postfix;
	int value = 0;
	while(std::getline(std::cin, codeline))
	{
		infix = parseLexem(codeline);
		postfix = buildPostfix(infix);
		value = evaluatePostfix(postfix);
		std::cout << value << std::endl;
	}
	return 0;
}
