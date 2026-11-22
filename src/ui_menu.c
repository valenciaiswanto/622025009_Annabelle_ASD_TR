#include "../include/restoran.h"

void menuEksplorasiKatalogPelanggan(void) {
    int pilihan, n, i;
    MenuNode *daftar[MAX_MENU];

    while (1) {
        clearScreen();
        printf("====================================================================\n");
        printf("                       CARI & URUTKAN MENU                          \n");
        printf("====================================================================\n");
        printf("1. Urutkan Harga (Termurah / Termahal)\n");
        printf("2. Urutkan Nama Menu (A-Z / Z-A)\n");
        printf("3. Lihat Menu Paling Laris\n");
        printf("4. Cari Menu Sesuai Budget Harga Pas\n");
        printf("5. Cari Menu Pakai Nama atau Kode\n");
        printf("6. Lihat Semua Menu Urut Kode (E1..M4)\n");
        printf("0. Kembali\n");
        printf("====================================================================\n");
        printf("Pilih: ");

        if (!bacaAngkaValid(&pilihan)) continue;
        n = ekstrakSemuaMenuArray(daftar, MAX_MENU);

        if (pilihan == 1) {
            int arah;
            printf("\n1. Termurah dulu | 2. Termahal dulu: ");
            if (bacaAngkaValid(&arah) && (arah == 1 || arah == 2)) {
                selectionSortHarga(daftar, n, arah == 1);
                printf("\n--------------------------------------------------------------------\n");
                for (i = 0; i < n; i++) {
                    printf("%2d. [%s] %-24s | Rp %d\n", i + 1, daftar[i]->kode, daftar[i]->nama, daftar[i]->harga);
                }
                printf("--------------------------------------------------------------------\n");
            }
            pauseScreen();

        } else if (pilihan == 2) {
            int arah;
            printf("\n1. A ke Z | 2. Z ke A: ");
            if (bacaAngkaValid(&arah) && (arah == 1 || arah == 2)) {
                insertionSortNama(daftar, n, arah == 1);
                printf("\n--------------------------------------------------------------------\n");
                for (i = 0; i < n; i++) {
                    printf("%2d. [%s] %-24s | Rp %d\n", i + 1, daftar[i]->kode, daftar[i]->nama, daftar[i]->harga);
                }
                printf("--------------------------------------------------------------------\n");
            }
            pauseScreen();

        } else if (pilihan == 3) {
            selectionSortTerlaris(daftar, n, 0);
            printf("\n--------------------------------------------------------------------\n");
            for (i = 0; i < n; i++) {
                printf("%2d. [%s] %-24s | Laku: %d porsi\n", i + 1, daftar[i]->kode, daftar[i]->nama, daftar[i]->total_terjual);
            }
            printf("--------------------------------------------------------------------\n");
            pauseScreen();

        } else if (pilihan == 4) {
            int target;
            selectionSortHarga(daftar, n, 1);
            printf("\nMasukkan harga pas yang dicari: Rp ");
            if (bacaAngkaValid(&target)) {
                MenuNode *res = binarySearchMenuHarga(daftar, n, target);
                if (res != NULL) {
                    printf("[+] Ketemu: [%s] %s (Rp %d)\n", res->kode, res->nama, res->harga);
                } else {
                    printf("[!] Nggak ada menu yang harganya pas Rp %d.\n", target);
                }
            }
            pauseScreen();

        } else if (pilihan == 5) {
            char kw[MAX_STR];
            printf("\nKetik nama menu atau kodenya: ");
            if (fgets(kw, sizeof(kw), stdin) != NULL) {
                hapusNewline(kw);
                sequentialSearchMenu(daftar, n, kw);
            }
            pauseScreen();

        } else if (pilihan == 6) {
            clearScreen();
            printf("====================================================================\n");
            printf("                    SEMUA MENU URUT KODE (E1..M4)                   \n");
            printf("====================================================================\n");
            if (rootMenuBST == NULL) {
                printf("[*] Daftar menu masih kosong.\n");
            } else {
                inOrderMenuBST(rootMenuBST);
            }
            printf("====================================================================\n");
            pauseScreen();

        } else if (pilihan == 0) {
            break;
        }
    }
}

