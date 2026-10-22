#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

ପୂର୍ଣ୍ଣସଂଖ୍ୟା iopoll (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdin, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdout, bool block);
bool iopoll_ନିବେଶ_ok (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdin);
bool iopoll_output_ok (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdout);

bool write_wait (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଶୂନ୍ୟ ଅଚଳ *buffer, size_t size);
