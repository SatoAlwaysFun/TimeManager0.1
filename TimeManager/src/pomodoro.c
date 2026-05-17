#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/pomodoro.h"

/* =========================================================
 * Ham ngu da nen tang (giay)
 * ========================================================= */
static void sleep_seconds(int sec) {
#ifdef _WIN32
    /* Windows */
    extern void Sleep(unsigned long);
    Sleep((unsigned long)sec * 1000UL);
#else
    /* POSIX (Linux/macOS) */
    struct timespec ts;
    ts.tv_sec  = (time_t)sec;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);
#endif
}

/* =========================================================
 * Dem nguoc de quy (Ch.2 - dung de quy)
 * ========================================================= */
void countdown(int seconds) {
    if (seconds <= 0) {
        printf("\r  [!] Het gio!                    \n");
        return;
    }

    int mins = seconds / 60;
    int secs = seconds % 60;
    printf("\r  Con lai: %02d:%02d  ", mins, secs);
    fflush(stdout);

    sleep_seconds(1);
    countdown(seconds - 1);     /* de quy */
}

/* =========================================================
 * Pomodoro Timer chinh
 * ========================================================= */
void timer_pomodoro(void) {
    int repeat = 1;
    int session = 1;

    printf("\n");
    printf("  +==================================+\n");
    printf("  |        POMODORO TIMER            |\n");
    printf("  +==================================+\n");
    printf("  Moi chu ky: %d phut lam viec + %d phut nghi\n\n",
           WORK_MINUTES, BREAK_MINUTES);

    while (repeat) {
        /* --- WORK TIME --- */
        printf("  [Phien %d] Bat dau lam viec (%d phut)...\n",
               session, WORK_MINUTES);
        countdown(WORK_MINUTES * 60);

        /* --- BREAK TIME --- */
        printf("  [Phien %d] Nghi ngoi (%d phut)...\n",
               session, BREAK_MINUTES);
        countdown(BREAK_MINUTES * 60);

        session++;

        /* Hoi lap lai */
        char ch;
        printf("\n  Lap lai Pomodoro? (y/n): ");
        scanf(" %c", &ch);
        /* Xoa buffer */
        while (getchar() != '\n');

        repeat = (ch == 'y' || ch == 'Y');
    }

    printf("\n  [OK] Ket thuc Pomodoro Timer. Quay ve MENU.\n\n");
}
