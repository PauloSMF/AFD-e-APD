//PAULO S�RGIO MOREIRA FERREIRA
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
        D[i] = new int[num_alfabeto];           //Cria��o da matriz de transi��es D.

    for(int i = 0 ; i < num_estados ; i++)
        for(int j = 0 ; j <num_alfabeto ; j++)
            arquivo >> D[i][j];                 //Preenchimento da matriz de transi��es D com as pr�ximas x linhas do arquivo.

    arquivo >> estado_inicial;                  //Leitura do estado inicial.
    arquivo >> num_estados_finais;              //Leitura da linha referente ao n�mero de estados finais.

    F = new int[num_estados_finais];            //Cria��o do vetor F de estados finais.

    for(int i = 0 ; i < num_estados_finais ; i++)   //Leitura de todos os estados finais.
        arquivo >> F[i];

    arquivo >> num_palavras;                        //Leitura do n�mero de palavras a ser testadas.

    palavras = new string[num_palavras];            //Cria��o de um vetor para palavras a ser testadas.

    for(int i = 0 ; i < num_palavras ; i++)         //Leitura das palavras a ser testadas.
        arquivo >> palavras[i];
}

bool testar_palavra_AFD(int **D, int estado_inicial, int *F, string palavras, int num_estados_finais)
{
    cout<<"Teste para a palavra "<<palavras<<":"<<endl;

    int estado_atual = estado_inicial - 1;      //Vari�vel para realizar a transi��o de estados.
                                                //� necess�rio subtrair 1 pois o primeiro estado armazenado na tabela � 1
                                                //entretando na matriz D � 0, logo 1 a menos que a tabela.

    cout<<"O teste se inicia pelo estado: "<<estado_inicial<<endl;

    for(int j = 0 ; j < palavras.length() ; j++)        //Loop para o consumo da palavra.
    {
        int simbolo = (int)palavras[j] - 97;            //Convertendo o s�mbolo char para seu equivalente em int na matriz D.
        estado_atual = D[estado_atual][simbolo] - 1;    //Transi��o entre estados. Novamente � necess�rio subtrair 1.
        cout<<"O teste passa pelo estado: "<<estado_atual + 1<<endl;
    }

    for(int k = 0 ; k < num_estados_finais ; k++)   //Verificando se o estado atual � um dos estados finais.
        if((estado_atual + 1) == F[k])
            return true;                            //Caso o estado atual � um estado final, a palavra pertence a linguagem.
    return false;                                   //Caso n�o est� em um estado final, a palavra n�o � aceita.
}

struct transicao    //Estrutura representando as transi��es da APD.
{
    int proximo_estado;         //Pr�ximo estado apontado por tal transi��o.

    int proximo_estado_lambda;  //Caso tal estado tenha uma transi��o dependente de lambda, esta ser� guardada em uma coluna
                                //referente ao �ltimo s�mbolo lido, ou ao a, caso seja o primeiro. (Adiciona uma restri��o ao
                                //documento txt).
    char desempilha_lambda;     //Qual s�mbolo a transi��o lambda ir� desempilhar.
    bool transicao_lambda;      //Flag indicando que tal estado tem uma transi��o a partir de lambda.

    vector<string> empilha;     //Vetores que armazenam as poss�veis transi��e a partir deste estado
    vector<char> desempilha;    //desconsiderando a transi��o a partir de lambda.
};

