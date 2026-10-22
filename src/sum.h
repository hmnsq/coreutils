बाह्य पूर्णांक
bsd_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length);

बाह्य पूर्णांक
sysv_sum_stream (खाता *stream, शून्य *resstream, intmax_t *length);

प्रकारव्याख्या पूर्णांक (*sumfn)(खाता *, शून्य *, intmax_t *);


बाह्य शून्य
output_bsd (अक्षर स्थिरांक *file, पूर्णांक द्विमान_file, शून्य स्थिरांक *digest,
            bool raw, bool tagged, अचिन्हित अक्षर delim, bool args,
            intmax_t length);

बाह्य शून्य
output_sysv (अक्षर स्थिरांक *file, पूर्णांक द्विमान_file, शून्य स्थिरांक *digest,
             bool raw, bool tagged, अचिन्हित अक्षर delim, bool args,
             intmax_t length);
