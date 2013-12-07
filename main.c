/* 
 * File:   main.c
 * Author: palmyman
 *
 * Created on December 11, 2012, 11:59 AM
 * 
 * TELEFONNI CISLA
 * 
 */

#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TELCO_NUMBERS 10

typedef struct TNode {
    char * m_Dest;
    struct TNode * m_Child[TELCO_NUMBERS];
} TNODE;
#endif /* __PROGTEST__ */

int checkNumber(const char * number) {
    int i = 0, asci;
    while (number[i]) {
        asci = number[i];
        if (asci < 48 || asci > 57)
            return 0;
        i++;
    }
    return i;
}

TNODE * mallocNode(void) {
    TNODE * r = (TNODE *) malloc(sizeof ( *r));
    int i;
    r->m_Dest = NULL;
    for (i = 0; i < TELCO_NUMBERS; i++) {
        r->m_Child[i] = NULL;
    }
    return r;
}

void delTree(TNODE * root) {
    /* funkce uvolní veškeré paměťové bloky, které byly použité pro zadaný 
     * strom. */
    int i;
    if (root) {
        for (i = 0; i < TELCO_NUMBERS; i++) {
            if (root->m_Child[i])
                delTree(root->m_Child[i]);
        }
        if (root->m_Dest) {
            free(root->m_Dest);
        }
        free(root);
    }
}

int addDest(TNODE ** root, const char * prefix, const char * dest) {
    /* je funkce, které do stávajícího trie přidá požadovaný prefix a jemu
     *  odpovídající cíl volání. Pokud daný prefix ve stromě již existuje, 
     * funkce pouze změní jemu asociovaný cíl. Pokud prefix neexistuje, funkce 
     * do stromu doplní odpovídající uzly. Funkce vrací úspěch (návratová 
     * hodnota 1) nebo neúspěch (návratová hodnota 0). Neúspěchem volání skončí 
     * pokud zadaný prefix tel. čísla obsahuje jiné znaky než cifry 0 až 9, v 
     * takovém případě funkce ponechá strom beze změn. */
    TNODE * leaf;
    int lenght = checkNumber(prefix), i, singleNumber;
    ;
    if (!lenght) {
        return 0;
    }
    if (*root == NULL) {
        *root = mallocNode();
    }
    leaf = *root;

    for (i = 0; i < lenght; i++) {
        singleNumber = prefix[i] - 48;
        if (leaf->m_Child[singleNumber] == NULL) {
            leaf->m_Child[singleNumber] = mallocNode();
        }
        leaf = leaf->m_Child[singleNumber];
    }
    leaf->m_Dest = (char *) malloc(strlen(dest) + 1);
    
    strcpy(leaf->m_Dest, dest);

    return 1;
}

int delUn(TNODE * root) {
    /* funkce uvolní veškeré paměťové bloky, které byly použité pro zadaný 
     * strom. */
    int i, childrens = 0;
    for (i = 0; i < TELCO_NUMBERS; i++) {
        if (root->m_Child[i]) {
            childrens++;
            if (delUn(root->m_Child[i])) {
                root->m_Child[i] = NULL;
                childrens--;
            }
        }
    }
    if (!root->m_Dest && !childrens) {
        free(root);
        return 1;
    }
    return 0;
}

int delDest(TNODE ** root, const char * prefix) {
    /* funkce odstraní ze stromu zadaný prefix a jemu asociovaný cíl volání. 
     * Pokud odkazovaný prefix ve stromu neexistuje nebo pokud zadaný prefix 
     * obsahuje nečíselné hodnoty, funkce vrací neúspěch (0) a strom ponechá 
     * beze změn. Pokud funkce úspěšně odstraní zadaný cíl volání, vrací 
     * hodnotu 1. Funkce z mazaného prefixu odstraní řetězec jména cíle 
     * (nahradí jej hodnotou NULL) a dále vymaže i všechny nepotřebné uzly, 
     * které ve stromu nemusí být (nemají žádnou funkci). V extrémním případě 
     * funkce může vymazat i celý strom - pokud odstraňuje poslední cíl volání 
     * ve stromu obsažený. 
     * 
     * chyba je v tom ze neodstranuju i nepotrebne uzly nademnou, potreba celou
     * dobu kontrolovat jestli je i jiny m_Child
     */
    TNODE * leaf;
    TNODE * del;
    int lenght = checkNumber(prefix), i, singleNumber;
    
    if (!lenght || !*root) {
        return 0;
    }
    leaf = *root;
    for (i = 0; i < lenght; i++) {
        singleNumber = prefix[i] - 48;
        leaf = leaf->m_Child[singleNumber];
        if (leaf == NULL) {
            return 0;
        }
    }
    free(leaf->m_Dest);
    leaf->m_Dest = NULL;
    for (i = 0; i < TELCO_NUMBERS; i++) {
        if (leaf->m_Child[i])
            return 1;
    }
    del = *root;
    if (delUn(del))
        * root = NULL;
    return 1;
}