void menuDashboardPelanggan(void) {
    int pilihan;
    jumlah_item_keranjang = 0;
    clearUndoStack();

    while (1) {
        int i;
        clearScreen();
        printf("====================================================================\n");
        printf("        MENU PEMESANAN PELANGGAN - Halo, %-20s       \n", current_user);
        printf("====================================================================\n");
        printf("%-6s | %-24s | %-8s | %-12s\n", "Kode", "Nama Menu", "Kategori", "Harga");
        printf("--------------------------------------------------------------------\n");
        cetakKatalogTabelBST(rootMenuBST);
        printf("====================================================================\n");
        printf("[Isi Keranjang: %d macam menu]\n", jumlah_item_keranjang);
        printf("1. Tambah Menu ke Keranjang\n");
        printf("2. Cari & Urutkan Menu\n");
        printf("3. Batalin Pesanan Terakhir di Keranjang\n");
        printf("4. Lanjut Bayar / Checkout\n");
        printf("5. Cek Status & Nomor Antrean Saya\n");
        printf("6. Cek Daftar Cabang Terdekat dari Rumahmu\n");
        printf("0. Keluar Akun (Logout)\n");
        printf("Pilih: ");

        if (!bacaAngkaValid(&pilihan)) continue;

        if (pilihan == 1) {
            char kode[MAX_KODE];
            int qty, found = 0, j;
            MenuNode *m;
            printf("Masukkan Kode Menu (contoh: E1): ");
            if (fgets(kode, sizeof(kode), stdin) == NULL) continue;
            hapusNewline(kode);
            m = cariMenuHash(kode);
            if (m == NULL) {
                printf("[!] Kode menu nggak ketemu!\n");
                pauseScreen();
                continue;
            }
            printf("Mau pesan berapa porsi %s? ", m->nama);
            if (!bacaAngkaValid(&qty) || qty <= 0) {
                printf("[!] Jumlah porsi minimal 1 ya.\n");
                pauseScreen();
                continue;
            }
            for (j = 0; j < jumlah_item_keranjang; j++) {
                if (bandingkanStringIgnoreCase(keranjangBelanja[j].item_menu->kode, m->kode) == 0) {
                    keranjangBelanja[j].qty += qty;
                    found = 1;
                    break;
                }
            }
            if (!found && jumlah_item_keranjang < MAX_KERANJANG) {
                keranjangBelanja[jumlah_item_keranjang].item_menu = m;
                keranjangBelanja[jumlah_item_keranjang].qty = qty;
                jumlah_item_keranjang++;
            }
            pushUndoStack(m->kode, qty);
            printf("[+] %dx %s sudah masuk keranjang!\n", qty, m->nama);
            pauseScreen();

        } else if (pilihan == 2) {
            menuEksplorasiKatalogPelanggan();

        } else if (pilihan == 3) {
            undoKeranjang();
            pauseScreen();

        } else if (pilihan == 4) {
            if (jumlah_item_keranjang == 0) {
                printf("[!] Keranjang kamu masih kosong!\n");
                pauseScreen();
                continue;
            }

            int metodeOrder;
            clearScreen();
            printf("====================================================================\n");
            printf("                        METODE PENGAMBILAN                          \n");
            printf("====================================================================\n");
            printf("1. DELIVERY  (Diantar Kurir dari Cabang Terdekat)\n");
            printf("2. TAKE AWAY (Ambil Sendiri di Cabang Pilihanmu)\n");
            printf("0. Batal\n");
            printf("Pilih (1/2/0): ");

            if (!bacaAngkaValid(&metodeOrder) || (metodeOrder != 1 && metodeOrder != 2)) {
                if (metodeOrder != 0) {
                    printf("[!] Pilihan salah.\n");
                    pauseScreen();
                }
                continue;
            }

            int j, subtotal = 0;
            char rincianStr[256] = "";

            if (metodeOrder == 1) {
                int idTujuan, jarakKm = 0;
                printf("\nPilih Wilayah Alamat Pengirimanmu:\n");
                for (i = JUMLAH_CABANG; i < jumlahLokasi; i++) {
                    printf("  [%d] %s\n", i, namaLokasi[i]);
                }
                printf("Masukkan Nomor Wilayah (%d-%d): ", JUMLAH_CABANG, jumlahLokasi - 1);
                if (!bacaAngkaValid(&idTujuan) || idTujuan < JUMLAH_CABANG || idTujuan >= jumlahLokasi) {
                    printf("[!] Nomor wilayah salah!\n");
                    pauseScreen();
                    continue;
                }

                ZonaNode *cabangTerpilih = tentukanCabangViaTreeDanGraph(idTujuan, &jarakKm);
                if (cabangTerpilih == NULL) {
                    printf("\n[X] Maaf, semua cabang lagi tutup. Coba lagi nanti ya.\n");
                    pauseScreen();
                    continue;
                }

                printf("\n[2] Cabang yang Memproses & Rute Kurir:\n");
                printf("    Cabang             : %s\n", cabangTerpilih->nama_zona);
                hitungDijkstra(cabangTerpilih->id_lokasi_graph, idTujuan, 1);

                int ongkir = jarakKm * 3000;
                printf("\n--------------------------------------------------------------------\n");
                for (j = 0; j < jumlah_item_keranjang; j++) {
                    char itemBuf[128];
                    int sub = keranjangBelanja[j].item_menu->harga * keranjangBelanja[j].qty;
                    subtotal += sub;
                    keranjangBelanja[j].item_menu->total_terjual += keranjangBelanja[j].qty;
                    printf("- %-24s (x%d) = Rp %d\n",
                           keranjangBelanja[j].item_menu->nama, keranjangBelanja[j].qty, sub);

                    snprintf(itemBuf, sizeof(itemBuf), "%s(x%d)%s",
                             keranjangBelanja[j].item_menu->nama, keranjangBelanja[j].qty,
                             (j < jumlah_item_keranjang - 1) ? ", " : "");
                    if (strlen(rincianStr) + strlen(itemBuf) < sizeof(rincianStr) - 1) {
                        strcat(rincianStr, itemBuf);
                    }
                }
                printf("--------------------------------------------------------------------\n");
                printf("Harga Makanan : Rp %d\n", subtotal);
                printf("Ongkos Kirim  : Rp %d (%d km x Rp 3.000)\n", ongkir, jarakKm);
                printf("TOTAL BAYAR   : Rp %d\n", subtotal + ongkir);
                printf("====================================================================\n");

                cabangTerpilih->antrean_aktif++;
                enqueueOrder("DELIVERY", current_user, namaLokasi[idTujuan],
                             cabangTerpilih->id_lokasi_graph, cabangTerpilih->nama_zona,
                             rincianStr, subtotal + ongkir);

            } else {
                int idCabangPilih;
                printf("\nDaftar Cabang Restoran untuk Take Away:\n");
                for (i = 0; i < JUMLAH_CABANG; i++) {
                    ZonaNode *cNode = cariNodeCabangByGraphID(rootWilayahTree, i);
                    printf("  [%d] %-34s | Status: %-6s | Antrean: %d\n",
                           i, namaLokasi[i],
                           (cNode && cNode->is_buka) ? "BUKA" : "TUTUP",
                           cNode ? cNode->antrean_aktif : 0);
                }
                printf("Pilih Nomor Cabang (0-%d): ", JUMLAH_CABANG - 1);
                if (!bacaAngkaValid(&idCabangPilih) || idCabangPilih < 0 || idCabangPilih >= JUMLAH_CABANG) {
                    printf("[!] Nomor cabang salah!\n");
                    pauseScreen();
                    continue;
                }

                ZonaNode *cabangDipilih = cariNodeCabangByGraphID(rootWilayahTree, idCabangPilih);
                if (cabangDipilih == NULL || !cabangDipilih->is_buka) {
                    printf("\n[X] Maaf, %s lagi TUTUP! Pilih cabang lain ya.\n",
                           namaLokasi[idCabangPilih]);
                    pauseScreen();
                    continue;
                }

                printf("\n--------------------------------------------------------------------\n");
                printf("Cara Pesan : TAKE AWAY (Ambil Sendiri)\n");
                printf("Cabang     : %s\n", cabangDipilih->nama_zona);
                printf("--------------------------------------------------------------------\n");
                for (j = 0; j < jumlah_item_keranjang; j++) {
                    char itemBuf[128];
                    int sub = keranjangBelanja[j].item_menu->harga * keranjangBelanja[j].qty;
                    subtotal += sub;
                    keranjangBelanja[j].item_menu->total_terjual += keranjangBelanja[j].qty;
                    printf("- %-24s (x%d) = Rp %d\n",
                           keranjangBelanja[j].item_menu->nama, keranjangBelanja[j].qty, sub);

                    snprintf(itemBuf, sizeof(itemBuf), "%s(x%d)%s",
                             keranjangBelanja[j].item_menu->nama, keranjangBelanja[j].qty,
                             (j < jumlah_item_keranjang - 1) ? ", " : "");
                    if (strlen(rincianStr) + strlen(itemBuf) < sizeof(rincianStr) - 1) {
                        strcat(rincianStr, itemBuf);
                    }
                }
                printf("--------------------------------------------------------------------\n");
                printf("Ongkos Kirim : Rp 0\n");
                printf("TOTAL BAYAR  : Rp %d\n", subtotal);
                printf("====================================================================\n");

                cabangDipilih->antrean_aktif++;
                enqueueOrder("TAKEAWAY-ONLINE", current_user, "Ambil Sendiri di Cabang",
                             cabangDipilih->id_lokasi_graph, cabangDipilih->nama_zona,
                             rincianStr, subtotal);
            }

            jumlah_item_keranjang = 0;
            clearUndoStack();
            pauseScreen();

        } else if (pilihan == 5) {
            cekStatusPesananPelanggan(current_user);

        } else if (pilihan == 6) {
            int idWil;
            clearScreen();
            printf("Pilih Wilayah Rumahmu untuk Cek Cabang Terdekat:\n");
            for (i = JUMLAH_CABANG; i < jumlahLokasi; i++) {
                printf("  [%d] %s\n", i, namaLokasi[i]);
            }
            printf("Masukkan Nomor Wilayah (%d-%d): ", JUMLAH_CABANG, jumlahLokasi - 1);
            if (bacaAngkaValid(&idWil) && idWil >= JUMLAH_CABANG && idWil < jumlahLokasi) {
                bfsJangkauanCabang(idWil);
            } else {
                printf("[!] Nomor wilayah salah.\n");
            }
            pauseScreen();

        } else if (pilihan == 0) {
            strcpy(current_user, "");
            strcpy(current_role, "");
            jumlah_item_keranjang = 0;
            clearUndoStack();
            break;
        }
    }
}

