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

#جے_تعریف_شدہ TERMIOS_NEEDS_XOPEN_سروت
# define XOPEN_سروت
#جے_انت

#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن <termios.h>
#إذا HAVE_STROPTS_H
# include <stropts.h>
#جے_انت
#ضمن <sys/ioctl.h>

#جے_تعریف_شدہ WINSIZE_وچ_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#جے_انت
#جے_تعریف_شدہ GWINSZ_وچ_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#جے_انت
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
#تعریف_کرو پروگرام_NAME "stty"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

#جے_بنا_تعریف _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#جے_انت

#تعریف_کرو Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#جے_بنا_تعریف CINTR
# define CINTR Control ('c')
#جے_انت
#جے_بنا_تعریف CQUIT
# define CQUIT 28
#جے_انت
#جے_بنا_تعریف CERASE
# define CERASE 127
#جے_انت
#جے_بنا_تعریف CKILL
# define CKILL Control ('u')
#جے_انت
#جے_بنا_تعریف CEOF
# define CEOF Control ('d')
#جے_انت
#جے_بنا_تعریف CEOL
# define CEOL _POSIX_VDISABLE
#جے_انت
#جے_بنا_تعریف CSTART
# define CSTART Control ('q')
#جے_انت
#جے_بنا_تعریف CSTOP
# define CSTOP Control ('s')
#جے_انت
#جے_بنا_تعریف CSUSP
# define CSUSP Control ('z')
#جے_انت
#إذا defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#جے_انت
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#إذا defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#جے_انت
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#إذا defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# إذا defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#جے_انت
#إذا defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#جے_انت

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#إذا defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#جے_انت

#إذا defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#جے_انت
#إذا defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#جے_انت
#إذا defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#جے_انت
#إذا defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#جے_انت
#إذا defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#جے_انت
#إذا defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#جے_انت
#إذا defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#جے_انت
#إذا defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#جے_انت
#إذا defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#جے_انت
#إذا defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#جے_انت
#إذا defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#جے_انت
#إذا defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#جے_انت
#إذا defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#جے_انت

/* Which speeds to set.  */
تعداد رفتار_setting
  {
    داخلہ_رفتار, output_رفتار, both_speeds
  };

/* What to output and how.  */
تعداد output_type
  {
    بدلیا, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
تعداد منوال_type
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
بنية منوال_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    تعداد منوال_type type;	/* Which structure element to change. */
    حرف flags;			/* Setting and display options.  */
    بنانشانی لما بٹ_2;		/* Bits to set for this mode.  */
    بنانشانی لما نقاب;		/* Other bits to turn off for this mode.  */
  };

ساكن بنية منوال_info ثابت منوال_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#جے_تعریف_شدہ CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#جے_انت
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_سیٹ | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#جے_تعریف_شدہ CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#جے_انت
#جے_تعریف_شدہ CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#جے_انت

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
#جے_تعریف_شدہ IUCLC
  {"iuclc", داخلہ, SANE_UNSET | REV, IUCLC, 0},
#جے_انت
#جے_تعریف_شدہ IXANY
  {"ixany", داخلہ, SANE_UNSET | REV, IXANY, 0},
#جے_انت
#جے_تعریف_شدہ IMAXBEL
  {"imaxbel", داخلہ, SANE_سیٹ | REV, IMAXBEL, 0},
#جے_انت
#جے_تعریف_شدہ IUTF8
  {"iutf8", داخلہ, SANE_UNSET | REV, IUTF8, 0},
#جے_انت

  {"opost", output, SANE_سیٹ | REV, OPOST, 0},
#جے_تعریف_شدہ OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#جے_انت
#جے_تعریف_شدہ OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#جے_انت
#جے_تعریف_شدہ ONLCR
  {"onlcr", output, SANE_سیٹ | REV, ONLCR, 0},
#جے_انت
#جے_تعریف_شدہ ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#جے_انت
#جے_تعریف_شدہ ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#جے_انت
#جے_تعریف_شدہ OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#جے_انت
#جے_تعریف_شدہ OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#جے_انت
#جے_تعریف_شدہ NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_سیٹ, NL0, NLDLY},
#جے_انت
#جے_تعریف_شدہ CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_سیٹ, CR0, CRDLY},
#جے_انت
#جے_تعریف_شدہ TABDLY
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
#وإلا
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#جے_انت
#جے_تعریف_شدہ BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_سیٹ, BS0, BSDLY},
#جے_انت
#جے_تعریف_شدہ VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_سیٹ, VT0, VTDLY},
#جے_انت
#جے_تعریف_شدہ FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_سیٹ, FF0, FFDLY},
#جے_انت

  {"isig", local, SANE_سیٹ | REV, ISIG, 0},
  {"icanon", local, SANE_سیٹ | REV, ICANON, 0},
#جے_تعریف_شدہ IEXTEN
  {"iexten", local, SANE_سیٹ | REV, IEXTEN, 0},
