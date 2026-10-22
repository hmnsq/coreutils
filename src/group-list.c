/* group-list.c --Print a list of group IDs or names.
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

/* Written by Arnold Robbins.
   Major rewrite by David MacKenzie, djm@gnu.ai.mit.edu.
   Extracted from id.c by James Youngman. */

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <grp.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "mgetgroups.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "group-list.h"


/* Print all of the distinct groups the user is in. */
ബാഹ്യ bool
print_group_list (അക്ഷരം സ്ഥിരം *username,
                  uid_t ruid, gid_t rgid, gid_t egid,
                  bool use_names, അക്ഷരം delim)
{
  bool ok = true;
  ഘടന passwd *pwd = ശൂന്യ;

  എങ്കിൽ (username)
    {
      pwd = getpwuid (ruid);
      എങ്കിൽ (pwd == ശൂന്യ)
        ok = false;
    }

  എങ്കിൽ (!print_group (rgid, use_names))
    ok = false;

  എങ്കിൽ (egid != rgid)
    {
      putchar (delim);
      എങ്കിൽ (!print_group (egid, use_names))
        ok = false;
    }

  {
    gid_t *groups;

    പൂർണ്ണസംഖ്യ n_groups = xgetgroups (username, (pwd ? pwd->pw_gid : egid), &groups);
    എങ്കിൽ (n_groups < 0)
      {
        എങ്കിൽ (username)
          {
            error (0, errno, _("failed to get groups for user %s"),
                   quote (username));
          }
        അല്ലെങ്കിൽ
          {
            error (0, errno, _("failed to get groups for the current process"));
          }
        തിരികെ false;
      }

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_groups; i++)
      എങ്കിൽ (groups[i] != rgid && groups[i] != egid)
        {
          putchar (delim);
          എങ്കിൽ (!print_group (groups[i], use_names))
            ok = false;
        }
    സ്വതന്ത്രം (groups);
  }
  തിരികെ ok;
}

/* Print the name or value of group ID GID. */
ബാഹ്യ bool
print_group (gid_t gid, bool use_name)
{
  ഘടന group *grp = ശൂന്യ;
  bool ok = true;

  എങ്കിൽ (use_name)
    {
      grp = getgrgid (gid);
      എങ്കിൽ (grp == ശൂന്യ)
        {
          എങ്കിൽ (TYPE_SIGNED (gid_t))
            {
              intmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %jd"), g);
            }
          അല്ലെങ്കിൽ
            {
              uintmax_t g = gid;
              error (0, 0, _("cannot find name for group ID %ju"), g);
            }
          ok = false;
        }
    }

  എങ്കിൽ (grp)
    printf ("%s", grp->gr_name);
  അല്ലെങ്കിൽ
    printf ("%ju", (uintmax_t) gid);
  തിരികെ ok;
}
