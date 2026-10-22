/* stty -- change and print terminal line settings
   Copyright (C) 1990-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Usage: stty [-ag] [--all] [--save] [-F device] [--file=device] [setting...]

   Options:
   -a, --all    Write all current settings to stdout in human-readable form.
   -g, --save   Write all current settings to stdout in stty-readable form.
   -F, --file   Open and use the specified device instead of standard input.

   If no args are given, write to stdout the baud rate and settings that
   have been changed from their defaults.  Mode reading and changes
   are done on the specified device, or stdin if none was specified.

   David MacKenzie <djm@gnu.ai.mit.edu> */

#अन्तर्भाव <config.h>

#यदि_परिभाषितम् TERMIOS_NEEDS_XOPEN_SOURCE
# define _XOPEN_SOURCE
#यदि_अन्तः

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव <termios.h>
#यदि HAVE_STROPTS_H
# include <stropts.h>
#यदि_अन्तः
#अन्तर्भाव <sys/ioctl.h>

#यदि_परिभाषितम् WINSIZE_अन्तर्_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#यदि_अन्तः
#यदि_परिभाषितम् GWINSZ_अन्तर्_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#यदि_अन्तः
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdarg.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "fd-reopen.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xdectoint.h"
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "stty"

#परिभाषय AUTHORS proper_name ("David MacKenzie")

#यदि_अपरिभाषितम् _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#यदि_अन्तः

#परिभाषय Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#यदि_अपरिभाषितम् CINTR
# define CINTR Control ('c')
#यदि_अन्तः
#यदि_अपरिभाषितम् CQUIT
# define CQUIT 28
#यदि_अन्तः
#यदि_अपरिभाषितम् CERASE
# define CERASE 127
#यदि_अन्तः
#यदि_अपरिभाषितम् CKILL
# define CKILL Control ('u')
#यदि_अन्तः
#यदि_अपरिभाषितम् CEOF
# define CEOF Control ('d')
#यदि_अन्तः
#यदि_अपरिभाषितम् CEOL
# define CEOL _POSIX_VDISABLE
#यदि_अन्तः
#यदि_अपरिभाषितम् CSTART
# define CSTART Control ('q')
#यदि_अन्तः
#यदि_अपरिभाषितम् CSTOP
# define CSTOP Control ('s')
#यदि_अन्तः
#यदि_अपरिभाषितम् CSUSP
# define CSUSP Control ('z')
#यदि_अन्तः
#यदि defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#यदि_अन्तः
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#यदि defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#यदि_अन्तः
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#यदि defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# यदि defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#यदि_अन्तः
#यदि defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#यदि_अन्तः

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#यदि defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#यदि_अन्तः

#यदि defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#यदि_अन्तः
#यदि defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#यदि_अन्तः
#यदि defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#यदि_अन्तः
#यदि defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#यदि_अन्तः
#यदि defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#यदि_अन्तः
#यदि defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#यदि_अन्तः
#यदि defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#यदि_अन्तः
#यदि defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#यदि_अन्तः
#यदि defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#यदि_अन्तः
#यदि defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#यदि_अन्तः
#यदि defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#यदि_अन्तः
#यदि defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#यदि_अन्तः
#यदि defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#यदि_अन्तः

/* Which speeds to set.  */
गणना जवः_setting
  {
    निवेशः_जवः, output_जवः, both_speeds
  };

