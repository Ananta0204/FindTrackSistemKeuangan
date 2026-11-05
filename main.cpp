#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <windows.h> 

using namespace std;

// Definisi kode warna ANSI untuk tampilan konsol
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string RED     = "\033[1;31m";
const string GREEN   = "\033[1;32m";
const string YELLOW  = "\033[1;33m";
const string CYAN    = "\033[1;36m";
const string WHITE   = "\033[1;37m";

// Struktur data untuk menampung satu rekaman transaksi
struct Transaksi {
    string tanggal;
    string jenis;
    string kategori;
    long long nominal; 
    string keterangan;
};

// Variabel status global
long long saldo = 500000;
long long targetBulanan = 1500000;

// Penyimpanan data menggunakan array statis
const int MAX_DATA = 1000;
Transaksi daftarTransaksi[MAX_DATA];
int jumlahTransaksi = 0;

// Daftar kategori pengeluaran dan pemasukan
const string KATEGORI[] = {
    "Makanan", "Transportasi", "Pendidikan", "Tagihan",
    "Hutang", "Liburan", "Kesehatan", "Pakaian", "Lain Lain"
};
