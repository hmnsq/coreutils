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

#شامل <config.h>

#اگر_تعریف_شدہ TERMIOS_NEEDS_XOPEN_ماخذ
# define XOPEN_ماخذ
#اگر_ختم

#شامل <stdio.h>
#شامل <sys/types.h>

#شامل <termios.h>
#اگر HAVE_STROPTS_H
# include <stropts.h>
#اگر_ختم
#شامل <sys/ioctl.h>

#اگر_تعریف_شدہ WINSIZE_اندر_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#اگر_ختم
#اگر_تعریف_شدہ GWINSZ_اندر_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#اگر_ختم
#شامل <getopt.h>
#شامل <stdarg.h>

#شامل "system.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "fd-reopen.h"
#شامل "quote.h"
#شامل "xdectoint.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "stty"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

#اگر_غیر_تعریف_شدہ _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#اگر_ختم

#تعریف_کرو Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#اگر_غیر_تعریف_شدہ CINTR
# define CINTR Control ('c')
#اگر_ختم
#اگر_غیر_تعریف_شدہ CQUIT
# define CQUIT 28
#اگر_ختم
#اگر_غیر_تعریف_شدہ CERASE
# define CERASE 127
#اگر_ختم
#اگر_غیر_تعریف_شدہ CKILL
# define CKILL Control ('u')
#اگر_ختم
#اگر_غیر_تعریف_شدہ CEOF
# define CEOF Control ('d')
#اگر_ختم
#اگر_غیر_تعریف_شدہ CEOL
# define CEOL _POSIX_VDISABLE
#اگر_ختم
#اگر_غیر_تعریف_شدہ CSTART
# define CSTART Control ('q')
#اگر_ختم
#اگر_غیر_تعریف_شدہ CSTOP
# define CSTOP Control ('s')
#اگر_ختم
#اگر_غیر_تعریف_شدہ CSUSP
# define CSUSP Control ('z')
#اگر_ختم
#اگر defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#اگر_ختم
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#اگر defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#اگر_ختم
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#اگر defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# اگر defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#اگر_ختم
#اگر defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#اگر_ختم

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#اگر defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#اگر_ختم

#اگر defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#اگر_ختم
#اگر defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#اگر_ختم
#اگر defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#اگر_ختم
#اگر defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#اگر_ختم
#اگر defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#اگر_ختم
#اگر defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#اگر_ختم
#اگر defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#اگر_ختم
#اگر defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#اگر_ختم
#اگر defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#اگر_ختم
#اگر defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#اگر_ختم
#اگر defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#اگر_ختم
#اگر defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#اگر_ختم
#اگر defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#اگر_ختم

/* Which speeds to set.  */
شمار رفتار_setting
  {
    داخلہ_رفتار, output_رفتار, both_speeds
  };

