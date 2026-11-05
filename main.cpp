#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <windows.h> 

using namespace std;

// Warna teks terminal (ANSI)
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string RED     = "\033[1;31m";
const string GREEN   = "\033[1;32m";
const string YELLOW  = "\033[1;33m";
const string CYAN    = "\033[1;36m";
const string WHITE   = "\033[1;37m";

// Struktur data satu transaksi
struct Transaksi {
    string tanggal;
    string jenis;
    string kategori;
    long long nominal; 
    string keterangan;
};

// Data utama
long long saldo = 500000;
long long targetBulanan = 1500000;

// Array transaksi (maksimal 1000 data)
const int MAX_DATA = 1000;
Transaksi daftarTransaksi[MAX_DATA];
int jumlahTransaksi = 0; // Penanda berapa data yang sudah terisi di array

const string KATEGORI[] = {
    "Makanan", "Transportasi", "Pendidikan", "Tagihan",
    "Hutang", "Liburan", "Kesehatan", "Pakaian", "Lain Lain"
};

// Hitung spasi kiri biar tampilan otomatis pas di tengah CMD
int hitungMarginTengah(int lebarKonten = 60) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int lebarLayar = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int margin = (lebarLayar - lebarKonten) / 2;
    return (margin > 0) ? margin : 2;
}

string marginSpasi() {
    return string(hitungMarginTengah(), ' ');
}

// Pasang titik tiap 3 angka dari belakang (contoh: 500000 -> Rp 500.000)
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

// Ambil angka 0-9 saja, buang titik/koma/spasi dari ketikan user
long long bersihkanNominal(string teks) {
    string angkaBersih = "";
    for (int i = 0; i < teks.length(); i++) {
        if (teks[i] >= '0' && teks[i] <= '9') angkaBersih += teks[i];
    }
    if (angkaBersih.empty()) return 0;
    return stoll(angkaBersih); // Ubah string angka ke long long
}

// Ambil tanggal hari ini dari sistem (format YYYY-MM-DD)
string ambilTanggalSekarang() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", ltm);
    return string(buf);
}

// Hitung total khusus yang jenisnya pengeluaran
long long hitungTotalPengeluaran() {
    long long total = 0;
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (daftarTransaksi[i].jenis == "Pengeluaran") total += daftarTransaksi[i].nominal;
    }
    return total;
}

// Baca file txt pas program pertama kali dibuka
void muatData() {
    ifstream file("data_keuangan.txt");
    if (!file.is_open()) return;

    // Baris 1 isinya saldo dan target budget
    file >> saldo >> targetBulanan;
    file.ignore(); // Buang sisa enter (\n)

    Transaksi t;
    string nominalStr;
    jumlahTransaksi = 0;

    // Baca per kolom pakai pemisah tanda pipa (|)
    while (getline(file, t.tanggal, '|')) {
        getline(file, t.jenis, '|');
        getline(file, t.kategori, '|');
        getline(file, nominalStr, '|');
        getline(file, t.keterangan); // Kolom terakhir sampai ujung baris

        if (!nominalStr.empty() && jumlahTransaksi < MAX_DATA) {
            t.nominal = stoll(nominalStr);
            daftarTransaksi[jumlahTransaksi] = t;
            jumlahTransaksi++;
        }
    }
    file.close();
}

// Simpan ulang semua data ke file txt (format: tgl|jenis|kategori|nominal|ket)
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

// Cetak tabel transaksi
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

// Menu navigasi panah keyboard
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

        // Di Windows, tombol panah ngirim 2 kode (awalan 224/0 lalu kode arahnya)
        int tombol = _getch();
        if (tombol == 224 || tombol == 0) {
            tombol = _getch();
            // 72 = panah atas, 80 = panah bawah
            // Pakai modulo (%) biar kursor muter (dari paling atas bisa lompat ke paling bawah)
            if (tombol == 72) kursor = (kursor - 1 + jumlahOpsi) % jumlahOpsi;
            else if (tombol == 80) kursor = (kursor + 1) % jumlahOpsi;
        } else if (tombol == 13) { // 13 = tombol Enter
            return kursor;
        }
    }
}

