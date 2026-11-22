#include "../include/restoran.h"

void sequentialSearchMenu(MenuNode *daftar[], int n, const char *keyword) {
    int i, ditemukan = 0;
    printf("\n[+] HASIL PENCARIAN MENU UNTUK '%s':\n", keyword);
    printf("--------------------------------------------------------------------\n");
    printf("%-6s | %-24s | %-8s | %-12s\n", "Kode", "Nama Menu", "Kategori", "Harga");
    printf("--------------------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        if (bandingkanStringIgnoreCase(daftar[i]->kode, keyword) == 0 ||
            strstr(daftar[i]->nama, keyword) != NULL) {
            printf("%-6s | %-24s | %-8s | Rp %d\n",
                   daftar[i]->kode, daftar[i]->nama, daftar[i]->kategori, daftar[i]->harga);
            ditemukan++;
        }
    }
    if (ditemukan == 0) printf("[!] Tidak ada menu yang cocok dengan kata '%s'.\n", keyword);
    printf("--------------------------------------------------------------------\n");
}

MenuNode *binarySearchMenuHarga(MenuNode *daftar[], int n, int targetHarga) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (daftar[mid]->harga == targetHarga) return daftar[mid];
        if (daftar[mid]->harga < targetHarga) low = mid + 1;
        else high = mid - 1;
    }
    return NULL;
}