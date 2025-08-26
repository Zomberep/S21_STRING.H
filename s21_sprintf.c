#include "s21_string.h"

void cleaning_flags(flags *F, mini_int type) {
  if (type == s || type == ls || type == c || type == lc || type == p ||
      type == n || type == non) {
    F->plus = 0;
    F->pound_sign = 0;
    F->space = 0;
    F->zero = 0;
    if (type == n || type == non) F->minus = 0;
  } else if (type == o || type == u || type == x || type == X || type == hX ||
             type == lX || type == ho || type == lo || type == hu ||
             type == hx || type == lu || type == lx) {
    F->plus = 0;
    F->space = 0;
    if (type == u || type == lu || type == hu) F->pound_sign = 0;
  }
}

int getting_next_symbol(int b, int number, mini_int type) {
  if (b == 10 || b == 8 || (b == 16 && number < 10))
    number += 48;
  else if (type % 29 == 0)
    number += 87;
  else
    number += 55;
  return number;
}

void end_from_double_e(total_variable *var, unsigned *len, unsigned *max_len,
                       char *value, mini_int *error_flag, long long exp_part) {
  if (var->type == e || var->type == Le)
    value[(*len)++] = 'e';
  else
    value[(*len)++] = 'E';
  if (*len + 3 >= *max_len) increasing_size(value, max_len, error_flag);
  if (!(*error_flag)) {
    value[(*len)++] = (exp_part >= 0) ? '+' : '-';
    exp_part = (exp_part > 0) ? exp_part : -exp_part;
    value[(*len)++] = (exp_part / 10) + 48;
    value[(*len)++] = (exp_part % 10) + 48;
  }
}

void changing_sign(total_variable *var) {
  if (is_long_double(var->type))
    var->value.llf = var->value.llf > 0 ? var->value.llf : -var->value.llf;
  else
    var->value.f = var->value.f > 0 ? var->value.f : -var->value.f;
}

void from_double_e(total_variable *var, flags *F, char *value, unsigned *len,
                   unsigned *max_len, mini_int *error_flag) {
  unsigned long long val = 0, exp_part = 0, exp = 0;
  changing_sign(var);
  unsigned long long integer_part =
      (unsigned long long)(is_long_double(var->type) ? var->value.llf
                                                     : var->value.f);
  int len_of_value = (integer_part != 0) ? (int)log10(integer_part) + 1 : 0;
  if (len_of_value != 0) {
    val = (unsigned long long)(pow(10, var->presicion + 2 - len_of_value) *
                               (is_long_double(var->type) ? var->value.llf
                                                          : var->value.f));
    exp_part = len_of_value - 1;
  } else if (is_long_double(var->type) ? var->value.llf : var->value.f != 0.0) {
    double_var d_var = {0};
    if (is_long_double(var->type))
      d_var.llf = var->value.llf;
    else
      d_var.f = var->value.f;
    while ((unsigned long long)(is_long_double(var->type) ? d_var.llf
                                                          : d_var.f) == 0) {
      exp_part -= 1;
      if (is_long_double(var->type))
        d_var.llf *= 10;
      else
        d_var.f *= 10;
    }
    val =
        (unsigned long long)(pow(10, var->presicion + 1) *
                             (is_long_double(var->type) ? d_var.llf : d_var.f));
  }
  val = (val % 10) > 4 ? val + 10 : val;
  val /= 10;

  len_of_value =
      ((is_long_double(var->type) ? var->value.llf : var->value.f) != 0.0)
          ? (int)log10(val)
          : var->presicion;
  exp = (unsigned long long)(pow(10, len_of_value));
  value[(*len)++] = (val / exp) + 48;
  val %= exp;
  exp /= 10;
  if (F->pound_sign || len_of_value) value[(*len)++] = '.';
  while (len_of_value > 0) {
    value[(*len)++] = (val / exp) + 48;
    val %= exp;
    exp /= 10;
    --len_of_value;
    checking_len(len, max_len, value, error_flag);
  }
  end_from_double_e(var, len, max_len, value, error_flag, exp_part);
}

