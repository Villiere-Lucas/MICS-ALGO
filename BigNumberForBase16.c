#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Définition de la structure bignum
typedef struct {
    int sign;   // 1 pour positif, -1 pour négatif
    int size;   // Taille du tableau
    int* tab;   // Tableau de chiffres
} bignum;

// Définition de la structure pour le résultat de la division
typedef struct {
    bignum* q;
    bignum* r;
} division_result;

// Déclaration des fonctions
bignum* create_bignum(int size);
bignum* str2bignum(char* str);
char* bignum2str(bignum* num);
bignum* add_bignum(bignum* a, bignum* b);
bignum* sub_bignum(bignum* a, bignum* b);
bignum* mult_bignum(bignum* a, bignum* b);
division_result binary_euclidean_division(bignum* a, bignum* b);
bool compare_bignum(bignum* a, bignum* b);
bignum* divide_by_2(bignum* num);
void print_bignum(bignum* num);
char* decimalToBase2(char* decStr);
char* base2ToDecimal(const char* binaryString);
char* decimalToBase16(char* decStr);
char* hexStringToDecimalString(const char* hexString);
division_result remainderbignum(bignum* a, bignum* b);

// Fonction pour créer un bignum
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

// -------------------------- DIVISION REMAINDER -----------------------

division_result remainderbignum(bignum* a, bignum* b) {
    // Mettre les bignums en string.
    char* hexStringa = bignum2str(a);
    char* hexStringb = bignum2str(b);

    // Puis mettre en décimale:
    char* decStringa = hexStringToDecimalString(hexStringa);
    char* decStringb = hexStringToDecimalString(hexStringb);

    // Puis mettre en binaire :
    char* binStringa = decimalToBase2(decStringa);
    char* binStringb = decimalToBase2(decStringb);

    // Et enfin mettre la valeur binaire en bignum:
    bignum* binaryA = str2bignum(binStringa);
    bignum* binaryB = str2bignum(binStringb);

    // Maintenant, on peut faire la division euclidienne binaire:
    division_result divisionRESULT = binary_euclidean_division(binaryA, binaryB);

    bignum* q_binary = divisionRESULT.q;
    bignum* r_binary = divisionRESULT.r;

    printf("\n\n -- RESULTAT DU BINARY EUCLIDEAN ALGORITHM -- \n\n");
    printf("QUOTIENT (format binaire puisque le binary euclidean algorithm retourne un résultat binaire):\n");
    print_bignum(q_binary);
    printf("\nREMAINDER (format binaire puisque le binary euclidean algorithm retourne un résultat binaire):\n");
    print_bignum(r_binary);

    // Chemin inverse, il va falloir remettre la valeur binaire en hexadecimale ET bignum.
    // Donc déjà, on la refout en string
    char* quotientBinaryString = bignum2str(q_binary);
    char* remainderBinaryString = bignum2str(r_binary);

    // Puis en décimale :
    char* quotientDecimalString = base2ToDecimal(quotientBinaryString);
    char* remainderDecimalString = base2ToDecimal(remainderBinaryString);

    printf("\n\nQUOTIENT (format decimale):\n");
    printf("%s", quotientDecimalString);
    printf("\nREMAINDER (format decimale):\n");
    printf("%s", remainderDecimalString);

    // Puis en hexadecimal :
    char* quotientHexString = decimalToBase16(quotientDecimalString);
    char* remainderHexString = decimalToBase16(remainderDecimalString);

    // Et enfin, en bignum
    bignum* q = str2bignum(quotientHexString);
    bignum* r = str2bignum(remainderHexString);

    division_result finalRESULT;
    finalRESULT.q = q;
    finalRESULT.r = r;
    return finalRESULT;
}

// Utile pour le binary euclidean algorithm
#define MAX(a, b) ((a) > (b) ? (a) : (b))