// Menu 1: Input transaksi baru
void catatTransaksi() {
    if (jumlahTransaksi >= MAX_DATA) {
        cout << RED << "\n" << marginSpasi() << "[Error] Kapasitas memori penyimpanan data sudah penuh.\n" << RESET;
        _getch(); // Nahan layar biar error kebaca
        return;
    }

    // Pilih jenis transaksi
    string opsiJenis[] = {"Pengeluaran", "Pemasukan", "Batal"};
    int pilJenis = pilihMenu(marginSpasi() + BOLD + "PILIH JENIS TRANSAKSI" + RESET, opsiJenis, 3);
    if (pilJenis == 2) return;
    string jenis = opsiJenis[pilJenis];

    // Pilih kategori
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

    // Cek saldo cukup atau tidak kalau pengeluaran
    if (jenis == "Pengeluaran" && nominal > saldo) {
        cout << RED << "\n" << spasi << "[Gagal] Saldo tidak mencukupi.\n" << RESET;
        cout << spasi << "Saldo saat ini   : " << CYAN << formatRupiah(saldo) << RESET << "\n";
        cout << spasi << "Tekan sembarang tombol untuk kembali...";
        _getch();
        return;
    }

    // Buang sisa enter (\n) di buffer keyboard biar getline keterangan ga ke-skip
    cin.ignore(1000, '\n');
    string keterangan;
    cout << spasi << "Keterangan       : ";
    getline(cin, keterangan);

    // Simpan ke array
    Transaksi baru = {ambilTanggalSekarang(), jenis, kategori, nominal, keterangan};
    daftarTransaksi[jumlahTransaksi] = baru;
    jumlahTransaksi++;

    // Update nilai saldo
    if (jenis == "Pemasukan") saldo += nominal;
    else saldo -= nominal;

    simpanData(); // Auto-save ke txt
    cout << GREEN << BOLD << "\n" << spasi << "[OK] Transaksi berhasil disimpan.\n" << RESET;

    // Cek apakah sudah tembus batas budget
    if (jenis == "Pengeluaran" && hitungTotalPengeluaran() >= targetBulanan) {
        cout << RED << BOLD << spasi << "[Peringatan] Total pengeluaran telah melampaui target anggaran bulanan.\n" << RESET;
    }

    cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali ke menu...";
    _getch(); // Tahan layar sampai user pencet tombol
}

// Menu 2: Tampilkan semua data di array
void lihatRiwayatTransaksi() {
    system("cls");
    cout << "\n" << marginSpasi() << CYAN << BOLD << "SEMUA RIWAYAT TRANSAKSI" << RESET;
    tampilkanTabel(daftarTransaksi, jumlahTransaksi);
    cout << "\n" << marginSpasi() << "Tekan sembarang tombol untuk kembali...";
    _getch();
}

// Menu 3: Cari data (Sequential Search)
void cariTransaksi() {
    system("cls");
    string spasi = marginSpasi();
    if (jumlahTransaksi == 0) {
        cout << YELLOW << "\n" << spasi << "[Info] Belum ada transaksi untuk dicari.\n" << RESET;
        cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali...";
        _getch();
        return;
    }

    cout << "\n" << spasi << CYAN << BOLD << "PENCARIAN TRANSAKSI" << RESET << "\n";
    cout << spasi << "Kata kunci (keterangan/kategori): ";
    string target;
    cin >> target;

    Transaksi hasil[MAX_DATA];
    int jumlahHasil = 0;

    // Sequential search: cek satu per satu dari indeks 0 sampai akhir
    for (int i = 0; i < jumlahTransaksi; i++) {
        // .find() mengecek apakah kata kunci ada di dalam keterangan atau kategori
        if (daftarTransaksi[i].keterangan.find(target) != string::npos ||
            daftarTransaksi[i].kategori.find(target) != string::npos) {
            hasil[jumlahHasil] = daftarTransaksi[i];
            jumlahHasil++;
        }
    }

    cout << "\n" << spasi << "Hasil pencarian \"" << YELLOW << target << RESET << "\":";
    tampilkanTabel(hasil, jumlahHasil);
    cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali...";
    _getch();
}

// Menu 4: Urutkan data nominal terbesar ke terkecil (Bubble Sort)
void urutkanNominal() {
    system("cls");
    string spasi = marginSpasi();
    if (jumlahTransaksi <= 1) {
        cout << YELLOW << "\n" << spasi << "[Info] Jumlah data belum mencukupi untuk diurutkan.\n" << RESET;
        cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali...";
        _getch();
        return;
    }

    // Salin data ke array sementara biar urutan tanggal asli ga berantakan
    Transaksi salinan[MAX_DATA];
    for (int i = 0; i < jumlahTransaksi; i++) {
        salinan[i] = daftarTransaksi[i];
    }

    // Bubble Sort: bandingkan 2 data bersebelahan, tukar pakai temp jika kiri < kanan
    for (int i = 0; i < jumlahTransaksi - 1; i++) {
        for (int j = 0; j < jumlahTransaksi - i - 1; j++) {
            if (salinan[j].nominal < salinan[j + 1].nominal) {
                Transaksi temp = salinan[j];
                salinan[j] = salinan[j + 1];
                salinan[j + 1] = temp;
            }
        }
    }

    cout << "\n" << spasi << CYAN << BOLD << "DAFTAR TRANSAKSI (URUT DARI NOMINAL TERBESAR)" << RESET;
    tampilkanTabel(salinan, jumlahTransaksi);
    cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali...";
    _getch();
}

