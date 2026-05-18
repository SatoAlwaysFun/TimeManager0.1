#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_io.h"

/* =========================================================
 * Tai du lieu tu file (Ch.4)
 * Dinh dang moi dong: id|name|duration|status
 * Tra ve so task doc duoc, -1 neu loi mo file
 * ========================================================= */
int load_data(TaskList* list, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        /* File chua ton tai lan dau chay - khong phai loi */
        return 0;
    }

    char line[MAX_LINE_LEN];
    int loaded = 0;
    int max_id = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Bo qua dong comment hoac rong */
        if (line[0] == '#' || line[0] == '\n') continue;

        Task* node = (Task*)malloc(sizeof(Task));
        if (node == NULL) {
            fprintf(stderr, "[LOI] Khong du bo nho khi tai du lieu!\n");
            fclose(fp);
            return -1;
        }
        node->next = NULL;

        /* Parse: id|name|duration|status */
        char tmp_name[MAX_NAME_LEN];
        int parsed = sscanf(line, "%d|%99[^|]|%d|%d",
                            &node->id, tmp_name,
                            &node->duration, &node->status);

        if (parsed != 4) {
            free(node);
            continue;   /* Bo qua dong loi dinh dang */
        }

        strncpy(node->name, tmp_name, MAX_NAME_LEN - 1);
        node->name[MAX_NAME_LEN - 1] = '\0';

        /* Them vao cuoi linked list */
        if (list->head == NULL) {
            list->head = node;
        } else {
            Task* cur = list->head;
            while (cur->next != NULL) cur = cur->next;
            cur->next = node;
        }
        list->count++;

        if (node->id > max_id) max_id = node->id;
        loaded++;
    }

    /* Dong file sau khi dung (Ch.4) */
    fclose(fp);

    /* Dam bao next_id khong trung */
    if (max_id >= list->next_id)
        list->next_id = max_id + 1;

    return loaded;
}

/* =========================================================
 * Ghi du lieu ra file (Ch.4)
 * Tra ve 1 neu thanh cong, 0 neu loi
 * ========================================================= */
int save_data(TaskList* list, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "[LOI] Khong the mo file de ghi: %s\n", filename);
        return 0;
    }

    fprintf(fp, "# TimeManager Data File\n");
    fprintf(fp, "# Format: id|name|duration|status\n");

    Task* cur = list->head;
    while (cur != NULL) {
        fprintf(fp, "%d|%s|%d|%d\n",
                cur->id, cur->name, cur->duration, cur->status);
        cur = cur->next;
    }

    /* Dong file sau khi dung */
    fclose(fp);
    return 1;
}
