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

#නම් HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#නම් HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# නම් HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  නැතහොත්
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

static sa_nracanaa option const uname_long_options[] =
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

static sa_nracanaa option const arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (පූර්ණ_සංඛ්‍යා status)
{
  නම් (status != EXIT_SUCCESS)
    emit_try_help ();
  නැතහොත්
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      නම් (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i නම් unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per line,\n\
                             except omit -p and -i නම් unknown\n\
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
      නැතහොත්
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
print_element (var_na const *element, var_na const *label)
{
  නම් (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  නැතහොත්
    {
      static bool printed;
      නම් (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

static void
print_element_env (var_na const *element,
                   MAYBE_UNUSED var_na const *envvar,
                   var_na const *label)
{
#ifdef __APPLE__
  නම් (envvar)
    {
      var_na const *val = getenv (envvar);
      නම් (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

static පූර්ණ_සංඛ්‍යා
decode_switches (පූර්ණ_සංඛ්‍යා argc, var_na **argv)
{
  පූර්ණ_සංඛ්‍යා c;
  unsigned පූර්ණ_සංඛ්‍යා toprint = 0;

  නම් (uname_mode == UNAME_ARCH)
    {
      අතරතුර ((c = getopt_long (argc, argv, "",
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
  නැතහොත්
    {
      අතරතුර ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, NULL))
             != -1)
        {
          switch (c)
            {
            pak_sa 'a':
              toprint = UINT_MAX;
              labeled = false;
              bha_nga;

            pak_sa 'A':
              toprint = UINT_MAX;
              labeled = true;
              bha_nga;

            pak_sa 's':
              toprint |= PRINT_KERNEL_NAME;
              bha_nga;

            pak_sa 'n':
              toprint |= PRINT_NODENAME;
              bha_nga;

            pak_sa 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              bha_nga;

            pak_sa 'v':
              toprint |= PRINT_KERNEL_VERSION;
              bha_nga;

            pak_sa 'm':
              toprint |= PRINT_MACHINE;
              bha_nga;

            pak_sa 'p':
              toprint |= PRINT_PROCESSOR;
              bha_nga;

            pak_sa 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              bha_nga;

            pak_sa 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              bha_nga;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            default:
              usage (EXIT_FAILURE);
            }
        }
    }

  නම් (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  ආපසු toprint;
}

පූර්ණ_සංඛ්‍යා
main (පූර්ණ_සංඛ්‍යා argc, var_na **argv)
{
  static var_na const unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned පූර්ණ_සංඛ්‍යා toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  නම් (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  නම් (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      sa_nracanaa utsname name;

      නම් (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      නම් (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      නම් (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      නම් (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      නම් (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      නම් (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  නම් (toprint & PRINT_PROCESSOR)
    {
      var_na const *element = unknown;
#ifdef __APPLE__
# නම් defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#නම් HAVE_SYSINFO && defined SI_ARCHITECTURE
      නම් (element == unknown)
        {
          static var_na processor[257];
          නම් (0 <= sysinfo (SI_ARCHITECTURE, processor, parimaa_na processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      නම් (element == unknown)
        {
          static var_na processor[257];
          size_t s = parimaa_na processor;
          static පූර්ණ_සංඛ්‍යා mib[] = { CTL_HW, UNAME_PROCESSOR };
          නම් (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      නම් (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  නම් (toprint & PRINT_HARDWARE_PLATFORM)
    {
      var_na const *element = unknown;
#නම් HAVE_SYSINFO && defined SI_PLATFORM
      {
        static var_na hardware_platform[257];
        නම් (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, parimaa_na hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      නම් (element == unknown)
        {
          static var_na hardware_platform[257];
          size_t s = parimaa_na hardware_platform;
          static පූර්ණ_සංඛ්‍යා mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          නම් (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      නම් (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  නම් (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  නම් (!labeled)
    putchar ('\n');

  ආපසු EXIT_SUCCESS;
}
