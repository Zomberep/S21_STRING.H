#include "s21_string.h"

// поиск первого вхождения символа c (беззнаковый тип) в первых n байтах строки,
// на которую указывает аргумент str.
void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *ptr = s21_NULL;
  const unsigned char *ptr_str = (const unsigned char *)str;
  if (str != s21_NULL) {
    for (s21_size_t i = 0; i < n && ptr == s21_NULL; i++)
      if (ptr_str[i] == c) ptr = (void *)(ptr_str + i);
  }
  return ptr;
}

// Сравнивает первые n байтов str1 и str2
int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int num = 0;
  const unsigned char *ptr_str1 = (const unsigned char *)str1;
  const unsigned char *ptr_str2 = (const unsigned char *)str2;
  if (str1 == s21_NULL || str2 == s21_NULL) num = -2;
  for (s21_size_t i = 0; i < n && !num; ++i) {
    if (ptr_str1[i] != ptr_str2[i]) {
      if (ptr_str1[i] > ptr_str2[i])
        num = 1;
      else
        num = -1;
    }
  }
  return num;
}

// Копирует n символов из src в dest
void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *ptr_dest = (unsigned char *)dest;
  const unsigned char *ptr_src = (const unsigned char *)src;
  unsigned char *ptr = s21_NULL;
  s21_size_t i = 0;
  if (ptr_dest != s21_NULL && ptr_src != s21_NULL) {
    ptr = dest;
    while (i < n) {
      *ptr_dest++ = *ptr_src++;
      i++;
    }
  }
  return (void *)ptr;
}

// Копирует символ c (беззнаковый тип) в первые n символов строки, на которую
// указывает аргумент str.
void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *ptr_str = (unsigned char *)str;
  unsigned char ptr_c = (unsigned char)c;
  if (str != s21_NULL)
    for (s21_size_t i = 0; i < n; ++i) ptr_str[i] = ptr_c;
  return (void *)str;
}

// Добавляет строку, на которую указывает src, в конец строки, на которую
// указывает dest, длиной до n символов.
char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *ptr = s21_NULL;
  if (n && dest && src) {
    ptr = dest;
    while (*dest != '\0') dest++;
    s21_size_t i = 0;
    while (i < n && *src) {
      *dest++ = *src++;
      ++i;
    }
    *dest = '\0';
  }
  return ptr;
}

// Выполняет поиск первого вхождения символа c (беззнаковый тип) в строке, на
// которую указывает аргумент str.
char *s21_strchr(const char *str, int c) {
  char *ptr_str = (char *)str;
  char *ptr = s21_NULL;
  if (str != s21_NULL) {
    int i = 0;
    while (*(ptr_str + i) != '\0' && !ptr) {
      if (ptr_str[i] == c) ptr = ptr_str + i;
      i++;
    }
    if (c == '\0') ptr = ptr_str + i;
  }
  return ptr;
}

// Сравнивает не более первых n байтов str1 и str2.
int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int num = 0;
  const unsigned char *ptr_str1 = (const unsigned char *)str1;
  const unsigned char *ptr_str2 = (const unsigned char *)str2;
  if (ptr_str1 != s21_NULL && ptr_str2 != s21_NULL && n > 0) {
    for (s21_size_t i = 0; i < n && !num; ++i) {
      if (ptr_str1[i] != ptr_str2[i]) {
        if (ptr_str1[i] > ptr_str2[i])
          num = 1;
        else
          num = -1;
      }
    }
  }
  return num;
}

// Копирует до n символов из строки, на которую указывает src, в dest.
char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  unsigned char *ptr_dest = (unsigned char *)dest;
  if (ptr_dest != s21_NULL && src != s21_NULL) {
    for (s21_size_t i = 0; i < n; ++i) {
      if (i <= s21_strlen(src))
        ptr_dest[i] = src[i];
      else
        ptr_dest[i] = '\0';
    }
  }
  return dest;
}

