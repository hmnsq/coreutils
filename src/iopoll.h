#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

पूर्णांक iopoll (पूर्णांक fdin, पूर्णांक fdout, bool block);
bool iopoll_आदान_ok (पूर्णांक fdin);
bool iopoll_output_ok (पूर्णांक fdout);

bool write_wait (पूर्णांक fd, शून्य स्थिरांक *buffer, माप_प्रकार size);
