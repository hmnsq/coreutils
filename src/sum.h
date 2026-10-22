বাহ্য পূর্ণাঙ্ক
bsd_sum_stream (FILE *stream, শূন্য *resstream, intmax_t *length);

বাহ্য পূর্ণাঙ্ক
sysv_sum_stream (FILE *stream, শূন্য *resstream, intmax_t *length);

প্রকারনির্দেশ পূর্ণাঙ্ক (*sumfn)(FILE *, শূন্য *, intmax_t *);


বাহ্য শূন্য
output_bsd (ৱর্ণ ধ্রুৱ *file, পূর্ণাঙ্ক দ্বিমিক_file, শূন্য ধ্রুৱ *digest,
            bool raw, bool tagged, unsigned ৱর্ণ delim, bool args,
            intmax_t length);

বাহ্য শূন্য
output_sysv (ৱর্ণ ধ্রুৱ *file, পূর্ণাঙ্ক দ্বিমিক_file, শূন্য ধ্রুৱ *digest,
             bool raw, bool tagged, unsigned ৱর্ণ delim, bool args,
             intmax_t length);