// Menu 5: Hitung ringkasan statistik
void laporanStatistik() {
    system("cls");
    string spasi = marginSpasi();
    long long totalKeluar = hitungTotalPengeluaran();
    long long keluarTerbesar = 0;
    int jmlKeluar = 0;

    // Cari frekuensi dan pengeluaran terbesar
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (daftarTransaksi[i].jenis == "Pengeluaran") {
            jmlKeluar++;
            if (daftarTransaksi[i].nominal > keluarTerbesar) keluarTerbesar = daftarTransaksi[i].nominal;
        }
    }

    // Cegah crash pembagian dengan 0 jika belum ada pengeluaran
    long long rataRata = (jmlKeluar > 0) ? (totalKeluar / jmlKeluar) : 0;

    cout << "\n" << spasi << CYAN << string(58, '=') << "\n";
    cout << spasi << BOLD << "               RINGKASAN STATISTIK KEUANGAN               \n" << RESET;
    cout << spasi << CYAN << string(58, '=') << RESET << "\n";
    cout << spasi << " Saldo Saat Ini        : " << CYAN << formatRupiah(saldo) << RESET << "\n";
    cout << spasi << " Total Pengeluaran     : " << RED << formatRupiah(totalKeluar) << RESET << "\n";
    cout << spasi << " Target Anggaran       : " << YELLOW << formatRupiah(targetBulanan) << RESET << "\n";
    cout << spasi << " Frekuensi Pengeluaran : " << jmlKeluar << " kali\n";
    cout << spasi << " Rata-rata Pengeluaran : " << formatRupiah(rataRata) << "\n";
    cout << spasi << " Pengeluaran Terbesar  : " << formatRupiah(keluarTerbesar) << "\n";
    cout << spasi << CYAN << string(58, '=') << RESET << "\n";

    cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali...";
    _getch();
}

// Menu 6: Ganti batas budget
void aturTargetBudget() {
    system("cls");
    string spasi = marginSpasi();
    cout << "\n" << spasi << CYAN << BOLD << "PENGATURAN TARGET ANGGARAN BULANAN" << RESET << "\n\n";
    cout << spasi << "Target saat ini  : " << YELLOW << formatRupiah(targetBulanan) << RESET << "\n";
    cout << spasi << string(45, '-') << "\n";

    string input;
    cout << spasi << "Target baru (Rp) : ";
    cin >> input;

    long long targetBaru = bersihkanNominal(input);
    if (targetBaru <= 0) {
        cout << RED << "\n" << spasi << "[Error] Target anggaran harus lebih dari 0.\n" << RESET;
    } else {
        targetBulanan = targetBaru;
        simpanData(); // Tulis target baru ke txt
        cout << GREEN << BOLD << "\n" << spasi << "[OK] Target anggaran berhasil diperbarui ke: " 
             << formatRupiah(targetBulanan) << "\n" << RESET;
    }

    cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali...";
    _getch();
}

// Menu 7: Tulis data ke txt laporan lalu otomatis buka Notepad
void eksporLaporanTeks() {
    system("cls");
    string spasi = marginSpasi();
    ofstream file("laporan_keuangan.txt");

    if (!file.is_open()) {
        cout << RED << "\n" << spasi << "[Error] Tidak dapat membuat file laporan_keuangan.txt.\n" << RESET;
        _getch();
        return;
    }

    // Tulis header dan tabel ke dalam file
    file << "===========================================================================\n";
    file << "                     LAPORAN KEUANGAN PRIBADI (FINTRACK)                   \n";
    file << "===========================================================================\n";
    file << " Saldo Terkini  : " << formatRupiah(saldo) << "\n";
    file << " Total Keluar   : " << formatRupiah(hitungTotalPengeluaran()) << "\n";
    file << " Target Budget  : " << formatRupiah(targetBulanan) << "\n";
    file << "---------------------------------------------------------------------------\n";
    file << left << setw(4) << "No" << setw(13) << "Tanggal" << setw(14) << "Jenis" 
         << setw(15) << "Kategori" << setw(16) << "Nominal" << "Keterangan\n";
    file << "---------------------------------------------------------------------------\n";

    for (int i = 0; i < jumlahTransaksi; i++) {
        file << left << setw(4) << (i + 1)
             << setw(13) << daftarTransaksi[i].tanggal
             << setw(14) << daftarTransaksi[i].jenis
             << setw(15) << daftarTransaksi[i].kategori
             << setw(16) << formatRupiah(daftarTransaksi[i].nominal)
             << daftarTransaksi[i].keterangan << "\n";
    }
    file.close();

    cout << "\n" << spasi << GREEN << BOLD << "[OK] Data berhasil diekspor ke laporan_keuangan.txt.\n" << RESET;
    cout << spasi << "Membuka berkas laporan...\n";
    
    // Perintah Windows untuk langsung buka file di Notepad
    system("notepad laporan_keuangan.txt");

    cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali...";
    _getch();
}

