extern puur_naa_nka
bsd_sum_stream (FILE *stream, void *resstream, intmax_t *length);

extern puur_naa_nka
sysv_sum_stream (FILE *stream, void *resstream, intmax_t *length);

typedef puur_naa_nka (*sumfn)(FILE *, void *, intmax_t *);


extern void
output_bsd (var_na const *file, puur_naa_nka binary_file, void const *digest,
            bool raw, bool tagged, unsigned var_na delim, bool args,
            intmax_t length);

extern void
output_sysv (var_na const *file, puur_naa_nka binary_file, void const *digest,
             bool raw, bool tagged, unsigned var_na delim, bool args,
             intmax_t length);
