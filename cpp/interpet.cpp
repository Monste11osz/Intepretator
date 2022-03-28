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
        LBRACKET,
        RBRACKET,
	ASSIGN,
        PLUS,
        MINUS,
        MULTIPLY

};

int PRIORITY[] = {
        -1, -1,
        0,
	1, 1,
	2
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
	Oper(char ch);
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

Oper::Oper(char ch) : Lexem(OPER)
{
	switch(ch)
	{
		case '+':
		{
			opertype = PLUS;
			break;
		}
		case '-':
		{
			opertype = MINUS;
			break;
		}
		case '*':
		{
			opertype = MULTIPLY;
			break;
		}
		case '(':
		{
			opertype = LBRACKET;
			break;
		}
		case ')':
		{
			opertype = RBRACKET;
			break;
		}
		case '=':
		{
			opertype = ASSIGN;
			break;
		}
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
			infix.push_back(new Oper(codeline[i]));
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
