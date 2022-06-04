#include "my_ls.h"


int my_strlen(char const *str)
{
	int count = 0;

	while (str[count] != '\0')
		count++;
	return (count);
}

int my_nbr_dig(int nbr)
{
	int n = 1;
	int po = 10;

	if (nbr < 0)
		nbr = -nbr;
	while (nbr >= po) {
		n++;
		if (po > 214748364)
			break;
		po *= 10;
	}
	return (n);
}

void my_del_char(char *str, int i)
{
	int len = my_strlen(str);

	for (; i < len - 1; i++) {
		str[i] = str[i+1];
	}
	str[i] = '\0';
}

double my_manage_sign(char *exceed, double nbr)
{
	int signe = 0;
	int i = 0;

	for (i = 0; exceed[i] != '\0'; i++) {
		if (signe == 0 && exceed[i] == '+')
			signe = 1;
		if (exceed[i] == '-')
			signe *= -1;
		else if (exceed[i] == '+')
			signe *= 1;
		}
	if (nbr > 0 && signe == 1)
		my_put_char('+');
	return (nbr);
}

int my_sum_stdarg(int i, int nb, ...)
{
	va_list ap;
	int nbr = 0;
	int n = 0;

	va_start(ap, nb);
	if (i == 0)
		for (n = 0; n < nb; n++)
			nbr += va_arg(ap, int);
	else if (i == 1)
		for (n = 0; n < nb; n++)
			nbr += my_strlen((const char*)va_arg(ap, char*));
	va_end(ap);
	return (nbr);
}

int my_disp_stdarg(char *s, ...)
{
	int len = my_strlen(s);
	int i = 0;
	va_list ap;

	va_start(ap, s);
	for (i = 0; i < len; i++) {
		switch (s[i]) {
			case 'c':
				my_put_char(va_arg(ap, int));
				break;
			case 's':
				my_put_str(va_arg(ap, char*));
				break;
			case 'i':
				my_put_nbr(va_arg(ap, int));
				break;
		}
	}
	va_end(ap);
	return (0);
}

int my_put_nbr_base(int nbr, char const *base)
{
	int result = 0;
	int power = 1;
	int len_base = my_strlen(base);

	if (nbr == -1) {
		my_put_str("FFFFFFFF");
		return (0);
	}
	if (nbr < 0) {
		my_put_char('-');
		nbr = -nbr;
	}
	while ((nbr / power) >= len_base)
		power *= len_base;
	while (power > 0) {
		result = (nbr / power) % len_base;
		my_put_char(base[result]);
		power = power / len_base;
	}
	return (1);
}

int my_put_nbr_base_long(long nbr, char *base)
{
	int i = 0;
	int size;
	long nb;

	if (nbr < 0) {
		nbr = - nbr;
		i += 1;
		my_put_char('-');
	}
	size = my_strlen(base);
	if (nbr > 0) {
		nb = nbr;
		nbr /= size;
		i += my_put_nbr_base_long(nbr, base);
		i += 1;
		my_put_char(base[nb % size]);
	}
	return (i);
}

int my_put_special_base_str(char *str)
{
	int i = 0;

	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] >= 32 && str[i] <= 126)
			my_put_char(str[i]);
		else
			my_put_o_format(my_convert_o((int)str[i]));
	}
	return (0);
}

void my_put_o_format(int octal)
{
	int count = 1;

	my_put_char('\\');
	while (octal * count < 100) {
		my_put_char('0');
		count *= 10;
	}
	my_put_nbr(octal);
}

void my_put_spaces(int requested_len, int nbr_len, int order)
{
	int i = 0;
	if (requested_len > 0 && order == 0)
		for (i = nbr_len; i < abs(requested_len); i++)
			my_put_char(' ');
	else if (requested_len < 0 && order == 1)
		for (i = nbr_len; i < abs(requested_len); i++)
			my_put_char(' ');
}

void my_put_char(char c)
{
	write(1, &c, 1);
}

int my_put_str(char const *str)
{
	int length = 0;

	while (str[length] != '\0') {
		my_put_char(str[length]);
		length++;
	}
	return (0);
}

