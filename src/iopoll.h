#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

পূর্ণসংখ্যা iopoll (পূর্ণসংখ্যা fdin, পূর্ণসংখ্যা fdout, bool block);
bool iopoll_প্রবেশ_ok (পূর্ণসংখ্যা fdin);
bool iopoll_output_ok (পূর্ণসংখ্যা fdout);

bool write_wait (পূর্ণসংখ্যা fd, শূন্য ধ্রুবক *বাফার, size_t আকার_2);
