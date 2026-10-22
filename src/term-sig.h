#ifndef TERM_SIG_H
# define TERM_SIG_H

# include <signal.h>

स्थिर पूर्णाङ्क ध्रुव term_sig[] =
  {
    SIGALRM, /* our timeout.  */
    संक_विघ्न,  /* Ctrl-C at terminal for example.  */
    SIGQUIT, /* Ctrl-\ at terminal for example.  */
    SIGHUP,  /* terminal closed for example.  */
    संक_इति, /* if terminated, stop monitored proc.  */

    SIGPIPE, SIGUSR1, SIGUSR2,

    संक_अवैध, SIGTRAP, SIGABRT, SIGBUS, संक_भ_त्रुटि, संक_अंश,

# ifdef SIGXCPU
    SIGXCPU,
# endif
# ifdef SIGXFSZ
    SIGXFSZ,
# endif
# ifdef SIGSYS
    SIGSYS,
# endif
# ifdef SIGVTALRM
    SIGVTALRM,
# endif
# ifdef SIGPROF
    SIGPROF,
# endif
# ifdef SIGPOLL
    SIGPOLL,
# endif
# ifdef SIGPWR
    SIGPWR,
# endif
# ifdef SIGSTKFLT
    SIGSTKFLT,
# endif
# ifdef SIGEMT
    SIGEMT,
# endif
# ifdef SIGBREAK
    SIGBREAK,
# endif
  };

#endif
