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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/utsname.h>
#அந்தர்பாவ <getopt.h>

#எனில் HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#எனில் HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# எனில் HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  இல்லையெனில்
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "uname.h"

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

நிலையான bool labeled = false;

நிலையான கட்டமைப்பு option மாறிலி uname_long_options[] =
{
  {"all", no_argument, வெற்று, 'a'},
  {"all-labeled", no_argument, வெற்று, 'A'},
  {"kernel-name", no_argument, வெற்று, 's'},
  {"sysname", no_argument, வெற்று, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, வெற்று, 'n'},
  {"kernel-release", no_argument, வெற்று, 'r'},
  {"release", no_argument, வெற்று, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, வெற்று, 'v'},
  {"machine", no_argument, வெற்று, 'm'},
  {"processor", no_argument, வெற்று, 'p'},
  {"hardware-platform", no_argument, வெற்று, 'i'},
  {"operating-system", no_argument, வெற்று, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

நிலையான கட்டமைப்பு option மாறிலி arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      எனில் (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i எனில் unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per வரி,\n\
                             except omit -p and -i எனில் unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network முனை hostname\n\
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
      இல்லையெனில்
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

நிலையான வெறுமை
print_element (எழுத்து மாறிலி *element, எழுத்து மாறிலி *label)
{
  எனில் (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  இல்லையெனில்
    {
      நிலையான bool printed;
      எனில் (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

நிலையான வெறுமை
print_element_env (எழுத்து மாறிலி *element,
                   MAYBE_UNUSED எழுத்து மாறிலி *envvar,
                   எழுத்து மாறிலி *label)
{
#ifdef __APPLE__
  எனில் (envvar)
    {
      எழுத்து மாறிலி *val = getenv (envvar);
      எனில் (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

நிலையான முழு
decode_switches (முழு argc, எழுத்து **argv)
{
  முழு c;
  unsigned முழு toprint = 0;

  எனில் (uname_mode == UNAME_ARCH)
    {
      போது ((c = getopt_long (argc, argv, "",
                               arch_long_options, வெற்று))
             != -1)
        {
          தேர்வு (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            இயல்பு:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  இல்லையெனில்
    {
      போது ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, வெற்று))
             != -1)
        {
          தேர்வு (c)
            {
            நிலை 'a':
              toprint = UINT_MAX;
              labeled = false;
              நிறுத்து;

            நிலை 'A':
              toprint = UINT_MAX;
              labeled = true;
              நிறுத்து;

            நிலை 's':
              toprint |= PRINT_KERNEL_NAME;
              நிறுத்து;

            நிலை 'n':
              toprint |= PRINT_NODENAME;
              நிறுத்து;

            நிலை 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              நிறுத்து;

            நிலை 'v':
              toprint |= PRINT_KERNEL_VERSION;
              நிறுத்து;

            நிலை 'm':
              toprint |= PRINT_MACHINE;
              நிறுத்து;

            நிலை 'p':
              toprint |= PRINT_PROCESSOR;
              நிறுத்து;

            நிலை 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              நிறுத்து;

            நிலை 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              நிறுத்து;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            இயல்பு:
              usage (EXIT_FAILURE);
            }
        }
    }

  எனில் (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  திரும்பு toprint;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  நிலையான எழுத்து மாறிலி unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned முழு toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  எனில் (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  எனில் (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      கட்டமைப்பு utsname name;

      எனில் (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      எனில் (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      எனில் (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      எனில் (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      எனில் (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      எனில் (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  எனில் (toprint & PRINT_PROCESSOR)
    {
      எழுத்து மாறிலி *element = unknown;
#ifdef __APPLE__
# எனில் defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#எனில் HAVE_SYSINFO && defined SI_ARCHITECTURE
      எனில் (element == unknown)
        {
          நிலையான எழுத்து processor[257];
          எனில் (0 <= sysinfo (SI_ARCHITECTURE, processor, அளவு processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      எனில் (element == unknown)
        {
          நிலையான எழுத்து processor[257];
          size_t s = அளவு processor;
          நிலையான முழு mib[] = { CTL_HW, UNAME_PROCESSOR };
          எனில் (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      எனில் (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  எனில் (toprint & PRINT_HARDWARE_PLATFORM)
    {
      எழுத்து மாறிலி *element = unknown;
#எனில் HAVE_SYSINFO && defined SI_PLATFORM
      {
        நிலையான எழுத்து hardware_platform[257];
        எனில் (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, அளவு hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      எனில் (element == unknown)
        {
          நிலையான எழுத்து hardware_platform[257];
          size_t s = அளவு hardware_platform;
          நிலையான முழு mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          எனில் (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      எனில் (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  எனில் (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  எனில் (!labeled)
    putchar ('\n');

  திரும்பு EXIT_SUCCESS;
}
