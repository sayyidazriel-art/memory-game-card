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

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void shuffle(char *cards, int size) {
    for (int i = 0; i < size; i++) {
        int r = rand() % size;
        char t = cards[i];
        cards[i] = cards[r];
        cards[r] = t;
    }
}

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
    while (fscanf(file, "%s %d %d",
                  papanSkor[jumlahSkor].nama,
                  &papanSkor[jumlahSkor].level,
                  &papanSkor[jumlahSkor].skor) != EOF)
    {
        jumlahSkor++;
        if (jumlahSkor >= MAX) break;
    }
    fclose(file);
}

void resetSkor() {
    FILE *file = fopen("papan_score.txt", "w");
    if (!file) return;
    fclose(file);

    printf("\n===== Papan skor berhasil dibersihkan! =====\n");
    printf("Tekan Enter untuk kembali...");
    clearInput();
    getchar();
}

void tampilkanSkor() {
    loadSkor();

    for (int i = 0; i < jumlahSkor - 1; i++) {
        for (int j = i + 1; j < jumlahSkor; j++) {
            if (papanSkor[i].level > papanSkor[j].level ||
               (papanSkor[i].level == papanSkor[j].level &&
                papanSkor[i].skor > papanSkor[j].skor))
            {
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
    clearInput();
    getchar();
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

void penjelasanGame() {
    system(CLEAR);
    printf("=================================== PENJELASAN GAME ===================================\n\n");
    printf("Memory Game adalah permainan mencocokkan pasangan kartu.\n");
    printf("Tujuan: Temukan semua pasangan kartu dengan langkah sesedikit mungkin.\n\n");

    printf("Aturan:\n");
    printf("1. Semua kartu tertutup di awal.\n");
    printf("2. Pilih dua posisi.\n");
    printf("3. Jika cocok, tetap terbuka.\n");
    printf("4. Jika tidak, menutup kembali.\n\n");

    printf("Kontrol:\n");
    printf("- Pilih kartu dengan memasukkan nomor.\n");
    printf("- Masukkan 'q' jika ingin keluar.\n\n");

    printf("Skor berdasar jumlah langkah.\n\n");

    printf("Tekan Enter untuk kembali...");
    clearInput();
    getchar();
}

int main() {
    penjelasanGame();

    int pilihan;

    while (1) {
        printf("\n=== MEMORY GAME ===\n");
        printf("1. Petunjuk game\n");
        printf("2. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: penjelasanGame();
                    break;
            case 2: printf("Terima kasih!\n");
                    return 0;
            default: printf("Pilihan tidak valid!\n");
        }
    }
}
