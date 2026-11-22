#include "../include/restoran.h"

void pushUndoStack(const char *kode, int qty) {
    UndoNode *baru = (UndoNode *)malloc(sizeof(UndoNode));
    if (baru == NULL) return;
    strcpy(baru->kode_menu, kode);
    baru->qty_dibatalkan = qty;
    baru->next = topUndoStack;
    topUndoStack = baru;
}

void clearUndoStack(void) {
    while (topUndoStack != NULL) {
        UndoNode *temp = topUndoStack;
        topUndoStack = topUndoStack->next;
        free(temp);
    }
}

void undoKeranjang(void) {
    UndoNode *top;
    int i;
    if (topUndoStack == NULL) {
        printf("[!] Keranjang masih kosong, nggak ada yang bisa dibatalkan.\n");
        return;
    }
    top = topUndoStack;
    topUndoStack = topUndoStack->next;
    for (i = 0; i < jumlah_item_keranjang; i++) {
        if (bandingkanStringIgnoreCase(keranjangBelanja[i].item_menu->kode, top->kode_menu) == 0) {
            keranjangBelanja[i].qty -= top->qty_dibatalkan;
            printf("[+] Batalin pesanan terakhir: %dx %s dihapus dari keranjang.\n",
                   top->qty_dibatalkan, keranjangBelanja[i].item_menu->nama);
            if (keranjangBelanja[i].qty <= 0) {
                int j;
                for (j = i; j < jumlah_item_keranjang - 1; j++) {
                    keranjangBelanja[j] = keranjangBelanja[j + 1];
                }
                jumlah_item_keranjang--;
            }
            free(top);
            return;
        }
    }
    free(top);
}

void enqueueOrder(const char *jenis, const char *pemesan, const char *tujuan,
                  int idCabang, const char *cabang, const char *rincian, int total) {
    QueueNode *baru = (QueueNode *)malloc(sizeof(QueueNode));
    if (baru == NULL) return;
    baru->nomor_order = counterNomorOrder++;
    strcpy(baru->jenis_order, jenis);
    strcpy(baru->nama_pemesan, pemesan);
    strcpy(baru->lokasi_tujuan, tujuan);
    baru->id_cabang = idCabang;
    strcpy(baru->cabang_pemroses, cabang);
    strcpy(baru->rincian_menu, rincian);
    baru->total_bayar = total;
    baru->next = NULL;

    if (rearOrderQueue == NULL) {
        frontOrderQueue = rearOrderQueue = baru;
    } else {
        rearOrderQueue->next = baru;
        rearOrderQueue = baru;
    }

    simpanDataAntrean();

    int posisiDiCabang = 0;
    QueueNode *curr = frontOrderQueue;
    while (curr != NULL) {
        if (curr->id_cabang == idCabang) {
            posisiDiCabang++;
        }
        curr = curr->next;
    }

    printf("\n[+] PESANAN BERHASIL DIBUAT!\n");
    printf("    Nomor Pesanan   : #%d (%s)\n", baru->nomor_order, jenis);
    printf("    Cabang          : %s\n", cabang);
    printf("    Nomor Antrean   : Antrean ke-%d di cabang ini\n", posisiDiCabang);
}

void dequeueOrderSelesaiCabang(int filterIdCabang) {
    QueueNode *curr = frontOrderQueue;
    QueueNode *prev = NULL;

    while (curr != NULL) {
        if (filterIdCabang == -1 || curr->id_cabang == filterIdCabang) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        if (filterIdCabang == -1) {
            printf("[!] Lagi nggak ada antrean pesanan.\n");
        } else {
            printf("[!] Lagi nggak ada antrean pesanan di %s.\n", namaLokasi[filterIdCabang]);
        }
        return;
    }

    if (prev == NULL) {
        frontOrderQueue = curr->next;
        if (frontOrderQueue == NULL) rearOrderQueue = NULL;
    } else {
        prev->next = curr->next;
        if (curr == rearOrderQueue) rearOrderQueue = prev;
    }

    ZonaNode *cNode = cariNodeCabangByGraphID(rootWilayahTree, curr->id_cabang);
    if (cNode != NULL && cNode->antrean_aktif > 0) {
        cNode->antrean_aktif--;
    }

    simpanDataAntrean();

    printf("\n[+] PESANAN SELESAI DISIAPKAN (%s):\n", current_user);
    printf("    Order #%d [%s] | Atas Nama: %s\n", curr->nomor_order, curr->jenis_order, curr->nama_pemesan);
    printf("    Cabang  : %s | Info: %s\n", curr->cabang_pemroses, curr->lokasi_tujuan);
    printf("    Total   : Rp %d | Status: SELESAI.\n", curr->total_bayar);
    free(curr);
}

