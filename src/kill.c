/* kill -- send a signal to a process
   Copyright (C) 2002-2026 Free Software Foundation, Inc.

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

/* Written by Paul Eggert.  */

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <signal.h>

#شامل "system.h"
#شامل "c-ctype.h"
#شامل "sig2str.h"
#شامل "operand2sig.h"
#شامل "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "kill"

#تعریف_کرو AUTHORS proper_name ("Paul Eggert")

ساکن حرف ثابت short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

ساکن ساخت option ثابت long_options[] =
{
  {"list", no_دلیل, تہی, 'l'},
  {"signal", required_دلیل, تہی, 's'},
  {"table", no_دلیل, تہی, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [-s SIGNAL | -SIGNAL] PID...\n\
  or:  %s -l [SIGNAL]...\n\
  or:  %s -t [SIGNAL]...\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Send اشارات مقصد processes, or list اشارات.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -s, --اشارہ=SIGNAL, -SIGNAL\n\
         specify the name or نمبر کا the اشارہ مقصد be بھیجا\n\
"));
      oputs (_("\
  -l, --list\n\
         list اشارہ نام, or convert اشارہ نام مقصد/from numbers\n\
"));
      oputs (_("\
  -t, --جدول\n\
         print a جدول کا اشارہ information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a اشارہ name like 'HUP', or a اشارہ نمبر like '1',\n\
or the باہر_نکلو حالت_2 کا a process terminated by a اشارہ.\n\
PID is an integer; اگر negative it identifies a process گروہ.\n\
"), stdout);
      لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

ساکن خالی
print_جدول_صف (صحیح عدد_چوڑائی, صحیح signum,
                 صحیح name_چوڑائی, حرف ثابت *signame)
{
  حرف ثابت *description = strsignal (signum);
  لکھو ("%*d %-*s %s\n", عدد_چوڑائی, signum, name_چوڑائی, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

ساکن صحیح
list_اشارات (bool جدول, حرف *ثابت *argv)
{
  صحیح حالت_2 = EXIT_SUCCESS;
  حرف signame[SIG2STR_MAX];

  اگر (جدول)
    {
      صحیح name_چوڑائی = 0;

      /* Compute the maximum width of a signal number.  */
      صحیح عدد_چوڑائی = 1;
      برائے (صحیح signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        عدد_چوڑائی++;

      /* Compute the maximum width of a signal name.  */
      برائے (صحیح signum = 0; signum <= SIGNUM_BOUND; signum++)
        اگر (sig2str (signum, signame) == 0)
          {
            idx_t len = لڑی_لمبائی (signame);
            اگر (name_چوڑائی < len)
              name_چوڑائی = len;
          }

      اگر (argv)
        برائے (; *argv; argv++)
          {
            صحیح signum = operand2sig (*argv);
            اگر (signum < 0)
              حالت_2 = EXIT_FAILURE;
            ورنہ
              {
                اگر (sig2str (signum, signame) != 0)
                  snprintf (signame, حجم signame, "SIG%d", signum);
                print_جدول_صف (عدد_چوڑائی, signum, name_چوڑائی, signame);
              }
          }
      ورنہ
        برائے (صحیح signum = 0; signum <= SIGNUM_BOUND; signum++)
          اگر (sig2str (signum, signame) == 0)
            print_جدول_صف (عدد_چوڑائی, signum, name_چوڑائی, signame);
    }
  ورنہ
    {
      اگر (argv)
        برائے (; *argv; argv++)
          {
            صحیح signum = operand2sig (*argv);
            اگر (signum < 0)
              حالت_2 = EXIT_FAILURE;
            ورنہ اگر (c_isdigit (**argv))
              {
                اگر (sig2str (signum, signame) == 0)
                  puts (signame);
                ورنہ
                  لکھو ("%d\n", signum);
              }
            ورنہ
              لکھو ("%d\n", signum);
          }
      ورنہ
        برائے (صحیح signum = 0; signum <= SIGNUM_BOUND; signum++)
          اگر (sig2str (signum, signame) == 0)
            puts (signame);
    }

  واپس حالت_2;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

ساکن صحیح
send_اشارات (صحیح signum, حرف *ثابت *argv)
{
  صحیح حالت_2 = EXIT_SUCCESS;
  حرف ثابت *arg = *argv;

  کرو
    {
      حرف *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      اگر (errno == ERANGE || ckd_جوڑو (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          حالت_2 = EXIT_FAILURE;
        }
      ورنہ اگر (kill (pid, signum) != 0)
        {
          اگر (errno == EINVAL)
            error (0, errno, "%d", signum);
          ورنہ
            error (0, errno, "%s", quote (arg));
          حالت_2 = EXIT_FAILURE;
        }
    }
  جب ((arg = *++argv));

  واپس حالت_2;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc;
  bool list = false;
  bool جدول = false;
  صحیح signum = -1;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, short_options, long_options, تہی))
         != -1)
    انتخاب (optc)
      {
      حالت '0': حالت '1': حالت '2': حالت '3': حالت '4':
      حالت '5': حالت '6': حالت '7': حالت '8': حالت '9':
        اگر (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            جاؤ no_مزید_options;
          }
        FALLTHROUGH;
      حالت 'A': حالت 'B': حالت 'C': حالت 'D': حالت 'E':
      حالت 'F': حالت 'G': حالت 'H': حالت 'I': حالت 'J':
      حالت 'K': /*case 'L':*/ حالت 'M': حالت 'N': حالت 'O':
      حالت 'P': حالت 'Q': حالت 'R': حالت 'S': حالت 'T':
      حالت 'U': حالت 'V': حالت 'W': حالت 'X': حالت 'Y':
      حالت 'Z':
        اگر (! optarg)
          optarg = argv[optind - 1] + لڑی_لمبائی (argv[optind - 1]);
        اگر (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      حالت 'n': /* -n is not documented, but is for Bash compatibility.  */
      حالت 's':
        اگر (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        اگر (signum < 0)
          usage (EXIT_FAILURE);
        توڑو;

      حالت 'L': /* -L is not documented, but is for procps compatibility.  */
      حالت 't':
        جدول = true;
        FALLTHROUGH;
      حالت 'l':
        اگر (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        توڑو;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
      طےشدہ:
        usage (EXIT_FAILURE);
      }
 no_مزید_options:

  اگر (signum < 0)
    signum = SIGTERM;
  ورنہ اگر (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  اگر ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  واپس (list
          ? list_اشارات (جدول, optind < argc ? argv + optind : تہی)
          : send_اشارات (signum, argv + optind));
}
