/* chown-core.h -- types and prototypes shared by chown and chgrp.

   Copyright (C) 2000-2026 Free Software Foundation, Inc.

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

#اگر_غیر_تعریف_شدہ CHOWN_CORE_H
# define CHOWN_CORE_H

# include "dev-ino.h"
# include <stdlib.h>

شمار Change_حالت
{
  CH_NOT_APPLIED = 1,
  CH_SUCCEEDED,
  CH_FAILED,
  CH_NO_CHANGE_REQUESTED
};

شمار Verbosity
{
  /* Print a message for each file that is processed.  */
  V_بلند,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

ساخت Chown_option
{
  /* Level of verbosity.  */
  شمار Verbosity verbosity;

  /* If nonzero, change the ownership of directories recursively. */
  bool recurse;

  /* Pointer to the device and inode numbers of '/', when --recursive.
     Need not be freed.  Otherwise NULL.  */
  ساخت dev_ino *جڑ_dev_ino;

  /* This corresponds to the --dereference (opposite of -h) option.  */
  bool affect_symlink_referent;

  /* If nonzero, force silence (no error messages). */
  bool force_silent;

  /* The name of the user to which ownership of the files is being given. */
  حرف *user_name;

  /* The name of the group to which ownership of the files is being given. */
  حرف *گروہ_name;
};

خالی
chopt_init (ساخت Chown_option *);

خالی
chopt_free (ساخت Chown_option *);

حرف *
gid_مقصد_name (gid_t)
  _GL_ATTRIBUTE_MALLOC _GL_ATTRIBUTE_DEALLOC_FREE
  _GL_ATTRIBUTE_RETURNS_NONNULL;

حرف *
uid_مقصد_name (uid_t)
  _GL_ATTRIBUTE_MALLOC _GL_ATTRIBUTE_DEALLOC_FREE
  _GL_ATTRIBUTE_RETURNS_NONNULL;

bool
chown_files (حرف **files, صحیح ثنائی_ہندسہ_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             ساخت Chown_option ثابت *chopt)
  _GL_ATTRIBUTE_NONNULL ();

#اگر_ختم /* CHOWN_CORE_H */
