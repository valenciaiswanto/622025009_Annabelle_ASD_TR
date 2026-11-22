#include "../include/restoran.h"

MenuBSTNode *buatBSTNode(const char *kode, const char *nama, const char *kategori, int harga) {
    MenuBSTNode *baru = (MenuBSTNode *)malloc(sizeof(MenuBSTNode));
    if (baru == NULL) return NULL;
    strcpy(baru->kode, kode);
    strcpy(baru->nama, nama);
    strcpy(baru->kategori, kategori);
    baru->harga = harga;
    baru->left = NULL;
    baru->right = NULL;
    return baru;
}

MenuBSTNode *insertMenuBST(MenuBSTNode *root, const char *kode, const char *nama, const char *kategori, int harga) {
    int cmp;
    if (root == NULL) {
        return buatBSTNode(kode, nama, kategori, harga);
    }
    cmp = bandingkanStringIgnoreCase(kode, root->kode);
    if (cmp < 0) {
        root->left = insertMenuBST(root->left, kode, nama, kategori, harga);
    } else if (cmp > 0) {
        root->right = insertMenuBST(root->right, kode, nama, kategori, harga);
    }
    return root;
}

MenuBSTNode *searchMenuBST(MenuBSTNode *root, const char *kode) {
    int cmp;
    if (root == NULL) return NULL;
    cmp = bandingkanStringIgnoreCase(kode, root->kode);
    if (cmp == 0) return root;
    if (cmp < 0) return searchMenuBST(root->left, kode);
    return searchMenuBST(root->right, kode);
}

MenuBSTNode *findMinBST(MenuBSTNode *node) {
    MenuBSTNode *curr = node;
    while (curr != NULL && curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}

MenuBSTNode *deleteMenuBST(MenuBSTNode *root, const char *kode, int *berhasilHapus) {
    int cmp;
    if (root == NULL) return NULL;

    cmp = bandingkanStringIgnoreCase(kode, root->kode);
    if (cmp < 0) {
        root->left = deleteMenuBST(root->left, kode, berhasilHapus);
    } else if (cmp > 0) {
        root->right = deleteMenuBST(root->right, kode, berhasilHapus);
    } else {
        *berhasilHapus = 1;
        if (root->left == NULL) {
            MenuBSTNode *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            MenuBSTNode *temp = root->left;
            free(root);
            return temp;
        }
        MenuBSTNode *temp = findMinBST(root->right);
        strcpy(root->kode, temp->kode);
        strcpy(root->nama, temp->nama);
        strcpy(root->kategori, temp->kategori);
        root->harga = temp->harga;
        root->right = deleteMenuBST(root->right, temp->kode, berhasilHapus);
    }
    return root;
}

void preOrderMenuBST(MenuBSTNode *root) {
    if (root == NULL) return;
    printf("  [%-4s] %-24s | %-8s | Rp %d\n", root->kode, root->nama, root->kategori, root->harga);
    preOrderMenuBST(root->left);
    preOrderMenuBST(root->right);
}

void inOrderMenuBST(MenuBSTNode *root) {
    if (root == NULL) return;
    inOrderMenuBST(root->left);
    printf("  [%-4s] %-24s | %-8s | Rp %d\n", root->kode, root->nama, root->kategori, root->harga);
    inOrderMenuBST(root->right);
}

void cetakKatalogTabelBST(MenuBSTNode *root) {
    if (root == NULL) return;
    cetakKatalogTabelBST(root->left);
    printf("%-6s | %-24s | %-8s | Rp %d\n",
           root->kode, root->nama, root->kategori, root->harga);
    cetakKatalogTabelBST(root->right);
}

void postOrderMenuBST(MenuBSTNode *root) {
    if (root == NULL) return;
    postOrderMenuBST(root->left);
    postOrderMenuBST(root->right);
    printf("  [%-4s] %-24s | %-8s | Rp %d\n", root->kode, root->nama, root->kategori, root->harga);
}

void freeMenuBST(MenuBSTNode *root) {
    if (root == NULL) return;
    freeMenuBST(root->left);
    freeMenuBST(root->right);
    free(root);
}

ZonaNode *buatZonaNode(const char *nama, int idGraph, int isBuka) {
    ZonaNode *baru = (ZonaNode *)malloc(sizeof(ZonaNode));
    if (baru == NULL) return NULL;
    strcpy(baru->nama_zona, nama);
    baru->id_lokasi_graph = idGraph;
    baru->is_buka = isBuka;
    baru->antrean_aktif = 0;
    baru->firstChild = NULL;
    baru->nextSibling = NULL;
    return baru;
}

void tambahChildZona(ZonaNode *parent, ZonaNode *child) {
    if (parent == NULL || child == NULL) return;
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    } else {
        ZonaNode *temp = parent->firstChild;
        while (temp->nextSibling != NULL) temp = temp->nextSibling;
        temp->nextSibling = child;
    }
}

