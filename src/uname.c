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
#endif

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
#  وگرنه
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#شامل "system.h"
#شامل "quote.h"
#شامل "uname.h"

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

ایستا bool labeled = false;

ایستا ساختار option ثابت uname_long_options[] =
{
  {"all", no_argument, پوچ, 'a'},
  {"all-labeled", no_argument, پوچ, 'A'},
  {"kernel-name", no_argument, پوچ, 's'},
  {"sysname", no_argument, پوچ, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, پوچ, 'n'},
  {"kernel-release", no_argument, پوچ, 'r'},
  {"release", no_argument, پوچ, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, پوچ, 'v'},
  {"machine", no_argument, پوچ, 'm'},
  {"processor", no_argument, پوچ, 'p'},
  {"hardware-platform", no_argument, پوچ, 'i'},
  {"operating-system", no_argument, پوچ, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

ایستا ساختار option ثابت arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      اگر (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i اگر unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per سطر,\n\
                             except omit -p and -i اگر unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network گره hostname\n\
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
      وگرنه
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

ایستا تهی
print_element (نویسه ثابت *element, نویسه ثابت *label)
{
  اگر (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  وگرنه
    {
      ایستا bool printed;
      اگر (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

ایستا تهی
print_element_env (نویسه ثابت *element,
                   MAYBE_UNUSED نویسه ثابت *envvar,
                   نویسه ثابت *label)
{
#ifdef __APPLE__
  اگر (envvar)
    {
      نویسه ثابت *val = getenv (envvar);
      اگر (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

ایستا صحیح
decode_switches (صحیح argc, نویسه **argv)
{
  صحیح c;
  unsigned صحیح toprint = 0;

  اگر (uname_mode == UNAME_ARCH)
    {
      وقتی ((c = getopt_long (argc, argv, "",
                               arch_long_options, پوچ))
             != -1)
        {
          انتخاب (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            پیشفرض:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  وگرنه
    {
      وقتی ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, پوچ))
             != -1)
        {
          انتخاب (c)
            {
            حالت 'a':
              toprint = UINT_MAX;
              labeled = false;
              بشکن;

            حالت 'A':
              toprint = UINT_MAX;
              labeled = true;
              بشکن;

            حالت 's':
              toprint |= PRINT_KERNEL_NAME;
              بشکن;

            حالت 'n':
              toprint |= PRINT_NODENAME;
              بشکن;

            حالت 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              بشکن;

            حالت 'v':
              toprint |= PRINT_KERNEL_VERSION;
              بشکن;

            حالت 'm':
              toprint |= PRINT_MACHINE;
              بشکن;

            حالت 'p':
              toprint |= PRINT_PROCESSOR;
              بشکن;

            حالت 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              بشکن;

            حالت 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              بشکن;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            پیشفرض:
              usage (EXIT_FAILURE);
            }
        }
    }

  اگر (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  بازگشت toprint;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  ایستا نویسه ثابت unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned صحیح toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  اگر (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  اگر (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      ساختار utsname name;

      اگر (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      اگر (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      اگر (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      اگر (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      اگر (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      اگر (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  اگر (toprint & PRINT_PROCESSOR)
    {
      نویسه ثابت *element = unknown;
#ifdef __APPLE__
# اگر defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#اگر HAVE_SYSINFO && defined SI_ARCHITECTURE
      اگر (element == unknown)
        {
          ایستا نویسه processor[257];
          اگر (0 <= sysinfo (SI_ARCHITECTURE, processor, اندازه processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      اگر (element == unknown)
        {
          ایستا نویسه processor[257];
          size_t s = اندازه processor;
          ایستا صحیح mib[] = { CTL_HW, UNAME_PROCESSOR };
          اگر (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      اگر (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  اگر (toprint & PRINT_HARDWARE_PLATFORM)
    {
      نویسه ثابت *element = unknown;
#اگر HAVE_SYSINFO && defined SI_PLATFORM
      {
        ایستا نویسه hardware_platform[257];
        اگر (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, اندازه hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      اگر (element == unknown)
        {
          ایستا نویسه hardware_platform[257];
          size_t s = اندازه hardware_platform;
          ایستا صحیح mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          اگر (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      اگر (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  اگر (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  اگر (!labeled)
    putchar ('\n');

  بازگشت EXIT_SUCCESS;
}
