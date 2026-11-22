#include "../include/restoran.h"

void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen(void) {
    printf("\nTekan ENTER untuk lanjut...");
    while (getchar() != '\n');
}

void hapusNewline(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

int isInputKosong(const char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) return 0;
    }
    return 1;
}

int bacaAngkaValid(int *hasil) {
    char buffer[64];
    char *endptr;
    long val;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    hapusNewline(buffer);
    if (isInputKosong(buffer)) return 0;

    val = strtol(buffer, &endptr, 10);
    while (*endptr != '\0') {
        if (!isspace((unsigned char)*endptr)) return 0;
        endptr++;
    }
    *hasil = (int)val;
    return 1;
}

int bandingkanStringIgnoreCase(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return (tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
        }
        s1++;
        s2++;
    }
    return (tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
}

int ekstrakSemuaMenuArray(MenuNode *daftar[], int max_size) {
    int i, n = 0;
    MenuNode *temp;
    for (i = 0; i < TABLE_SIZE; i++) {
        temp = hashTable[i];
        while (temp != NULL && n < max_size) {
            daftar[n++] = temp;
            temp = temp->next;
        }
    }
    return n;
}

void buatFileMenuDefault(void) {
    FILE *fp = fopen(FILE_DATABASE_MENU, "w");
    if (fp == NULL) {
        printf("[!] Gagal membuat file %s.\n", FILE_DATABASE_MENU);
        return;
    }

    fprintf(fp, "# Daftar Menu Restoran: KODE|NAMA_MENU|KATEGORI|HARGA\n");
    fprintf(fp, "# Makanan: E1, E2, ... | Minuman: M1, M2, ...\n");
    fprintf(fp, "E4|Sate Ayam Madura|Makanan|28000\n");
    fprintf(fp, "E2|Ayam Bakar Madu|Makanan|30000\n");
    fprintf(fp, "E1|Nasi Goreng Spesial|Makanan|25000\n");
    fprintf(fp, "E3|Mie Goreng Jawa|Makanan|22000\n");
    fprintf(fp, "E6|Bakso Beranak|Makanan|20000\n");
    fprintf(fp, "E5|Soto Ayam Lamongan|Makanan|23000\n");
    fprintf(fp, "E7|Mie Ayam Pangsit|Makanan|18000\n");
    fprintf(fp, "E8|Bebek Goreng Sambal Ijo|Makanan|35000\n");
    fprintf(fp, "M2|Es Jeruk Peras|Minuman|8000\n");
    fprintf(fp, "M1|Es Teh Manis|Minuman|5000\n");
    fprintf(fp, "M3|Kopi Susu Aren|Minuman|18000\n");
    fprintf(fp, "M4|Jus Alpukat|Minuman|15000\n");

    fclose(fp);
}

void simpanNodeBSTKeFile(FILE *fp, MenuBSTNode *root) {
    if (root == NULL) return;
    simpanNodeBSTKeFile(fp, root->left);
    fprintf(fp, "%s|%s|%s|%d\n", root->kode, root->nama, root->kategori, root->harga);
    simpanNodeBSTKeFile(fp, root->right);
}

void simpanSemuaMenuKeFile(void) {
    FILE *fp = fopen(FILE_DATABASE_MENU, "w");
    if (fp == NULL) {
        printf("[!] Gagal menyimpan menu ke %s.\n", FILE_DATABASE_MENU);
        return;
    }
    fprintf(fp, "# Daftar Menu Restoran: KODE|NAMA_MENU|KATEGORI|HARGA\n");
    simpanNodeBSTKeFile(fp, rootMenuBST);
    fclose(fp);
}

void loadDataMenu(void) {
    FILE *fp = fopen(FILE_DATABASE_MENU, "r");
    char line[256];
    char kode[MAX_KODE], nama[MAX_STR], kategori[20];
    int harga;

    if (fp == NULL) {
        buatFileMenuDefault();
        fp = fopen(FILE_DATABASE_MENU, "r");
        if (fp == NULL) return;
    }

    clearHashTable();
    freeMenuBST(rootMenuBST);
    rootMenuBST = NULL;

    while (fgets(line, sizeof(line), fp) != NULL) {
        hapusNewline(line);

        if (isInputKosong(line) || line[0] == '#') {
            continue;
        }

        if (sscanf(line, "%11[^|]|%63[^|]|%19[^|]|%d", kode, nama, kategori, &harga) == 4) {
            if (!isInputKosong(kode) && !isInputKosong(nama) && !isInputKosong(kategori) && harga > 0) {
                if (insertHashTable(kode, nama, kategori, harga) != NULL) {
                    rootMenuBST = insertMenuBST(rootMenuBST, kode, nama, kategori, harga);
                }
            }
        }
    }

    fclose(fp);
}

