#ifndef _CAMADA_H_
#define _CAMADA_H_

#include <vector>
#include "neuronio.h"

class Camada
{
private:
	std::vector<Neuronio*> neuronios;
public:
	~Camada();
	void receberNeuronio(Neuronio * neuronio);
	Neuronio* get(int j);
	unsigned int getQuantidadeNeuronios();
};

Camada::~Camada()
{
	for(int i = (int)neuronios.size() - 1; i >= 0; i--)
		delete neuronios[i];
}

void Camada::receberNeuronio(Neuronio * neuronio)
{
	neuronios.push_back(neuronio);
}

Neuronio* Camada::get(int j)
{
	return neuronios[j];
}

unsigned int Camada::getQuantidadeNeuronios()
{
	return neuronios.size(); 
}

#endif
