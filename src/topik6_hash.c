#include "../include/restoran.h"

int hashFunction(const char *kode) {
    int sum = 0, i;
    for (i = 0; kode[i] != '\0'; i++) {
        sum += (unsigned char)toupper((unsigned char)kode[i]);
    }
    return sum % TABLE_SIZE;
}

void initHashTable(void) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) hashTable[i] = NULL;
}

void clearHashTable(void) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        MenuNode *curr = hashTable[i];
        while (curr != NULL) {
            MenuNode *next = curr->next;
            free(curr);
            curr = next;
        }
        hashTable[i] = NULL;
    }
}

MenuNode *cariMenuHash(const char *kode) {
    int idx = hashFunction(kode);
    MenuNode *temp = hashTable[idx];
    while (temp != NULL) {
        if (bandingkanStringIgnoreCase(temp->kode, kode) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

MenuNode *insertHashTable(const char *kode, const char *nama, const char *kategori, int harga) {
    int idx = hashFunction(kode);
    MenuNode *baru;
    if (cariMenuHash(kode) != NULL) return NULL;

    baru = (MenuNode *)malloc(sizeof(MenuNode));
    if (baru == NULL) return NULL;

    strcpy(baru->kode, kode);
    strcpy(baru->nama, nama);
    strcpy(baru->kategori, kategori);
    baru->harga = harga;
    baru->total_terjual = 0;
    baru->next = NULL;

    if (hashTable[idx] == NULL) {
        hashTable[idx] = baru;
    } else {
        MenuNode *temp = hashTable[idx];
        while (temp->next != NULL) temp = temp->next;
        temp->next = baru;
    }
    return baru;
}