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

#اگر_تعریف_شده TERMIOS_NEEDS_XOPEN_منبع
# define XOPEN_منبع
#پایان_اگر

#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن <termios.h>
#إذا HAVE_STROPTS_H
# include <stropts.h>
#پایان_اگر
#ضمن <sys/ioctl.h>

#اگر_تعریف_شده WINSIZE_درون_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#پایان_اگر
#اگر_تعریف_شده GWINSZ_درون_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#پایان_اگر
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
#تعریف_کن برنامه_NAME "stty"

#تعریف_کن AUTHORS proper_name ("David MacKenzie")

#اگر_تعریف_نشده _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#پایان_اگر

#تعریف_کن Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#اگر_تعریف_نشده CINTR
# define CINTR Control ('c')
#پایان_اگر
#اگر_تعریف_نشده CQUIT
# define CQUIT 28
#پایان_اگر
#اگر_تعریف_نشده CERASE
# define CERASE 127
#پایان_اگر
#اگر_تعریف_نشده CKILL
# define CKILL Control ('u')
#پایان_اگر
#اگر_تعریف_نشده CEOF
# define CEOF Control ('d')
#پایان_اگر
#اگر_تعریف_نشده CEOL
# define CEOL _POSIX_VDISABLE
#پایان_اگر
#اگر_تعریف_نشده CSTART
# define CSTART Control ('q')
#پایان_اگر
#اگر_تعریف_نشده CSTOP
# define CSTOP Control ('s')
#پایان_اگر
#اگر_تعریف_نشده CSUSP
# define CSUSP Control ('z')
#پایان_اگر
#إذا defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#پایان_اگر
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#إذا defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#پایان_اگر
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#إذا defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# إذا defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#پایان_اگر
#إذا defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#پایان_اگر

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#إذا defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#پایان_اگر

#إذا defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#پایان_اگر
#إذا defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#پایان_اگر
#إذا defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#پایان_اگر
#إذا defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#پایان_اگر
#إذا defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#پایان_اگر
#إذا defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#پایان_اگر
#إذا defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#پایان_اگر
#إذا defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#پایان_اگر
#إذا defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#پایان_اگر
#إذا defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#پایان_اگر
#إذا defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#پایان_اگر
#إذا defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#پایان_اگر
#إذا defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#پایان_اگر

/* Which speeds to set.  */
تعداد سرعت_setting
  {
    درون_داد_سرعت, output_سرعت, both_speeds
  };

