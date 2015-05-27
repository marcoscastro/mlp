# Rede Neural Perceptron Multicamadas
Implementação de uma rede neural Perceptron Multicamadas (Multilayer perceptron - MLP) - Linguagem: C++

Uma MLP (Multilayer perceptron) é uma rede neural feedforward que mapeia conjuntos de dados de entrada para conjunto de saídas apropriadas.
 
MLP é composta por várias camadas de nós (vértices) em um grafo direcionado, cada camada é totalmente conectada na próxima. Exceto para os nós de entrada, cada nó é um neurônio com uma função de ativação.

MLP utiliza treinamento supervisionado, ou seja, você mostra as entradas com a respectiva saída para treinar a rede. Se você tem um conjunto de dados, geralmente é utilizado 70% para treinar e 30% para testar, isso pode variar bastante.

O processo de treinamento de redes MLP (em português a abreviação é PMC) utiliza o algoritmo backpropagation conhecido também como regra delta generalizada.

A primeira fase do treinamento é a propagação adiantada (forward) onde as amostras são inseridas nas entradas da rede e propagadas camada a camada até a produção das respectivas saídas. Isso é feito somente para obter as respostas da rede.

As respostas produzidas pelas saídas são comparadas com as respectivas respostas desejadas. São gerados desvios (erros), em seguida é aplicada a segunda fase do método backpropagation que é a backward (propagação reversa). Nessa fase as alterações dos pesos sinápticos e limiares de todos os neurônios da rede são executadas.

Para entender melhor a teoria, aconselho estudar o livro "Redes Neurais Artificiais" do Ivan Nunes.

A configuração da rede pode ser passada com um arquivo por parâmetro. As instruções sobre o formato do arquivo que deve ser passado encontra-se no arquivo "config_instrucoes".

Os arquivos "exemplo_rede" e "dataset_iris" são exemplos de redes com suas respectivas configurações. Em ambos os arquivos foi utilizada a função tangente hiperbólica, também está disponível a função linear.

Quaisquer contribuições serão bem-vindas!
