/* Exit with a status code indicating success.
   Copyright (C) 1999-2026 Free Software Foundation, Inc.

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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "true.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXIT_ਸਥਿਤੀ (true_ਬਹੁਲਕ != TRUE_TRUE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME (true_ਬਹੁਲਕ == TRUE_TRUE ? "true" : "false")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Jim Meyering")

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਲਿਖੋ (_("\
Usage: %s [ignored command ਸਤਰ arguments]\n\
  or:  %s OPTION\n\
"),
          ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
  ਲਿਖੋ ("%s\n\n",
          _(EXIT_ਸਥਿਤੀ == EXIT_SUCCESS
            ? N_("Exit with a status code indicating success.")
            : N_("Exit with a status code indicating failure.")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
  ਲਿਖੋ (USAGE_BUILTIN_WARNING, ਪ੍ਰੋਗਰਾਮ_NAME);
  emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  /* Recognize --help or --version only if it's the only command-line
     argument.  */
  ਜੇ (argc == 2)
    {
      initialize_main (&argc, &argv);
      ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
      setlocale (LC_ALL, "");
      bindtextdomain (PACKAGE, LOCALEDIR);
      textdomain (PACKAGE);

      /* Note true(1) will return EXIT_FAILURE in the
         edge case where writes fail with GNU specific options.  */
      atexit (close_stdout);

      ਜੇ (streq (argv[1], "--help"))
        usage (EXIT_ਸਥਿਤੀ);

      ਜੇ (streq (argv[1], "--version"))
        ਸੰਸਕਰਣ_etc (stdout, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME, ਸੰਸਕਰਣ, AUTHORS,
                     (ਅੱਖਰ *) ਨਿਰਾ);
    }

  ਵਾਪਸ EXIT_ਸਥਿਤੀ;
}
