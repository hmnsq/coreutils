बाह्य पूर्णाङ्क
bsd_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length);

बाह्य पूर्णाङ्क
sysv_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length);

प्रकारनिर्देश पूर्णाङ्क (*sumfn)(खाता *, शून्य *, intmax_t *);


बाह्य शून्य
output_bsd (वर्ण अचर *file, पूर्णाङ्क द्विआधारी_file, शून्य अचर *digest,
            bool raw, bool tagged, अचिन्हित वर्ण delim, bool args,
            intmax_t length);

बाह्य शून्य
output_sysv (वर्ण अचर *file, पूर्णाङ्क द्विआधारी_file, शून्य अचर *digest,
             bool raw, bool tagged, अचिन्हित वर्ण delim, bool args,
             intmax_t length);
