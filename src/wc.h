#include <stdint.h>
struct wc_lines { 정수 err; intmax_t lines; intmax_t bytes; };
struct wc_lines wc_lines_avx2 (정수);
struct wc_lines wc_lines_avx512 (정수);
struct wc_lines wc_lines_neon (정수 fd);
