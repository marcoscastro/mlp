#ifndef _NEURONIO_H_
#define _NEURONIO_H_

#include <vector>
#include "entrada.h"
#include "funcao_ativacao.h"

class Neuronio: public Entrada
{
private:
	std::vector<std::pair<Entrada*, double> > entradas; // vetor (entrada,peso)
	FuncaoAtivacao * funcao_ativacao;
	double sigma;
	double saida;
	bool calculado;
public:
	Neuronio(FuncaoAtivacao * funcao_ativacao);
	void receberEntrada(Entrada * e, double peso);
	double gerarSaida();
	double getSomatorio();
	FuncaoAtivacao* getFuncaoAtivacao();
	void setPeso(int indice_entrada, double peso);
	double getPeso(int indice_entrada);
	unsigned int getQuantidadeEntradas();
	Entrada* getEntrada(int indice_entrada);
	void setSigma(double sigma);
	double getSigma();
	void invalidaCalculo();
};

Neuronio::Neuronio(FuncaoAtivacao * funcao_ativacao)
{
	this->funcao_ativacao = funcao_ativacao;
	this->calculado = false;
}

double Neuronio::gerarSaida()
{
	return funcao_ativacao->rodar(getSomatorio());
}

void Neuronio::receberEntrada(Entrada * e, double peso)
{
	entradas.push_back(std::make_pair(e, peso));
}

double Neuronio::getSomatorio()
{
	unsigned int tam_entradas = entradas.size();
	if(calculado)
		return saida;
	saida = 0;
	for(unsigned int i = 0; i < tam_entradas; i++)
		saida += entradas[i].first->gerarSaida() * entradas[i].second;
	calculado = true;
	return saida;
}

FuncaoAtivacao* Neuronio::getFuncaoAtivacao()
{
	return funcao_ativacao;
}

void Neuronio::invalidaCalculo()
{
	unsigned int tam_entradas = entradas.size();
	if(!calculado)
		return;
	calculado = false;
	for(unsigned int i = 0; i < tam_entradas; i++)
		entradas[i].first->invalidaCalculo();
}

void Neuronio::setPeso(int indice_entrada, double peso)
{
	calculado = false;
	entradas[indice_entrada].second = peso;
}

double Neuronio::getPeso(int indice_entrada)
{
	return entradas[indice_entrada].second;
}

unsigned int Neuronio::getQuantidadeEntradas()
{
	return entradas.size();
}

Entrada* Neuronio::getEntrada(int indice_entrada)
{
	return entradas[indice_entrada].first;
}

void Neuronio::setSigma(double sigma)
{
	this->sigma = sigma;
}

double Neuronio::getSigma()
{
	return sigma;
}

#endif
