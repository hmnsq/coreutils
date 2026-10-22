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

#అన్తర్భావ <config.h>

#నిర్వచితమైతే TERMIOS_NEEDS_XOPEN_ఆధారం
# define XOPEN_ఆధారం
#అయితే_ముగింపు

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ <termios.h>
#ఒకవేళ HAVE_STROPTS_H
# include <stropts.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/ioctl.h>

#నిర్వచితమైతే WINSIZE_లోపల_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#అయితే_ముగింపు
#నిర్వచితమైతే GWINSZ_లోపల_SYS_PTY
# include <sys/tty.h>
# include <sys/pty.h>
#అయితే_ముగింపు
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdarg.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "fd-reopen.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xdectoint.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "stty"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

#నిర్వచితం_కాకపోతే _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#అయితే_ముగింపు

#నిర్వచించు Control(c) ((c) & 0x1f)
/* Canonical values for control characters. */
#నిర్వచితం_కాకపోతే CINTR
# define CINTR Control ('c')
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CQUIT
# define CQUIT 28
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CERASE
# define CERASE 127
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CKILL
# define CKILL Control ('u')
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CEOF
# define CEOF Control ('d')
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CEOL
# define CEOL _POSIX_VDISABLE
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CSTART
# define CSTART Control ('q')
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CSTOP
# define CSTOP Control ('s')
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CSUSP
# define CSUSP Control ('z')
#అయితే_ముగింపు
#ఒకవేళ defined VEOL2 && !defined CEOL2
# define CEOL2 _POSIX_VDISABLE
#అయితే_ముగింపు
/* Some platforms have VSWTC, others VSWTCH.  In both cases, this control
   character is initialized by CSWTCH, if present.  */
#ఒకవేళ defined VSWTC && !defined VSWTCH
# define VSWTCH VSWTC
#అయితే_ముగింపు
/* ISC renamed swtch to susp for termios, but we'll accept either name.  */
#ఒకవేళ defined VSUSP && !defined VSWTCH
# define VSWTCH VSUSP
# ఒకవేళ defined CSUSP && !defined CSWTCH
#  define CSWTCH CSUSP
# endif
#అయితే_ముగింపు
#ఒకవేళ defined VSWTCH && !defined CSWTCH
# define CSWTCH _POSIX_VDISABLE
#అయితే_ముగింపు

/* SunOS >= 5.3 loses (^Z doesn't work) if 'swtch' is the same as 'susp'.
   So the default is to disable 'swtch.'  */
#ఒకవేళ defined __sun
# undef CSWTCH
# define CSWTCH _POSIX_VDISABLE
#అయితే_ముగింపు

#ఒకవేళ defined VWERSE && !defined VWERASE	/* AIX-3.2.5 */
# define VWERASE VWERSE
#అయితే_ముగింపు
#ఒకవేళ defined VDSUSP && !defined CDSUSP
# define CDSUSP Control ('y')
#అయితే_ముగింపు
#ఒకవేళ defined VREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#అయితే_ముగింపు
#ఒకవేళ defined CREPRINT && !defined CRPRNT
# define CRPRNT Control ('r')
#అయితే_ముగింపు
#ఒకవేళ defined VWERASE && !defined CWERASE
# define CWERASE Control ('w')
#అయితే_ముగింపు
#ఒకవేళ defined VLNEXT && !defined CLNEXT
# define CLNEXT Control ('v')
#అయితే_ముగింపు
#ఒకవేళ defined VDISCARD && !defined VFLUSHO
# define VFLUSHO VDISCARD
#అయితే_ముగింపు
#ఒకవేళ defined VFLUSH && !defined VFLUSHO	/* Ultrix 4.2 */
# define VFLUSHO VFLUSH
#అయితే_ముగింపు
#ఒకవేళ defined CTLECH && !defined ECHOCTL	/* Ultrix 4.3 */
# define ECHOCTL CTLECH
#అయితే_ముగింపు
#ఒకవేళ defined TCTLECH && !defined ECHOCTL	/* Ultrix 4.2 */
# define ECHOCTL TCTLECH
#అయితే_ముగింపు
#ఒకవేళ defined CRTKIL && !defined ECHOKE	/* Ultrix 4.2 and 4.3 */
# define ECHOKE CRTKIL
#అయితే_ముగింపు
#ఒకవేళ defined VFLUSHO && !defined CFLUSHO
# define CFLUSHO Control ('o')
#అయితే_ముగింపు
#ఒకవేళ defined VSTATUS && !defined CSTATUS
# define CSTATUS Control ('t')
#అయితే_ముగింపు

/* Which speeds to set.  */
గణన వేగం_setting
  {
    ఆదానం_వేగం, output_వేగం, both_speeds
  };

/* What to output and how.  */
గణన output_type
  {
    మారింది, all, recoverable	/* Default, -a, -g.  */
  };

/* Which member(s) of 'struct termios' a mode uses.  */
గణన బహుళకం_type
  {
    control, ఆదానం, output, local, combination
  };

/* Flags for 'struct mode_info'. */
#నిర్వచించు SANE_సమితి 1		/* Set in 'sane' mode. */
#నిర్వచించు SANE_UNSET 2		/* Unset in 'sane' mode. */
#నిర్వచించు REV 4			/* Can be turned off by prepending '-'. */
#నిర్వచించు OMIT 8			/* Don't display value. */
#నిర్వచించు NO_SETATTR 16		/* tcsetattr not used to set mode bits.  */

/* Each mode.  */
నిర్మాణం బహుళకం_info
  {
    అక్షరం స్థిరరాశి *name;		/* Name given on command line.  */
    గణన బహుళకం_type type;	/* Which structure element to change. */
    అక్షరం flags;			/* Setting and display options.  */
    గుర్తులేని దీర్ఘ బిట్లు_2;		/* Bits to set for this mode.  */
    గుర్తులేని దీర్ఘ ముసుగు;		/* Other bits to turn off for this mode.  */
  };