division_result binary_euclidean_division(bignum* a, bignum* b) {
    // A SUPPRIMER C'EST JUSTE POUR TESTER UN PRINT
    int iter = 0;

    printf("\nVoici le bignum a en binaire:\n");
    print_bignum(a);
    printf("\nVoici le bignum b en binaire:\n");
    print_bignum(b);

    // --- INITIALISATION DE Q ---
    char* zero = "0";
    bignum* q = str2bignum(zero);
    printf("\nVerification de la valeur de q (qui ici devrait etre a 0):\n");
    print_bignum(q);

    // --- INITIALISATION DE R ---
    bignum* r = a;
    printf("\nVerification de la valeur de r (qui ici devrait etre egale a 'a'):\n");
    print_bignum(r);

    // Maintenant on fait c
    // Pour calculer la taille k et l, je pense qu'il faut calculer la taille en BINAIRE !
    char* a_string_binary = bignum2str(a);
    int k = strlen(a_string_binary);
    printf("\nLa taille du bignum a est : %d", k);

    char* b_string_binary = bignum2str(b);
    int l = strlen(b_string_binary);
    printf("\nLa taille du bignum b est : %d", l);

    int k_sub_l = k - l;
    printf("\nk-l = %d", k_sub_l);
    int maximum = MAX(0, k_sub_l);
    printf("\nLe maximum entre 0 et k-l est : %d", maximum);

    // Un peu chiant mais il faut remettre b en décimale pour le multiplier avec 2^max pour avoir la vraie valeur de c. Sinon ca multiplie la représentation binaire de b avec un nombre décimal et forcément ca bug.
    //Donc b actuellement en binaire doit retourner en décimale.
    // B : Bignum to String
    char* temp_b = bignum2str(b);
    // B : String binaire to decimal
    char* temp_decimal_b = base2ToDecimal(temp_b);

    char* temp_hexadecimal_b = decimalToBase16(temp_decimal_b);

    bignum* temp_hex_bignum_b = str2bignum(temp_hexadecimal_b);


    // B : Transformation en bignum
    bignum* temp_b_bignum = str2bignum(temp_decimal_b);
    printf("\nVoici la version decimale de b pour la multiplication avec 2^max()\n");
    print_bignum(temp_b_bignum);

    printf("\n Et voici sa representation hexadecimale pour la multiplication : \n");
    print_bignum(temp_hex_bignum_b);

    int two_power_max = (1 << maximum);
    // On transforme en char*, pour après le mettre en bignum
    char two_pow_max[50];
    sprintf(two_pow_max, "%d", two_power_max);
    printf("\nLa valeur de 2 puissance max(0, k-l) = %s", two_pow_max);
    // Transformation en hexadecimal pour le calcul
    char* hex_two_pow_max_string = decimalToBase16(two_pow_max);
    // Transformation en bignum
    bignum* two_power_maximum = str2bignum(hex_two_pow_max_string);
    printf("\nEn version bignum on a : \n");
    print_bignum(two_power_maximum);

    // On créer la valeur finale de c avec la fonction mult() :
    bignum* c_value = mult_bignum(two_power_maximum, temp_hex_bignum_b);
    // Ne pas oublier que le résultat est à la base 2^4 soit 16 soit héxadécimale.
    char* c_hexValue = bignum2str(c_value);
    char* c_decValue = hexStringToDecimalString(c_hexValue);
    printf("\nEt donc la valeur de c serait le resultat d'au dessus FOIS b: %s", c_decValue);

    // On le met en base binaire
    char* c_binary_string = decimalToBase2(c_decValue);
    // Et on le convertit en bignum. Et on a enfin notre valeur de c fonctionnelle.
    bignum* c = str2bignum(c_binary_string);

    printf("\nOn peut verifier si c'est bien sa version binaire :\n");
    print_bignum(c);

    printf("On a donc :\n");
    printf("- Q = \n");
    print_bignum(q);
    printf("\n- R = \n");
    print_bignum(r);
    printf("\n- C = \n");
    print_bignum(c);

    printf("\n\n ------ JUSQUE LA, TOUT EST BON ! LES VALEURS SONT TOUTES BONNES ET PRETE A L'EMPLOI ------ \n\n");
    // Maintenant, toutes nos variables, q, r, c, a et b sont prêtes. On peut faire la boucle.
    // 1ère étape : q <- 2*q
    char* two_string = "2";
    bignum* two = str2bignum(two_string);

    char* one_string = "1";
    bignum* one = str2bignum(one_string);

    // Je pense que y'a un problème avec l'attribution du x2 et du +1.
    bignum* q_temp;

    for (int i = 0; i <= maximum; i++)
    {
        printf("\nIteration numero %d\n", i);

        q = mult_bignum(two, q);
        printf("\nResultat de q*2 : --- FONCTIONNE --- \n");
        print_bignum(q);


        // 2ème étape : if r>= c then...
        if (compare_bignum(r, c) == true)
        {
            // 3ème étape :r <- r-c avec la fonction sub(). NOTE: Encore et toujours cette ptn de conversion. Pensez à faire des fonctions qui résume ces lignes pour faire un seul call parce que c'est redondant.
            char* temp_bin_string_r = bignum2str(r);
            char* temp_dec_string_r = base2ToDecimal(temp_bin_string_r);
            char* temp_hex_string_r = decimalToBase16(temp_dec_string_r);
            bignum* temp_r = str2bignum(temp_hex_string_r);

            char* temp_bin_string_c = bignum2str(c);
            char* temp_dec_string_c = base2ToDecimal(temp_bin_string_c);
            char* temp_hex_string_c = decimalToBase16(temp_dec_string_c);
            bignum* temp_c = str2bignum(temp_hex_string_c);

            // RESULTAT EN HEXADECIMAL
            r = sub_bignum(temp_r, temp_c);

            // A ton avis ? On reconvertit en binaire...
            char* hex_string_r = bignum2str(r);
            char* dec_string_r = hexStringToDecimalString(hex_string_r);
            char* bin_string_r = decimalToBase2(dec_string_r);
            r = str2bignum(bin_string_r);

            printf("\nLe resultat de r - c = --- FONCTIONNE ---\n");
            print_bignum(r);


            //4ème étape : q<- q+1 avec la fonction add.
            // (création du bignum pour "1")
            q = add_bignum(q, one);
            printf("\nLe resultat de q +1 = --- FONCTIONNE --- \n");
            print_bignum(q);
        }
        //5ème étape : c <- c/2
        printf("\nAppel numero %d\n", iter);
        iter++;
        printf("\nVoici la valeur de C:\n");
        print_bignum(c);
        c = divide_by_2(c);
        printf("\nLe resultat de c/2 = --- FONCTIONNE ---\n");
        print_bignum(c);
    }

    char* hex_q_string = bignum2str(q);
    char* dec_q_string = hexStringToDecimalString(hex_q_string);
    char* binary_q_string = decimalToBase2(dec_q_string);
    bignum* q_bin = str2bignum(binary_q_string);

    division_result divisionRESULT;
    divisionRESULT.q = q_bin; // Il est au format Hexadecimal
    divisionRESULT.r = r; // Il est au format binaire

    // Retourne au format binaire le quotient et le remainder.
    /*printf("quotient : ");
    print_bignum(divisionRESULT.q);
    printf("\nremainder : ");
    print_bignum(divisionRESULT.r);*/

    return divisionRESULT;
}


