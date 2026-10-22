extern ចំនួនគត់
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length);

extern ចំនួនគត់
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length);

typedef ចំនួនគត់ (*sumfn)(FILE *, void *, intmax_t *);


extern void
output_bsd (var_na const *file, ចំនួនគត់ binary_file, void const *digest,
            bool raw, bool tagged, unsigned var_na delim, bool args,
            intmax_t length);

extern void
output_sysv (var_na const *file, ចំនួនគត់ binary_file, void const *digest,
             bool raw, bool tagged, unsigned var_na delim, bool args,
             intmax_t length);
