வெளி முழு
bsd_sum_stream (FILE *stream, வெறுமை *resstream, intmax_t *length);

வெளி முழு
sysv_sum_stream (FILE *stream, வெறுமை *resstream, intmax_t *length);

வகைவரை முழு (*sumfn)(FILE *, வெறுமை *, intmax_t *);


வெளி வெறுமை
output_bsd (எழுத்து மாறிலி *file, முழு இருமம்_file, வெறுமை மாறிலி *digest,
            bool raw, bool tagged, unsigned எழுத்து delim, bool args,
            intmax_t length);

வெளி வெறுமை
output_sysv (எழுத்து மாறிலி *file, முழு இருமம்_file, வெறுமை மாறிலி *digest,
             bool raw, bool tagged, unsigned எழுத்து delim, bool args,
             intmax_t length);
