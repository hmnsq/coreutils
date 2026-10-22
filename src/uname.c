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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/utsname.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಯದಿ HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#ಯದಿ HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# ಯದಿ HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  ಅನ್ಯಥಾ
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "uname.h"

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

ಸ್ಥಿರ bool labeled = false;

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ uname_long_options[] =
{
  {"all", no_argument, ರಿಕ್ತ, 'a'},
  {"all-labeled", no_argument, ರಿಕ್ತ, 'A'},
  {"kernel-name", no_argument, ರಿಕ್ತ, 's'},
  {"sysname", no_argument, ರಿಕ್ತ, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, ರಿಕ್ತ, 'n'},
  {"kernel-release", no_argument, ರಿಕ್ತ, 'r'},
  {"release", no_argument, ರಿಕ್ತ, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, ರಿಕ್ತ, 'v'},
  {"machine", no_argument, ರಿಕ್ತ, 'm'},
  {"processor", no_argument, ರಿಕ್ತ, 'p'},
  {"hardware-platform", no_argument, ರಿಕ್ತ, 'i'},
  {"operating-system", no_argument, ರಿಕ್ತ, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      ಯದಿ (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i ಯದಿ unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per ಸಾಲು,\n\
                             except omit -p and -i ಯದಿ unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network ಗಂಟು hostname\n\
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
      ಅನ್ಯಥಾ
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

ಸ್ಥಿರ ಶೂನ್ಯ
print_element (ವರ್ಣ ಧ್ರುವ *element, ವರ್ಣ ಧ್ರುವ *label)
{
  ಯದಿ (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  ಅನ್ಯಥಾ
    {
      ಸ್ಥಿರ bool printed;
      ಯದಿ (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_element_env (ವರ್ಣ ಧ್ರುವ *element,
                   MAYBE_UNUSED ವರ್ಣ ಧ್ರುವ *envvar,
                   ವರ್ಣ ಧ್ರುವ *label)
{
#ifdef __APPLE__
  ಯದಿ (envvar)
    {
      ವರ್ಣ ಧ್ರುವ *val = getenv (envvar);
      ಯದಿ (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
decode_switches (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ c;
  unsigned ಪೂರ್ಣಾಙ್ಕ toprint = 0;

  ಯದಿ (uname_mode == UNAME_ARCH)
    {
      ಯಾವತ್ ((c = getopt_long (argc, argv, "",
                               arch_long_options, ರಿಕ್ತ))
             != -1)
        {
          ವಿಕಲ್ಪ (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            ಸ್ವಭಾವ:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  ಅನ್ಯಥಾ
    {
      ಯಾವತ್ ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, ರಿಕ್ತ))
             != -1)
        {
          ವಿಕಲ್ಪ (c)
            {
            ಪಕ್ಷ 'a':
              toprint = UINT_MAX;
              labeled = false;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'A':
              toprint = UINT_MAX;
              labeled = true;
              ಭಙ್ಗ;

            ಪಕ್ಷ 's':
              toprint |= PRINT_KERNEL_NAME;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'n':
              toprint |= PRINT_NODENAME;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'v':
              toprint |= PRINT_KERNEL_VERSION;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'm':
              toprint |= PRINT_MACHINE;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'p':
              toprint |= PRINT_PROCESSOR;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              ಭಙ್ಗ;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            ಸ್ವಭಾವ:
              usage (EXIT_FAILURE);
            }
        }
    }

  ಯದಿ (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  ಪ್ರತ್ಯಾಗಮ toprint;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned ಪೂರ್ಣಾಙ್ಕ toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  ಯದಿ (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  ಯದಿ (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      ಸಂರಚನಾ utsname name;

      ಯದಿ (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      ಯದಿ (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      ಯದಿ (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      ಯದಿ (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      ಯದಿ (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      ಯದಿ (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  ಯದಿ (toprint & PRINT_PROCESSOR)
    {
      ವರ್ಣ ಧ್ರುವ *element = unknown;
#ifdef __APPLE__
# ಯದಿ defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#ಯದಿ HAVE_SYSINFO && defined SI_ARCHITECTURE
      ಯದಿ (element == unknown)
        {
          ಸ್ಥಿರ ವರ್ಣ processor[257];
          ಯದಿ (0 <= sysinfo (SI_ARCHITECTURE, processor, ಪರಿಮಾಣ processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      ಯದಿ (element == unknown)
        {
          ಸ್ಥಿರ ವರ್ಣ processor[257];
          size_t s = ಪರಿಮಾಣ processor;
          ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mib[] = { CTL_HW, UNAME_PROCESSOR };
          ಯದಿ (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      ಯದಿ (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  ಯದಿ (toprint & PRINT_HARDWARE_PLATFORM)
    {
      ವರ್ಣ ಧ್ರುವ *element = unknown;
#ಯದಿ HAVE_SYSINFO && defined SI_PLATFORM
      {
        ಸ್ಥಿರ ವರ್ಣ hardware_platform[257];
        ಯದಿ (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, ಪರಿಮಾಣ hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      ಯದಿ (element == unknown)
        {
          ಸ್ಥಿರ ವರ್ಣ hardware_platform[257];
          size_t s = ಪರಿಮಾಣ hardware_platform;
          ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          ಯದಿ (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      ಯದಿ (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  ಯದಿ (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  ಯದಿ (!labeled)
    putchar ('\n');

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
