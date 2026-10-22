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
ರಚನೆ Src_ಗಮ್ಯ_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  ಅಕ್ಷರ *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
ಸ್ಥಿರ Hash_ಕೋಷ್ಟಕ *src_ಗಮ್ಯ_dest;

/* Initial size of the above hash table.  */
#ವ್ಯಾಖ್ಯಾನಿಸು INITIAL_ಕೋಷ್ಟಕ_ಗಾತ್ರ 103

ಸ್ಥಿರ size_t
src_ಗಮ್ಯ_dest_hash (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *x, size_t ಕೋಷ್ಟಕ_ಗಾತ್ರ)
{
  ರಚನೆ Src_ಗಮ್ಯ_dest ಸ್ಥಿರಾಂಕ *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  ಹಿಂತಿರುಗಿ (uintmax_t) {p->st_ino} % ಕೋಷ್ಟಕ_ಗಾತ್ರ;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
ಸ್ಥಿರ bool
src_ಗಮ್ಯ_dest_compare (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *x, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *y)
{
  ರಚನೆ Src_ಗಮ್ಯ_dest ಸ್ಥಿರಾಂಕ *a = x;
  ರಚನೆ Src_ಗಮ್ಯ_dest ಸ್ಥಿರಾಂಕ *b = y;
  ಹಿಂತಿರುಗಿ PSAME_INODE (a, b);
}

ಸ್ಥಿರ ಶೂನ್ಯ
src_ಗಮ್ಯ_dest_free (ಶೂನ್ಯ *x)
{
  ರಚನೆ Src_ಗಮ್ಯ_dest *a = x;
  ಬಿಡುಗಡೆ (a->name);
  ಬಿಡುಗಡೆ (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
forget_created (ino_t ino, dev_t dev)
{
  ರಚನೆ Src_ಗಮ್ಯ_dest probe;
  ರಚನೆ Src_ಗಮ್ಯ_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = ಖಾಲಿ;

  ent = hash_remove (src_ಗಮ್ಯ_dest, &probe);
  ಆದರೆ (ent)
    src_ಗಮ್ಯ_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

ಬಾಹ್ಯ ಅಕ್ಷರ *
src_ಗಮ್ಯ_dest_lookup (ino_t ino, dev_t dev)
{
  ರಚನೆ Src_ಗಮ್ಯ_dest ent;
  ರಚನೆ Src_ಗಮ್ಯ_dest ಸ್ಥಿರಾಂಕ *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_ಗಮ್ಯ_dest, &ent);
  ಹಿಂತಿರುಗಿ e ? e->name : ಖಾಲಿ;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

ಬಾಹ್ಯ ಅಕ್ಷರ *
remember_copied (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ino_t ino, dev_t dev)
{
  ರಚನೆ Src_ಗಮ್ಯ_dest *ent;
  ರಚನೆ Src_ಗಮ್ಯ_dest *ent_from_ಕೋಷ್ಟಕ;

  ent = xmalloc (ಗಾತ್ರ *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_ಕೋಷ್ಟಕ = hash_ಸೇರಿಸು (src_ಗಮ್ಯ_dest, ent);
  ಆದರೆ (ent_from_ಕೋಷ್ಟಕ == ಖಾಲಿ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  ಆದರೆ (ent_from_ಕೋಷ್ಟಕ != ent)
    {
      src_ಗಮ್ಯ_dest_free (ent);
      ಹಿಂತಿರುಗಿ ent_from_ಕೋಷ್ಟಕ->name;
    }

  /* New key;  insertion succeeded.  */
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/* Initialize the hash table.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
hash_init (ಶೂನ್ಯ)
{
  src_ಗಮ್ಯ_dest = hash_initialize (INITIAL_ಕೋಷ್ಟಕ_ಗಾತ್ರ, ಖಾಲಿ,
                                 src_ಗಮ್ಯ_dest_hash,
                                 src_ಗಮ್ಯ_dest_compare,
                                 src_ಗಮ್ಯ_dest_free);
  ಆದರೆ (src_ಗಮ್ಯ_dest == ಖಾಲಿ)
    xalloc_die ();
}
