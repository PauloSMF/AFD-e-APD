//Autômato Finito Determinístico e Autômato de Pilha Determinístico//

Algoritmo desenvolvido como requisito para disciplina ECOi13-Teoria da Computação
na Universidade Federal de Itajubá.
Linguagem usada: C++

Exemplo de formado de entrada para AFD (Comentários devem ser excluídos):

5 // número de estados
2 //número de símbolos do alfabeto
2 5 // transições (de 1 para 2 se ler a, de 1 para 5 se ler b)
5 3 //transições (de 2 para 5 se ler a, de 2 para 3 se ler b)
4 5 // transições (de 3 para 4 se ler a, de 3 para 5 se ler b)
4 4 // transições (de 4 para 4 se ler a, de 4 para 4 se ler b)
5 5 // transições (de 5 para 5 se ler a, de 5 para 5 se ler b)
1 //estado inicial
1 //número de estados finais
4 //estados finais
3 //número de palavras a ser testado
abaaa //palavras de teste 1
abbababa //palavras de teste 2
abaabaaba //palavras de teste 3

Exemplo de formado de entrada para APD (Comentários devem ser excluídos):

2 // Número de estados.
2 // Número de símbolos do alfabeto.
1 // Número de símbolos do alfabeto da pilha.
1 a 1 % X // Transição. (do estado 1 para 1 se a, desempilha 𝛌, empilha X).
1 b 2 X % // Transição. (do estado 1 para 2 se b, desempilha X, empilha 𝛌).
2 b 2 X % // Transição. (do estado 2 para 2 se b, desempilha X, empilha 𝛌).
-1 // Flag para sinalizar o fim das transições.
1 // Estado inicial.
2 // Número de estados finais.
1 2 // Estados finais.
3 // Número de palavras a serem testadas.
aab // Palavra 1.
aabb // Palavra 2.
abab // Palavra 3.