void from_double_f(total_variable *var, flags *F, char *value, unsigned *len,
                   unsigned *max_len, mini_int *error_flag) {
  changing_sign(var);
  unsigned long long integer_part =
      (unsigned long long)(is_long_double(var->type) ? var->value.llf
                                                     : var->value.f);
  if (integer_part != 0) {
    int len_of_value = log10(integer_part) + 1;
    unsigned long long exp = (unsigned long long)pow(10, len_of_value - 1);
    for (int i = len_of_value; !(*error_flag) && i > 0; --i) {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) {
        value[(*len)++] = (integer_part / exp) + 48;
        integer_part %= exp;
        exp /= 10;
      }
    }
  } else {
    value[(*len)++] = '0';
  }
  if (!(*error_flag) && (F->pound_sign || var->presicion != 0))
    value[(*len)++] = '.';
  if (var->presicion != 0) {
    integer_part =
        (unsigned long long)(is_long_double(var->type) ? var->value.llf
                                                       : var->value.f);
    unsigned long long frac_part =
        (unsigned long long)(pow(10, var->presicion + 1) *
                             ((is_long_double(var->type) ? var->value.llf
                                                         : var->value.f) -
                              integer_part));
    if (frac_part % 10 > 4) frac_part += 10;
    frac_part /= 10;
    int len_of_value = (frac_part != 0) ? log10(frac_part) + 1 : 0;
    int presicion = var->presicion;
    while (!(*error_flag) && (len_of_value < presicion)) {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) value[(*len)++] = '0';
      --presicion;
    }
    unsigned long long exp = (unsigned long long)pow(10, len_of_value - 1);
    for (int i = len_of_value; !(*error_flag) && i > 0; --i) {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) {
        value[(*len)++] = (frac_part / exp) + 48;
        frac_part %= exp;
        exp /= 10;
      }
    }
  }
}

void from_double_to_string(total_variable *var, flags *F, char *value,
                           unsigned *len, unsigned *max_len,
                           mini_int *error_flag) {
  if ((is_long_double(var->type) ? var->value.llf : var->value.f) < 0) {
    value[(*len)++] = '-';
  } else {
    if (F->plus) value[(*len)++] = '+';
    if (F->space) value[(*len)++] = ' ';
  }
  if (var->type == f || var->type == Lf)
    from_double_f(var, F, value, len, max_len, error_flag);
  if (var->type == e || var->type == Le || var->type == E || var->type == LE)
    from_double_e(var, F, value, len, max_len, error_flag);
  checking_len(len, max_len, value, error_flag);
  value[(*len)++] = '\0';
}

void from_ptr_to_string(total_variable *var, char *value, unsigned *len) {
  if (var->value.p == NULL) {
    value[(*len)++] = '(';
    value[(*len)++] = 'n';
    value[(*len)++] = 'i';
    value[(*len)++] = 'l';
    value[(*len)++] = ')';
  } else {
    value[(*len)++] = '0';
    value[(*len)++] = 'x';
    int len_of_value = (int)(log((double)((long)var->value.p)) / log(16)) + 1;
    long long exp = (long long)pow(16, len_of_value - 1);
    for (int i = len_of_value; i > 0; --i) {
      int figure = ((long)var->value.p) / exp;
      if (figure > 9)
        figure += 87;
      else
        figure += 48;
      value[(*len)++] = figure;
      var->value.ld %= exp;
      exp /= 16;
    }
  }
  value[(*len)++] = '\0';
}

void from_str_to_string(total_variable *var, char *value, unsigned *len,
                        unsigned *max_len, mini_int *error_flag) {
  unsigned j = 0;
  int sym = 0;
  if (var->presicion == 1) {
    while (!(*error_flag) &&
           (sym = (var->type == s ? var->value.s[j++] : var->value.ws[j++])) !=
               '\0') {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) value[(*len)++] = sym;
    }
  } else {
    while (!(*error_flag) &&
           (sym = (var->type == s ? var->value.s[j++] : var->value.ws[j++])) !=
               '\0' &&
           *len < (unsigned)var->presicion) {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) value[(*len)++] = sym;
    }
  }
  if (!(*error_flag)) value[(*len)++] = '\0';
}

