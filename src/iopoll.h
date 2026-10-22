#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IOPOLL_BROKEN_OUTPUT -2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IOPOLL_ERROR         -3

ਪੂਰਨ iopoll (ਪੂਰਨ fdin, ਪੂਰਨ fdout, bool ਖੰਡ);
bool iopoll_ਨਿਵੇਸ਼_ok (ਪੂਰਨ fdin);
bool iopoll_output_ok (ਪੂਰਨ fdout);

bool write_wait (ਪੂਰਨ fd, ਖਾਲੀ ਅਚਲ *ਬਫਰ, size_t ਮਾਪ);
