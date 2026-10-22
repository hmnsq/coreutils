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

#समावेश <config.h>

#अगर_घोषित TERMIOS_NEEDS_XOPEN_SOURCE
# define _XOPEN_SOURCE
#पूर्ण_अगर

#समावेश <stdio.h>
#समावेश <sys/types.h>

#समावेश <termios.h>
#अगर HAVE_STROPTS_H
# include <stropts.h>
#पूर्ण_अगर
#समावेश <sys/ioctl.h>

#अगर_घोषित WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#पूर्ण_अगर
#अगर_घोषित GWINSZ_IN_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#पूर्ण_अगर
#समावेश <getopt.h>
#समावेश <stdarg.h>

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "c-ctype.h"
#समावेश "fd-reopen.h"
#समावेश "quote.h"
#समावेश "xdectoint.h"
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "stty"

#घोषणा AUTHORS proper_name ("David MacKenzie")

#अगर_अघोषित _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#पूर्ण_अगर

#घोषणा Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#अगर_अघोषित CINTR
# define CINTR Control ('c')
#पूर्ण_अगर
#अगर_अघोषित CQUIT
# define CQUIT 28
#पूर्ण_अगर
#अगर_अघोषित CERASE
# define CERASE 127
#पूर्ण_अगर
#अगर_अघोषित CKILL
# define CKILL Control ('u')
#पूर्ण_अगर
#अगर_अघोषित CEOF
# define CEOF Control ('d')
#पूर्ण_अगर
#अगर_अघोषित CEOL
# define CEOL _POSIX_VDISABLE
#पूर्ण_अगर
#अगर_अघोषित CSTART
# define CSTART Control ('q')
#पूर्ण_अगर
#अगर_अघोषित CSTOP
# define CSTOP Control ('s')
#पूर्ण_अगर
#अगर_अघोषित CSUSP
# define CSUSP Control ('z')
#पूर्ण_अगर
#अगर defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#पूर्ण_अगर
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#अगर defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#पूर्ण_अगर
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#अगर defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# अगर defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#पूर्ण_अगर
#अगर defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#पूर्ण_अगर

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#अगर defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#पूर्ण_अगर

#अगर defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#पूर्ण_अगर
#अगर defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#पूर्ण_अगर
#अगर defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#पूर्ण_अगर
#अगर defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#पूर्ण_अगर
#अगर defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#पूर्ण_अगर
#अगर defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#पूर्ण_अगर
#अगर defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#पूर्ण_अगर
#अगर defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#पूर्ण_अगर
#अगर defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#पूर्ण_अगर
#अगर defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#पूर्ण_अगर
#अगर defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#पूर्ण_अगर
#अगर defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#पूर्ण_अगर
#अगर defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#पूर्ण_अगर

/* Which speeds to set.  */
क्रमागत speed_setting
  {
    निवेश_speed, output_speed, both_speeds
  };

