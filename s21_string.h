#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <stdarg.h>
#include <wchar.h>

typedef unsigned char mini_int;
#define s21_NULL ((void *)0)
typedef unsigned long s21_size_t;

enum {
    initial_size = 101,
    additive = 50,
    max_len_for_type = 3,
    max_len_for_int = 11,
};

enum spec {non, c, d, i, n, p, s, hd, lc, ld, hi, li, ls, e, f, g, LE, LG, Le, Lf, Lg, E, G,
        lu = 23, u = 46, hu = 69, hx = 29, lx = 58, x = 87, hX = 31, lX = 62, X = 93, o = 37, ho = 74, lo = 111}; // 35 типов

typedef union {
    unsigned long lu;
    long double llf;
    wchar_t *ws;
    wchar_t wc;
    unsigned u;
    double f;
    long ld;
    char *s;
    void *p;
    int *n;
    int d;
} var_value;

typedef union {
    double f;
    long double llf;
} double_var;

typedef struct {
    var_value value;
    mini_int type;
    int width;
    int presicion;
    mini_int add_type;
} total_variable;

typedef struct {
    mini_int minus;
    mini_int plus;
    mini_int space;
    mini_int pound_sign;
    mini_int zero;
} flags;

typedef struct {
    mini_int type;
    mini_int other_values;
} argument_type;

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);

int s21_sprintf(char *str, const char *format, ...);
void increasing_size(char *str, unsigned *max_len, mini_int *error_flag);
void processing_pattern(char *pattern, mini_int *error_flag, total_variable *var, flags *F);
char *getting_pattern(unsigned *j, const char *format, mini_int *error_flag);
void obtaining_types(char *pattern, argument_type* arg_type, mini_int *error_flag);
void getting_add_values(total_variable* var, va_list* curr_arg, mini_int other_values);
unsigned filling_str(char *str, total_variable *var, flags *F, mini_int *error_flag, unsigned curr_len);
int getting_number(char *pattern, unsigned *j, mini_int *error);
void getting_add_type(total_variable *var, char *pattern);
void switch_for_reading(total_variable* var, va_list* curr_arg);
void cleaning_flags(flags *F, mini_int type);
void checking_len(unsigned *len, unsigned *max_len, char *value, mini_int *error_flag);
void from_ptr_to_string(total_variable *var, char *value, unsigned *len);
void from_double_f(total_variable *var, flags *F, char *value, unsigned *len, unsigned *max_len, mini_int *error_flag);
void from_double_e(total_variable *var, flags *F, char *value, unsigned *len, unsigned *max_len, mini_int *error_flag);
void end_from_double_e(total_variable *var, unsigned *len, unsigned *max_len, char *value, mini_int *error_flag, long long exp_part);
void from_double_to_string(total_variable *var, flags *F, char *value, unsigned *len, unsigned *max_len, mini_int *error_flag);
void from_str_to_string(total_variable *var, char *value, unsigned *len, unsigned *max_len, mini_int *error_flag);
void from_int_to_string(flags *F, total_variable *var, char *value, unsigned *len, unsigned *max_len, mini_int *error_flag);
void from_unsigned_to_string(flags *F, total_variable *var, char *value, unsigned *len, unsigned *max_len, mini_int *error_flag);
void from_unsigned_to_string_case_0(total_variable *var, unsigned *len, unsigned *max_len, char *value, mini_int *error_flag);
int getting_next_symbol(int b, int number, mini_int type);
void changing_sign(total_variable *var);
mini_int switch_for_type_1(char *type, mini_int* result);
mini_int switch_for_type_2(char *type, mini_int* result);
mini_int available_symbol_for_pattern(int sym);
mini_int is_letter(int sym);
mini_int is_digit(int sym);
mini_int is_flag_sym(int sym);
mini_int check_sym(char sym);
mini_int is_long_int(mini_int type);
mini_int is_long_double(mini_int type);
mini_int is_unsigned_long(mini_int type);

