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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <sys/utsname.h>
#समावेश <getopt.h>

#अगर HAVE_SYSINFO && HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#पूर्ण_अगर

#अगर HAVE_SYS_SYSCTL_H && ! defined __GLIBC__ && ! defined __APPLE__
# अगर HAVE_SYS_PARAM_H
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
#पूर्ण_अगर

#समावेश "system.h"
#समावेश "quote.h"
#समावेश "uname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME (uname_mode == UNAME_UNAME ? "uname" : "arch")

#घोषणा AUTHORS proper_name ("David MacKenzie")
#घोषणा ARCH_AUTHORS "David MacKenzie", "Karel Zak"

/* Values that are bitwise or'd into 'toprint'. */
/* Kernel name. */
#घोषणा PRINT_KERNEL_NAME 1

/* Node name on a communications network. */
#घोषणा PRINT_NODENAME 2

/* Kernel release. */
#घोषणा PRINT_KERNEL_RELEASE 4

/* Kernel version. */
#घोषणा PRINT_KERNEL_VERSION 8

/* Machine hardware name. */
#घोषणा PRINT_MACHINE 16

/* Processor type. */
#घोषणा PRINT_PROCESSOR 32

/* Hardware platform.  */
#घोषणा PRINT_HARDWARE_PLATFORM 64

/* Operating system.  */
#घोषणा PRINT_OPERATING_SYSTEM 128

अटल bool labeled = false;