const char * search(TNODE * root, const char * number) {
    /* funkce vyhledá ve stromu jméno cíle, které odpovídá zadanému číslu. 
     * Návratovou hodnotou je nalezený řetězec - název cíle, případně hodnota 
     * NULL. NULL je vrácen pro dotazované číslo, které obsahovalo nečíselné 
     * znaky nebo pro číslo, pro které není definovaný cíl. Pozor: strom je 
     * prefixový, nemusí obsahovat celé zadávané číslo. Pokud by strom 
     * obsahoval jediný cíl - prefix 1 (USA), tedy strom bude tvořen dvojicí 
     * uzlů, bude volání search vracet pro číslo 123456 hodnotu "USA", ale 
     * např. pro číslo 23456 hodnotu NULL. */

    char * destination;
    int lenght, i, singleNumber;
    ;
    destination = NULL;
    lenght = checkNumber(number);
    if (!lenght || !root)
        return NULL;
    for (i = 0; i < lenght; i++) {
        singleNumber = number[i] - 48;
        root = root->m_Child[singleNumber];
        if (root == NULL) {
            return destination;
        }
        if (root->m_Dest) {
            destination = root->m_Dest;
        }
    }
    return destination;
}
#ifndef __PROGTEST__

int main(int argc, char * argv []) {
    TNODE * root;
    char tmpStr[100];
    const char * dst;
    int res;

    root = NULL;
    res = addDest(&root, "420", "Czech republic"); /* res = 1 */
    res = addDest(&root, "421", "Slovak republic"); /* res = 1 */
    res = addDest(&root, "1", "USA"); /* res = 1 */
    res = addDest(&root, "420606", "CZ - O2 mobil"); /* res = 1 */
    res = addDest(&root, "420606123456", "CZ - Vodafone"); /* res = 1 */
    dst = search(root, "420606334455"); /* dst = "CZ - O2 mobil" */
    dst = search(root, "420603212223"); /* dst = "Czech republic" */
    dst = search(root, "420606123456"); /* dst = "CZ - Vodafone" */
    dst = search(root, "42060612345"); /* dst = "CZ - O2 mobil" */
    dst = search(root, "37123456"); /* dst = NULL */
    dst = search(root, "1998877665544332211"); /* dst = "USA" */
    delTree(root);

    root = NULL;
    res = addDest(&root, "420", "Czech republic"); /* res = 1 */
    res = addDest(&root, "421", "Slovak republic"); /* res = 1 */
    res = addDest(&root, "1", "USA"); /* res = 1 */
    res = addDest(&root, "420606", "CZ - O2 mobil"); /* res = 1 */
    res = addDest(&root, "420606123456", "CZ - Vodafone"); /* res = 1 */
    dst = search(root, "420222333444"); /* dst = "Czech republic" */
    dst = search(root, "420606112233"); /* dst = "CZ - O2 mobil" */
    dst = search(root, "420606123456"); /* dst = "CZ - Vodafone" */
    res = delDest(&root, "420"); /* res = 1 */
    dst = search(root, "420222333444"); /* dst = NULL */
    dst = search(root, "420606112233"); /* dst = "CZ - O2 mobil" */
    dst = search(root, "420606123456"); /* dst = "CZ - Vodafone" */
    res = addDest(&root, "42", "Euro telco company"); /* res = 1 */
    dst = search(root, "420222333444"); /* dst = "Euro telco company" */
    dst = search(root, "420606112233"); /* dst = "CZ - O2 mobil" */
    dst = search(root, "420606123456"); /* dst = "CZ - Vodafone" */
    delTree(root);

    root = NULL;
    strncpy(tmpStr, "Czech republic", sizeof ( tmpStr));
    res = addDest(&root, "420", tmpStr); /* res = 1 */
    strncpy(tmpStr, "Slovak republic", sizeof ( tmpStr));
    res = addDest(&root, "421", tmpStr); /* res = 1 */
    strncpy(tmpStr, "USA", sizeof ( tmpStr));
    res = addDest(&root, "1", tmpStr); /* res = 1 */
    strncpy(tmpStr, "CZ - O2 mobil", sizeof ( tmpStr));
    res = addDest(&root, "420606", tmpStr); /* res = 1 */
    strncpy(tmpStr, "CZ - Vodafone", sizeof ( tmpStr));
    res = addDest(&root, "420606123456", tmpStr); /* res = 1 */
    dst = search(root, "420606334455"); /* dst = "CZ - O2 mobil" */
    dst = search(root, "420603212223"); /* dst = "Czech republic" */
    dst = search(root, "420606123456"); /* dst = "CZ - Vodafone" */
    dst = search(root, "37123456"); /* dst = NULL */
    dst = search(root, "1998877665544332211"); /* dst = "USA" */
    delTree(root);

    root = NULL;
    res = addDest(&root, "420", "Czech republic"); /* res = 1 */
    res = addDest(&root, "1", "USA"); /* res = 1 */
    dst = search(root, "420606334455"); /* dst = "Czech republic" */
    dst = search(root, "12345"); /* dst = "USA" */
    res = delDest(&root, "1"); /* res = 1 */
    dst = search(root, "12345"); /* dst = NULL */
    res = delDest(&root, "420"); /* res = 1 */
    dst = search(root, "420606334455"); /* dst = NULL */
    res = delDest(&root, "420"); /* res = 0 */
    res = addDest(&root, "420A", "???"); /* res = 0 */
    delTree(root);

    return 0;
}
#endif /* __PROGTEST__ */

