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

#ifdef TERMIOS_NEEDS_XOPEN_SOURCE
# define _XOPEN_SOURCE
#endif

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ <termios.h>
#ಯದಿ HAVE_STROPTS_H
# include <stropts.h>
#endif
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif
#ifdef GWINSZ_IN_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#endif
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
#define PROGRAM_NAME "stty"

#define AUTHORS proper_name ("David MacKenzie")

#ifndef _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#endif

#define Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#ifndef CINTR
# define CINTR Control ('c')
#endif
#ifndef CQUIT
# define CQUIT 28
#endif
#ifndef CERASE
# define CERASE 127
#endif
#ifndef CKILL
# define CKILL Control ('u')
#endif
#ifndef CEOF
# define CEOF Control ('d')
#endif
#ifndef CEOL
# define CEOL _POSIX_VDISABLE
#endif
#ifndef CSTART
# define CSTART Control ('q')
#endif
#ifndef CSTOP
# define CSTOP Control ('s')
#endif
#ifndef CSUSP
# define CSUSP Control ('z')
#endif
#ಯದಿ defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#endif
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#ಯದಿ defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#endif
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#ಯದಿ defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# ಯದಿ defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#endif
#ಯದಿ defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#ಯದಿ defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

#ಯದಿ defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#endif
#ಯದಿ defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#endif
#ಯದಿ defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#ಯದಿ defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#ಯದಿ defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#endif
#ಯದಿ defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#endif
#ಯದಿ defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#endif
#ಯದಿ defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#endif
#ಯದಿ defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#endif
#ಯದಿ defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#endif
#ಯದಿ defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#endif
#ಯದಿ defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#endif
#ಯದಿ defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#endif

/* Which speeds to set.  */
ಗಣನಾ speed_setting
  {
    ಒಳಹರಿವು_speed, output_speed, both_speeds
  };