void menuKasirOfflineStaf(void) {
    int pilihan;
    jumlah_item_keranjang = 0;
    clearUndoStack();

    while (1) {
        clearScreen();
        printf("====================================================================\n");
        printf("      KASIR OFFLINE - %s\n", namaLokasi[current_cabang_staf]);
        printf("====================================================================\n");
        printf("%-6s | %-24s | %-8s | %-12s\n", "Kode", "Nama Menu", "Kategori", "Harga");
        printf("--------------------------------------------------------------------\n");
        cetakKatalogTabelBST(rootMenuBST);
        printf("====================================================================\n");
        printf("[Keranjang : %d macam menu]\n", jumlah_item_keranjang);
        printf("1. Tambah Menu ke Keranjang\n");
        printf("2. Batalin Pesanan Terakhir\n");
        printf("3. Selesai & Masukkan ke Antrean %s\n", namaLokasi[current_cabang_staf]);
        printf("0. Kembali\n");
        printf("Pilih: ");

        if (!bacaAngkaValid(&pilihan)) continue;

        if (pilihan == 1) {
            char kode[MAX_KODE];
            int qty, found = 0, j;
            MenuNode *m;
            printf("Masukkan Kode Menu (contoh: E1): ");
            if (fgets(kode, sizeof(kode), stdin) == NULL) continue;
            hapusNewline(kode);
            m = cariMenuHash(kode);
            if (m == NULL) {
                printf("[!] Kode menu nggak ketemu!\n");
                pauseScreen();
                continue;
            }
            printf("Jumlah porsi %s: ", m->nama);
            if (!bacaAngkaValid(&qty) || qty <= 0) {
                printf("[!] Jumlah porsi minimal 1.\n");
                pauseScreen();
                continue;
            }
            for (j = 0; j < jumlah_item_keranjang; j++) {
                if (bandingkanStringIgnoreCase(keranjangBelanja[j].item_menu->kode, m->kode) == 0) {
                    keranjangBelanja[j].qty += qty;
                    found = 1;
                    break;
                }
            }
            if (!found && jumlah_item_keranjang < MAX_KERANJANG) {
                keranjangBelanja[jumlah_item_keranjang].item_menu = m;
                keranjangBelanja[jumlah_item_keranjang].qty = qty;
                jumlah_item_keranjang++;
            }
            pushUndoStack(m->kode, qty);
            printf("[+] %dx %s masuk keranjang!\n", qty, m->nama);
            pauseScreen();

        } else if (pilihan == 2) {
            undoKeranjang();
            pauseScreen();

        } else if (pilihan == 3) {
            if (jumlah_item_keranjang == 0) {
                printf("[!] Keranjang masih kosong!\n");
                pauseScreen();
                continue;
            }

            ZonaNode *cabangNode = cariNodeCabangByGraphID(rootWilayahTree, current_cabang_staf);
            if (cabangNode == NULL || !cabangNode->is_buka) {
                printf("[!] Cabang kamu (%s) statusnya lagi TUTUP! Buka dulu di menu Kelola Cabang.\n",
                       namaLokasi[current_cabang_staf]);
                pauseScreen();
                continue;
            }

            char namaTamu[MAX_STR], ketMeja[MAX_STR];
            int j, total = 0;
            char rincianStr[256] = "";

            clearScreen();
            printf("====================================================================\n");
            printf("      PESANAN OFFLINE - %s\n", namaLokasi[current_cabang_staf]);
            printf("====================================================================\n");
            printf("Nama Pembeli : ");
            if (fgets(namaTamu, sizeof(namaTamu), stdin) == NULL) continue;
            hapusNewline(namaTamu);
            if (isInputKosong(namaTamu)) strcpy(namaTamu, "Tamu Langsung");

            printf("Nomor Meja / Bungkus (contoh: Meja 03 / Bungkus) : ");
            if (fgets(ketMeja, sizeof(ketMeja), stdin) == NULL) continue;
            hapusNewline(ketMeja);
            if (isInputKosong(ketMeja)) strcpy(ketMeja, "Makan di Tempat");

            printf("\n--------------------------------------------------------------------\n");
            for (j = 0; j < jumlah_item_keranjang; j++) {
                char itemBuf[128];
                int sub = keranjangBelanja[j].item_menu->harga * keranjangBelanja[j].qty;
                total += sub;
                keranjangBelanja[j].item_menu->total_terjual += keranjangBelanja[j].qty;
                printf("- %-24s (x%d) = Rp %d\n",
                       keranjangBelanja[j].item_menu->nama, keranjangBelanja[j].qty, sub);

                snprintf(itemBuf, sizeof(itemBuf), "%s(x%d)%s",
                         keranjangBelanja[j].item_menu->nama, keranjangBelanja[j].qty,
                         (j < jumlah_item_keranjang - 1) ? ", " : "");
                if (strlen(rincianStr) + strlen(itemBuf) < sizeof(rincianStr) - 1) {
                    strcat(rincianStr, itemBuf);
                }
            }
            printf("--------------------------------------------------------------------\n");
            printf("TOTAL BAYAR : Rp %d\n", total);
            printf("====================================================================\n");

            cabangNode->antrean_aktif++;
            enqueueOrder("OFFLINE", namaTamu, ketMeja,
                         current_cabang_staf, namaLokasi[current_cabang_staf], rincianStr, total);

            jumlah_item_keranjang = 0;
            clearUndoStack();
            pauseScreen();

        } else if (pilihan == 0) {
            jumlah_item_keranjang = 0;
            clearUndoStack();
            break;
        }
    }
}