अटल काष्ठा option स्थिर uname_long_options[] =
{
  {"all", no_argument, शून्य, 'a'},
  {"all-labeled", no_argument, शून्य, 'A'},
  {"kernel-name", no_argument, शून्य, 's'},
  {"sysname", no_argument, शून्य, 's'},	/* Obsolescent.  */
  {"nodename", no_argument, शून्य, 'n'},
  {"kernel-release", no_argument, शून्य, 'r'},
  {"release", no_argument, शून्य, 'r'},  /* Obsolescent.  */
  {"kernel-version", no_argument, शून्य, 'v'},
  {"machine", no_argument, शून्य, 'm'},
  {"processor", no_argument, शून्य, 'p'},
  {"hardware-platform", no_argument, शून्य, 'i'},
  {"operating-system", no_argument, शून्य, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

अटल काष्ठा option स्थिर arch_long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]...\n"), program_name);

      अगर (uname_mode == UNAME_UNAME)
        {
          ख_माला_दो (_("\
Print certain प्रणाली information.  With no OPTION, same as -s.\n\
\n\
"), मानक_निकास);
          oputs (_("\
  -a, --all                print all information, in the following order,\n\
                             except omit -p and -i अगर unknown\n\
"));
          oputs (_("\
  -A, --all-labeled        print all information, one labeled field per पंक्ति,\n\
                             except omit -p and -i अगर unknown\n\
"));
          oputs (_("\
  -s, --kernel-name        print the kernel name\n\
"));
          oputs (_("\
  -n, --nodename           print the network गाँठ hostname\n\
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

अटल व्योम
print_element (अक्षर स्थिर *element, अक्षर स्थिर *label)
{
  अगर (labeled)
    {
      म_लिखो ("%s: %s\n", label, element);
    }
  अन्यथा
    {
      अटल bool printed;
      अगर (printed)
        अक्षर_दो (' ');
      printed = true;
      ख_माला_दो (element, मानक_निकास);
    }
}

/* Print ELEMENT, preceded by a space if something has already been
   printed.  But if the environment variable ENVVAR is set, print its
   value instead of ELEMENT.  */

अटल व्योम
print_element_env (अक्षर स्थिर *element,
                   MAYBE_UNUSED अक्षर स्थिर *envvar,
                   अक्षर स्थिर *label)
{
#अगर_घोषित __APPLE__
  अगर (envvar)
    {
      अक्षर स्थिर *val = दो_पर्या (envvar);
      अगर (val)
        element = val;
    }
#पूर्ण_अगर
  print_element (element, label);
}


/* Set all the option flags according to the switches specified.
   Return the mask indicating which elements to print.  */

अटल पूर्णांक
decode_switches (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक c;
  अचिन्हित पूर्णांक toprint = 0;

  अगर (uname_mode == UNAME_ARCH)
    {
      जबतक ((c = getopt_long (argc, argv, "",
                               arch_long_options, शून्य))
             != -1)
        {
          चयन (c)
            {
            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, ARCH_AUTHORS);

            शेष:
              usage (निकास_त्रुटि);
            }
        }
      toprint = PRINT_MACHINE;
    }
  अन्यथा
    {
      जबतक ((c = getopt_long (argc, argv, "asnrvmpioA",
                               uname_long_options, शून्य))
             != -1)
        {
          चयन (c)
            {
            हाल 'a':
              toprint = अच_पूर्णांक_उच्च;
              labeled = false;
              अवरोध;

            हाल 'A':
              toprint = अच_पूर्णांक_उच्च;
              labeled = true;
              अवरोध;

            हाल 's':
              toprint |= PRINT_KERNEL_NAME;
              अवरोध;

            हाल 'n':
              toprint |= PRINT_NODENAME;
              अवरोध;

            हाल 'r':
              toprint |= PRINT_KERNEL_RELEASE;
              अवरोध;

            हाल 'v':
              toprint |= PRINT_KERNEL_VERSION;
              अवरोध;

            हाल 'm':
              toprint |= PRINT_MACHINE;
              अवरोध;

            हाल 'p':
              toprint |= PRINT_PROCESSOR;
              अवरोध;

            हाल 'i':
              toprint |= PRINT_HARDWARE_PLATFORM;
              अवरोध;

            हाल 'o':
              toprint |= PRINT_OPERATING_SYSTEM;
              अवरोध;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            शेष:
              usage (निकास_त्रुटि);
            }
        }
    }

  अगर (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (निकास_त्रुटि);
    }

  वापस toprint;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अटल अक्षर स्थिर unknown[] = "unknown";

  /* Mask indicating which elements to print. */
  अचिन्हित पूर्णांक toprint = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  toprint = decode_switches (argc, argv);

  अगर (toprint == 0)
    toprint = PRINT_KERNEL_NAME;

  अगर (toprint
       & (PRINT_KERNEL_NAME | PRINT_NODENAME | PRINT_KERNEL_RELEASE
          | PRINT_KERNEL_VERSION | PRINT_MACHINE))
    {
      काष्ठा utsname name;

      अगर (uname (&name) == -1)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot get system name"));

      अगर (toprint & PRINT_KERNEL_NAME)
        print_element_env (name.sysname, "UNAME_SYSNAME", _("Kernel name"));
      अगर (toprint & PRINT_NODENAME)
        print_element_env (name.nodename, "UNAME_NODENAME", _("Node name"));
      अगर (toprint & PRINT_KERNEL_RELEASE)
        print_element_env (name.release, "UNAME_RELEASE", _("Kernel release"));
      अगर (toprint & PRINT_KERNEL_VERSION)
        print_element_env (name.version, "UNAME_VERSION", _("Kernel version"));
      अगर (toprint & PRINT_MACHINE)
        print_element_env (name.machine, "UNAME_MACHINE", _("Machine"));
    }

  अगर (toprint & PRINT_PROCESSOR)
    {
      अक्षर स्थिर *element = unknown;
#अगर_घोषित __APPLE__
# अगर defined __arm__ || defined __arm64__
      element = "arm";
# elif defined __i386__ || defined __x86_64__
      element = "i386";
# elif defined __ppc__ || defined __ppc64__
      element = "powerpc";
# endif
#पूर्ण_अगर
#अगर HAVE_SYSINFO && defined SI_ARCHITECTURE
      अगर (element == unknown)
        {
          अटल अक्षर processor[257];
          अगर (0 <= sysinfo (SI_ARCHITECTURE, processor, माप processor))
            element = processor;
        }
#पूर्ण_अगर
#अगर_घोषित UNAME_PROCESSOR
      अगर (element == unknown)
        {
          अटल अक्षर processor[257];
          माप_प्रकार s = माप processor;
          अटल पूर्णांक mib[] = { CTL_HW, UNAME_PROCESSOR };
          अगर (sysctl (mib, 2, processor, &s, 0, 0) >= 0)
            element = processor;
        }
#पूर्ण_अगर
      अगर (! (toprint == अच_पूर्णांक_उच्च && element == unknown))
        print_element (element, _("Processor"));
    }

  अगर (toprint & PRINT_HARDWARE_PLATFORM)
    {
      अक्षर स्थिर *element = unknown;
#अगर HAVE_SYSINFO && defined SI_PLATFORM
      {
        अटल अक्षर hardware_platform[257];
        अगर (0 <= sysinfo (SI_PLATFORM,
                          hardware_platform, माप hardware_platform))
          element = hardware_platform;
      }
#पूर्ण_अगर
#अगर_घोषित UNAME_HARDWARE_PLATFORM
      अगर (element == unknown)
        {
          अटल अक्षर hardware_platform[257];
          माप_प्रकार s = माप hardware_platform;
          अटल पूर्णांक mib[] = { CTL_HW, UNAME_HARDWARE_PLATFORM };
          अगर (sysctl (mib, 2, hardware_platform, &s, 0, 0) >= 0)
            element = hardware_platform;
        }
#पूर्ण_अगर
      अगर (! (toprint == अच_पूर्णांक_उच्च && element == unknown))
        print_element (element, _("Hardware platform"));
    }

  अगर (toprint & PRINT_OPERATING_SYSTEM)
    print_element (HOST_OPERATING_SYSTEM, _("Operating system"));

  अगर (!labeled)
    अक्षर_दो ('\n');

  वापस निकास_सफल;
}
