#ವ್ಯಾಖ್ಯಾನಿಸು IOPOLL_BROKEN_OUTPUT -2
#ವ್ಯಾಖ್ಯಾನಿಸು IOPOLL_ERROR         -3

ಪೂರ್ಣಾಂಕ iopoll (ಪೂರ್ಣಾಂಕ fdin, ಪೂರ್ಣಾಂಕ fdout, bool ಖಂಡ);
bool iopoll_ಒಳಹರಿವು_ok (ಪೂರ್ಣಾಂಕ fdin);
bool iopoll_output_ok (ಪೂರ್ಣಾಂಕ fdout);

bool write_wait (ಪೂರ್ಣಾಂಕ fd, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ಬಫರ್, size_t ಗಾತ್ರ_2);
