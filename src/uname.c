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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/utsname.h>
#അന്തര്ഭാവ <getopt.h>

#എങ്കിൽ HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#എങ്കിൽ HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# എങ്കിൽ HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  അല്ലെങ്കിൽ
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "uname.h"

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

സ്ഥിതം bool labeled = false;

സ്ഥിതം ഘടന option സ്ഥിരം uname_long_options[] =
{
  {"all", no_argument, ശൂന്യ, 'a'},
  {"all-labeled", no_argument, ശൂന്യ, 'A'},
  {"kernel-name", no_argument, ശൂന്യ, 's'},
  {"sysname", no_argument, ശൂന്യ, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, ശൂന്യ, 'n'},
  {"kernel-release", no_argument, ശൂന്യ, 'r'},
  {"release", no_argument, ശൂന്യ, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, ശൂന്യ, 'v'},
  {"machine", no_argument, ശൂന്യ, 'm'},
  {"processor", no_argument, ശൂന്യ, 'p'},
  {"hardware-platform", no_argument, ശൂന്യ, 'i'},
  {"operating-system", no_argument, ശൂന്യ, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

സ്ഥിതം ഘടന option സ്ഥിരം arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      എങ്കിൽ (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i എങ്കിൽ unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per വരി,\n\
                             except omit -p and -i എങ്കിൽ unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network കണ്ണി hostname\n\
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
      അല്ലെങ്കിൽ
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

സ്ഥിതം ശൂന്യം
print_element (അക്ഷരം സ്ഥിരം *element, അക്ഷരം സ്ഥിരം *label)
{
  എങ്കിൽ (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  അല്ലെങ്കിൽ
    {
      സ്ഥിതം bool printed;
      എങ്കിൽ (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

സ്ഥിതം ശൂന്യം
print_element_env (അക്ഷരം സ്ഥിരം *element,
                   MAYBE_UNUSED അക്ഷരം സ്ഥിരം *envvar,
                   അക്ഷരം സ്ഥിരം *label)
{
#ifdef __APPLE__
  എങ്കിൽ (envvar)
    {
      അക്ഷരം സ്ഥിരം *val = getenv (envvar);
      എങ്കിൽ (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
decode_switches (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ c;
  unsigned പൂർണ്ണസംഖ്യ toprint = 0;

  എങ്കിൽ (uname_mode == UNAME_ARCH)
    {
      ആകുമ്പോൾ ((c = getopt_long (argc, argv, "",
                               arch_long_options, ശൂന്യ))
             != -1)
        {
          തിരഞ്ഞെടുപ്പ് (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            സ്ഥിരസ്ഥിതി:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  അല്ലെങ്കിൽ
    {
      ആകുമ്പോൾ ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, ശൂന്യ))
             != -1)
        {
          തിരഞ്ഞെടുപ്പ് (c)
            {
            അവസ്ഥ 'a':
              toprint = UINT_MAX;
              labeled = false;
              നിർത്തുക;

            അവസ്ഥ 'A':
              toprint = UINT_MAX;
              labeled = true;
              നിർത്തുക;

            അവസ്ഥ 's':
              toprint |= PRINT_KERNEL_NAME;
              നിർത്തുക;

            അവസ്ഥ 'n':
              toprint |= PRINT_NODENAME;
              നിർത്തുക;

            അവസ്ഥ 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              നിർത്തുക;

            അവസ്ഥ 'v':
              toprint |= PRINT_KERNEL_VERSION;
              നിർത്തുക;

            അവസ്ഥ 'm':
              toprint |= PRINT_MACHINE;
              നിർത്തുക;

            അവസ്ഥ 'p':
              toprint |= PRINT_PROCESSOR;
              നിർത്തുക;

            അവസ്ഥ 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              നിർത്തുക;

            അവസ്ഥ 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              നിർത്തുക;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            സ്ഥിരസ്ഥിതി:
              usage (EXIT_FAILURE);
            }
        }
    }

  എങ്കിൽ (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  തിരികെ toprint;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  സ്ഥിതം അക്ഷരം സ്ഥിരം unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned പൂർണ്ണസംഖ്യ toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  എങ്കിൽ (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  എങ്കിൽ (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      ഘടന utsname name;

      എങ്കിൽ (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      എങ്കിൽ (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      എങ്കിൽ (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      എങ്കിൽ (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      എങ്കിൽ (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      എങ്കിൽ (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  എങ്കിൽ (toprint & PRINT_PROCESSOR)
    {
      അക്ഷരം സ്ഥിരം *element = unknown;
#ifdef __APPLE__
# എങ്കിൽ defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#എങ്കിൽ HAVE_SYSINFO && defined SI_ARCHITECTURE
      എങ്കിൽ (element == unknown)
        {
          സ്ഥിതം അക്ഷരം processor[257];
          എങ്കിൽ (0 <= sysinfo (SI_ARCHITECTURE, processor, വലുപ്പം processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      എങ്കിൽ (element == unknown)
        {
          സ്ഥിതം അക്ഷരം processor[257];
          size_t s = വലുപ്പം processor;
          സ്ഥിതം പൂർണ്ണസംഖ്യ mib[] = { CTL_HW, UNAME_PROCESSOR };
          എങ്കിൽ (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      എങ്കിൽ (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  എങ്കിൽ (toprint & PRINT_HARDWARE_PLATFORM)
    {
      അക്ഷരം സ്ഥിരം *element = unknown;
#എങ്കിൽ HAVE_SYSINFO && defined SI_PLATFORM
      {
        സ്ഥിതം അക്ഷരം hardware_platform[257];
        എങ്കിൽ (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, വലുപ്പം hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      എങ്കിൽ (element == unknown)
        {
          സ്ഥിതം അക്ഷരം hardware_platform[257];
          size_t s = വലുപ്പം hardware_platform;
          സ്ഥിതം പൂർണ്ണസംഖ്യ mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          എങ്കിൽ (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      എങ്കിൽ (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  എങ്കിൽ (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  എങ്കിൽ (!labeled)
    putchar ('\n');

  തിരികെ EXIT_SUCCESS;
}
