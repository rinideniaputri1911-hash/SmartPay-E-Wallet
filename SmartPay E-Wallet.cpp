#include <iostream>
#include <string>
#include <vector>
#include <list>       // Linked List
#include <stack>      // Stack
#include <queue>      // Queue
#include <map>        // Tree
#include <unordered_map> // Graph
#include <algorithm>  // Sorting
#include <iomanip>
using namespace std;

// Struktur Transaksi
struct Transaksi {
    int id;
    string jenis;     // Jenis transaksi: QRIS, Top Up, Transfer
    string merchant;
    double nominal;
};

// Struktur User (hanya satu akun)
struct User {
    string id;
    string nama;
    double saldo;
    vector<Transaksi> transaksi;
};

// Pointer untuk tampilkan user
void tampilkanUser(User* u) {
    cout << left << setw(10) << u->id
         << setw(20) << u->nama
         << "Rp" << u->saldo << endl;
}

// Searching transaksi
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

// Sorting transaksi berdasarkan nominal
bool compareNominal(Transaksi a, Transaksi b) {
    return a.nominal < b.nominal;
}

// ===== MAIN PROGRAM =====
int main() {
    cout << "=====================================\n";
    cout << "   SMARTPAY E-WALLET SYSTEM   \n";
    cout << "=====================================\n";

    // Buat satu user saja
    User user;
    cout << "Masukkan ID User   : "; cin >> user.id;
    cout << "Masukkan Nama User : "; cin.ignore(); getline(cin, user.nama);
    cout << "Masukkan Saldo Awal: "; cin >> user.saldo;

    list<Transaksi> riwayat;     // Linked List
    stack<Transaksi> undoStack;  // Stack
    queue<Transaksi> pendingQueue; // Queue
    map<string, vector<string>> kategori; // Tree
    unordered_map<string, vector<string>> merchantGraph; // Graph

    // Isi Tree kategori
    kategori["Makanan"] = {"Restoran", "Fast Food"};
    kategori["Transportasi"] = {"Ojek Online", "Taksi"};

    // Isi Graph merchant-bank
    merchantGraph["Bank A"] = {"Merchant1", "Merchant2"};
    merchantGraph["Bank B"] = {"Merchant3", "Merchant4"};

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
        cout << "9. Lihat Graph Merchant\n";
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
            cout << "ID Transaksi   : "; cin >> t.id;
            cout << "Jenis (QRIS/TopUp/Transfer): "; cin >> t.jenis;
            cout << "Nama Merchant  : "; cin >> t.merchant;
            cout << "Nominal        : "; cin >> t.nominal;

            if (t.nominal <= user.saldo || t.jenis == "TopUp") {
                if (t.jenis == "TopUp") {
                    user.saldo += t.nominal; // TopUp menambah saldo
                } else {
                    user.saldo -= t.nominal; // QRIS/Transfer mengurangi saldo
                }
                user.transaksi.push_back(t);
                riwayat.push_back(t);
                undoStack.push(t);
                cout << "Transaksi berhasil! Saldo sekarang: Rp" << user.saldo << endl;
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
                cout << "Transaksi pending: " << pendingQueue.front().jenis
                     << " | " << pendingQueue.front().merchant
                     << " - Rp" << pendingQueue.front().nominal << endl;
            } else {
                cout << "Tidak ada transaksi pending.\n";
            }
        }
        else if (pilihan == 8) {
            cout << "\n=== TREE KATEGORI ===\n";
            for (auto& k : kategori) {
                cout << k.first << ": ";
                for (auto& sub : k.second) cout << sub << " ";
                cout << endl;
            }
        }
        else if (pilihan == 9) {
            cout << "\n=== GRAPH MERCHANT ===\n";
            for (auto& m : merchantGraph) {
                cout << m.first << " terhubung ke: ";
                for (auto& adj : m.second) cout << adj << " ";
                cout << endl;
            }
        }
    } while (pilihan != 0);

    cout << "\nTerima kasih telah menggunakan E-Wallet!\n";
    return 0;
}
