#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

// --------------------------------------DEBUT CODE POUR CONVERTIR BASE DECIMAL VERS BASE 2-----------------------------------------------------

// Fonction pour convertir une chaîne de caractères décimale en binaire
char* decimalToBase2(char* decStr) {
    int len = strlen(decStr);
    int binSize = len * 4 + 1; // Taille approximative pour le stockage binaire (chaque chiffre décimal peut représenter jusqu'à 4 bits)
    char* binStr = malloc(binSize + 1);
    char* tempStr = malloc(len + 1);
    strcpy(tempStr, decStr);

    int carry;
    int tempLen = len;
    int binIndex = binSize;

    binStr[binIndex--] = '\0';

    while (tempLen > 0) {
        carry = 0;
        for (int i = 0; i < tempLen; ++i) {
            int num = carry * 10 + (tempStr[i] - '0');
            tempStr[i] = (num / 2) + '0';
            carry = num % 2;
        }
        binStr[binIndex--] = carry + '0';

        while (tempLen > 0 && tempStr[0] == '0') {
            ++tempStr;
            --tempLen;
        }
    }

    free(tempStr - len);

    while (binIndex >= 0) {
        binStr[binIndex--] = '0';
    }

    // Retirer les zéros en tête
    char* first_one = strchr(binStr, '1');
    if (first_one) {
        char* binTrimmed = strdup(first_one);
        free(binStr);
        return binTrimmed;
    }
    else {
        return binStr; // retourne "0" s'il n'y a pas de '1'
    }
}
// --------------------------------------FIN CODE POUR CONVERTIR BASE DECIMAL VERS BASE 2-----------------------------------------------------


// --------------------------------------DEBUT CODE POUR CONVERTIR BASE 2 VERS BASE DECIMAL-----------------------------------------------------

// Fonction pour multiplier une chaîne décimale par 2
void multiplyByTwo(char* number) {
    int carry = 0;
    int len = strlen(number);
    for (int i = len - 1; i >= 0; i--) {
        int digit = (number[i] - '0') * 2 + carry;
        carry = digit / 10;
        number[i] = (digit % 10) + '0';
    }
    if (carry > 0) {
        memmove(number + 1, number, len + 1);
        number[0] = carry + '0';
    }
}

// Fonction pour ajouter 1 à une chaîne décimale
void addOne(char* number) {
    int carry = 1;
    int len = strlen(number);
    for (int i = len - 1; i >= 0 && carry > 0; i--) {
        int digit = (number[i] - '0') + carry;
        carry = digit / 10;
        number[i] = (digit % 10) + '0';
    }
    if (carry > 0) {
        memmove(number + 1, number, len + 1);
        number[0] = carry + '0';
    }
}

// Fonction principale pour convertir une chaîne binaire en une chaîne décimale
char* base2ToDecimal(const char* binaryString) {
    int len = strlen(binaryString);
    char* decimalString = (char*)malloc((len + 1) * sizeof(char));
    strcpy(decimalString, "0");

    for (int i = 0; i < len; i++) {
        multiplyByTwo(decimalString);
        if (binaryString[i] == '1') {
            addOne(decimalString);
        }
    }

    return decimalString;
}
// --------------------------------------FIN CODE POUR CONVERTIR BASE 2 VERS BASE DECIMAL-----------------------------------------------------


// --------------------------------------DEBUT CODE POUR CONVERTIR BASE DECIMAL EN BASE 16-----------------------------------------------------
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
// --------------------------------------FIN CODE POUR CONVERTIR BASE DECIMAL EN BASE 16-----------------------------------------------------

// --------------------------------------DEBUT CODE POUR CONVERTIR BASE 16 EN BASE DECIMAL-----------------------------------------------------

// Fonction pour multiplier une chaîne décimale par une valeur (base)
void multiplyByBase(char* number, int base) {
    int carry = 0;
    int len = strlen(number);
    for (int i = len - 1; i >= 0; i--) {
        int digit = (number[i] - '0') * base + carry;
        carry = digit / 10;
        number[i] = (digit % 10) + '0';
    }
    while (carry > 0) {
        memmove(number + 1, number, len + 1);
        number[0] = (carry % 10) + '0';
        carry /= 10;
        len++;
    }
}

