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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن <signal.h>

#ضمن "system.h"
#ضمن "c-ctype.h"
#ضمن "sig2str.h"
#ضمن "operand2sig.h"
#ضمن "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "kill"

#تعریف_کن AUTHORS proper_name ("Paul Eggert")

ساكن حرف ثابت short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

ساكن بنية option ثابت long_options[] =
{
  {"list", no_آرگومان, عدم, 'l'},
  {"signal", required_آرگومان, عدم, 's'},
  {"table", no_آرگومان, عدم, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [-s SIGNAL | -SIGNAL] PID...\n\
  or:  %s -l [SIGNAL]...\n\
  or:  %s -t [SIGNAL]...\n\
"),
              برنامه_name, برنامه_name, برنامه_name);
      fputs (_("\
Send سیگنالها مقصد processes, or list سیگنالها.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -s, --إشارة=SIGNAL, -SIGNAL\n\
         specify the name or عدد از the إشارة مقصد be فرستاده\n\
"));
      oputs (_("\
  -l, --list\n\
         list إشارة نامها, or convert إشارة نامها مقصد/from numbers\n\
"));
      oputs (_("\
  -t, --جدول\n\
         print a جدول از إشارة information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a إشارة name like 'HUP', or a إشارة عدد like '1',\n\
or the خروج وضعیت از a process terminated by a إشارة.\n\
PID is an integer; إذا negative it identifies a process گروه.\n\
"), stdout);
      چاپ_کن (USAGE_BUILTIN_WARNING, برنامه_NAME);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

ساكن فراغ
print_جدول_ردیف (صحيح رقم_عرض, صحيح signum,
                 صحيح name_عرض, حرف ثابت *signame)
{
  حرف ثابت *description = strsignal (signum);
  چاپ_کن ("%*d %-*s %s\n", رقم_عرض, signum, name_عرض, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

ساكن صحيح
list_سیگنالها (bool جدول, حرف *ثابت *argv)
{
  صحيح وضعیت = EXIT_SUCCESS;
  حرف signame[SIG2STR_MAX];

  إذا (جدول)
    {
      صحيح name_عرض = 0;

      /* Compute the maximum width of a signal number.  */
      صحيح رقم_عرض = 1;
      لكل (صحيح signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        رقم_عرض++;

      /* Compute the maximum width of a signal name.  */
      لكل (صحيح signum = 0; signum <= SIGNUM_BOUND; signum++)
        إذا (sig2str (signum, signame) == 0)
          {
            idx_t len = طول_رشته (signame);
            إذا (name_عرض < len)
              name_عرض = len;
          }

      إذا (argv)
        لكل (; *argv; argv++)
          {
            صحيح signum = operand2sig (*argv);
            إذا (signum < 0)
              وضعیت = EXIT_FAILURE;
            وإلا
              {
                إذا (sig2str (signum, signame) != 0)
                  snprintf (signame, حجم signame, "SIG%d", signum);
                print_جدول_ردیف (رقم_عرض, signum, name_عرض, signame);
              }
          }
      وإلا
        لكل (صحيح signum = 0; signum <= SIGNUM_BOUND; signum++)
          إذا (sig2str (signum, signame) == 0)
            print_جدول_ردیف (رقم_عرض, signum, name_عرض, signame);
    }
  وإلا
    {
      إذا (argv)
        لكل (; *argv; argv++)
          {
            صحيح signum = operand2sig (*argv);
            إذا (signum < 0)
              وضعیت = EXIT_FAILURE;
            وإلا إذا (c_isdigit (**argv))
              {
                إذا (sig2str (signum, signame) == 0)
                  puts (signame);
                وإلا
                  چاپ_کن ("%d\n", signum);
              }
            وإلا
              چاپ_کن ("%d\n", signum);
          }
      وإلا
        لكل (صحيح signum = 0; signum <= SIGNUM_BOUND; signum++)
          إذا (sig2str (signum, signame) == 0)
            puts (signame);
    }

  أرجع وضعیت;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

ساكن صحيح
send_سیگنالها (صحيح signum, حرف *ثابت *argv)
{
  صحيح وضعیت = EXIT_SUCCESS;
  حرف ثابت *arg = *argv;

  افعل
    {
      حرف *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      إذا (errno == ERANGE || ckd_جمع_کن (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          وضعیت = EXIT_FAILURE;
        }
      وإلا إذا (kill (pid, signum) != 0)
        {
          إذا (errno == EINVAL)
            error (0, errno, "%d", signum);
          وإلا
            error (0, errno, "%s", quote (arg));
          وضعیت = EXIT_FAILURE;
        }
    }
  طالما ((arg = *++argv));

  أرجع وضعیت;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح optc;
  bool list = false;
  bool جدول = false;
  صحيح signum = -1;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, short_options, long_options, عدم))
         != -1)
    بدل (optc)
      {
      حالة '0': حالة '1': حالة '2': حالة '3': حالة '4':
      حالة '5': حالة '6': حالة '7': حالة '8': حالة '9':
        إذا (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            اذهب no_بیشتر_options;
          }
        FALLTHROUGH;
      حالة 'A': حالة 'B': حالة 'C': حالة 'D': حالة 'E':
      حالة 'F': حالة 'G': حالة 'H': حالة 'I': حالة 'J':
      حالة 'K': /*case 'L':*/ حالة 'M': حالة 'N': حالة 'O':
      حالة 'P': حالة 'Q': حالة 'R': حالة 'S': حالة 'T':
      حالة 'U': حالة 'V': حالة 'W': حالة 'X': حالة 'Y':
      حالة 'Z':
        إذا (! optarg)
          optarg = argv[optind - 1] + طول_رشته (argv[optind - 1]);
        إذا (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      حالة 'n': /* -n is not documented, but is for Bash compatibility.  */
      حالة 's':
        إذا (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        إذا (signum < 0)
          usage (EXIT_FAILURE);
        اكسر;

      حالة 'L': /* -L is not documented, but is for procps compatibility.  */
      حالة 't':
        جدول = true;
        FALLTHROUGH;
      حالة 'l':
        إذا (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        اكسر;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
      افتراضي:
        usage (EXIT_FAILURE);
      }
 no_بیشتر_options:

  إذا (signum < 0)
    signum = SIGTERM;
  وإلا إذا (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  إذا ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  أرجع (list
          ? list_سیگنالها (جدول, optind < argc ? argv + optind : عدم)
          : send_سیگنالها (signum, argv + optind));
}