void from_unsigned_to_string_case_0(total_variable *var, unsigned *len,
                                    unsigned *max_len, char *value,
                                    mini_int *error_flag) {
  if (!(var->type % 37) && !(var->presicion)) ++var->presicion;
  while (!(*error_flag) && var->presicion > 0) {
    checking_len(len, max_len, value, error_flag);
    if (!(*error_flag)) {
      value[(*len)++] = '0';
      --var->presicion;
    }
  }
}

void checking_len(unsigned *len, unsigned *max_len, char *value,
                  mini_int *error_flag) {
  if (*len >= (*max_len - 1)) increasing_size(value, max_len, error_flag);
}

void from_unsigned_to_string(flags *F, total_variable *var, char *value,
                             unsigned *len, unsigned *max_len,
                             mini_int *error_flag) {
  if ((is_unsigned_long(var->type) ? var->value.lu : var->value.u) != 0) {
    if (F->pound_sign) {
      if (var->type % 37 == 0) {
        value[(*len)++] = '0';
        --var->presicion;
      } else if (var->type % 29 == 0) {
        value[(*len)++] = '0';
        value[(*len)++] = 'x';
      } else if (var->type % 31 == 0) {
        value[(*len)++] = '0';
        value[(*len)++] = 'X';
      }
    }
    int len_of_value = 0,
        b = (var->type % 23 == 0 ? 10 : (var->type % 37 == 0 ? 8 : 16));
    len_of_value =
        (int)(log(is_unsigned_long(var->type) ? var->value.lu : var->value.u) /
              log(b)) +
        1;
    long long exp = (long long)pow(b, len_of_value - 1);
    while (!(*error_flag) && var->presicion > len_of_value) {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) {
        value[(*len)++] = '0';
        --var->presicion;
      }
    }
    for (int i = len_of_value; !(*error_flag) && i > 0; --i) {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) {
        if (is_unsigned_long(var->type)) {
          value[(*len)++] =
              getting_next_symbol(b, (var->value.lu) / exp, var->type);
          var->value.lu %= exp;
        } else {
          value[(*len)++] =
              getting_next_symbol(b, (var->value.u) / exp, var->type);
          var->value.u %= exp;
        }
        exp /= b;
      }
    }
  } else {
    from_unsigned_to_string_case_0(var, len, max_len, value, error_flag);
  }
  if (!(*error_flag)) value[(*len)++] = '\0';
}

void from_int_to_string(flags *F, total_variable *var, char *value,
                        unsigned *len, unsigned *max_len,
                        mini_int *error_flag) {
  if ((is_long_int(var->type) ? var->value.ld : var->value.d) < 0) {
    value[(*len)++] = '-';
  } else {
    if (F->plus) value[(*len)++] = '+';
    if (F->space) value[(*len)++] = ' ';
  }
  if (is_long_int(var->type))
    var->value.ld = var->value.ld > 0 ? var->value.ld : -var->value.ld;
  else
    var->value.d = var->value.d > 0 ? var->value.d : -var->value.d;
  if ((is_long_int(var->type) ? var->value.ld : var->value.d) != 0) {
    int len_of_value =
        (int)log10(
            (double)(is_long_int(var->type) ? var->value.ld : var->value.d)) +
        1;
    while (!(*error_flag) && len_of_value < var->presicion) {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) {
        value[(*len)++] = '0';
        --var->presicion;
      }
    }
    long long exp = (long long)pow(10, len_of_value - 1);
    for (int i = len_of_value; !(*error_flag) && i > 0; --i) {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) {
        if (is_long_int(var->type)) {
          value[(*len)++] = (var->value.ld / exp) + 48;
          var->value.ld %= exp;
        } else {
          value[(*len)++] = (var->value.d / exp) + 48;
          var->value.d %= exp;
        }
        exp /= 10;
      }
    }
  } else {
    while (!(*error_flag) && var->presicion > 0) {
      checking_len(len, max_len, value, error_flag);
      if (!(*error_flag)) {
        value[(*len)++] = '0';
        --var->presicion;
      }
    }
  }
  if (!(*error_flag)) value[(*len)++] = '\0';
}

