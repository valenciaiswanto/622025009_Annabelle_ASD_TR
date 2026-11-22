#ifndef RESTORAN_H
#define RESTORAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 10
#define MAX_MENU 100
#define MAX_KERANJANG 50
#define MAX_LOKASI 15
#define JUMLAH_CABANG 4
#define MAX_STR 64
#define MAX_KODE 12
#define INF 999999

#define FILE_AKUN_STAF "data/akun_staf.txt"
#define FILE_AKUN_PELANGGAN "data/akun_pelanggan.txt"
#define FILE_DATABASE_MENU "data/menu.txt"
#define FILE_DATABASE_ANTREAN "data/antrean.txt"

/* ============================================================================
 * DEFINISI STRUCT
 * ============================================================================ */

typedef struct MenuNode {
    char kode[MAX_KODE];
    char nama[MAX_STR];
    char kategori[20];
    int harga;
    int total_terjual;
    struct MenuNode *next;
} MenuNode;

typedef struct {
    MenuNode *item_menu;
    int qty;
} ItemPesanan;

typedef struct UndoNode {
    char kode_menu[MAX_KODE];
    int qty_dibatalkan;
    struct UndoNode *next;
} UndoNode;

typedef struct QueueNode {
    int nomor_order;
    char jenis_order[25];
    char nama_pemesan[MAX_STR];
    char lokasi_tujuan[MAX_STR];
    int id_cabang;
    char cabang_pemroses[MAX_STR];
    char rincian_menu[256];
    int total_bayar;
    struct QueueNode *next;
} QueueNode;

typedef struct MenuBSTNode {
    char kode[MAX_KODE];
    char nama[MAX_STR];
    char kategori[20];
    int harga;
    struct MenuBSTNode *left;
    struct MenuBSTNode *right;
} MenuBSTNode;

typedef struct ZonaNode {
    char nama_zona[MAX_STR];
    int id_lokasi_graph;
    int is_buka;
    int antrean_aktif;
    struct ZonaNode *firstChild;
    struct ZonaNode *nextSibling;
} ZonaNode;

/* ============================================================================
 * DEKLARASI VARIABEL GLOBAL (EXTERN)
 * ============================================================================ */
extern MenuNode *hashTable[TABLE_SIZE];
extern MenuBSTNode *rootMenuBST;
extern ZonaNode *rootWilayahTree;

extern char current_user[MAX_STR];
extern char current_role[20];
extern int current_cabang_staf;

extern ItemPesanan keranjangBelanja[MAX_KERANJANG];
extern int jumlah_item_keranjang;

extern UndoNode *topUndoStack;
extern QueueNode *frontOrderQueue;
extern QueueNode *rearOrderQueue;
extern int counterNomorOrder;

extern char namaLokasi[MAX_LOKASI][MAX_STR];
extern char passwordCabang[JUMLAH_CABANG][MAX_STR];
extern int tipeLokasi[MAX_LOKASI];
extern int zonaLokasi[MAX_LOKASI];
extern int adjMatrix[MAX_LOKASI][MAX_LOKASI];
extern int jumlahLokasi;

/* ============================================================================
 * PROTOTYPE FUNGSI (TERURUT SESUAI TOPIK 1 - 8 HANDOUT)
 * ============================================================================ */

/* --- TOPIK 1 & 2 --- */
void clearScreen(void);
void pauseScreen(void);
void hapusNewline(char *str);
int isInputKosong(const char *str);
int bacaAngkaValid(int *hasil);
int bandingkanStringIgnoreCase(const char *s1, const char *s2);
int ekstrakSemuaMenuArray(MenuNode *daftar[], int max_size);

void buatFileMenuDefault(void);
void simpanNodeBSTKeFile(FILE *fp, MenuBSTNode *root);
void simpanSemuaMenuKeFile(void);
void loadDataMenu(void);
void simpanDataAntrean(void);
void loadDataAntrean(void);

const char *getNamaFileAkun(const char *role);
int cekUsernameAdaDiFile(const char *namaFile, const char *target);
void registerAkunRole(const char *role);
int loginAkunRole(const char *role);
int verifikasiCabangStaf(void);
void portalAuthRole(const char *role);

/* --- TOPIK 3 --- */
void selectionSortHarga(MenuNode *daftar[], int n, int ascending);
void insertionSortNama(MenuNode *daftar[], int n, int ascending);
void selectionSortTerlaris(MenuNode *daftar[], int n, int ascending);

/* --- TOPIK 4 --- */
void pushUndoStack(const char *kode, int qty);
void clearUndoStack(void);
void undoKeranjang(void);

void enqueueOrder(const char *jenis, const char *pemesan, const char *tujuan,
                  int idCabang, const char *cabang, const char *rincian, int total);
void dequeueOrderSelesaiCabang(int filterIdCabang);
void tampilkanAntreanPesananStaf(void);
void cekStatusPesananPelanggan(const char *username);

/* --- TOPIK 5 --- */
void sequentialSearchMenu(MenuNode *daftar[], int n, const char *keyword);
MenuNode *binarySearchMenuHarga(MenuNode *daftar[], int n, int targetHarga);

/* --- TOPIK 6 --- */
int hashFunction(const char *kode);
void initHashTable(void);
void clearHashTable(void);
MenuNode *cariMenuHash(const char *kode);
MenuNode *insertHashTable(const char *kode, const char *nama, const char *kategori, int harga);

/* --- TOPIK 7 --- */
MenuBSTNode *buatBSTNode(const char *kode, const char *nama, const char *kategori, int harga);
MenuBSTNode *insertMenuBST(MenuBSTNode *root, const char *kode, const char *nama, const char *kategori, int harga);
MenuBSTNode *searchMenuBST(MenuBSTNode *root, const char *kode);
MenuBSTNode *findMinBST(MenuBSTNode *node);
MenuBSTNode *deleteMenuBST(MenuBSTNode *root, const char *kode, int *berhasilHapus);
void preOrderMenuBST(MenuBSTNode *root);
void inOrderMenuBST(MenuBSTNode *root);
void cetakKatalogTabelBST(MenuBSTNode *root);
void postOrderMenuBST(MenuBSTNode *root);
void freeMenuBST(MenuBSTNode *root);

ZonaNode *buatZonaNode(const char *nama, int idGraph, int isBuka);
void tambahChildZona(ZonaNode *parent, ZonaNode *child);
void initTreeWilayahCabang(void);
void tampilkanTreeWilayah(ZonaNode *node, int level);
ZonaNode *cariNodeCabangByGraphID(ZonaNode *node, int idGraph);
ZonaNode *tentukanCabangViaTreeDanGraph(int idTujuanPelanggan, int *jarakTerpilih);
void freeZonaTree(ZonaNode *node);

/* --- TOPIK 8 --- */
void initPetaKotaGraph(void);
void tampilkanPetaJaringanKota(void);
void bfsJangkauanCabang(int startNode);
void dfsHelper(int u, int visited[], int *urutan);
void dfsTelusurJaringan(int startNode);
int hitungDijkstra(int start, int goal, int cetakRute);

/* --- UI & MEMORI --- */
void menuEksplorasiKatalogPelanggan(void);
void menuDashboardPelanggan(void);
void menuKasirOfflineStaf(void);
void menuManajemenMenuBST(void);
void menuManajemenCabangDanPeta(void);
void menuDashboardStaf(void);
void freeAllMemory(void);

#endif