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

#include <config.h>

#ifdef TERMIOS_NEEDS_XOPEN_SOURCE
# define _XOPEN_SOURCE
#endif

#include <stdio.h>
#include <sys/types.h>

#include <termios.h>
#ᮚᮦᮔ᮪ HAVE_STROPTS_H
# include <stropts.h>
#endif
#include <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif
#ifdef GWINSZ_IN_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#endif
#include <getopt.h>
#include <stdarg.h>

#include "system.h"
#include "assure.h"
#include "c-ctype.h"
#include "fd-reopen.h"
#include "quote.h"
#include "xdectoint.h"
#include "xstrtol.h"

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
#ᮚᮦᮔ᮪ defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#endif
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#ᮚᮦᮔ᮪ defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#endif
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#ᮚᮦᮔ᮪ defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# ᮚᮦᮔ᮪ defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#endif
#ᮚᮦᮔ᮪ defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#ᮚᮦᮔ᮪ defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

#ᮚᮦᮔ᮪ defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#endif
#ᮚᮦᮔ᮪ defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#endif
#ᮚᮦᮔ᮪ defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#ᮚᮦᮔ᮪ defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#ᮚᮦᮔ᮪ defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#endif
#ᮚᮦᮔ᮪ defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#endif
#ᮚᮦᮔ᮪ defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#endif
#ᮚᮦᮔ᮪ defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#endif
#ᮚᮦᮔ᮪ defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#endif
#ᮚᮦᮔ᮪ defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#endif
#ᮚᮦᮔ᮪ defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#endif
#ᮚᮦᮔ᮪ defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#endif
#ᮚᮦᮔ᮪ defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#endif

/* Which speeds to set.  */
ga_nanaa speed_setting
  {
    input_speed, output_speed, both_speeds
  };

