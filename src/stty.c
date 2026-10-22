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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TERMIOS_NEEDS_XOPEN_ਸਰੋਤ
# define XOPEN_ਸਰੋਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <termios.h>
#ਜੇ HAVE_STROPTS_H
# include <stropts.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WINSIZE_ਵਿੱਚ_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GWINSZ_ਵਿੱਚ_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fd-reopen.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "stty"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CINTR
# define CINTR Control ('c')
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CQUIT
# define CQUIT 28
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CERASE
# define CERASE 127
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CKILL
# define CKILL Control ('u')
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CEOF
# define CEOF Control ('d')
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CEOL
# define CEOL _POSIX_VDISABLE
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CSTART
# define CSTART Control ('q')
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CSTOP
# define CSTOP Control ('s')
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CSUSP
# define CSUSP Control ('z')
#ਜੇ_ਅੰਤ
#ਜੇ defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#ਜੇ_ਅੰਤ
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#ਜੇ defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#ਜੇ_ਅੰਤ
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#ਜੇ defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# ਜੇ defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#ਜੇ_ਅੰਤ
#ਜੇ defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#ਜੇ_ਅੰਤ

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#ਜੇ defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#ਜੇ_ਅੰਤ

#ਜੇ defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#ਜੇ_ਅੰਤ
#ਜੇ defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#ਜੇ_ਅੰਤ
#ਜੇ defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#ਜੇ_ਅੰਤ
#ਜੇ defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#ਜੇ_ਅੰਤ
#ਜੇ defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#ਜੇ_ਅੰਤ
#ਜੇ defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#ਜੇ_ਅੰਤ
#ਜੇ defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#ਜੇ_ਅੰਤ
#ਜੇ defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#ਜੇ_ਅੰਤ
#ਜੇ defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#ਜੇ_ਅੰਤ
#ਜੇ defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#ਜੇ_ਅੰਤ
#ਜੇ defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#ਜੇ_ਅੰਤ
#ਜੇ defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#ਜੇ_ਅੰਤ
#ਜੇ defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#ਜੇ_ਅੰਤ

/* Which speeds to set.  */
ਗਿਣਤੀ ਰਫਤਾਰ_setting
  {
    ਨਿਵੇਸ਼_ਰਫਤਾਰ, output_ਰਫਤਾਰ, both_speeds
  };

