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

#ifdef TERMIOS_NEEDS_XOPEN_SOURCE
# define _XOPEN_SOURCE
#endif

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव <termios.h>
#यदि HAVE_STROPTS_H
# include <stropts.h>
#endif
#अन्तर्भाव <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif
#ifdef GWINSZ_IN_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#endif
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
#यदि defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#endif
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#यदि defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#endif
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#यदि defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# यदि defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#endif
#यदि defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#यदि defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#endif

#यदि defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#endif
#यदि defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#endif
#यदि defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#यदि defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#endif
#यदि defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#endif
#यदि defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#endif
#यदि defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#endif
#यदि defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#endif
#यदि defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#endif
#यदि defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#endif
#यदि defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#endif
#यदि defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#endif
#यदि defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#endif

/* Which speeds to set.  */
गणना speed_setting
  {
    आगत_speed, output_speed, both_speeds
  };

/* What to output and how.  */
गणना output_type
  {
    changed, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
गणना mode_type
  {
    control, आगत, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#define SANE_SET 1		/* Set in 'sane' mode. */
#define SANE_UNSET 2		/* Unset in 'sane' mode. */
#define REV 4			/* Can be turned off by prepending '-'. */
#define OMIT 8			/* Don't display value. */
#define NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
संरचना mode_info
  {
    वर्ण अचर *name;		/* Name given on command line.  */
    गणना mode_type type;	/* Which structure element to change. */
    वर्ण flags;			/* Setting and display options.  */
    अचिन्हित दीर्घ bits;		/* Bits to set for this mode.  */
    अचिन्हित दीर्घ mask;		/* Other bits to turn off for this mode.  */
  };

स्थिर संरचना mode_info अचर mode_info[] =
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

  {"ignbrk", आगत, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", आगत, SANE_SET | REV, BRKINT, 0},
  {"ignpar", आगत, REV, IGNPAR, 0},
  {"parmrk", आगत, REV, PARMRK, 0},
  {"inpck", आगत, REV, INPCK, 0},
  {"istrip", आगत, REV, ISTRIP, 0},
  {"inlcr", आगत, SANE_UNSET | REV, INLCR, 0},
  {"igncr", आगत, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", आगत, SANE_SET | REV, ICRNL, 0},
  {"ixon", आगत, REV, IXON, 0},
  {"ixoff", आगत, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", आगत, REV | OMIT, IXOFF, 0},
#ifdef IUCLC
  {"iuclc", आगत, SANE_UNSET | REV, IUCLC, 0},
#endif
#ifdef IXANY
  {"ixany", आगत, SANE_UNSET | REV, IXANY, 0},
#endif
#ifdef IMAXBEL
  {"imaxbel", आगत, SANE_SET | REV, IMAXBEL, 0},
#endif
#ifdef IUTF8
  {"iutf8", आगत, SANE_UNSET | REV, IUTF8, 0},
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
#अन्यथा
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
#यदि defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#अथवायदि defined EXTPROC
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
#यदि defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#endif
#यदि defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#endif
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {रिक्त, control, 0, 0, 0}
};

/* Control character settings.  */
संरचना control_info
  {
    वर्ण अचर *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    माप_प्रकार offset;		/* Offset in c_cc.  */
  };

/* Control characters. */

स्थिर संरचना control_info अचर control_info[] =
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
#अन्यथा
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
  {रिक्त, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
संरचना speed_desc
{
  /* The baud rate.  */
  speed_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

स्थिर वर्ण अचर *visible (cc_t ch);
स्थिर अचिन्हित दीर्घ पूर्णाङ्क baud_to_मान (speed_t speed);
स्थिर bool recover_mode (वर्ण अचर *arg, संरचना termios *mode);
स्थिर पूर्णाङ्क screen_columns (शून्य);
स्थिर bool set_mode (संरचना mode_info अचर *info, bool reversed,
                      संरचना termios *mode);
स्थिर bool eq_mode (संरचना termios अचर *mode1,
                     संरचना termios अचर *mode2);
स्थिर bool speed_matches (संरचना termios अचर *mode1,
                           संरचना termios अचर *mode2);
स्थिर uintmax_t integer_arg (वर्ण अचर *s, uintmax_t max);
स्थिर संरचना speed_desc string_to_baud (वर्ण अचर *arg);
स्थिर tcflag_t *mode_type_flag (गणना mode_type type, संरचना termios *mode);
स्थिर शून्य display_all (संरचना termios *mode, वर्ण अचर *device_name);
स्थिर शून्य display_changed (संरचना termios *mode);
स्थिर शून्य display_recoverable (संरचना termios *mode);
स्थिर शून्य display_settings (गणना output_type output_type,
                              संरचना termios *mode,
                              वर्ण अचर *device_name);
स्थिर शून्य check_speed (संरचना termios *mode);
स्थिर शून्य display_speed (संरचना termios *mode, bool fancy);
स्थिर शून्य display_window_size (bool fancy, वर्ण अचर *device_name);
स्थिर शून्य sane_mode (संरचना termios *mode);
स्थिर शून्य set_control_char (संरचना control_info अचर *info,
                              वर्ण अचर *arg,
                              संरचना termios *mode);
स्थिर शून्य set_speed (गणना speed_setting type, वर्ण अचर *arg,
                       संरचना speed_desc अचर speed, संरचना termios *mode);
स्थिर शून्य set_window_size (पूर्णाङ्क rows, पूर्णाङ्क cols, वर्ण अचर *device_name);

/* The width of the screen, for output wrapping. */
स्थिर पूर्णाङ्क max_col;

/* Current position, to know when to wrap. */
स्थिर पूर्णाङ्क current_col;

/* Default "drain" mode for tcsetattr.  */
स्थिर पूर्णाङ्क tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
स्थिर bool dev_debug;

/* Record last speed set for correlation.  */
स्थिर संरचना speed_desc last_ibaud =
  { .baud = (speed_t) -1, .valid = false };
स्थिर संरचना speed_desc last_obaud =
  { .baud = (speed_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEV_DEBUG_OPTION = अक्षर_उच्च + 1,
};

स्थिर संरचना option अचर longopts[] =
{
  {"all", no_argument, रिक्त, 'a'},
  {"save", no_argument, रिक्त, 'g'},
  {"file", required_argument, रिक्त, 'F'},
  {"-debug", no_argument, रिक्त, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((म_लिखो, 1, 2))
स्थिर शून्य
wrapf (वर्ण अचर *message,...)
{
  बहु_सूची args;
  वर्ण *buf;
  पूर्णाङ्क buflen;

  बहु_शुरू (args, message);
  buflen = vasprintf (&buf, message, args);
  बहु_पूर्ण (args);

  यदि (buflen < 0)
    xalloc_die ();

  यदि (0 < current_col)
    {
      यदि (max_col - current_col <= buflen)
        {
          अक्षर_दो ('\n');
          current_col = 0;
        }
      अन्यथा
        {
          अक्षर_दो (' ');
          current_col++;
        }
    }

  ख_माला_दो (buf, मानक_निकास);
  मुक्त (buf);
  current_col += buflen;
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              program_name, program_name, program_name);
      ख_माला_दो (_("\
Print or change terminal characteristics.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings in human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings in a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  खोलो and use DEVICE instead of standard आगत\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying प्रणाली defines which settings are available.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Special characters:\n"), मानक_निकास);
#ifdef VFLUSHO
      ख_माला_दो (_("\
 * discard CHAR  CHAR will toggle discarding of output\n\
"), मानक_निकास);
#endif
#ifdef VDSUSP
      ख_माला_दो (_("\
 * dsusp CHAR    CHAR will send a terminal stop सङ्केत once आगत flushed\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   eof CHAR      CHAR will send an end of file (terminate the आगत)\n\
   eol CHAR      CHAR will end the पङ्क्ति\n\
"), मानक_निकास);
#ifdef VEOL2
      ख_माला_दो (_("\
 * eol2 CHAR     alternate CHAR लागि ending the पङ्क्ति\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   erase CHAR    CHAR will erase the last character typed\n\
   intr CHAR     CHAR will send an interrupt सङ्केत\n\
   समाप्त CHAR     CHAR will erase the current पङ्क्ति\n\
"), मानक_निकास);
#ifdef VLNEXT
      ख_माला_दो (_("\
 * lnext CHAR    CHAR will enter the next character quoted\n\
"), मानक_निकास);
#endif
#ifdef VSTATUS
      ख_माला_दो (_("\
 * status CHAR   CHAR will send an info सङ्केत\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   quit CHAR     CHAR will send a quit सङ्केत\n\
"), मानक_निकास);
#यदि defined CREPRINT || defined VREPRINT
      ख_माला_दो (_("\
 * rprnt CHAR    CHAR will redraw the current पङ्क्ति\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   start CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop सङ्केत\n\
"), मानक_निकास);
#ifdef VSWTCH
      ख_माला_दो (_("\
 * swtch CHAR    CHAR will विकल्प to a different shell layer\n\
"), मानक_निकास);
#endif
#ifdef VWERASE
      ख_माला_दो (_("\
 * werase CHAR   CHAR will erase the last word typed\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
\n\
Special settings:\n\
   N             set the आगत and output speeds to N bauds\n\
"), मानक_निकास);
#ifdef TIOCGWINSZ
      ख_माला_दो (_("\
   cols N        tell the kernel that the terminal has N columns\n\
 * columns N     same as cols N\n\
"), मानक_निकास);
#endif
      म_लिखो (_("\
 * [-]drain      रुको लागि transmission before applying settings (%s by स्वभाव)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      ख_माला_दो (_("\
   ispeed N      set the आगत speed to N\n\
"), मानक_निकास);
#ifdef HAVE_C_पङ्क्ति
      ख_माला_दो (_("\
 * पङ्क्ति N        use पङ्क्ति discipline N\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   min N         with -icanon, set N characters minimum लागि a completed पठ\n\
   ospeed N      set the output speed to N\n\
"), मानक_निकास);
#ifdef TIOCGWINSZ
      ख_माला_दो (_("\
   rows N        tell the kernel that the terminal has N rows\n\
   size          print the सङ्ख्या of rows and columns according to the kernel\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   speed         print the terminal speed\n\
   समय N        with -icanon, set पठ timeout of N tenths of a second\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control signals\n\
   [-]cread      allow आगत to be received\n\
"), मानक_निकास);
#ifdef CRTSCTS
      ख_माला_दो (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), मानक_निकास);
#endif
#ifdef CDTRDSR
      ख_माला_दो (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   csN           set character size to N bits, N in [5..8]\n\
"), मानक_निकास);
      ख_माला_दो (_("\
   [-]cstopb     use two stop bits per character (one with '-')\n\
   [-]hup        send a hangup सङ्केत when the last process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity bit in output and expect parity bit in आगत\n\
   [-]parodd     set odd parity (or even parity with '-')\n\
"), मानक_निकास);
#ifdef CMSPAR
      ख_माला_दो (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
\n\
आगत_3 settings:\n\
   [-]brkint     breaks cause an interrupt सङ्केत\n\
   [-]icrnl      translate carriage फर्काऊ to newline\n\
   [-]ignbrk     ignore रोक characters\n\
   [-]igncr      ignore carriage फर्काऊ\n\
   [-]ignpar     ignore characters with parity errors\n\
"), मानक_निकास);
#ifdef IMAXBEL
      ख_माला_दो (_("\
 * [-]imaxbel    beep and गर not flush a full आगत buffer on a character\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   [-]inlcr      translate newline to carriage फर्काऊ\n\
   [-]inpck      enable आगत parity checking\n\
   [-]istrip     clear माथिल्लो (8th) bit of आगत characters\n\
"), मानक_निकास);
#ifdef IUTF8
      ख_माला_दो (_("\
 * [-]iutf8      assume आगत characters are UTF-8 encoded\n\
"), मानक_निकास);
#endif
#ifdef IUCLC
      ख_माला_दो (_("\
 * [-]iuclc      translate uppercase characters to lowercase\n\
"), मानक_निकास);
#endif
#ifdef IXANY
      ख_माला_दो (_("\
 * [-]ixany      let any character restart output, not only start character\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   [-]ixoff      enable sending of start/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Output settings:\n\
"), मानक_निकास);
#ifdef BSDLY
      ख_माला_दो (_("\
 * bsN           backspace delay style, N in [0..1]\n\
"), मानक_निकास);
#endif
#ifdef CRDLY
      ख_माला_दो (_("\
 * crN           carriage फर्काऊ delay style, N in [0..3]\n\
"), मानक_निकास);
#endif
#ifdef FFDLY
      ख_माला_दो (_("\
 * ffN           form feed delay style, N in [0..1]\n\
"), मानक_निकास);
#endif
#ifdef NLDLY
      ख_माला_दो (_("\
 * nlN           newline delay style, N in [0..1]\n\
"), मानक_निकास);
#endif
#ifdef OCRNL
      ख_माला_दो (_("\
 * [-]ocrnl      translate carriage फर्काऊ to newline\n\
"), मानक_निकास);
#endif
#ifdef OFDEL
      ख_माला_दो (_("\
 * [-]ofdel      use delete characters लागि fill instead of NUL characters\n\
"), मानक_निकास);
#endif
#ifdef OFILL
      ख_माला_दो (_("\
 * [-]ofill      use fill (padding) characters instead of timing लागि delays\n\
"), मानक_निकास);
#endif
#ifdef OLCUC
      ख_माला_दो (_("\
 * [-]olcuc      translate lowercase characters to uppercase\n\
"), मानक_निकास);
#endif
#ifdef ONLCR
      ख_माला_दो (_("\
 * [-]onlcr      translate newline to carriage फर्काऊ-newline\n\
"), मानक_निकास);
#endif
#ifdef ONLRET
      ख_माला_दो (_("\
 * [-]onlret     newline performs a carriage फर्काऊ\n\
"), मानक_निकास);
#endif
#ifdef ONOCR
      ख_माला_दो (_("\
 * [-]onocr      गर not print carriage returns in the first column\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   [-]opost      postprocess output\n\
"), मानक_निकास);
#यदि defined TABDLY || defined OXTABS
      ख_माला_दो (_("\
 * tabN          horizontal tab delay style, N in [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), मानक_निकास);
#endif
#ifdef VTDLY
      ख_माला_दो (_("\
 * vtN           vertical tab delay style, N in [0..1]\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), मानक_निकास);
#ifdef ECHOKE
      ख_माला_दो (_("\
 * crtkill       समाप्त all पङ्क्ति by obeying the echoprt and echoe settings\n\
 * -crtkill      समाप्त all पङ्क्ति by obeying the echoctl and echok settings\n\
"), मानक_निकास);
#endif
#ifdef ECHOCTL
      ख_माला_दो (_("\
 * [-]ctlecho    echo control characters in hat notation ('^c')\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   [-]echo       echo आगत characters\n\
"), मानक_निकास);
#ifdef ECHOCTL
      ख_माला_दो (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a समाप्त character\n\
"), मानक_निकास);
#ifdef ECHOKE
      ख_माला_दो (_("\
 * [-]echoke     same as [-]crtkill\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   [-]echonl     echo newline even यदि not echoing other characters\n\
"), मानक_निकास);
#ifdef ECHOPRT
      ख_माला_दो (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), मानक_निकास);
#endif
#यदि defined EXTPROC || defined TIOCEXT
      ख_माला_दो (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with माथिल्लो latency links\n\
"), मानक_निकास);
#endif
#यदि defined FLUSHO
      ख_माला_दो (_("\
 * [-]flusho     discard output\n\
"), मानक_निकास);
#endif
      म_लिखो (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, समाप्त"
#ifdef VWERASE
    ", werase"
#endif
#यदि defined CREPRINT || defined VREPRINT
    ", rprnt"
#endif
);
      ख_माला_दो (_("\
   [-]isig       enable interrupt, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after interrupt and quit special characters\n\
"), मानक_निकास);
#ifdef ECHOPRT
      ख_माला_दो (_("\
 * [-]prterase   same as [-]echoprt\n\
"), मानक_निकास);
#endif
#ifdef TOSTOP
      ख_माला_दो (_("\
 * [-]tostop     stop background jobs that try to लिख to the terminal\n\
"), मानक_निकास);
#endif
#ifdef XCASE
      ख_माला_दो (_("\
 * [-]xcase      with icanon, escape with '\\' लागि uppercase characters\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
\n\
Combination settings:\n\
"), मानक_निकास);
#यदि defined XCASE && defined IUCLC && defined OLCUC
      ख_माला_दो (_("\
 * [-]LCASE      same as [-]lcase\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), मानक_निकास);
      ख_माला_दो (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters to their स्वभाव मानहरू\n\
   -cooked       same as raw\n\
"), मानक_निकास);
      म_लिखो (_("\
   crt           same as %s\n\
"), "echoe"
#ifdef ECHOCTL
    " echoctl"
#endif
#ifdef ECHOKE
    " echoke"
#endif
);
      म_लिखो (_("\
   dec           same as %s intr ^c erase 0177\n\
                 समाप्त ^u\n\
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
      ख_माला_दो (_("\
 * [-]decctlq    same as [-]ixany\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   ek            erase and समाप्त characters to their स्वभाव मानहरू\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), मानक_निकास);
#यदि defined XCASE && defined IUCLC && defined OLCUC
      ख_माला_दो (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), मानक_निकास);
#endif
      ख_माला_दो (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), मानक_निकास);
      म_लिखो (_("\
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
      ख_माला_दो (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), मानक_निकास);
      म_लिखो (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 समय 0\n\
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
      म_लिखो (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters to their स्वभाव मानहरू\n\
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
      ख_माला_दो (_("\
\n\
Handle the tty पङ्क्ति connected to standard आगत.  Without arguments,\n\
prints baud rate, पङ्क्ति discipline, and deviations from stty sane.  In\n\
settings, CHAR is taken literally, or coded as in ^c, 0x37, 0177 or\n\
127; special मानहरू ^- or undef used to disable special characters.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

स्थिर शून्य
apply_settings (bool checking, वर्ण अचर *device_name,
                वर्ण * अचर *settings, पूर्णाङ्क n_settings,
                संरचना termios *mode, bool *require_set_attr)
{
#define check_argument(arg)						\
  गर									\
    {									\
      यदि (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (निकास_त्रुटि);						\
        }								\
    }									\
  जबसम्म (0)

  लागि (पूर्णाङ्क k = 1; k < n_settings; k++)
    {
      वर्ण अचर *arg = settings[k];
      bool match_found = false;
      bool not_set_attr = false;
      bool reversed = false;

      यदि (! arg)
        जारी;

      यदि (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      यदि (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          जारी;
        }
      लागि (पूर्णाङ्क i = 0; mode_info[i].name != रिक्त; ++i)
        {
          यदि (streq (arg, mode_info[i].name))
            {
              यदि ((mode_info[i].flags & NO_SETATTR) == 0)
                {
                  match_found = set_mode (&mode_info[i], reversed, mode);
                  *require_set_attr = true;
                }
              अन्यथा
                match_found = not_set_attr = true;
              रोक;
            }
        }
      यदि (!match_found && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (निकास_त्रुटि);
        }
      यदि (!match_found)
        {
          लागि (पूर्णाङ्क i = 0; control_info[i].name != रिक्त; ++i)
            {
              यदि (streq (arg, control_info[i].name))
                {
                  check_argument (arg);
                  match_found = true;
                  ++k;
                  set_control_char (&control_info[i], settings[k], mode);
                  *require_set_attr = true;
                  रोक;
                }
            }
        }
      यदि (!match_found || not_set_attr)
        {
          यदि (streq (arg, "ispeed"))
            {
              check_argument (arg);
              ++k;
              संरचना speed_desc अचर ispeed = string_to_baud (settings[k]);
              यदि (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (निकास_त्रुटि);
                }
              set_speed (आगत_speed, settings[k], ispeed, mode);
              यदि (checking)
                जारी;
              *require_set_attr = true;
            }
          अन्यथा यदि (streq (arg, "ospeed"))
            {
              check_argument (arg);
              ++k;
              संरचना speed_desc अचर ospeed = string_to_baud (settings[k]);
              यदि (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (निकास_त्रुटि);
                }
              set_speed (output_speed, settings[k], ospeed, mode);
              यदि (checking)
                जारी;
              *require_set_attr = true;
            }
#ifdef TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          अन्यथा यदि (streq (arg, "extproc"))
            {
              पूर्णाङ्क val = ! reversed;

              यदि (checking)
                जारी;

              यदि (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (निकास_त्रुटि, त्रुटि_सं, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#endif
#ifdef TIOCGWINSZ
          अन्यथा यदि (streq (arg, "rows"))
            {
              check_argument (arg);
              ++k;
              यदि (checking)
                जारी;
              set_window_size (integer_arg (settings[k], पूर्णांक_उच्च), -1,
                               device_name);
            }
          अन्यथा यदि (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_argument (arg);
              ++k;
              यदि (checking)
                जारी;
              set_window_size (-1, integer_arg (settings[k], पूर्णांक_उच्च),
                               device_name);
            }
          अन्यथा यदि (streq (arg, "size"))
            {
              यदि (checking)
                जारी;
              max_col = screen_columns ();
              current_col = 0;
              display_window_size (false, device_name);
            }
#endif
#ifdef HAVE_C_पङ्क्ति
          अन्यथा यदि (streq (arg, "line"))
            {
              check_argument (arg);
              ++k;
              uintmax_t मान = integer_arg (settings[k], UINTMAX_MAX);
              यदि (ckd_add (&mode->c_पङ्क्ति, मान, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_set_attr = true;
            }
#endif
          अन्यथा यदि (streq (arg, "speed"))
            {
              यदि (checking)
                जारी;
              max_col = screen_columns ();
              display_speed (mode, false);
            }
          अन्यथा
            {
              संरचना speed_desc अचर speed = string_to_baud (arg);
              यदि (speed.valid)
                {
                  set_speed (both_speeds, arg, speed, mode);
                  यदि (checking)
                    जारी;
                  *require_set_attr = true;
                }
              अन्यथा
                {
                  यदि (! recover_mode (arg, mode))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (निकास_त्रुटि);
                    }
                  *require_set_attr = true;
                }
            }
        }
    }

  यदि (checking)
    check_speed (mode);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  स्थिर संरचना termios mode;

  गणना output_type output_type;
  पूर्णाङ्क optc;
  पूर्णाङ्क argi = 0;
  पूर्णाङ्क opti = 1;
  bool require_set_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  वर्ण *file_name = रिक्त;
  वर्ण अचर *device_name;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

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

  जबसम्म ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'a':
          verbose_output = true;
          output_type = all;
          रोक;

        पक्ष 'g':
          recoverable_output = true;
          output_type = recoverable;
          रोक;

        पक्ष 'F':
          यदि (file_name)
            error (निकास_त्रुटि, 0, _("only one device may be specified"));
          file_name = optarg;
          रोक;

        पक्ष DEV_DEBUG_OPTION:
          dev_debug = true;
          रोक;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

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

          रोक;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      जबसम्म (opti < optind)
        argv[argi + opti++] = रिक्त;
    }

  /* Specifying both -a and -g gets an error.  */
  यदि (verbose_output && recoverable_output)
    error (निकास_त्रुटि, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  यदि (!noargs && (verbose_output || recoverable_output))
    error (निकास_त्रुटि, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  यदि (!noargs && !verbose_output && !recoverable_output)
    {
      स्थिर संरचना termios check_mode;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_mode, &require_set_attr);
    }

  यदि (file_name)
    {
      पूर्णाङ्क fdflags;
      यदि (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
      यदि ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  यदि (tcgetattr (STDIN_FILENO, &mode))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));

  यदि (verbose_output || recoverable_output || noargs)
    {
      max_col = screen_columns ();
      current_col = 0;
      display_settings (output_type, &mode, device_name);
      फर्काऊ निकास_सफल;
    }

  require_set_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &mode, &require_set_attr);

  यदि (require_set_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      स्थिर संरचना termios new_mode;

      यदि (tcsetattr (STDIN_FILENO, tcsetattr_options, &mode))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      यदि (tcgetattr (STDIN_FILENO, &new_mode))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));

      यदि (! eq_mode (&mode, &new_mode))
        {
          यदि (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              लागि (अचिन्हित पूर्णाङ्क i = 0; i < परिमाण (new_mode); i++)
                {
                  अचिन्हित पूर्णाङ्क newc = *(((अचिन्हित वर्ण *) &new_mode) + i);
                  अचिन्हित पूर्णाङ्क oldc = *(((अचिन्हित वर्ण *) &mode) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (निकास_त्रुटि, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  फर्काऊ निकास_सफल;
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
eq_mode (संरचना termios अचर *mode1, संरचना termios अचर *mode2)
{
  यदि (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#ifdef HAVE_C_पङ्क्ति
      || mode1->c_पङ्क्ति != mode2->c_पङ्क्ति
#endif
      || !memeq (mode1->c_cc, mode2->c_cc, परिमाण (mode1->c_cc)))
    फर्काऊ false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_mask = ~(tcflag_t) 0;
#ifdef CBAUD
  cflag_mask &= ~(tcflag_t) CBAUD;
#endif
#ifdef CIBAUD
  cflag_mask &= ~(tcflag_t) CIBAUD;
#endif

  यदि ((mode1->c_cflag ^ mode2->c_cflag) & cflag_mask)
    फर्काऊ false;

  फर्काऊ speed_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

स्थिर bool
set_mode (संरचना mode_info अचर *info, bool reversed, संरचना termios *mode)
{
  tcflag_t *bitsp;

  यदि (reversed && (info->flags & REV) == 0)
    फर्काऊ false;

  bitsp = mode_type_flag (info->type, mode);

  यदि (bitsp == रिक्त)
    {
      /* Combination mode. */
      यदि (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          यदि (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          अन्यथा
            mode->c_cflag = (mode->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      अन्यथा यदि (streq (info->name, "oddp"))
        {
          यदि (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          अन्यथा
            mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      अन्यथा यदि (streq (info->name, "nl"))
        {
          यदि (reversed)
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
          अन्यथा
            {
              mode->c_iflag = mode->c_iflag & ~ICRNL;
#ifdef ONLCR
              mode->c_oflag = mode->c_oflag & ~ONLCR;
#endif
            }
        }
      अन्यथा यदि (streq (info->name, "ek"))
        {
          mode->c_cc[VERASE] = CERASE;
          mode->c_cc[VKILL] = CKILL;
        }
      अन्यथा यदि (streq (info->name, "sane"))
        sane_mode (mode);
      अन्यथा यदि (streq (info->name, "cbreak"))
        {
          यदि (reversed)
            mode->c_lflag |= ICANON;
          अन्यथा
            mode->c_lflag &= ~ICANON;
        }
      अन्यथा यदि (streq (info->name, "pass8"))
        {
          यदि (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
            }
          अन्यथा
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
            }
        }
      अन्यथा यदि (streq (info->name, "litout"))
        {
          यदि (reversed)
            {
              mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARENB;
              mode->c_iflag |= ISTRIP;
              mode->c_oflag |= OPOST;
            }
          अन्यथा
            {
              mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
              mode->c_iflag &= ~ISTRIP;
              mode->c_oflag &= ~OPOST;
            }
        }
      अन्यथा यदि (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          यदि ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              mode->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              mode->c_oflag |= OPOST;
              mode->c_lflag |= ISIG | ICANON;
#यदि VMIN == VEOF
              mode->c_cc[VEOF] = CEOF;
#endif
#यदि VTIME == VEOL
              mode->c_cc[VEOL] = CEOL;
#endif
            }
          अन्यथा
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
      अन्यथा यदि (streq (info->name, "decctlq"))
        {
          यदि (reversed)
            mode->c_iflag |= IXANY;
          अन्यथा
            mode->c_iflag &= ~IXANY;
        }
#endif
#ifdef TABDLY
      अन्यथा यदि (streq (info->name, "tabs"))
        {
          यदि (reversed)
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB3;
          अन्यथा
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB0;
        }
#अन्यथा
# ifdef OXTABS
      अन्यथा यदि (streq (info->name, "tabs"))
        {
          यदि (reversed)
            mode->c_oflag = mode->c_oflag | OXTABS;
          अन्यथा
            mode->c_oflag = mode->c_oflag & ~OXTABS;
        }
# endif
#endif
#यदि defined XCASE && defined IUCLC && defined OLCUC
      अन्यथा यदि (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          यदि (reversed)
            {
              mode->c_lflag &= ~XCASE;
              mode->c_iflag &= ~IUCLC;
              mode->c_oflag &= ~OLCUC;
            }
          अन्यथा
            {
              mode->c_lflag |= XCASE;
              mode->c_iflag |= IUCLC;
              mode->c_oflag |= OLCUC;
            }
        }
#endif
      अन्यथा यदि (streq (info->name, "crt"))
        mode->c_lflag |= ECHOE
#ifdef ECHOCTL
          | ECHOCTL
#endif
#ifdef ECHOKE
          | ECHOKE
#endif
          ;
      अन्यथा यदि (streq (info->name, "dec"))
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
  अन्यथा यदि (reversed)
    *bitsp = *bitsp & ~info->mask & ~info->bits;
  अन्यथा
    *bitsp = (*bitsp & ~info->mask) | info->bits;

  फर्काऊ true;
}

स्थिर शून्य
set_control_char (संरचना control_info अचर *info, वर्ण अचर *arg,
                  संरचना termios *mode)
{
  अचिन्हित दीर्घ पूर्णाङ्क मान;

  यदि (streq (info->name, "min") || streq (info->name, "time"))
    मान = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  अन्यथा यदि (arg[0] == '\0' || arg[1] == '\0')
    मान = to_uchar (arg[0]);
  अन्यथा यदि (streq (arg, "^-") || streq (arg, "undef"))
    मान = _POSIX_VDISABLE;
  अन्यथा यदि (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      यदि (arg[1] == '?')
        मान = 127;
      अन्यथा
        मान = to_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  अन्यथा
    मान = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  mode->c_cc[info->offset] = मान;
}

स्थिर शून्य
set_speed (गणना speed_setting type, वर्ण अचर *arg,
           संरचना speed_desc अचर speed, संरचना termios *mode)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (speed.valid);

  यदि (type == आगत_speed || type == both_speeds)
    {
      last_ibaud = speed;
      यदि (cfsetispeed (mode, speed.baud))
        error (निकास_त्रुटि, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  यदि (type == output_speed || type == both_speeds)
    {
      last_obaud = speed;
      यदि (cfsetospeed (mode, speed.baud))
        error (निकास_त्रुटि, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#ifdef TIOCGWINSZ

स्थिर पूर्णाङ्क
get_win_size (पूर्णाङ्क fd, संरचना winsize *win)
{
  पूर्णाङ्क err = ioctl (fd, TIOCGWINSZ, (वर्ण *) win);
  फर्काऊ err;
}

स्थिर शून्य
set_window_size (पूर्णाङ्क rows, पूर्णाङ्क cols, वर्ण अचर *device_name)
{
  संरचना winsize win;

  यदि (get_win_size (STDIN_FILENO, &win))
    {
      यदि (त्रुटि_सं != EINVAL)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
      स_रखो (&win, 0, परिमाण (win));
    }

  यदि (rows >= 0)
    win.ws_row = rows;
  यदि (cols >= 0)
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

  यदि (win.ws_row == 0 || win.ws_col == 0)
    {
      संरचना ttysize ttysz;

      ttysz.ts_पङ्क्तिहरू = win.ws_row;
      ttysz.ts_cols = win.ws_col;

      win.ws_row = 1;
      win.ws_col = 1;

      यदि (ioctl (STDIN_FILENO, TIOCSWINSZ, (वर्ण *) &win))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));

      यदि (ioctl (STDIN_FILENO, TIOCSSIZE, (वर्ण *) &ttysz))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
      फर्काऊ;
    }
# endif

  यदि (ioctl (STDIN_FILENO, TIOCSWINSZ, (वर्ण *) &win))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
}

स्थिर शून्य
display_window_size (bool fancy, वर्ण अचर *device_name)
{
  संरचना winsize win;

  यदि (get_win_size (STDIN_FILENO, &win))
    {
      यदि (त्रुटि_सं != EINVAL)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
      यदि (!fancy)
        error (निकास_त्रुटि, 0,
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
#endif

स्थिर पूर्णाङ्क
screen_columns (शून्य)
{
#ifdef TIOCGWINSZ
  संरचना winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  यदि (get_win_size (STDOUT_FILENO, &win) == 0 && 0 < win.ws_col)
    फर्काऊ win.ws_col;
#endif
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    वर्ण *col_string = दो_पर्या ("COLUMNS");
    दीर्घ पूर्णाङ्क n_columns;
    यदि (!(col_string != रिक्त
          && xstrtol (col_string, रिक्त, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= पूर्णांक_उच्च))
      n_columns = 80;
    फर्काऊ n_columns;
  }
}

ATTRIBUTE_PURE
स्थिर tcflag_t *
mode_type_flag (गणना mode_type type, संरचना termios *mode)
{
  विकल्प (type)
    {
    पक्ष control:
      फर्काऊ &mode->c_cflag;

    पक्ष आगत:
      फर्काऊ &mode->c_iflag;

    पक्ष output:
      फर्काऊ &mode->c_oflag;

    पक्ष local:
      फर्काऊ &mode->c_lflag;

    पक्ष combination:
      फर्काऊ रिक्त;

    स्वभाव:
      unreachable ();
    }
}

स्थिर शून्य
display_settings (गणना output_type output_type, संरचना termios *mode,
                  वर्ण अचर *device_name)
{
  विकल्प (output_type)
    {
    पक्ष changed:
      display_changed (mode);
      रोक;

    पक्ष all:
      display_all (mode, device_name);
      रोक;

    पक्ष recoverable:
      display_recoverable (mode);
      रोक;
    }
}

स्थिर शून्य
display_changed (संरचना termios *mode)
{
  bool empty_पङ्क्ति;
  tcflag_t *bitsp;
  अचिन्हित दीर्घ mask;
  गणना mode_type prev_type = control;

  display_speed (mode, true);
#ifdef HAVE_C_पङ्क्ति
  wrapf ("line = %d;", mode->c_पङ्क्ति);
#endif
  अक्षर_दो ('\n');
  current_col = 0;

  empty_पङ्क्ति = true;
  लागि (पूर्णाङ्क i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      यदि (mode->c_cc[control_info[i].offset] == control_info[i].saneval)
        जारी;

#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      यदि (streq (control_info[i].name, "flush"))
        जारी;
#endif
      /* If swtch is the same as susp, don't print both.  */
#यदि VSWTCH == VSUSP
      यदि (streq (control_info[i].name, "swtch"))
        जारी;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#यदि VEOF == VMIN
      यदि ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        जारी;
#endif

      empty_पङ्क्ति = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
  यदि ((mode->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (अचिन्हित दीर्घ पूर्णाङ्क) mode->c_cc[VMIN],
             (अचिन्हित दीर्घ पूर्णाङ्क) mode->c_cc[VTIME]);
    }
  अन्यथा यदि (!empty_पङ्क्ति)
    अक्षर_दो ('\n');
  current_col = 0;

  empty_पङ्क्ति = true;
  लागि (पूर्णाङ्क i = 0; mode_info[i].name != रिक्त; ++i)
    {
      यदि (mode_info[i].flags & OMIT)
        जारी;
      यदि (mode_info[i].type != prev_type)
        {
          यदि (!empty_पङ्क्ति)
            {
              अक्षर_दो ('\n');
              current_col = 0;
              empty_पङ्क्ति = true;
            }
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      यदि ((*bitsp & mask) == mode_info[i].bits)
        {
          यदि (mode_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", mode_info[i].name);
              empty_पङ्क्ति = false;
            }
        }
      अन्यथा यदि ((mode_info[i].flags & (SANE_SET | REV)) == (SANE_SET | REV))
        {
          wrapf ("-%s", mode_info[i].name);
          empty_पङ्क्ति = false;
        }
    }
  यदि (!empty_पङ्क्ति)
    अक्षर_दो ('\n');
  current_col = 0;
}

स्थिर शून्य
display_all (संरचना termios *mode, वर्ण अचर *device_name)
{
  tcflag_t *bitsp;
  अचिन्हित दीर्घ mask;
  गणना mode_type prev_type = control;

  display_speed (mode, true);
#ifdef TIOCGWINSZ
  display_window_size (true, device_name);
#endif
#ifdef HAVE_C_पङ्क्ति
  wrapf ("line = %d;", mode->c_पङ्क्ति);
#endif
  अक्षर_दो ('\n');
  current_col = 0;

  लागि (पूर्णाङ्क i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#ifdef VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      यदि (streq (control_info[i].name, "flush"))
        जारी;
#endif
      /* If swtch is the same as susp, don't print both.  */
#यदि VSWTCH == VSUSP
      यदि (streq (control_info[i].name, "swtch"))
        जारी;
#endif
      /* If eof uses the same slot as min, only print whichever applies.  */
#यदि VEOF == VMIN
      यदि ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        जारी;
#endif
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
#यदि VEOF == VMIN
  यदि ((mode->c_lflag & ICANON) == 0)
#endif
    wrapf ("min = %lu; time = %lu;",
           (अचिन्हित दीर्घ पूर्णाङ्क) mode->c_cc[VMIN],
           (अचिन्हित दीर्घ पूर्णाङ्क) mode->c_cc[VTIME]);
  यदि (current_col != 0)
    अक्षर_दो ('\n');
  current_col = 0;

  लागि (पूर्णाङ्क i = 0; mode_info[i].name != रिक्त; ++i)
    {
      यदि (mode_info[i].flags & OMIT)
        जारी;
      यदि (mode_info[i].type != prev_type)
        {
          अक्षर_दो ('\n');
          current_col = 0;
          prev_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      यदि ((*bitsp & mask) == mode_info[i].bits)
        wrapf ("%s", mode_info[i].name);
      अन्यथा यदि (mode_info[i].flags & REV)
        wrapf ("-%s", mode_info[i].name);
    }
  अक्षर_दो ('\n');
  current_col = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

स्थिर शून्य
check_speed (संरचना termios *mode)
{
  यदि (last_ibaud.valid && last_obaud.valid)
    {
      यदि (cfgetispeed (mode) != last_ibaud.baud
          || cfgetospeed (mode) != last_obaud.baud)
        error (निकास_त्रुटि, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_to_मान (last_ibaud.baud),
               baud_to_मान (last_obaud.baud));
    }
}

स्थिर शून्य
display_speed (संरचना termios *mode, bool fancy)
{
  यदि (cfgetispeed (mode) == 0 || cfgetispeed (mode) == cfgetospeed (mode))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_to_मान (cfgetospeed (mode)));
  अन्यथा
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_to_मान (cfgetispeed (mode)),
           baud_to_मान (cfgetospeed (mode)));
  यदि (!fancy)
    current_col = 0;
}

स्थिर शून्य
display_recoverable (संरचना termios *mode)
{
  म_लिखो ("%lx:%lx:%lx:%lx",
          (अचिन्हित दीर्घ पूर्णाङ्क) mode->c_iflag,
          (अचिन्हित दीर्घ पूर्णाङ्क) mode->c_oflag,
          (अचिन्हित दीर्घ पूर्णाङ्क) mode->c_cflag,
          (अचिन्हित दीर्घ पूर्णाङ्क) mode->c_lflag);
  लागि (माप_प्रकार i = 0; i < NCCS; ++i)
    म_लिखो (":%lx", (अचिन्हित दीर्घ पूर्णाङ्क) mode->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#ifdef CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (mode->c_cflag & CBAUD) == BOTHER;
#  यदि defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((mode->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#endif
  यदि (separate_speeds)
    म_लिखो (":%lx:%lx", (अचिन्हित दीर्घ पूर्णाङ्क) cfgetispeed (mode),
            (अचिन्हित दीर्घ पूर्णाङ्क) cfgetospeed (mode));

  अक्षर_दो ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#define STRTOUL_TYPE(TYPE)                                               \
  स्थिर पूर्णाङ्क                                                             \
  strtoul_##TYPE (वर्ण अचर *s, पूर्णाङ्क base, वर्ण **p, TYPE *result,       \
                  वर्ण delim)                                            \
  {                                                                      \
    अचिन्हित दीर्घ ul;                                                    \
    त्रुटि_सं = 0;                                                           \
    ul = म_से_अदीर्घ (s, p, base);                                           \
    यदि (त्रुटि_सं || **p != delim || *p == s || (TYPE) ul != ul)             \
      फर्काऊ -1;                                                         \
    *result = ul;                                                        \
    फर्काऊ 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (speed_t)
#undef STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#अन्तर्भाव "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
स्थिर bool
recover_mode (वर्ण अचर *arg, संरचना termios *mode)
{
  tcflag_t flag[4];
  वर्ण अचर *s = arg;
  लागि (माप_प्रकार i = 0; i < 4; i++)
    {
      वर्ण *p;
      यदि (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        फर्काऊ false;
      s = p + 1;
    }
  mode->c_iflag = flag[0];
  mode->c_oflag = flag[1];
  mode->c_cflag = flag[2];
  mode->c_lflag = flag[3];

  लागि (माप_प्रकार i = 0; i < NCCS; ++i)
    {
      वर्ण *p;
      वर्ण delim = i < NCCS - 1 || म_अक्षर (s, ':') ? ':' : '\0';
      यदि (strtoul_cc_t (s, 16, &p, mode->c_cc + i, delim) != 0)
        फर्काऊ false;
      /* Accept the old format, which has no separate speed fields.  */
      यदि (! *p)
        {
#ifdef CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_SPEED_T_SANE
          संरचना speed_desc ospeed = cbaud_to_speed (mode->c_cflag & CBAUD);
          यदि (ospeed.valid && cfsetospeed (mode, ospeed.baud) != 0)
            फर्काऊ false;
#  यदि defined CIBAUD && defined IBSHIFT
          संरचना speed_desc ispeed =
            cbaud_to_speed ((mode->c_cflag & CIBAUD) >> IBSHIFT);
          यदि (ispeed.valid && cfsetispeed (mode, ispeed.baud) != 0)
            फर्काऊ false;
#  endif
# endif
          last_ibaud.baud = cfgetispeed (mode);
          last_obaud.baud = cfgetospeed (mode);
#endif
          फर्काऊ true;
        }
      s = p + 1;
    }

  speed_t speed[2];
  लागि (माप_प्रकार i = 0; i < 2; i++)
    {
      वर्ण *p;
      वर्ण delim = i == 0 ? ':' : '\0';
      यदि (strtoul_speed_t (s, 16, &p, speed + i, delim) != 0)
        फर्काऊ false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  यदि (cfsetospeed (mode, speed[1]) != 0
      || cfsetispeed (mode, speed[0]) != 0)
    फर्काऊ false;

  last_ibaud.baud = speed[0] == 0 ? cfgetispeed (mode) : speed[0];
  last_obaud.baud = speed[1];
  फर्काऊ true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
स्थिर bool
speed_matches (संरचना termios अचर *mode1, संरचना termios अचर *mode2)
{
  speed_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  speed_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  यदि (speed1[1] == 0)
    speed1[1] = speed1[0];
  यदि (speed2[1] == 0)
    speed2[1] = speed2[0];

  अचिन्हित पूर्णाङ्क bits = 2;  /* Start and stop bits.  */
  bits += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  bits += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  विकल्प (mode1->c_cflag & CSIZE)
    {
      पक्ष CS5: bits += 5; रोक;
      पक्ष CS6: bits += 6; रोक;
      पक्ष CS7: bits += 7; रोक;
      स्वभाव: bits += 8; रोक;
    }

  लागि (पूर्णाङ्क i = 0; i < 2; i++)
    {
      अचिन्हित दीर्घ पूर्णाङ्क value1;
      अचिन्हित दीर्घ पूर्णाङ्क value2;
      अचिन्हित दीर्घ पूर्णाङ्क delta;

      यदि (speed1[i] == speed2[i])
        जारी;

      value1 = baud_to_मान (speed1[i]);
      value2 = baud_to_मान (speed2[i]);

#ifndef TERMIOS_SPEED_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      यदि (value1 == अच_दीर्घ_उच्च || value2 == अच_दीर्घ_उच्च)
        फर्काऊ false;
#endif

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      यदि (delta > value1 / (bits * 4))
        फर्काऊ false;
    }

  फर्काऊ true;
}

ATTRIBUTE_PURE
स्थिर संरचना speed_desc
string_to_baud (वर्ण अचर *arg)
{
  वर्ण *ep;
  अचिन्हित दीर्घ मान;
  अचिन्हित वर्ण c;

  /* Explicitly disallow negative numbers.  */
  जबसम्म (c_isspace (*arg))
    arg++;
  यदि (*arg == '-')
    गच्छ invalid;

  /* Empty does not mean 0.  */
  यदि (! *arg)
    गच्छ invalid;

  मान = म_से_अदीर्घ (arg, &ep, 10);

  c = *ep++;
  यदि (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      यदि (c)
        {
          अचिन्हित वर्ण d = c - '0';
          यदि (d > 5)
            मान++;
          अन्यथा यदि (d == 5)
            {
              जबसम्म ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              यदि (c)
                मान++;                /* Nonzero, round up */
              अन्यथा
                मान += (मान & 1);   /* Exactly in the middle, round even */
            }

          जबसम्म (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          यदि (c)
            गच्छ invalid; /* Garbage after otherwise valid number */
        }
    }
  अन्यथा यदि (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      यदि (streq (arg, "exta"))
        फर्काऊ (संरचना speed_desc) { .baud = B19200, .valid = true };
      अन्यथा यदि (streq (arg, "extb"))
        फर्काऊ (संरचना speed_desc) { .baud = B38400, .valid = true };
      अन्यथा
        गच्छ invalid;
    }

  फर्काऊ मान_to_baud (मान);

 invalid:
  फर्काऊ (संरचना speed_desc) { .baud = (speed_t) -1, .valid = false };
}

स्थिर शून्य
sane_mode (संरचना termios *mode)
{
  tcflag_t *bitsp;

  लागि (पूर्णाङ्क i = 0; control_info[i].name; ++i)
    {
#यदि VMIN == VEOF
      यदि (streq (control_info[i].name, "min"))
        रोक;
#endif
      mode->c_cc[control_info[i].offset] = control_info[i].saneval;
    }

  लागि (पूर्णाङ्क i = 0; mode_info[i].name != रिक्त; ++i)
    {
      यदि (mode_info[i].flags & NO_SETATTR)
        जारी;

      यदि (mode_info[i].flags & SANE_SET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~mode_info[i].mask) | mode_info[i].bits;
        }
      अन्यथा यदि (mode_info[i].flags & SANE_UNSET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~mode_info[i].mask & ~mode_info[i].bits;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

स्थिर वर्ण अचर *
visible (cc_t ch)
{
  स्थिर वर्ण buf[10];
  वर्ण *bpout = buf;

  यदि (ch == _POSIX_VDISABLE)
    फर्काऊ "<undef>";

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
  फर्काऊ (वर्ण अचर *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

स्थिर uintmax_t
integer_arg (वर्ण अचर *s, uintmax_t maxval)
{
  फर्काऊ xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
