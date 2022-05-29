#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iomanip>

using namespace std;

const float INF = 0x3f3f3f3f;
const float limite = 1e-6;
int n, m;

int TABLEAU_N;
int TABLEAU_M; //vero, restricoes, folga, identidade do auxiliar, b

// vetores que indicam em que coluna começa cada parte do tableux
int idx_restricoes;
int idx_folga;
int idx_auxiliar;
int idx_b;
/////////////////////////////TESTES///////////////////////////////////


void iguala_a_zero(vector<vector<float>> &tableau) {
    for (int i = 0; i < TABLEAU_N; i++) {
        for (int j = 0; j < TABLEAU_M; j++) {
            if (tableau[i][j] <= limite) {
                tableau[i][j] = 0;
            }
        }
    }
}

void teste_mostrar_tableau (vector<vector<float>> tableau) {
    cout << endl;
    for (int i = 0; i <  TABLEAU_N; i++) {
        for (int j = 0; j < TABLEAU_M; j++) { 
            if (tableau[i][j] >= 0)
                cout << "   " << setprecision(2) << fixed << (tableau[i][j]);
            else
                cout << "  " << setprecision(2) << fixed << (tableau[i][j]);
            if(j == n - 1 or j == n + m + n - 1 or j == n + m + n + n - 1) {
                cout << "| ";
            }
        }
        if (i == 0) {
            cout << endl << string(TABLEAU_M * (8), '-');
        }
        cout << endl;
    }
}

void teste_mostrar_fun_obj_orig (vector<float> fun_objetivo_original) {
    cout << endl << "::::::::::::funcao objetivo original::::::::::::::" << endl;
    for (int i = 0; i < TABLEAU_M; i++) {
        cout << fun_objetivo_original[i] << " ";
    }
}

void teste_mostrar_fun_obj_aux (vector<float> fun_objetivo_aux) {
    cout << endl << "::::::::::::funcao objetivo auxiliar:::::::::::::" << endl;
    for (int i = 0; i < TABLEAU_M; i++) {
        cout << fun_objetivo_aux[i] << " ";
    }
}

//////////////////////////////////////////////////////////////

void ler_entradas (vector<vector<float>> &tableau) {
    //receber funcao objetiva
    for (int j = idx_restricoes; j < idx_folga; j++) {
        float entrada_c;
        cin >> entrada_c;
        tableau[0][j] = entrada_c * -1;
    }
    
    // receber vetor A (restricoes) (ele adiciona o vetor b logo ao lado pra depois mover ele)
    for (int i = 1; i < TABLEAU_N; i++) {
        for (int j = idx_restricoes; j < idx_folga + 1; j++) {
            float a;
            cin >> a;
            tableau[i][j] = a;
        }
    }
}

void montar_tableau (vector<vector<float>> &tableau) {
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

    // for (int j = idx_auxiliar; j < idx_b; j++) {
    //     tableau[0][j] = 1;
    // }
}

void guardar_fun_objetivo_orig (vector<float> &fun_objetivo_original, vector<vector<float>> tableau) {
    //guardando a objetivo original
    for (int j = 0; j < TABLEAU_M; j++) {
        fun_objetivo_original[j] = tableau[0][j];
    }
}

void guardar_fun_objetivo_aux (vector<float> &fun_objetivo_auxiliar) {
    //guardando a objetivo auxiliar
    for (int j = idx_auxiliar; j < idx_b; j++) {
        fun_objetivo_auxiliar[j] = 1;
    }
}

void tornar_b_positivo (vector<vector<float>> &tableau) {
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
}

// transforma o tableau de forma a usar apenas a função objetivo auxiliar
void usar_fun_obj_aux (vector<vector<float>> &tableau, vector<float> fun_objetivo_auxiliar) {
    for(int i = 0; i < TABLEAU_M; i++) {
        tableau[0][i] = fun_objetivo_auxiliar[i];
    }
}

// transforma o tableau de forma a usar apenas a função objetivo principal
void usar_fun_obj_principal (vector<vector<float>> &tableau, vector<float> fun_objetivo_original) {
    for(int i = 0; i < TABLEAU_M; i++) {
        tableau[0][i] = fun_objetivo_original[i];
    }

}



