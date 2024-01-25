#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>
#define NO_OF_CHARS 256
#define MAX 10000

int sum = 0;
int control = 0;

// Função utilitária para obter o máximo entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função de pré-processamento para a heurística do caractere ruim do Boyer Moore
void badCharHeuristic(char *str, int size, int badchar[NO_OF_CHARS]) {
    int i;

    // Inicializa todas as ocorrências como -1
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    // Preenche o valor real da última ocorrência de um caractere
    for (i = 0; i < size; i++)
        badchar[(int)str[i]] = i;
}

/* Função de busca de padrões que utiliza a heurística do caractere ruim do algoritmo de Boyer Moore */
void search(char *txt, char *pat, int k) {
    int m = strlen(pat);
    int n = strlen(txt);
    int badchar[NO_OF_CHARS], i, count;

    /* Preenche o array de caracteres ruins chamando a função de pré-processamento badCharHeuristic() para o padrão dado */
    badCharHeuristic(pat, m, badchar);

    int s = 0; // s é o deslocamento do padrão em relação ao texto
#pragma omp parallel for private(i, count) shared(s) // Adicionando paralelismo
    for (s = 0; s <= (n - m); s++) {
        int j = m - 1;

        /* Continue reduzindo o índice j do padrão enquanto os caracteres do padrão e do texto correspondem a este deslocamento s */
        while (j >= 0 && pat[j] == txt[s + j]) {
            count++;
            j--;
        }

        /* Se o padrão estiver presente no deslocamento atual, então o índice j se tornará -1 após o loop acima */
        if (j < 0) {
            // printf("O padrão ocorre no deslocamento = %d\n", s);

            /* Desloca o padrão para que o próximo caractere no texto
               alinhe-se com a última ocorrência no padrão.
               A condição s+m < n é necessária para o caso em que
               o padrão ocorre no final do texto */
            printf("O padrão desejado foi encontrado a partir da linha %d na posição %d\n", k, s + 1);
            control++;
        }
    }
    sum += count;
}

/* Programa principal para testar a função acima */
int main() {
    char txt[MAX], p[MAX];
    int k = 1;
    FILE *input = fopen("input.txt", "r");

    printf("Digite o texto no qual o padrão deve ser procurado:\n");
    fgets(p, MAX, stdin);
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    p[strlen(p) - 1] = '\0';

    while (!feof(input)) {
        if (fgets(txt, MAX, input) != NULL) {
            txt[strlen(txt) - 1] = '\0';
            search(txt, p, k);
        }
        k++;
    }

    if (control == 0) {
        printf("\nO padrão não foi encontrado no texto fornecido\n\n");
    }

    gettimeofday(&tv2, NULL);
    printf("Tempo total = %f segundos\n", (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec));

    fclose(input);

    printf("O número de comparações de caracteres: %d\n", sum);

    return 0;
}
