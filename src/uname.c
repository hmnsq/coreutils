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

#ಆದರೆ HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# ಆದರೆ HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  ಇಲ್ಲದಿದ್ದರೆ
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "uname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME (uname_ಬಹುಲಕ == UNAME_UNAME ? "uname" : "arch")

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")
#ವ್ಯಾಖ್ಯಾನಿಸು ARCH_AUTHORS "David MacKenzie", "Karel Zak"

/* Values that are bitwise or'd into 'toprint'. */
/* Kernel name. */
#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_KERNEL_NAME 1

/* Node name on a communications network. */
#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_NODENAME 2

/* Kernel release. */
#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_KERNEL_RELEASE 4

/* Kernel version. */
#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_KERNEL_ಆವೃತ್ತಿ 8

/* Machine hardware name. */
#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_MACHINE 16

/* Processor type. */
#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_PROCESSOR 32

/* Hardware platform.  */
#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_HARDWARE_PLATFORM 64

/* Operating system.  */
#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_OPERATING_SYSTEM 128

ಸ್ಥಿರ bool labeled = false;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ uname_long_options[] =
{
  {"all", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"all-labeled", no_ವಾದ, ಖಾಲಿ, 'A'},
  {"kernel-name", no_ವಾದ, ಖಾಲಿ, 's'},
  {"sysname", no_ವಾದ, ಖಾಲಿ, 's'},	/* Obsolescent.  */
  {"nodename", no_ವಾದ, ಖಾಲಿ, 'n'},
  {"kernel-release", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"release", no_ವಾದ, ಖಾಲಿ, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_ವಾದ, ಖಾಲಿ, 'v'},
  {"machine", no_ವಾದ, ಖಾಲಿ, 'm'},
  {"processor", no_ವಾದ, ಖಾಲಿ, 'p'},
  {"hardware-platform", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"operating-system", no_ವಾದ, ಖಾಲಿ, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]...\n"), ಕಾರ್ಯಕ್ರಮ_name);

      ಆದರೆ (uname_ಬಹುಲಕ == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, ಒಳ the following ಕ್ರಮ,\n\
                             except omit -p and -i ಆದರೆ unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled ಕ್ಷೇತ್ರ ಪ್ರತಿ ಸಾಲು,\n\
                             except omit -p and -i ಆದರೆ unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the ಜಾಲ ಗಂಟು hostname\n\
"));
          oputs (_("\
  -r, --kernel-release     print the kernel release\n\
"));
          oputs (_("\
  -v, --kernel-ಆವೃತ್ತಿ_3     print the kernel ಆವೃತ್ತಿ_3\n\
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
      ಇಲ್ಲದಿದ್ದರೆ
        {
          fputs (_("\
Print machine architecture.\n\
\n\
"), stdout);
        }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_element (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *element, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *label)
{
  ಆದರೆ (labeled)
    {
      ಬರೆ ("%s: %s\n", label, element);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರ bool printed;
      ಆದರೆ (printed)
        ಅಕ್ಷರ_ಬರೆ (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_element_env (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *element,
                   MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *envvar,
                   ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *label)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
  ಆದರೆ (envvar)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *val = getenv (envvar);
      ಆದರೆ (val)
        element = val;
    }
#ಆದರೆ_ಅಂತ್ಯ
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
decode_switches (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ c;
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ toprint = 0;

  ಆದರೆ (uname_ಬಹುಲಕ == UNAME_ARCH)
    {
      ಆಗ ((c = getopt_long (argc, argv, "",
                               arch_long_options, ಖಾಲಿ))
             != -1)
        {
          ಆಯ್ಕೆ (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, ARCH_AUTHORS);

            ಪೂರ್ವನಿಯೋಜಿತ:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆಗ ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, ಖಾಲಿ))
             != -1)
        {
          ಆಯ್ಕೆ (c)
            {
            ಸ್ಥಿತಿ 'a':
              toprint = UINT_MAX;
              labeled = false;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'A':
              toprint = UINT_MAX;
              labeled = true;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 's':
              toprint |= PRINT_KERNEL_NAME;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'n':
              toprint |= PRINT_NODENAME;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'v':
              toprint |= PRINT_KERNEL_ಆವೃತ್ತಿ;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'm':
              toprint |= PRINT_MACHINE;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'p':
              toprint |= PRINT_PROCESSOR;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              ನಿಲ್ಲಿಸು;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

            ಪೂರ್ವನಿಯೋಜಿತ:
              usage (EXIT_FAILURE);
            }
        }
    }

  ಆದರೆ (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  ಹಿಂತಿರುಗಿ toprint;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ toprint = 0;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  ಆದರೆ (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  ಆದರೆ (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_ಆವೃತ್ತಿ | PRINT_MACHINE))
    {
      ರಚನೆ utsname name;

      ಆದರೆ (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      ಆದರೆ (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      ಆದರೆ (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      ಆದರೆ (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      ಆದರೆ (toprint & PRINT_KERNEL_ಆವೃತ್ತಿ)
        print_element_env (name.ಆವೃತ್ತಿ_3, "UNAME_VERSION", _("Kernel version"));
      ಆದರೆ (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  ಆದರೆ (toprint & PRINT_PROCESSOR)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *element = unknown;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
# ಆದರೆ defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYSINFO && defined SI_ARCHITECTURE
      ಆದರೆ (element == unknown)
        {
          ಸ್ಥಿರ ಅಕ್ಷರ processor[257];
          ಆದರೆ (0 <= sysinfo (SI_ARCHITECTURE, processor, ಗಾತ್ರ processor))
            element = processor;
        }
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ UNAME_PROCESSOR
      ಆದರೆ (element == unknown)
        {
          ಸ್ಥಿರ ಅಕ್ಷರ processor[257];
          size_t s = ಗಾತ್ರ processor;
          ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mib[] = { CTL_HW, UNAME_PROCESSOR };
          ಆದರೆ (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#ಆದರೆ_ಅಂತ್ಯ
      ಆದರೆ (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  ಆದರೆ (toprint & PRINT_HARDWARE_PLATFORM)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *element = unknown;
#ಆದರೆ HAVE_SYSINFO && defined SI_PLATFORM
      {
        ಸ್ಥಿರ ಅಕ್ಷರ hardware_platform[257];
        ಆದರೆ (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, ಗಾತ್ರ hardware_platform))
          element = hardware_platform;
      }
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ UNAME_HARDWARE_PLATFORM
      ಆದರೆ (element == unknown)
        {
          ಸ್ಥಿರ ಅಕ್ಷರ hardware_platform[257];
          size_t s = ಗಾತ್ರ hardware_platform;
          ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          ಆದರೆ (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#ಆದರೆ_ಅಂತ್ಯ
      ಆದರೆ (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  ಆದರೆ (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  ಆದರೆ (!labeled)
    ಅಕ್ಷರ_ಬರೆ ('\n');

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