bool compare_bignum(bignum* a, bignum* b) {
    // Comparer la taille d'abord, c'est le + simple.
    if (a->size > b->size) return true;
    if (a->size < b->size) return false;

    // Si les tailles sont égales, alors il faut comparer chiffre par chiffres en partant du plus significatif.
    for (int i = a->size - 1; i >= 0; i--)
    {
        if (a->tab[i] > b->tab[i]) return true;
        if (a->tab[i] < b->tab[i]) return false;
    }

    // Enfin, si tous les chiffres sont égaux, alors a = b, et donc c'est true.
    return true;
}

// Fonction pour diviser un bignum binaire par 2 (en utilisant une conversion en chaîne)
bignum* divide_by_2(bignum* num) {
    char* num_str = bignum2str(num); // Convertir le bignum en chaîne
    int len = strlen(num_str);

    // Supprimer le dernier caractère de la chaîne
    if (len > 1) {
        num_str[len - 1] = '\0';
    }
    else {
        strcpy(num_str, "0");
    }

    bignum* result = str2bignum(num_str); // Convertir la chaîne modifiée en bignum
    free(num_str);
    return result;
}

// Fonction pour afficher un bignum
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

    printf("\n-------------- TEST DIVISION BINAIRE --------------\n");
    division_result testRESULT = remainderbignum(num1, num2);
    bignum* quotient = testRESULT.q;
    bignum* remainder = testRESULT.r;

    printf("\n\nVoici le quotient (base 2^4 soit 16):\n");
    print_bignum(quotient);
    printf("\nVoici le remainder (base 2^4 soit 16):\n");
    print_bignum(remainder);


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
