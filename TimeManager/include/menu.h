#ifndef MENU_H
#define MENU_H

#include "task.h"

/* Hien thi menu chinh */
void show_menu(void);

/* Xu ly cac chuc nang menu */
void handle_add_task(TaskList* list);
void handle_delete_task(TaskList* list);
void handle_update_task(TaskList* list);
void handle_show_tasks(TaskList* list);

/* Validate dau vao */
int  validate_name(const char* name);
int  validate_duration(int duration);
int  read_int(const char* prompt, int* out);
void read_string(const char* prompt, char* buf, int max_len);

/* Tien ich */
void clear_screen(void);
void print_separator(void);
void press_enter(void);

#endif /* MENU_H */