int my_put_nbr(int nb)
{
	int arr[999];
	int i = 0;
	int len = my_nbr_dig(nb);

	if (nb < 0) {
		my_put_char('-');
		nb = nb *-1;
	}
	for (i = 0; i < len; i++) {
		arr[i] = nb % 10;
		nb /= 10;
	}
	for (int b = i - 1; b > -1; b--)
		my_put_char(arr[b] + '0');
	return (0);
}

int my_put_float(double nbr)
{
	long dec;
	long ent;
	double dec2;
	int i = 0;

	ent = nbr;
	dec = nbr * (float)1000000;
	dec -= ent * 1000000;
	dec2 = (double)dec;
	i += my_put_nbr_base_long(ent, "0123456789");
	i++;
	my_put_char('.');
	i += my_put_nbr_base_long((long)dec2, "0123456789");
	return (i);
}

int my_put_address(long long nbr)
{
	int i = 0;

	if ((void*)nbr == NULL)
		i += my_put_str("(nil)");
	else {
		i += my_put_str("0x");
		i += my_put_nbr_base_long(nbr, "0123456789abcdef");
	}
	return (i);
}

void my_print_m(char *arg, va_list ap, char *exceed, int index)
{
	if (exceed == NULL || ap == NULL || arg[index] != '%')
		return;
	my_put_char('%');
}

void my_print_f(char *arg, va_list ap, char *exceed, int index)
{
	if (exceed == NULL || (arg[index] != 'f' && arg[index] != 'F'))
		return;
	my_put_float(my_manage_sign(arg, va_arg(ap, double)));
}

void my_print_b(char *arg, va_list ap, char *exceed, int index)
{
	if (exceed == NULL || (arg[index] != 'b' && arg[index] != 'B'))
		return;
	my_put_nbr_base(va_arg(ap, int), "01");
}

void my_print_p(char *arg, va_list ap, char *exceed, int index)
{
	if (exceed == NULL || (arg[index] != 'p' && arg[index] != 'P'))
		return;
	my_put_address((long long)va_arg(ap, long long));
}

void my_print_h(char *arg, va_list ap, char *exceed, int index)
{
	if (exceed == NULL || (arg[index] != 'x' && arg[index] != 'X'))
		return;
	int nbr = va_arg(ap, int);
	if (exceed[0] == '#')
		my_put_str("0x");
	my_put_nbr_base(my_manage_param_space(nbr, exceed, 0), "0123456789ABCDEF");
	my_manage_param_space(nbr, exceed, 1);
}

void my_print_d(char *arg, va_list ap, char *exceed, int index)
{
	int nbr;

	if (exceed == NULL || (arg[index] != 'd' && arg[index] != 'i'
		&& arg[index] != 'u'))
		return;
	nbr = va_arg(ap, int);
	my_put_nbr(my_manage_sign(arg, my_manage_param_space(nbr, exceed, 0)));
	my_manage_param_space(nbr, exceed, 1);
}

void my_print_str(char *arg, va_list ap, char *exceed, int index)
{
	if (exceed == NULL || arg[index] != 's')
		return;
	my_put_str((const char*)va_arg(ap, char *));
}

void my_print_char(char *arg, va_list ap, char *exceed, int index)
{
	if (exceed == NULL || arg[index] != 'c')
		return;
	my_put_char(va_arg(ap, int));
}

void my_print_special_string(char *arg, va_list ap, char *exceed, int index)
{
	if (exceed == NULL || arg[index] != 'S')
		return;
	my_put_special_base_str(va_arg(ap, char*));
}

void my_print_o(char *arg, va_list ap, char *exceed, int index)
{
	int nbr;

	if (exceed == NULL || arg[index] != 'o')
		return;
	nbr = va_arg(ap, int);
	my_put_nbr(my_convert_o(my_manage_param_space(nbr, exceed, 0)));
	my_manage_param_space(nbr, exceed, 1);
}

void my_print_sc(char *arg, va_list ap, char *exceed, int index)
{
	if (exceed == NULL)
		return;
	if (arg[index] == 'e')
		my_convert_s(va_arg(ap, double), 0);
	else if (arg[index] == 'E')
		my_convert_s(va_arg(ap, double), 1);
}

