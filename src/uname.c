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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <sys/utsname.h>
#شامل <getopt.h>

#اگر HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#اگر_ختم

#اگر HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# اگر HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  ورنہ
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#اگر_ختم

#شامل "system.h"
#شامل "quote.h"
#شامل "uname.h"

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

ساکن bool labeled = false;

ساکن ساخت option ثابت uname_long_options[] =
{
  {"all", no_دلیل, تہی, 'a'},
  {"all-labeled", no_دلیل, تہی, 'A'},
  {"kernel-name", no_دلیل, تہی, 's'},
  {"sysname", no_دلیل, تہی, 's'},	/* Obsolescent.  */
  {"nodename", no_دلیل, تہی, 'n'},
  {"kernel-release", no_دلیل, تہی, 'r'},
  {"release", no_دلیل, تہی, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_دلیل, تہی, 'v'},
  {"machine", no_دلیل, تہی, 'm'},
  {"processor", no_دلیل, تہی, 'p'},
  {"hardware-platform", no_دلیل, تہی, 'i'},
  {"operating-system", no_دلیل, تہی, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

ساکن ساخت option ثابت arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]...\n"), پروگرام_name);

      اگر (uname_منوال == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, اندر the following ترتیب,\n\
                             except omit -p and -i اگر unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled میدان فی سطر,\n\
                             except omit -p and -i اگر unknown\n\
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
      ورنہ
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
  باہر_نکلو (حالت_2);
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  */

ساکن خالی
print_element (حرف ثابت *element, حرف ثابت *label)
{
  اگر (labeled)
    {
      لکھو ("%s: %s\n", label, element);
    }
  ورنہ
    {
      ساکن bool printed;
      اگر (printed)
        حرف_لکھو (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

ساکن خالی
print_element_env (حرف ثابت *element,
                   MAYBE_غیر_مستعمل حرف ثابت *envvar,
                   حرف ثابت *label)
{
#اگر_تعریف_شدہ __APPLE__
  اگر (envvar)
    {
      حرف ثابت *val = getenv (envvar);
      اگر (val)
        element = val;
    }
#اگر_ختم
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

ساکن صحیح
decode_switches (صحیح argc, حرف **argv)
{
  صحیح c;
  غیرعلامتی صحیح toprint = 0;

  اگر (uname_منوال == UNAME_ARCH)
    {
      جب ((c = getopt_long (argc, argv, "",
                               arch_long_options, تہی))
             != -1)
        {
          انتخاب (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_نسخہ_CHAR (پروگرام_NAME, ARCH_AUTHORS);

            طےشدہ:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  ورنہ
    {
      جب ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, تہی))
             != -1)
        {
          انتخاب (c)
            {
            حالت 'a':
              toprint = UINT_MAX;
              labeled = false;
              توڑو;

            حالت 'A':
              toprint = UINT_MAX;
              labeled = true;
              توڑو;

            حالت 's':
              toprint |= PRINT_KERNEL_NAME;
              توڑو;

            حالت 'n':
              toprint |= PRINT_NODENAME;
              توڑو;

            حالت 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              توڑو;

            حالت 'v':
              toprint |= PRINT_KERNEL_نسخہ;
              توڑو;

            حالت 'm':
              toprint |= PRINT_MACHINE;
              توڑو;

            حالت 'p':
              toprint |= PRINT_PROCESSOR;
              توڑو;

            حالت 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              توڑو;

            حالت 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              توڑو;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

            طےشدہ:
              usage (EXIT_FAILURE);
            }
        }
    }

  اگر (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  واپس toprint;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  ساکن حرف ثابت unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  غیرعلامتی صحیح toprint = 0;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  اگر (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  اگر (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_نسخہ | PRINT_MACHINE))
    {
      ساخت utsname name;

      اگر (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      اگر (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      اگر (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      اگر (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      اگر (toprint & PRINT_KERNEL_نسخہ)
        print_element_env (name.نسخہ_3, "UNAME_VERSION", _("Kernel version"));
      اگر (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  اگر (toprint & PRINT_PROCESSOR)
    {
      حرف ثابت *element = unknown;
#اگر_تعریف_شدہ __APPLE__
# اگر defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#اگر_ختم
#اگر HAVE_SYSINFO && defined SI_ARCHITECTURE
      اگر (element == unknown)
        {
          ساکن حرف processor[257];
          اگر (0 <= sysinfo (SI_ARCHITECTURE, processor, حجم processor))
            element = processor;
        }
#اگر_ختم
#اگر_تعریف_شدہ UNAME_PROCESSOR
      اگر (element == unknown)
        {
          ساکن حرف processor[257];
          size_t s = حجم processor;
          ساکن صحیح mib[] = { CTL_HW, UNAME_PROCESSOR };
          اگر (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#اگر_ختم
      اگر (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  اگر (toprint & PRINT_HARDWARE_PLATFORM)
    {
      حرف ثابت *element = unknown;
#اگر HAVE_SYSINFO && defined SI_PLATFORM
      {
        ساکن حرف hardware_platform[257];
        اگر (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, حجم hardware_platform))
          element = hardware_platform;
      }
#اگر_ختم
#اگر_تعریف_شدہ UNAME_HARDWARE_PLATFORM
      اگر (element == unknown)
        {
          ساکن حرف hardware_platform[257];
          size_t s = حجم hardware_platform;
          ساکن صحیح mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          اگر (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#اگر_ختم
      اگر (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  اگر (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  اگر (!labeled)
    حرف_لکھو ('\n');

  واپس EXIT_SUCCESS;
}
