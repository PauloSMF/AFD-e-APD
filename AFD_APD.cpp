//PAULO SÉRGIO MOREIRA FERREIRA
#include<iostream>
#include<string>
#include<algorithm>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<stack>
using namespace std;

void criar_AFD(ifstream &arquivo, int **&D, int &num_estados, int &num_alfabeto,int &estado_inicial, int &num_estados_finais, int *&F, int &num_palavras, string *&palavras)
{
    arquivo >> num_estados;                     //Leitura da primeira linha do arquivo.
    arquivo >> num_alfabeto;                    //Leitura da segunda linha do arquivo.

    D = new int*[num_estados];
    for(int i =0 ; i < num_estados ; i++)
        D[i] = new int[num_alfabeto];           //Criação da matriz de transições D.

    for(int i = 0 ; i < num_estados ; i++)
        for(int j = 0 ; j <num_alfabeto ; j++)
            arquivo >> D[i][j];                 //Preenchimento da matriz de transições D com as próximas x linhas do arquivo.

    arquivo >> estado_inicial;                  //Leitura do estado inicial.
    arquivo >> num_estados_finais;              //Leitura da linha referente ao número de estados finais.

    F = new int[num_estados_finais];            //Criação do vetor F de estados finais.

    for(int i = 0 ; i < num_estados_finais ; i++)   //Leitura de todos os estados finais.
        arquivo >> F[i];

    arquivo >> num_palavras;                        //Leitura do número de palavras a ser testadas.

    palavras = new string[num_palavras];            //Criação de um vetor para palavras a ser testadas.

    for(int i = 0 ; i < num_palavras ; i++)         //Leitura das palavras a ser testadas.
        arquivo >> palavras[i];
}

bool testar_palavra_AFD(int **D, int estado_inicial, int *F, string palavras, int num_estados_finais)
{
    cout<<"Teste para a palavra "<<palavras<<":"<<endl;

    int estado_atual = estado_inicial - 1;      //Variável para realizar a transição de estados.
                                                //É necessário subtrair 1 pois o primeiro estado armazenado na tabela é 1
                                                //entretando na matriz D é 0, logo 1 a menos que a tabela.

    cout<<"O teste se inicia pelo estado: "<<estado_inicial<<endl;

    for(int j = 0 ; j < palavras.length() ; j++)        //Loop para o consumo da palavra.
    {
        int simbolo = (int)palavras[j] - 97;            //Convertendo o símbolo char para seu equivalente em int na matriz D.
        estado_atual = D[estado_atual][simbolo] - 1;    //Transição entre estados. Novamente é necessário subtrair 1.
        cout<<"O teste passa pelo estado: "<<estado_atual + 1<<endl;
    }

    for(int k = 0 ; k < num_estados_finais ; k++)   //Verificando se o estado atual é um dos estados finais.
        if((estado_atual + 1) == F[k])
            return true;                            //Caso o estado atual é um estado final, a palavra pertence a linguagem.
    return false;                                   //Caso não está em um estado final, a palavra não é aceita.
}

struct transicao    //Estrutura representando as transições da APD.
{
    int proximo_estado;         //Próximo estado apontado por tal transição.

    int proximo_estado_lambda;  //Caso tal estado tenha uma transição dependente de lambda, esta será guardada em uma coluna
                                //referente ao último símbolo lido, ou ao a, caso seja o primeiro. (Adiciona uma restrição ao
                                //documento txt).
    char desempilha_lambda;     //Qual símbolo a transição lambda irá desempilhar.
    bool transicao_lambda;      //Flag indicando que tal estado tem uma transição a partir de lambda.

    vector<string> empilha;     //Vetores que armazenam as possíveis transiçõe a partir deste estado
    vector<char> desempilha;    //desconsiderando a transição a partir de lambda.
};

