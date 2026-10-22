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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
ഘടന Src_to_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  അക്ഷരം *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
സ്ഥിതം Hash_table *src_to_dest;

/* Initial size of the above hash table.  */
#define INITIAL_TABLE_SIZE 103

സ്ഥിതം size_t
src_to_dest_hash (ശൂന്യം സ്ഥിരം *x, size_t table_size)
{
  ഘടന Src_to_dest സ്ഥിരം *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  തിരികെ (uintmax_t) {p->st_ino} % table_size;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
സ്ഥിതം bool
src_to_dest_compare (ശൂന്യം സ്ഥിരം *x, ശൂന്യം സ്ഥിരം *y)
{
  ഘടന Src_to_dest സ്ഥിരം *a = x;
  ഘടന Src_to_dest സ്ഥിരം *b = y;
  തിരികെ PSAME_INODE (a, b);
}

സ്ഥിതം ശൂന്യം
src_to_dest_free (ശൂന്യം *x)
{
  ഘടന Src_to_dest *a = x;
  സ്വതന്ത്രം (a->name);
  സ്വതന്ത്രം (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
ബാഹ്യ ശൂന്യം
forget_created (ino_t ino, dev_t dev)
{
  ഘടന Src_to_dest probe;
  ഘടന Src_to_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = ശൂന്യ;

  ent = hash_remove (src_to_dest, &probe);
  എങ്കിൽ (ent)
    src_to_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

ബാഹ്യ അക്ഷരം *
src_to_dest_lookup (ino_t ino, dev_t dev)
{
  ഘടന Src_to_dest ent;
  ഘടന Src_to_dest സ്ഥിരം *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_to_dest, &ent);
  തിരികെ e ? e->name : ശൂന്യ;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

ബാഹ്യ അക്ഷരം *
remember_copied (അക്ഷരം സ്ഥിരം *name, ino_t ino, dev_t dev)
{
  ഘടന Src_to_dest *ent;
  ഘടന Src_to_dest *ent_from_table;

  ent = xmalloc (വലുപ്പം *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_table = hash_തിരുകുക (src_to_dest, ent);
  എങ്കിൽ (ent_from_table == ശൂന്യ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  എങ്കിൽ (ent_from_table != ent)
    {
      src_to_dest_free (ent);
      തിരികെ ent_from_table->name;
    }

  /* New key;  insertion succeeded.  */
  തിരികെ ശൂന്യ;
}

/* Initialize the hash table.  */
ബാഹ്യ ശൂന്യം
hash_init (ശൂന്യം)
{
  src_to_dest = hash_initialize (INITIAL_TABLE_SIZE, ശൂന്യ,
                                 src_to_dest_hash,
                                 src_to_dest_compare,
                                 src_to_dest_free);
  എങ്കിൽ (src_to_dest == ശൂന്യ)
    xalloc_die ();
}
