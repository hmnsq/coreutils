خارجي صحيح
bsd_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length);

خارجي صحيح
sysv_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length);

تعريفنوع صحيح (*sumfn)(FILE *, فراغ *, intmax_t *);


خارجي فراغ
output_bsd (حرف ثابت *file, صحيح ئىككىلىك_file, فراغ ثابت *digest,
            bool raw, bool tagged, unsigned حرف delim, bool args,
            intmax_t length);

خارجي فراغ
output_sysv (حرف ثابت *file, صحيح ئىككىلىك_file, فراغ ثابت *digest,
             bool raw, bool tagged, unsigned حرف delim, bool args,
             intmax_t length);
