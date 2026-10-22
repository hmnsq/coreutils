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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/utsname.h>
#अन्तर्भाव <getopt.h>

#यदि HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#यदि HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# यदि HAVE_SYS_PARAM_H
#  include <sys/param.h> /* needed for OpenBSD 3.0 */
# endif
# include <sys/sysctl.h>
# ifdef HW_MODEL
#  ifdef HW_MACHINE_ARCH
/* E.g., FreeBSD 4.5, NetBSD 1.5.2 */
#   define UNAME_HARDWARE_PLATFORM HW_MODEL
#   define UNAME_PROCESSOR HW_MACHINE_ARCH
#  अन्यथा
/* E.g., OpenBSD 3.0 */
#   define UNAME_PROCESSOR HW_MODEL
#  endif
# endif
#endif

#अन्तर्भाव "system.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "uname.h"

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

स्थिर bool labeled = false;

स्थिर संरचना option अचर uname_long_options[] =
{
  {"all", no_argument, रिक्त, 'a'},
  {"all-labeled", no_argument, रिक्त, 'A'},
  {"kernel-name", no_argument, रिक्त, 's'},
  {"sysname", no_argument, रिक्त, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, रिक्त, 'n'},
  {"kernel-release", no_argument, रिक्त, 'r'},
  {"release", no_argument, रिक्त, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, रिक्त, 'v'},
  {"machine", no_argument, रिक्त, 'm'},
  {"processor", no_argument, रिक्त, 'p'},
  {"hardware-platform", no_argument, रिक्त, 'i'},
  {"operating-system", no_argument, रिक्त, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर संरचना option अचर arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]...\n"), program_name);

      यदि (uname_mode == UNAME_UNAME)
        {
          ख_माला_दो (_("\
Print certain प्रणाली information.  With no OPTION, same as -s.\n\
\n\
"), मानक_निकास);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i यदि unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per पङ्क्ति,\n\
                             except omit -p and -i यदि unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network गाँठो hostname\n\
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
  -o, --operating-प्रणाली   print the operating प्रणाली\n\
"));
        }
      अन्यथा
        {
          ख_माला_दो (_("\
Print machine architecture.\n\
\n\
"), मानक_निकास);
        }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  */

स्थिर शून्य
print_element (वर्ण अचर *element, वर्ण अचर *label)
{
  यदि (labeled)
    {
      म_लिखो ("%s: %s\n", label, element);
    }
  अन्यथा
    {
      स्थिर bool printed;
      यदि (printed)
        अक्षर_दो (' ');
      printed = true;
      ख_माला_दो (element, मानक_निकास);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

स्थिर शून्य
print_element_env (वर्ण अचर *element,
                   MAYBE_UNUSED वर्ण अचर *envvar,
                   वर्ण अचर *label)
{
#ifdef __APPLE__
  यदि (envvar)
    {
      वर्ण अचर *val = दो_पर्या (envvar);
      यदि (val)
        element = val;
    }
#endif
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

स्थिर पूर्णाङ्क
decode_switches (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;
  अचिन्हित पूर्णाङ्क toprint = 0;

  यदि (uname_mode == UNAME_ARCH)
    {
      जबसम्म ((c = getopt_long (argc, argv, "",
                               arch_long_options, रिक्त))
             != -1)
        {
          विकल्प (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            स्वभाव:
              usage (निकास_त्रुटि);
            }
        }
      toprint = PRINT_MACHINE;
    }
  अन्यथा
    {
      जबसम्म ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, रिक्त))
             != -1)
        {
          विकल्प (c)
            {
            पक्ष 'a':
              toprint = अच_पूर्णांक_उच्च;
              labeled = false;
              रोक;

            पक्ष 'A':
              toprint = अच_पूर्णांक_उच्च;
              labeled = true;
              रोक;

            पक्ष 's':
              toprint |= PRINT_KERNEL_NAME;
              रोक;

            पक्ष 'n':
              toprint |= PRINT_NODENAME;
              रोक;

            पक्ष 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              रोक;

            पक्ष 'v':
              toprint |= PRINT_KERNEL_VERSION;
              रोक;

            पक्ष 'm':
              toprint |= PRINT_MACHINE;
              रोक;

            पक्ष 'p':
              toprint |= PRINT_PROCESSOR;
              रोक;

            पक्ष 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              रोक;

            पक्ष 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              रोक;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            स्वभाव:
              usage (निकास_त्रुटि);
            }
        }
    }

  यदि (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (निकास_त्रुटि);
    }

  फर्काऊ toprint;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  स्थिर वर्ण अचर unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  अचिन्हित पूर्णाङ्क toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  toprint = decode_switches (argc, argv);

  यदि (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  यदि (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      संरचना utsname name;

      यदि (uname (&name) == -1)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot get system name"));

      यदि (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      यदि (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      यदि (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      यदि (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      यदि (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  यदि (toprint & PRINT_PROCESSOR)
    {
      वर्ण अचर *element = unknown;
#ifdef __APPLE__
# यदि defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#endif
#यदि HAVE_SYSINFO && defined SI_ARCHITECTURE
      यदि (element == unknown)
        {
          स्थिर वर्ण processor[257];
          यदि (0 <= sysinfo (SI_ARCHITECTURE, processor, परिमाण processor))
            element = processor;
        }
#endif
#ifdef UNAME_PROCESSOR
      यदि (element == unknown)
        {
          स्थिर वर्ण processor[257];
          माप_प्रकार s = परिमाण processor;
          स्थिर पूर्णाङ्क mib[] = { CTL_HW, UNAME_PROCESSOR };
          यदि (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#endif
      यदि (! (toprint == अच_पूर्णांक_उच्च && element == unknown))
        print_element (element, _("Processor"));
    }

  यदि (toprint & PRINT_HARDWARE_PLATFORM)
    {
      वर्ण अचर *element = unknown;
#यदि HAVE_SYSINFO && defined SI_PLATFORM
      {
        स्थिर वर्ण hardware_platform[257];
        यदि (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, परिमाण hardware_platform))
          element = hardware_platform;
      }
#endif
#ifdef UNAME_HARDWARE_PLATFORM
      यदि (element == unknown)
        {
          स्थिर वर्ण hardware_platform[257];
          माप_प्रकार s = परिमाण hardware_platform;
          स्थिर पूर्णाङ्क mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          यदि (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#endif
      यदि (! (toprint == अच_पूर्णांक_उच्च && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  यदि (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  यदि (!labeled)
    अक्षर_दो ('\n');

  फर्काऊ निकास_सफल;
}