void criar_APD(ifstream &arquivo, transicao **&D, int &num_estados, int &num_alfabeto, int &num_simbolos, int &estado_inicial, int &num_estados_finais, int *&F, int &num_palavras, string *&palavras)
{
    int posicao_arquivo;                        //Futuramente será necessário retornar linhas no arquivo, logo uma variável
                                                //nos dirá em qual posição do arquivo estamos.
    arquivo >> num_estados;                     //Leitura da primeira linha do arquivo.
    arquivo >> num_alfabeto;                    //Leitura da segunda linha do arquivo.
    arquivo >> num_simbolos;                    //Leitura da terceira linha do arquivo.

    D = new transicao*[num_estados];
    for(int i =0 ; i < num_estados ; i++)
        D[i] = new transicao[num_alfabeto];     //Criação da matriz de transições D.

    for(int i = 0 ; i < num_estados ; i++)
        for(int j = 0 ; j <num_alfabeto ; j++)
            D[i][j].transicao_lambda = false;   //Iniciando todas as posições como não tendo transição a partir de lambda.

    int verifica_estado;                        //Flag para verificação do estado atual referente a uma transição.
    char verifica_simbolo;                      //Flag para verificação do símbolo referente a uma transição.

    arquivo >> verifica_estado;                 //Leitura do estado no arquivo.
    arquivo >> verifica_simbolo;                //Leitura do símbolo no arquivo.

    if(verifica_simbolo == '%')                 //Caso o primeiro símbolo lido seja lambda.
    {
        verifica_simbolo = 'a';                 //Como o primeiro símbolo da matriz é 'a', a transição de lambda será guardada
        D[0][0].transicao_lambda = true;        //juntamente com as transições de a.
    }

    for(int i = 0 ; i < num_estados ; i++)      //Preenchimento da matriz de transições D com as próximas x linhas do arquivo.
        for(int j = 0 ; j <num_alfabeto ; j++)
        {
            bool fim_transicoes = false;

            while(verifica_estado == (i + 1) && (int)verifica_simbolo == (j + 97))
            {                                   //Enquanto as transições são referentes a um estado e a um símbolo (ou lambda).
                string aux;

                if(D[i][j].transicao_lambda == false)   //Caso o último símbolo lido não foi o lambda.
                {
                    arquivo >> D[i][j].proximo_estado;  //Atualizando o próximo estado apontado por tal símbolo.
                    arquivo >> aux;
                    D[i][j].desempilha.push_back(aux[0]); //Atualizando o que será desempilhado por tal símbolo em tal transição.
                    arquivo >> aux;
                    D[i][j].empilha.push_back(aux);       //Atualizando o que será empilhado por tal símbolo em tal transição.
                }
                else                            //Caso o último símbolo lido foi um lambda.
                {
                    arquivo >> D[i][j].proximo_estado_lambda;   //Atualizando as informações referentes a transição a partir do
                    arquivo >> D[i][j].desempilha_lambda;       //símbolo lambda.
                    arquivo >> aux;
                }

                int estado_anterior = verifica_estado;
                char simbolo_anterior = verifica_simbolo; //Caso o último símbolo lido seja lambda é necessário essa informação.

                arquivo >> verifica_estado;         //Lendo a próxima linha do arquivo.

                if(verifica_estado == -1)           //O fim da inserção de transições é marcada com um -1 no arquivo.
                {
                    verifica_estado = estado_anterior; //Caso o estado é -1 é necessário realizar as primeiras operações deste
                    fim_transicoes = true;             // laço novamente.
                }
                if(fim_transicoes == true)
                    break;  //Caso foi lido o fim das transições realizar as próximas operações acarretariam em um erro de leitura.

                arquivo >> verifica_simbolo;

                if(verifica_simbolo == '%')         //Caso um lambda foi lido.
                {
                    verifica_simbolo = simbolo_anterior; //É necessário voltar mais uma vez neste laço para atualizar

                    int indice = (int)simbolo_anterior - 97;
                    D[verifica_estado - 1][indice].transicao_lambda = true;     //as informações referentes a transição a partir do símbolo lambda.
                }
            }                                  //Atualizando a estrutura de transição armazenado a presente no arquivo.
        }

    arquivo >> estado_inicial;                  //Leitura do estado inicial.
    arquivo >> num_estados_finais;              //Leitura da linha referente ao número de estados finais.

    F = new int[num_estados_finais];            //Criação do vetor F de estados finais.

    for(int i = 0 ; i < num_estados_finais ; i++)   //Leitura de todos os estados finais.
        arquivo >> F[i];

    arquivo >> num_palavras;                        //Leitura do número de palavras a ser testadas.

    palavras = new string[num_palavras];            //Criação de um vetor para palavras a ser testadas.

    for(int i = 0 ; i < num_palavras ; i++)         //Leitura das palavras a ser testadas.
        arquivo >> palavras[i];
}