// Fonction pour ajouter une valeur (0-15) à une chaîne décimale
void addValue(char* number, int value) {
    int carry = value;
    int len = strlen(number);
    for (int i = len - 1; i >= 0 && carry > 0; i--) {
        int digit = (number[i] - '0') + carry;
        carry = digit / 10;
        number[i] = (digit % 10) + '0';
    }
    while (carry > 0) {
        memmove(number + 1, number, len + 1);
        number[0] = (carry % 10) + '0';
        carry /= 10;
        len++;
    }
}

// Fonction pour convertir un caractère hexadécimal en entier
int hexCharToInt(char c) {
    if (isdigit(c)) return c - '0';
    if (isxdigit(c)) return toupper(c) - 'A' + 10;
    return 0; // En cas d'entrée non valide, on retourne 0 (devrait être géré par ailleurs)
}

// Fonction principale pour convertir une chaîne hexadécimale en une chaîne décimale
char* hexStringToDecimalString(const char* hexString) {
    int len = strlen(hexString);
    int maxDecimalLen = len * 4 + 1; // Allocation large pour tenir compte des grands nombres
    char* decimalString = (char*)malloc(maxDecimalLen * sizeof(char));
    if (!decimalString) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    memset(decimalString, '0', maxDecimalLen);
    decimalString[maxDecimalLen - 1] = '\0';

    for (int i = 0; i < len; i++) {
        multiplyByBase(decimalString, 16);
        addValue(decimalString, hexCharToInt(hexString[i]));
    }

    // Supprimer les zéros initiaux
    char* nonZeroPtr = decimalString;
    while (*nonZeroPtr == '0' && *(nonZeroPtr + 1) != '\0') {
        nonZeroPtr++;
    }

    char* result = strdup(nonZeroPtr);
    free(decimalString);
    return result;
}

// --------------------------------------FIN CODE POUR CONVERTIR BASE 16 EN BASE DECIMAL-----------------------------------------------------

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

