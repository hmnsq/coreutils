बाह्य पूर्णांक
bsd_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई);

बाह्य पूर्णांक
sysv_योग_stream (खाता *stream, व्योम *resstream, intmax_t *लंबाई);

प्रकार पूर्णांक (*sumfn)(खाता *, व्योम *, intmax_t *);


बाह्य व्योम
output_bsd (अक्षर स्थिर *file, पूर्णांक द्विआधारी_file, व्योम स्थिर *digest,
            bool raw, bool tagged, अचिन्हित अक्षर delim, bool args,
            intmax_t लंबाई);

बाह्य व्योम
output_sysv (अक्षर स्थिर *file, पूर्णांक द्विआधारी_file, व्योम स्थिर *digest,
             bool raw, bool tagged, अचिन्हित अक्षर delim, bool args,
             intmax_t लंबाई);