bool testar_palavra_APD(transicao **D, int estado_inicial, int num_alfabeto, int *F, string palavras, int num_estados_finais)
{
    cout<<"Teste para a palavra "<<palavras<<":"<<endl;

    stack<char> pilha;
    int estado_atual = estado_inicial - 1;      //Variável para realizar a transição de estados.
                                                //É necessário subtrair 1 pois o primeiro estado armazenado na tabela é 1
                                                //entretando na matriz D é 0, logo 1 a menos que a tabela.

    cout<<"O teste se inicia pelo estado: "<<estado_inicial<<endl;

    for(int j = 0 ; j < palavras.length() ; j++)        //Loop para o consumo da palavra.
    {
        int simbolo = (int)palavras[j] - 97;            //Convertendo o símbolo char para seu equivalente em int na matriz D.
        bool pilha_atualizada = false;                  //Flag para verificar se a pilha foi atualizada.

        for(int k = 0 ; k < D[estado_atual][simbolo].empilha.size() ; k++) //Loop para que garantir que algumas das possíveis
        {                                                                  //transições de um símbolo sejam feitas.
            if(D[estado_atual][simbolo].desempilha[k] == '%')           //Caso na transição nada seja desempilhado (lambda)
            {
                for(int l = D[estado_atual][simbolo].empilha[k].length() - 1 ; l >= 0 ; l--)
                    if(D[estado_atual][simbolo].empilha[k][l] != '%')
                        pilha.push(D[estado_atual][simbolo].empilha[k][l]); //Empilhando os elementos.

                pilha_atualizada = true;
                break;  //Terminando o laço.
            }
            else if(!pilha.empty() && D[estado_atual][simbolo].desempilha[k] == pilha.top())
            {       //Caso o símbolo a se desempilhar esteja no topo.
                pilha.pop();

                for(int l = D[estado_atual][simbolo].empilha[k].length() - 1 ; l >= 0 ; l--)
                    if(D[estado_atual][simbolo].empilha[k][l] != '%')
                        pilha.push(D[estado_atual][simbolo].empilha[k][l]); //Empilhando os elementos.

                pilha_atualizada = true;
                break;  //Terminando o laço.
            }
        }

        if(pilha_atualizada == false)   //Se a pilha não foi atualizada anteriormente ainda há uma possibilidade de transição.
            for(int k = 0 ; k < D[estado_atual][simbolo].empilha.size() ; k++)
                if(!pilha.empty() && D[estado_atual][simbolo].desempilha[k] != pilha.top())
                {//A possibilidade de empilhar um símbolo diferente do topo.
                    for(int l = D[estado_atual][simbolo].empilha[k].length() - 1 ; l >= 0 ; l--)
                        if(D[estado_atual][simbolo].empilha[k][l] != '%')
                            pilha.push(D[estado_atual][simbolo].empilha[k][l]);//Empilhando os elementos.

                    if(D[estado_atual][simbolo].desempilha[k] == pilha.top())
                    {
                        pilha.pop();
                        pilha_atualizada = true;
                        break;  //Terminando o laço.
                    }//Caso tenha sido empilhado o elemento que deveria ser desempilhado.
                }//Esta laço condicional representa a mudança de comportamento do autômato.

        if(pilha_atualizada == false)
            return false;       //Se a pilha não foi atualizada até aqui, há uma inconsistência entre a linguagem e a palavra.

        estado_atual = D[estado_atual][simbolo].proximo_estado - 1; //Atualizando o estado atual.
        cout<<"O teste passa pelo estado: "<<estado_atual + 1<<endl;
    }

    for(int m = 0 ; m < num_alfabeto ; m++) //Testando se ao fim da leitura da palavra tem uma transição a partir de lambda
        if(!pilha.empty() && D[estado_atual][m].transicao_lambda == true) //no último estado visitado.
            if(pilha.top() == D[estado_atual][m].desempilha_lambda) //Caso o símbolo na pilha é o a ser desempilhado por lambda.
            {
                while(pilha.empty() == false) //Enquanto há elementos na pilha há "entrada" de lambda.
                {
                    estado_atual = D[estado_atual][m].proximo_estado_lambda - 1;
                    pilha.pop();
                    cout<<"O teste passa pelo estado: "<<estado_atual + 1<<endl;
                }
                break;
            }

    for(int m = 0 ; m < num_estados_finais ; m++)   //Verificando se o estado atual é um dos estados finais.
        if((estado_atual + 1) == F[m] && pilha.empty())
            return true;                            //Caso o estado atual é um estado final, a palavra pertence a linguagem.
    return false;                                   //Caso não está em um estado final, a palavra não é aceita.
}

