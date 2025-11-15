#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define MAX 100

typedef struct {
    char nama[50];
    int level;
    int skor;
} Score;

Score papanSkor[MAX];
int jumlahSkor = 0;

void simpanSkor(char *nama, int level, int skor) {
    FILE *file = fopen("papan_score.txt", "a");
    if (!file) return;
    fprintf(file, "%s %d %d\n", nama, level, skor);
    fclose(file);
}

void loadSkor() {
    FILE *file = fopen("papan_score.txt", "r");
    if (!file) return;

    jumlahSkor = 0;
    while (
        fscanf(file, "%s %d %d",
        papanSkor[jumlahSkor].nama,
        &papanSkor[jumlahSkor].level,
        &papanSkor[jumlahSkor].skor) != EOF)
    {
        jumlahSkor++;
    }
    fclose(file);
}

void tampilkanSkor() {
    loadSkor();

    // Sorting berdasarkan level lalu skor
    for (int i = 0; i < jumlahSkor - 1; i++) {
        for (int j = i + 1; j < jumlahSkor; j++) {

            // Jika level i lebih besar → tukar (level kecil dulu)
            if (papanSkor[i].level > papanSkor[j].level) {
                Score tmp = papanSkor[i];
                papanSkor[i] = papanSkor[j];
                papanSkor[j] = tmp;
            }

            // Jika level sama → urutkan berdasarkan skor (langkah)
            else if (papanSkor[i].level == papanSkor[j].level &&
                     papanSkor[i].skor > papanSkor[j].skor) {
                Score tmp = papanSkor[i];
                papanSkor[i] = papanSkor[j];
                papanSkor[j] = tmp;
            }
        }
    }
    system(CLEAR);
    printf("=========== Papan Skor ===========\n");
    printf("%-15s | %-5s | %-5s\n", "Nama", "Lvl", "Skor");
    printf("----------------------------------\n");

    for (int i = 0; i < jumlahSkor; i++) {
        printf("%-15s | %-5d | %-5d\n",
            papanSkor[i].nama,
            papanSkor[i].level,
            papanSkor[i].skor);
    }

    printf("\nTekan Enter untuk kembali...");
    getchar(); getchar();
}

void tampilkanBoard(char *cards, int *open, int size) {
    system(CLEAR);
    printf("=========== MEMORY GAME ===========\n\n");

    int side = sqrt(size);

    for (int i = 0; i < size; i++) {
        if (open[i])
            printf(" [ %c] ", cards[i]);
        else
            printf(" [%2d] ", i + 1);

        if ((i + 1) % side == 0)
            printf("\n");
    }
    printf("\n");
}

void shuffle(char *cards, int size) {
    for (int i = 0; i < size; i++) {
        int r = rand() % size;
        char t = cards[i];
        cards[i] = cards[r];
        cards[r] = t;
    }
}

void resetSkor() {
    FILE *file = fopen("papan_score.txt", "w");
    if (!file) return;
    fclose(file);

    printf("\n===== Papan skor berhasil dibersihkan! =====\n");
    printf("Tekan Enter untuk kembali...");
    getchar(); getchar();
}

