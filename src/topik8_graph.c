#include "../include/restoran.h"

void initPetaKotaGraph(void) {
    int i, j;
    jumlahLokasi = 11;
    for (i = 0; i < MAX_LOKASI; i++) {
        namaLokasi[i][0] = '\0';
        tipeLokasi[i] = 0;
        zonaLokasi[i] = 0;
        for (j = 0; j < MAX_LOKASI; j++) adjMatrix[i][j] = 0;
    }

    strcpy(namaLokasi[0], "Cabang_Utara_1 (Jl. Diponegoro)");  tipeLokasi[0] = 1; zonaLokasi[0] = 1;
    strcpy(passwordCabang[0], "utara1");

    strcpy(namaLokasi[1], "Cabang_Utara_2 (Jl. Sudirman)");    tipeLokasi[1] = 1; zonaLokasi[1] = 1;
    strcpy(passwordCabang[1], "utara2");

    strcpy(namaLokasi[2], "Cabang_Selatan_1 (Jl. Kartini)");   tipeLokasi[2] = 1; zonaLokasi[2] = 2;
    strcpy(passwordCabang[2], "selatan1");

    strcpy(namaLokasi[3], "Cabang_Selatan_2 (Jl. Veteran)");   tipeLokasi[3] = 1; zonaLokasi[3] = 2;
    strcpy(passwordCabang[3], "selatan2");

    strcpy(namaLokasi[4], "Kel. Blotongan (Utara)");           tipeLokasi[4] = 0; zonaLokasi[4] = 1;
    strcpy(namaLokasi[5], "Kel. Kauman Kidul (Utara)");        tipeLokasi[5] = 0; zonaLokasi[5] = 1;
    strcpy(namaLokasi[6], "Kel. Sidorejo Lor (Utara)");        tipeLokasi[6] = 0; zonaLokasi[6] = 1;
    strcpy(namaLokasi[7], "Kel. Tingkir (Selatan)");           tipeLokasi[7] = 0; zonaLokasi[7] = 2;
    strcpy(namaLokasi[8], "Kel. Cebongan (Selatan)");          tipeLokasi[8] = 0; zonaLokasi[8] = 2;
    strcpy(namaLokasi[9], "Kel. Ledok (Selatan)");             tipeLokasi[9] = 0; zonaLokasi[9] = 2;
    strcpy(namaLokasi[10], "Kawasan Kampus (Perbatasan)");     tipeLokasi[10] = 0; zonaLokasi[10] = 1;

    adjMatrix[0][4] = adjMatrix[4][0] = 2;
    adjMatrix[0][6] = adjMatrix[6][0] = 3;
    adjMatrix[4][5] = adjMatrix[5][4] = 3;
    adjMatrix[1][5] = adjMatrix[5][1] = 2;
    adjMatrix[1][6] = adjMatrix[6][1] = 4;
    adjMatrix[1][10] = adjMatrix[10][1] = 3;
    adjMatrix[6][10] = adjMatrix[10][6] = 2;

    adjMatrix[2][7] = adjMatrix[7][2] = 2;
    adjMatrix[2][9] = adjMatrix[9][2] = 3;
    adjMatrix[2][10] = adjMatrix[10][2] = 4;
    adjMatrix[3][8] = adjMatrix[8][3] = 2;
    adjMatrix[3][9] = adjMatrix[9][3] = 3;
    adjMatrix[7][8] = adjMatrix[8][7] = 4;
    adjMatrix[0][2] = adjMatrix[2][0] = 7;
}

void tampilkanPetaJaringanKota(void) {
    int i, j, pertama;
    clearScreen();
    printf("====================================================================================\n");
    printf("                        PETA KONEKSI CABANG & WILAYAH KOTA                          \n");
    printf("====================================================================================\n");
    for (i = 0; i < jumlahLokasi; i++) {
        printf("%-32s -> Nyambung ke:\n", namaLokasi[i]);
        pertama = 1;
        for (j = 0; j < jumlahLokasi; j++) {
            if (adjMatrix[i][j] > 0) {
                printf("      - %s (%d km)\n", namaLokasi[j], adjMatrix[i][j]);
                pertama = 0;
            }
        }
        if (pertama) {
            printf("      - (Belum ada jalan langsung)\n");
        }
        printf("------------------------------------------------------------------------------------\n");
    }
    printf("====================================================================================\n");
}

