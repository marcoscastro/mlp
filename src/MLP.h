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
	double taxa_aprendizado, taxa_reducao_aprendizado, erro_min;
	int num_camadas_escondidas, max_epocas;
	int qte_amostras, qte_teste;
	int num_entradas, neuronios_saida;
	FuncaoAtivacao * funcao_ativacao;
public:

	MLP(int qte_amostras, int qte_teste, int num_entradas,
		int num_camadas_escondidas, int neuronios_saida,
		double taxa_aprendizagem, double taxa_reducao_aprendizado, int max_epocas, double erro_min,
		FuncaoAtivacao * funcao_ativacao, std::vector<int>& neuronios_camadas_escondidas);
	~MLP();
	void adicionarCamada(Camada* camada);
	void treinar(std::vector<std::vector<double> >& amostras, std::vector<std::vector<double> >& saidas);
	double erroQuadraticoMedio(std::vector<std::vector<double> >& saidas);
};

MLP::MLP(int qte_amostras, int qte_teste, int num_entradas,
		 int num_camadas_escondidas, int neuronios_saida,
		 double taxa_aprendizagem, double taxa_reducao_aprendizado, int max_epocas, double erro_min,
		 FuncaoAtivacao * funcao_ativacao, std::vector<int>& neuronios_camadas_escondidas)
{
	// atribuição de variáveis
	this->taxa_aprendizado = taxa_aprendizagem;
	this->taxa_reducao_aprendizado = taxa_reducao_aprendizado;
	this->num_camadas_escondidas = num_camadas_escondidas;
	this->max_epocas = max_epocas;
	this->erro_min = erro_min;
	this->qte_amostras = qte_amostras;
	this->qte_teste = qte_teste;
	this->num_entradas = num_entradas;
	this->neuronios_saida = neuronios_saida;
	this->funcao_ativacao = funcao_ativacao;

	// nessa parte é feita o forward: propagar as entradas

	// adiciona sinal de entrada -1 em cada entrada
	// o <= é por causa do limiar de ativação
	for(int i = 0; i <= num_entradas; i++)
	{
		SinalEntrada * sinal_entrada = new SinalEntrada(-1);
		entradas.push_back(sinal_entrada);
	}

	// cria a camada de entrada e faz as conexoes
	Camada * camada_entrada = new Camada();

	for(int i = 0; i < num_entradas; i++)
	{
		Neuronio * neuronio = new Neuronio(funcao_ativacao);
		for(int j = 0; j <= num_entradas; j++)
			neuronio->receberEntrada(entradas[j], (rand() % 1001) / 1000.0);
		camada_entrada->receberNeuronio(neuronio);
	}

	adicionarCamada(camada_entrada); // adiciona a camada de entrada

	// cria as camadas escondidas e faz as conexões
	for(int k = 0; k < num_camadas_escondidas; k++)
	{
		Camada * camada_escondida = new Camada();

		for(int i = 0; i < neuronios_camadas_escondidas[k]; i++)
		{
			Neuronio * neuronio = new Neuronio(funcao_ativacao);
			neuronio->receberEntrada(entradas[0], (rand() % 1001) / 1000.0);
			for(int j = 0; j < neuronios_camadas_escondidas[k]; j++)
				neuronio->receberEntrada(camadas[k]->get(j), (rand() % 1001) / 1000.0);
			camada_escondida->receberNeuronio(neuronio);
		}

		adicionarCamada(camada_escondida); // adiciona a camada escondida
	}

	// cria a camada de saida e faz as conexoes
	Camada * camada_saida = new Camada();

	int num_neuronios_ultima_camada_escondida = camadas[num_camadas_escondidas]->getQuantidadeNeuronios();

	for(int i = 0; i < neuronios_saida; i++)
	{
		Neuronio * neuronio = new Neuronio(funcao_ativacao);
		neuronio->receberEntrada(entradas[0], (rand() % 1001) / 1000.0);
		for(int j = 0; j < num_neuronios_ultima_camada_escondida; j++)
			neuronio->receberEntrada(camadas[num_camadas_escondidas]->get(j), (rand() % 1001) / 1000.0);
		camada_saida->receberNeuronio(neuronio);
	}

	adicionarCamada(camada_saida); // adiciona a camada de saída
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
		double eq = 0; // erro quadratico

		for(int j = 0; j < neuronios_saida; j++)
			eq += pow(saida[j] - camada_saida->get(j)->gerarSaida(), 2);
		erro += eq / 2;
	}
	return erro / qte_amostras; // erro quadratico medio
}

void MLP::treinar(std::vector<std::vector<double> >& amostras, std::vector<std::vector<double> >& saidas)
{
	double erro;
	int num_epocas = 0;

	Camada* camada_saida = camadas[2];

	// treina pelo numero de epocas
	while(num_epocas <= max_epocas)
	{
		// calculo do erro quadratico medio
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
				neuronio->invalidaCalculo();
			}

			// faz o backpropagation da camada de saida
			for(int j = 0; j < neuronios_saida; j++)
			{
				Neuronio * neuronio = camada_saida->get(j);

				double sigma = (saida[j] - neuronio->gerarSaida()) *
							   neuronio->getFuncaoAtivacao()->derivada(neuronio->getSomatorio());

				neuronio->setSigma(sigma);

				for(unsigned int k = 0; k < neuronio->getQuantidadeEntradas(); k++)
				{
					neuronio->setPeso(k, neuronio->getPeso(k) +
									  taxa_aprendizado * sigma * neuronio->getEntrada(k)->gerarSaida());
				}
			}

			// faz o backpropagation das outras camadas (exceto a da saída)
			for(int t = camadas.size() - 2; t >= 0; t--)
			{
				Camada* camada = camadas[t];
				Camada* camada_frente = camadas[t + 1];

				for(unsigned int j = 0; j < camada->getQuantidadeNeuronios(); j++)
				{
					Neuronio * neuronio = camada->get(j);
					double somatorio = 0;

					// pega os que estao saindo exceto o -1
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

		// calculo do erro quadratico medio
		erro = erroQuadraticoMedio(saidas);
		num_epocas++;

		// atualiza a taxa de aprendizado
		taxa_aprendizado *= taxa_reducao_aprendizado;

		if(fabs(erro - erro_ant) <= erro_min)
		{
			printf("epoca: %d, erro: %lf\n", num_epocas, fabs(erro - erro_ant));
			break;
		}
	}

	// teste da rede com uma determinada quantidade de amostras
	for(int i = qte_amostras; i < qte_amostras + qte_teste; i++)
	{
		std::vector<double>& amostra = amostras[i];
		std::vector<double>& saida = saidas[i];

		// atribuindo os valores das entradas
		for(int j = 1; j <= num_entradas; j++)
			entradas[j]->setValor(amostra[j - 1]);

		for(int j = 0; j < neuronios_saida; j++)
		{
			Neuronio * neuronio = camada_saida->get(j);
			neuronio->invalidaCalculo();
		}

		for(int j = 0; j < neuronios_saida; j++)
		{
			Neuronio * neuronio = camada_saida->get(j);

			printf("desejada: %lf\tsaida da rede: %lf\n",
				   saida[j], neuronio->gerarSaida());
		}
	}
}

#endif
