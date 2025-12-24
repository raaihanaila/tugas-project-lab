#include <stdio.h>
#include <string.h>

#define MAX_USER 50

// STRUCT
struct User {
    char username[50];
    char password[50];
    int cycle;
    char hari[10];
    int day, month, year;
};

// VALIDASI 
int validHari(char hari[]) {
    char daftarHari[7][10] = {
        "Senin", "Selasa", "Rabu",
        "Kamis", "Jumat", "Sabtu", "Minggu"
    };

    for (int i = 0; i < 7; i++) {
        if (strcmp(hari, daftarHari[i]) == 0)
            return 1;
    }
    return 0;
}

int validTanggal(int day) {
    return (day >= 1 && day <= 31);
}

int validBulan(int month) {
    return (month >= 1 && month <= 12);
}

int validTahun(int year) {
    return (year >= 1900 && year <= 2100);
}

//  SORTING 
// Bubble sort berdasarkan tanggal TERBARU
void sortUsersByDate(struct User users[], int count) {
    struct User temp;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {

            int kosong1 = (users[j].day == 0);
            int kosong2 = (users[j + 1].day == 0);

            if (kosong1 && !kosong2) {
                temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
            else if (!kosong1 && !kosong2) {
                if (users[j].year < users[j + 1].year ||
                   (users[j].year == users[j + 1].year &&
                    users[j].month < users[j + 1].month) ||
                   (users[j].year == users[j + 1].year &&
                    users[j].month == users[j + 1].month &&
                    users[j].day < users[j + 1].day)) {

                    temp = users[j];
                    users[j] = users[j + 1];
                    users[j + 1] = temp;
                }
            }
        }
    }
}

//  FILE 
int loadUsers(struct User users[]) {
    FILE *f = fopen("users.txt", "r");
    if (!f) return 0;

    int count = 0;
    while (fscanf(f, "%s %s %d %s %d %d %d",
                  users[count].username,
                  users[count].password,
                  &users[count].cycle,
                  users[count].hari,
                  &users[count].day,
                  &users[count].month,
                  &users[count].year) == 7) {
        count++;
    }
    fclose(f);
    return count;
}

void saveUsers(struct User users[], int count) {
    FILE *f = fopen("users.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %s %d %s %d %d %d\n",
                users[i].username,
                users[i].password,
                users[i].cycle,
                users[i].hari,
                users[i].day,
                users[i].month,
                users[i].year);
    }
    fclose(f);
}

// SEARCH 
int usernameExists(struct User users[], int count, char uname[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, uname) == 0)
            return 1;
    }
    return 0;
}

// LOGIN 
int login(struct User users[], int count) {
    char name[50], pass[50];

    printf("Username: ");
    scanf("%s", name);
    printf("Password: ");
    scanf("%s", pass);

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, name) == 0 &&
            strcmp(users[i].password, pass) == 0)
            return i;
    }

    printf("Login gagal!\n");
    return -1;
}


void registerUser(struct User users[], int *count) {
    char name[50], pass[50];

    printf("Buat Username: ");
    scanf("%s", name);

    if (usernameExists(users, *count, name)) {
        printf("Username sudah dipakai!\n");
        return;
    }

    printf("Buat Password: ");
    scanf("%s", pass);

    strcpy(users[*count].username, name);
    strcpy(users[*count].password, pass);
    users[*count].cycle = 0;
    strcpy(users[*count].hari, "-");
    users[*count].day = 0;
    users[*count].month = 0;
    users[*count].year = 0;

    (*count)++;
    printf("Akun berhasil dibuat!\n");
}

void mainMenu(struct User *user) {
    int choice;

    while (1) {
        printf("\n=== MAIN MENU ===\n");
        printf("1. Lihat data cycle\n");
        printf("2. Update data cycle\n");
        printf("3. Logout\n");
        printf("Pilih: ");
        scanf("%d", &choice);

        if (choice == 1) {
            if (user->day == 0) {
                printf("Data cycle belum diinput.\n");
            } else {
                printf("Cycle : %d hari\n", user->cycle);
                printf("Hari  : %s\n", user->hari);
                printf("Tanggal terakhir: %02d-%02d-%04d\n",
                       user->day, user->month, user->year);
            }
        }

        else if (choice == 2) {
            printf("Panjang siklus (hari): ");
            scanf("%d", &user->cycle);

            do {
                printf("Hari (Senin-Minggu): ");
                scanf("%s", user->hari);
            } while (!validHari(user->hari));

            do {
                printf("Tanggal (1-31): ");
                scanf("%d", &user->day);
            } while (!validTanggal(user->day));

            do {
                printf("Bulan (1-12): ");
                scanf("%d", &user->month);
            } while (!validBulan(user->month));

            do {
                printf("Tahun (2025-2100): ");
                scanf("%d", &user->year);
            } while (!validTahun(user->year));

            printf("Data berhasil diperbarui!\n");
        }

        else if (choice == 3) {
            printf("Logout...\n");
            break;
        }

        else {
            printf("Pilihan tidak valid!\n");
        }
    }
}


int main() {
    struct User users[MAX_USER];
    int userCount = loadUsers(users);

    sortUsersByDate(users, userCount);

    int pilihan, indexLogin;

    while (1) {
        printf("\n==== PAGE AWAL ====\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            indexLogin = login(users, userCount);
            if (indexLogin != -1) {
                mainMenu(&users[indexLogin]);
                sortUsersByDate(users, userCount);
                saveUsers(users, userCount);
            }
        }
        else if (pilihan == 2) {
            registerUser(users, &userCount);
            sortUsersByDate(users, userCount);
            saveUsers(users, userCount);
        }
        else if (pilihan == 3) {
            printf("Program selesai.\n");
            break;
        }
        else {
            printf("Pilihan tidak tersedia!\n");
        }
    }

    return 0;
}

