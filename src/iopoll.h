#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

ಪೂರ್ಣಾಙ್ಕ iopoll (ಪೂರ್ಣಾಙ್ಕ fdin, ಪೂರ್ಣಾಙ್ಕ fdout, bool block);
bool iopoll_ಒಳಹರಿವು_ok (ಪೂರ್ಣಾಙ್ಕ fdin);
bool iopoll_output_ok (ಪೂರ್ಣಾಙ್ಕ fdout);

bool write_wait (ಪೂರ್ಣಾಙ್ಕ fd, ಶೂನ್ಯ ಧ್ರುವ *buffer, size_t size);
