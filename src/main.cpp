#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MLP.h"
#include "neuronio.h"
#include "funcao_linear.h"
#include "funcao_tan_hiperbolica.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	FILE* arq;

	if(argc < 2)
		arq = stdin; // ler da entrada padrão
	else
		arq = fopen(argv[1], "r");

	int num_entradas, neuronios_escondida, neuronios_saida;
	int qte_amostras, qte_teste, max_epocas;
	double erro_min, taxa_aprendizagem;
	char str_funcao_ativacao[255];
	
	fscanf(arq, "%d %d %d", &num_entradas, &neuronios_escondida, &neuronios_saida);
	fscanf(arq, "%s", str_funcao_ativacao);
	fscanf(arq, "%d %d", &qte_amostras, &qte_teste);
	fscanf(arq, "%d %lf %lf", &max_epocas, &erro_min, &taxa_aprendizagem);
	FuncaoAtivacao * funcao_ativacao = NULL;
	
	if(strcmp(str_funcao_ativacao, "hiperbolica") == 0)
		funcao_ativacao = new FuncaoTanHiperbolica(2);
	else if(strcmp(str_funcao_ativacao, "linear") == 0)
		funcao_ativacao = new FuncaoLinear();

	MLP mlp(qte_amostras, qte_teste, num_entradas, neuronios_escondida,
			neuronios_saida, taxa_aprendizagem, max_epocas, erro_min, funcao_ativacao);

	std::vector<std::vector<double> > amostras, saidas;

	for(int i = 0; i < qte_amostras + qte_teste; i++)
	{
		std::vector<double> amostra_entradas, amostra_saidas;
		double valor;

		for(int j = 0; j < num_entradas; j++)
		{
			fscanf(arq, "%lf", &valor);
			amostra_entradas.push_back(valor);
		}
		
		amostras.push_back(amostra_entradas);

		for(int j = 0; j < neuronios_saida; j++)
		{
			fscanf(arq, "%lf", &valor);
			amostra_saidas.push_back(valor);
		}
		
		saidas.push_back(amostra_saidas);
	}
	
	mlp.treinar(amostras, saidas);
	
	delete funcao_ativacao;
	
	if(arq != stdin)
		fclose(arq);

	return 0;
}
