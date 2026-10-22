#define IOPOLL_BROKEN_OUTPUT -2
#define IOPOLL_ERROR         -3

পূর্ণাঙ্ক iopoll (পূর্ণাঙ্ক fdin, পূর্ণাঙ্ক fdout, bool block);
bool iopoll_প্রবেশ_ok (পূর্ণাঙ্ক fdin);
bool iopoll_output_ok (পূর্ণাঙ্ক fdout);

bool write_wait (পূর্ণাঙ্ক fd, শূন্য ধ্রুব *buffer, size_t size);
