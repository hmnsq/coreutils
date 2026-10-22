بیرونی صحیح
bsd_sum_stream (FILE *stream, تهی *resstream, intmax_t *length);

بیرونی صحیح
sysv_sum_stream (FILE *stream, تهی *resstream, intmax_t *length);

تعریفنوع صحیح (*sumfn)(FILE *, تهی *, intmax_t *);


بیرونی تهی
output_bsd (نویسه ثابت *file, صحیح دودویی_file, تهی ثابت *digest,
            bool raw, bool tagged, unsigned نویسه delim, bool args,
            intmax_t length);

بیرونی تهی
output_sysv (نویسه ثابت *file, صحیح دودویی_file, تهی ثابت *digest,
             bool raw, bool tagged, unsigned نویسه delim, bool args,
             intmax_t length);
