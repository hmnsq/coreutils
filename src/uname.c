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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/utsname.h>
#અન્તર્ભાવ <getopt.h>

#જો HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#જો HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# જો HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  નહીંતર
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "uname.h"

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

સ્થિર bool labeled = false;

સ્થિર સંરચના option અચલ uname_long_options[] =
{
  {"all", no_argument, ખાલી, 'a'},
  {"all-labeled", no_argument, ખાલી, 'A'},
  {"kernel-name", no_argument, ખાલી, 's'},
  {"sysname", no_argument, ખાલી, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, ખાલી, 'n'},
  {"kernel-release", no_argument, ખાલી, 'r'},
  {"release", no_argument, ખાલી, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, ખાલી, 'v'},
  {"machine", no_argument, ખાલી, 'm'},
  {"processor", no_argument, ખાલી, 'p'},
  {"hardware-platform", no_argument, ખાલી, 'i'},
  {"operating-system", no_argument, ખાલી, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

સ્થિર સંરચના option અચલ arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      જો (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i જો unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per લીટી,\n\
                             except omit -p and -i જો unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network ગાંઠ hostname\n\
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
      નહીંતર
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

સ્થિર શૂન્ય
print_element (અક્ષર અચલ *element, અક્ષર અચલ *label)
{
  જો (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  નહીંતર
    {
      સ્થિર bool printed;
      જો (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

સ્થિર શૂન્ય
print_element_env (અક્ષર અચલ *element,
                   MAYBE_UNUSED અક્ષર અચલ *envvar,
                   અક્ષર અચલ *label)
{
#ifdef __APPLE__
  જો (envvar)
    {
      અક્ષર અચલ *val = getenv (envvar);
      જો (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

સ્થિર પૂર્ણાંક
decode_switches (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક c;
  unsigned પૂર્ણાંક toprint = 0;

  જો (uname_mode == UNAME_ARCH)
    {
      જ્યારે ((c = getopt_long (argc, argv, "",
                               arch_long_options, ખાલી))
             != -1)
        {
          પસંદગી (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            મૂળભૂત:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  નહીંતર
    {
      જ્યારે ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, ખાલી))
             != -1)
        {
          પસંદગી (c)
            {
            સ્થિતિ 'a':
              toprint = UINT_MAX;
              labeled = false;
              તોડો;

            સ્થિતિ 'A':
              toprint = UINT_MAX;
              labeled = true;
              તોડો;

            સ્થિતિ 's':
              toprint |= PRINT_KERNEL_NAME;
              તોડો;

            સ્થિતિ 'n':
              toprint |= PRINT_NODENAME;
              તોડો;

            સ્થિતિ 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              તોડો;

            સ્થિતિ 'v':
              toprint |= PRINT_KERNEL_VERSION;
              તોડો;

            સ્થિતિ 'm':
              toprint |= PRINT_MACHINE;
              તોડો;

            સ્થિતિ 'p':
              toprint |= PRINT_PROCESSOR;
              તોડો;

            સ્થિતિ 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              તોડો;

            સ્થિતિ 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              તોડો;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            મૂળભૂત:
              usage (EXIT_FAILURE);
            }
        }
    }

  જો (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  પાછા toprint;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  સ્થિર અક્ષર અચલ unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned પૂર્ણાંક toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  જો (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  જો (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      સંરચના utsname name;

      જો (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      જો (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      જો (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      જો (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      જો (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      જો (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  જો (toprint & PRINT_PROCESSOR)
    {
      અક્ષર અચલ *element = unknown;
#ifdef __APPLE__
# જો defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#જો HAVE_SYSINFO && defined SI_ARCHITECTURE
      જો (element == unknown)
        {
          સ્થિર અક્ષર processor[257];
          જો (0 <= sysinfo (SI_ARCHITECTURE, processor, કદ processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      જો (element == unknown)
        {
          સ્થિર અક્ષર processor[257];
          size_t s = કદ processor;
          સ્થિર પૂર્ણાંક mib[] = { CTL_HW, UNAME_PROCESSOR };
          જો (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      જો (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  જો (toprint & PRINT_HARDWARE_PLATFORM)
    {
      અક્ષર અચલ *element = unknown;
#જો HAVE_SYSINFO && defined SI_PLATFORM
      {
        સ્થિર અક્ષર hardware_platform[257];
        જો (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, કદ hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      જો (element == unknown)
        {
          સ્થિર અક્ષર hardware_platform[257];
          size_t s = કદ hardware_platform;
          સ્થિર પૂર્ણાંક mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          જો (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      જો (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  જો (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  જો (!labeled)
    putchar ('\n');

  પાછા EXIT_SUCCESS;
}
