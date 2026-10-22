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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/utsname.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਜੇ HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# ਜੇ HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  ਨਹੀਂਤਾਂ
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "uname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME (uname_ਬਹੁਲਕ == UNAME_UNAME ? "uname" : "arch")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARCH_AUTHORS "David MacKenzie", "Karel Zak"

/* Values that are bitwise or'd into 'toprint'. */
/* Kernel name. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_KERNEL_NAME 1

/* Node name on a communications network. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_NODENAME 2

/* Kernel release. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_KERNEL_RELEASE 4

/* Kernel version. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_KERNEL_ਸੰਸਕਰਣ 8

/* Machine hardware name. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_MACHINE 16

/* Processor type. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_PROCESSOR 32

/* Hardware platform.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_HARDWARE_PLATFORM 64

/* Operating system.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_OPERATING_SYSTEM 128

ਸਥਿਰ bool labeled = false;

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ uname_long_options[] =
{
  {"all", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"all-labeled", no_ਦਲੀਲ, ਨਿਰਾ, 'A'},
  {"kernel-name", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"sysname", no_ਦਲੀਲ, ਨਿਰਾ, 's'},	/* Obsolescent.  */
  {"nodename", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"kernel-release", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"release", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"machine", no_ਦਲੀਲ, ਨਿਰਾ, 'm'},
  {"processor", no_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {"hardware-platform", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"operating-system", no_ਦਲੀਲ, ਨਿਰਾ, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]...\n"), ਪ੍ਰੋਗਰਾਮ_name);

      ਜੇ (uname_ਬਹੁਲਕ == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, ਵਿੱਚ the following ਕ੍ਰਮ,\n\
                             except omit -p and -i ਜੇ unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled ਖੇਤਰ ਪ੍ਰਤੀ ਸਤਰ,\n\
                             except omit -p and -i ਜੇ unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the ਜਾਲ ਗੰਢ hostname\n\
"));
          oputs (_("\
  -r, --kernel-release     print the kernel release\n\
"));
          oputs (_("\
  -v, --kernel-ਸੰਸਕਰਣ_3     print the kernel ਸੰਸਕਰਣ_3\n\
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
      ਨਹੀਂਤਾਂ
        {
          fputs (_("\
Print machine architecture.\n\
\n\
"), stdout);
        }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  */

ਸਥਿਰ ਖਾਲੀ
print_element (ਅੱਖਰ ਅਚਲ *element, ਅੱਖਰ ਅਚਲ *label)
{
  ਜੇ (labeled)
    {
      ਲਿਖੋ ("%s: %s\n", label, element);
    }
  ਨਹੀਂਤਾਂ
    {
      ਸਥਿਰ bool printed;
      ਜੇ (printed)
        ਅੱਖਰ_ਲਿਖੋ (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

ਸਥਿਰ ਖਾਲੀ
print_element_env (ਅੱਖਰ ਅਚਲ *element,
                   MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ ਅਚਲ *envvar,
                   ਅੱਖਰ ਅਚਲ *label)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
  ਜੇ (envvar)
    {
      ਅੱਖਰ ਅਚਲ *val = getenv (envvar);
      ਜੇ (val)
        element = val;
    }
#ਜੇ_ਅੰਤ
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

ਸਥਿਰ ਪੂਰਨ
decode_switches (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ c;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ toprint = 0;

  ਜੇ (uname_ਬਹੁਲਕ == UNAME_ARCH)
    {
      ਜਦੋਂ ((c = getopt_long (argc, argv, "",
                               arch_long_options, ਨਿਰਾ))
             != -1)
        {
          ਚੋਣ (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, ARCH_AUTHORS);

            ਮੂਲ:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  ਨਹੀਂਤਾਂ
    {
      ਜਦੋਂ ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, ਨਿਰਾ))
             != -1)
        {
          ਚੋਣ (c)
            {
            ਹਾਲਤ 'a':
              toprint = UINT_MAX;
              labeled = false;
              ਰੋਕੋ;

            ਹਾਲਤ 'A':
              toprint = UINT_MAX;
              labeled = true;
              ਰੋਕੋ;

            ਹਾਲਤ 's':
              toprint |= PRINT_KERNEL_NAME;
              ਰੋਕੋ;

            ਹਾਲਤ 'n':
              toprint |= PRINT_NODENAME;
              ਰੋਕੋ;

            ਹਾਲਤ 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              ਰੋਕੋ;

            ਹਾਲਤ 'v':
              toprint |= PRINT_KERNEL_ਸੰਸਕਰਣ;
              ਰੋਕੋ;

            ਹਾਲਤ 'm':
              toprint |= PRINT_MACHINE;
              ਰੋਕੋ;

            ਹਾਲਤ 'p':
              toprint |= PRINT_PROCESSOR;
              ਰੋਕੋ;

            ਹਾਲਤ 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              ਰੋਕੋ;

            ਹਾਲਤ 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              ਰੋਕੋ;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

            ਮੂਲ:
              usage (EXIT_FAILURE);
            }
        }
    }

  ਜੇ (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  ਵਾਪਸ toprint;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ toprint = 0;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  ਜੇ (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  ਜੇ (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_ਸੰਸਕਰਣ | PRINT_MACHINE))
    {
      ਬਣਤਰ utsname name;

      ਜੇ (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      ਜੇ (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      ਜੇ (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      ਜੇ (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      ਜੇ (toprint & PRINT_KERNEL_ਸੰਸਕਰਣ)
        print_element_env (name.ਸੰਸਕਰਣ_3, "UNAME_VERSION", _("Kernel version"));
      ਜੇ (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  ਜੇ (toprint & PRINT_PROCESSOR)
    {
      ਅੱਖਰ ਅਚਲ *element = unknown;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
# ਜੇ defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYSINFO && defined SI_ARCHITECTURE
      ਜੇ (element == unknown)
        {
          ਸਥਿਰ ਅੱਖਰ processor[257];
          ਜੇ (0 <= sysinfo (SI_ARCHITECTURE, processor, ਆਕਾਰ processor))
            element = processor;
        }
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ UNAME_PROCESSOR
      ਜੇ (element == unknown)
        {
          ਸਥਿਰ ਅੱਖਰ processor[257];
          size_t s = ਆਕਾਰ processor;
          ਸਥਿਰ ਪੂਰਨ mib[] = { CTL_HW, UNAME_PROCESSOR };
          ਜੇ (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#ਜੇ_ਅੰਤ
      ਜੇ (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  ਜੇ (toprint & PRINT_HARDWARE_PLATFORM)
    {
      ਅੱਖਰ ਅਚਲ *element = unknown;
#ਜੇ HAVE_SYSINFO && defined SI_PLATFORM
      {
        ਸਥਿਰ ਅੱਖਰ hardware_platform[257];
        ਜੇ (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, ਆਕਾਰ hardware_platform))
          element = hardware_platform;
      }
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ UNAME_HARDWARE_PLATFORM
      ਜੇ (element == unknown)
        {
          ਸਥਿਰ ਅੱਖਰ hardware_platform[257];
          size_t s = ਆਕਾਰ hardware_platform;
          ਸਥਿਰ ਪੂਰਨ mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          ਜੇ (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#ਜੇ_ਅੰਤ
      ਜੇ (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  ਜੇ (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  ਜੇ (!labeled)
    ਅੱਖਰ_ਲਿਖੋ ('\n');

  ਵਾਪਸ EXIT_SUCCESS;
}