void menuManajemenMenuBST(void) {
    int pilihan;
    while (1) {
        clearScreen();
        printf("====================================================================\n");
        printf("                    KELOLA DAFTAR MENU RESTORAN                     \n");
        printf("====================================================================\n");
        printf("1. Tambah Menu Baru\n");
        printf("2. Cari Menu Pakai Kode\n");
        printf("3. Hapus Menu\n");
        printf("4. Lihat Semua Daftar Menu\n");
        printf("5. Muat Ulang Menu dari File (%s)\n", FILE_DATABASE_MENU);
        printf("0. Kembali\n");
        printf("Pilih: ");

        if (!bacaAngkaValid(&pilihan)) continue;

        if (pilihan == 1) {
            char kode[MAX_KODE], nama[MAX_STR], kategori[20];
            int harga, i;

            printf("\nKode Menu Baru (contoh: E9 / M5): ");
            if (fgets(kode, sizeof(kode), stdin) == NULL) continue;
            hapusNewline(kode);
            for (i = 0; kode[i]; i++) kode[i] = (char)toupper((unsigned char)kode[i]);

            if (isInputKosong(kode) || strchr(kode, '|') != NULL) {
                printf("[!] Kode menu nggak valid.\n");
                pauseScreen();
                continue;
            }
            if (searchMenuBST(rootMenuBST, kode) != NULL) {
                printf("[!] Kode '%s' sudah ada! Pakai kode lain ya.\n", kode);
                pauseScreen();
                continue;
            }

            printf("Nama Menu Baru : ");
            if (fgets(nama, sizeof(nama), stdin) == NULL) continue;
            hapusNewline(nama);
            if (isInputKosong(nama) || strchr(nama, '|') != NULL) {
                printf("[!] Nama menu nggak valid.\n");
                pauseScreen();
                continue;
            }

            printf("Jenis (Makanan / Minuman) : ");
            if (fgets(kategori, sizeof(kategori), stdin) == NULL) continue;
            hapusNewline(kategori);
            if (isInputKosong(kategori) || strchr(kategori, '|') != NULL) {
                printf("[!] Jenis menu nggak valid.\n");
                pauseScreen();
                continue;
            }

            printf("Harga (Rp) : ");
            if (!bacaAngkaValid(&harga) || harga <= 0) {
                printf("[!] Harga harus lebih dari 0.\n");
                pauseScreen();
                continue;
            }

            rootMenuBST = insertMenuBST(rootMenuBST, kode, nama, kategori, harga);
            insertHashTable(kode, nama, kategori, harga);
            simpanSemuaMenuKeFile();
            printf("\n[+] Menu [%s] %s berhasil ditambahkan dan disimpan!\n", kode, nama);
            pauseScreen();

        } else if (pilihan == 2) {
            char kode[MAX_KODE];
            MenuBSTNode *hasil;
            printf("\nMasukkan Kode Menu yang dicari: ");
            if (fgets(kode, sizeof(kode), stdin) == NULL) continue;
            hapusNewline(kode);

            hasil = searchMenuBST(rootMenuBST, kode);
            if (hasil != NULL) {
                printf("\n[+] MENU KETEMU:\n");
                printf("    Kode  : %s\n", hasil->kode);
                printf("    Nama  : %s\n", hasil->nama);
                printf("    Jenis : %s\n", hasil->kategori);
                printf("    Harga : Rp %d\n", hasil->harga);
            } else {
                printf("\n[!] Menu dengan kode '%s' nggak ketemu.\n", kode);
            }
            pauseScreen();

        } else if (pilihan == 3) {
            char kode[MAX_KODE];
            int berhasil = 0;
            printf("\nMasukkan Kode Menu yang mau dihapus: ");
            if (fgets(kode, sizeof(kode), stdin) == NULL) continue;
            hapusNewline(kode);

            rootMenuBST = deleteMenuBST(rootMenuBST, kode, &berhasil);
            if (berhasil) {
                simpanSemuaMenuKeFile();
                loadDataMenu();
                printf("[+] Menu '%s' berhasil dihapus!\n", kode);
            } else {
                printf("[!] Gagal hapus: Kode '%s' nggak ketemu.\n", kode);
            }
            pauseScreen();

        } else if (pilihan == 4) {
            clearScreen();
            printf("====================================================================\n");
            printf("                      DAFTAR MENU RESTORAN                          \n");
            printf("====================================================================\n");
            if (rootMenuBST == NULL) {
                printf("[*] Daftar menu masih kosong.\n");
            } else {
                printf("[1] URUT KODE MENU:\n");
                inOrderMenuBST(rootMenuBST);
                printf("\n[2] URUTAN DARI MENU UTAMA:\n");
                preOrderMenuBST(rootMenuBST);
                printf("\n[3] URUTAN DARI CABANG MENU BAWAH:\n");
                postOrderMenuBST(rootMenuBST);
            }
            printf("====================================================================\n");
            pauseScreen();

        } else if (pilihan == 5) {
            loadDataMenu();
            printf("\n[+] Daftar menu berhasil dimuat ulang dari '%s'!\n", FILE_DATABASE_MENU);
            pauseScreen();

        } else if (pilihan == 0) {
            break;
        }
    }
}