unsigned filling_str(char *str, total_variable *var, flags *F,
                     mini_int *error_flag, unsigned curr_len) {
  cleaning_flags(F, var->type);
  if (F->plus) F->space = 0;
  if (F->zero) F->minus = 0;
  unsigned len = 0, max_len = initial_size;

  char *value = (char *)malloc(sizeof(char) * initial_size);
  *error_flag = (value == NULL);
  if (!(*error_flag) &&
      (var->type == d || var->type == i || var->type == hi || var->type == hd ||
       var->type == li || var->type == ld)) {
    from_int_to_string(F, var, value, &len, &max_len, error_flag);
  } else if (!(*error_flag) && var->type == non) {
    value[len++] = '%';
    value[len++] = '\0';
  } else if (!(*error_flag) && (!(var->type % 23) || !(var->type % 29) ||
                                !(var->type % 31) || !(var->type % 37))) {
    from_unsigned_to_string(F, var, value, &len, &max_len, error_flag);
  } else if (!(*error_flag) && var->type == n) {
    *var->value.n = curr_len;
  } else if (!(*error_flag) && (var->type == s || var->type == ls)) {
    from_str_to_string(var, value, &len, &max_len, error_flag);
  } else if (!(*error_flag) && (var->type == c || var->type == lc)) {
    value[len++] = var->value.wc;
    value[len++] = '\0';
  } else if (!(*error_flag) && (var->type == p)) {
    from_ptr_to_string(var, value, &len);
  } else {
    from_double_to_string(var, F, value, &len, &max_len, error_flag);
  }
  if (!(var->type == n) && (var->width > 0) &&
      ((unsigned)var->width > (len - 1))) {
    unsigned len_add = var->width - len + 1, j = 0;
    if (F->zero) {
      while (j < len_add) {
        str[j++] = '0';
      }
      s21_memcpy(&str[len_add], value, len - 1);
    } else if (F->minus) {
      s21_memcpy(str, value, len - 1);
      while (j < len_add) {
        str[len - 1 + (j++)] = ' ';
      }
    } else {
      while (j < len_add) {
        str[j++] = ' ';
      }
      s21_memcpy(&str[len_add], value, len - 1);
    }
  } else if (var->type != n) {
    s21_memcpy(str, value, len - 1);
  }
  if (value != NULL) free(value);
  unsigned res = (var->width > 0 && (unsigned)var->width > (len - 1))
                     ? (unsigned)var->width
                     : (len - 1) * (var->type != n);
  return res;
}

int s21_sprintf(char *str, const char *format, ...) {
  mini_int error_flag = 0;
  unsigned curr_len = 0, j = 0;
  int sym = 0;

  va_list curr_arg;
  va_start(curr_arg, format);

  while (!error_flag && (sym = format[j++])) {
    if (sym == '%') {
      argument_type arg_type = {.type = 255};
      flags F = {0};
      char *pattern = getting_pattern(&j, format, &error_flag);
      error_flag = 4 * (pattern[0] == '\0');
      if (!error_flag) obtaining_types(pattern, &arg_type, &error_flag);
      total_variable var = {
          .width = -1,
          .presicion = (arg_type.type > 12 && arg_type.type < 23 ? 6 : 1)};
      if (!error_flag) {
        var.type = arg_type.type;
        getting_add_values(&var, &curr_arg, arg_type.other_values);
        switch_for_reading(&var, &curr_arg);
        processing_pattern(pattern, &error_flag, &var, &F);
        if (!error_flag)
          curr_len +=
              filling_str(str + curr_len, &var, &F, &error_flag, curr_len);
      }
      if (pattern != NULL) free(pattern);
    } else {
      str[curr_len++] = sym;
    }
  }
  str[curr_len++] = '\0';

  va_end(curr_arg);
  return error_flag ? -1 : (int)curr_len - 1;
}

