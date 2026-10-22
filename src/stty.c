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

#که_تعریف_شوی TERMIOS_NEEDS_XOPEN_سرچینه
# define XOPEN_سرچینه
#د_که_پای

#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن <termios.h>
#إذا HAVE_STROPTS_H
# include <stropts.h>
#د_که_پای
#ضمن <sys/ioctl.h>

#که_تعریف_شوی WINSIZE_دننه_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#د_که_پای
#که_تعریف_شوی GWINSZ_دننه_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#د_که_پای
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
#تعریف_کړه پروګرام_NAME "stty"

#تعریف_کړه AUTHORS proper_name ("David MacKenzie")

#که_نه_تعریف_شوی _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#د_که_پای

#تعریف_کړه Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#که_نه_تعریف_شوی CINTR
# define CINTR Control ('c')
#د_که_پای
#که_نه_تعریف_شوی CQUIT
# define CQUIT 28
#د_که_پای
#که_نه_تعریف_شوی CERASE
# define CERASE 127
#د_که_پای
#که_نه_تعریف_شوی CKILL
# define CKILL Control ('u')
#د_که_پای
#که_نه_تعریف_شوی CEOF
# define CEOF Control ('d')
#د_که_پای
#که_نه_تعریف_شوی CEOL
# define CEOL _POSIX_VDISABLE
#د_که_پای
#که_نه_تعریف_شوی CSTART
# define CSTART Control ('q')
#د_که_پای
#که_نه_تعریف_شوی CSTOP
# define CSTOP Control ('s')
#د_که_پای
#که_نه_تعریف_شوی CSUSP
# define CSUSP Control ('z')
#د_که_پای
#إذا defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#د_که_پای
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#إذا defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#د_که_پای
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#إذا defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# إذا defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#د_که_پای
#إذا defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#د_که_پای

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#إذا defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#د_که_پای

#إذا defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#د_که_پای
#إذا defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#د_که_پای
#إذا defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#د_که_پای
#إذا defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#د_که_پای
#إذا defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#د_که_پای
#إذا defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#د_که_پای
#إذا defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#د_که_پای
#إذا defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#د_که_پای
#إذا defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#د_که_پای
#إذا defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#د_که_پای
#إذا defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#د_که_پای
#إذا defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#د_که_پای
#إذا defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#د_که_پای

/* Which speeds to set.  */
تعداد چټکتیا_setting
  {
    ننوت_چټکتیا, output_چټکتیا, both_speeds
  };