/* What to output and how.  */
ga_nanaa output_type
  {
    changed, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
ga_nanaa mode_type
  {
    control, input, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#define SANE_SET 1		/* Set in 'sane' mode. */
#define SANE_UNSET 2		/* Unset in 'sane' mode. */
#define REV 4			/* Can be turned off by prepending '-'. */
#define OMIT 8			/* Don't display value. */
#define NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
sa_nracanaa mode_info
  {
    var_na const *name;		/* Name given on command line.  */
    ga_nanaa mode_type type;	/* Which structure element to change. */
    var_na flags;			/* Setting and display options.  */
    unsigned long bits;		/* Bits to set for this mode.  */
    unsigned long mask;		/* Other bits to turn off for this mode.  */
  };

static sa_nracanaa mode_info const mode_info[] =
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

  {"ignbrk", input, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", input, SANE_SET | REV, BRKINT, 0},
  {"ignpar", input, REV, IGNPAR, 0},
  {"parmrk", input, REV, PARMRK, 0},
  {"inpck", input, REV, INPCK, 0},
  {"istrip", input, REV, ISTRIP, 0},
  {"inlcr", input, SANE_UNSET | REV, INLCR, 0},
  {"igncr", input, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", input, SANE_SET | REV, ICRNL, 0},
  {"ixon", input, REV, IXON, 0},
  {"ixoff", input, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", input, REV | OMIT, IXOFF, 0},
#ifdef IUCLC
  {"iuclc", input, SANE_UNSET | REV, IUCLC, 0},
#endif
#ifdef IXANY
  {"ixany", input, SANE_UNSET | REV, IXANY, 0},
#endif
#ifdef IMAXBEL
  {"imaxbel", input, SANE_SET | REV, IMAXBEL, 0},
#endif
#ifdef IUTF8
  {"iutf8", input, SANE_UNSET | REV, IUTF8, 0},
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
#ᮜᮤᮚ
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
#ᮚᮦᮔ᮪ defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#elif defined EXTPROC
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
#ᮚᮦᮔ᮪ defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#endif
#ᮚᮦᮔ᮪ defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#endif
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {NULL, control, 0, 0, 0}
};

/* Control character settings.  */
sa_nracanaa control_info
  {
    var_na const *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t offset;		/* Offset in c_cc.  */
  };

/* Control characters. */

static sa_nracanaa control_info const control_info[] =
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
#ᮜᮤᮚ
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
  {NULL, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
sa_nracanaa speed_desc
{
  /* The baud rate.  */
  speed_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

static var_na const *visible (cc_t ch);
static unsigned long puur_naa_nka baud_to_value (speed_t speed);
static bool recover_mode (var_na const *arg, sa_nracanaa termios *mode);
static puur_naa_nka screen_columns (void);
static bool set_mode (sa_nracanaa mode_info const *info, bool reversed,
                      sa_nracanaa termios *mode);
static bool eq_mode (sa_nracanaa termios const *mode1,
                     sa_nracanaa termios const *mode2);
static bool speed_matches (sa_nracanaa termios const *mode1,
                           sa_nracanaa termios const *mode2);
static uintmax_t integer_arg (var_na const *s, uintmax_t max);
static sa_nracanaa speed_desc string_to_baud (var_na const *arg);
static tcflag_t *mode_type_flag (ga_nanaa mode_type type, sa_nracanaa termios *mode);
static void display_all (sa_nracanaa termios *mode, var_na const *device_name);
static void display_changed (sa_nracanaa termios *mode);
static void display_recoverable (sa_nracanaa termios *mode);
static void display_settings (ga_nanaa output_type output_type,
                              sa_nracanaa termios *mode,
                              var_na const *device_name);
static void check_speed (sa_nracanaa termios *mode);
static void display_speed (sa_nracanaa termios *mode, bool fancy);
static void display_window_size (bool fancy, var_na const *device_name);
static void sane_mode (sa_nracanaa termios *mode);
static void set_control_char (sa_nracanaa control_info const *info,
                              var_na const *arg,
                              sa_nracanaa termios *mode);
static void set_speed (ga_nanaa speed_setting type, var_na const *arg,
                       sa_nracanaa speed_desc const speed, sa_nracanaa termios *mode);
static void set_window_size (puur_naa_nka rows, puur_naa_nka cols, var_na const *device_name);

/* The width of the screen, for output wrapping. */
static puur_naa_nka max_col;

/* Current position, to know when to wrap. */
static puur_naa_nka current_col;

/* Default "drain" mode for tcsetattr.  */
static puur_naa_nka tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
static bool dev_debug;

/* Record last speed set for correlation.  */
static sa_nracanaa speed_desc last_ibaud =
  { .baud = (speed_t) -1, .valid = false };
static sa_nracanaa speed_desc last_obaud =
  { .baud = (speed_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

static sa_nracanaa option const longopts[] =
{
  {"all", no_argument, NULL, 'a'},
  {"save", no_argument, NULL, 'g'},
  {"file", required_argument, NULL, 'F'},
  {"-debug", no_argument, NULL, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((printf, 1, 2))
static void
wrapf (var_na const *message,...)
{
  va_list args;
  var_na *buf;
  puur_naa_nka buflen;

  va_start (args, message);
  buflen = vasprintf (&buf, message, args);
  va_end (args);

  ᮚᮦᮔ᮪ (buflen < 0)
    xalloc_die ();

  ᮚᮦᮔ᮪ (0 < current_col)
    {
      ᮚᮦᮔ᮪ (max_col - current_col <= buflen)
        {
          putchar ('\n');
          current_col = 0;
        }
      ᮜᮤᮚ
        {
          putchar (' ');
          current_col++;
        }
    }

  fputs (buf, stdout);
  free (buf);
  current_col += buflen;
}

void
usage (puur_naa_nka status)
{
  ᮚᮦᮔ᮪ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᮜᮤᮚ
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
  -F, --file=DEVICE  open and use DEVICE instead of standard input\n\
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
 * dsusp CHAR    CHAR will send a terminal stop sa_nkeeta once input flushed\n\
"), stdout);
#endif
      fputs (_("\
   eof CHAR      CHAR will send an end of file (terminate the input)\n\
   eol CHAR      CHAR will end the line\n\
"), stdout);
#ifdef VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR for ending the line\n\
"), stdout);
#endif
      fputs (_("\
   erase CHAR    CHAR will erase the last character typed\n\
   intr CHAR     CHAR will send an interrupt sa_nkeeta\n\
   kill CHAR     CHAR will erase the current line\n\
"), stdout);
#ifdef VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the next character quoted\n\
"), stdout);
#endif
#ifdef VSTATUS
      fputs (_("\
 * status CHAR   CHAR will send an info sa_nkeeta\n\
"), stdout);
#endif
      fputs (_("\
   quit CHAR     CHAR will send a quit sa_nkeeta\n\
"), stdout);
#ᮚᮦᮔ᮪ defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current line\n\
"), stdout);
#endif
      fputs (_("\
   start CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop sa_nkeeta\n\
"), stdout);
#ifdef VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will switch to a different shell layer\n\
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
   N             set the input and output speeds to N bauds\n\
"), stdout);
#ifdef TIOCGWINSZ
      fputs (_("\
   cols N        tell the kernel that the terminal has N columns\n\
 * columns N     same as cols N\n\
"), stdout);
#endif
      printf (_("\
 * [-]drain      wait for transmission before applying settings (%s by default)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      set the input speed to N\n\
"), stdout);
#ifdef HAVE_C_LINE
      fputs (_("\
 * line N        use line discipline N\n\
"), stdout);
#endif
      fputs (_("\
   min N         with -icanon, set N characters minimum for a completed read\n\
   ospeed N      set the output speed to N\n\
"), stdout);
#ifdef TIOCGWINSZ
      fputs (_("\
   rows N        tell the kernel that the terminal has N rows\n\
   size          print the number of rows and columns according to the kernel\n\
"), stdout);
#endif
      fputs (_("\
   speed         print the terminal speed\n\
   time N        with -icanon, set read timeout of N tenths of a second\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control signals\n\
   [-]cread      allow input to be received\n\
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
   [-]hup        send a hangup sa_nkeeta when the last process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity bit in output and expect parity bit in input\n\
   [-]parodd     set odd parity (or even parity with '-')\n\
"), stdout);
#ifdef CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#endif
      fputs (_("\
\n\
Input settings:\n\
   [-]brkint     breaks cause an interrupt sa_nkeeta\n\
   [-]icrnl      translate carriage ᮝᮜᮤ to newline\n\
   [-]ignbrk     ignore bha_nga characters\n\
   [-]igncr      ignore carriage ᮝᮜᮤ\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#ifdef IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and do not flush a full input buffer on a character\n\
"), stdout);
#endif
      fputs (_("\
   [-]inlcr      translate newline to carriage ᮝᮜᮤ\n\
   [-]inpck      enable input parity checking\n\
   [-]istrip     clear high (8th) bit of input characters\n\
"), stdout);
#ifdef IUTF8
      fputs (_("\
 * [-]iutf8      assume input characters are UTF-8 encoded\n\
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
 * crN           carriage ᮝᮜᮤ delay style, N in [0..3]\n\
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
 * [-]ocrnl      translate carriage ᮝᮜᮤ to newline\n\
"), stdout);
#endif
#ifdef OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters for fill instead of NUL characters\n\
"), stdout);
#endif
#ifdef OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead of timing for delays\n\
"), stdout);
#endif
#ifdef OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters to uppercase\n\
"), stdout);
#endif
#ifdef ONLCR
      fputs (_("\
 * [-]onlcr      translate newline to carriage ᮝᮜᮤ-newline\n\
"), stdout);
#endif
#ifdef ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage ᮝᮜᮤ\n\
"), stdout);
#endif
#ifdef ONOCR
      fputs (_("\
 * [-]onocr      do not print carriage returns in the first column\n\
"), stdout);
#endif
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#ᮚᮦᮔ᮪ defined TABDLY || defined OXTABS
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
 * crtkill       kill all line by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all line by obeying the echoctl and echok settings\n\
"), stdout);
#endif
#ifdef ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters in hat notation ('^c')\n\
"), stdout);
#endif
      fputs (_("\
   [-]echo       echo input characters\n\
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
   [-]echonl     echo newline even ᮚᮦᮔ᮪ not echoing other characters\n\
"), stdout);
#ifdef ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#endif
#ᮚᮦᮔ᮪ defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with high latency links\n\
"), stdout);
#endif
#ᮚᮦᮔ᮪ defined FLUSHO
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
#ᮚᮦᮔ᮪ defined CREPRINT || defined VREPRINT
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
 * [-]tostop     stop background jobs that try to write to the terminal\n\
"), stdout);
#endif
#ifdef XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' for uppercase characters\n\
"), stdout);
#endif
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#ᮚᮦᮔ᮪ defined XCASE && defined IUCLC && defined OLCUC
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
                 icanon, eof and eol characters to their default values\n\
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
   ek            erase and kill characters to their default values\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#ᮚᮦᮔ᮪ defined XCASE && defined IUCLC && defined OLCUC
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
                 all special characters to their default values\n\
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
Handle the tty line connected to standard input.  Without arguments,\n\
prints baud rate, line discipline, and deviations from stty sane.  In\n\
settings, CHAR is taken literally, or coded as in ^c, 0x37, 0177 or\n\
127; special values ^- or undef used to disable special characters.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

static void
apply_settings (bool checking, var_na const *device_name,
                var_na * const *settings, puur_naa_nka n_settings,
                sa_nracanaa termios *mode, bool *require_set_attr)
{
#define check_argument(arg)						\
  do									\
    {									\
      ᮚᮦᮔ᮪ (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  ᮞᮨᮓᮨᮜ (0)

  for (puur_naa_nka k = 1; k < n_settings; k++)
    {
      var_na const *arg = settings[k];
      bool match_found = false;
      bool not_set_attr = false;
      bool reversed = false;

      ᮚᮦᮔ᮪ (! arg)
        continue;

      ᮚᮦᮔ᮪ (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      ᮚᮦᮔ᮪ (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          continue;
        }
      for (puur_naa_nka i = 0; mode_info[i].name != NULL; ++i)
        {
          ᮚᮦᮔ᮪ (streq (arg, mode_info[i].name))
            {
              ᮚᮦᮔ᮪ ((mode_info[i].flags & NO_SETATTR) == 0)
                {
                  match_found = set_mode (&mode_info[i], reversed, mode);
                  *require_set_attr = true;
                }
              ᮜᮤᮚ
                match_found = not_set_attr = true;
              bha_nga;
            }
        }
      ᮚᮦᮔ᮪ (!match_found && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      ᮚᮦᮔ᮪ (!match_found)
        {
          for (puur_naa_nka i = 0; control_info[i].name != NULL; ++i)
            {
              ᮚᮦᮔ᮪ (streq (arg, control_info[i].name))
                {
                  check_argument (arg);
                  match_found = true;
                  ++k;
                  set_control_char (&control_info[i], settings[k], mode);
                  *require_set_attr = true;
                  bha_nga;
                }
            }
        }
      ᮚᮦᮔ᮪ (!match_found || not_set_attr)
        {
          ᮚᮦᮔ᮪ (streq (arg, "ispeed"))
            {
              check_argument (arg);
              ++k;
              sa_nracanaa speed_desc const ispeed = string_to_baud (settings[k]);
              ᮚᮦᮔ᮪ (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (input_speed, settings[k], ispeed, mode);
              ᮚᮦᮔ᮪ (checking)
                continue;
              *require_set_attr = true;
            }
          ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (arg, "ospeed"))
            {
              check_argument (arg);
              ++k;
              sa_nracanaa speed_desc const ospeed = string_to_baud (settings[k]);
              ᮚᮦᮔ᮪ (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (output_speed, settings[k], ospeed, mode);
              ᮚᮦᮔ᮪ (checking)
                continue;
              *require_set_attr = true;
            }
#ifdef TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (arg, "extproc"))
            {
              puur_naa_nka val = ! reversed;

              ᮚᮦᮔ᮪ (checking)
                continue;

              ᮚᮦᮔ᮪ (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#endif
#ifdef TIOCGWINSZ
          ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (arg, "rows"))
            {
              check_argument (arg);
              ++k;
              ᮚᮦᮔ᮪ (checking)
                continue;
              set_window_size (integer_arg (settings[k], INT_MAX), -1,
                               device_name);
            }
          ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_argument (arg);
              ++k;
              ᮚᮦᮔ᮪ (checking)
                continue;
              set_window_size (-1, integer_arg (settings[k], INT_MAX),
                               device_name);
            }
          ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (arg, "size"))
            {
              ᮚᮦᮔ᮪ (checking)
                continue;
              max_col = screen_columns ();
              current_col = 0;
              display_window_size (false, device_name);
            }
#endif
#ifdef HAVE_C_LINE
          ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (arg, "line"))
            {
              check_argument (arg);
              ++k;
              uintmax_t value = integer_arg (settings[k], UINTMAX_MAX);
              ᮚᮦᮔ᮪ (ckd_add (&mode->c_line, value, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_set_attr = true;
            }
#endif
          ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (arg, "speed"))
            {
              ᮚᮦᮔ᮪ (checking)
                continue;
              max_col = screen_columns ();
              display_speed (mode, false);
            }
          ᮜᮤᮚ
            {
              sa_nracanaa speed_desc const speed = string_to_baud (arg);
              ᮚᮦᮔ᮪ (speed.valid)
                {
                  set_speed (both_speeds, arg, speed, mode);
                  ᮚᮦᮔ᮪ (checking)
                    continue;
                  *require_set_attr = true;
                }
              ᮜᮤᮚ
                {
                  ᮚᮦᮔ᮪ (! recover_mode (arg, mode))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_set_attr = true;
                }
            }
        }
    }

  ᮚᮦᮔ᮪ (checking)
    check_speed (mode);
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  static sa_nracanaa termios mode;

  ga_nanaa output_type output_type;
  puur_naa_nka optc;
  puur_naa_nka argi = 0;
  puur_naa_nka opti = 1;
  bool require_set_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  var_na *file_name = NULL;
  var_na const *device_name;

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

  ᮞᮨᮓᮨᮜ ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, NULL))
         != -1)
    {
      switch (optc)
        {
        pak_sa 'a':
          verbose_output = true;
          output_type = all;
          bha_nga;

        pak_sa 'g':
          recoverable_output = true;
          output_type = recoverable;
          bha_nga;

        pak_sa 'F':
          ᮚᮦᮔ᮪ (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          bha_nga;

        pak_sa DEV_DEBUG_OPTION:
          dev_debug = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          ᮚᮦᮔ᮪ (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          bha_nga;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      ᮞᮨᮓᮨᮜ (opti < optind)
        argv[argi + opti++] = NULL;
    }

  /* Specifying both -a and -g gets an error.  */
  ᮚᮦᮔ᮪ (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  ᮚᮦᮔ᮪ (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  ᮚᮦᮔ᮪ (!noargs && !verbose_output && !recoverable_output)
    {
      static sa_nracanaa termios check_mode;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_mode, &require_set_attr);
    }

  ᮚᮦᮔ᮪ (file_name)
    {
      puur_naa_nka fdflags;
      ᮚᮦᮔ᮪ (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ᮚᮦᮔ᮪ ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  ᮚᮦᮔ᮪ (tcgetattr (STDIN_FILENO, &mode))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  ᮚᮦᮔ᮪ (verbose_output || recoverable_output || noargs)
    {
      max_col = screen_columns ();
      current_col = 0;
      display_settings (output_type, &mode, device_name);
      ᮝᮜᮤ EXIT_SUCCESS;
    }

  require_set_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &mode, &require_set_attr);

  ᮚᮦᮔ᮪ (require_set_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      static sa_nracanaa termios new_mode;

      ᮚᮦᮔ᮪ (tcsetattr (STDIN_FILENO, tcsetattr_options, &mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      ᮚᮦᮔ᮪ (tcgetattr (STDIN_FILENO, &new_mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ᮚᮦᮔ᮪ (! eq_mode (&mode, &new_mode))
        {
          ᮚᮦᮔ᮪ (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              for (unsigned puur_naa_nka i = 0; i < parimaa_na (new_mode); i++)
                {
                  unsigned puur_naa_nka newc = *(((unsigned var_na *) &new_mode) + i);
                  unsigned puur_naa_nka oldc = *(((unsigned var_na *) &mode) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  ᮝᮜᮤ EXIT_SUCCESS;
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
static bool
eq_mode (sa_nracanaa termios const *mode1, sa_nracanaa termios const *mode2)
{
  ᮚᮦᮔ᮪ (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#ifdef HAVE_C_LINE
      || mode1->c_line != mode2->c_line
#endif
      || !memeq (mode1->c_cc, mode2->c_cc, parimaa_na (mode1->c_cc)))
    ᮝᮜᮤ false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_mask = ~(tcflag_t) 0;
#ifdef CBAUD
  cflag_mask &= ~(tcflag_t) CBAUD;
#endif
#ifdef CIBAUD
  cflag_mask &= ~(tcflag_t) CIBAUD;
#endif

  ᮚᮦᮔ᮪ ((mode1->c_cflag ^ mode2->c_cflag) & cflag_mask)
    ᮝᮜᮤ false;

  ᮝᮜᮤ speed_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

static bool
set_mode (sa_nracanaa mode_info const *info, bool reversed, sa_nracanaa termios *mode)
{
  tcflag_t *bitsp;

  ᮚᮦᮔ᮪ (reversed && (info->flags & REV) == 0)
    ᮝᮜᮤ false;

  bitsp = mode_type_flag (info->type, mode);

  ᮚᮦᮔ᮪ (bitsp == NULL)
    {
      /* Combination mode. */
      ᮚᮦᮔ᮪ (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          ᮚᮦᮔ᮪ (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ᮜᮤᮚ
            mode->c_cflag = (mode->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "oddp"))
        {
          ᮚᮦᮔ᮪ (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ᮜᮤᮚ
            mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "nl"))
        {
          ᮚᮦᮔ᮪ (reversed)
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
          ᮜᮤᮚ
            {
              mode->c_iflag = mode->c_iflag & ~ICRNL;
#ifdef ONLCR
              mode->c_oflag = mode->c_oflag & ~ONLCR;
#endif
            }
        }
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "ek"))
        {
          mode->c_cc[VERASE] = CERASE;
          mode->c_cc[VKILL] = CKILL;
        }
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "sane"))
        sane_mode (mode);
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "cbreak"))
        {
          ᮚᮦᮔ᮪ (reversed)
            mode->c_lflag |= ICANON;
          ᮜᮤᮚ
            mode->c_lflag &= ~ICANON;
        }
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "pass8"))
        {
          ᮚᮦᮔ᮪ (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
            }
          ᮜᮤᮚ
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
            }
        }
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "litout"))
        {
          ᮚᮦᮔ᮪ (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
              mode->c_oflag |= OPOST;
            }
          ᮜᮤᮚ
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
              mode->c_oflag &= ~OPOST;
            }
        }
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          ᮚᮦᮔ᮪ ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              mode->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              mode->c_oflag |= OPOST;
              mode->c_lflag |= ISIG | ICANON;
#ᮚᮦᮔ᮪ VMIN == VEOF
              mode->c_cc[VEOF] = CEOF;
#endif
#ᮚᮦᮔ᮪ VTIME == VEOL
              mode->c_cc[VEOL] = CEOL;
#endif
            }
          ᮜᮤᮚ
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
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "decctlq"))
        {
          ᮚᮦᮔ᮪ (reversed)
            mode->c_iflag |= IXANY;
          ᮜᮤᮚ
            mode->c_iflag &= ~IXANY;
        }
#endif
#ifdef TABDLY
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "tabs"))
        {
          ᮚᮦᮔ᮪ (reversed)
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB3;
          ᮜᮤᮚ
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB0;
        }
#ᮜᮤᮚ
# ifdef OXTABS
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "tabs"))
        {
          ᮚᮦᮔ᮪ (reversed)
            mode->c_oflag = mode->c_oflag | OXTABS;
          ᮜᮤᮚ
            mode->c_oflag = mode->c_oflag & ~OXTABS;
        }
# endif
#endif
#ᮚᮦᮔ᮪ defined XCASE && defined IUCLC && defined OLCUC
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          ᮚᮦᮔ᮪ (reversed)
            {
              mode->c_lflag &= ~XCASE;
              mode->c_iflag &= ~IUCLC;
              mode->c_oflag &= ~OLCUC;
            }
          ᮜᮤᮚ
            {
              mode->c_lflag |= XCASE;
              mode->c_iflag |= IUCLC;
              mode->c_oflag |= OLCUC;
            }
        }
#endif
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "crt"))
        mode->c_lflag |= ECHOE
#ifdef ECHOCTL
          | ECHOCTL
#endif
#ifdef ECHOKE
          | ECHOKE
#endif
          ;
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (info->name, "dec"))
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
  ᮜᮤᮚ ᮚᮦᮔ᮪ (reversed)
    *bitsp = *bitsp & ~info->mask & ~info->bits;
  ᮜᮤᮚ
    *bitsp = (*bitsp & ~info->mask) | info->bits;

  ᮝᮜᮤ true;
}

static void
set_control_char (sa_nracanaa control_info const *info, var_na const *arg,
                  sa_nracanaa termios *mode)
{
  unsigned long puur_naa_nka value;

  ᮚᮦᮔ᮪ (streq (info->name, "min") || streq (info->name, "time"))
    value = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  ᮜᮤᮚ ᮚᮦᮔ᮪ (arg[0] == '\0' || arg[1] == '\0')
    value = to_uchar (arg[0]);
  ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (arg, "^-") || streq (arg, "undef"))
    value = _POSIX_VDISABLE;
  ᮜᮤᮚ ᮚᮦᮔ᮪ (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      ᮚᮦᮔ᮪ (arg[1] == '?')
        value = 127;
      ᮜᮤᮚ
        value = to_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  ᮜᮤᮚ
    value = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  mode->c_cc[info->offset] = value;
}

static void
set_speed (ga_nanaa speed_setting type, var_na const *arg,
           sa_nracanaa speed_desc const speed, sa_nracanaa termios *mode)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (speed.valid);

  ᮚᮦᮔ᮪ (type == input_speed || type == both_speeds)
    {
      last_ibaud = speed;
      ᮚᮦᮔ᮪ (cfsetispeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  ᮚᮦᮔ᮪ (type == output_speed || type == both_speeds)
    {
      last_obaud = speed;
      ᮚᮦᮔ᮪ (cfsetospeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#ifdef TIOCGWINSZ

static puur_naa_nka
get_win_size (puur_naa_nka fd, sa_nracanaa winsize *win)
{
  puur_naa_nka err = ioctl (fd, TIOCGWINSZ, (var_na *) win);
  ᮝᮜᮤ err;
}

static void
set_window_size (puur_naa_nka rows, puur_naa_nka cols, var_na const *device_name)
{
  sa_nracanaa winsize win;

  ᮚᮦᮔ᮪ (get_win_size (STDIN_FILENO, &win))
    {
      ᮚᮦᮔ᮪ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, parimaa_na (win));
    }

  ᮚᮦᮔ᮪ (rows >= 0)
    win.ws_row = rows;
  ᮚᮦᮔ᮪ (cols >= 0)
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

  ᮚᮦᮔ᮪ (win.ws_row == 0 || win.ws_col == 0)
    {
      sa_nracanaa ttysize ttysz;

      ttysz.ts_lines = win.ws_row;
      ttysz.ts_cols = win.ws_col;

      win.ws_row = 1;
      win.ws_col = 1;

      ᮚᮦᮔ᮪ (ioctl (STDIN_FILENO, TIOCSWINSZ, (var_na *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ᮚᮦᮔ᮪ (ioctl (STDIN_FILENO, TIOCSSIZE, (var_na *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ᮝᮜᮤ;
    }
# endif

  ᮚᮦᮔ᮪ (ioctl (STDIN_FILENO, TIOCSWINSZ, (var_na *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

static void
display_window_size (bool fancy, var_na const *device_name)
{
  sa_nracanaa winsize win;

  ᮚᮦᮔ᮪ (get_win_size (STDIN_FILENO, &win))
    {
      ᮚᮦᮔ᮪ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ᮚᮦᮔ᮪ (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  ᮜᮤᮚ
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_row, win.ws_col);
      ᮚᮦᮔ᮪ (!fancy)
        current_col = 0;
    }
}
#endif

static puur_naa_nka
screen_columns (void)
{
#ifdef TIOCGWINSZ
  sa_nracanaa winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  ᮚᮦᮔ᮪ (get_win_size (STDOUT_FILENO, &win) == 0 && 0 < win.ws_col)
    ᮝᮜᮤ win.ws_col;
#endif
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    var_na *col_string = getenv ("COLUMNS");
    long puur_naa_nka n_columns;
    ᮚᮦᮔ᮪ (!(col_string != NULL
          && xstrtol (col_string, NULL, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= INT_MAX))
      n_columns = 80;
    ᮝᮜᮤ n_columns;
  }
}

ATTRIBUTE_PURE
static tcflag_t *
mode_type_flag (ga_nanaa mode_type type, sa_nracanaa termios *mode)
{
  switch (type)
    {
    pak_sa control:
      ᮝᮜᮤ &mode->c_cflag;

    pak_sa input:
      ᮝᮜᮤ &mode->c_iflag;

    pak_sa output:
      ᮝᮜᮤ &mode->c_oflag;

    pak_sa local:
      ᮝᮜᮤ &mode->c_lflag;

    pak_sa combination:
      ᮝᮜᮤ NULL;

    default:
      unreachable ();
    }
}

static void
display_settings (ga_nanaa output_type output_type, sa_nracanaa termios *mode,
                  var_na const *device_name)
{
  switch (output_type)
    {
    pak_sa changed:
      display_changed (mode);
      bha_nga;

    pak_sa all:
      display_all (mode, device_name);
      bha_nga;

    pak_sa recoverable:
      display_recoverable (mode);
      bha_nga;
    }
}

static void
display_changed (sa_nracanaa termios *mode)
{
  bool empty_line;
  tcflag_t *bitsp;
  unsigned long mask;
  ga_nanaa mode_type prev_type = control;

  display_speed (mode, true);
#ifdef HAVE_C_LINE
  wrapf ("line = %d;", mode->c_line);
#endif
  putchar ('\n');
  current_col = 0;

  empty_line = true;
  for (puur_naa_nka i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      ᮚᮦᮔ᮪ (mode->c_cc[control_info[i].offset] == control_info[i].saneval)
        continue;

#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ᮚᮦᮔ᮪ (streq (control_info[i].name, "flush"))
        continue;
#endif
      /* If swtch is the same as susp, don't print both.  */
#ᮚᮦᮔ᮪ VSWTCH == VSUSP
      ᮚᮦᮔ᮪ (streq (control_info[i].name, "swtch"))
        continue;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#ᮚᮦᮔ᮪ VEOF == VMIN
      ᮚᮦᮔ᮪ ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        continue;
#endif

      empty_line = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
  ᮚᮦᮔ᮪ ((mode->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (unsigned long puur_naa_nka) mode->c_cc[VMIN],
             (unsigned long puur_naa_nka) mode->c_cc[VTIME]);
    }
  ᮜᮤᮚ ᮚᮦᮔ᮪ (!empty_line)
    putchar ('\n');
  current_col = 0;

  empty_line = true;
  for (puur_naa_nka i = 0; mode_info[i].name != NULL; ++i)
    {
      ᮚᮦᮔ᮪ (mode_info[i].flags & OMIT)
        continue;
      ᮚᮦᮔ᮪ (mode_info[i].type != prev_type)
        {
          ᮚᮦᮔ᮪ (!empty_line)
            {
              putchar ('\n');
              current_col = 0;
              empty_line = true;
            }
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      ᮚᮦᮔ᮪ ((*bitsp & mask) == mode_info[i].bits)
        {
          ᮚᮦᮔ᮪ (mode_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", mode_info[i].name);
              empty_line = false;
            }
        }
      ᮜᮤᮚ ᮚᮦᮔ᮪ ((mode_info[i].flags & (SANE_SET | REV)) == (SANE_SET | REV))
        {
          wrapf ("-%s", mode_info[i].name);
          empty_line = false;
        }
    }
  ᮚᮦᮔ᮪ (!empty_line)
    putchar ('\n');
  current_col = 0;
}

static void
display_all (sa_nracanaa termios *mode, var_na const *device_name)
{
  tcflag_t *bitsp;
  unsigned long mask;
  ga_nanaa mode_type prev_type = control;

  display_speed (mode, true);
#ifdef TIOCGWINSZ
  display_window_size (true, device_name);
#endif
#ifdef HAVE_C_LINE
  wrapf ("line = %d;", mode->c_line);
#endif
  putchar ('\n');
  current_col = 0;

  for (puur_naa_nka i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ᮚᮦᮔ᮪ (streq (control_info[i].name, "flush"))
        continue;
#endif
      /* If swtch is the same as susp, don't print both.  */
#ᮚᮦᮔ᮪ VSWTCH == VSUSP
      ᮚᮦᮔ᮪ (streq (control_info[i].name, "swtch"))
        continue;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#ᮚᮦᮔ᮪ VEOF == VMIN
      ᮚᮦᮔ᮪ ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        continue;
#endif
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
#ᮚᮦᮔ᮪ VEOF == VMIN
  ᮚᮦᮔ᮪ ((mode->c_lflag & ICANON) == 0)
#endif
    wrapf ("min = %lu; time = %lu;",
           (unsigned long puur_naa_nka) mode->c_cc[VMIN],
           (unsigned long puur_naa_nka) mode->c_cc[VTIME]);
  ᮚᮦᮔ᮪ (current_col != 0)
    putchar ('\n');
  current_col = 0;

  for (puur_naa_nka i = 0; mode_info[i].name != NULL; ++i)
    {
      ᮚᮦᮔ᮪ (mode_info[i].flags & OMIT)
        continue;
      ᮚᮦᮔ᮪ (mode_info[i].type != prev_type)
        {
          putchar ('\n');
          current_col = 0;
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      ᮚᮦᮔ᮪ ((*bitsp & mask) == mode_info[i].bits)
        wrapf ("%s", mode_info[i].name);
      ᮜᮤᮚ ᮚᮦᮔ᮪ (mode_info[i].flags & REV)
        wrapf ("-%s", mode_info[i].name);
    }
  putchar ('\n');
  current_col = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

static void
check_speed (sa_nracanaa termios *mode)
{
  ᮚᮦᮔ᮪ (last_ibaud.valid && last_obaud.valid)
    {
      ᮚᮦᮔ᮪ (cfgetispeed (mode) != last_ibaud.baud
          || cfgetospeed (mode) != last_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_to_value (last_ibaud.baud),
               baud_to_value (last_obaud.baud));
    }
}

static void
display_speed (sa_nracanaa termios *mode, bool fancy)
{
  ᮚᮦᮔ᮪ (cfgetispeed (mode) == 0 || cfgetispeed (mode) == cfgetospeed (mode))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_to_value (cfgetospeed (mode)));
  ᮜᮤᮚ
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_to_value (cfgetispeed (mode)),
           baud_to_value (cfgetospeed (mode)));
  ᮚᮦᮔ᮪ (!fancy)
    current_col = 0;
}

static void
display_recoverable (sa_nracanaa termios *mode)
{
  printf ("%lx:%lx:%lx:%lx",
          (unsigned long puur_naa_nka) mode->c_iflag,
          (unsigned long puur_naa_nka) mode->c_oflag,
          (unsigned long puur_naa_nka) mode->c_cflag,
          (unsigned long puur_naa_nka) mode->c_lflag);
  for (size_t i = 0; i < NCCS; ++i)
    printf (":%lx", (unsigned long puur_naa_nka) mode->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#ifdef CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (mode->c_cflag & CBAUD) == BOTHER;
#  ᮚᮦᮔ᮪ defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((mode->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#endif
  ᮚᮦᮔ᮪ (separate_speeds)
    printf (":%lx:%lx", (unsigned long puur_naa_nka) cfgetispeed (mode),
            (unsigned long puur_naa_nka) cfgetospeed (mode));

  putchar ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#define STRTOUL_TYPE(TYPE)                                               \
  static puur_naa_nka                                                             \
  strtoul_##TYPE (var_na const *s, puur_naa_nka base, var_na **p, TYPE *result,       \
                  var_na delim)                                            \
  {                                                                      \
    unsigned long ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, base);                                           \
    ᮚᮦᮔ᮪ (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      ᮝᮜᮤ -1;                                                         \
    *result = ul;                                                        \
    ᮝᮜᮤ 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (speed_t)
#undef STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#include "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
static bool
recover_mode (var_na const *arg, sa_nracanaa termios *mode)
{
  tcflag_t flag[4];
  var_na const *s = arg;
  for (size_t i = 0; i < 4; i++)
    {
      var_na *p;
      ᮚᮦᮔ᮪ (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        ᮝᮜᮤ false;
      s = p + 1;
    }
  mode->c_iflag = flag[0];
  mode->c_oflag = flag[1];
  mode->c_cflag = flag[2];
  mode->c_lflag = flag[3];

  for (size_t i = 0; i < NCCS; ++i)
    {
      var_na *p;
      var_na delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      ᮚᮦᮔ᮪ (strtoul_cc_t (s, 16, &p, mode->c_cc + i, delim) != 0)
        ᮝᮜᮤ false;
      /* Accept the old format, which has no separate speed fields.  */
      ᮚᮦᮔ᮪ (! *p)
        {
#ifdef CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_SPEED_T_SANE
          sa_nracanaa speed_desc ospeed = cbaud_to_speed (mode->c_cflag & CBAUD);
          ᮚᮦᮔ᮪ (ospeed.valid && cfsetospeed (mode, ospeed.baud) != 0)
            ᮝᮜᮤ false;
#  ᮚᮦᮔ᮪ defined CIBAUD && defined IBSHIFT
          sa_nracanaa speed_desc ispeed =
            cbaud_to_speed ((mode->c_cflag & CIBAUD) >> IBSHIFT);
          ᮚᮦᮔ᮪ (ispeed.valid && cfsetispeed (mode, ispeed.baud) != 0)
            ᮝᮜᮤ false;
#  endif
# endif
          last_ibaud.baud = cfgetispeed (mode);
          last_obaud.baud = cfgetospeed (mode);
#endif
          ᮝᮜᮤ true;
        }
      s = p + 1;
    }

  speed_t speed[2];
  for (size_t i = 0; i < 2; i++)
    {
      var_na *p;
      var_na delim = i == 0 ? ':' : '\0';
      ᮚᮦᮔ᮪ (strtoul_speed_t (s, 16, &p, speed + i, delim) != 0)
        ᮝᮜᮤ false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  ᮚᮦᮔ᮪ (cfsetospeed (mode, speed[1]) != 0
      || cfsetispeed (mode, speed[0]) != 0)
    ᮝᮜᮤ false;

  last_ibaud.baud = speed[0] == 0 ? cfgetispeed (mode) : speed[0];
  last_obaud.baud = speed[1];
  ᮝᮜᮤ true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
static bool
speed_matches (sa_nracanaa termios const *mode1, sa_nracanaa termios const *mode2)
{
  speed_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  speed_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  ᮚᮦᮔ᮪ (speed1[1] == 0)
    speed1[1] = speed1[0];
  ᮚᮦᮔ᮪ (speed2[1] == 0)
    speed2[1] = speed2[0];

  unsigned puur_naa_nka bits = 2;  /* Start and stop bits.  */
  bits += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  bits += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  switch (mode1->c_cflag & CSIZE)
    {
      pak_sa CS5: bits += 5; bha_nga;
      pak_sa CS6: bits += 6; bha_nga;
      pak_sa CS7: bits += 7; bha_nga;
      default: bits += 8; bha_nga;
    }

  for (puur_naa_nka i = 0; i < 2; i++)
    {
      unsigned long puur_naa_nka value1;
      unsigned long puur_naa_nka value2;
      unsigned long puur_naa_nka delta;

      ᮚᮦᮔ᮪ (speed1[i] == speed2[i])
        continue;

      value1 = baud_to_value (speed1[i]);
      value2 = baud_to_value (speed2[i]);

#ifndef TERMIOS_SPEED_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      ᮚᮦᮔ᮪ (value1 == ULONG_MAX || value2 == ULONG_MAX)
        ᮝᮜᮤ false;
#endif

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      ᮚᮦᮔ᮪ (delta > value1 / (bits * 4))
        ᮝᮜᮤ false;
    }

  ᮝᮜᮤ true;
}

ATTRIBUTE_PURE
static sa_nracanaa speed_desc
string_to_baud (var_na const *arg)
{
  var_na *ep;
  unsigned long value;
  unsigned var_na c;

  /* Explicitly disallow negative numbers.  */
  ᮞᮨᮓᮨᮜ (c_isspace (*arg))
    arg++;
  ᮚᮦᮔ᮪ (*arg == '-')
    goto invalid;

  /* Empty does not mean 0.  */
  ᮚᮦᮔ᮪ (! *arg)
    goto invalid;

  value = strtoul (arg, &ep, 10);

  c = *ep++;
  ᮚᮦᮔ᮪ (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      ᮚᮦᮔ᮪ (c)
        {
          unsigned var_na d = c - '0';
          ᮚᮦᮔ᮪ (d > 5)
            value++;
          ᮜᮤᮚ ᮚᮦᮔ᮪ (d == 5)
            {
              ᮞᮨᮓᮨᮜ ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              ᮚᮦᮔ᮪ (c)
                value++;                /* Nonzero, round up */
              ᮜᮤᮚ
                value += (value & 1);   /* Exactly in the middle, round even */
            }

          ᮞᮨᮓᮨᮜ (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          ᮚᮦᮔ᮪ (c)
            goto invalid; /* Garbage after otherwise valid number */
        }
    }
  ᮜᮤᮚ ᮚᮦᮔ᮪ (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      ᮚᮦᮔ᮪ (streq (arg, "exta"))
        ᮝᮜᮤ (sa_nracanaa speed_desc) { .baud = B19200, .valid = true };
      ᮜᮤᮚ ᮚᮦᮔ᮪ (streq (arg, "extb"))
        ᮝᮜᮤ (sa_nracanaa speed_desc) { .baud = B38400, .valid = true };
      ᮜᮤᮚ
        goto invalid;
    }

  ᮝᮜᮤ value_to_baud (value);

 invalid:
  ᮝᮜᮤ (sa_nracanaa speed_desc) { .baud = (speed_t) -1, .valid = false };
}

static void
sane_mode (sa_nracanaa termios *mode)
{
  tcflag_t *bitsp;

  for (puur_naa_nka i = 0; control_info[i].name; ++i)
    {
#ᮚᮦᮔ᮪ VMIN == VEOF
      ᮚᮦᮔ᮪ (streq (control_info[i].name, "min"))
        bha_nga;
#endif
      mode->c_cc[control_info[i].offset] = control_info[i].saneval;
    }

  for (puur_naa_nka i = 0; mode_info[i].name != NULL; ++i)
    {
      ᮚᮦᮔ᮪ (mode_info[i].flags & NO_SETATTR)
        continue;

      ᮚᮦᮔ᮪ (mode_info[i].flags & SANE_SET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~mode_info[i].mask) | mode_info[i].bits;
        }
      ᮜᮤᮚ ᮚᮦᮔ᮪ (mode_info[i].flags & SANE_UNSET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~mode_info[i].mask & ~mode_info[i].bits;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

static var_na const *
visible (cc_t ch)
{
  static var_na buf[10];
  var_na *bpout = buf;

  ᮚᮦᮔ᮪ (ch == _POSIX_VDISABLE)
    ᮝᮜᮤ "<undef>";

  ᮚᮦᮔ᮪ (ch >= 32)
    {
      ᮚᮦᮔ᮪ (ch < 127)
        *bpout++ = ch;
      ᮜᮤᮚ ᮚᮦᮔ᮪ (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      ᮜᮤᮚ
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          ᮚᮦᮔ᮪ (ch >= 128 + 32)
            {
              ᮚᮦᮔ᮪ (ch < 128 + 127)
                *bpout++ = ch - 128;
              ᮜᮤᮚ
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          ᮜᮤᮚ
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  ᮜᮤᮚ
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  ᮝᮜᮤ (var_na const *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

static uintmax_t
integer_arg (var_na const *s, uintmax_t maxval)
{
  ᮝᮜᮤ xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
