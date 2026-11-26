# Sistem Pemesanan Restoran Multi-Cabang (Tugas Rancang ASD 2026)

- **Nama** : Annabelle Valencia
- **NIM**  : 622025009

## Deskripsi Aplikasi
Aplikasi konsol berbasis Bahasa C (100% C) untuk sistem pemesanan restoran multi-cabang yang melayani alur **Pelanggan** (pemesanan *Delivery* otomatis dari cabang terdekat yang buka maupun *Take Away*) serta alur **Staf Restoran** (kasir *offline*, pengelolaan antrean dapur per cabang, buka/tutup cabang, manajemen katalog menu, dan pemantauan rute pengiriman). Seluruh materi Algoritma dan Struktur Data Topik 1 hingga Topik 8 diterapkan secara terintegrasi di dalam sistem.

## Daftar Fitur & Penerapan Materi (Topik 1–8)
1. **Refreshing C & Validasi Input (Topik 1):** Antarmuka berbasis menu perulangan dan percabangan yang terpecah menjadi fungsi modular, lengkap dengan proteksi input anti-*crash* / *infinite loop*.
2. **Array, Struct, Pointer, & Operasi File `.txt` (Topik 2):** Manajemen keranjang belanja dan database permanen (`data/menu.txt`, `data/antrean.txt`, `data/akun_pelanggan.txt`, `data/akun_staf.txt`) menggunakan alokasi memori dinamis (`malloc`/`free`).
3. **Sorting (Topik 3):** Pengurutan katalog menu secara *Ascending* dan *Descending* menggunakan *Selection Sort* (berdasarkan harga & menu terlaris) dan *Insertion Sort* (berdasarkan nama A–Z / Z–A).
4. **Stack & Queue (Topik 4):** *Stack* (LIFO) untuk fitur *Undo* membatalkan item terakhir di keranjang belanja, serta *Queue* (FIFO) untuk antrean pesanan masuk pada dapur tiap cabang.
5. **Searching (Topik 5):** *Sequential Search* untuk pencarian menu berdasarkan kata kunci nama/kode, dan *Binary Search* pada daftar harga yang telah diurutkan untuk pencarian menu sesuai budget pas.
6. **Hash Table (Topik 6):** Pencarian cepat data menu berdasarkan kode unik (`E1`, `M1`, dst.) menggunakan fungsi hash mandiri dan penanganan kolisi *Separate Chaining*.
7. **Tree (Topik 7):** *Binary Search Tree (BST)* untuk penyimpanan, pencarian, penghapusan, dan *traversal* (*Pre-Order*, *In-Order*, *Post-Order*) katalog menu, serta *N-ary Tree* untuk hierarki zona wilayah cabang.
8. **Graph (Topik 8):** *Weighted Graph* peta jalan kota yang menghubungkan 4 cabang restoran dan 7 kelurahan, dilengkapi **BFS** (cek cabang buka terdekat), **DFS** (verifikasi konektivitas seluruh jalan), dan **Algoritma Dijkstra** (penentuan rute terpendek & ongkos kirim kurir).

## Cara Kompilasi dan Menjalankan Program

Pastikan terminal berada di direktori utama proyek (`622025009_Annabelle_ASD_TR`).

### Menggunakan PowerShell (Windows)
Ketikkan:
gcc (Get-ChildItem src\*.c).FullName -Iinclude -o app
Jika tidak muncul pesan eror, jalankan programnya dengan mengetikkan:
.\app.exe
