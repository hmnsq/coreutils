#include <stdint.h>
sa_nracanaa wc_lines { ຈຳນວນເຕັມ err; intmax_t lines; intmax_t bytes; };
sa_nracanaa wc_lines wc_lines_avx2 (ຈຳນວນເຕັມ);
sa_nracanaa wc_lines wc_lines_avx512 (ຈຳນວນເຕັມ);
sa_nracanaa wc_lines wc_lines_neon (ຈຳນວນເຕັມ fd);
