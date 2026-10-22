extern ціле
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length);

extern ціле
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length);

typedef ціле (*sumfn)(FILE *, void *, intmax_t *);


extern void
output_bsd (char const *file, ціле binary_file, void const *digest,
            bool raw, bool tagged, unsigned char delim, bool args,
            intmax_t length);

extern void
output_sysv (char const *file, ціле binary_file, void const *digest,
             bool raw, bool tagged, unsigned char delim, bool args,
             intmax_t length);