// Вычисляет длину начального сегмента str1, который полностью состоит из
// символов, не входящих в str2.
s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t res = 0;
  if (str1 != s21_NULL && str2 != s21_NULL)
    while (!s21_strchr(str2, *str1)) {
      str1++;
      res++;
    }
  return res;
}

// Выполняет поиск во внутреннем массиве номера ошибки errnum и возвращает
// указатель на строку с сообщением об ошибке. Нужно объявить макросы,
// содержащие массивы сообщений об ошибке для операционных систем mac и linux.
// Описания ошибок есть в оригинальной библиотеке. Проверка текущей ОС
// осуществляется с помощью директив.
char *s21_strerror(int errnum) {
  const char *all_errors[] = S21_ERROR;
  static char error[60] = {0};
  int tmp_errnum = errnum;
  if (errnum >= 0 && errnum < ERROR_MAX) {
    s21_strncpy(error, all_errors[errnum], 60);
    error[s21_strlen(all_errors[errnum]) + 1] = '\0';
  } else {
    s21_strncpy(error, "Unknown error ", 16);
    char min = 0;
    char num[5] = {0};
    if (errnum < 0) {
      num[0] = '-';
      min = 1;
      tmp_errnum *= -1;
    }
    if (tmp_errnum < 10) {
      num[0 + min] = tmp_errnum + 48;
      num[1 + min] = '\0';
    }
    if (tmp_errnum > 9 && errnum < 100) {
      num[1 + min] = tmp_errnum % 10 + 48;
      tmp_errnum /= 10;
      num[0 + min] = tmp_errnum + 48;
      num[2 + min] = '\0';
      puts(num);
    }
    if (tmp_errnum > 99 && errnum < 1000) {
      num[2 + min] = tmp_errnum % 10 + 48;
      tmp_errnum /= 10;
      num[1 + min] = tmp_errnum % 10 + 48;
      tmp_errnum /= 10;
      num[0 + min] = tmp_errnum + 48;
      num[3 + min] = '\0';
    }
    s21_strncat(error, num, 4);
  }
  return error;
}

// Вычисляет длину строки str, не включая завершающий нулевой символ.
s21_size_t s21_strlen(const char *str) {
  const char *ptr_str = str;
  s21_size_t len = 0;
  if (str != s21_NULL)
    while (ptr_str[len] != '\0') len++;

  return len;
}

// Находит первый символ в строке str1, который соответствует любому символу,
// указанному в str2.
char *s21_strpbrk(const char *str1, const char *str2) {
  const char *ptr_str1 = (char *)str1;
  const char *res = s21_NULL;
  if (str1 != s21_NULL && str2 != s21_NULL)
    for (; *ptr_str1 != '\0' && !res; ptr_str1++)
      if (s21_strchr(str2, *ptr_str1)) res = ptr_str1;
  return res ? (char *)res : s21_NULL;
}

// Выполняет поиск последнего вхождения символа c (беззнаковый тип) в строке, на
// которую указывает аргумент str.
char *s21_strrchr(const char *str, int c) {
  unsigned char *ptr_str = (unsigned char *)str;
  unsigned char *ptr = s21_NULL;
  if (str != s21_NULL) {
    s21_size_t size = s21_strlen(str);
    for (s21_size_t i = size; i-- > 0 && !ptr;) {
      if (ptr_str[i] == c) ptr = ptr_str + i;
    }
  }
  return (char *)ptr;
}

// Находит первое вхождение всей строки needle (не включая завершающий нулевой
// символ), которая появляется в строке haystack.
char *s21_strstr(const char *haystack, const char *needle) {
  char *ptr = s21_NULL;
  if (haystack != s21_NULL && needle != s21_NULL) {
    s21_size_t len = s21_strlen(needle);
    if (len == 0) ptr = (char *)haystack;
    char flag = 1;
    while (*haystack != '\0' && flag) {
      if (*haystack == needle[0]) {
        if (s21_strncmp(haystack, needle, len) == 0) {
          ptr = (char *)haystack;
          flag = 0;
        }
      }
      haystack++;
    }
  }
  return ptr;
}

