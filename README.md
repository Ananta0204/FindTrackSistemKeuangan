# 💰 FinTrack - Sistem Manajemen Keuangan Pribadi

Aplikasi pencatatan dan pengelolaan keuangan berbasis konsol (*Command Line Interface*) yang dibangun menggunakan bahasa pemrograman **C++ murni**. Program ini dirancang untuk memantau arus kas harian, membatasi pengeluaran bulanan melalui sistem target anggaran (*budgeting*), serta menyajikan pencarian dan pengurutan data menggunakan algoritma fundamental.

---

## 📌 Studi Kasus

> **Latar Belakang Masalah:**  
> Mahasiswa rantau atau anak kos sering kali menghadapi tantangan dalam mengelola uang saku bulanan. Pengeluaran mahasiswa sangat beragam, mencakup kebutuhan pokok harian (makanan, transportasi), kebutuhan akademik (pendidikan/buku), hingga tagihan rutin dan pengeluaran darurat. Tanpa adanya sistem pencatatan yang teratur, mahasiswa kerap mengalami kesulitan dalam memantau sisa saldo riil dan tidak menyadari kapan pengeluaran mereka telah melampaui batas wajar (*overspending*), yang berujung pada krisis finansial di akhir bulan.

**Solusi yang Diterapkan:**  
Program **FinTrack** hadir sebagai alat bantu pencatatan keuangan mandiri yang:
1. Mengelompokkan setiap transaksi ke dalam **9 kategori spesifik** (*Makanan, Transportasi, Pendidikan, Tagihan, Hutang, Liburan, Kesehatan, Pakaian, Lain Lain*).
2. Memiliki **sistem pengawasan anggaran (*budget limit*)** yang memberikan peringatan otomatis jika akumulasi pengeluaran bulanan melewati batas yang ditentukan pengguna.
3. Menyimpan seluruh mutasi transaksi secara permanen ke dalam **file teks (*File I/O*)**, sehingga data tidak hilang saat program ditutup.
4. Memudahkan penelusuran riwayat menggunakan algoritma **Sequential Search** dan evaluasi pengeluaran terbesar dengan **Bubble Sort**.

---

## 📊 Diagram Alir (Flowchart)

Berikut adalah diagram alir yang menggambarkan logika kerja program dari proses inisialisasi berkas hingga keluar:

<p align="center">
  <img src="flowchart.png" alt="Diagram Alir FinTrack" width="100%">
</p>

### Penjelasan Alur Flowchart:
1. **Inisialisasi:** Program dimulai (`Mulai`) dengan membaca berkas `data_keuangan.txt` untuk memuat data saldo dan transaksi lama, kemudian menampilkan layar `Dashboard`.
2. **Pilihan Menu:** Pengguna memilih salah satu dari 9 menu yang tersedia:
   - **Catat Transaksi:** Menginput data transaksi. Jika jenisnya *Pemasukan*, saldo bertambah dan disimpan. Jika *Pengeluaran*, sistem memvalidasi `Saldo >= Nominal?`. Bila saldo cukup, saldo dikurangi dan disimpan; bila tidak, muncul pesan saldo tidak cukup.
   - **Lihat Riwayat:** Menampilkan seluruh daftar riwayat transaksi dalam bentuk tabel.
   - **Cari Transaksi:** Menerima input kata kunci, lalu menjalankan algoritma *Sequential Search* untuk menyaring baris data yang cocok.
   - **Urutkan Nominal:** Mengurutkan transaksi dari nominal terbesar ke terkecil menggunakan algoritma *Bubble Sort*.
   - **Laporan Statistik:** Menghitung total pengeluaran, rata-rata, dan pengeluaran terbesar, lalu menampilkan ringkasannya.
   - **Atur Budget:** Memperbarui nilai target anggaran bulanan baru dan menyimpannya ke berkas.
   - **Expor Laporan:** Menulis seluruh data ke berkas `laporan_keuangan.txt` dan membukanya otomatis di Notepad.
   - **Reset Data:** Meminta konfirmasi kata kunci `"RESET"`. Jika cocok, seluruh transaksi dihapus dan saldo kembali ke Rp 0. Jika batal, data tidak diubah.
   - **Keluar:** Menyimpan data akhir ke berkas penyimpanan, lalu program selesai (`Selesai`).
