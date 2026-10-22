#تعریف_کرو IOPOLL_BROKEN_OUTPUT -2
#تعریف_کرو IOPOLL_ERROR         -3

صحیح iopoll (صحیح fdin, صحیح fdout, bool بلاک);
bool iopoll_داخلہ_ok (صحیح fdin);
bool iopoll_output_ok (صحیح fdout);

bool write_wait (صحیح fd, خالی ثابت *بفر, size_t سائز);
