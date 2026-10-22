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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
கட்டமைப்பு Src_to_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  எழுத்து *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
நிலையான Hash_table *src_to_dest;

/* Initial size of the above hash table.  */
#define INITIAL_TABLE_SIZE 103

நிலையான size_t
src_to_dest_hash (வெறுமை மாறிலி *x, size_t table_size)
{
  கட்டமைப்பு Src_to_dest மாறிலி *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  திரும்பு (uintmax_t) {p->st_ino} % table_size;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
நிலையான bool
src_to_dest_compare (வெறுமை மாறிலி *x, வெறுமை மாறிலி *y)
{
  கட்டமைப்பு Src_to_dest மாறிலி *a = x;
  கட்டமைப்பு Src_to_dest மாறிலி *b = y;
  திரும்பு PSAME_INODE (a, b);
}

நிலையான வெறுமை
src_to_dest_free (வெறுமை *x)
{
  கட்டமைப்பு Src_to_dest *a = x;
  விடுவி (a->name);
  விடுவி (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
வெளி வெறுமை
forget_created (ino_t ino, dev_t dev)
{
  கட்டமைப்பு Src_to_dest probe;
  கட்டமைப்பு Src_to_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = வெற்று;

  ent = hash_remove (src_to_dest, &probe);
  எனில் (ent)
    src_to_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

வெளி எழுத்து *
src_to_dest_lookup (ino_t ino, dev_t dev)
{
  கட்டமைப்பு Src_to_dest ent;
  கட்டமைப்பு Src_to_dest மாறிலி *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_to_dest, &ent);
  திரும்பு e ? e->name : வெற்று;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

வெளி எழுத்து *
remember_copied (எழுத்து மாறிலி *name, ino_t ino, dev_t dev)
{
  கட்டமைப்பு Src_to_dest *ent;
  கட்டமைப்பு Src_to_dest *ent_from_table;

  ent = xmalloc (அளவு *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_table = hash_செருகு (src_to_dest, ent);
  எனில் (ent_from_table == வெற்று)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  எனில் (ent_from_table != ent)
    {
      src_to_dest_free (ent);
      திரும்பு ent_from_table->name;
    }

  /* New key;  insertion succeeded.  */
  திரும்பு வெற்று;
}

/* Initialize the hash table.  */
வெளி வெறுமை
hash_init (வெறுமை)
{
  src_to_dest = hash_initialize (INITIAL_TABLE_SIZE, வெற்று,
                                 src_to_dest_hash,
                                 src_to_dest_compare,
                                 src_to_dest_free);
  எனில் (src_to_dest == வெற்று)
    xalloc_die ();
}
