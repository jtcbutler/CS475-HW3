CC=gcc
CFLAGS=-Wall -Iinclude -g

BUILD_DIR=.
SOURCE_DIR=src

TARGET=ls2

default: main.o ls2.o v_malloc.o file_stack.o file_path.o
	$(CC) $(CFLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(TARGET)
	rm $(BUILD_DIR)/*.o

main.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/main.c -o $(BUILD_DIR)/main.o

ls2.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/ls2.c -o $(BUILD_DIR)/ls2.o

v_malloc.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/v_malloc.c -o $(BUILD_DIR)/v_malloc.o

file_stack.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/file_stack.c -o $(BUILD_DIR)/file_stack.o

file_path.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/file_path.c -o $(BUILD_DIR)/file_path.o

test: default
	python3 test.py