void criar_APD(ifstream &arquivo, transicao **&D, int &num_estados, int &num_alfabeto, int &num_simbolos, int &estado_inicial, int &num_estados_finais, int *&F, int &num_palavras, string *&palavras)
{
    int posicao_arquivo;                        //Futuramente ser� necess�rio retornar linhas no arquivo, logo uma vari�vel
                                                //nos dir� em qual posi��o do arquivo estamos.
    arquivo >> num_estados;                     //Leitura da primeira linha do arquivo.
    arquivo >> num_alfabeto;                    //Leitura da segunda linha do arquivo.
    arquivo >> num_simbolos;                    //Leitura da terceira linha do arquivo.

    D = new transicao*[num_estados];
    for(int i =0 ; i < num_estados ; i++)
        D[i] = new transicao[num_alfabeto];     //Cria��o da matriz de transi��es D.

    for(int i = 0 ; i < num_estados ; i++)
        for(int j = 0 ; j <num_alfabeto ; j++)
            D[i][j].transicao_lambda = false;   //Iniciando todas as posi��es como n�o tendo transi��o a partir de lambda.

    int verifica_estado;                        //Flag para verifica��o do estado atual referente a uma transi��o.
    char verifica_simbolo;                      //Flag para verifica��o do s�mbolo referente a uma transi��o.

    arquivo >> verifica_estado;                 //Leitura do estado no arquivo.
    arquivo >> verifica_simbolo;                //Leitura do s�mbolo no arquivo.

    if(verifica_simbolo == '%')                 //Caso o primeiro s�mbolo lido seja lambda.
    {
        verifica_simbolo = 'a';                 //Como o primeiro s�mbolo da matriz � 'a', a transi��o de lambda ser� guardada
        D[0][0].transicao_lambda = true;        //juntamente com as transi��es de a.
    }

    for(int i = 0 ; i < num_estados ; i++)      //Preenchimento da matriz de transi��es D com as pr�ximas x linhas do arquivo.
        for(int j = 0 ; j <num_alfabeto ; j++)
        {
            bool fim_transicoes = false;

            while(verifica_estado == (i + 1) && (int)verifica_simbolo == (j + 97))
            {                                   //Enquanto as transi��es s�o referentes a um estado e a um s�mbolo (ou lambda).
                string aux;

                if(D[i][j].transicao_lambda == false)   //Caso o �ltimo s�mbolo lido n�o foi o lambda.
                {
                    arquivo >> D[i][j].proximo_estado;  //Atualizando o pr�ximo estado apontado por tal s�mbolo.
                    arquivo >> aux;
                    D[i][j].desempilha.push_back(aux[0]); //Atualizando o que ser� desempilhado por tal s�mbolo em tal transi��o.
                    arquivo >> aux;
                    D[i][j].empilha.push_back(aux);       //Atualizando o que ser� empilhado por tal s�mbolo em tal transi��o.
                }
                else                            //Caso o �ltimo s�mbolo lido foi um lambda.
                {
                    arquivo >> D[i][j].proximo_estado_lambda;   //Atualizando as informa��es referentes a transi��o a partir do
                    arquivo >> D[i][j].desempilha_lambda;       //s�mbolo lambda.
                    arquivo >> aux;
                }

                int estado_anterior = verifica_estado;
                char simbolo_anterior = verifica_simbolo; //Caso o �ltimo s�mbolo lido seja lambda � necess�rio essa informa��o.

                arquivo >> verifica_estado;         //Lendo a pr�xima linha do arquivo.

                if(verifica_estado == -1)           //O fim da inser��o de transi��es � marcada com um -1 no arquivo.
                {
                    verifica_estado = estado_anterior; //Caso o estado � -1 � necess�rio realizar as primeiras opera��es deste
                    fim_transicoes = true;             // la�o novamente.
                }
                if(fim_transicoes == true)
                    break;  //Caso foi lido o fim das transi��es realizar as pr�ximas opera��es acarretariam em um erro de leitura.

                arquivo >> verifica_simbolo;

                if(verifica_simbolo == '%')         //Caso um lambda foi lido.
                {
                    verifica_simbolo = simbolo_anterior; //� necess�rio voltar mais uma vez neste la�o para atualizar

                    int indice = (int)simbolo_anterior - 97;
                    D[verifica_estado - 1][indice].transicao_lambda = true;     //as informa��es referentes a transi��o a partir do s�mbolo lambda.
                }
            }                                  //Atualizando a estrutura de transi��o armazenado a presente no arquivo.
        }

    arquivo >> estado_inicial;                  //Leitura do estado inicial.
    arquivo >> num_estados_finais;              //Leitura da linha referente ao n�mero de estados finais.

    F = new int[num_estados_finais];            //Cria��o do vetor F de estados finais.

    for(int i = 0 ; i < num_estados_finais ; i++)   //Leitura de todos os estados finais.
        arquivo >> F[i];

    arquivo >> num_palavras;                        //Leitura do n�mero de palavras a ser testadas.

    palavras = new string[num_palavras];            //Cria��o de um vetor para palavras a ser testadas.

    for(int i = 0 ; i < num_palavras ; i++)         //Leitura das palavras a ser testadas.
        arquivo >> palavras[i];
}

