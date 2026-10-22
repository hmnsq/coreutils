#include <stdint.h>
struct wc_lines { số_nguyên err; intmax_t lines; intmax_t bytes; };
struct wc_lines wc_lines_avx2 (số_nguyên);
struct wc_lines wc_lines_avx512 (số_nguyên);
struct wc_lines wc_lines_neon (số_nguyên fd);