3. **Looping:** Seluruh cabang menu (menu 1–8) setelah selesai dieksekusi akan bermuara kembali ke `Dashboard` bawah dan memutar kembali ke menu utama.

---

## 🛠️ Fitur-Fitur Aplikasi

1. **Navigasi Panah Keyboard (TUI):** Pemilihan menu menggunakan tombol panah (**⬆️ Atas / ⬇️ Bawah**) dan **Enter** tanpa mengetik angka secara manual.
2. **Pencatatan Pemasukan & Pengeluaran:** Mendukung 9 pilihan kategori, format tanggal otomatis dari sistem, dan validasi kecukupan saldo.
3. **Format Angka Rupiah:** Menampilkan nominal dengan pemisah ribuan otomatis (misal `Rp 500.000`) dan menerima input angka fleksibel (bisa diketik `500.000` maupun `500000`).
4. **Pencarian Data (Sequential Search):** Mencari transaksi berdasarkan kata kunci pada keterangan atau kategori secara linier baris demi baris.
5. **Pengurutan Data (Bubble Sort):** Mengurutkan riwayat transaksi dari nominal terbesar ke terkecil tanpa merusak urutan tanggal data asli.
6. **Laporan Statistik:** Menyajikan ringkasan frekuensi pengeluaran, rata-rata pengeluaran, dan nominal transaksi terbesar.
7. **Pengaturan Target Anggaran:** Pengguna dapat mengubah batas maksimal anggaran bulanan kapan saja.
8. **Ekspor & Pembacaan Notepad:** Mencetak laporan terformat ke berkas `laporan_keuangan.txt` dan langsung membukanya secara otomatis di Notepad.
9. **Reset Sistem:** Mengosongkan data riwayat transaksi dan mengembalikan saldo ke Rp 0 dengan proteksi konfirmasi ketik `RESET`.

---

## 📚 Penerapan Konsep Pemrograman

Struktur kode program disusun menggunakan konsep-konsep dasar pemrograman C++:

| Konsep / Modul | Implementasi Teknis dalam Kode |
| :--- | :--- |
| **Tipe Data & Variabel** | Penggunaan `struct Transaksi`, tipe data `long long` untuk nominal besar, dan array string. |
| **Percabangan (`if-else`)** | Validasi saldo cukup/tidak saat belanja, batas anggaran, dan verifikasi konfirmasi reset. |
| **Perulangan (`for` & `while`)** | Perulangan `while(true)` untuk siklus aplikasi, `for` untuk iterasi data array dan parsing file. |
| **Array Statis** | Penggunaan array statis 1 dimensi (`daftarTransaksi[1000]`) murni tanpa bantuan *STL Vector*. |
| **Searching & Sorting** | Algoritma **Sequential Search** pada fitur pencarian dan **Bubble Sort** (dengan variabel `temp`) pada fitur pengurutan. |
| **Fungsi Modular** | Pembagian tugas ke fungsi-fungsi mandiri (`formatRupiah`, `bersihkanNominal`, `simpanData`, dll). |
| **Operasi Berkas (*File I/O*)** | Pustaka `<fstream>` (`ifstream` untuk memuat data dan `ofstream` untuk menyimpan serta mengekspor laporan). |

---

## 🚀 Panduan Menjalankan Program

### Prasyarat
- Kompiler C++ yang mendukung C++11 ke atas (seperti **MinGW / GCC**).
- Sistem Operasi **Windows 10 / 11** (untuk pustaka `<conio.h>` dan escape code warna ANSI).

### Langkah Kompilasi
1. Buka **Command Prompt (CMD)** dan arahkan ke folder tempat file `main.cpp` disimpan.
2. Jalankan perintah kompilasi:
   ```cmd
   g++ main.cpp -o keuangan.exe
   Eksekusi program:
   keuangan.exe
