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
#جے_انت

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
#جے_انت

#ضمن "system.h"
#ضمن "quote.h"
#ضمن "uname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME (uname_منوال == UNAME_UNAME ? "uname" : "arch")

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")
#تعریف_کرو ARCH_AUTHORS "David MacKenzie", "Karel Zak"

/* Values that are bitwise or'd into 'toprint'. */
/* Kernel name. */
#تعریف_کرو PRINT_KERNEL_NAME 1

/* Node name on a communications network. */
#تعریف_کرو PRINT_NODENAME 2

/* Kernel release. */
#تعریف_کرو PRINT_KERNEL_RELEASE 4

/* Kernel version. */
#تعریف_کرو PRINT_KERNEL_نسخہ 8

/* Machine hardware name. */
#تعریف_کرو PRINT_MACHINE 16

/* Processor type. */
#تعریف_کرو PRINT_PROCESSOR 32

/* Hardware platform.  */
#تعریف_کرو PRINT_HARDWARE_PLATFORM 64

/* Operating system.  */
#تعریف_کرو PRINT_OPERATING_SYSTEM 128

ساكن bool labeled = false;

ساكن بنية option ثابت uname_long_options[] =
{
  {"all", no_دلیل, عدم, 'a'},
  {"all-labeled", no_دلیل, عدم, 'A'},
  {"kernel-name", no_دلیل, عدم, 's'},
  {"sysname", no_دلیل, عدم, 's'},	/* Obsolescent.  */
  {"nodename", no_دلیل, عدم, 'n'},
  {"kernel-release", no_دلیل, عدم, 'r'},
  {"release", no_دلیل, عدم, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_دلیل, عدم, 'v'},
  {"machine", no_دلیل, عدم, 'm'},
  {"processor", no_دلیل, عدم, 'p'},
  {"hardware-platform", no_دلیل, عدم, 'i'},
  {"operating-system", no_دلیل, عدم, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن بنية option ثابت arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]...\n"), پروگرام_name);

      إذا (uname_منوال == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, وچ the following ترتیب,\n\
                             except omit -p and -i إذا unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled میدان فی سطر,\n\
                             except omit -p and -i إذا unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the جال گرہ hostname\n\
"));
          oputs (_("\
  -r, --kernel-release     print the kernel release\n\
"));
          oputs (_("\
  -v, --kernel-نسخہ_3     print the kernel نسخہ_3\n\
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
      وإلا
        {
          fputs (_("\
Print machine architecture.\n\
\n\
"), stdout);
        }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  */

ساكن فراغ
print_element (حرف ثابت *element, حرف ثابت *label)
{
  إذا (labeled)
    {
      لکھو ("%s: %s\n", label, element);
    }
  وإلا
    {
      ساكن bool printed;
      إذا (printed)
        حرف_لکھو (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

ساكن فراغ
print_element_env (حرف ثابت *element,
                   MAYBE_بن_ورتیا حرف ثابت *envvar,
                   حرف ثابت *label)
{
#جے_تعریف_شدہ __APPLE__
  إذا (envvar)
    {
      حرف ثابت *val = getenv (envvar);
      إذا (val)
        element = val;
    }
#جے_انت
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

ساكن صحيح
decode_switches (صحيح argc, حرف **argv)
{
  صحيح c;
  بنانشانی صحيح toprint = 0;

  إذا (uname_منوال == UNAME_ARCH)
    {
      طالما ((c = getopt_long (argc, argv, "",
                               arch_long_options, عدم))
             != -1)
        {
          بدل (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_نسخہ_CHAR (پروگرام_NAME, ARCH_AUTHORS);

            افتراضي:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
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
              toprint |= PRINT_KERNEL_NAME;
              اكسر;

            حالة 'n':
              toprint |= PRINT_NODENAME;
              اكسر;

            حالة 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              اكسر;

            حالة 'v':
              toprint |= PRINT_KERNEL_نسخہ;
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

            case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

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
مکھ (صحيح argc, حرف **argv)
{
  ساكن حرف ثابت unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  بنانشانی صحيح toprint = 0;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  إذا (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  إذا (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_نسخہ | PRINT_MACHINE))
    {
      بنية utsname name;

      إذا (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      إذا (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      إذا (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      إذا (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      إذا (toprint & PRINT_KERNEL_نسخہ)
        print_element_env (name.نسخہ_3, "UNAME_VERSION", _("Kernel version"));
      إذا (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  إذا (toprint & PRINT_PROCESSOR)
    {
      حرف ثابت *element = unknown;
#جے_تعریف_شدہ __APPLE__
# إذا defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#جے_انت
#إذا HAVE_SYSINFO && defined SI_ARCHITECTURE
      إذا (element == unknown)
        {
          ساكن حرف processor[257];
          إذا (0 <= sysinfo (SI_ARCHITECTURE, processor, حجم processor))
            element = processor;
        }
#جے_انت
#جے_تعریف_شدہ UNAME_PROCESSOR
      إذا (element == unknown)
        {
          ساكن حرف processor[257];
          size_t s = حجم processor;
          ساكن صحيح mib[] = { CTL_HW, UNAME_PROCESSOR };
          إذا (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#جے_انت
      إذا (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  إذا (toprint & PRINT_HARDWARE_PLATFORM)
    {
      حرف ثابت *element = unknown;
#إذا HAVE_SYSINFO && defined SI_PLATFORM
      {
        ساكن حرف hardware_platform[257];
        إذا (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, حجم hardware_platform))
          element = hardware_platform;
      }
#جے_انت
#جے_تعریف_شدہ UNAME_HARDWARE_PLATFORM
      إذا (element == unknown)
        {
          ساكن حرف hardware_platform[257];
          size_t s = حجم hardware_platform;
          ساكن صحيح mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          إذا (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#جے_انت
      إذا (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  إذا (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  إذا (!labeled)
    حرف_لکھو ('\n');

  أرجع EXIT_SUCCESS;
}
