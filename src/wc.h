#ضمن <stdint.h>
بنية wc_lines { صحيح err; intmax_t lines; intmax_t bytes; };
بنية wc_lines wc_lines_avx2 (صحيح);
بنية wc_lines wc_lines_avx512 (صحيح);
بنية wc_lines wc_lines_neon (صحيح fd);
