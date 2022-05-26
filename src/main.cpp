#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

const int INF = 0x3f3f3f3f;
int n, m;

int TABLEAU_N;
int TABLEAU_M; //vero, restricoes, folga, identidade do auxiliar, b

// vetores que indicam em que coluna começa cada parte do tableux
int idx_restricoes;
int idx_folga;
int idx_auxiliar;
int idx_b;
/////////////////////////////TESTES///////////////////////////////////
void teste_mostrar_tableau (vector<vector<float>> tableau) {
    cout << endl;
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

    for (int j = idx_auxiliar; j < idx_b; j++) {
        tableau[0][j] = 1;
    }
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

void condicoes_pre_simplex (vector<vector<float>> &tableau) {
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

void colocar_forma_canonica (vector<vector<float>> &tableau, vector<pair<int,int>> pivos) {
    vector<float> linha(TABLEAU_M, 0);
    int fatores[n];
    // para cada coluna da base, temos um fator correspondente que é -ci
    for (int i = 0; i < n; i++) {
        int j = pivos[i].second;
        fatores[i] = tableau[0][j] * (-1);
    }

    
    // para cada pivo, fazemos:
    for (int i = 0; i < n; i++) {
        int lp = pivos[i].first;
        int cp = pivos[i].second;
        cout << lp << " " << cp << endl;

        //colocamos o pivo como 1
        for (int j = 0; j < TABLEAU_M; j++) {
            tableau[lp][j] = tableau[lp][j] / tableau[lp][cp];
        } 
        // se o pivo for negativo, coloca a linha positiva
        if (tableau[lp][cp] < 0) {
            for (int j = 0; j < TABLEAU_M; j++) {
                tableau[lp][j] = tableau[lp][j] * (-1);
            }   
        }


        // // agora pega essa linha do pivo, multiplica por -1 e soma na funcao objetivo
        vector<float> linha(TABLEAU_M);
        linha = tableau[lp];
        // // pega a linha do pivo, multiplica por -ci
        for (int j = 0; j < TABLEAU_M; j++) {
            linha[j] *= tableau[0][cp] * (-1);
        }
        // // soma essa linha na funcao objetivo
    
        for (int j = 0; j < TABLEAU_M; j++) {
            tableau[0][j] += linha[j];
        }

        
    }
 



    // for (int i = 1; i < TABLEAU_N; i++) {
    //     for (int j = idx_restricoes; j < idx_b; j++) {

    //     }
    // }


}

int main() {



    cin >> n >> m;

    TABLEAU_N = n + 1;
    TABLEAU_M = n + m + n + n + 1;

    idx_restricoes = n;
    idx_folga = n + m ;
    idx_auxiliar = n + m + n;
    idx_b = n + m + n + n;

    vector<vector<float>> tableau (TABLEAU_N, vector<float>(TABLEAU_M, 0));

    ler_entradas(tableau);

    montar_tableau(tableau);
    

    vector<float> fun_objetivo_original(TABLEAU_M);
    vector<float> fun_objetivo_auxiliar(TABLEAU_M, 0);


    guardar_fun_objetivo_orig(fun_objetivo_original, tableau);
    guardar_fun_objetivo_aux(fun_objetivo_auxiliar);


    condicoes_pre_simplex(tableau);

    usar_fun_obj_aux(tableau, fun_objetivo_auxiliar);


    vector<pair<int,int>> pivos(TABLEAU_N);
    for (int i = 1, j = idx_auxiliar, k = 0; j < idx_b; i++, j++, k++) {
        pair<int, int> pivo = {i, j};
        pivos[k] = pivo;
    }

    colocar_forma_canonica(tableau, pivos);

    //na hora de pivotear, eu marco os indices do pivo (bases) no vetor de pivos






    /*


    andamos pelo vetor c^T
    se acharmos uma entrada negativa:
        andamos pela coluna
        procuramos na coluna o elemento que minimiza bi/ai, tal que ci > 0
        se a coluna só tem elementos <= 0, PL ilimitada
        gera o certificado
        pivoteia o elemento que minimiza a razão acima

    */
   



   //inicia o simplex auxiliar
   //para cada coluna de restrição


    


    // colocando na forma canonica
    //soma todas as linhas começando da segunda, multiplica por -1 e soma na primeira
    
    //



    //simplex resolvendo a auxiliar (simplex fase 2)
    
  

    // escolhendo quem vai pivotear
    


    //COMENTE O CODIGO ABAIXO PARA PRINTAR A PRINCIPAL. DESCOMENTE PRA PRINTAR A AUXILIAR.
    // for(int i = 0; i < TABLEAU_M; i++) {
    //     tableau[0][i] = objetivo_auxiliar[i];
    // }


    /////////////////////////////////////////////////////////////////////////////////////



    cout << endl << "::::::::::::::PRINT DE TESTES:::::::::::::::" << endl;
    teste_mostrar_tableau(tableau);
    teste_mostrar_fun_obj_orig(fun_objetivo_original);
    teste_mostrar_fun_obj_aux(fun_objetivo_auxiliar);


    // cout << endl << ":::::::::::::::linha do pivo:::::::::::::" << endl;
    // for (int i = 0; i < TABLEAU_M; i++) {
    //     cout << linha[i] << " ";
    // }
}