void pivotear_elemento (vector<vector<float>> &tableau, int lp, int cp) {
         //pega a linha do elemento a ser pivoteado
        vector<float> linha(TABLEAU_M);
        linha = tableau[lp];
        //para todas as linhas, menos a própria linha
        // o que essa função vai fazer é zerar um elemento da coluna de cada vez, MENOS o elemento pivô, então teremos dois for's pra fazer isso
        

        for (int i = 0; i < TABLEAU_N; i++) {
            //para cada linha que não seja a do pivô
            if (i == lp or tableau[i][cp] == 0)    
                continue;
            linha = tableau[lp];
            for (int j = 0; j < TABLEAU_M; j++) {
                //multiplicamos a linha do pivô pela entrada que queremos zerar negativa
                linha[j] *= tableau[i][cp] * (-1);
            }

            //somamos essa linha na linha da entrada que queremos zerar
            //transform (tableau[i].begin(), tableau[i].end(), linha.begin(), tableau[i].begin(), plus<int>());
            for (int j = 0; j < TABLEAU_M; j++) {
                tableau[i][j] += linha[j];
            }
            // cout << "zerando elemento da linha " << i << endl; 
            // teste_mostrar_tableau(tableau);
        }
}


void colocar_forma_canonica (vector<vector<float>> &tableau, vector<pair<int,int>> pivos) {
    vector<float> linha(TABLEAU_M, 0);
   
    // para cada pivo, fazemos:
    //cout << "pivos (forma canonica): " << endl;
    for (int i = 0; i < n; i++) {
        int lp = pivos[i].first;
        int cp = pivos[i].second;
        //cout << lp << " " << cp << endl;

        pivotear_elemento(tableau, lp, cp);
        // //colocamos o pivo como 1
        // for (int j = 0; j < TABLEAU_M; j++) {
        //     tableau[lp][j] = tableau[lp][j] / tableau[lp][cp];
        // } 
        // // se o pivo for negativo, coloca a linha positiva
        // if (tableau[lp][cp] < 0) {
        //     for (int j = 0; j < TABLEAU_M; j++) {
        //         tableau[lp][j] = tableau[lp][j] * (-1);
        //     }   
        // }


        // // // agora pega essa linha do pivo, multiplica por -1 e soma na funcao objetivo
        // vector<float> linha(TABLEAU_M);
        // linha = tableau[lp];
        // // // pega a linha do pivo, multiplica por -ci
        // for (int j = 0; j < TABLEAU_M; j++) {
        //     linha[j] *= tableau[0][cp] * (-1);
        // }
        // // // soma essa linha na funcao objetivo
    
        // for (int j = 0; j < TABLEAU_M; j++) {
        //     tableau[0][j] += linha[j];
        // }   
    }

}

void mostrar_solucao_viavel(vector<vector<float>> &tableau) {
    for (int j = idx_restricoes; j < idx_folga; j++) {
        if(tableau[0][j] == 0) {
            int count = TABLEAU_N - 1;
            int lp = 0;
            for (int i = 0; i < TABLEAU_N; i++){
                if (tableau[i][j] == 0)
                    count--;
                if (tableau[i][j] == 1)
                    lp = i;

            }
            if (count == 0 and lp != 0) {
                cout << tableau[lp][idx_b] << " ";
            } else {
                cout << "0" << " ";
            }
        } else {
            cout << "0" << " ";
        }
    }
    cout << endl;
}


pair<int, int> verificar_pivo (int i_min, int j_min, int ultima_col, vector<vector<float>> &tableau) {
    if (i_min == -1 and j_min == -1) {
        cout << "ilimitada" << endl;
        mostrar_solucao_viavel(tableau);
        //certificado de ilimitada
        // vou precisar do j da coluna que é ilimitada
        for (int j = idx_restricoes; j < idx_folga; j++) {
            if (j == ultima_col) {
                cout << "1" << " ";
            } else if (tableau[0][j] == 0) {
                int count = TABLEAU_N - 1;
                int lp = 0;
                for (int i = 0; i < TABLEAU_N; i++) {
                    if (tableau[i][j] == 0) 
                        count--;
                    if (tableau[i][j] == 1)
                        lp = i;
                }
                if (count == 0 and lp != 0)
                    cout << tableau[lp][ultima_col] * (-1) << " ";
                else
                    cout << "0" << " ";
            } else {
                cout << "0" << " ";
            }
        }
        cout << endl;


        //teste_mostrar_tableau(tableau);
        exit(0);
    }
   
    return {i_min, j_min};
     
}

