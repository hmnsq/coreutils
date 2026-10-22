extern ကိန်းပြည့်
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length);

extern ကိန်းပြည့်
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length);

typedef ကိန်းပြည့် (*sumfn)(FILE *, void *, intmax_t *);


extern void
output_bsd (var_na const *file, ကိန်းပြည့် binary_file, void const *digest,
            bool raw, bool tagged, unsigned var_na delim, bool args,
            intmax_t length);

extern void
output_sysv (var_na const *file, ကိန်းပြည့် binary_file, void const *digest,
             bool raw, bool tagged, unsigned var_na delim, bool args,
             intmax_t length);
