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

#ಅನ್ತರ್ಭಾವ <config.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TERMIOS_NEEDS_XOPEN_ಮೂಲ
# define XOPEN_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ <termios.h>
#ಆದರೆ HAVE_STROPTS_H
# include <stropts.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINSIZE_ಒಳ_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GWINSZ_ಒಳ_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fd-reopen.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "stty"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CINTR
# define CINTR Control ('c')
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CQUIT
# define CQUIT 28
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CERASE
# define CERASE 127
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CKILL
# define CKILL Control ('u')
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CEOF
# define CEOF Control ('d')
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CEOL
# define CEOL _POSIX_VDISABLE
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CSTART
# define CSTART Control ('q')
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CSTOP
# define CSTOP Control ('s')
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CSUSP
# define CSUSP Control ('z')
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#ಆದರೆ_ಅಂತ್ಯ
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#ಆದರೆ defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#ಆದರೆ_ಅಂತ್ಯ
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#ಆದರೆ defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# ಆದರೆ defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#ಆದರೆ_ಅಂತ್ಯ

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#ಆದರೆ defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#ಆದರೆ_ಅಂತ್ಯ

/* Which speeds to set.  */
ಎಣಿಕೆ ವೇಗ_setting
  {
    ಒಳಹರಿವು_ವೇಗ, output_ವೇಗ, both_speeds
  };