int main()
{
    setlocale(LC_ALL,"Portuguese");

    int opcao;

    do
    {
        cout<<"||MENU||"<<endl<<endl;
        cout<<"1 - Testar palavras em um AFD."<<endl;
        cout<<"2 - Testar palavras em um APD."<<endl;
        cout<<"3 - Sair."<<endl;
        cout<<"Escolha uma opção: "<<endl;
        cin>>opcao;

        switch(opcao)
        {
            case 1:
            {
                cout<<"Antes de continuar coloque o arquivo com o nome AFD.txt na pasta do programa."<<endl;
                system("pause");
                system("CLS");

                ifstream arquivo_automato;
                arquivo_automato.open("AFD.txt");

                while(!arquivo_automato.is_open())
                {
                    cout<<"Erro na leitura do arquivo! Favor nomeá-lo com 'AFD.txt' e colocá-lo na pasta do programa."<<endl;
                    system("pause");
                    system("CLS");
                }

                int num_estados, num_alfabeto,estado_inicial, num_estados_finais, num_palavras;
                int *F;
                int **D;
                string *palavras;

                criar_AFD(arquivo_automato,D,num_estados,num_alfabeto,estado_inicial,num_estados_finais,F,num_palavras,palavras);

                for(int i = 0 ; i < num_palavras ; i++)
                {
                    bool teste_palavra = testar_palavra_AFD(D,estado_inicial,F,palavras[i],num_estados_finais);

                    if(teste_palavra == true) cout<<"A palavra "<<palavras[i]<<" pertence a linguagem."<<endl<<endl;
                    else cout<<"A palavra "<<palavras[i]<<" não pertence a linguagem."<<endl<<endl;
                }

                system("pause");
                system("CLS");
                arquivo_automato.close();
                break;
            }
            case 2:
            {
                cout<<"Antes de continuar coloque o arquivo com o nome APD.txt na pasta do programa."<<endl;
                system("pause");
                system("CLS");

                ifstream arquivo_automato;
                arquivo_automato.open("APD.txt");

                while(!arquivo_automato.is_open())
                {
                    cout<<"Erro na leitura do arquivo! Favor nomeá-lo com 'APD.txt' e colocá-lo na pasta do programa."<<endl;
                    system("pause");
                    system("CLS");
                }

                int num_estados, num_alfabeto,estado_inicial, num_estados_finais, num_palavras, num_simbolos;;
                int *F;
                transicao **teste;
                string *palavras;

                criar_APD(arquivo_automato,teste,num_estados,num_alfabeto,num_simbolos,estado_inicial,num_estados_finais,F,num_palavras,palavras);

                for(int i=0;i<num_palavras;i++)
                {
                    bool teste_palavra = testar_palavra_APD(teste,estado_inicial,num_alfabeto,F,palavras[i],num_estados_finais);

                    if(teste_palavra == true) cout<<"A palavra "<<palavras[i]<<" pertence a linguagem."<<endl<<endl;
                    else cout<<"A palavra "<<palavras[i]<<" não pertence a linguagem."<<endl<<endl;
                }

                system("pause");
                system("CLS");
                arquivo_automato.close();
                break;
            }
            case 3:
                cout<<endl<<"Saindo..."<<endl;
                break;

            default:
                cout<<endl<<"Opção inválida!"<<endl<<endl;
                system("pause");
                system("CLS");
        }
    }while(opcao != 3);

    return 0;
}
