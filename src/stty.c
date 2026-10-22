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

#ضمن <config.h>

#جيڪڏهن_وصف_ٿيل TERMIOS_NEEDS_XOPEN_ذريعو
# define XOPEN_ذريعو
#جيڪڏهن_پورو

#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن <termios.h>
#إذا HAVE_STROPTS_H
# include <stropts.h>
#جيڪڏهن_پورو
#ضمن <sys/ioctl.h>

#جيڪڏهن_وصف_ٿيل WINSIZE_اندريون_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل GWINSZ_اندريون_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#جيڪڏهن_پورو
#ضمن <getopt.h>
#ضمن <stdarg.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "fd-reopen.h"
#ضمن "quote.h"
#ضمن "xdectoint.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "stty"

#وصف_ڪريو AUTHORS proper_name ("David MacKenzie")

#جيڪڏهن_وصف_نه_ٿيل _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#جيڪڏهن_پورو

#وصف_ڪريو Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#جيڪڏهن_وصف_نه_ٿيل CINTR
# define CINTR Control ('c')
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CQUIT
# define CQUIT 28
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CERASE
# define CERASE 127
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CKILL
# define CKILL Control ('u')
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CEOF
# define CEOF Control ('d')
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CEOL
# define CEOL _POSIX_VDISABLE
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CSTART
# define CSTART Control ('q')
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CSTOP
# define CSTOP Control ('s')
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CSUSP
# define CSUSP Control ('z')
#جيڪڏهن_پورو
#إذا defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#جيڪڏهن_پورو
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#إذا defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#جيڪڏهن_پورو
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#إذا defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# إذا defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#جيڪڏهن_پورو
#إذا defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#جيڪڏهن_پورو

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#إذا defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#جيڪڏهن_پورو

#إذا defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#جيڪڏهن_پورو
#إذا defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#جيڪڏهن_پورو
#إذا defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#جيڪڏهن_پورو
#إذا defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#جيڪڏهن_پورو
#إذا defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#جيڪڏهن_پورو
#إذا defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#جيڪڏهن_پورو
#إذا defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#جيڪڏهن_پورو
#إذا defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#جيڪڏهن_پورو
#إذا defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#جيڪڏهن_پورو
#إذا defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#جيڪڏهن_پورو
#إذا defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#جيڪڏهن_پورو
#إذا defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#جيڪڏهن_پورو
#إذا defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#جيڪڏهن_پورو

/* Which speeds to set.  */
تعداد رفتار_setting
  {
    داخلا_رفتار, output_رفتار, both_speeds
  };