pair<int, int> procurar_candidato_a_pivo(vector<vector<float>> &tableau, bool& tem_negativo) {
    //percorre toda a llinha de restricoes e acha um ci negativo
    int i_min = -2;
    int j_min = -2;
    float min = INF;
    for (int j = idx_restricoes; j < idx_b; j++) {   
        //se temos um ci negativo
        if (tableau[0][j] < 0) {
            i_min = -1;
            j_min = -1;
            // então percorra toda a coluna abaixo desse ci negativo minimizando bi/ai
            for (int i = 1; i < TABLEAU_N; i++) {
                // se temos uma entrada maior que zero na coluna de tal ci
                if(tableau[i][j] > 0) {
                    //escolhemos a menor razao
                    float razao = tableau[i][idx_b] / tableau[i][j];
                    if (razao < min) {
                        min = razao;
                        //o elemento que será nosso pivô será esse
                        i_min = i;
                        j_min = j;
                    }
                }
            }
            return verificar_pivo(i_min, j_min, j, tableau); //ja percorreu a coluna, então sai do loop, ou seja, para de procurar por outro ci negativo
        }
    }

    // se chegou no fim da coluna e não achou o menor, então é ilimitada

    tem_negativo = false;
    return {i_min, j_min};
}


void mostrar_certificado(vector<vector<float>> &tableau) {
    for (int j = 0; j < idx_restricoes; j++) {
        cout << tableau[0][j] << " ";
    }
    cout << endl;
}

void verificar_viabilidade (vector<vector<float>> &tableau) {
    if (tableau[0][idx_b] != (0)) {
        cout << "inviavel" << endl;
        mostrar_certificado(tableau);
        exit(0);
    }
}

void simplex_fase_2 (vector<vector<float>> &tableau, vector<pair<int, int>> &pivos) {
    bool tem_negativo = true;
    
    int idx_pivo = 0;
    while (tem_negativo) {
        
        pair<int, int> pivo = procurar_candidato_a_pivo(tableau, tem_negativo);

        int lp = pivo.first;
        int cp = pivo.second;

        if (tem_negativo == false or (lp == -1 and cp == -1))
            break;
        pivos[idx_pivo] = pivo;
        idx_pivo++;
        
        //cout << "pivoteando elemento " << lp << " " << cp << endl;
        

        // transforma o elemento pivô em 1
        if (tableau[lp][cp] != 1) {
            float fator = tableau[lp][cp];
            for (int i = 0; i < TABLEAU_M; i++) {
                tableau[lp][i] = tableau[lp][i] / fator;
                //cout << tableau[lp][i] << " ";
            }
            //cout << endl;
        }

        pivotear_elemento(tableau, lp, cp);

    }
    //iguala_a_zero(tableau);
    // teste_mostrar_tableau(tableau);
    verificar_viabilidade(tableau);
}

void simplex_fase_1 (vector<vector<float>> &tableau) {
    //faz o mesmo que o simplex fase 2, mas não pega o intervalo da auxiliar e deve tratar o b sozinho
    bool tem_negativo = true;
    

    while(tem_negativo) {
        //zera as entradas do vetor c da auxiliar para evitar que ela seja pivoteada
        for (int j = idx_auxiliar; j < idx_b; j++) {
            tableau[0][j] = 0;
        }
        pair<int, int> pivo = procurar_candidato_a_pivo(tableau, tem_negativo);
        
        int lp = pivo.first;
        int cp = pivo.second;

        if(tem_negativo == false or (lp == -1 and cp == -1))
            break;
        //cout << "pivoteando elemento (fase 1)" << lp << " " << cp << endl;

        // transforma o elemento pivô em 1
        if (tableau[lp][cp] != 1) {
            float fator = tableau[lp][cp];
            for (int i = 0; i < TABLEAU_M; i++) {
                tableau[lp][i] = tableau[lp][i] / fator;
            }
        }

        pivotear_elemento(tableau, lp, cp);
    }

}


