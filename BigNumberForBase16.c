#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int sign;   // 1 pour positif, -1 pour négatif
    int size;   // Taille du tableau
    int* tab;   // Tableau de chiffres
} bignum;

bignum* create_bignum(int size) {
    bignum* num = malloc(sizeof(bignum));
    num->sign = 1; // Positif par défaut
    num->size = size;
    num->tab = calloc(size, sizeof(int)); // Initialisation à zéro
    return num;
}

char* decimalToBase16(char* decStr) {
    int len = strlen(decStr);
    int hexSize = (len * 4 + 3) / 4; // Taille approximative pour le stockage hexadécimal
    char* hexStr = malloc(hexSize + 1);
    char* tempStr = malloc(len + 1);
    strcpy(tempStr, decStr);

    int carry;
    int tempLen = len;
    int hexIndex = hexSize;

    hexStr[hexIndex--] = '\0';

    while (tempLen > 0) {
        carry = 0;
        for (int i = 0; i < tempLen; ++i) {
            int num = carry * 10 + (tempStr[i] - '0');
            tempStr[i] = (num / 16) + '0';
            carry = num % 16;
        }
        hexStr[hexIndex--] = (carry < 10) ? carry + '0' : carry - 10 + 'A';

        while (tempLen > 0 && tempStr[0] == '0') {
            ++tempStr;
            --tempLen;
        }
    }

    free(tempStr - len);

    while (hexIndex >= 0) {
        hexStr[hexIndex--] = '0';
    }

    return hexStr;
}

bignum* str2bignum(char* str) {
    int len = strlen(str);
    int size = (len + 3) / 4; // Chaque chiffre en base 2^4 (base 16) peut représenter 4 bits

    bignum* num = create_bignum(size);

    int index = 0;
    for (int i = len - 1; i >= 0; i -= 4) {
        int end = i;
        int start = (i - 3 >= 0) ? i - 3 : 0;

        char chunk[5] = { 0 }; // Initialiser un tableau de 5 caractères pour stocker la sous-chaîne
        strncpy(chunk, &str[start], end - start + 1); // Copier la sous-chaîne
        num->tab[index++] = strtoul(chunk, NULL, 16); // Convertir en entier en base 16 et stocker dans le tableau
    }

    return num;
}

bignum* add_bignum(bignum* a, bignum* b) {
    int maxSize = (a->size > b->size) ? a->size : b->size;
    bignum* result = create_bignum(maxSize + 1); // +1 pour la retenue éventuelle

    int carry = 0;
    for (int i = 0; i < maxSize; i++) {
        int sum = carry;
        if (i < a->size) sum += a->tab[i];
        if (i < b->size) sum += b->tab[i];

        result->tab[i] = sum % 65536; // Base 16^4 = 65536
        carry = sum / 65536;
    }
    result->tab[maxSize] = carry;

    return result;
}

void print_bignum(bignum* num) {
    if (num->sign < 0) printf("-");
    int start = num->size - 1;
    while (start > 0 && num->tab[start] == 0) start--; // Ignorer les zéros initiaux

    for (int i = start; i >= 0; i--) {
        if (i != start) printf("%04X", num->tab[i]); // Afficher avec des zéros initiaux si nécessaire
        else printf("%X", num->tab[i]); // Afficher le premier chiffre sans zéros initiaux
    }
    printf("\n");
}

int main() {
    char decStr1[1024];
    char decStr2[1024];

    printf("Veuillez entrer le premier nombre en base 10: ");
    scanf("%1023s", decStr1);
    printf("Veuillez entrer le deuxième nombre en base 10: ");
    scanf("%1023s", decStr2);

    char* hexStr1 = decimalToBase16(decStr1);
    char* hexStr2 = decimalToBase16(decStr2);

    if (hexStr1 == NULL || hexStr2 == NULL) {
        fprintf(stderr, "Erreur lors de la conversion des nombres en base 16.\n");
        return 1;
    }

    printf("Hexadecimal 1: %s\n", hexStr1);
    printf("Hexadecimal 2: %s\n", hexStr2);

    bignum* num1 = str2bignum(hexStr1);
    bignum* num2 = str2bignum(hexStr2);

    bignum* result = add_bignum(num1, num2);

    printf("Num1: ");
    print_bignum(num1);
    printf("Num2: ");
    print_bignum(num2);
    printf("Sum:  ");
    print_bignum(result);

    free(hexStr1);
    free(hexStr2);
    free(num1->tab);
    free(num2->tab);
    free(result->tab);
    free(num1);
    free(num2);
    free(result);

    return 0;
}