/* What to output and how.  */
गणना output_type
  {
    परिवर्तितम्, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
गणना बहुलकम्_type
  {
    control, निवेशः, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#परिभाषय SANE_समुच्चयः 1		/* Set in 'sane' mode. */
#परिभाषय SANE_UNSET 2		/* Unset in 'sane' mode. */
#परिभाषय REV 4			/* Can be turned off by prepending '-'. */
#परिभाषय OMIT 8			/* Don't display value. */
#परिभाषय NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
संरचना बहुलकम्_info
  {
    वर्ण ध्रुव *name;		/* Name given on command line.  */
    गणना बहुलकम्_type type;	/* Which structure element to change. */
    वर्ण flags;			/* Setting and display options.  */
    अचिह्नित दीर्घ द्वयङ्काः_2;		/* Bits to set for this mode.  */
    अचिह्नित दीर्घ आवरणम्;		/* Other bits to turn off for this mode.  */
  };

स्थिर संरचना बहुलकम्_info ध्रुव बहुलकम्_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#यदि_परिभाषितम् CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#यदि_अन्तः
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_समुच्चयः | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#यदि_परिभाषितम् CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#यदि_अन्तः
#यदि_परिभाषितम् CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#यदि_अन्तः

  {"ignbrk", निवेशः, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", निवेशः, SANE_समुच्चयः | REV, BRKINT, 0},
  {"ignpar", निवेशः, REV, IGNPAR, 0},
  {"parmrk", निवेशः, REV, PARMRK, 0},
  {"inpck", निवेशः, REV, INPCK, 0},
  {"istrip", निवेशः, REV, ISTRIP, 0},
  {"inlcr", निवेशः, SANE_UNSET | REV, INLCR, 0},
  {"igncr", निवेशः, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", निवेशः, SANE_समुच्चयः | REV, ICRNL, 0},
  {"ixon", निवेशः, REV, IXON, 0},
  {"ixoff", निवेशः, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", निवेशः, REV | OMIT, IXOFF, 0},
#यदि_परिभाषितम् IUCLC
  {"iuclc", निवेशः, SANE_UNSET | REV, IUCLC, 0},
#यदि_अन्तः
#यदि_परिभाषितम् IXANY
  {"ixany", निवेशः, SANE_UNSET | REV, IXANY, 0},
#यदि_अन्तः
#यदि_परिभाषितम् IMAXBEL
  {"imaxbel", निवेशः, SANE_समुच्चयः | REV, IMAXBEL, 0},
#यदि_अन्तः
#यदि_परिभाषितम् IUTF8
  {"iutf8", निवेशः, SANE_UNSET | REV, IUTF8, 0},
#यदि_अन्तः

  {"opost", output, SANE_समुच्चयः | REV, OPOST, 0},
#यदि_परिभाषितम् OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#यदि_अन्तः
#यदि_परिभाषितम् OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#यदि_अन्तः
#यदि_परिभाषितम् ONLCR
  {"onlcr", output, SANE_समुच्चयः | REV, ONLCR, 0},
#यदि_अन्तः
#यदि_परिभाषितम् ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#यदि_अन्तः
#यदि_परिभाषितम् ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#यदि_अन्तः
#यदि_परिभाषितम् OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#यदि_अन्तः
#यदि_परिभाषितम् OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#यदि_अन्तः
#यदि_परिभाषितम् NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_समुच्चयः, NL0, NLDLY},
#यदि_अन्तः
#यदि_परिभाषितम् CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_समुच्चयः, CR0, CRDLY},
#यदि_अन्तः
#यदि_परिभाषितम् TABDLY
# ifdef TAB3
  {"tab3", output, SANE_UNSET, TAB3, TABDLY},
# endif
# ifdef TAB2
  {"tab2", output, SANE_UNSET, TAB2, TABDLY},
# endif
# ifdef TAB1
  {"tab1", output, SANE_UNSET, TAB1, TABDLY},
# endif
# ifdef TAB0
  {"tab0", output, SANE_समुच्चयः, TAB0, TABDLY},
# endif
#अन्यथा
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#यदि_अन्तः
#यदि_परिभाषितम् BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_समुच्चयः, BS0, BSDLY},
#यदि_अन्तः
#यदि_परिभाषितम् VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_समुच्चयः, VT0, VTDLY},
#यदि_अन्तः
#यदि_परिभाषितम् FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_समुच्चयः, FF0, FFDLY},
#यदि_अन्तः

  {"isig", local, SANE_समुच्चयः | REV, ISIG, 0},
  {"icanon", local, SANE_समुच्चयः | REV, ICANON, 0},
#यदि_परिभाषितम् IEXTEN
  {"iexten", local, SANE_समुच्चयः | REV, IEXTEN, 0},
#यदि_अन्तः
  {"echo", local, SANE_समुच्चयः | REV, ECHO, 0},
  {"echoe", local, SANE_समुच्चयः | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_समुच्चयः | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#यदि_परिभाषितम् XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#यदि_अन्तः
#यदि_परिभाषितम् TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#यदि_अन्तः
#यदि_परिभाषितम् ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#यदि_अन्तः
#यदि_परिभाषितम् ECHOCTL
  {"echoctl", local, SANE_समुच्चयः | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#यदि_अन्तः
#यदि_परिभाषितम् ECHOKE
  {"echoke", local, SANE_समुच्चयः | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#यदि_अन्तः
#यदि_परिभाषितम् FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#यदि_अन्तः
#यदि defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#नोचेद्यदि defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#यदि_अन्तः

  {"evenp", combination, REV | OMIT, 0, 0},
  {"parity", combination, REV | OMIT, 0, 0},
  {"oddp", combination, REV | OMIT, 0, 0},
  {"nl", combination, REV | OMIT, 0, 0},
  {"ek", combination, OMIT, 0, 0},
  {"sane", combination, OMIT, 0, 0},
  {"cooked", combination, REV | OMIT, 0, 0},
  {"raw", combination, REV | OMIT, 0, 0},
  {"pass8", combination, REV | OMIT, 0, 0},
  {"litout", combination, REV | OMIT, 0, 0},
  {"cbreak", combination, REV | OMIT, 0, 0},
#यदि_परिभाषितम् IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#यदि_अन्तः
#यदि defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#यदि_अन्तः
#यदि defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#यदि_अन्तः
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {रिक्त, control, 0, 0, 0}
};

/* Control character settings.  */
संरचना control_info
  {
    वर्ण ध्रुव *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t विस्थापनम्;		/* Offset in c_cc.  */
  };

/* Control characters. */

स्थिर संरचना control_info ध्रुव control_info[] =
{
  {"intr", CINTR, VINTR},
  {"quit", CQUIT, VQUIT},
  {"erase", CERASE, VERASE},
  {"kill", CKILL, VKILL},
  {"eof", CEOF, VEOF},
  {"eol", CEOL, VEOL},
#यदि_परिभाषितम् VEOL2
  {"eol2", CEOL2, VEOL2},
#यदि_अन्तः
#यदि_परिभाषितम् VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#यदि_अन्तः
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#यदि_परिभाषितम् VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#यदि_अन्तः
#यदि_परिभाषितम् VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#अन्यथा
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#यदि_अन्तः
#यदि_परिभाषितम् VWERASE
  {"werase", CWERASE, VWERASE},
#यदि_अन्तः
#यदि_परिभाषितम् VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#यदि_अन्तः
#यदि_परिभाषितम् VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#यदि_अन्तः
#यदि_परिभाषितम् VSTATUS
  {"status", CSTATUS, VSTATUS},
#यदि_अन्तः

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {रिक्त, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
संरचना जवः_desc
{
  /* The baud rate.  */
  जवः_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

स्थिर वर्ण ध्रुव *visible (cc_t ch);
स्थिर अचिह्नित दीर्घ पूर्णाङ्क baud_गन्तव्यम्_मानम् (जवः_t जवः);
स्थिर bool recover_बहुलकम् (वर्ण ध्रुव *arg, संरचना termios *बहुलकम्_2);
स्थिर पूर्णाङ्क screen_columns (शून्य);
स्थिर bool समुच्चयः_बहुलकम् (संरचना बहुलकम्_info ध्रुव *info, bool reversed,
                      संरचना termios *बहुलकम्_2);
स्थिर bool eq_बहुलकम् (संरचना termios ध्रुव *mode1,
                     संरचना termios ध्रुव *mode2);
स्थिर bool जवः_matches (संरचना termios ध्रुव *mode1,
                           संरचना termios ध्रुव *mode2);
स्थिर uintmax_t integer_arg (वर्ण ध्रुव *s, uintmax_t max);
स्थिर संरचना जवः_desc string_गन्तव्यम्_baud (वर्ण ध्रुव *arg);
स्थिर tcflag_t *बहुलकम्_type_flag (गणना बहुलकम्_type type, संरचना termios *बहुलकम्_2);
स्थिर शून्य display_all (संरचना termios *बहुलकम्_2, वर्ण ध्रुव *device_name);
स्थिर शून्य display_परिवर्तितम् (संरचना termios *बहुलकम्_2);
स्थिर शून्य display_recoverable (संरचना termios *बहुलकम्_2);
स्थिर शून्य display_settings (गणना output_type output_type,
                              संरचना termios *बहुलकम्_2,
                              वर्ण ध्रुव *device_name);
स्थिर शून्य check_जवः (संरचना termios *बहुलकम्_2);
स्थिर शून्य display_जवः (संरचना termios *बहुलकम्_2, bool fancy);
स्थिर शून्य display_गवाक्षः_आकार (bool fancy, वर्ण ध्रुव *device_name);
स्थिर शून्य sane_बहुलकम् (संरचना termios *बहुलकम्_2);
स्थिर शून्य समुच्चयः_control_char (संरचना control_info ध्रुव *info,
                              वर्ण ध्रुव *arg,
                              संरचना termios *बहुलकम्_2);
स्थिर शून्य समुच्चयः_जवः (गणना जवः_setting type, वर्ण ध्रुव *arg,
                       संरचना जवः_desc ध्रुव जवः, संरचना termios *बहुलकम्_2);
स्थिर शून्य समुच्चयः_गवाक्षः_आकार (पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या_2, वर्ण ध्रुव *device_name);

/* The width of the screen, for output wrapping. */
स्थिर पूर्णाङ्क max_col;

/* Current position, to know when to wrap. */
स्थिर पूर्णाङ्क current_col;

/* Default "drain" mode for tcsetattr.  */
स्थिर पूर्णाङ्क tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
स्थिर bool dev_debug;

/* Record last speed set for correlation.  */
स्थिर संरचना जवः_desc चरमः_ibaud =
  { .baud = (जवः_t) -1, .valid = false };
स्थिर संरचना जवः_desc चरमः_obaud =
  { .baud = (जवः_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"all", no_तर्कः, रिक्त, 'a'},
  {"save", no_तर्कः, रिक्त, 'g'},
  {"file", required_तर्कः, रिक्त, 'F'},
  {"-debug", no_तर्कः, रिक्त, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((प्रारूप_लिख, 1, 2))
स्थिर शून्य
wrapf (वर्ण ध्रुव *सन्देशः_2,...)
{
  va_list तर्काः;
  वर्ण *buf;
  पूर्णाङ्क buflen;

  va_start (तर्काः, सन्देशः_2);
  buflen = vasprintf (&buf, सन्देशः_2, तर्काः);
  va_अन्तः (तर्काः);

  यदि (buflen < 0)
    xalloc_die ();

  यदि (0 < current_col)
    {
      यदि (max_col - current_col <= buflen)
        {
          अक्षरं_लिख ('\n');
          current_col = 0;
        }
      अन्यथा
        {
          अक्षरं_लिख (' ');
          current_col++;
        }
    }

  fputs (buf, stdout);
  मुक्त (buf);
  current_col += buflen;
}

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Print or change terminal characteristics.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings अन्तर् human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings अन्तर् a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  उद्घाटय and use DEVICE instead सम्बन्धि standard निवेशः\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system परिभाषते which settings are उपलब्धम्.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#यदि_परिभाषितम् VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding सम्बन्धि output\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop सङ्केत once निवेशः flushed\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   eof CHAR      CHAR will send an अन्तः सम्बन्धि file (terminate the निवेशः)\n\
   eol CHAR      CHAR will अन्तः the पङ्क्तिः\n\
"), stdout);
#यदि_परिभाषितम् VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR प्रति ending the पङ्क्तिः\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   erase CHAR    CHAR will erase the चरमः character typed\n\
   intr CHAR     CHAR will send an व्यवधानम् सङ्केत\n\
   kill CHAR     CHAR will erase the current पङ्क्तिः\n\
"), stdout);
#यदि_परिभाषितम् VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the अग्र character quoted\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् VSTATUS
      fputs (_("\
 * स्थितिः CHAR   CHAR will send an info सङ्केत\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   quit CHAR     CHAR will send a quit सङ्केत\n\
"), stdout);
#यदि defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current पङ्क्तिः\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   start CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop सङ्केत\n\
"), stdout);
#यदि_परिभाषितम् VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will विकल्प गन्तव्यम् a different shell layer\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the चरमः word typed\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
\n\
Special settings:\n\
   N             समुच्चयः_2 the निवेशः and output speeds गन्तव्यम् N bauds\n\
"), stdout);
#यदि_परिभाषितम् TIOCGWINSZ
      fputs (_("\
   स्तम्भसङ्ख्या_2 N        tell the kernel that the terminal has N columns\n\
 * columns N     same as स्तम्भसङ्ख्या_2 N\n\
"), stdout);
#यदि_अन्तः
      प्रारूप_लिख (_("\
 * [-]drain      प्रतीक्षस्व प्रति सम्प्रेषणम् before applying settings (%s by स्वभाव)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      समुच्चयः_2 the निवेशः जवः गन्तव्यम् N\n\
"), stdout);
#यदि_परिभाषितम् HAVE_C_पङ्क्तिः
      fputs (_("\
 * पङ्क्तिः N        use पङ्क्तिः discipline N\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   min N         with -icanon, समुच्चयः_2 N characters minimum प्रति a completed पठ\n\
   ospeed N      समुच्चयः_2 the output जवः गन्तव्यम् N\n\
"), stdout);
#यदि_परिभाषितम् TIOCGWINSZ
      fputs (_("\
   पङ्क्तिकाः N        tell the kernel that the terminal has N पङ्क्तिकाः\n\
   आकार          print the सङ्ख्या सम्बन्धि पङ्क्तिकाः and columns according गन्तव्यम् the kernel\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   जवः         print the terminal जवः\n\
   time N        with -icanon, समुच्चयः_2 पठ timeout सम्बन्धि N tenths सम्बन्धि a second\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control सङ्केताः\n\
   [-]cread      allow निवेशः गन्तव्यम् be received\n\
"), stdout);
#यदि_परिभाषितम् CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   csN           समुच्चयः_2 character आकार गन्तव्यम् N द्वयङ्काः_2, N अन्तर् [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop द्वयङ्काः_2 प्रत्येकम् character (one with '-')\n\
   [-]hup        send a hangup सङ्केत when the चरमः process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity bit अन्तर् output and expect parity bit अन्तर् निवेशः\n\
   [-]parodd     समुच्चयः_2 odd parity (or even parity with '-')\n\
"), stdout);
#यदि_परिभाषितम् CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
\n\
निवेशः_3 settings:\n\
   [-]brkint     breaks cause an व्यवधानम् सङ्केत\n\
   [-]icrnl      translate carriage प्रत्यागम गन्तव्यम् newline\n\
   [-]ignbrk     ignore भङ्ग characters\n\
   [-]igncr      ignore carriage प्रत्यागम\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#यदि_परिभाषितम् IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and कुरु not flush a पूर्णम् निवेशः बफर on a character\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   [-]inlcr      translate newline गन्तव्यम् carriage प्रत्यागम\n\
   [-]inpck      enable निवेशः parity checking\n\
   [-]istrip     clear उच्चः (8th) bit सम्बन्धि निवेशः characters\n\
"), stdout);
#यदि_परिभाषितम् IUTF8
      fputs (_("\
 * [-]iutf8      assume निवेशः characters are UTF-8 encoded\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters गन्तव्यम् lowercase\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only start character\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   [-]ixoff      enable sending सम्बन्धि start/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#यदि_परिभाषितम् BSDLY
      fputs (_("\
 * bsN           backspace delay style, N अन्तर् [0..1]\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् CRDLY
      fputs (_("\
 * crN           carriage प्रत्यागम delay style, N अन्तर् [0..3]\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् FFDLY
      fputs (_("\
 * ffN           form feed delay style, N अन्तर् [0..1]\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् NLDLY
      fputs (_("\
 * nlN           newline delay style, N अन्तर् [0..1]\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage प्रत्यागम गन्तव्यम् newline\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters प्रति fill instead सम्बन्धि NUL characters\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead सम्बन्धि timing प्रति delays\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters गन्तव्यम् uppercase\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् ONLCR
      fputs (_("\
 * [-]onlcr      translate newline गन्तव्यम् carriage प्रत्यागम-newline\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage प्रत्यागम\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् ONOCR
      fputs (_("\
 * [-]onocr      कुरु not print carriage returns अन्तर् the first column\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#यदि defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N अन्तर् [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N अन्तर् [0..1]\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#यदि_परिभाषितम् ECHOKE
      fputs (_("\
 * crtkill       kill all पङ्क्तिः by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all पङ्क्तिः by obeying the echoctl and echok settings\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters अन्तर् hat notation ('^c')\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   [-]echo       echo निवेशः characters\n\
"), stdout);
#यदि_परिभाषितम् ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#यदि_परिभाषितम् ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   [-]echonl     echo newline even यदि not echoing other characters\n\
"), stdout);
#यदि_परिभाषितम् ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#यदि_अन्तः
#यदि defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with उच्चः latency links\n\
"), stdout);
#यदि_अन्तः
#यदि defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#यदि_अन्तः
      प्रारूप_लिख (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#यदि_परिभाषितम् VWERASE
    ", werase"
#यदि_अन्तः
#यदि defined CREPRINT || defined VREPRINT
    ", rprnt"
#यदि_अन्तः
);
      fputs (_("\
   [-]isig       enable व्यवधानम्, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after व्यवधानम् and quit special characters\n\
"), stdout);
#यदि_परिभाषितम् ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try गन्तव्यम् लिख गन्तव्यम् the terminal\n\
"), stdout);
#यदि_अन्तः
#यदि_परिभाषितम् XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' प्रति uppercase characters\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#यदि defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters गन्तव्यम् their स्वभाव मानानि\n\
   -cooked       same as raw\n\
"), stdout);
      प्रारूप_लिख (_("\
   crt           same as %s\n\
"), "echoe"
#यदि_परिभाषितम् ECHOCTL
    " echoctl"
#यदि_अन्तः
#यदि_परिभाषितम् ECHOKE
    " echoke"
#यदि_अन्तः
);
      प्रारूप_लिख (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#यदि_परिभाषितम् ECHOCTL
    " echoctl"
#यदि_अन्तः
#यदि_परिभाषितम् ECHOKE
    " echoke"
#यदि_अन्तः
#यदि_परिभाषितम् IXANY
    " -ixany"
#यदि_अन्तः
);
#यदि_परिभाषितम् IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   ek            erase and kill characters गन्तव्यम् their स्वभाव मानानि\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#यदि defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#यदि_अन्तः
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      प्रारूप_लिख (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#यदि_परिभाषितम् ONLCR
   " -onlcr"
#यदि_अन्तः
  , "icrnl -inlcr -igncr"
#यदि_परिभाषितम् ONLCR
   " onlcr"
#यदि_अन्तः
#यदि_परिभाषितम् OCRNL
   " -ocrnl"
#यदि_अन्तः
#यदि_परिभाषितम् ONLRET
   " -onlret"
#यदि_अन्तः
);
      fputs (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), stdout);
      प्रारूप_लिख (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 time 0\n\
   -raw          same as cooked\n\
"),
#यदि_परिभाषितम् IUCLC
   " -iuclc"
#यदि_अन्तः
#यदि_परिभाषितम् IXANY
   " -ixany"
#यदि_अन्तः
#यदि_परिभाषितम् IMAXBEL
   " -imaxbel"
#यदि_अन्तः
#यदि_परिभाषितम् XCASE
   " -xcase"
#यदि_अन्तः
);
      प्रारूप_लिख (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters गन्तव्यम् their स्वभाव मानानि\n\
"),
   "-ixoff"
#यदि_परिभाषितम् IUTF8
   " -iutf8"
#यदि_अन्तः
#यदि_परिभाषितम् IUCLC
   " -iuclc"
#यदि_अन्तः
#यदि_परिभाषितम् IXANY
   " -ixany"
#यदि_अन्तः
#यदि_परिभाषितम् IMAXBEL
   " imaxbel"
#यदि_अन्तः
#यदि_परिभाषितम् XCASE
   " -xcase"
#यदि_अन्तः
#यदि_परिभाषितम् OLCUC
   " -olcuc"
#यदि_अन्तः
#यदि_परिभाषितम् OCRNL
   " -ocrnl"
#यदि_अन्तः

 , "opost"
#यदि_परिभाषितम् OFILL
   " -ofill"
#यदि_अन्तः
#यदि_परिभाषितम् ONLCR
   " onlcr"
#यदि_अन्तः
#यदि_परिभाषितम् ONOCR
   " -onocr"
#यदि_अन्तः
#यदि_परिभाषितम् ONLRET
   " -onlret"
#यदि_अन्तः
#यदि_परिभाषितम् NLDLY
   " nl0"
#यदि_अन्तः
#यदि_परिभाषितम् CRDLY
   " cr0"
#यदि_अन्तः
#यदि_परिभाषितम् TAB0
   " tab0"
#यदि_अन्तः
#यदि_परिभाषितम् BSDLY
   " bs0"
#यदि_अन्तः
#यदि_परिभाषितम् VTDLY
   " vt0"
#यदि_अन्तः
#यदि_परिभाषितम् FFDLY
   " ff0"
#यदि_अन्तः

 , "isig"
#यदि_परिभाषितम् TOSTOP
   " -tostop"
#यदि_अन्तः
#यदि_परिभाषितम् OFDEL
   " -ofdel"
#यदि_अन्तः
#यदि_परिभाषितम् ECHOPRT
   " -echoprt"
#यदि_अन्तः
#यदि_परिभाषितम् ECHOCTL
   " echoctl"
#यदि_अन्तः
#यदि_परिभाषितम् ECHOKE
   " echoke"
#यदि_अन्तः
#यदि_परिभाषितम् EXTPROC
   " -extproc"
#यदि_अन्तः
#यदि_परिभाषितम् FLUSHO
   " -flusho"
#यदि_अन्तः
);
      fputs (_("\
\n\
Handle the tty पङ्क्तिः connected गन्तव्यम् standard निवेशः.  Without arguments,\n\
prints baud दरः, पङ्क्तिः discipline, and deviations from stty sane.  अन्तर्_2\n\
settings, CHAR is गृहीतम् literally, or coded as अन्तर् ^c, 0x37, 0177 or\n\
127; special मानानि ^- or undef प्रयुक्तम् गन्तव्यम् disable special characters.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

स्थिर शून्य
apply_settings (bool checking, वर्ण ध्रुव *device_name,
                वर्ण * ध्रुव *settings, पूर्णाङ्क n_settings,
                संरचना termios *बहुलकम्_2, bool *require_समुच्चयः_attr)
{
#परिभाषय check_तर्कः(arg)						\
  कुरु									\
    {									\
      यदि (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  यावत् (0)

  प्रति (पूर्णाङ्क k = 1; k < n_settings; k++)
    {
      वर्ण ध्रुव *arg = settings[k];
      bool मेलनम्_प्राप्तम् = false;
      bool not_समुच्चयः_attr = false;
      bool reversed = false;

      यदि (! arg)
        अनुवर्त;

      यदि (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      यदि (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          अनुवर्त;
        }
      प्रति (पूर्णाङ्क i = 0; बहुलकम्_info[i].name != रिक्त; ++i)
        {
          यदि (streq (arg, बहुलकम्_info[i].name))
            {
              यदि ((बहुलकम्_info[i].flags & NO_SETATTR) == 0)
                {
                  मेलनम्_प्राप्तम् = समुच्चयः_बहुलकम् (&बहुलकम्_info[i], reversed, बहुलकम्_2);
                  *require_समुच्चयः_attr = true;
                }
              अन्यथा
                मेलनम्_प्राप्तम् = not_समुच्चयः_attr = true;
              भङ्ग;
            }
        }
      यदि (!मेलनम्_प्राप्तम् && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      यदि (!मेलनम्_प्राप्तम्)
        {
          प्रति (पूर्णाङ्क i = 0; control_info[i].name != रिक्त; ++i)
            {
              यदि (streq (arg, control_info[i].name))
                {
                  check_तर्कः (arg);
                  मेलनम्_प्राप्तम् = true;
                  ++k;
                  समुच्चयः_control_char (&control_info[i], settings[k], बहुलकम्_2);
                  *require_समुच्चयः_attr = true;
                  भङ्ग;
                }
            }
        }
      यदि (!मेलनम्_प्राप्तम् || not_समुच्चयः_attr)
        {
          यदि (streq (arg, "ispeed"))
            {
              check_तर्कः (arg);
              ++k;
              संरचना जवः_desc ध्रुव ispeed = string_गन्तव्यम्_baud (settings[k]);
              यदि (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              समुच्चयः_जवः (निवेशः_जवः, settings[k], ispeed, बहुलकम्_2);
              यदि (checking)
                अनुवर्त;
              *require_समुच्चयः_attr = true;
            }
          अन्यथा यदि (streq (arg, "ospeed"))
            {
              check_तर्कः (arg);
              ++k;
              संरचना जवः_desc ध्रुव ospeed = string_गन्तव्यम्_baud (settings[k]);
              यदि (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              समुच्चयः_जवः (output_जवः, settings[k], ospeed, बहुलकम्_2);
              यदि (checking)
                अनुवर्त;
              *require_समुच्चयः_attr = true;
            }
#यदि_परिभाषितम् TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          अन्यथा यदि (streq (arg, "extproc"))
            {
              पूर्णाङ्क val = ! reversed;

              यदि (checking)
                अनुवर्त;

              यदि (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#यदि_अन्तः
#यदि_परिभाषितम् TIOCGWINSZ
          अन्यथा यदि (streq (arg, "rows"))
            {
              check_तर्कः (arg);
              ++k;
              यदि (checking)
                अनुवर्त;
              समुच्चयः_गवाक्षः_आकार (integer_arg (settings[k], अधिकतम_पूर्णाङ्कः), -1,
                               device_name);
            }
          अन्यथा यदि (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_तर्कः (arg);
              ++k;
              यदि (checking)
                अनुवर्त;
              समुच्चयः_गवाक्षः_आकार (-1, integer_arg (settings[k], अधिकतम_पूर्णाङ्कः),
                               device_name);
            }
          अन्यथा यदि (streq (arg, "size"))
            {
              यदि (checking)
                अनुवर्त;
              max_col = screen_columns ();
              current_col = 0;
              display_गवाक्षः_आकार (false, device_name);
            }
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_C_पङ्क्तिः
          अन्यथा यदि (streq (arg, "line"))
            {
              check_तर्कः (arg);
              ++k;
              uintmax_t मानम् = integer_arg (settings[k], UINTMAX_MAX);
              यदि (ckd_सङ्कलय (&बहुलकम्_2->c_पङ्क्तिः, मानम्, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_समुच्चयः_attr = true;
            }
#यदि_अन्तः
          अन्यथा यदि (streq (arg, "speed"))
            {
              यदि (checking)
                अनुवर्त;
              max_col = screen_columns ();
              display_जवः (बहुलकम्_2, false);
            }
          अन्यथा
            {
              संरचना जवः_desc ध्रुव जवः = string_गन्तव्यम्_baud (arg);
              यदि (जवः.valid)
                {
                  समुच्चयः_जवः (both_speeds, arg, जवः, बहुलकम्_2);
                  यदि (checking)
                    अनुवर्त;
                  *require_समुच्चयः_attr = true;
                }
              अन्यथा
                {
                  यदि (! recover_बहुलकम् (arg, बहुलकम्_2))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_समुच्चयः_attr = true;
                }
            }
        }
    }

  यदि (checking)
    check_जवः (बहुलकम्_2);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  स्थिर संरचना termios बहुलकम्_2;

  गणना output_type output_type;
  पूर्णाङ्क optc;
  पूर्णाङ्क argi = 0;
  पूर्णाङ्क opti = 1;
  bool require_समुच्चयः_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  वर्ण *file_name = रिक्त;
  वर्ण ध्रुव *device_name;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = परिवर्तितम्;
  verbose_output = false;
  recoverable_output = false;

  /* Don't print error messages for unrecognized options.  */
  opterr = 0;

  /* If any new options are ever added to stty, the short options MUST
     NOT allow any ambiguity with the stty settings.  For example, the
     stty setting "-gagFork" would not be feasible, since it will be
     parsed as "-g -a -g -F ork".  If you change anything about how
     stty parses options, be sure it still works with combinations of
     short and long options, --, POSIXLY_CORRECT, etc.  */

  यावत् ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'a':
          verbose_output = true;
          output_type = all;
          भङ्ग;

        पक्ष 'g':
          recoverable_output = true;
          output_type = recoverable;
          भङ्ग;

        पक्ष 'F':
          यदि (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          भङ्ग;

        पक्ष DEV_DEBUG_OPTION:
          dev_debug = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          यदि (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          भङ्ग;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      यावत् (opti < optind)
        argv[argi + opti++] = रिक्त;
    }

  /* Specifying both -a and -g gets an error.  */
  यदि (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  यदि (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  यदि (!noargs && !verbose_output && !recoverable_output)
    {
      स्थिर संरचना termios check_बहुलकम्;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_बहुलकम्, &require_समुच्चयः_attr);
    }

  यदि (file_name)
    {
      पूर्णाङ्क fdflags;
      यदि (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      यदि ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  यदि (tcgetattr (STDIN_FILENO, &बहुलकम्_2))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  यदि (verbose_output || recoverable_output || noargs)
    {
      max_col = screen_columns ();
      current_col = 0;
      display_settings (output_type, &बहुलकम्_2, device_name);
      प्रत्यागम EXIT_SUCCESS;
    }

  require_समुच्चयः_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &बहुलकम्_2, &require_समुच्चयः_attr);

  यदि (require_समुच्चयः_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      स्थिर संरचना termios new_बहुलकम्;

      यदि (tcsetattr (STDIN_FILENO, tcsetattr_options, &बहुलकम्_2))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      यदि (tcgetattr (STDIN_FILENO, &new_बहुलकम्))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      यदि (! eq_बहुलकम् (&बहुलकम्_2, &new_बहुलकम्))
        {
          यदि (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              प्रति (अचिह्नित पूर्णाङ्क i = 0; i < परिमाण (new_बहुलकम्); i++)
                {
                  अचिह्नित पूर्णाङ्क newc = *(((अचिह्नित वर्ण *) &new_बहुलकम्) + i);
                  अचिह्नित पूर्णाङ्क oldc = *(((अचिह्नित वर्ण *) &बहुलकम्_2) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  प्रत्यागम EXIT_SUCCESS;
}

/* Return true if modes are equivalent.

   However, the speed detection is a bit finicky:

   1. the kernel MAY round the speed to the nearest supported
      value, so allow a variation of +/- 1/4 bit time per character from
      what was originally requested.

   2. There MAY be more than one internal binary encoding of a specific
      speed (e.g. on Linux).  On systems where that information
      affects c_cflag, this may introduce inconsistencies.

   3. An input speed of zero may be retained or normalized to the output
      speed across tcsetattr () ... tcgetattr ().  */
स्थिर bool
eq_बहुलकम् (संरचना termios ध्रुव *mode1, संरचना termios ध्रुव *mode2)
{
  यदि (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#यदि_परिभाषितम् HAVE_C_पङ्क्तिः
      || mode1->c_पङ्क्तिः != mode2->c_पङ्क्तिः
#यदि_अन्तः
      || !memeq (mode1->c_cc, mode2->c_cc, परिमाण (mode1->c_cc)))
    प्रत्यागम false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_आवरणम् = ~(tcflag_t) 0;
#यदि_परिभाषितम् CBAUD
  cflag_आवरणम् &= ~(tcflag_t) CBAUD;
#यदि_अन्तः
#यदि_परिभाषितम् CIBAUD
  cflag_आवरणम् &= ~(tcflag_t) CIBAUD;
#यदि_अन्तः

  यदि ((mode1->c_cflag ^ mode2->c_cflag) & cflag_आवरणम्)
    प्रत्यागम false;

  प्रत्यागम जवः_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

स्थिर bool
समुच्चयः_बहुलकम् (संरचना बहुलकम्_info ध्रुव *info, bool reversed, संरचना termios *बहुलकम्_2)
{
  tcflag_t *bitsp;

  यदि (reversed && (info->flags & REV) == 0)
    प्रत्यागम false;

  bitsp = बहुलकम्_type_flag (info->type, बहुलकम्_2);

  यदि (bitsp == रिक्त)
    {
      /* Combination mode. */
      यदि (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          यदि (reversed)
            बहुलकम्_2->c_cflag = (बहुलकम्_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          अन्यथा
            बहुलकम्_2->c_cflag = (बहुलकम्_2->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      अन्यथा यदि (streq (info->name, "oddp"))
        {
          यदि (reversed)
            बहुलकम्_2->c_cflag = (बहुलकम्_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          अन्यथा
            बहुलकम्_2->c_cflag = (बहुलकम्_2->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      अन्यथा यदि (streq (info->name, "nl"))
        {
          यदि (reversed)
            {
              बहुलकम्_2->c_iflag = (बहुलकम्_2->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              बहुलकम्_2->c_oflag = (बहुलकम्_2->c_oflag
#यदि_परिभाषितम् ONLCR
                               | ONLCR
#यदि_अन्तः
                )
#यदि_परिभाषितम् OCRNL
                & ~OCRNL
#यदि_अन्तः
#यदि_परिभाषितम् ONLRET
                & ~ONLRET
#यदि_अन्तः
                ;
            }
          अन्यथा
            {
              बहुलकम्_2->c_iflag = बहुलकम्_2->c_iflag & ~ICRNL;
#यदि_परिभाषितम् ONLCR
              बहुलकम्_2->c_oflag = बहुलकम्_2->c_oflag & ~ONLCR;
#यदि_अन्तः
            }
        }
      अन्यथा यदि (streq (info->name, "ek"))
        {
          बहुलकम्_2->c_cc[VERASE] = CERASE;
          बहुलकम्_2->c_cc[VKILL] = CKILL;
        }
      अन्यथा यदि (streq (info->name, "sane"))
        sane_बहुलकम् (बहुलकम्_2);
      अन्यथा यदि (streq (info->name, "cbreak"))
        {
          यदि (reversed)
            बहुलकम्_2->c_lflag |= ICANON;
          अन्यथा
            बहुलकम्_2->c_lflag &= ~ICANON;
        }
      अन्यथा यदि (streq (info->name, "pass8"))
        {
          यदि (reversed)
            {
              बहुलकम्_2->c_cflag = (बहुलकम्_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              बहुलकम्_2->c_iflag |= ISTRIP;
            }
          अन्यथा
            {
              बहुलकम्_2->c_cflag = (बहुलकम्_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              बहुलकम्_2->c_iflag &= ~ISTRIP;
            }
        }
      अन्यथा यदि (streq (info->name, "litout"))
        {
          यदि (reversed)
            {
              बहुलकम्_2->c_cflag = (बहुलकम्_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              बहुलकम्_2->c_iflag |= ISTRIP;
              बहुलकम्_2->c_oflag |= OPOST;
            }
          अन्यथा
            {
              बहुलकम्_2->c_cflag = (बहुलकम्_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              बहुलकम्_2->c_iflag &= ~ISTRIP;
              बहुलकम्_2->c_oflag &= ~OPOST;
            }
        }
      अन्यथा यदि (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          यदि ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              बहुलकम्_2->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              बहुलकम्_2->c_oflag |= OPOST;
              बहुलकम्_2->c_lflag |= ISIG | ICANON;
#यदि VMIN == VEOF
              बहुलकम्_2->c_cc[VEOF] = CEOF;
#यदि_अन्तः
#यदि VTIME == VEOL
              बहुलकम्_2->c_cc[VEOL] = CEOL;
#यदि_अन्तः
            }
          अन्यथा
            {
              /* Raw mode. */
              बहुलकम्_2->c_iflag = 0;
              बहुलकम्_2->c_oflag &= ~OPOST;
              बहुलकम्_2->c_lflag &= ~(ISIG | ICANON
#यदि_परिभाषितम् XCASE
                                 | XCASE
#यदि_अन्तः
                );
              बहुलकम्_2->c_cc[VMIN] = 1;
              बहुलकम्_2->c_cc[VTIME] = 0;
            }
        }
#यदि_परिभाषितम् IXANY
      अन्यथा यदि (streq (info->name, "decctlq"))
        {
          यदि (reversed)
            बहुलकम्_2->c_iflag |= IXANY;
          अन्यथा
            बहुलकम्_2->c_iflag &= ~IXANY;
        }
#यदि_अन्तः
#यदि_परिभाषितम् TABDLY
      अन्यथा यदि (streq (info->name, "tabs"))
        {
          यदि (reversed)
            बहुलकम्_2->c_oflag = (बहुलकम्_2->c_oflag & ~TABDLY) | TAB3;
          अन्यथा
            बहुलकम्_2->c_oflag = (बहुलकम्_2->c_oflag & ~TABDLY) | TAB0;
        }
#अन्यथा
# ifdef OXTABS
      अन्यथा यदि (streq (info->name, "tabs"))
        {
          यदि (reversed)
            बहुलकम्_2->c_oflag = बहुलकम्_2->c_oflag | OXTABS;
          अन्यथा
            बहुलकम्_2->c_oflag = बहुलकम्_2->c_oflag & ~OXTABS;
        }
# endif
#यदि_अन्तः
#यदि defined XCASE && defined IUCLC && defined OLCUC
      अन्यथा यदि (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          यदि (reversed)
            {
              बहुलकम्_2->c_lflag &= ~XCASE;
              बहुलकम्_2->c_iflag &= ~IUCLC;
              बहुलकम्_2->c_oflag &= ~OLCUC;
            }
          अन्यथा
            {
              बहुलकम्_2->c_lflag |= XCASE;
              बहुलकम्_2->c_iflag |= IUCLC;
              बहुलकम्_2->c_oflag |= OLCUC;
            }
        }
#यदि_अन्तः
      अन्यथा यदि (streq (info->name, "crt"))
        बहुलकम्_2->c_lflag |= ECHOE
#यदि_परिभाषितम् ECHOCTL
          | ECHOCTL
#यदि_अन्तः
#यदि_परिभाषितम् ECHOKE
          | ECHOKE
#यदि_अन्तः
          ;
      अन्यथा यदि (streq (info->name, "dec"))
        {
          बहुलकम्_2->c_cc[VINTR] = 3;	/* ^C */
          बहुलकम्_2->c_cc[VERASE] = 127;	/* DEL */
          बहुलकम्_2->c_cc[VKILL] = 21;	/* ^U */
          बहुलकम्_2->c_lflag |= ECHOE
#यदि_परिभाषितम् ECHOCTL
            | ECHOCTL
#यदि_अन्तः
#यदि_परिभाषितम् ECHOKE
            | ECHOKE
#यदि_अन्तः
            ;
#यदि_परिभाषितम् IXANY
          बहुलकम्_2->c_iflag &= ~IXANY;
#यदि_अन्तः
        }
    }
  अन्यथा यदि (reversed)
    *bitsp = *bitsp & ~info->आवरणम् & ~info->द्वयङ्काः_2;
  अन्यथा
    *bitsp = (*bitsp & ~info->आवरणम्) | info->द्वयङ्काः_2;

  प्रत्यागम true;
}

स्थिर शून्य
समुच्चयः_control_char (संरचना control_info ध्रुव *info, वर्ण ध्रुव *arg,
                  संरचना termios *बहुलकम्_2)
{
  अचिह्नित दीर्घ पूर्णाङ्क मानम्;

  यदि (streq (info->name, "min") || streq (info->name, "time"))
    मानम् = integer_arg (arg, TYPE_अधिकतमम् (cc_t));
  अन्यथा यदि (arg[0] == '\0' || arg[1] == '\0')
    मानम् = गन्तव्यम्_uchar (arg[0]);
  अन्यथा यदि (streq (arg, "^-") || streq (arg, "undef"))
    मानम् = _POSIX_VDISABLE;
  अन्यथा यदि (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      यदि (arg[1] == '?')
        मानम् = 127;
      अन्यथा
        मानम् = गन्तव्यम्_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  अन्यथा
    मानम् = integer_arg (arg, TYPE_अधिकतमम् (cc_t));
  बहुलकम्_2->c_cc[info->विस्थापनम्] = मानम्;
}

स्थिर शून्य
समुच्चयः_जवः (गणना जवः_setting type, वर्ण ध्रुव *arg,
           संरचना जवः_desc ध्रुव जवः, संरचना termios *बहुलकम्_2)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (जवः.valid);

  यदि (type == निवेशः_जवः || type == both_speeds)
    {
      चरमः_ibaud = जवः;
      यदि (cfsetispeed (बहुलकम्_2, जवः.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  यदि (type == output_जवः || type == both_speeds)
    {
      चरमः_obaud = जवः;
      यदि (cfsetospeed (बहुलकम्_2, जवः.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#यदि_परिभाषितम् TIOCGWINSZ

स्थिर पूर्णाङ्क
get_win_आकार (पूर्णाङ्क fd, संरचना winsize *win)
{
  पूर्णाङ्क err = ioctl (fd, TIOCGWINSZ, (वर्ण *) win);
  प्रत्यागम err;
}

स्थिर शून्य
समुच्चयः_गवाक्षः_आकार (पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या_2, वर्ण ध्रुव *device_name)
{
  संरचना winsize win;

  यदि (get_win_आकार (STDIN_FILENO, &win))
    {
      यदि (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      स्मृतिं_पूरय (&win, 0, परिमाण (win));
    }

  यदि (पङ्क्तिकाः >= 0)
    win.ws_row = पङ्क्तिकाः;
  यदि (स्तम्भसङ्ख्या_2 >= 0)
    win.ws_col = स्तम्भसङ्ख्या_2;

# ifdef TIOCSSIZE
  /* Alexander Dupuy <dupuy@cs.columbia.edu> wrote:
     The following code deals with a bug in the SunOS 4.x (and 3.x?) kernel.
     This comment from sys/ttold.h describes Sun's twisted logic - a better
     test would have been (ts_lines > 64k || ts_cols > 64k || ts_cols == 0).
     At any rate, the problem is gone in Solaris 2.x.

     Unfortunately, the old TIOCSSIZE code does collide with TIOCSWINSZ,
     but they can be disambiguated by checking whether a "struct ttysize"
     structure's "ts_lines" field is greater than 64K or not.  If so,
     it's almost certainly a "struct winsize" instead.

     At any rate, the bug manifests itself when ws_row == 0; the symptom is
     that ws_row is set to ws_col, and ws_col is set to (ws_xpixel<<16)
     + ws_ypixel.  Since GNU stty sets rows and columns separately, this bug
     caused "stty rows 0 cols 0" to set rows to cols and cols to 0, while
     "stty cols 0 rows 0" would do the right thing.  On a little-endian
     machine like the sun386i, the problem is the same, but for ws_col == 0.

     The workaround is to do the ioctl once with row and col = 1 to set the
     pixel info, and then do it again using a TIOCSSIZE to set rows/cols.  */

  यदि (win.ws_row == 0 || win.ws_col == 0)
    {
      संरचना ttysize ttysz;

      ttysz.ts_पङ्क्तयः = win.ws_row;
      ttysz.ts_स्तम्भसङ्ख्या = win.ws_col;

      win.ws_row = 1;
      win.ws_col = 1;

      यदि (ioctl (STDIN_FILENO, TIOCSWINSZ, (वर्ण *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      यदि (ioctl (STDIN_FILENO, TIOCSSIZE, (वर्ण *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      प्रत्यागम;
    }
# endif

  यदि (ioctl (STDIN_FILENO, TIOCSWINSZ, (वर्ण *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

स्थिर शून्य
display_गवाक्षः_आकार (bool fancy, वर्ण ध्रुव *device_name)
{
  संरचना winsize win;

  यदि (get_win_आकार (STDIN_FILENO, &win))
    {
      यदि (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      यदि (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  अन्यथा
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_row, win.ws_col);
      यदि (!fancy)
        current_col = 0;
    }
}
#यदि_अन्तः

स्थिर पूर्णाङ्क
screen_columns (शून्य)
{
#यदि_परिभाषितम् TIOCGWINSZ
  संरचना winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  यदि (get_win_आकार (STDOUT_FILENO, &win) == 0 && 0 < win.ws_col)
    प्रत्यागम win.ws_col;
#यदि_अन्तः
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    वर्ण *col_string = getenv ("COLUMNS");
    दीर्घ पूर्णाङ्क n_columns;
    यदि (!(col_string != रिक्त
          && xstrtol (col_string, रिक्त, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= अधिकतम_पूर्णाङ्कः))
      n_columns = 80;
    प्रत्यागम n_columns;
  }
}

ATTRIBUTE_विशुद्धम्
स्थिर tcflag_t *
बहुलकम्_type_flag (गणना बहुलकम्_type type, संरचना termios *बहुलकम्_2)
{
  विकल्प (type)
    {
    पक्ष control:
      प्रत्यागम &बहुलकम्_2->c_cflag;

    पक्ष निवेशः:
      प्रत्यागम &बहुलकम्_2->c_iflag;

    पक्ष output:
      प्रत्यागम &बहुलकम्_2->c_oflag;

    पक्ष local:
      प्रत्यागम &बहुलकम्_2->c_lflag;

    पक्ष combination:
      प्रत्यागम रिक्त;

    स्वभाव:
      unreachable ();
    }
}

स्थिर शून्य
display_settings (गणना output_type output_type, संरचना termios *बहुलकम्_2,
                  वर्ण ध्रुव *device_name)
{
  विकल्प (output_type)
    {
    पक्ष परिवर्तितम्:
      display_परिवर्तितम् (बहुलकम्_2);
      भङ्ग;

    पक्ष all:
      display_all (बहुलकम्_2, device_name);
      भङ्ग;

    पक्ष recoverable:
      display_recoverable (बहुलकम्_2);
      भङ्ग;
    }
}

स्थिर शून्य
display_परिवर्तितम् (संरचना termios *बहुलकम्_2)
{
  bool रिक्तानि_पङ्क्तिः;
  tcflag_t *bitsp;
  अचिह्नित दीर्घ आवरणम्;
  गणना बहुलकम्_type पूर्व_type = control;

  display_जवः (बहुलकम्_2, true);
#यदि_परिभाषितम् HAVE_C_पङ्क्तिः
  wrapf ("line = %d;", बहुलकम्_2->c_पङ्क्तिः);
#यदि_अन्तः
  अक्षरं_लिख ('\n');
  current_col = 0;

  रिक्तानि_पङ्क्तिः = true;
  प्रति (पूर्णाङ्क i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      यदि (बहुलकम्_2->c_cc[control_info[i].विस्थापनम्] == control_info[i].saneval)
        अनुवर्त;

#यदि_परिभाषितम् VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      यदि (streq (control_info[i].name, "flush"))
        अनुवर्त;
#यदि_अन्तः
      /* If swtch is the same as susp, don't print both.  */
#यदि VSWTCH == VSUSP
      यदि (streq (control_info[i].name, "swtch"))
        अनुवर्त;
#यदि_अन्तः
      /* If eof uses the same slot as min, only print whichever applies.  */
#यदि VEOF == VMIN
      यदि ((बहुलकम्_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        अनुवर्त;
#यदि_अन्तः

      रिक्तानि_पङ्क्तिः = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (बहुलकम्_2->c_cc[control_info[i].विस्थापनम्]));
    }
  यदि ((बहुलकम्_2->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (अचिह्नित दीर्घ पूर्णाङ्क) बहुलकम्_2->c_cc[VMIN],
             (अचिह्नित दीर्घ पूर्णाङ्क) बहुलकम्_2->c_cc[VTIME]);
    }
  अन्यथा यदि (!रिक्तानि_पङ्क्तिः)
    अक्षरं_लिख ('\n');
  current_col = 0;

  रिक्तानि_पङ्क्तिः = true;
  प्रति (पूर्णाङ्क i = 0; बहुलकम्_info[i].name != रिक्त; ++i)
    {
      यदि (बहुलकम्_info[i].flags & OMIT)
        अनुवर्त;
      यदि (बहुलकम्_info[i].type != पूर्व_type)
        {
          यदि (!रिक्तानि_पङ्क्तिः)
            {
              अक्षरं_लिख ('\n');
              current_col = 0;
              रिक्तानि_पङ्क्तिः = true;
            }
          पूर्व_type = बहुलकम्_info[i].type;
        }

      bitsp = बहुलकम्_type_flag (बहुलकम्_info[i].type, बहुलकम्_2);
      आवरणम् = बहुलकम्_info[i].आवरणम् ? बहुलकम्_info[i].आवरणम् : बहुलकम्_info[i].द्वयङ्काः_2;
      यदि ((*bitsp & आवरणम्) == बहुलकम्_info[i].द्वयङ्काः_2)
        {
          यदि (बहुलकम्_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", बहुलकम्_info[i].name);
              रिक्तानि_पङ्क्तिः = false;
            }
        }
      अन्यथा यदि ((बहुलकम्_info[i].flags & (SANE_समुच्चयः | REV)) == (SANE_समुच्चयः | REV))
        {
          wrapf ("-%s", बहुलकम्_info[i].name);
          रिक्तानि_पङ्क्तिः = false;
        }
    }
  यदि (!रिक्तानि_पङ्क्तिः)
    अक्षरं_लिख ('\n');
  current_col = 0;
}

स्थिर शून्य
display_all (संरचना termios *बहुलकम्_2, वर्ण ध्रुव *device_name)
{
  tcflag_t *bitsp;
  अचिह्नित दीर्घ आवरणम्;
  गणना बहुलकम्_type पूर्व_type = control;

  display_जवः (बहुलकम्_2, true);
#यदि_परिभाषितम् TIOCGWINSZ
  display_गवाक्षः_आकार (true, device_name);
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_C_पङ्क्तिः
  wrapf ("line = %d;", बहुलकम्_2->c_पङ्क्तिः);
#यदि_अन्तः
  अक्षरं_लिख ('\n');
  current_col = 0;

  प्रति (पूर्णाङ्क i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#यदि_परिभाषितम् VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      यदि (streq (control_info[i].name, "flush"))
        अनुवर्त;
#यदि_अन्तः
      /* If swtch is the same as susp, don't print both.  */
#यदि VSWTCH == VSUSP
      यदि (streq (control_info[i].name, "swtch"))
        अनुवर्त;
#यदि_अन्तः
      /* If eof uses the same slot as min, only print whichever applies.  */
#यदि VEOF == VMIN
      यदि ((बहुलकम्_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        अनुवर्त;
#यदि_अन्तः
      wrapf ("%s = %s;", control_info[i].name,
             visible (बहुलकम्_2->c_cc[control_info[i].विस्थापनम्]));
    }
#यदि VEOF == VMIN
  यदि ((बहुलकम्_2->c_lflag & ICANON) == 0)
#यदि_अन्तः
    wrapf ("min = %lu; time = %lu;",
           (अचिह्नित दीर्घ पूर्णाङ्क) बहुलकम्_2->c_cc[VMIN],
           (अचिह्नित दीर्घ पूर्णाङ्क) बहुलकम्_2->c_cc[VTIME]);
  यदि (current_col != 0)
    अक्षरं_लिख ('\n');
  current_col = 0;

  प्रति (पूर्णाङ्क i = 0; बहुलकम्_info[i].name != रिक्त; ++i)
    {
      यदि (बहुलकम्_info[i].flags & OMIT)
        अनुवर्त;
      यदि (बहुलकम्_info[i].type != पूर्व_type)
        {
          अक्षरं_लिख ('\n');
          current_col = 0;
          पूर्व_type = बहुलकम्_info[i].type;
        }

      bitsp = बहुलकम्_type_flag (बहुलकम्_info[i].type, बहुलकम्_2);
      आवरणम् = बहुलकम्_info[i].आवरणम् ? बहुलकम्_info[i].आवरणम् : बहुलकम्_info[i].द्वयङ्काः_2;
      यदि ((*bitsp & आवरणम्) == बहुलकम्_info[i].द्वयङ्काः_2)
        wrapf ("%s", बहुलकम्_info[i].name);
      अन्यथा यदि (बहुलकम्_info[i].flags & REV)
        wrapf ("-%s", बहुलकम्_info[i].name);
    }
  अक्षरं_लिख ('\n');
  current_col = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

स्थिर शून्य
check_जवः (संरचना termios *बहुलकम्_2)
{
  यदि (चरमः_ibaud.valid && चरमः_obaud.valid)
    {
      यदि (cfgetispeed (बहुलकम्_2) != चरमः_ibaud.baud
          || cfgetospeed (बहुलकम्_2) != चरमः_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_गन्तव्यम्_मानम् (चरमः_ibaud.baud),
               baud_गन्तव्यम्_मानम् (चरमः_obaud.baud));
    }
}

स्थिर शून्य
display_जवः (संरचना termios *बहुलकम्_2, bool fancy)
{
  यदि (cfgetispeed (बहुलकम्_2) == 0 || cfgetispeed (बहुलकम्_2) == cfgetospeed (बहुलकम्_2))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_गन्तव्यम्_मानम् (cfgetospeed (बहुलकम्_2)));
  अन्यथा
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_गन्तव्यम्_मानम् (cfgetispeed (बहुलकम्_2)),
           baud_गन्तव्यम्_मानम् (cfgetospeed (बहुलकम्_2)));
  यदि (!fancy)
    current_col = 0;
}

स्थिर शून्य
display_recoverable (संरचना termios *बहुलकम्_2)
{
  प्रारूप_लिख ("%lx:%lx:%lx:%lx",
          (अचिह्नित दीर्घ पूर्णाङ्क) बहुलकम्_2->c_iflag,
          (अचिह्नित दीर्घ पूर्णाङ्क) बहुलकम्_2->c_oflag,
          (अचिह्नित दीर्घ पूर्णाङ्क) बहुलकम्_2->c_cflag,
          (अचिह्नित दीर्घ पूर्णाङ्क) बहुलकम्_2->c_lflag);
  प्रति (size_t i = 0; i < NCCS; ++i)
    प्रारूप_लिख (":%lx", (अचिह्नित दीर्घ पूर्णाङ्क) बहुलकम्_2->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#यदि_परिभाषितम् CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (बहुलकम्_2->c_cflag & CBAUD) == BOTHER;
#  यदि defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((बहुलकम्_2->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#यदि_अन्तः
  यदि (separate_speeds)
    प्रारूप_लिख (":%lx:%lx", (अचिह्नित दीर्घ पूर्णाङ्क) cfgetispeed (बहुलकम्_2),
            (अचिह्नित दीर्घ पूर्णाङ्क) cfgetospeed (बहुलकम्_2));

  अक्षरं_लिख ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#परिभाषय STRTOUL_TYPE(TYPE)                                               \
  स्थिर पूर्णाङ्क                                                             \
  strtoul_##TYPE (वर्ण ध्रुव *s, पूर्णाङ्क आधारः_2, वर्ण **p, TYPE *परिणाम,       \
                  वर्ण delim)                                            \
  {                                                                      \
    अचिह्नित दीर्घ ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, आधारः_2);                                           \
    यदि (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      प्रत्यागम -1;                                                         \
    *परिणाम = ul;                                                        \
    प्रत्यागम 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (जवः_t)
#अपरिभाषय STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#अन्तर्भाव "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
स्थिर bool
recover_बहुलकम् (वर्ण ध्रुव *arg, संरचना termios *बहुलकम्_2)
{
  tcflag_t flag[4];
  वर्ण ध्रुव *s = arg;
  प्रति (size_t i = 0; i < 4; i++)
    {
      वर्ण *p;
      यदि (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        प्रत्यागम false;
      s = p + 1;
    }
  बहुलकम्_2->c_iflag = flag[0];
  बहुलकम्_2->c_oflag = flag[1];
  बहुलकम्_2->c_cflag = flag[2];
  बहुलकम्_2->c_lflag = flag[3];

  प्रति (size_t i = 0; i < NCCS; ++i)
    {
      वर्ण *p;
      वर्ण delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      यदि (strtoul_cc_t (s, 16, &p, बहुलकम्_2->c_cc + i, delim) != 0)
        प्रत्यागम false;
      /* Accept the old format, which has no separate speed fields.  */
      यदि (! *p)
        {
#यदि_परिभाषितम् CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_जवः_T_SANE
          संरचना जवः_desc ospeed = cbaud_गन्तव्यम्_जवः (बहुलकम्_2->c_cflag & CBAUD);
          यदि (ospeed.valid && cfsetospeed (बहुलकम्_2, ospeed.baud) != 0)
            प्रत्यागम false;
#  यदि defined CIBAUD && defined IBSHIFT
          संरचना जवः_desc ispeed =
            cbaud_गन्तव्यम्_जवः ((बहुलकम्_2->c_cflag & CIBAUD) >> IBSHIFT);
          यदि (ispeed.valid && cfsetispeed (बहुलकम्_2, ispeed.baud) != 0)
            प्रत्यागम false;
#  endif
# endif
          चरमः_ibaud.baud = cfgetispeed (बहुलकम्_2);
          चरमः_obaud.baud = cfgetospeed (बहुलकम्_2);
#यदि_अन्तः
          प्रत्यागम true;
        }
      s = p + 1;
    }

  जवः_t जवः[2];
  प्रति (size_t i = 0; i < 2; i++)
    {
      वर्ण *p;
      वर्ण delim = i == 0 ? ':' : '\0';
      यदि (strtoul_जवः_t (s, 16, &p, जवः + i, delim) != 0)
        प्रत्यागम false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  यदि (cfsetospeed (बहुलकम्_2, जवः[1]) != 0
      || cfsetispeed (बहुलकम्_2, जवः[0]) != 0)
    प्रत्यागम false;

  चरमः_ibaud.baud = जवः[0] == 0 ? cfgetispeed (बहुलकम्_2) : जवः[0];
  चरमः_obaud.baud = जवः[1];
  प्रत्यागम true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
स्थिर bool
जवः_matches (संरचना termios ध्रुव *mode1, संरचना termios ध्रुव *mode2)
{
  जवः_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  जवः_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  यदि (speed1[1] == 0)
    speed1[1] = speed1[0];
  यदि (speed2[1] == 0)
    speed2[1] = speed2[0];

  अचिह्नित पूर्णाङ्क द्वयङ्काः_2 = 2;  /* Start and stop bits.  */
  द्वयङ्काः_2 += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  द्वयङ्काः_2 += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  विकल्प (mode1->c_cflag & CSIZE)
    {
      पक्ष CS5: द्वयङ्काः_2 += 5; भङ्ग;
      पक्ष CS6: द्वयङ्काः_2 += 6; भङ्ग;
      पक्ष CS7: द्वयङ्काः_2 += 7; भङ्ग;
      स्वभाव: द्वयङ्काः_2 += 8; भङ्ग;
    }

  प्रति (पूर्णाङ्क i = 0; i < 2; i++)
    {
      अचिह्नित दीर्घ पूर्णाङ्क value1;
      अचिह्नित दीर्घ पूर्णाङ्क value2;
      अचिह्नित दीर्घ पूर्णाङ्क delta;

      यदि (speed1[i] == speed2[i])
        अनुवर्त;

      value1 = baud_गन्तव्यम्_मानम् (speed1[i]);
      value2 = baud_गन्तव्यम्_मानम् (speed2[i]);

#यदि_अपरिभाषितम् TERMIOS_जवः_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      यदि (value1 == ULONG_MAX || value2 == ULONG_MAX)
        प्रत्यागम false;
#यदि_अन्तः

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      यदि (delta > value1 / (द्वयङ्काः_2 * 4))
        प्रत्यागम false;
    }

  प्रत्यागम true;
}

ATTRIBUTE_विशुद्धम्
स्थिर संरचना जवः_desc
string_गन्तव्यम्_baud (वर्ण ध्रुव *arg)
{
  वर्ण *ep;
  अचिह्नित दीर्घ मानम्;
  अचिह्नित वर्ण c;

  /* Explicitly disallow negative numbers.  */
  यावत् (c_isspace (*arg))
    arg++;
  यदि (*arg == '-')
    गच्छ invalid;

  /* Empty does not mean 0.  */
  यदि (! *arg)
    गच्छ invalid;

  मानम् = strtoul (arg, &ep, 10);

  c = *ep++;
  यदि (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      यदि (c)
        {
          अचिह्नित वर्ण d = c - '0';
          यदि (d > 5)
            मानम्++;
          अन्यथा यदि (d == 5)
            {
              यावत् ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              यदि (c)
                मानम्++;                /* Nonzero, round up */
              अन्यथा
                मानम् += (मानम् & 1);   /* Exactly in the middle, round even */
            }

          यावत् (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          यदि (c)
            गच्छ invalid; /* Garbage after otherwise valid number */
        }
    }
  अन्यथा यदि (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      यदि (streq (arg, "exta"))
        प्रत्यागम (संरचना जवः_desc) { .baud = B19200, .valid = true };
      अन्यथा यदि (streq (arg, "extb"))
        प्रत्यागम (संरचना जवः_desc) { .baud = B38400, .valid = true };
      अन्यथा
        गच्छ invalid;
    }

  प्रत्यागम मानम्_गन्तव्यम्_baud (मानम्);

 invalid:
  प्रत्यागम (संरचना जवः_desc) { .baud = (जवः_t) -1, .valid = false };
}

स्थिर शून्य
sane_बहुलकम् (संरचना termios *बहुलकम्_2)
{
  tcflag_t *bitsp;

  प्रति (पूर्णाङ्क i = 0; control_info[i].name; ++i)
    {
#यदि VMIN == VEOF
      यदि (streq (control_info[i].name, "min"))
        भङ्ग;
#यदि_अन्तः
      बहुलकम्_2->c_cc[control_info[i].विस्थापनम्] = control_info[i].saneval;
    }

  प्रति (पूर्णाङ्क i = 0; बहुलकम्_info[i].name != रिक्त; ++i)
    {
      यदि (बहुलकम्_info[i].flags & NO_SETATTR)
        अनुवर्त;

      यदि (बहुलकम्_info[i].flags & SANE_समुच्चयः)
        {
          bitsp = बहुलकम्_type_flag (बहुलकम्_info[i].type, बहुलकम्_2);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~बहुलकम्_info[i].आवरणम्) | बहुलकम्_info[i].द्वयङ्काः_2;
        }
      अन्यथा यदि (बहुलकम्_info[i].flags & SANE_UNSET)
        {
          bitsp = बहुलकम्_type_flag (बहुलकम्_info[i].type, बहुलकम्_2);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~बहुलकम्_info[i].आवरणम् & ~बहुलकम्_info[i].द्वयङ्काः_2;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

स्थिर वर्ण ध्रुव *
visible (cc_t ch)
{
  स्थिर वर्ण buf[10];
  वर्ण *bpout = buf;

  यदि (ch == _POSIX_VDISABLE)
    प्रत्यागम "<undef>";

  यदि (ch >= 32)
    {
      यदि (ch < 127)
        *bpout++ = ch;
      अन्यथा यदि (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      अन्यथा
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          यदि (ch >= 128 + 32)
            {
              यदि (ch < 128 + 127)
                *bpout++ = ch - 128;
              अन्यथा
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          अन्यथा
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  अन्यथा
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  प्रत्यागम (वर्ण ध्रुव *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

स्थिर uintmax_t
integer_arg (वर्ण ध्रुव *s, uintmax_t maxval)
{
  प्रत्यागम xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