void playGame() {
    char nama[50];
    printf("Masukkan nama anda: ");
    scanf("%s", nama);

    int level, size;
    printf("\n====== PILIH LEVEL ======\n");
    printf("1. Veri Easy     (2x2)\n");
    printf("1. Easy          (4x4)\n");
    printf("2. Medium        (6x6)\n");
    printf("3. Hard          (8x8)\n");
    printf("Pilih level: ");
    scanf("%d", &level);

    if (level == 1) size = 4;
    else if (level == 2) size = 16;
    else if (level == 3) size = 36;
    else if (level == 4) size = 64;
    else {
        printf("Level tidak valid!\n");
        return;
    }

    // Alokasi memory
    char *cards = malloc(size);
    int *open = calloc(size, sizeof(int));

    int pairs = size / 2;

    // Generate simbol kartu
    for (int i = 0; i < pairs; i++) {
        char sym = (i < 26) ? 'A' + i : 'a' + (i - 26);
        cards[i * 2]     = sym;
        cards[i * 2 + 1] = sym;
    }

    srand(time(NULL));
    shuffle(cards, size);

    int langkah = 0;
    int selesai = 0;

    while (!selesai) {
        tampilkanBoard(cards, open, size);

        int p1, p2;

        while (1) {
            printf("Pilih posisi pertama (1-%d): ", size);
            scanf("%d", &p1);
            printf("Pilih posisi kedua   (1-%d): ", size);
            scanf("%d", &p2);

            if (p1 == 0 && p2 == 0) {
                char y;
                printf("Apakah kamu yakin keluar? [y/n]: ");
                scanf(" %c", &y);
                if (y == 'y') {
                    free(cards);
                    free(open);
                    system(CLEAR);
                    printf("===== GAME DIBATALKAN =====\n");
                    return;
                }
                continue;
            }

            p1--; p2--;

            if (p1 < 0 || p2 < 0 || p1 >= size || p2 >= size) {
                printf("Posisi tidak valid!\n\n");
                continue;
            }
            if (p1 == p2) {
                printf("Tidak boleh posisi sama!\n\n");
                continue;
            }
            if (open[p1] || open[p2]) {
                printf("Ada kartu sudah terbuka!\n\n");
                continue;
            }
            break;
        }

        langkah++;
        open[p1] = open[p2] = 1;

        tampilkanBoard(cards, open, size);

        if (cards[p1] == cards[p2]) {
            printf("Cocok!\n(Enter untuk lanjut)");
        } else {
            printf("Tidak cocok!\n(Enter untuk lanjut)");
            open[p1] = open[p2] = 0;
        }

        getchar(); getchar();

        selesai = 1;
        for (int i = 0; i < size; i++)
            if (!open[i]) selesai = 0;
    }

    system(CLEAR);
    int skor = (size * 1000) - langkah * 10;

    printf("===== GAME SELESAI =====\n");
    printf("Total langkah : %d\n", langkah);
    printf("Skor akhir    : %d\n", (size * 1000) - langkah * 10);
    printf("=========================\n");

    simpanSkor(nama, level, skor);
    tampilkanSkor();

    free(cards);
    free(open);
}

void penjelasanGame() {
    system(CLEAR);
    printf("=================================== PENJELASAN GAME ===================================\n\n");

    printf("Memory Game adalah permainan mencocokkan pasangan kartu.\n");
    printf("Tujuan permainan: Temukan semua pasangan kartu dengan langkah sesedikit mungkin.\n\n");

    printf("Aturan permainan:\n");
    printf("1. Pada awal game semua kartu tertutup.\n");
    printf("2. Setiap giliran, pilih dua posisi kartu.\n");
    printf("3. Jika dua kartu sama, maka kartu tetap terbuka.\n");
    printf("4. Jika dua kartu berbeda, maka kartu akan menutup kembali.\n");
    printf("5. Permainan selesai ketika semua kartu terbuka.\n\n");

    printf("Kontrol:\n");
    printf("- Pilih kartu dengan memasukkan nomor posisi kartu.\n");
    printf("- Jika ingin keluar saat bermain, masukkan: 0 0\n");
    printf("- Setelah membuka dua kartu, tekan Enter untuk melanjutkan.\n\n");

    printf("Skor:\n");
    printf("- Skor dihitung berdasarkan langkah.\n");
    printf("- Semakin sedikit langkah, maka semakin tinggi skor.\n\n");

    printf("Tekan Enter untuk kembali...\n");
    printf("=======================================================================================");
    getchar(); getchar();
}

int main() {
    penjelasanGame();
    srand(time(NULL));

    int pilihan;

    while (1) {
        printf("\n=== MEMORY GAME ===\n");
        printf("1. Petunjuk game\n");
        printf("2. Mulai game\n");
        printf("3. Lihat Papan Skor\n");
        printf("4. Reset papan skor\n");
        printf("5. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: penjelasanGame(); break;
            case 2: playGame(); break;
            case 3: tampilkanSkor(); break;
            case 4: resetSkor(); break;
            case 5: printf("Terima kasih!\n"); return 0;
            default: printf("Pilihan tidak valid!\n");
        }
    }
}