int s21_atoi(char *str) {
  int result = 0;
  char flag = 1;
  if (str != NULL) {
    for (int i = 0; str[i]; ++i) {
      if (str[0] == '-') {
        if ((result * 10 - (str[i] - 48)) <= result && is_digit(str[i]) &&
            i != 0)
          result = result * 10 - (str[i] - 48);
        else if (((result * 10 - (str[i] - 48)) > result ||
                  !(is_digit(str[i]))) &&
                 i != 0)
          flag = 0;
      } else {
        if ((result * 10 + (str[i] - 48)) >= result && is_digit(str[i]))
          result = result * 10 + (str[i] - 48);
        else
          flag = 0;
      }
    }
  } else
    flag = 0;
  if (!flag) result = -1;
  return result;
}

int getting_number(char *pattern, unsigned *j, mini_int *error_flag) {
  unsigned curr_len = 0;
  char number[max_len_for_int] = {0};
  int sym, result = -1;

  while (is_digit(sym = pattern[(*j)++])) {
    if (curr_len < (max_len_for_int - 1))
      number[curr_len++] = sym;
    else
      curr_len = UINT_MAX;
  }
  number[curr_len++] = '\0';
  --(*j);
  if (curr_len != UINT_MAX) {
    result = s21_atoi(number);
    if (result < 0) *error_flag = 7;  // семерка, если некорректное число
  } else {
    *error_flag = 7;  // семёрка, если число слишком большое для int
  }
  return result;
}

void processing_flag_sym(int sym, flags *F, mini_int *error_flag) {
  switch (sym) {
    case '-':
      if (!F->minus)
        F->minus = 1;
      else
        *error_flag = 6;  // шестерка если дважды встретил флаг
      break;
    case '+':
      if (!F->plus)
        F->plus = 1;
      else
        *error_flag = 6;
      break;
    case ' ':
      if (!F->space)
        F->space = 1;
      else
        *error_flag = 6;
      break;
    case '#':
      if (!F->pound_sign)
        F->pound_sign = 1;
      else
        *error_flag = 6;
      break;
    case '0':
      if (!F->zero)
        F->zero = 1;
      else
        *error_flag = 6;
      break;
  }
}

void getting_add_type(total_variable *var, char *pattern) {
  mini_int count =
      (var->width != -1) +
      ((var->presicion != 1 && (var->type < 13 || var->type > 22)) ||
       (var->presicion != 6 && (var->type > 12 && var->type < 23)));
  if (count == 2) {
    var->add_type = 0;
  } else if (count == 1) {
    if (s21_strstr(pattern, ".*") == NULL) {
      var->add_type = 1;
    } else {
      var->presicion = var->width;
      var->width = -1;
      var->add_type = 2;
    }
  } else {
    var->add_type = -1;
  }
}

void processing_pattern(char *pattern, mini_int *error_flag,
                        total_variable *var, flags *F) {
  unsigned j = 0;
  int sym;
  getting_add_type(var, pattern);
  while (!(*error_flag) && !is_letter(sym = pattern[j++])) {
    if (is_flag_sym(sym))
      processing_flag_sym(sym, F, error_flag);
    else if (is_digit(sym)) {
      j -= 1;
      int width = getting_number(pattern, &j, error_flag);
      if (var->add_type == 1 || var->add_type == 0) {
        *error_flag = 8;  // восьмерка, если в строке лишнее число
      } else {
        var->width = width;
        var->add_type = (var->add_type != 2);
      }
    } else if (sym == '.') {
      if (!is_digit(pattern[j]) && pattern[j] != '*')
        *error_flag = 9;  // девятка, если после точки идёт некорректный символ
      else if (pattern[j] != '*') {
        int precision = getting_number(pattern, &j, error_flag);
        if (var->add_type == 2 || var->add_type == 0) {
          *error_flag = 8;  // восьмерка, если в строке лишнее число
        } else {
          var->presicion = precision;
          var->add_type = 2 * (var->add_type != 1);
        }
      }
    }
  }
}

