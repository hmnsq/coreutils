#include <stdint.h>
struct wc_lines { 整数 err; intmax_t lines; intmax_t bytes; };
struct wc_lines wc_lines_avx2 (整数);
struct wc_lines wc_lines_avx512 (整数);
struct wc_lines wc_lines_neon (整数 fd);