// Разбивает строку str на ряд токенов, разделенных delim.
char *s21_strtok(char *str, const char *delim) {
  static char *total = s21_NULL;  // запоминаем глобано указатель
  char *result = s21_NULL;
  char flag = 1;  // проверка на непустоту строки и все еще наличие токенов
  if (str != s21_NULL)
    total = str;  // первичное присваивание
  else if (total == s21_NULL)
    flag = 0;
  if (flag) {
    while (*total != '\0' && s21_strchr(delim, *total) != s21_NULL)
      total++;  // пропускаем токены начала строки
    if (*total == '\0')
      result = s21_NULL;  // если вся строка из токенов, то ничего не выовдим
    else {
      result =
          total;  // чтобы не потерять указатель на начало выводимосй строки
      while (*total != '\0' && s21_strchr(delim, *total) == s21_NULL)
        total++;  // пропускаемм НЕ токены
      if (*total != '\0') {
        *total = '\0';
        total++;
      } else
        total = s21_NULL;
    }
  }
  return result;
}

// Возвращает копию строки (str), преобразованной в верхний регистр. В случае
// какой-либо ошибки следует вернуть значение NULL
void *s21_to_upper(const char *str) {
  char *res = s21_NULL;
  if (str != s21_NULL) {
    s21_size_t len = s21_strlen(str);
    res = calloc(len, sizeof(char));
    if (res) {
      for (s21_size_t i = 0; i < len; ++i) {
        if (str[i] >= 97 && str[i] <= 122)
          res[i] = str[i] - 32;
        else
          res[i] = str[i];
      }
    }
  }
  return (void *)res;
}

// Возвращает копию строки (str), преобразованной в нижний регистр. В случае
// какой-либо ошибки следует вернуть значение NULL
void *s21_to_lower(const char *str) {
  char *res = s21_NULL;
  if (str != s21_NULL) {
    s21_size_t len = s21_strlen(str);
    res = calloc(len, sizeof(char));
    if (res) {
      for (s21_size_t i = 0; i < len; ++i)
        if (str[i] >= 65 && str[i] <= 90)
          res[i] = str[i] + 32;
        else
          res[i] = str[i];
    }
  }
  return (void *)res;
}

// Возвращает новую строку, в которой указанная строка (str) вставлена в
// указанную позицию (start_index) в данной строке (src). В случае какой-либо
// ошибки следует вернуть значение NULL.
void *s21_insert(const char *src, const char *str, size_t start_index) {
  char *res = s21_NULL;
  if (src != s21_NULL && str != s21_NULL && start_index < s21_strlen(src)) {
    char *ptr = calloc((s21_strlen(src) + s21_strlen(str) + 1), sizeof(char));
    if (ptr) {
      s21_strncpy(ptr, src, start_index);
      char *ptr_src = (char *)src + start_index;
      s21_strncat(ptr, str, s21_strlen(str));
      s21_strncat(ptr, ptr_src, s21_strlen(ptr_src) + 1);
      res = ptr;
    }
  }
  return (void *)res;
}

// для trim
char check(char el, const char *trim_chars) {
  char flag = 0;
  for (size_t i = 0; i < s21_strlen(trim_chars); i++) {
    if (el == trim_chars[i]) {
      flag = 1;
    }
  }
  return flag;
}

// Возвращает новую строку, в которой удаляются все начальные и конечные
// вхождения набора заданных символов (trim_chars) из данной строки (src). В
// случае какой-либо ошибки следует вернуть значение NULL.
void *s21_trim(const char *src, const char *trim_chars) {
  char *result = s21_NULL;
  if (src != s21_NULL && trim_chars != s21_NULL) {
    int len = s21_strlen(src);
    int start = 0;
    while (start < len && check(src[start], trim_chars)) start++;
    int end = len - 1;
    while (end >= start && check(src[end], trim_chars)) end--;
    int result_length = end - start + 1;
    result = malloc(result_length + 1);
    if (result) {
      s21_strncpy(result, src + start, result_length);
      result[result_length] = '\0';
    }
  }
  return result;
}
