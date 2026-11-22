#include "../include/restoran.h"

MenuNode *hashTable[TABLE_SIZE];
MenuBSTNode *rootMenuBST = NULL;
ZonaNode *rootWilayahTree = NULL;

char current_user[MAX_STR] = "";
char current_role[20] = "";
int current_cabang_staf = -1;

ItemPesanan keranjangBelanja[MAX_KERANJANG];
int jumlah_item_keranjang = 0;

UndoNode *topUndoStack = NULL;
QueueNode *frontOrderQueue = NULL;
QueueNode *rearOrderQueue = NULL;
int counterNomorOrder = 1001;

char namaLokasi[MAX_LOKASI][MAX_STR];
char passwordCabang[JUMLAH_CABANG][MAX_STR];
int tipeLokasi[MAX_LOKASI];
int zonaLokasi[MAX_LOKASI];
int adjMatrix[MAX_LOKASI][MAX_LOKASI];
int jumlahLokasi = 0;

void freeAllMemory(void) {
    clearUndoStack();
    while (frontOrderQueue != NULL) {
        QueueNode *temp = frontOrderQueue;
        frontOrderQueue = frontOrderQueue->next;
        free(temp);
    }
    freeZonaTree(rootWilayahTree);
    freeMenuBST(rootMenuBST);
    clearHashTable();
}

int main(void) {
    int pilihan;
    setvbuf(stdout, NULL, _IONBF, 0);

    initHashTable();
    loadDataMenu();
    initPetaKotaGraph();
    initTreeWilayahCabang();
    loadDataAntrean();

    while (1) {
        clearScreen();
        printf("============================================\n");
        printf("       APLIKASI PEMESANAN RESTORAN          \n");
        printf("============================================\n");
        printf("Masuk Sebagai:\n");
        printf("1. PELANGGAN (Pesan Makanan)\n");
        printf("2. STAF RESTORAN (Kasir & Dapur)\n");
        printf("0. Tutup Aplikasi\n");
        printf("Pilih (1/2/0): ");

        if (!bacaAngkaValid(&pilihan)) continue;

        if (pilihan == 1) {
            portalAuthRole("PELANGGAN");
        } else if (pilihan == 2) {
            portalAuthRole("STAF");
        } else if (pilihan == 0) {
            freeAllMemory();
            clearScreen();
            printf("Terima kasih! Sampai jumpa lagi.\n");
            break;
        }
    }
    return 0;
}