int my_manage_param(char *arg, va_list ap)
{
	int len = my_strlen(arg);
	int index = 1;
	int i = 0;
	char *exceed = malloc(sizeof(char) * len - 1);

	if(exceed == NULL)
		exit(84);
	if (len > 2 && (arg[1] == '#' || arg[1] == '+' || arg[1] == '-'
	|| arg[1] == ' ' || (arg[1] <= '9' && arg[1] >= '0'))) {
		index = len - 1;
		for (i = 1; i < len - 1; i++)
			exceed[i - 1] = arg[i];
	}
	if (len != 0) {
		if (exceed[0] == ' ' && !(exceed[1] >= '0' && exceed[1] <= '9'))
			my_put_char(' ');
		if (my_dispatch_param(arg, ap, index, exceed) == 1)
			return (1);
	}
	return (0);
}

int my_dispatch_param(char *arg, va_list ap, int index, char *exceed)
{
	if (arg[index] == 'l' && arg[index + 1] == 'x')
		index++;
	if (print_entries[(int)arg[index]] != NULL) {
		print_entries[(int)arg[index]](arg, ap, exceed, index);
		return (1);
	}
	return (0);
}

int my_manage_param_space(int nbr, char *exceed, int order)
{
	int requested_len = my_get_nbr(exceed);
	int nbr_len = my_nbr_dig(nbr);
	int exceed_len = my_strlen(exceed);

	if (nbr < 0 || (exceed[exceed_len - 1] == '+' ||
	exceed[exceed_len - 1] == '-'))
		nbr_len++;
	if (requested_len != 0)
		my_put_spaces(requested_len, nbr_len, order);
	return (nbr);
}

int my_get_nbr(char *str)
{
	int i = 0;
	int n = 0;
	int signe = 1;

	for(i = 0; str[i] != '\0'; i++) {
		if ((str[i] == '-' && signe == -1 && n == 0) ||
		(str[i] == '+' && n == 0))
			signe = 1;
		else if (str[i] == '-' && n == 0)
			signe = -1;
		if ((str[i] < '0' || str[i] > '9') && n > 0)
			break;
		else if ((str[i] < '0' || str[i] > '9') && n == 0)
			continue;
		n = n + str[i] - 48;
		n = n * 10;
	}
	n /= 10;
	return (n * signe);
}

char *my_get_param(char *str, int start)
{
	int i = start;
	int count = 0;
	char *result;

	result = malloc(sizeof(char) * 4);
	for (; str[i] != '\0'; i++) {
		if (count <= 1 && (str[i] == '%' || str[i] == ' ')) {
			result[count] = str[i];
			count++;
			continue;
		}
		if (count > 0 && my_get_syntax(str[i]) == 1) {
			result[count] = str[i];
			count++;
		} else if (count > 0)
			return (result);
	}
	return (result);
}

int my_get_syntax(char c)
{
	char allowed_chars[32] = "pcBbEeXlxFfSsdiuo0123456789#+-.";
	int i = 0;

	for (i = 0; i < 32; i++)
		if(allowed_chars[i] == c)
			return (1);
	return (0);
}

char *my_convert_low(char *str)
{
	int i = 0;

	for (; str[i] != '\0'; i++)
		if (str[i] <= 'Z' && str[i] >= 'A')
			str[i] += 32;
	return (str);
}

int my_convert_o(int dec)
{
	int octal = 0;
	int i = 1;

	while (dec != 0) {
		octal += (dec % 8) * i;
		dec /= 8;
		i *= 10;
	}
	return (octal);
}

void my_convert_s(double dec, int maj)
{
	int i = 0;

	while (!(dec < 10 && dec > -10)) {
		dec /= 10;
		i++;
	}
	my_put_float(dec);
	if (maj == 1)
		my_put_char('E');
	else
		my_put_char('e');
	my_put_char('+');
	if (my_nbr_dig(i) < 2)
		my_put_char('0');
	my_put_nbr(i);
}

int my_param(char *str, va_list ap, int index)
{
	char *arg;

	arg = my_get_param(str, index);
	if (my_manage_param(arg, ap) == 1)
		index += my_strlen(arg) - 1;
	else
		my_put_char(str[index]);
	return (index);
}