void initTreeWilayahCabang(void) {
    rootWilayahTree = buatZonaNode("SEMUA CABANG KOTA", -1, 1);

    ZonaNode *zonaUtara = buatZonaNode("WILAYAH UTARA", -1, 1);
    ZonaNode *zonaSelatan = buatZonaNode("WILAYAH SELATAN", -1, 1);

    tambahChildZona(rootWilayahTree, zonaUtara);
    tambahChildZona(rootWilayahTree, zonaSelatan);

    tambahChildZona(zonaUtara, buatZonaNode(namaLokasi[0], 0, 1));
    tambahChildZona(zonaUtara, buatZonaNode(namaLokasi[1], 1, 1));

    tambahChildZona(zonaSelatan, buatZonaNode(namaLokasi[2], 2, 1));
    tambahChildZona(zonaSelatan, buatZonaNode(namaLokasi[3], 3, 1));
}

void tampilkanTreeWilayah(ZonaNode *node, int level) {
    int i;
    if (node == NULL) return;
    for (i = 0; i < level; i++) printf("    ");
    if (node->id_lokasi_graph == -1) {
        printf("|-- [%s]\n", node->nama_zona);
    } else {
        printf("|-- [ID:%d] %s | Status: %s | Antrean: %d pesanan\n",
               node->id_lokasi_graph, node->nama_zona,
               node->is_buka ? "BUKA" : "TUTUP",
               node->antrean_aktif);
    }
    tampilkanTreeWilayah(node->firstChild, level + 1);
    tampilkanTreeWilayah(node->nextSibling, level);
}

ZonaNode *cariNodeCabangByGraphID(ZonaNode *node, int idGraph) {
    ZonaNode *res;
    if (node == NULL) return NULL;
    if (node->id_lokasi_graph == idGraph) return node;
    res = cariNodeCabangByGraphID(node->firstChild, idGraph);
    if (res != NULL) return res;
    return cariNodeCabangByGraphID(node->nextSibling, idGraph);
}

ZonaNode *tentukanCabangViaTreeDanGraph(int idTujuanPelanggan, int *jarakTerpilih) {
    int targetZona = zonaLokasi[idTujuanPelanggan];
    const char *namaTargetZona = (targetZona == 1) ? "WILAYAH UTARA" : "WILAYAH SELATAN";

    ZonaNode *zonaNode = rootWilayahTree->firstChild;
    ZonaNode *cabangTerbaik = NULL;
    int jarakMin = INF;

    printf("\n[1] Mencari Cabang Terdekat yang Buka...\n");
    printf("    Alamat Kirim   : %s\n", namaLokasi[idTujuanPelanggan]);
    printf("    Area           : %s\n", namaTargetZona);

    while (zonaNode != NULL) {
        if (strcmp(zonaNode->nama_zona, namaTargetZona) == 0) break;
        zonaNode = zonaNode->nextSibling;
    }

    if (zonaNode != NULL) {
        ZonaNode *cabang = zonaNode->firstChild;
        while (cabang != NULL) {
            if (cabang->is_buka) {
                int d = hitungDijkstra(cabang->id_lokasi_graph, idTujuanPelanggan, 0);
                printf("    -> Cek %s : BUKA (Jarak: %d km)\n", cabang->nama_zona, d);
                if (d != -1 && d < jarakMin) {
                    jarakMin = d;
                    cabangTerbaik = cabang;
                }
            } else {
                printf("    -> Cek %s : [LEWATI - LAGI TUTUP]\n", cabang->nama_zona);
            }
            cabang = cabang->nextSibling;
        }
    }

    if (cabangTerbaik == NULL) {
        printf("\n[!] Semua cabang di %s lagi tutup!\n", namaTargetZona);
        printf("[!] Mencari cabang yang buka di wilayah sebelah...\n");
        ZonaNode *zLain = rootWilayahTree->firstChild;
        while (zLain != NULL) {
            if (strcmp(zLain->nama_zona, namaTargetZona) != 0) {
                ZonaNode *cabang = zLain->firstChild;
                while (cabang != NULL) {
                    if (cabang->is_buka) {
                        int d = hitungDijkstra(cabang->id_lokasi_graph, idTujuanPelanggan, 0);
                        printf("    -> Cek %s : BUKA (Jarak: %d km)\n", cabang->nama_zona, d);
                        if (d != -1 && d < jarakMin) {
                            jarakMin = d;
                            cabangTerbaik = cabang;
                        }
                    }
                    cabang = cabang->nextSibling;
                }
            }
            zLain = zLain->nextSibling;
        }
    }

    *jarakTerpilih = jarakMin;
    return cabangTerbaik;
}

void freeZonaTree(ZonaNode *node) {
    if (node == NULL) return;
    freeZonaTree(node->firstChild);
    freeZonaTree(node->nextSibling);
    free(node);
}