/* What to output and how.  */
تعداد output_type
  {
    بدل_شوی, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
تعداد ډېرپېښ_type
  {
    control, ننوت, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#تعریف_کړه SANE_سېټ 1		/* Set in 'sane' mode. */
#تعریف_کړه SANE_UNSET 2		/* Unset in 'sane' mode. */
#تعریف_کړه REV 4			/* Can be turned off by prepending '-'. */
#تعریف_کړه OMIT 8			/* Don't display value. */
#تعریف_کړه NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
بنية ډېرپېښ_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    تعداد ډېرپېښ_type type;	/* Which structure element to change. */
    حرف flags;			/* Setting and display options.  */
    بینښه اوږد بټونه_2;		/* Bits to set for this mode.  */
    بینښه اوږد ماسک;		/* Other bits to turn off for this mode.  */
  };

ساكن بنية ډېرپېښ_info ثابت ډېرپېښ_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#که_تعریف_شوی CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#د_که_پای
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_سېټ | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#که_تعریف_شوی CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#د_که_پای
#که_تعریف_شوی CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#د_که_پای

  {"ignbrk", ننوت, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", ننوت, SANE_سېټ | REV, BRKINT, 0},
  {"ignpar", ننوت, REV, IGNPAR, 0},
  {"parmrk", ننوت, REV, PARMRK, 0},
  {"inpck", ننوت, REV, INPCK, 0},
  {"istrip", ننوت, REV, ISTRIP, 0},
  {"inlcr", ننوت, SANE_UNSET | REV, INLCR, 0},
  {"igncr", ننوت, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", ننوت, SANE_سېټ | REV, ICRNL, 0},
  {"ixon", ننوت, REV, IXON, 0},
  {"ixoff", ننوت, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", ننوت, REV | OMIT, IXOFF, 0},
#که_تعریف_شوی IUCLC
  {"iuclc", ننوت, SANE_UNSET | REV, IUCLC, 0},
#د_که_پای
#که_تعریف_شوی IXANY
  {"ixany", ننوت, SANE_UNSET | REV, IXANY, 0},
#د_که_پای
#که_تعریف_شوی IMAXBEL
  {"imaxbel", ننوت, SANE_سېټ | REV, IMAXBEL, 0},
#د_که_پای
#که_تعریف_شوی IUTF8
  {"iutf8", ننوت, SANE_UNSET | REV, IUTF8, 0},
#د_که_پای

  {"opost", output, SANE_سېټ | REV, OPOST, 0},
#که_تعریف_شوی OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#د_که_پای
#که_تعریف_شوی OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#د_که_پای
#که_تعریف_شوی ONLCR
  {"onlcr", output, SANE_سېټ | REV, ONLCR, 0},
#د_که_پای
#که_تعریف_شوی ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#د_که_پای
#که_تعریف_شوی ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#د_که_پای
#که_تعریف_شوی OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#د_که_پای
#که_تعریف_شوی OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#د_که_پای
#که_تعریف_شوی NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_سېټ, NL0, NLDLY},
#د_که_پای
#که_تعریف_شوی CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_سېټ, CR0, CRDLY},
#د_که_پای
#که_تعریف_شوی TABDLY
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
  {"tab0", output, SANE_سېټ, TAB0, TABDLY},
# endif
#وإلا
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#د_که_پای
#که_تعریف_شوی BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_سېټ, BS0, BSDLY},
#د_که_پای
#که_تعریف_شوی VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_سېټ, VT0, VTDLY},
#د_که_پای
#که_تعریف_شوی FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_سېټ, FF0, FFDLY},
#د_که_پای

  {"isig", local, SANE_سېټ | REV, ISIG, 0},
  {"icanon", local, SANE_سېټ | REV, ICANON, 0},
#که_تعریف_شوی IEXTEN
  {"iexten", local, SANE_سېټ | REV, IEXTEN, 0},
#د_که_پای
  {"echo", local, SANE_سېټ | REV, ECHO, 0},
  {"echoe", local, SANE_سېټ | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_سېټ | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#که_تعریف_شوی XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#د_که_پای
#که_تعریف_شوی TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#د_که_پای
#که_تعریف_شوی ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#د_که_پای
#که_تعریف_شوی ECHOCTL
  {"echoctl", local, SANE_سېټ | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#د_که_پای
#که_تعریف_شوی ECHOKE
  {"echoke", local, SANE_سېټ | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#د_که_پای
#که_تعریف_شوی FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#د_که_پای
#إذا defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#وإلاإذا defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#د_که_پای

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
#که_تعریف_شوی IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#د_که_پای
#إذا defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#د_که_پای
#إذا defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#د_که_پای
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {عدم, control, 0, 0, 0}
};

/* Control character settings.  */
بنية control_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t دننه_واټن;		/* Offset in c_cc.  */
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
#که_تعریف_شوی VEOL2
  {"eol2", CEOL2, VEOL2},
#د_که_پای
#که_تعریف_شوی VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#د_که_پای
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#که_تعریف_شوی VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#د_که_پای
#که_تعریف_شوی VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#وإلا
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#د_که_پای
#که_تعریف_شوی VWERASE
  {"werase", CWERASE, VWERASE},
#د_که_پای
#که_تعریف_شوی VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#د_که_پای
#که_تعریف_شوی VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#د_که_پای
#که_تعریف_شوی VSTATUS
  {"status", CSTATUS, VSTATUS},
#د_که_پای

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {عدم, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
بنية چټکتیا_desc
{
  /* The baud rate.  */
  چټکتیا_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ساكن حرف ثابت *visible (cc_t ch);
ساكن بینښه اوږد صحيح baud_منزل_ارزښت (چټکتیا_t چټکتیا);
ساكن bool recover_ډېرپېښ (حرف ثابت *arg, بنية termios *ډېرپېښ_2);
ساكن صحيح screen_columns (فراغ);
ساكن bool سېټ_ډېرپېښ (بنية ډېرپېښ_info ثابت *info, bool reversed,
                      بنية termios *ډېرپېښ_2);
ساكن bool eq_ډېرپېښ (بنية termios ثابت *mode1,
                     بنية termios ثابت *mode2);
ساكن bool چټکتیا_matches (بنية termios ثابت *mode1,
                           بنية termios ثابت *mode2);
ساكن uintmax_t integer_arg (حرف ثابت *s, uintmax_t max);
ساكن بنية چټکتیا_desc string_منزل_baud (حرف ثابت *arg);
ساكن tcflag_t *ډېرپېښ_type_flag (تعداد ډېرپېښ_type type, بنية termios *ډېرپېښ_2);
ساكن فراغ display_all (بنية termios *ډېرپېښ_2, حرف ثابت *device_name);
ساكن فراغ display_بدل_شوی (بنية termios *ډېرپېښ_2);
ساكن فراغ display_recoverable (بنية termios *ډېرپېښ_2);
ساكن فراغ display_settings (تعداد output_type output_type,
                              بنية termios *ډېرپېښ_2,
                              حرف ثابت *device_name);
ساكن فراغ check_چټکتیا (بنية termios *ډېرپېښ_2);
ساكن فراغ display_چټکتیا (بنية termios *ډېرپېښ_2, bool fancy);
ساكن فراغ display_کړکۍ_اندازه (bool fancy, حرف ثابت *device_name);
ساكن فراغ sane_ډېرپېښ (بنية termios *ډېرپېښ_2);
ساكن فراغ سېټ_control_char (بنية control_info ثابت *info,
                              حرف ثابت *arg,
                              بنية termios *ډېرپېښ_2);
ساكن فراغ سېټ_چټکتیا (تعداد چټکتیا_setting type, حرف ثابت *arg,
                       بنية چټکتیا_desc ثابت چټکتیا, بنية termios *ډېرپېښ_2);
ساكن فراغ سېټ_کړکۍ_اندازه (صحيح لیکې, صحيح ستون_شمېر_2, حرف ثابت *device_name);

/* The width of the screen, for output wrapping. */
ساكن صحيح max_ستون;

/* Current position, to know when to wrap. */
ساكن صحيح current_ستون;

/* Default "drain" mode for tcsetattr.  */
ساكن صحيح tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ساكن bool dev_debug;

/* Record last speed set for correlation.  */
ساكن بنية چټکتیا_desc وروستی_ibaud =
  { .baud = (چټکتیا_t) -1, .valid = false };
ساكن بنية چټکتیا_desc وروستی_obaud =
  { .baud = (چټکتیا_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

ساكن بنية option ثابت longopts[] =
{
  {"all", no_دلیل, عدم, 'a'},
  {"save", no_دلیل, عدم, 'g'},
  {"file", required_دلیل, عدم, 'F'},
  {"-debug", no_دلیل, عدم, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((چاپ_کړه, 1, 2))
ساكن فراغ
wrapf (حرف ثابت *پیغام_2,...)
{
  va_list دلیلونه;
  حرف *buf;
  صحيح buflen;

  va_پیل (دلیلونه, پیغام_2);
  buflen = vasprintf (&buf, پیغام_2, دلیلونه);
  va_پای_ټکی (دلیلونه);

  إذا (buflen < 0)
    xalloc_die ();

  إذا (0 < current_ستون)
    {
      إذا (max_ستون - current_ستون <= buflen)
        {
          توری_چاپ_کړه ('\n');
          current_ستون = 0;
        }
      وإلا
        {
          توری_چاپ_کړه (' ');
          current_ستون++;
        }
    }

  fputs (buf, stdout);
  حرر (buf);
  current_ستون += buflen;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              پروګرام_name, پروګرام_name, پروګرام_name);
      fputs (_("\
Print or change terminal characteristics.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings دننه human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings دننه a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  open and use DEVICE instead د standard ننوت\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system تعریفوي which settings are شته.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#که_تعریف_شوی VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding د output\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop إشارة once ننوت flushed\n\
"), stdout);
#د_که_پای
      fputs (_("\
   eof CHAR      CHAR will send an پای_ټکی د file (terminate the ننوت)\n\
   eol CHAR      CHAR will پای_ټکی the کرښه\n\
"), stdout);
#که_تعریف_شوی VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR لكل ending the کرښه\n\
"), stdout);
#د_که_پای
      fputs (_("\
   erase CHAR    CHAR will erase the وروستی character typed\n\
   intr CHAR     CHAR will send an مداخله إشارة\n\
   kill CHAR     CHAR will erase the current کرښه\n\
"), stdout);
#که_تعریف_شوی VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the بل character quoted\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی VSTATUS
      fputs (_("\
 * حالت CHAR   CHAR will send an info إشارة\n\
"), stdout);
#د_که_پای
      fputs (_("\
   quit CHAR     CHAR will send a quit إشارة\n\
"), stdout);
#إذا defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current کرښه\n\
"), stdout);
#د_که_پای
      fputs (_("\
   پیل CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop إشارة\n\
"), stdout);
#که_تعریف_شوی VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will بدل منزل a different shell layer\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the وروستی کلمه_2 typed\n\
"), stdout);
#د_که_پای
      fputs (_("\
\n\
Special settings:\n\
   N             سېټ_2 the ننوت and output speeds منزل N bauds\n\
"), stdout);
#که_تعریف_شوی TIOCGWINSZ
      fputs (_("\
   ستون_شمېر_2 N        tell the kernel that the terminal has N columns\n\
 * columns N     same as ستون_شمېر_2 N\n\
"), stdout);
#د_که_پای
      چاپ_کړه (_("\
 * [-]drain      wait لكل لېږدونه before applying settings (%s by افتراضي)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      سېټ_2 the ننوت چټکتیا منزل N\n\
"), stdout);
#که_تعریف_شوی HAVE_C_کرښه
      fputs (_("\
 * کرښه N        use کرښه discipline N\n\
"), stdout);
#د_که_پای
      fputs (_("\
   min N         with -icanon, سېټ_2 N characters minimum لكل a completed اقرأ\n\
   ospeed N      سېټ_2 the output چټکتیا منزل N\n\
"), stdout);
#که_تعریف_شوی TIOCGWINSZ
      fputs (_("\
   لیکې N        tell the kernel that the terminal has N لیکې\n\
   اندازه          print the شمېره د لیکې and columns according منزل the kernel\n\
"), stdout);
#د_که_پای
      fputs (_("\
   چټکتیا         print the terminal چټکتیا\n\
   وخت N        with -icanon, سېټ_2 اقرأ timeout د N tenths د a دوهم\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control سیګنالونه\n\
   [-]cread      allow ننوت منزل be received\n\
"), stdout);
#که_تعریف_شوی CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#د_که_پای
      fputs (_("\
   csN           سېټ_2 character اندازه منزل N بټونه_2, N دننه [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop بټونه_2 فی character (one with '-')\n\
   [-]hup        send a hangup إشارة when the وروستی process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity بټ دننه output and expect parity بټ دننه ننوت\n\
   [-]parodd     سېټ_2 odd parity (or even parity with '-')\n\
"), stdout);
#که_تعریف_شوی CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#د_که_پای
      fputs (_("\
\n\
ننوت_3 settings:\n\
   [-]brkint     breaks cause an مداخله إشارة\n\
   [-]icrnl      translate carriage أرجع منزل newline\n\
   [-]ignbrk     ignore اكسر characters\n\
   [-]igncr      ignore carriage أرجع\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#که_تعریف_شوی IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and افعل not flush a ډک ننوت بفر on a character\n\
"), stdout);
#د_که_پای
      fputs (_("\
   [-]inlcr      translate newline منزل carriage أرجع\n\
   [-]inpck      enable ننوت parity checking\n\
   [-]istrip     clear لوړ (8th) بټ د ننوت characters\n\
"), stdout);
#که_تعریف_شوی IUTF8
      fputs (_("\
 * [-]iutf8      assume ننوت characters are UTF-8 encoded\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters منزل lowercase\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only پیل character\n\
"), stdout);
#د_که_پای
      fputs (_("\
   [-]ixoff      enable sending د پیل/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#که_تعریف_شوی BSDLY
      fputs (_("\
 * bsN           backspace delay style, N دننه [0..1]\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی CRDLY
      fputs (_("\
 * crN           carriage أرجع delay style, N دننه [0..3]\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی FFDLY
      fputs (_("\
 * ffN           form feed delay style, N دننه [0..1]\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی NLDLY
      fputs (_("\
 * nlN           newline delay style, N دننه [0..1]\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage أرجع منزل newline\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters لكل fill instead د NUL characters\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead د timing لكل delays\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters منزل uppercase\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی ONLCR
      fputs (_("\
 * [-]onlcr      translate newline منزل carriage أرجع-newline\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage أرجع\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی ONOCR
      fputs (_("\
 * [-]onocr      افعل not print carriage returns دننه the لومړی ستنه\n\
"), stdout);
#د_که_پای
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#إذا defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N دننه [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N دننه [0..1]\n\
"), stdout);
#د_که_پای
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#که_تعریف_شوی ECHOKE
      fputs (_("\
 * crtkill       kill all کرښه by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all کرښه by obeying the echoctl and echok settings\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters دننه hat notation ('^c')\n\
"), stdout);
#د_که_پای
      fputs (_("\
   [-]echo       echo ننوت characters\n\
"), stdout);
#که_تعریف_شوی ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#د_که_پای
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#که_تعریف_شوی ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#د_که_پای
      fputs (_("\
   [-]echonl     echo newline even إذا not echoing other characters\n\
"), stdout);
#که_تعریف_شوی ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#د_که_پای
#إذا defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with لوړ latency links\n\
"), stdout);
#د_که_پای
#إذا defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#د_که_پای
      چاپ_کړه (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#که_تعریف_شوی VWERASE
    ", werase"
#د_که_پای
#إذا defined CREPRINT || defined VREPRINT
    ", rprnt"
#د_که_پای
);
      fputs (_("\
   [-]isig       enable مداخله, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after مداخله and quit special characters\n\
"), stdout);
#که_تعریف_شوی ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try منزل اكتب منزل the terminal\n\
"), stdout);
#د_که_پای
#که_تعریف_شوی XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' لكل uppercase characters\n\
"), stdout);
#د_که_پای
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#إذا defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#د_که_پای
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters منزل their افتراضي ارزښتونه\n\
   -cooked       same as raw\n\
"), stdout);
      چاپ_کړه (_("\
   crt           same as %s\n\
"), "echoe"
#که_تعریف_شوی ECHOCTL
    " echoctl"
#د_که_پای
#که_تعریف_شوی ECHOKE
    " echoke"
#د_که_پای
);
      چاپ_کړه (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#که_تعریف_شوی ECHOCTL
    " echoctl"
#د_که_پای
#که_تعریف_شوی ECHOKE
    " echoke"
#د_که_پای
#که_تعریف_شوی IXANY
    " -ixany"
#د_که_پای
);
#که_تعریف_شوی IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#د_که_پای
      fputs (_("\
   ek            erase and kill characters منزل their افتراضي ارزښتونه\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#إذا defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#د_که_پای
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      چاپ_کړه (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#که_تعریف_شوی ONLCR
   " -onlcr"
#د_که_پای
  , "icrnl -inlcr -igncr"
#که_تعریف_شوی ONLCR
   " onlcr"
#د_که_پای
#که_تعریف_شوی OCRNL
   " -ocrnl"
#د_که_پای
#که_تعریف_شوی ONLRET
   " -onlret"
#د_که_پای
);
      fputs (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), stdout);
      چاپ_کړه (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 وخت 0\n\
   -raw          same as cooked\n\
"),
#که_تعریف_شوی IUCLC
   " -iuclc"
#د_که_پای
#که_تعریف_شوی IXANY
   " -ixany"
#د_که_پای
#که_تعریف_شوی IMAXBEL
   " -imaxbel"
#د_که_پای
#که_تعریف_شوی XCASE
   " -xcase"
#د_که_پای
);
      چاپ_کړه (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters منزل their افتراضي ارزښتونه\n\
"),
   "-ixoff"
#که_تعریف_شوی IUTF8
   " -iutf8"
#د_که_پای
#که_تعریف_شوی IUCLC
   " -iuclc"
#د_که_پای
#که_تعریف_شوی IXANY
   " -ixany"
#د_که_پای
#که_تعریف_شوی IMAXBEL
   " imaxbel"
#د_که_پای
#که_تعریف_شوی XCASE
   " -xcase"
#د_که_پای
#که_تعریف_شوی OLCUC
   " -olcuc"
#د_که_پای
#که_تعریف_شوی OCRNL
   " -ocrnl"
#د_که_پای

 , "opost"
#که_تعریف_شوی OFILL
   " -ofill"
#د_که_پای
#که_تعریف_شوی ONLCR
   " onlcr"
#د_که_پای
#که_تعریف_شوی ONOCR
   " -onocr"
#د_که_پای
#که_تعریف_شوی ONLRET
   " -onlret"
#د_که_پای
#که_تعریف_شوی NLDLY
   " nl0"
#د_که_پای
#که_تعریف_شوی CRDLY
   " cr0"
#د_که_پای
#که_تعریف_شوی TAB0
   " tab0"
#د_که_پای
#که_تعریف_شوی BSDLY
   " bs0"
#د_که_پای
#که_تعریف_شوی VTDLY
   " vt0"
#د_که_پای
#که_تعریف_شوی FFDLY
   " ff0"
#د_که_پای

 , "isig"
#که_تعریف_شوی TOSTOP
   " -tostop"
#د_که_پای
#که_تعریف_شوی OFDEL
   " -ofdel"
#د_که_پای
#که_تعریف_شوی ECHOPRT
   " -echoprt"
#د_که_پای
#که_تعریف_شوی ECHOCTL
   " echoctl"
#د_که_پای
#که_تعریف_شوی ECHOKE
   " echoke"
#د_که_پای
#که_تعریف_شوی EXTPROC
   " -extproc"
#د_که_پای
#که_تعریف_شوی FLUSHO
   " -flusho"
#د_که_پای
);
      fputs (_("\
\n\
Handle the tty کرښه connected منزل standard ننوت.  Without arguments,\n\
prints baud شرح, کرښه discipline, and deviations from stty sane.  دننه_2\n\
settings, CHAR is اخیستل_شوی literally, or coded as دننه ^c, 0x37, 0177 or\n\
127; special ارزښتونه ^- or undef کارول_شوی منزل disable special characters.\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

ساكن فراغ
apply_settings (bool checking, حرف ثابت *device_name,
                حرف * ثابت *settings, صحيح n_settings,
                بنية termios *ډېرپېښ_2, bool *require_سېټ_attr)
{
#تعریف_کړه check_دلیل(arg)						\
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
      bool سمون_وموندل = false;
      bool not_سېټ_attr = false;
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
      لكل (صحيح i = 0; ډېرپېښ_info[i].name != عدم; ++i)
        {
          إذا (streq (arg, ډېرپېښ_info[i].name))
            {
              إذا ((ډېرپېښ_info[i].flags & NO_SETATTR) == 0)
                {
                  سمون_وموندل = سېټ_ډېرپېښ (&ډېرپېښ_info[i], reversed, ډېرپېښ_2);
                  *require_سېټ_attr = true;
                }
              وإلا
                سمون_وموندل = not_سېټ_attr = true;
              اكسر;
            }
        }
      إذا (!سمون_وموندل && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      إذا (!سمون_وموندل)
        {
          لكل (صحيح i = 0; control_info[i].name != عدم; ++i)
            {
              إذا (streq (arg, control_info[i].name))
                {
                  check_دلیل (arg);
                  سمون_وموندل = true;
                  ++k;
                  سېټ_control_char (&control_info[i], settings[k], ډېرپېښ_2);
                  *require_سېټ_attr = true;
                  اكسر;
                }
            }
        }
      إذا (!سمون_وموندل || not_سېټ_attr)
        {
          إذا (streq (arg, "ispeed"))
            {
              check_دلیل (arg);
              ++k;
              بنية چټکتیا_desc ثابت ispeed = string_منزل_baud (settings[k]);
              إذا (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              سېټ_چټکتیا (ننوت_چټکتیا, settings[k], ispeed, ډېرپېښ_2);
              إذا (checking)
                استمر;
              *require_سېټ_attr = true;
            }
          وإلا إذا (streq (arg, "ospeed"))
            {
              check_دلیل (arg);
              ++k;
              بنية چټکتیا_desc ثابت ospeed = string_منزل_baud (settings[k]);
              إذا (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              سېټ_چټکتیا (output_چټکتیا, settings[k], ospeed, ډېرپېښ_2);
              إذا (checking)
                استمر;
              *require_سېټ_attr = true;
            }
#که_تعریف_شوی TIOCEXT
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
#د_که_پای
#که_تعریف_شوی TIOCGWINSZ
          وإلا إذا (streq (arg, "rows"))
            {
              check_دلیل (arg);
              ++k;
              إذا (checking)
                استمر;
              سېټ_کړکۍ_اندازه (integer_arg (settings[k], لوی_عدد), -1,
                               device_name);
            }
          وإلا إذا (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_دلیل (arg);
              ++k;
              إذا (checking)
                استمر;
              سېټ_کړکۍ_اندازه (-1, integer_arg (settings[k], لوی_عدد),
                               device_name);
            }
          وإلا إذا (streq (arg, "size"))
            {
              إذا (checking)
                استمر;
              max_ستون = screen_columns ();
              current_ستون = 0;
              display_کړکۍ_اندازه (false, device_name);
            }
#د_که_پای
#که_تعریف_شوی HAVE_C_کرښه
          وإلا إذا (streq (arg, "line"))
            {
              check_دلیل (arg);
              ++k;
              uintmax_t ارزښت = integer_arg (settings[k], UINTMAX_MAX);
              إذا (ckd_جمع_کړه (&ډېرپېښ_2->c_کرښه, ارزښت, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_سېټ_attr = true;
            }
#د_که_پای
          وإلا إذا (streq (arg, "speed"))
            {
              إذا (checking)
                استمر;
              max_ستون = screen_columns ();
              display_چټکتیا (ډېرپېښ_2, false);
            }
          وإلا
            {
              بنية چټکتیا_desc ثابت چټکتیا = string_منزل_baud (arg);
              إذا (چټکتیا.valid)
                {
                  سېټ_چټکتیا (both_speeds, arg, چټکتیا, ډېرپېښ_2);
                  إذا (checking)
                    استمر;
                  *require_سېټ_attr = true;
                }
              وإلا
                {
                  إذا (! recover_ډېرپېښ (arg, ډېرپېښ_2))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_سېټ_attr = true;
                }
            }
        }
    }

  إذا (checking)
    check_چټکتیا (ډېرپېښ_2);
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ساكن بنية termios ډېرپېښ_2;

  تعداد output_type output_type;
  صحيح optc;
  صحيح argi = 0;
  صحيح opti = 1;
  bool require_سېټ_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  حرف *file_name = عدم;
  حرف ثابت *device_name;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = بدل_شوی;
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

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

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
      ساكن بنية termios check_ډېرپېښ;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_ډېرپېښ, &require_سېټ_attr);
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

  إذا (tcgetattr (STDIN_FILENO, &ډېرپېښ_2))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  إذا (verbose_output || recoverable_output || noargs)
    {
      max_ستون = screen_columns ();
      current_ستون = 0;
      display_settings (output_type, &ډېرپېښ_2, device_name);
      أرجع EXIT_SUCCESS;
    }

  require_سېټ_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &ډېرپېښ_2, &require_سېټ_attr);

  إذا (require_سېټ_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ساكن بنية termios new_ډېرپېښ;

      إذا (tcsetattr (STDIN_FILENO, tcsetattr_options, &ډېرپېښ_2))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      إذا (tcgetattr (STDIN_FILENO, &new_ډېرپېښ))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      إذا (! eq_ډېرپېښ (&ډېرپېښ_2, &new_ډېرپېښ))
        {
          إذا (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              لكل (بینښه صحيح i = 0; i < حجم (new_ډېرپېښ); i++)
                {
                  بینښه صحيح newc = *(((بینښه حرف *) &new_ډېرپېښ) + i);
                  بینښه صحيح oldc = *(((بینښه حرف *) &ډېرپېښ_2) + i);
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
eq_ډېرپېښ (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  إذا (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#که_تعریف_شوی HAVE_C_کرښه
      || mode1->c_کرښه != mode2->c_کرښه
#د_که_پای
      || !memeq (mode1->c_cc, mode2->c_cc, حجم (mode1->c_cc)))
    أرجع false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_ماسک = ~(tcflag_t) 0;
#که_تعریف_شوی CBAUD
  cflag_ماسک &= ~(tcflag_t) CBAUD;
#د_که_پای
#که_تعریف_شوی CIBAUD
  cflag_ماسک &= ~(tcflag_t) CIBAUD;
#د_که_پای

  إذا ((mode1->c_cflag ^ mode2->c_cflag) & cflag_ماسک)
    أرجع false;

  أرجع چټکتیا_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ساكن bool
سېټ_ډېرپېښ (بنية ډېرپېښ_info ثابت *info, bool reversed, بنية termios *ډېرپېښ_2)
{
  tcflag_t *bitsp;

  إذا (reversed && (info->flags & REV) == 0)
    أرجع false;

  bitsp = ډېرپېښ_type_flag (info->type, ډېرپېښ_2);

  إذا (bitsp == عدم)
    {
      /* Combination mode. */
      إذا (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          إذا (reversed)
            ډېرپېښ_2->c_cflag = (ډېرپېښ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            ډېرپېښ_2->c_cflag = (ډېرپېښ_2->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      وإلا إذا (streq (info->name, "oddp"))
        {
          إذا (reversed)
            ډېرپېښ_2->c_cflag = (ډېرپېښ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            ډېرپېښ_2->c_cflag = (ډېرپېښ_2->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      وإلا إذا (streq (info->name, "nl"))
        {
          إذا (reversed)
            {
              ډېرپېښ_2->c_iflag = (ډېرپېښ_2->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              ډېرپېښ_2->c_oflag = (ډېرپېښ_2->c_oflag
#که_تعریف_شوی ONLCR
                               | ONLCR
#د_که_پای
                )
#که_تعریف_شوی OCRNL
                & ~OCRNL
#د_که_پای
#که_تعریف_شوی ONLRET
                & ~ONLRET
#د_که_پای
                ;
            }
          وإلا
            {
              ډېرپېښ_2->c_iflag = ډېرپېښ_2->c_iflag & ~ICRNL;
#که_تعریف_شوی ONLCR
              ډېرپېښ_2->c_oflag = ډېرپېښ_2->c_oflag & ~ONLCR;
#د_که_پای
            }
        }
      وإلا إذا (streq (info->name, "ek"))
        {
          ډېرپېښ_2->c_cc[VERASE] = CERASE;
          ډېرپېښ_2->c_cc[VKILL] = CKILL;
        }
      وإلا إذا (streq (info->name, "sane"))
        sane_ډېرپېښ (ډېرپېښ_2);
      وإلا إذا (streq (info->name, "cbreak"))
        {
          إذا (reversed)
            ډېرپېښ_2->c_lflag |= ICANON;
          وإلا
            ډېرپېښ_2->c_lflag &= ~ICANON;
        }
      وإلا إذا (streq (info->name, "pass8"))
        {
          إذا (reversed)
            {
              ډېرپېښ_2->c_cflag = (ډېرپېښ_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              ډېرپېښ_2->c_iflag |= ISTRIP;
            }
          وإلا
            {
              ډېرپېښ_2->c_cflag = (ډېرپېښ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              ډېرپېښ_2->c_iflag &= ~ISTRIP;
            }
        }
      وإلا إذا (streq (info->name, "litout"))
        {
          إذا (reversed)
            {
              ډېرپېښ_2->c_cflag = (ډېرپېښ_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              ډېرپېښ_2->c_iflag |= ISTRIP;
              ډېرپېښ_2->c_oflag |= OPOST;
            }
          وإلا
            {
              ډېرپېښ_2->c_cflag = (ډېرپېښ_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              ډېرپېښ_2->c_iflag &= ~ISTRIP;
              ډېرپېښ_2->c_oflag &= ~OPOST;
            }
        }
      وإلا إذا (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          إذا ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              ډېرپېښ_2->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              ډېرپېښ_2->c_oflag |= OPOST;
              ډېرپېښ_2->c_lflag |= ISIG | ICANON;
#إذا VMIN == VEOF
              ډېرپېښ_2->c_cc[VEOF] = CEOF;
#د_که_پای
#إذا VTIME == VEOL
              ډېرپېښ_2->c_cc[VEOL] = CEOL;
#د_که_پای
            }
          وإلا
            {
              /* Raw mode. */
              ډېرپېښ_2->c_iflag = 0;
              ډېرپېښ_2->c_oflag &= ~OPOST;
              ډېرپېښ_2->c_lflag &= ~(ISIG | ICANON
#که_تعریف_شوی XCASE
                                 | XCASE
#د_که_پای
                );
              ډېرپېښ_2->c_cc[VMIN] = 1;
              ډېرپېښ_2->c_cc[VTIME] = 0;
            }
        }
#که_تعریف_شوی IXANY
      وإلا إذا (streq (info->name, "decctlq"))
        {
          إذا (reversed)
            ډېرپېښ_2->c_iflag |= IXANY;
          وإلا
            ډېرپېښ_2->c_iflag &= ~IXANY;
        }
#د_که_پای
#که_تعریف_شوی TABDLY
      وإلا إذا (streq (info->name, "tabs"))
        {
          إذا (reversed)
            ډېرپېښ_2->c_oflag = (ډېرپېښ_2->c_oflag & ~TABDLY) | TAB3;
          وإلا
            ډېرپېښ_2->c_oflag = (ډېرپېښ_2->c_oflag & ~TABDLY) | TAB0;
        }
#وإلا
# ifdef OXTABS
      وإلا إذا (streq (info->name, "tabs"))
        {
          إذا (reversed)
            ډېرپېښ_2->c_oflag = ډېرپېښ_2->c_oflag | OXTABS;
          وإلا
            ډېرپېښ_2->c_oflag = ډېرپېښ_2->c_oflag & ~OXTABS;
        }
# endif
#د_که_پای
#إذا defined XCASE && defined IUCLC && defined OLCUC
      وإلا إذا (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          إذا (reversed)
            {
              ډېرپېښ_2->c_lflag &= ~XCASE;
              ډېرپېښ_2->c_iflag &= ~IUCLC;
              ډېرپېښ_2->c_oflag &= ~OLCUC;
            }
          وإلا
            {
              ډېرپېښ_2->c_lflag |= XCASE;
              ډېرپېښ_2->c_iflag |= IUCLC;
              ډېرپېښ_2->c_oflag |= OLCUC;
            }
        }
#د_که_پای
      وإلا إذا (streq (info->name, "crt"))
        ډېرپېښ_2->c_lflag |= ECHOE
#که_تعریف_شوی ECHOCTL
          | ECHOCTL
#د_که_پای
#که_تعریف_شوی ECHOKE
          | ECHOKE
#د_که_پای
          ;
      وإلا إذا (streq (info->name, "dec"))
        {
          ډېرپېښ_2->c_cc[VINTR] = 3;	/* ^C */
          ډېرپېښ_2->c_cc[VERASE] = 127;	/* DEL */
          ډېرپېښ_2->c_cc[VKILL] = 21;	/* ^U */
          ډېرپېښ_2->c_lflag |= ECHOE
#که_تعریف_شوی ECHOCTL
            | ECHOCTL
#د_که_پای
#که_تعریف_شوی ECHOKE
            | ECHOKE
#د_که_پای
            ;
#که_تعریف_شوی IXANY
          ډېرپېښ_2->c_iflag &= ~IXANY;
#د_که_پای
        }
    }
  وإلا إذا (reversed)
    *bitsp = *bitsp & ~info->ماسک & ~info->بټونه_2;
  وإلا
    *bitsp = (*bitsp & ~info->ماسک) | info->بټونه_2;

  أرجع true;
}

ساكن فراغ
سېټ_control_char (بنية control_info ثابت *info, حرف ثابت *arg,
                  بنية termios *ډېرپېښ_2)
{
  بینښه اوږد صحيح ارزښت;

  إذا (streq (info->name, "min") || streq (info->name, "time"))
    ارزښت = integer_arg (arg, TYPE_اعظمي (cc_t));
  وإلا إذا (arg[0] == '\0' || arg[1] == '\0')
    ارزښت = منزل_uchar (arg[0]);
  وإلا إذا (streq (arg, "^-") || streq (arg, "undef"))
    ارزښت = _POSIX_VDISABLE;
  وإلا إذا (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      إذا (arg[1] == '?')
        ارزښت = 127;
      وإلا
        ارزښت = منزل_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  وإلا
    ارزښت = integer_arg (arg, TYPE_اعظمي (cc_t));
  ډېرپېښ_2->c_cc[info->دننه_واټن] = ارزښت;
}

ساكن فراغ
سېټ_چټکتیا (تعداد چټکتیا_setting type, حرف ثابت *arg,
           بنية چټکتیا_desc ثابت چټکتیا, بنية termios *ډېرپېښ_2)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (چټکتیا.valid);

  إذا (type == ننوت_چټکتیا || type == both_speeds)
    {
      وروستی_ibaud = چټکتیا;
      إذا (cfsetispeed (ډېرپېښ_2, چټکتیا.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  إذا (type == output_چټکتیا || type == both_speeds)
    {
      وروستی_obaud = چټکتیا;
      إذا (cfsetospeed (ډېرپېښ_2, چټکتیا.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#که_تعریف_شوی TIOCGWINSZ

ساكن صحيح
get_win_اندازه (صحيح fd, بنية winsize *win)
{
  صحيح err = ioctl (fd, TIOCGWINSZ, (حرف *) win);
  أرجع err;
}

ساكن فراغ
سېټ_کړکۍ_اندازه (صحيح لیکې, صحيح ستون_شمېر_2, حرف ثابت *device_name)
{
  بنية winsize win;

  إذا (get_win_اندازه (STDIN_FILENO, &win))
    {
      إذا (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, حجم (win));
    }

  إذا (لیکې >= 0)
    win.ws_لیکه = لیکې;
  إذا (ستون_شمېر_2 >= 0)
    win.ws_ستون = ستون_شمېر_2;

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

  إذا (win.ws_لیکه == 0 || win.ws_ستون == 0)
    {
      بنية ttysize ttysz;

      ttysz.ts_کرښې = win.ws_لیکه;
      ttysz.ts_ستون_شمېر = win.ws_ستون;

      win.ws_لیکه = 1;
      win.ws_ستون = 1;

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
display_کړکۍ_اندازه (bool fancy, حرف ثابت *device_name)
{
  بنية winsize win;

  إذا (get_win_اندازه (STDIN_FILENO, &win))
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
             win.ws_لیکه, win.ws_ستون);
      إذا (!fancy)
        current_ستون = 0;
    }
}
#د_که_پای

ساكن صحيح
screen_columns (فراغ)
{
#که_تعریف_شوی TIOCGWINSZ
  بنية winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  إذا (get_win_اندازه (STDOUT_FILENO, &win) == 0 && 0 < win.ws_ستون)
    أرجع win.ws_ستون;
#د_که_پای
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    حرف *ستون_string = getenv ("COLUMNS");
    اوږد صحيح n_columns;
    إذا (!(ستون_string != عدم
          && xstrtol (ستون_string, عدم, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= لوی_عدد))
      n_columns = 80;
    أرجع n_columns;
  }
}

ATTRIBUTE_خالص
ساكن tcflag_t *
ډېرپېښ_type_flag (تعداد ډېرپېښ_type type, بنية termios *ډېرپېښ_2)
{
  بدل (type)
    {
    حالة control:
      أرجع &ډېرپېښ_2->c_cflag;

    حالة ننوت:
      أرجع &ډېرپېښ_2->c_iflag;

    حالة output:
      أرجع &ډېرپېښ_2->c_oflag;

    حالة local:
      أرجع &ډېرپېښ_2->c_lflag;

    حالة combination:
      أرجع عدم;

    افتراضي:
      unreachable ();
    }
}

ساكن فراغ
display_settings (تعداد output_type output_type, بنية termios *ډېرپېښ_2,
                  حرف ثابت *device_name)
{
  بدل (output_type)
    {
    حالة بدل_شوی:
      display_بدل_شوی (ډېرپېښ_2);
      اكسر;

    حالة all:
      display_all (ډېرپېښ_2, device_name);
      اكسر;

    حالة recoverable:
      display_recoverable (ډېرپېښ_2);
      اكسر;
    }
}

ساكن فراغ
display_بدل_شوی (بنية termios *ډېرپېښ_2)
{
  bool تشه_کرښه;
  tcflag_t *bitsp;
  بینښه اوږد ماسک;
  تعداد ډېرپېښ_type prev_type = control;

  display_چټکتیا (ډېرپېښ_2, true);
#که_تعریف_شوی HAVE_C_کرښه
  wrapf ("line = %d;", ډېرپېښ_2->c_کرښه);
#د_که_پای
  توری_چاپ_کړه ('\n');
  current_ستون = 0;

  تشه_کرښه = true;
  لكل (صحيح i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      إذا (ډېرپېښ_2->c_cc[control_info[i].دننه_واټن] == control_info[i].saneval)
        استمر;

#که_تعریف_شوی VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      إذا (streq (control_info[i].name, "flush"))
        استمر;
#د_که_پای
      /* If swtch is the same as susp, don't print both.  */
#إذا VSWTCH == VSUSP
      إذا (streq (control_info[i].name, "swtch"))
        استمر;
#د_که_پای
      /* If eof uses the same slot as min, only print whichever applies.  */
#إذا VEOF == VMIN
      إذا ((ډېرپېښ_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#د_که_پای

      تشه_کرښه = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (ډېرپېښ_2->c_cc[control_info[i].دننه_واټن]));
    }
  إذا ((ډېرپېښ_2->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (بینښه اوږد صحيح) ډېرپېښ_2->c_cc[VMIN],
             (بینښه اوږد صحيح) ډېرپېښ_2->c_cc[VTIME]);
    }
  وإلا إذا (!تشه_کرښه)
    توری_چاپ_کړه ('\n');
  current_ستون = 0;

  تشه_کرښه = true;
  لكل (صحيح i = 0; ډېرپېښ_info[i].name != عدم; ++i)
    {
      إذا (ډېرپېښ_info[i].flags & OMIT)
        استمر;
      إذا (ډېرپېښ_info[i].type != prev_type)
        {
          إذا (!تشه_کرښه)
            {
              توری_چاپ_کړه ('\n');
              current_ستون = 0;
              تشه_کرښه = true;
            }
          prev_type = ډېرپېښ_info[i].type;
        }

      bitsp = ډېرپېښ_type_flag (ډېرپېښ_info[i].type, ډېرپېښ_2);
      ماسک = ډېرپېښ_info[i].ماسک ? ډېرپېښ_info[i].ماسک : ډېرپېښ_info[i].بټونه_2;
      إذا ((*bitsp & ماسک) == ډېرپېښ_info[i].بټونه_2)
        {
          إذا (ډېرپېښ_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", ډېرپېښ_info[i].name);
              تشه_کرښه = false;
            }
        }
      وإلا إذا ((ډېرپېښ_info[i].flags & (SANE_سېټ | REV)) == (SANE_سېټ | REV))
        {
          wrapf ("-%s", ډېرپېښ_info[i].name);
          تشه_کرښه = false;
        }
    }
  إذا (!تشه_کرښه)
    توری_چاپ_کړه ('\n');
  current_ستون = 0;
}

ساكن فراغ
display_all (بنية termios *ډېرپېښ_2, حرف ثابت *device_name)
{
  tcflag_t *bitsp;
  بینښه اوږد ماسک;
  تعداد ډېرپېښ_type prev_type = control;

  display_چټکتیا (ډېرپېښ_2, true);
#که_تعریف_شوی TIOCGWINSZ
  display_کړکۍ_اندازه (true, device_name);
#د_که_پای
#که_تعریف_شوی HAVE_C_کرښه
  wrapf ("line = %d;", ډېرپېښ_2->c_کرښه);
#د_که_پای
  توری_چاپ_کړه ('\n');
  current_ستون = 0;

  لكل (صحيح i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#که_تعریف_شوی VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      إذا (streq (control_info[i].name, "flush"))
        استمر;
#د_که_پای
      /* If swtch is the same as susp, don't print both.  */
#إذا VSWTCH == VSUSP
      إذا (streq (control_info[i].name, "swtch"))
        استمر;
#د_که_پای
      /* If eof uses the same slot as min, only print whichever applies.  */
#إذا VEOF == VMIN
      إذا ((ډېرپېښ_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#د_که_پای
      wrapf ("%s = %s;", control_info[i].name,
             visible (ډېرپېښ_2->c_cc[control_info[i].دننه_واټن]));
    }
#إذا VEOF == VMIN
  إذا ((ډېرپېښ_2->c_lflag & ICANON) == 0)
#د_که_پای
    wrapf ("min = %lu; time = %lu;",
           (بینښه اوږد صحيح) ډېرپېښ_2->c_cc[VMIN],
           (بینښه اوږد صحيح) ډېرپېښ_2->c_cc[VTIME]);
  إذا (current_ستون != 0)
    توری_چاپ_کړه ('\n');
  current_ستون = 0;

  لكل (صحيح i = 0; ډېرپېښ_info[i].name != عدم; ++i)
    {
      إذا (ډېرپېښ_info[i].flags & OMIT)
        استمر;
      إذا (ډېرپېښ_info[i].type != prev_type)
        {
          توری_چاپ_کړه ('\n');
          current_ستون = 0;
          prev_type = ډېرپېښ_info[i].type;
        }

      bitsp = ډېرپېښ_type_flag (ډېرپېښ_info[i].type, ډېرپېښ_2);
      ماسک = ډېرپېښ_info[i].ماسک ? ډېرپېښ_info[i].ماسک : ډېرپېښ_info[i].بټونه_2;
      إذا ((*bitsp & ماسک) == ډېرپېښ_info[i].بټونه_2)
        wrapf ("%s", ډېرپېښ_info[i].name);
      وإلا إذا (ډېرپېښ_info[i].flags & REV)
        wrapf ("-%s", ډېرپېښ_info[i].name);
    }
  توری_چاپ_کړه ('\n');
  current_ستون = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ساكن فراغ
check_چټکتیا (بنية termios *ډېرپېښ_2)
{
  إذا (وروستی_ibaud.valid && وروستی_obaud.valid)
    {
      إذا (cfgetispeed (ډېرپېښ_2) != وروستی_ibaud.baud
          || cfgetospeed (ډېرپېښ_2) != وروستی_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_منزل_ارزښت (وروستی_ibaud.baud),
               baud_منزل_ارزښت (وروستی_obaud.baud));
    }
}

ساكن فراغ
display_چټکتیا (بنية termios *ډېرپېښ_2, bool fancy)
{
  إذا (cfgetispeed (ډېرپېښ_2) == 0 || cfgetispeed (ډېرپېښ_2) == cfgetospeed (ډېرپېښ_2))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_منزل_ارزښت (cfgetospeed (ډېرپېښ_2)));
  وإلا
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_منزل_ارزښت (cfgetispeed (ډېرپېښ_2)),
           baud_منزل_ارزښت (cfgetospeed (ډېرپېښ_2)));
  إذا (!fancy)
    current_ستون = 0;
}

ساكن فراغ
display_recoverable (بنية termios *ډېرپېښ_2)
{
  چاپ_کړه ("%lx:%lx:%lx:%lx",
          (بینښه اوږد صحيح) ډېرپېښ_2->c_iflag,
          (بینښه اوږد صحيح) ډېرپېښ_2->c_oflag,
          (بینښه اوږد صحيح) ډېرپېښ_2->c_cflag,
          (بینښه اوږد صحيح) ډېرپېښ_2->c_lflag);
  لكل (size_t i = 0; i < NCCS; ++i)
    چاپ_کړه (":%lx", (بینښه اوږد صحيح) ډېرپېښ_2->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#که_تعریف_شوی CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (ډېرپېښ_2->c_cflag & CBAUD) == BOTHER;
#  إذا defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((ډېرپېښ_2->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#د_که_پای
  إذا (separate_speeds)
    چاپ_کړه (":%lx:%lx", (بینښه اوږد صحيح) cfgetispeed (ډېرپېښ_2),
            (بینښه اوږد صحيح) cfgetospeed (ډېرپېښ_2));

  توری_چاپ_کړه ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#تعریف_کړه STRTOUL_TYPE(TYPE)                                               \
  ساكن صحيح                                                             \
  strtoul_##TYPE (حرف ثابت *s, صحيح بنسټ_2, حرف **p, TYPE *پایله,       \
                  حرف delim)                                            \
  {                                                                      \
    بینښه اوږد ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, بنسټ_2);                                           \
    إذا (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      أرجع -1;                                                         \
    *پایله = ul;                                                        \
    أرجع 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (چټکتیا_t)
#تعریف_لغوه STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#ضمن "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ساكن bool
recover_ډېرپېښ (حرف ثابت *arg, بنية termios *ډېرپېښ_2)
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
  ډېرپېښ_2->c_iflag = flag[0];
  ډېرپېښ_2->c_oflag = flag[1];
  ډېرپېښ_2->c_cflag = flag[2];
  ډېرپېښ_2->c_lflag = flag[3];

  لكل (size_t i = 0; i < NCCS; ++i)
    {
      حرف *p;
      حرف delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      إذا (strtoul_cc_t (s, 16, &p, ډېرپېښ_2->c_cc + i, delim) != 0)
        أرجع false;
      /* Accept the old format, which has no separate speed fields.  */
      إذا (! *p)
        {
#که_تعریف_شوی CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_چټکتیا_T_SANE
          بنية چټکتیا_desc ospeed = cbaud_منزل_چټکتیا (ډېرپېښ_2->c_cflag & CBAUD);
          إذا (ospeed.valid && cfsetospeed (ډېرپېښ_2, ospeed.baud) != 0)
            أرجع false;
#  إذا defined CIBAUD && defined IBSHIFT
          بنية چټکتیا_desc ispeed =
            cbaud_منزل_چټکتیا ((ډېرپېښ_2->c_cflag & CIBAUD) >> IBSHIFT);
          إذا (ispeed.valid && cfsetispeed (ډېرپېښ_2, ispeed.baud) != 0)
            أرجع false;
#  endif
# endif
          وروستی_ibaud.baud = cfgetispeed (ډېرپېښ_2);
          وروستی_obaud.baud = cfgetospeed (ډېرپېښ_2);
#د_که_پای
          أرجع true;
        }
      s = p + 1;
    }

  چټکتیا_t چټکتیا[2];
  لكل (size_t i = 0; i < 2; i++)
    {
      حرف *p;
      حرف delim = i == 0 ? ':' : '\0';
      إذا (strtoul_چټکتیا_t (s, 16, &p, چټکتیا + i, delim) != 0)
        أرجع false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  إذا (cfsetospeed (ډېرپېښ_2, چټکتیا[1]) != 0
      || cfsetispeed (ډېرپېښ_2, چټکتیا[0]) != 0)
    أرجع false;

  وروستی_ibaud.baud = چټکتیا[0] == 0 ? cfgetispeed (ډېرپېښ_2) : چټکتیا[0];
  وروستی_obaud.baud = چټکتیا[1];
  أرجع true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ساكن bool
چټکتیا_matches (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  چټکتیا_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  چټکتیا_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  إذا (speed1[1] == 0)
    speed1[1] = speed1[0];
  إذا (speed2[1] == 0)
    speed2[1] = speed2[0];

  بینښه صحيح بټونه_2 = 2;  /* Start and stop bits.  */
  بټونه_2 += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  بټونه_2 += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  بدل (mode1->c_cflag & CSIZE)
    {
      حالة CS5: بټونه_2 += 5; اكسر;
      حالة CS6: بټونه_2 += 6; اكسر;
      حالة CS7: بټونه_2 += 7; اكسر;
      افتراضي: بټونه_2 += 8; اكسر;
    }

  لكل (صحيح i = 0; i < 2; i++)
    {
      بینښه اوږد صحيح value1;
      بینښه اوږد صحيح value2;
      بینښه اوږد صحيح delta;

      إذا (speed1[i] == speed2[i])
        استمر;

      value1 = baud_منزل_ارزښت (speed1[i]);
      value2 = baud_منزل_ارزښت (speed2[i]);

#که_نه_تعریف_شوی TERMIOS_چټکتیا_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      إذا (value1 == ULONG_MAX || value2 == ULONG_MAX)
        أرجع false;
#د_که_پای

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      إذا (delta > value1 / (بټونه_2 * 4))
        أرجع false;
    }

  أرجع true;
}

ATTRIBUTE_خالص
ساكن بنية چټکتیا_desc
string_منزل_baud (حرف ثابت *arg)
{
  حرف *ep;
  بینښه اوږد ارزښت;
  بینښه حرف c;

  /* Explicitly disallow negative numbers.  */
  طالما (c_isspace (*arg))
    arg++;
  إذا (*arg == '-')
    اذهب invalid;

  /* Empty does not mean 0.  */
  إذا (! *arg)
    اذهب invalid;

  ارزښت = strtoul (arg, &ep, 10);

  c = *ep++;
  إذا (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      إذا (c)
        {
          بینښه حرف d = c - '0';
          إذا (d > 5)
            ارزښت++;
          وإلا إذا (d == 5)
            {
              طالما ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              إذا (c)
                ارزښت++;                /* Nonzero, round up */
              وإلا
                ارزښت += (ارزښت & 1);   /* Exactly in the middle, round even */
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
        أرجع (بنية چټکتیا_desc) { .baud = B19200, .valid = true };
      وإلا إذا (streq (arg, "extb"))
        أرجع (بنية چټکتیا_desc) { .baud = B38400, .valid = true };
      وإلا
        اذهب invalid;
    }

  أرجع ارزښت_منزل_baud (ارزښت);

 invalid:
  أرجع (بنية چټکتیا_desc) { .baud = (چټکتیا_t) -1, .valid = false };
}

ساكن فراغ
sane_ډېرپېښ (بنية termios *ډېرپېښ_2)
{
  tcflag_t *bitsp;

  لكل (صحيح i = 0; control_info[i].name; ++i)
    {
#إذا VMIN == VEOF
      إذا (streq (control_info[i].name, "min"))
        اكسر;
#د_که_پای
      ډېرپېښ_2->c_cc[control_info[i].دننه_واټن] = control_info[i].saneval;
    }

  لكل (صحيح i = 0; ډېرپېښ_info[i].name != عدم; ++i)
    {
      إذا (ډېرپېښ_info[i].flags & NO_SETATTR)
        استمر;

      إذا (ډېرپېښ_info[i].flags & SANE_سېټ)
        {
          bitsp = ډېرپېښ_type_flag (ډېرپېښ_info[i].type, ډېرپېښ_2);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~ډېرپېښ_info[i].ماسک) | ډېرپېښ_info[i].بټونه_2;
        }
      وإلا إذا (ډېرپېښ_info[i].flags & SANE_UNSET)
        {
          bitsp = ډېرپېښ_type_flag (ډېرپېښ_info[i].type, ډېرپېښ_2);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~ډېرپېښ_info[i].ماسک & ~ډېرپېښ_info[i].بټونه_2;
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
