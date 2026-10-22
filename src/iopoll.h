#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

ကိန်းပြည့် iopoll (ကိန်းပြည့် fdin, ကိန်းပြည့် fdout, bool block);
bool iopoll_input_ok (ကိန်းပြည့် fdin);
bool iopoll_output_ok (ကိန်းပြည့် fdout);

bool write_wait (ကိန်းပြည့် fd, void const *buffer, size_t size);
