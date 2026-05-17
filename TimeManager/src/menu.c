#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/menu.h"

/* =========================================================
 * Tien ich man hinh
 * ========================================================= */
void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_separator(void) {
    printf("  ==========================================\n");
}

void press_enter(void) {
    printf("\n  [Nhan Enter de tiep tuc...]");
    while (getchar() != '\n');
}

/* =========================================================
 * Validate ten task (Ch.5 - validate toan bo dau vao)
 * ========================================================= */
int validate_name(const char* name) {
    if (name == NULL || strlen(name) == 0) return 0;
    if (strlen(name) >= MAX_NAME_LEN)       return 0;
    /* Ten khong duoc chi la khoang trang */
    for (int i = 0; name[i]; i++) {
        if (!isspace((unsigned char)name[i])) return 1;
    }
    return 0;
}

/* =========================================================
 * Validate thoi gian (phai > 0 va <= 1440 phut = 24h)
 * ========================================================= */
int validate_duration(int duration) {
    return (duration > 0 && duration <= 1440);
}

/* =========================================================
 * Doc so nguyen an toan
 * Tra ve 1 neu hop le, 0 neu khong
 * ========================================================= */
int read_int(const char* prompt, int* out) {
    char buf[64];
    printf("%s", prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;

    char* end;
    long val = strtol(buf, &end, 10);
    /* end phai tro den '\n' hoac '\0' */
    if (end == buf || (*end != '\n' && *end != '\0')) return 0;

    *out = (int)val;
    return 1;
}

/* =========================================================
 * Doc chuoi an toan
 * ========================================================= */
void read_string(const char* prompt, char* buf, int max_len) {
    printf("%s", prompt);
    if (fgets(buf, max_len, stdin) != NULL) {
        /* Xoa ky tu xuong dong */
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n')
            buf[len - 1] = '\0';
    }
}

/* =========================================================
 * Hien thi MENU chinh
 * ========================================================= */
void show_menu(void) {
    printf("\n");
    printf("  +==========================================+\n");
    printf("  |        [*]  TIME MANAGER  v1.0          |\n");
    printf("  +==========================================+\n");
    printf("  |  1. [+]  Them cong viec                 |\n");
    printf("  |  2. [-]  Xoa cong viec                  |\n");
    printf("  |  3. [E]  Sua cong viec                  |\n");
    printf("  |  4. [L]  Hien thi danh sach             |\n");
    printf("  |  5. [P]  Bat dau Pomodoro Timer         |\n");
    printf("  |  6. [Q]  Thoat chuong trinh             |\n");
    printf("  +==========================================+\n");
    printf("  Lua chon: ");
}

/* =========================================================
 * Xu ly: THEM TASK
 * ========================================================= */
void handle_add_task(TaskList* list) {
    clear_screen();
    printf("\n  -- THEM CONG VIEC MOI --\n");
    print_separator();

    char name[MAX_NAME_LEN];
    int  duration = 0;

    /* Vong lap validate ten */
    while (1) {
        read_string("  Ten cong viec: ", name, MAX_NAME_LEN);
        if (validate_name(name)) break;
        printf("  [LOI] Ten khong hop le. Vui long nhap lai.\n");
    }

    /* Vong lap validate thoi gian */
    while (1) {
        if (!read_int("  Thoi gian (phut, 1-1440): ", &duration) ||
            !validate_duration(duration)) {
            printf("  [LOI] Thoi gian phai tu 1 den 1440 phut.\n");
        } else {
            break;
        }
    }

    int new_id = add_task(list, name, duration);
    if (new_id > 0) {
        printf("\n  [OK] Them thanh cong! (ID: %d)\n", new_id);
    }
    press_enter();
}

/* =========================================================
 * Xu ly: XOA TASK
 * ========================================================= */
void handle_delete_task(TaskList* list) {
    clear_screen();
    printf("\n  -- XOA CONG VIEC --\n");
    print_separator();

    if (list->head == NULL) {
        printf("  [!] Danh sach trong, khong co gi de xoa.\n");
        press_enter();
        return;
    }

    show_tasks(list);
    print_separator();

    int id = 0;
    while (1) {
        if (!read_int("  Nhap ID can xoa (0 = huy): ", &id)) {
            printf("  [LOI] ID khong hop le.\n");
            continue;
        }
        if (id == 0) { printf("  Da huy.\n"); press_enter(); return; }
        break;
    }

    Task* found = find_task_by_id(list, id);
    if (found == NULL) {
        printf("  [LOI] Khong tim thay task voi ID = %d.\n", id);
    } else {
        printf("  Xac nhan xoa task \"%s\"? (y/n): ", found->name);
        char ch;
        scanf(" %c", &ch);
        while (getchar() != '\n');

        if (ch == 'y' || ch == 'Y') {
            delete_task(list, id);
            printf("  [OK] Da xoa thanh cong!\n");
        } else {
            printf("  Da huy thao tac.\n");
        }
    }
    press_enter();
}

/* =========================================================
 * Xu ly: SUA TASK
 * ========================================================= */
void handle_update_task(TaskList* list) {
    clear_screen();
    printf("\n  -- SUA CONG VIEC --\n");
    print_separator();

    if (list->head == NULL) {
        printf("  [!] Danh sach trong.\n");
        press_enter();
        return;
    }

    show_tasks(list);
    print_separator();

    int id = 0;
    while (1) {
        if (!read_int("  Nhap ID can sua (0 = huy): ", &id)) {
            printf("  [LOI] ID khong hop le.\n");
            continue;
        }
        if (id == 0) { printf("  Da huy.\n"); press_enter(); return; }
        break;
    }

    Task* found = find_task_by_id(list, id);
    if (found == NULL) {
        printf("  [LOI] Khong tim thay task voi ID = %d.\n", id);
        press_enter();
        return;
    }

    printf("  Task hien tai: [%d] %s | %d phut | %s\n",
           found->id, found->name, found->duration,
           found->status ? "Da xong" : "Chua xong");
    print_separator();

    char name[MAX_NAME_LEN];
    int  duration = 0, status = 0;

    /* Nhap thong tin moi */
    while (1) {
        read_string("  Ten moi: ", name, MAX_NAME_LEN);
        if (validate_name(name)) break;
        printf("  [LOI] Ten khong hop le.\n");
    }

    while (1) {
        if (!read_int("  Thoi gian moi (phut): ", &duration) ||
            !validate_duration(duration)) {
            printf("  [LOI] Thoi gian phai tu 1 den 1440 phut.\n");
        } else break;
    }

    while (1) {
        if (!read_int("  Trang thai (0=Chua xong, 1=Da xong): ", &status) ||
            (status != 0 && status != 1)) {
            printf("  [LOI] Trang thai phai la 0 hoac 1.\n");
        } else break;
    }

    update_task(list, id, name, duration, status);
    printf("\n  [OK] Cap nhat thanh cong!\n");
    press_enter();
}

/* =========================================================
 * Xu ly: HIEN THI DANH SACH
 * ========================================================= */
void handle_show_tasks(TaskList* list) {
    clear_screen();
    printf("\n  -- DANH SACH CONG VIEC --\n");
    print_separator();
    show_tasks(list);

    if (list->head != NULL) {
        char ch;
        printf("\n  Xem nguoc (reverse order)? (y/n): ");
        scanf(" %c", &ch);
        while (getchar() != '\n');
        if (ch == 'y' || ch == 'Y') {
            printf("\n  -- DANH SACH NGUOC (DE QUY) --\n");
            show_tasks_reverse(list->head);
        }
    }
    press_enter();
}
