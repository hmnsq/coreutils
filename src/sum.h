extern 정수
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length);

extern 정수
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length);

typedef 정수 (*sumfn)(FILE *, void *, intmax_t *);


extern void
output_bsd (char const *file, 정수 binary_file, void const *digest,
            bool raw, bool tagged, unsigned char delim, bool args,
            intmax_t length);

extern void
output_sysv (char const *file, 정수 binary_file, void const *digest,
             bool raw, bool tagged, unsigned char delim, bool args,
             intmax_t length);
