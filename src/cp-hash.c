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

#شامل <config.h>

#شامل <sys/types.h>
#شامل "system.h"

#شامل "hash.h"
#شامل "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
ساخت Src_مقصد_dest
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
ساکن Hash_جدول *src_مقصد_dest;

/* Initial size of the above hash table.  */
#تعریف_کرو INITIAL_جدول_سائز 103

ساکن size_t
src_مقصد_dest_hash (خالی ثابت *x, size_t جدول_سائز)
{
  ساخت Src_مقصد_dest ثابت *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  واپس (uintmax_t) {p->st_ino} % جدول_سائز;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
ساکن bool
src_مقصد_dest_compare (خالی ثابت *x, خالی ثابت *y)
{
  ساخت Src_مقصد_dest ثابت *a = x;
  ساخت Src_مقصد_dest ثابت *b = y;
  واپس PSAME_INODE (a, b);
}

ساکن خالی
src_مقصد_dest_free (خالی *x)
{
  ساخت Src_مقصد_dest *a = x;
  آزاد (a->name);
  آزاد (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
خارجی خالی
forget_created (ino_t ino, dev_t dev)
{
  ساخت Src_مقصد_dest probe;
  ساخت Src_مقصد_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = تہی;

  ent = hash_remove (src_مقصد_dest, &probe);
  اگر (ent)
    src_مقصد_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

خارجی حرف *
src_مقصد_dest_lookup (ino_t ino, dev_t dev)
{
  ساخت Src_مقصد_dest ent;
  ساخت Src_مقصد_dest ثابت *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_مقصد_dest, &ent);
  واپس e ? e->name : تہی;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

خارجی حرف *
remember_copied (حرف ثابت *name, ino_t ino, dev_t dev)
{
  ساخت Src_مقصد_dest *ent;
  ساخت Src_مقصد_dest *ent_from_جدول;

  ent = xmalloc (حجم *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_جدول = hash_داخل_کرو (src_مقصد_dest, ent);
  اگر (ent_from_جدول == تہی)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  اگر (ent_from_جدول != ent)
    {
      src_مقصد_dest_free (ent);
      واپس ent_from_جدول->name;
    }

  /* New key;  insertion succeeded.  */
  واپس تہی;
}

/* Initialize the hash table.  */
خارجی خالی
hash_init (خالی)
{
  src_مقصد_dest = hash_initialize (INITIAL_جدول_سائز, تہی,
                                 src_مقصد_dest_hash,
                                 src_مقصد_dest_compare,
                                 src_مقصد_dest_free);
  اگر (src_مقصد_dest == تہی)
    xalloc_die ();
}
