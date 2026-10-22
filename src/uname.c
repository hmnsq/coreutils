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
#यदि_अन्तः

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
#यदि_अन्तः

#अन्तर्भाव "system.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "uname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME (uname_बहुलकम् == UNAME_UNAME ? "uname" : "arch")

#परिभाषय AUTHORS proper_name ("David MacKenzie")
#परिभाषय ARCH_AUTHORS "David MacKenzie", "Karel Zak"

/* Values that are bitwise or'd into 'toprint'. */
/* Kernel name. */
#परिभाषय PRINT_KERNEL_NAME 1

/* Node name on a communications network. */
#परिभाषय PRINT_NODENAME 2

/* Kernel release. */
#परिभाषय PRINT_KERNEL_RELEASE 4

/* Kernel version. */
#परिभाषय PRINT_KERNEL_संस्करणम् 8

/* Machine hardware name. */
#परिभाषय PRINT_MACHINE 16

/* Processor type. */
#परिभाषय PRINT_PROCESSOR 32

/* Hardware platform.  */
#परिभाषय PRINT_HARDWARE_PLATFORM 64

/* Operating system.  */
#परिभाषय PRINT_OPERATING_SYSTEM 128

स्थिर bool labeled = false;

स्थिर संरचना option ध्रुव uname_long_options[] =
{
  {"all", no_तर्कः, रिक्त, 'a'},
  {"all-labeled", no_तर्कः, रिक्त, 'A'},
  {"kernel-name", no_तर्कः, रिक्त, 's'},
  {"sysname", no_तर्कः, रिक्त, 's'},	/* Obsolescent.  */
  {"nodename", no_तर्कः, रिक्त, 'n'},
  {"kernel-release", no_तर्कः, रिक्त, 'r'},
  {"release", no_तर्कः, रिक्त, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_तर्कः, रिक्त, 'v'},
  {"machine", no_तर्कः, रिक्त, 'm'},
  {"processor", no_तर्कः, रिक्त, 'p'},
  {"hardware-platform", no_तर्कः, रिक्त, 'i'},
  {"operating-system", no_तर्कः, रिक्त, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर संरचना option ध्रुव arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]...\n"), क्रमादेशः_name);

      यदि (uname_बहुलकम् == UNAME_UNAME)
        {
          fputs (_("\
Print certain system information.  With no OPTION, same as -s.\n\
\n\
"), stdout);
          oputs (_("\
  -a, --all                print all information, अन्तर् the following क्रमः,\n\
                             except omit -p and -i यदि unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled क्षेत्रम् प्रत्येकम् पङ्क्तिः,\n\
                             except omit -p and -i यदि unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the जालम् ग्रन्थिः hostname\n\
"));
          oputs (_("\
  -r, --kernel-release     print the kernel release\n\
"));
          oputs (_("\
  -v, --kernel-संस्करणम्_3     print the kernel संस्करणम्_3\n\
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
      अन्यथा
        {
          fputs (_("\
Print machine architecture.\n\
\n\
"), stdout);
        }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  */

स्थिर शून्य
print_element (वर्ण ध्रुव *element, वर्ण ध्रुव *label)
{
  यदि (labeled)
    {
      प्रारूप_लिख ("%s: %s\n", label, element);
    }
  अन्यथा
    {
      स्थिर bool printed;
      यदि (printed)
        अक्षरं_लिख (' ');
      printed = true;
      fputs (element, stdout);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

स्थिर शून्य
print_element_env (वर्ण ध्रुव *element,
                   MAYBE_अप्रयुक्तम् वर्ण ध्रुव *envvar,
                   वर्ण ध्रुव *label)
{
#यदि_परिभाषितम् __APPLE__
  यदि (envvar)
    {
      वर्ण ध्रुव *val = getenv (envvar);
      यदि (val)
        element = val;
    }
#यदि_अन्तः
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

स्थिर पूर्णाङ्क
decode_switches (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;
  अचिह्नित पूर्णाङ्क toprint = 0;

  यदि (uname_बहुलकम् == UNAME_ARCH)
    {
      यावत् ((c = getopt_long (argc, argv, "",
                               arch_long_options, रिक्त))
             != -1)
        {
          विकल्प (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, ARCH_AUTHORS);

            स्वभाव:
              usage (EXIT_FAILURE);
            }
        }
      toprint = PRINT_MACHINE;
    }
  अन्यथा
    {
      यावत् ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, रिक्त))
             != -1)
        {
          विकल्प (c)
            {
            पक्ष 'a':
              toprint = UINT_MAX;
              labeled = false;
              भङ्ग;

            पक्ष 'A':
              toprint = UINT_MAX;
              labeled = true;
              भङ्ग;

            पक्ष 's':
              toprint |= PRINT_KERNEL_NAME;
              भङ्ग;

            पक्ष 'n':
              toprint |= PRINT_NODENAME;
              भङ्ग;

            पक्ष 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              भङ्ग;

            पक्ष 'v':
              toprint |= PRINT_KERNEL_संस्करणम्;
              भङ्ग;

            पक्ष 'm':
              toprint |= PRINT_MACHINE;
              भङ्ग;

            पक्ष 'p':
              toprint |= PRINT_PROCESSOR;
              भङ्ग;

            पक्ष 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              भङ्ग;

            पक्ष 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              भङ्ग;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

            स्वभाव:
              usage (EXIT_FAILURE);
            }
        }
    }

  यदि (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  प्रत्यागम toprint;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  स्थिर वर्ण ध्रुव unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  अचिह्नित पूर्णाङ्क toprint = 0;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  toprint = decode_switches (argc, argv);

  यदि (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  यदि (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_संस्करणम् | PRINT_MACHINE))
    {
      संरचना utsname name;

      यदि (uname (&name) == -1)
        error (EXIT_FAILURE, errno, _("cannot get system name"));

      यदि (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      यदि (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      यदि (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      यदि (toprint & PRINT_KERNEL_संस्करणम्)
        print_element_env (name.संस्करणम्_3, "UNAME_VERSION", _("Kernel version"));
      यदि (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  यदि (toprint & PRINT_PROCESSOR)
    {
      वर्ण ध्रुव *element = unknown;
#यदि_परिभाषितम् __APPLE__
# यदि defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#यदि_अन्तः
#यदि HAVE_SYSINFO && defined SI_ARCHITECTURE
      यदि (element == unknown)
        {
          स्थिर वर्ण processor[257];
          यदि (0 <= sysinfo (SI_ARCHITECTURE, processor, परिमाण processor))
            element = processor;
        }
#यदि_अन्तः
#यदि_परिभाषितम् UNAME_PROCESSOR
      यदि (element == unknown)
        {
          स्थिर वर्ण processor[257];
          size_t s = परिमाण processor;
          स्थिर पूर्णाङ्क mib[] = { CTL_HW, UNAME_PROCESSOR };
          यदि (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#यदि_अन्तः
      यदि (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Processor"));
    }

  यदि (toprint & PRINT_HARDWARE_PLATFORM)
    {
      वर्ण ध्रुव *element = unknown;
#यदि HAVE_SYSINFO && defined SI_PLATFORM
      {
        स्थिर वर्ण hardware_platform[257];
        यदि (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, परिमाण hardware_platform))
          element = hardware_platform;
      }
#यदि_अन्तः
#यदि_परिभाषितम् UNAME_HARDWARE_PLATFORM
      यदि (element == unknown)
        {
          स्थिर वर्ण hardware_platform[257];
          size_t s = परिमाण hardware_platform;
          स्थिर पूर्णाङ्क mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          यदि (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#यदि_अन्तः
      यदि (! (toprint == UINT_MAX && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  यदि (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  यदि (!labeled)
    अक्षरं_लिख ('\n');

  प्रत्यागम EXIT_SUCCESS;
}
