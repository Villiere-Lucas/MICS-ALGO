#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/*
* @author: Lucas VILLIERE, Maxence QUINET, Naman SHARMA
* Supervised by M. Coron.
* 
* This work is a student project carried out as part of a master's degree in computer science at the University of Luxembourg.
* 
* It presents the implementation of big numbers in C, as well as their application in the RSA encryption system, following Mr. Coron's course.
* 
* @date: 28 June 2024
*/


// --------- STRUCTURES ---------
typedef struct {
    int sign;
    int size;
    int* tab;
} bignum;

typedef struct {
    bignum* quotient;
    bignum* remainder;
} remainder_and_quotient;

// ------------------------------
// ------------------------------------ AUXILIARY FUNCTIONS ------------------------------------
bignum* str2bignum(char* str) {
    int sign = 0;
    if (str[0] == '-') {
        sign = 1;
        str++;
    }

    int length = strlen(str);
    bignum* result = (bignum*)malloc(sizeof(bignum));
    result->sign = sign;
    result->size = length;
    result->tab = (int*)malloc(length * sizeof(int));

    for (int i = 0; i < length; i++) {
        if (str[i] == '1') {
            result->tab[length - 1 - i] = 1;
        }
        else if (str[i] == '0') {
            result->tab[length - 1 - i] = 0;
        }
        else {
            free(result->tab);
            free(result);
            return NULL;
        }
    }

    return result;
}

char* bignum2str(bignum* num) {
    int length = num->size;
    int total_length = length + num->sign;
    char* str = (char*)malloc((total_length + 1) * sizeof(char));

    if (num->sign) {
        str[0] = '-';
    }

    for (int i = 0; i < length; i++) {
        str[num->sign + i] = num->tab[length - 1 - i] + '0';
    }

    str[total_length] = '\0';
    return str;
}

void print_bignum(bignum* num) {
    if (num->sign) {
        printf("-");
    }
    for (int i = num->size - 1; i >= 0; i--) {
        printf("%d", num->tab[i]);
    }
    printf("\n");
}

char* decimalToBinary(char* decStr) {
    int len = strlen(decStr);
    int maxDigits = len * 4;

    char* baseStr = (char*)malloc(maxDigits + 1);
    if (baseStr == NULL) {
        return NULL;
    }

    char* tempStr = (char*)malloc(len + 1);
    if (tempStr == NULL) {
        free(baseStr);
        return NULL;
    }
    strcpy(tempStr, decStr);

    int carry;
    int tempLen = len;
    int baseIndex = maxDigits;

    baseStr[baseIndex] = '\0';

    while (tempLen > 0) {
        carry = 0;
        for (int i = 0; i < tempLen; ++i) {
            int num = carry * 10 + (tempStr[i] - '0');
            tempStr[i] = (num / 2) + '0';
            carry = num % 2;
        }
        baseStr[--baseIndex] = carry + '0';

        int leadingZeros = 0;
        while (leadingZeros < tempLen && tempStr[leadingZeros] == '0') {
            ++leadingZeros;
        }
        if (leadingZeros > 0) {
            memmove(tempStr, tempStr + leadingZeros, tempLen - leadingZeros + 1);
            tempLen -= leadingZeros;
        }
    }

    free(tempStr);

    char* first_non_zero = baseStr + baseIndex;
    char* resultStr = strdup(first_non_zero);
    free(baseStr);

    return resultStr;
}

char* binaryToDecimal(char* binStr) {
    int len = strlen(binStr);

    int maxLen = len + 1;
    char* decimalStr = (char*)malloc(maxLen + 1);
    if (decimalStr == NULL) {
        return NULL;
    }

    memset(decimalStr, '0', maxLen);
    decimalStr[maxLen] = '\0';

    for (int i = 0; i < len; ++i) {
        int value = binStr[i] - '0';
        if (value != 0 && value != 1) {
            free(decimalStr);
            return NULL;
        }

        int carry = value;
        for (int j = maxLen - 1; j >= 0; --j) {
            int num = (decimalStr[j] - '0') * 2 + carry;
            decimalStr[j] = (num % 10) + '0';
            carry = num / 10;
        }
    }

    char* first_non_zero = decimalStr;
    while (*first_non_zero == '0' && *(first_non_zero + 1) != '\0') {
        ++first_non_zero;
    }

    char* resultStr = strdup(first_non_zero);
    free(decimalStr);

    return resultStr;
}