void my_printf(char *str, ...)
{
	va_list ap;
	int i = 0;

	my_print_dictionary();
	va_start(ap, str);
	for (i = 0; str[i] != '\0'; i++)
		if (str[i] == '%')
			i = my_param(str, ap, i);
		else
			my_put_char(str[i]);
	va_end(ap);
}

void my_print_dictionary(void)
{
	print_entries[(int)('s')] = my_print_str;
	print_entries[(int)('c')] = my_print_char;
	print_entries[(int)('S')] = my_print_special_string;
	print_entries[(int)('o')] = my_print_o;
	print_entries[(int)('e')] = my_print_sc;
	print_entries[(int)('E')] = my_print_sc;
	print_entries[(int)('f')] = my_print_f;
	print_entries[(int)('F')] = my_print_f;
	print_entries[(int)('b')] = my_print_b;
	print_entries[(int)('B')] = my_print_b;
	print_entries[(int)('p')] = my_print_p;
	print_entries[(int)('P')] = my_print_p;
	print_entries[(int)('x')] = my_print_h;
	print_entries[(int)('X')] = my_print_h;
	print_entries[(int)('d')] = my_print_d;
	print_entries[(int)('i')] = my_print_d;
	print_entries[(int)('u')] = my_print_d;
	print_entries[(int)('%')] = my_print_m;
}

char my_lower(char const c)
{
	char value;

	if(c >= 'A' && c <= 'Z')
		value = c + 32;
	else
		value = c;
	return (value);
}

int my_strcmp(char const *s1, char const *s2)
{
	int i = 0;

	while ((my_lower(s1[i]) == my_lower(s2[i])) &&
	(s1[i] != '\0') && (s2[i] != '\0')) {
		i++;
	}
	return (my_lower(s1[i]) - my_lower(s2[i]));
}