/* What to output and how.  */
تعداد output_type
  {
    بدليو, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
تعداد گهڻائي_type
  {
    control, داخلا, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#وصف_ڪريو SANE_سيٽ 1		/* Set in 'sane' mode. */
#وصف_ڪريو SANE_UNSET 2		/* Unset in 'sane' mode. */
#وصف_ڪريو REV 4			/* Can be turned off by prepending '-'. */
#وصف_ڪريو OMIT 8			/* Don't display value. */
#وصف_ڪريو NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
بنية گهڻائي_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    تعداد گهڻائي_type type;	/* Which structure element to change. */
    حرف flags;			/* Setting and display options.  */
    بنانشان ڊگهو بٽ_2;		/* Bits to set for this mode.  */
    بنانشان ڊگهو نقاب;		/* Other bits to turn off for this mode.  */
  };

ساكن بنية گهڻائي_info ثابت گهڻائي_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#جيڪڏهن_وصف_ٿيل CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#جيڪڏهن_پورو
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_سيٽ | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#جيڪڏهن_وصف_ٿيل CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#جيڪڏهن_پورو

  {"ignbrk", داخلا, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", داخلا, SANE_سيٽ | REV, BRKINT, 0},
  {"ignpar", داخلا, REV, IGNPAR, 0},
  {"parmrk", داخلا, REV, PARMRK, 0},
  {"inpck", داخلا, REV, INPCK, 0},
  {"istrip", داخلا, REV, ISTRIP, 0},
  {"inlcr", داخلا, SANE_UNSET | REV, INLCR, 0},
  {"igncr", داخلا, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", داخلا, SANE_سيٽ | REV, ICRNL, 0},
  {"ixon", داخلا, REV, IXON, 0},
  {"ixoff", داخلا, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", داخلا, REV | OMIT, IXOFF, 0},
#جيڪڏهن_وصف_ٿيل IUCLC
  {"iuclc", داخلا, SANE_UNSET | REV, IUCLC, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IXANY
  {"ixany", داخلا, SANE_UNSET | REV, IXANY, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IMAXBEL
  {"imaxbel", داخلا, SANE_سيٽ | REV, IMAXBEL, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IUTF8
  {"iutf8", داخلا, SANE_UNSET | REV, IUTF8, 0},
#جيڪڏهن_پورو

  {"opost", output, SANE_سيٽ | REV, OPOST, 0},
#جيڪڏهن_وصف_ٿيل OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONLCR
  {"onlcr", output, SANE_سيٽ | REV, ONLCR, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_سيٽ, NL0, NLDLY},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_سيٽ, CR0, CRDLY},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل TABDLY
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
  {"tab0", output, SANE_سيٽ, TAB0, TABDLY},
# endif
#وإلا
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_سيٽ, BS0, BSDLY},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_سيٽ, VT0, VTDLY},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_سيٽ, FF0, FFDLY},
#جيڪڏهن_پورو

  {"isig", local, SANE_سيٽ | REV, ISIG, 0},
  {"icanon", local, SANE_سيٽ | REV, ICANON, 0},
#جيڪڏهن_وصف_ٿيل IEXTEN
  {"iexten", local, SANE_سيٽ | REV, IEXTEN, 0},
#جيڪڏهن_پورو
  {"echo", local, SANE_سيٽ | REV, ECHO, 0},
  {"echoe", local, SANE_سيٽ | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_سيٽ | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#جيڪڏهن_وصف_ٿيل XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOCTL
  {"echoctl", local, SANE_سيٽ | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOKE
  {"echoke", local, SANE_سيٽ | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#جيڪڏهن_پورو
#إذا defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#وإلاإذا defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#جيڪڏهن_پورو

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
#جيڪڏهن_وصف_ٿيل IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#جيڪڏهن_پورو
#إذا defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#جيڪڏهن_پورو
#إذا defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#جيڪڏهن_پورو
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {عدم, control, 0, 0, 0}
};

/* Control character settings.  */
بنية control_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t وٿي;		/* Offset in c_cc.  */
  };

/* Control characters. */

ساكن بنية control_info ثابت control_info[] =
{
  {"intr", CINTR, VINTR},
  {"quit", CQUIT, VQUIT},
  {"erase", CERASE, VERASE},
  {"kill", CKILL, VKILL},
  {"eof", CEOF, VEOF},
  {"eol", CEOL, VEOL},
#جيڪڏهن_وصف_ٿيل VEOL2
  {"eol2", CEOL2, VEOL2},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#جيڪڏهن_پورو
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#جيڪڏهن_وصف_ٿيل VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#وإلا
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VWERASE
  {"werase", CWERASE, VWERASE},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VSTATUS
  {"status", CSTATUS, VSTATUS},
#جيڪڏهن_پورو

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {عدم, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
بنية رفتار_desc
{
  /* The baud rate.  */
  رفتار_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ساكن حرف ثابت *visible (cc_t ch);
ساكن بنانشان ڊگهو صحيح baud_منزل_قدر (رفتار_t رفتار);
ساكن bool recover_گهڻائي (حرف ثابت *arg, بنية termios *گهڻائي_2);
ساكن صحيح screen_columns (فراغ);
ساكن bool سيٽ_گهڻائي (بنية گهڻائي_info ثابت *info, bool reversed,
                      بنية termios *گهڻائي_2);
ساكن bool eq_گهڻائي (بنية termios ثابت *mode1,
                     بنية termios ثابت *mode2);
ساكن bool رفتار_matches (بنية termios ثابت *mode1,
                           بنية termios ثابت *mode2);
ساكن uintmax_t integer_arg (حرف ثابت *s, uintmax_t max);
ساكن بنية رفتار_desc string_منزل_baud (حرف ثابت *arg);
ساكن tcflag_t *گهڻائي_type_flag (تعداد گهڻائي_type type, بنية termios *گهڻائي_2);
ساكن فراغ display_all (بنية termios *گهڻائي_2, حرف ثابت *device_name);
ساكن فراغ display_بدليو (بنية termios *گهڻائي_2);
ساكن فراغ display_recoverable (بنية termios *گهڻائي_2);
ساكن فراغ display_settings (تعداد output_type output_type,
                              بنية termios *گهڻائي_2,
                              حرف ثابت *device_name);
ساكن فراغ check_رفتار (بنية termios *گهڻائي_2);
ساكن فراغ display_رفتار (بنية termios *گهڻائي_2, bool fancy);
ساكن فراغ display_دري_سائيز (bool fancy, حرف ثابت *device_name);
ساكن فراغ sane_گهڻائي (بنية termios *گهڻائي_2);
ساكن فراغ سيٽ_control_char (بنية control_info ثابت *info,
                              حرف ثابت *arg,
                              بنية termios *گهڻائي_2);
ساكن فراغ سيٽ_رفتار (تعداد رفتار_setting type, حرف ثابت *arg,
                       بنية رفتار_desc ثابت رفتار, بنية termios *گهڻائي_2);
ساكن فراغ سيٽ_دري_سائيز (صحيح صفون, صحيح کانن_ڳڻپ_2, حرف ثابت *device_name);

/* The width of the screen, for output wrapping. */
ساكن صحيح max_کانو;

/* Current position, to know when to wrap. */
ساكن صحيح current_کانو;

/* Default "drain" mode for tcsetattr.  */
ساكن صحيح tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ساكن bool dev_debug;

/* Record last speed set for correlation.  */
ساكن بنية رفتار_desc آخري_ibaud =
  { .baud = (رفتار_t) -1, .valid = false };
ساكن بنية رفتار_desc آخري_obaud =
  { .baud = (رفتار_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

ساكن بنية option ثابت longopts[] =
{
  {"all", no_دليل, عدم, 'a'},
  {"save", no_دليل, عدم, 'g'},
  {"file", required_دليل, عدم, 'F'},
  {"-debug", no_دليل, عدم, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((لکو, 1, 2))
ساكن فراغ
wrapf (حرف ثابت *پيغام_2,...)
{
  va_list دليلون;
  حرف *buf;
  صحيح buflen;

  va_شروعات (دليلون, پيغام_2);
  buflen = vasprintf (&buf, پيغام_2, دليلون);
  va_پڇاڙي (دليلون);

  إذا (buflen < 0)
    xalloc_die ();

  إذا (0 < current_کانو)
    {
      إذا (max_کانو - current_کانو <= buflen)
        {
          اکر_لکو ('\n');
          current_کانو = 0;
        }
      وإلا
        {
          اکر_لکو (' ');
          current_کانو++;
        }
    }

  fputs (buf, stdout);
  حرر (buf);
  current_کانو += buflen;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Print or change terminal characteristics.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings اندريون human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings اندريون a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  open and use DEVICE instead جو standard داخلا\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system مقرر_ڪري which settings are موجود.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#جيڪڏهن_وصف_ٿيل VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding جو output\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop إشارة once داخلا flushed\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   eof CHAR      CHAR will send an پڇاڙي جو file (terminate the داخلا)\n\
   eol CHAR      CHAR will پڇاڙي the سٽ\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR لكل ending the سٽ\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   erase CHAR    CHAR will erase the آخري character typed\n\
   intr CHAR     CHAR will send an مداخلت إشارة\n\
   kill CHAR     CHAR will erase the current سٽ\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the ايندڙ character quoted\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VSTATUS
      fputs (_("\
 * حالت CHAR   CHAR will send an info إشارة\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   quit CHAR     CHAR will send a quit إشارة\n\
"), stdout);
#إذا defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current سٽ\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   شروعات CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop إشارة\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will بدل منزل a different shell layer\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the آخري لفظ_2 typed\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
\n\
Special settings:\n\
   N             سيٽ_2 the داخلا and output speeds منزل N bauds\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل TIOCGWINSZ
      fputs (_("\
   کانن_ڳڻپ_2 N        tell the kernel that the terminal has N columns\n\
 * columns N     same as کانن_ڳڻپ_2 N\n\
"), stdout);
#جيڪڏهن_پورو
      لکو (_("\
 * [-]drain      wait لكل ترسيل before applying settings (%s by افتراضي)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      سيٽ_2 the داخلا رفتار منزل N\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل HAVE_C_سٽ
      fputs (_("\
 * سٽ N        use سٽ discipline N\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   min N         with -icanon, سيٽ_2 N characters minimum لكل a completed اقرأ\n\
   ospeed N      سيٽ_2 the output رفتار منزل N\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل TIOCGWINSZ
      fputs (_("\
   صفون N        tell the kernel that the terminal has N صفون\n\
   سائيز          print the عدد جو صفون and columns according منزل the kernel\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   رفتار         print the terminal رفتار\n\
   وقت N        with -icanon, سيٽ_2 اقرأ timeout جو N tenths جو a ٻيو\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control سگنل\n\
   [-]cread      allow داخلا منزل be received\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   csN           سيٽ_2 character سائيز منزل N بٽ_2, N اندريون [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop بٽ_2 في character (one with '-')\n\
   [-]hup        send a hangup إشارة when the آخري process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity ٻٽو_انگ اندريون output and expect parity ٻٽو_انگ اندريون داخلا\n\
   [-]parodd     سيٽ_2 odd parity (or even parity with '-')\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
\n\
داخلا_4 settings:\n\
   [-]brkint     breaks cause an مداخلت إشارة\n\
   [-]icrnl      translate carriage أرجع منزل newline\n\
   [-]ignbrk     ignore اكسر characters\n\
   [-]igncr      ignore carriage أرجع\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and افعل not flush a ڀريل داخلا بفر on a character\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   [-]inlcr      translate newline منزل carriage أرجع\n\
   [-]inpck      enable داخلا parity checking\n\
   [-]istrip     clear مٿاهون (8th) ٻٽو_انگ جو داخلا characters\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل IUTF8
      fputs (_("\
 * [-]iutf8      assume داخلا characters are UTF-8 encoded\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters منزل lowercase\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only شروعات character\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   [-]ixoff      enable sending جو شروعات/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل BSDLY
      fputs (_("\
 * bsN           backspace delay style, N اندريون [0..1]\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CRDLY
      fputs (_("\
 * crN           carriage أرجع delay style, N اندريون [0..3]\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل FFDLY
      fputs (_("\
 * ffN           form feed delay style, N اندريون [0..1]\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل NLDLY
      fputs (_("\
 * nlN           newline delay style, N اندريون [0..1]\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage أرجع منزل newline\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters لكل fill instead جو NUL characters\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead جو timing لكل delays\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters منزل uppercase\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONLCR
      fputs (_("\
 * [-]onlcr      translate newline منزل carriage أرجع-newline\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage أرجع\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONOCR
      fputs (_("\
 * [-]onocr      افعل not print carriage returns اندريون the پهريون ڪالم\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#إذا defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N اندريون [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N اندريون [0..1]\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل ECHOKE
      fputs (_("\
 * crtkill       kill all سٽ by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all سٽ by obeying the echoctl and echok settings\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters اندريون hat notation ('^c')\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   [-]echo       echo داخلا characters\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   [-]echonl     echo newline even إذا not echoing other characters\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#جيڪڏهن_پورو
#إذا defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with مٿاهون latency links\n\
"), stdout);
#جيڪڏهن_پورو
#إذا defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#جيڪڏهن_پورو
      لکو (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#جيڪڏهن_وصف_ٿيل VWERASE
    ", werase"
#جيڪڏهن_پورو
#إذا defined CREPRINT || defined VREPRINT
    ", rprnt"
#جيڪڏهن_پورو
);
      fputs (_("\
   [-]isig       enable مداخلت, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after مداخلت and quit special characters\n\
"), stdout);
#جيڪڏهن_وصف_ٿيل ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try منزل اكتب منزل the terminal\n\
"), stdout);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' لكل uppercase characters\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#إذا defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters منزل their افتراضي قدرون\n\
   -cooked       same as raw\n\
"), stdout);
      لکو (_("\
   crt           same as %s\n\
"), "echoe"
#جيڪڏهن_وصف_ٿيل ECHOCTL
    " echoctl"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOKE
    " echoke"
#جيڪڏهن_پورو
);
      لکو (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#جيڪڏهن_وصف_ٿيل ECHOCTL
    " echoctl"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOKE
    " echoke"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IXANY
    " -ixany"
#جيڪڏهن_پورو
);
#جيڪڏهن_وصف_ٿيل IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   ek            erase and kill characters منزل their افتراضي قدرون\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#إذا defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#جيڪڏهن_پورو
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      لکو (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#جيڪڏهن_وصف_ٿيل ONLCR
   " -onlcr"
#جيڪڏهن_پورو
  , "icrnl -inlcr -igncr"
#جيڪڏهن_وصف_ٿيل ONLCR
   " onlcr"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OCRNL
   " -ocrnl"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONLRET
   " -onlret"
#جيڪڏهن_پورو
);
      fputs (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), stdout);
      لکو (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 وقت 0\n\
   -raw          same as cooked\n\
"),
#جيڪڏهن_وصف_ٿيل IUCLC
   " -iuclc"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IXANY
   " -ixany"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IMAXBEL
   " -imaxbel"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل XCASE
   " -xcase"
#جيڪڏهن_پورو
);
      لکو (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters منزل their افتراضي قدرون\n\
"),
   "-ixoff"
#جيڪڏهن_وصف_ٿيل IUTF8
   " -iutf8"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IUCLC
   " -iuclc"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IXANY
   " -ixany"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل IMAXBEL
   " imaxbel"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل XCASE
   " -xcase"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OLCUC
   " -olcuc"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OCRNL
   " -ocrnl"
#جيڪڏهن_پورو

 , "opost"
#جيڪڏهن_وصف_ٿيل OFILL
   " -ofill"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONLCR
   " onlcr"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONOCR
   " -onocr"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONLRET
   " -onlret"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل NLDLY
   " nl0"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CRDLY
   " cr0"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل TAB0
   " tab0"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل BSDLY
   " bs0"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل VTDLY
   " vt0"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل FFDLY
   " ff0"
#جيڪڏهن_پورو

 , "isig"
#جيڪڏهن_وصف_ٿيل TOSTOP
   " -tostop"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OFDEL
   " -ofdel"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOPRT
   " -echoprt"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOCTL
   " echoctl"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOKE
   " echoke"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل EXTPROC
   " -extproc"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل FLUSHO
   " -flusho"
#جيڪڏهن_پورو
);
      fputs (_("\
\n\
Handle the tty سٽ connected منزل standard داخلا.  Without arguments,\n\
prints baud شرح, سٽ discipline, and deviations from stty sane.  اندريون_2\n\
settings, CHAR is ورتل literally, or coded as اندريون ^c, 0x37, 0177 or\n\
127; special قدرون ^- or undef استعمال_ٿيل منزل disable special characters.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

ساكن فراغ
apply_settings (bool checking, حرف ثابت *device_name,
                حرف * ثابت *settings, صحيح n_settings,
                بنية termios *گهڻائي_2, bool *require_سيٽ_attr)
{
#وصف_ڪريو check_دليل(arg)						\
  افعل									\
    {									\
      إذا (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  طالما (0)

  لكل (صحيح k = 1; k < n_settings; k++)
    {
      حرف ثابت *arg = settings[k];
      bool ميلاپ_مليو = false;
      bool not_سيٽ_attr = false;
      bool reversed = false;

      إذا (! arg)
        استمر;

      إذا (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      إذا (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          استمر;
        }
      لكل (صحيح i = 0; گهڻائي_info[i].name != عدم; ++i)
        {
          إذا (streq (arg, گهڻائي_info[i].name))
            {
              إذا ((گهڻائي_info[i].flags & NO_SETATTR) == 0)
                {
                  ميلاپ_مليو = سيٽ_گهڻائي (&گهڻائي_info[i], reversed, گهڻائي_2);
                  *require_سيٽ_attr = true;
                }
              وإلا
                ميلاپ_مليو = not_سيٽ_attr = true;
              اكسر;
            }
        }
      إذا (!ميلاپ_مليو && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      إذا (!ميلاپ_مليو)
        {
          لكل (صحيح i = 0; control_info[i].name != عدم; ++i)
            {
              إذا (streq (arg, control_info[i].name))
                {
                  check_دليل (arg);
                  ميلاپ_مليو = true;
                  ++k;
                  سيٽ_control_char (&control_info[i], settings[k], گهڻائي_2);
                  *require_سيٽ_attr = true;
                  اكسر;
                }
            }
        }
      إذا (!ميلاپ_مليو || not_سيٽ_attr)
        {
          إذا (streq (arg, "ispeed"))
            {
              check_دليل (arg);
              ++k;
              بنية رفتار_desc ثابت ispeed = string_منزل_baud (settings[k]);
              إذا (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              سيٽ_رفتار (داخلا_رفتار, settings[k], ispeed, گهڻائي_2);
              إذا (checking)
                استمر;
              *require_سيٽ_attr = true;
            }
          وإلا إذا (streq (arg, "ospeed"))
            {
              check_دليل (arg);
              ++k;
              بنية رفتار_desc ثابت ospeed = string_منزل_baud (settings[k]);
              إذا (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              سيٽ_رفتار (output_رفتار, settings[k], ospeed, گهڻائي_2);
              إذا (checking)
                استمر;
              *require_سيٽ_attr = true;
            }
#جيڪڏهن_وصف_ٿيل TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          وإلا إذا (streq (arg, "extproc"))
            {
              صحيح val = ! reversed;

              إذا (checking)
                استمر;

              إذا (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل TIOCGWINSZ
          وإلا إذا (streq (arg, "rows"))
            {
              check_دليل (arg);
              ++k;
              إذا (checking)
                استمر;
              سيٽ_دري_سائيز (integer_arg (settings[k], وڌ_کان_وڌ_عدد), -1,
                               device_name);
            }
          وإلا إذا (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_دليل (arg);
              ++k;
              إذا (checking)
                استمر;
              سيٽ_دري_سائيز (-1, integer_arg (settings[k], وڌ_کان_وڌ_عدد),
                               device_name);
            }
          وإلا إذا (streq (arg, "size"))
            {
              إذا (checking)
                استمر;
              max_کانو = screen_columns ();
              current_کانو = 0;
              display_دري_سائيز (false, device_name);
            }
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_C_سٽ
          وإلا إذا (streq (arg, "line"))
            {
              check_دليل (arg);
              ++k;
              uintmax_t قدر = integer_arg (settings[k], UINTMAX_MAX);
              إذا (ckd_جوڙيو (&گهڻائي_2->c_سٽ, قدر, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_سيٽ_attr = true;
            }
#جيڪڏهن_پورو
          وإلا إذا (streq (arg, "speed"))
            {
              إذا (checking)
                استمر;
              max_کانو = screen_columns ();
              display_رفتار (گهڻائي_2, false);
            }
          وإلا
            {
              بنية رفتار_desc ثابت رفتار = string_منزل_baud (arg);
              إذا (رفتار.valid)
                {
                  سيٽ_رفتار (both_speeds, arg, رفتار, گهڻائي_2);
                  إذا (checking)
                    استمر;
                  *require_سيٽ_attr = true;
                }
              وإلا
                {
                  إذا (! recover_گهڻائي (arg, گهڻائي_2))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_سيٽ_attr = true;
                }
            }
        }
    }

  إذا (checking)
    check_رفتار (گهڻائي_2);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ساكن بنية termios گهڻائي_2;

  تعداد output_type output_type;
  صحيح optc;
  صحيح argi = 0;
  صحيح opti = 1;
  bool require_سيٽ_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  حرف *file_name = عدم;
  حرف ثابت *device_name;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = بدليو;
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

  طالما ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, عدم))
         != -1)
    {
      بدل (optc)
        {
        حالة 'a':
          verbose_output = true;
          output_type = all;
          اكسر;

        حالة 'g':
          recoverable_output = true;
          output_type = recoverable;
          اكسر;

        حالة 'F':
          إذا (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          اكسر;

        حالة DEV_DEBUG_OPTION:
          dev_debug = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          إذا (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          اكسر;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      طالما (opti < optind)
        argv[argi + opti++] = عدم;
    }

  /* Specifying both -a and -g gets an error.  */
  إذا (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  إذا (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  إذا (!noargs && !verbose_output && !recoverable_output)
    {
      ساكن بنية termios check_گهڻائي;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_گهڻائي, &require_سيٽ_attr);
    }

  إذا (file_name)
    {
      صحيح fdflags;
      إذا (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      إذا ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  إذا (tcgetattr (STDIN_FILENO, &گهڻائي_2))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  إذا (verbose_output || recoverable_output || noargs)
    {
      max_کانو = screen_columns ();
      current_کانو = 0;
      display_settings (output_type, &گهڻائي_2, device_name);
      أرجع EXIT_SUCCESS;
    }

  require_سيٽ_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &گهڻائي_2, &require_سيٽ_attr);

  إذا (require_سيٽ_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ساكن بنية termios new_گهڻائي;

      إذا (tcsetattr (STDIN_FILENO, tcsetattr_options, &گهڻائي_2))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      إذا (tcgetattr (STDIN_FILENO, &new_گهڻائي))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      إذا (! eq_گهڻائي (&گهڻائي_2, &new_گهڻائي))
        {
          إذا (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              لكل (بنانشان صحيح i = 0; i < حجم (new_گهڻائي); i++)
                {
                  بنانشان صحيح newc = *(((بنانشان حرف *) &new_گهڻائي) + i);
                  بنانشان صحيح oldc = *(((بنانشان حرف *) &گهڻائي_2) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  أرجع EXIT_SUCCESS;
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
ساكن bool
eq_گهڻائي (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  إذا (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#جيڪڏهن_وصف_ٿيل HAVE_C_سٽ
      || mode1->c_سٽ != mode2->c_سٽ
#جيڪڏهن_پورو
      || !memeq (mode1->c_cc, mode2->c_cc, حجم (mode1->c_cc)))
    أرجع false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_نقاب = ~(tcflag_t) 0;
#جيڪڏهن_وصف_ٿيل CBAUD
  cflag_نقاب &= ~(tcflag_t) CBAUD;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CIBAUD
  cflag_نقاب &= ~(tcflag_t) CIBAUD;
#جيڪڏهن_پورو

  إذا ((mode1->c_cflag ^ mode2->c_cflag) & cflag_نقاب)
    أرجع false;

  أرجع رفتار_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ساكن bool
سيٽ_گهڻائي (بنية گهڻائي_info ثابت *info, bool reversed, بنية termios *گهڻائي_2)
{
  tcflag_t *bitsp;

  إذا (reversed && (info->flags & REV) == 0)
    أرجع false;

  bitsp = گهڻائي_type_flag (info->type, گهڻائي_2);

  إذا (bitsp == عدم)
    {
      /* Combination mode. */
      إذا (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          إذا (reversed)
            گهڻائي_2->c_cflag = (گهڻائي_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            گهڻائي_2->c_cflag = (گهڻائي_2->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      وإلا إذا (streq (info->name, "oddp"))
        {
          إذا (reversed)
            گهڻائي_2->c_cflag = (گهڻائي_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            گهڻائي_2->c_cflag = (گهڻائي_2->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      وإلا إذا (streq (info->name, "nl"))
        {
          إذا (reversed)
            {
              گهڻائي_2->c_iflag = (گهڻائي_2->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              گهڻائي_2->c_oflag = (گهڻائي_2->c_oflag
#جيڪڏهن_وصف_ٿيل ONLCR
                               | ONLCR
#جيڪڏهن_پورو
                )
#جيڪڏهن_وصف_ٿيل OCRNL
                & ~OCRNL
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ONLRET
                & ~ONLRET
#جيڪڏهن_پورو
                ;
            }
          وإلا
            {
              گهڻائي_2->c_iflag = گهڻائي_2->c_iflag & ~ICRNL;
#جيڪڏهن_وصف_ٿيل ONLCR
              گهڻائي_2->c_oflag = گهڻائي_2->c_oflag & ~ONLCR;
#جيڪڏهن_پورو
            }
        }
      وإلا إذا (streq (info->name, "ek"))
        {
          گهڻائي_2->c_cc[VERASE] = CERASE;
          گهڻائي_2->c_cc[VKILL] = CKILL;
        }
      وإلا إذا (streq (info->name, "sane"))
        sane_گهڻائي (گهڻائي_2);
      وإلا إذا (streq (info->name, "cbreak"))
        {
          إذا (reversed)
            گهڻائي_2->c_lflag |= ICANON;
          وإلا
            گهڻائي_2->c_lflag &= ~ICANON;
        }
      وإلا إذا (streq (info->name, "pass8"))
        {
          إذا (reversed)
            {
              گهڻائي_2->c_cflag = (گهڻائي_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              گهڻائي_2->c_iflag |= ISTRIP;
            }
          وإلا
            {
              گهڻائي_2->c_cflag = (گهڻائي_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              گهڻائي_2->c_iflag &= ~ISTRIP;
            }
        }
      وإلا إذا (streq (info->name, "litout"))
        {
          إذا (reversed)
            {
              گهڻائي_2->c_cflag = (گهڻائي_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              گهڻائي_2->c_iflag |= ISTRIP;
              گهڻائي_2->c_oflag |= OPOST;
            }
          وإلا
            {
              گهڻائي_2->c_cflag = (گهڻائي_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              گهڻائي_2->c_iflag &= ~ISTRIP;
              گهڻائي_2->c_oflag &= ~OPOST;
            }
        }
      وإلا إذا (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          إذا ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              گهڻائي_2->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              گهڻائي_2->c_oflag |= OPOST;
              گهڻائي_2->c_lflag |= ISIG | ICANON;
#إذا VMIN == VEOF
              گهڻائي_2->c_cc[VEOF] = CEOF;
#جيڪڏهن_پورو
#إذا VTIME == VEOL
              گهڻائي_2->c_cc[VEOL] = CEOL;
#جيڪڏهن_پورو
            }
          وإلا
            {
              /* Raw mode. */
              گهڻائي_2->c_iflag = 0;
              گهڻائي_2->c_oflag &= ~OPOST;
              گهڻائي_2->c_lflag &= ~(ISIG | ICANON
#جيڪڏهن_وصف_ٿيل XCASE
                                 | XCASE
#جيڪڏهن_پورو
                );
              گهڻائي_2->c_cc[VMIN] = 1;
              گهڻائي_2->c_cc[VTIME] = 0;
            }
        }
#جيڪڏهن_وصف_ٿيل IXANY
      وإلا إذا (streq (info->name, "decctlq"))
        {
          إذا (reversed)
            گهڻائي_2->c_iflag |= IXANY;
          وإلا
            گهڻائي_2->c_iflag &= ~IXANY;
        }
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل TABDLY
      وإلا إذا (streq (info->name, "tabs"))
        {
          إذا (reversed)
            گهڻائي_2->c_oflag = (گهڻائي_2->c_oflag & ~TABDLY) | TAB3;
          وإلا
            گهڻائي_2->c_oflag = (گهڻائي_2->c_oflag & ~TABDLY) | TAB0;
        }
#وإلا
# ifdef OXTABS
      وإلا إذا (streq (info->name, "tabs"))
        {
          إذا (reversed)
            گهڻائي_2->c_oflag = گهڻائي_2->c_oflag | OXTABS;
          وإلا
            گهڻائي_2->c_oflag = گهڻائي_2->c_oflag & ~OXTABS;
        }
# endif
#جيڪڏهن_پورو
#إذا defined XCASE && defined IUCLC && defined OLCUC
      وإلا إذا (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          إذا (reversed)
            {
              گهڻائي_2->c_lflag &= ~XCASE;
              گهڻائي_2->c_iflag &= ~IUCLC;
              گهڻائي_2->c_oflag &= ~OLCUC;
            }
          وإلا
            {
              گهڻائي_2->c_lflag |= XCASE;
              گهڻائي_2->c_iflag |= IUCLC;
              گهڻائي_2->c_oflag |= OLCUC;
            }
        }
#جيڪڏهن_پورو
      وإلا إذا (streq (info->name, "crt"))
        گهڻائي_2->c_lflag |= ECHOE
#جيڪڏهن_وصف_ٿيل ECHOCTL
          | ECHOCTL
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOKE
          | ECHOKE
#جيڪڏهن_پورو
          ;
      وإلا إذا (streq (info->name, "dec"))
        {
          گهڻائي_2->c_cc[VINTR] = 3;	/* ^C */
          گهڻائي_2->c_cc[VERASE] = 127;	/* DEL */
          گهڻائي_2->c_cc[VKILL] = 21;	/* ^U */
          گهڻائي_2->c_lflag |= ECHOE
#جيڪڏهن_وصف_ٿيل ECHOCTL
            | ECHOCTL
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل ECHOKE
            | ECHOKE
#جيڪڏهن_پورو
            ;
#جيڪڏهن_وصف_ٿيل IXANY
          گهڻائي_2->c_iflag &= ~IXANY;
#جيڪڏهن_پورو
        }
    }
  وإلا إذا (reversed)
    *bitsp = *bitsp & ~info->نقاب & ~info->بٽ_2;
  وإلا
    *bitsp = (*bitsp & ~info->نقاب) | info->بٽ_2;

  أرجع true;
}

ساكن فراغ
سيٽ_control_char (بنية control_info ثابت *info, حرف ثابت *arg,
                  بنية termios *گهڻائي_2)
{
  بنانشان ڊگهو صحيح قدر;

  إذا (streq (info->name, "min") || streq (info->name, "time"))
    قدر = integer_arg (arg, TYPE_وڌ_کان_وڌ (cc_t));
  وإلا إذا (arg[0] == '\0' || arg[1] == '\0')
    قدر = منزل_uchar (arg[0]);
  وإلا إذا (streq (arg, "^-") || streq (arg, "undef"))
    قدر = _POSIX_VDISABLE;
  وإلا إذا (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      إذا (arg[1] == '?')
        قدر = 127;
      وإلا
        قدر = منزل_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  وإلا
    قدر = integer_arg (arg, TYPE_وڌ_کان_وڌ (cc_t));
  گهڻائي_2->c_cc[info->وٿي] = قدر;
}

ساكن فراغ
سيٽ_رفتار (تعداد رفتار_setting type, حرف ثابت *arg,
           بنية رفتار_desc ثابت رفتار, بنية termios *گهڻائي_2)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (رفتار.valid);

  إذا (type == داخلا_رفتار || type == both_speeds)
    {
      آخري_ibaud = رفتار;
      إذا (cfsetispeed (گهڻائي_2, رفتار.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  إذا (type == output_رفتار || type == both_speeds)
    {
      آخري_obaud = رفتار;
      إذا (cfsetospeed (گهڻائي_2, رفتار.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#جيڪڏهن_وصف_ٿيل TIOCGWINSZ

ساكن صحيح
get_win_سائيز (صحيح fd, بنية winsize *win)
{
  صحيح err = ioctl (fd, TIOCGWINSZ, (حرف *) win);
  أرجع err;
}

ساكن فراغ
سيٽ_دري_سائيز (صحيح صفون, صحيح کانن_ڳڻپ_2, حرف ثابت *device_name)
{
  بنية winsize win;

  إذا (get_win_سائيز (STDIN_FILENO, &win))
    {
      إذا (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, حجم (win));
    }

  إذا (صفون >= 0)
    win.ws_صف = صفون;
  إذا (کانن_ڳڻپ_2 >= 0)
    win.ws_کانو = کانن_ڳڻپ_2;

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

  إذا (win.ws_صف == 0 || win.ws_کانو == 0)
    {
      بنية ttysize ttysz;

      ttysz.ts_سٽون = win.ws_صف;
      ttysz.ts_کانن_ڳڻپ = win.ws_کانو;

      win.ws_صف = 1;
      win.ws_کانو = 1;

      إذا (ioctl (STDIN_FILENO, TIOCSWINSZ, (حرف *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      إذا (ioctl (STDIN_FILENO, TIOCSSIZE, (حرف *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      أرجع;
    }
# endif

  إذا (ioctl (STDIN_FILENO, TIOCSWINSZ, (حرف *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

ساكن فراغ
display_دري_سائيز (bool fancy, حرف ثابت *device_name)
{
  بنية winsize win;

  إذا (get_win_سائيز (STDIN_FILENO, &win))
    {
      إذا (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      إذا (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  وإلا
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_صف, win.ws_کانو);
      إذا (!fancy)
        current_کانو = 0;
    }
}
#جيڪڏهن_پورو

ساكن صحيح
screen_columns (فراغ)
{
#جيڪڏهن_وصف_ٿيل TIOCGWINSZ
  بنية winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  إذا (get_win_سائيز (STDOUT_FILENO, &win) == 0 && 0 < win.ws_کانو)
    أرجع win.ws_کانو;
#جيڪڏهن_پورو
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    حرف *کانو_string = getenv ("COLUMNS");
    ڊگهو صحيح n_columns;
    إذا (!(کانو_string != عدم
          && xstrtol (کانو_string, عدم, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= وڌ_کان_وڌ_عدد))
      n_columns = 80;
    أرجع n_columns;
  }
}

ATTRIBUTE_خالص
ساكن tcflag_t *
گهڻائي_type_flag (تعداد گهڻائي_type type, بنية termios *گهڻائي_2)
{
  بدل (type)
    {
    حالة control:
      أرجع &گهڻائي_2->c_cflag;

    حالة داخلا:
      أرجع &گهڻائي_2->c_iflag;

    حالة output:
      أرجع &گهڻائي_2->c_oflag;

    حالة local:
      أرجع &گهڻائي_2->c_lflag;

    حالة combination:
      أرجع عدم;

    افتراضي:
      unreachable ();
    }
}

ساكن فراغ
display_settings (تعداد output_type output_type, بنية termios *گهڻائي_2,
                  حرف ثابت *device_name)
{
  بدل (output_type)
    {
    حالة بدليو:
      display_بدليو (گهڻائي_2);
      اكسر;

    حالة all:
      display_all (گهڻائي_2, device_name);
      اكسر;

    حالة recoverable:
      display_recoverable (گهڻائي_2);
      اكسر;
    }
}

ساكن فراغ
display_بدليو (بنية termios *گهڻائي_2)
{
  bool خالي_جايون_سٽ;
  tcflag_t *bitsp;
  بنانشان ڊگهو نقاب;
  تعداد گهڻائي_type prev_type = control;

  display_رفتار (گهڻائي_2, true);
#جيڪڏهن_وصف_ٿيل HAVE_C_سٽ
  wrapf ("line = %d;", گهڻائي_2->c_سٽ);
#جيڪڏهن_پورو
  اکر_لکو ('\n');
  current_کانو = 0;

  خالي_جايون_سٽ = true;
  لكل (صحيح i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      إذا (گهڻائي_2->c_cc[control_info[i].وٿي] == control_info[i].saneval)
        استمر;

#جيڪڏهن_وصف_ٿيل VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      إذا (streq (control_info[i].name, "flush"))
        استمر;
#جيڪڏهن_پورو
      /* If swtch is the same as susp, don't print both.  */
#إذا VSWTCH == VSUSP
      إذا (streq (control_info[i].name, "swtch"))
        استمر;
#جيڪڏهن_پورو
      /* If eof uses the same slot as min, only print whichever applies.  */
#إذا VEOF == VMIN
      إذا ((گهڻائي_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#جيڪڏهن_پورو

      خالي_جايون_سٽ = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (گهڻائي_2->c_cc[control_info[i].وٿي]));
    }
  إذا ((گهڻائي_2->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (بنانشان ڊگهو صحيح) گهڻائي_2->c_cc[VMIN],
             (بنانشان ڊگهو صحيح) گهڻائي_2->c_cc[VTIME]);
    }
  وإلا إذا (!خالي_جايون_سٽ)
    اکر_لکو ('\n');
  current_کانو = 0;

  خالي_جايون_سٽ = true;
  لكل (صحيح i = 0; گهڻائي_info[i].name != عدم; ++i)
    {
      إذا (گهڻائي_info[i].flags & OMIT)
        استمر;
      إذا (گهڻائي_info[i].type != prev_type)
        {
          إذا (!خالي_جايون_سٽ)
            {
              اکر_لکو ('\n');
              current_کانو = 0;
              خالي_جايون_سٽ = true;
            }
          prev_type = گهڻائي_info[i].type;
        }

      bitsp = گهڻائي_type_flag (گهڻائي_info[i].type, گهڻائي_2);
      نقاب = گهڻائي_info[i].نقاب ? گهڻائي_info[i].نقاب : گهڻائي_info[i].بٽ_2;
      إذا ((*bitsp & نقاب) == گهڻائي_info[i].بٽ_2)
        {
          إذا (گهڻائي_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", گهڻائي_info[i].name);
              خالي_جايون_سٽ = false;
            }
        }
      وإلا إذا ((گهڻائي_info[i].flags & (SANE_سيٽ | REV)) == (SANE_سيٽ | REV))
        {
          wrapf ("-%s", گهڻائي_info[i].name);
          خالي_جايون_سٽ = false;
        }
    }
  إذا (!خالي_جايون_سٽ)
    اکر_لکو ('\n');
  current_کانو = 0;
}

ساكن فراغ
display_all (بنية termios *گهڻائي_2, حرف ثابت *device_name)
{
  tcflag_t *bitsp;
  بنانشان ڊگهو نقاب;
  تعداد گهڻائي_type prev_type = control;

  display_رفتار (گهڻائي_2, true);
#جيڪڏهن_وصف_ٿيل TIOCGWINSZ
  display_دري_سائيز (true, device_name);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_C_سٽ
  wrapf ("line = %d;", گهڻائي_2->c_سٽ);
#جيڪڏهن_پورو
  اکر_لکو ('\n');
  current_کانو = 0;

  لكل (صحيح i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#جيڪڏهن_وصف_ٿيل VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      إذا (streq (control_info[i].name, "flush"))
        استمر;
#جيڪڏهن_پورو
      /* If swtch is the same as susp, don't print both.  */
#إذا VSWTCH == VSUSP
      إذا (streq (control_info[i].name, "swtch"))
        استمر;
#جيڪڏهن_پورو
      /* If eof uses the same slot as min, only print whichever applies.  */
#إذا VEOF == VMIN
      إذا ((گهڻائي_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#جيڪڏهن_پورو
      wrapf ("%s = %s;", control_info[i].name,
             visible (گهڻائي_2->c_cc[control_info[i].وٿي]));
    }
#إذا VEOF == VMIN
  إذا ((گهڻائي_2->c_lflag & ICANON) == 0)
#جيڪڏهن_پورو
    wrapf ("min = %lu; time = %lu;",
           (بنانشان ڊگهو صحيح) گهڻائي_2->c_cc[VMIN],
           (بنانشان ڊگهو صحيح) گهڻائي_2->c_cc[VTIME]);
  إذا (current_کانو != 0)
    اکر_لکو ('\n');
  current_کانو = 0;

  لكل (صحيح i = 0; گهڻائي_info[i].name != عدم; ++i)
    {
      إذا (گهڻائي_info[i].flags & OMIT)
        استمر;
      إذا (گهڻائي_info[i].type != prev_type)
        {
          اکر_لکو ('\n');
          current_کانو = 0;
          prev_type = گهڻائي_info[i].type;
        }

      bitsp = گهڻائي_type_flag (گهڻائي_info[i].type, گهڻائي_2);
      نقاب = گهڻائي_info[i].نقاب ? گهڻائي_info[i].نقاب : گهڻائي_info[i].بٽ_2;
      إذا ((*bitsp & نقاب) == گهڻائي_info[i].بٽ_2)
        wrapf ("%s", گهڻائي_info[i].name);
      وإلا إذا (گهڻائي_info[i].flags & REV)
        wrapf ("-%s", گهڻائي_info[i].name);
    }
  اکر_لکو ('\n');
  current_کانو = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ساكن فراغ
check_رفتار (بنية termios *گهڻائي_2)
{
  إذا (آخري_ibaud.valid && آخري_obaud.valid)
    {
      إذا (cfgetispeed (گهڻائي_2) != آخري_ibaud.baud
          || cfgetospeed (گهڻائي_2) != آخري_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_منزل_قدر (آخري_ibaud.baud),
               baud_منزل_قدر (آخري_obaud.baud));
    }
}

ساكن فراغ
display_رفتار (بنية termios *گهڻائي_2, bool fancy)
{
  إذا (cfgetispeed (گهڻائي_2) == 0 || cfgetispeed (گهڻائي_2) == cfgetospeed (گهڻائي_2))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_منزل_قدر (cfgetospeed (گهڻائي_2)));
  وإلا
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_منزل_قدر (cfgetispeed (گهڻائي_2)),
           baud_منزل_قدر (cfgetospeed (گهڻائي_2)));
  إذا (!fancy)
    current_کانو = 0;
}

ساكن فراغ
display_recoverable (بنية termios *گهڻائي_2)
{
  لکو ("%lx:%lx:%lx:%lx",
          (بنانشان ڊگهو صحيح) گهڻائي_2->c_iflag,
          (بنانشان ڊگهو صحيح) گهڻائي_2->c_oflag,
          (بنانشان ڊگهو صحيح) گهڻائي_2->c_cflag,
          (بنانشان ڊگهو صحيح) گهڻائي_2->c_lflag);
  لكل (size_t i = 0; i < NCCS; ++i)
    لکو (":%lx", (بنانشان ڊگهو صحيح) گهڻائي_2->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#جيڪڏهن_وصف_ٿيل CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (گهڻائي_2->c_cflag & CBAUD) == BOTHER;
#  إذا defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((گهڻائي_2->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#جيڪڏهن_پورو
  إذا (separate_speeds)
    لکو (":%lx:%lx", (بنانشان ڊگهو صحيح) cfgetispeed (گهڻائي_2),
            (بنانشان ڊگهو صحيح) cfgetospeed (گهڻائي_2));

  اکر_لکو ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#وصف_ڪريو STRTOUL_TYPE(TYPE)                                               \
  ساكن صحيح                                                             \
  strtoul_##TYPE (حرف ثابت *s, صحيح بنياد_2, حرف **p, TYPE *نتيجو,       \
                  حرف delim)                                            \
  {                                                                      \
    بنانشان ڊگهو ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, بنياد_2);                                           \
    إذا (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      أرجع -1;                                                         \
    *نتيجو = ul;                                                        \
    أرجع 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (رفتار_t)
#وصف_رد STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#ضمن "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ساكن bool
recover_گهڻائي (حرف ثابت *arg, بنية termios *گهڻائي_2)
{
  tcflag_t flag[4];
  حرف ثابت *s = arg;
  لكل (size_t i = 0; i < 4; i++)
    {
      حرف *p;
      إذا (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        أرجع false;
      s = p + 1;
    }
  گهڻائي_2->c_iflag = flag[0];
  گهڻائي_2->c_oflag = flag[1];
  گهڻائي_2->c_cflag = flag[2];
  گهڻائي_2->c_lflag = flag[3];

  لكل (size_t i = 0; i < NCCS; ++i)
    {
      حرف *p;
      حرف delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      إذا (strtoul_cc_t (s, 16, &p, گهڻائي_2->c_cc + i, delim) != 0)
        أرجع false;
      /* Accept the old format, which has no separate speed fields.  */
      إذا (! *p)
        {
#جيڪڏهن_وصف_ٿيل CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_رفتار_T_SANE
          بنية رفتار_desc ospeed = cbaud_منزل_رفتار (گهڻائي_2->c_cflag & CBAUD);
          إذا (ospeed.valid && cfsetospeed (گهڻائي_2, ospeed.baud) != 0)
            أرجع false;
#  إذا defined CIBAUD && defined IBSHIFT
          بنية رفتار_desc ispeed =
            cbaud_منزل_رفتار ((گهڻائي_2->c_cflag & CIBAUD) >> IBSHIFT);
          إذا (ispeed.valid && cfsetispeed (گهڻائي_2, ispeed.baud) != 0)
            أرجع false;
#  endif
# endif
          آخري_ibaud.baud = cfgetispeed (گهڻائي_2);
          آخري_obaud.baud = cfgetospeed (گهڻائي_2);
#جيڪڏهن_پورو
          أرجع true;
        }
      s = p + 1;
    }

  رفتار_t رفتار[2];
  لكل (size_t i = 0; i < 2; i++)
    {
      حرف *p;
      حرف delim = i == 0 ? ':' : '\0';
      إذا (strtoul_رفتار_t (s, 16, &p, رفتار + i, delim) != 0)
        أرجع false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  إذا (cfsetospeed (گهڻائي_2, رفتار[1]) != 0
      || cfsetispeed (گهڻائي_2, رفتار[0]) != 0)
    أرجع false;

  آخري_ibaud.baud = رفتار[0] == 0 ? cfgetispeed (گهڻائي_2) : رفتار[0];
  آخري_obaud.baud = رفتار[1];
  أرجع true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ساكن bool
رفتار_matches (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  رفتار_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  رفتار_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  إذا (speed1[1] == 0)
    speed1[1] = speed1[0];
  إذا (speed2[1] == 0)
    speed2[1] = speed2[0];

  بنانشان صحيح بٽ_2 = 2;  /* Start and stop bits.  */
  بٽ_2 += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  بٽ_2 += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  بدل (mode1->c_cflag & CSIZE)
    {
      حالة CS5: بٽ_2 += 5; اكسر;
      حالة CS6: بٽ_2 += 6; اكسر;
      حالة CS7: بٽ_2 += 7; اكسر;
      افتراضي: بٽ_2 += 8; اكسر;
    }

  لكل (صحيح i = 0; i < 2; i++)
    {
      بنانشان ڊگهو صحيح value1;
      بنانشان ڊگهو صحيح value2;
      بنانشان ڊگهو صحيح delta;

      إذا (speed1[i] == speed2[i])
        استمر;

      value1 = baud_منزل_قدر (speed1[i]);
      value2 = baud_منزل_قدر (speed2[i]);

#جيڪڏهن_وصف_نه_ٿيل TERMIOS_رفتار_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      إذا (value1 == ULONG_MAX || value2 == ULONG_MAX)
        أرجع false;
#جيڪڏهن_پورو

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      إذا (delta > value1 / (بٽ_2 * 4))
        أرجع false;
    }

  أرجع true;
}

ATTRIBUTE_خالص
ساكن بنية رفتار_desc
string_منزل_baud (حرف ثابت *arg)
{
  حرف *ep;
  بنانشان ڊگهو قدر;
  بنانشان حرف c;

  /* Explicitly disallow negative numbers.  */
  طالما (c_isspace (*arg))
    arg++;
  إذا (*arg == '-')
    اذهب invalid;

  /* Empty does not mean 0.  */
  إذا (! *arg)
    اذهب invalid;

  قدر = strtoul (arg, &ep, 10);

  c = *ep++;
  إذا (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      إذا (c)
        {
          بنانشان حرف d = c - '0';
          إذا (d > 5)
            قدر++;
          وإلا إذا (d == 5)
            {
              طالما ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              إذا (c)
                قدر++;                /* Nonzero, round up */
              وإلا
                قدر += (قدر & 1);   /* Exactly in the middle, round even */
            }

          طالما (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          إذا (c)
            اذهب invalid; /* Garbage after otherwise valid number */
        }
    }
  وإلا إذا (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      إذا (streq (arg, "exta"))
        أرجع (بنية رفتار_desc) { .baud = B19200, .valid = true };
      وإلا إذا (streq (arg, "extb"))
        أرجع (بنية رفتار_desc) { .baud = B38400, .valid = true };
      وإلا
        اذهب invalid;
    }

  أرجع قدر_منزل_baud (قدر);

 invalid:
  أرجع (بنية رفتار_desc) { .baud = (رفتار_t) -1, .valid = false };
}

ساكن فراغ
sane_گهڻائي (بنية termios *گهڻائي_2)
{
  tcflag_t *bitsp;

  لكل (صحيح i = 0; control_info[i].name; ++i)
    {
#إذا VMIN == VEOF
      إذا (streq (control_info[i].name, "min"))
        اكسر;
#جيڪڏهن_پورو
      گهڻائي_2->c_cc[control_info[i].وٿي] = control_info[i].saneval;
    }

  لكل (صحيح i = 0; گهڻائي_info[i].name != عدم; ++i)
    {
      إذا (گهڻائي_info[i].flags & NO_SETATTR)
        استمر;

      إذا (گهڻائي_info[i].flags & SANE_سيٽ)
        {
          bitsp = گهڻائي_type_flag (گهڻائي_info[i].type, گهڻائي_2);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~گهڻائي_info[i].نقاب) | گهڻائي_info[i].بٽ_2;
        }
      وإلا إذا (گهڻائي_info[i].flags & SANE_UNSET)
        {
          bitsp = گهڻائي_type_flag (گهڻائي_info[i].type, گهڻائي_2);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~گهڻائي_info[i].نقاب & ~گهڻائي_info[i].بٽ_2;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

ساكن حرف ثابت *
visible (cc_t ch)
{
  ساكن حرف buf[10];
  حرف *bpout = buf;

  إذا (ch == _POSIX_VDISABLE)
    أرجع "<undef>";

  إذا (ch >= 32)
    {
      إذا (ch < 127)
        *bpout++ = ch;
      وإلا إذا (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      وإلا
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          إذا (ch >= 128 + 32)
            {
              إذا (ch < 128 + 127)
                *bpout++ = ch - 128;
              وإلا
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          وإلا
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  وإلا
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  أرجع (حرف ثابت *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

ساكن uintmax_t
integer_arg (حرف ثابت *s, uintmax_t maxval)
{
  أرجع xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