// -------------------------------------------------------------------------------------------------------------------


// --------------------------------------------- ARITHMETICS FUNCTIONS (ADD, SUB & MULT) -------------------------------------------------------
bignum* add_positive_bignums(bignum* a, bignum* b) {
    int max_size = (a->size > b->size) ? a->size : b->size;
    int* result_tab = (int*)calloc(max_size + 1, sizeof(int));

    int carry = 0;
    for (int i = 0; i < max_size; i++) {
        int digit_a = (i < a->size) ? a->tab[i] : 0;
        int digit_b = (i < b->size) ? b->tab[i] : 0;
        int sum = digit_a + digit_b + carry;
        result_tab[i] = sum % 2;
        carry = sum / 2;
    }

    if (carry) {
        result_tab[max_size] = carry;
        max_size++;
    }

    int new_size = max_size;
    while (new_size > 1 && result_tab[new_size - 1] == 0) {
        new_size--;
    }

    bignum* result = (bignum*)malloc(sizeof(bignum));
    result->sign = 0;
    result->size = new_size;
    result->tab = result_tab;

    return result;
}

bignum* subtract_positive_bignums(bignum* a, bignum* b) {
    int* result_tab = (int*)calloc(a->size, sizeof(int));

    int borrow = 0;
    for (int i = 0; i < a->size; i++) {
        int digit_a = a->tab[i];
        int digit_b = (i < b->size) ? b->tab[i] : 0;
        int diff = digit_a - digit_b - borrow;
        if (diff < 0) {
            diff += 2;
            borrow = 1;
        }
        else {
            borrow = 0;
        }
        result_tab[i] = diff;
    }

    int new_size = a->size;
    while (new_size > 1 && result_tab[new_size - 1] == 0) {
        new_size--;
    }

    bignum* result = (bignum*)malloc(sizeof(bignum));
    result->sign = 0;
    result->size = new_size;
    result->tab = result_tab;

    return result;
}

int compare_positive_bignums(bignum* a, bignum* b) {
    if (a->size > b->size) return 1;
    if (a->size < b->size) return -1;
    for (int i = a->size - 1; i >= 0; i--) {
        if (a->tab[i] > b->tab[i]) return 1;
        if (a->tab[i] < b->tab[i]) return -1;
    }
    return 0;
}

bignum* add(bignum* a, bignum* b) {
    if (a->sign == b->sign) {
        bignum* result = add_positive_bignums(a, b);
        result->sign = a->sign;
        return result;
    }
    else {
        int cmp = compare_positive_bignums(a, b);
        if (cmp == 0) {
            bignum* result = (bignum*)malloc(sizeof(bignum));
            result->sign = 0;
            result->size = 1;
            result->tab = (int*)calloc(1, sizeof(int));
            result->tab[0] = 0;
            return result;
        }
        else if (cmp > 0) {
            bignum* result = subtract_positive_bignums(a, b);
            result->sign = a->sign;
            return result;
        }
        else {
            bignum* result = subtract_positive_bignums(b, a);
            result->sign = b->sign;
            return result;
        }
    }
}

bignum* sub(bignum* a, bignum* b) {
    bignum* neg_b = (bignum*)malloc(sizeof(bignum));
    neg_b->sign = !b->sign;
    neg_b->size = b->size;
    neg_b->tab = (int*)malloc(b->size * sizeof(int));
    memcpy(neg_b->tab, b->tab, b->size * sizeof(int));
    bignum* result = add(a, neg_b);
    free(neg_b->tab);
    free(neg_b);
    return result;
}

