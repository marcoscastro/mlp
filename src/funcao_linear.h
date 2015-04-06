#ifndef _FUNCAO_LINEAR_H_
#define _FUNCAO_LINEAR_H_
#include "funcao_ativacao.h"

class FuncaoLinear: public FuncaoAtivacao
{
public:
	double rodar(double x);
	double derivada(double u);
};

double FuncaoLinear::rodar(double u)
{
	return u;
}

double FuncaoLinear::derivada(double u)
{
	return 1;
}

#endif