char *my_strcat(char *dest, const char *src) // s, append
{
	char *result = dest;

	while (*dest) {
		dest++;
	}
	while (*src) {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = *src;
	return (result);
}

char *my_strcpy(char *dest, const char *src)
{
	char *result = dest;

	while (*src) {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = *src;
	return (result);
}

void my_amazing_sorter(void)
{
	int size = search_entry.files_len;
	int success = 0;

	if (my_contain_letter('f') || my_contain_letter('U'))
		return;
	while (success == 0) {
		success = 1;
		if (my_contain_letter('t'))
			success = my_sort_time(size, success);
		else
			success = my_sort_alpha(size, success);
		size--;
	}
	if (my_contain_letter('r'))
		my_sort_reverse(size);
}

int my_sort_alpha(int size, int success)
{
	char *file1;
	char *file2;
	struct content tmp;
	int i = 0;

	for (i = 0; i < size - 1; i++) {
		file1 = search_entry.files[i].file_name;
		file2 = search_entry.files[i + 1].file_name;
		if (my_strcmp(file1, file2) > 0) {
			tmp = search_entry.files[i];
			search_entry.files[i] = search_entry.files[i + 1];
			search_entry.files[i + 1] = tmp;
			success = 0;
		}
	}
	return (success);
}

int my_sort_time(int size, int success)
{
	struct content file1;
	struct content file2;
	struct content tmp;
	int i = 0;

	for (i = 0; i < size - 1; i++) {
		file1 = search_entry.files[i];
		file2 = search_entry.files[i + 1];
		if (file1.stat.st_mtime < file2.stat.st_mtime) {
			tmp = search_entry.files[i];
			search_entry.files[i] = search_entry.files[i + 1];
			search_entry.files[i + 1] = tmp;
			success = 0;
		}
	}
	return (success);
}

void my_sort_reverse(int size)
{
	struct content tmp;
	int len = search_entry.files_len - 1;
	int i = 0;

	if (size == 0)
		size += 1;
	for (i = 0; i < len; i++) {
		tmp = search_entry.files[i];
		search_entry.files[i] = search_entry.files[len];
		search_entry.files[len] = tmp;
		len--;
	}
}

void my_report_error(const char *str)
{
	int len = 0;

	for (len = 0; str[len]; len++);
	write(2, str, len);
	exit(84);
}

void my_basic_print_list(void)
{
	int i;
	int wrote = 0;

	for (i = 0; i < search_entry.files_len; i++)
		wrote = my_basic_print_elem(search_entry.files[i], wrote);
	my_put_char('\n');
}

void my_print_list(void)
{
	int i;

	my_put_str("total ");
	my_put_nbr(search_entry.total_blocks / 2);
	my_put_char('\n');
	for (i = 0; i < search_entry.files_len; i++)
		if (my_print_elem(search_entry.files[i]))
			my_put_char('\n');
}

void my_print_single_files(void)
{
	int i;
	int wrote;
	int display = 0;

	for (i = 0; i < search_entry.path_len; i++) {
		if (my_dir_exist(search_entry.list_path[i]))
			continue;
		my_prep_sing_file(search_entry.list_path[i]);
		if (!(search_entry.single_files
		[search_entry.single_files_len - 1].stat.st_mode & S_IRUSR)) {
			my_put_str("Cannot open: Permission denied\n");
			continue;
		}
		wrote = my_print_single_file(display, wrote);
		display = 1;
	}
	if (search_entry.single_files_len == 0)
		return;
	my_put_char('\n');
	if (search_entry.path_len - search_entry.single_files_len > 0)
		my_put_char('\n');
}

void my_print_file_name(struct content file)
{
	if (S_ISDIR(file.mode)) {
		my_write_blue(file.file_name);
		if (my_contain_letter('p'))
			my_put_char('/');
		return;
	}
	if (DT_LNK == file.file_type)
		my_write_cyan(file.file_name);
	else if ((S_ISREG(file.mode)) && (file.mode & 0111))
		my_write_green(file.file_name);
	else
		my_put_str(file.file_name);
	if (DT_LNK == file.file_type && my_contain_letter('l'))
		my_print_link_target(file);
	my_print_file_type(file);
}

void my_print_file_type(struct content file)
{
	if (!my_contain_letter('F'))
		return;
	if (S_ISDIR(file.mode))
		my_put_char('/');
	if (S_ISFIFO(file.mode))
		my_put_char('|');
	else if ((S_ISREG(file.mode)) && (file.mode & 0111))
		my_put_char('*');
}

void my_print_size_space(struct content file, int max_digits)
{
	int i;

	if (my_contain_letter('k') == 1)
		for (i = 0; i + my_nbr_dig(file.size / 8) < max_digits; i++)
			my_put_char(' ');
	else
		for (i = 0; i + my_nbr_dig(file.size) < max_digits; i++)
			my_put_char(' ');
}

int my_file_end_with(char *filename, char c)
{
	int i;

	for (i = 0; filename[i]; i++)
		if (filename[i + 1] == '\0' && filename[i] == c)
			return (1);
	return (0);
}

int my_dir_exist(char *file_path)
{
	DIR *dir = opendir(file_path);

	if (dir == NULL)
		return (0);
	else {
		closedir(dir);
		return (1);
	}
}

int my_ignored_file(struct content entry)
{
	if ((!my_contain_letter('a') && !my_contain_letter('f') && entry.hidden == 1)
	|| (my_contain_letter('B') && my_file_end_with(entry.file_name, '~') == 1))
		return (1);
	return (0);
}

int my_basic_display(void)
{
	if (my_contain_letter('f'))
		return (1);
	if (!my_contain_letter('l') && !my_contain_letter('g') &&
		!my_contain_letter('n') && !my_contain_letter('o'))
		return (1);
	else
		return (0);
}

int my_basic_print_elem(struct content entry, int wrote)
{
	if (my_ignored_file(entry) == 1)
		return (wrote);
	if (my_contain_letter('1') && wrote == 1)
		my_put_char('\n');
	else if (wrote == 1)
		my_put_str("  ");
	wrote = 1;
	my_print_file_name(entry);
	return (wrote);
}

int my_print_elem(struct content entry)
{
	if (my_ignored_file(entry) == 1)
		return (0);
	my_print_acl(entry);
	my_put_char(' ');
	my_print_user_info(entry);
	my_print_size(entry);
	my_print_date(entry);
	my_put_char(' ');
	my_print_file_name(entry);
	return (1);
}

int my_print_single_file(int display, int wrote)
{
	if (display == 1 && !my_basic_display())
		my_put_char('\n');
	if (my_basic_display())
		wrote = my_basic_print_elem(search_entry.single_files
		[search_entry.single_files_len - 1], wrote);
	else
		my_print_elem(search_entry.single_files[
			search_entry.single_files_len - 1]);
	return (wrote);
}

void my_print_header_files(int i)
{
	if (search_entry.path_len > 1 || my_contain_letter('R')) {
		if (my_contain_letter('R') && (search_entry.list_path[i][0] != '.'
		&& search_entry.list_path[i][0] != '\0'))
			my_put_str("./");
		my_put_str(search_entry.list_path[i]);
		my_put_char(':');
		my_put_char('\n');
	}
	search_entry.total_blocks = 0;
}

void my_manage_recursive(struct stat st, char *tmp, char *full_path, struct content ct)
{
	if ((my_contain_letter('R') && S_ISDIR(st.st_mode))
	&& ((ct.hidden != 1) || (my_contain_letter('a') && ct.hidden == 1))) {
		if (!(st.st_mode & S_IRUSR)) {
			my_put_str("Cannot open, permission denied : ./");
			my_put_str(full_path);
			return;
		}
		my_strcat(tmp, full_path);
		search_entry.list_path[search_entry.path_len] = tmp;
		search_entry.path_len++;
	}
}

void my_print_acl(struct content file)
{
	if (S_ISDIR(file.mode))
		my_put_char('d');
	else if (DT_LNK == file.file_type) {
		my_put_str("lrwxrwxrwx. ");
		my_put_nbr(file.stat.st_nlink);
		my_put_char(' ');
		return;
	}
	else
		my_put_char('-');
	my_put_str((file.mode & S_IRUSR) ? "r" : "-");
	my_put_str((file.mode & S_IWUSR) ? "w" : "-");
	my_put_str((file.mode & S_IXUSR) ? "x" : "-");
	my_put_str((file.mode & S_IRGRP) ? "r" : "-");
	my_put_str((file.mode & S_IWGRP) ? "w" : "-");
	my_put_str((file.mode & S_IXGRP) ? "x" : "-");
	my_put_str((file.mode & S_IROTH) ? "r" : "-");
	my_put_str((file.mode & S_IWOTH) ? "w" : "-");
	my_put_str((file.mode & S_IXOTH) ? "x" : "-");
	my_put_str(". ");
	my_put_nbr(file.stat.st_nlink);
}

void my_print_user_info(struct content file)
{
	struct passwd *pw = getpwuid(file.stat.st_uid);
	struct group  *gr = getgrgid(file.stat.st_gid);

	if (my_contain_letter('n')) {
		my_put_nbr(file.stat.st_uid);
		my_put_char(' ');
		my_put_nbr(file.stat.st_uid);
		return;
	}
	if (!my_contain_letter('g')) {
		if (pw != 0)
			my_put_str(pw->pw_name);
		else
			my_put_str("Unknown");
		my_put_char(' ');
	}
	if (!my_contain_letter('o') && gr != 0)
		my_put_str(gr->gr_name);
	else if (!my_contain_letter('o'))
		my_put_str("Unknown");
}

void my_print_date(struct content file)
{
	char *date = malloc(sizeof(char) * 36);
	int i;

	if (date == NULL)
		my_report_error("Fatal error: malloc failed.");
	my_put_char(' ');
	if (my_contain_letter('u'))
		date = ctime(&file.stat.st_atime);
	else
		date = ctime(&file.stat.st_mtime);
	for (i = 0; i < 4; i++)
		my_del_char(date, 0);
	for (i = 11; i < 19; i++)
		my_del_char(date, 12);
	date[my_strlen(date) - 1] = '\0';
	my_put_str(date);
}

void my_print_size(struct content file)
{
	int max_digits = -1;
	struct content entry;
	int i;

	for (i = 0; i < search_entry.files_len; i++) {
		entry = search_entry.files[i];
		if (my_nbr_dig(entry.size) > max_digits &&
		my_contain_letter('k') == 1)
			max_digits = my_nbr_dig(entry.size / 8);
		else if (my_nbr_dig(entry.size) > max_digits)
			max_digits = my_nbr_dig(entry.size);
	}
	my_print_size_space(file, max_digits);
	my_put_char(' ');
	if (my_contain_letter('k') == 1)
		my_put_nbr(file.size / 8);
	else
		my_put_nbr(file.size);
}

void my_print_link_target(struct content file)
{
	const char *ERROR_MALLOC = "Fatal error: malloc failed.";
	size_t size = 256;
	ssize_t link_len;
	char *buffer = malloc(size);

	if (buffer == NULL)
		my_report_error(ERROR_MALLOC);
	link_len = readlink(file.file_name, buffer, size);
	if (link_len < 0)
		return;
	if ((size_t)link_len < size) {
		buffer[link_len] = '\0';
		my_put_str(" -> ");
		my_put_str(buffer);
	}
}

void my_write_blue(const char *str)
{
	const char *BLUE = "\x1B[94m";
	const char *RESET = "\033[0m";

	if (my_contain_letter('f')) {
		my_put_str(str);
		return;
	}
	my_put_str(BLUE);
	my_put_str(str);
	my_put_str(RESET);
}

void my_write_yellow(const char *str)
{
	const char *BRED = "\x1B[41m";
	const char *YEL = "\x1B[93m";
	const char *RESET = "\033[0m";
	const char *RESETB = "\x1B[49m";

	if (my_contain_letter('f')) {
		my_put_str(str);
		return;
	}
	my_put_str(BRED);
	my_put_str(YEL);
	my_put_str(str);
	my_put_str(RESET);
	my_put_str(RESETB);
}

void my_write_cyan(const char *str)
{
	const char *CYAN = "\x1B[36m";
	const char *RESET = "\033[0m";

	if (my_contain_letter('f')) {
		my_put_str(str);
		return;
	}
	my_put_str(CYAN);
	my_put_str(str);
	my_put_str(RESET);
}

void my_write_green(const char *str)
{
	const char *BOLD = "\x1B[1m";
	const char *GREEN = "\x1B[32m";
	const char *RESET = "\033[0m";
	const char *RESETB = "\x1B[49m";

	if (my_contain_letter('f')) {
		my_put_str(str);
		return;
	}
	my_put_str(BOLD);
	my_put_str(GREEN);
	my_put_str(str);
	my_put_str(RESET);
	my_put_str(RESETB);
}

void parse_args(int ac, char **as)
{
	const int allowed_syntax_len = 20;
	const char *ERROR_MALLOC = "Fatal error: malloc failed";
	int letters_index = 0;
	int i = 1;

	search_entry.path_len = 0;
	search_entry.letters = malloc(sizeof(char) * allowed_syntax_len);
	search_entry.list_path = malloc(sizeof(char*) * ac);
	if (search_entry.letters == NULL || search_entry.list_path == NULL)
		my_report_error(ERROR_MALLOC);
	for (i = 1; i < ac; i++)
		if (as[i][0] == '-')
			letters_index = my_process_arg(as[i], letters_index);
		else
			my_process_path(as, i);
	if (search_entry.path_len == 0) {
		search_entry.list_path[0] = ".";
		search_entry.path_len++;
	}
}

int my_process_arg(char *arg, int letters_index)
{
	int i;

	for (i = 1; arg[i]; i++)
		if (my_check_arg(arg[i]) == 1) {
			search_entry.letters[letters_index] = arg[i];
			letters_index++;
		}
	return (letters_index);
}

void my_process_path(char **as, int i)
{
	int len = 0;

	len = my_strlen(as[i]);
	if (as[i][len - 1] == '/' && len > 1)
		as[i][len - 1] = '\0';
	search_entry.list_path[search_entry.path_len] = as[i];
	search_entry.path_len++;
}

int my_check_arg(char c)
{
	const char *allowed_syntax = "lRdrtuFgfaAkBfgnopU1";
	int i;

	for (i = 0; allowed_syntax[i]; i++)
		if (allowed_syntax[i] == c)
			return (1);
	return (0);
}

int my_contain_letter(char c)
{
	int i;

	for (i = 0; search_entry.letters[i]; i++)
		if (search_entry.letters[i] == c)
			return (1);
	return (0);
}

char *my_get_file_name_from_path(char *path)
{
	const char *ERROR_MALLOC = "Fatal error: malloc failed.\n";
	char *file_name;
	int path_len = my_strlen(path);
	int last_slash = 0;
	int i = 0;

	for (i = 0; path[i]; i++)
		if (path[i] == '/')
			last_slash = i + 1;
	if (i == 0)
		return (path);
	file_name = malloc(sizeof(char) * (path_len - last_slash));
	if (file_name == NULL)
		my_report_error(ERROR_MALLOC);
	for (i = last_slash; path[i]; i++)
		file_name[i - last_slash] = path[i];
	return (file_name);
}

struct content my_get_file_content(struct stat st, char *name, char type,
int status)
{
	struct content dir_content;

	if (name[0] == '.')
		dir_content.hidden = 1;
	dir_content.stat = st;
	dir_content.file_name = name;
	dir_content.file_type = type;
	dir_content.mode = st.st_mode;
	dir_content.status = status;
	dir_content.size = st.st_size;
	return (dir_content);
}


int main(int ac, char **as)
{
	int i;

	parse_args(ac, as);
	my_print_single_files();
	for (i = 0; i < search_entry.path_len; i++) {
		if (!my_dir_exist(search_entry.list_path[i]))
			continue;
		my_print_header_files(i);
		my_prep_list(i);
		my_amazing_sorter();
		if (my_basic_display())
			my_basic_print_list();
		else
			my_print_list();
		if (search_entry.path_len > 1 && i + 1 +
		search_entry.single_files_len < search_entry.path_len)
			my_put_char('\n');
	}
	return (0);
}

int my_prep_sing_file(char *file_path)
{
	const char *ERROR_STATUS = "No such file or directory.\n";
	struct stat info;
	int status = stat(file_path,&info);

	if (status != 0)
		my_report_error(ERROR_STATUS);
	if (search_entry.single_files_len == 0)
		search_entry.single_files = malloc(sizeof(struct content)
		* 999);
	search_entry.single_files[search_entry.single_files_len] =
	my_get_file_content(info, file_path, 'f', status);
	search_entry.single_files_len++;
	return (1);
}

void my_prep_list(int index)
{
	DIR *rep = opendir(search_entry.list_path[index]);
	struct dirent *reader;
	struct content *dir_content;
	char *full_path = NULL;
	int i = 0;

	dir_content = malloc(sizeof(struct content) * 999);
	if (rep == NULL)
		return;
	while ((reader = readdir(rep))) {
		full_path = my_prep_path(reader, index);
		my_prep_file(full_path, dir_content, reader, i);
		i++;
		search_entry.files_len = i;
		free(full_path);
	}
	closedir(rep);
	search_entry.files = dir_content;
}

void my_prep_file(char *full_path, struct content *dir_content,
struct dirent *reader, int i)
{
	int status;
	struct stat st;
	char *tmp = malloc(sizeof(char) * (my_strlen(full_path)) + 1);

	my_printf("", full_path);
	status = stat(full_path, &st);
	dir_content[i] = my_get_file_content(st, reader->d_name,
	reader->d_type, status);
	if (dir_content[i].hidden != 1 ||
	(my_contain_letter('a') && dir_content[i].hidden == 1))
		search_entry.total_blocks +=
		(int)dir_content[i].stat.st_blocks;
	my_manage_recursive(st, tmp, full_path, dir_content[i]);
}

char *my_prep_path(struct dirent *reader, int index)
{
	char *full_path = NULL;
	int len = (my_strlen(search_entry.list_path[index]) +
	my_strlen(reader->d_name) + 2);

	if (search_entry.list_path[index][0] != '.') {
		full_path = malloc(sizeof(char) * len);
		my_strcpy(full_path, search_entry.list_path[index]);
		my_strcat(full_path, "/");
		my_strcat(full_path, reader->d_name);
	}
	else {
		full_path = malloc(sizeof(char) * my_strlen(reader->d_name));
		my_strcpy(full_path, reader->d_name);
	}
	if (full_path == NULL)
		my_report_error("Fatal error: malloc failed.");
	return (full_path);
}