// Menu 8: Reset seluruh data
void resetData() {
    system("cls");
    string spasi = marginSpasi();
    cout << "\n" << spasi << RED << BOLD << "KONFIRMASI RESET DATA" << RESET << "\n\n";
    cout << spasi << "Peringatan:\n";
    cout << spasi << "1. Seluruh rekaman transaksi akan dihapus permanen.\n";
    cout << spasi << "2. Saldo sistem akan dikembalikan ke Rp 0.\n";
    cout << spasi << "3. Target anggaran bulanan diatur ulang ke Rp 1.500.000.\n\n";
    cout << spasi << YELLOW << "Ketik 'RESET' untuk melanjutkan: " << RESET;
    
    string konfirmasi;
    cin >> konfirmasi;

    // Reset array cukup kembalikan jumlahTransaksi ke 0
    if (konfirmasi == "RESET") {
        jumlahTransaksi = 0;
        saldo = 0;
        targetBulanan = 1500000;
        simpanData(); // Timpa file txt dengan kondisi kosong
        cout << GREEN << BOLD << "\n" << spasi << "[OK] Seluruh data telah diatur ulang ke kondisi awal.\n" << RESET;
    } else {
        cout << "\n" << spasi << "[Info] Tindakan dibatalkan. Data tidak diubah.\n";
    }

    cout << "\n" << spasi << "Tekan sembarang tombol untuk kembali ke menu...";
    _getch();
}

int main() {
    // Aktifkan UTF-8 dan kode warna ANSI di terminal Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // Ambil data lama dari file
    muatData();

    const string MENU_UTAMA[] = {
        "Catat Transaksi Baru",              // 0
        "Lihat Semua Riwayat Transaksi",     // 1
        "Cari Transaksi (Sequential Search)",// 2
        "Urutkan Nominal (Bubble Sort)",     // 3
        "Laporan Statistik",                 // 4
        "Atur Target Budget Bulanan",        // 5
        "Ekspor Laporan ke File Teks",       // 6
        "Reset Semua Data Keuangan",         // 7
        "Keluar dari Program"                // 8
    };

    // Loop utama program
    while (true) {
        long long totalKeluar = hitungTotalPengeluaran();

        string spasi = marginSpasi();
        string garisTebal = spasi + CYAN + string(60, '=') + RESET + "\n";
        string garisTipis = spasi + CYAN + string(60, '-') + RESET + "\n";

        // Teks kartu ringkasan dashboard
        string header = 
            garisTebal +
            spasi + BOLD + WHITE + "                 FINTRACK - SISTEM KEUANGAN                 \n" + RESET +
            garisTebal +
            spasi + " Saldo Terkini : " + CYAN + BOLD + formatRupiah(saldo) + RESET + "\n" +
            spasi + " Pengeluaran   : " + RED + formatRupiah(totalKeluar) + RESET + 
            " / " + YELLOW + formatRupiah(targetBulanan) + RESET + "\n" +
            spasi + " File Data     : " + GREEN + "data_keuangan.txt (Tersimpan)" + RESET + "\n" +
            garisTipis;

        int pilihan = pilihMenu(header, MENU_UTAMA, 9);

        // Eksekusi fungsi sesuai indeks pilihan menu
        if (pilihan == 0) catatTransaksi();
        else if (pilihan == 1) lihatRiwayatTransaksi();
        else if (pilihan == 2) cariTransaksi();
        else if (pilihan == 3) urutkanNominal();
        else if (pilihan == 4) laporanStatistik();
        else if (pilihan == 5) aturTargetBudget();
        else if (pilihan == 6) eksporLaporanTeks();
        else if (pilihan == 7) resetData();
        else if (pilihan == 8) {
            system("cls");
            simpanData(); // Pastikan data tersimpan sebelum program keluar
            cout << "\n" << marginSpasi() << GREEN << BOLD << "[Info] Data tersimpan. Sesi selesai.\n" << RESET;
            break;
        }
    }

    return 0;
}