స్థిర నిర్మాణం బహుళకం_info స్థిరరాశి బహుళకం_info[] =
{
  {"parenb", control, REV, PARENB, 0},
  {"parodd", control, REV, PARODD, 0},
#నిర్వచితమైతే CMSPAR
  {"cmspar", control, REV, CMSPAR, 0},
#అయితే_ముగింపు
  {"cs5", control, 0, CS5, CSIZE},
  {"cs6", control, 0, CS6, CSIZE},
  {"cs7", control, 0, CS7, CSIZE},
  {"cs8", control, 0, CS8, CSIZE},
  {"hupcl", control, REV, HUPCL, 0},
  {"hup", control, REV | OMIT, HUPCL, 0},
  {"cstopb", control, REV, CSTOPB, 0},
  {"cread", control, SANE_సమితి | REV, CREAD, 0},
  {"clocal", control, REV, CLOCAL, 0},
#నిర్వచితమైతే CRTSCTS
  {"crtscts", control, REV, CRTSCTS, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే CDTRDSR
  {"cdtrdsr", control, REV, CDTRDSR, 0},
#అయితే_ముగింపు

  {"ignbrk", ఆదానం, SANE_UNSET | REV, IGNBRK, 0},
  {"brkint", ఆదానం, SANE_సమితి | REV, BRKINT, 0},
  {"ignpar", ఆదానం, REV, IGNPAR, 0},
  {"parmrk", ఆదానం, REV, PARMRK, 0},
  {"inpck", ఆదానం, REV, INPCK, 0},
  {"istrip", ఆదానం, REV, ISTRIP, 0},
  {"inlcr", ఆదానం, SANE_UNSET | REV, INLCR, 0},
  {"igncr", ఆదానం, SANE_UNSET | REV, IGNCR, 0},
  {"icrnl", ఆదానం, SANE_సమితి | REV, ICRNL, 0},
  {"ixon", ఆదానం, REV, IXON, 0},
  {"ixoff", ఆదానం, SANE_UNSET | REV, IXOFF, 0},
  {"tandem", ఆదానం, REV | OMIT, IXOFF, 0},
#నిర్వచితమైతే IUCLC
  {"iuclc", ఆదానం, SANE_UNSET | REV, IUCLC, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే IXANY
  {"ixany", ఆదానం, SANE_UNSET | REV, IXANY, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే IMAXBEL
  {"imaxbel", ఆదానం, SANE_సమితి | REV, IMAXBEL, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే IUTF8
  {"iutf8", ఆదానం, SANE_UNSET | REV, IUTF8, 0},
#అయితే_ముగింపు

  {"opost", output, SANE_సమితి | REV, OPOST, 0},
#నిర్వచితమైతే OLCUC
  {"olcuc", output, SANE_UNSET | REV, OLCUC, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే OCRNL
  {"ocrnl", output, SANE_UNSET | REV, OCRNL, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే ONLCR
  {"onlcr", output, SANE_సమితి | REV, ONLCR, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే ONOCR
  {"onocr", output, SANE_UNSET | REV, ONOCR, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే ONLRET
  {"onlret", output, SANE_UNSET | REV, ONLRET, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే OFILL
  {"ofill", output, SANE_UNSET | REV, OFILL, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే OFDEL
  {"ofdel", output, SANE_UNSET | REV, OFDEL, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే NLDLY
  {"nl1", output, SANE_UNSET, NL1, NLDLY},
  {"nl0", output, SANE_సమితి, NL0, NLDLY},
#అయితే_ముగింపు
#నిర్వచితమైతే CRDLY
  {"cr3", output, SANE_UNSET, CR3, CRDLY},
  {"cr2", output, SANE_UNSET, CR2, CRDLY},
  {"cr1", output, SANE_UNSET, CR1, CRDLY},
  {"cr0", output, SANE_సమితి, CR0, CRDLY},
#అయితే_ముగింపు
#నిర్వచితమైతే TABDLY
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
  {"tab0", output, SANE_సమితి, TAB0, TABDLY},
# endif
#లేకపోతే
# ifdef OXTABS
  {"tab3", output, SANE_UNSET, OXTABS, 0},
# endif
#అయితే_ముగింపు
#నిర్వచితమైతే BSDLY
  {"bs1", output, SANE_UNSET, BS1, BSDLY},
  {"bs0", output, SANE_సమితి, BS0, BSDLY},
#అయితే_ముగింపు
#నిర్వచితమైతే VTDLY
  {"vt1", output, SANE_UNSET, VT1, VTDLY},
  {"vt0", output, SANE_సమితి, VT0, VTDLY},
#అయితే_ముగింపు
#నిర్వచితమైతే FFDLY
  {"ff1", output, SANE_UNSET, FF1, FFDLY},
  {"ff0", output, SANE_సమితి, FF0, FFDLY},
#అయితే_ముగింపు

  {"isig", local, SANE_సమితి | REV, ISIG, 0},
  {"icanon", local, SANE_సమితి | REV, ICANON, 0},
#నిర్వచితమైతే IEXTEN
  {"iexten", local, SANE_సమితి | REV, IEXTEN, 0},
#అయితే_ముగింపు
  {"echo", local, SANE_సమితి | REV, ECHO, 0},
  {"echoe", local, SANE_సమితి | REV, ECHOE, 0},
  {"crterase", local, REV | OMIT, ECHOE, 0},
  {"echok", local, SANE_సమితి | REV, ECHOK, 0},
  {"echonl", local, SANE_UNSET | REV, ECHONL, 0},
  {"noflsh", local, SANE_UNSET | REV, NOFLSH, 0},
#నిర్వచితమైతే XCASE
  {"xcase", local, SANE_UNSET | REV, XCASE, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే TOSTOP
  {"tostop", local, SANE_UNSET | REV, TOSTOP, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOPRT
  {"echoprt", local, SANE_UNSET | REV, ECHOPRT, 0},
  {"prterase", local, REV | OMIT, ECHOPRT, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOCTL
  {"echoctl", local, SANE_సమితి | REV, ECHOCTL, 0},
  {"ctlecho", local, REV | OMIT, ECHOCTL, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOKE
  {"echoke", local, SANE_సమితి | REV, ECHOKE, 0},
  {"crtkill", local, REV | OMIT, ECHOKE, 0},
#అయితే_ముగింపు
#నిర్వచితమైతే FLUSHO
  {"flusho", local, SANE_UNSET | REV, FLUSHO, 0},
#అయితే_ముగింపు
#ఒకవేళ defined TIOCEXT
  {"extproc", local, SANE_UNSET | REV | NO_SETATTR, EXTPROC, 0},
#లేదావేళ defined EXTPROC
  {"extproc", local, SANE_UNSET | REV, EXTPROC, 0},
#అయితే_ముగింపు

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
#నిర్వచితమైతే IXANY
  {"decctlq", combination, REV | OMIT, 0, 0},
#అయితే_ముగింపు
#ఒకవేళ defined TABDLY || defined OXTABS
  {"tabs", combination, REV | OMIT, 0, 0},
#అయితే_ముగింపు
#ఒకవేళ defined XCASE && defined IUCLC && defined OLCUC
  {"lcase", combination, REV | OMIT, 0, 0},
  {"LCASE", combination, REV | OMIT, 0, 0},
#అయితే_ముగింపు
  {"crt", combination, OMIT, 0, 0},
  {"dec", combination, OMIT, 0, 0},

  {ఖాళీ, control, 0, 0, 0}
};

/* Control character settings.  */
నిర్మాణం control_info
  {
    అక్షరం స్థిరరాశి *name;		/* Name given on command line.  */
    cc_t saneval;		/* Value to set for 'stty sane'.  */
    size_t స్థానభ్రంశం;		/* Offset in c_cc.  */
  };

/* Control characters. */

స్థిర నిర్మాణం control_info స్థిరరాశి control_info[] =
{
  {"intr", CINTR, VINTR},
  {"quit", CQUIT, VQUIT},
  {"erase", CERASE, VERASE},
  {"kill", CKILL, VKILL},
  {"eof", CEOF, VEOF},
  {"eol", CEOL, VEOL},
#నిర్వచితమైతే VEOL2
  {"eol2", CEOL2, VEOL2},
#అయితే_ముగింపు
#నిర్వచితమైతే VSWTCH
  {"swtch", CSWTCH, VSWTCH},
#అయితే_ముగింపు
  {"start", CSTART, VSTART},
  {"stop", CSTOP, VSTOP},
  {"susp", CSUSP, VSUSP},
#నిర్వచితమైతే VDSUSP
  {"dsusp", CDSUSP, VDSUSP},
#అయితే_ముగింపు
#నిర్వచితమైతే VREPRINT
  {"rprnt", CRPRNT, VREPRINT},
#లేకపోతే
# ifdef CREPRINT /* HPUX 10.20 needs this */
  {"rprnt", CRPRNT, CREPRINT},
# endif
#అయితే_ముగింపు
#నిర్వచితమైతే VWERASE
  {"werase", CWERASE, VWERASE},
#అయితే_ముగింపు
#నిర్వచితమైతే VLNEXT
  {"lnext", CLNEXT, VLNEXT},
#అయితే_ముగింపు
#నిర్వచితమైతే VFLUSHO
  {"flush", CFLUSHO, VFLUSHO},   /* deprecated compat option.  */
  {"discard", CFLUSHO, VFLUSHO},
#అయితే_ముగింపు
#నిర్వచితమైతే VSTATUS
  {"status", CSTATUS, VSTATUS},
#అయితే_ముగింపు

  /* These must be last because of the display routines. */
  {"min", 1, VMIN},
  {"time", 0, VTIME},
  {ఖాళీ, 0, 0}
};

/* This struct is needed because some systems, e.g., GNU/Linux,
   support baud rates equal to (speed_t) -1.  */
నిర్మాణం వేగం_desc
{
  /* The baud rate.  */
  వేగం_t baud;

  /* True if the baud member is valid and did not overflow.  */
  bool valid;
};

స్థిర అక్షరం స్థిరరాశి *visible (cc_t ch);
స్థిర గుర్తులేని దీర్ఘ పూర్ణాంకం baud_గమ్యం_విలువ (వేగం_t వేగం);
స్థిర bool recover_బహుళకం (అక్షరం స్థిరరాశి *arg, నిర్మాణం termios *బహుళకం_2);
స్థిర పూర్ణాంకం screen_columns (శూన్య);
స్థిర bool సమితి_బహుళకం (నిర్మాణం బహుళకం_info స్థిరరాశి *info, bool reversed,
                      నిర్మాణం termios *బహుళకం_2);
స్థిర bool eq_బహుళకం (నిర్మాణం termios స్థిరరాశి *mode1,
                     నిర్మాణం termios స్థిరరాశి *mode2);
స్థిర bool వేగం_matches (నిర్మాణం termios స్థిరరాశి *mode1,
                           నిర్మాణం termios స్థిరరాశి *mode2);
స్థిర uintmax_t integer_arg (అక్షరం స్థిరరాశి *s, uintmax_t max);
స్థిర నిర్మాణం వేగం_desc string_గమ్యం_baud (అక్షరం స్థిరరాశి *arg);
స్థిర tcflag_t *బహుళకం_type_flag (గణన బహుళకం_type type, నిర్మాణం termios *బహుళకం_2);
స్థిర శూన్య display_all (నిర్మాణం termios *బహుళకం_2, అక్షరం స్థిరరాశి *device_name);
స్థిర శూన్య display_మారింది (నిర్మాణం termios *బహుళకం_2);
స్థిర శూన్య display_recoverable (నిర్మాణం termios *బహుళకం_2);
స్థిర శూన్య display_settings (గణన output_type output_type,
                              నిర్మాణం termios *బహుళకం_2,
                              అక్షరం స్థిరరాశి *device_name);
స్థిర శూన్య check_వేగం (నిర్మాణం termios *బహుళకం_2);
స్థిర శూన్య display_వేగం (నిర్మాణం termios *బహుళకం_2, bool fancy);
స్థిర శూన్య display_కిటికీ_కొలత (bool fancy, అక్షరం స్థిరరాశి *device_name);
స్థిర శూన్య sane_బహుళకం (నిర్మాణం termios *బహుళకం_2);
స్థిర శూన్య సమితి_control_char (నిర్మాణం control_info స్థిరరాశి *info,
                              అక్షరం స్థిరరాశి *arg,
                              నిర్మాణం termios *బహుళకం_2);
స్థిర శూన్య సమితి_వేగం (గణన వేగం_setting type, అక్షరం స్థిరరాశి *arg,
                       నిర్మాణం వేగం_desc స్థిరరాశి వేగం, నిర్మాణం termios *బహుళకం_2);
స్థిర శూన్య సమితి_కిటికీ_కొలత (పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య_2, అక్షరం స్థిరరాశి *device_name);

/* The width of the screen, for output wrapping. */
స్థిర పూర్ణాంకం max_నిలువు;

/* Current position, to know when to wrap. */
స్థిర పూర్ణాంకం current_నిలువు;

/* Default "drain" mode for tcsetattr.  */
స్థిర పూర్ణాంకం tcsetattr_options = TCSADRAIN;

/* Extra info to aid stty development.  */
స్థిర bool dev_debug;

/* Record last speed set for correlation.  */
స్థిర నిర్మాణం వేగం_desc చివరి_ibaud =
  { .baud = (వేగం_t) -1, .valid = false };
స్థిర నిర్మాణం వేగం_desc చివరి_obaud =
  { .baud = (వేగం_t) -1, .valid = false };

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1,
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"all", no_వాదం, ఖాళీ, 'a'},
  {"save", no_వాదం, ఖాళీ, 'g'},
  {"file", required_వాదం, ఖాళీ, 'F'},
  {"-debug", no_వాదం, ఖాళీ, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Print format string MESSAGE and optional args.
   Wrap to next line first if it won't fit.
   Print a space first unless MESSAGE will start a new line. */

ATTRIBUTE_FORMAT ((రాయి, 1, 2))
స్థిర శూన్య
wrapf (అక్షరం స్థిరరాశి *సందేశం_2,...)
{
  va_list వాదనలు;
  అక్షరం *buf;
  పూర్ణాంకం buflen;

  va_ప్రారంభం (వాదనలు, సందేశం_2);
  buflen = vasprintf (&buf, సందేశం_2, వాదనలు);
  va_చివర (వాదనలు);

  ఒకవేళ (buflen < 0)
    xalloc_die ();

  ఒకవేళ (0 < current_నిలువు)
    {
      ఒకవేళ (max_నిలువు - current_నిలువు <= buflen)
        {
          అక్షరం_రాయి ('\n');
          current_నిలువు = 0;
        }
      లేకపోతే
        {
          అక్షరం_రాయి (' ');
          current_నిలువు++;
        }
    }

  fputs (buf, stdout);
  విడుదల (buf);
  current_నిలువు += buflen;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [-F DEVICE | --file=DEVICE] [SETTING]...\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-a|--all]\n\
  or:  %s [-F DEVICE | --file=DEVICE] [-g|--save]\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Print or change terminal characteristics.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all          print all current settings లోపల human-readable form\n\
"));
      oputs (_("\
  -g, --save         print all current settings లోపల a stty-readable form\n\
"));
      oputs (_("\
  -F, --file=DEVICE  open and use DEVICE instead యొక్క standard ఆదానం\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Optional - before SETTING indicates negation.  An * marks non-POSIX\n\
settings.  The underlying system నిర్వచిస్తుంది which settings are అందుబాటు.\n\
"), stdout);
      fputs (_("\
\n\
Special characters:\n"), stdout);
#నిర్వచితమైతే VFLUSHO
      fputs (_("\
 * discard CHAR  CHAR will toggle discarding యొక్క output\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే VDSUSP
      fputs (_("\
 * dsusp CHAR    CHAR will send a terminal stop సంకేతం once ఆదానం flushed\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   eof CHAR      CHAR will send an చివర యొక్క file (terminate the ఆదానం)\n\
   eol CHAR      CHAR will చివర the పంక్తి\n\
"), stdout);
#నిర్వచితమైతే VEOL2
      fputs (_("\
 * eol2 CHAR     alternate CHAR కొరకు ending the పంక్తి\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   erase CHAR    CHAR will erase the చివరి character typed\n\
   intr CHAR     CHAR will send an అంతరాయం సంకేతం\n\
   kill CHAR     CHAR will erase the current పంక్తి\n\
"), stdout);
#నిర్వచితమైతే VLNEXT
      fputs (_("\
 * lnext CHAR    CHAR will enter the తదుపరి character quoted\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే VSTATUS
      fputs (_("\
 * స్థితి_2 CHAR   CHAR will send an info సంకేతం\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   quit CHAR     CHAR will send a quit సంకేతం\n\
"), stdout);
#ఒకవేళ defined CREPRINT || defined VREPRINT
      fputs (_("\
 * rprnt CHAR    CHAR will redraw the current పంక్తి\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   ప్రారంభం CHAR    CHAR will restart the output after stopping it\n\
   stop CHAR     CHAR will stop the output\n\
   susp CHAR     CHAR will send a terminal stop సంకేతం\n\
"), stdout);
#నిర్వచితమైతే VSWTCH
      fputs (_("\
 * swtch CHAR    CHAR will ఎంపిక గమ్యం a different shell layer\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే VWERASE
      fputs (_("\
 * werase CHAR   CHAR will erase the చివరి మాట_2 typed\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
\n\
Special settings:\n\
   N             సమితి_2 the ఆదానం and output speeds గమ్యం N bauds\n\
"), stdout);
#నిర్వచితమైతే TIOCGWINSZ
      fputs (_("\
   నిలువుల_సంఖ్య_2 N        tell the kernel that the terminal has N columns\n\
 * columns N     same as నిలువుల_సంఖ్య_2 N\n\
"), stdout);
#అయితే_ముగింపు
      రాయి (_("\
 * [-]drain      wait కొరకు సంప్రేషణ before applying settings (%s by అప్రమేయ)\
\n"), tcsetattr_options == TCSADRAIN ? _("on") : _("off"));
      fputs (_("\
   ispeed N      సమితి_2 the ఆదానం వేగం గమ్యం N\n\
"), stdout);
#నిర్వచితమైతే HAVE_C_పంక్తి
      fputs (_("\
 * పంక్తి N        use పంక్తి discipline N\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   min N         with -icanon, సమితి_2 N characters minimum కొరకు a completed పఠ\n\
   ospeed N      సమితి_2 the output వేగం గమ్యం N\n\
"), stdout);
#నిర్వచితమైతే TIOCGWINSZ
      fputs (_("\
   అడ్డువరుసలు N        tell the kernel that the terminal has N అడ్డువరుసలు\n\
   కొలత          print the సంఖ్య యొక్క అడ్డువరుసలు and columns according గమ్యం the kernel\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   వేగం         print the terminal వేగం\n\
   సమయం N        with -icanon, సమితి_2 పఠ timeout యొక్క N tenths యొక్క a రెండవ\n\
"), stdout);
      fputs (_("\
\n\
Control settings:\n\
   [-]clocal     disable modem control సంకేతాలు\n\
   [-]cread      allow ఆదానం గమ్యం be received\n\
"), stdout);
#నిర్వచితమైతే CRTSCTS
      fputs (_("\
 * [-]crtscts    enable RTS/CTS handshaking\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే CDTRDSR
      fputs (_("\
 * [-]cdtrdsr    enable DTR/DSR handshaking\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   csN           సమితి_2 character కొలత గమ్యం N బిట్లు_2, N లోపల [5..8]\n\
"), stdout);
      fputs (_("\
   [-]cstopb     use two stop బిట్లు_2 చొప్పున character (one with '-')\n\
   [-]hup        send a hangup సంకేతం when the చివరి process closes the tty\n\
   [-]hupcl      same as [-]hup\n\
   [-]parenb     generate parity బిట్టు లోపల output and expect parity బిట్టు లోపల ఆదానం\n\
   [-]parodd     సమితి_2 odd parity (or even parity with '-')\n\
"), stdout);
#నిర్వచితమైతే CMSPAR
      fputs (_("\
 * [-]cmspar     use \"stick\" (mark/space) parity\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
\n\
ఆదానం_3 settings:\n\
   [-]brkint     breaks cause an అంతరాయం సంకేతం\n\
   [-]icrnl      translate carriage తిరిగి గమ్యం newline\n\
   [-]ignbrk     ignore ఆపు characters\n\
   [-]igncr      ignore carriage తిరిగి\n\
   [-]ignpar     ignore characters with parity errors\n\
"), stdout);
#నిర్వచితమైతే IMAXBEL
      fputs (_("\
 * [-]imaxbel    beep and చేయి not flush a నిండిన ఆదానం బఫరు on a character\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   [-]inlcr      translate newline గమ్యం carriage తిరిగి\n\
   [-]inpck      enable ఆదానం parity checking\n\
   [-]istrip     clear ఎగువ (8th) బిట్టు యొక్క ఆదానం characters\n\
"), stdout);
#నిర్వచితమైతే IUTF8
      fputs (_("\
 * [-]iutf8      assume ఆదానం characters are UTF-8 encoded\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే IUCLC
      fputs (_("\
 * [-]iuclc      translate uppercase characters గమ్యం lowercase\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే IXANY
      fputs (_("\
 * [-]ixany      let any character restart output, not only ప్రారంభం character\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   [-]ixoff      enable sending యొక్క ప్రారంభం/stop characters\n\
   [-]ixon       enable XON/XOFF flow control\n\
   [-]parmrk     mark parity errors (with a 255-0-character sequence)\n\
   [-]tandem     same as [-]ixoff\n\
"), stdout);
      fputs (_("\
\n\
Output settings:\n\
"), stdout);
#నిర్వచితమైతే BSDLY
      fputs (_("\
 * bsN           backspace delay style, N లోపల [0..1]\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే CRDLY
      fputs (_("\
 * crN           carriage తిరిగి delay style, N లోపల [0..3]\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే FFDLY
      fputs (_("\
 * ffN           form feed delay style, N లోపల [0..1]\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే NLDLY
      fputs (_("\
 * nlN           newline delay style, N లోపల [0..1]\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే OCRNL
      fputs (_("\
 * [-]ocrnl      translate carriage తిరిగి గమ్యం newline\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే OFDEL
      fputs (_("\
 * [-]ofdel      use delete characters కొరకు fill instead యొక్క NUL characters\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే OFILL
      fputs (_("\
 * [-]ofill      use fill (padding) characters instead యొక్క timing కొరకు delays\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే OLCUC
      fputs (_("\
 * [-]olcuc      translate lowercase characters గమ్యం uppercase\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే ONLCR
      fputs (_("\
 * [-]onlcr      translate newline గమ్యం carriage తిరిగి-newline\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే ONLRET
      fputs (_("\
 * [-]onlret     newline performs a carriage తిరిగి\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే ONOCR
      fputs (_("\
 * [-]onocr      చేయి not print carriage returns లోపల the మొదటి స్తంభం\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   [-]opost      postprocess output\n\
"), stdout);
#ఒకవేళ defined TABDLY || defined OXTABS
      fputs (_("\
 * tabN          horizontal tab delay style, N లోపల [0..3]\n\
 * tabs          same as tab0\n\
 * -tabs         same as tab3\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే VTDLY
      fputs (_("\
 * vtN           vertical tab delay style, N లోపల [0..1]\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
\n\
Local settings:\n\
   [-]crterase   echo erase characters as backspace-space-backspace\n\
"), stdout);
#నిర్వచితమైతే ECHOKE
      fputs (_("\
 * crtkill       kill all పంక్తి by obeying the echoprt and echoe settings\n\
 * -crtkill      kill all పంక్తి by obeying the echoctl and echok settings\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOCTL
      fputs (_("\
 * [-]ctlecho    echo control characters లోపల hat notation ('^c')\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   [-]echo       echo ఆదానం characters\n\
"), stdout);
#నిర్వచితమైతే ECHOCTL
      fputs (_("\
 * [-]echoctl    same as [-]ctlecho\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   [-]echoe      same as [-]crterase\n\
   [-]echok      echo a newline after a kill character\n\
"), stdout);
#నిర్వచితమైతే ECHOKE
      fputs (_("\
 * [-]echoke     same as [-]crtkill\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   [-]echonl     echo newline even ఒకవేళ not echoing other characters\n\
"), stdout);
#నిర్వచితమైతే ECHOPRT
      fputs (_("\
 * [-]echoprt    echo erased characters backward, between '\\' and '/'\n\
"), stdout);
#అయితే_ముగింపు
#ఒకవేళ defined EXTPROC || defined TIOCEXT
      fputs (_("\
 * [-]extproc    enable \"LINEMODE\"; useful with ఎగువ latency links\n\
"), stdout);
#అయితే_ముగింపు
#ఒకవేళ defined FLUSHO
      fputs (_("\
 * [-]flusho     discard output\n\
"), stdout);
#అయితే_ముగింపు
      రాయి (_("\
   [-]icanon     enable special characters: %s\n\
   [-]iexten     enable non-POSIX special characters\n\
"), "erase, kill"
#నిర్వచితమైతే VWERASE
    ", werase"
#అయితే_ముగింపు
#ఒకవేళ defined CREPRINT || defined VREPRINT
    ", rprnt"
#అయితే_ముగింపు
);
      fputs (_("\
   [-]isig       enable అంతరాయం, quit, and suspend special characters\n\
   [-]noflsh     disable flushing after అంతరాయం and quit special characters\n\
"), stdout);
#నిర్వచితమైతే ECHOPRT
      fputs (_("\
 * [-]prterase   same as [-]echoprt\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే TOSTOP
      fputs (_("\
 * [-]tostop     stop background jobs that try గమ్యం లిఖ గమ్యం the terminal\n\
"), stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే XCASE
      fputs (_("\
 * [-]xcase      with icanon, escape with '\\' కొరకు uppercase characters\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
\n\
Combination settings:\n\
"), stdout);
#ఒకవేళ defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]LCASE      same as [-]lcase\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   cbreak        same as -icanon\n\
   -cbreak       same as icanon\n\
"), stdout);
      fputs (_("\
   cooked        same as brkint ignpar istrip icrnl ixon opost isig\n\
                 icanon, eof and eol characters గమ్యం their అప్రమేయ విలువలు\n\
   -cooked       same as raw\n\
"), stdout);
      రాయి (_("\
   crt           same as %s\n\
"), "echoe"
#నిర్వచితమైతే ECHOCTL
    " echoctl"
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOKE
    " echoke"
#అయితే_ముగింపు
);
      రాయి (_("\
   dec           same as %s intr ^c erase 0177\n\
                 kill ^u\n\
"), "echoe"
#నిర్వచితమైతే ECHOCTL
    " echoctl"
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOKE
    " echoke"
#అయితే_ముగింపు
#నిర్వచితమైతే IXANY
    " -ixany"
#అయితే_ముగింపు
);
#నిర్వచితమైతే IXANY
      fputs (_("\
 * [-]decctlq    same as [-]ixany\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   ek            erase and kill characters గమ్యం their అప్రమేయ విలువలు\n\
   evenp         same as parenb -parodd cs7\n\
   -evenp        same as -parenb cs8\n\
"), stdout);
#ఒకవేళ defined XCASE && defined IUCLC && defined OLCUC
      fputs (_("\
 * [-]lcase      same as xcase iuclc olcuc\n\
"), stdout);
#అయితే_ముగింపు
      fputs (_("\
   litout        same as -parenb -istrip -opost cs8\n\
   -litout       same as parenb istrip opost cs7\n\
"), stdout);
      రాయి (_("\
   nl            same as %s\n\
   -nl           same as %s\n\
"), "-icrnl"
#నిర్వచితమైతే ONLCR
   " -onlcr"
#అయితే_ముగింపు
  , "icrnl -inlcr -igncr"
#నిర్వచితమైతే ONLCR
   " onlcr"
#అయితే_ముగింపు
#నిర్వచితమైతే OCRNL
   " -ocrnl"
#అయితే_ముగింపు
#నిర్వచితమైతే ONLRET
   " -onlret"
#అయితే_ముగింపు
);
      fputs (_("\
   oddp          same as parenb parodd cs7\n\
   -oddp         same as -parenb cs8\n\
   [-]parity     same as [-]evenp\n\
   pass8         same as -parenb -istrip cs8\n\
   -pass8        same as parenb istrip cs7\n\
"), stdout);
      రాయి (_("\
   raw           same as -ignbrk -brkint -ignpar -parmrk -inpck -istrip\n\
                 -inlcr -igncr -icrnl -ixon -ixoff -icanon -opost\n\
                 -isig%s min 1 సమయం 0\n\
   -raw          same as cooked\n\
"),
#నిర్వచితమైతే IUCLC
   " -iuclc"
#అయితే_ముగింపు
#నిర్వచితమైతే IXANY
   " -ixany"
#అయితే_ముగింపు
#నిర్వచితమైతే IMAXBEL
   " -imaxbel"
#అయితే_ముగింపు
#నిర్వచితమైతే XCASE
   " -xcase"
#అయితే_ముగింపు
);
      రాయి (_("\
   sane          same as cread -ignbrk brkint -inlcr -igncr icrnl\n\
                 icanon iexten echo echoe echok -echonl -noflsh\n\
                 %s\n\
                 %s\n\
                 %s,\n\
                 all special characters గమ్యం their అప్రమేయ విలువలు\n\
"),
   "-ixoff"
#నిర్వచితమైతే IUTF8
   " -iutf8"
#అయితే_ముగింపు
#నిర్వచితమైతే IUCLC
   " -iuclc"
#అయితే_ముగింపు
#నిర్వచితమైతే IXANY
   " -ixany"
#అయితే_ముగింపు
#నిర్వచితమైతే IMAXBEL
   " imaxbel"
#అయితే_ముగింపు
#నిర్వచితమైతే XCASE
   " -xcase"
#అయితే_ముగింపు
#నిర్వచితమైతే OLCUC
   " -olcuc"
#అయితే_ముగింపు
#నిర్వచితమైతే OCRNL
   " -ocrnl"
#అయితే_ముగింపు

 , "opost"
#నిర్వచితమైతే OFILL
   " -ofill"
#అయితే_ముగింపు
#నిర్వచితమైతే ONLCR
   " onlcr"
#అయితే_ముగింపు
#నిర్వచితమైతే ONOCR
   " -onocr"
#అయితే_ముగింపు
#నిర్వచితమైతే ONLRET
   " -onlret"
#అయితే_ముగింపు
#నిర్వచితమైతే NLDLY
   " nl0"
#అయితే_ముగింపు
#నిర్వచితమైతే CRDLY
   " cr0"
#అయితే_ముగింపు
#నిర్వచితమైతే TAB0
   " tab0"
#అయితే_ముగింపు
#నిర్వచితమైతే BSDLY
   " bs0"
#అయితే_ముగింపు
#నిర్వచితమైతే VTDLY
   " vt0"
#అయితే_ముగింపు
#నిర్వచితమైతే FFDLY
   " ff0"
#అయితే_ముగింపు

 , "isig"
#నిర్వచితమైతే TOSTOP
   " -tostop"
#అయితే_ముగింపు
#నిర్వచితమైతే OFDEL
   " -ofdel"
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOPRT
   " -echoprt"
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOCTL
   " echoctl"
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOKE
   " echoke"
#అయితే_ముగింపు
#నిర్వచితమైతే EXTPROC
   " -extproc"
#అయితే_ముగింపు
#నిర్వచితమైతే FLUSHO
   " -flusho"
#అయితే_ముగింపు
);
      fputs (_("\
\n\
Handle the tty పంక్తి connected గమ్యం standard ఆదానం.  Without arguments,\n\
prints baud రేటు, పంక్తి discipline, and deviations from stty sane.  లోపల_2\n\
settings, CHAR is తీసుకున్న literally, or coded as లోపల ^c, 0x37, 0177 or\n\
127; special విలువలు ^- or undef వాడిన గమ్యం disable special characters.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}


/* Apply specified settings to MODE and REQUIRE_SET_ATTR as required.
   If CHECKING is true, this function doesn't interact
   with a device, and only validates specified settings.  */

స్థిర శూన్య
apply_settings (bool checking, అక్షరం స్థిరరాశి *device_name,
                అక్షరం * స్థిరరాశి *settings, పూర్ణాంకం n_settings,
                నిర్మాణం termios *బహుళకం_2, bool *require_సమితి_attr)
{
#నిర్వచించు check_వాదం(arg)						\
  చేయి									\
    {									\
      ఒకవేళ (k == n_settings - 1 || ! settings[k + 1])			\
        {								\
          error (0, 0, _("missing argument to %s"), quote (arg));	\
          usage (EXIT_FAILURE);						\
        }								\
    }									\
  అయితే (0)

  కొరకు (పూర్ణాంకం k = 1; k < n_settings; k++)
    {
      అక్షరం స్థిరరాశి *arg = settings[k];
      bool సరిపోలిక_దొరికింది = false;
      bool not_సమితి_attr = false;
      bool reversed = false;

      ఒకవేళ (! arg)
        కొనసాగు;

      ఒకవేళ (arg[0] == '-')
        {
          ++arg;
          reversed = true;
        }
      ఒకవేళ (streq (arg, "drain"))
        {
          tcsetattr_options = reversed ? TCSANOW : TCSADRAIN;
          కొనసాగు;
        }
      కొరకు (పూర్ణాంకం i = 0; బహుళకం_info[i].name != ఖాళీ; ++i)
        {
          ఒకవేళ (streq (arg, బహుళకం_info[i].name))
            {
              ఒకవేళ ((బహుళకం_info[i].flags & NO_SETATTR) == 0)
                {
                  సరిపోలిక_దొరికింది = సమితి_బహుళకం (&బహుళకం_info[i], reversed, బహుళకం_2);
                  *require_సమితి_attr = true;
                }
              లేకపోతే
                సరిపోలిక_దొరికింది = not_సమితి_attr = true;
              ఆపు;
            }
        }
      ఒకవేళ (!సరిపోలిక_దొరికింది && reversed)
        {
          error (0, 0, _("invalid argument %s"), quote (arg - 1));
          usage (EXIT_FAILURE);
        }
      ఒకవేళ (!సరిపోలిక_దొరికింది)
        {
          కొరకు (పూర్ణాంకం i = 0; control_info[i].name != ఖాళీ; ++i)
            {
              ఒకవేళ (streq (arg, control_info[i].name))
                {
                  check_వాదం (arg);
                  సరిపోలిక_దొరికింది = true;
                  ++k;
                  సమితి_control_char (&control_info[i], settings[k], బహుళకం_2);
                  *require_సమితి_attr = true;
                  ఆపు;
                }
            }
        }
      ఒకవేళ (!సరిపోలిక_దొరికింది || not_సమితి_attr)
        {
          ఒకవేళ (streq (arg, "ispeed"))
            {
              check_వాదం (arg);
              ++k;
              నిర్మాణం వేగం_desc స్థిరరాశి ispeed = string_గమ్యం_baud (settings[k]);
              ఒకవేళ (! ispeed.valid)
                {
                  error (0, 0, _("invalid ispeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              సమితి_వేగం (ఆదానం_వేగం, settings[k], ispeed, బహుళకం_2);
              ఒకవేళ (checking)
                కొనసాగు;
              *require_సమితి_attr = true;
            }
          లేకపోతే ఒకవేళ (streq (arg, "ospeed"))
            {
              check_వాదం (arg);
              ++k;
              నిర్మాణం వేగం_desc స్థిరరాశి ospeed = string_గమ్యం_baud (settings[k]);
              ఒకవేళ (! ospeed.valid)
                {
                  error (0, 0, _("invalid ospeed %s"), quote (settings[k]));
                  usage (EXIT_FAILURE);
                }
              సమితి_వేగం (output_వేగం, settings[k], ospeed, బహుళకం_2);
              ఒకవేళ (checking)
                కొనసాగు;
              *require_సమితి_attr = true;
            }
#నిర్వచితమైతే TIOCEXT
          /* This is the BSD interface to "extproc".
            Even though it's an lflag, an ioctl is used to set it.  */
          లేకపోతే ఒకవేళ (streq (arg, "extproc"))
            {
              పూర్ణాంకం val = ! reversed;

              ఒకవేళ (checking)
                కొనసాగు;

              ఒకవేళ (ioctl (STDIN_FILENO, TIOCEXT, &val) != 0)
                error (EXIT_FAILURE, errno, _("%s: error setting %s"),
                       quotef_n (0, device_name), quote_n (1, arg));
            }
#అయితే_ముగింపు
#నిర్వచితమైతే TIOCGWINSZ
          లేకపోతే ఒకవేళ (streq (arg, "rows"))
            {
              check_వాదం (arg);
              ++k;
              ఒకవేళ (checking)
                కొనసాగు;
              సమితి_కిటికీ_కొలత (integer_arg (settings[k], గరిష్ఠ_పూర్ణాంకం), -1,
                               device_name);
            }
          లేకపోతే ఒకవేళ (streq (arg, "cols")
                   || streq (arg, "columns"))
            {
              check_వాదం (arg);
              ++k;
              ఒకవేళ (checking)
                కొనసాగు;
              సమితి_కిటికీ_కొలత (-1, integer_arg (settings[k], గరిష్ఠ_పూర్ణాంకం),
                               device_name);
            }
          లేకపోతే ఒకవేళ (streq (arg, "size"))
            {
              ఒకవేళ (checking)
                కొనసాగు;
              max_నిలువు = screen_columns ();
              current_నిలువు = 0;
              display_కిటికీ_కొలత (false, device_name);
            }
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_C_పంక్తి
          లేకపోతే ఒకవేళ (streq (arg, "line"))
            {
              check_వాదం (arg);
              ++k;
              uintmax_t విలువ = integer_arg (settings[k], UINTMAX_MAX);
              ఒకవేళ (ckd_కూడు (&బహుళకం_2->c_పంక్తి, విలువ, 0))
                error (0, EOVERFLOW, _("invalid line discipline %s"),
                       quote (settings[k]));
              *require_సమితి_attr = true;
            }
#అయితే_ముగింపు
          లేకపోతే ఒకవేళ (streq (arg, "speed"))
            {
              ఒకవేళ (checking)
                కొనసాగు;
              max_నిలువు = screen_columns ();
              display_వేగం (బహుళకం_2, false);
            }
          లేకపోతే
            {
              నిర్మాణం వేగం_desc స్థిరరాశి వేగం = string_గమ్యం_baud (arg);
              ఒకవేళ (వేగం.valid)
                {
                  సమితి_వేగం (both_speeds, arg, వేగం, బహుళకం_2);
                  ఒకవేళ (checking)
                    కొనసాగు;
                  *require_సమితి_attr = true;
                }
              లేకపోతే
                {
                  ఒకవేళ (! recover_బహుళకం (arg, బహుళకం_2))
                    {
                      error (0, 0, _("invalid argument %s"), quote (arg));
                      usage (EXIT_FAILURE);
                    }
                  *require_సమితి_attr = true;
                }
            }
        }
    }

  ఒకవేళ (checking)
    check_వేగం (బహుళకం_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  /* Initialize to all zeroes so there is no risk memeq will report a
     spurious difference in an uninitialized portion of the structure.  */
  స్థిర నిర్మాణం termios బహుళకం_2;

  గణన output_type output_type;
  పూర్ణాంకం optc;
  పూర్ణాంకం argi = 0;
  పూర్ణాంకం opti = 1;
  bool require_సమితి_attr;
  bool verbose_output;
  bool recoverable_output;
  bool noargs = true;
  అక్షరం *file_name = ఖాళీ;
  అక్షరం స్థిరరాశి *device_name;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  output_type = మారింది;
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

  అయితే ((optc = getopt_long (argc - argi, argv + argi, "-agF:",
                              longopts, ఖాళీ))
         != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'a':
          verbose_output = true;
          output_type = all;
          ఆపు;

        స్థితి 'g':
          recoverable_output = true;
          output_type = recoverable;
          ఆపు;

        స్థితి 'F':
          ఒకవేళ (file_name)
            error (EXIT_FAILURE, 0, _("only one device may be specified"));
          file_name = optarg;
          ఆపు;

        స్థితి DEV_DEBUG_OPTION:
          dev_debug = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          /* Consider "drain" as an option rather than a setting,
             to support: alias stty='stty -drain'  etc.  */
          ఒకవేళ (! streq (argv[argi + opti], "-drain")
              && ! streq (argv[argi + opti], "drain"))
            noargs = false;

          /* Skip the argument containing this unrecognized option;
             the 2nd pass will analyze it.  */
          argi += opti;

          /* Restart getopt_long from the first unskipped argument.  */
          opti = 1;
          optind = 0;

          ఆపు;
        }

      /* Clear fully-parsed arguments, so they don't confuse the 2nd pass.  */
      అయితే (opti < optind)
        argv[argi + opti++] = ఖాళీ;
    }

  /* Specifying both -a and -g gets an error.  */
  ఒకవేళ (verbose_output && recoverable_output)
    error (EXIT_FAILURE, 0,
           _("the options for verbose and stty-readable output styles are\n"
             "mutually exclusive"));

  /* Specifying any other arguments with -a or -g gets an error.  */
  ఒకవేళ (!noargs && (verbose_output || recoverable_output))
    error (EXIT_FAILURE, 0,
           _("when specifying an output style, modes may not be set"));

  device_name = file_name ? file_name : _("standard input");

  ఒకవేళ (!noargs && !verbose_output && !recoverable_output)
    {
      స్థిర నిర్మాణం termios check_బహుళకం;
      apply_settings (/* checking= */ true, device_name, argv, argc,
                      &check_బహుళకం, &require_సమితి_attr);
    }

  ఒకవేళ (file_name)
    {
      పూర్ణాంకం fdflags;
      ఒకవేళ (fd_reopen (STDIN_FILENO, device_name, O_RDONLY | O_NONBLOCK, 0) < 0)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ఒకవేళ ((fdflags = fcntl (STDIN_FILENO, F_GETFL)) == -1
          || fcntl (STDIN_FILENO, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
        error (EXIT_FAILURE, errno, _("%s: couldn't reset non-blocking mode"),
               quotef (device_name));
    }

  ఒకవేళ (tcgetattr (STDIN_FILENO, &బహుళకం_2))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));

  ఒకవేళ (verbose_output || recoverable_output || noargs)
    {
      max_నిలువు = screen_columns ();
      current_నిలువు = 0;
      display_settings (output_type, &బహుళకం_2, device_name);
      తిరిగి EXIT_SUCCESS;
    }

  require_సమితి_attr = false;
  apply_settings (/* checking= */ false, device_name, argv, argc,
                  &బహుళకం_2, &require_సమితి_attr);

  ఒకవేళ (require_సమితి_attr)
    {
      /* Initialize to all zeroes so there is no risk memeq will report a
         spurious difference in an uninitialized portion of the structure.  */
      స్థిర నిర్మాణం termios new_బహుళకం;

      ఒకవేళ (tcsetattr (STDIN_FILENO, tcsetattr_options, &బహుళకం_2))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report 'success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones.  */

      ఒకవేళ (tcgetattr (STDIN_FILENO, &new_బహుళకం))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ఒకవేళ (! eq_బహుళకం (&బహుళకం_2, &new_బహుళకం))
        {
          ఒకవేళ (dev_debug)
            {
              error (0, 0, _("indx: mode: actual mode"));
              కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < పరిమాణం (new_బహుళకం); i++)
                {
                  గుర్తులేని పూర్ణాంకం newc = *(((గుర్తులేని అక్షరం *) &new_బహుళకం) + i);
                  గుర్తులేని పూర్ణాంకం oldc = *(((గుర్తులేని అక్షరం *) &బహుళకం_2) + i);
                  error (0, 0, "0x%02x, 0x%02x: 0x%02x%s", i, oldc, newc,
                          newc == oldc ? "" : " *");
                }
            }

          error (EXIT_FAILURE, 0,
                 _("%s: unable to perform all requested operations"),
                 quotef (device_name));
        }
    }

  తిరిగి EXIT_SUCCESS;
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
స్థిర bool
eq_బహుళకం (నిర్మాణం termios స్థిరరాశి *mode1, నిర్మాణం termios స్థిరరాశి *mode2)
{
  ఒకవేళ (mode1->c_iflag != mode2->c_iflag
      || mode1->c_oflag != mode2->c_oflag
      || mode1->c_lflag != mode2->c_lflag
#నిర్వచితమైతే HAVE_C_పంక్తి
      || mode1->c_పంక్తి != mode2->c_పంక్తి
#అయితే_ముగింపు
      || !memeq (mode1->c_cc, mode2->c_cc, పరిమాణం (mode1->c_cc)))
    తిరిగి false;

  /* Strip known baud-related flags from c_cflag before comparison.  */
  tcflag_t cflag_ముసుగు = ~(tcflag_t) 0;
#నిర్వచితమైతే CBAUD
  cflag_ముసుగు &= ~(tcflag_t) CBAUD;
#అయితే_ముగింపు
#నిర్వచితమైతే CIBAUD
  cflag_ముసుగు &= ~(tcflag_t) CIBAUD;
#అయితే_ముగింపు

  ఒకవేళ ((mode1->c_cflag ^ mode2->c_cflag) & cflag_ముసుగు)
    తిరిగి false;

  తిరిగి వేగం_matches (mode1, mode2);
}

/* Return false if not applied because not reversible; otherwise
   return true.  */

స్థిర bool
సమితి_బహుళకం (నిర్మాణం బహుళకం_info స్థిరరాశి *info, bool reversed, నిర్మాణం termios *బహుళకం_2)
{
  tcflag_t *bitsp;

  ఒకవేళ (reversed && (info->flags & REV) == 0)
    తిరిగి false;

  bitsp = బహుళకం_type_flag (info->type, బహుళకం_2);

  ఒకవేళ (bitsp == ఖాళీ)
    {
      /* Combination mode. */
      ఒకవేళ (streq (info->name, "evenp") || streq (info->name, "parity"))
        {
          ఒకవేళ (reversed)
            బహుళకం_2->c_cflag = (బహుళకం_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          లేకపోతే
            బహుళకం_2->c_cflag = (బహుళకం_2->c_cflag & ~PARODD & ~CSIZE) | PARENB | CS7;
        }
      లేకపోతే ఒకవేళ (streq (info->name, "oddp"))
        {
          ఒకవేళ (reversed)
            బహుళకం_2->c_cflag = (బహుళకం_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
          లేకపోతే
            బహుళకం_2->c_cflag = (బహుళకం_2->c_cflag & ~CSIZE) | CS7 | PARODD | PARENB;
        }
      లేకపోతే ఒకవేళ (streq (info->name, "nl"))
        {
          ఒకవేళ (reversed)
            {
              బహుళకం_2->c_iflag = (బహుళకం_2->c_iflag | ICRNL) & ~INLCR & ~IGNCR;
              బహుళకం_2->c_oflag = (బహుళకం_2->c_oflag
#నిర్వచితమైతే ONLCR
                               | ONLCR
#అయితే_ముగింపు
                )
#నిర్వచితమైతే OCRNL
                & ~OCRNL
#అయితే_ముగింపు
#నిర్వచితమైతే ONLRET
                & ~ONLRET
#అయితే_ముగింపు
                ;
            }
          లేకపోతే
            {
              బహుళకం_2->c_iflag = బహుళకం_2->c_iflag & ~ICRNL;
#నిర్వచితమైతే ONLCR
              బహుళకం_2->c_oflag = బహుళకం_2->c_oflag & ~ONLCR;
#అయితే_ముగింపు
            }
        }
      లేకపోతే ఒకవేళ (streq (info->name, "ek"))
        {
          బహుళకం_2->c_cc[VERASE] = CERASE;
          బహుళకం_2->c_cc[VKILL] = CKILL;
        }
      లేకపోతే ఒకవేళ (streq (info->name, "sane"))
        sane_బహుళకం (బహుళకం_2);
      లేకపోతే ఒకవేళ (streq (info->name, "cbreak"))
        {
          ఒకవేళ (reversed)
            బహుళకం_2->c_lflag |= ICANON;
          లేకపోతే
            బహుళకం_2->c_lflag &= ~ICANON;
        }
      లేకపోతే ఒకవేళ (streq (info->name, "pass8"))
        {
          ఒకవేళ (reversed)
            {
              బహుళకం_2->c_cflag = (బహుళకం_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              బహుళకం_2->c_iflag |= ISTRIP;
            }
          లేకపోతే
            {
              బహుళకం_2->c_cflag = (బహుళకం_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              బహుళకం_2->c_iflag &= ~ISTRIP;
            }
        }
      లేకపోతే ఒకవేళ (streq (info->name, "litout"))
        {
          ఒకవేళ (reversed)
            {
              బహుళకం_2->c_cflag = (బహుళకం_2->c_cflag & ~CSIZE) | CS7 | PARENB;
              బహుళకం_2->c_iflag |= ISTRIP;
              బహుళకం_2->c_oflag |= OPOST;
            }
          లేకపోతే
            {
              బహుళకం_2->c_cflag = (బహుళకం_2->c_cflag & ~PARENB & ~CSIZE) | CS8;
              బహుళకం_2->c_iflag &= ~ISTRIP;
              బహుళకం_2->c_oflag &= ~OPOST;
            }
        }
      లేకపోతే ఒకవేళ (streq (info->name, "raw") || streq (info->name, "cooked"))
        {
          ఒకవేళ ((info->name[0] == 'r' && reversed)
              || (info->name[0] == 'c' && !reversed))
            {
              /* Cooked mode. */
              బహుళకం_2->c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
              బహుళకం_2->c_oflag |= OPOST;
              బహుళకం_2->c_lflag |= ISIG | ICANON;
#ఒకవేళ VMIN == VEOF
              బహుళకం_2->c_cc[VEOF] = CEOF;
#అయితే_ముగింపు
#ఒకవేళ VTIME == VEOL
              బహుళకం_2->c_cc[VEOL] = CEOL;
#అయితే_ముగింపు
            }
          లేకపోతే
            {
              /* Raw mode. */
              బహుళకం_2->c_iflag = 0;
              బహుళకం_2->c_oflag &= ~OPOST;
              బహుళకం_2->c_lflag &= ~(ISIG | ICANON
#నిర్వచితమైతే XCASE
                                 | XCASE
#అయితే_ముగింపు
                );
              బహుళకం_2->c_cc[VMIN] = 1;
              బహుళకం_2->c_cc[VTIME] = 0;
            }
        }
#నిర్వచితమైతే IXANY
      లేకపోతే ఒకవేళ (streq (info->name, "decctlq"))
        {
          ఒకవేళ (reversed)
            బహుళకం_2->c_iflag |= IXANY;
          లేకపోతే
            బహుళకం_2->c_iflag &= ~IXANY;
        }
#అయితే_ముగింపు
#నిర్వచితమైతే TABDLY
      లేకపోతే ఒకవేళ (streq (info->name, "tabs"))
        {
          ఒకవేళ (reversed)
            బహుళకం_2->c_oflag = (బహుళకం_2->c_oflag & ~TABDLY) | TAB3;
          లేకపోతే
            బహుళకం_2->c_oflag = (బహుళకం_2->c_oflag & ~TABDLY) | TAB0;
        }
#లేకపోతే
# ifdef OXTABS
      లేకపోతే ఒకవేళ (streq (info->name, "tabs"))
        {
          ఒకవేళ (reversed)
            బహుళకం_2->c_oflag = బహుళకం_2->c_oflag | OXTABS;
          లేకపోతే
            బహుళకం_2->c_oflag = బహుళకం_2->c_oflag & ~OXTABS;
        }
# endif
#అయితే_ముగింపు
#ఒకవేళ defined XCASE && defined IUCLC && defined OLCUC
      లేకపోతే ఒకవేళ (streq (info->name, "lcase")
               || streq (info->name, "LCASE"))
        {
          ఒకవేళ (reversed)
            {
              బహుళకం_2->c_lflag &= ~XCASE;
              బహుళకం_2->c_iflag &= ~IUCLC;
              బహుళకం_2->c_oflag &= ~OLCUC;
            }
          లేకపోతే
            {
              బహుళకం_2->c_lflag |= XCASE;
              బహుళకం_2->c_iflag |= IUCLC;
              బహుళకం_2->c_oflag |= OLCUC;
            }
        }
#అయితే_ముగింపు
      లేకపోతే ఒకవేళ (streq (info->name, "crt"))
        బహుళకం_2->c_lflag |= ECHOE
#నిర్వచితమైతే ECHOCTL
          | ECHOCTL
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOKE
          | ECHOKE
#అయితే_ముగింపు
          ;
      లేకపోతే ఒకవేళ (streq (info->name, "dec"))
        {
          బహుళకం_2->c_cc[VINTR] = 3;	/* ^C */
          బహుళకం_2->c_cc[VERASE] = 127;	/* DEL */
          బహుళకం_2->c_cc[VKILL] = 21;	/* ^U */
          బహుళకం_2->c_lflag |= ECHOE
#నిర్వచితమైతే ECHOCTL
            | ECHOCTL
#అయితే_ముగింపు
#నిర్వచితమైతే ECHOKE
            | ECHOKE
#అయితే_ముగింపు
            ;
#నిర్వచితమైతే IXANY
          బహుళకం_2->c_iflag &= ~IXANY;
#అయితే_ముగింపు
        }
    }
  లేకపోతే ఒకవేళ (reversed)
    *bitsp = *bitsp & ~info->ముసుగు & ~info->బిట్లు_2;
  లేకపోతే
    *bitsp = (*bitsp & ~info->ముసుగు) | info->బిట్లు_2;

  తిరిగి true;
}

స్థిర శూన్య
సమితి_control_char (నిర్మాణం control_info స్థిరరాశి *info, అక్షరం స్థిరరాశి *arg,
                  నిర్మాణం termios *బహుళకం_2)
{
  గుర్తులేని దీర్ఘ పూర్ణాంకం విలువ;

  ఒకవేళ (streq (info->name, "min") || streq (info->name, "time"))
    విలువ = integer_arg (arg, TYPE_గరిష్ఠ (cc_t));
  లేకపోతే ఒకవేళ (arg[0] == '\0' || arg[1] == '\0')
    విలువ = గమ్యం_uchar (arg[0]);
  లేకపోతే ఒకవేళ (streq (arg, "^-") || streq (arg, "undef"))
    విలువ = _POSIX_VDISABLE;
  లేకపోతే ఒకవేళ (arg[0] == '^' && arg[1] != '\0')	/* Ignore any trailing junk. */
    {
      ఒకవేళ (arg[1] == '?')
        విలువ = 127;
      లేకపోతే
        విలువ = గమ్యం_uchar (arg[1]) & ~0140; /* Non-letters get weird results. */
    }
  లేకపోతే
    విలువ = integer_arg (arg, TYPE_గరిష్ఠ (cc_t));
  బహుళకం_2->c_cc[info->స్థానభ్రంశం] = విలువ;
}

స్థిర శూన్య
సమితి_వేగం (గణన వేగం_setting type, అక్షరం స్థిరరాశి *arg,
           నిర్మాణం వేగం_desc స్థిరరాశి వేగం, నిర్మాణం termios *బహుళకం_2)
{
  /* Note cfset[io]speed(), do not check with the device,
     and only check whether the system logic supports the specified speed.
     Therefore we don't report the device name in any errors.  */

  affirm (వేగం.valid);

  ఒకవేళ (type == ఆదానం_వేగం || type == both_speeds)
    {
      చివరి_ibaud = వేగం;
      ఒకవేళ (cfsetispeed (బహుళకం_2, వేగం.baud))
        error (EXIT_FAILURE, 0, _("unsupported ispeed %s"), quoteaf (arg));
    }
  ఒకవేళ (type == output_వేగం || type == both_speeds)
    {
      చివరి_obaud = వేగం;
      ఒకవేళ (cfsetospeed (బహుళకం_2, వేగం.baud))
        error (EXIT_FAILURE, 0, _("unsupported ospeed %s"), quoteaf (arg));
    }
}

#నిర్వచితమైతే TIOCGWINSZ

స్థిర పూర్ణాంకం
get_win_కొలత (పూర్ణాంకం fd, నిర్మాణం winsize *win)
{
  పూర్ణాంకం err = ioctl (fd, TIOCGWINSZ, (అక్షరం *) win);
  తిరిగి err;
}

స్థిర శూన్య
సమితి_కిటికీ_కొలత (పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య_2, అక్షరం స్థిరరాశి *device_name)
{
  నిర్మాణం winsize win;

  ఒకవేళ (get_win_కొలత (STDIN_FILENO, &win))
    {
      ఒకవేళ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      memset (&win, 0, పరిమాణం (win));
    }

  ఒకవేళ (అడ్డువరుసలు >= 0)
    win.ws_అడ్డువరుస = అడ్డువరుసలు;
  ఒకవేళ (నిలువుల_సంఖ్య_2 >= 0)
    win.ws_నిలువు = నిలువుల_సంఖ్య_2;

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

  ఒకవేళ (win.ws_అడ్డువరుస == 0 || win.ws_నిలువు == 0)
    {
      నిర్మాణం ttysize ttysz;

      ttysz.ts_పంక్తులు = win.ws_అడ్డువరుస;
      ttysz.ts_నిలువుల_సంఖ్య = win.ws_నిలువు;

      win.ws_అడ్డువరుస = 1;
      win.ws_నిలువు = 1;

      ఒకవేళ (ioctl (STDIN_FILENO, TIOCSWINSZ, (అక్షరం *) &win))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));

      ఒకవేళ (ioctl (STDIN_FILENO, TIOCSSIZE, (అక్షరం *) &ttysz))
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      తిరిగి;
    }
# endif

  ఒకవేళ (ioctl (STDIN_FILENO, TIOCSWINSZ, (అక్షరం *) &win))
    error (EXIT_FAILURE, errno, "%s", quotef (device_name));
}

స్థిర శూన్య
display_కిటికీ_కొలత (bool fancy, అక్షరం స్థిరరాశి *device_name)
{
  నిర్మాణం winsize win;

  ఒకవేళ (get_win_కొలత (STDIN_FILENO, &win))
    {
      ఒకవేళ (errno != EINVAL)
        error (EXIT_FAILURE, errno, "%s", quotef (device_name));
      ఒకవేళ (!fancy)
        error (EXIT_FAILURE, 0,
               _("%s: no size information for this device"),
               quotef (device_name));
    }
  లేకపోతే
    {
      wrapf (fancy ? "rows %d; columns %d;" : "%d %d\n",
             win.ws_అడ్డువరుస, win.ws_నిలువు);
      ఒకవేళ (!fancy)
        current_నిలువు = 0;
    }
}
#అయితే_ముగింపు

స్థిర పూర్ణాంకం
screen_columns (శూన్య)
{
#నిర్వచితమైతే TIOCGWINSZ
  నిర్మాణం winsize win;

  /* With Solaris 2.[123], this ioctl fails and errno is set to
     EINVAL for telnet (but not rlogin) sessions.
     On ISC 3.0, it fails for the console and the serial port
     (but it works for ptys).
     It can also fail on any system when stdout isn't a tty.
     In case of any failure, just use the default.  */
  ఒకవేళ (get_win_కొలత (STDOUT_FILENO, &win) == 0 && 0 < win.ws_నిలువు)
    తిరిగి win.ws_నిలువు;
#అయితే_ముగింపు
  {
    /* Use $COLUMNS if it's in [1..INT_MAX].  */
    అక్షరం *నిలువు_string = getenv ("COLUMNS");
    దీర్ఘ పూర్ణాంకం n_columns;
    ఒకవేళ (!(నిలువు_string != ఖాళీ
          && xstrtol (నిలువు_string, ఖాళీ, 0, &n_columns, "") == LONGINT_OK
          && 0 < n_columns
          && n_columns <= గరిష్ఠ_పూర్ణాంకం))
      n_columns = 80;
    తిరిగి n_columns;
  }
}

ATTRIBUTE_శుద్ధ
స్థిర tcflag_t *
బహుళకం_type_flag (గణన బహుళకం_type type, నిర్మాణం termios *బహుళకం_2)
{
  ఎంపిక (type)
    {
    స్థితి control:
      తిరిగి &బహుళకం_2->c_cflag;

    స్థితి ఆదానం:
      తిరిగి &బహుళకం_2->c_iflag;

    స్థితి output:
      తిరిగి &బహుళకం_2->c_oflag;

    స్థితి local:
      తిరిగి &బహుళకం_2->c_lflag;

    స్థితి combination:
      తిరిగి ఖాళీ;

    అప్రమేయ:
      unreachable ();
    }
}

స్థిర శూన్య
display_settings (గణన output_type output_type, నిర్మాణం termios *బహుళకం_2,
                  అక్షరం స్థిరరాశి *device_name)
{
  ఎంపిక (output_type)
    {
    స్థితి మారింది:
      display_మారింది (బహుళకం_2);
      ఆపు;

    స్థితి all:
      display_all (బహుళకం_2, device_name);
      ఆపు;

    స్థితి recoverable:
      display_recoverable (బహుళకం_2);
      ఆపు;
    }
}

స్థిర శూన్య
display_మారింది (నిర్మాణం termios *బహుళకం_2)
{
  bool ఖాళీలు_పంక్తి;
  tcflag_t *bitsp;
  గుర్తులేని దీర్ఘ ముసుగు;
  గణన బహుళకం_type prev_type = control;

  display_వేగం (బహుళకం_2, true);
#నిర్వచితమైతే HAVE_C_పంక్తి
  wrapf ("line = %d;", బహుళకం_2->c_పంక్తి);
#అయితే_ముగింపు
  అక్షరం_రాయి ('\n');
  current_నిలువు = 0;

  ఖాళీలు_పంక్తి = true;
  కొరకు (పూర్ణాంకం i = 0; !streq (control_info[i].name, "min"); ++i)
    {
      ఒకవేళ (బహుళకం_2->c_cc[control_info[i].స్థానభ్రంశం] == control_info[i].saneval)
        కొనసాగు;

#నిర్వచితమైతే VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ఒకవేళ (streq (control_info[i].name, "flush"))
        కొనసాగు;
#అయితే_ముగింపు
      /* If swtch is the same as susp, don't print both.  */
#ఒకవేళ VSWTCH == VSUSP
      ఒకవేళ (streq (control_info[i].name, "swtch"))
        కొనసాగు;
#అయితే_ముగింపు
      /* If eof uses the same slot as min, only print whichever applies.  */
#ఒకవేళ VEOF == VMIN
      ఒకవేళ ((బహుళకం_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        కొనసాగు;
#అయితే_ముగింపు

      ఖాళీలు_పంక్తి = false;
      wrapf ("%s = %s;", control_info[i].name,
             visible (బహుళకం_2->c_cc[control_info[i].స్థానభ్రంశం]));
    }
  ఒకవేళ ((బహుళకం_2->c_lflag & ICANON) == 0)
    {
      wrapf ("min = %lu; time = %lu;\n",
             (గుర్తులేని దీర్ఘ పూర్ణాంకం) బహుళకం_2->c_cc[VMIN],
             (గుర్తులేని దీర్ఘ పూర్ణాంకం) బహుళకం_2->c_cc[VTIME]);
    }
  లేకపోతే ఒకవేళ (!ఖాళీలు_పంక్తి)
    అక్షరం_రాయి ('\n');
  current_నిలువు = 0;

  ఖాళీలు_పంక్తి = true;
  కొరకు (పూర్ణాంకం i = 0; బహుళకం_info[i].name != ఖాళీ; ++i)
    {
      ఒకవేళ (బహుళకం_info[i].flags & OMIT)
        కొనసాగు;
      ఒకవేళ (బహుళకం_info[i].type != prev_type)
        {
          ఒకవేళ (!ఖాళీలు_పంక్తి)
            {
              అక్షరం_రాయి ('\n');
              current_నిలువు = 0;
              ఖాళీలు_పంక్తి = true;
            }
          prev_type = బహుళకం_info[i].type;
        }

      bitsp = బహుళకం_type_flag (బహుళకం_info[i].type, బహుళకం_2);
      ముసుగు = బహుళకం_info[i].ముసుగు ? బహుళకం_info[i].ముసుగు : బహుళకం_info[i].బిట్లు_2;
      ఒకవేళ ((*bitsp & ముసుగు) == బహుళకం_info[i].బిట్లు_2)
        {
          ఒకవేళ (బహుళకం_info[i].flags & SANE_UNSET)
            {
              wrapf ("%s", బహుళకం_info[i].name);
              ఖాళీలు_పంక్తి = false;
            }
        }
      లేకపోతే ఒకవేళ ((బహుళకం_info[i].flags & (SANE_సమితి | REV)) == (SANE_సమితి | REV))
        {
          wrapf ("-%s", బహుళకం_info[i].name);
          ఖాళీలు_పంక్తి = false;
        }
    }
  ఒకవేళ (!ఖాళీలు_పంక్తి)
    అక్షరం_రాయి ('\n');
  current_నిలువు = 0;
}

స్థిర శూన్య
display_all (నిర్మాణం termios *బహుళకం_2, అక్షరం స్థిరరాశి *device_name)
{
  tcflag_t *bitsp;
  గుర్తులేని దీర్ఘ ముసుగు;
  గణన బహుళకం_type prev_type = control;

  display_వేగం (బహుళకం_2, true);
#నిర్వచితమైతే TIOCGWINSZ
  display_కిటికీ_కొలత (true, device_name);
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_C_పంక్తి
  wrapf ("line = %d;", బహుళకం_2->c_పంక్తి);
#అయితే_ముగింపు
  అక్షరం_రాయి ('\n');
  current_నిలువు = 0;

  కొరకు (పూర్ణాంకం i = 0; ! streq (control_info[i].name, "min"); ++i)
    {
#నిర్వచితమైతే VFLUSHO
      /* 'flush' is the deprecated equivalent of 'discard'.  */
      ఒకవేళ (streq (control_info[i].name, "flush"))
        కొనసాగు;
#అయితే_ముగింపు
      /* If swtch is the same as susp, don't print both.  */
#ఒకవేళ VSWTCH == VSUSP
      ఒకవేళ (streq (control_info[i].name, "swtch"))
        కొనసాగు;
#అయితే_ముగింపు
      /* If eof uses the same slot as min, only print whichever applies.  */
#ఒకవేళ VEOF == VMIN
      ఒకవేళ ((బహుళకం_2->c_lflag & ICANON) == 0
          && (streq (control_info[i].name, "eof")
              || streq (control_info[i].name, "eol")))
        కొనసాగు;
#అయితే_ముగింపు
      wrapf ("%s = %s;", control_info[i].name,
             visible (బహుళకం_2->c_cc[control_info[i].స్థానభ్రంశం]));
    }
#ఒకవేళ VEOF == VMIN
  ఒకవేళ ((బహుళకం_2->c_lflag & ICANON) == 0)
#అయితే_ముగింపు
    wrapf ("min = %lu; time = %lu;",
           (గుర్తులేని దీర్ఘ పూర్ణాంకం) బహుళకం_2->c_cc[VMIN],
           (గుర్తులేని దీర్ఘ పూర్ణాంకం) బహుళకం_2->c_cc[VTIME]);
  ఒకవేళ (current_నిలువు != 0)
    అక్షరం_రాయి ('\n');
  current_నిలువు = 0;

  కొరకు (పూర్ణాంకం i = 0; బహుళకం_info[i].name != ఖాళీ; ++i)
    {
      ఒకవేళ (బహుళకం_info[i].flags & OMIT)
        కొనసాగు;
      ఒకవేళ (బహుళకం_info[i].type != prev_type)
        {
          అక్షరం_రాయి ('\n');
          current_నిలువు = 0;
          prev_type = బహుళకం_info[i].type;
        }

      bitsp = బహుళకం_type_flag (బహుళకం_info[i].type, బహుళకం_2);
      ముసుగు = బహుళకం_info[i].ముసుగు ? బహుళకం_info[i].ముసుగు : బహుళకం_info[i].బిట్లు_2;
      ఒకవేళ ((*bitsp & ముసుగు) == బహుళకం_info[i].బిట్లు_2)
        wrapf ("%s", బహుళకం_info[i].name);
      లేకపోతే ఒకవేళ (బహుళకం_info[i].flags & REV)
        wrapf ("-%s", బహుళకం_info[i].name);
    }
  అక్షరం_రాయి ('\n');
  current_నిలువు = 0;
}

/* Verify requested asymmetric speeds are supported.
   Note we don't flag the case where only ispeed or
   ospeed is set, when that would set both.  */

స్థిర శూన్య
check_వేగం (నిర్మాణం termios *బహుళకం_2)
{
  ఒకవేళ (చివరి_ibaud.valid && చివరి_obaud.valid)
    {
      ఒకవేళ (cfgetispeed (బహుళకం_2) != చివరి_ibaud.baud
          || cfgetospeed (బహుళకం_2) != చివరి_obaud.baud)
        error (EXIT_FAILURE, 0,
               _("asymmetric input (%lu), output (%lu) speeds not supported"),
               baud_గమ్యం_విలువ (చివరి_ibaud.baud),
               baud_గమ్యం_విలువ (చివరి_obaud.baud));
    }
}

స్థిర శూన్య
display_వేగం (నిర్మాణం termios *బహుళకం_2, bool fancy)
{
  ఒకవేళ (cfgetispeed (బహుళకం_2) == 0 || cfgetispeed (బహుళకం_2) == cfgetospeed (బహుళకం_2))
    wrapf (fancy ? "speed %lu baud;" : "%lu\n",
           baud_గమ్యం_విలువ (cfgetospeed (బహుళకం_2)));
  లేకపోతే
    wrapf (fancy ? "ispeed %lu baud; ospeed %lu baud;" : "%lu %lu\n",
           baud_గమ్యం_విలువ (cfgetispeed (బహుళకం_2)),
           baud_గమ్యం_విలువ (cfgetospeed (బహుళకం_2)));
  ఒకవేళ (!fancy)
    current_నిలువు = 0;
}

స్థిర శూన్య
display_recoverable (నిర్మాణం termios *బహుళకం_2)
{
  రాయి ("%lx:%lx:%lx:%lx",
          (గుర్తులేని దీర్ఘ పూర్ణాంకం) బహుళకం_2->c_iflag,
          (గుర్తులేని దీర్ఘ పూర్ణాంకం) బహుళకం_2->c_oflag,
          (గుర్తులేని దీర్ఘ పూర్ణాంకం) బహుళకం_2->c_cflag,
          (గుర్తులేని దీర్ఘ పూర్ణాంకం) బహుళకం_2->c_lflag);
  కొరకు (size_t i = 0; i < NCCS; ++i)
    రాయి (":%lx", (గుర్తులేని దీర్ఘ పూర్ణాంకం) బహుళకం_2->c_cc[i]);

  /* Without CBAUD, speeds are stored separately from c_cflag (e.g.,
     on BSD).  Otherwise preserve the old format unless BOTHER is used.  */
  bool separate_speeds = true;
#నిర్వచితమైతే CBAUD
  separate_speeds = false;
# ifdef BOTHER
  separate_speeds = (బహుళకం_2->c_cflag & CBAUD) == BOTHER;
#  ఒకవేళ defined CIBAUD && defined IBSHIFT
  separate_speeds |= ((బహుళకం_2->c_cflag & CIBAUD) >> IBSHIFT) == BOTHER;
#  endif
# endif
#అయితే_ముగింపు
  ఒకవేళ (separate_speeds)
    రాయి (":%lx:%lx", (గుర్తులేని దీర్ఘ పూర్ణాంకం) cfgetispeed (బహుళకం_2),
            (గుర్తులేని దీర్ఘ పూర్ణాంకం) cfgetospeed (బహుళకం_2));

  అక్షరం_రాయి ('\n');
}

/* Define a strtoul wrapper that checks the delimiter and result range.  */
#నిర్వచించు STRTOUL_TYPE(TYPE)                                               \
  స్థిర పూర్ణాంకం                                                             \
  strtoul_##TYPE (అక్షరం స్థిరరాశి *s, పూర్ణాంకం పునాది_2, అక్షరం **p, TYPE *ఫలితం,       \
                  అక్షరం delim)                                            \
  {                                                                      \
    గుర్తులేని దీర్ఘ ul;                                                    \
    errno = 0;                                                           \
    ul = strtoul (s, p, పునాది_2);                                           \
    ఒకవేళ (errno || **p != delim || *p == s || (TYPE) ul != ul)             \
      తిరిగి -1;                                                         \
    *ఫలితం = ul;                                                        \
    తిరిగి 0;                                                            \
  }

STRTOUL_TYPE (tcflag_t)
STRTOUL_TYPE (cc_t)
STRTOUL_TYPE (వేగం_t)
#నిర్వచనం_రద్దు STRTOUL_TYPE

/* Autogenerated conversion functions to/from speed_t */
#అన్తర్భావ "speedlist.h"

/* Parse the output of display_recoverable.
   Return false if any part of it is invalid.  */
స్థిర bool
recover_బహుళకం (అక్షరం స్థిరరాశి *arg, నిర్మాణం termios *బహుళకం_2)
{
  tcflag_t flag[4];
  అక్షరం స్థిరరాశి *s = arg;
  కొరకు (size_t i = 0; i < 4; i++)
    {
      అక్షరం *p;
      ఒకవేళ (strtoul_tcflag_t (s, 16, &p, flag + i, ':') != 0)
        తిరిగి false;
      s = p + 1;
    }
  బహుళకం_2->c_iflag = flag[0];
  బహుళకం_2->c_oflag = flag[1];
  బహుళకం_2->c_cflag = flag[2];
  బహుళకం_2->c_lflag = flag[3];

  కొరకు (size_t i = 0; i < NCCS; ++i)
    {
      అక్షరం *p;
      అక్షరం delim = i < NCCS - 1 || strchr (s, ':') ? ':' : '\0';
      ఒకవేళ (strtoul_cc_t (s, 16, &p, బహుళకం_2->c_cc + i, delim) != 0)
        తిరిగి false;
      /* Accept the old format, which has no separate speed fields.  */
      ఒకవేళ (! *p)
        {
#నిర్వచితమైతే CBAUD
          /* Some systems keep numeric speeds separately even when the
             control flags encode them.  Ensure those speeds are sync'd
             after restoring the flags.  */
# ifdef TERMIOS_వేగం_T_SANE
          నిర్మాణం వేగం_desc ospeed = cbaud_గమ్యం_వేగం (బహుళకం_2->c_cflag & CBAUD);
          ఒకవేళ (ospeed.valid && cfsetospeed (బహుళకం_2, ospeed.baud) != 0)
            తిరిగి false;
#  ఒకవేళ defined CIBAUD && defined IBSHIFT
          నిర్మాణం వేగం_desc ispeed =
            cbaud_గమ్యం_వేగం ((బహుళకం_2->c_cflag & CIBAUD) >> IBSHIFT);
          ఒకవేళ (ispeed.valid && cfsetispeed (బహుళకం_2, ispeed.baud) != 0)
            తిరిగి false;
#  endif
# endif
          చివరి_ibaud.baud = cfgetispeed (బహుళకం_2);
          చివరి_obaud.baud = cfgetospeed (బహుళకం_2);
#అయితే_ముగింపు
          తిరిగి true;
        }
      s = p + 1;
    }

  వేగం_t వేగం[2];
  కొరకు (size_t i = 0; i < 2; i++)
    {
      అక్షరం *p;
      అక్షరం delim = i == 0 ? ':' : '\0';
      ఒకవేళ (strtoul_వేగం_t (s, 16, &p, వేగం + i, delim) != 0)
        తిరిగి false;
      s = p + (i == 0);
    }

  /* Set output first, since an input speed of zero can mean to use
     the output speed.  */
  ఒకవేళ (cfsetospeed (బహుళకం_2, వేగం[1]) != 0
      || cfsetispeed (బహుళకం_2, వేగం[0]) != 0)
    తిరిగి false;

  చివరి_ibaud.baud = వేగం[0] == 0 ? cfgetispeed (బహుళకం_2) : వేగం[0];
  చివరి_obaud.baud = వేగం[1];
  తిరిగి true;
}

/* Return true if MODE1 and MODE2 have equivalent speeds.  */
స్థిర bool
వేగం_matches (నిర్మాణం termios స్థిరరాశి *mode1, నిర్మాణం termios స్థిరరాశి *mode2)
{
  వేగం_t speed1[2] = { cfgetospeed (mode1), cfgetispeed (mode1) };
  వేగం_t speed2[2] = { cfgetospeed (mode2), cfgetispeed (mode2) };
  ఒకవేళ (speed1[1] == 0)
    speed1[1] = speed1[0];
  ఒకవేళ (speed2[1] == 0)
    speed2[1] = speed2[0];

  గుర్తులేని పూర్ణాంకం బిట్లు_2 = 2;  /* Start and stop bits.  */
  బిట్లు_2 += !!(mode1->c_cflag & CSTOPB);  /* Extra stop bit.  */
  బిట్లు_2 += !!(mode1->c_cflag & PARENB);  /* Parity bit.  */
  /* Data bits.  */
  ఎంపిక (mode1->c_cflag & CSIZE)
    {
      స్థితి CS5: బిట్లు_2 += 5; ఆపు;
      స్థితి CS6: బిట్లు_2 += 6; ఆపు;
      స్థితి CS7: బిట్లు_2 += 7; ఆపు;
      అప్రమేయ: బిట్లు_2 += 8; ఆపు;
    }

  కొరకు (పూర్ణాంకం i = 0; i < 2; i++)
    {
      గుర్తులేని దీర్ఘ పూర్ణాంకం value1;
      గుర్తులేని దీర్ఘ పూర్ణాంకం value2;
      గుర్తులేని దీర్ఘ పూర్ణాంకం delta;

      ఒకవేళ (speed1[i] == speed2[i])
        కొనసాగు;

      value1 = baud_గమ్యం_విలువ (speed1[i]);
      value2 = baud_గమ్యం_విలువ (speed2[i]);

#నిర్వచితం_కాకపోతే TERMIOS_వేగం_T_SANE
      /* baud_to_value uses ULONG_MAX for an unrecognized encoding.  */
      ఒకవేళ (value1 == ULONG_MAX || value2 == ULONG_MAX)
        తిరిగి false;
#అయితే_ముగింపు

      /* Accept a value that's within a 1/4 bit per character.
         E.g., for 8N1, within 2.5% of the requested speed.  */
      delta = value1 < value2 ? value2 - value1 : value1 - value2;
      ఒకవేళ (delta > value1 / (బిట్లు_2 * 4))
        తిరిగి false;
    }

  తిరిగి true;
}

ATTRIBUTE_శుద్ధ
స్థిర నిర్మాణం వేగం_desc
string_గమ్యం_baud (అక్షరం స్థిరరాశి *arg)
{
  అక్షరం *ep;
  గుర్తులేని దీర్ఘ విలువ;
  గుర్తులేని అక్షరం c;

  /* Explicitly disallow negative numbers.  */
  అయితే (c_isspace (*arg))
    arg++;
  ఒకవేళ (*arg == '-')
    వెళ్ళు invalid;

  /* Empty does not mean 0.  */
  ఒకవేళ (! *arg)
    వెళ్ళు invalid;

  విలువ = strtoul (arg, &ep, 10);

  c = *ep++;
  ఒకవేళ (c == '.')
    {
      /* Number includes a fraction. Round it to nearest-even.
         Note in particular that 134.5 must round to 134! */
      c = *ep++;
      ఒకవేళ (c)
        {
          గుర్తులేని అక్షరం d = c - '0';
          ఒకవేళ (d > 5)
            విలువ++;
          లేకపోతే ఒకవేళ (d == 5)
            {
              అయితే ((c = *ep++) == '0'); /* Skip zeroes after .5 */

              ఒకవేళ (c)
                విలువ++;                /* Nonzero, round up */
              లేకపోతే
                విలువ += (విలువ & 1);   /* Exactly in the middle, round even */
            }

          అయితే (c_isdigit (c)) /* Skip remaining digits.  */
            c = *ep++;

          ఒకవేళ (c)
            వెళ్ళు invalid; /* Garbage after otherwise valid number */
        }
    }
  లేకపోతే ఒకవేళ (c)
    {
      /* Not a valid number; check for legacy aliases "exta" and "extb" */
      ఒకవేళ (streq (arg, "exta"))
        తిరిగి (నిర్మాణం వేగం_desc) { .baud = B19200, .valid = true };
      లేకపోతే ఒకవేళ (streq (arg, "extb"))
        తిరిగి (నిర్మాణం వేగం_desc) { .baud = B38400, .valid = true };
      లేకపోతే
        వెళ్ళు invalid;
    }

  తిరిగి విలువ_గమ్యం_baud (విలువ);

 invalid:
  తిరిగి (నిర్మాణం వేగం_desc) { .baud = (వేగం_t) -1, .valid = false };
}

స్థిర శూన్య
sane_బహుళకం (నిర్మాణం termios *బహుళకం_2)
{
  tcflag_t *bitsp;

  కొరకు (పూర్ణాంకం i = 0; control_info[i].name; ++i)
    {
#ఒకవేళ VMIN == VEOF
      ఒకవేళ (streq (control_info[i].name, "min"))
        ఆపు;
#అయితే_ముగింపు
      బహుళకం_2->c_cc[control_info[i].స్థానభ్రంశం] = control_info[i].saneval;
    }

  కొరకు (పూర్ణాంకం i = 0; బహుళకం_info[i].name != ఖాళీ; ++i)
    {
      ఒకవేళ (బహుళకం_info[i].flags & NO_SETATTR)
        కొనసాగు;

      ఒకవేళ (బహుళకం_info[i].flags & SANE_సమితి)
        {
          bitsp = బహుళకం_type_flag (బహుళకం_info[i].type, బహుళకం_2);
          assume (bitsp); /* combination modes will not have SANE_SET.  */
          *bitsp = (*bitsp & ~బహుళకం_info[i].ముసుగు) | బహుళకం_info[i].బిట్లు_2;
        }
      లేకపోతే ఒకవేళ (బహుళకం_info[i].flags & SANE_UNSET)
        {
          bitsp = బహుళకం_type_flag (బహుళకం_info[i].type, బహుళకం_2);
          assume (bitsp); /* combination modes will not have SANE_UNSET.  */
          *bitsp = *bitsp & ~బహుళకం_info[i].ముసుగు & ~బహుళకం_info[i].బిట్లు_2;
        }
    }
}

/* Return a string that is the printable representation of character CH.  */
/* Adapted from 'cat' by Torbjörn Granlund.  */

స్థిర అక్షరం స్థిరరాశి *
visible (cc_t ch)
{
  స్థిర అక్షరం buf[10];
  అక్షరం *bpout = buf;

  ఒకవేళ (ch == _POSIX_VDISABLE)
    తిరిగి "<undef>";

  ఒకవేళ (ch >= 32)
    {
      ఒకవేళ (ch < 127)
        *bpout++ = ch;
      లేకపోతే ఒకవేళ (ch == 127)
        {
          *bpout++ = '^';
          *bpout++ = '?';
        }
      లేకపోతే
        {
          *bpout++ = 'M';
          *bpout++ = '-';
          ఒకవేళ (ch >= 128 + 32)
            {
              ఒకవేళ (ch < 128 + 127)
                *bpout++ = ch - 128;
              లేకపోతే
                {
                  *bpout++ = '^';
                  *bpout++ = '?';
                }
            }
          లేకపోతే
            {
              *bpout++ = '^';
              *bpout++ = ch - 128 + 64;
            }
        }
    }
  లేకపోతే
    {
      *bpout++ = '^';
      *bpout++ = ch + 64;
    }
  *bpout = '\0';
  తిరిగి (అక్షరం స్థిరరాశి *) buf;
}

/* Parse string S as an integer, using decimal radix by default,
   but allowing octal and hex numbers as in C.  Reject values
   larger than MAXVAL.  */

స్థిర uintmax_t
integer_arg (అక్షరం స్థిరరాశి *s, uintmax_t maxval)
{
  తిరిగి xnumtoumax (s, 0, 0, maxval, "bB", _("invalid integer argument"),
                     0, 0);
}