/* What to output and how.  */
ਗਿਣਤੀ output_type
  {
    ਬਦਲਿਆ, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
ਗਿਣਤੀ ਬਹੁਲਕ_type
  {
    control, ਨਿਵੇਸ਼, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SANE_ਸੈੱਟ 1		/* Set in 'sane' mode. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SANE_UNSET 2		/* Unset in 'sane' mode. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REV 4			/* Can be turned off by prepending '-'. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OMIT 8			/* Don't display value. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
ਬਣਤਰ ਬਹੁਲਕ_info
  {
    ਅੱਖਰ ਅਚਲ *name;		/* Name given on command line.  */
    ਗਿਣਤੀ ਬਹੁਲਕ_type type;	/* Which structure element to change. */
    ਅੱਖਰ flags;			/* Setting and display options.  */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਬਿੱਟ_2;		/* Bits to set for this mode.  */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨਕਾਬ;		/* Other bits to turn off for this mode.  */
  };

ਸਥਿਰ ਬਣਤਰ ਬਹੁਲਕ_info ਅਚਲ ਬਹੁਲਕ_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#ਜੇ_ਅੰਤ
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_ਸੈੱਟ | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#ਜੇ_ਅੰਤ

  {"ignbrk", ਨਿਵੇਸ਼, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", ਨਿਵੇਸ਼, SANE_ਸੈੱਟ | REV, BRKINT, 0},
  {"ignpar", ਨਿਵੇਸ਼, REV, IGNPAR, 0},
  {"parmrk", ਨਿਵੇਸ਼, REV, PARMRK, 0},
  {"inpck", ਨਿਵੇਸ਼, REV, INPCK, 0},
  {"istrip", ਨਿਵੇਸ਼, REV, ISTRIP, 0},
  {"inlcr", ਨਿਵੇਸ਼, SANE_UNSET | REV, INLCR, 0},
  {"igncr", ਨਿਵੇਸ਼, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", ਨਿਵੇਸ਼, SANE_ਸੈੱਟ | REV, ICRNL, 0},
  {"ixon", ਨਿਵੇਸ਼, REV, IXON, 0},
  {"ixoff", ਨਿਵੇਸ਼, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", ਨਿਵੇਸ਼, REV | OMIT, IXOFF, 0},
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IUCLC
  {"iuclc", ਨਿਵੇਸ਼, SANE_UNSET | REV, IUCLC, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IXANY
  {"ixany", ਨਿਵੇਸ਼, SANE_UNSET | REV, IXANY, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IMAXBEL
  {"imaxbel", ਨਿਵੇਸ਼, SANE_ਸੈੱਟ | REV, IMAXBEL, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IUTF8
  {"iutf8", ਨਿਵੇਸ਼, SANE_UNSET | REV, IUTF8, 0},
#ਜੇ_ਅੰਤ

  {"opost", output, SANE_ਸੈੱਟ | REV, OPOST, 0},
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLCR
  {"onlcr", output, SANE_ਸੈੱਟ | REV, ONLCR, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_ਸੈੱਟ, NL0, NLDLY},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_ਸੈੱਟ, CR0, CRDLY},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TABDLY
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
  {"tab0", output, SANE_ਸੈੱਟ, TAB0, TABDLY},
# endif
#ਨਹੀਂਤਾਂ
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_ਸੈੱਟ, BS0, BSDLY},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_ਸੈੱਟ, VT0, VTDLY},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_ਸੈੱਟ, FF0, FFDLY},
#ਜੇ_ਅੰਤ

  {"isig", local, SANE_ਸੈੱਟ | REV, ISIG, 0},
  {"icanon", local, SANE_ਸੈੱਟ | REV, ICANON, 0},
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IEXTEN
  {"iexten", local, SANE_ਸੈੱਟ | REV, IEXTEN, 0},
#ਜੇ_ਅੰਤ
  {"echo", local, SANE_ਸੈੱਟ | REV, ECHO, 0},
  {"echoe", local, SANE_ਸੈੱਟ | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_ਸੈੱਟ | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOCTL
  {"echoctl", local, SANE_ਸੈੱਟ | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOKE
  {"echoke", local, SANE_ਸੈੱਟ | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#ਜੇ_ਅੰਤ
#ਜੇ defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#ਜਾਂਜੇ defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#ਜੇ_ਅੰਤ

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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#ਜੇ_ਅੰਤ
#ਜੇ defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#ਜੇ_ਅੰਤ
#ਜੇ defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#ਜੇ_ਅੰਤ
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {ਨਿਰਾ, control, 0, 0, 0}
};

/* Control character settings.  */
ਬਣਤਰ control_info
  {
    ਅੱਖਰ ਅਚਲ *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t ਵਿੱਥ;		/* Offset in c_cc.  */
  };

/* Control characters. */

ਸਥਿਰ ਬਣਤਰ control_info ਅਚਲ control_info[] =
{
  {"intr", CINTR, VINTR},
  {"quit", CQUIT, VQUIT},
  {"erase", CERASE, VERASE},
  {"kill", CKILL, VKILL},
  {"eof", CEOF, VEOF},
  {"eol", CEOL, VEOL},
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VEOL2
  {"eol2", CEOL2, VEOL2},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#ਜੇ_ਅੰਤ
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#ਨਹੀਂਤਾਂ
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VWERASE
  {"werase", CWERASE, VWERASE},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VSTATUS
  {"status", CSTATUS, VSTATUS},
#ਜੇ_ਅੰਤ

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {ਨਿਰਾ, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
ਬਣਤਰ ਰਫਤਾਰ_desc
{
  /* The baud rate.  */
  ਰਫਤਾਰ_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *visible (cc_t ch);
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ baud_ਮੰਜ਼ਿਲ_ਮੁੱਲ (ਰਫਤਾਰ_t ਰਫਤਾਰ);
ਸਥਿਰ bool recover_ਬਹੁਲਕ (ਅੱਖਰ ਅਚਲ *arg, ਬਣਤਰ termios *ਬਹੁਲਕ_2);
ਸਥਿਰ ਪੂਰਨ screen_columns (ਖਾਲੀ);
ਸਥਿਰ bool ਸੈੱਟ_ਬਹੁਲਕ (ਬਣਤਰ ਬਹੁਲਕ_info ਅਚਲ *info, bool reversed,
                      ਬਣਤਰ termios *ਬਹੁਲਕ_2);
ਸਥਿਰ bool eq_ਬਹੁਲਕ (ਬਣਤਰ termios ਅਚਲ *mode1,
                     ਬਣਤਰ termios ਅਚਲ *mode2);
ਸਥਿਰ bool ਰਫਤਾਰ_matches (ਬਣਤਰ termios ਅਚਲ *mode1,
                           ਬਣਤਰ termios ਅਚਲ *mode2);
ਸਥਿਰ uintmax_t integer_arg (ਅੱਖਰ ਅਚਲ *s, uintmax_t max);
ਸਥਿਰ ਬਣਤਰ ਰਫਤਾਰ_desc string_ਮੰਜ਼ਿਲ_baud (ਅੱਖਰ ਅਚਲ *arg);
ਸਥਿਰ tcflag_t *ਬਹੁਲਕ_type_flag (ਗਿਣਤੀ ਬਹੁਲਕ_type type, ਬਣਤਰ termios *ਬਹੁਲਕ_2);
ਸਥਿਰ ਖਾਲੀ display_all (ਬਣਤਰ termios *ਬਹੁਲਕ_2, ਅੱਖਰ ਅਚਲ *device_name);
ਸਥਿਰ ਖਾਲੀ display_ਬਦਲਿਆ (ਬਣਤਰ termios *ਬਹੁਲਕ_2);
ਸਥਿਰ ਖਾਲੀ display_recoverable (ਬਣਤਰ termios *ਬਹੁਲਕ_2);
ਸਥਿਰ ਖਾਲੀ display_settings (ਗਿਣਤੀ output_type output_type,
                              ਬਣਤਰ termios *ਬਹੁਲਕ_2,
                              ਅੱਖਰ ਅਚਲ *device_name);
ਸਥਿਰ ਖਾਲੀ check_ਰਫਤਾਰ (ਬਣਤਰ termios *ਬਹੁਲਕ_2);
ਸਥਿਰ ਖਾਲੀ display_ਰਫਤਾਰ (ਬਣਤਰ termios *ਬਹੁਲਕ_2, bool fancy);
ਸਥਿਰ ਖਾਲੀ display_ਖਿੜਕੀ_ਮਾਪ (bool fancy, ਅੱਖਰ ਅਚਲ *device_name);
ਸਥਿਰ ਖਾਲੀ sane_ਬਹੁਲਕ (ਬਣਤਰ termios *ਬਹੁਲਕ_2);
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_control_char (ਬਣਤਰ control_info ਅਚਲ *info,
                              ਅੱਖਰ ਅਚਲ *arg,
                              ਬਣਤਰ termios *ਬਹੁਲਕ_2);
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਰਫਤਾਰ (ਗਿਣਤੀ ਰਫਤਾਰ_setting type, ਅੱਖਰ ਅਚਲ *arg,
                       ਬਣਤਰ ਰਫਤਾਰ_desc ਅਚਲ ਰਫਤਾਰ, ਬਣਤਰ termios *ਬਹੁਲਕ_2);
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਖਿੜਕੀ_ਮਾਪ (ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_2, ਅੱਖਰ ਅਚਲ *device_name);

/* The width of the screen, for output wrapping. */
ਸਥਿਰ ਪੂਰਨ max_ਖਾਨਾ;

/* Current position, to know when to wrap. */
ਸਥਿਰ ਪੂਰਨ current_ਖਾਨਾ;

/* Default "drain" mode for tcsetattr.  */
ਸਥਿਰ ਪੂਰਨ tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ਸਥਿਰ bool dev_debug;

/* Record last speed set for correlation.  */
ਸਥਿਰ ਬਣਤਰ ਰਫਤਾਰ_desc ਆਖਰੀ_ibaud =
  { .baud = (ਰਫਤਾਰ_t) -1, .valid = false };
ਸਥਿਰ ਬਣਤਰ ਰਫਤਾਰ_desc ਆਖਰੀ_obaud =
  { .baud = (ਰਫਤਾਰ_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"all", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"save", no_ਦਲੀਲ, ਨਿਰਾ, 'g'},
  {"file", required_ਦਲੀਲ, ਨਿਰਾ, 'F'},
  {"-debug", no_ਦਲੀਲ, ਨਿਰਾ, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((ਲਿਖੋ, 1, 2))
ਸਥਿਰ ਖਾਲੀ
wrapf (ਅੱਖਰ ਅਚਲ *ਸੁਨੇਹਾ_2,...)
{
  va_list ਦਲੀਲਾਂ;
  ਅੱਖਰ *buf;
  ਪੂਰਨ buflen;

  va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ, ਸੁਨੇਹਾ_2);
  buflen = vasprintf (&buf, ਸੁਨੇਹਾ_2, ਦਲੀਲਾਂ);
  va_ਸਿਰਾ (ਦਲੀਲਾਂ);

  ਜੇ (buflen < 0)
    xalloc_die ();

  ਜੇ (0 < current_ਖਾਨਾ)
    {
      ਜੇ (max_ਖਾਨਾ - current_ਖਾਨਾ <= buflen)
        {
          ਅੱਖਰ_ਲਿਖੋ ('\n');
          current_ਖਾਨਾ = 0;
        }
      ਨਹੀਂਤਾਂ
        {
          ਅੱਖਰ_ਲਿਖੋ (' ');
          current_ਖਾਨਾ++;
        }
    }

  fputs (buf, stdout);
  ਮੁਕਤ (buf);
  current_ਖਾਨਾ += buflen;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print or change terminal characteristics.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings ਵਿੱਚ human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings ਵਿੱਚ a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  open and use DEVICE instead ਦਾ standard ਨਿਵੇਸ਼\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system ਨਿਰਧਾਰਤ_ਕਰਦਾ which settings are ਉਪਲਬਧ.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding ਦਾ output\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop ਸੰਕੇਤ once ਨਿਵੇਸ਼ flushed\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   eof CHAR      CHAR will send an ਸਿਰਾ ਦਾ file (terminate the ਨਿਵੇਸ਼)\n\
   eol CHAR      CHAR will ਸਿਰਾ the ਸਤਰ\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR ਲਈ ending the ਸਤਰ\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   erase CHAR    CHAR will erase the ਆਖਰੀ character typed\n\
   intr CHAR     CHAR will send an ਰੁਕਾਵਟ ਸੰਕੇਤ\n\
   kill CHAR     CHAR will erase the current ਸਤਰ\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the ਅਗਲਾ character quoted\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VSTATUS
      fputs (_("\
 * ਸਥਿਤੀ CHAR   CHAR will send an info ਸੰਕੇਤ\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   quit CHAR     CHAR will send a quit ਸੰਕੇਤ\n\
"), stdout);
#ਜੇ defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current ਸਤਰ\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   ਸ਼ੁਰੂ CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop ਸੰਕੇਤ\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will ਚੋਣ ਮੰਜ਼ਿਲ a different shell layer\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the ਆਖਰੀ ਸ਼ਬਦ_2 typed\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
\n\
Special settings:\n\
   N             ਸੈੱਟ_2 the ਨਿਵੇਸ਼ and output speeds ਮੰਜ਼ਿਲ N bauds\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TIOCGWINSZ
      fputs (_("\
   ਖਾਨੇ_ਗਿਣਤੀ_2 N        tell the kernel that the terminal has N columns\n\
 * columns N     same as ਖਾਨੇ_ਗਿਣਤੀ_2 N\n\
"), stdout);
#ਜੇ_ਅੰਤ
      ਲਿਖੋ (_("\
 * [-]drain      wait ਲਈ ਸੰਚਾਰ before applying settings (%s by ਮੂਲ)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      ਸੈੱਟ_2 the ਨਿਵੇਸ਼ ਰਫਤਾਰ ਮੰਜ਼ਿਲ N\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_C_ਸਤਰ
      fputs (_("\
 * ਸਤਰ N        use ਸਤਰ discipline N\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   min N         with -icanon, ਸੈੱਟ_2 N characters minimum ਲਈ a completed ਪਠ\n\
   ospeed N      ਸੈੱਟ_2 the output ਰਫਤਾਰ ਮੰਜ਼ਿਲ N\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TIOCGWINSZ
      fputs (_("\
   ਪੰਕਤੀਆਂ N        tell the kernel that the terminal has N ਪੰਕਤੀਆਂ\n\
   ਮਾਪ          print the ਸੰਖਿਆ ਦਾ ਪੰਕਤੀਆਂ and columns according ਮੰਜ਼ਿਲ the kernel\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   ਰਫਤਾਰ         print the terminal ਰਫਤਾਰ\n\
   ਸਮਾਂ N        with -icanon, ਸੈੱਟ_2 ਪਠ timeout ਦਾ N tenths ਦਾ a ਦੂਜਾ\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control ਸੰਕੇਤ_2\n\
   [-]cread      allow ਨਿਵੇਸ਼ ਮੰਜ਼ਿਲ be received\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   csN           ਸੈੱਟ_2 character ਮਾਪ ਮੰਜ਼ਿਲ N ਬਿੱਟ_2, N ਵਿੱਚ [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop ਬਿੱਟ_2 ਪ੍ਰਤੀ character (one with '-')\n\
   [-]hup        send a hangup ਸੰਕੇਤ when the ਆਖਰੀ process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity ਦੋਆਧਾਰੀ_ਅੰਕ ਵਿੱਚ output and expect parity ਦੋਆਧਾਰੀ_ਅੰਕ ਵਿੱਚ ਨਿਵੇਸ਼\n\
   [-]parodd     ਸੈੱਟ_2 odd parity (or even parity with '-')\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
\n\
ਨਿਵੇਸ਼_3 settings:\n\
   [-]brkint     breaks cause an ਰੁਕਾਵਟ ਸੰਕੇਤ\n\
   [-]icrnl      translate carriage ਵਾਪਸ ਮੰਜ਼ਿਲ newline\n\
   [-]ignbrk     ignore ਰੋਕੋ characters\n\
   [-]igncr      ignore carriage ਵਾਪਸ\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and ਕਰੋ not flush a ਭਰਿਆ ਨਿਵੇਸ਼ ਬਫਰ on a character\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   [-]inlcr      translate newline ਮੰਜ਼ਿਲ carriage ਵਾਪਸ\n\
   [-]inpck      enable ਨਿਵੇਸ਼ parity checking\n\
   [-]istrip     clear ਉੱਚਾ (8th) ਦੋਆਧਾਰੀ_ਅੰਕ ਦਾ ਨਿਵੇਸ਼ characters\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IUTF8
      fputs (_("\
 * [-]iutf8      assume ਨਿਵੇਸ਼ characters are UTF-8 encoded\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters ਮੰਜ਼ਿਲ lowercase\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only ਸ਼ੁਰੂ character\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   [-]ixoff      enable sending ਦਾ ਸ਼ੁਰੂ/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BSDLY
      fputs (_("\
 * bsN           backspace delay style, N ਵਿੱਚ [0..1]\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CRDLY
      fputs (_("\
 * crN           carriage ਵਾਪਸ delay style, N ਵਿੱਚ [0..3]\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FFDLY
      fputs (_("\
 * ffN           form feed delay style, N ਵਿੱਚ [0..1]\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NLDLY
      fputs (_("\
 * nlN           newline delay style, N ਵਿੱਚ [0..1]\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage ਵਾਪਸ ਮੰਜ਼ਿਲ newline\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters ਲਈ fill instead ਦਾ NUL characters\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead ਦਾ timing ਲਈ delays\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters ਮੰਜ਼ਿਲ uppercase\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLCR
      fputs (_("\
 * [-]onlcr      translate newline ਮੰਜ਼ਿਲ carriage ਵਾਪਸ-newline\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage ਵਾਪਸ\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONOCR
      fputs (_("\
 * [-]onocr      ਕਰੋ not print carriage returns ਵਿੱਚ the ਪਹਿਲਾ ਕਾਲਮ\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#ਜੇ defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N ਵਿੱਚ [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N ਵਿੱਚ [0..1]\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOKE
      fputs (_("\
 * crtkill       kill all ਸਤਰ by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all ਸਤਰ by obeying the echoctl and echok settings\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters ਵਿੱਚ hat notation ('^c')\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   [-]echo       echo ਨਿਵੇਸ਼ characters\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   [-]echonl     echo newline even ਜੇ not echoing other characters\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with ਉੱਚਾ latency links\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#ਜੇ_ਅੰਤ
      ਲਿਖੋ (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VWERASE
    ", werase"
#ਜੇ_ਅੰਤ
#ਜੇ defined CREPRINT || defined VREPRINT
    ", rprnt"
#ਜੇ_ਅੰਤ
);
      fputs (_("\
   [-]isig       enable ਰੁਕਾਵਟ, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after ਰੁਕਾਵਟ and quit special characters\n\
"), stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try ਮੰਜ਼ਿਲ ਲਿਖ ਮੰਜ਼ਿਲ the terminal\n\
"), stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' ਲਈ uppercase characters\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#ਜੇ defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters ਮੰਜ਼ਿਲ their ਮੂਲ ਮੁੱਲਾਂ\n\
   -cooked       same as raw\n\
"), stdout);
      ਲਿਖੋ (_("\
   crt           same as %s\n\
"), "echoe"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOCTL
    " echoctl"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOKE
    " echoke"
#ਜੇ_ਅੰਤ
);
      ਲਿਖੋ (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOCTL
    " echoctl"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOKE
    " echoke"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IXANY
    " -ixany"
#ਜੇ_ਅੰਤ
);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   ek            erase and kill characters ਮੰਜ਼ਿਲ their ਮੂਲ ਮੁੱਲਾਂ\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#ਜੇ defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#ਜੇ_ਅੰਤ
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      ਲਿਖੋ (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLCR
   " -onlcr"
#ਜੇ_ਅੰਤ
  , "icrnl -inlcr -igncr"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLCR
   " onlcr"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OCRNL
   " -ocrnl"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLRET
   " -onlret"
#ਜੇ_ਅੰਤ
);
      fputs (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), stdout);
      ਲਿਖੋ (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 ਸਮਾਂ 0\n\
   -raw          same as cooked\n\
"),
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IUCLC
   " -iuclc"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IXANY
   " -ixany"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IMAXBEL
   " -imaxbel"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XCASE
   " -xcase"
#ਜੇ_ਅੰਤ
);
      ਲਿਖੋ (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters ਮੰਜ਼ਿਲ their ਮੂਲ ਮੁੱਲਾਂ\n\
"),
   "-ixoff"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IUTF8
   " -iutf8"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IUCLC
   " -iuclc"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IXANY
   " -ixany"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IMAXBEL
   " imaxbel"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XCASE
   " -xcase"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OLCUC
   " -olcuc"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OCRNL
   " -ocrnl"
#ਜੇ_ਅੰਤ

 , "opost"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OFILL
   " -ofill"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLCR
   " onlcr"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONOCR
   " -onocr"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLRET
   " -onlret"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NLDLY
   " nl0"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CRDLY
   " cr0"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TAB0
   " tab0"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BSDLY
   " bs0"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VTDLY
   " vt0"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FFDLY
   " ff0"
#ਜੇ_ਅੰਤ

 , "isig"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TOSTOP
   " -tostop"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OFDEL
   " -ofdel"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOPRT
   " -echoprt"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOCTL
   " echoctl"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOKE
   " echoke"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EXTPROC
   " -extproc"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FLUSHO
   " -flusho"
#ਜੇ_ਅੰਤ
);
      fputs (_("\
\n\
Handle the tty ਸਤਰ connected ਮੰਜ਼ਿਲ standard ਨਿਵੇਸ਼.  Without arguments,\n\
prints baud ਦਰ, ਸਤਰ discipline, and deviations from stty sane.  ਵਿੱਚ_2\n\
settings, CHAR is ਲਿਆ literally, or coded as ਵਿੱਚ ^c, 0x37, 0177 or\n\
127; special ਮੁੱਲਾਂ ^- or undef ਵਰਤਿਆ ਮੰਜ਼ਿਲ disable special characters.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

ਸਥਿਰ ਖਾਲੀ
apply_settings (bool checking, ਅੱਖਰ ਅਚਲ *device_name,
                ਅੱਖਰ * ਅਚਲ *settings, ਪੂਰਨ n_settings,
                ਬਣਤਰ termios *ਬਹੁਲਕ_2, bool *require_ਸੈੱਟ_attr)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ check_ਦਲੀਲ(arg)						\
  ਕਰੋ									\
    {									\
      ਜੇ (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  ਜਦੋਂ (0)

  ਲਈ (ਪੂਰਨ k = 1; k < n_settings; k++)
    {
      ਅੱਖਰ ਅਚਲ *arg = settings[k];
      bool ਮੇਲ_ਮਿਲਿਆ = false;
      bool not_ਸੈੱਟ_attr = false;
      bool reversed = false;

      ਜੇ (! arg)
        ਜਾਰੀ;

      ਜੇ (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      ਜੇ (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          ਜਾਰੀ;
        }
      ਲਈ (ਪੂਰਨ i = 0; ਬਹੁਲਕ_info[i].name != ਨਿਰਾ; ++i)
        {
          ਜੇ (streq (arg, ਬਹੁਲਕ_info[i].name))
            {
              ਜੇ ((ਬਹੁਲਕ_info[i].flags & NO_SETATTR) == 0)
                {
                  ਮੇਲ_ਮਿਲਿਆ = ਸੈੱਟ_ਬਹੁਲਕ (&ਬਹੁਲਕ_info[i], reversed, ਬਹੁਲਕ_2);
                  *require_ਸੈੱਟ_attr = true;
                }
              ਨਹੀਂਤਾਂ
                ਮੇਲ_ਮਿਲਿਆ = not_ਸੈੱਟ_attr = true;
              ਰੋਕੋ;
            }
        }
      ਜੇ (!ਮੇਲ_ਮਿਲਿਆ && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      ਜੇ (!ਮੇਲ_ਮਿਲਿਆ)
        {
          ਲਈ (ਪੂਰਨ i = 0; control_info[i].name != ਨਿਰਾ; ++i)
            {
              ਜੇ (streq (arg, control_info[i].name))
                {
                  check_ਦਲੀਲ (arg);
                  ਮੇਲ_ਮਿਲਿਆ = true;
                  ++k;
                  ਸੈੱਟ_control_char (&control_info[i], settings[k], ਬਹੁਲਕ_2);
                  *require_ਸੈੱਟ_attr = true;
                  ਰੋਕੋ;
                }
            }
        }
      ਜੇ (!ਮੇਲ_ਮਿਲਿਆ || not_ਸੈੱਟ_attr)
        {
          ਜੇ (streq (arg, "ispeed"))
            {
              check_ਦਲੀਲ (arg);
              ++k;
              ਬਣਤਰ ਰਫਤਾਰ_desc ਅਚਲ ispeed = string_ਮੰਜ਼ਿਲ_baud (settings[k]);
              ਜੇ (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              ਸੈੱਟ_ਰਫਤਾਰ (ਨਿਵੇਸ਼_ਰਫਤਾਰ, settings[k], ispeed, ਬਹੁਲਕ_2);
              ਜੇ (checking)
                ਜਾਰੀ;
              *require_ਸੈੱਟ_attr = true;
            }
          ਨਹੀਂਤਾਂ ਜੇ (streq (arg, "ospeed"))
            {
              check_ਦਲੀਲ (arg);
              ++k;
              ਬਣਤਰ ਰਫਤਾਰ_desc ਅਚਲ ospeed = string_ਮੰਜ਼ਿਲ_baud (settings[k]);
              ਜੇ (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              ਸੈੱਟ_ਰਫਤਾਰ (output_ਰਫਤਾਰ, settings[k], ospeed, ਬਹੁਲਕ_2);
              ਜੇ (checking)
                ਜਾਰੀ;
              *require_ਸੈੱਟ_attr = true;
            }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          ਨਹੀਂਤਾਂ ਜੇ (streq (arg, "extproc"))
            {
              ਪੂਰਨ val = ! reversed;

              ਜੇ (checking)
                ਜਾਰੀ;

              ਜੇ (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TIOCGWINSZ
          ਨਹੀਂਤਾਂ ਜੇ (streq (arg, "rows"))
            {
              check_ਦਲੀਲ (arg);
              ++k;
              ਜੇ (checking)
                ਜਾਰੀ;
              ਸੈੱਟ_ਖਿੜਕੀ_ਮਾਪ (integer_arg (settings[k], ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ), -1,
                               device_name);
            }
          ਨਹੀਂਤਾਂ ਜੇ (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_ਦਲੀਲ (arg);
              ++k;
              ਜੇ (checking)
                ਜਾਰੀ;
              ਸੈੱਟ_ਖਿੜਕੀ_ਮਾਪ (-1, integer_arg (settings[k], ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ),
                               device_name);
            }
          ਨਹੀਂਤਾਂ ਜੇ (streq (arg, "size"))
            {
              ਜੇ (checking)
                ਜਾਰੀ;
              max_ਖਾਨਾ = screen_columns ();
              current_ਖਾਨਾ = 0;
              display_ਖਿੜਕੀ_ਮਾਪ (false, device_name);
            }
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_C_ਸਤਰ
          ਨਹੀਂਤਾਂ ਜੇ (streq (arg, "line"))
            {
              check_ਦਲੀਲ (arg);
              ++k;
              uintmax_t ਮੁੱਲ = integer_arg (settings[k], UINTMAX_MAX);
              ਜੇ (ckd_ਜੋੜੋ (&ਬਹੁਲਕ_2->c_ਸਤਰ, ਮੁੱਲ, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_ਸੈੱਟ_attr = true;
            }
#ਜੇ_ਅੰਤ
          ਨਹੀਂਤਾਂ ਜੇ (streq (arg, "speed"))
            {
              ਜੇ (checking)
                ਜਾਰੀ;
              max_ਖਾਨਾ = screen_columns ();
              display_ਰਫਤਾਰ (ਬਹੁਲਕ_2, false);
            }
          ਨਹੀਂਤਾਂ
            {
              ਬਣਤਰ ਰਫਤਾਰ_desc ਅਚਲ ਰਫਤਾਰ = string_ਮੰਜ਼ਿਲ_baud (arg);
              ਜੇ (ਰਫਤਾਰ.valid)
                {
                  ਸੈੱਟ_ਰਫਤਾਰ (both_speeds, arg, ਰਫਤਾਰ, ਬਹੁਲਕ_2);
                  ਜੇ (checking)
                    ਜਾਰੀ;
                  *require_ਸੈੱਟ_attr = true;
                }
              ਨਹੀਂਤਾਂ
                {
                  ਜੇ (! recover_ਬਹੁਲਕ (arg, ਬਹੁਲਕ_2))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_ਸੈੱਟ_attr = true;
                }
            }
        }
    }

  ਜੇ (checking)
    check_ਰਫਤਾਰ (ਬਹੁਲਕ_2);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ਸਥਿਰ ਬਣਤਰ termios ਬਹੁਲਕ_2;

  ਗਿਣਤੀ output_type output_type;
  ਪੂਰਨ optc;
  ਪੂਰਨ argi = 0;
  ਪੂਰਨ opti = 1;
  bool require_ਸੈੱਟ_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  ਅੱਖਰ *file_name = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *device_name;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = ਬਦਲਿਆ;
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

  ਜਦੋਂ ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'a':
          verbose_output = true;
          output_type = all;
          ਰੋਕੋ;

        ਹਾਲਤ 'g':
          recoverable_output = true;
          output_type = recoverable;
          ਰੋਕੋ;

        ਹਾਲਤ 'F':
          ਜੇ (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ DEV_DEBUG_OPTION:
          dev_debug = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          ਜੇ (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          ਰੋਕੋ;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      ਜਦੋਂ (opti < optind)
        argv[argi + opti++] = ਨਿਰਾ;
    }

  /* Specifying both -a and -g gets an error.  */
  ਜੇ (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  ਜੇ (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  ਜੇ (!noargs && !verbose_output && !recoverable_output)
    {
      ਸਥਿਰ ਬਣਤਰ termios check_ਬਹੁਲਕ;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_ਬਹੁਲਕ, &require_ਸੈੱਟ_attr);
    }

  ਜੇ (file_name)
    {
      ਪੂਰਨ fdflags;
      ਜੇ (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ਜੇ ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  ਜੇ (tcgetattr (STDIN_FILENO, &ਬਹੁਲਕ_2))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  ਜੇ (verbose_output || recoverable_output || noargs)
    {
      max_ਖਾਨਾ = screen_columns ();
      current_ਖਾਨਾ = 0;
      display_settings (output_type, &ਬਹੁਲਕ_2, device_name);
      ਵਾਪਸ EXIT_SUCCESS;
    }

  require_ਸੈੱਟ_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &ਬਹੁਲਕ_2, &require_ਸੈੱਟ_attr);

  ਜੇ (require_ਸੈੱਟ_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ਸਥਿਰ ਬਣਤਰ termios new_ਬਹੁਲਕ;

      ਜੇ (tcsetattr (STDIN_FILENO, tcsetattr_options, &ਬਹੁਲਕ_2))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      ਜੇ (tcgetattr (STDIN_FILENO, &new_ਬਹੁਲਕ))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ਜੇ (! eq_ਬਹੁਲਕ (&ਬਹੁਲਕ_2, &new_ਬਹੁਲਕ))
        {
          ਜੇ (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਆਕਾਰ (new_ਬਹੁਲਕ); i++)
                {
                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ newc = *(((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &new_ਬਹੁਲਕ) + i);
                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ oldc = *(((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &ਬਹੁਲਕ_2) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  ਵਾਪਸ EXIT_SUCCESS;
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
ਸਥਿਰ bool
eq_ਬਹੁਲਕ (ਬਣਤਰ termios ਅਚਲ *mode1, ਬਣਤਰ termios ਅਚਲ *mode2)
{
  ਜੇ (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_C_ਸਤਰ
      || mode1->c_ਸਤਰ != mode2->c_ਸਤਰ
#ਜੇ_ਅੰਤ
      || !memeq (mode1->c_cc, mode2->c_cc, ਆਕਾਰ (mode1->c_cc)))
    ਵਾਪਸ false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_ਨਕਾਬ = ~(tcflag_t) 0;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CBAUD
  cflag_ਨਕਾਬ &= ~(tcflag_t) CBAUD;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CIBAUD
  cflag_ਨਕਾਬ &= ~(tcflag_t) CIBAUD;
#ਜੇ_ਅੰਤ

  ਜੇ ((mode1->c_cflag ^ mode2->c_cflag) & cflag_ਨਕਾਬ)
    ਵਾਪਸ false;

  ਵਾਪਸ ਰਫਤਾਰ_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ਸਥਿਰ bool
ਸੈੱਟ_ਬਹੁਲਕ (ਬਣਤਰ ਬਹੁਲਕ_info ਅਚਲ *info, bool reversed, ਬਣਤਰ termios *ਬਹੁਲਕ_2)
{
  tcflag_t *bitsp;

  ਜੇ (reversed && (info->flags & REV) == 0)
    ਵਾਪਸ false;

  bitsp = ਬਹੁਲਕ_type_flag (info->type, ਬਹੁਲਕ_2);

  ਜੇ (bitsp == ਨਿਰਾ)
    {
      /* Combination mode. */
      ਜੇ (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          ਜੇ (reversed)
            ਬਹੁਲਕ_2->c_cflag = (ਬਹੁਲਕ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ਨਹੀਂਤਾਂ
            ਬਹੁਲਕ_2->c_cflag = (ਬਹੁਲਕ_2->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "oddp"))
        {
          ਜੇ (reversed)
            ਬਹੁਲਕ_2->c_cflag = (ਬਹੁਲਕ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ਨਹੀਂਤਾਂ
            ਬਹੁਲਕ_2->c_cflag = (ਬਹੁਲਕ_2->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "nl"))
        {
          ਜੇ (reversed)
            {
              ਬਹੁਲਕ_2->c_iflag = (ਬਹੁਲਕ_2->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              ਬਹੁਲਕ_2->c_oflag = (ਬਹੁਲਕ_2->c_oflag
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLCR
                               | ONLCR
#ਜੇ_ਅੰਤ
                )
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OCRNL
                & ~OCRNL
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLRET
                & ~ONLRET
#ਜੇ_ਅੰਤ
                ;
            }
          ਨਹੀਂਤਾਂ
            {
              ਬਹੁਲਕ_2->c_iflag = ਬਹੁਲਕ_2->c_iflag & ~ICRNL;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ONLCR
              ਬਹੁਲਕ_2->c_oflag = ਬਹੁਲਕ_2->c_oflag & ~ONLCR;
#ਜੇ_ਅੰਤ
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "ek"))
        {
          ਬਹੁਲਕ_2->c_cc[VERASE] = CERASE;
          ਬਹੁਲਕ_2->c_cc[VKILL] = CKILL;
        }
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "sane"))
        sane_ਬਹੁਲਕ (ਬਹੁਲਕ_2);
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "cbreak"))
        {
          ਜੇ (reversed)
            ਬਹੁਲਕ_2->c_lflag |= ICANON;
          ਨਹੀਂਤਾਂ
            ਬਹੁਲਕ_2->c_lflag &= ~ICANON;
        }
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "pass8"))
        {
          ਜੇ (reversed)
            {
              ਬਹੁਲਕ_2->c_cflag = (ਬਹੁਲਕ_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              ਬਹੁਲਕ_2->c_iflag |= ISTRIP;
            }
          ਨਹੀਂਤਾਂ
            {
              ਬਹੁਲਕ_2->c_cflag = (ਬਹੁਲਕ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              ਬਹੁਲਕ_2->c_iflag &= ~ISTRIP;
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "litout"))
        {
          ਜੇ (reversed)
            {
              ਬਹੁਲਕ_2->c_cflag = (ਬਹੁਲਕ_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              ਬਹੁਲਕ_2->c_iflag |= ISTRIP;
              ਬਹੁਲਕ_2->c_oflag |= OPOST;
            }
          ਨਹੀਂਤਾਂ
            {
              ਬਹੁਲਕ_2->c_cflag = (ਬਹੁਲਕ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              ਬਹੁਲਕ_2->c_iflag &= ~ISTRIP;
              ਬਹੁਲਕ_2->c_oflag &= ~OPOST;
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          ਜੇ ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              ਬਹੁਲਕ_2->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              ਬਹੁਲਕ_2->c_oflag |= OPOST;
              ਬਹੁਲਕ_2->c_lflag |= ISIG | ICANON;
#ਜੇ VMIN == VEOF
              ਬਹੁਲਕ_2->c_cc[VEOF] = CEOF;
#ਜੇ_ਅੰਤ
#ਜੇ VTIME == VEOL
              ਬਹੁਲਕ_2->c_cc[VEOL] = CEOL;
#ਜੇ_ਅੰਤ
            }
          ਨਹੀਂਤਾਂ
            {
              /* Raw mode. */
              ਬਹੁਲਕ_2->c_iflag = 0;
              ਬਹੁਲਕ_2->c_oflag &= ~OPOST;
              ਬਹੁਲਕ_2->c_lflag &= ~(ISIG | ICANON
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XCASE
                                 | XCASE
#ਜੇ_ਅੰਤ
                );
              ਬਹੁਲਕ_2->c_cc[VMIN] = 1;
              ਬਹੁਲਕ_2->c_cc[VTIME] = 0;
            }
        }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IXANY
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "decctlq"))
        {
          ਜੇ (reversed)
            ਬਹੁਲਕ_2->c_iflag |= IXANY;
          ਨਹੀਂਤਾਂ
            ਬਹੁਲਕ_2->c_iflag &= ~IXANY;
        }
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TABDLY
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "tabs"))
        {
          ਜੇ (reversed)
            ਬਹੁਲਕ_2->c_oflag = (ਬਹੁਲਕ_2->c_oflag & ~TABDLY) | TAB3;
          ਨਹੀਂਤਾਂ
            ਬਹੁਲਕ_2->c_oflag = (ਬਹੁਲਕ_2->c_oflag & ~TABDLY) | TAB0;
        }
#ਨਹੀਂਤਾਂ
# ifdef OXTABS
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "tabs"))
        {
          ਜੇ (reversed)
            ਬਹੁਲਕ_2->c_oflag = ਬਹੁਲਕ_2->c_oflag | OXTABS;
          ਨਹੀਂਤਾਂ
            ਬਹੁਲਕ_2->c_oflag = ਬਹੁਲਕ_2->c_oflag & ~OXTABS;
        }
# endif
#ਜੇ_ਅੰਤ
#ਜੇ defined XCASE && defined IUCLC && defined OLCUC
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          ਜੇ (reversed)
            {
              ਬਹੁਲਕ_2->c_lflag &= ~XCASE;
              ਬਹੁਲਕ_2->c_iflag &= ~IUCLC;
              ਬਹੁਲਕ_2->c_oflag &= ~OLCUC;
            }
          ਨਹੀਂਤਾਂ
            {
              ਬਹੁਲਕ_2->c_lflag |= XCASE;
              ਬਹੁਲਕ_2->c_iflag |= IUCLC;
              ਬਹੁਲਕ_2->c_oflag |= OLCUC;
            }
        }
#ਜੇ_ਅੰਤ
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "crt"))
        ਬਹੁਲਕ_2->c_lflag |= ECHOE
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOCTL
          | ECHOCTL
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOKE
          | ECHOKE
#ਜੇ_ਅੰਤ
          ;
      ਨਹੀਂਤਾਂ ਜੇ (streq (info->name, "dec"))
        {
          ਬਹੁਲਕ_2->c_cc[VINTR] = 3;	/* ^C */
          ਬਹੁਲਕ_2->c_cc[VERASE] = 127;	/* DEL */
          ਬਹੁਲਕ_2->c_cc[VKILL] = 21;	/* ^U */
          ਬਹੁਲਕ_2->c_lflag |= ECHOE
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOCTL
            | ECHOCTL
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ECHOKE
            | ECHOKE
#ਜੇ_ਅੰਤ
            ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ IXANY
          ਬਹੁਲਕ_2->c_iflag &= ~IXANY;
#ਜੇ_ਅੰਤ
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (reversed)
    *bitsp = *bitsp & ~info->ਨਕਾਬ & ~info->ਬਿੱਟ_2;
  ਨਹੀਂਤਾਂ
    *bitsp = (*bitsp & ~info->ਨਕਾਬ) | info->ਬਿੱਟ_2;

  ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_control_char (ਬਣਤਰ control_info ਅਚਲ *info, ਅੱਖਰ ਅਚਲ *arg,
                  ਬਣਤਰ termios *ਬਹੁਲਕ_2)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ ਮੁੱਲ;

  ਜੇ (streq (info->name, "min") || streq (info->name, "time"))
    ਮੁੱਲ = integer_arg (arg, TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (cc_t));
  ਨਹੀਂਤਾਂ ਜੇ (arg[0] == '\0' || arg[1] == '\0')
    ਮੁੱਲ = ਮੰਜ਼ਿਲ_uchar (arg[0]);
  ਨਹੀਂਤਾਂ ਜੇ (streq (arg, "^-") || streq (arg, "undef"))
    ਮੁੱਲ = _POSIX_VDISABLE;
  ਨਹੀਂਤਾਂ ਜੇ (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      ਜੇ (arg[1] == '?')
        ਮੁੱਲ = 127;
      ਨਹੀਂਤਾਂ
        ਮੁੱਲ = ਮੰਜ਼ਿਲ_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  ਨਹੀਂਤਾਂ
    ਮੁੱਲ = integer_arg (arg, TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (cc_t));
  ਬਹੁਲਕ_2->c_cc[info->ਵਿੱਥ] = ਮੁੱਲ;
}

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_ਰਫਤਾਰ (ਗਿਣਤੀ ਰਫਤਾਰ_setting type, ਅੱਖਰ ਅਚਲ *arg,
           ਬਣਤਰ ਰਫਤਾਰ_desc ਅਚਲ ਰਫਤਾਰ, ਬਣਤਰ termios *ਬਹੁਲਕ_2)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (ਰਫਤਾਰ.valid);

  ਜੇ (type == ਨਿਵੇਸ਼_ਰਫਤਾਰ || type == both_speeds)
    {
      ਆਖਰੀ_ibaud = ਰਫਤਾਰ;
      ਜੇ (cfsetispeed (ਬਹੁਲਕ_2, ਰਫਤਾਰ.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  ਜੇ (type == output_ਰਫਤਾਰ || type == both_speeds)
    {
      ਆਖਰੀ_obaud = ਰਫਤਾਰ;
      ਜੇ (cfsetospeed (ਬਹੁਲਕ_2, ਰਫਤਾਰ.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TIOCGWINSZ

ਸਥਿਰ ਪੂਰਨ
get_win_ਮਾਪ (ਪੂਰਨ fd, ਬਣਤਰ winsize *win)
{
  ਪੂਰਨ err = ioctl (fd, TIOCGWINSZ, (ਅੱਖਰ *) win);
  ਵਾਪਸ err;
}

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_ਖਿੜਕੀ_ਮਾਪ (ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_2, ਅੱਖਰ ਅਚਲ *device_name)
{
  ਬਣਤਰ winsize win;

  ਜੇ (get_win_ਮਾਪ (STDIN_FILENO, &win))
    {
      ਜੇ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, ਆਕਾਰ (win));
    }

  ਜੇ (ਪੰਕਤੀਆਂ >= 0)
    win.ws_ਪੰਕਤੀ = ਪੰਕਤੀਆਂ;
  ਜੇ (ਖਾਨੇ_ਗਿਣਤੀ_2 >= 0)
    win.ws_ਖਾਨਾ = ਖਾਨੇ_ਗਿਣਤੀ_2;

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

  ਜੇ (win.ws_ਪੰਕਤੀ == 0 || win.ws_ਖਾਨਾ == 0)
    {
      ਬਣਤਰ ttysize ttysz;

      ttysz.ts_ਸਤਰਾਂ = win.ws_ਪੰਕਤੀ;
      ttysz.ts_ਖਾਨੇ_ਗਿਣਤੀ = win.ws_ਖਾਨਾ;

      win.ws_ਪੰਕਤੀ = 1;
      win.ws_ਖਾਨਾ = 1;

      ਜੇ (ioctl (STDIN_FILENO, TIOCSWINSZ, (ਅੱਖਰ *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ਜੇ (ioctl (STDIN_FILENO, TIOCSSIZE, (ਅੱਖਰ *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ਵਾਪਸ;
    }
# endif

  ਜੇ (ioctl (STDIN_FILENO, TIOCSWINSZ, (ਅੱਖਰ *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

ਸਥਿਰ ਖਾਲੀ
display_ਖਿੜਕੀ_ਮਾਪ (bool fancy, ਅੱਖਰ ਅਚਲ *device_name)
{
  ਬਣਤਰ winsize win;

  ਜੇ (get_win_ਮਾਪ (STDIN_FILENO, &win))
    {
      ਜੇ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ਜੇ (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  ਨਹੀਂਤਾਂ
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_ਪੰਕਤੀ, win.ws_ਖਾਨਾ);
      ਜੇ (!fancy)
        current_ਖਾਨਾ = 0;
    }
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਪੂਰਨ
screen_columns (ਖਾਲੀ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TIOCGWINSZ
  ਬਣਤਰ winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  ਜੇ (get_win_ਮਾਪ (STDOUT_FILENO, &win) == 0 && 0 < win.ws_ਖਾਨਾ)
    ਵਾਪਸ win.ws_ਖਾਨਾ;
#ਜੇ_ਅੰਤ
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    ਅੱਖਰ *ਖਾਨਾ_string = getenv ("COLUMNS");
    ਲੰਮਾ ਪੂਰਨ n_columns;
    ਜੇ (!(ਖਾਨਾ_string != ਨਿਰਾ
          && xstrtol (ਖਾਨਾ_string, ਨਿਰਾ, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ))
      n_columns = 80;
    ਵਾਪਸ n_columns;
  }
}

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ tcflag_t *
ਬਹੁਲਕ_type_flag (ਗਿਣਤੀ ਬਹੁਲਕ_type type, ਬਣਤਰ termios *ਬਹੁਲਕ_2)
{
  ਚੋਣ (type)
    {
    ਹਾਲਤ control:
      ਵਾਪਸ &ਬਹੁਲਕ_2->c_cflag;

    ਹਾਲਤ ਨਿਵੇਸ਼:
      ਵਾਪਸ &ਬਹੁਲਕ_2->c_iflag;

    ਹਾਲਤ output:
      ਵਾਪਸ &ਬਹੁਲਕ_2->c_oflag;

    ਹਾਲਤ local:
      ਵਾਪਸ &ਬਹੁਲਕ_2->c_lflag;

    ਹਾਲਤ combination:
      ਵਾਪਸ ਨਿਰਾ;

    ਮੂਲ:
      unreachable ();
    }
}

ਸਥਿਰ ਖਾਲੀ
display_settings (ਗਿਣਤੀ output_type output_type, ਬਣਤਰ termios *ਬਹੁਲਕ_2,
                  ਅੱਖਰ ਅਚਲ *device_name)
{
  ਚੋਣ (output_type)
    {
    ਹਾਲਤ ਬਦਲਿਆ:
      display_ਬਦਲਿਆ (ਬਹੁਲਕ_2);
      ਰੋਕੋ;

    ਹਾਲਤ all:
      display_all (ਬਹੁਲਕ_2, device_name);
      ਰੋਕੋ;

    ਹਾਲਤ recoverable:
      display_recoverable (ਬਹੁਲਕ_2);
      ਰੋਕੋ;
    }
}

ਸਥਿਰ ਖਾਲੀ
display_ਬਦਲਿਆ (ਬਣਤਰ termios *ਬਹੁਲਕ_2)
{
  bool ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ;
  tcflag_t *bitsp;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨਕਾਬ;
  ਗਿਣਤੀ ਬਹੁਲਕ_type prev_type = control;

  display_ਰਫਤਾਰ (ਬਹੁਲਕ_2, true);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_C_ਸਤਰ
  wrapf ("line = %d;", ਬਹੁਲਕ_2->c_ਸਤਰ);
#ਜੇ_ਅੰਤ
  ਅੱਖਰ_ਲਿਖੋ ('\n');
  current_ਖਾਨਾ = 0;

  ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ = true;
  ਲਈ (ਪੂਰਨ i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      ਜੇ (ਬਹੁਲਕ_2->c_cc[control_info[i].ਵਿੱਥ] == control_info[i].saneval)
        ਜਾਰੀ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ਜੇ (streq (control_info[i].name, "flush"))
        ਜਾਰੀ;
#ਜੇ_ਅੰਤ
      /* If swtch is the same as susp, don't print both.  */
#ਜੇ VSWTCH == VSUSP
      ਜੇ (streq (control_info[i].name, "swtch"))
        ਜਾਰੀ;
#ਜੇ_ਅੰਤ
      /* If eof uses the same slot as min, only print whichever applies.  */
#ਜੇ VEOF == VMIN
      ਜੇ ((ਬਹੁਲਕ_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ਜਾਰੀ;
#ਜੇ_ਅੰਤ

      ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (ਬਹੁਲਕ_2->c_cc[control_info[i].ਵਿੱਥ]));
    }
  ਜੇ ((ਬਹੁਲਕ_2->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) ਬਹੁਲਕ_2->c_cc[VMIN],
             (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) ਬਹੁਲਕ_2->c_cc[VTIME]);
    }
  ਨਹੀਂਤਾਂ ਜੇ (!ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ)
    ਅੱਖਰ_ਲਿਖੋ ('\n');
  current_ਖਾਨਾ = 0;

  ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ = true;
  ਲਈ (ਪੂਰਨ i = 0; ਬਹੁਲਕ_info[i].name != ਨਿਰਾ; ++i)
    {
      ਜੇ (ਬਹੁਲਕ_info[i].flags & OMIT)
        ਜਾਰੀ;
      ਜੇ (ਬਹੁਲਕ_info[i].type != prev_type)
        {
          ਜੇ (!ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ)
            {
              ਅੱਖਰ_ਲਿਖੋ ('\n');
              current_ਖਾਨਾ = 0;
              ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ = true;
            }
          prev_type = ਬਹੁਲਕ_info[i].type;
        }

      bitsp = ਬਹੁਲਕ_type_flag (ਬਹੁਲਕ_info[i].type, ਬਹੁਲਕ_2);
      ਨਕਾਬ = ਬਹੁਲਕ_info[i].ਨਕਾਬ ? ਬਹੁਲਕ_info[i].ਨਕਾਬ : ਬਹੁਲਕ_info[i].ਬਿੱਟ_2;
      ਜੇ ((*bitsp & ਨਕਾਬ) == ਬਹੁਲਕ_info[i].ਬਿੱਟ_2)
        {
          ਜੇ (ਬਹੁਲਕ_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", ਬਹੁਲਕ_info[i].name);
              ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ = false;
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ ((ਬਹੁਲਕ_info[i].flags & (SANE_ਸੈੱਟ | REV)) == (SANE_ਸੈੱਟ | REV))
        {
          wrapf ("-%s", ਬਹੁਲਕ_info[i].name);
          ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ = false;
        }
    }
  ਜੇ (!ਖਾਲੀ_ਥਾਵਾਂ_ਸਤਰ)
    ਅੱਖਰ_ਲਿਖੋ ('\n');
  current_ਖਾਨਾ = 0;
}

ਸਥਿਰ ਖਾਲੀ
display_all (ਬਣਤਰ termios *ਬਹੁਲਕ_2, ਅੱਖਰ ਅਚਲ *device_name)
{
  tcflag_t *bitsp;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨਕਾਬ;
  ਗਿਣਤੀ ਬਹੁਲਕ_type prev_type = control;

  display_ਰਫਤਾਰ (ਬਹੁਲਕ_2, true);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ TIOCGWINSZ
  display_ਖਿੜਕੀ_ਮਾਪ (true, device_name);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_C_ਸਤਰ
  wrapf ("line = %d;", ਬਹੁਲਕ_2->c_ਸਤਰ);
#ਜੇ_ਅੰਤ
  ਅੱਖਰ_ਲਿਖੋ ('\n');
  current_ਖਾਨਾ = 0;

  ਲਈ (ਪੂਰਨ i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ਜੇ (streq (control_info[i].name, "flush"))
        ਜਾਰੀ;
#ਜੇ_ਅੰਤ
      /* If swtch is the same as susp, don't print both.  */
#ਜੇ VSWTCH == VSUSP
      ਜੇ (streq (control_info[i].name, "swtch"))
        ਜਾਰੀ;
#ਜੇ_ਅੰਤ
      /* If eof uses the same slot as min, only print whichever applies.  */
#ਜੇ VEOF == VMIN
      ਜੇ ((ਬਹੁਲਕ_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ਜਾਰੀ;
#ਜੇ_ਅੰਤ
      wrapf ("%s = %s;", control_info[i].name,
             visible (ਬਹੁਲਕ_2->c_cc[control_info[i].ਵਿੱਥ]));
    }
#ਜੇ VEOF == VMIN
  ਜੇ ((ਬਹੁਲਕ_2->c_lflag & ICANON) == 0)
#ਜੇ_ਅੰਤ
    wrapf ("min = %lu; time = %lu;",
           (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) ਬਹੁਲਕ_2->c_cc[VMIN],
           (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) ਬਹੁਲਕ_2->c_cc[VTIME]);
  ਜੇ (current_ਖਾਨਾ != 0)
    ਅੱਖਰ_ਲਿਖੋ ('\n');
  current_ਖਾਨਾ = 0;

  ਲਈ (ਪੂਰਨ i = 0; ਬਹੁਲਕ_info[i].name != ਨਿਰਾ; ++i)
    {
      ਜੇ (ਬਹੁਲਕ_info[i].flags & OMIT)
        ਜਾਰੀ;
      ਜੇ (ਬਹੁਲਕ_info[i].type != prev_type)
        {
          ਅੱਖਰ_ਲਿਖੋ ('\n');
          current_ਖਾਨਾ = 0;
          prev_type = ਬਹੁਲਕ_info[i].type;
        }

      bitsp = ਬਹੁਲਕ_type_flag (ਬਹੁਲਕ_info[i].type, ਬਹੁਲਕ_2);
      ਨਕਾਬ = ਬਹੁਲਕ_info[i].ਨਕਾਬ ? ਬਹੁਲਕ_info[i].ਨਕਾਬ : ਬਹੁਲਕ_info[i].ਬਿੱਟ_2;
      ਜੇ ((*bitsp & ਨਕਾਬ) == ਬਹੁਲਕ_info[i].ਬਿੱਟ_2)
        wrapf ("%s", ਬਹੁਲਕ_info[i].name);
      ਨਹੀਂਤਾਂ ਜੇ (ਬਹੁਲਕ_info[i].flags & REV)
        wrapf ("-%s", ਬਹੁਲਕ_info[i].name);
    }
  ਅੱਖਰ_ਲਿਖੋ ('\n');
  current_ਖਾਨਾ = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ਸਥਿਰ ਖਾਲੀ
check_ਰਫਤਾਰ (ਬਣਤਰ termios *ਬਹੁਲਕ_2)
{
  ਜੇ (ਆਖਰੀ_ibaud.valid && ਆਖਰੀ_obaud.valid)
    {
      ਜੇ (cfgetispeed (ਬਹੁਲਕ_2) != ਆਖਰੀ_ibaud.baud
          || cfgetospeed (ਬਹੁਲਕ_2) != ਆਖਰੀ_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_ਮੰਜ਼ਿਲ_ਮੁੱਲ (ਆਖਰੀ_ibaud.baud),
               baud_ਮੰਜ਼ਿਲ_ਮੁੱਲ (ਆਖਰੀ_obaud.baud));
    }
}

ਸਥਿਰ ਖਾਲੀ
display_ਰਫਤਾਰ (ਬਣਤਰ termios *ਬਹੁਲਕ_2, bool fancy)
{
  ਜੇ (cfgetispeed (ਬਹੁਲਕ_2) == 0 || cfgetispeed (ਬਹੁਲਕ_2) == cfgetospeed (ਬਹੁਲਕ_2))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_ਮੰਜ਼ਿਲ_ਮੁੱਲ (cfgetospeed (ਬਹੁਲਕ_2)));
  ਨਹੀਂਤਾਂ
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_ਮੰਜ਼ਿਲ_ਮੁੱਲ (cfgetispeed (ਬਹੁਲਕ_2)),
           baud_ਮੰਜ਼ਿਲ_ਮੁੱਲ (cfgetospeed (ਬਹੁਲਕ_2)));
  ਜੇ (!fancy)
    current_ਖਾਨਾ = 0;
}

ਸਥਿਰ ਖਾਲੀ
display_recoverable (ਬਣਤਰ termios *ਬਹੁਲਕ_2)
{
  ਲਿਖੋ ("%lx:%lx:%lx:%lx",
          (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) ਬਹੁਲਕ_2->c_iflag,
          (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) ਬਹੁਲਕ_2->c_oflag,
          (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) ਬਹੁਲਕ_2->c_cflag,
          (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) ਬਹੁਲਕ_2->c_lflag);
  ਲਈ (size_t i = 0; i < NCCS; ++i)
    ਲਿਖੋ (":%lx", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) ਬਹੁਲਕ_2->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (ਬਹੁਲਕ_2->c_cflag & CBAUD) == BOTHER;
#  ਜੇ defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((ਬਹੁਲਕ_2->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#ਜੇ_ਅੰਤ
  ਜੇ (separate_speeds)
    ਲਿਖੋ (":%lx:%lx", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) cfgetispeed (ਬਹੁਲਕ_2),
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ) cfgetospeed (ਬਹੁਲਕ_2));

  ਅੱਖਰ_ਲਿਖੋ ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STRTOUL_TYPE(TYPE)                                               \
  ਸਥਿਰ ਪੂਰਨ                                                             \
  strtoul_##TYPE (ਅੱਖਰ ਅਚਲ *s, ਪੂਰਨ ਅਧਾਰ_2, ਅੱਖਰ **p, TYPE *ਨਤੀਜਾ,       \
                  ਅੱਖਰ delim)                                            \
  {                                                                      \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, ਅਧਾਰ_2);                                           \
    ਜੇ (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      ਵਾਪਸ -1;                                                         \
    *ਨਤੀਜਾ = ul;                                                        \
    ਵਾਪਸ 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (ਰਫਤਾਰ_t)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#ਅਨ੍ਤਰ੍ਭਾਵ "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ਸਥਿਰ bool
recover_ਬਹੁਲਕ (ਅੱਖਰ ਅਚਲ *arg, ਬਣਤਰ termios *ਬਹੁਲਕ_2)
{
  tcflag_t flag[4];
  ਅੱਖਰ ਅਚਲ *s = arg;
  ਲਈ (size_t i = 0; i < 4; i++)
    {
      ਅੱਖਰ *p;
      ਜੇ (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        ਵਾਪਸ false;
      s = p + 1;
    }
  ਬਹੁਲਕ_2->c_iflag = flag[0];
  ਬਹੁਲਕ_2->c_oflag = flag[1];
  ਬਹੁਲਕ_2->c_cflag = flag[2];
  ਬਹੁਲਕ_2->c_lflag = flag[3];

  ਲਈ (size_t i = 0; i < NCCS; ++i)
    {
      ਅੱਖਰ *p;
      ਅੱਖਰ delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      ਜੇ (strtoul_cc_t (s, 16, &p, ਬਹੁਲਕ_2->c_cc + i, delim) != 0)
        ਵਾਪਸ false;
      /* Accept the old format, which has no separate speed fields.  */
      ਜੇ (! *p)
        {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_ਰਫਤਾਰ_T_SANE
          ਬਣਤਰ ਰਫਤਾਰ_desc ospeed = cbaud_ਮੰਜ਼ਿਲ_ਰਫਤਾਰ (ਬਹੁਲਕ_2->c_cflag & CBAUD);
          ਜੇ (ospeed.valid && cfsetospeed (ਬਹੁਲਕ_2, ospeed.baud) != 0)
            ਵਾਪਸ false;
#  ਜੇ defined CIBAUD && defined IBSHIFT
          ਬਣਤਰ ਰਫਤਾਰ_desc ispeed =
            cbaud_ਮੰਜ਼ਿਲ_ਰਫਤਾਰ ((ਬਹੁਲਕ_2->c_cflag & CIBAUD) >> IBSHIFT);
          ਜੇ (ispeed.valid && cfsetispeed (ਬਹੁਲਕ_2, ispeed.baud) != 0)
            ਵਾਪਸ false;
#  endif
# endif
          ਆਖਰੀ_ibaud.baud = cfgetispeed (ਬਹੁਲਕ_2);
          ਆਖਰੀ_obaud.baud = cfgetospeed (ਬਹੁਲਕ_2);
#ਜੇ_ਅੰਤ
          ਵਾਪਸ true;
        }
      s = p + 1;
    }

  ਰਫਤਾਰ_t ਰਫਤਾਰ[2];
  ਲਈ (size_t i = 0; i < 2; i++)
    {
      ਅੱਖਰ *p;
      ਅੱਖਰ delim = i == 0 ? ':' : '\0';
      ਜੇ (strtoul_ਰਫਤਾਰ_t (s, 16, &p, ਰਫਤਾਰ + i, delim) != 0)
        ਵਾਪਸ false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  ਜੇ (cfsetospeed (ਬਹੁਲਕ_2, ਰਫਤਾਰ[1]) != 0
      || cfsetispeed (ਬਹੁਲਕ_2, ਰਫਤਾਰ[0]) != 0)
    ਵਾਪਸ false;

  ਆਖਰੀ_ibaud.baud = ਰਫਤਾਰ[0] == 0 ? cfgetispeed (ਬਹੁਲਕ_2) : ਰਫਤਾਰ[0];
  ਆਖਰੀ_obaud.baud = ਰਫਤਾਰ[1];
  ਵਾਪਸ true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ਸਥਿਰ bool
ਰਫਤਾਰ_matches (ਬਣਤਰ termios ਅਚਲ *mode1, ਬਣਤਰ termios ਅਚਲ *mode2)
{
  ਰਫਤਾਰ_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  ਰਫਤਾਰ_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  ਜੇ (speed1[1] == 0)
    speed1[1] = speed1[0];
  ਜੇ (speed2[1] == 0)
    speed2[1] = speed2[0];

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੱਟ_2 = 2;  /* Start and stop bits.  */
  ਬਿੱਟ_2 += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  ਬਿੱਟ_2 += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  ਚੋਣ (mode1->c_cflag & CSIZE)
    {
      ਹਾਲਤ CS5: ਬਿੱਟ_2 += 5; ਰੋਕੋ;
      ਹਾਲਤ CS6: ਬਿੱਟ_2 += 6; ਰੋਕੋ;
      ਹਾਲਤ CS7: ਬਿੱਟ_2 += 7; ਰੋਕੋ;
      ਮੂਲ: ਬਿੱਟ_2 += 8; ਰੋਕੋ;
    }

  ਲਈ (ਪੂਰਨ i = 0; i < 2; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ value1;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ value2;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ delta;

      ਜੇ (speed1[i] == speed2[i])
        ਜਾਰੀ;

      value1 = baud_ਮੰਜ਼ਿਲ_ਮੁੱਲ (speed1[i]);
      value2 = baud_ਮੰਜ਼ਿਲ_ਮੁੱਲ (speed2[i]);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ TERMIOS_ਰਫਤਾਰ_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      ਜੇ (value1 == ULONG_MAX || value2 == ULONG_MAX)
        ਵਾਪਸ false;
#ਜੇ_ਅੰਤ

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      ਜੇ (delta > value1 / (ਬਿੱਟ_2 * 4))
        ਵਾਪਸ false;
    }

  ਵਾਪਸ true;
}

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਬਣਤਰ ਰਫਤਾਰ_desc
string_ਮੰਜ਼ਿਲ_baud (ਅੱਖਰ ਅਚਲ *arg)
{
  ਅੱਖਰ *ep;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮੁੱਲ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c;

  /* Explicitly disallow negative numbers.  */
  ਜਦੋਂ (c_isspace (*arg))
    arg++;
  ਜੇ (*arg == '-')
    ਜਾਓ invalid;

  /* Empty does not mean 0.  */
  ਜੇ (! *arg)
    ਜਾਓ invalid;

  ਮੁੱਲ = strtoul (arg, &ep, 10);

  c = *ep++;
  ਜੇ (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      ਜੇ (c)
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ d = c - '0';
          ਜੇ (d > 5)
            ਮੁੱਲ++;
          ਨਹੀਂਤਾਂ ਜੇ (d == 5)
            {
              ਜਦੋਂ ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              ਜੇ (c)
                ਮੁੱਲ++;                /* Nonzero, round up */
              ਨਹੀਂਤਾਂ
                ਮੁੱਲ += (ਮੁੱਲ & 1);   /* Exactly in the middle, round even */
            }

          ਜਦੋਂ (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          ਜੇ (c)
            ਜਾਓ invalid; /* Garbage after otherwise valid number */
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      ਜੇ (streq (arg, "exta"))
        ਵਾਪਸ (ਬਣਤਰ ਰਫਤਾਰ_desc) { .baud = B19200, .valid = true };
      ਨਹੀਂਤਾਂ ਜੇ (streq (arg, "extb"))
        ਵਾਪਸ (ਬਣਤਰ ਰਫਤਾਰ_desc) { .baud = B38400, .valid = true };
      ਨਹੀਂਤਾਂ
        ਜਾਓ invalid;
    }

  ਵਾਪਸ ਮੁੱਲ_ਮੰਜ਼ਿਲ_baud (ਮੁੱਲ);

 invalid:
  ਵਾਪਸ (ਬਣਤਰ ਰਫਤਾਰ_desc) { .baud = (ਰਫਤਾਰ_t) -1, .valid = false };
}

ਸਥਿਰ ਖਾਲੀ
sane_ਬਹੁਲਕ (ਬਣਤਰ termios *ਬਹੁਲਕ_2)
{
  tcflag_t *bitsp;

  ਲਈ (ਪੂਰਨ i = 0; control_info[i].name; ++i)
    {
#ਜੇ VMIN == VEOF
      ਜੇ (streq (control_info[i].name, "min"))
        ਰੋਕੋ;
#ਜੇ_ਅੰਤ
      ਬਹੁਲਕ_2->c_cc[control_info[i].ਵਿੱਥ] = control_info[i].saneval;
    }

  ਲਈ (ਪੂਰਨ i = 0; ਬਹੁਲਕ_info[i].name != ਨਿਰਾ; ++i)
    {
      ਜੇ (ਬਹੁਲਕ_info[i].flags & NO_SETATTR)
        ਜਾਰੀ;

      ਜੇ (ਬਹੁਲਕ_info[i].flags & SANE_ਸੈੱਟ)
        {
          bitsp = ਬਹੁਲਕ_type_flag (ਬਹੁਲਕ_info[i].type, ਬਹੁਲਕ_2);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~ਬਹੁਲਕ_info[i].ਨਕਾਬ) | ਬਹੁਲਕ_info[i].ਬਿੱਟ_2;
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਬਹੁਲਕ_info[i].flags & SANE_UNSET)
        {
          bitsp = ਬਹੁਲਕ_type_flag (ਬਹੁਲਕ_info[i].type, ਬਹੁਲਕ_2);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~ਬਹੁਲਕ_info[i].ਨਕਾਬ & ~ਬਹੁਲਕ_info[i].ਬਿੱਟ_2;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
visible (cc_t ch)
{
  ਸਥਿਰ ਅੱਖਰ buf[10];
  ਅੱਖਰ *bpout = buf;

  ਜੇ (ch == _POSIX_VDISABLE)
    ਵਾਪਸ "<undef>";

  ਜੇ (ch >= 32)
    {
      ਜੇ (ch < 127)
        *bpout++ = ch;
      ਨਹੀਂਤਾਂ ਜੇ (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      ਨਹੀਂਤਾਂ
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          ਜੇ (ch >= 128 + 32)
            {
              ਜੇ (ch < 128 + 127)
                *bpout++ = ch - 128;
              ਨਹੀਂਤਾਂ
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          ਨਹੀਂਤਾਂ
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  ਨਹੀਂਤਾਂ
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  ਵਾਪਸ (ਅੱਖਰ ਅਚਲ *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

ਸਥਿਰ uintmax_t
integer_arg (ਅੱਖਰ ਅਚਲ *s, uintmax_t maxval)
{
  ਵਾਪਸ xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
