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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ifdef TERMIOS_NEEDS_XOPEN_SOURCE
# define _XOPEN_SOURCE
#endif

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ <termios.h>
#ଯଦି HAVE_STROPTS_H
# include <stropts.h>
#endif
#ଅନ୍ତର୍ଭାଵ <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif
#ifdef GWINSZ_IN_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#endif
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "fd-reopen.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "xdectoint.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

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
#ଯଦି defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#endif
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#ଯଦି defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#endif
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#ଯଦି defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# ଯଦି defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#endif
#ଯଦି defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#ଯଦି defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

#ଯଦି defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#endif
#ଯଦି defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#endif
#ଯଦି defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#ଯଦି defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#ଯଦି defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#endif
#ଯଦି defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#endif
#ଯଦି defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#endif
#ଯଦି defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#endif
#ଯଦି defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#endif
#ଯଦି defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#endif
#ଯଦି defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#endif
#ଯଦି defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#endif
#ଯଦି defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#endif

/* Which speeds to set.  */
ଗଣନା speed_setting
  {
    ନିବେଶ_speed, output_speed, both_speeds
  };

/* What to output and how.  */
ଗଣନା output_type
  {
    changed, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
ଗଣନା mode_type
  {
    control, ନିବେଶ, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#define SANE_SET 1		/* Set in 'sane' mode. */
#define SANE_UNSET 2		/* Unset in 'sane' mode. */
#define REV 4			/* Can be turned off by prepending '-'. */
#define OMIT 8			/* Don't display value. */
#define NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
ସଂରଚନା mode_info
  {
    ଅକ୍ଷର ଅଚଳ *name;		/* Name given on command line.  */
    ଗଣନା mode_type type;	/* Which structure element to change. */
    ଅକ୍ଷର flags;			/* Setting and display options.  */
    unsigned long bits;		/* Bits to set for this mode.  */
    unsigned long mask;		/* Other bits to turn off for this mode.  */
  };

ସ୍ଥିର ସଂରଚନା mode_info ଅଚଳ mode_info[] =
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

  {"ignbrk", ନିବେଶ, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", ନିବେଶ, SANE_SET | REV, BRKINT, 0},
  {"ignpar", ନିବେଶ, REV, IGNPAR, 0},
  {"parmrk", ନିବେଶ, REV, PARMRK, 0},
  {"inpck", ନିବେଶ, REV, INPCK, 0},
  {"istrip", ନିବେଶ, REV, ISTRIP, 0},
  {"inlcr", ନିବେଶ, SANE_UNSET | REV, INLCR, 0},
  {"igncr", ନିବେଶ, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", ନିବେଶ, SANE_SET | REV, ICRNL, 0},
  {"ixon", ନିବେଶ, REV, IXON, 0},
  {"ixoff", ନିବେଶ, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", ନିବେଶ, REV | OMIT, IXOFF, 0},
#ifdef IUCLC
  {"iuclc", ନିବେଶ, SANE_UNSET | REV, IUCLC, 0},
#endif
#ifdef IXANY
  {"ixany", ନିବେଶ, SANE_UNSET | REV, IXANY, 0},
#endif
#ifdef IMAXBEL
  {"imaxbel", ନିବେଶ, SANE_SET | REV, IMAXBEL, 0},
#endif
#ifdef IUTF8
  {"iutf8", ନିବେଶ, SANE_UNSET | REV, IUTF8, 0},
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
#ଅନ୍ୟଥା
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
#ଯଦି defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#ନଚେତଯଦି defined EXTPROC
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
#ଯଦି defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#endif
#ଯଦି defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#endif
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {ଖାଲି, control, 0, 0, 0}
};

/* Control character settings.  */
ସଂରଚନା control_info
  {
    ଅକ୍ଷର ଅଚଳ *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t offset;		/* Offset in c_cc.  */
  };

/* Control characters. */

ସ୍ଥିର ସଂରଚନା control_info ଅଚଳ control_info[] =
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
#ଅନ୍ୟଥା
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
  {ଖାଲି, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
ସଂରଚନା speed_desc
{
  /* The baud rate.  */
  speed_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *visible (cc_t ch);
ସ୍ଥିର unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା baud_to_ମୂଲ୍ୟ (speed_t speed);
ସ୍ଥିର bool recover_mode (ଅକ୍ଷର ଅଚଳ *arg, ସଂରଚନା termios *mode);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା screen_columns (ଶୂନ୍ୟ);
ସ୍ଥିର bool set_mode (ସଂରଚନା mode_info ଅଚଳ *info, bool reversed,
                      ସଂରଚନା termios *mode);
ସ୍ଥିର bool eq_mode (ସଂରଚନା termios ଅଚଳ *mode1,
                     ସଂରଚନା termios ଅଚଳ *mode2);
ସ୍ଥିର bool speed_matches (ସଂରଚନା termios ଅଚଳ *mode1,
                           ସଂରଚନା termios ଅଚଳ *mode2);
ସ୍ଥିର uintmax_t integer_arg (ଅକ୍ଷର ଅଚଳ *s, uintmax_t max);
ସ୍ଥିର ସଂରଚନା speed_desc string_to_baud (ଅକ୍ଷର ଅଚଳ *arg);
ସ୍ଥିର tcflag_t *mode_type_flag (ଗଣନା mode_type type, ସଂରଚନା termios *mode);
ସ୍ଥିର ଶୂନ୍ୟ display_all (ସଂରଚନା termios *mode, ଅକ୍ଷର ଅଚଳ *device_name);
ସ୍ଥିର ଶୂନ୍ୟ display_changed (ସଂରଚନା termios *mode);
ସ୍ଥିର ଶୂନ୍ୟ display_recoverable (ସଂରଚନା termios *mode);
ସ୍ଥିର ଶୂନ୍ୟ display_settings (ଗଣନା output_type output_type,
                              ସଂରଚନା termios *mode,
                              ଅକ୍ଷର ଅଚଳ *device_name);
ସ୍ଥିର ଶୂନ୍ୟ check_speed (ସଂରଚନା termios *mode);
ସ୍ଥିର ଶୂନ୍ୟ display_speed (ସଂରଚନା termios *mode, bool fancy);
ସ୍ଥିର ଶୂନ୍ୟ display_window_size (bool fancy, ଅକ୍ଷର ଅଚଳ *device_name);
ସ୍ଥିର ଶୂନ୍ୟ sane_mode (ସଂରଚନା termios *mode);
ସ୍ଥିର ଶୂନ୍ୟ set_control_char (ସଂରଚନା control_info ଅଚଳ *info,
                              ଅକ୍ଷର ଅଚଳ *arg,
                              ସଂରଚନା termios *mode);
ସ୍ଥିର ଶୂନ୍ୟ set_speed (ଗଣନା speed_setting type, ଅକ୍ଷର ଅଚଳ *arg,
                       ସଂରଚନା speed_desc ଅଚଳ speed, ସଂରଚନା termios *mode);
ସ୍ଥିର ଶୂନ୍ୟ set_window_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ଅକ୍ଷର ଅଚଳ *device_name);

/* The width of the screen, for output wrapping. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_col;

/* Current position, to know when to wrap. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_col;

/* Default "drain" mode for tcsetattr.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ସ୍ଥିର bool dev_debug;

/* Record last speed set for correlation.  */
ସ୍ଥିର ସଂରଚନା speed_desc last_ibaud =
  { .baud = (speed_t) -1, .valid = false };
ସ୍ଥିର ସଂରଚନା speed_desc last_obaud =
  { .baud = (speed_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"all", no_argument, ଖାଲି, 'a'},
  {"save", no_argument, ଖାଲି, 'g'},
  {"file", required_argument, ଖାଲି, 'F'},
  {"-debug", no_argument, ଖାଲି, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((printf, 1, 2))
ସ୍ଥିର ଶୂନ୍ୟ
wrapf (ଅକ୍ଷର ଅଚଳ *message,...)
{
  va_list args;
  ଅକ୍ଷର *buf;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା buflen;

  va_start (args, message);
  buflen = vasprintf (&buf, message, args);
  va_end (args);

  ଯଦି (buflen < 0)
    xalloc_die ();

  ଯଦି (0 < current_col)
    {
      ଯଦି (max_col - current_col <= buflen)
        {
          putchar ('\n');
          current_col = 0;
        }
      ଅନ୍ୟଥା
        {
          putchar (' ');
          current_col++;
        }
    }

  fputs (buf, stdout);
  ମୁକ୍ତ (buf);
  current_col += buflen;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
  -F, --file=DEVICE  open and use DEVICE instead of standard ନିବେଶ\n\
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
 * dsusp CHAR    CHAR will send a terminal stop ସଙ୍କେତ once ନିବେଶ flushed\n\
"), stdout);
#endif
      fputs (_("\
   eof CHAR      CHAR will send an end of file (terminate the ନିବେଶ)\n\
   eol CHAR      CHAR will end the ଧାଡ଼ି\n\
"), stdout);
#ifdef VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR ପାଇଁ ending the ଧାଡ଼ି\n\
"), stdout);
#endif
      fputs (_("\
   erase CHAR    CHAR will erase the last character typed\n\
   intr CHAR     CHAR will send an interrupt ସଙ୍କେତ\n\
   kill CHAR     CHAR will erase the current ଧାଡ଼ି\n\
"), stdout);
#ifdef VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the next character quoted\n\
"), stdout);
#endif
#ifdef VSTATUS
      fputs (_("\
 * status CHAR   CHAR will send an info ସଙ୍କେତ\n\
"), stdout);
#endif
      fputs (_("\
   quit CHAR     CHAR will send a quit ସଙ୍କେତ\n\
"), stdout);
#ଯଦି defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current ଧାଡ଼ି\n\
"), stdout);
#endif
      fputs (_("\
   start CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop ସଙ୍କେତ\n\
"), stdout);
#ifdef VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will ବାଛ to a different shell layer\n\
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
   N             set the ନିବେଶ and output speeds to N bauds\n\
"), stdout);
#ifdef TIOCGWINSZ
      fputs (_("\
   cols N        tell the kernel that the terminal has N columns\n\
 * columns N     same as cols N\n\
"), stdout);
#endif
      printf (_("\
 * [-]drain      wait ପାଇଁ transmission before applying settings (%s by ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      set the ନିବେଶ speed to N\n\
"), stdout);
#ifdef HAVE_C_ଧାଡ଼ି
      fputs (_("\
 * ଧାଡ଼ି N        use ଧାଡ଼ି discipline N\n\
"), stdout);
#endif
      fputs (_("\
   min N         with -icanon, set N characters minimum ପାଇଁ a completed ପଠ\n\
   ospeed N      set the output speed to N\n\
"), stdout);
#ifdef TIOCGWINSZ
      fputs (_("\
   rows N        tell the kernel that the terminal has N rows\n\
   size          print the ସଂଖ୍ୟା of rows and columns according to the kernel\n\
"), stdout);
#endif
      fputs (_("\
   speed         print the terminal speed\n\
   time N        with -icanon, set ପଠ timeout of N tenths of a second\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control signals\n\
   [-]cread      allow ନିବେଶ to be received\n\
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
   [-]hup        send a hangup ସଙ୍କେତ when the last process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity bit in output and expect parity bit in ନିବେଶ\n\
   [-]parodd     set odd parity (or even parity with '-')\n\
"), stdout);
#ifdef CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#endif
      fputs (_("\
\n\
ନିବେଶ_3 settings:\n\
   [-]brkint     breaks cause an interrupt ସଙ୍କେତ\n\
   [-]icrnl      translate carriage ଫେରନ୍ତୁ to newline\n\
   [-]ignbrk     ignore ଭାଙ୍ଗ characters\n\
   [-]igncr      ignore carriage ଫେରନ୍ତୁ\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#ifdef IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and କର not flush a full ନିବେଶ buffer on a character\n\
"), stdout);
#endif
      fputs (_("\
   [-]inlcr      translate newline to carriage ଫେରନ୍ତୁ\n\
   [-]inpck      enable ନିବେଶ parity checking\n\
   [-]istrip     clear ଉପର (8th) bit of ନିବେଶ characters\n\
"), stdout);
#ifdef IUTF8
      fputs (_("\
 * [-]iutf8      assume ନିବେଶ characters are UTF-8 encoded\n\
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
 * crN           carriage ଫେରନ୍ତୁ delay style, N in [0..3]\n\
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
 * [-]ocrnl      translate carriage ଫେରନ୍ତୁ to newline\n\
"), stdout);
#endif
#ifdef OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters ପାଇଁ fill instead of NUL characters\n\
"), stdout);
#endif
#ifdef OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead of timing ପାଇଁ delays\n\
"), stdout);
#endif
#ifdef OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters to uppercase\n\
"), stdout);
#endif
#ifdef ONLCR
      fputs (_("\
 * [-]onlcr      translate newline to carriage ଫେରନ୍ତୁ-newline\n\
"), stdout);
#endif
#ifdef ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage ଫେରନ୍ତୁ\n\
"), stdout);
#endif
#ifdef ONOCR
      fputs (_("\
 * [-]onocr      କର not print carriage returns in the first column\n\
"), stdout);
#endif
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#ଯଦି defined TABDLY || defined OXTABS
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
 * crtkill       kill all ଧାଡ଼ି by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all ଧାଡ଼ି by obeying the echoctl and echok settings\n\
"), stdout);
#endif
#ifdef ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters in hat notation ('^c')\n\
"), stdout);
#endif
      fputs (_("\
   [-]echo       echo ନିବେଶ characters\n\
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
   [-]echonl     echo newline even ଯଦି not echoing other characters\n\
"), stdout);
#ifdef ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#endif
#ଯଦି defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with ଉପର latency links\n\
"), stdout);
#endif
#ଯଦି defined FLUSHO
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
#ଯଦି defined CREPRINT || defined VREPRINT
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
 * [-]tostop     stop background jobs that try to ଲିଖ to the terminal\n\
"), stdout);
#endif
#ifdef XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' ପାଇଁ uppercase characters\n\
"), stdout);
#endif
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#ଯଦି defined XCASE && defined IUCLC && defined OLCUC
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
                 icanon, eof and eol characters to their ପୂର୍ବନିର୍ଦ୍ଧାରିତ ମୂଲ୍ୟଗୁଡ଼ିକ\n\
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
   ek            erase and kill characters to their ପୂର୍ବନିର୍ଦ୍ଧାରିତ ମୂଲ୍ୟଗୁଡ଼ିକ\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#ଯଦି defined XCASE && defined IUCLC && defined OLCUC
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
                 all special characters to their ପୂର୍ବନିର୍ଦ୍ଧାରିତ ମୂଲ୍ୟଗୁଡ଼ିକ\n\
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
Handle the tty ଧାଡ଼ି connected to standard ନିବେଶ.  Without arguments,\n\
prints baud rate, ଧାଡ଼ି discipline, and deviations from stty sane.  In\n\
settings, CHAR is taken literally, or coded as in ^c, 0x37, 0177 or\n\
127; special ମୂଲ୍ୟଗୁଡ଼ିକ ^- or undef used to disable special characters.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

ସ୍ଥିର ଶୂନ୍ୟ
apply_settings (bool checking, ଅକ୍ଷର ଅଚଳ *device_name,
                ଅକ୍ଷର * ଅଚଳ *settings, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_settings,
                ସଂରଚନା termios *mode, bool *require_set_attr)
{
#define check_argument(arg)						\
  କର									\
    {									\
      ଯଦି (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  ଯେତେବେଳେ (0)

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 1; k < n_settings; k++)
    {
      ଅକ୍ଷର ଅଚଳ *arg = settings[k];
      bool match_found = false;
      bool not_set_attr = false;
      bool reversed = false;

      ଯଦି (! arg)
        ଜାରି;

      ଯଦି (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      ଯଦି (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          ଜାରି;
        }
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; mode_info[i].name != ଖାଲି; ++i)
        {
          ଯଦି (streq (arg, mode_info[i].name))
            {
              ଯଦି ((mode_info[i].flags & NO_SETATTR) == 0)
                {
                  match_found = set_mode (&mode_info[i], reversed, mode);
                  *require_set_attr = true;
                }
              ଅନ୍ୟଥା
                match_found = not_set_attr = true;
              ଭାଙ୍ଗ;
            }
        }
      ଯଦି (!match_found && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      ଯଦି (!match_found)
        {
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; control_info[i].name != ଖାଲି; ++i)
            {
              ଯଦି (streq (arg, control_info[i].name))
                {
                  check_argument (arg);
                  match_found = true;
                  ++k;
                  set_control_char (&control_info[i], settings[k], mode);
                  *require_set_attr = true;
                  ଭାଙ୍ଗ;
                }
            }
        }
      ଯଦି (!match_found || not_set_attr)
        {
          ଯଦି (streq (arg, "ispeed"))
            {
              check_argument (arg);
              ++k;
              ସଂରଚନା speed_desc ଅଚଳ ispeed = string_to_baud (settings[k]);
              ଯଦି (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (ନିବେଶ_speed, settings[k], ispeed, mode);
              ଯଦି (checking)
                ଜାରି;
              *require_set_attr = true;
            }
          ଅନ୍ୟଥା ଯଦି (streq (arg, "ospeed"))
            {
              check_argument (arg);
              ++k;
              ସଂରଚନା speed_desc ଅଚଳ ospeed = string_to_baud (settings[k]);
              ଯଦି (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (output_speed, settings[k], ospeed, mode);
              ଯଦି (checking)
                ଜାରି;
              *require_set_attr = true;
            }
#ifdef TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          ଅନ୍ୟଥା ଯଦି (streq (arg, "extproc"))
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = ! reversed;

              ଯଦି (checking)
                ଜାରି;

              ଯଦି (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#endif
#ifdef TIOCGWINSZ
          ଅନ୍ୟଥା ଯଦି (streq (arg, "rows"))
            {
              check_argument (arg);
              ++k;
              ଯଦି (checking)
                ଜାରି;
              set_window_size (integer_arg (settings[k], INT_MAX), -1,
                               device_name);
            }
          ଅନ୍ୟଥା ଯଦି (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_argument (arg);
              ++k;
              ଯଦି (checking)
                ଜାରି;
              set_window_size (-1, integer_arg (settings[k], INT_MAX),
                               device_name);
            }
          ଅନ୍ୟଥା ଯଦି (streq (arg, "size"))
            {
              ଯଦି (checking)
                ଜାରି;
              max_col = screen_columns ();
              current_col = 0;
              display_window_size (false, device_name);
            }
#endif
#ifdef HAVE_C_ଧାଡ଼ି
          ଅନ୍ୟଥା ଯଦି (streq (arg, "line"))
            {
              check_argument (arg);
              ++k;
              uintmax_t ମୂଲ୍ୟ = integer_arg (settings[k], UINTMAX_MAX);
              ଯଦି (ckd_add (&mode->c_ଧାଡ଼ି, ମୂଲ୍ୟ, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_set_attr = true;
            }
#endif
          ଅନ୍ୟଥା ଯଦି (streq (arg, "speed"))
            {
              ଯଦି (checking)
                ଜାରି;
              max_col = screen_columns ();
              display_speed (mode, false);
            }
          ଅନ୍ୟଥା
            {
              ସଂରଚନା speed_desc ଅଚଳ speed = string_to_baud (arg);
              ଯଦି (speed.valid)
                {
                  set_speed (both_speeds, arg, speed, mode);
                  ଯଦି (checking)
                    ଜାରି;
                  *require_set_attr = true;
                }
              ଅନ୍ୟଥା
                {
                  ଯଦି (! recover_mode (arg, mode))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_set_attr = true;
                }
            }
        }
    }

  ଯଦି (checking)
    check_speed (mode);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ସ୍ଥିର ସଂରଚନା termios mode;

  ଗଣନା output_type output_type;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argi = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା opti = 1;
  bool require_set_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  ଅକ୍ଷର *file_name = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *device_name;

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

  ଯେତେବେଳେ ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, ଖାଲି))
         != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'a':
          verbose_output = true;
          output_type = all;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'g':
          recoverable_output = true;
          output_type = recoverable;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'F':
          ଯଦି (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି DEV_DEBUG_OPTION:
          dev_debug = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          ଯଦି (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          ଭାଙ୍ଗ;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      ଯେତେବେଳେ (opti < optind)
        argv[argi + opti++] = ଖାଲି;
    }

  /* Specifying both -a and -g gets an error.  */
  ଯଦି (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  ଯଦି (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  ଯଦି (!noargs && !verbose_output && !recoverable_output)
    {
      ସ୍ଥିର ସଂରଚନା termios check_mode;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_mode, &require_set_attr);
    }

  ଯଦି (file_name)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdflags;
      ଯଦି (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ଯଦି ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  ଯଦି (tcgetattr (STDIN_FILENO, &mode))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  ଯଦି (verbose_output || recoverable_output || noargs)
    {
      max_col = screen_columns ();
      current_col = 0;
      display_settings (output_type, &mode, device_name);
      ଫେରନ୍ତୁ EXIT_SUCCESS;
    }

  require_set_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &mode, &require_set_attr);

  ଯଦି (require_set_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ସ୍ଥିର ସଂରଚନା termios new_mode;

      ଯଦି (tcsetattr (STDIN_FILENO, tcsetattr_options, &mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      ଯଦି (tcgetattr (STDIN_FILENO, &new_mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ଯଦି (! eq_mode (&mode, &new_mode))
        {
          ଯଦି (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଆକାର (new_mode); i++)
                {
                  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା newc = *(((unsigned ଅକ୍ଷର *) &new_mode) + i);
                  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା oldc = *(((unsigned ଅକ୍ଷର *) &mode) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  ଫେରନ୍ତୁ EXIT_SUCCESS;
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
ସ୍ଥିର bool
eq_mode (ସଂରଚନା termios ଅଚଳ *mode1, ସଂରଚନା termios ଅଚଳ *mode2)
{
  ଯଦି (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#ifdef HAVE_C_ଧାଡ଼ି
      || mode1->c_ଧାଡ଼ି != mode2->c_ଧାଡ଼ି
#endif
      || !memeq (mode1->c_cc, mode2->c_cc, ଆକାର (mode1->c_cc)))
    ଫେରନ୍ତୁ false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_mask = ~(tcflag_t) 0;
#ifdef CBAUD
  cflag_mask &= ~(tcflag_t) CBAUD;
#endif
#ifdef CIBAUD
  cflag_mask &= ~(tcflag_t) CIBAUD;
#endif

  ଯଦି ((mode1->c_cflag ^ mode2->c_cflag) & cflag_mask)
    ଫେରନ୍ତୁ false;

  ଫେରନ୍ତୁ speed_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ସ୍ଥିର bool
set_mode (ସଂରଚନା mode_info ଅଚଳ *info, bool reversed, ସଂରଚନା termios *mode)
{
  tcflag_t *bitsp;

  ଯଦି (reversed && (info->flags & REV) == 0)
    ଫେରନ୍ତୁ false;

  bitsp = mode_type_flag (info->type, mode);

  ଯଦି (bitsp == ଖାଲି)
    {
      /* Combination mode. */
      ଯଦି (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          ଯଦି (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ଅନ୍ୟଥା
            mode->c_cflag = (mode->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "oddp"))
        {
          ଯଦି (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ଅନ୍ୟଥା
            mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "nl"))
        {
          ଯଦି (reversed)
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
          ଅନ୍ୟଥା
            {
              mode->c_iflag = mode->c_iflag & ~ICRNL;
#ifdef ONLCR
              mode->c_oflag = mode->c_oflag & ~ONLCR;
#endif
            }
        }
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "ek"))
        {
          mode->c_cc[VERASE] = CERASE;
          mode->c_cc[VKILL] = CKILL;
        }
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "sane"))
        sane_mode (mode);
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "cbreak"))
        {
          ଯଦି (reversed)
            mode->c_lflag |= ICANON;
          ଅନ୍ୟଥା
            mode->c_lflag &= ~ICANON;
        }
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "pass8"))
        {
          ଯଦି (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
            }
          ଅନ୍ୟଥା
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
            }
        }
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "litout"))
        {
          ଯଦି (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
              mode->c_oflag |= OPOST;
            }
          ଅନ୍ୟଥା
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
              mode->c_oflag &= ~OPOST;
            }
        }
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          ଯଦି ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              mode->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              mode->c_oflag |= OPOST;
              mode->c_lflag |= ISIG | ICANON;
#ଯଦି VMIN == VEOF
              mode->c_cc[VEOF] = CEOF;
#endif
#ଯଦି VTIME == VEOL
              mode->c_cc[VEOL] = CEOL;
#endif
            }
          ଅନ୍ୟଥା
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
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "decctlq"))
        {
          ଯଦି (reversed)
            mode->c_iflag |= IXANY;
          ଅନ୍ୟଥା
            mode->c_iflag &= ~IXANY;
        }
#endif
#ifdef TABDLY
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "tabs"))
        {
          ଯଦି (reversed)
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB3;
          ଅନ୍ୟଥା
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB0;
        }
#ଅନ୍ୟଥା
# ifdef OXTABS
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "tabs"))
        {
          ଯଦି (reversed)
            mode->c_oflag = mode->c_oflag | OXTABS;
          ଅନ୍ୟଥା
            mode->c_oflag = mode->c_oflag & ~OXTABS;
        }
# endif
#endif
#ଯଦି defined XCASE && defined IUCLC && defined OLCUC
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          ଯଦି (reversed)
            {
              mode->c_lflag &= ~XCASE;
              mode->c_iflag &= ~IUCLC;
              mode->c_oflag &= ~OLCUC;
            }
          ଅନ୍ୟଥା
            {
              mode->c_lflag |= XCASE;
              mode->c_iflag |= IUCLC;
              mode->c_oflag |= OLCUC;
            }
        }
#endif
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "crt"))
        mode->c_lflag |= ECHOE
#ifdef ECHOCTL
          | ECHOCTL
#endif
#ifdef ECHOKE
          | ECHOKE
#endif
          ;
      ଅନ୍ୟଥା ଯଦି (streq (info->name, "dec"))
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
  ଅନ୍ୟଥା ଯଦି (reversed)
    *bitsp = *bitsp & ~info->mask & ~info->bits;
  ଅନ୍ୟଥା
    *bitsp = (*bitsp & ~info->mask) | info->bits;

  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ
set_control_char (ସଂରଚନା control_info ଅଚଳ *info, ଅକ୍ଷର ଅଚଳ *arg,
                  ସଂରଚନା termios *mode)
{
  unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;

  ଯଦି (streq (info->name, "min") || streq (info->name, "time"))
    ମୂଲ୍ୟ = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  ଅନ୍ୟଥା ଯଦି (arg[0] == '\0' || arg[1] == '\0')
    ମୂଲ୍ୟ = to_uchar (arg[0]);
  ଅନ୍ୟଥା ଯଦି (streq (arg, "^-") || streq (arg, "undef"))
    ମୂଲ୍ୟ = _POSIX_VDISABLE;
  ଅନ୍ୟଥା ଯଦି (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      ଯଦି (arg[1] == '?')
        ମୂଲ୍ୟ = 127;
      ଅନ୍ୟଥା
        ମୂଲ୍ୟ = to_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  ଅନ୍ୟଥା
    ମୂଲ୍ୟ = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  mode->c_cc[info->offset] = ମୂଲ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ
set_speed (ଗଣନା speed_setting type, ଅକ୍ଷର ଅଚଳ *arg,
           ସଂରଚନା speed_desc ଅଚଳ speed, ସଂରଚନା termios *mode)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (speed.valid);

  ଯଦି (type == ନିବେଶ_speed || type == both_speeds)
    {
      last_ibaud = speed;
      ଯଦି (cfsetispeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  ଯଦି (type == output_speed || type == both_speeds)
    {
      last_obaud = speed;
      ଯଦି (cfsetospeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#ifdef TIOCGWINSZ

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_win_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା winsize *win)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = ioctl (fd, TIOCGWINSZ, (ଅକ୍ଷର *) win);
  ଫେରନ୍ତୁ err;
}

ସ୍ଥିର ଶୂନ୍ୟ
set_window_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ଅକ୍ଷର ଅଚଳ *device_name)
{
  ସଂରଚନା winsize win;

  ଯଦି (get_win_size (STDIN_FILENO, &win))
    {
      ଯଦି (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, ଆକାର (win));
    }

  ଯଦି (rows >= 0)
    win.ws_row = rows;
  ଯଦି (cols >= 0)
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

  ଯଦି (win.ws_row == 0 || win.ws_col == 0)
    {
      ସଂରଚନା ttysize ttysz;

      ttysz.ts_ଧାଡ଼ିଗୁଡ଼ିକ = win.ws_row;
      ttysz.ts_cols = win.ws_col;

      win.ws_row = 1;
      win.ws_col = 1;

      ଯଦି (ioctl (STDIN_FILENO, TIOCSWINSZ, (ଅକ୍ଷର *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ଯଦି (ioctl (STDIN_FILENO, TIOCSSIZE, (ଅକ୍ଷର *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ଫେରନ୍ତୁ;
    }
# endif

  ଯଦି (ioctl (STDIN_FILENO, TIOCSWINSZ, (ଅକ୍ଷର *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

ସ୍ଥିର ଶୂନ୍ୟ
display_window_size (bool fancy, ଅକ୍ଷର ଅଚଳ *device_name)
{
  ସଂରଚନା winsize win;

  ଯଦି (get_win_size (STDIN_FILENO, &win))
    {
      ଯଦି (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ଯଦି (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  ଅନ୍ୟଥା
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_row, win.ws_col);
      ଯଦି (!fancy)
        current_col = 0;
    }
}
#endif

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
screen_columns (ଶୂନ୍ୟ)
{
#ifdef TIOCGWINSZ
  ସଂରଚନା winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  ଯଦି (get_win_size (STDOUT_FILENO, &win) == 0 && 0 < win.ws_col)
    ଫେରନ୍ତୁ win.ws_col;
#endif
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    ଅକ୍ଷର *col_string = getenv ("COLUMNS");
    long ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_columns;
    ଯଦି (!(col_string != ଖାଲି
          && xstrtol (col_string, ଖାଲି, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= INT_MAX))
      n_columns = 80;
    ଫେରନ୍ତୁ n_columns;
  }
}

ATTRIBUTE_PURE
ସ୍ଥିର tcflag_t *
mode_type_flag (ଗଣନା mode_type type, ସଂରଚନା termios *mode)
{
  ବାଛ (type)
    {
    ସ୍ଥିତି control:
      ଫେରନ୍ତୁ &mode->c_cflag;

    ସ୍ଥିତି ନିବେଶ:
      ଫେରନ୍ତୁ &mode->c_iflag;

    ସ୍ଥିତି output:
      ଫେରନ୍ତୁ &mode->c_oflag;

    ସ୍ଥିତି local:
      ଫେରନ୍ତୁ &mode->c_lflag;

    ସ୍ଥିତି combination:
      ଫେରନ୍ତୁ ଖାଲି;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      unreachable ();
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
display_settings (ଗଣନା output_type output_type, ସଂରଚନା termios *mode,
                  ଅକ୍ଷର ଅଚଳ *device_name)
{
  ବାଛ (output_type)
    {
    ସ୍ଥିତି changed:
      display_changed (mode);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି all:
      display_all (mode, device_name);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି recoverable:
      display_recoverable (mode);
      ଭାଙ୍ଗ;
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
display_changed (ସଂରଚନା termios *mode)
{
  bool empty_ଧାଡ଼ି;
  tcflag_t *bitsp;
  unsigned long mask;
  ଗଣନା mode_type prev_type = control;

  display_speed (mode, true);
#ifdef HAVE_C_ଧାଡ଼ି
  wrapf ("line = %d;", mode->c_ଧାଡ଼ି);
#endif
  putchar ('\n');
  current_col = 0;

  empty_ଧାଡ଼ି = true;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      ଯଦି (mode->c_cc[control_info[i].offset] == control_info[i].saneval)
        ଜାରି;

#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ଯଦି (streq (control_info[i].name, "flush"))
        ଜାରି;
#endif
      /* If swtch is the same as susp, don't print both.  */
#ଯଦି VSWTCH == VSUSP
      ଯଦି (streq (control_info[i].name, "swtch"))
        ଜାରି;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#ଯଦି VEOF == VMIN
      ଯଦି ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ଜାରି;
#endif

      empty_ଧାଡ଼ି = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
  ଯଦି ((mode->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode->c_cc[VMIN],
             (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode->c_cc[VTIME]);
    }
  ଅନ୍ୟଥା ଯଦି (!empty_ଧାଡ଼ି)
    putchar ('\n');
  current_col = 0;

  empty_ଧାଡ଼ି = true;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; mode_info[i].name != ଖାଲି; ++i)
    {
      ଯଦି (mode_info[i].flags & OMIT)
        ଜାରି;
      ଯଦି (mode_info[i].type != prev_type)
        {
          ଯଦି (!empty_ଧାଡ଼ି)
            {
              putchar ('\n');
              current_col = 0;
              empty_ଧାଡ଼ି = true;
            }
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      ଯଦି ((*bitsp & mask) == mode_info[i].bits)
        {
          ଯଦି (mode_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", mode_info[i].name);
              empty_ଧାଡ଼ି = false;
            }
        }
      ଅନ୍ୟଥା ଯଦି ((mode_info[i].flags & (SANE_SET | REV)) == (SANE_SET | REV))
        {
          wrapf ("-%s", mode_info[i].name);
          empty_ଧାଡ଼ି = false;
        }
    }
  ଯଦି (!empty_ଧାଡ଼ି)
    putchar ('\n');
  current_col = 0;
}

ସ୍ଥିର ଶୂନ୍ୟ
display_all (ସଂରଚନା termios *mode, ଅକ୍ଷର ଅଚଳ *device_name)
{
  tcflag_t *bitsp;
  unsigned long mask;
  ଗଣନା mode_type prev_type = control;

  display_speed (mode, true);
#ifdef TIOCGWINSZ
  display_window_size (true, device_name);
#endif
#ifdef HAVE_C_ଧାଡ଼ି
  wrapf ("line = %d;", mode->c_ଧାଡ଼ି);
#endif
  putchar ('\n');
  current_col = 0;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ଯଦି (streq (control_info[i].name, "flush"))
        ଜାରି;
#endif
      /* If swtch is the same as susp, don't print both.  */
#ଯଦି VSWTCH == VSUSP
      ଯଦି (streq (control_info[i].name, "swtch"))
        ଜାରି;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#ଯଦି VEOF == VMIN
      ଯଦି ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ଜାରି;
#endif
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
#ଯଦି VEOF == VMIN
  ଯଦି ((mode->c_lflag & ICANON) == 0)
#endif
    wrapf ("min = %lu; time = %lu;",
           (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode->c_cc[VMIN],
           (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode->c_cc[VTIME]);
  ଯଦି (current_col != 0)
    putchar ('\n');
  current_col = 0;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; mode_info[i].name != ଖାଲି; ++i)
    {
      ଯଦି (mode_info[i].flags & OMIT)
        ଜାରି;
      ଯଦି (mode_info[i].type != prev_type)
        {
          putchar ('\n');
          current_col = 0;
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      ଯଦି ((*bitsp & mask) == mode_info[i].bits)
        wrapf ("%s", mode_info[i].name);
      ଅନ୍ୟଥା ଯଦି (mode_info[i].flags & REV)
        wrapf ("-%s", mode_info[i].name);
    }
  putchar ('\n');
  current_col = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ସ୍ଥିର ଶୂନ୍ୟ
check_speed (ସଂରଚନା termios *mode)
{
  ଯଦି (last_ibaud.valid && last_obaud.valid)
    {
      ଯଦି (cfgetispeed (mode) != last_ibaud.baud
          || cfgetospeed (mode) != last_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_to_ମୂଲ୍ୟ (last_ibaud.baud),
               baud_to_ମୂଲ୍ୟ (last_obaud.baud));
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
display_speed (ସଂରଚନା termios *mode, bool fancy)
{
  ଯଦି (cfgetispeed (mode) == 0 || cfgetispeed (mode) == cfgetospeed (mode))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_to_ମୂଲ୍ୟ (cfgetospeed (mode)));
  ଅନ୍ୟଥା
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_to_ମୂଲ୍ୟ (cfgetispeed (mode)),
           baud_to_ମୂଲ୍ୟ (cfgetospeed (mode)));
  ଯଦି (!fancy)
    current_col = 0;
}

ସ୍ଥିର ଶୂନ୍ୟ
display_recoverable (ସଂରଚନା termios *mode)
{
  printf ("%lx:%lx:%lx:%lx",
          (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode->c_iflag,
          (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode->c_oflag,
          (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode->c_cflag,
          (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode->c_lflag);
  ପାଇଁ (size_t i = 0; i < NCCS; ++i)
    printf (":%lx", (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#ifdef CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (mode->c_cflag & CBAUD) == BOTHER;
#  ଯଦି defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((mode->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#endif
  ଯଦି (separate_speeds)
    printf (":%lx:%lx", (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) cfgetispeed (mode),
            (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) cfgetospeed (mode));

  putchar ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#define STRTOUL_TYPE(TYPE)                                               \
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା                                                             \
  strtoul_##TYPE (ଅକ୍ଷର ଅଚଳ *s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା base, ଅକ୍ଷର **p, TYPE *result,       \
                  ଅକ୍ଷର delim)                                            \
  {                                                                      \
    unsigned long ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, base);                                           \
    ଯଦି (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      ଫେରନ୍ତୁ -1;                                                         \
    *result = ul;                                                        \
    ଫେରନ୍ତୁ 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (speed_t)
#undef STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#ଅନ୍ତର୍ଭାଵ "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ସ୍ଥିର bool
recover_mode (ଅକ୍ଷର ଅଚଳ *arg, ସଂରଚନା termios *mode)
{
  tcflag_t flag[4];
  ଅକ୍ଷର ଅଚଳ *s = arg;
  ପାଇଁ (size_t i = 0; i < 4; i++)
    {
      ଅକ୍ଷର *p;
      ଯଦି (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        ଫେରନ୍ତୁ false;
      s = p + 1;
    }
  mode->c_iflag = flag[0];
  mode->c_oflag = flag[1];
  mode->c_cflag = flag[2];
  mode->c_lflag = flag[3];

  ପାଇଁ (size_t i = 0; i < NCCS; ++i)
    {
      ଅକ୍ଷର *p;
      ଅକ୍ଷର delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      ଯଦି (strtoul_cc_t (s, 16, &p, mode->c_cc + i, delim) != 0)
        ଫେରନ୍ତୁ false;
      /* Accept the old format, which has no separate speed fields.  */
      ଯଦି (! *p)
        {
#ifdef CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_SPEED_T_SANE
          ସଂରଚନା speed_desc ospeed = cbaud_to_speed (mode->c_cflag & CBAUD);
          ଯଦି (ospeed.valid && cfsetospeed (mode, ospeed.baud) != 0)
            ଫେରନ୍ତୁ false;
#  ଯଦି defined CIBAUD && defined IBSHIFT
          ସଂରଚନା speed_desc ispeed =
            cbaud_to_speed ((mode->c_cflag & CIBAUD) >> IBSHIFT);
          ଯଦି (ispeed.valid && cfsetispeed (mode, ispeed.baud) != 0)
            ଫେରନ୍ତୁ false;
#  endif
# endif
          last_ibaud.baud = cfgetispeed (mode);
          last_obaud.baud = cfgetospeed (mode);
#endif
          ଫେରନ୍ତୁ true;
        }
      s = p + 1;
    }

  speed_t speed[2];
  ପାଇଁ (size_t i = 0; i < 2; i++)
    {
      ଅକ୍ଷର *p;
      ଅକ୍ଷର delim = i == 0 ? ':' : '\0';
      ଯଦି (strtoul_speed_t (s, 16, &p, speed + i, delim) != 0)
        ଫେରନ୍ତୁ false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  ଯଦି (cfsetospeed (mode, speed[1]) != 0
      || cfsetispeed (mode, speed[0]) != 0)
    ଫେରନ୍ତୁ false;

  last_ibaud.baud = speed[0] == 0 ? cfgetispeed (mode) : speed[0];
  last_obaud.baud = speed[1];
  ଫେରନ୍ତୁ true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ସ୍ଥିର bool
speed_matches (ସଂରଚନା termios ଅଚଳ *mode1, ସଂରଚନା termios ଅଚଳ *mode2)
{
  speed_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  speed_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  ଯଦି (speed1[1] == 0)
    speed1[1] = speed1[0];
  ଯଦି (speed2[1] == 0)
    speed2[1] = speed2[0];

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bits = 2;  /* Start and stop bits.  */
  bits += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  bits += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  ବାଛ (mode1->c_cflag & CSIZE)
    {
      ସ୍ଥିତି CS5: bits += 5; ଭାଙ୍ଗ;
      ସ୍ଥିତି CS6: bits += 6; ଭାଙ୍ଗ;
      ସ୍ଥିତି CS7: bits += 7; ଭାଙ୍ଗ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: bits += 8; ଭାଙ୍ଗ;
    }

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 2; i++)
    {
      unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା value1;
      unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା value2;
      unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta;

      ଯଦି (speed1[i] == speed2[i])
        ଜାରି;

      value1 = baud_to_ମୂଲ୍ୟ (speed1[i]);
      value2 = baud_to_ମୂଲ୍ୟ (speed2[i]);

#ifndef TERMIOS_SPEED_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      ଯଦି (value1 == ULONG_MAX || value2 == ULONG_MAX)
        ଫେରନ୍ତୁ false;
#endif

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      ଯଦି (delta > value1 / (bits * 4))
        ଫେରନ୍ତୁ false;
    }

  ଫେରନ୍ତୁ true;
}

ATTRIBUTE_PURE
ସ୍ଥିର ସଂରଚନା speed_desc
string_to_baud (ଅକ୍ଷର ଅଚଳ *arg)
{
  ଅକ୍ଷର *ep;
  unsigned long ମୂଲ୍ୟ;
  unsigned ଅକ୍ଷର c;

  /* Explicitly disallow negative numbers.  */
  ଯେତେବେଳେ (c_isspace (*arg))
    arg++;
  ଯଦି (*arg == '-')
    ଯାଅ invalid;

  /* Empty does not mean 0.  */
  ଯଦି (! *arg)
    ଯାଅ invalid;

  ମୂଲ୍ୟ = strtoul (arg, &ep, 10);

  c = *ep++;
  ଯଦି (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      ଯଦି (c)
        {
          unsigned ଅକ୍ଷର d = c - '0';
          ଯଦି (d > 5)
            ମୂଲ୍ୟ++;
          ଅନ୍ୟଥା ଯଦି (d == 5)
            {
              ଯେତେବେଳେ ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              ଯଦି (c)
                ମୂଲ୍ୟ++;                /* Nonzero, round up */
              ଅନ୍ୟଥା
                ମୂଲ୍ୟ += (ମୂଲ୍ୟ & 1);   /* Exactly in the middle, round even */
            }

          ଯେତେବେଳେ (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          ଯଦି (c)
            ଯାଅ invalid; /* Garbage after otherwise valid number */
        }
    }
  ଅନ୍ୟଥା ଯଦି (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      ଯଦି (streq (arg, "exta"))
        ଫେରନ୍ତୁ (ସଂରଚନା speed_desc) { .baud = B19200, .valid = true };
      ଅନ୍ୟଥା ଯଦି (streq (arg, "extb"))
        ଫେରନ୍ତୁ (ସଂରଚନା speed_desc) { .baud = B38400, .valid = true };
      ଅନ୍ୟଥା
        ଯାଅ invalid;
    }

  ଫେରନ୍ତୁ ମୂଲ୍ୟ_to_baud (ମୂଲ୍ୟ);

 invalid:
  ଫେରନ୍ତୁ (ସଂରଚନା speed_desc) { .baud = (speed_t) -1, .valid = false };
}

ସ୍ଥିର ଶୂନ୍ୟ
sane_mode (ସଂରଚନା termios *mode)
{
  tcflag_t *bitsp;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; control_info[i].name; ++i)
    {
#ଯଦି VMIN == VEOF
      ଯଦି (streq (control_info[i].name, "min"))
        ଭାଙ୍ଗ;
#endif
      mode->c_cc[control_info[i].offset] = control_info[i].saneval;
    }

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; mode_info[i].name != ଖାଲି; ++i)
    {
      ଯଦି (mode_info[i].flags & NO_SETATTR)
        ଜାରି;

      ଯଦି (mode_info[i].flags & SANE_SET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~mode_info[i].mask) | mode_info[i].bits;
        }
      ଅନ୍ୟଥା ଯଦି (mode_info[i].flags & SANE_UNSET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~mode_info[i].mask & ~mode_info[i].bits;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
visible (cc_t ch)
{
  ସ୍ଥିର ଅକ୍ଷର buf[10];
  ଅକ୍ଷର *bpout = buf;

  ଯଦି (ch == _POSIX_VDISABLE)
    ଫେରନ୍ତୁ "<undef>";

  ଯଦି (ch >= 32)
    {
      ଯଦି (ch < 127)
        *bpout++ = ch;
      ଅନ୍ୟଥା ଯଦି (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      ଅନ୍ୟଥା
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          ଯଦି (ch >= 128 + 32)
            {
              ଯଦି (ch < 128 + 127)
                *bpout++ = ch - 128;
              ଅନ୍ୟଥା
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          ଅନ୍ୟଥା
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  ଅନ୍ୟଥା
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  ଫେରନ୍ତୁ (ଅକ୍ଷର ଅଚଳ *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

ସ୍ଥିର uintmax_t
integer_arg (ଅକ୍ଷର ଅଚଳ *s, uintmax_t maxval)
{
  ଫେରନ୍ତୁ xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