void tampilkanAntreanPesananStaf(void) {
    QueueNode *curr = frontOrderQueue;
    int posCabang[JUMLAH_CABANG] = {0};

    clearScreen();
    printf("====================================================================================\n");
    printf("       DAFTAR ANTREAN PESANAN (CABANG KAMU: %s)\n", namaLokasi[current_cabang_staf]);
    printf("====================================================================================\n");
    if (curr == NULL) {
        printf("[*] Belum ada pesanan yang masuk.\n");
    } else {
        while (curr != NULL) {
            posCabang[curr->id_cabang]++;
            printf("[Order #%d | %-15s] Antrean ke-%d di %s%s\n",
                   curr->nomor_order, curr->jenis_order,
                   posCabang[curr->id_cabang], curr->cabang_pemroses,
                   (curr->id_cabang == current_cabang_staf) ? "  <-- [CABANG KAMU]" : "");
            printf("   Pemesan     : %s (%s)\n", curr->nama_pemesan, curr->lokasi_tujuan);
            printf("   Pesanan     : %s\n", curr->rincian_menu);
            printf("   Total Bayar : Rp %d | Status: [SEDANG DISIAPKAN]\n", curr->total_bayar);
            printf("------------------------------------------------------------------------------------\n");
            curr = curr->next;
        }
    }
    printf("====================================================================================\n");
    printf("1. Selesaikan Pesanan Paling Depan di Cabang ini\n");
    printf("0. Kembali\n");
    printf("Pilih: ");
}

void cekStatusPesananPelanggan(const char *username) {
    QueueNode *curr = frontOrderQueue;
    int posCabang[JUMLAH_CABANG] = {0};
    int ketemu = 0;

    clearScreen();
    printf("====================================================================\n");
    printf("             STATUS PESANAN MILIK: %-20s             \n", username);
    printf("====================================================================\n");

    while (curr != NULL) {
        posCabang[curr->id_cabang]++;

        if ((strcmp(curr->jenis_order, "DELIVERY") == 0 ||
             strcmp(curr->jenis_order, "TAKEAWAY-ONLINE") == 0) &&
            strcmp(curr->nama_pemesan, username) == 0) {
            ketemu++;
            int sisaAntreanDiDepan = posCabang[curr->id_cabang] - 1;
            printf(">> Order #%d [%s]\n", curr->nomor_order, curr->jenis_order);
            printf("   Cabang          : %s\n", curr->cabang_pemroses);
            printf("   Urutan Antrean  : ANTREAN KE-%d di cabang ini", posCabang[curr->id_cabang]);
            if (sisaAntreanDiDepan == 0) {
                printf(" (LAGI DIMASAK SEKARANG!)\n");
            } else {
                printf(" (Nunggu %d pesanan lagi di depanmu)\n", sisaAntreanDiDepan);
            }
            printf("   Info            : %s\n", curr->lokasi_tujuan);
            printf("   Menu Dipesan    : %s\n", curr->rincian_menu);
            printf("   Total Bayar     : Rp %d\n", curr->total_bayar);
            printf("--------------------------------------------------------------------\n");
        }
        curr = curr->next;
    }

    if (ketemu == 0) {
        printf("[*] Kamu lagi nggak punya pesanan yang antre.\n");
    }
    printf("====================================================================\n");
    pauseScreen();
}