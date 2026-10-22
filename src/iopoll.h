#నిర్వచించు IOPOLL_BROKEN_OUTPUT -2
#నిర్వచించు IOPOLL_ERROR         -3

పూర్ణాంకం iopoll (పూర్ణాంకం fdin, పూర్ణాంకం fdout, bool ఖండం);
bool iopoll_ఆదానం_ok (పూర్ణాంకం fdin);
bool iopoll_output_ok (పూర్ణాంకం fdout);

bool write_wait (పూర్ణాంకం fd, శూన్య స్థిరరాశి *బఫరు, size_t కొలత);
