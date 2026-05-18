#ifndef FILE_IO_H
#define FILE_IO_H

#include "task.h"

/* Doc/ghi du lieu tu file */
int load_data(TaskList* list, const char* filename);
int save_data(TaskList* list, const char* filename);

#endif /* FILE_IO_H */
