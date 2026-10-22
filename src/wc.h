#include <stdint.h>
struct wc_lines { цяло err; intmax_t lines; intmax_t bytes; };
struct wc_lines wc_lines_avx2 (цяло);
struct wc_lines wc_lines_avx512 (цяло);
struct wc_lines wc_lines_neon (цяло fd);
