ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ
bsd_sum_stream (FILE *stream, ಶೂನ್ಯ *resstream, intmax_t *length);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ
sysv_sum_stream (FILE *stream, ಶೂನ್ಯ *resstream, intmax_t *length);

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*sumfn)(FILE *, ಶೂನ್ಯ *, intmax_t *);


ಬಾಹ್ಯ ಶೂನ್ಯ
output_bsd (ವರ್ಣ ಧ್ರುವ *file, ಪೂರ್ಣಾಙ್ಕ ದ್ವಿಮಾನ_file, ಶೂನ್ಯ ಧ್ರುವ *digest,
            bool raw, bool tagged, unsigned ವರ್ಣ delim, bool args,
            intmax_t length);

ಬಾಹ್ಯ ಶೂನ್ಯ
output_sysv (ವರ್ಣ ಧ್ರುವ *file, ಪೂರ್ಣಾಙ್ಕ ದ್ವಿಮಾನ_file, ಶೂನ್ಯ ಧ್ರುವ *digest,
             bool raw, bool tagged, unsigned ವರ್ಣ delim, bool args,
             intmax_t length);
