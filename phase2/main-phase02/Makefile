# ============================================================
# Choose ONE language: keep exactly one of the two lines below
# uncommented.
# ============================================================
LANGUAGE = c
# LANGUAGE = java

TARGET = main

.PHONY: all clean

all: $(TARGET)

ifeq ($(LANGUAGE),c)
# ---------------------------- C / C++ ----------------------------
# CC = gcc-10 # Compiler version installed in most department machines
# CC = g++-10 # For C++ compilation
CC = gcc
CFLAGS = -g -O0 -std=c99 -Wall -Wextra -Wpedantic #-DDEBUG_PRINTS_ENABLED -Wno-unused-parameter
# Uncomment -DDEBUG_PRINTS_ENABLED to enable event parsing debug prints
# Uncomment -Wno-unused-parameter to disable unused parameter warnings
LDLIBS = -lm
SRC = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDLIBS)

clean:
	rm -f $(TARGET)

else ifeq ($(LANGUAGE),java)
# ----------------------------- Java ------------------------------
# Produces a single executable file "main" (a runnable jar with a
# small shell launcher in front), so you still run: ./main testfile.txt
JAVAC = javac
JAR = jar
JAVA = java
JFLAGS = -g -Xlint:all -encoding UTF-8
JRUNFLAGS = #-DDEBUG_PRINTS_ENABLED -Xss64m
# Uncomment -DDEBUG_PRINTS_ENABLED to enable event parsing debug prints
# Uncomment -Xss64m to raise the stack size (deep recursion)
SRC = $(wildcard *.java)
MAIN_CLASS = Main
BUILD_DIR = build

$(TARGET): $(SRC) Makefile
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/classes
	$(JAVAC) $(JFLAGS) -d $(BUILD_DIR)/classes $(SRC)
	$(JAR) cfe $(BUILD_DIR)/$(TARGET).jar $(MAIN_CLASS) -C $(BUILD_DIR)/classes .
	printf '#!/bin/sh\nexec $(JAVA) $(JRUNFLAGS) -jar "$$0" "$$@"\n' > $(TARGET)
	cat $(BUILD_DIR)/$(TARGET).jar >> $(TARGET)
	chmod +x $(TARGET)

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

else
$(error LANGUAGE must be "c" or "java")
endif
