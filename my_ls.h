#ifndef ls_
#define ls_

#include <unistd.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>

struct entry {
	char **list_path;
	struct content *files;
	struct content *single_files;
	int path_len;
	int files_len;
	int single_files_len;
	char *letters;
	int total_blocks;
} search_entry;

struct content {
	char *file_name;
	char file_type;
	struct stat stat;
	int status;
	int size;
	int mode;
	int hidden;
};

struct content my_get_file_content(struct stat st, char *name, char type,int status);


int my_convert_o(int dec);
int my_get_nbr(char *str);
int my_get_syntax(char c);
int my_manage_param(char *arg, va_list ap);
int my_param(char *str, va_list ap, int index);
int my_put_nbr_base(int nbr, char const *base);
int my_put_nbr_base_long(long nbr, char *base);
int my_put_special_base_str(char *str);
int my_put_str(char const *str);
int my_put_nbr(int nb);
int my_put_float(double nbr);
int my_put_address(long long nbr);
int my_strlen(char const *str);
int my_nbr_dig(int nbr);
int my_manage_param_space(int nbr, char *exceed, int order);
int my_sum_stdarg(int i, int nb, ...);
int my_disp_stdarg(char *s, ...);
int my_dispatch_param(char *arg, va_list ap, int index, char *exceed);
int my_prep_sing_file(char *file_path);
int my_check_arg(char c);
int my_process_arg(char *arg, int letters_index);
int my_contain_letter(char c);
int my_file_end_with(char *filename, char c);
int my_dir_exist(char *file_path);
int my_ignored_file(struct content entry);
int my_basic_display(void);
int my_basic_print_elem(struct content entry, int wrote);
int my_print_elem(struct content entry);
int my_print_single_file(int display, int wrote);
int my_sort_alpha(int size, int success);
int my_sort_time(int size, int success);
int my_strlen(char const *str);
int my_strcmp(char const *s1, char const *s2);

char *my_convert_low(char *str);
char *my_get_param(char *str, int start);
char *my_get_file_name_from_path(char *path);
char my_lower(char const c);
char *my_strcat(char *dest, const char *src);
char *my_strcpy(char *dest, char const *src);
char *my_prep_path(struct dirent *reader, int index);

void my_convert_s(double dec, int maj);
void my_printf(char *str, ...);
void my_put_char(char c);
void my_put_spaces(int requested_len, int nbr_len, int order);
void my_put_o_format(int octal);
void my_print_str(char *arg, va_list ap, char *exceed, int index);
void my_print_o(char *arg, va_list ap, char *exceed, int index);
void my_print_sc(char *arg, va_list ap, char *exceed, int index);
void my_del_char(char *str, int i);
void my_print_d(char *arg, va_list ap, char *exceed, int index);
void my_print_m(char *arg, va_list ap, char *exceed, int index);
void my_print_p(char *arg, va_list ap, char *exceed, int index);
void my_print_h(char *arg, va_list ap, char *exceed, int index);
void my_print_char(char *arg, va_list ap, char *exceed, int index);
void my_print_spec_str(char *arg, va_list ap, char *exceed, int index);
void my_print_f(char *arg, va_list ap, char *exceed, int index);
void my_print_b(char *arg, va_list ap, char *exceed, int index);
void my_print_dictionary(void);
void my_prep_list(int index);
void (*print_entries[172])(char *arg, va_list ap, char *exceed, int index);
void my_prep_file(char *full_path, struct content *dir_content, struct dirent *reader, int i);
void my_print_size(struct content file);
void my_print_link_target(struct content file);
void my_print_file_name(struct content entry);
void my_print_file_type(struct content file);
void my_write_cyan(const char *str);
void my_write_green(const char *str);
void my_report_error(const char *str);
void my_write_blue(const char *str);
void my_write_yellow(const char *str);
void my_amazing_sorter(void);
void my_sort_reverse(int size);
void my_parse_args(int ac, char **as);
void my_basic_print_list(void);
void my_print_list(void);
void my_print_single_files(void);
void my_process_path(char **as, int i);
void my_print_user_info(struct content file);
void my_print_acl(struct content file);
void my_print_date(struct content file);
void my_print_header_files(int i);
void my_manage_recursive(struct stat st, char *tmp, char *full_path, struct content ct);
void my_print_size_space(struct content file, int max_digits);

double my_manage_sign(char *exceed, double nbr);

#endif