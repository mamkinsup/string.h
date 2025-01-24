CC = gcc
TEST_LIBS = -lcheck
ifeq ($(shell uname -s), Linux)
	TEST_LIBS += -lm
endif
CFLAGS = -std=c11 -Wall -Werror -Wextra -g
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

all: clean test

test: s21_string.a
	$(CC) $(CFLAGS) tests/*.c s21_string.a $(TEST_LIBS) -o test
	./test
	rm -rf *.o tests/*.o

s21_string.a: $(OBJECTS)
	ar rcs s21_string.a $(OBJECTS)

gcov_flag:
	$(eval CFLAGS += -fprofile-arcs -ftest-coverage)

gcov_report: clean gcov_flag test
	gcov $(SOURCES)
	rm -rf test*.gcno test*.gcda test*.gcov
	gcovr -s --html-details -o coverage.html
	open coverage.html
	rm -rf *.gcno *.gcda *.gcov
	
style:
	clang-format --style=google -i *.c *.h
	clang-format --style=google -i tests/*.c tests/*.h

rebuild: clean all

test_leaks: test
ifeq ($(shell uname -s), Linux)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test
else
	leaks --atExit -- ./test
endif

clean:
	rm -rf *.a *.o tests/*.o test *.gcno *.gcda *.gcov *.info coverage* *.dSYM

.PHONY: s21_string.a all clean test style rebuild
