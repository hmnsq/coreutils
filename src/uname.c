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

#إذا HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#إذا HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# إذا HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  وإلا
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
#define بغ_KERNEL_NAME 1

/* Node name on a communications network. */
#define بغ_NODENAME 2

/* Kernel release. */
#define بغ_KERNEL_RELEASE 4

/* Kernel version. */
#define بغ_KERNEL_VERSION 8

/* Machine hardware name. */
#define بغ_MACHINE 16

/* Processor type. */
#define بغ_PROCESSOR 32

/* Hardware platform.  */
#define بغ_HARDWARE_PLATFORM 64

/* Operating system.  */
#define بغ_OPERATING_SYSTEM 128

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
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      إذا (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
بغ certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                بغ_2 all information, in the following order,\n\
                             except omit -p and -i إذا unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        بغ_2 all information, one labeled field per لي,\n\
                             except omit -p and -i إذا unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        بغ_2 the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           بغ_2 the network كلي hostname\n\
"));
          oputs (_("\
  -r, --kernel-release     بغ_2 the kernel release\n\
"));
          oputs (_("\
  -v, --kernel-version     بغ_2 the kernel version\n\
"));
          oputs (_("\
  -m, --machine            بغ_2 the machine hardware name\n\
"));
          oputs (_("\
  -p, --processor          بغ_2 the processor type (non-portable)\n\
"));
          oputs (_("\
  -i, --hardware-platform  بغ_2 the hardware platform (non-portable)\n\
"));
          oputs (_("\
  -o, --operating-system   بغ_2 the operating system\n\
"));
        }
      وإلا
        {
          fputs (_("\
بغ machine architecture.\n\
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
بغ_element (حرف ثابت *element, حرف ثابت *label)
{
  إذا (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  وإلا
    {
      ساكن bool printed;
      إذا (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

ساكن فراغ
بغ_element_env (حرف ثابت *element,
                   MAYBE_UNUSED حرف ثابت *envvar,
                   حرف ثابت *label)
{
#ifdef __APPLE__
  إذا (envvar)
    {
      حرف ثابت *val = getenv (envvar);
      إذا (val)
        element = val;
    }
#endif
  بغ_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

ساكن صحيح
decode_switches (صحيح argc, حرف **argv)
{
  صحيح c;
  unsigned صحيح toprint = 0;

  إذا (uname_mode == UNAME_ARCH)
    {
      طالما ((c = getopt_long (argc, argv, "",
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
      toprint = بغ_MACHINE;
    }
  وإلا
    {
      طالما ((c = getopt_long (argc, argv, "asnrvmpioA",
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
              toprint |= بغ_KERNEL_NAME;
              اكسر;

            حالة 'n':
              toprint |= بغ_NODENAME;
              اكسر;

            حالة 'r':
              toprint |= بغ_KERNEL_RELEASE;
              اكسر;

            حالة 'v':
              toprint |= بغ_KERNEL_VERSION;
              اكسر;

            حالة 'm':
              toprint |= بغ_MACHINE;
              اكسر;

            حالة 'p':
              toprint |= بغ_PROCESSOR;
              اكسر;

            حالة 'i':
              toprint |= بغ_HARDWARE_PLATFORM;
              اكسر;

            حالة 'o':
              toprint |= بغ_OPERATING_SYSTEM;
              اكسر;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            افتراضي:
              usage (EXIT_FAILURE);
            }
        }
    }

  إذا (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  أرجع toprint;
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

  إذا (toprint == 0)
    toprint = بغ_KERNEL_NAME;

  إذا (toprint
       & (بغ_KERNEL_NAME | بغ_NODENAME | بغ_KERNEL_RELEASE
          | بغ_KERNEL_VERSION | بغ_MACHINE))
    {
      بنية utsname name;

      إذا (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      إذا (toprint & بغ_KERNEL_NAME)
        بغ_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      إذا (toprint & بغ_NODENAME)
        بغ_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      إذا (toprint & بغ_KERNEL_RELEASE)
        بغ_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      إذا (toprint & بغ_KERNEL_VERSION)
        بغ_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      إذا (toprint & بغ_MACHINE)
        بغ_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  إذا (toprint & بغ_PROCESSOR)
    {
      حرف ثابت *element = unknown;
#ifdef __APPLE__
# إذا defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#إذا HAVE_SYSINFO && defined SI_ARCHITECTURE
      إذا (element == unknown)
        {
          ساكن حرف processor[257];
          إذا (0 <= sysinfo (SI_ARCHITECTURE, processor, حجم processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      إذا (element == unknown)
        {
          ساكن حرف processor[257];
          size_t s = حجم processor;
          ساكن صحيح mib[] = { CTL_HW, UNAME_PROCESSOR };
          إذا (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      إذا (! (toprint == UINT_MAX && element == unknown))
        بغ_element (element, _("Processor"));
    }

  إذا (toprint & بغ_HARDWARE_PLATFORM)
    {
      حرف ثابت *element = unknown;
#إذا HAVE_SYSINFO && defined SI_PLATFORM
      {
        ساكن حرف hardware_platform[257];
        إذا (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, حجم hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      إذا (element == unknown)
        {
          ساكن حرف hardware_platform[257];
          size_t s = حجم hardware_platform;
          ساكن صحيح mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          إذا (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      إذا (! (toprint == UINT_MAX && element == unknown))
        بغ_element (element, _("Hardware platform"));
    }

  إذا (toprint & بغ_OPERATING_SYSTEM)
    بغ_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  إذا (!labeled)
    putchar ('\n');

  أرجع EXIT_SUCCESS;
}
