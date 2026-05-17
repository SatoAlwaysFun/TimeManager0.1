#include <stdio.h>
#include <stdlib.h>

#include "include/task.h"
#include "include/file_io.h"
#include "include/menu.h"
#include "include/pomodoro.h"

/* =========================================================
 * Dam bao thu muc data ton tai
 * ========================================================= */
static void ensure_data_dir(void) {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif
}

/* =========================================================
 * Chuong trinh chinh - Time Manager
 * ========================================================= */
int main(void) {
    TaskList list;
    init_list(&list);

    /* Dam bao thu muc data ton tai */
    ensure_data_dir();

    /* Tai du lieu tu file khi khoi dong (Ch.4) */
    int loaded = load_data(&list, DATA_FILE);
    if (loaded > 0)
        printf("  [OK] Da tai %d task tu file.\n", loaded);
    else if (loaded == 0)
        printf("  [i] Bat dau voi danh sach trong.\n");
    else
        fprintf(stderr, "  [!] Loi tai du lieu tu file!\n");

    int choice = 0;
    int running = 1;

    while (running) {
        clear_screen();
        show_menu();

        if (!read_int("", &choice)) {
            /* Doc loi - xoa buffer */
            while (getchar() != '\n');
            choice = -1;
        }

        switch (choice) {
            case 1:
                handle_add_task(&list);
                break;

            case 2:
                handle_delete_task(&list);
                break;

            case 3:
                handle_update_task(&list);
                break;

            case 4:
                handle_show_tasks(&list);
                break;

            case 5:
                clear_screen();
                timer_pomodoro();
                press_enter();
                break;

            case 6: {
                /* Xac nhan thoat */
                char ch;
                printf("\n  Xac nhan thoat? (y/n): ");
                scanf(" %c", &ch);
                while (getchar() != '\n');

                if (ch == 'y' || ch == 'Y') {
                    /* Luu du lieu ra file truoc khi thoat (Ch.4) */
                    if (save_data(&list, DATA_FILE))
                        printf("  [OK] Du lieu da duoc luu.\n");
                    else
                        fprintf(stderr, "  [!] Loi luu du lieu!\n");

                    running = 0;
                }
                break;
            }

            default:
                printf("\n  [LOI] Lua chon khong hop le. Vui long chon 1-6.\n");
                press_enter();
                break;
        }
    }

    /* Giai phong toan bo bo nho truoc khi ket thuc */
    free_list(&list);

    printf("\n  Cam on da dung Time Manager! Tam biet.\n\n");
    return 0;
}
