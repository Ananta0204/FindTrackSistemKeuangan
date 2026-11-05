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
// Menghitung indentasi kiri agar tata letak konsol berada di posisi tengah
int hitungMarginTengah(int lebarKonten = 60) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int lebarLayar = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int margin = (lebarLayar - lebarKonten) / 2;
    return (margin > 0) ? margin : 2;
}

// Menghasilkan string spasi pembentuk margin tengah
string marginSpasi() {
    return string(hitungMarginTengah(), ' ');
}

// Mengonversi bilangan bulat ke format mata uang Rupiah
string formatRupiah(long long nominal) {
    string str = to_string(nominal);
    string hasil = "";
    int hitung = 0;
    for (int i = str.length() - 1; i >= 0; i--) {
        hasil = str[i] + hasil;
        hitung++;
        if (hitung % 3 == 0 && i != 0) hasil = "." + hasil;
    }
    return "Rp " + hasil;
}

// Menghapus karakter non-angka dari input teks dan mengubahnya ke tipe long long
long long bersihkanNominal(string teks) {
    string angkaBersih = "";
    for (int i = 0; i < teks.length(); i++) {
        if (teks[i] >= '0' && teks[i] <= '9') angkaBersih += teks[i];
    }
    if (angkaBersih.empty()) return 0;
    return stoll(angkaBersih);
}

// Mengambil tanggal sistem saat ini dengan format YYYY-MM-DD
string ambilTanggalSekarang() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", ltm);
    return string(buf);
}

// Menghitung akumulasi nominal pada transaksi bertipe Pengeluaran
long long hitungTotalPengeluaran() {
    long long total = 0;
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (daftarTransaksi[i].jenis == "Pengeluaran") total += daftarTransaksi[i].nominal;
    }
    return total;
}

// Menampilkan menu interaktif berbasis tombol panah (Atas/Bawah) dan Enter
int pilihMenu(string header, const string opsi[], int jumlahOpsi) {
    int kursor = 0;

    while (true) {
        system("cls");
        cout << "\n" << header << "\n\n";
        cout << marginSpasi() << YELLOW << "Gunakan [Panah Atas / Bawah] lalu tekan [ENTER]:\n\n" << RESET;

        for (int i = 0; i < jumlahOpsi; i++) {
            if (i == kursor) {
                cout << marginSpasi() << CYAN << BOLD << "  ➤  [ " << opsi[i] << " ]\n" << RESET;
            } else {
                cout << marginSpasi() << "     [ " << opsi[i] << " ]\n";
            }
        }
        cout << "\n" << marginSpasi() << CYAN << string(60, '-') << RESET << "\n";

        int tombol = _getch();
        if (tombol == 224 || tombol == 0) {
            tombol = _getch();
            if (tombol == 72) kursor = (kursor - 1 + jumlahOpsi) % jumlahOpsi;
            else if (tombol == 80) kursor = (kursor + 1) % jumlahOpsi;
        } else if (tombol == 13) {
            return kursor;
        }
    }
}

// Membaca data tersimpan dari file teks ke dalam array
void muatData() {
    ifstream file("data_keuangan.txt");
    if (!file.is_open()) return;

    file >> saldo >> targetBulanan;
    file.ignore();

    Transaksi t;
    string nominalStr;
    jumlahTransaksi = 0;

    while (getline(file, t.tanggal, '|')) {
        getline(file, t.jenis, '|');
        getline(file, t.kategori, '|');
        getline(file, nominalStr, '|');
        getline(file, t.keterangan);

        if (!nominalStr.empty() && jumlahTransaksi < MAX_DATA) {
            t.nominal = stoll(nominalStr);
            daftarTransaksi[jumlahTransaksi] = t;
            jumlahTransaksi++;
        }
    }
    file.close();
}

