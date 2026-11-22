#include "../include/restoran.h"

void selectionSortHarga(MenuNode *daftar[], int n, int ascending) {
    int i, j, target_idx;
    MenuNode *swap;
    for (i = 0; i < n - 1; i++) {
        target_idx = i;
        for (j = i + 1; j < n; j++) {
            if (ascending ? (daftar[j]->harga < daftar[target_idx]->harga)
                          : (daftar[j]->harga > daftar[target_idx]->harga)) {
                target_idx = j;
            }
        }
        swap = daftar[i];
        daftar[i] = daftar[target_idx];
        daftar[target_idx] = swap;
    }
}

void insertionSortNama(MenuNode *daftar[], int n, int ascending) {
    int i, j;
    MenuNode *key;
    for (i = 1; i < n; i++) {
        key = daftar[i];
        j = i - 1;
        while (j >= 0 && ((ascending && bandingkanStringIgnoreCase(daftar[j]->nama, key->nama) > 0) ||
                          (!ascending && bandingkanStringIgnoreCase(daftar[j]->nama, key->nama) < 0))) {
            daftar[j + 1] = daftar[j];
            j = j - 1;
        }
        daftar[j + 1] = key;
    }
}

void selectionSortTerlaris(MenuNode *daftar[], int n, int ascending) {
    int i, j, target_idx;
    MenuNode *swap;
    for (i = 0; i < n - 1; i++) {
        target_idx = i;
        for (j = i + 1; j < n; j++) {
            if (ascending ? (daftar[j]->total_terjual < daftar[target_idx]->total_terjual)
                          : (daftar[j]->total_terjual > daftar[target_idx]->total_terjual)) {
                target_idx = j;
            }
        }
        swap = daftar[i];
        daftar[i] = daftar[target_idx];
        daftar[target_idx] = swap;
    }
}