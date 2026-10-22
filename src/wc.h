#include <stdint.h>
struct wc_lines { ціле err; intmax_t lines; intmax_t bytes; };
struct wc_lines wc_lines_avx2 (ціле);
struct wc_lines wc_lines_avx512 (ціле);
struct wc_lines wc_lines_neon (ціле fd);
