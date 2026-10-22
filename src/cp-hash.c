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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
ಸಂರಚನಾ Src_to_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  ವರ್ಣ *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
ಸ್ಥಿರ Hash_table *src_to_dest;

/* Initial size of the above hash table.  */
#define INITIAL_TABLE_SIZE 103

ಸ್ಥಿರ size_t
src_to_dest_hash (ಶೂನ್ಯ ಧ್ರುವ *x, size_t table_size)
{
  ಸಂರಚನಾ Src_to_dest ಧ್ರುವ *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  ಪ್ರತ್ಯಾಗಮ (uintmax_t) {p->st_ino} % table_size;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
ಸ್ಥಿರ bool
src_to_dest_compare (ಶೂನ್ಯ ಧ್ರುವ *x, ಶೂನ್ಯ ಧ್ರುವ *y)
{
  ಸಂರಚನಾ Src_to_dest ಧ್ರುವ *a = x;
  ಸಂರಚನಾ Src_to_dest ಧ್ರುವ *b = y;
  ಪ್ರತ್ಯಾಗಮ PSAME_INODE (a, b);
}

ಸ್ಥಿರ ಶೂನ್ಯ
src_to_dest_free (ಶೂನ್ಯ *x)
{
  ಸಂರಚನಾ Src_to_dest *a = x;
  ಮುಕ್ತ (a->name);
  ಮುಕ್ತ (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
forget_created (ino_t ino, dev_t dev)
{
  ಸಂರಚನಾ Src_to_dest probe;
  ಸಂರಚನಾ Src_to_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = ರಿಕ್ತ;

  ent = hash_remove (src_to_dest, &probe);
  ಯದಿ (ent)
    src_to_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

ಬಾಹ್ಯ ವರ್ಣ *
src_to_dest_lookup (ino_t ino, dev_t dev)
{
  ಸಂರಚನಾ Src_to_dest ent;
  ಸಂರಚನಾ Src_to_dest ಧ್ರುವ *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_to_dest, &ent);
  ಪ್ರತ್ಯಾಗಮ e ? e->name : ರಿಕ್ತ;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

ಬಾಹ್ಯ ವರ್ಣ *
remember_copied (ವರ್ಣ ಧ್ರುವ *name, ino_t ino, dev_t dev)
{
  ಸಂರಚನಾ Src_to_dest *ent;
  ಸಂರಚನಾ Src_to_dest *ent_from_table;

  ent = xmalloc (ಪರಿಮಾಣ *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_table = hash_ಸೇರಿಸು (src_to_dest, ent);
  ಯದಿ (ent_from_table == ರಿಕ್ತ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  ಯದಿ (ent_from_table != ent)
    {
      src_to_dest_free (ent);
      ಪ್ರತ್ಯಾಗಮ ent_from_table->name;
    }

  /* New key;  insertion succeeded.  */
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/* Initialize the hash table.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
hash_init (ಶೂನ್ಯ)
{
  src_to_dest = hash_initialize (INITIAL_TABLE_SIZE, ರಿಕ್ತ,
                                 src_to_dest_hash,
                                 src_to_dest_compare,
                                 src_to_dest_free);
  ಯದಿ (src_to_dest == ರಿಕ್ತ)
    xalloc_die ();
}
