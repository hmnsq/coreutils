#परिभाषय IOPOLL_BROKEN_OUTPUT -2
#परिभाषय IOPOLL_ERROR         -3

पूर्णाङ्क iopoll (पूर्णाङ्क fdin, पूर्णाङ्क fdout, bool खण्डम्);
bool iopoll_निवेशः_ok (पूर्णाङ्क fdin);
bool iopoll_output_ok (पूर्णाङ्क fdout);

bool write_wait (पूर्णाङ्क fd, शून्य ध्रुव *बफर, size_t आकार);