char* bignum2str(bignum* num) {
    int len = num->size * 4;
    char* str = malloc(len + 1);
    str[0] = '\0';

    for (int i = num->size - 1; i >= 0; i--) {
        char chunk[5];
        snprintf(chunk, 5, "%04X", num->tab[i]);
        strcat(str, chunk);
    }

    // Retirer les zéros en tête
    char* first_non_zero = str;
    while (*first_non_zero == '0' && *(first_non_zero + 1) != '\0') {
        first_non_zero++;
    }

    char* result = strdup(first_non_zero);
    free(str);
    return result;
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

bignum* sub_bignum(bignum* a, bignum* b) {
    // Assurez-vous que a >= b pour simplifier l'implémentation
    int maxSize = a->size;
    bignum* result = create_bignum(maxSize);

    int borrow = 0;
    for (int i = 0; i < maxSize; i++) {
        int subtrahend = (i < b->size) ? b->tab[i] : 0;
        int diff = a->tab[i] - subtrahend - borrow;
        if (diff < 0) {
            diff += 65536; // Base 16^4 = 65536
            borrow = 1;
        }
        else {
            borrow = 0;
        }
        result->tab[i] = diff;
    }

    // Ajuster la taille du résultat
    while (result->size > 1 && result->tab[result->size - 1] == 0) {
        result->size--;
    }

    return result;
}

bignum* mult_bignum(bignum* a, bignum* b) {
    int size = a->size + b->size;
    bignum* result = create_bignum(size);

    for (int i = 0; i < a->size; i++) {
        unsigned long long carry = 0;
        for (int j = 0; j < b->size; j++) {
            unsigned long long product = (unsigned long long)a->tab[i] * b->tab[j] + result->tab[i + j] + carry;
            result->tab[i + j] = product % 65536;
            carry = product / 65536;
        }
        result->tab[i + b->size] = carry;
    }

    // Ajuster la taille du résultat
    while (result->size > 1 && result->tab[result->size - 1] == 0) {
        result->size--;
    }

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
    printf("--- POUR SIMPLIFIER LES TESTS, ASSUREZ-VOUS QUE a>b --- \n\n");
    printf("Veuillez entrer le premier nombre en base 10: ");
    scanf("%1023s", decStr1);
    printf("Veuillez entrer le deuxième nombre en base 10: ");
    scanf("%1023s", decStr2);


    printf("\n-------------- TEST CONVERSION EN HEXADECIMAL --------------\n");
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
    bignum* result_sub = sub_bignum(num1, num2);
    bignum* result_mult = mult_bignum(num1, num2);

    printf("\nNum1: ");
    print_bignum(num1);
    printf("Num2: ");
    print_bignum(num2);

    printf("\n-------------- TEST ADDITION --------------\n");

    printf("\nSum:  ");
    print_bignum(result);

    printf("\n-------------- TEST SOUSTRACTION --------------\n");

    printf("\nDifference: ");
    print_bignum(result_sub);

    printf("\n-------------- TEST MULTIPLICATION --------------\n");

    printf("\nProduct: ");
    print_bignum(result_mult);


    printf("\n-------------- TEST CONVERSION BIGNUM --> CHAINE DE CARACTERE --------------\n");

    // Test de la conversion bignum -> chaîne de caractères
    char* str_from_bignum1 = bignum2str(num1);
    char* str_from_bignum2 = bignum2str(num2);
    printf("\nNum1 en chaine de caracteres: %s\n", str_from_bignum1);
    printf("Num2 en chaine de caracteres: %s\n", str_from_bignum2);

    printf("\n-------------- TEST CONVERSION HEXADECIMAL --> DECIMAL --------------\n");
    char* decimal_str_from_hexa1 = hexStringToDecimalString(str_from_bignum1);
    char* decimal_str_from_hexa2 = hexStringToDecimalString(str_from_bignum2);

    printf("\n Retour en version decimal pour num1: %s\n", decimal_str_from_hexa1);
    printf("Retour en version decimal pour num2: %s\n", decimal_str_from_hexa2);

    printf("\n-------------- TEST CONVERSION DECIMAL --> BINAIRE --------------\n");
    char* binary_str_from_decimal1 = decimalToBase2(decimal_str_from_hexa1);
    char* binary_str_from_decimal2 = decimalToBase2(decimal_str_from_hexa2);

    printf("\n Conversion en base binaire pour num1: %1023s\n", binary_str_from_decimal1);
    printf("Conversion en base binaire pour num2: %1023s\n", binary_str_from_decimal2);

    printf("\n-------------- TEST CONVERSION BINAIRE --> DECIMAL --------------\n");
    char* decimal_str_from_binary1 = base2ToDecimal(binary_str_from_decimal1);
    char* decimal_str_from_binary2 = base2ToDecimal(binary_str_from_decimal2);

    printf("\n Retour en base decimal pour num1: %1023s\n", decimal_str_from_binary1);
    printf("Retour en base decimal pour num2: %1023s\n", decimal_str_from_binary2);

    printf("\n-------------- TEST CONVERSION BINAIRE --> BIGNUM--------------\n");
    bignum* binary_num1 = str2bignum(binary_str_from_decimal1);
    bignum* binary_num2 = str2bignum(binary_str_from_decimal2);

    print_bignum(binary_num1);
    print_bignum(binary_num2);

    // Libérer la mémoire allouée dynamiquement
    free(hexStr1);
    free(hexStr2);
    free(str_from_bignum1);
    free(str_from_bignum2);
    free(num1->tab);
    free(num2->tab);
    free(result->tab);
    free(result_sub->tab);
    free(result_mult->tab);
    free(num1);
    free(num2);
    free(result);
    free(result_sub);
    free(result_mult);
    free(decimal_str_from_hexa1);
    free(decimal_str_from_hexa2);
    free(binary_str_from_decimal1);
    free(binary_str_from_decimal2);
    free(decimal_str_from_binary1);
    free(decimal_str_from_binary2);

    return 0;
}
