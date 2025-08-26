CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
UNAME := $(shell uname)

ifeq ($(UNAME), APPLE)
CFLAGS = -lcheck
endif

ifeq ($(UNAME), Linux)
CFLAGS = -lrt -lcheck -lpthread -lsubunit -lm
GCFLAGS = -lrt -lcheck -lpthread -lsubunit -lm -fprofile-arcs -ftest-coverage  
endif

all: clean s21_string.a test gcov_report

s21_string.o: s21_string.c s21_string.h
	$(CC) $(CFLAGS) -c s21_string.c -o s21_string.o

s21_string.a: s21_string.o
	ar rc s21_string.a s21_string.o

test:
	@mkdir -p Tests 
	@checkmk clean_mode=1 checks/s21_memchr.check > Tests/s21_memchr.c
	@checkmk clean_mode=1 checks/s21_memcmp.check > Tests/s21_memcmp.c
	@checkmk clean_mode=1 checks/s21_memcpy.check > Tests/s21_memcpy.c
	@checkmk clean_mode=1 checks/s21_memset.check > Tests/s21_memset.c
	@checkmk clean_mode=1 checks/s21_strchr.check > Tests/s21_strchr.c
	@checkmk clean_mode=1 checks/s21_strncmp.check > Tests/s21_strncmp.c
	@checkmk clean_mode=1 checks/s21_strcspn.check > Tests/s21_strcspn.c
	@checkmk clean_mode=1 checks/s21_strerror.check > Tests/s21_strerror.c
	@checkmk clean_mode=1 checks/s21_strlen.check > Tests/s21_strlen.c
	@checkmk clean_mode=1 checks/s21_strncat.check > Tests/s21_strncat.c
	@checkmk clean_mode=1 checks/s21_strncpy.check > Tests/s21_strncpy.c
	@checkmk clean_mode=1 checks/s21_strpbrk.check > Tests/s21_strpbrk.c
	@checkmk clean_mode=1 checks/s21_insert.check > Tests/s21_insert.c
	@checkmk clean_mode=1 checks/s21_strrchr.check > Tests/s21_strrchr.c
	@checkmk clean_mode=1 checks/s21_strtok.check > Tests/s21_strtok.c
	@checkmk clean_mode=1 checks/s21_to_lower.check > Tests/s21_to_lower.c
	@checkmk clean_mode=1 checks/s21_to_upper.check > Tests/s21_to_upper.c
	@checkmk clean_mode=1 checks/s21_trim.check > Tests/s21_trim.c
	@checkmk clean_mode=1 checks/s21_strstr.check > Tests/s21_strstr.c
	@checkmk clean_mode=1 checks/sprintf.check > Tests/s21_stringtest.c
	@$(CC) $(FLAGS) -o Tests/test_memchr Tests/s21_memchr.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_memcmp Tests/s21_memcmp.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strchr Tests/s21_strchr.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strncmp Tests/s21_strncmp.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strlen Tests/s21_strlen.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strpbrk Tests/s21_strpbrk.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strncpy Tests/s21_strncpy.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strncat Tests/s21_strncat.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strerror Tests/s21_strerror.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strcspn Tests/s21_strcspn.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_memcpy Tests/s21_memcpy.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_memset Tests/s21_memset.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_insert Tests/s21_insert.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strrchr Tests/s21_strrchr.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strtok Tests/s21_strtok.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_to_lower Tests/s21_to_lower.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_to_upper Tests/s21_to_upper.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_trim Tests/s21_trim.c s21_string.c $(GCFLAGS)
	@$(CC) -o Tests/test_sprintf Tests/s21_stringtest.c s21_sprintf.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strstr Tests/s21_strstr.c s21_string.c $(GCFLAGS)
	printf "\n\n\n====================================== Результаты: ======================================\n"
	./Tests/test_memset
	./Tests/test_strchr
	./Tests/test_strncmp
	./Tests/test_strlen
	./Tests/test_memchr
	./Tests/test_strpbrk
	./Tests/test_strncpy
	./Tests/test_strncat
	./Tests/test_memcmp
	./Tests/test_strerror
	./Tests/test_strcspn
	./Tests/test_memcpy
	./Tests/test_insert
	./Tests/test_strrchr
	./Tests/test_strtok
	./Tests/test_to_lower
	./Tests/test_to_upper
	./Tests/test_trim
	./Tests/test_strstr
	./Tests/test_sprintf