/* What to output and how.  */
ಎಣಿಕೆ output_type
  {
    ಬದಲಾಯಿತು, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
ಎಣಿಕೆ ಬಹುಲಕ_type
  {
    control, ಒಳಹರಿವು, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#ವ್ಯಾಖ್ಯಾನಿಸು SANE_ಗಣ 1		/* Set in 'sane' mode. */
#ವ್ಯಾಖ್ಯಾನಿಸು SANE_UNSET 2		/* Unset in 'sane' mode. */
#ವ್ಯಾಖ್ಯಾನಿಸು REV 4			/* Can be turned off by prepending '-'. */
#ವ್ಯಾಖ್ಯಾನಿಸು OMIT 8			/* Don't display value. */
#ವ್ಯಾಖ್ಯಾನಿಸು NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
ರಚನೆ ಬಹುಲಕ_info
  {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name;		/* Name given on command line.  */
    ಎಣಿಕೆ ಬಹುಲಕ_type type;	/* Which structure element to change. */
    ಅಕ್ಷರ flags;			/* Setting and display options.  */
    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಬಿಟ್ಟುಗಳು_2;		/* Bits to set for this mode.  */
    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಖವಾಡ;		/* Other bits to turn off for this mode.  */
  };

ಸ್ಥಿರ ರಚನೆ ಬಹುಲಕ_info ಸ್ಥಿರಾಂಕ ಬಹುಲಕ_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#ಆದರೆ_ಅಂತ್ಯ
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_ಗಣ | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#ಆದರೆ_ಅಂತ್ಯ

  {"ignbrk", ಒಳಹರಿವು, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", ಒಳಹರಿವು, SANE_ಗಣ | REV, BRKINT, 0},
  {"ignpar", ಒಳಹರಿವು, REV, IGNPAR, 0},
  {"parmrk", ಒಳಹರಿವು, REV, PARMRK, 0},
  {"inpck", ಒಳಹರಿವು, REV, INPCK, 0},
  {"istrip", ಒಳಹರಿವು, REV, ISTRIP, 0},
  {"inlcr", ಒಳಹರಿವು, SANE_UNSET | REV, INLCR, 0},
  {"igncr", ಒಳಹರಿವು, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", ಒಳಹರಿವು, SANE_ಗಣ | REV, ICRNL, 0},
  {"ixon", ಒಳಹರಿವು, REV, IXON, 0},
  {"ixoff", ಒಳಹರಿವು, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", ಒಳಹರಿವು, REV | OMIT, IXOFF, 0},
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IUCLC
  {"iuclc", ಒಳಹರಿವು, SANE_UNSET | REV, IUCLC, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IXANY
  {"ixany", ಒಳಹರಿವು, SANE_UNSET | REV, IXANY, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IMAXBEL
  {"imaxbel", ಒಳಹರಿವು, SANE_ಗಣ | REV, IMAXBEL, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IUTF8
  {"iutf8", ಒಳಹರಿವು, SANE_UNSET | REV, IUTF8, 0},
#ಆದರೆ_ಅಂತ್ಯ

  {"opost", output, SANE_ಗಣ | REV, OPOST, 0},
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLCR
  {"onlcr", output, SANE_ಗಣ | REV, ONLCR, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_ಗಣ, NL0, NLDLY},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_ಗಣ, CR0, CRDLY},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TABDLY
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
  {"tab0", output, SANE_ಗಣ, TAB0, TABDLY},
# endif
#ಇಲ್ಲದಿದ್ದರೆ
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_ಗಣ, BS0, BSDLY},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_ಗಣ, VT0, VTDLY},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_ಗಣ, FF0, FFDLY},
#ಆದರೆ_ಅಂತ್ಯ

  {"isig", local, SANE_ಗಣ | REV, ISIG, 0},
  {"icanon", local, SANE_ಗಣ | REV, ICANON, 0},
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IEXTEN
  {"iexten", local, SANE_ಗಣ | REV, IEXTEN, 0},
#ಆದರೆ_ಅಂತ್ಯ
  {"echo", local, SANE_ಗಣ | REV, ECHO, 0},
  {"echoe", local, SANE_ಗಣ | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_ಗಣ | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOCTL
  {"echoctl", local, SANE_ಗಣ | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOKE
  {"echoke", local, SANE_ಗಣ | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#ಅಥವಾಆದರೆ defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#ಆದರೆ_ಅಂತ್ಯ

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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#ಆದರೆ_ಅಂತ್ಯ
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {ಖಾಲಿ, control, 0, 0, 0}
};

/* Control character settings.  */
ರಚನೆ control_info
  {
    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t ಸ್ಥಾನಾಂತರ;		/* Offset in c_cc.  */
  };

/* Control characters. */

ಸ್ಥಿರ ರಚನೆ control_info ಸ್ಥಿರಾಂಕ control_info[] =
{
  {"intr", CINTR, VINTR},
  {"quit", CQUIT, VQUIT},
  {"erase", CERASE, VERASE},
  {"kill", CKILL, VKILL},
  {"eof", CEOF, VEOF},
  {"eol", CEOL, VEOL},
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VEOL2
  {"eol2", CEOL2, VEOL2},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#ಆದರೆ_ಅಂತ್ಯ
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#ಇಲ್ಲದಿದ್ದರೆ
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VWERASE
  {"werase", CWERASE, VWERASE},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VSTATUS
  {"status", CSTATUS, VSTATUS},
#ಆದರೆ_ಅಂತ್ಯ

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {ಖಾಲಿ, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
ರಚನೆ ವೇಗ_desc
{
  /* The baud rate.  */
  ವೇಗ_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *visible (cc_t ch);
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ baud_ಗಮ್ಯ_ಮೌಲ್ಯ (ವೇಗ_t ವೇಗ);
ಸ್ಥಿರ bool recover_ಬಹುಲಕ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, ರಚನೆ termios *ಬಹುಲಕ_2);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ screen_columns (ಶೂನ್ಯ);
ಸ್ಥಿರ bool ಗಣ_ಬಹುಲಕ (ರಚನೆ ಬಹುಲಕ_info ಸ್ಥಿರಾಂಕ *info, bool reversed,
                      ರಚನೆ termios *ಬಹುಲಕ_2);
ಸ್ಥಿರ bool eq_ಬಹುಲಕ (ರಚನೆ termios ಸ್ಥಿರಾಂಕ *mode1,
                     ರಚನೆ termios ಸ್ಥಿರಾಂಕ *mode2);
ಸ್ಥಿರ bool ವೇಗ_matches (ರಚನೆ termios ಸ್ಥಿರಾಂಕ *mode1,
                           ರಚನೆ termios ಸ್ಥಿರಾಂಕ *mode2);
ಸ್ಥಿರ uintmax_t integer_arg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, uintmax_t max);
ಸ್ಥಿರ ರಚನೆ ವೇಗ_desc string_ಗಮ್ಯ_baud (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg);
ಸ್ಥಿರ tcflag_t *ಬಹುಲಕ_type_flag (ಎಣಿಕೆ ಬಹುಲಕ_type type, ರಚನೆ termios *ಬಹುಲಕ_2);
ಸ್ಥಿರ ಶೂನ್ಯ display_all (ರಚನೆ termios *ಬಹುಲಕ_2, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name);
ಸ್ಥಿರ ಶೂನ್ಯ display_ಬದಲಾಯಿತು (ರಚನೆ termios *ಬಹುಲಕ_2);
ಸ್ಥಿರ ಶೂನ್ಯ display_recoverable (ರಚನೆ termios *ಬಹುಲಕ_2);
ಸ್ಥಿರ ಶೂನ್ಯ display_settings (ಎಣಿಕೆ output_type output_type,
                              ರಚನೆ termios *ಬಹುಲಕ_2,
                              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name);
ಸ್ಥಿರ ಶೂನ್ಯ check_ವೇಗ (ರಚನೆ termios *ಬಹುಲಕ_2);
ಸ್ಥಿರ ಶೂನ್ಯ display_ವೇಗ (ರಚನೆ termios *ಬಹುಲಕ_2, bool fancy);
ಸ್ಥಿರ ಶೂನ್ಯ display_ಕಿಟಕಿ_ಗಾತ್ರ (bool fancy, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name);
ಸ್ಥಿರ ಶೂನ್ಯ sane_ಬಹುಲಕ (ರಚನೆ termios *ಬಹುಲಕ_2);
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_control_char (ರಚನೆ control_info ಸ್ಥಿರಾಂಕ *info,
                              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg,
                              ರಚನೆ termios *ಬಹುಲಕ_2);
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ವೇಗ (ಎಣಿಕೆ ವೇಗ_setting type, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg,
                       ರಚನೆ ವೇಗ_desc ಸ್ಥಿರಾಂಕ ವೇಗ, ರಚನೆ termios *ಬಹುಲಕ_2);
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ಕಿಟಕಿ_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name);

/* The width of the screen, for output wrapping. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ max_ಲಂಬಸಾಲು;

/* Current position, to know when to wrap. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ current_ಲಂಬಸಾಲು;

/* Default "drain" mode for tcsetattr.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ಸ್ಥಿರ bool dev_debug;

/* Record last speed set for correlation.  */
ಸ್ಥಿರ ರಚನೆ ವೇಗ_desc ಕೊನೆಯ_ibaud =
  { .baud = (ವೇಗ_t) -1, .valid = false };
ಸ್ಥಿರ ರಚನೆ ವೇಗ_desc ಕೊನೆಯ_obaud =
  { .baud = (ವೇಗ_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"all", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"save", no_ವಾದ, ಖಾಲಿ, 'g'},
  {"file", required_ವಾದ, ಖಾಲಿ, 'F'},
  {"-debug", no_ವಾದ, ಖಾಲಿ, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((ಬರೆ, 1, 2))
ಸ್ಥಿರ ಶೂನ್ಯ
wrapf (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_2,...)
{
  va_list ವಾದಗಳು;
  ಅಕ್ಷರ *buf;
  ಪೂರ್ಣಾಂಕ buflen;

  va_ಆರಂಭ (ವಾದಗಳು, ಸಂದೇಶ_2);
  buflen = vasprintf (&buf, ಸಂದೇಶ_2, ವಾದಗಳು);
  va_ಕೊನೆ (ವಾದಗಳು);

  ಆದರೆ (buflen < 0)
    xalloc_die ();

  ಆದರೆ (0 < current_ಲಂಬಸಾಲು)
    {
      ಆದರೆ (max_ಲಂಬಸಾಲು - current_ಲಂಬಸಾಲು <= buflen)
        {
          ಅಕ್ಷರ_ಬರೆ ('\n');
          current_ಲಂಬಸಾಲು = 0;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಅಕ್ಷರ_ಬರೆ (' ');
          current_ಲಂಬಸಾಲು++;
        }
    }

  fputs (buf, stdout);
  ಬಿಡುಗಡೆ (buf);
  current_ಲಂಬಸಾಲು += buflen;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print or change terminal characteristics.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings ಒಳ human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings ಒಳ a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  open and use DEVICE instead ದ standard ಒಳಹರಿವು\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system ನಿರೂಪಿಸುತ್ತದೆ which settings are ಲಭ್ಯ.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding ದ output\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop ಸಂಕೇತ once ಒಳಹರಿವು flushed\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   eof CHAR      CHAR will send an ಕೊನೆ ದ file (terminate the ಒಳಹರಿವು)\n\
   eol CHAR      CHAR will ಕೊನೆ the ಸಾಲು\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR ಗಾಗಿ ending the ಸಾಲು\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   erase CHAR    CHAR will erase the ಕೊನೆಯ character typed\n\
   intr CHAR     CHAR will send an ಅಡಚಣೆ ಸಂಕೇತ\n\
   kill CHAR     CHAR will erase the current ಸಾಲು\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the ಮುಂದಿನ character quoted\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VSTATUS
      fputs (_("\
 * ಸ್ಥಿತಿ_2 CHAR   CHAR will send an info ಸಂಕೇತ\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   quit CHAR     CHAR will send a quit ಸಂಕೇತ\n\
"), stdout);
#ಆದರೆ defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current ಸಾಲು\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   ಆರಂಭ CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop ಸಂಕೇತ\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will ಆಯ್ಕೆ ಗಮ್ಯ a different shell layer\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the ಕೊನೆಯ ಶಬ್ದ_2 typed\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
\n\
Special settings:\n\
   N             ಗಣ_2 the ಒಳಹರಿವು and output speeds ಗಮ್ಯ N bauds\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIOCGWINSZ
      fputs (_("\
   ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 N        tell the kernel that the terminal has N columns\n\
 * columns N     same as ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 N\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      ಬರೆ (_("\
 * [-]drain      wait ಗಾಗಿ ಸಂವಹನ before applying settings (%s by ಪೂರ್ವನಿಯೋಜಿತ)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      ಗಣ_2 the ಒಳಹರಿವು ವೇಗ ಗಮ್ಯ N\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_C_ಸಾಲು
      fputs (_("\
 * ಸಾಲು N        use ಸಾಲು discipline N\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   min N         with -icanon, ಗಣ_2 N characters minimum ಗಾಗಿ a completed ಪಠ\n\
   ospeed N      ಗಣ_2 the output ವೇಗ ಗಮ್ಯ N\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIOCGWINSZ
      fputs (_("\
   ಅಡ್ಡಸಾಲುಗಳು N        tell the kernel that the terminal has N ಅಡ್ಡಸಾಲುಗಳು\n\
   ಗಾತ್ರ_2          print the ಸಂಖ್ಯೆ ದ ಅಡ್ಡಸಾಲುಗಳು and columns according ಗಮ್ಯ the kernel\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   ವೇಗ         print the terminal ವೇಗ\n\
   ಸಮಯ N        with -icanon, ಗಣ_2 ಪಠ timeout ದ N tenths ದ a ಎರಡನೇ\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control ಸಂಕೇತಗಳು\n\
   [-]cread      allow ಒಳಹರಿವು ಗಮ್ಯ be received\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   csN           ಗಣ_2 character ಗಾತ್ರ_2 ಗಮ್ಯ N ಬಿಟ್ಟುಗಳು_2, N ಒಳ [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop ಬಿಟ್ಟುಗಳು_2 ಪ್ರತಿ character (one with '-')\n\
   [-]hup        send a hangup ಸಂಕೇತ when the ಕೊನೆಯ process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity ಬಿಟ್ ಒಳ output and expect parity ಬಿಟ್ ಒಳ ಒಳಹರಿವು\n\
   [-]parodd     ಗಣ_2 odd parity (or even parity with '-')\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
\n\
ಒಳಹರಿವು_3 settings:\n\
   [-]brkint     breaks cause an ಅಡಚಣೆ ಸಂಕೇತ\n\
   [-]icrnl      translate carriage ಹಿಂತಿರುಗಿ ಗಮ್ಯ newline\n\
   [-]ignbrk     ignore ನಿಲ್ಲಿಸು characters\n\
   [-]igncr      ignore carriage ಹಿಂತಿರುಗಿ\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and ಮಾಡು not flush a ತುಂಬಿದ ಒಳಹರಿವು ಬಫರ್ on a character\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   [-]inlcr      translate newline ಗಮ್ಯ carriage ಹಿಂತಿರುಗಿ\n\
   [-]inpck      enable ಒಳಹರಿವು parity checking\n\
   [-]istrip     clear ಮೇಲಿನ (8th) ಬಿಟ್ ದ ಒಳಹರಿವು characters\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IUTF8
      fputs (_("\
 * [-]iutf8      assume ಒಳಹರಿವು characters are UTF-8 encoded\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters ಗಮ್ಯ lowercase\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only ಆರಂಭ character\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   [-]ixoff      enable sending ದ ಆರಂಭ/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BSDLY
      fputs (_("\
 * bsN           backspace delay style, N ಒಳ [0..1]\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CRDLY
      fputs (_("\
 * crN           carriage ಹಿಂತಿರುಗಿ delay style, N ಒಳ [0..3]\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FFDLY
      fputs (_("\
 * ffN           form feed delay style, N ಒಳ [0..1]\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NLDLY
      fputs (_("\
 * nlN           newline delay style, N ಒಳ [0..1]\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage ಹಿಂತಿರುಗಿ ಗಮ್ಯ newline\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters ಗಾಗಿ fill instead ದ NUL characters\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead ದ timing ಗಾಗಿ delays\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters ಗಮ್ಯ uppercase\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLCR
      fputs (_("\
 * [-]onlcr      translate newline ಗಮ್ಯ carriage ಹಿಂತಿರುಗಿ-newline\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage ಹಿಂತಿರುಗಿ\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONOCR
      fputs (_("\
 * [-]onocr      ಮಾಡು not print carriage returns ಒಳ the ಮೊದಲ ಸ್ತಂಭ\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#ಆದರೆ defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N ಒಳ [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N ಒಳ [0..1]\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOKE
      fputs (_("\
 * crtkill       kill all ಸಾಲು by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all ಸಾಲು by obeying the echoctl and echok settings\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters ಒಳ hat notation ('^c')\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   [-]echo       echo ಒಳಹರಿವು characters\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   [-]echonl     echo newline even ಆದರೆ not echoing other characters\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with ಮೇಲಿನ latency links\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      ಬರೆ (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VWERASE
    ", werase"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined CREPRINT || defined VREPRINT
    ", rprnt"
#ಆದರೆ_ಅಂತ್ಯ
);
      fputs (_("\
   [-]isig       enable ಅಡಚಣೆ, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after ಅಡಚಣೆ and quit special characters\n\
"), stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try ಗಮ್ಯ ಲಿಖ ಗಮ್ಯ the terminal\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' ಗಾಗಿ uppercase characters\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#ಆದರೆ defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters ಗಮ್ಯ their ಪೂರ್ವನಿಯೋಜಿತ ಮೌಲ್ಯಗಳು\n\
   -cooked       same as raw\n\
"), stdout);
      ಬರೆ (_("\
   crt           same as %s\n\
"), "echoe"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOCTL
    " echoctl"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOKE
    " echoke"
#ಆದರೆ_ಅಂತ್ಯ
);
      ಬರೆ (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOCTL
    " echoctl"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOKE
    " echoke"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IXANY
    " -ixany"
#ಆದರೆ_ಅಂತ್ಯ
);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   ek            erase and kill characters ಗಮ್ಯ their ಪೂರ್ವನಿಯೋಜಿತ ಮೌಲ್ಯಗಳು\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#ಆದರೆ defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#ಆದರೆ_ಅಂತ್ಯ
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      ಬರೆ (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLCR
   " -onlcr"
#ಆದರೆ_ಅಂತ್ಯ
  , "icrnl -inlcr -igncr"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLCR
   " onlcr"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OCRNL
   " -ocrnl"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLRET
   " -onlret"
#ಆದರೆ_ಅಂತ್ಯ
);
      fputs (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), stdout);
      ಬರೆ (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 ಸಮಯ 0\n\
   -raw          same as cooked\n\
"),
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IUCLC
   " -iuclc"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IXANY
   " -ixany"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IMAXBEL
   " -imaxbel"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ XCASE
   " -xcase"
#ಆದರೆ_ಅಂತ್ಯ
);
      ಬರೆ (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters ಗಮ್ಯ their ಪೂರ್ವನಿಯೋಜಿತ ಮೌಲ್ಯಗಳು\n\
"),
   "-ixoff"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IUTF8
   " -iutf8"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IUCLC
   " -iuclc"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IXANY
   " -ixany"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IMAXBEL
   " imaxbel"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ XCASE
   " -xcase"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OLCUC
   " -olcuc"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OCRNL
   " -ocrnl"
#ಆದರೆ_ಅಂತ್ಯ

 , "opost"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OFILL
   " -ofill"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLCR
   " onlcr"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONOCR
   " -onocr"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLRET
   " -onlret"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NLDLY
   " nl0"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CRDLY
   " cr0"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TAB0
   " tab0"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BSDLY
   " bs0"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VTDLY
   " vt0"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FFDLY
   " ff0"
#ಆದರೆ_ಅಂತ್ಯ

 , "isig"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TOSTOP
   " -tostop"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OFDEL
   " -ofdel"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOPRT
   " -echoprt"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOCTL
   " echoctl"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOKE
   " echoke"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EXTPROC
   " -extproc"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FLUSHO
   " -flusho"
#ಆದರೆ_ಅಂತ್ಯ
);
      fputs (_("\
\n\
Handle the tty ಸಾಲು connected ಗಮ್ಯ standard ಒಳಹರಿವು.  Without arguments,\n\
prints baud ದರ, ಸಾಲು discipline, and deviations from stty sane.  ಒಳ_2\n\
settings, CHAR is ತೆಗೆದುಕೊಂಡ literally, or coded as ಒಳ ^c, 0x37, 0177 or\n\
127; special ಮೌಲ್ಯಗಳು ^- or undef ಬಳಸಿದ ಗಮ್ಯ disable special characters.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

ಸ್ಥಿರ ಶೂನ್ಯ
apply_settings (bool checking, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name,
                ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ *settings, ಪೂರ್ಣಾಂಕ n_settings,
                ರಚನೆ termios *ಬಹುಲಕ_2, bool *require_ಗಣ_attr)
{
#ವ್ಯಾಖ್ಯಾನಿಸು check_ವಾದ(arg)						\
  ಮಾಡು									\
    {									\
      ಆದರೆ (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  ಆಗ (0)

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 1; k < n_settings; k++)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg = settings[k];
      bool ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = false;
      bool not_ಗಣ_attr = false;
      bool reversed = false;

      ಆದರೆ (! arg)
        ಮುಂದುವರಿ;

      ಆದರೆ (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      ಆದರೆ (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          ಮುಂದುವರಿ;
        }
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; ಬಹುಲಕ_info[i].name != ಖಾಲಿ; ++i)
        {
          ಆದರೆ (streq (arg, ಬಹುಲಕ_info[i].name))
            {
              ಆದರೆ ((ಬಹುಲಕ_info[i].flags & NO_SETATTR) == 0)
                {
                  ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = ಗಣ_ಬಹುಲಕ (&ಬಹುಲಕ_info[i], reversed, ಬಹುಲಕ_2);
                  *require_ಗಣ_attr = true;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = not_ಗಣ_attr = true;
              ನಿಲ್ಲಿಸು;
            }
        }
      ಆದರೆ (!ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      ಆದರೆ (!ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು)
        {
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; control_info[i].name != ಖಾಲಿ; ++i)
            {
              ಆದರೆ (streq (arg, control_info[i].name))
                {
                  check_ವಾದ (arg);
                  ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = true;
                  ++k;
                  ಗಣ_control_char (&control_info[i], settings[k], ಬಹುಲಕ_2);
                  *require_ಗಣ_attr = true;
                  ನಿಲ್ಲಿಸು;
                }
            }
        }
      ಆದರೆ (!ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು || not_ಗಣ_attr)
        {
          ಆದರೆ (streq (arg, "ispeed"))
            {
              check_ವಾದ (arg);
              ++k;
              ರಚನೆ ವೇಗ_desc ಸ್ಥಿರಾಂಕ ispeed = string_ಗಮ್ಯ_baud (settings[k]);
              ಆದರೆ (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              ಗಣ_ವೇಗ (ಒಳಹರಿವು_ವೇಗ, settings[k], ispeed, ಬಹುಲಕ_2);
              ಆದರೆ (checking)
                ಮುಂದುವರಿ;
              *require_ಗಣ_attr = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (arg, "ospeed"))
            {
              check_ವಾದ (arg);
              ++k;
              ರಚನೆ ವೇಗ_desc ಸ್ಥಿರಾಂಕ ospeed = string_ಗಮ್ಯ_baud (settings[k]);
              ಆದರೆ (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              ಗಣ_ವೇಗ (output_ವೇಗ, settings[k], ospeed, ಬಹುಲಕ_2);
              ಆದರೆ (checking)
                ಮುಂದುವರಿ;
              *require_ಗಣ_attr = true;
            }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (arg, "extproc"))
            {
              ಪೂರ್ಣಾಂಕ val = ! reversed;

              ಆದರೆ (checking)
                ಮುಂದುವರಿ;

              ಆದರೆ (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIOCGWINSZ
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (arg, "rows"))
            {
              check_ವಾದ (arg);
              ++k;
              ಆದರೆ (checking)
                ಮುಂದುವರಿ;
              ಗಣ_ಕಿಟಕಿ_ಗಾತ್ರ (integer_arg (settings[k], ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ), -1,
                               device_name);
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_ವಾದ (arg);
              ++k;
              ಆದರೆ (checking)
                ಮುಂದುವರಿ;
              ಗಣ_ಕಿಟಕಿ_ಗಾತ್ರ (-1, integer_arg (settings[k], ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ),
                               device_name);
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (arg, "size"))
            {
              ಆದರೆ (checking)
                ಮುಂದುವರಿ;
              max_ಲಂಬಸಾಲು = screen_columns ();
              current_ಲಂಬಸಾಲು = 0;
              display_ಕಿಟಕಿ_ಗಾತ್ರ (false, device_name);
            }
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_C_ಸಾಲು
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (arg, "line"))
            {
              check_ವಾದ (arg);
              ++k;
              uintmax_t ಮೌಲ್ಯ = integer_arg (settings[k], UINTMAX_MAX);
              ಆದರೆ (ckd_ಕೂಡಿಸು (&ಬಹುಲಕ_2->c_ಸಾಲು, ಮೌಲ್ಯ, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_ಗಣ_attr = true;
            }
#ಆದರೆ_ಅಂತ್ಯ
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (arg, "speed"))
            {
              ಆದರೆ (checking)
                ಮುಂದುವರಿ;
              max_ಲಂಬಸಾಲು = screen_columns ();
              display_ವೇಗ (ಬಹುಲಕ_2, false);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ರಚನೆ ವೇಗ_desc ಸ್ಥಿರಾಂಕ ವೇಗ = string_ಗಮ್ಯ_baud (arg);
              ಆದರೆ (ವೇಗ.valid)
                {
                  ಗಣ_ವೇಗ (both_speeds, arg, ವೇಗ, ಬಹುಲಕ_2);
                  ಆದರೆ (checking)
                    ಮುಂದುವರಿ;
                  *require_ಗಣ_attr = true;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಆದರೆ (! recover_ಬಹುಲಕ (arg, ಬಹುಲಕ_2))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_ಗಣ_attr = true;
                }
            }
        }
    }

  ಆದರೆ (checking)
    check_ವೇಗ (ಬಹುಲಕ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ಸ್ಥಿರ ರಚನೆ termios ಬಹುಲಕ_2;

  ಎಣಿಕೆ output_type output_type;
  ಪೂರ್ಣಾಂಕ optc;
  ಪೂರ್ಣಾಂಕ argi = 0;
  ಪೂರ್ಣಾಂಕ opti = 1;
  bool require_ಗಣ_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  ಅಕ್ಷರ *file_name = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = ಬದಲಾಯಿತು;
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

  ಆಗ ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'a':
          verbose_output = true;
          output_type = all;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'g':
          recoverable_output = true;
          output_type = recoverable;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'F':
          ಆದರೆ (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEV_DEBUG_OPTION:
          dev_debug = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          ಆದರೆ (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          ನಿಲ್ಲಿಸು;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      ಆಗ (opti < optind)
        argv[argi + opti++] = ಖಾಲಿ;
    }

  /* Specifying both -a and -g gets an error.  */
  ಆದರೆ (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  ಆದರೆ (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  ಆದರೆ (!noargs && !verbose_output && !recoverable_output)
    {
      ಸ್ಥಿರ ರಚನೆ termios check_ಬಹುಲಕ;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_ಬಹುಲಕ, &require_ಗಣ_attr);
    }

  ಆದರೆ (file_name)
    {
      ಪೂರ್ಣಾಂಕ fdflags;
      ಆದರೆ (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ಆದರೆ ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  ಆದರೆ (tcgetattr (STDIN_FILENO, &ಬಹುಲಕ_2))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  ಆದರೆ (verbose_output || recoverable_output || noargs)
    {
      max_ಲಂಬಸಾಲು = screen_columns ();
      current_ಲಂಬಸಾಲು = 0;
      display_settings (output_type, &ಬಹುಲಕ_2, device_name);
      ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
    }

  require_ಗಣ_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &ಬಹುಲಕ_2, &require_ಗಣ_attr);

  ಆದರೆ (require_ಗಣ_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ಸ್ಥಿರ ರಚನೆ termios new_ಬಹುಲಕ;

      ಆದರೆ (tcsetattr (STDIN_FILENO, tcsetattr_options, &ಬಹುಲಕ_2))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      ಆದರೆ (tcgetattr (STDIN_FILENO, &new_ಬಹುಲಕ))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ಆದರೆ (! eq_ಬಹುಲಕ (&ಬಹುಲಕ_2, &new_ಬಹುಲಕ))
        {
          ಆದರೆ (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ (new_ಬಹುಲಕ); i++)
                {
                  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ newc = *(((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) &new_ಬಹುಲಕ) + i);
                  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ oldc = *(((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) &ಬಹುಲಕ_2) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
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
ಸ್ಥಿರ bool
eq_ಬಹುಲಕ (ರಚನೆ termios ಸ್ಥಿರಾಂಕ *mode1, ರಚನೆ termios ಸ್ಥಿರಾಂಕ *mode2)
{
  ಆದರೆ (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_C_ಸಾಲು
      || mode1->c_ಸಾಲು != mode2->c_ಸಾಲು
#ಆದರೆ_ಅಂತ್ಯ
      || !memeq (mode1->c_cc, mode2->c_cc, ಗಾತ್ರ (mode1->c_cc)))
    ಹಿಂತಿರುಗಿ false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_ಮುಖವಾಡ = ~(tcflag_t) 0;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CBAUD
  cflag_ಮುಖವಾಡ &= ~(tcflag_t) CBAUD;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CIBAUD
  cflag_ಮುಖವಾಡ &= ~(tcflag_t) CIBAUD;
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ ((mode1->c_cflag ^ mode2->c_cflag) & cflag_ಮುಖವಾಡ)
    ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ ವೇಗ_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ಸ್ಥಿರ bool
ಗಣ_ಬಹುಲಕ (ರಚನೆ ಬಹುಲಕ_info ಸ್ಥಿರಾಂಕ *info, bool reversed, ರಚನೆ termios *ಬಹುಲಕ_2)
{
  tcflag_t *bitsp;

  ಆದರೆ (reversed && (info->flags & REV) == 0)
    ಹಿಂತಿರುಗಿ false;

  bitsp = ಬಹುಲಕ_type_flag (info->type, ಬಹುಲಕ_2);

  ಆದರೆ (bitsp == ಖಾಲಿ)
    {
      /* Combination mode. */
      ಆದರೆ (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          ಆದರೆ (reversed)
            ಬಹುಲಕ_2->c_cflag = (ಬಹುಲಕ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ಇಲ್ಲದಿದ್ದರೆ
            ಬಹುಲಕ_2->c_cflag = (ಬಹುಲಕ_2->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "oddp"))
        {
          ಆದರೆ (reversed)
            ಬಹುಲಕ_2->c_cflag = (ಬಹುಲಕ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ಇಲ್ಲದಿದ್ದರೆ
            ಬಹುಲಕ_2->c_cflag = (ಬಹುಲಕ_2->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "nl"))
        {
          ಆದರೆ (reversed)
            {
              ಬಹುಲಕ_2->c_iflag = (ಬಹುಲಕ_2->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              ಬಹುಲಕ_2->c_oflag = (ಬಹುಲಕ_2->c_oflag
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLCR
                               | ONLCR
#ಆದರೆ_ಅಂತ್ಯ
                )
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OCRNL
                & ~OCRNL
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLRET
                & ~ONLRET
#ಆದರೆ_ಅಂತ್ಯ
                ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಬಹುಲಕ_2->c_iflag = ಬಹುಲಕ_2->c_iflag & ~ICRNL;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ONLCR
              ಬಹುಲಕ_2->c_oflag = ಬಹುಲಕ_2->c_oflag & ~ONLCR;
#ಆದರೆ_ಅಂತ್ಯ
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "ek"))
        {
          ಬಹುಲಕ_2->c_cc[VERASE] = CERASE;
          ಬಹುಲಕ_2->c_cc[VKILL] = CKILL;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "sane"))
        sane_ಬಹುಲಕ (ಬಹುಲಕ_2);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "cbreak"))
        {
          ಆದರೆ (reversed)
            ಬಹುಲಕ_2->c_lflag |= ICANON;
          ಇಲ್ಲದಿದ್ದರೆ
            ಬಹುಲಕ_2->c_lflag &= ~ICANON;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "pass8"))
        {
          ಆದರೆ (reversed)
            {
              ಬಹುಲಕ_2->c_cflag = (ಬಹುಲಕ_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              ಬಹುಲಕ_2->c_iflag |= ISTRIP;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಬಹುಲಕ_2->c_cflag = (ಬಹುಲಕ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              ಬಹುಲಕ_2->c_iflag &= ~ISTRIP;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "litout"))
        {
          ಆದರೆ (reversed)
            {
              ಬಹುಲಕ_2->c_cflag = (ಬಹುಲಕ_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              ಬಹುಲಕ_2->c_iflag |= ISTRIP;
              ಬಹುಲಕ_2->c_oflag |= OPOST;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಬಹುಲಕ_2->c_cflag = (ಬಹುಲಕ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              ಬಹುಲಕ_2->c_iflag &= ~ISTRIP;
              ಬಹುಲಕ_2->c_oflag &= ~OPOST;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          ಆದರೆ ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              ಬಹುಲಕ_2->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              ಬಹುಲಕ_2->c_oflag |= OPOST;
              ಬಹುಲಕ_2->c_lflag |= ISIG | ICANON;
#ಆದರೆ VMIN == VEOF
              ಬಹುಲಕ_2->c_cc[VEOF] = CEOF;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ VTIME == VEOL
              ಬಹುಲಕ_2->c_cc[VEOL] = CEOL;
#ಆದರೆ_ಅಂತ್ಯ
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Raw mode. */
              ಬಹುಲಕ_2->c_iflag = 0;
              ಬಹುಲಕ_2->c_oflag &= ~OPOST;
              ಬಹುಲಕ_2->c_lflag &= ~(ISIG | ICANON
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ XCASE
                                 | XCASE
#ಆದರೆ_ಅಂತ್ಯ
                );
              ಬಹುಲಕ_2->c_cc[VMIN] = 1;
              ಬಹುಲಕ_2->c_cc[VTIME] = 0;
            }
        }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IXANY
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "decctlq"))
        {
          ಆದರೆ (reversed)
            ಬಹುಲಕ_2->c_iflag |= IXANY;
          ಇಲ್ಲದಿದ್ದರೆ
            ಬಹುಲಕ_2->c_iflag &= ~IXANY;
        }
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TABDLY
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "tabs"))
        {
          ಆದರೆ (reversed)
            ಬಹುಲಕ_2->c_oflag = (ಬಹುಲಕ_2->c_oflag & ~TABDLY) | TAB3;
          ಇಲ್ಲದಿದ್ದರೆ
            ಬಹುಲಕ_2->c_oflag = (ಬಹುಲಕ_2->c_oflag & ~TABDLY) | TAB0;
        }
#ಇಲ್ಲದಿದ್ದರೆ
# ifdef OXTABS
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "tabs"))
        {
          ಆದರೆ (reversed)
            ಬಹುಲಕ_2->c_oflag = ಬಹುಲಕ_2->c_oflag | OXTABS;
          ಇಲ್ಲದಿದ್ದರೆ
            ಬಹುಲಕ_2->c_oflag = ಬಹುಲಕ_2->c_oflag & ~OXTABS;
        }
# endif
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined XCASE && defined IUCLC && defined OLCUC
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          ಆದರೆ (reversed)
            {
              ಬಹುಲಕ_2->c_lflag &= ~XCASE;
              ಬಹುಲಕ_2->c_iflag &= ~IUCLC;
              ಬಹುಲಕ_2->c_oflag &= ~OLCUC;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಬಹುಲಕ_2->c_lflag |= XCASE;
              ಬಹುಲಕ_2->c_iflag |= IUCLC;
              ಬಹುಲಕ_2->c_oflag |= OLCUC;
            }
        }
#ಆದರೆ_ಅಂತ್ಯ
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "crt"))
        ಬಹುಲಕ_2->c_lflag |= ECHOE
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOCTL
          | ECHOCTL
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOKE
          | ECHOKE
#ಆದರೆ_ಅಂತ್ಯ
          ;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (info->name, "dec"))
        {
          ಬಹುಲಕ_2->c_cc[VINTR] = 3;	/* ^C */
          ಬಹುಲಕ_2->c_cc[VERASE] = 127;	/* DEL */
          ಬಹುಲಕ_2->c_cc[VKILL] = 21;	/* ^U */
          ಬಹುಲಕ_2->c_lflag |= ECHOE
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOCTL
            | ECHOCTL
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ECHOKE
            | ECHOKE
#ಆದರೆ_ಅಂತ್ಯ
            ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ IXANY
          ಬಹುಲಕ_2->c_iflag &= ~IXANY;
#ಆದರೆ_ಅಂತ್ಯ
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (reversed)
    *bitsp = *bitsp & ~info->ಮುಖವಾಡ & ~info->ಬಿಟ್ಟುಗಳು_2;
  ಇಲ್ಲದಿದ್ದರೆ
    *bitsp = (*bitsp & ~info->ಮುಖವಾಡ) | info->ಬಿಟ್ಟುಗಳು_2;

  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_control_char (ರಚನೆ control_info ಸ್ಥಿರಾಂಕ *info, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg,
                  ರಚನೆ termios *ಬಹುಲಕ_2)
{
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;

  ಆದರೆ (streq (info->name, "min") || streq (info->name, "time"))
    ಮೌಲ್ಯ = integer_arg (arg, TYPE_ಗರಿಷ್ಠ (cc_t));
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg[0] == '\0' || arg[1] == '\0')
    ಮೌಲ್ಯ = ಗಮ್ಯ_uchar (arg[0]);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (arg, "^-") || streq (arg, "undef"))
    ಮೌಲ್ಯ = _POSIX_VDISABLE;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      ಆದರೆ (arg[1] == '?')
        ಮೌಲ್ಯ = 127;
      ಇಲ್ಲದಿದ್ದರೆ
        ಮೌಲ್ಯ = ಗಮ್ಯ_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಮೌಲ್ಯ = integer_arg (arg, TYPE_ಗರಿಷ್ಠ (cc_t));
  ಬಹುಲಕ_2->c_cc[info->ಸ್ಥಾನಾಂತರ] = ಮೌಲ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ವೇಗ (ಎಣಿಕೆ ವೇಗ_setting type, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg,
           ರಚನೆ ವೇಗ_desc ಸ್ಥಿರಾಂಕ ವೇಗ, ರಚನೆ termios *ಬಹುಲಕ_2)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (ವೇಗ.valid);

  ಆದರೆ (type == ಒಳಹರಿವು_ವೇಗ || type == both_speeds)
    {
      ಕೊನೆಯ_ibaud = ವೇಗ;
      ಆದರೆ (cfsetispeed (ಬಹುಲಕ_2, ವೇಗ.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  ಆದರೆ (type == output_ವೇಗ || type == both_speeds)
    {
      ಕೊನೆಯ_obaud = ವೇಗ;
      ಆದರೆ (cfsetospeed (ಬಹುಲಕ_2, ವೇಗ.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIOCGWINSZ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_win_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ fd, ರಚನೆ winsize *win)
{
  ಪೂರ್ಣಾಂಕ err = ioctl (fd, TIOCGWINSZ, (ಅಕ್ಷರ *) win);
  ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ಕಿಟಕಿ_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name)
{
  ರಚನೆ winsize win;

  ಆದರೆ (get_win_ಗಾತ್ರ (STDIN_FILENO, &win))
    {
      ಆದರೆ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, ಗಾತ್ರ (win));
    }

  ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು >= 0)
    win.ws_ಅಡ್ಡಸಾಲು = ಅಡ್ಡಸಾಲುಗಳು;
  ಆದರೆ (ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 >= 0)
    win.ws_ಲಂಬಸಾಲು = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2;

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

  ಆದರೆ (win.ws_ಅಡ್ಡಸಾಲು == 0 || win.ws_ಲಂಬಸಾಲು == 0)
    {
      ರಚನೆ ttysize ttysz;

      ttysz.ts_ಸಾಲುಗಳು = win.ws_ಅಡ್ಡಸಾಲು;
      ttysz.ts_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = win.ws_ಲಂಬಸಾಲು;

      win.ws_ಅಡ್ಡಸಾಲು = 1;
      win.ws_ಲಂಬಸಾಲು = 1;

      ಆದರೆ (ioctl (STDIN_FILENO, TIOCSWINSZ, (ಅಕ್ಷರ *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ಆದರೆ (ioctl (STDIN_FILENO, TIOCSSIZE, (ಅಕ್ಷರ *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ಹಿಂತಿರುಗಿ;
    }
# endif

  ಆದರೆ (ioctl (STDIN_FILENO, TIOCSWINSZ, (ಅಕ್ಷರ *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_ಕಿಟಕಿ_ಗಾತ್ರ (bool fancy, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name)
{
  ರಚನೆ winsize win;

  ಆದರೆ (get_win_ಗಾತ್ರ (STDIN_FILENO, &win))
    {
      ಆದರೆ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ಆದರೆ (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_ಅಡ್ಡಸಾಲು, win.ws_ಲಂಬಸಾಲು);
      ಆದರೆ (!fancy)
        current_ಲಂಬಸಾಲು = 0;
    }
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
screen_columns (ಶೂನ್ಯ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIOCGWINSZ
  ರಚನೆ winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  ಆದರೆ (get_win_ಗಾತ್ರ (STDOUT_FILENO, &win) == 0 && 0 < win.ws_ಲಂಬಸಾಲು)
    ಹಿಂತಿರುಗಿ win.ws_ಲಂಬಸಾಲು;
#ಆದರೆ_ಅಂತ್ಯ
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    ಅಕ್ಷರ *ಲಂಬಸಾಲು_string = getenv ("COLUMNS");
    ದೀರ್ಘ ಪೂರ್ಣಾಂಕ n_columns;
    ಆದರೆ (!(ಲಂಬಸಾಲು_string != ಖಾಲಿ
          && xstrtol (ಲಂಬಸಾಲು_string, ಖಾಲಿ, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ))
      n_columns = 80;
    ಹಿಂತಿರುಗಿ n_columns;
  }
}

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ tcflag_t *
ಬಹುಲಕ_type_flag (ಎಣಿಕೆ ಬಹುಲಕ_type type, ರಚನೆ termios *ಬಹುಲಕ_2)
{
  ಆಯ್ಕೆ (type)
    {
    ಸ್ಥಿತಿ control:
      ಹಿಂತಿರುಗಿ &ಬಹುಲಕ_2->c_cflag;

    ಸ್ಥಿತಿ ಒಳಹರಿವು:
      ಹಿಂತಿರುಗಿ &ಬಹುಲಕ_2->c_iflag;

    ಸ್ಥಿತಿ output:
      ಹಿಂತಿರುಗಿ &ಬಹುಲಕ_2->c_oflag;

    ಸ್ಥಿತಿ local:
      ಹಿಂತಿರುಗಿ &ಬಹುಲಕ_2->c_lflag;

    ಸ್ಥಿತಿ combination:
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_settings (ಎಣಿಕೆ output_type output_type, ರಚನೆ termios *ಬಹುಲಕ_2,
                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name)
{
  ಆಯ್ಕೆ (output_type)
    {
    ಸ್ಥಿತಿ ಬದಲಾಯಿತು:
      display_ಬದಲಾಯಿತು (ಬಹುಲಕ_2);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ all:
      display_all (ಬಹುಲಕ_2, device_name);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ recoverable:
      display_recoverable (ಬಹುಲಕ_2);
      ನಿಲ್ಲಿಸು;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_ಬದಲಾಯಿತು (ರಚನೆ termios *ಬಹುಲಕ_2)
{
  bool ಖಾಲಿಗಳು_ಸಾಲು;
  tcflag_t *bitsp;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಖವಾಡ;
  ಎಣಿಕೆ ಬಹುಲಕ_type prev_type = control;

  display_ವೇಗ (ಬಹುಲಕ_2, true);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_C_ಸಾಲು
  wrapf ("line = %d;", ಬಹುಲಕ_2->c_ಸಾಲು);
#ಆದರೆ_ಅಂತ್ಯ
  ಅಕ್ಷರ_ಬರೆ ('\n');
  current_ಲಂಬಸಾಲು = 0;

  ಖಾಲಿಗಳು_ಸಾಲು = true;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      ಆದರೆ (ಬಹುಲಕ_2->c_cc[control_info[i].ಸ್ಥಾನಾಂತರ] == control_info[i].saneval)
        ಮುಂದುವರಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ಆದರೆ (streq (control_info[i].name, "flush"))
        ಮುಂದುವರಿ;
#ಆದರೆ_ಅಂತ್ಯ
      /* If swtch is the same as susp, don't print both.  */
#ಆದರೆ VSWTCH == VSUSP
      ಆದರೆ (streq (control_info[i].name, "swtch"))
        ಮುಂದುವರಿ;
#ಆದರೆ_ಅಂತ್ಯ
      /* If eof uses the same slot as min, only print whichever applies.  */
#ಆದರೆ VEOF == VMIN
      ಆದರೆ ((ಬಹುಲಕ_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ಮುಂದುವರಿ;
#ಆದರೆ_ಅಂತ್ಯ

      ಖಾಲಿಗಳು_ಸಾಲು = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (ಬಹುಲಕ_2->c_cc[control_info[i].ಸ್ಥಾನಾಂತರ]));
    }
  ಆದರೆ ((ಬಹುಲಕ_2->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) ಬಹುಲಕ_2->c_cc[VMIN],
             (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) ಬಹುಲಕ_2->c_cc[VTIME]);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಖಾಲಿಗಳು_ಸಾಲು)
    ಅಕ್ಷರ_ಬರೆ ('\n');
  current_ಲಂಬಸಾಲು = 0;

  ಖಾಲಿಗಳು_ಸಾಲು = true;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; ಬಹುಲಕ_info[i].name != ಖಾಲಿ; ++i)
    {
      ಆದರೆ (ಬಹುಲಕ_info[i].flags & OMIT)
        ಮುಂದುವರಿ;
      ಆದರೆ (ಬಹುಲಕ_info[i].type != prev_type)
        {
          ಆದರೆ (!ಖಾಲಿಗಳು_ಸಾಲು)
            {
              ಅಕ್ಷರ_ಬರೆ ('\n');
              current_ಲಂಬಸಾಲು = 0;
              ಖಾಲಿಗಳು_ಸಾಲು = true;
            }
          prev_type = ಬಹುಲಕ_info[i].type;
        }

      bitsp = ಬಹುಲಕ_type_flag (ಬಹುಲಕ_info[i].type, ಬಹುಲಕ_2);
      ಮುಖವಾಡ = ಬಹುಲಕ_info[i].ಮುಖವಾಡ ? ಬಹುಲಕ_info[i].ಮುಖವಾಡ : ಬಹುಲಕ_info[i].ಬಿಟ್ಟುಗಳು_2;
      ಆದರೆ ((*bitsp & ಮುಖವಾಡ) == ಬಹುಲಕ_info[i].ಬಿಟ್ಟುಗಳು_2)
        {
          ಆದರೆ (ಬಹುಲಕ_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", ಬಹುಲಕ_info[i].name);
              ಖಾಲಿಗಳು_ಸಾಲು = false;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಬಹುಲಕ_info[i].flags & (SANE_ಗಣ | REV)) == (SANE_ಗಣ | REV))
        {
          wrapf ("-%s", ಬಹುಲಕ_info[i].name);
          ಖಾಲಿಗಳು_ಸಾಲು = false;
        }
    }
  ಆದರೆ (!ಖಾಲಿಗಳು_ಸಾಲು)
    ಅಕ್ಷರ_ಬರೆ ('\n');
  current_ಲಂಬಸಾಲು = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_all (ರಚನೆ termios *ಬಹುಲಕ_2, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device_name)
{
  tcflag_t *bitsp;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಖವಾಡ;
  ಎಣಿಕೆ ಬಹುಲಕ_type prev_type = control;

  display_ವೇಗ (ಬಹುಲಕ_2, true);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ TIOCGWINSZ
  display_ಕಿಟಕಿ_ಗಾತ್ರ (true, device_name);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_C_ಸಾಲು
  wrapf ("line = %d;", ಬಹುಲಕ_2->c_ಸಾಲು);
#ಆದರೆ_ಅಂತ್ಯ
  ಅಕ್ಷರ_ಬರೆ ('\n');
  current_ಲಂಬಸಾಲು = 0;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ಆದರೆ (streq (control_info[i].name, "flush"))
        ಮುಂದುವರಿ;
#ಆದರೆ_ಅಂತ್ಯ
      /* If swtch is the same as susp, don't print both.  */
#ಆದರೆ VSWTCH == VSUSP
      ಆದರೆ (streq (control_info[i].name, "swtch"))
        ಮುಂದುವರಿ;
#ಆದರೆ_ಅಂತ್ಯ
      /* If eof uses the same slot as min, only print whichever applies.  */
#ಆದರೆ VEOF == VMIN
      ಆದರೆ ((ಬಹುಲಕ_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ಮುಂದುವರಿ;
#ಆದರೆ_ಅಂತ್ಯ
      wrapf ("%s = %s;", control_info[i].name,
             visible (ಬಹುಲಕ_2->c_cc[control_info[i].ಸ್ಥಾನಾಂತರ]));
    }
#ಆದರೆ VEOF == VMIN
  ಆದರೆ ((ಬಹುಲಕ_2->c_lflag & ICANON) == 0)
#ಆದರೆ_ಅಂತ್ಯ
    wrapf ("min = %lu; time = %lu;",
           (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) ಬಹುಲಕ_2->c_cc[VMIN],
           (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) ಬಹುಲಕ_2->c_cc[VTIME]);
  ಆದರೆ (current_ಲಂಬಸಾಲು != 0)
    ಅಕ್ಷರ_ಬರೆ ('\n');
  current_ಲಂಬಸಾಲು = 0;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; ಬಹುಲಕ_info[i].name != ಖಾಲಿ; ++i)
    {
      ಆದರೆ (ಬಹುಲಕ_info[i].flags & OMIT)
        ಮುಂದುವರಿ;
      ಆದರೆ (ಬಹುಲಕ_info[i].type != prev_type)
        {
          ಅಕ್ಷರ_ಬರೆ ('\n');
          current_ಲಂಬಸಾಲು = 0;
          prev_type = ಬಹುಲಕ_info[i].type;
        }

      bitsp = ಬಹುಲಕ_type_flag (ಬಹುಲಕ_info[i].type, ಬಹುಲಕ_2);
      ಮುಖವಾಡ = ಬಹುಲಕ_info[i].ಮುಖವಾಡ ? ಬಹುಲಕ_info[i].ಮುಖವಾಡ : ಬಹುಲಕ_info[i].ಬಿಟ್ಟುಗಳು_2;
      ಆದರೆ ((*bitsp & ಮುಖವಾಡ) == ಬಹುಲಕ_info[i].ಬಿಟ್ಟುಗಳು_2)
        wrapf ("%s", ಬಹುಲಕ_info[i].name);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಬಹುಲಕ_info[i].flags & REV)
        wrapf ("-%s", ಬಹುಲಕ_info[i].name);
    }
  ಅಕ್ಷರ_ಬರೆ ('\n');
  current_ಲಂಬಸಾಲು = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ಸ್ಥಿರ ಶೂನ್ಯ
check_ವೇಗ (ರಚನೆ termios *ಬಹುಲಕ_2)
{
  ಆದರೆ (ಕೊನೆಯ_ibaud.valid && ಕೊನೆಯ_obaud.valid)
    {
      ಆದರೆ (cfgetispeed (ಬಹುಲಕ_2) != ಕೊನೆಯ_ibaud.baud
          || cfgetospeed (ಬಹುಲಕ_2) != ಕೊನೆಯ_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_ಗಮ್ಯ_ಮೌಲ್ಯ (ಕೊನೆಯ_ibaud.baud),
               baud_ಗಮ್ಯ_ಮೌಲ್ಯ (ಕೊನೆಯ_obaud.baud));
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_ವೇಗ (ರಚನೆ termios *ಬಹುಲಕ_2, bool fancy)
{
  ಆದರೆ (cfgetispeed (ಬಹುಲಕ_2) == 0 || cfgetispeed (ಬಹುಲಕ_2) == cfgetospeed (ಬಹುಲಕ_2))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_ಗಮ್ಯ_ಮೌಲ್ಯ (cfgetospeed (ಬಹುಲಕ_2)));
  ಇಲ್ಲದಿದ್ದರೆ
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_ಗಮ್ಯ_ಮೌಲ್ಯ (cfgetispeed (ಬಹುಲಕ_2)),
           baud_ಗಮ್ಯ_ಮೌಲ್ಯ (cfgetospeed (ಬಹುಲಕ_2)));
  ಆದರೆ (!fancy)
    current_ಲಂಬಸಾಲು = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_recoverable (ರಚನೆ termios *ಬಹುಲಕ_2)
{
  ಬರೆ ("%lx:%lx:%lx:%lx",
          (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) ಬಹುಲಕ_2->c_iflag,
          (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) ಬಹುಲಕ_2->c_oflag,
          (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) ಬಹುಲಕ_2->c_cflag,
          (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) ಬಹುಲಕ_2->c_lflag);
  ಗಾಗಿ (size_t i = 0; i < NCCS; ++i)
    ಬರೆ (":%lx", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) ಬಹುಲಕ_2->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (ಬಹುಲಕ_2->c_cflag & CBAUD) == BOTHER;
#  ಆದರೆ defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((ಬಹುಲಕ_2->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#ಆದರೆ_ಅಂತ್ಯ
  ಆದರೆ (separate_speeds)
    ಬರೆ (":%lx:%lx", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) cfgetispeed (ಬಹುಲಕ_2),
            (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) cfgetospeed (ಬಹುಲಕ_2));

  ಅಕ್ಷರ_ಬರೆ ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#ವ್ಯಾಖ್ಯಾನಿಸು STRTOUL_TYPE(TYPE)                                               \
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ                                                             \
  strtoul_##TYPE (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ಪೂರ್ಣಾಂಕ ತಳ_2, ಅಕ್ಷರ **p, TYPE *ಫಲಿತಾಂಶ,       \
                  ಅಕ್ಷರ delim)                                            \
  {                                                                      \
    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, ತಳ_2);                                           \
    ಆದರೆ (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      ಹಿಂತಿರುಗಿ -1;                                                         \
    *ಫಲಿತಾಂಶ = ul;                                                        \
    ಹಿಂತಿರುಗಿ 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (ವೇಗ_t)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#ಅನ್ತರ್ಭಾವ "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ಸ್ಥಿರ bool
recover_ಬಹುಲಕ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, ರಚನೆ termios *ಬಹುಲಕ_2)
{
  tcflag_t flag[4];
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s = arg;
  ಗಾಗಿ (size_t i = 0; i < 4; i++)
    {
      ಅಕ್ಷರ *p;
      ಆದರೆ (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        ಹಿಂತಿರುಗಿ false;
      s = p + 1;
    }
  ಬಹುಲಕ_2->c_iflag = flag[0];
  ಬಹುಲಕ_2->c_oflag = flag[1];
  ಬಹುಲಕ_2->c_cflag = flag[2];
  ಬಹುಲಕ_2->c_lflag = flag[3];

  ಗಾಗಿ (size_t i = 0; i < NCCS; ++i)
    {
      ಅಕ್ಷರ *p;
      ಅಕ್ಷರ delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      ಆದರೆ (strtoul_cc_t (s, 16, &p, ಬಹುಲಕ_2->c_cc + i, delim) != 0)
        ಹಿಂತಿರುಗಿ false;
      /* Accept the old format, which has no separate speed fields.  */
      ಆದರೆ (! *p)
        {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_ವೇಗ_T_SANE
          ರಚನೆ ವೇಗ_desc ospeed = cbaud_ಗಮ್ಯ_ವೇಗ (ಬಹುಲಕ_2->c_cflag & CBAUD);
          ಆದರೆ (ospeed.valid && cfsetospeed (ಬಹುಲಕ_2, ospeed.baud) != 0)
            ಹಿಂತಿರುಗಿ false;
#  ಆದರೆ defined CIBAUD && defined IBSHIFT
          ರಚನೆ ವೇಗ_desc ispeed =
            cbaud_ಗಮ್ಯ_ವೇಗ ((ಬಹುಲಕ_2->c_cflag & CIBAUD) >> IBSHIFT);
          ಆದರೆ (ispeed.valid && cfsetispeed (ಬಹುಲಕ_2, ispeed.baud) != 0)
            ಹಿಂತಿರುಗಿ false;
#  endif
# endif
          ಕೊನೆಯ_ibaud.baud = cfgetispeed (ಬಹುಲಕ_2);
          ಕೊನೆಯ_obaud.baud = cfgetospeed (ಬಹುಲಕ_2);
#ಆದರೆ_ಅಂತ್ಯ
          ಹಿಂತಿರುಗಿ true;
        }
      s = p + 1;
    }

  ವೇಗ_t ವೇಗ[2];
  ಗಾಗಿ (size_t i = 0; i < 2; i++)
    {
      ಅಕ್ಷರ *p;
      ಅಕ್ಷರ delim = i == 0 ? ':' : '\0';
      ಆದರೆ (strtoul_ವೇಗ_t (s, 16, &p, ವೇಗ + i, delim) != 0)
        ಹಿಂತಿರುಗಿ false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  ಆದರೆ (cfsetospeed (ಬಹುಲಕ_2, ವೇಗ[1]) != 0
      || cfsetispeed (ಬಹುಲಕ_2, ವೇಗ[0]) != 0)
    ಹಿಂತಿರುಗಿ false;

  ಕೊನೆಯ_ibaud.baud = ವೇಗ[0] == 0 ? cfgetispeed (ಬಹುಲಕ_2) : ವೇಗ[0];
  ಕೊನೆಯ_obaud.baud = ವೇಗ[1];
  ಹಿಂತಿರುಗಿ true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ಸ್ಥಿರ bool
ವೇಗ_matches (ರಚನೆ termios ಸ್ಥಿರಾಂಕ *mode1, ರಚನೆ termios ಸ್ಥಿರಾಂಕ *mode2)
{
  ವೇಗ_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  ವೇಗ_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  ಆದರೆ (speed1[1] == 0)
    speed1[1] = speed1[0];
  ಆದರೆ (speed2[1] == 0)
    speed2[1] = speed2[0];

  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು_2 = 2;  /* Start and stop bits.  */
  ಬಿಟ್ಟುಗಳು_2 += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  ಬಿಟ್ಟುಗಳು_2 += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  ಆಯ್ಕೆ (mode1->c_cflag & CSIZE)
    {
      ಸ್ಥಿತಿ CS5: ಬಿಟ್ಟುಗಳು_2 += 5; ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ CS6: ಬಿಟ್ಟುಗಳು_2 += 6; ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ CS7: ಬಿಟ್ಟುಗಳು_2 += 7; ನಿಲ್ಲಿಸು;
      ಪೂರ್ವನಿಯೋಜಿತ: ಬಿಟ್ಟುಗಳು_2 += 8; ನಿಲ್ಲಿಸು;
    }

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; i++)
    {
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ value1;
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ value2;
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ delta;

      ಆದರೆ (speed1[i] == speed2[i])
        ಮುಂದುವರಿ;

      value1 = baud_ಗಮ್ಯ_ಮೌಲ್ಯ (speed1[i]);
      value2 = baud_ಗಮ್ಯ_ಮೌಲ್ಯ (speed2[i]);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ TERMIOS_ವೇಗ_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      ಆದರೆ (value1 == ULONG_MAX || value2 == ULONG_MAX)
        ಹಿಂತಿರುಗಿ false;
#ಆದರೆ_ಅಂತ್ಯ

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      ಆದರೆ (delta > value1 / (ಬಿಟ್ಟುಗಳು_2 * 4))
        ಹಿಂತಿರುಗಿ false;
    }

  ಹಿಂತಿರುಗಿ true;
}

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ರಚನೆ ವೇಗ_desc
string_ಗಮ್ಯ_baud (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg)
{
  ಅಕ್ಷರ *ep;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮೌಲ್ಯ;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c;

  /* Explicitly disallow negative numbers.  */
  ಆಗ (c_isspace (*arg))
    arg++;
  ಆದರೆ (*arg == '-')
    ಹೋಗು invalid;

  /* Empty does not mean 0.  */
  ಆದರೆ (! *arg)
    ಹೋಗು invalid;

  ಮೌಲ್ಯ = strtoul (arg, &ep, 10);

  c = *ep++;
  ಆದರೆ (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      ಆದರೆ (c)
        {
          ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ d = c - '0';
          ಆದರೆ (d > 5)
            ಮೌಲ್ಯ++;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (d == 5)
            {
              ಆಗ ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              ಆದರೆ (c)
                ಮೌಲ್ಯ++;                /* Nonzero, round up */
              ಇಲ್ಲದಿದ್ದರೆ
                ಮೌಲ್ಯ += (ಮೌಲ್ಯ & 1);   /* Exactly in the middle, round even */
            }

          ಆಗ (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          ಆದರೆ (c)
            ಹೋಗು invalid; /* Garbage after otherwise valid number */
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      ಆದರೆ (streq (arg, "exta"))
        ಹಿಂತಿರುಗಿ (ರಚನೆ ವೇಗ_desc) { .baud = B19200, .valid = true };
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (arg, "extb"))
        ಹಿಂತಿರುಗಿ (ರಚನೆ ವೇಗ_desc) { .baud = B38400, .valid = true };
      ಇಲ್ಲದಿದ್ದರೆ
        ಹೋಗು invalid;
    }

  ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ_ಗಮ್ಯ_baud (ಮೌಲ್ಯ);

 invalid:
  ಹಿಂತಿರುಗಿ (ರಚನೆ ವೇಗ_desc) { .baud = (ವೇಗ_t) -1, .valid = false };
}

ಸ್ಥಿರ ಶೂನ್ಯ
sane_ಬಹುಲಕ (ರಚನೆ termios *ಬಹುಲಕ_2)
{
  tcflag_t *bitsp;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; control_info[i].name; ++i)
    {
#ಆದರೆ VMIN == VEOF
      ಆದರೆ (streq (control_info[i].name, "min"))
        ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
      ಬಹುಲಕ_2->c_cc[control_info[i].ಸ್ಥಾನಾಂತರ] = control_info[i].saneval;
    }

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; ಬಹುಲಕ_info[i].name != ಖಾಲಿ; ++i)
    {
      ಆದರೆ (ಬಹುಲಕ_info[i].flags & NO_SETATTR)
        ಮುಂದುವರಿ;

      ಆದರೆ (ಬಹುಲಕ_info[i].flags & SANE_ಗಣ)
        {
          bitsp = ಬಹುಲಕ_type_flag (ಬಹುಲಕ_info[i].type, ಬಹುಲಕ_2);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~ಬಹುಲಕ_info[i].ಮುಖವಾಡ) | ಬಹುಲಕ_info[i].ಬಿಟ್ಟುಗಳು_2;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಬಹುಲಕ_info[i].flags & SANE_UNSET)
        {
          bitsp = ಬಹುಲಕ_type_flag (ಬಹುಲಕ_info[i].type, ಬಹುಲಕ_2);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~ಬಹುಲಕ_info[i].ಮುಖವಾಡ & ~ಬಹುಲಕ_info[i].ಬಿಟ್ಟುಗಳು_2;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
visible (cc_t ch)
{
  ಸ್ಥಿರ ಅಕ್ಷರ buf[10];
  ಅಕ್ಷರ *bpout = buf;

  ಆದರೆ (ch == _POSIX_VDISABLE)
    ಹಿಂತಿರುಗಿ "<undef>";

  ಆದರೆ (ch >= 32)
    {
      ಆದರೆ (ch < 127)
        *bpout++ = ch;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          ಆದರೆ (ch >= 128 + 32)
            {
              ಆದರೆ (ch < 128 + 127)
                *bpout++ = ch - 128;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

ಸ್ಥಿರ uintmax_t
integer_arg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, uintmax_t maxval)
{
  ಹಿಂತಿರುಗಿ xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
