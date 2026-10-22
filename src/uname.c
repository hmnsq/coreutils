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

স্থির সংরচনা option ধ্রুব uname_long_options[] =
{
  {"all", no_argument, রিক্ত, 'a'},
  {"all-labeled", no_argument, রিক্ত, 'A'},
  {"kernel-name", no_argument, রিক্ত, 's'},
  {"sysname", no_argument, রিক্ত, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, রিক্ত, 'n'},
  {"kernel-release", no_argument, রিক্ত, 'r'},
  {"release", no_argument, রিক্ত, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, রিক্ত, 'v'},
  {"machine", no_argument, রিক্ত, 'm'},
  {"processor", no_argument, রিক্ত, 'p'},
  {"hardware-platform", no_argument, রিক্ত, 'i'},
  {"operating-system", no_argument, রিক্ত, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

স্থির সংরচনা option ধ্রুব arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
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
print_element (বর্ণ ধ্রুব *element, বর্ণ ধ্রুব *label)
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
print_element_env (বর্ণ ধ্রুব *element,
                   MAYBE_UNUSED বর্ণ ধ্রুব *envvar,
                   বর্ণ ধ্রুব *label)
{
#ifdef __APPLE__
  যদি (envvar)
    {
      বর্ণ ধ্রুব *val = getenv (envvar);
      যদি (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

স্থির পূর্ণাঙ্ক
decode_switches (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  পূর্ণাঙ্ক c;
  unsigned পূর্ণাঙ্ক toprint = 0;

  যদি (uname_mode == UNAME_ARCH)
    {
      যাবত্ ((c = getopt_long (argc, argv, "",
                               arch_long_options, রিক্ত))
             != -1)
        {
          বিকল্প (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            স্বভাব:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  অন্যথা
    {
      যাবত্ ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, রিক্ত))
             != -1)
        {
          বিকল্প (c)
            {
            পক্ষ 'a':
              toprint = UINT_MAX;
              labeled = false;
              ভঙ্গ;

            পক্ষ 'A':
              toprint = UINT_MAX;
              labeled = true;
              ভঙ্গ;

            পক্ষ 's':
              toprint |= PRINT_KERNEL_NAME;
              ভঙ্গ;

            পক্ষ 'n':
              toprint |= PRINT_NODENAME;
              ভঙ্গ;

            পক্ষ 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              ভঙ্গ;

            পক্ষ 'v':
              toprint |= PRINT_KERNEL_VERSION;
              ভঙ্গ;

            পক্ষ 'm':
              toprint |= PRINT_MACHINE;
              ভঙ্গ;

            পক্ষ 'p':
              toprint |= PRINT_PROCESSOR;
              ভঙ্গ;

            পক্ষ 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              ভঙ্গ;

            পক্ষ 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              ভঙ্গ;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            স্বভাব:
              usage (EXIT_FAILURE);
            }
        }
    }

  যদি (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  প্রত্যাগম toprint;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  স্থির বর্ণ ধ্রুব unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned পূর্ণাঙ্ক toprint = 0;

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
      সংরচনা utsname name;

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
      বর্ণ ধ্রুব *element = unknown;
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
          স্থির বর্ণ processor[257];
          যদি (0 <= sysinfo (SI_ARCHITECTURE, processor, পরিমাণ processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      যদি (element == unknown)
        {
          স্থির বর্ণ processor[257];
          size_t s = পরিমাণ processor;
          স্থির পূর্ণাঙ্ক mib[] = { CTL_HW, UNAME_PROCESSOR };
          যদি (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      যদি (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  যদি (toprint & PRINT_HARDWARE_PLATFORM)
    {
      বর্ণ ধ্রুব *element = unknown;
#যদি HAVE_SYSINFO && defined SI_PLATFORM
      {
        স্থির বর্ণ hardware_platform[257];
        যদি (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, পরিমাণ hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      যদি (element == unknown)
        {
          স্থির বর্ণ hardware_platform[257];
          size_t s = পরিমাণ hardware_platform;
          স্থির পূর্ণাঙ্ক mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
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

  প্রত্যাগম EXIT_SUCCESS;
}
