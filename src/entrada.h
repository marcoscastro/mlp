#ifndef _ENTRADA_H_
#define _ENTRADA_H_

class Entrada
{
public:
	virtual double gerarSaida() = 0;
	virtual void invalidaCalculo() = 0;
};

#endif