# ===================================================
# Makefile - Time Manager
# Chuẩn C99, GCC, không cảnh báo lỗi
# ===================================================

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -Wpedantic -g
TARGET  = timemanager
SRCDIR  = src
INCDIR  = include

SRCS = main.c \
       $(SRCDIR)/task.c \
       $(SRCDIR)/file_io.c \
       $(SRCDIR)/menu.c \
       $(SRCDIR)/pomodoro.c

OBJS = $(SRCS:.c=.o)

# Thêm -lrt cho nanosleep trên Linux cũ
LIBS =

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "✅ Build thành công: ./$(TARGET)"

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "🧹 Đã xóa file build."

run: all
	./$(TARGET)

.PHONY: all clean run