/* What to output and how.  */
شمار output_type
  {
    بدلا, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
شمار منوال_type
  {
    control, داخلہ, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#تعریف_کرو SANE_سیٹ 1		/* Set in 'sane' mode. */
#تعریف_کرو SANE_UNSET 2		/* Unset in 'sane' mode. */
#تعریف_کرو REV 4			/* Can be turned off by prepending '-'. */
#تعریف_کرو OMIT 8			/* Don't display value. */
#تعریف_کرو NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
ساخت منوال_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    شمار منوال_type type;	/* Which structure element to change. */
    حرف flags;			/* Setting and display options.  */
    غیرعلامتی طویل بٹ_2;		/* Bits to set for this mode.  */
    غیرعلامتی طویل نقاب;		/* Other bits to turn off for this mode.  */
  };

ساکن ساخت منوال_info ثابت منوال_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#اگر_تعریف_شدہ CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#اگر_ختم
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_سیٹ | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#اگر_تعریف_شدہ CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#اگر_ختم
#اگر_تعریف_شدہ CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#اگر_ختم

  {"ignbrk", داخلہ, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", داخلہ, SANE_سیٹ | REV, BRKINT, 0},
  {"ignpar", داخلہ, REV, IGNPAR, 0},
  {"parmrk", داخلہ, REV, PARMRK, 0},
  {"inpck", داخلہ, REV, INPCK, 0},
  {"istrip", داخلہ, REV, ISTRIP, 0},
  {"inlcr", داخلہ, SANE_UNSET | REV, INLCR, 0},
  {"igncr", داخلہ, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", داخلہ, SANE_سیٹ | REV, ICRNL, 0},
  {"ixon", داخلہ, REV, IXON, 0},
  {"ixoff", داخلہ, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", داخلہ, REV | OMIT, IXOFF, 0},
#اگر_تعریف_شدہ IUCLC
  {"iuclc", داخلہ, SANE_UNSET | REV, IUCLC, 0},
#اگر_ختم
#اگر_تعریف_شدہ IXANY
  {"ixany", داخلہ, SANE_UNSET | REV, IXANY, 0},
#اگر_ختم
#اگر_تعریف_شدہ IMAXBEL
  {"imaxbel", داخلہ, SANE_سیٹ | REV, IMAXBEL, 0},
#اگر_ختم
#اگر_تعریف_شدہ IUTF8
  {"iutf8", داخلہ, SANE_UNSET | REV, IUTF8, 0},
#اگر_ختم

  {"opost", output, SANE_سیٹ | REV, OPOST, 0},
#اگر_تعریف_شدہ OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#اگر_ختم
#اگر_تعریف_شدہ OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#اگر_ختم
#اگر_تعریف_شدہ ONLCR
  {"onlcr", output, SANE_سیٹ | REV, ONLCR, 0},
#اگر_ختم
#اگر_تعریف_شدہ ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#اگر_ختم
#اگر_تعریف_شدہ ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#اگر_ختم
#اگر_تعریف_شدہ OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#اگر_ختم
#اگر_تعریف_شدہ OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#اگر_ختم
#اگر_تعریف_شدہ NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_سیٹ, NL0, NLDLY},
#اگر_ختم
#اگر_تعریف_شدہ CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_سیٹ, CR0, CRDLY},
#اگر_ختم
#اگر_تعریف_شدہ TABDLY
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
  {"tab0", output, SANE_سیٹ, TAB0, TABDLY},
# endif
#ورنہ
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#اگر_ختم
#اگر_تعریف_شدہ BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_سیٹ, BS0, BSDLY},
#اگر_ختم
#اگر_تعریف_شدہ VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_سیٹ, VT0, VTDLY},
#اگر_ختم
#اگر_تعریف_شدہ FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_سیٹ, FF0, FFDLY},
#اگر_ختم

  {"isig", local, SANE_سیٹ | REV, ISIG, 0},
  {"icanon", local, SANE_سیٹ | REV, ICANON, 0},
#اگر_تعریف_شدہ IEXTEN
  {"iexten", local, SANE_سیٹ | REV, IEXTEN, 0},
#اگر_ختم
  {"echo", local, SANE_سیٹ | REV, ECHO, 0},
  {"echoe", local, SANE_سیٹ | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_سیٹ | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#اگر_تعریف_شدہ XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#اگر_ختم
#اگر_تعریف_شدہ TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#اگر_ختم
#اگر_تعریف_شدہ ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#اگر_ختم
#اگر_تعریف_شدہ ECHOCTL
  {"echoctl", local, SANE_سیٹ | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#اگر_ختم
#اگر_تعریف_شدہ ECHOKE
  {"echoke", local, SANE_سیٹ | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#اگر_ختم
#اگر_تعریف_شدہ FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#اگر_ختم
#اگر defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#وگرنہ_اگر defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#اگر_ختم

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
#اگر_تعریف_شدہ IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#اگر_ختم
#اگر defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#اگر_ختم
#اگر defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#اگر_ختم
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {تہی, control, 0, 0, 0}
};

/* Control character settings.  */
ساخت control_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t انحراف;		/* Offset in c_cc.  */
  };

/* Control characters. */

ساکن ساخت control_info ثابت control_info[] =
{
  {"intr", CINTR, VINTR},
  {"quit", CQUIT, VQUIT},
  {"erase", CERASE, VERASE},
  {"kill", CKILL, VKILL},
  {"eof", CEOF, VEOF},
  {"eol", CEOL, VEOL},
#اگر_تعریف_شدہ VEOL2
  {"eol2", CEOL2, VEOL2},
#اگر_ختم
#اگر_تعریف_شدہ VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#اگر_ختم
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#اگر_تعریف_شدہ VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#اگر_ختم
#اگر_تعریف_شدہ VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#ورنہ
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#اگر_ختم
#اگر_تعریف_شدہ VWERASE
  {"werase", CWERASE, VWERASE},
#اگر_ختم
#اگر_تعریف_شدہ VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#اگر_ختم
#اگر_تعریف_شدہ VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#اگر_ختم
#اگر_تعریف_شدہ VSTATUS
  {"status", CSTATUS, VSTATUS},
#اگر_ختم

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {تہی, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
ساخت رفتار_desc
{
  /* The baud rate.  */
  رفتار_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ساکن حرف ثابت *visible (cc_t ch);
ساکن غیرعلامتی طویل صحیح baud_مقصد_قدر (رفتار_t رفتار);
ساکن bool recover_منوال (حرف ثابت *arg, ساخت termios *منوال_2);
ساکن صحیح screen_columns (خالی);
ساکن bool سیٹ_منوال (ساخت منوال_info ثابت *info, bool reversed,
                      ساخت termios *منوال_2);
ساکن bool eq_منوال (ساخت termios ثابت *mode1,
                     ساخت termios ثابت *mode2);
ساکن bool رفتار_matches (ساخت termios ثابت *mode1,
                           ساخت termios ثابت *mode2);
ساکن uintmax_t integer_arg (حرف ثابت *s, uintmax_t max);
ساکن ساخت رفتار_desc string_مقصد_baud (حرف ثابت *arg);
ساکن tcflag_t *منوال_type_flag (شمار منوال_type type, ساخت termios *منوال_2);
ساکن خالی display_all (ساخت termios *منوال_2, حرف ثابت *device_name);
ساکن خالی display_بدلا (ساخت termios *منوال_2);
ساکن خالی display_recoverable (ساخت termios *منوال_2);
ساکن خالی display_settings (شمار output_type output_type,
                              ساخت termios *منوال_2,
                              حرف ثابت *device_name);
ساکن خالی check_رفتار (ساخت termios *منوال_2);
ساکن خالی display_رفتار (ساخت termios *منوال_2, bool fancy);
ساکن خالی display_کھڑکی_سائز (bool fancy, حرف ثابت *device_name);
ساکن خالی sane_منوال (ساخت termios *منوال_2);
ساکن خالی سیٹ_control_char (ساخت control_info ثابت *info,
                              حرف ثابت *arg,
                              ساخت termios *منوال_2);
ساکن خالی سیٹ_رفتار (شمار رفتار_setting type, حرف ثابت *arg,
                       ساخت رفتار_desc ثابت رفتار, ساخت termios *منوال_2);
ساکن خالی سیٹ_کھڑکی_سائز (صحیح صفیں, صحیح ستون_تعداد_2, حرف ثابت *device_name);

/* The width of the screen, for output wrapping. */
ساکن صحیح max_ستون;

/* Current position, to know when to wrap. */
ساکن صحیح current_ستون;

/* Default "drain" mode for tcsetattr.  */
ساکن صحیح tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ساکن bool dev_debug;

/* Record last speed set for correlation.  */
ساکن ساخت رفتار_desc آخری_ibaud =
  { .baud = (رفتار_t) -1, .valid = false };
ساکن ساخت رفتار_desc آخری_obaud =
  { .baud = (رفتار_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

ساکن ساخت option ثابت longopts[] =
{
  {"all", no_دلیل, تہی, 'a'},
  {"save", no_دلیل, تہی, 'g'},
  {"file", required_دلیل, تہی, 'F'},
  {"-debug", no_دلیل, تہی, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((لکھو, 1, 2))
ساکن خالی
wrapf (حرف ثابت *پیغام_2,...)
{
  va_list دلائل;
  حرف *buf;
  صحیح buflen;

  va_آغاز (دلائل, پیغام_2);
  buflen = vasprintf (&buf, پیغام_2, دلائل);
  va_انتہا (دلائل);

  اگر (buflen < 0)
    xalloc_die ();

  اگر (0 < current_ستون)
    {
      اگر (max_ستون - current_ستون <= buflen)
        {
          حرف_لکھو ('\n');
          current_ستون = 0;
        }
      ورنہ
        {
          حرف_لکھو (' ');
          current_ستون++;
        }
    }

  fputs (buf, stdout);
  آزاد (buf);
  current_ستون += buflen;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Print or change terminal characteristics.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings اندر human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings اندر a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  open and use DEVICE instead کا standard داخلہ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system متعین_کرتا which settings are دستیاب.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#اگر_تعریف_شدہ VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding کا output\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop اشارہ once داخلہ flushed\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   eof CHAR      CHAR will send an انتہا کا file (terminate the داخلہ)\n\
   eol CHAR      CHAR will انتہا the سطر\n\
"), stdout);
#اگر_تعریف_شدہ VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR برائے ending the سطر\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   erase CHAR    CHAR will erase the آخری character typed\n\
   intr CHAR     CHAR will send an مداخلت اشارہ\n\
   kill CHAR     CHAR will erase the current سطر\n\
"), stdout);
#اگر_تعریف_شدہ VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the اگلا character quoted\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ VSTATUS
      fputs (_("\
 * حالت_2 CHAR   CHAR will send an info اشارہ\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   quit CHAR     CHAR will send a quit اشارہ\n\
"), stdout);
#اگر defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current سطر\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   آغاز CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop اشارہ\n\
"), stdout);
#اگر_تعریف_شدہ VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will انتخاب مقصد a different shell layer\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the آخری لفظ_2 typed\n\
"), stdout);
#اگر_ختم
      fputs (_("\
\n\
Special settings:\n\
   N             سیٹ_2 the داخلہ and output speeds مقصد N bauds\n\
"), stdout);
#اگر_تعریف_شدہ TIOCGWINSZ
      fputs (_("\
   ستون_تعداد_2 N        tell the kernel that the terminal has N columns\n\
 * columns N     same as ستون_تعداد_2 N\n\
"), stdout);
#اگر_ختم
      لکھو (_("\
 * [-]drain      wait برائے ترسیل before applying settings (%s by طےشدہ)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      سیٹ_2 the داخلہ رفتار مقصد N\n\
"), stdout);
#اگر_تعریف_شدہ HAVE_C_سطر
      fputs (_("\
 * سطر N        use سطر discipline N\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   min N         with -icanon, سیٹ_2 N characters minimum برائے a completed اقرأ\n\
   ospeed N      سیٹ_2 the output رفتار مقصد N\n\
"), stdout);
#اگر_تعریف_شدہ TIOCGWINSZ
      fputs (_("\
   صفیں N        tell the kernel that the terminal has N صفیں\n\
   سائز          print the نمبر کا صفیں and columns according مقصد the kernel\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   رفتار         print the terminal رفتار\n\
   وقت N        with -icanon, سیٹ_2 اقرأ timeout کا N tenths کا a دوسرا\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control اشارات\n\
   [-]cread      allow داخلہ مقصد be received\n\
"), stdout);
#اگر_تعریف_شدہ CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   csN           سیٹ_2 character سائز مقصد N بٹ_2, N اندر [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop بٹ_2 فی character (one with '-')\n\
   [-]hup        send a hangup اشارہ when the آخری process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity ثنائی_ہندسہ اندر output and expect parity ثنائی_ہندسہ اندر داخلہ\n\
   [-]parodd     سیٹ_2 odd parity (or even parity with '-')\n\
"), stdout);
#اگر_تعریف_شدہ CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#اگر_ختم
      fputs (_("\
\n\
داخلہ_3 settings:\n\
   [-]brkint     breaks cause an مداخلت اشارہ\n\
   [-]icrnl      translate carriage واپس مقصد newline\n\
   [-]ignbrk     ignore توڑو characters\n\
   [-]igncr      ignore carriage واپس\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#اگر_تعریف_شدہ IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and کرو not flush a بھرا داخلہ بفر on a character\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   [-]inlcr      translate newline مقصد carriage واپس\n\
   [-]inpck      enable داخلہ parity checking\n\
   [-]istrip     clear بلند (8th) ثنائی_ہندسہ کا داخلہ characters\n\
"), stdout);
#اگر_تعریف_شدہ IUTF8
      fputs (_("\
 * [-]iutf8      assume داخلہ characters are UTF-8 encoded\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters مقصد lowercase\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only آغاز character\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   [-]ixoff      enable sending کا آغاز/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#اگر_تعریف_شدہ BSDLY
      fputs (_("\
 * bsN           backspace delay style, N اندر [0..1]\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ CRDLY
      fputs (_("\
 * crN           carriage واپس delay style, N اندر [0..3]\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ FFDLY
      fputs (_("\
 * ffN           form feed delay style, N اندر [0..1]\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ NLDLY
      fputs (_("\
 * nlN           newline delay style, N اندر [0..1]\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage واپس مقصد newline\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters برائے fill instead کا NUL characters\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead کا timing برائے delays\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters مقصد uppercase\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ ONLCR
      fputs (_("\
 * [-]onlcr      translate newline مقصد carriage واپس-newline\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage واپس\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ ONOCR
      fputs (_("\
 * [-]onocr      کرو not print carriage returns اندر the پہلا عمود\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#اگر defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N اندر [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N اندر [0..1]\n\
"), stdout);
#اگر_ختم
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#اگر_تعریف_شدہ ECHOKE
      fputs (_("\
 * crtkill       kill all سطر by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all سطر by obeying the echoctl and echok settings\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters اندر hat notation ('^c')\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   [-]echo       echo داخلہ characters\n\
"), stdout);
#اگر_تعریف_شدہ ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#اگر_تعریف_شدہ ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   [-]echonl     echo newline even اگر not echoing other characters\n\
"), stdout);
#اگر_تعریف_شدہ ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#اگر_ختم
#اگر defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with بلند latency links\n\
"), stdout);
#اگر_ختم
#اگر defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#اگر_ختم
      لکھو (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#اگر_تعریف_شدہ VWERASE
    ", werase"
#اگر_ختم
#اگر defined CREPRINT || defined VREPRINT
    ", rprnt"
#اگر_ختم
);
      fputs (_("\
   [-]isig       enable مداخلت, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after مداخلت and quit special characters\n\
"), stdout);
#اگر_تعریف_شدہ ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try مقصد اكتب مقصد the terminal\n\
"), stdout);
#اگر_ختم
#اگر_تعریف_شدہ XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' برائے uppercase characters\n\
"), stdout);
#اگر_ختم
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#اگر defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters مقصد their طےشدہ اقدار\n\
   -cooked       same as raw\n\
"), stdout);
      لکھو (_("\
   crt           same as %s\n\
"), "echoe"
#اگر_تعریف_شدہ ECHOCTL
    " echoctl"
#اگر_ختم
#اگر_تعریف_شدہ ECHOKE
    " echoke"
#اگر_ختم
);
      لکھو (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#اگر_تعریف_شدہ ECHOCTL
    " echoctl"
#اگر_ختم
#اگر_تعریف_شدہ ECHOKE
    " echoke"
#اگر_ختم
#اگر_تعریف_شدہ IXANY
    " -ixany"
#اگر_ختم
);
#اگر_تعریف_شدہ IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   ek            erase and kill characters مقصد their طےشدہ اقدار\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#اگر defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#اگر_ختم
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      لکھو (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#اگر_تعریف_شدہ ONLCR
   " -onlcr"
#اگر_ختم
  , "icrnl -inlcr -igncr"
#اگر_تعریف_شدہ ONLCR
   " onlcr"
#اگر_ختم
#اگر_تعریف_شدہ OCRNL
   " -ocrnl"
#اگر_ختم
#اگر_تعریف_شدہ ONLRET
   " -onlret"
#اگر_ختم
);
      fputs (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), stdout);
      لکھو (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 وقت 0\n\
   -raw          same as cooked\n\
"),
#اگر_تعریف_شدہ IUCLC
   " -iuclc"
#اگر_ختم
#اگر_تعریف_شدہ IXANY
   " -ixany"
#اگر_ختم
#اگر_تعریف_شدہ IMAXBEL
   " -imaxbel"
#اگر_ختم
#اگر_تعریف_شدہ XCASE
   " -xcase"
#اگر_ختم
);
      لکھو (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters مقصد their طےشدہ اقدار\n\
"),
   "-ixoff"
#اگر_تعریف_شدہ IUTF8
   " -iutf8"
#اگر_ختم
#اگر_تعریف_شدہ IUCLC
   " -iuclc"
#اگر_ختم
#اگر_تعریف_شدہ IXANY
   " -ixany"
#اگر_ختم
#اگر_تعریف_شدہ IMAXBEL
   " imaxbel"
#اگر_ختم
#اگر_تعریف_شدہ XCASE
   " -xcase"
#اگر_ختم
#اگر_تعریف_شدہ OLCUC
   " -olcuc"
#اگر_ختم
#اگر_تعریف_شدہ OCRNL
   " -ocrnl"
#اگر_ختم

 , "opost"
#اگر_تعریف_شدہ OFILL
   " -ofill"
#اگر_ختم
#اگر_تعریف_شدہ ONLCR
   " onlcr"
#اگر_ختم
#اگر_تعریف_شدہ ONOCR
   " -onocr"
#اگر_ختم
#اگر_تعریف_شدہ ONLRET
   " -onlret"
#اگر_ختم
#اگر_تعریف_شدہ NLDLY
   " nl0"
#اگر_ختم
#اگر_تعریف_شدہ CRDLY
   " cr0"
#اگر_ختم
#اگر_تعریف_شدہ TAB0
   " tab0"
#اگر_ختم
#اگر_تعریف_شدہ BSDLY
   " bs0"
#اگر_ختم
#اگر_تعریف_شدہ VTDLY
   " vt0"
#اگر_ختم
#اگر_تعریف_شدہ FFDLY
   " ff0"
#اگر_ختم

 , "isig"
#اگر_تعریف_شدہ TOSTOP
   " -tostop"
#اگر_ختم
#اگر_تعریف_شدہ OFDEL
   " -ofdel"
#اگر_ختم
#اگر_تعریف_شدہ ECHOPRT
   " -echoprt"
#اگر_ختم
#اگر_تعریف_شدہ ECHOCTL
   " echoctl"
#اگر_ختم
#اگر_تعریف_شدہ ECHOKE
   " echoke"
#اگر_ختم
#اگر_تعریف_شدہ EXTPROC
   " -extproc"
#اگر_ختم
#اگر_تعریف_شدہ FLUSHO
   " -flusho"
#اگر_ختم
);
      fputs (_("\
\n\
Handle the tty سطر connected مقصد standard داخلہ.  Without arguments,\n\
prints baud شرح, سطر discipline, and deviations from stty sane.  اندر_2\n\
settings, CHAR is لیا_گیا literally, or coded as اندر ^c, 0x37, 0177 or\n\
127; special اقدار ^- or undef مستعمل مقصد disable special characters.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

ساکن خالی
apply_settings (bool checking, حرف ثابت *device_name,
                حرف * ثابت *settings, صحیح n_settings,
                ساخت termios *منوال_2, bool *require_سیٹ_attr)
{
#تعریف_کرو check_دلیل(arg)						\
  کرو									\
    {									\
      اگر (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  جب (0)

  برائے (صحیح k = 1; k < n_settings; k++)
    {
      حرف ثابت *arg = settings[k];
      bool مطابقت_ملا = false;
      bool not_سیٹ_attr = false;
      bool reversed = false;

      اگر (! arg)
        جاری;

      اگر (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      اگر (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          جاری;
        }
      برائے (صحیح i = 0; منوال_info[i].name != تہی; ++i)
        {
          اگر (streq (arg, منوال_info[i].name))
            {
              اگر ((منوال_info[i].flags & NO_SETATTR) == 0)
                {
                  مطابقت_ملا = سیٹ_منوال (&منوال_info[i], reversed, منوال_2);
                  *require_سیٹ_attr = true;
                }
              ورنہ
                مطابقت_ملا = not_سیٹ_attr = true;
              توڑو;
            }
        }
      اگر (!مطابقت_ملا && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      اگر (!مطابقت_ملا)
        {
          برائے (صحیح i = 0; control_info[i].name != تہی; ++i)
            {
              اگر (streq (arg, control_info[i].name))
                {
                  check_دلیل (arg);
                  مطابقت_ملا = true;
                  ++k;
                  سیٹ_control_char (&control_info[i], settings[k], منوال_2);
                  *require_سیٹ_attr = true;
                  توڑو;
                }
            }
        }
      اگر (!مطابقت_ملا || not_سیٹ_attr)
        {
          اگر (streq (arg, "ispeed"))
            {
              check_دلیل (arg);
              ++k;
              ساخت رفتار_desc ثابت ispeed = string_مقصد_baud (settings[k]);
              اگر (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              سیٹ_رفتار (داخلہ_رفتار, settings[k], ispeed, منوال_2);
              اگر (checking)
                جاری;
              *require_سیٹ_attr = true;
            }
          ورنہ اگر (streq (arg, "ospeed"))
            {
              check_دلیل (arg);
              ++k;
              ساخت رفتار_desc ثابت ospeed = string_مقصد_baud (settings[k]);
              اگر (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              سیٹ_رفتار (output_رفتار, settings[k], ospeed, منوال_2);
              اگر (checking)
                جاری;
              *require_سیٹ_attr = true;
            }
#اگر_تعریف_شدہ TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          ورنہ اگر (streq (arg, "extproc"))
            {
              صحیح val = ! reversed;

              اگر (checking)
                جاری;

              اگر (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#اگر_ختم
#اگر_تعریف_شدہ TIOCGWINSZ
          ورنہ اگر (streq (arg, "rows"))
            {
              check_دلیل (arg);
              ++k;
              اگر (checking)
                جاری;
              سیٹ_کھڑکی_سائز (integer_arg (settings[k], زیادہ_سے_زیادہ_عدد), -1,
                               device_name);
            }
          ورنہ اگر (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_دلیل (arg);
              ++k;
              اگر (checking)
                جاری;
              سیٹ_کھڑکی_سائز (-1, integer_arg (settings[k], زیادہ_سے_زیادہ_عدد),
                               device_name);
            }
          ورنہ اگر (streq (arg, "size"))
            {
              اگر (checking)
                جاری;
              max_ستون = screen_columns ();
              current_ستون = 0;
              display_کھڑکی_سائز (false, device_name);
            }
#اگر_ختم
#اگر_تعریف_شدہ HAVE_C_سطر
          ورنہ اگر (streq (arg, "line"))
            {
              check_دلیل (arg);
              ++k;
              uintmax_t قدر = integer_arg (settings[k], UINTMAX_MAX);
              اگر (ckd_جوڑو (&منوال_2->c_سطر, قدر, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_سیٹ_attr = true;
            }
#اگر_ختم
          ورنہ اگر (streq (arg, "speed"))
            {
              اگر (checking)
                جاری;
              max_ستون = screen_columns ();
              display_رفتار (منوال_2, false);
            }
          ورنہ
            {
              ساخت رفتار_desc ثابت رفتار = string_مقصد_baud (arg);
              اگر (رفتار.valid)
                {
                  سیٹ_رفتار (both_speeds, arg, رفتار, منوال_2);
                  اگر (checking)
                    جاری;
                  *require_سیٹ_attr = true;
                }
              ورنہ
                {
                  اگر (! recover_منوال (arg, منوال_2))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_سیٹ_attr = true;
                }
            }
        }
    }

  اگر (checking)
    check_رفتار (منوال_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ساکن ساخت termios منوال_2;

  شمار output_type output_type;
  صحیح optc;
  صحیح argi = 0;
  صحیح opti = 1;
  bool require_سیٹ_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  حرف *file_name = تہی;
  حرف ثابت *device_name;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = بدلا;
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

  جب ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, تہی))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'a':
          verbose_output = true;
          output_type = all;
          توڑو;

        حالت 'g':
          recoverable_output = true;
          output_type = recoverable;
          توڑو;

        حالت 'F':
          اگر (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          توڑو;

        حالت DEV_DEBUG_OPTION:
          dev_debug = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          اگر (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          توڑو;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      جب (opti < optind)
        argv[argi + opti++] = تہی;
    }

  /* Specifying both -a and -g gets an error.  */
  اگر (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  اگر (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  اگر (!noargs && !verbose_output && !recoverable_output)
    {
      ساکن ساخت termios check_منوال;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_منوال, &require_سیٹ_attr);
    }

  اگر (file_name)
    {
      صحیح fdflags;
      اگر (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      اگر ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  اگر (tcgetattr (STDIN_FILENO, &منوال_2))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  اگر (verbose_output || recoverable_output || noargs)
    {
      max_ستون = screen_columns ();
      current_ستون = 0;
      display_settings (output_type, &منوال_2, device_name);
      واپس EXIT_SUCCESS;
    }

  require_سیٹ_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &منوال_2, &require_سیٹ_attr);

  اگر (require_سیٹ_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ساکن ساخت termios new_منوال;

      اگر (tcsetattr (STDIN_FILENO, tcsetattr_options, &منوال_2))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      اگر (tcgetattr (STDIN_FILENO, &new_منوال))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      اگر (! eq_منوال (&منوال_2, &new_منوال))
        {
          اگر (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              برائے (غیرعلامتی صحیح i = 0; i < حجم (new_منوال); i++)
                {
                  غیرعلامتی صحیح newc = *(((غیرعلامتی حرف *) &new_منوال) + i);
                  غیرعلامتی صحیح oldc = *(((غیرعلامتی حرف *) &منوال_2) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  واپس EXIT_SUCCESS;
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
ساکن bool
eq_منوال (ساخت termios ثابت *mode1, ساخت termios ثابت *mode2)
{
  اگر (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#اگر_تعریف_شدہ HAVE_C_سطر
      || mode1->c_سطر != mode2->c_سطر
#اگر_ختم
      || !memeq (mode1->c_cc, mode2->c_cc, حجم (mode1->c_cc)))
    واپس false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_نقاب = ~(tcflag_t) 0;
#اگر_تعریف_شدہ CBAUD
  cflag_نقاب &= ~(tcflag_t) CBAUD;
#اگر_ختم
#اگر_تعریف_شدہ CIBAUD
  cflag_نقاب &= ~(tcflag_t) CIBAUD;
#اگر_ختم

  اگر ((mode1->c_cflag ^ mode2->c_cflag) & cflag_نقاب)
    واپس false;

  واپس رفتار_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ساکن bool
سیٹ_منوال (ساخت منوال_info ثابت *info, bool reversed, ساخت termios *منوال_2)
{
  tcflag_t *bitsp;

  اگر (reversed && (info->flags & REV) == 0)
    واپس false;

  bitsp = منوال_type_flag (info->type, منوال_2);

  اگر (bitsp == تہی)
    {
      /* Combination mode. */
      اگر (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          اگر (reversed)
            منوال_2->c_cflag = (منوال_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ورنہ
            منوال_2->c_cflag = (منوال_2->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      ورنہ اگر (streq (info->name, "oddp"))
        {
          اگر (reversed)
            منوال_2->c_cflag = (منوال_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          ورنہ
            منوال_2->c_cflag = (منوال_2->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      ورنہ اگر (streq (info->name, "nl"))
        {
          اگر (reversed)
            {
              منوال_2->c_iflag = (منوال_2->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              منوال_2->c_oflag = (منوال_2->c_oflag
#اگر_تعریف_شدہ ONLCR
                               | ONLCR
#اگر_ختم
                )
#اگر_تعریف_شدہ OCRNL
                & ~OCRNL
#اگر_ختم
#اگر_تعریف_شدہ ONLRET
                & ~ONLRET
#اگر_ختم
                ;
            }
          ورنہ
            {
              منوال_2->c_iflag = منوال_2->c_iflag & ~ICRNL;
#اگر_تعریف_شدہ ONLCR
              منوال_2->c_oflag = منوال_2->c_oflag & ~ONLCR;
#اگر_ختم
            }
        }
      ورنہ اگر (streq (info->name, "ek"))
        {
          منوال_2->c_cc[VERASE] = CERASE;
          منوال_2->c_cc[VKILL] = CKILL;
        }
      ورنہ اگر (streq (info->name, "sane"))
        sane_منوال (منوال_2);
      ورنہ اگر (streq (info->name, "cbreak"))
        {
          اگر (reversed)
            منوال_2->c_lflag |= ICANON;
          ورنہ
            منوال_2->c_lflag &= ~ICANON;
        }
      ورنہ اگر (streq (info->name, "pass8"))
        {
          اگر (reversed)
            {
              منوال_2->c_cflag = (منوال_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              منوال_2->c_iflag |= ISTRIP;
            }
          ورنہ
            {
              منوال_2->c_cflag = (منوال_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              منوال_2->c_iflag &= ~ISTRIP;
            }
        }
      ورنہ اگر (streq (info->name, "litout"))
        {
          اگر (reversed)
            {
              منوال_2->c_cflag = (منوال_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              منوال_2->c_iflag |= ISTRIP;
              منوال_2->c_oflag |= OPOST;
            }
          ورنہ
            {
              منوال_2->c_cflag = (منوال_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              منوال_2->c_iflag &= ~ISTRIP;
              منوال_2->c_oflag &= ~OPOST;
            }
        }
      ورنہ اگر (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          اگر ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              منوال_2->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              منوال_2->c_oflag |= OPOST;
              منوال_2->c_lflag |= ISIG | ICANON;
#اگر VMIN == VEOF
              منوال_2->c_cc[VEOF] = CEOF;
#اگر_ختم
#اگر VTIME == VEOL
              منوال_2->c_cc[VEOL] = CEOL;
#اگر_ختم
            }
          ورنہ
            {
              /* Raw mode. */
              منوال_2->c_iflag = 0;
              منوال_2->c_oflag &= ~OPOST;
              منوال_2->c_lflag &= ~(ISIG | ICANON
#اگر_تعریف_شدہ XCASE
                                 | XCASE
#اگر_ختم
                );
              منوال_2->c_cc[VMIN] = 1;
              منوال_2->c_cc[VTIME] = 0;
            }
        }
#اگر_تعریف_شدہ IXANY
      ورنہ اگر (streq (info->name, "decctlq"))
        {
          اگر (reversed)
            منوال_2->c_iflag |= IXANY;
          ورنہ
            منوال_2->c_iflag &= ~IXANY;
        }
#اگر_ختم
#اگر_تعریف_شدہ TABDLY
      ورنہ اگر (streq (info->name, "tabs"))
        {
          اگر (reversed)
            منوال_2->c_oflag = (منوال_2->c_oflag & ~TABDLY) | TAB3;
          ورنہ
            منوال_2->c_oflag = (منوال_2->c_oflag & ~TABDLY) | TAB0;
        }
#ورنہ
# ifdef OXTABS
      ورنہ اگر (streq (info->name, "tabs"))
        {
          اگر (reversed)
            منوال_2->c_oflag = منوال_2->c_oflag | OXTABS;
          ورنہ
            منوال_2->c_oflag = منوال_2->c_oflag & ~OXTABS;
        }
# endif
#اگر_ختم
#اگر defined XCASE && defined IUCLC && defined OLCUC
      ورنہ اگر (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          اگر (reversed)
            {
              منوال_2->c_lflag &= ~XCASE;
              منوال_2->c_iflag &= ~IUCLC;
              منوال_2->c_oflag &= ~OLCUC;
            }
          ورنہ
            {
              منوال_2->c_lflag |= XCASE;
              منوال_2->c_iflag |= IUCLC;
              منوال_2->c_oflag |= OLCUC;
            }
        }
#اگر_ختم
      ورنہ اگر (streq (info->name, "crt"))
        منوال_2->c_lflag |= ECHOE
#اگر_تعریف_شدہ ECHOCTL
          | ECHOCTL
#اگر_ختم
#اگر_تعریف_شدہ ECHOKE
          | ECHOKE
#اگر_ختم
          ;
      ورنہ اگر (streq (info->name, "dec"))
        {
          منوال_2->c_cc[VINTR] = 3;	/* ^C */
          منوال_2->c_cc[VERASE] = 127;	/* DEL */
          منوال_2->c_cc[VKILL] = 21;	/* ^U */
          منوال_2->c_lflag |= ECHOE
#اگر_تعریف_شدہ ECHOCTL
            | ECHOCTL
#اگر_ختم
#اگر_تعریف_شدہ ECHOKE
            | ECHOKE
#اگر_ختم
            ;
#اگر_تعریف_شدہ IXANY
          منوال_2->c_iflag &= ~IXANY;
#اگر_ختم
        }
    }
  ورنہ اگر (reversed)
    *bitsp = *bitsp & ~info->نقاب & ~info->بٹ_2;
  ورنہ
    *bitsp = (*bitsp & ~info->نقاب) | info->بٹ_2;

  واپس true;
}

ساکن خالی
سیٹ_control_char (ساخت control_info ثابت *info, حرف ثابت *arg,
                  ساخت termios *منوال_2)
{
  غیرعلامتی طویل صحیح قدر;

  اگر (streq (info->name, "min") || streq (info->name, "time"))
    قدر = integer_arg (arg, TYPE_زیادہ_سے_زیادہ (cc_t));
  ورنہ اگر (arg[0] == '\0' || arg[1] == '\0')
    قدر = مقصد_uchar (arg[0]);
  ورنہ اگر (streq (arg, "^-") || streq (arg, "undef"))
    قدر = _POSIX_VDISABLE;
  ورنہ اگر (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      اگر (arg[1] == '?')
        قدر = 127;
      ورنہ
        قدر = مقصد_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  ورنہ
    قدر = integer_arg (arg, TYPE_زیادہ_سے_زیادہ (cc_t));
  منوال_2->c_cc[info->انحراف] = قدر;
}

ساکن خالی
سیٹ_رفتار (شمار رفتار_setting type, حرف ثابت *arg,
           ساخت رفتار_desc ثابت رفتار, ساخت termios *منوال_2)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (رفتار.valid);

  اگر (type == داخلہ_رفتار || type == both_speeds)
    {
      آخری_ibaud = رفتار;
      اگر (cfsetispeed (منوال_2, رفتار.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  اگر (type == output_رفتار || type == both_speeds)
    {
      آخری_obaud = رفتار;
      اگر (cfsetospeed (منوال_2, رفتار.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#اگر_تعریف_شدہ TIOCGWINSZ

ساکن صحیح
get_win_سائز (صحیح fd, ساخت winsize *win)
{
  صحیح err = ioctl (fd, TIOCGWINSZ, (حرف *) win);
  واپس err;
}

ساکن خالی
سیٹ_کھڑکی_سائز (صحیح صفیں, صحیح ستون_تعداد_2, حرف ثابت *device_name)
{
  ساخت winsize win;

  اگر (get_win_سائز (STDIN_FILENO, &win))
    {
      اگر (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, حجم (win));
    }

  اگر (صفیں >= 0)
    win.ws_صف = صفیں;
  اگر (ستون_تعداد_2 >= 0)
    win.ws_ستون = ستون_تعداد_2;

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

  اگر (win.ws_صف == 0 || win.ws_ستون == 0)
    {
      ساخت ttysize ttysz;

      ttysz.ts_سطریں = win.ws_صف;
      ttysz.ts_ستون_تعداد = win.ws_ستون;

      win.ws_صف = 1;
      win.ws_ستون = 1;

      اگر (ioctl (STDIN_FILENO, TIOCSWINSZ, (حرف *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      اگر (ioctl (STDIN_FILENO, TIOCSSIZE, (حرف *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      واپس;
    }
# endif

  اگر (ioctl (STDIN_FILENO, TIOCSWINSZ, (حرف *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

ساکن خالی
display_کھڑکی_سائز (bool fancy, حرف ثابت *device_name)
{
  ساخت winsize win;

  اگر (get_win_سائز (STDIN_FILENO, &win))
    {
      اگر (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      اگر (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  ورنہ
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_صف, win.ws_ستون);
      اگر (!fancy)
        current_ستون = 0;
    }
}
#اگر_ختم

ساکن صحیح
screen_columns (خالی)
{
#اگر_تعریف_شدہ TIOCGWINSZ
  ساخت winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  اگر (get_win_سائز (STDOUT_FILENO, &win) == 0 && 0 < win.ws_ستون)
    واپس win.ws_ستون;
#اگر_ختم
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    حرف *ستون_string = getenv ("COLUMNS");
    طویل صحیح n_columns;
    اگر (!(ستون_string != تہی
          && xstrtol (ستون_string, تہی, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= زیادہ_سے_زیادہ_عدد))
      n_columns = 80;
    واپس n_columns;
  }
}

ATTRIBUTE_خالص
ساکن tcflag_t *
منوال_type_flag (شمار منوال_type type, ساخت termios *منوال_2)
{
  انتخاب (type)
    {
    حالت control:
      واپس &منوال_2->c_cflag;

    حالت داخلہ:
      واپس &منوال_2->c_iflag;

    حالت output:
      واپس &منوال_2->c_oflag;

    حالت local:
      واپس &منوال_2->c_lflag;

    حالت combination:
      واپس تہی;

    طےشدہ:
      unreachable ();
    }
}

ساکن خالی
display_settings (شمار output_type output_type, ساخت termios *منوال_2,
                  حرف ثابت *device_name)
{
  انتخاب (output_type)
    {
    حالت بدلا:
      display_بدلا (منوال_2);
      توڑو;

    حالت all:
      display_all (منوال_2, device_name);
      توڑو;

    حالت recoverable:
      display_recoverable (منوال_2);
      توڑو;
    }
}

ساکن خالی
display_بدلا (ساخت termios *منوال_2)
{
  bool خالی_خانے_سطر;
  tcflag_t *bitsp;
  غیرعلامتی طویل نقاب;
  شمار منوال_type پچھلا_type = control;

  display_رفتار (منوال_2, true);
#اگر_تعریف_شدہ HAVE_C_سطر
  wrapf ("line = %d;", منوال_2->c_سطر);
#اگر_ختم
  حرف_لکھو ('\n');
  current_ستون = 0;

  خالی_خانے_سطر = true;
  برائے (صحیح i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      اگر (منوال_2->c_cc[control_info[i].انحراف] == control_info[i].saneval)
        جاری;

#اگر_تعریف_شدہ VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      اگر (streq (control_info[i].name, "flush"))
        جاری;
#اگر_ختم
      /* If swtch is the same as susp, don't print both.  */
#اگر VSWTCH == VSUSP
      اگر (streq (control_info[i].name, "swtch"))
        جاری;
#اگر_ختم
      /* If eof uses the same slot as min, only print whichever applies.  */
#اگر VEOF == VMIN
      اگر ((منوال_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        جاری;
#اگر_ختم

      خالی_خانے_سطر = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (منوال_2->c_cc[control_info[i].انحراف]));
    }
  اگر ((منوال_2->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (غیرعلامتی طویل صحیح) منوال_2->c_cc[VMIN],
             (غیرعلامتی طویل صحیح) منوال_2->c_cc[VTIME]);
    }
  ورنہ اگر (!خالی_خانے_سطر)
    حرف_لکھو ('\n');
  current_ستون = 0;

  خالی_خانے_سطر = true;
  برائے (صحیح i = 0; منوال_info[i].name != تہی; ++i)
    {
      اگر (منوال_info[i].flags & OMIT)
        جاری;
      اگر (منوال_info[i].type != پچھلا_type)
        {
          اگر (!خالی_خانے_سطر)
            {
              حرف_لکھو ('\n');
              current_ستون = 0;
              خالی_خانے_سطر = true;
            }
          پچھلا_type = منوال_info[i].type;
        }

      bitsp = منوال_type_flag (منوال_info[i].type, منوال_2);
      نقاب = منوال_info[i].نقاب ? منوال_info[i].نقاب : منوال_info[i].بٹ_2;
      اگر ((*bitsp & نقاب) == منوال_info[i].بٹ_2)
        {
          اگر (منوال_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", منوال_info[i].name);
              خالی_خانے_سطر = false;
            }
        }
      ورنہ اگر ((منوال_info[i].flags & (SANE_سیٹ | REV)) == (SANE_سیٹ | REV))
        {
          wrapf ("-%s", منوال_info[i].name);
          خالی_خانے_سطر = false;
        }
    }
  اگر (!خالی_خانے_سطر)
    حرف_لکھو ('\n');
  current_ستون = 0;
}

ساکن خالی
display_all (ساخت termios *منوال_2, حرف ثابت *device_name)
{
  tcflag_t *bitsp;
  غیرعلامتی طویل نقاب;
  شمار منوال_type پچھلا_type = control;

  display_رفتار (منوال_2, true);
#اگر_تعریف_شدہ TIOCGWINSZ
  display_کھڑکی_سائز (true, device_name);
#اگر_ختم
#اگر_تعریف_شدہ HAVE_C_سطر
  wrapf ("line = %d;", منوال_2->c_سطر);
#اگر_ختم
  حرف_لکھو ('\n');
  current_ستون = 0;

  برائے (صحیح i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#اگر_تعریف_شدہ VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      اگر (streq (control_info[i].name, "flush"))
        جاری;
#اگر_ختم
      /* If swtch is the same as susp, don't print both.  */
#اگر VSWTCH == VSUSP
      اگر (streq (control_info[i].name, "swtch"))
        جاری;
#اگر_ختم
      /* If eof uses the same slot as min, only print whichever applies.  */
#اگر VEOF == VMIN
      اگر ((منوال_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        جاری;
#اگر_ختم
      wrapf ("%s = %s;", control_info[i].name,
             visible (منوال_2->c_cc[control_info[i].انحراف]));
    }
#اگر VEOF == VMIN
  اگر ((منوال_2->c_lflag & ICANON) == 0)
#اگر_ختم
    wrapf ("min = %lu; time = %lu;",
           (غیرعلامتی طویل صحیح) منوال_2->c_cc[VMIN],
           (غیرعلامتی طویل صحیح) منوال_2->c_cc[VTIME]);
  اگر (current_ستون != 0)
    حرف_لکھو ('\n');
  current_ستون = 0;

  برائے (صحیح i = 0; منوال_info[i].name != تہی; ++i)
    {
      اگر (منوال_info[i].flags & OMIT)
        جاری;
      اگر (منوال_info[i].type != پچھلا_type)
        {
          حرف_لکھو ('\n');
          current_ستون = 0;
          پچھلا_type = منوال_info[i].type;
        }

      bitsp = منوال_type_flag (منوال_info[i].type, منوال_2);
      نقاب = منوال_info[i].نقاب ? منوال_info[i].نقاب : منوال_info[i].بٹ_2;
      اگر ((*bitsp & نقاب) == منوال_info[i].بٹ_2)
        wrapf ("%s", منوال_info[i].name);
      ورنہ اگر (منوال_info[i].flags & REV)
        wrapf ("-%s", منوال_info[i].name);
    }
  حرف_لکھو ('\n');
  current_ستون = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ساکن خالی
check_رفتار (ساخت termios *منوال_2)
{
  اگر (آخری_ibaud.valid && آخری_obaud.valid)
    {
      اگر (cfgetispeed (منوال_2) != آخری_ibaud.baud
          || cfgetospeed (منوال_2) != آخری_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_مقصد_قدر (آخری_ibaud.baud),
               baud_مقصد_قدر (آخری_obaud.baud));
    }
}

ساکن خالی
display_رفتار (ساخت termios *منوال_2, bool fancy)
{
  اگر (cfgetispeed (منوال_2) == 0 || cfgetispeed (منوال_2) == cfgetospeed (منوال_2))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_مقصد_قدر (cfgetospeed (منوال_2)));
  ورنہ
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_مقصد_قدر (cfgetispeed (منوال_2)),
           baud_مقصد_قدر (cfgetospeed (منوال_2)));
  اگر (!fancy)
    current_ستون = 0;
}

ساکن خالی
display_recoverable (ساخت termios *منوال_2)
{
  لکھو ("%lx:%lx:%lx:%lx",
          (غیرعلامتی طویل صحیح) منوال_2->c_iflag,
          (غیرعلامتی طویل صحیح) منوال_2->c_oflag,
          (غیرعلامتی طویل صحیح) منوال_2->c_cflag,
          (غیرعلامتی طویل صحیح) منوال_2->c_lflag);
  برائے (size_t i = 0; i < NCCS; ++i)
    لکھو (":%lx", (غیرعلامتی طویل صحیح) منوال_2->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#اگر_تعریف_شدہ CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (منوال_2->c_cflag & CBAUD) == BOTHER;
#  اگر defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((منوال_2->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#اگر_ختم
  اگر (separate_speeds)
    لکھو (":%lx:%lx", (غیرعلامتی طویل صحیح) cfgetispeed (منوال_2),
            (غیرعلامتی طویل صحیح) cfgetospeed (منوال_2));

  حرف_لکھو ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#تعریف_کرو STRTOUL_TYPE(TYPE)                                               \
  ساکن صحیح                                                             \
  strtoul_##TYPE (حرف ثابت *s, صحیح بنیاد_2, حرف **p, TYPE *نتیجہ,       \
                  حرف delim)                                            \
  {                                                                      \
    غیرعلامتی طویل ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, بنیاد_2);                                           \
    اگر (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      واپس -1;                                                         \
    *نتیجہ = ul;                                                        \
    واپس 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (رفتار_t)
#تعریف_منسوخ STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#شامل "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ساکن bool
recover_منوال (حرف ثابت *arg, ساخت termios *منوال_2)
{
  tcflag_t flag[4];
  حرف ثابت *s = arg;
  برائے (size_t i = 0; i < 4; i++)
    {
      حرف *p;
      اگر (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        واپس false;
      s = p + 1;
    }
  منوال_2->c_iflag = flag[0];
  منوال_2->c_oflag = flag[1];
  منوال_2->c_cflag = flag[2];
  منوال_2->c_lflag = flag[3];

  برائے (size_t i = 0; i < NCCS; ++i)
    {
      حرف *p;
      حرف delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      اگر (strtoul_cc_t (s, 16, &p, منوال_2->c_cc + i, delim) != 0)
        واپس false;
      /* Accept the old format, which has no separate speed fields.  */
      اگر (! *p)
        {
#اگر_تعریف_شدہ CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_رفتار_T_SANE
          ساخت رفتار_desc ospeed = cbaud_مقصد_رفتار (منوال_2->c_cflag & CBAUD);
          اگر (ospeed.valid && cfsetospeed (منوال_2, ospeed.baud) != 0)
            واپس false;
#  اگر defined CIBAUD && defined IBSHIFT
          ساخت رفتار_desc ispeed =
            cbaud_مقصد_رفتار ((منوال_2->c_cflag & CIBAUD) >> IBSHIFT);
          اگر (ispeed.valid && cfsetispeed (منوال_2, ispeed.baud) != 0)
            واپس false;
#  endif
# endif
          آخری_ibaud.baud = cfgetispeed (منوال_2);
          آخری_obaud.baud = cfgetospeed (منوال_2);
#اگر_ختم
          واپس true;
        }
      s = p + 1;
    }

  رفتار_t رفتار[2];
  برائے (size_t i = 0; i < 2; i++)
    {
      حرف *p;
      حرف delim = i == 0 ? ':' : '\0';
      اگر (strtoul_رفتار_t (s, 16, &p, رفتار + i, delim) != 0)
        واپس false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  اگر (cfsetospeed (منوال_2, رفتار[1]) != 0
      || cfsetispeed (منوال_2, رفتار[0]) != 0)
    واپس false;

  آخری_ibaud.baud = رفتار[0] == 0 ? cfgetispeed (منوال_2) : رفتار[0];
  آخری_obaud.baud = رفتار[1];
  واپس true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ساکن bool
رفتار_matches (ساخت termios ثابت *mode1, ساخت termios ثابت *mode2)
{
  رفتار_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  رفتار_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  اگر (speed1[1] == 0)
    speed1[1] = speed1[0];
  اگر (speed2[1] == 0)
    speed2[1] = speed2[0];

  غیرعلامتی صحیح بٹ_2 = 2;  /* Start and stop bits.  */
  بٹ_2 += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  بٹ_2 += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  انتخاب (mode1->c_cflag & CSIZE)
    {
      حالت CS5: بٹ_2 += 5; توڑو;
      حالت CS6: بٹ_2 += 6; توڑو;
      حالت CS7: بٹ_2 += 7; توڑو;
      طےشدہ: بٹ_2 += 8; توڑو;
    }

  برائے (صحیح i = 0; i < 2; i++)
    {
      غیرعلامتی طویل صحیح value1;
      غیرعلامتی طویل صحیح value2;
      غیرعلامتی طویل صحیح delta;

      اگر (speed1[i] == speed2[i])
        جاری;

      value1 = baud_مقصد_قدر (speed1[i]);
      value2 = baud_مقصد_قدر (speed2[i]);

#اگر_غیر_تعریف_شدہ TERMIOS_رفتار_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      اگر (value1 == ULONG_MAX || value2 == ULONG_MAX)
        واپس false;
#اگر_ختم

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      اگر (delta > value1 / (بٹ_2 * 4))
        واپس false;
    }

  واپس true;
}

ATTRIBUTE_خالص
ساکن ساخت رفتار_desc
string_مقصد_baud (حرف ثابت *arg)
{
  حرف *ep;
  غیرعلامتی طویل قدر;
  غیرعلامتی حرف c;

  /* Explicitly disallow negative numbers.  */
  جب (c_isspace (*arg))
    arg++;
  اگر (*arg == '-')
    جاؤ invalid;

  /* Empty does not mean 0.  */
  اگر (! *arg)
    جاؤ invalid;

  قدر = strtoul (arg, &ep, 10);

  c = *ep++;
  اگر (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      اگر (c)
        {
          غیرعلامتی حرف d = c - '0';
          اگر (d > 5)
            قدر++;
          ورنہ اگر (d == 5)
            {
              جب ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              اگر (c)
                قدر++;                /* Nonzero, round up */
              ورنہ
                قدر += (قدر & 1);   /* Exactly in the middle, round even */
            }

          جب (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          اگر (c)
            جاؤ invalid; /* Garbage after otherwise valid number */
        }
    }
  ورنہ اگر (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      اگر (streq (arg, "exta"))
        واپس (ساخت رفتار_desc) { .baud = B19200, .valid = true };
      ورنہ اگر (streq (arg, "extb"))
        واپس (ساخت رفتار_desc) { .baud = B38400, .valid = true };
      ورنہ
        جاؤ invalid;
    }

  واپس قدر_مقصد_baud (قدر);

 invalid:
  واپس (ساخت رفتار_desc) { .baud = (رفتار_t) -1, .valid = false };
}

ساکن خالی
sane_منوال (ساخت termios *منوال_2)
{
  tcflag_t *bitsp;

  برائے (صحیح i = 0; control_info[i].name; ++i)
    {
#اگر VMIN == VEOF
      اگر (streq (control_info[i].name, "min"))
        توڑو;
#اگر_ختم
      منوال_2->c_cc[control_info[i].انحراف] = control_info[i].saneval;
    }

  برائے (صحیح i = 0; منوال_info[i].name != تہی; ++i)
    {
      اگر (منوال_info[i].flags & NO_SETATTR)
        جاری;

      اگر (منوال_info[i].flags & SANE_سیٹ)
        {
          bitsp = منوال_type_flag (منوال_info[i].type, منوال_2);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~منوال_info[i].نقاب) | منوال_info[i].بٹ_2;
        }
      ورنہ اگر (منوال_info[i].flags & SANE_UNSET)
        {
          bitsp = منوال_type_flag (منوال_info[i].type, منوال_2);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~منوال_info[i].نقاب & ~منوال_info[i].بٹ_2;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

ساکن حرف ثابت *
visible (cc_t ch)
{
  ساکن حرف buf[10];
  حرف *bpout = buf;

  اگر (ch == _POSIX_VDISABLE)
    واپس "<undef>";

  اگر (ch >= 32)
    {
      اگر (ch < 127)
        *bpout++ = ch;
      ورنہ اگر (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      ورنہ
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          اگر (ch >= 128 + 32)
            {
              اگر (ch < 128 + 127)
                *bpout++ = ch - 128;
              ورنہ
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          ورنہ
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  ورنہ
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  واپس (حرف ثابت *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

ساکن uintmax_t
integer_arg (حرف ثابت *s, uintmax_t maxval)
{
  واپس xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
