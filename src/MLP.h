#ifndef _MLP_H_
#define _MLP_H_

#include <vector>
#include <math.h>
#include "camada.h"
#include "sinal_entrada.h"

class MLP
{
private:
	std::vector<Camada*> camadas;
	std::vector<SinalEntrada*> entradas;
	double taxa_aprendizado, erro_min;
	int neuronios_escondida, max_epocas;
	int qte_amostras, qte_teste;
	int num_entradas, neuronios_saida;
	FuncaoAtivacao * funcao_ativacao;
public:

	MLP(int qte_amostras, int qte_teste, int num_entradas,
		int neuronios_escondida, int neuronios_saida,
		double taxa_aprendizagem, int max_epocas, double erro_min, FuncaoAtivacao * funcao_ativacao);
	~MLP();
	void adicionarCamada(Camada* camada);
	void treinar(std::vector<std::vector<double> >& amostras, std::vector<std::vector<double> >& saidas);
	double erroQuadraticoMedio(std::vector<std::vector<double> >& saidas);
};

MLP::MLP(int qte_amostras, int qte_teste, int num_entradas,
		 int neuronios_escondida, int neuronios_saida,
		 double taxa_aprendizagem, int max_epocas, double erro_min, FuncaoAtivacao * funcao_ativacao)
{
	this->taxa_aprendizado = taxa_aprendizagem;
	this->neuronios_escondida = neuronios_escondida;
	this->max_epocas = max_epocas;
	this->erro_min = erro_min;
	this->qte_amostras = qte_amostras;
	this->qte_teste = qte_teste;
	this->num_entradas = num_entradas;
	this->neuronios_saida = neuronios_saida;
	this->funcao_ativacao = funcao_ativacao;

	for(int i = 0; i <= num_entradas; i++)
	{
		SinalEntrada * sinal_entrada = new SinalEntrada(-1);
		entradas.push_back(sinal_entrada);
	}

	Camada * camada_entrada = new Camada();

	for(int i = 0; i < num_entradas; i++)
	{
		Neuronio * neuronio = new Neuronio(funcao_ativacao);
		for(int j = 0; j <= num_entradas; j++)
			neuronio->receberEntrada(entradas[j], (rand() % 1001) / 1000.0);
		camada_entrada->receberNeuronio(neuronio);
	}

	Camada * camada_escondida = new Camada();

	for(int i = 0; i < neuronios_escondida; i++)
	{
		Neuronio * neuronio = new Neuronio(funcao_ativacao);
		neuronio->receberEntrada(entradas[0], (rand() % 1001) / 1000.0);
		for(int j = 0; j < num_entradas; j++)
			neuronio->receberEntrada(camada_entrada->get(j), (rand() % 1001) / 1000.0);
		camada_escondida->receberNeuronio(neuronio);
	}

	Camada * camada_saida = new Camada();

	for(int i = 0; i < neuronios_saida; i++)
	{
		Neuronio * neuronio = new Neuronio(funcao_ativacao);
		neuronio->receberEntrada(entradas[0], (rand() % 1001) / 1000.0);
		for(int j = 0; j < neuronios_escondida; j++)
			neuronio->receberEntrada(camada_escondida->get(j), (rand() % 1001) / 1000.0);
		camada_saida->receberNeuronio(neuronio);
	}

	adicionarCamada(camada_entrada);
	adicionarCamada(camada_escondida);
	adicionarCamada(camada_saida);
}

MLP::~MLP()
{
	for(int i = (int)entradas.size() - 1; i >= 0; i--)
		delete entradas[i];

	for(int i = (int)camadas.size() - 1; i >= 0; i--)
		delete camadas[i];
}

void MLP::adicionarCamada(Camada* camada)
{
	camadas.push_back(camada);
}

double MLP::erroQuadraticoMedio(std::vector<std::vector<double> >& saidas)
{
	double erro = 0;
	Camada* camada_saida = camadas[2];

	for(int i = 0; i < qte_amostras; i++)
	{
		std::vector<double>& saida = saidas[i];
		double eq = 0; // erro quadrático

		for(int j = 0; j < neuronios_saida; j++)
			eq += pow(saida[j] - camada_saida->get(j)->gerarSaida(), 2);
		erro += eq / 2;
	}
	return erro / qte_amostras; // erro quadrático médio
}

void MLP::treinar(std::vector<std::vector<double> >& amostras, std::vector<std::vector<double> >& saidas)
{
	double erro;
	int num_epocas = 0;

	Camada* camada_saida = camadas[2];

	
	
	while(num_epocas <= max_epocas)
	{
		// cálculo do erro quadrático médio
		erro = erroQuadraticoMedio(saidas);

		for(int i = 0; i < qte_amostras; i++)
		{
			std::vector<double>& amostra = amostras[i];
			std::vector<double>& saida = saidas[i];

			// atribuindo os valores das entradas
			for(int j = 1; j <= num_entradas; j++)
				entradas[j]->setValor(amostra[j - 1]);


			for(int j = 0; j < neuronios_saida; j++)
			{
				Neuronio * neuronio = camada_saida->get(j);

				double sigma = (saida[j] - neuronio->gerarSaida()) * neuronio->getFuncaoAtivacao()->derivada(neuronio->getSomatorio());

				neuronio->setSigma(sigma);

				for(unsigned int k = 0; k < neuronio->getQuantidadeEntradas(); k++)
				{
					neuronio->setPeso(k, neuronio->getPeso(k) +
									  taxa_aprendizado * sigma * neuronio->getEntrada(k)->gerarSaida());
				}
			}

			for(int t = 1; t >= 0; t--)
			{
				Camada* camada = camadas[t];
				Camada* camada_frente = camadas[t + 1];

				for(unsigned int j = 0; j < camada->getQuantidadeNeuronios(); j++)
				{
					Neuronio * neuronio = camada->get(j);
					double somatorio = 0;

					// pega os que estão saindo exceto o -1
					for(unsigned int k = 0; k < camada_frente->getQuantidadeNeuronios(); k++)
					{
						Neuronio * neuronio_frente = camada_frente->get(k);
						somatorio += neuronio_frente->getSigma() * neuronio_frente->getPeso(j + 1);
					}

					double sigma = somatorio * neuronio->getFuncaoAtivacao()->derivada(neuronio->getSomatorio());
					
					neuronio->setSigma(sigma);

					for(unsigned int k = 0; k < neuronio->getQuantidadeEntradas(); k++)
					{
						neuronio->setPeso(k, neuronio->getPeso(k) +
										  taxa_aprendizado * sigma * neuronio->getEntrada(k)->gerarSaida());
					}
				}
			}
		}

		double erro_ant = erro;

		// cálculo do erro quadrático médio
		erro = erroQuadraticoMedio(saidas);
		num_epocas++;

		if(fabs(erro - erro_ant) <= erro_min)
		{
			printf("epoca: %d, erro: %lf\n", num_epocas, fabs(erro - erro_ant));
			break;
		}
	}

	// teste
	for(int i = qte_amostras; i < qte_amostras + qte_teste; i++)
	{
		std::vector<double>& amostra = amostras[i];
		std::vector<double>& saida = saidas[i];

		// atribuindo os valores das entradas
		for(int j = 1; j <= num_entradas; j++)
		{
			entradas[j]->setValor(amostra[j - 1]);
		}

		for(int j = 0; j < neuronios_saida; j++)
		{
			Neuronio * neuronio = camada_saida->get(j);

			printf("desejada: %lf, calculada: %lf\n", saida[j], neuronio->gerarSaida());
		}
	}
}

#endif
















