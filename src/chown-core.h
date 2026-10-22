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

#अगर_अघोषित CHOWN_CORE_H
# define CHOWN_CORE_H

# include "dev-ino.h"
# include <stdlib.h>

क्रमागत Change_status
{
  CH_NOT_APPLIED = 1,
  CH_SUCCEEDED,
  CH_FAILED,
  CH_NO_CHANGE_REQUESTED
};

क्रमागत Verbosity
{
  /* Print a message for each file that is processed.  */
  V_ऊपरी,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

काष्ठा Chown_option
{
  /* Level of verbosity.  */
  क्रमागत Verbosity verbosity;

  /* If nonzero, change the ownership of directories recursively. */
  bool recurse;

  /* Pointer to the device and inode numbers of '/', when --recursive.
     Need not be freed.  Otherwise NULL.  */
  काष्ठा dev_ino *मूल_dev_ino;

  /* This corresponds to the --dereference (opposite of -h) option.  */
  bool affect_symlink_referent;

  /* If nonzero, force silence (no error messages). */
  bool force_silent;

  /* The name of the user to which ownership of the files is being given. */
  अक्षर *user_name;

  /* The name of the group to which ownership of the files is being given. */
  अक्षर *group_name;
};

व्योम
chopt_init (काष्ठा Chown_option *);

व्योम
chopt_free (काष्ठा Chown_option *);

अक्षर *
gid_to_name (gid_t)
  _GL_ATTRIBUTE_MALLOC _GL_ATTRIBUTE_DEALLOC_FREE
  _GL_ATTRIBUTE_RETURNS_NONNULL;

अक्षर *
uid_to_name (uid_t)
  _GL_ATTRIBUTE_MALLOC _GL_ATTRIBUTE_DEALLOC_FREE
  _GL_ATTRIBUTE_RETURNS_NONNULL;

bool
chown_files (अक्षर **files, पूर्णांक bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             काष्ठा Chown_option स्थिर *chopt)
  _GL_ATTRIBUTE_NONNULL ();

#पूर्ण_अगर /* CHOWN_CORE_H */
