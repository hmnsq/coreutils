बाह्य पूर्णाङ्क
bsd_sum_stream (FILE *stream, शून्य *resstream, intmax_t *length);

बाह्य पूर्णाङ्क
sysv_sum_stream (FILE *stream, शून्य *resstream, intmax_t *length);

प्रकारनिर्देश पूर्णाङ्क (*sumfn)(FILE *, शून्य *, intmax_t *);


बाह्य शून्य
output_bsd (वर्ण ध्रुव *file, पूर्णाङ्क द्विमान_file, शून्य ध्रुव *digest,
            bool raw, bool tagged, unsigned वर्ण delim, bool args,
            intmax_t length);

बाह्य शून्य
output_sysv (वर्ण ध्रुव *file, पूर्णाङ्क द्विमान_file, शून्य ध्रुव *digest,
             bool raw, bool tagged, unsigned वर्ण delim, bool args,
             intmax_t length);