void menuManajemenCabangDanPeta(void) {
    int pilihan;
    while (1) {
        clearScreen();
        printf("====================================================================\n");
        printf("                  KELOLA CABANG & CEK PETA WILAYAH                  \n");
        printf("====================================================================\n");
        tampilkanTreeWilayah(rootWilayahTree, 0);
        printf("\n====================================================================\n");
        printf("1. Buka / Tutup Cabang\n");
        printf("2. Lihat Peta Jalan Antar-Cabang & Wilayah\n");
        printf("3. Cek Jarak & Rute Terdekat ke Rumah Pelanggan\n");
        printf("4. Cek Cabang Terdekat dari Cabang Kamu\n");
        printf("5. Cek Semua Jalan Apakah Nyambung\n");
        printf("0. Kembali\n");
        printf("Pilih: ");

        if (!bacaAngkaValid(&pilihan)) continue;

        if (pilihan == 1) {
            int idCab, st;
            printf("Masukkan Nomor Cabang (0-%d): ", JUMLAH_CABANG - 1);
            if (bacaAngkaValid(&idCab) && idCab >= 0 && idCab < JUMLAH_CABANG) {
                ZonaNode *nodeCab = cariNodeCabangByGraphID(rootWilayahTree, idCab);
                printf("Ubah Status (1 = BUKA, 0 = TUTUP): ");
                if (nodeCab != NULL && bacaAngkaValid(&st) && (st == 0 || st == 1)) {
                    nodeCab->is_buka = st;
                    printf("[+] Status %s sekarang: %s!\n",
                           nodeCab->nama_zona, st ? "BUKA" : "TUTUP");
                }
            } else {
                printf("[!] Nomor cabang salah.\n");
            }
            pauseScreen();

        } else if (pilihan == 2) {
            tampilkanPetaJaringanKota();
            pauseScreen();

        } else if (pilihan == 3) {
            int asal, tujuan, i;
            clearScreen();
            printf("====================================================================\n");
            printf("               CEK JARAK & RUTE PENGIRIMAN KE PELANGGAN             \n");
            printf("====================================================================\n");
            printf("Pilih Cabang Asal:\n");
            for (i = 0; i < JUMLAH_CABANG; i++) {
                printf("  [%d] %s\n", i, namaLokasi[i]);
            }
            printf("\nPilih Wilayah Tujuan:\n");
            for (i = JUMLAH_CABANG; i < jumlahLokasi; i++) {
                printf("  [%d] %s\n", i, namaLokasi[i]);
            }
            printf("--------------------------------------------------------------------\n");
            printf("Nomor Cabang Asal (0-%d)      : ", JUMLAH_CABANG - 1);
            if (!bacaAngkaValid(&asal) || asal < 0 || asal >= JUMLAH_CABANG) continue;
            printf("Nomor Wilayah Tujuan (%d-%d)  : ", JUMLAH_CABANG, jumlahLokasi - 1);
            if (!bacaAngkaValid(&tujuan) || tujuan < JUMLAH_CABANG || tujuan >= jumlahLokasi) continue;
            printf("\n");
            hitungDijkstra(asal, tujuan, 1);
            pauseScreen();

        } else if (pilihan == 4) {
            clearScreen();
            bfsJangkauanCabang(current_cabang_staf);
            pauseScreen();

        } else if (pilihan == 5) {
            clearScreen();
            dfsTelusurJaringan(current_cabang_staf);
            pauseScreen();

        } else if (pilihan == 0) {
            break;
        }
    }
}

