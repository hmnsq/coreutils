ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
bsd_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, intmax_t *length);

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sysv_sum_stream (FILE *stream, ଶୂନ୍ୟ *resstream, intmax_t *length);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*sumfn)(FILE *, ଶୂନ୍ୟ *, intmax_t *);


ବାହ୍ୟ ଶୂନ୍ୟ
output_bsd (ଅକ୍ଷର ଅଚଳ *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ_file, ଶୂନ୍ୟ ଅଚଳ *digest,
            bool raw, bool tagged, unsigned ଅକ୍ଷର delim, bool args,
            intmax_t length);

ବାହ୍ୟ ଶୂନ୍ୟ
output_sysv (ଅକ୍ଷର ଅଚଳ *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ_file, ଶୂନ୍ୟ ଅଚଳ *digest,
             bool raw, bool tagged, unsigned ଅକ୍ଷର delim, bool args,
             intmax_t length);
