CC=gcc
CFLAGS=-Wall -Iinclude -g

BUILD_DIR=.
SOURCE_DIR=src

TARGET=ls2

default: main.o ls2.o valid_malloc.o file_stack.o dynamic_string.o
	$(CC) $(CFLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(TARGET)
	rm $(BUILD_DIR)/*.o

main.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/main.c -o $(BUILD_DIR)/main.o

ls2.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/ls2.c -o $(BUILD_DIR)/ls2.o

valid_malloc.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/valid_malloc.c -o $(BUILD_DIR)/valid_malloc.o

file_stack.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/file_stack.c -o $(BUILD_DIR)/file_stack.o

dynamic_string.o:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/dynamic_string.c -o $(BUILD_DIR)/dynamic_string.o
