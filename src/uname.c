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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/utsname.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଯଦି HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#ଯଦି HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# ଯଦି HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  ଅନ୍ୟଥା
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "uname.h"

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

ସ୍ଥିର bool labeled = false;

ସ୍ଥିର ସଂରଚନା option ଅଚଳ uname_long_options[] =
{
  {"all", no_argument, ଖାଲି, 'a'},
  {"all-labeled", no_argument, ଖାଲି, 'A'},
  {"kernel-name", no_argument, ଖାଲି, 's'},
  {"sysname", no_argument, ଖାଲି, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, ଖାଲି, 'n'},
  {"kernel-release", no_argument, ଖାଲି, 'r'},
  {"release", no_argument, ଖାଲି, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, ଖାଲି, 'v'},
  {"machine", no_argument, ଖାଲି, 'm'},
  {"processor", no_argument, ଖାଲି, 'p'},
  {"hardware-platform", no_argument, ଖାଲି, 'i'},
  {"operating-system", no_argument, ଖାଲି, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);

      ଯଦି (uname_mode == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i ଯଦି unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per ଧାଡ଼ି,\n\
                             except omit -p and -i ଯଦି unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network ଗଣ୍ଠି hostname\n\
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
      ଅନ୍ୟଥା
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

ସ୍ଥିର ଶୂନ୍ୟ
print_element (ଅକ୍ଷର ଅଚଳ *element, ଅକ୍ଷର ଅଚଳ *label)
{
  ଯଦି (labeled)
    {
      printf ("%s: %s\n", label, element);
    }
  ଅନ୍ୟଥା
    {
      ସ୍ଥିର bool printed;
      ଯଦି (printed)
        putchar (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

ସ୍ଥିର ଶୂନ୍ୟ
print_element_env (ଅକ୍ଷର ଅଚଳ *element,
                   MAYBE_UNUSED ଅକ୍ଷର ଅଚଳ *envvar,
                   ଅକ୍ଷର ଅଚଳ *label)
{
#ifdef __APPLE__
  ଯଦି (envvar)
    {
      ଅକ୍ଷର ଅଚଳ *val = getenv (envvar);
      ଯଦି (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
decode_switches (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା toprint = 0;

  ଯଦି (uname_mode == UNAME_ARCH)
    {
      ଯେତେବେଳେ ((c = getopt_long (argc, argv, "",
                               arch_long_options, ଖାଲି))
             != -1)
        {
          ବାଛ (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  ଅନ୍ୟଥା
    {
      ଯେତେବେଳେ ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, ଖାଲି))
             != -1)
        {
          ବାଛ (c)
            {
            ସ୍ଥିତି 'a':
              toprint = UINT_MAX;
              labeled = false;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'A':
              toprint = UINT_MAX;
              labeled = true;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 's':
              toprint |= PRINT_KERNEL_NAME;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'n':
              toprint |= PRINT_NODENAME;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'v':
              toprint |= PRINT_KERNEL_VERSION;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'm':
              toprint |= PRINT_MACHINE;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'p':
              toprint |= PRINT_PROCESSOR;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              ଭାଙ୍ଗ;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              usage (EXIT_FAILURE);
            }
        }
    }

  ଯଦି (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  ଫେରନ୍ତୁ toprint;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  ଯଦି (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  ଯଦି (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      ସଂରଚନା utsname name;

      ଯଦି (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      ଯଦି (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      ଯଦି (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      ଯଦି (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      ଯଦି (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      ଯଦି (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  ଯଦି (toprint & PRINT_PROCESSOR)
    {
      ଅକ୍ଷର ଅଚଳ *element = unknown;
#ifdef __APPLE__
# ଯଦି defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#ଯଦି HAVE_SYSINFO && defined SI_ARCHITECTURE
      ଯଦି (element == unknown)
        {
          ସ୍ଥିର ଅକ୍ଷର processor[257];
          ଯଦି (0 <= sysinfo (SI_ARCHITECTURE, processor, ଆକାର processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      ଯଦି (element == unknown)
        {
          ସ୍ଥିର ଅକ୍ଷର processor[257];
          size_t s = ଆକାର processor;
          ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mib[] = { CTL_HW, UNAME_PROCESSOR };
          ଯଦି (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      ଯଦି (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  ଯଦି (toprint & PRINT_HARDWARE_PLATFORM)
    {
      ଅକ୍ଷର ଅଚଳ *element = unknown;
#ଯଦି HAVE_SYSINFO && defined SI_PLATFORM
      {
        ସ୍ଥିର ଅକ୍ଷର hardware_platform[257];
        ଯଦି (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, ଆକାର hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      ଯଦି (element == unknown)
        {
          ସ୍ଥିର ଅକ୍ଷର hardware_platform[257];
          size_t s = ଆକାର hardware_platform;
          ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          ଯଦି (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      ଯଦି (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  ଯଦି (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  ଯଦି (!labeled)
    putchar ('\n');

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
