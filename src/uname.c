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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/utsname.h>
#অন্তর্ভাব <getopt.h>

#যদি HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#যদি HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# যদি HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  অন্যথা
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "uname.h"

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

স্থির bool labeled = false;

স্থির কাঠামো option ধ্রুবক uname_long_options[] =
{
  {"all", no_argument, ফাঁকা, 'a'},
  {"all-labeled", no_argument, ফাঁকা, 'A'},
  {"kernel-name", no_argument, ফাঁকা, 's'},
  {"sysname", no_argument, ফাঁকা, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, ফাঁকা, 'n'},
  {"kernel-release", no_argument, ফাঁকা, 'r'},
  {"release", no_argument, ফাঁকা, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, ফাঁকা, 'v'},
  {"machine", no_argument, ফাঁকা, 'm'},
  {"processor", no_argument, ফাঁকা, 'p'},
  {"hardware-platform", no_argument, ফাঁকা, 'i'},
  {"operating-system", no_argument, ফাঁকা, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

স্থির কাঠামো option ধ্রুবক arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      যদি (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i যদি unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per সারি,\n\
                             except omit -p and -i যদি unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network গ্রন্থি hostname\n\
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
      অন্যথা
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

স্থির শূন্য
print_element (অক্ষর ধ্রুবক *element, অক্ষর ধ্রুবক *label)
{
  যদি (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  অন্যথা
    {
      স্থির bool printed;
      যদি (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

স্থির শূন্য
print_element_env (অক্ষর ধ্রুবক *element,
                   MAYBE_UNUSED অক্ষর ধ্রুবক *envvar,
                   অক্ষর ধ্রুবক *label)
{
#ifdef __APPLE__
  যদি (envvar)
    {
      অক্ষর ধ্রুবক *val = getenv (envvar);
      যদি (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

স্থির পূর্ণসংখ্যা
decode_switches (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা c;
  unsigned পূর্ণসংখ্যা toprint = 0;

  যদি (uname_mode == UNAME_ARCH)
    {
      যখন ((c = getopt_long (argc, argv, "",
                               arch_long_options, ফাঁকা))
             != -1)
        {
          নির্বাচন (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            পূর্বনির্ধারিত:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  অন্যথা
    {
      যখন ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, ফাঁকা))
             != -1)
        {
          নির্বাচন (c)
            {
            ক্ষেত্র 'a':
              toprint = UINT_MAX;
              labeled = false;
              বিরতি;

            ক্ষেত্র 'A':
              toprint = UINT_MAX;
              labeled = true;
              বিরতি;

            ক্ষেত্র 's':
              toprint |= PRINT_KERNEL_NAME;
              বিরতি;

            ক্ষেত্র 'n':
              toprint |= PRINT_NODENAME;
              বিরতি;

            ক্ষেত্র 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              বিরতি;

            ক্ষেত্র 'v':
              toprint |= PRINT_KERNEL_VERSION;
              বিরতি;

            ক্ষেত্র 'm':
              toprint |= PRINT_MACHINE;
              বিরতি;

            ক্ষেত্র 'p':
              toprint |= PRINT_PROCESSOR;
              বিরতি;

            ক্ষেত্র 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              বিরতি;

            ক্ষেত্র 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              বিরতি;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            পূর্বনির্ধারিত:
              usage (EXIT_FAILURE);
            }
        }
    }

  যদি (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  ফেরত toprint;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  স্থির অক্ষর ধ্রুবক unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned পূর্ণসংখ্যা toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  যদি (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  যদি (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      কাঠামো utsname name;

      যদি (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      যদি (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      যদি (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      যদি (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      যদি (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      যদি (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  যদি (toprint & PRINT_PROCESSOR)
    {
      অক্ষর ধ্রুবক *element = unknown;
#ifdef __APPLE__
# যদি defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#যদি HAVE_SYSINFO && defined SI_ARCHITECTURE
      যদি (element == unknown)
        {
          স্থির অক্ষর processor[257];
          যদি (0 <= sysinfo (SI_ARCHITECTURE, processor, আকার processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      যদি (element == unknown)
        {
          স্থির অক্ষর processor[257];
          size_t s = আকার processor;
          স্থির পূর্ণসংখ্যা mib[] = { CTL_HW, UNAME_PROCESSOR };
          যদি (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      যদি (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  যদি (toprint & PRINT_HARDWARE_PLATFORM)
    {
      অক্ষর ধ্রুবক *element = unknown;
#যদি HAVE_SYSINFO && defined SI_PLATFORM
      {
        স্থির অক্ষর hardware_platform[257];
        যদি (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, আকার hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      যদি (element == unknown)
        {
          স্থির অক্ষর hardware_platform[257];
          size_t s = আকার hardware_platform;
          স্থির পূর্ণসংখ্যা mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          যদি (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      যদি (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  যদি (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  যদি (!labeled)
    putchar ('\n');

  ফেরত EXIT_SUCCESS;
}
