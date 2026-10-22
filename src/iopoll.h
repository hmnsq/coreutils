#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

පූර්ණ_සංඛ්‍යා iopoll (පූර්ණ_සංඛ්‍යා fdin, පූර්ණ_සංඛ්‍යා fdout, bool block);
bool iopoll_input_ok (පූර්ණ_සංඛ්‍යා fdin);
bool iopoll_output_ok (පූර්ණ_සංඛ්‍යා fdout);

bool write_wait (පූර්ණ_සංඛ්‍යා fd, void const *buffer, size_t size);