bool tem_b_negativo (vector<vector<float>> &tableau) {
    // se tem b negativo, retorna true
    // se não tem b negativo, retorna false
    for (int i = 1; i < TABLEAU_N; i++) {
        if (tableau[i][idx_b] < 0)
            return true;
    }
    return false;
}

// void procura_degenerada_na_auxiliar () {
//     // se após o fim da auxiliar, restar alguma base nela e o valor objetivo for zero:
//     //      pivoteia alguma coluna de "A" que não seja base. Essa coluna terá como pivô um elemento que esteja
//     //      na mesma linha que a base da auxiliar
//     int count = n;
//     for (int j = idx_auxiliar; j < idx_b; j++) {
//         for (int i = 0; i < TABLEAU_N; i++) {

//         }
//     }
// }

void mostrar_resposta (vector<vector<float>> &tableau) {
    cout << "otima" << endl;
    //valor objetivo
    cout << tableau[0][idx_b] << endl;
    // uma solução que atinja o valor máximo
    mostrar_solucao_viavel(tableau);
    // certificado de otimalidade
    mostrar_certificado(tableau);

}

int main() {



    cin >> n >> m;

    TABLEAU_N = n + 1;
    TABLEAU_M = n + m + n + n + 1; //vero(n), restricoes(m), folga(n), identidade do auxiliar(n), b(1)

    idx_restricoes = n;
    idx_folga = n + m ;
    idx_auxiliar = n + m + n;
    idx_b = n + m + n + n;

    vector<vector<float>> tableau (TABLEAU_N, vector<float>(TABLEAU_M, 0));

    ler_entradas(tableau);

    montar_tableau(tableau);
    

    vector<float> fun_objetivo_original(TABLEAU_M);
    vector<float> fun_objetivo_auxiliar(TABLEAU_M, 0);
    vector<float> fun_objetivo_final(TABLEAU_M);

    guardar_fun_objetivo_orig(fun_objetivo_original, tableau);
    guardar_fun_objetivo_aux(fun_objetivo_auxiliar);




    vector<pair<int,int>> pivos(TABLEAU_N);
    for (int i = 1, j = idx_auxiliar, k = 0; j < idx_b; i++, j++, k++) {
        pair<int, int> pivo = {i, j};
        pivos[k] = pivo;
    }


    // teste_mostrar_tableau(tableau);
    if (tem_b_negativo(tableau)){
    // se existe algum b negativo:
        // multiplicamos a linha dele por -1

        tornar_b_positivo(tableau);
        //     trocamos a funcao objetivo pela auxiliar
        usar_fun_obj_aux(tableau, fun_objetivo_auxiliar);
        //     colocamos na forma canonica
        // teste_mostrar_tableau(tableau);
        colocar_forma_canonica(tableau, pivos);
        // teste_mostrar_tableau(tableau);
        //     aplicamos o simplex fase 2

        simplex_fase_2 (tableau, pivos);
        
        //procura_degenerada_na_auxiliar(tableau);
        //     voltamos a funcao objetivo original
        usar_fun_obj_principal(tableau, fun_objetivo_original);

        
        // // colocamos na forma canonica se baseando nas colunas do resultado da auxiliar
        colocar_forma_canonica(tableau, pivos);
        // //aplicamos o simplex
        simplex_fase_1 (tableau);

    
        
    } else {
        // se não existe b negativo
        //     aplicamos o simplex fase 1
        simplex_fase_1(tableau);
    }
    

 

    mostrar_resposta(tableau);
   //////////////////////TESTES////////////////////////
    // cout << endl << "Tableau de saída";
    // teste_mostrar_tableau(tableau);
    // cout << endl << "Função Objetivo Original";
    // teste_mostrar_fun_obj_orig(fun_objetivo_original);

    // cout << endl << "Função Objetivo Auxiliar";
    // teste_mostrar_fun_obj_aux(fun_objetivo_auxiliar);
    // cout << endl;
}

// passa em todos os testes do pdf do tp
// nos casos de teste extra passa em: 1, 6,7