void switch_for_reading(total_variable *var, va_list *curr_arg) {
  switch (var->type) {
    case hi:
    case hd:
    case d:
    case i:
      var->value.d = va_arg(*curr_arg, int);
      break;
    case e:
    case E:
    case f:
      var->value.f = va_arg(*curr_arg, double);
      break;
    case ho:
    case hu:
    case hx:
    case hX:
    case u:
    case o:
    case x:
    case X:
      var->value.u = va_arg(*curr_arg, unsigned);
      break;
    case p:
      var->value.p = va_arg(*curr_arg, void *);
      break;
    case s:
      var->value.s = va_arg(*curr_arg, char *);
      break;
    case n:
      var->value.n = va_arg(*curr_arg, int *);
      break;
    case li:
    case ld:
      var->value.ld = va_arg(*curr_arg, long);
      break;
    case lo:
    case lu:
    case lx:
    case lX:
      var->value.lu = va_arg(*curr_arg, unsigned long);
      break;
    case lc:
    case c:
      var->value.wc = va_arg(*curr_arg, wchar_t);
      break;
    case ls:
      var->value.ws = va_arg(*curr_arg, wchar_t *);
      break;
    case Le:
    case LE:
    case Lf:
      var->value.llf = va_arg(*curr_arg, long double);
    case non:
  }
}

void getting_add_values(total_variable *var, va_list *curr_arg,
                        mini_int other_values) {
  if (other_values == 2) {
    var->width = va_arg(*curr_arg, int);
    var->presicion = va_arg(*curr_arg, int);
  } else if (other_values == 1) {
    var->width = va_arg(*curr_arg, int);
  }
}

mini_int switch_for_type_1(char *type, mini_int *result) {
  if (type[0] == '\0')
    *result = non;
  else if (!s21_strncmp(type, "d", 2))
    *result = d;
  else if (!s21_strncmp(type, "E", 2))
    *result = E;
  else if (!s21_strncmp(type, "X", 2))
    *result = X;
  else if (!s21_strncmp(type, "c", 2))
    *result = c;
  else if (!s21_strncmp(type, "i", 2))
    *result = i;
  else if (!s21_strncmp(type, "f", 2))
    *result = f;
  else if (!s21_strncmp(type, "e", 2))
    *result = e;
  else if (!s21_strncmp(type, "n", 2))
    *result = n;
  else if (!s21_strncmp(type, "o", 2))
    *result = o;
  else if (!s21_strncmp(type, "p", 2))
    *result = p;
  else if (!s21_strncmp(type, "s", 2))
    *result = s;
  else if (!s21_strncmp(type, "u", 2))
    *result = u;
  else if (!s21_strncmp(type, "x", 2))
    *result = x;
  else if (!s21_strncmp(type, "LE", 3))
    *result = LE;
  else if (!s21_strncmp(type, "Le", 3))
    *result = Le;
  else if (!s21_strncmp(type, "Lf", 3))
    *result = Lf;
  return *result != 255;
}

mini_int switch_for_type_2(char *type, mini_int *result) {
  if (!s21_strncmp(type, "hX", 3))
    *result = hX;
  else if (!s21_strncmp(type, "lX", 3))
    *result = lX;
  else if (!s21_strncmp(type, "hd", 3))
    *result = hd;
  else if (!s21_strncmp(type, "lc", 3))
    *result = lc;
  else if (!s21_strncmp(type, "ld", 3))
    *result = ld;
  else if (!s21_strncmp(type, "hi", 3))
    *result = hi;
  else if (!s21_strncmp(type, "li", 3))
    *result = li;
  else if (!s21_strncmp(type, "ho", 3))
    *result = ho;
  else if (!s21_strncmp(type, "lo", 3))
    *result = lo;
  else if (!s21_strncmp(type, "hu", 3))
    *result = hu;
  else if (!s21_strncmp(type, "ls", 3))
    *result = ls;
  else if (!s21_strncmp(type, "hx", 3))
    *result = hx;
  else if (!s21_strncmp(type, "lu", 3))
    *result = lu;
  else if (!s21_strncmp(type, "lx", 3))
    *result = lx;
  return *result != 255;
}