#جے_انت
  {"echo", local, SANE_سیٹ | REV, ECHO, 0},
  {"echoe", local, SANE_سیٹ | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_سیٹ | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#جے_تعریف_شدہ XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#جے_انت
#جے_تعریف_شدہ TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#جے_انت
#جے_تعریف_شدہ ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#جے_انت
#جے_تعریف_شدہ ECHOCTL
  {"echoctl", local, SANE_سیٹ | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#جے_انت
#جے_تعریف_شدہ ECHOKE
  {"echoke", local, SANE_سیٹ | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#جے_انت
#جے_تعریف_شدہ FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#جے_انت
#إذا defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#وإلاإذا defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#جے_انت

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
#جے_تعریف_شدہ IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#جے_انت
#إذا defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#جے_انت
#إذا defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#جے_انت
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {عدم, control, 0, 0, 0}
};

/* Control character settings.  */
بنية control_info
  {
    حرف ثابت *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t وتھ;		/* Offset in c_cc.  */
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
#جے_تعریف_شدہ VEOL2
  {"eol2", CEOL2, VEOL2},
#جے_انت
#جے_تعریف_شدہ VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#جے_انت
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#جے_تعریف_شدہ VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#جے_انت
#جے_تعریف_شدہ VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#وإلا
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#جے_انت
#جے_تعریف_شدہ VWERASE
  {"werase", CWERASE, VWERASE},
#جے_انت
#جے_تعریف_شدہ VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#جے_انت
#جے_تعریف_شدہ VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#جے_انت
#جے_تعریف_شدہ VSTATUS
  {"status", CSTATUS, VSTATUS},
#جے_انت

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {عدم, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
بنية رفتار_desc
{
  /* The baud rate.  */
  رفتار_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

ساكن حرف ثابت *visible (cc_t ch);
ساكن بنانشانی لما صحيح baud_منزل_قدر (رفتار_t رفتار);
ساكن bool recover_منوال (حرف ثابت *arg, بنية termios *منوال_2);
ساكن صحيح screen_columns (فراغ);
ساكن bool سیٹ_منوال (بنية منوال_info ثابت *info, bool reversed,
                      بنية termios *منوال_2);
ساكن bool eq_منوال (بنية termios ثابت *mode1,
                     بنية termios ثابت *mode2);
ساكن bool رفتار_matches (بنية termios ثابت *mode1,
                           بنية termios ثابت *mode2);
ساكن uintmax_t integer_arg (حرف ثابت *s, uintmax_t max);
ساكن بنية رفتار_desc string_منزل_baud (حرف ثابت *arg);
ساكن tcflag_t *منوال_type_flag (تعداد منوال_type type, بنية termios *منوال_2);
ساكن فراغ display_all (بنية termios *منوال_2, حرف ثابت *device_name);
ساكن فراغ display_بدلیا (بنية termios *منوال_2);
ساكن فراغ display_recoverable (بنية termios *منوال_2);
ساكن فراغ display_settings (تعداد output_type output_type,
                              بنية termios *منوال_2,
                              حرف ثابت *device_name);
ساكن فراغ check_رفتار (بنية termios *منوال_2);
ساكن فراغ display_رفتار (بنية termios *منوال_2, bool fancy);
ساكن فراغ display_کھڑکی_ناپ (bool fancy, حرف ثابت *device_name);
ساكن فراغ sane_منوال (بنية termios *منوال_2);
ساكن فراغ سیٹ_control_char (بنية control_info ثابت *info,
                              حرف ثابت *arg,
                              بنية termios *منوال_2);
ساكن فراغ سیٹ_رفتار (تعداد رفتار_setting type, حرف ثابت *arg,
                       بنية رفتار_desc ثابت رفتار, بنية termios *منوال_2);
ساكن فراغ سیٹ_کھڑکی_ناپ (صحيح صفاں, صحيح خانے_گنتی_2, حرف ثابت *device_name);

/* The width of the screen, for output wrapping. */
ساكن صحيح max_خانہ;

/* Current position, to know when to wrap. */
ساكن صحيح current_خانہ;

/* Default "drain" mode for tcsetattr.  */
ساكن صحيح tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
ساكن bool dev_debug;

/* Record last speed set for correlation.  */
ساكن بنية رفتار_desc آخری_ibaud =
  { .baud = (رفتار_t) -1, .valid = false };
ساكن بنية رفتار_desc آخری_obaud =
  { .baud = (رفتار_t) -1, .valid = false };

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
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((لکھو, 1, 2))
ساكن فراغ
wrapf (حرف ثابت *سنیہا_2,...)
{
  va_list دلیلاں;
  حرف *buf;
  صحيح buflen;

  va_شروع (دلیلاں, سنیہا_2);
  buflen = vasprintf (&buf, سنیہا_2, دلیلاں);
  va_سرا (دلیلاں);

  إذا (buflen < 0)
    xalloc_die ();

  إذا (0 < current_خانہ)
    {
      إذا (max_خانہ - current_خانہ <= buflen)
        {
          حرف_لکھو ('\n');
          current_خانہ = 0;
        }
      وإلا
        {
          حرف_لکھو (' ');
          current_خانہ++;
        }
    }

  fputs (buf, stdout);
  حرر (buf);
  current_خانہ += buflen;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
  -a, --all          print all current settings وچ human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings وچ a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  open and use DEVICE instead دا standard داخلہ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system مقرر_کردا which settings are دستیاب.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#جے_تعریف_شدہ VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding دا output\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop إشارة once داخلہ flushed\n\
"), stdout);
#جے_انت
      fputs (_("\
   eof CHAR      CHAR will send an سرا دا file (terminate the داخلہ)\n\
   eol CHAR      CHAR will سرا the سطر\n\
"), stdout);
#جے_تعریف_شدہ VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR لكل ending the سطر\n\
"), stdout);
#جے_انت
      fputs (_("\
   erase CHAR    CHAR will erase the آخری character typed\n\
   intr CHAR     CHAR will send an رکاوٹ إشارة\n\
   kill CHAR     CHAR will erase the current سطر\n\
"), stdout);
#جے_تعریف_شدہ VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the اگلا character quoted\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ VSTATUS
      fputs (_("\
 * حالت CHAR   CHAR will send an info إشارة\n\
"), stdout);
#جے_انت
      fputs (_("\
   quit CHAR     CHAR will send a quit إشارة\n\
"), stdout);
#إذا defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current سطر\n\
"), stdout);
#جے_انت
      fputs (_("\
   شروع CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop إشارة\n\
"), stdout);
#جے_تعریف_شدہ VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will بدل منزل a different shell layer\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the آخری لفظ_2 typed\n\
"), stdout);
#جے_انت
      fputs (_("\
\n\
Special settings:\n\
   N             سیٹ_2 the داخلہ and output speeds منزل N bauds\n\
"), stdout);
#جے_تعریف_شدہ TIOCGWINSZ
      fputs (_("\
   خانے_گنتی_2 N        tell the kernel that the terminal has N columns\n\
 * columns N     same as خانے_گنتی_2 N\n\
"), stdout);
#جے_انت
      لکھو (_("\
 * [-]drain      wait لكل ترسیل before applying settings (%s by افتراضي)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      سیٹ_2 the داخلہ رفتار منزل N\n\
"), stdout);
#جے_تعریف_شدہ HAVE_C_سطر
      fputs (_("\
 * سطر N        use سطر discipline N\n\
"), stdout);
#جے_انت
      fputs (_("\
   min N         with -icanon, سیٹ_2 N characters minimum لكل a completed اقرأ\n\
   ospeed N      سیٹ_2 the output رفتار منزل N\n\
"), stdout);
#جے_تعریف_شدہ TIOCGWINSZ
      fputs (_("\
   صفاں N        tell the kernel that the terminal has N صفاں\n\
   ناپ          print the نمبر دا صفاں and columns according منزل the kernel\n\
"), stdout);
#جے_انت
      fputs (_("\
   رفتار         print the terminal رفتار\n\
   ویلا N        with -icanon, سیٹ_2 اقرأ timeout دا N tenths دا a دوجا\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control اشارے\n\
   [-]cread      allow داخلہ منزل be received\n\
"), stdout);
#جے_تعریف_شدہ CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#جے_انت
      fputs (_("\
   csN           سیٹ_2 character ناپ منزل N بٹ_2, N وچ [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop بٹ_2 فی character (one with '-')\n\
   [-]hup        send a hangup إشارة when the آخری process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity ثنائی_ہندسہ وچ output and expect parity ثنائی_ہندسہ وچ داخلہ\n\
   [-]parodd     سیٹ_2 odd parity (or even parity with '-')\n\
"), stdout);
#جے_تعریف_شدہ CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#جے_انت
      fputs (_("\
\n\
داخلہ_3 settings:\n\
   [-]brkint     breaks cause an رکاوٹ إشارة\n\
   [-]icrnl      translate carriage أرجع منزل newline\n\
   [-]ignbrk     ignore اكسر characters\n\
   [-]igncr      ignore carriage أرجع\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#جے_تعریف_شدہ IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and افعل not flush a بھریا داخلہ بفر on a character\n\
"), stdout);
#جے_انت
      fputs (_("\
   [-]inlcr      translate newline منزل carriage أرجع\n\
   [-]inpck      enable داخلہ parity checking\n\
   [-]istrip     clear بلند (8th) ثنائی_ہندسہ دا داخلہ characters\n\
"), stdout);
#جے_تعریف_شدہ IUTF8
      fputs (_("\
 * [-]iutf8      assume داخلہ characters are UTF-8 encoded\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters منزل lowercase\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only شروع character\n\
"), stdout);
#جے_انت
      fputs (_("\
   [-]ixoff      enable sending دا شروع/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#جے_تعریف_شدہ BSDLY
      fputs (_("\
 * bsN           backspace delay style, N وچ [0..1]\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ CRDLY
      fputs (_("\
 * crN           carriage أرجع delay style, N وچ [0..3]\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ FFDLY
      fputs (_("\
 * ffN           form feed delay style, N وچ [0..1]\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ NLDLY
      fputs (_("\
 * nlN           newline delay style, N وچ [0..1]\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage أرجع منزل newline\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters لكل fill instead دا NUL characters\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead دا timing لكل delays\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters منزل uppercase\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ ONLCR
      fputs (_("\
 * [-]onlcr      translate newline منزل carriage أرجع-newline\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage أرجع\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ ONOCR
      fputs (_("\
 * [-]onocr      افعل not print carriage returns وچ the پہلا کالم\n\
"), stdout);
#جے_انت
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#إذا defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N وچ [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N وچ [0..1]\n\
"), stdout);
#جے_انت
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#جے_تعریف_شدہ ECHOKE
      fputs (_("\
 * crtkill       kill all سطر by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all سطر by obeying the echoctl and echok settings\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters وچ hat notation ('^c')\n\
"), stdout);
#جے_انت
      fputs (_("\
   [-]echo       echo داخلہ characters\n\
"), stdout);
#جے_تعریف_شدہ ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#جے_انت
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#جے_تعریف_شدہ ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#جے_انت
      fputs (_("\
   [-]echonl     echo newline even إذا not echoing other characters\n\
"), stdout);
#جے_تعریف_شدہ ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#جے_انت
#إذا defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with بلند latency links\n\
"), stdout);
#جے_انت
#إذا defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#جے_انت
      لکھو (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#جے_تعریف_شدہ VWERASE
    ", werase"
#جے_انت
#إذا defined CREPRINT || defined VREPRINT
    ", rprnt"
#جے_انت
);
      fputs (_("\
   [-]isig       enable رکاوٹ, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after رکاوٹ and quit special characters\n\
"), stdout);
#جے_تعریف_شدہ ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try منزل اكتب منزل the terminal\n\
"), stdout);
#جے_انت
#جے_تعریف_شدہ XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' لكل uppercase characters\n\
"), stdout);
#جے_انت
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#إذا defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#جے_انت
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters منزل their افتراضي اقدار\n\
   -cooked       same as raw\n\
"), stdout);
      لکھو (_("\
   crt           same as %s\n\
"), "echoe"
#جے_تعریف_شدہ ECHOCTL
    " echoctl"
#جے_انت
#جے_تعریف_شدہ ECHOKE
    " echoke"
#جے_انت
);
      لکھو (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#جے_تعریف_شدہ ECHOCTL
    " echoctl"
#جے_انت
#جے_تعریف_شدہ ECHOKE
    " echoke"
#جے_انت
#جے_تعریف_شدہ IXANY
    " -ixany"
#جے_انت
);
#جے_تعریف_شدہ IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#جے_انت
      fputs (_("\
   ek            erase and kill characters منزل their افتراضي اقدار\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#إذا defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#جے_انت
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      لکھو (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#جے_تعریف_شدہ ONLCR
   " -onlcr"
#جے_انت
  , "icrnl -inlcr -igncr"
#جے_تعریف_شدہ ONLCR
   " onlcr"
#جے_انت
#جے_تعریف_شدہ OCRNL
   " -ocrnl"
#جے_انت
#جے_تعریف_شدہ ONLRET
   " -onlret"
#جے_انت
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
                 -isig%s min 1 ویلا 0\n\
   -raw          same as cooked\n\
"),
#جے_تعریف_شدہ IUCLC
   " -iuclc"
#جے_انت
#جے_تعریف_شدہ IXANY
   " -ixany"
#جے_انت
#جے_تعریف_شدہ IMAXBEL
   " -imaxbel"
#جے_انت
#جے_تعریف_شدہ XCASE
   " -xcase"
#جے_انت
);
      لکھو (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters منزل their افتراضي اقدار\n\
"),
   "-ixoff"
#جے_تعریف_شدہ IUTF8
   " -iutf8"
#جے_انت
#جے_تعریف_شدہ IUCLC
   " -iuclc"
#جے_انت
#جے_تعریف_شدہ IXANY
   " -ixany"
#جے_انت
#جے_تعریف_شدہ IMAXBEL
   " imaxbel"
#جے_انت
#جے_تعریف_شدہ XCASE
   " -xcase"
#جے_انت
#جے_تعریف_شدہ OLCUC
   " -olcuc"
#جے_انت
#جے_تعریف_شدہ OCRNL
   " -ocrnl"
#جے_انت

 , "opost"
#جے_تعریف_شدہ OFILL
   " -ofill"
#جے_انت
#جے_تعریف_شدہ ONLCR
   " onlcr"
#جے_انت
#جے_تعریف_شدہ ONOCR
   " -onocr"
#جے_انت
#جے_تعریف_شدہ ONLRET
   " -onlret"
#جے_انت
#جے_تعریف_شدہ NLDLY
   " nl0"
#جے_انت
#جے_تعریف_شدہ CRDLY
   " cr0"
#جے_انت
#جے_تعریف_شدہ TAB0
   " tab0"
#جے_انت
#جے_تعریف_شدہ BSDLY
   " bs0"
#جے_انت
#جے_تعریف_شدہ VTDLY
   " vt0"
#جے_انت
#جے_تعریف_شدہ FFDLY
   " ff0"
#جے_انت

 , "isig"
#جے_تعریف_شدہ TOSTOP
   " -tostop"
#جے_انت
#جے_تعریف_شدہ OFDEL
   " -ofdel"
#جے_انت
#جے_تعریف_شدہ ECHOPRT
   " -echoprt"
#جے_انت
#جے_تعریف_شدہ ECHOCTL
   " echoctl"
#جے_انت
#جے_تعریف_شدہ ECHOKE
   " echoke"
#جے_انت
#جے_تعریف_شدہ EXTPROC
   " -extproc"
#جے_انت
#جے_تعریف_شدہ FLUSHO
   " -flusho"
#جے_انت
);
      fputs (_("\
\n\
Handle the tty سطر connected منزل standard داخلہ.  Without arguments,\n\
prints baud شرح, سطر discipline, and deviations from stty sane.  وچ_2\n\
settings, CHAR is لیا literally, or coded as وچ ^c, 0x37, 0177 or\n\
127; special اقدار ^- or undef ورتیا منزل disable special characters.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

ساكن فراغ
apply_settings (bool checking, حرف ثابت *device_name,
                حرف * ثابت *settings, صحيح n_settings,
                بنية termios *منوال_2, bool *require_سیٹ_attr)
{
#تعریف_کرو check_دلیل(arg)						\
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
      bool میل_لبھیا = false;
      bool not_سیٹ_attr = false;
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
      لكل (صحيح i = 0; منوال_info[i].name != عدم; ++i)
        {
          إذا (streq (arg, منوال_info[i].name))
            {
              إذا ((منوال_info[i].flags & NO_SETATTR) == 0)
                {
                  میل_لبھیا = سیٹ_منوال (&منوال_info[i], reversed, منوال_2);
                  *require_سیٹ_attr = true;
                }
              وإلا
                میل_لبھیا = not_سیٹ_attr = true;
              اكسر;
            }
        }
      إذا (!میل_لبھیا && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      إذا (!میل_لبھیا)
        {
          لكل (صحيح i = 0; control_info[i].name != عدم; ++i)
            {
              إذا (streq (arg, control_info[i].name))
                {
                  check_دلیل (arg);
                  میل_لبھیا = true;
                  ++k;
                  سیٹ_control_char (&control_info[i], settings[k], منوال_2);
                  *require_سیٹ_attr = true;
                  اكسر;
                }
            }
        }
      إذا (!میل_لبھیا || not_سیٹ_attr)
        {
          إذا (streq (arg, "ispeed"))
            {
              check_دلیل (arg);
              ++k;
              بنية رفتار_desc ثابت ispeed = string_منزل_baud (settings[k]);
              إذا (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              سیٹ_رفتار (داخلہ_رفتار, settings[k], ispeed, منوال_2);
              إذا (checking)
                استمر;
              *require_سیٹ_attr = true;
            }
          وإلا إذا (streq (arg, "ospeed"))
            {
              check_دلیل (arg);
              ++k;
              بنية رفتار_desc ثابت ospeed = string_منزل_baud (settings[k]);
              إذا (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              سیٹ_رفتار (output_رفتار, settings[k], ospeed, منوال_2);
              إذا (checking)
                استمر;
              *require_سیٹ_attr = true;
            }
#جے_تعریف_شدہ TIOCEXT
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
#جے_انت
#جے_تعریف_شدہ TIOCGWINSZ
          وإلا إذا (streq (arg, "rows"))
            {
              check_دلیل (arg);
              ++k;
              إذا (checking)
                استمر;
              سیٹ_کھڑکی_ناپ (integer_arg (settings[k], ودھ_توں_ودھ_عدد), -1,
                               device_name);
            }
          وإلا إذا (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_دلیل (arg);
              ++k;
              إذا (checking)
                استمر;
              سیٹ_کھڑکی_ناپ (-1, integer_arg (settings[k], ودھ_توں_ودھ_عدد),
                               device_name);
            }
          وإلا إذا (streq (arg, "size"))
            {
              إذا (checking)
                استمر;
              max_خانہ = screen_columns ();
              current_خانہ = 0;
              display_کھڑکی_ناپ (false, device_name);
            }
#جے_انت
#جے_تعریف_شدہ HAVE_C_سطر
          وإلا إذا (streq (arg, "line"))
            {
              check_دلیل (arg);
              ++k;
              uintmax_t قدر = integer_arg (settings[k], UINTMAX_MAX);
              إذا (ckd_جوڑو (&منوال_2->c_سطر, قدر, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_سیٹ_attr = true;
            }
#جے_انت
          وإلا إذا (streq (arg, "speed"))
            {
              إذا (checking)
                استمر;
              max_خانہ = screen_columns ();
              display_رفتار (منوال_2, false);
            }
          وإلا
            {
              بنية رفتار_desc ثابت رفتار = string_منزل_baud (arg);
              إذا (رفتار.valid)
                {
                  سیٹ_رفتار (both_speeds, arg, رفتار, منوال_2);
                  إذا (checking)
                    استمر;
                  *require_سیٹ_attr = true;
                }
              وإلا
                {
                  إذا (! recover_منوال (arg, منوال_2))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_سیٹ_attr = true;
                }
            }
        }
    }

  إذا (checking)
    check_رفتار (منوال_2);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  ساكن بنية termios منوال_2;

  تعداد output_type output_type;
  صحيح optc;
  صحيح argi = 0;
  صحيح opti = 1;
  bool require_سیٹ_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  حرف *file_name = عدم;
  حرف ثابت *device_name;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = بدلیا;
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

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

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
      ساكن بنية termios check_منوال;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_منوال, &require_سیٹ_attr);
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

  إذا (tcgetattr (STDIN_FILENO, &منوال_2))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  إذا (verbose_output || recoverable_output || noargs)
    {
      max_خانہ = screen_columns ();
      current_خانہ = 0;
      display_settings (output_type, &منوال_2, device_name);
      أرجع EXIT_SUCCESS;
    }

  require_سیٹ_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &منوال_2, &require_سیٹ_attr);

  إذا (require_سیٹ_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      ساكن بنية termios new_منوال;

      إذا (tcsetattr (STDIN_FILENO, tcsetattr_options, &منوال_2))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      إذا (tcgetattr (STDIN_FILENO, &new_منوال))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      إذا (! eq_منوال (&منوال_2, &new_منوال))
        {
          إذا (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              لكل (بنانشانی صحيح i = 0; i < حجم (new_منوال); i++)
                {
                  بنانشانی صحيح newc = *(((بنانشانی حرف *) &new_منوال) + i);
                  بنانشانی صحيح oldc = *(((بنانشانی حرف *) &منوال_2) + i);
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
eq_منوال (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  إذا (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#جے_تعریف_شدہ HAVE_C_سطر
      || mode1->c_سطر != mode2->c_سطر
#جے_انت
      || !memeq (mode1->c_cc, mode2->c_cc, حجم (mode1->c_cc)))
    أرجع false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_نقاب = ~(tcflag_t) 0;
#جے_تعریف_شدہ CBAUD
  cflag_نقاب &= ~(tcflag_t) CBAUD;
#جے_انت
#جے_تعریف_شدہ CIBAUD
  cflag_نقاب &= ~(tcflag_t) CIBAUD;
#جے_انت

  إذا ((mode1->c_cflag ^ mode2->c_cflag) & cflag_نقاب)
    أرجع false;

  أرجع رفتار_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

ساكن bool
سیٹ_منوال (بنية منوال_info ثابت *info, bool reversed, بنية termios *منوال_2)
{
  tcflag_t *bitsp;

  إذا (reversed && (info->flags & REV) == 0)
    أرجع false;

  bitsp = منوال_type_flag (info->type, منوال_2);

  إذا (bitsp == عدم)
    {
      /* Combination mode. */
      إذا (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          إذا (reversed)
            منوال_2->c_cflag = (منوال_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            منوال_2->c_cflag = (منوال_2->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      وإلا إذا (streq (info->name, "oddp"))
        {
          إذا (reversed)
            منوال_2->c_cflag = (منوال_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          وإلا
            منوال_2->c_cflag = (منوال_2->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      وإلا إذا (streq (info->name, "nl"))
        {
          إذا (reversed)
            {
              منوال_2->c_iflag = (منوال_2->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              منوال_2->c_oflag = (منوال_2->c_oflag
#جے_تعریف_شدہ ONLCR
                               | ONLCR
#جے_انت
                )
#جے_تعریف_شدہ OCRNL
                & ~OCRNL
#جے_انت
#جے_تعریف_شدہ ONLRET
                & ~ONLRET
#جے_انت
                ;
            }
          وإلا
            {
              منوال_2->c_iflag = منوال_2->c_iflag & ~ICRNL;
#جے_تعریف_شدہ ONLCR
              منوال_2->c_oflag = منوال_2->c_oflag & ~ONLCR;
#جے_انت
            }
        }
      وإلا إذا (streq (info->name, "ek"))
        {
          منوال_2->c_cc[VERASE] = CERASE;
          منوال_2->c_cc[VKILL] = CKILL;
        }
      وإلا إذا (streq (info->name, "sane"))
        sane_منوال (منوال_2);
      وإلا إذا (streq (info->name, "cbreak"))
        {
          إذا (reversed)
            منوال_2->c_lflag |= ICANON;
          وإلا
            منوال_2->c_lflag &= ~ICANON;
        }
      وإلا إذا (streq (info->name, "pass8"))
        {
          إذا (reversed)
            {
              منوال_2->c_cflag = (منوال_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              منوال_2->c_iflag |= ISTRIP;
            }
          وإلا
            {
              منوال_2->c_cflag = (منوال_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              منوال_2->c_iflag &= ~ISTRIP;
            }
        }
      وإلا إذا (streq (info->name, "litout"))
        {
          إذا (reversed)
            {
              منوال_2->c_cflag = (منوال_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              منوال_2->c_iflag |= ISTRIP;
              منوال_2->c_oflag |= OPOST;
            }
          وإلا
            {
              منوال_2->c_cflag = (منوال_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              منوال_2->c_iflag &= ~ISTRIP;
              منوال_2->c_oflag &= ~OPOST;
            }
        }
      وإلا إذا (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          إذا ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              منوال_2->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              منوال_2->c_oflag |= OPOST;
              منوال_2->c_lflag |= ISIG | ICANON;
#إذا VMIN == VEOF
              منوال_2->c_cc[VEOF] = CEOF;
#جے_انت
#إذا VTIME == VEOL
              منوال_2->c_cc[VEOL] = CEOL;
#جے_انت
            }
          وإلا
            {
              /* Raw mode. */
              منوال_2->c_iflag = 0;
              منوال_2->c_oflag &= ~OPOST;
              منوال_2->c_lflag &= ~(ISIG | ICANON
#جے_تعریف_شدہ XCASE
                                 | XCASE
#جے_انت
                );
              منوال_2->c_cc[VMIN] = 1;
              منوال_2->c_cc[VTIME] = 0;
            }
        }
#جے_تعریف_شدہ IXANY
      وإلا إذا (streq (info->name, "decctlq"))
        {
          إذا (reversed)
            منوال_2->c_iflag |= IXANY;
          وإلا
            منوال_2->c_iflag &= ~IXANY;
        }
#جے_انت
#جے_تعریف_شدہ TABDLY
      وإلا إذا (streq (info->name, "tabs"))
        {
          إذا (reversed)
            منوال_2->c_oflag = (منوال_2->c_oflag & ~TABDLY) | TAB3;
          وإلا
            منوال_2->c_oflag = (منوال_2->c_oflag & ~TABDLY) | TAB0;
        }
#وإلا
# ifdef OXTABS
      وإلا إذا (streq (info->name, "tabs"))
        {
          إذا (reversed)
            منوال_2->c_oflag = منوال_2->c_oflag | OXTABS;
          وإلا
            منوال_2->c_oflag = منوال_2->c_oflag & ~OXTABS;
        }
# endif
#جے_انت
#إذا defined XCASE && defined IUCLC && defined OLCUC
      وإلا إذا (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          إذا (reversed)
            {
              منوال_2->c_lflag &= ~XCASE;
              منوال_2->c_iflag &= ~IUCLC;
              منوال_2->c_oflag &= ~OLCUC;
            }
          وإلا
            {
              منوال_2->c_lflag |= XCASE;
              منوال_2->c_iflag |= IUCLC;
              منوال_2->c_oflag |= OLCUC;
            }
        }
#جے_انت
      وإلا إذا (streq (info->name, "crt"))
        منوال_2->c_lflag |= ECHOE
#جے_تعریف_شدہ ECHOCTL
          | ECHOCTL
#جے_انت
#جے_تعریف_شدہ ECHOKE
          | ECHOKE
#جے_انت
          ;
      وإلا إذا (streq (info->name, "dec"))
        {
          منوال_2->c_cc[VINTR] = 3;	/* ^C */
          منوال_2->c_cc[VERASE] = 127;	/* DEL */
          منوال_2->c_cc[VKILL] = 21;	/* ^U */
          منوال_2->c_lflag |= ECHOE
#جے_تعریف_شدہ ECHOCTL
            | ECHOCTL
#جے_انت
#جے_تعریف_شدہ ECHOKE
            | ECHOKE
#جے_انت
            ;
#جے_تعریف_شدہ IXANY
          منوال_2->c_iflag &= ~IXANY;
#جے_انت
        }
    }
  وإلا إذا (reversed)
    *bitsp = *bitsp & ~info->نقاب & ~info->بٹ_2;
  وإلا
    *bitsp = (*bitsp & ~info->نقاب) | info->بٹ_2;

  أرجع true;
}

ساكن فراغ
سیٹ_control_char (بنية control_info ثابت *info, حرف ثابت *arg,
                  بنية termios *منوال_2)
{
  بنانشانی لما صحيح قدر;

  إذا (streq (info->name, "min") || streq (info->name, "time"))
    قدر = integer_arg (arg, TYPE_ودھ_توں_ودھ (cc_t));
  وإلا إذا (arg[0] == '\0' || arg[1] == '\0')
    قدر = منزل_uchar (arg[0]);
  وإلا إذا (streq (arg, "^-") || streq (arg, "undef"))
    قدر = _POSIX_VDISABLE;
  وإلا إذا (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      إذا (arg[1] == '?')
        قدر = 127;
      وإلا
        قدر = منزل_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  وإلا
    قدر = integer_arg (arg, TYPE_ودھ_توں_ودھ (cc_t));
  منوال_2->c_cc[info->وتھ] = قدر;
}

ساكن فراغ
سیٹ_رفتار (تعداد رفتار_setting type, حرف ثابت *arg,
           بنية رفتار_desc ثابت رفتار, بنية termios *منوال_2)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (رفتار.valid);

  إذا (type == داخلہ_رفتار || type == both_speeds)
    {
      آخری_ibaud = رفتار;
      إذا (cfsetispeed (منوال_2, رفتار.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  إذا (type == output_رفتار || type == both_speeds)
    {
      آخری_obaud = رفتار;
      إذا (cfsetospeed (منوال_2, رفتار.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#جے_تعریف_شدہ TIOCGWINSZ

ساكن صحيح
get_win_ناپ (صحيح fd, بنية winsize *win)
{
  صحيح err = ioctl (fd, TIOCGWINSZ, (حرف *) win);
  أرجع err;
}

ساكن فراغ
سیٹ_کھڑکی_ناپ (صحيح صفاں, صحيح خانے_گنتی_2, حرف ثابت *device_name)
{
  بنية winsize win;

  إذا (get_win_ناپ (STDIN_FILENO, &win))
    {
      إذا (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, حجم (win));
    }

  إذا (صفاں >= 0)
    win.ws_صف = صفاں;
  إذا (خانے_گنتی_2 >= 0)
    win.ws_خانہ = خانے_گنتی_2;

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

  إذا (win.ws_صف == 0 || win.ws_خانہ == 0)
    {
      بنية ttysize ttysz;

      ttysz.ts_سطریں = win.ws_صف;
      ttysz.ts_خانے_گنتی = win.ws_خانہ;

      win.ws_صف = 1;
      win.ws_خانہ = 1;

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
display_کھڑکی_ناپ (bool fancy, حرف ثابت *device_name)
{
  بنية winsize win;

  إذا (get_win_ناپ (STDIN_FILENO, &win))
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
             win.ws_صف, win.ws_خانہ);
      إذا (!fancy)
        current_خانہ = 0;
    }
}
#جے_انت

ساكن صحيح
screen_columns (فراغ)
{
#جے_تعریف_شدہ TIOCGWINSZ
  بنية winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  إذا (get_win_ناپ (STDOUT_FILENO, &win) == 0 && 0 < win.ws_خانہ)
    أرجع win.ws_خانہ;
#جے_انت
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    حرف *خانہ_string = getenv ("COLUMNS");
    لما صحيح n_columns;
    إذا (!(خانہ_string != عدم
          && xstrtol (خانہ_string, عدم, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= ودھ_توں_ودھ_عدد))
      n_columns = 80;
    أرجع n_columns;
  }
}

ATTRIBUTE_خالص
ساكن tcflag_t *
منوال_type_flag (تعداد منوال_type type, بنية termios *منوال_2)
{
  بدل (type)
    {
    حالة control:
      أرجع &منوال_2->c_cflag;

    حالة داخلہ:
      أرجع &منوال_2->c_iflag;

    حالة output:
      أرجع &منوال_2->c_oflag;

    حالة local:
      أرجع &منوال_2->c_lflag;

    حالة combination:
      أرجع عدم;

    افتراضي:
      unreachable ();
    }
}

ساكن فراغ
display_settings (تعداد output_type output_type, بنية termios *منوال_2,
                  حرف ثابت *device_name)
{
  بدل (output_type)
    {
    حالة بدلیا:
      display_بدلیا (منوال_2);
      اكسر;

    حالة all:
      display_all (منوال_2, device_name);
      اكسر;

    حالة recoverable:
      display_recoverable (منوال_2);
      اكسر;
    }
}

ساكن فراغ
display_بدلیا (بنية termios *منوال_2)
{
  bool خالی_تھاواں_سطر;
  tcflag_t *bitsp;
  بنانشانی لما نقاب;
  تعداد منوال_type prev_type = control;

  display_رفتار (منوال_2, true);
#جے_تعریف_شدہ HAVE_C_سطر
  wrapf ("line = %d;", منوال_2->c_سطر);
#جے_انت
  حرف_لکھو ('\n');
  current_خانہ = 0;

  خالی_تھاواں_سطر = true;
  لكل (صحيح i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      إذا (منوال_2->c_cc[control_info[i].وتھ] == control_info[i].saneval)
        استمر;

#جے_تعریف_شدہ VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      إذا (streq (control_info[i].name, "flush"))
        استمر;
#جے_انت
      /* If swtch is the same as susp, don't print both.  */
#إذا VSWTCH == VSUSP
      إذا (streq (control_info[i].name, "swtch"))
        استمر;
#جے_انت
      /* If eof uses the same slot as min, only print whichever applies.  */
#إذا VEOF == VMIN
      إذا ((منوال_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#جے_انت

      خالی_تھاواں_سطر = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (منوال_2->c_cc[control_info[i].وتھ]));
    }
  إذا ((منوال_2->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (بنانشانی لما صحيح) منوال_2->c_cc[VMIN],
             (بنانشانی لما صحيح) منوال_2->c_cc[VTIME]);
    }
  وإلا إذا (!خالی_تھاواں_سطر)
    حرف_لکھو ('\n');
  current_خانہ = 0;

  خالی_تھاواں_سطر = true;
  لكل (صحيح i = 0; منوال_info[i].name != عدم; ++i)
    {
      إذا (منوال_info[i].flags & OMIT)
        استمر;
      إذا (منوال_info[i].type != prev_type)
        {
          إذا (!خالی_تھاواں_سطر)
            {
              حرف_لکھو ('\n');
              current_خانہ = 0;
              خالی_تھاواں_سطر = true;
            }
          prev_type = منوال_info[i].type;
        }

      bitsp = منوال_type_flag (منوال_info[i].type, منوال_2);
      نقاب = منوال_info[i].نقاب ? منوال_info[i].نقاب : منوال_info[i].بٹ_2;
      إذا ((*bitsp & نقاب) == منوال_info[i].بٹ_2)
        {
          إذا (منوال_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", منوال_info[i].name);
              خالی_تھاواں_سطر = false;
            }
        }
      وإلا إذا ((منوال_info[i].flags & (SANE_سیٹ | REV)) == (SANE_سیٹ | REV))
        {
          wrapf ("-%s", منوال_info[i].name);
          خالی_تھاواں_سطر = false;
        }
    }
  إذا (!خالی_تھاواں_سطر)
    حرف_لکھو ('\n');
  current_خانہ = 0;
}

ساكن فراغ
display_all (بنية termios *منوال_2, حرف ثابت *device_name)
{
  tcflag_t *bitsp;
  بنانشانی لما نقاب;
  تعداد منوال_type prev_type = control;

  display_رفتار (منوال_2, true);
#جے_تعریف_شدہ TIOCGWINSZ
  display_کھڑکی_ناپ (true, device_name);
#جے_انت
#جے_تعریف_شدہ HAVE_C_سطر
  wrapf ("line = %d;", منوال_2->c_سطر);
#جے_انت
  حرف_لکھو ('\n');
  current_خانہ = 0;

  لكل (صحيح i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#جے_تعریف_شدہ VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      إذا (streq (control_info[i].name, "flush"))
        استمر;
#جے_انت
      /* If swtch is the same as susp, don't print both.  */
#إذا VSWTCH == VSUSP
      إذا (streq (control_info[i].name, "swtch"))
        استمر;
#جے_انت
      /* If eof uses the same slot as min, only print whichever applies.  */
#إذا VEOF == VMIN
      إذا ((منوال_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        استمر;
#جے_انت
      wrapf ("%s = %s;", control_info[i].name,
             visible (منوال_2->c_cc[control_info[i].وتھ]));
    }
#إذا VEOF == VMIN
  إذا ((منوال_2->c_lflag & ICANON) == 0)
#جے_انت
    wrapf ("min = %lu; time = %lu;",
           (بنانشانی لما صحيح) منوال_2->c_cc[VMIN],
           (بنانشانی لما صحيح) منوال_2->c_cc[VTIME]);
  إذا (current_خانہ != 0)
    حرف_لکھو ('\n');
  current_خانہ = 0;

  لكل (صحيح i = 0; منوال_info[i].name != عدم; ++i)
    {
      إذا (منوال_info[i].flags & OMIT)
        استمر;
      إذا (منوال_info[i].type != prev_type)
        {
          حرف_لکھو ('\n');
          current_خانہ = 0;
          prev_type = منوال_info[i].type;
        }

      bitsp = منوال_type_flag (منوال_info[i].type, منوال_2);
      نقاب = منوال_info[i].نقاب ? منوال_info[i].نقاب : منوال_info[i].بٹ_2;
      إذا ((*bitsp & نقاب) == منوال_info[i].بٹ_2)
        wrapf ("%s", منوال_info[i].name);
      وإلا إذا (منوال_info[i].flags & REV)
        wrapf ("-%s", منوال_info[i].name);
    }
  حرف_لکھو ('\n');
  current_خانہ = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

ساكن فراغ
check_رفتار (بنية termios *منوال_2)
{
  إذا (آخری_ibaud.valid && آخری_obaud.valid)
    {
      إذا (cfgetispeed (منوال_2) != آخری_ibaud.baud
          || cfgetospeed (منوال_2) != آخری_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_منزل_قدر (آخری_ibaud.baud),
               baud_منزل_قدر (آخری_obaud.baud));
    }
}

ساكن فراغ
display_رفتار (بنية termios *منوال_2, bool fancy)
{
  إذا (cfgetispeed (منوال_2) == 0 || cfgetispeed (منوال_2) == cfgetospeed (منوال_2))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_منزل_قدر (cfgetospeed (منوال_2)));
  وإلا
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_منزل_قدر (cfgetispeed (منوال_2)),
           baud_منزل_قدر (cfgetospeed (منوال_2)));
  إذا (!fancy)
    current_خانہ = 0;
}

ساكن فراغ
display_recoverable (بنية termios *منوال_2)
{
  لکھو ("%lx:%lx:%lx:%lx",
          (بنانشانی لما صحيح) منوال_2->c_iflag,
          (بنانشانی لما صحيح) منوال_2->c_oflag,
          (بنانشانی لما صحيح) منوال_2->c_cflag,
          (بنانشانی لما صحيح) منوال_2->c_lflag);
  لكل (size_t i = 0; i < NCCS; ++i)
    لکھو (":%lx", (بنانشانی لما صحيح) منوال_2->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#جے_تعریف_شدہ CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (منوال_2->c_cflag & CBAUD) == BOTHER;
#  إذا defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((منوال_2->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#جے_انت
  إذا (separate_speeds)
    لکھو (":%lx:%lx", (بنانشانی لما صحيح) cfgetispeed (منوال_2),
            (بنانشانی لما صحيح) cfgetospeed (منوال_2));

  حرف_لکھو ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#تعریف_کرو STRTOUL_TYPE(TYPE)                                               \
  ساكن صحيح                                                             \
  strtoul_##TYPE (حرف ثابت *s, صحيح ادھار_2, حرف **p, TYPE *نتیجہ,       \
                  حرف delim)                                            \
  {                                                                      \
    بنانشانی لما ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, ادھار_2);                                           \
    إذا (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      أرجع -1;                                                         \
    *نتیجہ = ul;                                                        \
    أرجع 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (رفتار_t)
#تعریف_رد STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#ضمن "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
ساكن bool
recover_منوال (حرف ثابت *arg, بنية termios *منوال_2)
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
  منوال_2->c_iflag = flag[0];
  منوال_2->c_oflag = flag[1];
  منوال_2->c_cflag = flag[2];
  منوال_2->c_lflag = flag[3];

  لكل (size_t i = 0; i < NCCS; ++i)
    {
      حرف *p;
      حرف delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      إذا (strtoul_cc_t (s, 16, &p, منوال_2->c_cc + i, delim) != 0)
        أرجع false;
      /* Accept the old format, which has no separate speed fields.  */
      إذا (! *p)
        {
#جے_تعریف_شدہ CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_رفتار_T_SANE
          بنية رفتار_desc ospeed = cbaud_منزل_رفتار (منوال_2->c_cflag & CBAUD);
          إذا (ospeed.valid && cfsetospeed (منوال_2, ospeed.baud) != 0)
            أرجع false;
#  إذا defined CIBAUD && defined IBSHIFT
          بنية رفتار_desc ispeed =
            cbaud_منزل_رفتار ((منوال_2->c_cflag & CIBAUD) >> IBSHIFT);
          إذا (ispeed.valid && cfsetispeed (منوال_2, ispeed.baud) != 0)
            أرجع false;
#  endif
# endif
          آخری_ibaud.baud = cfgetispeed (منوال_2);
          آخری_obaud.baud = cfgetospeed (منوال_2);
#جے_انت
          أرجع true;
        }
      s = p + 1;
    }

  رفتار_t رفتار[2];
  لكل (size_t i = 0; i < 2; i++)
    {
      حرف *p;
      حرف delim = i == 0 ? ':' : '\0';
      إذا (strtoul_رفتار_t (s, 16, &p, رفتار + i, delim) != 0)
        أرجع false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  إذا (cfsetospeed (منوال_2, رفتار[1]) != 0
      || cfsetispeed (منوال_2, رفتار[0]) != 0)
    أرجع false;

  آخری_ibaud.baud = رفتار[0] == 0 ? cfgetispeed (منوال_2) : رفتار[0];
  آخری_obaud.baud = رفتار[1];
  أرجع true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
ساكن bool
رفتار_matches (بنية termios ثابت *mode1, بنية termios ثابت *mode2)
{
  رفتار_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  رفتار_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  إذا (speed1[1] == 0)
    speed1[1] = speed1[0];
  إذا (speed2[1] == 0)
    speed2[1] = speed2[0];

  بنانشانی صحيح بٹ_2 = 2;  /* Start and stop bits.  */
  بٹ_2 += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  بٹ_2 += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  بدل (mode1->c_cflag & CSIZE)
    {
      حالة CS5: بٹ_2 += 5; اكسر;
      حالة CS6: بٹ_2 += 6; اكسر;
      حالة CS7: بٹ_2 += 7; اكسر;
      افتراضي: بٹ_2 += 8; اكسر;
    }

  لكل (صحيح i = 0; i < 2; i++)
    {
      بنانشانی لما صحيح value1;
      بنانشانی لما صحيح value2;
      بنانشانی لما صحيح delta;

      إذا (speed1[i] == speed2[i])
        استمر;

      value1 = baud_منزل_قدر (speed1[i]);
      value2 = baud_منزل_قدر (speed2[i]);

#جے_بنا_تعریف TERMIOS_رفتار_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      إذا (value1 == ULONG_MAX || value2 == ULONG_MAX)
        أرجع false;
#جے_انت

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      إذا (delta > value1 / (بٹ_2 * 4))
        أرجع false;
    }

  أرجع true;
}

ATTRIBUTE_خالص
ساكن بنية رفتار_desc
string_منزل_baud (حرف ثابت *arg)
{
  حرف *ep;
  بنانشانی لما قدر;
  بنانشانی حرف c;

  /* Explicitly disallow negative numbers.  */
  طالما (c_isspace (*arg))
    arg++;
  إذا (*arg == '-')
    اذهب invalid;

  /* Empty does not mean 0.  */
  إذا (! *arg)
    اذهب invalid;

  قدر = strtoul (arg, &ep, 10);

  c = *ep++;
  إذا (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      إذا (c)
        {
          بنانشانی حرف d = c - '0';
          إذا (d > 5)
            قدر++;
          وإلا إذا (d == 5)
            {
              طالما ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              إذا (c)
                قدر++;                /* Nonzero, round up */
              وإلا
                قدر += (قدر & 1);   /* Exactly in the middle, round even */
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
        أرجع (بنية رفتار_desc) { .baud = B19200, .valid = true };
      وإلا إذا (streq (arg, "extb"))
        أرجع (بنية رفتار_desc) { .baud = B38400, .valid = true };
      وإلا
        اذهب invalid;
    }

  أرجع قدر_منزل_baud (قدر);

 invalid:
  أرجع (بنية رفتار_desc) { .baud = (رفتار_t) -1, .valid = false };
}

ساكن فراغ
sane_منوال (بنية termios *منوال_2)
{
  tcflag_t *bitsp;

  لكل (صحيح i = 0; control_info[i].name; ++i)
    {
#إذا VMIN == VEOF
      إذا (streq (control_info[i].name, "min"))
        اكسر;
#جے_انت
      منوال_2->c_cc[control_info[i].وتھ] = control_info[i].saneval;
    }

  لكل (صحيح i = 0; منوال_info[i].name != عدم; ++i)
    {
      إذا (منوال_info[i].flags & NO_SETATTR)
        استمر;

      إذا (منوال_info[i].flags & SANE_سیٹ)
        {
          bitsp = منوال_type_flag (منوال_info[i].type, منوال_2);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~منوال_info[i].نقاب) | منوال_info[i].بٹ_2;
        }
      وإلا إذا (منوال_info[i].flags & SANE_UNSET)
        {
          bitsp = منوال_type_flag (منوال_info[i].type, منوال_2);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~منوال_info[i].نقاب & ~منوال_info[i].بٹ_2;
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
