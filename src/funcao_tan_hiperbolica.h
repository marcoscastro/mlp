#ifndef _FUNCAO_TAN_HIPERBOLICA_H_
#define _FUNCAO_TAN_HIPERBOLICA_H_
#include <math.h>
#include "funcao_ativacao.h"

class FuncaoTanHiperbolica: public FuncaoAtivacao
{
private:
	double beta;
public:
	FuncaoTanHiperbolica(double beta);
	double rodar(double x);
	double derivada(double u);
};

FuncaoTanHiperbolica::FuncaoTanHiperbolica(double beta)
{
	this->beta = beta;
}

double FuncaoTanHiperbolica::rodar(double u)
{
	return (1 - exp(-beta * u)) / (1 + exp(-beta * u));
}

double FuncaoTanHiperbolica::derivada(double u)
{
	return 1 / pow(cosh(u), beta);
}

#endif