void obtaining_types(char *pattern, argument_type *arg_type,
                     mini_int *error_flag) {
  char type[max_len_for_type] = {0};
  unsigned count = 0, j = 0, type_len = 0;
  mini_int break_1 = 0;
  int c = 0;
  while (!break_1 && (c = pattern[j++])) {
    count += (c == '*');
    if (is_letter(c)) {
      ++break_1;
      --j;
    }
  }
  while (break_1 && (c = pattern[j++])) {
    if (type_len < max_len_for_type - 1) type[type_len++] = c;
  }
  type[type_len++] = '\0';
  if (count > 2)
    *error_flag = 2;  // двойка если количество "*" больше чем нада
  else {
    arg_type->other_values = (count > 1) + (count > 0);
    if (!switch_for_type_1(type, &arg_type->type)) {
      *error_flag = !switch_for_type_2(type, &arg_type->type) *
                    3;  // тройка, если не подошло ни одно число
    }
  }
}

char *getting_pattern(unsigned *j, const char *format, mini_int *error_flag) {
  unsigned curr_len = 0, max_len = initial_size;
  mini_int stop_loop = 0, letter_flag = 0;

  char *pattern = (char *)malloc(sizeof(char) * initial_size);
  *error_flag = (pattern == NULL);

  int sym;
  while (!(*error_flag) && !stop_loop) {
    sym = format[(*j)++];
    if (curr_len == 0 && sym == '%') {
      pattern[curr_len++] = sym;
      stop_loop = 1;
    }
    if (!stop_loop &&
        (!available_symbol_for_pattern(sym) || (sym == ' ' && letter_flag))) {
      stop_loop = 1;
      --(*j);
    };
    if (!stop_loop && is_letter(sym)) letter_flag = 1;

    if (!stop_loop && !(*error_flag)) pattern[curr_len++] = sym;
    if (!stop_loop && (curr_len == max_len))
      increasing_size(pattern, &max_len, error_flag);
  }
  pattern[curr_len++] = '\0';
  return pattern;
}

void increasing_size(
    char *str, unsigned *max_len,
    mini_int *error_flag) {  // it allocate more memory for string
  char *new_str = NULL;
  *max_len += additive;
  new_str = realloc(str, sizeof(char) * (*max_len));
  if (new_str == NULL)
    *error_flag = 1;
  else
    str = new_str;
}

mini_int is_unsigned_long(mini_int type) {
  return (type == lo || type == lu || type == lx || type == lX);
}

mini_int is_long_double(mini_int type) {
  return (type == Le || type == Lf || type == LE || type == LG || type == Lg);
}

mini_int is_long_int(mini_int type) { return (type == li || type == ld); }

mini_int is_digit(int sym) { return (sym >= '0') && (sym <= '9'); }

mini_int is_letter(int sym) {
  return (sym >= 'A' && sym <= 'Z') || (sym >= 'a' && sym <= 'z');
}

mini_int is_flag_sym(int sym) {
  return (sym == '-') || (sym == '+') || (sym == ' ') || (sym == '#') ||
         (sym == '0');
}

mini_int available_symbol_for_pattern(
    int sym) {  // it checks symbol in the string
  return (sym == '-' || sym == '+' || sym == '#' || sym == '*' || sym == '.' ||
          (sym >= '0' && sym <= '9') || sym == ' ' ||
          (sym >= 'c' && sym <= 'i') || (sym == 'E') || (sym == 'G') ||
          (sym == 'X') || (sym == 'L') || (sym >= 'n' && sym <= 'p') ||
          (sym == 's') || (sym == 'u') || (sym == 'x') || (sym == 'l'));
}
