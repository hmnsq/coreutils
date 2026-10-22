ബാഹ്യ പൂർണ്ണസംഖ്യ
bsd_sum_stream (FILE *stream, ശൂന്യം *resstream, intmax_t *length);

ബാഹ്യ പൂർണ്ണസംഖ്യ
sysv_sum_stream (FILE *stream, ശൂന്യം *resstream, intmax_t *length);

തരനിർവചനം പൂർണ്ണസംഖ്യ (*sumfn)(FILE *, ശൂന്യം *, intmax_t *);


ബാഹ്യ ശൂന്യം
output_bsd (അക്ഷരം സ്ഥിരം *file, പൂർണ്ണസംഖ്യ ദ്വിമാന_file, ശൂന്യം സ്ഥിരം *digest,
            bool raw, bool tagged, unsigned അക്ഷരം delim, bool args,
            intmax_t length);

ബാഹ്യ ശൂന്യം
output_sysv (അക്ഷരം സ്ഥിരം *file, പൂർണ്ണസംഖ്യ ദ്വിമാന_file, ശൂന്യം സ്ഥിരം *digest,
             bool raw, bool tagged, unsigned അക്ഷരം delim, bool args,
             intmax_t length);
