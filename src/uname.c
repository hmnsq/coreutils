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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <sys/utsname.h>
#ضمن <getopt.h>

#אם HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#אם HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# אם HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  אחרת
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#ضمن "system.h"
#ضمن "quote.h"
#ضمن "uname.h"

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

ساكن bool labeled = false;

ساكن بنية option ثابت uname_long_options[] =
{
  {"all", no_argument, عدم, 'a'},
  {"all-labeled", no_argument, عدم, 'A'},
  {"kernel-name", no_argument, عدم, 's'},
  {"sysname", no_argument, عدم, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, عدم, 'n'},
  {"kernel-release", no_argument, عدم, 'r'},
  {"release", no_argument, عدم, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, عدم, 'v'},
  {"machine", no_argument, عدم, 'm'},
  {"processor", no_argument, عدم, 'p'},
  {"hardware-platform", no_argument, عدم, 'i'},
  {"operating-system", no_argument, عدم, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن بنية option ثابت arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  אם (status != EXIT_SUCCESS)
    emit_try_help ();
  אחרת
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      אם (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i אם unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per line,\n\
                             except omit -p and -i אם unknown\n\
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
      אחרת
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

ساكن فراغ
print_element (حرف ثابت *element, حرف ثابت *label)
{
  אם (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  אחרת
    {
      ساكن bool printed;
      אם (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

ساكن فراغ
print_element_env (حرف ثابت *element,
                   MAYBE_UNUSED حرف ثابت *envvar,
                   حرف ثابت *label)
{
#ifdef __APPLE__
  אם (envvar)
    {
      حرف ثابت *val = getenv (envvar);
      אם (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

ساكن صحيح
decode_switches (صحيح argc, حرف **argv)
{
  صحيح c;
  unsigned صحيح toprint = 0;

  אם (uname_mode == UNAME_ARCH)
    {
      כלעוד ((c = getopt_long (argc, argv, "",
                               arch_long_options, عدم))
             != -1)
        {
          بدل (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            افتراضي:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  אחרת
    {
      כלעוד ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, عدم))
             != -1)
        {
          بدل (c)
            {
            حالة 'a':
              toprint = UINT_MAX;
              labeled = false;
              اكسر;

            حالة 'A':
              toprint = UINT_MAX;
              labeled = true;
              اكسر;

            حالة 's':
              toprint |= PRINT_KERNEL_NAME;
              اكسر;

            حالة 'n':
              toprint |= PRINT_NODENAME;
              اكسر;

            حالة 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              اكسر;

            حالة 'v':
              toprint |= PRINT_KERNEL_VERSION;
              اكسر;

            حالة 'm':
              toprint |= PRINT_MACHINE;
              اكسر;

            حالة 'p':
              toprint |= PRINT_PROCESSOR;
              اكسر;

            حالة 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              اكسر;

            حالة 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              اكسر;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            افتراضي:
              usage (EXIT_FAILURE);
            }
        }
    }

  אם (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  החזר toprint;
}

صحيح
main (صحيح argc, حرف **argv)
{
  ساكن حرف ثابت unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned صحيح toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  אם (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  אם (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      بنية utsname name;

      אם (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      אם (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      אם (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      אם (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      אם (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      אם (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  אם (toprint & PRINT_PROCESSOR)
    {
      حرف ثابت *element = unknown;
#ifdef __APPLE__
# אם defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#אם HAVE_SYSINFO && defined SI_ARCHITECTURE
      אם (element == unknown)
        {
          ساكن حرف processor[257];
          אם (0 <= sysinfo (SI_ARCHITECTURE, processor, حجم processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      אם (element == unknown)
        {
          ساكن حرف processor[257];
          size_t s = حجم processor;
          ساكن صحيح mib[] = { CTL_HW, UNAME_PROCESSOR };
          אם (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      אם (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  אם (toprint & PRINT_HARDWARE_PLATFORM)
    {
      حرف ثابت *element = unknown;
#אם HAVE_SYSINFO && defined SI_PLATFORM
      {
        ساكن حرف hardware_platform[257];
        אם (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, حجم hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      אם (element == unknown)
        {
          ساكن حرف hardware_platform[257];
          size_t s = حجم hardware_platform;
          ساكن صحيح mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          אם (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      אם (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  אם (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  אם (!labeled)
    putchar ('\n');

  החזר EXIT_SUCCESS;
}