#ifdef __linux__
#define ERROR_MAX 134
#define S21_ERROR {\
	"Success", "Operation not permitted", "No such file or directory", "No such process", \
	"Interrupted system call", "Input/output error", "No such device or address",\
	"Argument list too long", "Exec format error", "Bad file descriptor", "No child processes", \
    "Resource temporarily unavailable", "Cannot allocate memory", "Permission denied", "Bad address",\
	"Block device required", "Device or resource busy", "File exists", "Invalid cross-device link", \
    "No such device", "Not a directory", "Is a directory", "Invalid argument", "Too many open files in system",\
    "Too many open files", "Inappropriate ioctl for device", "Text file busy", "File too large", \
    "No space left on device", "Illegal seek", "Read-only file system", "Too many links", "Broken pipe", \
    "Numerical argument out of domain", "Numerical result out of range", "Resource deadlock avoided", \
    "File name too long", "No locks available", "Function not implemented", "Directory not empty", \
    "Too many levels of symbolic links", "Unknown error 41", "No message of desired type", "Identifier removed", \
    "Channel number out of range", "Level 2 not synchronized", "Level 3 halted", "Level 3 reset", \
    "Link number out of range", "Protocol driver not attached", "No CSI structure available", "Level 2 halted",\
    "Invalid exchange", "Invalid request descriptor", "Exchange full", "No anode", "Invalid request code",\
	"Invalid slot", "Unknown error 58", "Bad font file format", "Device not a stream", "No data available", \
    "Timer expired", "Out of streams resources", "Machine is not on the network", "Package not installed", \
    "Object is remote", "Link has been severed", "Advertise error", "Srmount error", "Communication error on send", \
    "Protocol error", "Multihop attempted", "RFS specific error", "Bad message", "Value too large for defined data type", \
    "Name not unique on network", "File descriptor in bad state", "Remote address changed", \
    "Can not access a needed shared library", "Accessing a corrupted shared library", ".lib section in a.out corrupted", \
    "Attempting to link in too many shared libraries", "Cannot exec a shared library directly", \
    "Invalid or incomplete multibyte or wide character", "Interrupted system call should be restarted", \
    "Streams pipe error", "Too many users", "Socket operation on non-socket", "Destination address required",\
    "Message too long", "Protocol wrong type for socket", "Protocol not available", "Protocol not supported", \
    "Socket type not supported", "Operation not supported", "Protocol family not supported", \
    "Address family not supported by protocol", "Address already in use", "Cannot assign requested address", \
    "Network is down", "Network is unreachable", "Network dropped connection on reset", \
    "Software caused connection abort", "Connection reset by peer", "No buffer space available",\
    "Transport endpoint is already connected", "Transport endpoint is not connected", \
    "Cannot send after transport endpoint shutdown", "Too many references: cannot splice", "Connection timed out", \
    "Connection refused", "Host is down", "No route to host", "Operation already in progress", "Operation now in progress",\
    "Stale file handle", "Structure needs cleaning",  "Not a XENIX named type file",  "No XENIX semaphores available", \
    "Is a named type file", "Remote I/O error", "Disk quota exceeded", "No medium found","Wrong medium type", \
    "Operation canceled", "Required key not available", "Key has expired", "Key has been revoked", \
    "Key was rejected by service", "Owner died", "State not recoverable", "Operation not possible due to RF-kill",\
    "Memory page has hardware error"}
#else
#define ERROR_MAX 108
#define S21_ERROR {\
    "Undefined error: 0", "Operation not permitted", "No such file or directory", "No such process",\
    "Interrupted system call", "Input/output error", "Device not configured", "Argument list too long",\
    "Exec format error", "Bad file descriptor", "No child processes", "Resource deadlock avoided", \
	"Cannot #endifallocate memory","Permission denied", "Bad address", "Block device required", "Resource busy",\
	"File exists", "Cross-device link", "Operation not supported by device", "Not a directory",\
    "Is a directory", "Invalid argument", "Too many open files in system", "Too many open files",\
	"Inappropriate ioctl for device", "Text file busy", "File too large", "No space left on device",\
    "Illegal seek", "Read-only file system", "Too many links", "Broken pipe", \
	"Numerical argument out of domain", "Result too large", "Resource temporarily unavailable", \
	"Operation now in progress", "Operation already in progress", "Socket operation on non-socket",\
    "Destination address required", "Message too long", "Protocol wrong type for socket", \
	"Protocol not available", "Protocol not supported", "Socket type not supported",\
    "Operation not supported", "Protocol family not supported", "Address family not supported by protocol family",\
    "Address already in use", "Can't assign requested address", "Network is down", "Network is unreachable",\
    "Network dropped connection on reset", "Software caused connection abort", "Connection reset by peer",\
    "No buffer space available", "Socket is already connected", "Socket is not connected", \
	"Can't send after socket shutdown", "Too many references: can't splice", "Operation timed out",\
    "Connection refused", "Too many levels of symbolic links", "File name too long", "Host is down",\
	"No route to host", "Directory not empty", "Too many processes", "Too many users",\
    "Disc quota exceeded", "Stale NFS file handle", "Too many levels of remote in path", \
	"RPC struct is bad", "RPC version wrong", "RPC prog. not avail", "Program version wrong",\
    "Bad procedure for program", "No locks available", "Function not implemented", \
	"Inappropriate file type or format", "Authentication error", "Need authenticator",\
	"Device power is off", "Device error", "Value too large to be stored in data type",\
    "Bad executable (or shared library)", "Bad CPU type in executable", "Shared library version mismatch",\
	"Malformed Mach-o file", "Operation canceled", "Identifier removed", "No message of desired type", \
	"Illegal byte sequence", "Attribute not found", "Bad message", "EMULTIHOP (Reserved)",\
    "No message available on STREAM", "ENOLINK (Reserved)", "No STREAM resources", "Not a STREAM",\
	"Protocol error", "STREAM ioctl timeout", "Operation not supported on socket",\
    "Policy not found", "State not recoverable", "Previous owner died", "Interface output queue is full",\
	"Unknown error"
  }
#endif
