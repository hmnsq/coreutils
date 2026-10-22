extern 整数
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length);

extern 整数
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length);

typedef 整数 (*sumfn)(FILE *, void *, intmax_t *);


extern void
output_bsd (char const *file, 整数 binary_file, void const *digest,
            bool raw, bool tagged, unsigned char delim, bool args,
            intmax_t length);

extern void
output_sysv (char const *file, 整数 binary_file, void const *digest,
             bool raw, bool tagged, unsigned char delim, bool args,
             intmax_t length);
