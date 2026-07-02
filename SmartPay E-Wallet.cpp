#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Transaksi {
    int id;
    string jenis;
    string merchant;
    double nominal;
};

struct User {
    string id;
    string nama;
    string password;
    double saldo;
    vector<Transaksi> transaksi;
};

void tampilkanUser(User* u) {
    cout << left << setw(10) << u->id
         << setw(20) << u->nama
         << "Rp" << u->saldo << endl;
}

void cariTransaksi(const vector<Transaksi>& arr, int id) {
    for (auto& t : arr) {
        if (t.id == id) {
            cout << "ID: " << t.id
                 << " | Jenis: " << t.jenis
                 << " | Merchant: " << t.merchant
                 << " | Nominal: Rp" << t.nominal << endl;
            return;
        }
    }
    cout << "Transaksi tidak ditemukan.\n";
}

bool compareNominal(Transaksi a, Transaksi b) {
    return a.nominal < b.nominal;
}

int main() {
    cout << "=====================================\n";
    cout << "   SMARTPAY E-WALLET SYSTEM   \n";
    cout << "=====================================\n";

    User user;
    cout << "Masukkan ID User   : "; cin >> user.id;
    cout << "Masukkan Nama User : "; cin.ignore(); getline(cin, user.nama);
    cout << "Masukkan Password  : "; cin >> user.password;
    cout << "Masukkan Saldo Awal: "; cin >> user.saldo;

    // Login check
    string inputPass;
    cout << "Login - Masukkan Password: ";
    cin >> inputPass;
    if (inputPass != user.password) {
        cout << "Password salah! Program berhenti.\n";
        return 0;
    }

    list<Transaksi> riwayat;     
    stack<Transaksi> undoStack;  
    queue<Transaksi> pendingQueue; 

    map<string,string> merchantKategori;
    merchantKategori["Burger"] = "Makanan";
    merchantKategori["Kopi"] = "Minuman";
    merchantKategori["Restoran"] = "Makanan";
    merchantKategori["OjekOnline"] = "Transportasi";
    merchantKategori["Taksi"] = "Transportasi";

    map<string,int> kategoriCounter;
    map<string,int> bankCounter;
    map<pair<string,string>, int> bankKategoriCounter;

    int autoID = 1234;
    int pilihan;
    do {
        cout << "\n=============================\n";
        cout << "        MENU E-WALLET        \n";
        cout << "=============================\n";
        cout << "1. Lihat User\n";
        cout << "2. Tambah Transaksi\n";
        cout << "3. Lihat Transaksi\n";
        cout << "4. Cari Transaksi\n";
        cout << "5. Urutkan Transaksi\n";
        cout << "6. Undo Transaksi\n";
        cout << "7. Lihat Transaksi Pending\n";
        cout << "8. Lihat Tree Kategori\n";
        cout << "9. Lihat Graph Bank-Kategori\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        if (pilihan == 1) {
            cout << "\n=== DATA USER ===\n";
            cout << left << setw(10) << "ID"
                 << setw(20) << "Nama"
                 << setw(15) << "Saldo" << endl;
            cout << "----------------------------------\n";
            tampilkanUser(&user);
        }
        else if (pilihan == 2) {
            Transaksi t;
            t.id = autoID++;
            cout << "ID Transaksi otomatis: " << t.id << endl;
            cout << "Jenis (QRIS/TopUp/Transfer): "; cin >> t.jenis;
            cout << "Nama Merchant  : "; cin >> t.merchant;
            cout << "Nominal        : "; cin >> t.nominal;

            string bankUser;
            cout << "Masukkan Bank User (Mandiri/BCA/BTN): ";
            cin >> bankUser;

            // Validasi password sebelum transaksi
            string inputPassTrans;
            cout << "Masukkan Password untuk konfirmasi transaksi: ";
            cin >> inputPassTrans;
            if (inputPassTrans != user.password) {
                cout << "Password salah! Transaksi ditolak.\n";
                continue;
            }

            string kategoriTransaksi = "Lainnya";
            if (merchantKategori.find(t.merchant) != merchantKategori.end()) {
                kategoriTransaksi = merchantKategori[t.merchant];
            }

            if (t.nominal <= user.saldo || t.jenis == "TopUp") {
                if (t.jenis == "TopUp") {
                    user.saldo += t.nominal;
                } else {
                    user.saldo -= t.nominal;
                }
                user.transaksi.push_back(t);
                riwayat.push_back(t);
                undoStack.push(t);

                kategoriCounter[kategoriTransaksi]++;
                bankCounter[bankUser]++;
                bankKategoriCounter[{bankUser, kategoriTransaksi}]++;

                cout << "Transaksi berhasil! Saldo sekarang: Rp" << user.saldo << endl;
                cout << bankUser << " digunakan di kategori " << kategoriTransaksi << endl;
            } else {
                cout << "Saldo tidak cukup! Transaksi pending.\n";
                pendingQueue.push(t);
            }
        }
        else if (pilihan == 3) {
            cout << "\n=== TRANSAKSI USER " << user.nama << " ===\n";
            for (auto& t : user.transaksi) {
                cout << t.id << " | " << t.jenis << " | " << t.merchant << " - Rp" << t.nominal << endl;
            }
        }
        else if (pilihan == 4) {
            int cariID;
            cout << "Masukkan ID transaksi: ";
            cin >> cariID;
            cariTransaksi(user.transaksi, cariID);
        }
        else if (pilihan == 5) {
            sort(user.transaksi.begin(), user.transaksi.end(), compareNominal);
            cout << "\n=== TRANSAKSI TERURUT ===\n";
            for (auto& t : user.transaksi) {
                cout << t.jenis << " | " << t.merchant << " - Rp" << t.nominal << endl;
            }
        }
        else if (pilihan == 6) {
            if (!undoStack.empty()) {
                Transaksi terakhir = undoStack.top();
                undoStack.pop();
                cout << "Undo transaksi: " << terakhir.jenis << " | " << terakhir.merchant << " - Rp" << terakhir.nominal << endl;
            } else {
                cout << "Tidak ada transaksi untuk di-undo.\n";
            }
        }
        else if (pilihan == 7) {
            if (!pendingQueue.empty()) {
                cout << "\n=== DAFTAR TRANSAKSI PENDING ===\n";
                queue<Transaksi> temp = pendingQueue;
                while (!temp.empty()) {
                    Transaksi p = temp.front();
                    cout << p.id << " | " << p.jenis 
                         << " | " << p.merchant 
                         << " - Rp" << p.nominal << endl;
                    temp.pop();
                }
            } else {
                cout << "Tidak ada transaksi pending.\n";
            }
        }
        else if (pilihan == 8) {
            cout << "\n=== TREE KATEGORI (Ranking berdasarkan penggunaan) ===\n";
            vector<pair<string,int>> ranking(kategoriCounter.begin(), kategoriCounter.end());
            sort(ranking.begin(), ranking.end(), [](auto &a, auto &b){ return a.second > b.second; });
            for (auto& r : ranking) {
                cout << r.first << " digunakan " << r.second << " kali\n";
            }
        }
        else if (pilihan == 9) {
            cout << "\n=== GRAPH BANK → KATEGORI (Penggunaan nyata) ===\n";
            for (auto& bk : bankKategoriCounter) {
                cout << bk.first.first << " digunakan di kategori "
                     << bk.first.second << " sebanyak " << bk.second << " kali\n";
            }
        }
    } while (pilihan != 0);

    cout << "\nTerima kasih telah menggunakan E-Wallet!\n";
    return 0;
}
