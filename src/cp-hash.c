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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
સંરચના Src_to_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  અક્ષર *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
સ્થિર Hash_table *src_to_dest;

/* Initial size of the above hash table.  */
#define INITIAL_TABLE_SIZE 103

સ્થિર size_t
src_to_dest_hash (શૂન્ય અચલ *x, size_t table_size)
{
  સંરચના Src_to_dest અચલ *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  પાછા (uintmax_t) {p->st_ino} % table_size;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
સ્થિર bool
src_to_dest_compare (શૂન્ય અચલ *x, શૂન્ય અચલ *y)
{
  સંરચના Src_to_dest અચલ *a = x;
  સંરચના Src_to_dest અચલ *b = y;
  પાછા PSAME_INODE (a, b);
}

સ્થિર શૂન્ય
src_to_dest_free (શૂન્ય *x)
{
  સંરચના Src_to_dest *a = x;
  મુક્ત (a->name);
  મુક્ત (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
બાહ્ય શૂન્ય
forget_created (ino_t ino, dev_t dev)
{
  સંરચના Src_to_dest probe;
  સંરચના Src_to_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = ખાલી;

  ent = hash_remove (src_to_dest, &probe);
  જો (ent)
    src_to_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

બાહ્ય અક્ષર *
src_to_dest_lookup (ino_t ino, dev_t dev)
{
  સંરચના Src_to_dest ent;
  સંરચના Src_to_dest અચલ *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_to_dest, &ent);
  પાછા e ? e->name : ખાલી;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

બાહ્ય અક્ષર *
remember_copied (અક્ષર અચલ *name, ino_t ino, dev_t dev)
{
  સંરચના Src_to_dest *ent;
  સંરચના Src_to_dest *ent_from_table;

  ent = xmalloc (કદ *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_table = hash_ઉમેરો (src_to_dest, ent);
  જો (ent_from_table == ખાલી)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  જો (ent_from_table != ent)
    {
      src_to_dest_free (ent);
      પાછા ent_from_table->name;
    }

  /* New key;  insertion succeeded.  */
  પાછા ખાલી;
}

/* Initialize the hash table.  */
બાહ્ય શૂન્ય
hash_init (શૂન્ય)
{
  src_to_dest = hash_initialize (INITIAL_TABLE_SIZE, ખાલી,
                                 src_to_dest_hash,
                                 src_to_dest_compare,
                                 src_to_dest_free);
  જો (src_to_dest == ખાલી)
    xalloc_die ();
}