void menuDashboardStaf(void) {
    int pilihan;
    while (1) {
        clearScreen();
        printf("====================================================================\n");
        printf("  MENU STAF: %-12s | %s\n", current_user, namaLokasi[current_cabang_staf]);
        printf("====================================================================\n");
        printf("1. Kasir offline\n");
        printf("2. Lihat & Selesaikan Antrean Pesanan\n");
        printf("3. Kelola Cabang & Cek Peta Wilayah\n");
        printf("4. Kelola Daftar Menu (Tambah / Cari / Hapus Menu)\n");
        printf("0. Keluar Akun (Logout)\n");
        printf("Pilih: ");

        if (!bacaAngkaValid(&pilihan)) continue;

        if (pilihan == 1) {
            menuKasirOfflineStaf();
        } else if (pilihan == 2) {
            int sub;
            while (1) {
                tampilkanAntreanPesananStaf();
                if (!bacaAngkaValid(&sub)) continue;
                if (sub == 1) {
                    dequeueOrderSelesaiCabang(current_cabang_staf);
                    pauseScreen();
                } else if (sub == 0) {
                    break;
                }
            }
        } else if (pilihan == 3) {
            menuManajemenCabangDanPeta();
        } else if (pilihan == 4) {
            menuManajemenMenuBST();
        } else if (pilihan == 0) {
            strcpy(current_user, "");
            strcpy(current_role, "");
            current_cabang_staf = -1;
            break;
        }
    }
}