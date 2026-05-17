#ifndef TASK_H
#define TASK_H

#define MAX_NAME_LEN 100
#define MAX_LINE_LEN 256
#define DATA_FILE "data/tasks.dat"

/* Cau truc du lieu chinh - Task (self-referential struct) */
typedef struct Task {
    int id;                     /* ID cong viec */
    char name[MAX_NAME_LEN];    /* Ten cong viec */
    int duration;               /* Thoi gian (phut) */
    int status;                 /* 0: Chua xong, 1: Da xong */
    struct Task* next;          /* Con tro toi task tiep theo */
} Task;

/* Danh sach lien ket */
typedef struct {
    Task* head;
    int count;
    int next_id;
} TaskList;

/* Khoi tao va giai phong */
void init_list(TaskList* list);
void free_list(TaskList* list);

/* CRUD operations */
int   add_task(TaskList* list, const char* name, int duration);
int   delete_task(TaskList* list, int id);
int   update_task(TaskList* list, int id, const char* name, int duration, int status);
Task* find_task_by_id(TaskList* list, int id);
void  show_tasks(TaskList* list);

/* De quy - in danh sach theo thu tu nguoc */
void show_tasks_reverse(Task* node);

#endif /* TASK_H */