/* What to output and how.  */
تعداد output_type
  {
    تغییریافته, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
تعداد نما_type
  {
    control, درون_داد, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#تعریف_کن SANE_مجموعه 1		/* Set in 'sane' mode. */
#تعریف_کن SANE_UNSET 2		/* Unset in 'sane' mode. */
#تعریف_کن REV 4			/* Can be turned off by prepending '-'. */
#تعریف_کن OMIT 8			/* Don't display value. */
#تعریف_کن NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
بنية نما_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    تعداد نما_type type;	/* Which structure element to change. */
    حرف flags;			/* Setting and display options.  */
    بیعلامت دراز بیتها_2;		/* Bits to set for this mode.  */
    بیعلامت دراز ماسک;		/* Other bits to turn off for this mode.  */
  };

ساكن بنية نما_info ثابت نما_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#اگر_تعریف_شده CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#پایان_اگر
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_مجموعه | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#اگر_تعریف_شده CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#پایان_اگر
#اگر_تعریف_شده CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#پایان_اگر

  {"ignbrk", درون_داد, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", درون_داد, SANE_مجموعه | REV, BRKINT, 0},
  {"ignpar", درون_داد, REV, IGNPAR, 0},
  {"parmrk", درون_داد, REV, PARMRK, 0},
  {"inpck", درون_داد, REV, INPCK, 0},
  {"istrip", درون_داد, REV, ISTRIP, 0},
  {"inlcr", درون_داد, SANE_UNSET | REV, INLCR, 0},
  {"igncr", درون_داد, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", درون_داد, SANE_مجموعه | REV, ICRNL, 0},
  {"ixon", درون_داد, REV, IXON, 0},
  {"ixoff", درون_داد, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", درون_داد, REV | OMIT, IXOFF, 0},
#اگر_تعریف_شده IUCLC
  {"iuclc", درون_داد, SANE_UNSET | REV, IUCLC, 0},
#پایان_اگر
#اگر_تعریف_شده IXANY
  {"ixany", درون_داد, SANE_UNSET | REV, IXANY, 0},
#پایان_اگر
#اگر_تعریف_شده IMAXBEL
  {"imaxbel", درون_داد, SANE_مجموعه | REV, IMAXBEL, 0},
#پایان_اگر
#اگر_تعریف_شده IUTF8
  {"iutf8", درون_داد, SANE_UNSET | REV, IUTF8, 0},
#پایان_اگر

  {"opost", output, SANE_مجموعه | REV, OPOST, 0},
#اگر_تعریف_شده OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#پایان_اگر
#اگر_تعریف_شده OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#پایان_اگر
#اگر_تعریف_شده ONLCR
  {"onlcr", output, SANE_مجموعه | REV, ONLCR, 0},
#پایان_اگر
#اگر_تعریف_شده ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#پایان_اگر
#اگر_تعریف_شده ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#پایان_اگر
#اگر_تعریف_شده OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#پایان_اگر
#اگر_تعریف_شده OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#پایان_اگر
#اگر_تعریف_شده NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_مجموعه, NL0, NLDLY},
#پایان_اگر
#اگر_تعریف_شده CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_مجموعه, CR0, CRDLY},
#پایان_اگر
#اگر_تعریف_شده TABDLY
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
  {"tab0", output, SANE_مجموعه, TAB0, TABDLY},
# endif
#وإلا
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#پایان_اگر
#اگر_تعریف_شده BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_مجموعه, BS0, BSDLY},
#پایان_اگر
#اگر_تعریف_شده VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_مجموعه, VT0, VTDLY},
#پایان_اگر
#اگر_تعریف_شده FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_مجموعه, FF0, FFDLY},
#پایان_اگر

  {"isig", local, SANE_مجموعه | REV, ISIG, 0},
  {"icanon", local, SANE_مجموعه | REV, ICANON, 0},
#اگر_تعریف_شده IEXTEN
  {"iexten", local, SANE_مجموعه | REV, IEXTEN, 0},
#پایان_اگر
  {"echo", local, SANE_مجموعه | REV, ECHO, 0},
  {"echoe", local, SANE_مجموعه | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_مجموعه | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#اگر_تعریف_شده XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#پایان_اگر
#اگر_تعریف_شده TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#پایان_اگر
#اگر_تعریف_شده ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#پایان_اگر
#اگر_تعریف_شده ECHOCTL
  {"echoctl", local, SANE_مجموعه | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#پایان_اگر
#اگر_تعریف_شده ECHOKE
  {"echoke", local, SANE_مجموعه | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#پایان_اگر
#اگر_تعریف_شده FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#پایان_اگر
#إذا defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#وإلاإذا defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#پایان_اگر

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
#اگر_تعریف_شده IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#پایان_اگر
#إذا defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#پایان_اگر
#إذا defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#پایان_اگر
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {عدم, control, 0, 0, 0}
};

/* Control character settings.  */
بنية control_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t جابه_جایی;		/* Offset in c_cc.  */
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
#اگر_تعریف_شده VEOL2
  {"eol2", CEOL2, VEOL2},
#پایان_اگر
#اگر_تعریف_شده VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#پایان_اگر
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#اگر_تعریف_شده VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#پایان_اگر
#اگر_تعریف_شده VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#وإلا
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#پایان_اگر
#اگر_تعریف_شده VWERASE
  {"werase", CWERASE, VWERASE},
#پایان_اگر
#اگر_تعریف_شده VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#پایان_اگر
#اگر_تعریف_شده VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#پایان_اگر
#اگر_تعریف_شده VSTATUS
  {"status", CSTATUS, VSTATUS},
#پایان_اگر

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {عدم, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
بنية سرعت_desc
{
  /* The baud rate.  */
  سرعت_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ساكن حرف ثابت *visible (cc_t ch);
ساكن بیعلامت دراز صحيح baud_مقصد_مقدار (سرعت_t سرعت);
ساكن bool recover_نما (حرف ثابت *arg, بنية termios *نما_2);
ساكن صحيح screen_columns (فراغ);
ساكن bool مجموعه_نما (بنية نما_info ثابت *info, bool reversed,
                      بنية termios *نما_2);
ساكن bool eq_نما (بنية termios ثابت *mode1,
                     بنية termios ثابت *mode2);
ساكن bool سرعت_matches (بنية termios ثابت *mode1,
                           بنية termios ثابت *mode2);
ساكن uintmax_t integer_arg (حرف ثابت *s, uintmax_t max);
ساكن بنية سرعت_desc string_مقصد_baud (حرف ثابت *arg);
ساكن tcflag_t *نما_type_flag (تعداد نما_type type, بنية termios *نما_2);
ساكن فراغ display_all (بنية termios *نما_2, حرف ثابت *device_name);
ساكن فراغ display_تغییریافته (بنية termios *نما_2);
ساكن فراغ display_recoverable (بنية termios *نما_2);
ساكن فراغ display_settings (تعداد output_type output_type,
                              بنية termios *نما_2,
                              حرف ثابت *device_name);
ساكن فراغ check_سرعت (بنية termios *نما_2);
ساكن فراغ display_سرعت (بنية termios *نما_2, bool fancy);
ساكن فراغ display_پنجره_اندازه (bool fancy, حرف ثابت *device_name);
ساكن فراغ sane_نما (بنية termios *نما_2);
ساكن فراغ مجموعه_control_char (بنية control_info ثابت *info,
                              حرف ثابت *arg,
                              بنية termios *نما_2);
ساكن فراغ مجموعه_سرعت (تعداد سرعت_setting type, حرف ثابت *arg,
                       بنية سرعت_desc ثابت سرعت, بنية termios *نما_2);
ساكن فراغ مجموعه_پنجره_اندازه (صحيح ردیفها, صحيح تعداد_ستون_2, حرف ثابت *device_name);

/* The width of the screen, for output wrapping. */
ساكن صحيح max_ستون;

/* Current position, to know when to wrap. */
ساكن صحيح current_ستون;

/* Default "drain" mode for tcsetattr.  */
ساكن صحيح tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ساكن bool dev_debug;

/* Record last speed set for correlation.  */
ساكن بنية سرعت_desc آخرین_ibaud =
  { .baud = (سرعت_t) -1, .valid = false };
ساكن بنية سرعت_desc آخرین_obaud =
  { .baud = (سرعت_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

ساكن بنية option ثابت longopts[] =
{
  {"all", no_آرگومان, عدم, 'a'},
  {"save", no_آرگومان, عدم, 'g'},
  {"file", required_آرگومان, عدم, 'F'},
  {"-debug", no_آرگومان, عدم, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((چاپ_کن, 1, 2))
ساكن فراغ
wrapf (حرف ثابت *پیام_2,...)
{
  va_list آرگومانها;
  حرف *buf;
  صحيح buflen;

  va_شروع (آرگومانها, پیام_2);
  buflen = vasprintf (&buf, پیام_2, آرگومانها);
  va_انتها (آرگومانها);

  إذا (buflen < 0)
    xalloc_die ();

  إذا (0 < current_ستون)
    {
      إذا (max_ستون - current_ستون <= buflen)
        {
          حرف_چاپ_کن ('\n');
          current_ستون = 0;
        }
      وإلا
        {
          حرف_چاپ_کن (' ');
          current_ستون++;
        }
    }

  fputs (buf, stdout);
  حرر (buf);
  current_ستون += buflen;
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              برنامه_name, برنامه_name, برنامه_name);
      fputs (_("\
Print or change terminal characteristics.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings درون human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings درون a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  open and use DEVICE instead از standard درون_داد\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system تعریف_میکند which settings are موجود.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#اگر_تعریف_شده VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding از output\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop إشارة once درون_داد flushed\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   eof CHAR      CHAR will send an انتها از file (terminate the درون_داد)\n\
   eol CHAR      CHAR will انتها the سطر\n\
"), stdout);
#اگر_تعریف_شده VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR لكل ending the سطر\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   erase CHAR    CHAR will erase the آخرین character typed\n\
   intr CHAR     CHAR will send an وقفه إشارة\n\
   kill CHAR     CHAR will erase the current سطر\n\
"), stdout);
#اگر_تعریف_شده VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the بعدی character quoted\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده VSTATUS
      fputs (_("\
 * وضعیت CHAR   CHAR will send an info إشارة\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   quit CHAR     CHAR will send a quit إشارة\n\
"), stdout);
#إذا defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current سطر\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   شروع CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop إشارة\n\
"), stdout);
#اگر_تعریف_شده VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will بدل مقصد a different shell layer\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the آخرین کلمه_2 typed\n\
"), stdout);
#پایان_اگر
      fputs (_("\
\n\
Special settings:\n\
   N             مجموعه_2 the درون_داد and output speeds مقصد N bauds\n\
"), stdout);
#اگر_تعریف_شده TIOCGWINSZ
      fputs (_("\
   تعداد_ستون_2 N        tell the kernel that the terminal has N columns\n\
 * columns N     same as تعداد_ستون_2 N\n\
"), stdout);
#پایان_اگر
      چاپ_کن (_("\
 * [-]drain      wait لكل ارسال before applying settings (%s by افتراضي)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      مجموعه_2 the درون_داد سرعت مقصد N\n\
"), stdout);
#اگر_تعریف_شده HAVE_C_سطر
      fputs (_("\
 * سطر N        use سطر discipline N\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   min N         with -icanon, مجموعه_2 N characters minimum لكل a completed اقرأ\n\
   ospeed N      مجموعه_2 the output سرعت مقصد N\n\
"), stdout);
#اگر_تعریف_شده TIOCGWINSZ
      fputs (_("\
   ردیفها N        tell the kernel that the terminal has N ردیفها\n\
   اندازه          print the عدد از ردیفها and columns according مقصد the kernel\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   سرعت         print the terminal سرعت\n\
   زمان N        with -icanon, مجموعه_2 اقرأ timeout از N tenths از a دومی\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control سیگنالها\n\
   [-]cread      allow درون_داد مقصد be received\n\
"), stdout);
#اگر_تعریف_شده CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   csN           مجموعه_2 character اندازه مقصد N بیتها_2, N درون [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop بیتها_2 در_هر character (one with '-')\n\
   [-]hup        send a hangup إشارة when the آخرین process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity بیت درون output and expect parity بیت درون درون_داد\n\
   [-]parodd     مجموعه_2 odd parity (or even parity with '-')\n\
"), stdout);
#اگر_تعریف_شده CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#پایان_اگر
      fputs (_("\
\n\
درون_داد_3 settings:\n\
   [-]brkint     breaks cause an وقفه إشارة\n\
   [-]icrnl      translate carriage أرجع مقصد newline\n\
   [-]ignbrk     ignore اكسر characters\n\
   [-]igncr      ignore carriage أرجع\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#اگر_تعریف_شده IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and افعل not flush a پر درون_داد میانگیر on a character\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   [-]inlcr      translate newline مقصد carriage أرجع\n\
   [-]inpck      enable درون_داد parity checking\n\
   [-]istrip     clear بالا (8th) بیت از درون_داد characters\n\
"), stdout);
#اگر_تعریف_شده IUTF8
      fputs (_("\
 * [-]iutf8      assume درون_داد characters are UTF-8 encoded\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters مقصد lowercase\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only شروع character\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   [-]ixoff      enable sending از شروع/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#اگر_تعریف_شده BSDLY
      fputs (_("\
 * bsN           backspace delay style, N درون [0..1]\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده CRDLY
      fputs (_("\
 * crN           carriage أرجع delay style, N درون [0..3]\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده FFDLY
      fputs (_("\
 * ffN           form feed delay style, N درون [0..1]\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده NLDLY
      fputs (_("\
 * nlN           newline delay style, N درون [0..1]\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage أرجع مقصد newline\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters لكل fill instead از NUL characters\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead از timing لكل delays\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters مقصد uppercase\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده ONLCR
      fputs (_("\
 * [-]onlcr      translate newline مقصد carriage أرجع-newline\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage أرجع\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده ONOCR
      fputs (_("\
 * [-]onocr      افعل not print carriage returns درون the اولی ستونی\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#إذا defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N درون [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N درون [0..1]\n\
"), stdout);
#پایان_اگر
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#اگر_تعریف_شده ECHOKE
      fputs (_("\
 * crtkill       kill all سطر by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all سطر by obeying the echoctl and echok settings\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters درون hat notation ('^c')\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   [-]echo       echo درون_داد characters\n\
"), stdout);
#اگر_تعریف_شده ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#اگر_تعریف_شده ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   [-]echonl     echo newline even إذا not echoing other characters\n\
"), stdout);
#اگر_تعریف_شده ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#پایان_اگر
#إذا defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with بالا latency links\n\
"), stdout);
#پایان_اگر
#إذا defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#پایان_اگر
      چاپ_کن (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#اگر_تعریف_شده VWERASE
    ", werase"
#پایان_اگر
#إذا defined CREPRINT || defined VREPRINT
    ", rprnt"
#پایان_اگر
);
      fputs (_("\
   [-]isig       enable وقفه, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after وقفه and quit special characters\n\
"), stdout);
#اگر_تعریف_شده ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try مقصد اكتب مقصد the terminal\n\
"), stdout);
#پایان_اگر
#اگر_تعریف_شده XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' لكل uppercase characters\n\
"), stdout);
#پایان_اگر
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#إذا defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters مقصد their افتراضي مقادیر\n\
   -cooked       same as raw\n\
"), stdout);
      چاپ_کن (_("\
   crt           same as %s\n\
"), "echoe"
#اگر_تعریف_شده ECHOCTL
    " echoctl"
#پایان_اگر
#اگر_تعریف_شده ECHOKE
    " echoke"
#پایان_اگر
);
      چاپ_کن (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#اگر_تعریف_شده ECHOCTL
    " echoctl"
#پایان_اگر
#اگر_تعریف_شده ECHOKE
    " echoke"
#پایان_اگر
#اگر_تعریف_شده IXANY
    " -ixany"
#پایان_اگر
);
#اگر_تعریف_شده IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   ek            erase and kill characters مقصد their افتراضي مقادیر\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#إذا defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#پایان_اگر
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      چاپ_کن (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#اگر_تعریف_شده ONLCR
   " -onlcr"
#پایان_اگر
  , "icrnl -inlcr -igncr"
#اگر_تعریف_شده ONLCR
   " onlcr"
#پایان_اگر
#اگر_تعریف_شده OCRNL
   " -ocrnl"
#پایان_اگر
#اگر_تعریف_شده ONLRET
   " -onlret"
#پایان_اگر
);
      fputs (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), stdout);
      چاپ_کن (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 زمان 0\n\
   -raw          same as cooked\n\
"),
#اگر_تعریف_شده IUCLC
   " -iuclc"
#پایان_اگر
#اگر_تعریف_شده IXANY
   " -ixany"
#پایان_اگر
#اگر_تعریف_شده IMAXBEL
   " -imaxbel"
#پایان_اگر
#اگر_تعریف_شده XCASE
   " -xcase"
#پایان_اگر
);
      چاپ_کن (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters مقصد their افتراضي مقادیر\n\
"),
   "-ixoff"
#اگر_تعریف_شده IUTF8
   " -iutf8"
#پایان_اگر
#اگر_تعریف_شده IUCLC
   " -iuclc"
#پایان_اگر
#اگر_تعریف_شده IXANY
   " -ixany"
#پایان_اگر
#اگر_تعریف_شده IMAXBEL
   " imaxbel"
#پایان_اگر
#اگر_تعریف_شده XCASE
   " -xcase"
#پایان_اگر
#اگر_تعریف_شده OLCUC
   " -olcuc"
#پایان_اگر
#اگر_تعریف_شده OCRNL
   " -ocrnl"
#پایان_اگر

 , "opost"
#اگر_تعریف_شده OFILL
   " -ofill"
#پایان_اگر
#اگر_تعریف_شده ONLCR
   " onlcr"
#پایان_اگر
#اگر_تعریف_شده ONOCR
   " -onocr"
#پایان_اگر
#اگر_تعریف_شده ONLRET
   " -onlret"
#پایان_اگر
#اگر_تعریف_شده NLDLY
   " nl0"
#پایان_اگر
#اگر_تعریف_شده CRDLY
   " cr0"
#پایان_اگر
#اگر_تعریف_شده TAB0
   " tab0"
#پایان_اگر
#اگر_تعریف_شده BSDLY
   " bs0"
#پایان_اگر
#اگر_تعریف_شده VTDLY
   " vt0"
#پایان_اگر
#اگر_تعریف_شده FFDLY
   " ff0"
#پایان_اگر

 , "isig"
#اگر_تعریف_شده TOSTOP
   " -tostop"
#پایان_اگر
#اگر_تعریف_شده OFDEL
   " -ofdel"
#پایان_اگر
#اگر_تعریف_شده ECHOPRT
   " -echoprt"
#پایان_اگر
#اگر_تعریف_شده ECHOCTL
   " echoctl"
#پایان_اگر
#اگر_تعریف_شده ECHOKE
   " echoke"
#پایان_اگر
#اگر_تعریف_شده EXTPROC
   " -extproc"
#پایان_اگر
#اگر_تعریف_شده FLUSHO
   " -flusho"
#پایان_اگر
);
      fputs (_("\
\n\
Handle the tty سطر connected مقصد standard درون_داد.  Without arguments,\n\
prints baud نرخ, سطر discipline, and deviations from stty sane.  درون_2\n\
settings, CHAR is گرفته literally, or coded as درون ^c, 0x37, 0177 or\n\
127; special مقادیر ^- or undef استفاده_شده مقصد disable special characters.\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

ساكن فراغ
apply_settings (bool checking, حرف ثابت *device_name,
                حرف * ثابت *settings, صحيح n_settings,
                بنية termios *نما_2, bool *require_مجموعه_attr)
{
#تعریف_کن check_آرگومان(arg)						\
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
      bool تطابق_یافت = false;
      bool not_مجموعه_attr = false;
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
      لكل (صحيح i = 0; نما_info[i].name != عدم; ++i)
        {
          إذا (streq (arg, نما_info[i].name))
            {
              إذا ((نما_info[i].flags & NO_SETATTR) == 0)
                {
                  تطابق_یافت = مجموعه_نما (&نما_info[i], reversed, نما_2);
                  *require_مجموعه_attr = true;
                }
              وإلا
                تطابق_یافت = not_مجموعه_attr = true;
              اكسر;
            }
        }
      إذا (!تطابق_یافت && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      إذا (!تطابق_یافت)
        {
          لكل (صحيح i = 0; control_info[i].name != عدم; ++i)
            {
              إذا (streq (arg, control_info[i].name))
                {
                  check_آرگومان (arg);
                  تطابق_یافت = true;
                  ++k;
                  مجموعه_control_char (&control_info[i], settings[k], نما_2);
                  *require_مجموعه_attr = true;
                  اكسر;
                }
            }
        }
      إذا (!تطابق_یافت || not_مجموعه_attr)
        {
          إذا (streq (arg, "ispeed"))
            {
              check_آرگومان (arg);
              ++k;
              بنية سرعت_desc ثابت ispeed = string_مقصد_baud (settings[k]);
              إذا (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              مجموعه_سرعت (درون_داد_سرعت, settings[k], ispeed, نما_2);
              إذا (checking)
                استمر;
              *require_مجموعه_attr = true;
            }
          وإلا إذا (streq (arg, "ospeed"))
            {
              check_آرگومان (arg);
              ++k;
              بنية سرعت_desc ثابت ospeed = string_مقصد_baud (settings[k]);
              إذا (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              مجموعه_سرعت (output_سرعت, settings[k], ospeed, نما_2);
              إذا (checking)
                استمر;
              *require_مجموعه_attr = true;
            }
#اگر_تعریف_شده TIOCEXT
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
#پایان_اگر
#اگر_تعریف_شده TIOCGWINSZ
          وإلا إذا (streq (arg, "rows"))
            {
              check_آرگومان (arg);
              ++k;
              إذا (checking)
                استمر;
              مجموعه_پنجره_اندازه (integer_arg (settings[k], بیشترین_صحیح), -1,
                               device_name);
            }
          وإلا إذا (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_آرگومان (arg);
              ++k;
              إذا (checking)
                استمر;
              مجموعه_پنجره_اندازه (-1, integer_arg (settings[k], بیشترین_صحیح),
                               device_name);
            }
          وإلا إذا (streq (arg, "size"))
            {
              إذا (checking)
                استمر;
              max_ستون = screen_columns ();
              current_ستون = 0;
              display_پنجره_اندازه (false, device_name);
            }
#پایان_اگر
#اگر_تعریف_شده HAVE_C_سطر
          وإلا إذا (streq (arg, "line"))
            {
              check_آرگومان (arg);
              ++k;
              uintmax_t مقدار = integer_arg (settings[k], UINTMAX_MAX);
              إذا (ckd_جمع_کن (&نما_2->c_سطر, مقدار, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_مجموعه_attr = true;
            }
#پایان_اگر
          وإلا إذا (streq (arg, "speed"))
            {
              إذا (checking)
                استمر;
              max_ستون = screen_columns ();
              display_سرعت (نما_2, false);
            }
          وإلا
            {
              بنية سرعت_desc ثابت سرعت = string_مقصد_baud (arg);
              إذا (سرعت.valid)
                {
                  مجموعه_سرعت (both_speeds, arg, سرعت, نما_2);
                  إذا (checking)
                    استمر;
                  *require_مجموعه_attr = true;
                }
              وإلا
                {
                  إذا (! recover_نما (arg, نما_2))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_مجموعه_attr = true;
                }
            }
        }
    }

  إذا (checking)
    check_سرعت (نما_2);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ساكن بنية termios نما_2;

  تعداد output_type output_type;
  صحيح optc;
  صحيح argi = 0;
  صحيح opti = 1;
  bool require_مجموعه_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  حرف *file_name = عدم;
  حرف ثابت *device_name;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = تغییریافته;
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

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

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
      ساكن بنية termios check_نما;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_نما, &require_مجموعه_attr);
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

  إذا (tcgetattr (STDIN_FILENO, &نما_2))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  إذا (verbose_output || recoverable_output || noargs)
    {
      max_ستون = screen_columns ();
      current_ستون = 0;
      display_settings (output_type, &نما_2, device_name);
      أرجع EXIT_SUCCESS;
    }

  require_مجموعه_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &نما_2, &require_مجموعه_attr);

  إذا (require_مجموعه_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ساكن بنية termios new_نما;

      إذا (tcsetattr (STDIN_FILENO, tcsetattr_options, &نما_2))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      إذا (tcgetattr (STDIN_FILENO, &new_نما))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      إذا (! eq_نما (&نما_2, &new_نما))
        {
          إذا (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              لكل (بیعلامت صحيح i = 0; i < حجم (new_نما); i++)
                {
                  بیعلامت صحيح newc = *(((بیعلامت حرف *) &new_نما) + i);
                  بیعلامت صحيح oldc = *(((بیعلامت حرف *) &نما_2) + i);
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
eq_نما (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  إذا (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#اگر_تعریف_شده HAVE_C_سطر
      || mode1->c_سطر != mode2->c_سطر
#پایان_اگر
      || !memeq (mode1->c_cc, mode2->c_cc, حجم (mode1->c_cc)))
    أرجع false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_ماسک = ~(tcflag_t) 0;
#اگر_تعریف_شده CBAUD
  cflag_ماسک &= ~(tcflag_t) CBAUD;
#پایان_اگر
#اگر_تعریف_شده CIBAUD
  cflag_ماسک &= ~(tcflag_t) CIBAUD;
#پایان_اگر

  إذا ((mode1->c_cflag ^ mode2->c_cflag) & cflag_ماسک)
    أرجع false;

  أرجع سرعت_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ساكن bool
مجموعه_نما (بنية نما_info ثابت *info, bool reversed, بنية termios *نما_2)
{
  tcflag_t *bitsp;

  إذا (reversed && (info->flags & REV) == 0)
    أرجع false;

  bitsp = نما_type_flag (info->type, نما_2);

  إذا (bitsp == عدم)
    {
      /* Combination mode. */
      إذا (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          إذا (reversed)
            نما_2->c_cflag = (نما_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            نما_2->c_cflag = (نما_2->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      وإلا إذا (streq (info->name, "oddp"))
        {
          إذا (reversed)
            نما_2->c_cflag = (نما_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            نما_2->c_cflag = (نما_2->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      وإلا إذا (streq (info->name, "nl"))
        {
          إذا (reversed)
            {
              نما_2->c_iflag = (نما_2->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              نما_2->c_oflag = (نما_2->c_oflag
#اگر_تعریف_شده ONLCR
                               | ONLCR
#پایان_اگر
                )
#اگر_تعریف_شده OCRNL
                & ~OCRNL
#پایان_اگر
#اگر_تعریف_شده ONLRET
                & ~ONLRET
#پایان_اگر
                ;
            }
          وإلا
            {
              نما_2->c_iflag = نما_2->c_iflag & ~ICRNL;
#اگر_تعریف_شده ONLCR
              نما_2->c_oflag = نما_2->c_oflag & ~ONLCR;
#پایان_اگر
            }
        }
      وإلا إذا (streq (info->name, "ek"))
        {
          نما_2->c_cc[VERASE] = CERASE;
          نما_2->c_cc[VKILL] = CKILL;
        }
      وإلا إذا (streq (info->name, "sane"))
        sane_نما (نما_2);
      وإلا إذا (streq (info->name, "cbreak"))
        {
          إذا (reversed)
            نما_2->c_lflag |= ICANON;
          وإلا
            نما_2->c_lflag &= ~ICANON;
        }
      وإلا إذا (streq (info->name, "pass8"))
        {
          إذا (reversed)
            {
              نما_2->c_cflag = (نما_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              نما_2->c_iflag |= ISTRIP;
            }
          وإلا
            {
              نما_2->c_cflag = (نما_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              نما_2->c_iflag &= ~ISTRIP;
            }
        }
      وإلا إذا (streq (info->name, "litout"))
        {
          إذا (reversed)
            {
              نما_2->c_cflag = (نما_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              نما_2->c_iflag |= ISTRIP;
              نما_2->c_oflag |= OPOST;
            }
          وإلا
            {
              نما_2->c_cflag = (نما_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              نما_2->c_iflag &= ~ISTRIP;
              نما_2->c_oflag &= ~OPOST;
            }
        }
      وإلا إذا (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          إذا ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              نما_2->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              نما_2->c_oflag |= OPOST;
              نما_2->c_lflag |= ISIG | ICANON;
#إذا VMIN == VEOF
              نما_2->c_cc[VEOF] = CEOF;
#پایان_اگر
#إذا VTIME == VEOL
              نما_2->c_cc[VEOL] = CEOL;
#پایان_اگر
            }
          وإلا
            {
              /* Raw mode. */
              نما_2->c_iflag = 0;
              نما_2->c_oflag &= ~OPOST;
              نما_2->c_lflag &= ~(ISIG | ICANON
#اگر_تعریف_شده XCASE
                                 | XCASE
#پایان_اگر
                );
              نما_2->c_cc[VMIN] = 1;
              نما_2->c_cc[VTIME] = 0;
            }
        }
#اگر_تعریف_شده IXANY
      وإلا إذا (streq (info->name, "decctlq"))
        {
          إذا (reversed)
            نما_2->c_iflag |= IXANY;
          وإلا
            نما_2->c_iflag &= ~IXANY;
        }
#پایان_اگر
#اگر_تعریف_شده TABDLY
      وإلا إذا (streq (info->name, "tabs"))
        {
          إذا (reversed)
            نما_2->c_oflag = (نما_2->c_oflag & ~TABDLY) | TAB3;
          وإلا
            نما_2->c_oflag = (نما_2->c_oflag & ~TABDLY) | TAB0;
        }
#وإلا
# ifdef OXTABS
      وإلا إذا (streq (info->name, "tabs"))
        {
          إذا (reversed)
            نما_2->c_oflag = نما_2->c_oflag | OXTABS;
          وإلا
            نما_2->c_oflag = نما_2->c_oflag & ~OXTABS;
        }
# endif
#پایان_اگر
#إذا defined XCASE && defined IUCLC && defined OLCUC
      وإلا إذا (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          إذا (reversed)
            {
              نما_2->c_lflag &= ~XCASE;
              نما_2->c_iflag &= ~IUCLC;
              نما_2->c_oflag &= ~OLCUC;
            }
          وإلا
            {
              نما_2->c_lflag |= XCASE;
              نما_2->c_iflag |= IUCLC;
              نما_2->c_oflag |= OLCUC;
            }
        }
#پایان_اگر
      وإلا إذا (streq (info->name, "crt"))
        نما_2->c_lflag |= ECHOE
#اگر_تعریف_شده ECHOCTL
          | ECHOCTL
#پایان_اگر
#اگر_تعریف_شده ECHOKE
          | ECHOKE
#پایان_اگر
          ;
      وإلا إذا (streq (info->name, "dec"))
        {
          نما_2->c_cc[VINTR] = 3;	/* ^C */
          نما_2->c_cc[VERASE] = 127;	/* DEL */
          نما_2->c_cc[VKILL] = 21;	/* ^U */
          نما_2->c_lflag |= ECHOE
#اگر_تعریف_شده ECHOCTL
            | ECHOCTL
#پایان_اگر
#اگر_تعریف_شده ECHOKE
            | ECHOKE
#پایان_اگر
            ;
#اگر_تعریف_شده IXANY
          نما_2->c_iflag &= ~IXANY;
#پایان_اگر
        }
    }
  وإلا إذا (reversed)
    *bitsp = *bitsp & ~info->ماسک & ~info->بیتها_2;
  وإلا
    *bitsp = (*bitsp & ~info->ماسک) | info->بیتها_2;

  أرجع true;
}

ساكن فراغ
مجموعه_control_char (بنية control_info ثابت *info, حرف ثابت *arg,
                  بنية termios *نما_2)
{
  بیعلامت دراز صحيح مقدار;

  إذا (streq (info->name, "min") || streq (info->name, "time"))
    مقدار = integer_arg (arg, TYPE_بیشینه (cc_t));
  وإلا إذا (arg[0] == '\0' || arg[1] == '\0')
    مقدار = مقصد_uchar (arg[0]);
  وإلا إذا (streq (arg, "^-") || streq (arg, "undef"))
    مقدار = _POSIX_VDISABLE;
  وإلا إذا (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      إذا (arg[1] == '?')
        مقدار = 127;
      وإلا
        مقدار = مقصد_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  وإلا
    مقدار = integer_arg (arg, TYPE_بیشینه (cc_t));
  نما_2->c_cc[info->جابه_جایی] = مقدار;
}

ساكن فراغ
مجموعه_سرعت (تعداد سرعت_setting type, حرف ثابت *arg,
           بنية سرعت_desc ثابت سرعت, بنية termios *نما_2)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (سرعت.valid);

  إذا (type == درون_داد_سرعت || type == both_speeds)
    {
      آخرین_ibaud = سرعت;
      إذا (cfsetispeed (نما_2, سرعت.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  إذا (type == output_سرعت || type == both_speeds)
    {
      آخرین_obaud = سرعت;
      إذا (cfsetospeed (نما_2, سرعت.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#اگر_تعریف_شده TIOCGWINSZ

ساكن صحيح
get_win_اندازه (صحيح fd, بنية winsize *win)
{
  صحيح err = ioctl (fd, TIOCGWINSZ, (حرف *) win);
  أرجع err;
}

ساكن فراغ
مجموعه_پنجره_اندازه (صحيح ردیفها, صحيح تعداد_ستون_2, حرف ثابت *device_name)
{
  بنية winsize win;

  إذا (get_win_اندازه (STDIN_FILENO, &win))
    {
      إذا (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, حجم (win));
    }

  إذا (ردیفها >= 0)
    win.ws_ردیف = ردیفها;
  إذا (تعداد_ستون_2 >= 0)
    win.ws_ستون = تعداد_ستون_2;

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

  إذا (win.ws_ردیف == 0 || win.ws_ستون == 0)
    {
      بنية ttysize ttysz;

      ttysz.ts_سطرها = win.ws_ردیف;
      ttysz.ts_تعداد_ستون = win.ws_ستون;

      win.ws_ردیف = 1;
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
display_پنجره_اندازه (bool fancy, حرف ثابت *device_name)
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
             win.ws_ردیف, win.ws_ستون);
      إذا (!fancy)
        current_ستون = 0;
    }
}
#پایان_اگر

ساكن صحيح
screen_columns (فراغ)
{
#اگر_تعریف_شده TIOCGWINSZ
  بنية winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  إذا (get_win_اندازه (STDOUT_FILENO, &win) == 0 && 0 < win.ws_ستون)
    أرجع win.ws_ستون;
#پایان_اگر
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    حرف *ستون_string = getenv ("COLUMNS");
    دراز صحيح n_columns;
    إذا (!(ستون_string != عدم
          && xstrtol (ستون_string, عدم, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= بیشترین_صحیح))
      n_columns = 80;
    أرجع n_columns;
  }
}

ATTRIBUTE_خالص
ساكن tcflag_t *
نما_type_flag (تعداد نما_type type, بنية termios *نما_2)
{
  بدل (type)
    {
    حالة control:
      أرجع &نما_2->c_cflag;

    حالة درون_داد:
      أرجع &نما_2->c_iflag;

    حالة output:
      أرجع &نما_2->c_oflag;

    حالة local:
      أرجع &نما_2->c_lflag;

    حالة combination:
      أرجع عدم;

    افتراضي:
      unreachable ();
    }
}

ساكن فراغ
display_settings (تعداد output_type output_type, بنية termios *نما_2,
                  حرف ثابت *device_name)
{
  بدل (output_type)
    {
    حالة تغییریافته:
      display_تغییریافته (نما_2);
      اكسر;

    حالة all:
      display_all (نما_2, device_name);
      اكسر;

    حالة recoverable:
      display_recoverable (نما_2);
      اكسر;
    }
}

ساكن فراغ
display_تغییریافته (بنية termios *نما_2)
{
  bool خالی_سطر;
  tcflag_t *bitsp;
  بیعلامت دراز ماسک;
  تعداد نما_type prev_type = control;

  display_سرعت (نما_2, true);
#اگر_تعریف_شده HAVE_C_سطر
  wrapf ("line = %d;", نما_2->c_سطر);
#پایان_اگر
  حرف_چاپ_کن ('\n');
  current_ستون = 0;

  خالی_سطر = true;
  لكل (صحيح i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      إذا (نما_2->c_cc[control_info[i].جابه_جایی] == control_info[i].saneval)
        استمر;

#اگر_تعریف_شده VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      إذا (streq (control_info[i].name, "flush"))
        استمر;
#پایان_اگر
      /* If swtch is the same as susp, don't print both.  */
#إذا VSWTCH == VSUSP
      إذا (streq (control_info[i].name, "swtch"))
        استمر;
#پایان_اگر
      /* If eof uses the same slot as min, only print whichever applies.  */
#إذا VEOF == VMIN
      إذا ((نما_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#پایان_اگر

      خالی_سطر = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (نما_2->c_cc[control_info[i].جابه_جایی]));
    }
  إذا ((نما_2->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (بیعلامت دراز صحيح) نما_2->c_cc[VMIN],
             (بیعلامت دراز صحيح) نما_2->c_cc[VTIME]);
    }
  وإلا إذا (!خالی_سطر)
    حرف_چاپ_کن ('\n');
  current_ستون = 0;

  خالی_سطر = true;
  لكل (صحيح i = 0; نما_info[i].name != عدم; ++i)
    {
      إذا (نما_info[i].flags & OMIT)
        استمر;
      إذا (نما_info[i].type != prev_type)
        {
          إذا (!خالی_سطر)
            {
              حرف_چاپ_کن ('\n');
              current_ستون = 0;
              خالی_سطر = true;
            }
          prev_type = نما_info[i].type;
        }

      bitsp = نما_type_flag (نما_info[i].type, نما_2);
      ماسک = نما_info[i].ماسک ? نما_info[i].ماسک : نما_info[i].بیتها_2;
      إذا ((*bitsp & ماسک) == نما_info[i].بیتها_2)
        {
          إذا (نما_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", نما_info[i].name);
              خالی_سطر = false;
            }
        }
      وإلا إذا ((نما_info[i].flags & (SANE_مجموعه | REV)) == (SANE_مجموعه | REV))
        {
          wrapf ("-%s", نما_info[i].name);
          خالی_سطر = false;
        }
    }
  إذا (!خالی_سطر)
    حرف_چاپ_کن ('\n');
  current_ستون = 0;
}

ساكن فراغ
display_all (بنية termios *نما_2, حرف ثابت *device_name)
{
  tcflag_t *bitsp;
  بیعلامت دراز ماسک;
  تعداد نما_type prev_type = control;

  display_سرعت (نما_2, true);
#اگر_تعریف_شده TIOCGWINSZ
  display_پنجره_اندازه (true, device_name);
#پایان_اگر
#اگر_تعریف_شده HAVE_C_سطر
  wrapf ("line = %d;", نما_2->c_سطر);
#پایان_اگر
  حرف_چاپ_کن ('\n');
  current_ستون = 0;

  لكل (صحيح i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#اگر_تعریف_شده VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      إذا (streq (control_info[i].name, "flush"))
        استمر;
#پایان_اگر
      /* If swtch is the same as susp, don't print both.  */
#إذا VSWTCH == VSUSP
      إذا (streq (control_info[i].name, "swtch"))
        استمر;
#پایان_اگر
      /* If eof uses the same slot as min, only print whichever applies.  */
#إذا VEOF == VMIN
      إذا ((نما_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#پایان_اگر
      wrapf ("%s = %s;", control_info[i].name,
             visible (نما_2->c_cc[control_info[i].جابه_جایی]));
    }
#إذا VEOF == VMIN
  إذا ((نما_2->c_lflag & ICANON) == 0)
#پایان_اگر
    wrapf ("min = %lu; time = %lu;",
           (بیعلامت دراز صحيح) نما_2->c_cc[VMIN],
           (بیعلامت دراز صحيح) نما_2->c_cc[VTIME]);
  إذا (current_ستون != 0)
    حرف_چاپ_کن ('\n');
  current_ستون = 0;

  لكل (صحيح i = 0; نما_info[i].name != عدم; ++i)
    {
      إذا (نما_info[i].flags & OMIT)
        استمر;
      إذا (نما_info[i].type != prev_type)
        {
          حرف_چاپ_کن ('\n');
          current_ستون = 0;
          prev_type = نما_info[i].type;
        }

      bitsp = نما_type_flag (نما_info[i].type, نما_2);
      ماسک = نما_info[i].ماسک ? نما_info[i].ماسک : نما_info[i].بیتها_2;
      إذا ((*bitsp & ماسک) == نما_info[i].بیتها_2)
        wrapf ("%s", نما_info[i].name);
      وإلا إذا (نما_info[i].flags & REV)
        wrapf ("-%s", نما_info[i].name);
    }
  حرف_چاپ_کن ('\n');
  current_ستون = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ساكن فراغ
check_سرعت (بنية termios *نما_2)
{
  إذا (آخرین_ibaud.valid && آخرین_obaud.valid)
    {
      إذا (cfgetispeed (نما_2) != آخرین_ibaud.baud
          || cfgetospeed (نما_2) != آخرین_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_مقصد_مقدار (آخرین_ibaud.baud),
               baud_مقصد_مقدار (آخرین_obaud.baud));
    }
}

ساكن فراغ
display_سرعت (بنية termios *نما_2, bool fancy)
{
  إذا (cfgetispeed (نما_2) == 0 || cfgetispeed (نما_2) == cfgetospeed (نما_2))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_مقصد_مقدار (cfgetospeed (نما_2)));
  وإلا
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_مقصد_مقدار (cfgetispeed (نما_2)),
           baud_مقصد_مقدار (cfgetospeed (نما_2)));
  إذا (!fancy)
    current_ستون = 0;
}

ساكن فراغ
display_recoverable (بنية termios *نما_2)
{
  چاپ_کن ("%lx:%lx:%lx:%lx",
          (بیعلامت دراز صحيح) نما_2->c_iflag,
          (بیعلامت دراز صحيح) نما_2->c_oflag,
          (بیعلامت دراز صحيح) نما_2->c_cflag,
          (بیعلامت دراز صحيح) نما_2->c_lflag);
  لكل (size_t i = 0; i < NCCS; ++i)
    چاپ_کن (":%lx", (بیعلامت دراز صحيح) نما_2->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#اگر_تعریف_شده CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (نما_2->c_cflag & CBAUD) == BOTHER;
#  إذا defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((نما_2->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#پایان_اگر
  إذا (separate_speeds)
    چاپ_کن (":%lx:%lx", (بیعلامت دراز صحيح) cfgetispeed (نما_2),
            (بیعلامت دراز صحيح) cfgetospeed (نما_2));

  حرف_چاپ_کن ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#تعریف_کن STRTOUL_TYPE(TYPE)                                               \
  ساكن صحيح                                                             \
  strtoul_##TYPE (حرف ثابت *s, صحيح پایه_2, حرف **p, TYPE *نتیجه,       \
                  حرف delim)                                            \
  {                                                                      \
    بیعلامت دراز ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, پایه_2);                                           \
    إذا (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      أرجع -1;                                                         \
    *نتیجه = ul;                                                        \
    أرجع 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (سرعت_t)
#لغو_تعریف STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#ضمن "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ساكن bool
recover_نما (حرف ثابت *arg, بنية termios *نما_2)
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
  نما_2->c_iflag = flag[0];
  نما_2->c_oflag = flag[1];
  نما_2->c_cflag = flag[2];
  نما_2->c_lflag = flag[3];

  لكل (size_t i = 0; i < NCCS; ++i)
    {
      حرف *p;
      حرف delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      إذا (strtoul_cc_t (s, 16, &p, نما_2->c_cc + i, delim) != 0)
        أرجع false;
      /* Accept the old format, which has no separate speed fields.  */
      إذا (! *p)
        {
#اگر_تعریف_شده CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_سرعت_T_SANE
          بنية سرعت_desc ospeed = cbaud_مقصد_سرعت (نما_2->c_cflag & CBAUD);
          إذا (ospeed.valid && cfsetospeed (نما_2, ospeed.baud) != 0)
            أرجع false;
#  إذا defined CIBAUD && defined IBSHIFT
          بنية سرعت_desc ispeed =
            cbaud_مقصد_سرعت ((نما_2->c_cflag & CIBAUD) >> IBSHIFT);
          إذا (ispeed.valid && cfsetispeed (نما_2, ispeed.baud) != 0)
            أرجع false;
#  endif
# endif
          آخرین_ibaud.baud = cfgetispeed (نما_2);
          آخرین_obaud.baud = cfgetospeed (نما_2);
#پایان_اگر
          أرجع true;
        }
      s = p + 1;
    }

  سرعت_t سرعت[2];
  لكل (size_t i = 0; i < 2; i++)
    {
      حرف *p;
      حرف delim = i == 0 ? ':' : '\0';
      إذا (strtoul_سرعت_t (s, 16, &p, سرعت + i, delim) != 0)
        أرجع false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  إذا (cfsetospeed (نما_2, سرعت[1]) != 0
      || cfsetispeed (نما_2, سرعت[0]) != 0)
    أرجع false;

  آخرین_ibaud.baud = سرعت[0] == 0 ? cfgetispeed (نما_2) : سرعت[0];
  آخرین_obaud.baud = سرعت[1];
  أرجع true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ساكن bool
سرعت_matches (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  سرعت_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  سرعت_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  إذا (speed1[1] == 0)
    speed1[1] = speed1[0];
  إذا (speed2[1] == 0)
    speed2[1] = speed2[0];

  بیعلامت صحيح بیتها_2 = 2;  /* Start and stop bits.  */
  بیتها_2 += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  بیتها_2 += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  بدل (mode1->c_cflag & CSIZE)
    {
      حالة CS5: بیتها_2 += 5; اكسر;
      حالة CS6: بیتها_2 += 6; اكسر;
      حالة CS7: بیتها_2 += 7; اكسر;
      افتراضي: بیتها_2 += 8; اكسر;
    }

  لكل (صحيح i = 0; i < 2; i++)
    {
      بیعلامت دراز صحيح value1;
      بیعلامت دراز صحيح value2;
      بیعلامت دراز صحيح delta;

      إذا (speed1[i] == speed2[i])
        استمر;

      value1 = baud_مقصد_مقدار (speed1[i]);
      value2 = baud_مقصد_مقدار (speed2[i]);

#اگر_تعریف_نشده TERMIOS_سرعت_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      إذا (value1 == ULONG_MAX || value2 == ULONG_MAX)
        أرجع false;
#پایان_اگر

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      إذا (delta > value1 / (بیتها_2 * 4))
        أرجع false;
    }

  أرجع true;
}

ATTRIBUTE_خالص
ساكن بنية سرعت_desc
string_مقصد_baud (حرف ثابت *arg)
{
  حرف *ep;
  بیعلامت دراز مقدار;
  بیعلامت حرف c;

  /* Explicitly disallow negative numbers.  */
  طالما (c_isspace (*arg))
    arg++;
  إذا (*arg == '-')
    اذهب invalid;

  /* Empty does not mean 0.  */
  إذا (! *arg)
    اذهب invalid;

  مقدار = strtoul (arg, &ep, 10);

  c = *ep++;
  إذا (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      إذا (c)
        {
          بیعلامت حرف d = c - '0';
          إذا (d > 5)
            مقدار++;
          وإلا إذا (d == 5)
            {
              طالما ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              إذا (c)
                مقدار++;                /* Nonzero, round up */
              وإلا
                مقدار += (مقدار & 1);   /* Exactly in the middle, round even */
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
        أرجع (بنية سرعت_desc) { .baud = B19200, .valid = true };
      وإلا إذا (streq (arg, "extb"))
        أرجع (بنية سرعت_desc) { .baud = B38400, .valid = true };
      وإلا
        اذهب invalid;
    }

  أرجع مقدار_مقصد_baud (مقدار);

 invalid:
  أرجع (بنية سرعت_desc) { .baud = (سرعت_t) -1, .valid = false };
}

ساكن فراغ
sane_نما (بنية termios *نما_2)
{
  tcflag_t *bitsp;

  لكل (صحيح i = 0; control_info[i].name; ++i)
    {
#إذا VMIN == VEOF
      إذا (streq (control_info[i].name, "min"))
        اكسر;
#پایان_اگر
      نما_2->c_cc[control_info[i].جابه_جایی] = control_info[i].saneval;
    }

  لكل (صحيح i = 0; نما_info[i].name != عدم; ++i)
    {
      إذا (نما_info[i].flags & NO_SETATTR)
        استمر;

      إذا (نما_info[i].flags & SANE_مجموعه)
        {
          bitsp = نما_type_flag (نما_info[i].type, نما_2);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~نما_info[i].ماسک) | نما_info[i].بیتها_2;
        }
      وإلا إذا (نما_info[i].flags & SANE_UNSET)
        {
          bitsp = نما_type_flag (نما_info[i].type, نما_2);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~نما_info[i].ماسک & ~نما_info[i].بیتها_2;
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
