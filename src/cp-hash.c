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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
সংরচনা Src_to_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  বর্ণ *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
স্থির Hash_table *src_to_dest;

/* Initial size of the above hash table.  */
#define INITIAL_TABLE_SIZE 103

স্থির size_t
src_to_dest_hash (শূন্য ধ্রুব *x, size_t table_size)
{
  সংরচনা Src_to_dest ধ্রুব *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  প্রত্যাগম (uintmax_t) {p->st_ino} % table_size;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
স্থির bool
src_to_dest_compare (শূন্য ধ্রুব *x, শূন্য ধ্রুব *y)
{
  সংরচনা Src_to_dest ধ্রুব *a = x;
  সংরচনা Src_to_dest ধ্রুব *b = y;
  প্রত্যাগম PSAME_INODE (a, b);
}

স্থির শূন্য
src_to_dest_free (শূন্য *x)
{
  সংরচনা Src_to_dest *a = x;
  মুক্ত (a->name);
  মুক্ত (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
বাহ্য শূন্য
forget_created (ino_t ino, dev_t dev)
{
  সংরচনা Src_to_dest probe;
  সংরচনা Src_to_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = রিক্ত;

  ent = hash_remove (src_to_dest, &probe);
  যদি (ent)
    src_to_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

বাহ্য বর্ণ *
src_to_dest_lookup (ino_t ino, dev_t dev)
{
  সংরচনা Src_to_dest ent;
  সংরচনা Src_to_dest ধ্রুব *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_to_dest, &ent);
  প্রত্যাগম e ? e->name : রিক্ত;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

বাহ্য বর্ণ *
remember_copied (বর্ণ ধ্রুব *name, ino_t ino, dev_t dev)
{
  সংরচনা Src_to_dest *ent;
  সংরচনা Src_to_dest *ent_from_table;

  ent = xmalloc (পরিমাণ *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_table = hash_ঢোকাও (src_to_dest, ent);
  যদি (ent_from_table == রিক্ত)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  যদি (ent_from_table != ent)
    {
      src_to_dest_free (ent);
      প্রত্যাগম ent_from_table->name;
    }

  /* New key;  insertion succeeded.  */
  প্রত্যাগম রিক্ত;
}

/* Initialize the hash table.  */
বাহ্য শূন্য
hash_init (শূন্য)
{
  src_to_dest = hash_initialize (INITIAL_TABLE_SIZE, রিক্ত,
                                 src_to_dest_hash,
                                 src_to_dest_compare,
                                 src_to_dest_free);
  যদি (src_to_dest == রিক্ত)
    xalloc_die ();
}
