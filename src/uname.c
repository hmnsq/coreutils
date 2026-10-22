/* uname -- print system information

   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Written by David MacKenzie <djm@gnu.ai.mit.edu> */

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <getopt.h>

#ако HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#ако HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# ако HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  инако
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#include "system.h"
#include "quote.h"
#include "uname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (uname_mode == UNAME_UNAME ? "uname" : "arch")

#define AUTHORS proper_name ("David MacKenzie")
#define ARCH_AUTHORS "David MacKenzie", "Karel Zak"

/* Values that are bitwise or'd into 'toprint'. */
/* Kernel name. */
#define PRINT_KERNEL_NAME 1

/* Node name on a communications network. */
#define PRINT_NODENAME 2

/* Kernel release. */
#define PRINT_KERNEL_RELEASE 4

/* Kernel version. */
#define PRINT_KERNEL_VERSION 8

/* Machine hardware name. */
#define PRINT_MACHINE 16

/* Processor type. */
#define PRINT_PROCESSOR 32

/* Hardware platform.  */
#define PRINT_HARDWARE_PLATFORM 64

/* Operating system.  */
#define PRINT_OPERATING_SYSTEM 128

static bool labeled = false;

static struct option const uname_long_options[] =
{
  {"all", no_argument, NULL, 'a'},
  {"all-labeled", no_argument, NULL, 'A'},
  {"kernel-name", no_argument, NULL, 's'},
  {"sysname", no_argument, NULL, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, NULL, 'n'},
  {"kernel-release", no_argument, NULL, 'r'},
  {"release", no_argument, NULL, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, NULL, 'v'},
  {"machine", no_argument, NULL, 'm'},
  {"processor", no_argument, NULL, 'p'},
  {"hardware-platform", no_argument, NULL, 'i'},
  {"operating-system", no_argument, NULL, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

static struct option const arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (цео status)
{
  ако (status != EXIT_SUCCESS)
    emit_try_help ();
  инако
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      ако (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i ако unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per line,\n\
                             except omit -p and -i ако unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network node hostname\n\
"));
          oputs (_("\
  -r, --kernel-release     print the kernel release\n\
"));
          oputs (_("\
  -v, --kernel-version     print the kernel version\n\
"));
          oputs (_("\
  -m, --machine            print the machine hardware name\n\
"));
          oputs (_("\
  -p, --processor          print the processor type (non-portable)\n\
"));
          oputs (_("\
  -i, --hardware-platform  print the hardware platform (non-portable)\n\
"));
          oputs (_("\
  -o, --operating-system   print the operating system\n\
"));
        }
      инако
        {
          fputs (_("\
Print machine architecture.\n\
\n\
"), stdout);
        }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  */

static void
print_element (char const *element, char const *label)
{
  ако (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  инако
    {
      static bool printed;
      ако (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

static void
print_element_env (char const *element,
                   MAYBE_UNUSED char const *envvar,
                   char const *label)
{
#ifdef __APPLE__
  ако (envvar)
    {
      char const *val = getenv (envvar);
      ако (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

static цео
decode_switches (цео argc, char **argv)
{
  цео c;
  unsigned цео toprint = 0;

  ако (uname_mode == UNAME_ARCH)
    {
      док ((c = getopt_long (argc, argv, "",
                               arch_long_options, NULL))
             != -1)
        {
          switch (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            default:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  инако
    {
      док ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, NULL))
             != -1)
        {
          switch (c)
            {
            case 'a':
              toprint = UINT_MAX;
              labeled = false;
              break;

            case 'A':
              toprint = UINT_MAX;
              labeled = true;
              break;

            case 's':
              toprint |= PRINT_KERNEL_NAME;
              break;

            case 'n':
              toprint |= PRINT_NODENAME;
              break;

            case 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              break;

            case 'v':
              toprint |= PRINT_KERNEL_VERSION;
              break;

            case 'm':
              toprint |= PRINT_MACHINE;
              break;

            case 'p':
              toprint |= PRINT_PROCESSOR;
              break;

            case 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              break;

            case 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              break;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            default:
              usage (EXIT_FAILURE);
            }
        }
    }

  ако (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  врати toprint;
}

цео
main (цео argc, char **argv)
{
  static char const unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned цео toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  ако (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  ако (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      struct utsname name;

      ако (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      ако (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      ако (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      ако (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      ако (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      ако (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  ако (toprint & PRINT_PROCESSOR)
    {
      char const *element = unknown;
#ifdef __APPLE__
# ако defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#ако HAVE_SYSINFO && defined SI_ARCHITECTURE
      ако (element == unknown)
        {
          static char processor[257];
          ако (0 <= sysinfo (SI_ARCHITECTURE, processor, sizeof processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      ако (element == unknown)
        {
          static char processor[257];
          size_t s = sizeof processor;
          static цео mib[] = { CTL_HW, UNAME_PROCESSOR };
          ако (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      ако (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  ако (toprint & PRINT_HARDWARE_PLATFORM)
    {
      char const *element = unknown;
#ако HAVE_SYSINFO && defined SI_PLATFORM
      {
        static char hardware_platform[257];
        ако (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, sizeof hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      ако (element == unknown)
        {
          static char hardware_platform[257];
          size_t s = sizeof hardware_platform;
          static цео mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          ако (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      ако (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  ако (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  ако (!labeled)
    putchar ('\n');

  врати EXIT_SUCCESS;
}
