#include <stdint.h>
struct wc_lines { целое err; intmax_t lines; intmax_t bytes; };
struct wc_lines wc_lines_avx2 (целое);
struct wc_lines wc_lines_avx512 (целое);
struct wc_lines wc_lines_neon (целое fd);
