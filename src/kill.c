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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sig2str.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "operand2sig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "kill"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Paul Eggert")

ਸਥਿਰ ਅੱਖਰ ਅਚਲ short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"list", no_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"signal", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"table", no_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [-s SIGNAL | -SIGNAL] PID...\n\
  or:  %s -l [SIGNAL]...\n\
  or:  %s -t [SIGNAL]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Send ਸੰਕੇਤ_2 ਮੰਜ਼ਿਲ processes, or list ਸੰਕੇਤ_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -s, --ਸੰਕੇਤ=SIGNAL, -SIGNAL\n\
         specify the name or ਸੰਖਿਆ ਦਾ the ਸੰਕੇਤ ਮੰਜ਼ਿਲ be ਭੇਜਿਆ\n\
"));
      oputs (_("\
  -l, --list\n\
         list ਸੰਕੇਤ ਨਾਂ, or convert ਸੰਕੇਤ ਨਾਂ ਮੰਜ਼ਿਲ/from numbers\n\
"));
      oputs (_("\
  -t, --ਸਾਰਣੀ\n\
         print a ਸਾਰਣੀ ਦਾ ਸੰਕੇਤ information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a ਸੰਕੇਤ name like 'HUP', or a ਸੰਕੇਤ ਸੰਖਿਆ like '1',\n\
or the ਬਾਹਰ_ਨਿਕਲੋ ਸਥਿਤੀ ਦਾ a process terminated by a ਸੰਕੇਤ.\n\
PID is an integer; ਜੇ negative it identifies a process ਸਮੂਹ.\n\
"), stdout);
      ਲਿਖੋ (USAGE_BUILTIN_WARNING, ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

ਸਥਿਰ ਖਾਲੀ
print_ਸਾਰਣੀ_ਪੰਕਤੀ (ਪੂਰਨ ਅੰਕ_ਚੌੜਾਈ, ਪੂਰਨ signum,
                 ਪੂਰਨ name_ਚੌੜਾਈ, ਅੱਖਰ ਅਚਲ *signame)
{
  ਅੱਖਰ ਅਚਲ *description = strsignal (signum);
  ਲਿਖੋ ("%*d %-*s %s\n", ਅੰਕ_ਚੌੜਾਈ, signum, name_ਚੌੜਾਈ, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

ਸਥਿਰ ਪੂਰਨ
list_ਸੰਕੇਤ (bool ਸਾਰਣੀ, ਅੱਖਰ *ਅਚਲ *argv)
{
  ਪੂਰਨ ਸਥਿਤੀ = EXIT_SUCCESS;
  ਅੱਖਰ signame[SIG2STR_MAX];

  ਜੇ (ਸਾਰਣੀ)
    {
      ਪੂਰਨ name_ਚੌੜਾਈ = 0;

      /* Compute the maximum width of a signal number.  */
      ਪੂਰਨ ਅੰਕ_ਚੌੜਾਈ = 1;
      ਲਈ (ਪੂਰਨ signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        ਅੰਕ_ਚੌੜਾਈ++;

      /* Compute the maximum width of a signal name.  */
      ਲਈ (ਪੂਰਨ signum = 0; signum <= SIGNUM_BOUND; signum++)
        ਜੇ (sig2str (signum, signame) == 0)
          {
            idx_t len = ਲੜੀ_ਲੰਬਾਈ (signame);
            ਜੇ (name_ਚੌੜਾਈ < len)
              name_ਚੌੜਾਈ = len;
          }

      ਜੇ (argv)
        ਲਈ (; *argv; argv++)
          {
            ਪੂਰਨ signum = operand2sig (*argv);
            ਜੇ (signum < 0)
              ਸਥਿਤੀ = EXIT_FAILURE;
            ਨਹੀਂਤਾਂ
              {
                ਜੇ (sig2str (signum, signame) != 0)
                  snprintf (signame, ਆਕਾਰ signame, "SIG%d", signum);
                print_ਸਾਰਣੀ_ਪੰਕਤੀ (ਅੰਕ_ਚੌੜਾਈ, signum, name_ਚੌੜਾਈ, signame);
              }
          }
      ਨਹੀਂਤਾਂ
        ਲਈ (ਪੂਰਨ signum = 0; signum <= SIGNUM_BOUND; signum++)
          ਜੇ (sig2str (signum, signame) == 0)
            print_ਸਾਰਣੀ_ਪੰਕਤੀ (ਅੰਕ_ਚੌੜਾਈ, signum, name_ਚੌੜਾਈ, signame);
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (argv)
        ਲਈ (; *argv; argv++)
          {
            ਪੂਰਨ signum = operand2sig (*argv);
            ਜੇ (signum < 0)
              ਸਥਿਤੀ = EXIT_FAILURE;
            ਨਹੀਂਤਾਂ ਜੇ (c_isdigit (**argv))
              {
                ਜੇ (sig2str (signum, signame) == 0)
                  puts (signame);
                ਨਹੀਂਤਾਂ
                  ਲਿਖੋ ("%d\n", signum);
              }
            ਨਹੀਂਤਾਂ
              ਲਿਖੋ ("%d\n", signum);
          }
      ਨਹੀਂਤਾਂ
        ਲਈ (ਪੂਰਨ signum = 0; signum <= SIGNUM_BOUND; signum++)
          ਜੇ (sig2str (signum, signame) == 0)
            puts (signame);
    }

  ਵਾਪਸ ਸਥਿਤੀ;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

ਸਥਿਰ ਪੂਰਨ
send_ਸੰਕੇਤ (ਪੂਰਨ signum, ਅੱਖਰ *ਅਚਲ *argv)
{
  ਪੂਰਨ ਸਥਿਤੀ = EXIT_SUCCESS;
  ਅੱਖਰ ਅਚਲ *arg = *argv;

  ਕਰੋ
    {
      ਅੱਖਰ *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      ਜੇ (errno == ERANGE || ckd_ਜੋੜੋ (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          ਸਥਿਤੀ = EXIT_FAILURE;
        }
      ਨਹੀਂਤਾਂ ਜੇ (kill (pid, signum) != 0)
        {
          ਜੇ (errno == EINVAL)
            error (0, errno, "%d", signum);
          ਨਹੀਂਤਾਂ
            error (0, errno, "%s", quote (arg));
          ਸਥਿਤੀ = EXIT_FAILURE;
        }
    }
  ਜਦੋਂ ((arg = *++argv));

  ਵਾਪਸ ਸਥਿਤੀ;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optc;
  bool list = false;
  bool ਸਾਰਣੀ = false;
  ਪੂਰਨ signum = -1;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, short_options, long_options, ਨਿਰਾ))
         != -1)
    ਚੋਣ (optc)
      {
      ਹਾਲਤ '0': ਹਾਲਤ '1': ਹਾਲਤ '2': ਹਾਲਤ '3': ਹਾਲਤ '4':
      ਹਾਲਤ '5': ਹਾਲਤ '6': ਹਾਲਤ '7': ਹਾਲਤ '8': ਹਾਲਤ '9':
        ਜੇ (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            ਜਾਓ no_ਹੋਰ_options;
          }
        FALLTHROUGH;
      ਹਾਲਤ 'A': ਹਾਲਤ 'B': ਹਾਲਤ 'C': ਹਾਲਤ 'D': ਹਾਲਤ 'E':
      ਹਾਲਤ 'F': ਹਾਲਤ 'G': ਹਾਲਤ 'H': ਹਾਲਤ 'I': ਹਾਲਤ 'J':
      ਹਾਲਤ 'K': /*case 'L':*/ ਹਾਲਤ 'M': ਹਾਲਤ 'N': ਹਾਲਤ 'O':
      ਹਾਲਤ 'P': ਹਾਲਤ 'Q': ਹਾਲਤ 'R': ਹਾਲਤ 'S': ਹਾਲਤ 'T':
      ਹਾਲਤ 'U': ਹਾਲਤ 'V': ਹਾਲਤ 'W': ਹਾਲਤ 'X': ਹਾਲਤ 'Y':
      ਹਾਲਤ 'Z':
        ਜੇ (! optarg)
          optarg = argv[optind - 1] + ਲੜੀ_ਲੰਬਾਈ (argv[optind - 1]);
        ਜੇ (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      ਹਾਲਤ 'n': /* -n is not documented, but is for Bash compatibility.  */
      ਹਾਲਤ 's':
        ਜੇ (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        ਜੇ (signum < 0)
          usage (EXIT_FAILURE);
        ਰੋਕੋ;

      ਹਾਲਤ 'L': /* -L is not documented, but is for procps compatibility.  */
      ਹਾਲਤ 't':
        ਸਾਰਣੀ = true;
        FALLTHROUGH;
      ਹਾਲਤ 'l':
        ਜੇ (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        ਰੋਕੋ;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
      ਮੂਲ:
        usage (EXIT_FAILURE);
      }
 no_ਹੋਰ_options:

  ਜੇ (signum < 0)
    signum = SIGTERM;
  ਨਹੀਂਤਾਂ ਜੇ (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  ਜੇ ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  ਵਾਪਸ (list
          ? list_ਸੰਕੇਤ (ਸਾਰਣੀ, optind < argc ? argv + optind : ਨਿਰਾ)
          : send_ਸੰਕੇਤ (signum, argv + optind));
}
