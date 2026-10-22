/* cp-hash.c  -- file copying (hash search routines)
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Written by Torbjörn Granlund, Sweden (tege@sics.se).
   Rewritten to use lib/hash.c by Jim Meyering.  */

#ضمن <config.h>

#ضمن <sys/types.h>
#ضمن "system.h"

#ضمن "hash.h"
#ضمن "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
بنية Src_منزل_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  حرف *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
ساكن Hash_جدول *src_منزل_dest;

/* Initial size of the above hash table.  */
#وصف_ڪريو INITIAL_جدول_سائيز 103

ساكن size_t
src_منزل_dest_hash (فراغ ثابت *x, size_t جدول_سائيز)
{
  بنية Src_منزل_dest ثابت *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  أرجع (uintmax_t) {p->st_ino} % جدول_سائيز;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
ساكن bool
src_منزل_dest_compare (فراغ ثابت *x, فراغ ثابت *y)
{
  بنية Src_منزل_dest ثابت *a = x;
  بنية Src_منزل_dest ثابت *b = y;
  أرجع PSAME_INODE (a, b);
}

ساكن فراغ
src_منزل_dest_free (فراغ *x)
{
  بنية Src_منزل_dest *a = x;
  حرر (a->name);
  حرر (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
خارجي فراغ
forget_created (ino_t ino, dev_t dev)
{
  بنية Src_منزل_dest probe;
  بنية Src_منزل_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = عدم;

  ent = hash_remove (src_منزل_dest, &probe);
  إذا (ent)
    src_منزل_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

خارجي حرف *
src_منزل_dest_lookup (ino_t ino, dev_t dev)
{
  بنية Src_منزل_dest ent;
  بنية Src_منزل_dest ثابت *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_منزل_dest, &ent);
  أرجع e ? e->name : عدم;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

خارجي حرف *
remember_copied (حرف ثابت *name, ino_t ino, dev_t dev)
{
  بنية Src_منزل_dest *ent;
  بنية Src_منزل_dest *ent_from_جدول;

  ent = xmalloc (حجم *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_جدول = hash_وجهو (src_منزل_dest, ent);
  إذا (ent_from_جدول == عدم)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  إذا (ent_from_جدول != ent)
    {
      src_منزل_dest_free (ent);
      أرجع ent_from_جدول->name;
    }

  /* New key;  insertion succeeded.  */
  أرجع عدم;
}

/* Initialize the hash table.  */
خارجي فراغ
hash_init (فراغ)
{
  src_منزل_dest = hash_initialize (INITIAL_جدول_سائيز, عدم,
                                 src_منزل_dest_hash,
                                 src_منزل_dest_compare,
                                 src_منزل_dest_free);
  إذا (src_منزل_dest == عدم)
    xalloc_die ();
}
