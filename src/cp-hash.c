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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  ਅੱਖਰ *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
ਸਥਿਰ Hash_ਸਾਰਣੀ *src_ਮੰਜ਼ਿਲ_dest;

/* Initial size of the above hash table.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INITIAL_ਸਾਰਣੀ_ਮਾਪ 103

ਸਥਿਰ size_t
src_ਮੰਜ਼ਿਲ_dest_hash (ਖਾਲੀ ਅਚਲ *x, size_t ਸਾਰਣੀ_ਮਾਪ)
{
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest ਅਚਲ *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  ਵਾਪਸ (uintmax_t) {p->st_ino} % ਸਾਰਣੀ_ਮਾਪ;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
ਸਥਿਰ bool
src_ਮੰਜ਼ਿਲ_dest_compare (ਖਾਲੀ ਅਚਲ *x, ਖਾਲੀ ਅਚਲ *y)
{
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest ਅਚਲ *a = x;
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest ਅਚਲ *b = y;
  ਵਾਪਸ PSAME_INODE (a, b);
}

ਸਥਿਰ ਖਾਲੀ
src_ਮੰਜ਼ਿਲ_dest_free (ਖਾਲੀ *x)
{
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest *a = x;
  ਮੁਕਤ (a->name);
  ਮੁਕਤ (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
ਬਾਹਰੀ ਖਾਲੀ
forget_created (ino_t ino, dev_t dev)
{
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest probe;
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = ਨਿਰਾ;

  ent = hash_remove (src_ਮੰਜ਼ਿਲ_dest, &probe);
  ਜੇ (ent)
    src_ਮੰਜ਼ਿਲ_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

ਬਾਹਰੀ ਅੱਖਰ *
src_ਮੰਜ਼ਿਲ_dest_lookup (ino_t ino, dev_t dev)
{
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest ent;
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest ਅਚਲ *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_ਮੰਜ਼ਿਲ_dest, &ent);
  ਵਾਪਸ e ? e->name : ਨਿਰਾ;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

ਬਾਹਰੀ ਅੱਖਰ *
remember_copied (ਅੱਖਰ ਅਚਲ *name, ino_t ino, dev_t dev)
{
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest *ent;
  ਬਣਤਰ Src_ਮੰਜ਼ਿਲ_dest *ent_from_ਸਾਰਣੀ;

  ent = xmalloc (ਆਕਾਰ *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_ਸਾਰਣੀ = hash_ਪਾਓ (src_ਮੰਜ਼ਿਲ_dest, ent);
  ਜੇ (ent_from_ਸਾਰਣੀ == ਨਿਰਾ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  ਜੇ (ent_from_ਸਾਰਣੀ != ent)
    {
      src_ਮੰਜ਼ਿਲ_dest_free (ent);
      ਵਾਪਸ ent_from_ਸਾਰਣੀ->name;
    }

  /* New key;  insertion succeeded.  */
  ਵਾਪਸ ਨਿਰਾ;
}

/* Initialize the hash table.  */
ਬਾਹਰੀ ਖਾਲੀ
hash_init (ਖਾਲੀ)
{
  src_ਮੰਜ਼ਿਲ_dest = hash_initialize (INITIAL_ਸਾਰਣੀ_ਮਾਪ, ਨਿਰਾ,
                                 src_ਮੰਜ਼ਿਲ_dest_hash,
                                 src_ਮੰਜ਼ਿਲ_dest_compare,
                                 src_ਮੰਜ਼ਿਲ_dest_free);
  ਜੇ (src_ਮੰਜ਼ਿਲ_dest == ਨਿਰਾ)
    xalloc_die ();
}