/* What to output and how.  */
क्रमागत output_type
  {
    changed, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
क्रमागत mode_type
  {
    control, निवेश, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#घोषणा SANE_SET 1		/* Set in 'sane' mode. */
#घोषणा SANE_UNSET 2		/* Unset in 'sane' mode. */
#घोषणा REV 4			/* Can be turned off by prepending '-'. */
#घोषणा OMIT 8			/* Don't display value. */
#घोषणा NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
काष्ठा mode_info
  {
    अक्षर स्थिर *name;		/* Name given on command line.  */
    क्रमागत mode_type type;	/* Which structure element to change. */
    अक्षर flags;			/* Setting and display options.  */
    अचिन्हित दीर्घ bits;		/* Bits to set for this mode.  */
    अचिन्हित दीर्घ mask;		/* Other bits to turn off for this mode.  */
  };

अटल काष्ठा mode_info स्थिर mode_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#अगर_घोषित CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#पूर्ण_अगर
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_SET | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#अगर_घोषित CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#पूर्ण_अगर
#अगर_घोषित CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#पूर्ण_अगर

  {"ignbrk", निवेश, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", निवेश, SANE_SET | REV, BRKINT, 0},
  {"ignpar", निवेश, REV, IGNPAR, 0},
  {"parmrk", निवेश, REV, PARMRK, 0},
  {"inpck", निवेश, REV, INPCK, 0},
  {"istrip", निवेश, REV, ISTRIP, 0},
  {"inlcr", निवेश, SANE_UNSET | REV, INLCR, 0},
  {"igncr", निवेश, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", निवेश, SANE_SET | REV, ICRNL, 0},
  {"ixon", निवेश, REV, IXON, 0},
  {"ixoff", निवेश, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", निवेश, REV | OMIT, IXOFF, 0},
#अगर_घोषित IUCLC
  {"iuclc", निवेश, SANE_UNSET | REV, IUCLC, 0},
#पूर्ण_अगर
#अगर_घोषित IXANY
  {"ixany", निवेश, SANE_UNSET | REV, IXANY, 0},
#पूर्ण_अगर
#अगर_घोषित IMAXBEL
  {"imaxbel", निवेश, SANE_SET | REV, IMAXBEL, 0},
#पूर्ण_अगर
#अगर_घोषित IUTF8
  {"iutf8", निवेश, SANE_UNSET | REV, IUTF8, 0},
#पूर्ण_अगर

  {"opost", output, SANE_SET | REV, OPOST, 0},
#अगर_घोषित OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#पूर्ण_अगर
#अगर_घोषित OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#पूर्ण_अगर
#अगर_घोषित ONLCR
  {"onlcr", output, SANE_SET | REV, ONLCR, 0},
#पूर्ण_अगर
#अगर_घोषित ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#पूर्ण_अगर
#अगर_घोषित ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#पूर्ण_अगर
#अगर_घोषित OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#पूर्ण_अगर
#अगर_घोषित OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#पूर्ण_अगर
#अगर_घोषित NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_SET, NL0, NLDLY},
#पूर्ण_अगर
#अगर_घोषित CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_SET, CR0, CRDLY},
#पूर्ण_अगर
#अगर_घोषित TABDLY
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
#पूर्ण_अगर
#अगर_घोषित BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_SET, BS0, BSDLY},
#पूर्ण_अगर
#अगर_घोषित VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_SET, VT0, VTDLY},
#पूर्ण_अगर
#अगर_घोषित FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_SET, FF0, FFDLY},
#पूर्ण_अगर

  {"isig", local, SANE_SET | REV, ISIG, 0},
  {"icanon", local, SANE_SET | REV, ICANON, 0},
#अगर_घोषित IEXTEN
  {"iexten", local, SANE_SET | REV, IEXTEN, 0},
#पूर्ण_अगर
  {"echo", local, SANE_SET | REV, ECHO, 0},
  {"echoe", local, SANE_SET | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_SET | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#अगर_घोषित XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#पूर्ण_अगर
#अगर_घोषित TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#पूर्ण_अगर
#अगर_घोषित ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#पूर्ण_अगर
#अगर_घोषित ECHOCTL
  {"echoctl", local, SANE_SET | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#पूर्ण_अगर
#अगर_घोषित ECHOKE
  {"echoke", local, SANE_SET | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#पूर्ण_अगर
#अगर_घोषित FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#पूर्ण_अगर
#अगर defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#या_अगर defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#पूर्ण_अगर

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
#अगर_घोषित IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#पूर्ण_अगर
#अगर defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#पूर्ण_अगर
#अगर defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#पूर्ण_अगर
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {शून्य, control, 0, 0, 0}
};

/* Control character settings.  */
काष्ठा control_info
  {
    अक्षर स्थिर *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    माप_प्रकार offset;		/* Offset in c_cc.  */
  };

/* Control characters. */

अटल काष्ठा control_info स्थिर control_info[] =
{
  {"intr", CINTR, VINTR},
  {"quit", CQUIT, VQUIT},
  {"erase", CERASE, VERASE},
  {"kill", CKILL, VKILL},
  {"eof", CEOF, VEOF},
  {"eol", CEOL, VEOL},
#अगर_घोषित VEOL2
  {"eol2", CEOL2, VEOL2},
#पूर्ण_अगर
#अगर_घोषित VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#पूर्ण_अगर
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#अगर_घोषित VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#पूर्ण_अगर
#अगर_घोषित VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#अन्यथा
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#पूर्ण_अगर
#अगर_घोषित VWERASE
  {"werase", CWERASE, VWERASE},
#पूर्ण_अगर
#अगर_घोषित VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#पूर्ण_अगर
#अगर_घोषित VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#पूर्ण_अगर
#अगर_घोषित VSTATUS
  {"status", CSTATUS, VSTATUS},
#पूर्ण_अगर

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {शून्य, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
काष्ठा speed_desc
{
  /* The baud rate.  */
  speed_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

अटल अक्षर स्थिर *visible (cc_t ch);
अटल अचिन्हित दीर्घ पूर्णांक baud_to_मूल्य (speed_t speed);
अटल bool recover_mode (अक्षर स्थिर *arg, काष्ठा termios *mode);
अटल पूर्णांक screen_columns (व्योम);
अटल bool set_mode (काष्ठा mode_info स्थिर *info, bool reversed,
                      काष्ठा termios *mode);
अटल bool eq_mode (काष्ठा termios स्थिर *mode1,
                     काष्ठा termios स्थिर *mode2);
अटल bool speed_matches (काष्ठा termios स्थिर *mode1,
                           काष्ठा termios स्थिर *mode2);
अटल uintmax_t integer_arg (अक्षर स्थिर *s, uintmax_t max);
अटल काष्ठा speed_desc string_to_baud (अक्षर स्थिर *arg);
अटल tcflag_t *mode_type_flag (क्रमागत mode_type type, काष्ठा termios *mode);
अटल व्योम display_all (काष्ठा termios *mode, अक्षर स्थिर *device_name);
अटल व्योम display_changed (काष्ठा termios *mode);
अटल व्योम display_recoverable (काष्ठा termios *mode);
अटल व्योम display_settings (क्रमागत output_type output_type,
                              काष्ठा termios *mode,
                              अक्षर स्थिर *device_name);
अटल व्योम check_speed (काष्ठा termios *mode);
अटल व्योम display_speed (काष्ठा termios *mode, bool fancy);
अटल व्योम display_window_आकार (bool fancy, अक्षर स्थिर *device_name);
अटल व्योम sane_mode (काष्ठा termios *mode);
अटल व्योम set_control_char (काष्ठा control_info स्थिर *info,
                              अक्षर स्थिर *arg,
                              काष्ठा termios *mode);
अटल व्योम set_speed (क्रमागत speed_setting type, अक्षर स्थिर *arg,
                       काष्ठा speed_desc स्थिर speed, काष्ठा termios *mode);
अटल व्योम set_window_आकार (पूर्णांक rows, पूर्णांक cols, अक्षर स्थिर *device_name);

/* The width of the screen, for output wrapping. */
अटल पूर्णांक max_col;

/* Current position, to know when to wrap. */
अटल पूर्णांक current_col;

/* Default "drain" mode for tcsetattr.  */
अटल पूर्णांक tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
अटल bool dev_debug;

/* Record last speed set for correlation.  */
अटल काष्ठा speed_desc last_ibaud =
  { .baud = (speed_t) -1, .valid = false };
अटल काष्ठा speed_desc last_obaud =
  { .baud = (speed_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  DEV_DEBUG_OPTION = अक्षर_उच्च + 1,
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"all", no_argument, शून्य, 'a'},
  {"save", no_argument, शून्य, 'g'},
  {"file", required_argument, शून्य, 'F'},
  {"-debug", no_argument, शून्य, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((म_लिखो, 1, 2))
अटल व्योम
wrapf (अक्षर स्थिर *message,...)
{
  बहु_सूची args;
  अक्षर *buf;
  पूर्णांक buflen;

  बहु_शुरू (args, message);
  buflen = vasprintf (&buf, message, args);
  बहु_पूर्ण (args);

  अगर (buflen < 0)
    xalloc_die ();

  अगर (0 < current_col)
    {
      अगर (max_col - current_col <= buflen)
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

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
  -F, --file=DEVICE  खोलो and use DEVICE instead of standard निवेश\n\
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
#अगर_घोषित VFLUSHO
      ख_माला_दो (_("\
 * discard CHAR  CHAR will toggle discarding of output\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित VDSUSP
      ख_माला_दो (_("\
 * dsusp CHAR    CHAR will send a terminal stop संकेत once निवेश flushed\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   eof CHAR      CHAR will send an end of file (terminate the निवेश)\n\
   eol CHAR      CHAR will end the पंक्ति\n\
"), मानक_निकास);
#अगर_घोषित VEOL2
      ख_माला_दो (_("\
 * eol2 CHAR     alternate CHAR क्रम ending the पंक्ति\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   erase CHAR    CHAR will erase the last character typed\n\
   intr CHAR     CHAR will send an interrupt संकेत\n\
   समाप्त CHAR     CHAR will erase the current पंक्ति\n\
"), मानक_निकास);
#अगर_घोषित VLNEXT
      ख_माला_दो (_("\
 * lnext CHAR    CHAR will enter the अग्र character quoted\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित VSTATUS
      ख_माला_दो (_("\
 * status CHAR   CHAR will send an info संकेत\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   quit CHAR     CHAR will send a quit संकेत\n\
"), मानक_निकास);
#अगर defined CREPRINT || defined VREPRINT
      ख_माला_दो (_("\
 * rprnt CHAR    CHAR will redraw the current पंक्ति\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   start CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop संकेत\n\
"), मानक_निकास);
#अगर_घोषित VSWTCH
      ख_माला_दो (_("\
 * swtch CHAR    CHAR will चयन to a different shell layer\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित VWERASE
      ख_माला_दो (_("\
 * werase CHAR   CHAR will erase the last word typed\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
\n\
Special settings:\n\
   N             set the निवेश and output speeds to N bauds\n\
"), मानक_निकास);
#अगर_घोषित TIOCGWINSZ
      ख_माला_दो (_("\
   cols N        tell the kernel that the terminal has N columns\n\
 * columns N     same as cols N\n\
"), मानक_निकास);
#पूर्ण_अगर
      म_लिखो (_("\
 * [-]drain      रुको क्रम transmission before applying settings (%s by शेष)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      ख_माला_दो (_("\
   ispeed N      set the निवेश speed to N\n\
"), मानक_निकास);
#अगर_घोषित HAVE_C_पंक्ति
      ख_माला_दो (_("\
 * पंक्ति N        use पंक्ति discipline N\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   min N         with -icanon, set N characters minimum क्रम a completed पढ़ो\n\
   ospeed N      set the output speed to N\n\
"), मानक_निकास);
#अगर_घोषित TIOCGWINSZ
      ख_माला_दो (_("\
   rows N        tell the kernel that the terminal has N rows\n\
   आकार          print the संख्या of rows and columns according to the kernel\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   speed         print the terminal speed\n\
   समय N        with -icanon, set पढ़ो timeout of N tenths of a second\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control signals\n\
   [-]cread      allow निवेश to be received\n\
"), मानक_निकास);
#अगर_घोषित CRTSCTS
      ख_माला_दो (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित CDTRDSR
      ख_माला_दो (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   csN           set character आकार to N bits, N in [5..8]\n\
"), मानक_निकास);
      ख_माला_दो (_("\
   [-]cstopb     use two stop bits per character (one with '-')\n\
   [-]hup        send a hangup संकेत when the last process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity bit in output and expect parity bit in निवेश\n\
   [-]parodd     set odd parity (or even parity with '-')\n\
"), मानक_निकास);
#अगर_घोषित CMSPAR
      ख_माला_दो (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
\n\
निवेश_3 settings:\n\
   [-]brkint     breaks cause an interrupt संकेत\n\
   [-]icrnl      translate carriage वापस to newline\n\
   [-]ignbrk     ignore अवरोध characters\n\
   [-]igncr      ignore carriage वापस\n\
   [-]ignpar     ignore characters with parity errors\n\
"), मानक_निकास);
#अगर_घोषित IMAXBEL
      ख_माला_दो (_("\
 * [-]imaxbel    beep and करो not flush a full निवेश बफर on a character\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   [-]inlcr      translate newline to carriage वापस\n\
   [-]inpck      enable निवेश parity checking\n\
   [-]istrip     clear ऊपरी (8th) bit of निवेश characters\n\
"), मानक_निकास);
#अगर_घोषित IUTF8
      ख_माला_दो (_("\
 * [-]iutf8      assume निवेश characters are UTF-8 encoded\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित IUCLC
      ख_माला_दो (_("\
 * [-]iuclc      translate uppercase characters to lowercase\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित IXANY
      ख_माला_दो (_("\
 * [-]ixany      let any character restart output, not only start character\n\
"), मानक_निकास);
#पूर्ण_अगर
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
#अगर_घोषित BSDLY
      ख_माला_दो (_("\
 * bsN           backspace delay style, N in [0..1]\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित CRDLY
      ख_माला_दो (_("\
 * crN           carriage वापस delay style, N in [0..3]\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित FFDLY
      ख_माला_दो (_("\
 * ffN           form feed delay style, N in [0..1]\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित NLDLY
      ख_माला_दो (_("\
 * nlN           newline delay style, N in [0..1]\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित OCRNL
      ख_माला_दो (_("\
 * [-]ocrnl      translate carriage वापस to newline\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित OFDEL
      ख_माला_दो (_("\
 * [-]ofdel      use delete characters क्रम fill instead of NUL characters\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित OFILL
      ख_माला_दो (_("\
 * [-]ofill      use fill (padding) characters instead of timing क्रम delays\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित OLCUC
      ख_माला_दो (_("\
 * [-]olcuc      translate lowercase characters to uppercase\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित ONLCR
      ख_माला_दो (_("\
 * [-]onlcr      translate newline to carriage वापस-newline\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित ONLRET
      ख_माला_दो (_("\
 * [-]onlret     newline performs a carriage वापस\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित ONOCR
      ख_माला_दो (_("\
 * [-]onocr      करो not print carriage returns in the first column\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   [-]opost      postprocess output\n\
"), मानक_निकास);
#अगर defined TABDLY || defined OXTABS
      ख_माला_दो (_("\
 * tabN          horizontal tab delay style, N in [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित VTDLY
      ख_माला_दो (_("\
 * vtN           vertical tab delay style, N in [0..1]\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), मानक_निकास);
#अगर_घोषित ECHOKE
      ख_माला_दो (_("\
 * crtkill       समाप्त all पंक्ति by obeying the echoprt and echoe settings\n\
 * -crtkill      समाप्त all पंक्ति by obeying the echoctl and echok settings\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित ECHOCTL
      ख_माला_दो (_("\
 * [-]ctlecho    echo control characters in hat notation ('^c')\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   [-]echo       echo निवेश characters\n\
"), मानक_निकास);
#अगर_घोषित ECHOCTL
      ख_माला_दो (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a समाप्त character\n\
"), मानक_निकास);
#अगर_घोषित ECHOKE
      ख_माला_दो (_("\
 * [-]echoke     same as [-]crtkill\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   [-]echonl     echo newline even अगर not echoing other characters\n\
"), मानक_निकास);
#अगर_घोषित ECHOPRT
      ख_माला_दो (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर defined EXTPROC || defined TIOCEXT
      ख_माला_दो (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with ऊपरी latency links\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर defined FLUSHO
      ख_माला_दो (_("\
 * [-]flusho     discard output\n\
"), मानक_निकास);
#पूर्ण_अगर
      म_लिखो (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, समाप्त"
#अगर_घोषित VWERASE
    ", werase"
#पूर्ण_अगर
#अगर defined CREPRINT || defined VREPRINT
    ", rprnt"
#पूर्ण_अगर
);
      ख_माला_दो (_("\
   [-]isig       enable interrupt, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after interrupt and quit special characters\n\
"), मानक_निकास);
#अगर_घोषित ECHOPRT
      ख_माला_दो (_("\
 * [-]prterase   same as [-]echoprt\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित TOSTOP
      ख_माला_दो (_("\
 * [-]tostop     stop background jobs that try to ग_लिखो to the terminal\n\
"), मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित XCASE
      ख_माला_दो (_("\
 * [-]xcase      with icanon, escape with '\\' क्रम uppercase characters\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
\n\
Combination settings:\n\
"), मानक_निकास);
#अगर defined XCASE && defined IUCLC && defined OLCUC
      ख_माला_दो (_("\
 * [-]LCASE      same as [-]lcase\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), मानक_निकास);
      ख_माला_दो (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters to their शेष मूल्यावली\n\
   -cooked       same as raw\n\
"), मानक_निकास);
      म_लिखो (_("\
   crt           same as %s\n\
"), "echoe"
#अगर_घोषित ECHOCTL
    " echoctl"
#पूर्ण_अगर
#अगर_घोषित ECHOKE
    " echoke"
#पूर्ण_अगर
);
      म_लिखो (_("\
   dec           same as %s intr ^c erase 0177\n\
                 समाप्त ^u\n\
"), "echoe"
#अगर_घोषित ECHOCTL
    " echoctl"
#पूर्ण_अगर
#अगर_घोषित ECHOKE
    " echoke"
#पूर्ण_अगर
#अगर_घोषित IXANY
    " -ixany"
#पूर्ण_अगर
);
#अगर_घोषित IXANY
      ख_माला_दो (_("\
 * [-]decctlq    same as [-]ixany\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   ek            erase and समाप्त characters to their शेष मूल्यावली\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), मानक_निकास);
#अगर defined XCASE && defined IUCLC && defined OLCUC
      ख_माला_दो (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), मानक_निकास);
#पूर्ण_अगर
      ख_माला_दो (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), मानक_निकास);
      म_लिखो (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#अगर_घोषित ONLCR
   " -onlcr"
#पूर्ण_अगर
  , "icrnl -inlcr -igncr"
#अगर_घोषित ONLCR
   " onlcr"
#पूर्ण_अगर
#अगर_घोषित OCRNL
   " -ocrnl"
#पूर्ण_अगर
#अगर_घोषित ONLRET
   " -onlret"
#पूर्ण_अगर
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
#अगर_घोषित IUCLC
   " -iuclc"
#पूर्ण_अगर
#अगर_घोषित IXANY
   " -ixany"
#पूर्ण_अगर
#अगर_घोषित IMAXBEL
   " -imaxbel"
#पूर्ण_अगर
#अगर_घोषित XCASE
   " -xcase"
#पूर्ण_अगर
);
      म_लिखो (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters to their शेष मूल्यावली\n\
"),
   "-ixoff"
#अगर_घोषित IUTF8
   " -iutf8"
#पूर्ण_अगर
#अगर_घोषित IUCLC
   " -iuclc"
#पूर्ण_अगर
#अगर_घोषित IXANY
   " -ixany"
#पूर्ण_अगर
#अगर_घोषित IMAXBEL
   " imaxbel"
#पूर्ण_अगर
#अगर_घोषित XCASE
   " -xcase"
#पूर्ण_अगर
#अगर_घोषित OLCUC
   " -olcuc"
#पूर्ण_अगर
#अगर_घोषित OCRNL
   " -ocrnl"
#पूर्ण_अगर

 , "opost"
#अगर_घोषित OFILL
   " -ofill"
#पूर्ण_अगर
#अगर_घोषित ONLCR
   " onlcr"
#पूर्ण_अगर
#अगर_घोषित ONOCR
   " -onocr"
#पूर्ण_अगर
#अगर_घोषित ONLRET
   " -onlret"
#पूर्ण_अगर
#अगर_घोषित NLDLY
   " nl0"
#पूर्ण_अगर
#अगर_घोषित CRDLY
   " cr0"
#पूर्ण_अगर
#अगर_घोषित TAB0
   " tab0"
#पूर्ण_अगर
#अगर_घोषित BSDLY
   " bs0"
#पूर्ण_अगर
#अगर_घोषित VTDLY
   " vt0"
#पूर्ण_अगर
#अगर_घोषित FFDLY
   " ff0"
#पूर्ण_अगर

 , "isig"
#अगर_घोषित TOSTOP
   " -tostop"
#पूर्ण_अगर
#अगर_घोषित OFDEL
   " -ofdel"
#पूर्ण_अगर
#अगर_घोषित ECHOPRT
   " -echoprt"
#पूर्ण_अगर
#अगर_घोषित ECHOCTL
   " echoctl"
#पूर्ण_अगर
#अगर_घोषित ECHOKE
   " echoke"
#पूर्ण_अगर
#अगर_घोषित EXTPROC
   " -extproc"
#पूर्ण_अगर
#अगर_घोषित FLUSHO
   " -flusho"
#पूर्ण_अगर
);
      ख_माला_दो (_("\
\n\
Handle the tty पंक्ति connected to standard निवेश.  Without arguments,\n\
prints baud rate, पंक्ति discipline, and deviations from stty sane.  In\n\
settings, CHAR is taken literally, or coded as in ^c, 0x37, 0177 or\n\
127; special मूल्यावली ^- or undef used to disable special characters.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

अटल व्योम
apply_settings (bool checking, अक्षर स्थिर *device_name,
                अक्षर * स्थिर *settings, पूर्णांक n_settings,
                काष्ठा termios *mode, bool *require_set_attr)
{
#घोषणा check_argument(arg)						\
  करो									\
    {									\
      अगर (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (निकास_त्रुटि);						\
        }								\
    }									\
  जबतक (0)

  क्रम (पूर्णांक k = 1; k < n_settings; k++)
    {
      अक्षर स्थिर *arg = settings[k];
      bool match_found = false;
      bool not_set_attr = false;
      bool reversed = false;

      अगर (! arg)
        जारी;

      अगर (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      अगर (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          जारी;
        }
      क्रम (पूर्णांक i = 0; mode_info[i].name != शून्य; ++i)
        {
          अगर (streq (arg, mode_info[i].name))
            {
              अगर ((mode_info[i].flags & NO_SETATTR) == 0)
                {
                  match_found = set_mode (&mode_info[i], reversed, mode);
                  *require_set_attr = true;
                }
              अन्यथा
                match_found = not_set_attr = true;
              अवरोध;
            }
        }
      अगर (!match_found && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (निकास_त्रुटि);
        }
      अगर (!match_found)
        {
          क्रम (पूर्णांक i = 0; control_info[i].name != शून्य; ++i)
            {
              अगर (streq (arg, control_info[i].name))
                {
                  check_argument (arg);
                  match_found = true;
                  ++k;
                  set_control_char (&control_info[i], settings[k], mode);
                  *require_set_attr = true;
                  अवरोध;
                }
            }
        }
      अगर (!match_found || not_set_attr)
        {
          अगर (streq (arg, "ispeed"))
            {
              check_argument (arg);
              ++k;
              काष्ठा speed_desc स्थिर ispeed = string_to_baud (settings[k]);
              अगर (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (निकास_त्रुटि);
                }
              set_speed (निवेश_speed, settings[k], ispeed, mode);
              अगर (checking)
                जारी;
              *require_set_attr = true;
            }
          अन्यथा अगर (streq (arg, "ospeed"))
            {
              check_argument (arg);
              ++k;
              काष्ठा speed_desc स्थिर ospeed = string_to_baud (settings[k]);
              अगर (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (निकास_त्रुटि);
                }
              set_speed (output_speed, settings[k], ospeed, mode);
              अगर (checking)
                जारी;
              *require_set_attr = true;
            }
#अगर_घोषित TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          अन्यथा अगर (streq (arg, "extproc"))
            {
              पूर्णांक val = ! reversed;

              अगर (checking)
                जारी;

              अगर (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (निकास_त्रुटि, त्रुटि_सं, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#पूर्ण_अगर
#अगर_घोषित TIOCGWINSZ
          अन्यथा अगर (streq (arg, "rows"))
            {
              check_argument (arg);
              ++k;
              अगर (checking)
                जारी;
              set_window_आकार (integer_arg (settings[k], पूर्णांक_उच्च), -1,
                               device_name);
            }
          अन्यथा अगर (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_argument (arg);
              ++k;
              अगर (checking)
                जारी;
              set_window_आकार (-1, integer_arg (settings[k], पूर्णांक_उच्च),
                               device_name);
            }
          अन्यथा अगर (streq (arg, "size"))
            {
              अगर (checking)
                जारी;
              max_col = screen_columns ();
              current_col = 0;
              display_window_आकार (false, device_name);
            }
#पूर्ण_अगर
#अगर_घोषित HAVE_C_पंक्ति
          अन्यथा अगर (streq (arg, "line"))
            {
              check_argument (arg);
              ++k;
              uintmax_t मूल्य = integer_arg (settings[k], UINTMAX_MAX);
              अगर (ckd_add (&mode->c_पंक्ति, मूल्य, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_set_attr = true;
            }
#पूर्ण_अगर
          अन्यथा अगर (streq (arg, "speed"))
            {
              अगर (checking)
                जारी;
              max_col = screen_columns ();
              display_speed (mode, false);
            }
          अन्यथा
            {
              काष्ठा speed_desc स्थिर speed = string_to_baud (arg);
              अगर (speed.valid)
                {
                  set_speed (both_speeds, arg, speed, mode);
                  अगर (checking)
                    जारी;
                  *require_set_attr = true;
                }
              अन्यथा
                {
                  अगर (! recover_mode (arg, mode))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (निकास_त्रुटि);
                    }
                  *require_set_attr = true;
                }
            }
        }
    }

  अगर (checking)
    check_speed (mode);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  अटल काष्ठा termios mode;

  क्रमागत output_type output_type;
  पूर्णांक optc;
  पूर्णांक argi = 0;
  पूर्णांक opti = 1;
  bool require_set_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  अक्षर *file_name = शून्य;
  अक्षर स्थिर *device_name;

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

  जबतक ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, शून्य))
         != -1)
    {
      चयन (optc)
        {
        हाल 'a':
          verbose_output = true;
          output_type = all;
          अवरोध;

        हाल 'g':
          recoverable_output = true;
          output_type = recoverable;
          अवरोध;

        हाल 'F':
          अगर (file_name)
            error (निकास_त्रुटि, 0, _("only one device may be specified"));
          file_name = optarg;
          अवरोध;

        हाल DEV_DEBUG_OPTION:
          dev_debug = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          अगर (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          अवरोध;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      जबतक (opti < optind)
        argv[argi + opti++] = शून्य;
    }

  /* Specifying both -a and -g gets an error.  */
  अगर (verbose_output && recoverable_output)
    error (निकास_त्रुटि, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  अगर (!noargs && (verbose_output || recoverable_output))
    error (निकास_त्रुटि, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  अगर (!noargs && !verbose_output && !recoverable_output)
    {
      अटल काष्ठा termios check_mode;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_mode, &require_set_attr);
    }

  अगर (file_name)
    {
      पूर्णांक fdflags;
      अगर (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
      अगर ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  अगर (tcgetattr (STDIN_FILENO, &mode))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));

  अगर (verbose_output || recoverable_output || noargs)
    {
      max_col = screen_columns ();
      current_col = 0;
      display_settings (output_type, &mode, device_name);
      वापस निकास_सफल;
    }

  require_set_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &mode, &require_set_attr);

  अगर (require_set_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      अटल काष्ठा termios new_mode;

      अगर (tcsetattr (STDIN_FILENO, tcsetattr_options, &mode))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      अगर (tcgetattr (STDIN_FILENO, &new_mode))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));

      अगर (! eq_mode (&mode, &new_mode))
        {
          अगर (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              क्रम (अचिन्हित पूर्णांक i = 0; i < माप (new_mode); i++)
                {
                  अचिन्हित पूर्णांक newc = *(((अचिन्हित अक्षर *) &new_mode) + i);
                  अचिन्हित पूर्णांक oldc = *(((अचिन्हित अक्षर *) &mode) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (निकास_त्रुटि, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  वापस निकास_सफल;
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
अटल bool
eq_mode (काष्ठा termios स्थिर *mode1, काष्ठा termios स्थिर *mode2)
{
  अगर (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#अगर_घोषित HAVE_C_पंक्ति
      || mode1->c_पंक्ति != mode2->c_पंक्ति
#पूर्ण_अगर
      || !memeq (mode1->c_cc, mode2->c_cc, माप (mode1->c_cc)))
    वापस false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_mask = ~(tcflag_t) 0;
#अगर_घोषित CBAUD
  cflag_mask &= ~(tcflag_t) CBAUD;
#पूर्ण_अगर
#अगर_घोषित CIBAUD
  cflag_mask &= ~(tcflag_t) CIBAUD;
#पूर्ण_अगर

  अगर ((mode1->c_cflag ^ mode2->c_cflag) & cflag_mask)
    वापस false;

  वापस speed_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

अटल bool
set_mode (काष्ठा mode_info स्थिर *info, bool reversed, काष्ठा termios *mode)
{
  tcflag_t *bitsp;

  अगर (reversed && (info->flags & REV) == 0)
    वापस false;

  bitsp = mode_type_flag (info->type, mode);

  अगर (bitsp == शून्य)
    {
      /* Combination mode. */
      अगर (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          अगर (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          अन्यथा
            mode->c_cflag = (mode->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      अन्यथा अगर (streq (info->name, "oddp"))
        {
          अगर (reversed)
            mode->c_cflag = (mode->c_cflag & ~PARENB & ~CSIZE) | CS8;
          अन्यथा
            mode->c_cflag = (mode->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      अन्यथा अगर (streq (info->name, "nl"))
        {
          अगर (reversed)
            {
              mode->c_iflag = (mode->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              mode->c_oflag = (mode->c_oflag
#अगर_घोषित ONLCR
                               | ONLCR
#पूर्ण_अगर
                )
#अगर_घोषित OCRNL
                & ~OCRNL
#पूर्ण_अगर
#अगर_घोषित ONLRET
                & ~ONLRET
#पूर्ण_अगर
                ;
            }
          अन्यथा
            {
              mode->c_iflag = mode->c_iflag & ~ICRNL;
#अगर_घोषित ONLCR
              mode->c_oflag = mode->c_oflag & ~ONLCR;
#पूर्ण_अगर
            }
        }
      अन्यथा अगर (streq (info->name, "ek"))
        {
          mode->c_cc[VERASE] = CERASE;
          mode->c_cc[VKILL] = CKILL;
        }
      अन्यथा अगर (streq (info->name, "sane"))
        sane_mode (mode);
      अन्यथा अगर (streq (info->name, "cbreak"))
        {
          अगर (reversed)
            mode->c_lflag |= ICANON;
          अन्यथा
            mode->c_lflag &= ~ICANON;
        }
      अन्यथा अगर (streq (info->name, "pass8"))
        {
          अगर (reversed)
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
      अन्यथा अगर (streq (info->name, "litout"))
        {
          अगर (reversed)
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
      अन्यथा अगर (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          अगर ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              mode->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              mode->c_oflag |= OPOST;
              mode->c_lflag |= ISIG | ICANON;
#अगर VMIN == VEOF
              mode->c_cc[VEOF] = CEOF;
#पूर्ण_अगर
#अगर VTIME == VEOL
              mode->c_cc[VEOL] = CEOL;
#पूर्ण_अगर
            }
          अन्यथा
            {
              /* Raw mode. */
              mode->c_iflag = 0;
              mode->c_oflag &= ~OPOST;
              mode->c_lflag &= ~(ISIG | ICANON
#अगर_घोषित XCASE
                                 | XCASE
#पूर्ण_अगर
                );
              mode->c_cc[VMIN] = 1;
              mode->c_cc[VTIME] = 0;
            }
        }
#अगर_घोषित IXANY
      अन्यथा अगर (streq (info->name, "decctlq"))
        {
          अगर (reversed)
            mode->c_iflag |= IXANY;
          अन्यथा
            mode->c_iflag &= ~IXANY;
        }
#पूर्ण_अगर
#अगर_घोषित TABDLY
      अन्यथा अगर (streq (info->name, "tabs"))
        {
          अगर (reversed)
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB3;
          अन्यथा
            mode->c_oflag = (mode->c_oflag & ~TABDLY) | TAB0;
        }
#अन्यथा
# ifdef OXTABS
      अन्यथा अगर (streq (info->name, "tabs"))
        {
          अगर (reversed)
            mode->c_oflag = mode->c_oflag | OXTABS;
          अन्यथा
            mode->c_oflag = mode->c_oflag & ~OXTABS;
        }
# endif
#पूर्ण_अगर
#अगर defined XCASE && defined IUCLC && defined OLCUC
      अन्यथा अगर (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          अगर (reversed)
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
#पूर्ण_अगर
      अन्यथा अगर (streq (info->name, "crt"))
        mode->c_lflag |= ECHOE
#अगर_घोषित ECHOCTL
          | ECHOCTL
#पूर्ण_अगर
#अगर_घोषित ECHOKE
          | ECHOKE
#पूर्ण_अगर
          ;
      अन्यथा अगर (streq (info->name, "dec"))
        {
          mode->c_cc[VINTR] = 3;	/* ^C */
          mode->c_cc[VERASE] = 127;	/* DEL */
          mode->c_cc[VKILL] = 21;	/* ^U */
          mode->c_lflag |= ECHOE
#अगर_घोषित ECHOCTL
            | ECHOCTL
#पूर्ण_अगर
#अगर_घोषित ECHOKE
            | ECHOKE
#पूर्ण_अगर
            ;
#अगर_घोषित IXANY
          mode->c_iflag &= ~IXANY;
#पूर्ण_अगर
        }
    }
  अन्यथा अगर (reversed)
    *bitsp = *bitsp & ~info->mask & ~info->bits;
  अन्यथा
    *bitsp = (*bitsp & ~info->mask) | info->bits;

  वापस true;
}

अटल व्योम
set_control_char (काष्ठा control_info स्थिर *info, अक्षर स्थिर *arg,
                  काष्ठा termios *mode)
{
  अचिन्हित दीर्घ पूर्णांक मूल्य;

  अगर (streq (info->name, "min") || streq (info->name, "time"))
    मूल्य = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  अन्यथा अगर (arg[0] == '\0' || arg[1] == '\0')
    मूल्य = to_uchar (arg[0]);
  अन्यथा अगर (streq (arg, "^-") || streq (arg, "undef"))
    मूल्य = _POSIX_VDISABLE;
  अन्यथा अगर (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      अगर (arg[1] == '?')
        मूल्य = 127;
      अन्यथा
        मूल्य = to_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  अन्यथा
    मूल्य = integer_arg (arg, TYPE_MAXIMUM (cc_t));
  mode->c_cc[info->offset] = मूल्य;
}

अटल व्योम
set_speed (क्रमागत speed_setting type, अक्षर स्थिर *arg,
           काष्ठा speed_desc स्थिर speed, काष्ठा termios *mode)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (speed.valid);

  अगर (type == निवेश_speed || type == both_speeds)
    {
      last_ibaud = speed;
      अगर (cfsetispeed (mode, speed.baud))
        error (निकास_त्रुटि, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  अगर (type == output_speed || type == both_speeds)
    {
      last_obaud = speed;
      अगर (cfsetospeed (mode, speed.baud))
        error (निकास_त्रुटि, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#अगर_घोषित TIOCGWINSZ

अटल पूर्णांक
get_win_आकार (पूर्णांक fd, काष्ठा winsize *win)
{
  पूर्णांक err = ioctl (fd, TIOCGWINSZ, (अक्षर *) win);
  वापस err;
}

अटल व्योम
set_window_आकार (पूर्णांक rows, पूर्णांक cols, अक्षर स्थिर *device_name)
{
  काष्ठा winsize win;

  अगर (get_win_आकार (STDIN_FILENO, &win))
    {
      अगर (त्रुटि_सं != EINVAL)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
      स_रखो (&win, 0, माप (win));
    }

  अगर (rows >= 0)
    win.ws_row = rows;
  अगर (cols >= 0)
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

  अगर (win.ws_row == 0 || win.ws_col == 0)
    {
      काष्ठा ttysize ttysz;

      ttysz.ts_पंक्तियाँ = win.ws_row;
      ttysz.ts_cols = win.ws_col;

      win.ws_row = 1;
      win.ws_col = 1;

      अगर (ioctl (STDIN_FILENO, TIOCSWINSZ, (अक्षर *) &win))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));

      अगर (ioctl (STDIN_FILENO, TIOCSSIZE, (अक्षर *) &ttysz))
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
      वापस;
    }
# endif

  अगर (ioctl (STDIN_FILENO, TIOCSWINSZ, (अक्षर *) &win))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
}

अटल व्योम
display_window_आकार (bool fancy, अक्षर स्थिर *device_name)
{
  काष्ठा winsize win;

  अगर (get_win_आकार (STDIN_FILENO, &win))
    {
      अगर (त्रुटि_सं != EINVAL)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (device_name));
      अगर (!fancy)
        error (निकास_त्रुटि, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  अन्यथा
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_row, win.ws_col);
      अगर (!fancy)
        current_col = 0;
    }
}
#पूर्ण_अगर

अटल पूर्णांक
screen_columns (व्योम)
{
#अगर_घोषित TIOCGWINSZ
  काष्ठा winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  अगर (get_win_आकार (STDOUT_FILENO, &win) == 0 && 0 < win.ws_col)
    वापस win.ws_col;
#पूर्ण_अगर
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    अक्षर *col_string = दो_पर्या ("COLUMNS");
    दीर्घ पूर्णांक n_columns;
    अगर (!(col_string != शून्य
          && xstrtol (col_string, शून्य, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= पूर्णांक_उच्च))
      n_columns = 80;
    वापस n_columns;
  }
}

ATTRIBUTE_PURE
अटल tcflag_t *
mode_type_flag (क्रमागत mode_type type, काष्ठा termios *mode)
{
  चयन (type)
    {
    हाल control:
      वापस &mode->c_cflag;

    हाल निवेश:
      वापस &mode->c_iflag;

    हाल output:
      वापस &mode->c_oflag;

    हाल local:
      वापस &mode->c_lflag;

    हाल combination:
      वापस शून्य;

    शेष:
      unreachable ();
    }
}

अटल व्योम
display_settings (क्रमागत output_type output_type, काष्ठा termios *mode,
                  अक्षर स्थिर *device_name)
{
  चयन (output_type)
    {
    हाल changed:
      display_changed (mode);
      अवरोध;

    हाल all:
      display_all (mode, device_name);
      अवरोध;

    हाल recoverable:
      display_recoverable (mode);
      अवरोध;
    }
}

अटल व्योम
display_changed (काष्ठा termios *mode)
{
  bool empty_पंक्ति;
  tcflag_t *bitsp;
  अचिन्हित दीर्घ mask;
  क्रमागत mode_type पूर्व_type = control;

  display_speed (mode, true);
#अगर_घोषित HAVE_C_पंक्ति
  wrapf ("line = %d;", mode->c_पंक्ति);
#पूर्ण_अगर
  अक्षर_दो ('\n');
  current_col = 0;

  empty_पंक्ति = true;
  क्रम (पूर्णांक i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      अगर (mode->c_cc[control_info[i].offset] == control_info[i].saneval)
        जारी;

#अगर_घोषित VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      अगर (streq (control_info[i].name, "flush"))
        जारी;
#पूर्ण_अगर
      /* If swtch is the same as susp, don't print both.  */
#अगर VSWTCH == VSUSP
      अगर (streq (control_info[i].name, "swtch"))
        जारी;
#पूर्ण_अगर
      /* If eof uses the same slot as min, only print whichever applies.  */
#अगर VEOF == VMIN
      अगर ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        जारी;
#पूर्ण_अगर

      empty_पंक्ति = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
  अगर ((mode->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (अचिन्हित दीर्घ पूर्णांक) mode->c_cc[VMIN],
             (अचिन्हित दीर्घ पूर्णांक) mode->c_cc[VTIME]);
    }
  अन्यथा अगर (!empty_पंक्ति)
    अक्षर_दो ('\n');
  current_col = 0;

  empty_पंक्ति = true;
  क्रम (पूर्णांक i = 0; mode_info[i].name != शून्य; ++i)
    {
      अगर (mode_info[i].flags & OMIT)
        जारी;
      अगर (mode_info[i].type != पूर्व_type)
        {
          अगर (!empty_पंक्ति)
            {
              अक्षर_दो ('\n');
              current_col = 0;
              empty_पंक्ति = true;
            }
          पूर्व_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      अगर ((*bitsp & mask) == mode_info[i].bits)
        {
          अगर (mode_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", mode_info[i].name);
              empty_पंक्ति = false;
            }
        }
      अन्यथा अगर ((mode_info[i].flags & (SANE_SET | REV)) == (SANE_SET | REV))
        {
          wrapf ("-%s", mode_info[i].name);
          empty_पंक्ति = false;
        }
    }
  अगर (!empty_पंक्ति)
    अक्षर_दो ('\n');
  current_col = 0;
}

अटल व्योम
display_all (काष्ठा termios *mode, अक्षर स्थिर *device_name)
{
  tcflag_t *bitsp;
  अचिन्हित दीर्घ mask;
  क्रमागत mode_type पूर्व_type = control;

  display_speed (mode, true);
#अगर_घोषित TIOCGWINSZ
  display_window_आकार (true, device_name);
#पूर्ण_अगर
#अगर_घोषित HAVE_C_पंक्ति
  wrapf ("line = %d;", mode->c_पंक्ति);
#पूर्ण_अगर
  अक्षर_दो ('\n');
  current_col = 0;

  क्रम (पूर्णांक i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#अगर_घोषित VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      अगर (streq (control_info[i].name, "flush"))
        जारी;
#पूर्ण_अगर
      /* If swtch is the same as susp, don't print both.  */
#अगर VSWTCH == VSUSP
      अगर (streq (control_info[i].name, "swtch"))
        जारी;
#पूर्ण_अगर
      /* If eof uses the same slot as min, only print whichever applies.  */
#अगर VEOF == VMIN
      अगर ((mode->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        जारी;
#पूर्ण_अगर
      wrapf ("%s = %s;", control_info[i].name,
             visible (mode->c_cc[control_info[i].offset]));
    }
#अगर VEOF == VMIN
  अगर ((mode->c_lflag & ICANON) == 0)
#पूर्ण_अगर
    wrapf ("min = %lu; time = %lu;",
           (अचिन्हित दीर्घ पूर्णांक) mode->c_cc[VMIN],
           (अचिन्हित दीर्घ पूर्णांक) mode->c_cc[VTIME]);
  अगर (current_col != 0)
    अक्षर_दो ('\n');
  current_col = 0;

  क्रम (पूर्णांक i = 0; mode_info[i].name != शून्य; ++i)
    {
      अगर (mode_info[i].flags & OMIT)
        जारी;
      अगर (mode_info[i].type != पूर्व_type)
        {
          अक्षर_दो ('\n');
          current_col = 0;
          पूर्व_type = mode_info[i].type;
        }

      bitsp = mode_type_flag (mode_info[i].type, mode);
      mask = mode_info[i].mask ? mode_info[i].mask : mode_info[i].bits;
      अगर ((*bitsp & mask) == mode_info[i].bits)
        wrapf ("%s", mode_info[i].name);
      अन्यथा अगर (mode_info[i].flags & REV)
        wrapf ("-%s", mode_info[i].name);
    }
  अक्षर_दो ('\n');
  current_col = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

अटल व्योम
check_speed (काष्ठा termios *mode)
{
  अगर (last_ibaud.valid && last_obaud.valid)
    {
      अगर (cfgetispeed (mode) != last_ibaud.baud
          || cfgetospeed (mode) != last_obaud.baud)
        error (निकास_त्रुटि, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_to_मूल्य (last_ibaud.baud),
               baud_to_मूल्य (last_obaud.baud));
    }
}

अटल व्योम
display_speed (काष्ठा termios *mode, bool fancy)
{
  अगर (cfgetispeed (mode) == 0 || cfgetispeed (mode) == cfgetospeed (mode))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_to_मूल्य (cfgetospeed (mode)));
  अन्यथा
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_to_मूल्य (cfgetispeed (mode)),
           baud_to_मूल्य (cfgetospeed (mode)));
  अगर (!fancy)
    current_col = 0;
}

अटल व्योम
display_recoverable (काष्ठा termios *mode)
{
  म_लिखो ("%lx:%lx:%lx:%lx",
          (अचिन्हित दीर्घ पूर्णांक) mode->c_iflag,
          (अचिन्हित दीर्घ पूर्णांक) mode->c_oflag,
          (अचिन्हित दीर्घ पूर्णांक) mode->c_cflag,
          (अचिन्हित दीर्घ पूर्णांक) mode->c_lflag);
  क्रम (माप_प्रकार i = 0; i < NCCS; ++i)
    म_लिखो (":%lx", (अचिन्हित दीर्घ पूर्णांक) mode->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#अगर_घोषित CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (mode->c_cflag & CBAUD) == BOTHER;
#  अगर defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((mode->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#पूर्ण_अगर
  अगर (separate_speeds)
    म_लिखो (":%lx:%lx", (अचिन्हित दीर्घ पूर्णांक) cfgetispeed (mode),
            (अचिन्हित दीर्घ पूर्णांक) cfgetospeed (mode));

  अक्षर_दो ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#घोषणा STRTOUL_TYPE(TYPE)                                               \
  अटल पूर्णांक                                                             \
  strtoul_##TYPE (अक्षर स्थिर *s, पूर्णांक base, अक्षर **p, TYPE *परिणाम,       \
                  अक्षर delim)                                            \
  {                                                                      \
    अचिन्हित दीर्घ ul;                                                    \
    त्रुटि_सं = 0;                                                           \
    ul = म_से_अदीर्घ (s, p, base);                                           \
    अगर (त्रुटि_सं || **p != delim || *p == s || (TYPE) ul != ul)             \
      वापस -1;                                                         \
    *परिणाम = ul;                                                        \
    वापस 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (speed_t)
#अघोषित STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#समावेश "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
अटल bool
recover_mode (अक्षर स्थिर *arg, काष्ठा termios *mode)
{
  tcflag_t flag[4];
  अक्षर स्थिर *s = arg;
  क्रम (माप_प्रकार i = 0; i < 4; i++)
    {
      अक्षर *p;
      अगर (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        वापस false;
      s = p + 1;
    }
  mode->c_iflag = flag[0];
  mode->c_oflag = flag[1];
  mode->c_cflag = flag[2];
  mode->c_lflag = flag[3];

  क्रम (माप_प्रकार i = 0; i < NCCS; ++i)
    {
      अक्षर *p;
      अक्षर delim = i < NCCS - 1 || म_अक्षर (s, ':') ? ':' : '\0';
      अगर (strtoul_cc_t (s, 16, &p, mode->c_cc + i, delim) != 0)
        वापस false;
      /* Accept the old format, which has no separate speed fields.  */
      अगर (! *p)
        {
#अगर_घोषित CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_SPEED_T_SANE
          काष्ठा speed_desc ospeed = cbaud_to_speed (mode->c_cflag & CBAUD);
          अगर (ospeed.valid && cfsetospeed (mode, ospeed.baud) != 0)
            वापस false;
#  अगर defined CIBAUD && defined IBSHIFT
          काष्ठा speed_desc ispeed =
            cbaud_to_speed ((mode->c_cflag & CIBAUD) >> IBSHIFT);
          अगर (ispeed.valid && cfsetispeed (mode, ispeed.baud) != 0)
            वापस false;
#  endif
# endif
          last_ibaud.baud = cfgetispeed (mode);
          last_obaud.baud = cfgetospeed (mode);
#पूर्ण_अगर
          वापस true;
        }
      s = p + 1;
    }

  speed_t speed[2];
  क्रम (माप_प्रकार i = 0; i < 2; i++)
    {
      अक्षर *p;
      अक्षर delim = i == 0 ? ':' : '\0';
      अगर (strtoul_speed_t (s, 16, &p, speed + i, delim) != 0)
        वापस false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  अगर (cfsetospeed (mode, speed[1]) != 0
      || cfsetispeed (mode, speed[0]) != 0)
    वापस false;

  last_ibaud.baud = speed[0] == 0 ? cfgetispeed (mode) : speed[0];
  last_obaud.baud = speed[1];
  वापस true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
अटल bool
speed_matches (काष्ठा termios स्थिर *mode1, काष्ठा termios स्थिर *mode2)
{
  speed_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  speed_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  अगर (speed1[1] == 0)
    speed1[1] = speed1[0];
  अगर (speed2[1] == 0)
    speed2[1] = speed2[0];

  अचिन्हित पूर्णांक bits = 2;  /* Start and stop bits.  */
  bits += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  bits += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  चयन (mode1->c_cflag & CSIZE)
    {
      हाल CS5: bits += 5; अवरोध;
      हाल CS6: bits += 6; अवरोध;
      हाल CS7: bits += 7; अवरोध;
      शेष: bits += 8; अवरोध;
    }

  क्रम (पूर्णांक i = 0; i < 2; i++)
    {
      अचिन्हित दीर्घ पूर्णांक value1;
      अचिन्हित दीर्घ पूर्णांक value2;
      अचिन्हित दीर्घ पूर्णांक delta;

      अगर (speed1[i] == speed2[i])
        जारी;

      value1 = baud_to_मूल्य (speed1[i]);
      value2 = baud_to_मूल्य (speed2[i]);

#अगर_अघोषित TERMIOS_SPEED_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      अगर (value1 == अच_दीर्घ_उच्च || value2 == अच_दीर्घ_उच्च)
        वापस false;
#पूर्ण_अगर

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      अगर (delta > value1 / (bits * 4))
        वापस false;
    }

  वापस true;
}

ATTRIBUTE_PURE
अटल काष्ठा speed_desc
string_to_baud (अक्षर स्थिर *arg)
{
  अक्षर *ep;
  अचिन्हित दीर्घ मूल्य;
  अचिन्हित अक्षर c;

  /* Explicitly disallow negative numbers.  */
  जबतक (c_isspace (*arg))
    arg++;
  अगर (*arg == '-')
    जाओ invalid;

  /* Empty does not mean 0.  */
  अगर (! *arg)
    जाओ invalid;

  मूल्य = म_से_अदीर्घ (arg, &ep, 10);

  c = *ep++;
  अगर (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      अगर (c)
        {
          अचिन्हित अक्षर d = c - '0';
          अगर (d > 5)
            मूल्य++;
          अन्यथा अगर (d == 5)
            {
              जबतक ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              अगर (c)
                मूल्य++;                /* Nonzero, round up */
              अन्यथा
                मूल्य += (मूल्य & 1);   /* Exactly in the middle, round even */
            }

          जबतक (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          अगर (c)
            जाओ invalid; /* Garbage after otherwise valid number */
        }
    }
  अन्यथा अगर (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      अगर (streq (arg, "exta"))
        वापस (काष्ठा speed_desc) { .baud = B19200, .valid = true };
      अन्यथा अगर (streq (arg, "extb"))
        वापस (काष्ठा speed_desc) { .baud = B38400, .valid = true };
      अन्यथा
        जाओ invalid;
    }

  वापस मूल्य_to_baud (मूल्य);

 invalid:
  वापस (काष्ठा speed_desc) { .baud = (speed_t) -1, .valid = false };
}

अटल व्योम
sane_mode (काष्ठा termios *mode)
{
  tcflag_t *bitsp;

  क्रम (पूर्णांक i = 0; control_info[i].name; ++i)
    {
#अगर VMIN == VEOF
      अगर (streq (control_info[i].name, "min"))
        अवरोध;
#पूर्ण_अगर
      mode->c_cc[control_info[i].offset] = control_info[i].saneval;
    }

  क्रम (पूर्णांक i = 0; mode_info[i].name != शून्य; ++i)
    {
      अगर (mode_info[i].flags & NO_SETATTR)
        जारी;

      अगर (mode_info[i].flags & SANE_SET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~mode_info[i].mask) | mode_info[i].bits;
        }
      अन्यथा अगर (mode_info[i].flags & SANE_UNSET)
        {
          bitsp = mode_type_flag (mode_info[i].type, mode);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~mode_info[i].mask & ~mode_info[i].bits;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

अटल अक्षर स्थिर *
visible (cc_t ch)
{
  अटल अक्षर buf[10];
  अक्षर *bpout = buf;

  अगर (ch == _POSIX_VDISABLE)
    वापस "<undef>";

  अगर (ch >= 32)
    {
      अगर (ch < 127)
        *bpout++ = ch;
      अन्यथा अगर (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      अन्यथा
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          अगर (ch >= 128 + 32)
            {
              अगर (ch < 128 + 127)
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
  वापस (अक्षर स्थिर *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

अटल uintmax_t
integer_arg (अक्षर स्थिर *s, uintmax_t maxval)
{
  वापस xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
