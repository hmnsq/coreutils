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

#અન્તર્ભાવ <config.h>

#ifdef TERMIOS_NEEDS_XOPEN_SOURCE
# define _XOPEN_SOURCE
#endif

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ <termios.h>
#જો HAVE_STROPTS_H
# include <stropts.h>
#endif
#અન્તર્ભાવ <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif
#ifdef GWINSZ_IN_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#endif
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdarg.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "fd-reopen.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "xdectoint.h"
#અન્તર્ભાવ "xstrtol.h"

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
#જો defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#endif
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#જો defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#endif
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#જો defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# જો defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#endif
#જો defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#જો defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

#જો defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#endif
#જો defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#endif
#જો defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#જો defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#જો defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#endif
#જો defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#endif
#જો defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#endif
#જો defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#endif
#જો defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#endif
#જો defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#endif
#જો defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#endif
#જો defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#endif
#જો defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#endif

/* Which speeds to set.  */
ગણના speed_setting
  {
    આગત_speed, output_speed, both_speeds
  };

/* What to output and how.  */
ગણના output_type
  {
    changed, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
ગણના mode_type
  {
    control, આગત, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#define SANE_SET 1		/* Set in 'sane' mode. */
#define SANE_UNSET 2		/* Unset in 'sane' mode. */
#define REV 4			/* Can be turned off by prepending '-'. */
#define OMIT 8			/* Don't display value. */
#define NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
સંરચના mode_info
  {
    અક્ષર અચલ *name;		/* Name given on command line.  */
    ગણના mode_type type;	/* Which structure element to change. */
    અક્ષર flags;			/* Setting and display options.  */
    unsigned long bits;		/* Bits to set for this mode.  */
    unsigned long mask;		/* Other bits to turn off for this mode.  */
  };

સ્થિર સંરચના mode_info અચલ mode_info[] =
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

  {"ignbrk", આગત, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", આગત, SANE_SET | REV, BRKINT, 0},
  {"ignpar", આગત, REV, IGNPAR, 0},
  {"parmrk", આગત, REV, PARMRK, 0},
  {"inpck", આગત, REV, INPCK, 0},
  {"istrip", આગત, REV, ISTRIP, 0},
  {"inlcr", આગત, SANE_UNSET | REV, INLCR, 0},
  {"igncr", આગત, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", આગત, SANE_SET | REV, ICRNL, 0},
  {"ixon", આગત, REV, IXON, 0},
  {"ixoff", આગત, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", આગત, REV | OMIT, IXOFF, 0},
#ifdef IUCLC
  {"iuclc", આગત, SANE_UNSET | REV, IUCLC, 0},
#endif
#ifdef IXANY
  {"ixany", આગત, SANE_UNSET | REV, IXANY, 0},
#endif
#ifdef IMAXBEL
  {"imaxbel", આગત, SANE_SET | REV, IMAXBEL, 0},
#endif
#ifdef IUTF8
  {"iutf8", આગત, SANE_UNSET | REV, IUTF8, 0},
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
#નહીંતર
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
#જો defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#અથવાજો defined EXTPROC
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
#જો defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#endif
#જો defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#endif
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {ખાલી, control, 0, 0, 0}
};

/* Control character settings.  */
સંરચના control_info
  {
    અક્ષર અચલ *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t offset;		/* Offset in c_cc.  */
  };

/* Control characters. */

સ્થિર સંરચના control_info અચલ control_info[] =
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
#નહીંતર
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
  {ખાલી, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
સંરચના speed_desc
{
  /* The baud rate.  */
  speed_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

સ્થિર અક્ષર અચલ *visible (cc_t ch);
સ્થિર unsigned long પૂર્ણાંક baud_to_કિંમત (speed_t speed);
સ્થિર bool recover_mode (અક્ષર અચલ *arg, સંરચના termios *mode);
સ્થિર પૂર્ણાંક screen_columns (શૂન્ય);
સ્થિર bool set_mode (સંરચના mode_info અચલ *info, bool reversed,
                      સંરચના termios *mode);
સ્થિર bool eq_mode (સંરચના termios અચલ *mode1,
                     સંરચના termios અચલ *mode2);
સ્થિર bool speed_matches (સંરચના termios અચલ *mode1,
                           સંરચના termios અચલ *mode2);
સ્થિર uintmax_t integer_arg (અક્ષર અચલ *s, uintmax_t max);
સ્થિર સંરચના speed_desc string_to_baud (અક્ષર અચલ *arg);
સ્થિર tcflag_t *mode_type_flag (ગણના mode_type type, સંરચના termios *mode);
સ્થિર શૂન્ય display_all (સંરચના termios *mode, અક્ષર અચલ *device_name);
સ્થિર શૂન્ય display_changed (સંરચના termios *mode);
સ્થિર શૂન્ય display_recoverable (સંરચના termios *mode);
સ્થિર શૂન્ય display_settings (ગણના output_type output_type,
                              સંરચના termios *mode,
                              અક્ષર અચલ *device_name);
સ્થિર શૂન્ય check_speed (સંરચના termios *mode);
સ્થિર શૂન્ય display_speed (સંરચના termios *mode, bool fancy);
સ્થિર શૂન્ય display_window_size (bool fancy, અક્ષર અચલ *device_name);
સ્થિર શૂન્ય sane_mode (સંરચના termios *mode);
સ્થિર શૂન્ય set_control_char (સંરચના control_info અચલ *info,
                              અક્ષર અચલ *arg,
                              સંરચના termios *mode);
સ્થિર શૂન્ય set_speed (ગણના speed_setting type, અક્ષર અચલ *arg,
                       સંરચના speed_desc અચલ speed, સંરચના termios *mode);
સ્થિર શૂન્ય set_window_size (પૂર્ણાંક rows, પૂર્ણાંક cols, અક્ષર અચલ *device_name);

/* The width of the screen, for output wrapping. */
સ્થિર પૂર્ણાંક max_col;

/* Current position, to know when to wrap. */
સ્થિર પૂર્ણાંક current_col;

/* Default "drain" mode for tcsetattr.  */
સ્થિર પૂર્ણાંક tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
સ્થિર bool dev_debug;

/* Record last speed set for correlation.  */
સ્થિર સંરચના speed_desc last_ibaud =
  { .baud = (speed_t) -1, .valid = false };
સ્થિર સંરચના speed_desc last_obaud =
  { .baud = (speed_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"all", no_argument, ખાલી, 'a'},
  {"save", no_argument, ખાલી, 'g'},
  {"file", required_argument, ખાલી, 'F'},
  {"-debug", no_argument, ખાલી, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((printf, 1, 2))
સ્થિર શૂન્ય
wrapf (અક્ષર અચલ *message,...)
{
  va_list args;
  અક્ષર *buf;
  પૂર્ણાંક buflen;

  va_start (args, message);
  buflen = vasprintf (&buf, message, args);
  va_end (args);

  જો (buflen < 0)
    xalloc_die ();

  જો (0 < current_col)
    {
      જો (max_col - current_col <= buflen)
        {
          putchar ('\n');
          current_col = 0;
        }
      નહીંતર
        {
          putchar (' ');
          current_col++;
        }
    }

  fputs (buf, stdout);
  મુક્ત (buf);
  current_col += buflen;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
  -F, --file=DEVICE  open and use DEVICE instead of standard આગત\n\
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
 * dsusp CHAR    CHAR will send a terminal stop સંકેત once આગત flushed\n\
"), stdout);
#endif
      fputs (_("\
   eof CHAR      CHAR will send an end of file (terminate the આગત)\n\
   eol CHAR      CHAR will end the લીટી\n\
"), stdout);
#ifdef VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR માટે ending the લીટી\n\
"), stdout);
#endif
      fputs (_("\
   erase CHAR    CHAR will erase the last character typed\n\
   intr CHAR     CHAR will send an interrupt સંકેત\n\
   kill CHAR     CHAR will erase the current લીટી\n\
"), stdout);
#ifdef VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the next character quoted\n\
"), stdout);
#endif
#ifdef VSTATUS
      fputs (_("\
 * status CHAR   CHAR will send an info સંકેત\n\
"), stdout);
#endif
      fputs (_("\
   quit CHAR     CHAR will send a quit સંકેત\n\
"), stdout);
#જો defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current લીટી\n\
"), stdout);
#endif
      fputs (_("\
   start CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop સંકેત\n\
"), stdout);
#ifdef VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will પસંદગી to a different shell layer\n\
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
   N             set the આગત and output speeds to N bauds\n\
"), stdout);
#ifdef TIOCGWINSZ
      fputs (_("\
   cols N        tell the kernel that the terminal has N columns\n\
 * columns N     same as cols N\n\
"), stdout);
#endif
      printf (_("\
 * [-]drain      wait માટે transmission before applying settings (%s by મૂળભૂત)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      set the આગત speed to N\n\
"), stdout);
#ifdef HAVE_C_લીટી
      fputs (_("\
 * લીટી N        use લીટી discipline N\n\
"), stdout);
#endif
      fputs (_("\
   min N         with -icanon, set N characters minimum માટે a completed પઠ\n\
   ospeed N      set the output speed to N\n\
"), stdout);
#ifdef TIOCGWINSZ
      fputs (_("\
   rows N        tell the kernel that the terminal has N rows\n\
   size          print the સંખ્યા of rows and columns according to the kernel\n\
"), stdout);
#endif
      fputs (_("\
   speed         print the terminal speed\n\
   time N        with -icanon, set પઠ timeout of N tenths of a second\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control signals\n\
   [-]cread      allow આગત to be received\n\
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
   [-]hup        send a hangup સંકેત when the last process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity bit in output and expect parity bit in આગત\n\
   [-]parodd     set odd parity (or even parity with '-')\n\
"), stdout);
#ifdef CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#endif
      fputs (_("\
\n\
આગત_3 settings:\n\
   [-]brkint     breaks cause an interrupt સંકેત\n\
   [-]icrnl      translate carriage પાછા to newline\n\
   [-]ignbrk     ignore તોડો characters\n\
   [-]igncr      ignore carriage પાછા\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#ifdef IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and કરો not flush a full આગત buffer on a character\n\
"), stdout);
#endif
      fputs (_("\
   [-]inlcr      translate newline to carriage પાછા\n\
   [-]inpck      enable આગત parity checking\n\
   [-]istrip     clear ઉપલું (8th) bit of આગત characters\n\
"), stdout);
#ifdef IUTF8
      fputs (_("\
 * [-]iutf8      assume આગત characters are UTF-8 encoded\n\
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
 * crN           carriage પાછા delay style, N in [0..3]\n\
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
 * [-]ocrnl      translate carriage પાછા to newline\n\
"), stdout);
#endif
#ifdef OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters માટે fill instead of NUL characters\n\
"), stdout);
#endif
#ifdef OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead of timing માટે delays\n\
"), stdout);
#endif
#ifdef OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters to uppercase\n\
"), stdout);
#endif
#ifdef ONLCR
      fputs (_("\
 * [-]onlcr      translate newline to carriage પાછા-newline\n\
"), stdout);
#endif
#ifdef ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage પાછા\n\
"), stdout);
#endif
#ifdef ONOCR
      fputs (_("\
 * [-]onocr      કરો not print carriage returns in the first column\n\
"), stdout);
#endif
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#જો defined TABDLY || defined OXTABS
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
 * crtkill       kill all લીટી by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all લીટી by obeying the echoctl and echok settings\n\
"), stdout);
#endif
#ifdef ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters in hat notation ('^c')\n\
"), stdout);
#endif
      fputs (_("\
   [-]echo       echo આગત characters\n\
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
   [-]echonl     echo newline even જો not echoing other characters\n\
"), stdout);
#ifdef ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#endif
#જો defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with ઉપલું latency links\n\
"), stdout);
#endif
#જો defined FLUSHO
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
#જો defined CREPRINT || defined VREPRINT
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
 * [-]tostop     stop background jobs that try to લિખ to the terminal\n\
"), stdout);
#endif
#ifdef XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' માટે uppercase characters\n\
"), stdout);
#endif
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#જો defined XCASE && defined IUCLC && defined OLCUC
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
                 icanon, eof and eol characters to their મૂળભૂત કિંમતો\n\
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
   ek            erase and kill characters to their મૂળભૂત કિંમતો\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#જો defined XCASE && defined IUCLC && defined OLCUC
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
                 all special characters to their મૂળભૂત કિંમતો\n\
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
Handle the tty લીટી connected to standard આગત.  Without arguments,\n\
prints baud rate, લીટી discipline, and deviations from stty sane.  In\n\
settings, CHAR is taken literally, or coded as in ^c, 0x37, 0177 or\n\
127; special કિંમતો ^- or undef used to disable special characters.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

સ્થિર શૂન્ય
apply_settings (bool checking, અક્ષર અચલ *device_name,
                અક્ષર * અચલ *settings, પૂર્ણાંક n_settings,
                સંરચના termios *mode, bool *require_set_attr)
{
#define check_argument(arg)						\
  કરો									\
    {									\
      જો (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  જ્યારે (0)

  માટે (પૂર્ણાંક k = 1; k < n_settings; k++)
    {
      અક્ષર અચલ *arg = settings[k];
      bool match_found = false;
      bool not_set_attr = false;
      bool reversed = false;

      જો (! arg)
        ચાલુ;

      જો (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      જો (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          ચાલુ;
        }
      માટે (પૂર્ણાંક i = 0; mode_info[i].name != ખાલી; ++i)
        {
          જો (streq (arg, mode_info[i].name))
            {
              જો ((mode_info[i].flags & NO_SETATTR) == 0)
                {
                  match_found = set_mode (&mode_info[i], reversed, mode);
                  *require_set_attr = true;
                }
              નહીંતર
                match_found = not_set_attr = true;
              તોડો;
            }
        }
      જો (!match_found && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      જો (!match_found)
        {
          માટે (પૂર્ણાંક i = 0; control_info[i].name != ખાલી; ++i)
            {
              જો (streq (arg, control_info[i].name))
                {
                  check_argument (arg);
                  match_found = true;
                  ++k;
                  set_control_char (&control_info[i], settings[k], mode);
                  *require_set_attr = true;
                  તોડો;
                }
            }
        }
      જો (!match_found || not_set_attr)
        {
          જો (streq (arg, "ispeed"))
            {
              check_argument (arg);
              ++k;
              સંરચના speed_desc અચલ ispeed = string_to_baud (settings[k]);
              જો (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (આગત_speed, settings[k], ispeed, mode);
              જો (checking)
                ચાલુ;
              *require_set_attr = true;
            }
          નહીંતર જો (streq (arg, "ospeed"))
            {
              check_argument (arg);
              ++k;
              સંરચના speed_desc અચલ ospeed = string_to_baud (settings[k]);
              જો (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              set_speed (output_speed, settings[k], ospeed, mode);
              જો (checking)
                ચાલુ;
              *require_set_attr = true;
            }
#ifdef TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          નહીંતર જો (streq (arg, "extproc"))
            {
              પૂર્ણાંક val = ! reversed;

              જો (checking)
                ચાલુ;

              જો (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#endif
#ifdef TIOCGWINSZ
          નહીંતર જો (streq (arg, "rows"))
            {
              check_argument (arg);
              ++k;
              જો (checking)
                ચાલુ;
              set_window_size (integer_arg (settings[k], INT_MAX), -1,
                               device_name);
            }
          નહીંતર જો (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_argument (arg);
              ++k;
              જો (checking)
                ચાલુ;
              set_window_size (-1, integer_arg (settings[k], INT_MAX),
                               device_name);
            }
          નહીંતર જો (streq (arg, "size"))
            {
              જો (checking)
                ચાલુ;
              max_col = screen_columns ();
              current_col = 0;
              display_window_size (false, device_name);
            }
#endif
#ifdef HAVE_C_લીટી
          નહીંતર જો (streq (arg, "line"))
            {
              check_argument (arg);
              ++k;
              uintmax_t કિંમત = integer_arg (settings[k], UINTMAX_MAX);
              જો (ckd_add (&mode->c_લીટી, કિંમત, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_set_attr = true;
            }
#endif
          નહીંતર જો (streq (arg, "speed"))
            {
              જો (checking)
                ચાલુ;
              max_col = screen_columns ();
              display_speed (mode, false);
            }
          નહીંતર
            {
              સંરચના speed_desc અચલ speed = string_to_baud (arg);
              જો (speed.valid)
                {
                  set_speed (both_speeds, arg, speed, mode);
                  જો (checking)
                    ચાલુ;
                  *require_set_attr = true;
                }
              નહીંતર
                {
                  જો (! recover_mode (arg, mode))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_set_attr = true;
                }
            }
        }
    }

  જો (checking)
    check_speed (mode);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  સ્થિર સંરચના termios mode;

  ગણના output_type output_type;
  પૂર્ણાંક optc;
  પૂર્ણાંક argi = 0;
  પૂર્ણાંક opti = 1;
  bool require_set_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  અક્ષર *file_name = ખાલી;
  અક્ષર અચલ *device_name;

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

  જ્યારે ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, ખાલી))
         != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'a':
          verbose_output = true;
          output_type = all;
          તોડો;

        સ્થિતિ 'g':
          recoverable_output = true;
          output_type = recoverable;
          તોડો;

        સ્થિતિ 'F':
          જો (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          તોડો;

        સ્થિતિ DEV_DEBUG_OPTION:
          dev_debug = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          જો (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          તોડો;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      જ્યારે (opti < optind)
        argv[argi + opti++] = ખાલી;
    }

  /* Specifying both -a and -g gets an error.  */
  જો (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  જો (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  જો (!noargs && !verbose_output && !recoverable_output)
    {
      સ્થિર સંરચના termios check_mode;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_mode, &require_set_attr);
    }

  જો (file_name)
    {
      પૂર્ણાંક fdflags;
      જો (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      જો ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  જો (tcgetattr (STDIN_FILENO, &mode))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  જો (verbose_output || recoverable_output || noargs)
    {
      max_col = screen_columns ();
      current_col = 0;
      display_settings (output_type, &mode, device_name);
      પાછા EXIT_SUCCESS;
    }

  require_set_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &mode, &require_set_attr);

  જો (require_set_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      સ્થિર સંરચના termios new_mode;

      જો (tcsetattr (STDIN_FILENO, tcsetattr_options, &mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      જો (tcgetattr (STDIN_FILENO, &new_mode))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      જો (! eq_mode (&mode, &new_mode))
        {
          જો (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              માટે (unsigned પૂર્ણાંક i = 0; i < કદ (new_mode); i++)
                {
                  unsigned પૂર્ણાંક newc = *(((unsigned અક્ષર *) &new_mode) + i);
                  unsigned પૂર્ણાંક oldc = *(((unsigned અક્ષર *) &mode) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  પાછા EXIT_SUCCESS;
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
સ્થિર bool
eq_mode (સંરચના termios અચલ *mode1, સંરચના termios અચલ *mode2)
{
  જો (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#ifdef HAVE_C_લીટી
      || mode1->c_લીટી != mode2->c_લીટી
#endif
      || !memeq (mode1->c_cc, mode2->c_cc, કદ (mode1->c_cc)))
    પાછા false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_mask = ~(tcflag_t) 0;
#ifdef CBAUD
  cflag_mask &= ~(tcflag_t) CBAUD;
#endif
#ifdef CIBAUD
  cflag_mask &= ~(tcflag_t) CIBAUD;
#endif

  જો ((mode1->c_cflag ^ mode2->c_cflag) & cflag_mask)
    પાછા false;

  પાછા speed_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

સ્થિર bool
set_mode (સંરચના mode_info અચલ *info, bool reversed, સંરચના termios *mode)
{
  tcflag_t *bitsp;

  જો (reversed && (info->flags & REV) == 0)
    પાછા false;

  bitsp = mode_type_flag (info->type, mode);

  જો (bitsp == ખાલી)
    {
      /* Combination mode. */
      જો (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          જો (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          નહીંતર
            mode->c_cflag = (mode->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      નહીંતર જો (streq (info->name, "oddp"))
        {
          જો (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          નહીંતર
            mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      નહીંતર જો (streq (info->name, "nl"))
        {
          જો (reversed)
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
          નહીંતર
            {
              mode->c_iflag = mode->c_iflag & ~ICRNL;
#ifdef ONLCR
              mode->c_oflag = mode->c_oflag & ~ONLCR;
#endif
            }
        }
      નહીંતર જો (streq (info->name, "ek"))
        {
          mode->c_cc[VERASE] = CERASE;
          mode->c_cc[VKILL] = CKILL;
        }
      નહીંતર જો (streq (info->name, "sane"))
        sane_mode (mode);
      નહીંતર જો (streq (info->name, "cbreak"))
        {
          જો (reversed)
            mode->c_lflag |= ICANON;
          નહીંતર
            mode->c_lflag &= ~ICANON;
        }
      નહીંતર જો (streq (info->name, "pass8"))
        {
          જો (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
            }
          નહીંતર
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
            }
        }
      નહીંતર જો (streq (info->name, "litout"))
        {
          જો (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
              mode->c_oflag |= OPOST;
            }
          નહીંતર
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
              mode->c_oflag &= ~OPOST;
            }
        }
      નહીંતર જો (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          જો ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              mode->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              mode->c_oflag |= OPOST;
              mode->c_lflag |= ISIG | ICANON;
#જો VMIN == VEOF
              mode->c_cc[VEOF] = CEOF;
#endif
#જો VTIME == VEOL
              mode->c_cc[VEOL] = CEOL;
#endif
            }
          નહીંતર
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
      નહીંતર જો (streq (info->name, "decctlq"))
        {
          જો (reversed)
            mode->c_iflag |= IXANY;
          નહીંતર
            mode->c_iflag &= ~IXANY;
        }
#endif
#ifdef TABDLY
      નહીંતર જો (streq (info->name, "tabs"))
        {
          જો (reversed)
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB3;
          નહીંતર
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB0;
        }
#નહીંતર
# ifdef OXTABS
      નહીંતર જો (streq (info->name, "tabs"))
        {
          જો (reversed)
            mode->c_oflag = mode->c_oflag | OXTABS;
          નહીંતર
            mode->c_oflag = mode->c_oflag & ~OXTABS;
        }
# endif
#endif
#જો defined XCASE && defined IUCLC && defined OLCUC
      નહીંતર જો (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          જો (reversed)
            {
              mode->c_lflag &= ~XCASE;
              mode->c_iflag &= ~IUCLC;
              mode->c_oflag &= ~OLCUC;
            }
          નહીંતર
            {
              mode->c_lflag |= XCASE;
              mode->c_iflag |= IUCLC;
              mode->c_oflag |= OLCUC;
            }
        }
#endif
      નહીંતર જો (streq (info->name, "crt"))
        mode->c_lflag |= ECHOE
#ifdef ECHOCTL
          | ECHOCTL
#endif
#ifdef ECHOKE
          | ECHOKE
#endif
          ;
      નહીંતર જો (streq (info->name, "dec"))
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
  નહીંતર જો (reversed)
    *bitsp = *bitsp & ~info->mask & ~info->bits;
  નહીંતર
    *bitsp = (*bitsp & ~info->mask) | info->bits;

  પાછા true;
}

સ્થિર શૂન્ય
set_control_char (સંરચના control_info અચલ *info, અક્ષર અચલ *arg,
                  સંરચના termios *mode)
{
  unsigned long પૂર્ણાંક કિંમત;

  જો (streq (info->name, "min") || streq (info->name, "time"))
    કિંમત = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  નહીંતર જો (arg[0] == '\0' || arg[1] == '\0')
    કિંમત = to_uchar (arg[0]);
  નહીંતર જો (streq (arg, "^-") || streq (arg, "undef"))
    કિંમત = _POSIX_VDISABLE;
  નહીંતર જો (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      જો (arg[1] == '?')
        કિંમત = 127;
      નહીંતર
        કિંમત = to_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  નહીંતર
    કિંમત = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  mode->c_cc[info->offset] = કિંમત;
}

સ્થિર શૂન્ય
set_speed (ગણના speed_setting type, અક્ષર અચલ *arg,
           સંરચના speed_desc અચલ speed, સંરચના termios *mode)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (speed.valid);

  જો (type == આગત_speed || type == both_speeds)
    {
      last_ibaud = speed;
      જો (cfsetispeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  જો (type == output_speed || type == both_speeds)
    {
      last_obaud = speed;
      જો (cfsetospeed (mode, speed.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#ifdef TIOCGWINSZ

સ્થિર પૂર્ણાંક
get_win_size (પૂર્ણાંક fd, સંરચના winsize *win)
{
  પૂર્ણાંક err = ioctl (fd, TIOCGWINSZ, (અક્ષર *) win);
  પાછા err;
}

સ્થિર શૂન્ય
set_window_size (પૂર્ણાંક rows, પૂર્ણાંક cols, અક્ષર અચલ *device_name)
{
  સંરચના winsize win;

  જો (get_win_size (STDIN_FILENO, &win))
    {
      જો (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, કદ (win));
    }

  જો (rows >= 0)
    win.ws_row = rows;
  જો (cols >= 0)
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

  જો (win.ws_row == 0 || win.ws_col == 0)
    {
      સંરચના ttysize ttysz;

      ttysz.ts_લીટીઓ = win.ws_row;
      ttysz.ts_cols = win.ws_col;

      win.ws_row = 1;
      win.ws_col = 1;

      જો (ioctl (STDIN_FILENO, TIOCSWINSZ, (અક્ષર *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      જો (ioctl (STDIN_FILENO, TIOCSSIZE, (અક્ષર *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      પાછા;
    }
# endif

  જો (ioctl (STDIN_FILENO, TIOCSWINSZ, (અક્ષર *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

સ્થિર શૂન્ય
display_window_size (bool fancy, અક્ષર અચલ *device_name)
{
  સંરચના winsize win;

  જો (get_win_size (STDIN_FILENO, &win))
    {
      જો (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      જો (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  નહીંતર
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_row, win.ws_col);
      જો (!fancy)
        current_col = 0;
    }
}
#endif

સ્થિર પૂર્ણાંક
screen_columns (શૂન્ય)
{
#ifdef TIOCGWINSZ
  સંરચના winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  જો (get_win_size (STDOUT_FILENO, &win) == 0 && 0 < win.ws_col)
    પાછા win.ws_col;
#endif
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    અક્ષર *col_string = getenv ("COLUMNS");
    long પૂર્ણાંક n_columns;
    જો (!(col_string != ખાલી
          && xstrtol (col_string, ખાલી, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= INT_MAX))
      n_columns = 80;
    પાછા n_columns;
  }
}

ATTRIBUTE_PURE
સ્થિર tcflag_t *
mode_type_flag (ગણના mode_type type, સંરચના termios *mode)
{
  પસંદગી (type)
    {
    સ્થિતિ control:
      પાછા &mode->c_cflag;

    સ્થિતિ આગત:
      પાછા &mode->c_iflag;

    સ્થિતિ output:
      પાછા &mode->c_oflag;

    સ્થિતિ local:
      પાછા &mode->c_lflag;

    સ્થિતિ combination:
      પાછા ખાલી;

    મૂળભૂત:
      unreachable ();
    }
}

સ્થિર શૂન્ય
display_settings (ગણના output_type output_type, સંરચના termios *mode,
                  અક્ષર અચલ *device_name)
{
  પસંદગી (output_type)
    {
    સ્થિતિ changed:
      display_changed (mode);
      તોડો;

    સ્થિતિ all:
      display_all (mode, device_name);
      તોડો;

    સ્થિતિ recoverable:
      display_recoverable (mode);
      તોડો;
    }
}

સ્થિર શૂન્ય
display_changed (સંરચના termios *mode)
{
  bool empty_લીટી;
  tcflag_t *bitsp;
  unsigned long mask;
  ગણના mode_type prev_type = control;

  display_speed (mode, true);
#ifdef HAVE_C_લીટી
  wrapf ("line = %d;", mode->c_લીટી);
#endif
  putchar ('\n');
  current_col = 0;

  empty_લીટી = true;
  માટે (પૂર્ણાંક i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      જો (mode->c_cc[control_info[i].offset] == control_info[i].saneval)
        ચાલુ;

#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      જો (streq (control_info[i].name, "flush"))
        ચાલુ;
#endif
      /* If swtch is the same as susp, don't print both.  */
#જો VSWTCH == VSUSP
      જો (streq (control_info[i].name, "swtch"))
        ચાલુ;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#જો VEOF == VMIN
      જો ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ચાલુ;
#endif

      empty_લીટી = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
  જો ((mode->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (unsigned long પૂર્ણાંક) mode->c_cc[VMIN],
             (unsigned long પૂર્ણાંક) mode->c_cc[VTIME]);
    }
  નહીંતર જો (!empty_લીટી)
    putchar ('\n');
  current_col = 0;

  empty_લીટી = true;
  માટે (પૂર્ણાંક i = 0; mode_info[i].name != ખાલી; ++i)
    {
      જો (mode_info[i].flags & OMIT)
        ચાલુ;
      જો (mode_info[i].type != prev_type)
        {
          જો (!empty_લીટી)
            {
              putchar ('\n');
              current_col = 0;
              empty_લીટી = true;
            }
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      જો ((*bitsp & mask) == mode_info[i].bits)
        {
          જો (mode_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", mode_info[i].name);
              empty_લીટી = false;
            }
        }
      નહીંતર જો ((mode_info[i].flags & (SANE_SET | REV)) == (SANE_SET | REV))
        {
          wrapf ("-%s", mode_info[i].name);
          empty_લીટી = false;
        }
    }
  જો (!empty_લીટી)
    putchar ('\n');
  current_col = 0;
}

સ્થિર શૂન્ય
display_all (સંરચના termios *mode, અક્ષર અચલ *device_name)
{
  tcflag_t *bitsp;
  unsigned long mask;
  ગણના mode_type prev_type = control;

  display_speed (mode, true);
#ifdef TIOCGWINSZ
  display_window_size (true, device_name);
#endif
#ifdef HAVE_C_લીટી
  wrapf ("line = %d;", mode->c_લીટી);
#endif
  putchar ('\n');
  current_col = 0;

  માટે (પૂર્ણાંક i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      જો (streq (control_info[i].name, "flush"))
        ચાલુ;
#endif
      /* If swtch is the same as susp, don't print both.  */
#જો VSWTCH == VSUSP
      જો (streq (control_info[i].name, "swtch"))
        ચાલુ;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#જો VEOF == VMIN
      જો ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        ચાલુ;
#endif
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
#જો VEOF == VMIN
  જો ((mode->c_lflag & ICANON) == 0)
#endif
    wrapf ("min = %lu; time = %lu;",
           (unsigned long પૂર્ણાંક) mode->c_cc[VMIN],
           (unsigned long પૂર્ણાંક) mode->c_cc[VTIME]);
  જો (current_col != 0)
    putchar ('\n');
  current_col = 0;

  માટે (પૂર્ણાંક i = 0; mode_info[i].name != ખાલી; ++i)
    {
      જો (mode_info[i].flags & OMIT)
        ચાલુ;
      જો (mode_info[i].type != prev_type)
        {
          putchar ('\n');
          current_col = 0;
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      જો ((*bitsp & mask) == mode_info[i].bits)
        wrapf ("%s", mode_info[i].name);
      નહીંતર જો (mode_info[i].flags & REV)
        wrapf ("-%s", mode_info[i].name);
    }
  putchar ('\n');
  current_col = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

સ્થિર શૂન્ય
check_speed (સંરચના termios *mode)
{
  જો (last_ibaud.valid && last_obaud.valid)
    {
      જો (cfgetispeed (mode) != last_ibaud.baud
          || cfgetospeed (mode) != last_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_to_કિંમત (last_ibaud.baud),
               baud_to_કિંમત (last_obaud.baud));
    }
}

સ્થિર શૂન્ય
display_speed (સંરચના termios *mode, bool fancy)
{
  જો (cfgetispeed (mode) == 0 || cfgetispeed (mode) == cfgetospeed (mode))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_to_કિંમત (cfgetospeed (mode)));
  નહીંતર
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_to_કિંમત (cfgetispeed (mode)),
           baud_to_કિંમત (cfgetospeed (mode)));
  જો (!fancy)
    current_col = 0;
}

સ્થિર શૂન્ય
display_recoverable (સંરચના termios *mode)
{
  printf ("%lx:%lx:%lx:%lx",
          (unsigned long પૂર્ણાંક) mode->c_iflag,
          (unsigned long પૂર્ણાંક) mode->c_oflag,
          (unsigned long પૂર્ણાંક) mode->c_cflag,
          (unsigned long પૂર્ણાંક) mode->c_lflag);
  માટે (size_t i = 0; i < NCCS; ++i)
    printf (":%lx", (unsigned long પૂર્ણાંક) mode->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#ifdef CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (mode->c_cflag & CBAUD) == BOTHER;
#  જો defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((mode->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#endif
  જો (separate_speeds)
    printf (":%lx:%lx", (unsigned long પૂર્ણાંક) cfgetispeed (mode),
            (unsigned long પૂર્ણાંક) cfgetospeed (mode));

  putchar ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#define STRTOUL_TYPE(TYPE)                                               \
  સ્થિર પૂર્ણાંક                                                             \
  strtoul_##TYPE (અક્ષર અચલ *s, પૂર્ણાંક base, અક્ષર **p, TYPE *result,       \
                  અક્ષર delim)                                            \
  {                                                                      \
    unsigned long ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, base);                                           \
    જો (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      પાછા -1;                                                         \
    *result = ul;                                                        \
    પાછા 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (speed_t)
#undef STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#અન્તર્ભાવ "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
સ્થિર bool
recover_mode (અક્ષર અચલ *arg, સંરચના termios *mode)
{
  tcflag_t flag[4];
  અક્ષર અચલ *s = arg;
  માટે (size_t i = 0; i < 4; i++)
    {
      અક્ષર *p;
      જો (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        પાછા false;
      s = p + 1;
    }
  mode->c_iflag = flag[0];
  mode->c_oflag = flag[1];
  mode->c_cflag = flag[2];
  mode->c_lflag = flag[3];

  માટે (size_t i = 0; i < NCCS; ++i)
    {
      અક્ષર *p;
      અક્ષર delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      જો (strtoul_cc_t (s, 16, &p, mode->c_cc + i, delim) != 0)
        પાછા false;
      /* Accept the old format, which has no separate speed fields.  */
      જો (! *p)
        {
#ifdef CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_SPEED_T_SANE
          સંરચના speed_desc ospeed = cbaud_to_speed (mode->c_cflag & CBAUD);
          જો (ospeed.valid && cfsetospeed (mode, ospeed.baud) != 0)
            પાછા false;
#  જો defined CIBAUD && defined IBSHIFT
          સંરચના speed_desc ispeed =
            cbaud_to_speed ((mode->c_cflag & CIBAUD) >> IBSHIFT);
          જો (ispeed.valid && cfsetispeed (mode, ispeed.baud) != 0)
            પાછા false;
#  endif
# endif
          last_ibaud.baud = cfgetispeed (mode);
          last_obaud.baud = cfgetospeed (mode);
#endif
          પાછા true;
        }
      s = p + 1;
    }

  speed_t speed[2];
  માટે (size_t i = 0; i < 2; i++)
    {
      અક્ષર *p;
      અક્ષર delim = i == 0 ? ':' : '\0';
      જો (strtoul_speed_t (s, 16, &p, speed + i, delim) != 0)
        પાછા false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  જો (cfsetospeed (mode, speed[1]) != 0
      || cfsetispeed (mode, speed[0]) != 0)
    પાછા false;

  last_ibaud.baud = speed[0] == 0 ? cfgetispeed (mode) : speed[0];
  last_obaud.baud = speed[1];
  પાછા true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
સ્થિર bool
speed_matches (સંરચના termios અચલ *mode1, સંરચના termios અચલ *mode2)
{
  speed_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  speed_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  જો (speed1[1] == 0)
    speed1[1] = speed1[0];
  જો (speed2[1] == 0)
    speed2[1] = speed2[0];

  unsigned પૂર્ણાંક bits = 2;  /* Start and stop bits.  */
  bits += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  bits += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  પસંદગી (mode1->c_cflag & CSIZE)
    {
      સ્થિતિ CS5: bits += 5; તોડો;
      સ્થિતિ CS6: bits += 6; તોડો;
      સ્થિતિ CS7: bits += 7; તોડો;
      મૂળભૂત: bits += 8; તોડો;
    }

  માટે (પૂર્ણાંક i = 0; i < 2; i++)
    {
      unsigned long પૂર્ણાંક value1;
      unsigned long પૂર્ણાંક value2;
      unsigned long પૂર્ણાંક delta;

      જો (speed1[i] == speed2[i])
        ચાલુ;

      value1 = baud_to_કિંમત (speed1[i]);
      value2 = baud_to_કિંમત (speed2[i]);

#ifndef TERMIOS_SPEED_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      જો (value1 == ULONG_MAX || value2 == ULONG_MAX)
        પાછા false;
#endif

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      જો (delta > value1 / (bits * 4))
        પાછા false;
    }

  પાછા true;
}

ATTRIBUTE_PURE
સ્થિર સંરચના speed_desc
string_to_baud (અક્ષર અચલ *arg)
{
  અક્ષર *ep;
  unsigned long કિંમત;
  unsigned અક્ષર c;

  /* Explicitly disallow negative numbers.  */
  જ્યારે (c_isspace (*arg))
    arg++;
  જો (*arg == '-')
    જાઓ invalid;

  /* Empty does not mean 0.  */
  જો (! *arg)
    જાઓ invalid;

  કિંમત = strtoul (arg, &ep, 10);

  c = *ep++;
  જો (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      જો (c)
        {
          unsigned અક્ષર d = c - '0';
          જો (d > 5)
            કિંમત++;
          નહીંતર જો (d == 5)
            {
              જ્યારે ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              જો (c)
                કિંમત++;                /* Nonzero, round up */
              નહીંતર
                કિંમત += (કિંમત & 1);   /* Exactly in the middle, round even */
            }

          જ્યારે (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          જો (c)
            જાઓ invalid; /* Garbage after otherwise valid number */
        }
    }
  નહીંતર જો (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      જો (streq (arg, "exta"))
        પાછા (સંરચના speed_desc) { .baud = B19200, .valid = true };
      નહીંતર જો (streq (arg, "extb"))
        પાછા (સંરચના speed_desc) { .baud = B38400, .valid = true };
      નહીંતર
        જાઓ invalid;
    }

  પાછા કિંમત_to_baud (કિંમત);

 invalid:
  પાછા (સંરચના speed_desc) { .baud = (speed_t) -1, .valid = false };
}

સ્થિર શૂન્ય
sane_mode (સંરચના termios *mode)
{
  tcflag_t *bitsp;

  માટે (પૂર્ણાંક i = 0; control_info[i].name; ++i)
    {
#જો VMIN == VEOF
      જો (streq (control_info[i].name, "min"))
        તોડો;
#endif
      mode->c_cc[control_info[i].offset] = control_info[i].saneval;
    }

  માટે (પૂર્ણાંક i = 0; mode_info[i].name != ખાલી; ++i)
    {
      જો (mode_info[i].flags & NO_SETATTR)
        ચાલુ;

      જો (mode_info[i].flags & SANE_SET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~mode_info[i].mask) | mode_info[i].bits;
        }
      નહીંતર જો (mode_info[i].flags & SANE_UNSET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~mode_info[i].mask & ~mode_info[i].bits;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

સ્થિર અક્ષર અચલ *
visible (cc_t ch)
{
  સ્થિર અક્ષર buf[10];
  અક્ષર *bpout = buf;

  જો (ch == _POSIX_VDISABLE)
    પાછા "<undef>";

  જો (ch >= 32)
    {
      જો (ch < 127)
        *bpout++ = ch;
      નહીંતર જો (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      નહીંતર
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          જો (ch >= 128 + 32)
            {
              જો (ch < 128 + 127)
                *bpout++ = ch - 128;
              નહીંતર
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          નહીંતર
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  નહીંતર
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  પાછા (અક્ષર અચલ *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

સ્થિર uintmax_t
integer_arg (અક્ષર અચલ *s, uintmax_t maxval)
{
  પાછા xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