// Menyimpan saldo, target, dan seluruh elemen array ke file teks
void simpanData() {
    ofstream file("data_keuangan.txt");
    if (!file.is_open()) return;

    file << saldo << " " << targetBulanan << "\n";
    for (int i = 0; i < jumlahTransaksi; i++) {
        file << daftarTransaksi[i].tanggal << "|"
             << daftarTransaksi[i].jenis << "|"
             << daftarTransaksi[i].kategori << "|"
             << daftarTransaksi[i].nominal << "|"
             << daftarTransaksi[i].keterangan << "\n";
    }
    file.close();
}
// Menampilkan data transaksi dalam bentuk tabel
void tampilkanTabel(const Transaksi data[], int totalData) {
    if (totalData == 0) {
        cout << YELLOW << "\n" << marginSpasi() << "[Info] Belum ada transaksi yang tercatat.\n" << RESET;
        return;
    }

    string spasi = marginSpasi();
    cout << "\n" << spasi << CYAN << string(75, '=') << RESET << "\n";
    cout << spasi << BOLD << left << setw(4)  << "No"
         << setw(13) << "Tanggal"
         << setw(14) << "Jenis"
         << setw(15) << "Kategori"
         << setw(16) << "Nominal"
         << "Keterangan" << RESET << "\n";
    cout << spasi << string(75, '-') << "\n";

    for (int i = 0; i < totalData; i++) {
        cout << spasi << left << setw(4) << (i + 1)
             << setw(13) << data[i].tanggal;

        if (data[i].jenis == "Pemasukan") cout << GREEN << setw(14) << data[i].jenis << RESET;
        else cout << RED << setw(14) << data[i].jenis << RESET;

        cout << setw(15) << data[i].kategori
             << setw(16) << formatRupiah(data[i].nominal)
             << data[i].keterangan << "\n";
    }
    cout << spasi << CYAN << string(75, '=') << RESET << "\n";
}

// Menangani input transaksi baru, validasi nominal, dan update saldo
void catatTransaksi() {
    if (jumlahTransaksi >= MAX_DATA) {
        cout << RED << "\n" << marginSpasi() << "[Error] Kapasitas memori penyimpanan data sudah penuh.\n" << RESET;
        _getch();
        return;
    }

    string opsiJenis[] = {"Pengeluaran", "Pemasukan", "Batal"};
    int pilJenis = pilihMenu(marginSpasi() + BOLD + "PILIH JENIS TRANSAKSI" + RESET, opsiJenis, 3);
    if (pilJenis == 2) return;
    string jenis = opsiJenis[pilJenis];

    string opsiKategori[10];
    for (int i = 0; i < 9; i++) opsiKategori[i] = KATEGORI[i];
    opsiKategori[9] = "Batal";

    int pilKat = pilihMenu(marginSpasi() + BOLD + "PILIH KATEGORI TRANSAKSI" + RESET, opsiKategori, 10);
    if (pilKat == 9) return;
    string kategori = opsiKategori[pilKat];

    system("cls");
    string spasi = marginSpasi();
    cout << "\n" << spasi << CYAN << BOLD << "INPUT DETAIL TRANSAKSI" << RESET << "\n\n";
    cout << spasi << "Jenis Transaksi  : " << (jenis == "Pemasukan" ? GREEN : RED) << jenis << RESET << "\n";
    cout << spasi << "Kategori         : " << YELLOW << kategori << RESET << "\n";
    cout << spasi << string(46, '-') << "\n";

    string inputNominal;
    cout << spasi << "Nominal (Rp)     : ";
    cin >> inputNominal;
    long long nominal = bersihkanNominal(inputNominal);

    if (nominal <= 0) {
        cout << RED << "\n" << spasi << "[Error] Nominal harus lebih dari 0.\n" << RESET;
        cout << spasi << "Tekan sembarang tombol untuk kembali...";
        _getch();
        return;
    }

    if (jenis == "Pengeluaran" && nominal > saldo) {
        cout << RED << "\n" << spasi << "[Gagal] Saldo tidak mencukupi.\n" << RESET;
        cout << spasi << "Saldo saat ini   : " << CYAN << formatRupiah(saldo) << RESET << "\n";
        cout << spasi << "Tekan sembarang tombol untuk kembali...";
        _getch();
        return;
    }

    cin.ignore(1000, '\n');
    string keterangan;
    cout << spasi << "Keterangan       : ";
    getline(cin, keterangan);

    // Menyimpan rekaman baru ke array
    Transaksi baru = {ambilTanggalSekarang(), jenis, kategori, nominal, keterangan};
    daftarTransaksi[jumlahTransaksi] = baru;
    jumlahTransaksi++;

    // Penyesuaian saldo sistem
    if (jenis == "Pemasukan") saldo += nominal;
    else saldo -= nominal;

    simpanData();
    cout << GREEN << BOLD << "\n" << spasi << "[OK] Transaksi berhasil disimpan.\n" << RESET;

    if (jenis == "Pengeluaran" && hitungTotalPengeluaran() >= targetBulanan) {
        cout << RED << BOLD << spasi << "[Peringatan] Total pengeluaran telah melampaui target anggaran bulanan.\n" << RESET;
    }

    cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali ke menu...";
    _getch();
}