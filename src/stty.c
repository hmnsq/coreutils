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

#ifdef TERMIOS_NEEDS_XOPEN_SOURCE
# define _XOPEN_SOURCE
#endif

#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن <termios.h>
#𐡄𐡍 HAVE_STROPTS_H
# include <stropts.h>
#endif
#ضمن <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif
#ifdef GWINSZ_IN_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#endif
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
#𐡄𐡍 defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#endif
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#𐡄𐡍 defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#endif
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#𐡄𐡍 defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# 𐡄𐡍 defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#endif
#𐡄𐡍 defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#𐡄𐡍 defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

#𐡄𐡍 defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#endif
#𐡄𐡍 defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#endif
#𐡄𐡍 defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#𐡄𐡍 defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#𐡄𐡍 defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#endif
#𐡄𐡍 defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#endif
#𐡄𐡍 defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#endif
#𐡄𐡍 defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#endif
#𐡄𐡍 defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#endif
#𐡄𐡍 defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#endif
#𐡄𐡍 defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#endif
#𐡄𐡍 defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#endif
#𐡄𐡍 defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#endif

/* Which speeds to set.  */
تعداد speed_setting
  {
    input_speed, output_speed, both_speeds
  };

/* What to output and how.  */
تعداد output_type
  {
    changed, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
تعداد mode_type
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
بنية mode_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    تعداد mode_type type;	/* Which structure element to change. */
    حرف flags;			/* Setting and display options.  */
    unsigned long bits;		/* Bits to set for this mode.  */
    unsigned long mask;		/* Other bits to turn off for this mode.  */
  };

ساكن بنية mode_info ثابت mode_info[] =
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
#وإلا
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
#𐡄𐡍 defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#وإلاإذا defined EXTPROC
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
#𐡄𐡍 defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#endif
#𐡄𐡍 defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#endif
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {عدم, control, 0, 0, 0}
};