void simpanDataAntrean(void) {
    FILE *fp = fopen(FILE_DATABASE_ANTREAN, "w");
    QueueNode *curr = frontOrderQueue;
    if (fp == NULL) return;

    while (curr != NULL) {
        fprintf(fp, "%d|%s|%s|%s|%d|%s|%s|%d\n",
                curr->nomor_order,
                curr->jenis_order,
                curr->nama_pemesan,
                curr->lokasi_tujuan,
                curr->id_cabang,
                curr->cabang_pemroses,
                curr->rincian_menu,
                curr->total_bayar);
        curr = curr->next;
    }
    fclose(fp);
}

void loadDataAntrean(void) {
    FILE *fp = fopen(FILE_DATABASE_ANTREAN, "r");
    char line[512];
    if (fp == NULL) return;

    while (fgets(line, sizeof(line), fp) != NULL) {
        hapusNewline(line);
        if (isInputKosong(line)) continue;

        QueueNode *baru = (QueueNode *)malloc(sizeof(QueueNode));
        if (baru == NULL) break;

        if (sscanf(line, "%d|%24[^|]|%63[^|]|%63[^|]|%d|%63[^|]|%255[^|]|%d",
                   &baru->nomor_order,
                   baru->jenis_order,
                   baru->nama_pemesan,
                   baru->lokasi_tujuan,
                   &baru->id_cabang,
                   baru->cabang_pemroses,
                   baru->rincian_menu,
                   &baru->total_bayar) == 8) {
            baru->next = NULL;

            if (rearOrderQueue == NULL) {
                frontOrderQueue = rearOrderQueue = baru;
            } else {
                rearOrderQueue->next = baru;
                rearOrderQueue = baru;
            }

            ZonaNode *cNode = cariNodeCabangByGraphID(rootWilayahTree, baru->id_cabang);
            if (cNode != NULL) {
                cNode->antrean_aktif++;
            }

            if (baru->nomor_order >= counterNomorOrder) {
                counterNomorOrder = baru->nomor_order + 1;
            }
        } else {
            free(baru);
        }
    }
    fclose(fp);
}

const char *getNamaFileAkun(const char *role) {
    if (strcmp(role, "STAF") == 0) return FILE_AKUN_STAF;
    return FILE_AKUN_PELANGGAN;
}