void bfsJangkauanCabang(int startNode) {
    int visited[MAX_LOKASI] = {0};
    int levelHop[MAX_LOKASI] = {0};
    int queue[MAX_LOKASI];
    int front = 0, rear = 0, v;
    int cabangRekomendasi = -1;
    int radiusCabangTerdekat = -1;
    int nomorUrut = 1;

    visited[startNode] = 1;
    levelHop[startNode] = 0;
    queue[rear++] = startNode;

    printf("\n====================================================================\n");
    printf("   DAFTAR CABANG TERDEKAT DARI: %s\n", namaLokasi[startNode]);
    printf("====================================================================\n");

    while (front < rear) {
        int u = queue[front++];

        if (tipeLokasi[u] == 1 && u != startNode) {
            ZonaNode *cNode = cariNodeCabangByGraphID(rootWilayahTree, u);
            int buka = (cNode != NULL && cNode->is_buka);

            printf("  %d. %-33s | Lewat %d Bangjo | [%s]\n",
                   nomorUrut++,
                   namaLokasi[u],
                   levelHop[u],
                   buka ? "BUKA" : "TUTUP");

            if (buka && cabangRekomendasi == -1) {
                cabangRekomendasi = u;
                radiusCabangTerdekat = levelHop[u];
            }
        }

        for (v = 0; v < jumlahLokasi; v++) {
            if (adjMatrix[u][v] > 0 && !visited[v]) {
                visited[v] = 1;
                levelHop[v] = levelHop[u] + 1;
                queue[rear++] = v;
            }
        }
    }

    printf("--------------------------------------------------------------------\n");
    if (cabangRekomendasi != -1) {
        printf("[+] Cabang Buka Terdekat: %s (Lewat %d bangjo)\n",
               namaLokasi[cabangRekomendasi], radiusCabangTerdekat);
    } else {
        printf("[!] Lagi nggak ada cabang yang buka di dekat lokasi ini.\n");
    }
    printf("====================================================================\n");
}

void dfsHelper(int u, int visited[], int *urutan) {
    int v;
    visited[u] = 1;
    printf("  Titik %2d -> [ID:%2d] %-32s (%s)\n",
           (*urutan)++, u, namaLokasi[u],
           tipeLokasi[u] == 1 ? "CABANG" : "WILAYAH");

    for (v = 0; v < jumlahLokasi; v++) {
        if (adjMatrix[u][v] > 0 && !visited[v]) {
            dfsHelper(v, visited, urutan);
        }
    }
}

void dfsTelusurJaringan(int startNode) {
    int visited[MAX_LOKASI] = {0};
    int urutan = 1, i, terhubungSemua = 1;

    printf("\n====================================================================\n");
    printf("   CEK SAMBUNGAN JALAN ANTAR-CABANG & WILAYAH\n");
    printf("   Mulai dari: %s\n", namaLokasi[startNode]);
    printf("====================================================================\n");

    dfsHelper(startNode, visited, &urutan);

    for (i = 0; i < jumlahLokasi; i++) {
        if (!visited[i]) {
            terhubungSemua = 0;
            break;
        }
    }
    printf("--------------------------------------------------------------------\n");
    printf("Hasil Cek Jalan: %s (%d dari %d lokasi terjangkau)\n",
           terhubungSemua ? "SEMUA JALAN NYAMBUNG (AMAN)" : "ADA LOKASI TERPUTUS",
           urutan - 1, jumlahLokasi);
    printf("====================================================================\n");
}

int hitungDijkstra(int start, int goal, int cetakRute) {
    int dist[MAX_LOKASI], visited[MAX_LOKASI], parent[MAX_LOKASI];
    int i, count;

    for (i = 0; i < jumlahLokasi; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[start] = 0;

    for (count = 0; count < jumlahLokasi - 1; count++) {
        int min = INF, u = -1, v;
        for (v = 0; v < jumlahLokasi; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        for (v = 0; v < jumlahLokasi; v++) {
            if (!visited[v] && adjMatrix[u][v] > 0 && dist[u] != INF &&
                dist[u] + adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adjMatrix[u][v];
                parent[v] = u;
            }
        }
    }

    if (dist[goal] == INF) return -1;

    if (cetakRute) {
        int path[MAX_LOKASI], pathLen = 0, curr = goal;
        while (curr != -1) {
            path[pathLen++] = curr;
            curr = parent[curr];
        }
        printf("    Rute Jalan Kurir   : ");
        for (i = pathLen - 1; i >= 0; i--) {
            printf("%s%s", namaLokasi[path[i]], (i > 0) ? " -> " : "");
        }
        printf("\n    Jarak & Perkiraan  : %d km (sekitar %d menit)\n",
               dist[goal], dist[goal] * 4 + 10);
    }
    return dist[goal];
}