#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

const int INF = 0x3f3f3f3f;
int n, m;

int main() {



    cin >> n >> m;
    
    int TABLEAU_N = n + 1;
    int TABLEAU_M = n + m + n + n + 1; //vero, restricoes, folga, identidade do auxiliar, b

    // vetores que indicam em que coluna começa cada parte do tableux
    int idx_restricoes = n;
    int idx_folga = n + m ;
    int idx_auxiliar = n + m + n;
    int idx_b = n + m + n + n;
    ////////////////////////////////////////////////////////////////

    vector<vector<float>> tableau (TABLEAU_N, vector<float>(TABLEAU_M, 0));

    // receber vetor funcao objetiva
    for (int j = idx_restricoes; j < idx_folga; j++) {
        int entrada_c;
        cin >> entrada_c;
        tableau[0][j] = entrada_c * -1;
    }

    // receber vetor A (restricoes) (ele adiciona o vetor b logo ao lado pra depois mover ele)
    for (int i = 1; i < TABLEAU_N; i++) {
        for (int j = idx_restricoes; j < idx_folga + 1; j++) {
            cin >> tableau[i][j];
        }
    }
    //movendo o vetor b
    for (int i = 1; i < TABLEAU_N; i++) {
        tableau[i][idx_b] = tableau[i][idx_folga];
        tableau[i][idx_folga] = 0;
    }

    //identidade do vero
    for (int j = 0; j < idx_restricoes; j++) {
        tableau[j+1][j] = 1;
    }

    //identidade da folga
    for (int i = 1, j = idx_folga; i < TABLEAU_N; i++, j++) {
        tableau[i][j] = 1;
    }
    // identidade da auxiliar
    for (int i = 1, j = idx_auxiliar; i < TABLEAU_N; i++, j++) {
        tableau[i][j] = 1;
    }
    // entradas no vetor c correspondentes a pl auxiliar

    for (int j = idx_auxiliar; j < idx_b; j++) {
        tableau[0][j] = 1;
    }

    /*


    andamos pelo vetor c^T
    se acharmos uma entrada negativa:
        andamos pela coluna
        procuramos na coluna o elemento que minimiza bi/ai, tal que ci > 0
        se a coluna só tem elementos <= 0, PL ilimitada
        gera o certificado
        pivoteia o elemento que minimiza a razão acima

    */
    //verificações pré-simplex
    //verifica se o vetor b é positivo, se não for, multiplica a linha dele por -1
    for (int i = 1; i < TABLEAU_N; i++) {
        if (tableau[i][idx_b] < 0) {
            for (int j = 0; j < idx_auxiliar; j++) {
                if (tableau[i][j] != 0)//tentar tirar esse if depois
                    tableau[i][j] = tableau[i][j] * (-1);
            }
            tableau[i][idx_b] = tableau[i][idx_b] * (-1);
        }
    }



   //inicia o simplex auxiliar
   //para cada coluna de restrição

    vector<float> objetivo_original(TABLEAU_M);
    vector<float> objetivo_auxiliar(TABLEAU_M, 0);

    //guardando a objetivo original
    for (int j = 0; j < TABLEAU_M; j++) {
        objetivo_original[j] = tableau[0][j];
    }
    //guardando a objetivo auxiliar
    for (int j = idx_auxiliar; j < idx_b; j++) {
        objetivo_auxiliar[j] = 1;
    }
    


    // colocando na forma canonica
    //soma todas as linhas começando da segunda, multiplica por -1 e soma na primeira
    vector<float> linha(TABLEAU_M, 0);
    for (int j = 0; j < TABLEAU_M; j++) {
        for (int i = 1; i < TABLEAU_N; i++) {
            linha[j] += tableau[i][j] * (-1);
        }
    }
    for (int j = 0; j < TABLEAU_M; j++) {
        objetivo_auxiliar[j] += linha[j];
    }
    //



    //simplex resolvendo a auxiliar (simplex fase 2)
    
    //passa a funcao objetivo original para a funcao objetivo da auxiliar
    for(int i = 0; i < TABLEAU_M; i++) {
        tableau[0][i] = objetivo_auxiliar[i];
    }

    // escolhendo quem vai pivotear
    bool tem_negativo = true;
    while (tem_negativo) {
        tem_negativo = false;
        for (int j = idx_restricoes; j < idx_auxiliar; j++) {

            if (tableau[0][j] < 0) {
                tem_negativo = true;
                int i_min = -1;
                int j_min = -1;
                float min = INF;
                    // for que percorre toda a coluna procurando pelo menor elemento (procuraMenor())
                for(int i = 1; i < TABLEAU_N; i++) {

                    if (tableau[i][j] > 0) {
                            float razao = tableau[i][idx_b] / tableau[i][j];
                            if (razao < min) {
                                min = razao;
                                i_min = i;
                                j_min = j;
                            }
                    } 
                }
                
                    // se não achou o menor, então é ilimitada
                    if (i_min == -1 and j_min == -1) {
                        cout << "ilimitada" << endl;
                        return 0;
                    }
                    // se achou o menor, pivoteia ele
                    cout << "pivoteando elemento " << tableau[i_min][j_min] << endl;
                    //pega a linha do elemento a ser pivoteado
                    linha = tableau[i_min];
                    //para todas as linhas, pega o elemento da coluna jmin que seja diferente do pivo e faz operacao pra tornar o elemento zero
                    for (int i = 0; i < TABLEAU_N; i++) {
                        if (tableau[i][j_min] != tableau[i_min][j_min]) {
                            float fator = tableau[i][j_min] * (-1);
                            for (auto &a : linha)
                                a *= fator;
                            transform(tableau[i].begin(), tableau[i].end(), linha.begin(), tableau[i].begin(), plus<int>());
                        }
                    }
                
            }

        }

    }

















    //COMENTE O CODIGO ABAIXO PARA PRINTAR A PRINCIPAL. DESCOMENTE PRA PRINTAR A AUXILIAR.
    // for(int i = 0; i < TABLEAU_M; i++) {
    //     tableau[0][i] = objetivo_auxiliar[i];
    // }


    /////////////////////////////////////////////////////////////////////////////////////



    cout << endl << "::::::::::::::PRINT DE TESTES:::::::::::::::" << endl;

    cout << "numero de restricoes(n): " << n << endl;
    cout << "numero de variaveis (m):" << m << endl;
    cout << "tableau::::" << endl;
    for (int i = 0; i <  TABLEAU_N; i++) {
        for (int j = 0; j < TABLEAU_M; j++) { 
            if (tableau[i][j] >= 0)
                cout << "   " << tableau[i][j];
            else
                cout << "  " << tableau[i][j];
            if(j == n - 1 or j == n + m + n - 1 or j == n + m + n + n - 1) {
                cout << "| ";
            }
        }
        if (i == 0) {
            cout << endl << string(TABLEAU_M * (5), '-');
        }
        cout << endl;
    }



    cout << endl << "::::::::::::funcao objetivo original::::::::::::::" << endl;
    for (int i = 0; i < TABLEAU_M; i++) {
        cout << objetivo_original[i] << " ";
    }

    cout << endl << "::::::::::::funcao objetivo auxiliar:::::::::::::" << endl;
    for (int i = 0; i < TABLEAU_M; i++) {
        cout << objetivo_auxiliar[i] << " ";
    }

    cout << endl << ":::::::::::::::linha do pivo:::::::::::::" << endl;
    for (int i = 0; i < TABLEAU_M; i++) {
        cout << linha[i] << " ";
    }
}