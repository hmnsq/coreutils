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

#अन्तर्भाव <config.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "hash.h"
#अन्तर्भाव "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
संरचना Src_to_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  वर्ण *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
स्थिर Hash_table *src_to_dest;

/* Initial size of the above hash table.  */
#define INITIAL_TABLE_SIZE 103

स्थिर माप_प्रकार
src_to_dest_hash (शून्य ध्रुव *x, माप_प्रकार table_size)
{
  संरचना Src_to_dest ध्रुव *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  प्रत्यागम (uintmax_t) {p->st_ino} % table_size;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
स्थिर bool
src_to_dest_compare (शून्य ध्रुव *x, शून्य ध्रुव *y)
{
  संरचना Src_to_dest ध्रुव *a = x;
  संरचना Src_to_dest ध्रुव *b = y;
  प्रत्यागम PSAME_INODE (a, b);
}

स्थिर शून्य
src_to_dest_free (शून्य *x)
{
  संरचना Src_to_dest *a = x;
  मुक्त (a->name);
  मुक्त (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
बाह्य शून्य
forget_created (ino_t ino, dev_t dev)
{
  संरचना Src_to_dest probe;
  संरचना Src_to_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = रिक्त;

  ent = hash_remove (src_to_dest, &probe);
  यदि (ent)
    src_to_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

बाह्य वर्ण *
src_to_dest_lookup (ino_t ino, dev_t dev)
{
  संरचना Src_to_dest ent;
  संरचना Src_to_dest ध्रुव *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_to_dest, &ent);
  प्रत्यागम e ? e->name : रिक्त;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

बाह्य वर्ण *
remember_copied (वर्ण ध्रुव *name, ino_t ino, dev_t dev)
{
  संरचना Src_to_dest *ent;
  संरचना Src_to_dest *ent_from_table;

  ent = xmalloc (परिमाण *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_table = hash_जोड़ो (src_to_dest, ent);
  यदि (ent_from_table == रिक्त)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  यदि (ent_from_table != ent)
    {
      src_to_dest_free (ent);
      प्रत्यागम ent_from_table->name;
    }

  /* New key;  insertion succeeded.  */
  प्रत्यागम रिक्त;
}

/* Initialize the hash table.  */
बाह्य शून्य
hash_init (शून्य)
{
  src_to_dest = hash_initialize (INITIAL_TABLE_SIZE, रिक्त,
                                 src_to_dest_hash,
                                 src_to_dest_compare,
                                 src_to_dest_free);
  यदि (src_to_dest == रिक्त)
    xalloc_die ();
}
