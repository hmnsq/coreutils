#include <stdint.h>
struct wc_lines { цео err; intmax_t lines; intmax_t bytes; };
struct wc_lines wc_lines_avx2 (цео);
struct wc_lines wc_lines_avx512 (цео);
struct wc_lines wc_lines_neon (цео fd);