/* Control character settings.  */
بنية control_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t offset;		/* Offset in c_cc.  */
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
#وإلا
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
  {عدم, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
بنية speed_desc
{
  /* The baud rate.  */
  speed_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ساكن حرف ثابت *visible (cc_t ch);
ساكن unsigned long صحيح baud_to_value (speed_t speed);
ساكن bool recover_mode (حرف ثابت *arg, بنية termios *mode);
ساكن صحيح screen_columns (فراغ);
ساكن bool set_mode (بنية mode_info ثابت *info, bool reversed,
                      بنية termios *mode);
ساكن bool eq_mode (بنية termios ثابت *mode1,
                     بنية termios ثابت *mode2);
ساكن bool speed_matches (بنية termios ثابت *mode1,
                           بنية termios ثابت *mode2);
ساكن uintmax_t integer_arg (حرف ثابت *s, uintmax_t max);
ساكن بنية speed_desc string_to_baud (حرف ثابت *arg);
ساكن tcflag_t *mode_type_flag (تعداد mode_type type, بنية termios *mode);
ساكن فراغ display_all (بنية termios *mode, حرف ثابت *device_name);
ساكن فراغ display_changed (بنية termios *mode);
ساكن فراغ display_recoverable (بنية termios *mode);
ساكن فراغ display_settings (تعداد output_type output_type,
                              بنية termios *mode,
                              حرف ثابت *device_name);
ساكن فراغ check_speed (بنية termios *mode);
ساكن فراغ display_speed (بنية termios *mode, bool fancy);
ساكن فراغ display_window_size (bool fancy, حرف ثابت *device_name);
ساكن فراغ sane_mode (بنية termios *mode);
ساكن فراغ set_control_char (بنية control_info ثابت *info,
                              حرف ثابت *arg,
                              بنية termios *mode);
ساكن فراغ set_speed (تعداد speed_setting type, حرف ثابت *arg,
                       بنية speed_desc ثابت speed, بنية termios *mode);
ساكن فراغ set_window_size (صحيح rows, صحيح cols, حرف ثابت *device_name);

/* The width of the screen, for output wrapping. */
ساكن صحيح max_col;

/* Current position, to know when to wrap. */
ساكن صحيح current_col;

/* Default "drain" mode for tcsetattr.  */
ساكن صحيح tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ساكن bool dev_debug;

/* Record last speed set for correlation.  */
ساكن بنية speed_desc last_ibaud =
  { .baud = (speed_t) -1, .valid = false };
ساكن بنية speed_desc last_obaud =
  { .baud = (speed_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

ساكن بنية option ثابت longopts[] =
{
  {"all", no_argument, عدم, 'a'},
  {"save", no_argument, عدم, 'g'},
  {"file", required_argument, عدم, 'F'},
  {"-debug", no_argument, عدم, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((printf, 1, 2))
ساكن فراغ
wrapf (حرف ثابت *message,...)
{
  va_list args;
  حرف *buf;
  صحيح buflen;

  va_start (args, message);
  buflen = vasprintf (&buf, message, args);
  va_end (args);

  𐡄𐡍 (buflen < 0)
    xalloc_die ();

  𐡄𐡍 (0 < current_col)
    {
      𐡄𐡍 (max_col - current_col <= buflen)
        {
          putchar ('\n');
          current_col = 0;
        }
      وإلا
        {
          putchar (' ');
          current_col++;
        }
    }

  fputs (buf, stdout);
  حرر (buf);
  current_col += buflen;
}

فراغ
usage (صحيح status)
{
  𐡄𐡍 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
 * dsusp CHAR    CHAR will send a terminal stop إشارة once input flushed\n\
"), stdout);
#endif
      fputs (_("\
   eof CHAR      CHAR will send an end of file (terminate the input)\n\
   eol CHAR      CHAR will end the line\n\
"), stdout);
#ifdef VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR لكل ending the line\n\
"), stdout);
#endif
      fputs (_("\
   erase CHAR    CHAR will erase the last character typed\n\
   intr CHAR     CHAR will send an interrupt إشارة\n\
   kill CHAR     CHAR will erase the current line\n\
"), stdout);
#ifdef VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the next character quoted\n\
"), stdout);
#endif
#ifdef VSTATUS
      fputs (_("\
 * status CHAR   CHAR will send an info إشارة\n\
"), stdout);
#endif
      fputs (_("\
   quit CHAR     CHAR will send a quit إشارة\n\
"), stdout);
#𐡄𐡍 defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current line\n\
"), stdout);
#endif
      fputs (_("\
   start CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop إشارة\n\
"), stdout);
#ifdef VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will بدل to a different shell layer\n\
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
 * [-]drain      wait لكل transmission before applying settings (%s by افتراضي)\
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
   min N         with -icanon, set N characters minimum لكل a completed اقرأ\n\
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
   time N        with -icanon, set اقرأ timeout of N tenths of a second\n\
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
   [-]hup        send a hangup إشارة when the last process closes the tty\n\
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
   [-]brkint     breaks cause an interrupt إشارة\n\
   [-]icrnl      translate carriage أرجع to newline\n\
   [-]ignbrk     ignore اكسر characters\n\
   [-]igncr      ignore carriage أرجع\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#ifdef IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and 𐡏𐡁𐡃 not flush a full input buffer on a character\n\
"), stdout);
#endif
      fputs (_("\
   [-]inlcr      translate newline to carriage أرجع\n\
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
 * crN           carriage أرجع delay style, N in [0..3]\n\
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
 * [-]ocrnl      translate carriage أرجع to newline\n\
"), stdout);
#endif
#ifdef OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters لكل fill instead of NUL characters\n\
"), stdout);
#endif
#ifdef OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead of timing لكل delays\n\
"), stdout);
#endif
#ifdef OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters to uppercase\n\
"), stdout);
#endif
#ifdef ONLCR
      fputs (_("\
 * [-]onlcr      translate newline to carriage أرجع-newline\n\
"), stdout);
#endif
#ifdef ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage أرجع\n\
"), stdout);
#endif
#ifdef ONOCR
      fputs (_("\
 * [-]onocr      𐡏𐡁𐡃 not print carriage returns in the first column\n\
"), stdout);
#endif
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#𐡄𐡍 defined TABDLY || defined OXTABS
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
   [-]echonl     echo newline even 𐡄𐡍 not echoing other characters\n\
"), stdout);
#ifdef ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#endif
#𐡄𐡍 defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with high latency links\n\
"), stdout);
#endif
#𐡄𐡍 defined FLUSHO
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
#𐡄𐡍 defined CREPRINT || defined VREPRINT
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
 * [-]tostop     stop background jobs that try to اكتب to the terminal\n\
"), stdout);
#endif
#ifdef XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' لكل uppercase characters\n\
"), stdout);
#endif
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#𐡄𐡍 defined XCASE && defined IUCLC && defined OLCUC
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
                 icanon, eof and eol characters to their افتراضي values\n\
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
   ek            erase and kill characters to their افتراضي values\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#𐡄𐡍 defined XCASE && defined IUCLC && defined OLCUC
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
                 all special characters to their افتراضي values\n\
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

ساكن فراغ
apply_settings (bool checking, حرف ثابت *device_name,
                حرف * ثابت *settings, صحيح n_settings,
                بنية termios *mode, bool *require_set_attr)
{
#define check_argument(arg)						\
  𐡏𐡁𐡃									\
    {									\
      𐡄𐡍 (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  طالما (0)

  لكل (صحيح k = 1; k < n_settings; k++)
    {
      حرف ثابت *arg = settings[k];
      bool match_found = false;
      bool not_set_attr = false;
      bool reversed = false;

      𐡄𐡍 (! arg)
        استمر;

      𐡄𐡍 (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      𐡄𐡍 (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          استمر;
        }
      لكل (صحيح i = 0; mode_info[i].name != عدم; ++i)
        {
          𐡄𐡍 (streq (arg, mode_info[i].name))
            {
              𐡄𐡍 ((mode_info[i].flags & NO_SETATTR) == 0)
                {
                  match_found = set_mode (&mode_info[i], reversed, mode);
                  *require_set_attr = true;
                }
              وإلا
                match_found = not_set_attr = true;
              اكسر;
            }
        }
      𐡄𐡍 (!match_found && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      𐡄𐡍 (!match_found)
        {
          لكل (صحيح i = 0; control_info[i].name != عدم; ++i)
            {
              𐡄𐡍 (streq (arg, control_info[i].name))
                {
                  check_argument (arg);
                  match_found = true;
                  ++k;
                  set_control_char (&control_info[i], settings[k], mode);
                  *require_set_attr = true;
                  اكسر;
                }
            }
        }
      𐡄𐡍 (!match_found || not_set_attr)
        {
          𐡄𐡍 (streq (arg, "ispeed"))
            {
              check_argument (arg);
              ++k;
              بنية speed_desc ثابت ispeed = string_to_baud (settings[k]);
              𐡄𐡍 (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (input_speed, settings[k], ispeed, mode);
              𐡄𐡍 (checking)
                استمر;
              *require_set_attr = true;
            }
          وإلا 𐡄𐡍 (streq (arg, "ospeed"))
            {
              check_argument (arg);
              ++k;
              بنية speed_desc ثابت ospeed = string_to_baud (settings[k]);
              𐡄𐡍 (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (output_speed, settings[k], ospeed, mode);
              𐡄𐡍 (checking)
                استمر;
              *require_set_attr = true;
            }
#ifdef TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          وإلا 𐡄𐡍 (streq (arg, "extproc"))
            {
              صحيح val = ! reversed;

              𐡄𐡍 (checking)
                استمر;

              𐡄𐡍 (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#endif
#ifdef TIOCGWINSZ
          وإلا 𐡄𐡍 (streq (arg, "rows"))
            {
              check_argument (arg);
              ++k;
              𐡄𐡍 (checking)
                استمر;
              set_window_size (integer_arg (settings[k], INT_MAX), -1,
                               device_name);
            }
          وإلا 𐡄𐡍 (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_argument (arg);
              ++k;
              𐡄𐡍 (checking)
                استمر;
              set_window_size (-1, integer_arg (settings[k], INT_MAX),
                               device_name);
            }
          وإلا 𐡄𐡍 (streq (arg, "size"))
            {
              𐡄𐡍 (checking)
                استمر;
              max_col = screen_columns ();
              current_col = 0;
              display_window_size (false, device_name);
            }
#endif
#ifdef HAVE_C_LINE
          وإلا 𐡄𐡍 (streq (arg, "line"))
            {
              check_argument (arg);
              ++k;
              uintmax_t value = integer_arg (settings[k], UINTMAX_MAX);
              𐡄𐡍 (ckd_add (&mode->c_line, value, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_set_attr = true;
            }
#endif
          وإلا 𐡄𐡍 (streq (arg, "speed"))
            {
              𐡄𐡍 (checking)
                استمر;
              max_col = screen_columns ();
              display_speed (mode, false);
            }
          وإلا
            {
              بنية speed_desc ثابت speed = string_to_baud (arg);
              𐡄𐡍 (speed.valid)
                {
                  set_speed (both_speeds, arg, speed, mode);
                  𐡄𐡍 (checking)
                    استمر;
                  *require_set_attr = true;
                }
              وإلا
                {
                  𐡄𐡍 (! recover_mode (arg, mode))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_set_attr = true;
                }
            }
        }
    }

  𐡄𐡍 (checking)
    check_speed (mode);
}

صحيح
main (صحيح argc, حرف **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ساكن بنية termios mode;

  تعداد output_type output_type;
  صحيح optc;
  صحيح argi = 0;
  صحيح opti = 1;
  bool require_set_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  حرف *file_name = عدم;
  حرف ثابت *device_name;

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
          𐡄𐡍 (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          اكسر;

        حالة DEV_DEBUG_OPTION:
          dev_debug = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          𐡄𐡍 (! streq (argv[argi + opti], "-drain")
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
  𐡄𐡍 (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  𐡄𐡍 (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  𐡄𐡍 (!noargs && !verbose_output && !recoverable_output)
    {
      ساكن بنية termios check_mode;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_mode, &require_set_attr);
    }

  𐡄𐡍 (file_name)
    {
      صحيح fdflags;
      𐡄𐡍 (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      𐡄𐡍 ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  𐡄𐡍 (tcgetattr (STDIN_FILENO, &mode))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  𐡄𐡍 (verbose_output || recoverable_output || noargs)
    {
      max_col = screen_columns ();
      current_col = 0;
      display_settings (output_type, &mode, device_name);
      أرجع EXIT_SUCCESS;
    }

  require_set_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &mode, &require_set_attr);

  𐡄𐡍 (require_set_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ساكن بنية termios new_mode;

      𐡄𐡍 (tcsetattr (STDIN_FILENO, tcsetattr_options, &mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      𐡄𐡍 (tcgetattr (STDIN_FILENO, &new_mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      𐡄𐡍 (! eq_mode (&mode, &new_mode))
        {
          𐡄𐡍 (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              لكل (unsigned صحيح i = 0; i < حجم (new_mode); i++)
                {
                  unsigned صحيح newc = *(((unsigned حرف *) &new_mode) + i);
                  unsigned صحيح oldc = *(((unsigned حرف *) &mode) + i);
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
eq_mode (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  𐡄𐡍 (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#ifdef HAVE_C_LINE
      || mode1->c_line != mode2->c_line
#endif
      || !memeq (mode1->c_cc, mode2->c_cc, حجم (mode1->c_cc)))
    أرجع false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_mask = ~(tcflag_t) 0;
#ifdef CBAUD
  cflag_mask &= ~(tcflag_t) CBAUD;
#endif
#ifdef CIBAUD
  cflag_mask &= ~(tcflag_t) CIBAUD;
#endif

  𐡄𐡍 ((mode1->c_cflag ^ mode2->c_cflag) & cflag_mask)
    أرجع false;

  أرجع speed_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ساكن bool
set_mode (بنية mode_info ثابت *info, bool reversed, بنية termios *mode)
{
  tcflag_t *bitsp;

  𐡄𐡍 (reversed && (info->flags & REV) == 0)
    أرجع false;

  bitsp = mode_type_flag (info->type, mode);

  𐡄𐡍 (bitsp == عدم)
    {
      /* Combination mode. */
      𐡄𐡍 (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          𐡄𐡍 (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            mode->c_cflag = (mode->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      وإلا 𐡄𐡍 (streq (info->name, "oddp"))
        {
          𐡄𐡍 (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      وإلا 𐡄𐡍 (streq (info->name, "nl"))
        {
          𐡄𐡍 (reversed)
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
          وإلا
            {
              mode->c_iflag = mode->c_iflag & ~ICRNL;
#ifdef ONLCR
              mode->c_oflag = mode->c_oflag & ~ONLCR;
#endif
            }
        }
      وإلا 𐡄𐡍 (streq (info->name, "ek"))
        {
          mode->c_cc[VERASE] = CERASE;
          mode->c_cc[VKILL] = CKILL;
        }
      وإلا 𐡄𐡍 (streq (info->name, "sane"))
        sane_mode (mode);
      وإلا 𐡄𐡍 (streq (info->name, "cbreak"))
        {
          𐡄𐡍 (reversed)
            mode->c_lflag |= ICANON;
          وإلا
            mode->c_lflag &= ~ICANON;
        }
      وإلا 𐡄𐡍 (streq (info->name, "pass8"))
        {
          𐡄𐡍 (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
            }
          وإلا
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
            }
        }
      وإلا 𐡄𐡍 (streq (info->name, "litout"))
        {
          𐡄𐡍 (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
              mode->c_oflag |= OPOST;
            }
          وإلا
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
              mode->c_oflag &= ~OPOST;
            }
        }
      وإلا 𐡄𐡍 (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          𐡄𐡍 ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              mode->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              mode->c_oflag |= OPOST;
              mode->c_lflag |= ISIG | ICANON;
#𐡄𐡍 VMIN == VEOF
              mode->c_cc[VEOF] = CEOF;
#endif
#𐡄𐡍 VTIME == VEOL
              mode->c_cc[VEOL] = CEOL;
#endif
            }
          وإلا
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
      وإلا 𐡄𐡍 (streq (info->name, "decctlq"))
        {
          𐡄𐡍 (reversed)
            mode->c_iflag |= IXANY;
          وإلا
            mode->c_iflag &= ~IXANY;
        }
#endif
#ifdef TABDLY
      وإلا 𐡄𐡍 (streq (info->name, "tabs"))
        {
          𐡄𐡍 (reversed)
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB3;
          وإلا
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB0;
        }
#وإلا
# ifdef OXTABS
      وإلا 𐡄𐡍 (streq (info->name, "tabs"))
        {
          𐡄𐡍 (reversed)
            mode->c_oflag = mode->c_oflag | OXTABS;
          وإلا
            mode->c_oflag = mode->c_oflag & ~OXTABS;
        }
# endif
#endif
#𐡄𐡍 defined XCASE && defined IUCLC && defined OLCUC
      وإلا 𐡄𐡍 (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          𐡄𐡍 (reversed)
            {
              mode->c_lflag &= ~XCASE;
              mode->c_iflag &= ~IUCLC;
              mode->c_oflag &= ~OLCUC;
            }
          وإلا
            {
              mode->c_lflag |= XCASE;
              mode->c_iflag |= IUCLC;
              mode->c_oflag |= OLCUC;
            }
        }
#endif
      وإلا 𐡄𐡍 (streq (info->name, "crt"))
        mode->c_lflag |= ECHOE
#ifdef ECHOCTL
          | ECHOCTL
#endif
#ifdef ECHOKE
          | ECHOKE
#endif
          ;
      وإلا 𐡄𐡍 (streq (info->name, "dec"))
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
  وإلا 𐡄𐡍 (reversed)
    *bitsp = *bitsp & ~info->mask & ~info->bits;
  وإلا
    *bitsp = (*bitsp & ~info->mask) | info->bits;

  أرجع true;
}

ساكن فراغ
set_control_char (بنية control_info ثابت *info, حرف ثابت *arg,
                  بنية termios *mode)
{
  unsigned long صحيح value;

  𐡄𐡍 (streq (info->name, "min") || streq (info->name, "time"))
    value = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  وإلا 𐡄𐡍 (arg[0] == '\0' || arg[1] == '\0')
    value = to_uchar (arg[0]);
  وإلا 𐡄𐡍 (streq (arg, "^-") || streq (arg, "undef"))
    value = _POSIX_VDISABLE;
  وإلا 𐡄𐡍 (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      𐡄𐡍 (arg[1] == '?')
        value = 127;
      وإلا
        value = to_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  وإلا
    value = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  mode->c_cc[info->offset] = value;
}

ساكن فراغ
set_speed (تعداد speed_setting type, حرف ثابت *arg,
           بنية speed_desc ثابت speed, بنية termios *mode)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (speed.valid);

  𐡄𐡍 (type == input_speed || type == both_speeds)
    {
      last_ibaud = speed;
      𐡄𐡍 (cfsetispeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  𐡄𐡍 (type == output_speed || type == both_speeds)
    {
      last_obaud = speed;
      𐡄𐡍 (cfsetospeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#ifdef TIOCGWINSZ

ساكن صحيح
get_win_size (صحيح fd, بنية winsize *win)
{
  صحيح err = ioctl (fd, TIOCGWINSZ, (حرف *) win);
  أرجع err;
}

ساكن فراغ
set_window_size (صحيح rows, صحيح cols, حرف ثابت *device_name)
{
  بنية winsize win;

  𐡄𐡍 (get_win_size (STDIN_FILENO, &win))
    {
      𐡄𐡍 (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, حجم (win));
    }

  𐡄𐡍 (rows >= 0)
    win.ws_row = rows;
  𐡄𐡍 (cols >= 0)
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

  𐡄𐡍 (win.ws_row == 0 || win.ws_col == 0)
    {
      بنية ttysize ttysz;

      ttysz.ts_lines = win.ws_row;
      ttysz.ts_cols = win.ws_col;

      win.ws_row = 1;
      win.ws_col = 1;

      𐡄𐡍 (ioctl (STDIN_FILENO, TIOCSWINSZ, (حرف *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      𐡄𐡍 (ioctl (STDIN_FILENO, TIOCSSIZE, (حرف *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      أرجع;
    }
# endif

  𐡄𐡍 (ioctl (STDIN_FILENO, TIOCSWINSZ, (حرف *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

ساكن فراغ
display_window_size (bool fancy, حرف ثابت *device_name)
{
  بنية winsize win;

  𐡄𐡍 (get_win_size (STDIN_FILENO, &win))
    {
      𐡄𐡍 (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      𐡄𐡍 (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  وإلا
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_row, win.ws_col);
      𐡄𐡍 (!fancy)
        current_col = 0;
    }
}
#endif

ساكن صحيح
screen_columns (فراغ)
{
#ifdef TIOCGWINSZ
  بنية winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  𐡄𐡍 (get_win_size (STDOUT_FILENO, &win) == 0 && 0 < win.ws_col)
    أرجع win.ws_col;
#endif
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    حرف *col_string = getenv ("COLUMNS");
    long صحيح n_columns;
    𐡄𐡍 (!(col_string != عدم
          && xstrtol (col_string, عدم, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= INT_MAX))
      n_columns = 80;
    أرجع n_columns;
  }
}

ATTRIBUTE_PURE
ساكن tcflag_t *
mode_type_flag (تعداد mode_type type, بنية termios *mode)
{
  بدل (type)
    {
    حالة control:
      أرجع &mode->c_cflag;

    حالة input:
      أرجع &mode->c_iflag;

    حالة output:
      أرجع &mode->c_oflag;

    حالة local:
      أرجع &mode->c_lflag;

    حالة combination:
      أرجع عدم;

    افتراضي:
      unreachable ();
    }
}

ساكن فراغ
display_settings (تعداد output_type output_type, بنية termios *mode,
                  حرف ثابت *device_name)
{
  بدل (output_type)
    {
    حالة changed:
      display_changed (mode);
      اكسر;

    حالة all:
      display_all (mode, device_name);
      اكسر;

    حالة recoverable:
      display_recoverable (mode);
      اكسر;
    }
}

ساكن فراغ
display_changed (بنية termios *mode)
{
  bool empty_line;
  tcflag_t *bitsp;
  unsigned long mask;
  تعداد mode_type prev_type = control;

  display_speed (mode, true);
#ifdef HAVE_C_LINE
  wrapf ("line = %d;", mode->c_line);
#endif
  putchar ('\n');
  current_col = 0;

  empty_line = true;
  لكل (صحيح i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      𐡄𐡍 (mode->c_cc[control_info[i].offset] == control_info[i].saneval)
        استمر;

#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      𐡄𐡍 (streq (control_info[i].name, "flush"))
        استمر;
#endif
      /* If swtch is the same as susp, don't print both.  */
#𐡄𐡍 VSWTCH == VSUSP
      𐡄𐡍 (streq (control_info[i].name, "swtch"))
        استمر;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#𐡄𐡍 VEOF == VMIN
      𐡄𐡍 ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#endif

      empty_line = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
  𐡄𐡍 ((mode->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (unsigned long صحيح) mode->c_cc[VMIN],
             (unsigned long صحيح) mode->c_cc[VTIME]);
    }
  وإلا 𐡄𐡍 (!empty_line)
    putchar ('\n');
  current_col = 0;

  empty_line = true;
  لكل (صحيح i = 0; mode_info[i].name != عدم; ++i)
    {
      𐡄𐡍 (mode_info[i].flags & OMIT)
        استمر;
      𐡄𐡍 (mode_info[i].type != prev_type)
        {
          𐡄𐡍 (!empty_line)
            {
              putchar ('\n');
              current_col = 0;
              empty_line = true;
            }
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      𐡄𐡍 ((*bitsp & mask) == mode_info[i].bits)
        {
          𐡄𐡍 (mode_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", mode_info[i].name);
              empty_line = false;
            }
        }
      وإلا 𐡄𐡍 ((mode_info[i].flags & (SANE_SET | REV)) == (SANE_SET | REV))
        {
          wrapf ("-%s", mode_info[i].name);
          empty_line = false;
        }
    }
  𐡄𐡍 (!empty_line)
    putchar ('\n');
  current_col = 0;
}

ساكن فراغ
display_all (بنية termios *mode, حرف ثابت *device_name)
{
  tcflag_t *bitsp;
  unsigned long mask;
  تعداد mode_type prev_type = control;

  display_speed (mode, true);
#ifdef TIOCGWINSZ
  display_window_size (true, device_name);
#endif
#ifdef HAVE_C_LINE
  wrapf ("line = %d;", mode->c_line);
#endif
  putchar ('\n');
  current_col = 0;

  لكل (صحيح i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      𐡄𐡍 (streq (control_info[i].name, "flush"))
        استمر;
#endif
      /* If swtch is the same as susp, don't print both.  */
#𐡄𐡍 VSWTCH == VSUSP
      𐡄𐡍 (streq (control_info[i].name, "swtch"))
        استمر;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#𐡄𐡍 VEOF == VMIN
      𐡄𐡍 ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#endif
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
#𐡄𐡍 VEOF == VMIN
  𐡄𐡍 ((mode->c_lflag & ICANON) == 0)
#endif
    wrapf ("min = %lu; time = %lu;",
           (unsigned long صحيح) mode->c_cc[VMIN],
           (unsigned long صحيح) mode->c_cc[VTIME]);
  𐡄𐡍 (current_col != 0)
    putchar ('\n');
  current_col = 0;

  لكل (صحيح i = 0; mode_info[i].name != عدم; ++i)
    {
      𐡄𐡍 (mode_info[i].flags & OMIT)
        استمر;
      𐡄𐡍 (mode_info[i].type != prev_type)
        {
          putchar ('\n');
          current_col = 0;
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      𐡄𐡍 ((*bitsp & mask) == mode_info[i].bits)
        wrapf ("%s", mode_info[i].name);
      وإلا 𐡄𐡍 (mode_info[i].flags & REV)
        wrapf ("-%s", mode_info[i].name);
    }
  putchar ('\n');
  current_col = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ساكن فراغ
check_speed (بنية termios *mode)
{
  𐡄𐡍 (last_ibaud.valid && last_obaud.valid)
    {
      𐡄𐡍 (cfgetispeed (mode) != last_ibaud.baud
          || cfgetospeed (mode) != last_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_to_value (last_ibaud.baud),
               baud_to_value (last_obaud.baud));
    }
}

ساكن فراغ
display_speed (بنية termios *mode, bool fancy)
{
  𐡄𐡍 (cfgetispeed (mode) == 0 || cfgetispeed (mode) == cfgetospeed (mode))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_to_value (cfgetospeed (mode)));
  وإلا
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_to_value (cfgetispeed (mode)),
           baud_to_value (cfgetospeed (mode)));
  𐡄𐡍 (!fancy)
    current_col = 0;
}

ساكن فراغ
display_recoverable (بنية termios *mode)
{
  printf ("%lx:%lx:%lx:%lx",
          (unsigned long صحيح) mode->c_iflag,
          (unsigned long صحيح) mode->c_oflag,
          (unsigned long صحيح) mode->c_cflag,
          (unsigned long صحيح) mode->c_lflag);
  لكل (size_t i = 0; i < NCCS; ++i)
    printf (":%lx", (unsigned long صحيح) mode->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#ifdef CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (mode->c_cflag & CBAUD) == BOTHER;
#  𐡄𐡍 defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((mode->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#endif
  𐡄𐡍 (separate_speeds)
    printf (":%lx:%lx", (unsigned long صحيح) cfgetispeed (mode),
            (unsigned long صحيح) cfgetospeed (mode));

  putchar ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#define STRTOUL_TYPE(TYPE)                                               \
  ساكن صحيح                                                             \
  strtoul_##TYPE (حرف ثابت *s, صحيح base, حرف **p, TYPE *result,       \
                  حرف delim)                                            \
  {                                                                      \
    unsigned long ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, base);                                           \
    𐡄𐡍 (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      أرجع -1;                                                         \
    *result = ul;                                                        \
    أرجع 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (speed_t)
#undef STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#ضمن "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ساكن bool
recover_mode (حرف ثابت *arg, بنية termios *mode)
{
  tcflag_t flag[4];
  حرف ثابت *s = arg;
  لكل (size_t i = 0; i < 4; i++)
    {
      حرف *p;
      𐡄𐡍 (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        أرجع false;
      s = p + 1;
    }
  mode->c_iflag = flag[0];
  mode->c_oflag = flag[1];
  mode->c_cflag = flag[2];
  mode->c_lflag = flag[3];

  لكل (size_t i = 0; i < NCCS; ++i)
    {
      حرف *p;
      حرف delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      𐡄𐡍 (strtoul_cc_t (s, 16, &p, mode->c_cc + i, delim) != 0)
        أرجع false;
      /* Accept the old format, which has no separate speed fields.  */
      𐡄𐡍 (! *p)
        {
#ifdef CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_SPEED_T_SANE
          بنية speed_desc ospeed = cbaud_to_speed (mode->c_cflag & CBAUD);
          𐡄𐡍 (ospeed.valid && cfsetospeed (mode, ospeed.baud) != 0)
            أرجع false;
#  𐡄𐡍 defined CIBAUD && defined IBSHIFT
          بنية speed_desc ispeed =
            cbaud_to_speed ((mode->c_cflag & CIBAUD) >> IBSHIFT);
          𐡄𐡍 (ispeed.valid && cfsetispeed (mode, ispeed.baud) != 0)
            أرجع false;
#  endif
# endif
          last_ibaud.baud = cfgetispeed (mode);
          last_obaud.baud = cfgetospeed (mode);
#endif
          أرجع true;
        }
      s = p + 1;
    }

  speed_t speed[2];
  لكل (size_t i = 0; i < 2; i++)
    {
      حرف *p;
      حرف delim = i == 0 ? ':' : '\0';
      𐡄𐡍 (strtoul_speed_t (s, 16, &p, speed + i, delim) != 0)
        أرجع false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  𐡄𐡍 (cfsetospeed (mode, speed[1]) != 0
      || cfsetispeed (mode, speed[0]) != 0)
    أرجع false;

  last_ibaud.baud = speed[0] == 0 ? cfgetispeed (mode) : speed[0];
  last_obaud.baud = speed[1];
  أرجع true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ساكن bool
speed_matches (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  speed_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  speed_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  𐡄𐡍 (speed1[1] == 0)
    speed1[1] = speed1[0];
  𐡄𐡍 (speed2[1] == 0)
    speed2[1] = speed2[0];

  unsigned صحيح bits = 2;  /* Start and stop bits.  */
  bits += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  bits += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  بدل (mode1->c_cflag & CSIZE)
    {
      حالة CS5: bits += 5; اكسر;
      حالة CS6: bits += 6; اكسر;
      حالة CS7: bits += 7; اكسر;
      افتراضي: bits += 8; اكسر;
    }

  لكل (صحيح i = 0; i < 2; i++)
    {
      unsigned long صحيح value1;
      unsigned long صحيح value2;
      unsigned long صحيح delta;

      𐡄𐡍 (speed1[i] == speed2[i])
        استمر;

      value1 = baud_to_value (speed1[i]);
      value2 = baud_to_value (speed2[i]);

#ifndef TERMIOS_SPEED_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      𐡄𐡍 (value1 == ULONG_MAX || value2 == ULONG_MAX)
        أرجع false;
#endif

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      𐡄𐡍 (delta > value1 / (bits * 4))
        أرجع false;
    }

  أرجع true;
}

ATTRIBUTE_PURE
ساكن بنية speed_desc
string_to_baud (حرف ثابت *arg)
{
  حرف *ep;
  unsigned long value;
  unsigned حرف c;

  /* Explicitly disallow negative numbers.  */
  طالما (c_isspace (*arg))
    arg++;
  𐡄𐡍 (*arg == '-')
    اذهب invalid;

  /* Empty does not mean 0.  */
  𐡄𐡍 (! *arg)
    اذهب invalid;

  value = strtoul (arg, &ep, 10);

  c = *ep++;
  𐡄𐡍 (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      𐡄𐡍 (c)
        {
          unsigned حرف d = c - '0';
          𐡄𐡍 (d > 5)
            value++;
          وإلا 𐡄𐡍 (d == 5)
            {
              طالما ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              𐡄𐡍 (c)
                value++;                /* Nonzero, round up */
              وإلا
                value += (value & 1);   /* Exactly in the middle, round even */
            }

          طالما (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          𐡄𐡍 (c)
            اذهب invalid; /* Garbage after otherwise valid number */
        }
    }
  وإلا 𐡄𐡍 (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      𐡄𐡍 (streq (arg, "exta"))
        أرجع (بنية speed_desc) { .baud = B19200, .valid = true };
      وإلا 𐡄𐡍 (streq (arg, "extb"))
        أرجع (بنية speed_desc) { .baud = B38400, .valid = true };
      وإلا
        اذهب invalid;
    }

  أرجع value_to_baud (value);

 invalid:
  أرجع (بنية speed_desc) { .baud = (speed_t) -1, .valid = false };
}

ساكن فراغ
sane_mode (بنية termios *mode)
{
  tcflag_t *bitsp;

  لكل (صحيح i = 0; control_info[i].name; ++i)
    {
#𐡄𐡍 VMIN == VEOF
      𐡄𐡍 (streq (control_info[i].name, "min"))
        اكسر;
#endif
      mode->c_cc[control_info[i].offset] = control_info[i].saneval;
    }

  لكل (صحيح i = 0; mode_info[i].name != عدم; ++i)
    {
      𐡄𐡍 (mode_info[i].flags & NO_SETATTR)
        استمر;

      𐡄𐡍 (mode_info[i].flags & SANE_SET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~mode_info[i].mask) | mode_info[i].bits;
        }
      وإلا 𐡄𐡍 (mode_info[i].flags & SANE_UNSET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~mode_info[i].mask & ~mode_info[i].bits;
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

  𐡄𐡍 (ch == _POSIX_VDISABLE)
    أرجع "<undef>";

  𐡄𐡍 (ch >= 32)
    {
      𐡄𐡍 (ch < 127)
        *bpout++ = ch;
      وإلا 𐡄𐡍 (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      وإلا
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          𐡄𐡍 (ch >= 128 + 32)
            {
              𐡄𐡍 (ch < 128 + 127)
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
