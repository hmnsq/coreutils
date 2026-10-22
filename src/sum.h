extern số_nguyên
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length);

extern số_nguyên
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length);

typedef số_nguyên (*sumfn)(FILE *, void *, intmax_t *);


extern void
output_bsd (char const *file, số_nguyên binary_file, void const *digest,
            bool raw, bool tagged, unsigned char delim, bool args,
            intmax_t length);

extern void
output_sysv (char const *file, số_nguyên binary_file, void const *digest,
             bool raw, bool tagged, unsigned char delim, bool args,
             intmax_t length);
