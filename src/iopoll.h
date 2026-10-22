#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

पूर्णाङ्क iopoll (पूर्णाङ्क fdin, पूर्णाङ्क fdout, bool block);
bool iopoll_निवेश_ok (पूर्णाङ्क fdin);
bool iopoll_output_ok (पूर्णाङ्क fdout);

bool write_wait (पूर्णाङ्क fd, शून्य ध्रुव *buffer, माप_प्रकार size);