bignum* multiply_positive_bignums(bignum* a, bignum* b) {
    int max_size = a->size + b->size;
    int* result_tab = (int*)calloc(max_size, sizeof(int));

    for (int i = 0; i < a->size; i++) {
        int carry = 0;
        for (int j = 0; j < b->size; j++) {
            int product = a->tab[i] * b->tab[j] + result_tab[i + j] + carry;
            result_tab[i + j] = product % 2;
            carry = product / 2;
        }
        result_tab[i + b->size] = carry;
    }

    int new_size = max_size;
    while (new_size > 1 && result_tab[new_size - 1] == 0) {
        new_size--;
    }

    bignum* result = (bignum*)malloc(sizeof(bignum));
    result->sign = 0;
    result->size = new_size;
    result->tab = result_tab;

    return result;
}

bignum* mult(bignum* a, bignum* b) {
    bignum* result = multiply_positive_bignums(a, b);
    result->sign = a->sign ^ b->sign;
    return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------


bignum* divide_by_2(bignum* a) {
    if (a->size == 1 && a->tab[0] == 0) {
        return str2bignum("0");
    }
    int new_size = a->size;
    int* result_tab = (int*)malloc(new_size * sizeof(int));
    int carry = 0;
    for (int i = new_size - 1; i >= 0; i--) {
        int current = a->tab[i] + carry * 2;
        result_tab[i] = current / 2;
        carry = current % 2;
    }
    while (new_size > 1 && result_tab[new_size - 1] == 0) {
        new_size--;
    }
    bignum* result = (bignum*)malloc(sizeof(bignum));
    result->sign = a->sign;
    result->size = new_size;
    result->tab = result_tab;
    return result;
}


bool compare_bignum(bignum* a, bignum* b) {
    if (a->size > b->size) return true;
    if (a->size < b->size) return false;
    for (int i = a->size - 1; i >= 0; i--) {
        if (a->tab[i] > b->tab[i]) return true;
        if (a->tab[i] < b->tab[i]) return false;
    }
    return true;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
remainder_and_quotient remainder_bignum(bignum* a, bignum* b) {
    bignum* q = str2bignum("0");
    bignum* r = a;

    int a_len = a->size;
    int b_len = b->size;
    int max_i = a_len - b_len;
    int maximum = MAX(0, max_i);

    int two_max_temp = (1 << maximum);
    char two_max_str[50];
    sprintf(two_max_str, "%d", two_max_temp);

    char* two_max_baseK = decimalToBinary(two_max_str);
    bignum* two_power_max = str2bignum(two_max_baseK);

    bignum* c = mult(two_power_max, b);

    bignum* one = str2bignum("1");
    bignum* two = str2bignum("10");
    for (int i = 0; i <= max_i; i++) {
        q = mult(q, two);
        if (compare_bignum(r, c)) {
            r = sub(r, c);
            q = add(q, one);
        }
        c = divide_by_2(c);
    }

    remainder_and_quotient result;
    result.quotient = q;
    result.remainder = r;

    free(two_max_baseK);
    free(two_power_max->tab);
    free(two_power_max);
    free(one->tab);
    free(one);
    free(two->tab);
    free(two);

    return result;
}

bignum* addmod(bignum* a, bignum* b, bignum* n) {
    bignum* sum = add(a, b);

    while (sum->sign) {
        bignum* new_sum = add(sum, n);
        free(sum->tab);
        free(sum);
        sum = new_sum;
    }

    remainder_and_quotient result = remainder_bignum(sum, n);
    bignum* mod_result = result.remainder;

    free(sum->tab);
    free(sum);
    free(result.quotient->tab);
    free(result.quotient);

    return mod_result;
}

bignum* multmod(bignum* a, bignum* b, bignum* n) {
    bignum* product = mult(a, b);
    
    while (product->sign) {
        bignum* new_product = add(product, n);
        free(product->tab);
        free(product);
        product = new_product;
    }

    remainder_and_quotient result = remainder_bignum(product, n);
    bignum* mod_result = result.remainder;
  
    return mod_result;
}

bignum* expmod(bignum* a, bignum* b, bignum* n) {
    bignum* result = str2bignum("1");
    bignum* base = a;
    char* b_str = bignum2str(b);

    int len = strlen(b_str);
    int j = 0;
  
    for (int i = len - 1; i >= 0; i--) {

        result = multmod(result, result, n);

        if (b_str[j] == '1') {

            result = multmod(result, base, n);

        }
        j++;
    }

    return result;
}

// --------- EXTENDED EUCLIDEAN ALGORITHM ---------
typedef struct {
    bignum* gcd;
    bignum* x;
    bignum* y;
} extended_gcd_result;


extended_gcd_result extended_gcd(bignum* a, bignum* b) {

    bignum* x0 = str2bignum("1");
    bignum* x1 = str2bignum("0");
    bignum* y0 = str2bignum("0");
    bignum* y1 = str2bignum("1");

    bignum* r0 = a;
    bignum* r1 = b;

    while (r1->size > 1 || r1->tab[0] != 0) {
        remainder_and_quotient q_and_r = remainder_bignum(r0, r1);

        bignum* remainder = q_and_r.remainder;
        bignum* quotient = q_and_r.quotient;

        // Convertir chaque temp a la base que l'on veut.
        bignum* temp_r = sub(r0, mult(quotient, r1));
        bignum* temp_x = sub(x0, mult(quotient, x1));
        bignum* temporaire_y = mult(quotient, y1);
        bignum* temp_y = sub(y0, temporaire_y);

        r0 = r1;
        r1 = temp_r;
        x0 = x1;
        x1 = temp_x;
        y0 = y1;
        y1 = temp_y;
    }

    extended_gcd_result result;
    result.gcd = r0;
    result.x = x0;
    result.y = y0;

    return result;
}

// --------- INVERSEMOD() ---------
bignum* inversemod(bignum* a, bignum* n) {
    extended_gcd_result result = extended_gcd(a, n);
    bignum* gcd = result.gcd;
    bignum* x = result.x;

    // If gcd = 1, so there is a modular inverse !
    char* gcd_str = bignum2str(gcd);
    if (strcmp(gcd_str, "1") == 0) {
        bignum* inv = remainder_bignum(x, n).remainder;

        // If the inverse is negatif, we can adjust it to have the positif version
        if (inv->sign == 1) {
            inv = add(inv, n);
        }

        free(gcd->tab);
        free(gcd);
        free(gcd_str);
        return inv;
    }
    else {
        // No modular inverse
        free(gcd->tab);
        free(gcd);
        free(x->tab);
        free(x);
        free(result.y->tab);
        free(result.y);
        free(gcd_str);
        return NULL;
    }
}



// --------------------------------------------- RSA IMPLEMENTATION --------------------------------------------------------------------------

// Function to generate a random bignum with a specific length
bignum* genrandom(int length) {
    srand(time(NULL));

    bignum* result = (bignum*)malloc(sizeof(bignum));
    result->sign = 0; // --> The generate numbers are positifs
    result->size = length;
    result->tab = (int*)malloc(length * sizeof(int));

    // Random bit generation
    for (int i = 0; i < length; i++) {
        result->tab[i] = rand() % 2;
    }

    return result;
}

bignum* genrandom_in_range(bignum* min, bignum* max) {
    int length = max->size;
    bignum* random_num;

    do {
        random_num = genrandom(length);
    } while (compare_positive_bignums(random_num, min) < 0 || compare_positive_bignums(random_num, max) > 0);

    return random_num;
}

// FERMAT PRIMALITY TEST
int fermat(bignum* n, int t) {
    bignum* one = str2bignum("1");
    bignum* n_minus_one = sub(n, one); // n-1
    bignum* two = str2bignum("10");
    bignum* n_minus_two = sub(n, two); // n-2

    for (int i = 0; i < t; i++) {
        bignum* a = genrandom_in_range(two, n_minus_two); // 2 < a < n-2
        bignum* r = expmod(a, n_minus_one, n); // a^n-1 % n

        char* r_str = bignum2str(r);
        if (strcmp(r_str, "1") != 0) {
            return 0; // composite
        }

        free(r_str);
        free(a->tab);
        free(a);
        free(r->tab);
        free(r);
    }

    free(one->tab);
    free(one);
    free(n_minus_two->tab);
    free(n_minus_two);
    free(n_minus_one->tab);
    free(n_minus_one);
    free(two->tab);
    free(two);

    return 1; // prime
}

// Function to generate a prime bignum with a specific length
bignum* genrandomprime(int length, int t) {
    bignum* prime_candidate;

    do {
        prime_candidate = genrandom(length);
        //Just to be sure that the length is correct
        prime_candidate->tab[0] = 1;
        prime_candidate->tab[length - 1] = 1;
    } while (fermat(prime_candidate, t) == 0);

    return prime_candidate;
}


// Function to generate the RSA keys.
void keygen(bignum** n, bignum** e, bignum** d, int length) {
    int t = 5; // Security parameter for the fermat test.

    // Generation of 2 differents primes numbers (p & q)
    bignum* p = genrandomprime(length / 2, t);
    printf("\n P = ");
    print_bignum(p);

    bignum* q;
    do {
        q = genrandomprime(length / 2, t);
    } while (compare_positive_bignums(p, q) == 0);

    printf("\n Q = ");
    print_bignum(q);

    // Computing of n = p * q
    *n = mult(p, q);

    // Computing of phi(n) = (p-1) * (q-1)
    bignum* one = str2bignum("1");
    bignum* p_minus_1 = sub(p, one);
    bignum* q_minus_1 = sub(q, one);
    bignum* phi_n = mult(p_minus_1, q_minus_1);
    printf("\n Phi N = ");
    print_bignum(phi_n);

    // Choose e as : 1 < e < phi(n) and gcd(e, phi(n)) = 1
    *e = str2bignum("10000000000000001"); // e = 65537 en binaire

    while (1) {
        printf("Boucle");
        extended_gcd_result result = extended_gcd(*e, phi_n);
        if (strcmp(bignum2str(result.gcd), "1") == 0) {
            // Compute 'd', the modular inverse of e%phi(n)
            *d = inversemod(*e, phi_n);
            printf("break");
            break;
        }
        free((*e)->tab);
        free(*e);
        *e = add(*e, one);
    }

    // Free memory
    free(p->tab);
    free(p);
    free(q->tab);
    free(q);
    free(p_minus_1->tab);
    free(p_minus_1);
    free(q_minus_1->tab);
    free(q_minus_1);
    free(phi_n->tab);
    free(phi_n);
    free(one->tab);
    free(one);
}



// Fonction de chiffrement RSA
bignum* RSAencrypt(bignum* m, bignum* e, bignum* n) {
    return expmod(m, e, n);
}

bignum* RSAdecrypt(bignum* c, bignum* d, bignum* n) {
    return expmod(c, d, n);
}


void testRSA(int length) {
    bignum* n;
    bignum* e;
    bignum* d;
    keygen(&n, &e, &d, length);

    printf("\nPublic Key (n, e) :\n");
    printf("n = ");
    print_bignum(n);
    printf("e = ");
    print_bignum(e);

    printf("\nPrivate Key (d) :\n");
    printf("d = ");
    print_bignum(d);

    // Enter a message to encrypt
    char message_str[1024];
    printf("\nPlease enter a message to cipher in binary: ");
    scanf("%1023s", message_str);
    bignum* message = str2bignum(message_str);

    // Encryption
    bignum* ciphertext = RSAencrypt(message, e, n);
    printf("\nCiphertext : ");
    print_bignum(ciphertext);

    // Decryption
    bignum* decrypted_message = RSAdecrypt(ciphertext, d, n);
    printf("\nPlaintext : ");
    print_bignum(decrypted_message);

    // Verification of the decryption
    char* decrypted_message_str = bignum2str(decrypted_message);
    if (strcmp(message_str, decrypted_message_str) == 0) {
        printf("\nThe original message was successfully retrieved.\n");
    } else {
        printf("\nError: The original message was not retrieved correctly.\n");
    }

    // Free memory
    free(n->tab);
    free(n);
    free(e->tab);
    free(e);
    free(d->tab);
    free(d);
    free(message->tab);
    free(message);
    free(ciphertext->tab);
    free(ciphertext);
    free(decrypted_message->tab);
    free(decrypted_message);
    free(decrypted_message_str);
}

// -------------------------------------------------------------------------------------------------------------------------------------------

int main() {
    int length;
    printf("\n ----- TEST RSA -----\n\n");
    printf("Please enter the RSA key length in bits: ");
    scanf("%d", &length);

    testRSA(length);

    return 0;
}