bool testar_palavra_APD(transicao **D, int estado_inicial, int num_alfabeto, int *F, string palavras, int num_estados_finais)
{
    cout<<"Teste para a palavra "<<palavras<<":"<<endl;

    stack<char> pilha;
    int estado_atual = estado_inicial - 1;      //Vari�vel para realizar a transi��o de estados.
                                                //� necess�rio subtrair 1 pois o primeiro estado armazenado na tabela � 1
                                                //entretando na matriz D � 0, logo 1 a menos que a tabela.

    cout<<"O teste se inicia pelo estado: "<<estado_inicial<<endl;

    for(int j = 0 ; j < palavras.length() ; j++)        //Loop para o consumo da palavra.
    {
        int simbolo = (int)palavras[j] - 97;            //Convertendo o s�mbolo char para seu equivalente em int na matriz D.
        bool pilha_atualizada = false;                  //Flag para verificar se a pilha foi atualizada.

        for(int k = 0 ; k < D[estado_atual][simbolo].empilha.size() ; k++) //Loop para que garantir que algumas das poss�veis
        {                                                                  //transi��es de um s�mbolo sejam feitas.
            if(D[estado_atual][simbolo].desempilha[k] == '%')           //Caso na transi��o nada seja desempilhado (lambda)
            {
                for(int l = D[estado_atual][simbolo].empilha[k].length() - 1 ; l >= 0 ; l--)
                    if(D[estado_atual][simbolo].empilha[k][l] != '%')
                        pilha.push(D[estado_atual][simbolo].empilha[k][l]); //Empilhando os elementos.

                pilha_atualizada = true;
                break;  //Terminando o la�o.
            }
            else if(!pilha.empty() && D[estado_atual][simbolo].desempilha[k] == pilha.top())
            {       //Caso o s�mbolo a se desempilhar esteja no topo.
                pilha.pop();

                for(int l = D[estado_atual][simbolo].empilha[k].length() - 1 ; l >= 0 ; l--)
                    if(D[estado_atual][simbolo].empilha[k][l] != '%')
                        pilha.push(D[estado_atual][simbolo].empilha[k][l]); //Empilhando os elementos.

                pilha_atualizada = true;
                break;  //Terminando o la�o.
            }
        }

        if(pilha_atualizada == false)   //Se a pilha n�o foi atualizada anteriormente ainda h� uma possibilidade de transi��o.
            for(int k = 0 ; k < D[estado_atual][simbolo].empilha.size() ; k++)
                if(!pilha.empty() && D[estado_atual][simbolo].desempilha[k] != pilha.top())
                {//A possibilidade de empilhar um s�mbolo diferente do topo.
                    for(int l = D[estado_atual][simbolo].empilha[k].length() - 1 ; l >= 0 ; l--)
                        if(D[estado_atual][simbolo].empilha[k][l] != '%')
                            pilha.push(D[estado_atual][simbolo].empilha[k][l]);//Empilhando os elementos.

                    if(D[estado_atual][simbolo].desempilha[k] == pilha.top())
                    {
                        pilha.pop();
                        pilha_atualizada = true;
                        break;  //Terminando o la�o.
                    }//Caso tenha sido empilhado o elemento que deveria ser desempilhado.
                }//Esta la�o condicional representa a mudan�a de comportamento do aut�mato.

        if(pilha_atualizada == false)
            return false;       //Se a pilha n�o foi atualizada at� aqui, h� uma inconsist�ncia entre a linguagem e a palavra.

        estado_atual = D[estado_atual][simbolo].proximo_estado - 1; //Atualizando o estado atual.
        cout<<"O teste passa pelo estado: "<<estado_atual + 1<<endl;
    }

    for(int m = 0 ; m < num_alfabeto ; m++) //Testando se ao fim da leitura da palavra tem uma transi��o a partir de lambda
        if(!pilha.empty() && D[estado_atual][m].transicao_lambda == true) //no �ltimo estado visitado.
            if(pilha.top() == D[estado_atual][m].desempilha_lambda) //Caso o s�mbolo na pilha � o a ser desempilhado por lambda.
            {
                while(pilha.empty() == false) //Enquanto h� elementos na pilha h� "entrada" de lambda.
                {
                    estado_atual = D[estado_atual][m].proximo_estado_lambda - 1;
                    pilha.pop();
                    cout<<"O teste passa pelo estado: "<<estado_atual + 1<<endl;
                }
                break;
            }

    for(int m = 0 ; m < num_estados_finais ; m++)   //Verificando se o estado atual � um dos estados finais.
        if((estado_atual + 1) == F[m] && pilha.empty())
            return true;                            //Caso o estado atual � um estado final, a palavra pertence a linguagem.
    return false;                                   //Caso n�o est� em um estado final, a palavra n�o � aceita.
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
        cout<<"Escolha uma op��o: "<<endl;
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
                    cout<<"Erro na leitura do arquivo! Favor nome�-lo com 'AFD.txt' e coloc�-lo na pasta do programa."<<endl;
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
                    else cout<<"A palavra "<<palavras[i]<<" n�o pertence a linguagem."<<endl<<endl;
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
                    cout<<"Erro na leitura do arquivo! Favor nome�-lo com 'APD.txt' e coloc�-lo na pasta do programa."<<endl;
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
                    else cout<<"A palavra "<<palavras[i]<<" n�o pertence a linguagem."<<endl<<endl;
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
                cout<<endl<<"Op��o inv�lida!"<<endl<<endl;
                system("pause");
                system("CLS");
        }
    }while(opcao != 3);

    return 0;
}
