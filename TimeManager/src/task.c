#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/task.h"

/* =========================================================
 * Khoi tao danh sach lien ket
 * ========================================================= */
void init_list(TaskList* list) {
    list->head     = NULL;
    list->count    = 0;
    list->next_id  = 1;
}

/* =========================================================
 * Giai phong toan bo bo nho danh sach (khong ro ri)
 * ========================================================= */
void free_list(TaskList* list) {
    Task* cur = list->head;
    while (cur != NULL) {
        Task* tmp = cur->next;
        free(cur);          /* Moi malloc deu co free tuong ung */
        cur = tmp;
    }
    list->head    = NULL;
    list->count   = 0;
}

/* =========================================================
 * Them task moi vao cuoi danh sach lien ket
 * Tra ve id moi hoac -1 neu that bai
 * ========================================================= */
int add_task(TaskList* list, const char* name, int duration) {
    Task* node = (Task*)malloc(sizeof(Task));   /* cap phat dong */
    if (node == NULL) {
        fprintf(stderr, "[LOI] Khong du bo nho de them task!\n");
        return -1;
    }

    node->id       = list->next_id++;
    strncpy(node->name, name, MAX_NAME_LEN - 1);
    node->name[MAX_NAME_LEN - 1] = '\0';
    node->duration = duration;
    node->status   = 0;         /* Mac dinh: Chua xong */
    node->next     = NULL;

    /* Them vao cuoi danh sach */
    if (list->head == NULL) {
        list->head = node;
    } else {
        Task* cur = list->head;
        while (cur->next != NULL) cur = cur->next;
        cur->next = node;
    }

    list->count++;
    return node->id;
}

/* =========================================================
 * Xoa task theo ID
 * Tra ve 1 neu thanh cong, 0 neu khong tim thay
 * ========================================================= */
int delete_task(TaskList* list, int id) {
    Task* cur  = list->head;
    Task* prev = NULL;

    while (cur != NULL) {
        if (cur->id == id) {
            if (prev == NULL)
                list->head = cur->next;
            else
                prev->next = cur->next;

            free(cur);          /* Giai phong bo nho */
            list->count--;
            return 1;
        }
        prev = cur;
        cur  = cur->next;
    }
    return 0;   /* Khong tim thay */
}

/* =========================================================
 * Cap nhat thong tin task theo ID
 * Tra ve 1 neu thanh cong, 0 neu khong tim thay
 * ========================================================= */
int update_task(TaskList* list, int id,
                const char* name, int duration, int status) {
    Task* node = find_task_by_id(list, id);
    if (node == NULL) return 0;

    strncpy(node->name, name, MAX_NAME_LEN - 1);
    node->name[MAX_NAME_LEN - 1] = '\0';
    node->duration = duration;
    node->status   = status;
    return 1;
}

/* =========================================================
 * Tim task theo ID (tra ve con tro hoac NULL)
 * ========================================================= */
Task* find_task_by_id(TaskList* list, int id) {
    Task* cur = list->head;
    while (cur != NULL) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* =========================================================
 * Hien thi danh sach task (theo thu tu)
 * ========================================================= */
void show_tasks(TaskList* list) {
    if (list->head == NULL) {
        printf("  [!] Danh sach trong.\n");
        return;
    }

    printf("\n  %-5s %-30s %-10s %-12s\n",
           "ID", "TEN CONG VIEC", "THOI GIAN", "TRANG THAI");
    printf("  %-5s %-30s %-10s %-12s\n",
           "----", "-----------------------------", "---------", "-----------");

    Task* cur = list->head;
    while (cur != NULL) {
        printf("  %-5d %-30s %-10d %-12s\n",
               cur->id,
               cur->name,
               cur->duration,
               (cur->status == 1) ? "Da xong" : "Chua xong");
        cur = cur->next;
    }
    printf("\n  Tong: %d task(s)\n", list->count);
}

/* =========================================================
 * In danh sach theo thu tu NGUOC (dung de quy - Ch.2)
 * ========================================================= */
void show_tasks_reverse(Task* node) {
    if (node == NULL) return;           /* base case */
    show_tasks_reverse(node->next);     /* de quy */
    printf("  [%d] %s (%d phut) - %s\n",
           node->id, node->name, node->duration,
           (node->status == 1) ? "Da xong" : "Chua xong");
}
