#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#define MAX 10000

int count = 0;

/* Tenta encontrar o padrão fornecido na string de busca */
int bruteForce(char search[], char pattern[], int slen, int plen) {
    int i, j, k;
    
    for (i = 0; i <= slen - plen; i++) {
        count++;
        for (j = 0, k = i; (search[k] == pattern[j]) && (j < plen); j++, k++);
            if (j == plen)
                    return k-j+1;
    }
    
    return -1;
}

void main() {
    char search[MAX], pattern[MAX], temp[MAX], temp2[MAX], tempP[MAX], out[MAX];
    char *b;
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.html", "w");
    int pos;
    int control;
    char v;
    
    printf("%c", v);
    printf("Digite o texto no qual o padrão deve ser procurado:\n");
    
    fgets(pattern, MAX, stdin);
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    strcpy(tempP, pattern);
    strcat(tempP, "</mark>");
    pattern[strlen(pattern) - 1] = '\0';
    
    int leng = strlen(pattern);
    int slen, plen, sum, i, g;
    
    while (!feof(input)) {
        if (fgets(search, MAX, input) != NULL) {
            search[strlen(search) - 1] = '\0';
            slen = strlen(search);
            plen = strlen(pattern);
            pos = bruteForce(search, pattern, slen, plen);
            sum += count;
            
            if (pos >= 0) {
                printf("O padrão desejado foi encontrado a partir da linha %d na posição %d\n", g + 1, pos);
                control++;
            }
            
            if (pos != -1) {
                strncpy(temp, search, pos);
                temp[pos] = '\0';
                strcat(temp, "<mark>");
                strcat(temp, tempP);
                for (i = 0; i < MAX; i++) {									
                    if ((pos + leng + i) < strlen(search))
                        temp2[i] = search[pos + leng + i];
                    else
                        temp2[i] = v;
                }               
                strcat(temp, temp2);
                fprintf(output, "%s\n", temp);
            }
            else
                fprintf(output, "%s\n", search);		
        }        
        g++;
    }
    
    if (control == 0) {
        printf("\nO padrão não foi encontrado no texto fornecido\n\n");
    }
    
    gettimeofday(&tv2, NULL);
    printf("Tempo total = %f segundos\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
    
    fclose(input);
    fclose(output);
    
    printf("O número total de comparações de caracteres: %d\n", sum);
}
