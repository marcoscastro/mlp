#ifndef _SINAL_ENTRADA_H_
#define _SINAL_ENTRADA_H_

#include "entrada.h"

class SinalEntrada: public Entrada
{
private:
	double valor;
public:
	SinalEntrada(double valor);
	double gerarSaida();
	void setValor(double valor);
	void invalidaCalculo() {}
};

SinalEntrada::SinalEntrada(double valor)
{
	this->valor = valor;
}

double SinalEntrada::gerarSaida()
{
	return valor;
}

void SinalEntrada::setValor(double valor)
{
	this->valor = valor;
}

#endif