clean:
	rm -rf Tests/* *.html all* ./Web/* ./test *.css coverage* s21_stringtest.c test_sprintf *.gcov *.gcda *.gcno *.o
	
gcov_report: clean
	@mkdir -p Web
	@mkdir -p Tests 
	@checkmk clean_mode=1 checks/s21_memchr.check > Tests/s21_memchr.c
	@checkmk clean_mode=1 checks/s21_memcmp.check > Tests/s21_memcmp.c
	@checkmk clean_mode=1 checks/s21_memcpy.check > Tests/s21_memcpy.c
	@checkmk clean_mode=1 checks/s21_memset.check > Tests/s21_memset.c
	@checkmk clean_mode=1 checks/s21_strchr.check > Tests/s21_strchr.c
	@checkmk clean_mode=1 checks/s21_strncmp.check > Tests/s21_strncmp.c
	@checkmk clean_mode=1 checks/s21_strcspn.check > Tests/s21_strcspn.c
	@checkmk clean_mode=1 checks/s21_strerror.check > Tests/s21_strerror.c
	@checkmk clean_mode=1 checks/s21_strlen.check > Tests/s21_strlen.c
	@checkmk clean_mode=1 checks/s21_strncat.check > Tests/s21_strncat.c
	@checkmk clean_mode=1 checks/s21_strncpy.check > Tests/s21_strncpy.c
	@checkmk clean_mode=1 checks/s21_strpbrk.check > Tests/s21_strpbrk.c
	@checkmk clean_mode=1 checks/s21_insert.check > Tests/s21_insert.c
	@checkmk clean_mode=1 checks/s21_strrchr.check > Tests/s21_strrchr.c
	@checkmk clean_mode=1 checks/s21_strtok.check > Tests/s21_strtok.c
	@checkmk clean_mode=1 checks/s21_to_lower.check > Tests/s21_to_lower.c
	@checkmk clean_mode=1 checks/s21_to_upper.check > Tests/s21_to_upper.c
	@checkmk clean_mode=1 checks/s21_trim.check > Tests/s21_trim.c
	@checkmk clean_mode=1 checks/s21_strstr.check > Tests/s21_strstr.c
	@checkmk clean_mode=1 checks/sprintf.check > Tests/s21_stringtest.c
	@$(CC) $(FLAGS) -o Tests/test_memchr Tests/s21_memchr.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_memcmp Tests/s21_memcmp.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strchr Tests/s21_strchr.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strncmp Tests/s21_strncmp.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strlen Tests/s21_strlen.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strpbrk Tests/s21_strpbrk.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strncpy Tests/s21_strncpy.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strncat Tests/s21_strncat.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strerror Tests/s21_strerror.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strcspn Tests/s21_strcspn.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_memcpy Tests/s21_memcpy.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_memset Tests/s21_memset.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_insert Tests/s21_insert.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strrchr Tests/s21_strrchr.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strtok Tests/s21_strtok.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_to_lower Tests/s21_to_lower.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_to_upper Tests/s21_to_upper.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_trim Tests/s21_trim.c s21_string.c $(GCFLAGS)
	@$(CC) -o Tests/test_sprintf Tests/s21_stringtest.c s21_sprintf.c s21_string.c $(GCFLAGS)
	@$(CC) $(FLAGS) -o Tests/test_strstr Tests/s21_strstr.c s21_string.c $(GCFLAGS)
	@./Tests/test_memset
	@./Tests/test_strchr
	@./Tests/test_strncmp
	@./Tests/test_strlen
	@./Tests/test_memchr
	@./Tests/test_strpbrk
	@./Tests/test_strncpy
	@./Tests/test_strncat
	@./Tests/test_memcmp
	@./Tests/test_strerror
	@./Tests/test_strcspn
	@./Tests/test_memcpy
	@./Tests/test_insert
	@./Tests/test_strrchr
	@./Tests/test_strtok
	@./Tests/test_to_lower
	@./Tests/test_to_upper
	@./Tests/test_trim
	@./Tests/test_strstr
	@./Tests/test_sprintf
	gcov ./Tests/*.gcno ./Tests/*.gcda 
	find ./Tests/ ! -name '*s21_string.*' ! -name '*s21_sprintf.*' -type f -delete
	gcovr -r . --html --html-details -o coverage_report.html
	mv ./*.html ./Web
	mv ./coverage* ./Web
	open Web/coverage_report.html