/* What to output and how.  */
ಗಣನಾ output_type
  {
    changed, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
ಗಣನಾ mode_type
  {
    control, ಒಳಹರಿವು, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#define SANE_SET 1		/* Set in 'sane' mode. */
#define SANE_UNSET 2		/* Unset in 'sane' mode. */
#define REV 4			/* Can be turned off by prepending '-'. */
#define OMIT 8			/* Don't display value. */
#define NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
ಸಂರಚನಾ mode_info
  {
    ವರ್ಣ ಧ್ರುವ *name;		/* Name given on command line.  */
    ಗಣನಾ mode_type type;	/* Which structure element to change. */
    ವರ್ಣ flags;			/* Setting and display options.  */
    unsigned long bits;		/* Bits to set for this mode.  */
    unsigned long mask;		/* Other bits to turn off for this mode.  */
  };

ಸ್ಥಿರ ಸಂರಚನಾ mode_info ಧ್ರುವ mode_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#ifdef CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#endif
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_SET | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#ifdef CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#endif
#ifdef CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#endif

  {"ignbrk", ಒಳಹರಿವು, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", ಒಳಹರಿವು, SANE_SET | REV, BRKINT, 0},
  {"ignpar", ಒಳಹರಿವು, REV, IGNPAR, 0},
  {"parmrk", ಒಳಹರಿವು, REV, PARMRK, 0},
  {"inpck", ಒಳಹರಿವು, REV, INPCK, 0},
  {"istrip", ಒಳಹರಿವು, REV, ISTRIP, 0},
  {"inlcr", ಒಳಹರಿವು, SANE_UNSET | REV, INLCR, 0},
  {"igncr", ಒಳಹರಿವು, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", ಒಳಹರಿವು, SANE_SET | REV, ICRNL, 0},
  {"ixon", ಒಳಹರಿವು, REV, IXON, 0},
  {"ixoff", ಒಳಹರಿವು, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", ಒಳಹರಿವು, REV | OMIT, IXOFF, 0},
#ifdef IUCLC
  {"iuclc", ಒಳಹರಿವು, SANE_UNSET | REV, IUCLC, 0},
#endif
#ifdef IXANY
  {"ixany", ಒಳಹರಿವು, SANE_UNSET | REV, IXANY, 0},
#endif
#ifdef IMAXBEL
  {"imaxbel", ಒಳಹರಿವು, SANE_SET | REV, IMAXBEL, 0},
#endif
#ifdef IUTF8
  {"iutf8", ಒಳಹರಿವು, SANE_UNSET | REV, IUTF8, 0},
#endif

  {"opost", output, SANE_SET | REV, OPOST, 0},
#ifdef OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#endif
#ifdef OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#endif
#ifdef ONLCR
  {"onlcr", output, SANE_SET | REV, ONLCR, 0},
#endif
#ifdef ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#endif
#ifdef ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#endif
#ifdef OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#endif
#ifdef OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#endif
#ifdef NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_SET, NL0, NLDLY},
#endif
#ifdef CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_SET, CR0, CRDLY},
#endif
#ifdef TABDLY
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
  {"tab0", output, SANE_SET, TAB0, TABDLY},
# endif
#ಅನ್ಯಥಾ
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#endif
#ifdef BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_SET, BS0, BSDLY},
#endif
#ifdef VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_SET, VT0, VTDLY},
#endif
#ifdef FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_SET, FF0, FFDLY},
#endif

  {"isig", local, SANE_SET | REV, ISIG, 0},
  {"icanon", local, SANE_SET | REV, ICANON, 0},
#ifdef IEXTEN
  {"iexten", local, SANE_SET | REV, IEXTEN, 0},
#endif
  {"echo", local, SANE_SET | REV, ECHO, 0},
  {"echoe", local, SANE_SET | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_SET | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#ifdef XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#endif
#ifdef TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#endif
#ifdef ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#endif
#ifdef ECHOCTL
  {"echoctl", local, SANE_SET | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#endif
#ifdef ECHOKE
  {"echoke", local, SANE_SET | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#endif
#ifdef FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#endif
#ಯದಿ defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#ನೋಚೇದ್ಯದಿ defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#endif

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
#ifdef IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#endif
#ಯದಿ defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#endif
#ಯದಿ defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#endif
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {ರಿಕ್ತ, control, 0, 0, 0}
};

/* Control character settings.  */
ಸಂರಚನಾ control_info
  {
    ವರ್ಣ ಧ್ರುವ *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t offset;		/* Offset in c_cc.  */
  };

/* Control characters. */

ಸ್ಥಿರ ಸಂರಚನಾ control_info ಧ್ರುವ control_info[] =
{
  {"intr", CINTR, VINTR},
  {"quit", CQUIT, VQUIT},
  {"erase", CERASE, VERASE},
  {"kill", CKILL, VKILL},
  {"eof", CEOF, VEOF},
  {"eol", CEOL, VEOL},
#ifdef VEOL2
  {"eol2", CEOL2, VEOL2},
#endif
#ifdef VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#endif
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#ifdef VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#endif
#ifdef VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#ಅನ್ಯಥಾ
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#endif
#ifdef VWERASE
  {"werase", CWERASE, VWERASE},
#endif
#ifdef VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#endif
#ifdef VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#endif
#ifdef VSTATUS
  {"status", CSTATUS, VSTATUS},
#endif

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {ರಿಕ್ತ, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
ಸಂರಚನಾ speed_desc
{
  /* The baud rate.  */
  speed_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *visible (cc_t ch);
ಸ್ಥಿರ unsigned long ಪೂರ್ಣಾಙ್ಕ baud_to_ಮೌಲ್ಯ (speed_t speed);
ಸ್ಥಿರ bool recover_mode (ವರ್ಣ ಧ್ರುವ *arg, ಸಂರಚನಾ termios *mode);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ screen_columns (ಶೂನ್ಯ);
ಸ್ಥಿರ bool set_mode (ಸಂರಚನಾ mode_info ಧ್ರುವ *info, bool reversed,
                      ಸಂರಚನಾ termios *mode);
ಸ್ಥಿರ bool eq_mode (ಸಂರಚನಾ termios ಧ್ರುವ *mode1,
                     ಸಂರಚನಾ termios ಧ್ರುವ *mode2);
ಸ್ಥಿರ bool speed_matches (ಸಂರಚನಾ termios ಧ್ರುವ *mode1,
                           ಸಂರಚನಾ termios ಧ್ರುವ *mode2);
ಸ್ಥಿರ uintmax_t integer_arg (ವರ್ಣ ಧ್ರುವ *s, uintmax_t max);
ಸ್ಥಿರ ಸಂರಚನಾ speed_desc string_to_baud (ವರ್ಣ ಧ್ರುವ *arg);
ಸ್ಥಿರ tcflag_t *mode_type_flag (ಗಣನಾ mode_type type, ಸಂರಚನಾ termios *mode);
ಸ್ಥಿರ ಶೂನ್ಯ display_all (ಸಂರಚನಾ termios *mode, ವರ್ಣ ಧ್ರುವ *device_name);
ಸ್ಥಿರ ಶೂನ್ಯ display_changed (ಸಂರಚನಾ termios *mode);
ಸ್ಥಿರ ಶೂನ್ಯ display_recoverable (ಸಂರಚನಾ termios *mode);
ಸ್ಥಿರ ಶೂನ್ಯ display_settings (ಗಣನಾ output_type output_type,
                              ಸಂರಚನಾ termios *mode,
                              ವರ್ಣ ಧ್ರುವ *device_name);
ಸ್ಥಿರ ಶೂನ್ಯ check_speed (ಸಂರಚನಾ termios *mode);
ಸ್ಥಿರ ಶೂನ್ಯ display_speed (ಸಂರಚನಾ termios *mode, bool fancy);
ಸ್ಥಿರ ಶೂನ್ಯ display_window_size (bool fancy, ವರ್ಣ ಧ್ರುವ *device_name);
ಸ್ಥಿರ ಶೂನ್ಯ sane_mode (ಸಂರಚನಾ termios *mode);
ಸ್ಥಿರ ಶೂನ್ಯ set_control_char (ಸಂರಚನಾ control_info ಧ್ರುವ *info,
                              ವರ್ಣ ಧ್ರುವ *arg,
                              ಸಂರಚನಾ termios *mode);
ಸ್ಥಿರ ಶೂನ್ಯ set_speed (ಗಣನಾ speed_setting type, ವರ್ಣ ಧ್ರುವ *arg,
                       ಸಂರಚನಾ speed_desc ಧ್ರುವ speed, ಸಂರಚನಾ termios *mode);
ಸ್ಥಿರ ಶೂನ್ಯ set_window_size (ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ವರ್ಣ ಧ್ರುವ *device_name);

/* The width of the screen, for output wrapping. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ max_col;

/* Current position, to know when to wrap. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ current_col;

/* Default "drain" mode for tcsetattr.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ಸ್ಥಿರ bool dev_debug;

/* Record last speed set for correlation.  */
ಸ್ಥಿರ ಸಂರಚನಾ speed_desc last_ibaud =
  { .baud = (speed_t) -1, .valid = false };
ಸ್ಥಿರ ಸಂರಚನಾ speed_desc last_obaud =
  { .baud = (speed_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"all", no_argument, ರಿಕ್ತ, 'a'},
  {"save", no_argument, ರಿಕ್ತ, 'g'},
  {"file", required_argument, ರಿಕ್ತ, 'F'},
  {"-debug", no_argument, ರಿಕ್ತ, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((printf, 1, 2))
ಸ್ಥಿರ ಶೂನ್ಯ
wrapf (ವರ್ಣ ಧ್ರುವ *message,...)
{
  va_list args;
  ವರ್ಣ *buf;
  ಪೂರ್ಣಾಙ್ಕ buflen;

  va_start (args, message);
  buflen = vasprintf (&buf, message, args);
  va_end (args);

  ಯದಿ (buflen < 0)
    xalloc_die ();

  ಯದಿ (0 < current_col)
    {
      ಯದಿ (max_col - current_col <= buflen)
        {
          putchar ('\n');
          current_col = 0;
        }
      ಅನ್ಯಥಾ
        {
          putchar (' ');
          current_col++;
        }
    }

  fputs (buf, stdout);
  ಮುಕ್ತ (buf);
  current_col += buflen;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Print or change terminal characteristics.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings in human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings in a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  open and use DEVICE instead of standard ಒಳಹರಿವು\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system defines which settings are available.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#ifdef VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding of output\n\
"), stdout);
#endif
#ifdef VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop ಸಙ್ಕೇತ once ಒಳಹರಿವು flushed\n\
"), stdout);
#endif
      fputs (_("\
   eof CHAR      CHAR will send an end of file (terminate the ಒಳಹರಿವು)\n\
   eol CHAR      CHAR will end the ಸಾಲು\n\
"), stdout);
#ifdef VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR ಪ್ರತಿ ending the ಸಾಲು\n\
"), stdout);
#endif
      fputs (_("\
   erase CHAR    CHAR will erase the last character typed\n\
   intr CHAR     CHAR will send an interrupt ಸಙ್ಕೇತ\n\
   kill CHAR     CHAR will erase the current ಸಾಲು\n\
"), stdout);
#ifdef VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the next character quoted\n\
"), stdout);
#endif
#ifdef VSTATUS
      fputs (_("\
 * status CHAR   CHAR will send an info ಸಙ್ಕೇತ\n\
"), stdout);
#endif
      fputs (_("\
   quit CHAR     CHAR will send a quit ಸಙ್ಕೇತ\n\
"), stdout);
#ಯದಿ defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current ಸಾಲು\n\
"), stdout);
#endif
      fputs (_("\
   start CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop ಸಙ್ಕೇತ\n\
"), stdout);
#ifdef VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will ವಿಕಲ್ಪ to a different shell layer\n\
"), stdout);
#endif
#ifdef VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the last word typed\n\
"), stdout);
#endif
      fputs (_("\
\n\
Special settings:\n\
   N             set the ಒಳಹರಿವು and output speeds to N bauds\n\
"), stdout);
#ifdef TIOCGWINSZ
      fputs (_("\
   cols N        tell the kernel that the terminal has N columns\n\
 * columns N     same as cols N\n\
"), stdout);
#endif
      printf (_("\
 * [-]drain      wait ಪ್ರತಿ transmission before applying settings (%s by ಸ್ವಭಾವ)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      set the ಒಳಹರಿವು speed to N\n\
"), stdout);
#ifdef HAVE_C_ಸಾಲು
      fputs (_("\
 * ಸಾಲು N        use ಸಾಲು discipline N\n\
"), stdout);
#endif
      fputs (_("\
   min N         with -icanon, set N characters minimum ಪ್ರತಿ a completed ಪಠ\n\
   ospeed N      set the output speed to N\n\
"), stdout);
#ifdef TIOCGWINSZ
      fputs (_("\
   rows N        tell the kernel that the terminal has N rows\n\
   size          print the ಸಂಖ್ಯೆ of rows and columns according to the kernel\n\
"), stdout);
#endif
      fputs (_("\
   speed         print the terminal speed\n\
   time N        with -icanon, set ಪಠ timeout of N tenths of a second\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control signals\n\
   [-]cread      allow ಒಳಹರಿವು to be received\n\
"), stdout);
#ifdef CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#endif
#ifdef CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#endif
      fputs (_("\
   csN           set character size to N bits, N in [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop bits per character (one with '-')\n\
   [-]hup        send a hangup ಸಙ್ಕೇತ when the last process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity bit in output and expect parity bit in ಒಳಹರಿವು\n\
   [-]parodd     set odd parity (or even parity with '-')\n\
"), stdout);
#ifdef CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#endif
      fputs (_("\
\n\
ಒಳಹರಿವು_3 settings:\n\
   [-]brkint     breaks cause an interrupt ಸಙ್ಕೇತ\n\
   [-]icrnl      translate carriage ಪ್ರತ್ಯಾಗಮ to newline\n\
   [-]ignbrk     ignore ಭಙ್ಗ characters\n\
   [-]igncr      ignore carriage ಪ್ರತ್ಯಾಗಮ\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#ifdef IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and ಕುರು not flush a full ಒಳಹರಿವು buffer on a character\n\
"), stdout);
#endif
      fputs (_("\
   [-]inlcr      translate newline to carriage ಪ್ರತ್ಯಾಗಮ\n\
   [-]inpck      enable ಒಳಹರಿವು parity checking\n\
   [-]istrip     clear ಮೇಲಿನ (8th) bit of ಒಳಹರಿವು characters\n\
"), stdout);
#ifdef IUTF8
      fputs (_("\
 * [-]iutf8      assume ಒಳಹರಿವು characters are UTF-8 encoded\n\
"), stdout);
#endif
#ifdef IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters to lowercase\n\
"), stdout);
#endif
#ifdef IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only start character\n\
"), stdout);
#endif
      fputs (_("\
   [-]ixoff      enable sending of start/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#ifdef BSDLY
      fputs (_("\
 * bsN           backspace delay style, N in [0..1]\n\
"), stdout);
#endif
#ifdef CRDLY
      fputs (_("\
 * crN           carriage ಪ್ರತ್ಯಾಗಮ delay style, N in [0..3]\n\
"), stdout);
#endif
#ifdef FFDLY
      fputs (_("\
 * ffN           form feed delay style, N in [0..1]\n\
"), stdout);
#endif
#ifdef NLDLY
      fputs (_("\
 * nlN           newline delay style, N in [0..1]\n\
"), stdout);
#endif
#ifdef OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage ಪ್ರತ್ಯಾಗಮ to newline\n\
"), stdout);
#endif
#ifdef OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters ಪ್ರತಿ fill instead of NUL characters\n\
"), stdout);
#endif
#ifdef OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead of timing ಪ್ರತಿ delays\n\
"), stdout);
#endif
#ifdef OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters to uppercase\n\
"), stdout);
#endif
#ifdef ONLCR
      fputs (_("\
 * [-]onlcr      translate newline to carriage ಪ್ರತ್ಯಾಗಮ-newline\n\
"), stdout);
#endif
#ifdef ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage ಪ್ರತ್ಯಾಗಮ\n\
"), stdout);
#endif
#ifdef ONOCR
      fputs (_("\
 * [-]onocr      ಕುರು not print carriage returns in the first column\n\
"), stdout);
#endif
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#ಯದಿ defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N in [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#endif
#ifdef VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N in [0..1]\n\
"), stdout);
#endif
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#ifdef ECHOKE
      fputs (_("\
 * crtkill       kill all ಸಾಲು by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all ಸಾಲು by obeying the echoctl and echok settings\n\
"), stdout);
#endif
#ifdef ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters in hat notation ('^c')\n\
"), stdout);
#endif
      fputs (_("\
   [-]echo       echo ಒಳಹರಿವು characters\n\
"), stdout);
#ifdef ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#endif
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#ifdef ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#endif
      fputs (_("\
   [-]echonl     echo newline even ಯದಿ not echoing other characters\n\
"), stdout);
#ifdef ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#endif
#ಯದಿ defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with ಮೇಲಿನ latency links\n\
"), stdout);
#endif
#ಯದಿ defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#endif
      printf (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#ifdef VWERASE
    ", werase"
#endif
#ಯದಿ defined CREPRINT || defined VREPRINT
    ", rprnt"
#endif
);
      fputs (_("\
   [-]isig       enable interrupt, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after interrupt and quit special characters\n\
"), stdout);
#ifdef ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#endif
#ifdef TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try to ಲಿಖ to the terminal\n\
"), stdout);
#endif
#ifdef XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' ಪ್ರತಿ uppercase characters\n\
"), stdout);
#endif
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#ಯದಿ defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#endif
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters to their ಸ್ವಭಾವ ಮೌಲ್ಯಗಳು\n\
   -cooked       same as raw\n\
"), stdout);
      printf (_("\
   crt           same as %s\n\
"), "echoe"
#ifdef ECHOCTL
    " echoctl"
#endif
#ifdef ECHOKE
    " echoke"
#endif
);
      printf (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#ifdef ECHOCTL
    " echoctl"
#endif
#ifdef ECHOKE
    " echoke"
#endif
#ifdef IXANY
    " -ixany"
#endif
);
#ifdef IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#endif
      fputs (_("\
   ek            erase and kill characters to their ಸ್ವಭಾವ ಮೌಲ್ಯಗಳು\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#ಯದಿ defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#endif
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      printf (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#ifdef ONLCR
   " -onlcr"
#endif
  , "icrnl -inlcr -igncr"
#ifdef ONLCR
   " onlcr"
#endif
#ifdef OCRNL
   " -ocrnl"
#endif
#ifdef ONLRET
   " -onlret"
#endif
);
      fputs (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), stdout);
      printf (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 time 0\n\
   -raw          same as cooked\n\
"),
#ifdef IUCLC
   " -iuclc"
#endif
#ifdef IXANY
   " -ixany"
#endif
#ifdef IMAXBEL
   " -imaxbel"
#endif
#ifdef XCASE
   " -xcase"
#endif
);
      printf (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters to their ಸ್ವಭಾವ ಮೌಲ್ಯಗಳು\n\
"),
   "-ixoff"
#ifdef IUTF8
   " -iutf8"
#endif
#ifdef IUCLC
   " -iuclc"
#endif
#ifdef IXANY
   " -ixany"
#endif
#ifdef IMAXBEL
   " imaxbel"
#endif
#ifdef XCASE
   " -xcase"
#endif
#ifdef OLCUC
   " -olcuc"
#endif
#ifdef OCRNL
   " -ocrnl"
#endif

 , "opost"
#ifdef OFILL
   " -ofill"
#endif
#ifdef ONLCR
   " onlcr"
#endif
#ifdef ONOCR
   " -onocr"
#endif
#ifdef ONLRET
   " -onlret"
#endif
#ifdef NLDLY
   " nl0"
#endif
#ifdef CRDLY
   " cr0"
#endif
#ifdef TAB0
   " tab0"
#endif
#ifdef BSDLY
   " bs0"
#endif
#ifdef VTDLY
   " vt0"
#endif
#ifdef FFDLY
   " ff0"
#endif

 , "isig"
#ifdef TOSTOP
   " -tostop"
#endif
#ifdef OFDEL
   " -ofdel"
#endif
#ifdef ECHOPRT
   " -echoprt"
#endif
#ifdef ECHOCTL
   " echoctl"
#endif
#ifdef ECHOKE
   " echoke"
#endif
#ifdef EXTPROC
   " -extproc"
#endif
#ifdef FLUSHO
   " -flusho"
#endif
);
      fputs (_("\
\n\
Handle the tty ಸಾಲು connected to standard ಒಳಹರಿವು.  Without arguments,\n\
prints baud rate, ಸಾಲು discipline, and deviations from stty sane.  In\n\
settings, CHAR is taken literally, or coded as in ^c, 0x37, 0177 or\n\
127; special ಮೌಲ್ಯಗಳು ^- or undef used to disable special characters.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

ಸ್ಥಿರ ಶೂನ್ಯ
apply_settings (bool checking, ವರ್ಣ ಧ್ರುವ *device_name,
                ವರ್ಣ * ಧ್ರುವ *settings, ಪೂರ್ಣಾಙ್ಕ n_settings,
                ಸಂರಚನಾ termios *mode, bool *require_set_attr)
{
#define check_argument(arg)						\
  ಕುರು									\
    {									\
      ಯದಿ (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  ಯಾವತ್ (0)

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 1; k < n_settings; k++)
    {
      ವರ್ಣ ಧ್ರುವ *arg = settings[k];
      bool match_found = false;
      bool not_set_attr = false;
      bool reversed = false;

      ಯದಿ (! arg)
        ಅನುವರ್ತ;

      ಯದಿ (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      ಯದಿ (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          ಅನುವರ್ತ;
        }
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; mode_info[i].name != ರಿಕ್ತ; ++i)
        {
          ಯದಿ (streq (arg, mode_info[i].name))
            {
              ಯದಿ ((mode_info[i].flags & NO_SETATTR) == 0)
                {
                  match_found = set_mode (&mode_info[i], reversed, mode);
                  *require_set_attr = true;
                }
              ಅನ್ಯಥಾ
                match_found = not_set_attr = true;
              ಭಙ್ಗ;
            }
        }
      ಯದಿ (!match_found && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      ಯದಿ (!match_found)
        {
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; control_info[i].name != ರಿಕ್ತ; ++i)
            {
              ಯದಿ (streq (arg, control_info[i].name))
                {
                  check_argument (arg);
                  match_found = true;
                  ++k;
                  set_control_char (&control_info[i], settings[k], mode);
                  *require_set_attr = true;
                  ಭಙ್ಗ;
                }
            }
        }
      ಯದಿ (!match_found || not_set_attr)
        {
          ಯದಿ (streq (arg, "ispeed"))
            {
              check_argument (arg);
              ++k;
              ಸಂರಚನಾ speed_desc ಧ್ರುವ ispeed = string_to_baud (settings[k]);
              ಯದಿ (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (ಒಳಹರಿವು_speed, settings[k], ispeed, mode);
              ಯದಿ (checking)
                ಅನುವರ್ತ;
              *require_set_attr = true;
            }
          ಅನ್ಯಥಾ ಯದಿ (streq (arg, "ospeed"))
            {
              check_argument (arg);
              ++k;
              ಸಂರಚನಾ speed_desc ಧ್ರುವ ospeed = string_to_baud (settings[k]);
              ಯದಿ (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (output_speed, settings[k], ospeed, mode);
              ಯದಿ (checking)
                ಅನುವರ್ತ;
              *require_set_attr = true;
            }
#ifdef TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          ಅನ್ಯಥಾ ಯದಿ (streq (arg, "extproc"))
            {
              ಪೂರ್ಣಾಙ್ಕ val = ! reversed;

              ಯದಿ (checking)
                ಅನುವರ್ತ;

              ಯದಿ (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#endif
#ifdef TIOCGWINSZ
          ಅನ್ಯಥಾ ಯದಿ (streq (arg, "rows"))
            {
              check_argument (arg);
              ++k;
              ಯದಿ (checking)
                ಅನುವರ್ತ;
              set_window_size (integer_arg (settings[k], INT_MAX), -1,
                               device_name);
            }
          ಅನ್ಯಥಾ ಯದಿ (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_argument (arg);
              ++k;
              ಯದಿ (checking)
                ಅನುವರ್ತ;
              set_window_size (-1, integer_arg (settings[k], INT_MAX),
                               device_name);
            }
          ಅನ್ಯಥಾ ಯದಿ (streq (arg, "size"))
            {
              ಯದಿ (checking)
                ಅನುವರ್ತ;
              max_col = screen_columns ();
              current_col = 0;
              display_window_size (false, device_name);
            }
#endif
#ifdef HAVE_C_ಸಾಲು
          ಅನ್ಯಥಾ ಯದಿ (streq (arg, "line"))
            {
              check_argument (arg);
              ++k;
              uintmax_t ಮೌಲ್ಯ = integer_arg (settings[k], UINTMAX_MAX);
              ಯದಿ (ckd_add (&mode->c_ಸಾಲು, ಮೌಲ್ಯ, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_set_attr = true;
            }
#endif
          ಅನ್ಯಥಾ ಯದಿ (streq (arg, "speed"))
            {
              ಯದಿ (checking)
                ಅನುವರ್ತ;
              max_col = screen_columns ();
              display_speed (mode, false);
            }
          ಅನ್ಯಥಾ
            {
              ಸಂರಚನಾ speed_desc ಧ್ರುವ speed = string_to_baud (arg);
              ಯದಿ (speed.valid)
                {
                  set_speed (both_speeds, arg, speed, mode);
                  ಯದಿ (checking)
                    ಅನುವರ್ತ;
                  *require_set_attr = true;
                }
              ಅನ್ಯಥಾ
                {
                  ಯದಿ (! recover_mode (arg, mode))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_set_attr = true;
                }
            }
        }
    }

  ಯದಿ (checking)
    check_speed (mode);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ಸ್ಥಿರ ಸಂರಚನಾ termios mode;

  ಗಣನಾ output_type output_type;
  ಪೂರ್ಣಾಙ್ಕ optc;
  ಪೂರ್ಣಾಙ್ಕ argi = 0;
  ಪೂರ್ಣಾಙ್ಕ opti = 1;
  bool require_set_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  ವರ್ಣ *file_name = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *device_name;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = changed;
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

  ಯಾವತ್ ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'a':
          verbose_output = true;
          output_type = all;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'g':
          recoverable_output = true;
          output_type = recoverable;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'F':
          ಯದಿ (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEV_DEBUG_OPTION:
          dev_debug = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          ಯದಿ (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          ಭಙ್ಗ;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      ಯಾವತ್ (opti < optind)
        argv[argi + opti++] = ರಿಕ್ತ;
    }

  /* Specifying both -a and -g gets an error.  */
  ಯದಿ (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  ಯದಿ (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  ಯದಿ (!noargs && !verbose_output && !recoverable_output)
    {
      ಸ್ಥಿರ ಸಂರಚನಾ termios check_mode;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_mode, &require_set_attr);
    }

  ಯದಿ (file_name)
    {
      ಪೂರ್ಣಾಙ್ಕ fdflags;
      ಯದಿ (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ಯದಿ ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  ಯದಿ (tcgetattr (STDIN_FILENO, &mode))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  ಯದಿ (verbose_output || recoverable_output || noargs)
    {
      max_col = screen_columns ();
      current_col = 0;
      display_settings (output_type, &mode, device_name);
      ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
    }

  require_set_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &mode, &require_set_attr);

  ಯದಿ (require_set_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ಸ್ಥಿರ ಸಂರಚನಾ termios new_mode;

      ಯದಿ (tcsetattr (STDIN_FILENO, tcsetattr_options, &mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      ಯದಿ (tcgetattr (STDIN_FILENO, &new_mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ಯದಿ (! eq_mode (&mode, &new_mode))
        {
          ಯದಿ (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಪರಿಮಾಣ (new_mode); i++)
                {
                  unsigned ಪೂರ್ಣಾಙ್ಕ newc = *(((unsigned ವರ್ಣ *) &new_mode) + i);
                  unsigned ಪೂರ್ಣಾಙ್ಕ oldc = *(((unsigned ವರ್ಣ *) &mode) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
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
eq_mode (ಸಂರಚನಾ termios ಧ್ರುವ *mode1, ಸಂರಚನಾ termios ಧ್ರುವ *mode2)
{
  ಯದಿ (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#ifdef HAVE_C_ಸಾಲು
      || mode1->c_ಸಾಲು != mode2->c_ಸಾಲು
#endif
      || !memeq (mode1->c_cc, mode2->c_cc, ಪರಿಮಾಣ (mode1->c_cc)))
    ಪ್ರತ್ಯಾಗಮ false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_mask = ~(tcflag_t) 0;
#ifdef CBAUD
  cflag_mask &= ~(tcflag_t) CBAUD;
#endif
#ifdef CIBAUD
  cflag_mask &= ~(tcflag_t) CIBAUD;
#endif

  ಯದಿ ((mode1->c_cflag ^ mode2->c_cflag) & cflag_mask)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ speed_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ಸ್ಥಿರ bool
set_mode (ಸಂರಚನಾ mode_info ಧ್ರುವ *info, bool reversed, ಸಂರಚನಾ termios *mode)
{
  tcflag_t *bitsp;

  ಯದಿ (reversed && (info->flags & REV) == 0)
    ಪ್ರತ್ಯಾಗಮ false;

  bitsp = mode_type_flag (info->type, mode);

  ಯದಿ (bitsp == ರಿಕ್ತ)
    {
      /* Combination mode. */
      ಯದಿ (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          ಯದಿ (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ಅನ್ಯಥಾ
            mode->c_cflag = (mode->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "oddp"))
        {
          ಯದಿ (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ಅನ್ಯಥಾ
            mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "nl"))
        {
          ಯದಿ (reversed)
            {
              mode->c_iflag = (mode->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              mode->c_oflag = (mode->c_oflag
#ifdef ONLCR
                               | ONLCR
#endif
                )
#ifdef OCRNL
                & ~OCRNL
#endif
#ifdef ONLRET
                & ~ONLRET
#endif
                ;
            }
          ಅನ್ಯಥಾ
            {
              mode->c_iflag = mode->c_iflag & ~ICRNL;
#ifdef ONLCR
              mode->c_oflag = mode->c_oflag & ~ONLCR;
#endif
            }
        }
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "ek"))
        {
          mode->c_cc[VERASE] = CERASE;
          mode->c_cc[VKILL] = CKILL;
        }
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "sane"))
        sane_mode (mode);
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "cbreak"))
        {
          ಯದಿ (reversed)
            mode->c_lflag |= ICANON;
          ಅನ್ಯಥಾ
            mode->c_lflag &= ~ICANON;
        }
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "pass8"))
        {
          ಯದಿ (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
            }
          ಅನ್ಯಥಾ
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
            }
        }
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "litout"))
        {
          ಯದಿ (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
              mode->c_oflag |= OPOST;
            }
          ಅನ್ಯಥಾ
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
              mode->c_oflag &= ~OPOST;
            }
        }
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          ಯದಿ ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              mode->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              mode->c_oflag |= OPOST;
              mode->c_lflag |= ISIG | ICANON;
#ಯದಿ VMIN == VEOF
              mode->c_cc[VEOF] = CEOF;
#endif
#ಯದಿ VTIME == VEOL
              mode->c_cc[VEOL] = CEOL;
#endif
            }
          ಅನ್ಯಥಾ
            {
              /* Raw mode. */
              mode->c_iflag = 0;
              mode->c_oflag &= ~OPOST;
              mode->c_lflag &= ~(ISIG | ICANON
#ifdef XCASE
                                 | XCASE
#endif
                );
              mode->c_cc[VMIN] = 1;
              mode->c_cc[VTIME] = 0;
            }
        }
#ifdef IXANY
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "decctlq"))
        {
          ಯದಿ (reversed)
            mode->c_iflag |= IXANY;
          ಅನ್ಯಥಾ
            mode->c_iflag &= ~IXANY;
        }
#endif
#ifdef TABDLY
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "tabs"))
        {
          ಯದಿ (reversed)
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB3;
          ಅನ್ಯಥಾ
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB0;
        }
#ಅನ್ಯಥಾ
# ifdef OXTABS
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "tabs"))
        {
          ಯದಿ (reversed)
            mode->c_oflag = mode->c_oflag | OXTABS;
          ಅನ್ಯಥಾ
            mode->c_oflag = mode->c_oflag & ~OXTABS;
        }
# endif
#endif
#ಯದಿ defined XCASE && defined IUCLC && defined OLCUC
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          ಯದಿ (reversed)
            {
              mode->c_lflag &= ~XCASE;
              mode->c_iflag &= ~IUCLC;
              mode->c_oflag &= ~OLCUC;
            }
          ಅನ್ಯಥಾ
            {
              mode->c_lflag |= XCASE;
              mode->c_iflag |= IUCLC;
              mode->c_oflag |= OLCUC;
            }
        }
#endif
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "crt"))
        mode->c_lflag |= ECHOE
#ifdef ECHOCTL
          | ECHOCTL
#endif
#ifdef ECHOKE
          | ECHOKE
#endif
          ;
      ಅನ್ಯಥಾ ಯದಿ (streq (info->name, "dec"))
        {
          mode->c_cc[VINTR] = 3;	/* ^C */
          mode->c_cc[VERASE] = 127;	/* DEL */
          mode->c_cc[VKILL] = 21;	/* ^U */
          mode->c_lflag |= ECHOE
#ifdef ECHOCTL
            | ECHOCTL
#endif
#ifdef ECHOKE
            | ECHOKE
#endif
            ;
#ifdef IXANY
          mode->c_iflag &= ~IXANY;
#endif
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (reversed)
    *bitsp = *bitsp & ~info->mask & ~info->bits;
  ಅನ್ಯಥಾ
    *bitsp = (*bitsp & ~info->mask) | info->bits;

  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
set_control_char (ಸಂರಚನಾ control_info ಧ್ರುವ *info, ವರ್ಣ ಧ್ರುವ *arg,
                  ಸಂರಚನಾ termios *mode)
{
  unsigned long ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;

  ಯದಿ (streq (info->name, "min") || streq (info->name, "time"))
    ಮೌಲ್ಯ = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  ಅನ್ಯಥಾ ಯದಿ (arg[0] == '\0' || arg[1] == '\0')
    ಮೌಲ್ಯ = to_uchar (arg[0]);
  ಅನ್ಯಥಾ ಯದಿ (streq (arg, "^-") || streq (arg, "undef"))
    ಮೌಲ್ಯ = _POSIX_VDISABLE;
  ಅನ್ಯಥಾ ಯದಿ (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      ಯದಿ (arg[1] == '?')
        ಮೌಲ್ಯ = 127;
      ಅನ್ಯಥಾ
        ಮೌಲ್ಯ = to_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  ಅನ್ಯಥಾ
    ಮೌಲ್ಯ = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  mode->c_cc[info->offset] = ಮೌಲ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
set_speed (ಗಣನಾ speed_setting type, ವರ್ಣ ಧ್ರುವ *arg,
           ಸಂರಚನಾ speed_desc ಧ್ರುವ speed, ಸಂರಚನಾ termios *mode)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (speed.valid);

  ಯದಿ (type == ಒಳಹರಿವು_speed || type == both_speeds)
    {
      last_ibaud = speed;
      ಯದಿ (cfsetispeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  ಯದಿ (type == output_speed || type == both_speeds)
    {
      last_obaud = speed;
      ಯದಿ (cfsetospeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#ifdef TIOCGWINSZ

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_win_size (ಪೂರ್ಣಾಙ್ಕ fd, ಸಂರಚನಾ winsize *win)
{
  ಪೂರ್ಣಾಙ್ಕ err = ioctl (fd, TIOCGWINSZ, (ವರ್ಣ *) win);
  ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ ಶೂನ್ಯ
set_window_size (ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ವರ್ಣ ಧ್ರುವ *device_name)
{
  ಸಂರಚನಾ winsize win;

  ಯದಿ (get_win_size (STDIN_FILENO, &win))
    {
      ಯದಿ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, ಪರಿಮಾಣ (win));
    }

  ಯದಿ (rows >= 0)
    win.ws_row = rows;
  ಯದಿ (cols >= 0)
    win.ws_col = cols;

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

  ಯದಿ (win.ws_row == 0 || win.ws_col == 0)
    {
      ಸಂರಚನಾ ttysize ttysz;

      ttysz.ts_ಸಾಲುಗಳು = win.ws_row;
      ttysz.ts_cols = win.ws_col;

      win.ws_row = 1;
      win.ws_col = 1;

      ಯದಿ (ioctl (STDIN_FILENO, TIOCSWINSZ, (ವರ್ಣ *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ಯದಿ (ioctl (STDIN_FILENO, TIOCSSIZE, (ವರ್ಣ *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ಪ್ರತ್ಯಾಗಮ;
    }
# endif

  ಯದಿ (ioctl (STDIN_FILENO, TIOCSWINSZ, (ವರ್ಣ *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_window_size (bool fancy, ವರ್ಣ ಧ್ರುವ *device_name)
{
  ಸಂರಚನಾ winsize win;

  ಯದಿ (get_win_size (STDIN_FILENO, &win))
    {
      ಯದಿ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ಯದಿ (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  ಅನ್ಯಥಾ
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_row, win.ws_col);
      ಯದಿ (!fancy)
        current_col = 0;
    }
}
#endif

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
screen_columns (ಶೂನ್ಯ)
{
#ifdef TIOCGWINSZ
  ಸಂರಚನಾ winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  ಯದಿ (get_win_size (STDOUT_FILENO, &win) == 0 && 0 < win.ws_col)
    ಪ್ರತ್ಯಾಗಮ win.ws_col;
#endif
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    ವರ್ಣ *col_string = getenv ("COLUMNS");
    long ಪೂರ್ಣಾಙ್ಕ n_columns;
    ಯದಿ (!(col_string != ರಿಕ್ತ
          && xstrtol (col_string, ರಿಕ್ತ, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= INT_MAX))
      n_columns = 80;
    ಪ್ರತ್ಯಾಗಮ n_columns;
  }
}

ATTRIBUTE_PURE
ಸ್ಥಿರ tcflag_t *
mode_type_flag (ಗಣನಾ mode_type type, ಸಂರಚನಾ termios *mode)
{
  ವಿಕಲ್ಪ (type)
    {
    ಪಕ್ಷ control:
      ಪ್ರತ್ಯಾಗಮ &mode->c_cflag;

    ಪಕ್ಷ ಒಳಹರಿವು:
      ಪ್ರತ್ಯಾಗಮ &mode->c_iflag;

    ಪಕ್ಷ output:
      ಪ್ರತ್ಯಾಗಮ &mode->c_oflag;

    ಪಕ್ಷ local:
      ಪ್ರತ್ಯಾಗಮ &mode->c_lflag;

    ಪಕ್ಷ combination:
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಸ್ವಭಾವ:
      unreachable ();
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_settings (ಗಣನಾ output_type output_type, ಸಂರಚನಾ termios *mode,
                  ವರ್ಣ ಧ್ರುವ *device_name)
{
  ವಿಕಲ್ಪ (output_type)
    {
    ಪಕ್ಷ changed:
      display_changed (mode);
      ಭಙ್ಗ;

    ಪಕ್ಷ all:
      display_all (mode, device_name);
      ಭಙ್ಗ;

    ಪಕ್ಷ recoverable:
      display_recoverable (mode);
      ಭಙ್ಗ;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_changed (ಸಂರಚನಾ termios *mode)
{
  bool empty_ಸಾಲು;
  tcflag_t *bitsp;
  unsigned long mask;
  ಗಣನಾ mode_type prev_type = control;

  display_speed (mode, true);
#ifdef HAVE_C_ಸಾಲು
  wrapf ("line = %d;", mode->c_ಸಾಲು);
#endif
  putchar ('\n');
  current_col = 0;

  empty_ಸಾಲು = true;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      ಯದಿ (mode->c_cc[control_info[i].offset] == control_info[i].saneval)
        ಅನುವರ್ತ;

#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ಯದಿ (streq (control_info[i].name, "flush"))
        ಅನುವರ್ತ;
#endif
      /* If swtch is the same as susp, don't print both.  */
#ಯದಿ VSWTCH == VSUSP
      ಯದಿ (streq (control_info[i].name, "swtch"))
        ಅನುವರ್ತ;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#ಯದಿ VEOF == VMIN
      ಯದಿ ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ಅನುವರ್ತ;
#endif

      empty_ಸಾಲು = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
  ಯದಿ ((mode->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (unsigned long ಪೂರ್ಣಾಙ್ಕ) mode->c_cc[VMIN],
             (unsigned long ಪೂರ್ಣಾಙ್ಕ) mode->c_cc[VTIME]);
    }
  ಅನ್ಯಥಾ ಯದಿ (!empty_ಸಾಲು)
    putchar ('\n');
  current_col = 0;

  empty_ಸಾಲು = true;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; mode_info[i].name != ರಿಕ್ತ; ++i)
    {
      ಯದಿ (mode_info[i].flags & OMIT)
        ಅನುವರ್ತ;
      ಯದಿ (mode_info[i].type != prev_type)
        {
          ಯದಿ (!empty_ಸಾಲು)
            {
              putchar ('\n');
              current_col = 0;
              empty_ಸಾಲು = true;
            }
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      ಯದಿ ((*bitsp & mask) == mode_info[i].bits)
        {
          ಯದಿ (mode_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", mode_info[i].name);
              empty_ಸಾಲು = false;
            }
        }
      ಅನ್ಯಥಾ ಯದಿ ((mode_info[i].flags & (SANE_SET | REV)) == (SANE_SET | REV))
        {
          wrapf ("-%s", mode_info[i].name);
          empty_ಸಾಲು = false;
        }
    }
  ಯದಿ (!empty_ಸಾಲು)
    putchar ('\n');
  current_col = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_all (ಸಂರಚನಾ termios *mode, ವರ್ಣ ಧ್ರುವ *device_name)
{
  tcflag_t *bitsp;
  unsigned long mask;
  ಗಣನಾ mode_type prev_type = control;

  display_speed (mode, true);
#ifdef TIOCGWINSZ
  display_window_size (true, device_name);
#endif
#ifdef HAVE_C_ಸಾಲು
  wrapf ("line = %d;", mode->c_ಸಾಲು);
#endif
  putchar ('\n');
  current_col = 0;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ಯದಿ (streq (control_info[i].name, "flush"))
        ಅನುವರ್ತ;
#endif
      /* If swtch is the same as susp, don't print both.  */
#ಯದಿ VSWTCH == VSUSP
      ಯದಿ (streq (control_info[i].name, "swtch"))
        ಅನುವರ್ತ;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#ಯದಿ VEOF == VMIN
      ಯದಿ ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ಅನುವರ್ತ;
#endif
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
#ಯದಿ VEOF == VMIN
  ಯದಿ ((mode->c_lflag & ICANON) == 0)
#endif
    wrapf ("min = %lu; time = %lu;",
           (unsigned long ಪೂರ್ಣಾಙ್ಕ) mode->c_cc[VMIN],
           (unsigned long ಪೂರ್ಣಾಙ್ಕ) mode->c_cc[VTIME]);
  ಯದಿ (current_col != 0)
    putchar ('\n');
  current_col = 0;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; mode_info[i].name != ರಿಕ್ತ; ++i)
    {
      ಯದಿ (mode_info[i].flags & OMIT)
        ಅನುವರ್ತ;
      ಯದಿ (mode_info[i].type != prev_type)
        {
          putchar ('\n');
          current_col = 0;
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      ಯದಿ ((*bitsp & mask) == mode_info[i].bits)
        wrapf ("%s", mode_info[i].name);
      ಅನ್ಯಥಾ ಯದಿ (mode_info[i].flags & REV)
        wrapf ("-%s", mode_info[i].name);
    }
  putchar ('\n');
  current_col = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ಸ್ಥಿರ ಶೂನ್ಯ
check_speed (ಸಂರಚನಾ termios *mode)
{
  ಯದಿ (last_ibaud.valid && last_obaud.valid)
    {
      ಯದಿ (cfgetispeed (mode) != last_ibaud.baud
          || cfgetospeed (mode) != last_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_to_ಮೌಲ್ಯ (last_ibaud.baud),
               baud_to_ಮೌಲ್ಯ (last_obaud.baud));
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_speed (ಸಂರಚನಾ termios *mode, bool fancy)
{
  ಯದಿ (cfgetispeed (mode) == 0 || cfgetispeed (mode) == cfgetospeed (mode))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_to_ಮೌಲ್ಯ (cfgetospeed (mode)));
  ಅನ್ಯಥಾ
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_to_ಮೌಲ್ಯ (cfgetispeed (mode)),
           baud_to_ಮೌಲ್ಯ (cfgetospeed (mode)));
  ಯದಿ (!fancy)
    current_col = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
display_recoverable (ಸಂರಚನಾ termios *mode)
{
  printf ("%lx:%lx:%lx:%lx",
          (unsigned long ಪೂರ್ಣಾಙ್ಕ) mode->c_iflag,
          (unsigned long ಪೂರ್ಣಾಙ್ಕ) mode->c_oflag,
          (unsigned long ಪೂರ್ಣಾಙ್ಕ) mode->c_cflag,
          (unsigned long ಪೂರ್ಣಾಙ್ಕ) mode->c_lflag);
  ಪ್ರತಿ (size_t i = 0; i < NCCS; ++i)
    printf (":%lx", (unsigned long ಪೂರ್ಣಾಙ್ಕ) mode->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#ifdef CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (mode->c_cflag & CBAUD) == BOTHER;
#  ಯದಿ defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((mode->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#endif
  ಯದಿ (separate_speeds)
    printf (":%lx:%lx", (unsigned long ಪೂರ್ಣಾಙ್ಕ) cfgetispeed (mode),
            (unsigned long ಪೂರ್ಣಾಙ್ಕ) cfgetospeed (mode));

  putchar ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#define STRTOUL_TYPE(TYPE)                                               \
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ                                                             \
  strtoul_##TYPE (ವರ್ಣ ಧ್ರುವ *s, ಪೂರ್ಣಾಙ್ಕ base, ವರ್ಣ **p, TYPE *result,       \
                  ವರ್ಣ delim)                                            \
  {                                                                      \
    unsigned long ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, base);                                           \
    ಯದಿ (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      ಪ್ರತ್ಯಾಗಮ -1;                                                         \
    *result = ul;                                                        \
    ಪ್ರತ್ಯಾಗಮ 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (speed_t)
#undef STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#ಅನ್ತರ್ಭಾವ "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ಸ್ಥಿರ bool
recover_mode (ವರ್ಣ ಧ್ರುವ *arg, ಸಂರಚನಾ termios *mode)
{
  tcflag_t flag[4];
  ವರ್ಣ ಧ್ರುವ *s = arg;
  ಪ್ರತಿ (size_t i = 0; i < 4; i++)
    {
      ವರ್ಣ *p;
      ಯದಿ (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        ಪ್ರತ್ಯಾಗಮ false;
      s = p + 1;
    }
  mode->c_iflag = flag[0];
  mode->c_oflag = flag[1];
  mode->c_cflag = flag[2];
  mode->c_lflag = flag[3];

  ಪ್ರತಿ (size_t i = 0; i < NCCS; ++i)
    {
      ವರ್ಣ *p;
      ವರ್ಣ delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      ಯದಿ (strtoul_cc_t (s, 16, &p, mode->c_cc + i, delim) != 0)
        ಪ್ರತ್ಯಾಗಮ false;
      /* Accept the old format, which has no separate speed fields.  */
      ಯದಿ (! *p)
        {
#ifdef CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_SPEED_T_SANE
          ಸಂರಚನಾ speed_desc ospeed = cbaud_to_speed (mode->c_cflag & CBAUD);
          ಯದಿ (ospeed.valid && cfsetospeed (mode, ospeed.baud) != 0)
            ಪ್ರತ್ಯಾಗಮ false;
#  ಯದಿ defined CIBAUD && defined IBSHIFT
          ಸಂರಚನಾ speed_desc ispeed =
            cbaud_to_speed ((mode->c_cflag & CIBAUD) >> IBSHIFT);
          ಯದಿ (ispeed.valid && cfsetispeed (mode, ispeed.baud) != 0)
            ಪ್ರತ್ಯಾಗಮ false;
#  endif
# endif
          last_ibaud.baud = cfgetispeed (mode);
          last_obaud.baud = cfgetospeed (mode);
#endif
          ಪ್ರತ್ಯಾಗಮ true;
        }
      s = p + 1;
    }

  speed_t speed[2];
  ಪ್ರತಿ (size_t i = 0; i < 2; i++)
    {
      ವರ್ಣ *p;
      ವರ್ಣ delim = i == 0 ? ':' : '\0';
      ಯದಿ (strtoul_speed_t (s, 16, &p, speed + i, delim) != 0)
        ಪ್ರತ್ಯಾಗಮ false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  ಯದಿ (cfsetospeed (mode, speed[1]) != 0
      || cfsetispeed (mode, speed[0]) != 0)
    ಪ್ರತ್ಯಾಗಮ false;

  last_ibaud.baud = speed[0] == 0 ? cfgetispeed (mode) : speed[0];
  last_obaud.baud = speed[1];
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ಸ್ಥಿರ bool
speed_matches (ಸಂರಚನಾ termios ಧ್ರುವ *mode1, ಸಂರಚನಾ termios ಧ್ರುವ *mode2)
{
  speed_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  speed_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  ಯದಿ (speed1[1] == 0)
    speed1[1] = speed1[0];
  ಯದಿ (speed2[1] == 0)
    speed2[1] = speed2[0];

  unsigned ಪೂರ್ಣಾಙ್ಕ bits = 2;  /* Start and stop bits.  */
  bits += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  bits += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  ವಿಕಲ್ಪ (mode1->c_cflag & CSIZE)
    {
      ಪಕ್ಷ CS5: bits += 5; ಭಙ್ಗ;
      ಪಕ್ಷ CS6: bits += 6; ಭಙ್ಗ;
      ಪಕ್ಷ CS7: bits += 7; ಭಙ್ಗ;
      ಸ್ವಭಾವ: bits += 8; ಭಙ್ಗ;
    }

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; i++)
    {
      unsigned long ಪೂರ್ಣಾಙ್ಕ value1;
      unsigned long ಪೂರ್ಣಾಙ್ಕ value2;
      unsigned long ಪೂರ್ಣಾಙ್ಕ delta;

      ಯದಿ (speed1[i] == speed2[i])
        ಅನುವರ್ತ;

      value1 = baud_to_ಮೌಲ್ಯ (speed1[i]);
      value2 = baud_to_ಮೌಲ್ಯ (speed2[i]);

#ifndef TERMIOS_SPEED_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      ಯದಿ (value1 == ULONG_MAX || value2 == ULONG_MAX)
        ಪ್ರತ್ಯಾಗಮ false;
#endif

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      ಯದಿ (delta > value1 / (bits * 4))
        ಪ್ರತ್ಯಾಗಮ false;
    }

  ಪ್ರತ್ಯಾಗಮ true;
}

ATTRIBUTE_PURE
ಸ್ಥಿರ ಸಂರಚನಾ speed_desc
string_to_baud (ವರ್ಣ ಧ್ರುವ *arg)
{
  ವರ್ಣ *ep;
  unsigned long ಮೌಲ್ಯ;
  unsigned ವರ್ಣ c;

  /* Explicitly disallow negative numbers.  */
  ಯಾವತ್ (c_isspace (*arg))
    arg++;
  ಯದಿ (*arg == '-')
    ಗಚ್ಛ invalid;

  /* Empty does not mean 0.  */
  ಯದಿ (! *arg)
    ಗಚ್ಛ invalid;

  ಮೌಲ್ಯ = strtoul (arg, &ep, 10);

  c = *ep++;
  ಯದಿ (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      ಯದಿ (c)
        {
          unsigned ವರ್ಣ d = c - '0';
          ಯದಿ (d > 5)
            ಮೌಲ್ಯ++;
          ಅನ್ಯಥಾ ಯದಿ (d == 5)
            {
              ಯಾವತ್ ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              ಯದಿ (c)
                ಮೌಲ್ಯ++;                /* Nonzero, round up */
              ಅನ್ಯಥಾ
                ಮೌಲ್ಯ += (ಮೌಲ್ಯ & 1);   /* Exactly in the middle, round even */
            }

          ಯಾವತ್ (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          ಯದಿ (c)
            ಗಚ್ಛ invalid; /* Garbage after otherwise valid number */
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      ಯದಿ (streq (arg, "exta"))
        ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ speed_desc) { .baud = B19200, .valid = true };
      ಅನ್ಯಥಾ ಯದಿ (streq (arg, "extb"))
        ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ speed_desc) { .baud = B38400, .valid = true };
      ಅನ್ಯಥಾ
        ಗಚ್ಛ invalid;
    }

  ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ_to_baud (ಮೌಲ್ಯ);

 invalid:
  ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ speed_desc) { .baud = (speed_t) -1, .valid = false };
}

ಸ್ಥಿರ ಶೂನ್ಯ
sane_mode (ಸಂರಚನಾ termios *mode)
{
  tcflag_t *bitsp;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; control_info[i].name; ++i)
    {
#ಯದಿ VMIN == VEOF
      ಯದಿ (streq (control_info[i].name, "min"))
        ಭಙ್ಗ;
#endif
      mode->c_cc[control_info[i].offset] = control_info[i].saneval;
    }

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; mode_info[i].name != ರಿಕ್ತ; ++i)
    {
      ಯದಿ (mode_info[i].flags & NO_SETATTR)
        ಅನುವರ್ತ;

      ಯದಿ (mode_info[i].flags & SANE_SET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~mode_info[i].mask) | mode_info[i].bits;
        }
      ಅನ್ಯಥಾ ಯದಿ (mode_info[i].flags & SANE_UNSET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~mode_info[i].mask & ~mode_info[i].bits;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
visible (cc_t ch)
{
  ಸ್ಥಿರ ವರ್ಣ buf[10];
  ವರ್ಣ *bpout = buf;

  ಯದಿ (ch == _POSIX_VDISABLE)
    ಪ್ರತ್ಯಾಗಮ "<undef>";

  ಯದಿ (ch >= 32)
    {
      ಯದಿ (ch < 127)
        *bpout++ = ch;
      ಅನ್ಯಥಾ ಯದಿ (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      ಅನ್ಯಥಾ
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          ಯದಿ (ch >= 128 + 32)
            {
              ಯದಿ (ch < 128 + 127)
                *bpout++ = ch - 128;
              ಅನ್ಯಥಾ
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          ಅನ್ಯಥಾ
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  ಅನ್ಯಥಾ
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ ಧ್ರುವ *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

ಸ್ಥಿರ uintmax_t
integer_arg (ವರ್ಣ ಧ್ರುವ *s, uintmax_t maxval)
{
  ಪ್ರತ್ಯಾಗಮ xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