int cekUsernameAdaDiFile(const char *namaFile, const char *target) {
    FILE *fp = fopen(namaFile, "r");
    char line[200], fUser[MAX_STR];
    if (fp == NULL) return 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        hapusNewline(line);
        if (sscanf(line, "%63[^|]", fUser) == 1) {
            if (strcmp(fUser, target) == 0) {
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

void registerAkunRole(const char *role) {
    char user[MAX_STR], pass[MAX_STR];
    const char *namaFile = getNamaFileAkun(role);
    FILE *fp;

    clearScreen();
    printf("============================================\n");
    printf("       DAFTAR AKUN %-10s BARU          \n", role);
    printf("============================================\n");
    printf("Username Baru : ");
    if (fgets(user, sizeof(user), stdin) == NULL) return;
    hapusNewline(user);
    if (isInputKosong(user) || strchr(user, '|') != NULL) {
        printf("[!] Username nggak boleh kosong atau pakai tanda '|'.\n");
        pauseScreen();
        return;
    }
    if (cekUsernameAdaDiFile(namaFile, user)) {
        printf("[!] Username '%s' sudah dipakai! Coba nama lain.\n", user);
        pauseScreen();
        return;
    }
    printf("Password Baru : ");
    if (fgets(pass, sizeof(pass), stdin) == NULL) return;
    hapusNewline(pass);
    if (isInputKosong(pass) || strchr(pass, '|') != NULL) {
        printf("[!] Password nggak valid!\n");
        pauseScreen();
        return;
    }

    fp = fopen(namaFile, "a");
    if (fp == NULL) {
        printf("[!] Gagal menyimpan akun ke file.\n");
        pauseScreen();
        return;
    }
    fprintf(fp, "%s|%s\n", user, pass);
    fclose(fp);
    printf("\n[+] Akun %s berhasil dibuat! Silakan masuk/login.\n", role);
    pauseScreen();
}

int loginAkunRole(const char *role) {
    char input_user[MAX_STR], input_pass[MAX_STR];
    char line[200], fUser[MAX_STR], fPass[MAX_STR];
    const char *namaFile = getNamaFileAkun(role);
    int cocok = 0;
    FILE *fp;

    clearScreen();
    printf("============================================\n");
    printf("          MASUK SEBAGAI %-10s          \n", role);
    printf("============================================\n");
    printf("Username : ");
    if (fgets(input_user, sizeof(input_user), stdin) == NULL) return 0;
    hapusNewline(input_user);
    printf("Password : ");
    if (fgets(input_pass, sizeof(input_pass), stdin) == NULL) return 0;
    hapusNewline(input_pass);

    fp = fopen(namaFile, "r");
    if (fp == NULL) {
        printf("\n[!] Belum ada akun %s yang terdaftar. Daftar dulu ya!\n", role);
        pauseScreen();
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        hapusNewline(line);
        if (sscanf(line, "%63[^|]|%63[^\n]", fUser, fPass) == 2) {
            if (strcmp(input_user, fUser) == 0 && strcmp(input_pass, fPass) == 0) {
                cocok = 1;
                strcpy(current_user, fUser);
                strcpy(current_role, role);
                break;
            }
        }
    }
    fclose(fp);

    if (cocok) {
        printf("\n[+] Berhasil masuk! Halo, %s.\n", current_user);
        pauseScreen();
        return 1;
    } else {
        printf("\n[!] Username atau Password salah!\n");
        pauseScreen();
        return 0;
    }
}

int verifikasiCabangStaf(void) {
    int idCabang, i;
    char inputPassCabang[MAX_STR];

    while (1) {
        clearScreen();
        printf("============================================================\n");
        printf("            PILIH CABANG TEMPAT JAGA (%s)                  \n", current_user);
        printf("============================================================\n");
        printf("Hari ini kamu jaga di cabang mana?\n");
        for (i = 0; i < JUMLAH_CABANG; i++) {
            printf("  [%d] %s\n", i, namaLokasi[i]);
        }
        printf("  [9] Batal / Keluar\n");
        printf("------------------------------------------------------------\n");
        printf("Pilih Nomor Cabang (0-%d) atau 9: ", JUMLAH_CABANG - 1);

        if (!bacaAngkaValid(&idCabang)) {
            printf("[!] Masukkan angka saja ya.\n");
            pauseScreen();
            continue;
        }

        if (idCabang == 9) {
            return 0;
        }

        if (idCabang < 0 || idCabang >= JUMLAH_CABANG) {
            printf("[!] Nomor cabang salah! Pilih antara 0 sampai %d.\n", JUMLAH_CABANG - 1);
            pauseScreen();
            continue;
        }

        printf("Masukkan PIN/Password Cabang %s: ", namaLokasi[idCabang]);
        if (fgets(inputPassCabang, sizeof(inputPassCabang), stdin) == NULL) return 0;
        hapusNewline(inputPassCabang);

        if (strcmp(inputPassCabang, passwordCabang[idCabang]) == 0) {
            current_cabang_staf = idCabang;
            printf("\n[+] Berhasil masuk ke sistem %s.\n", namaLokasi[idCabang]);
            pauseScreen();
            return 1;
        } else {
            printf("\n[!] Password Cabang salah! Akses ditolak.\n");
            pauseScreen();
        }
    }
}

void portalAuthRole(const char *role) {
    int pilih;
    while (1) {
        clearScreen();
        printf("============================================\n");
        printf("         HALAMAN %-10s                 \n", role);
        printf("============================================\n");
        printf("1. Masuk (Login)\n");
        printf("2. Daftar Akun Baru (Register)\n");
        printf("0. Kembali\n");
        printf("Pilih: ");

        if (!bacaAngkaValid(&pilih)) continue;
        if (pilih == 1) {
            if (loginAkunRole(role)) {
                if (strcmp(role, "PELANGGAN") == 0) {
                    menuDashboardPelanggan();
                } else {
                    if (verifikasiCabangStaf()) {
                        menuDashboardStaf();
                    } else {
                        strcpy(current_user, "");
                        strcpy(current_role, "");
                        current_cabang_staf = -1;
                    }
                }
                break;
            }
        } else if (pilih == 2) {
            registerAkunRole(role);
        } else if (pilih == 0) {
            break;
        }
    }
}