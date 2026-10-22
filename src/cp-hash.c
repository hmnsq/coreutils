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

#అన్తర్భావ <config.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ "hash.h"
#అన్తర్భావ "cp-hash.h"

/* Use ST_DEV and ST_INO as the key, FILENAME as the value.
   These are used e.g., in copy.c to associate the destination name with
   the source device/inode pair so that if we encounter a matching dev/ino
   pair in the source tree we can arrange to create a hard link between
   the corresponding names in the destination tree.  */
నిర్మాణం Src_గమ్యం_dest
{
  ino_t st_ino;
  dev_t st_dev;
  /* Destination file name (of non-directory or pre-existing directory)
     corresponding to the dev/ino of a copied file, or the destination file
     name corresponding to a dev/ino pair for a newly-created directory. */
  అక్షరం *name;
};

/* This table maps source dev/ino to destination file name.
   We use it to preserve hard links when copying.  */
స్థిర Hash_పట్టిక *src_గమ్యం_dest;

/* Initial size of the above hash table.  */
#నిర్వచించు INITIAL_పట్టిక_కొలత 103

స్థిర size_t
src_గమ్యం_dest_hash (శూన్య స్థిరరాశి *x, size_t పట్టిక_కొలత)
{
  నిర్మాణం Src_గమ్యం_dest స్థిరరాశి *p = x;

  /* Ignoring the device number here should be fine.  */
  /* The cast to uintmax_t prevents negative remainders
     if st_ino is negative.  */
  తిరిగి (uintmax_t) {p->st_ino} % పట్టిక_కొలత;
}

/* Compare two Src_to_dest entries.
   Return true if their keys are judged 'equal'.  */
స్థిర bool
src_గమ్యం_dest_compare (శూన్య స్థిరరాశి *x, శూన్య స్థిరరాశి *y)
{
  నిర్మాణం Src_గమ్యం_dest స్థిరరాశి *a = x;
  నిర్మాణం Src_గమ్యం_dest స్థిరరాశి *b = y;
  తిరిగి PSAME_INODE (a, b);
}

స్థిర శూన్య
src_గమ్యం_dest_free (శూన్య *x)
{
  నిర్మాణం Src_గమ్యం_dest *a = x;
  విడుదల (a->name);
  విడుదల (x);
}

/* Remove the entry matching INO/DEV from the table
   that maps source ino/dev to destination file name.  */
బాహ్య శూన్య
forget_created (ino_t ino, dev_t dev)
{
  నిర్మాణం Src_గమ్యం_dest probe;
  నిర్మాణం Src_గమ్యం_dest *ent;

  probe.st_ino = ino;
  probe.st_dev = dev;
  probe.name = ఖాళీ;

  ent = hash_remove (src_గమ్యం_dest, &probe);
  ఒకవేళ (ent)
    src_గమ్యం_dest_free (ent);
}

/* If INO/DEV correspond to an already-copied source file, return the
   name of the corresponding destination file.  Otherwise, return NULL.  */

బాహ్య అక్షరం *
src_గమ్యం_dest_lookup (ino_t ino, dev_t dev)
{
  నిర్మాణం Src_గమ్యం_dest ent;
  నిర్మాణం Src_గమ్యం_dest స్థిరరాశి *e;
  ent.st_ino = ino;
  ent.st_dev = dev;
  e = hash_lookup (src_గమ్యం_dest, &ent);
  తిరిగి e ? e->name : ఖాళీ;
}

/* Add file NAME, copied from inode number INO and device number DEV,
   to the list of files we have copied.
   Return NULL if inserted, otherwise a non-null pointer. */

బాహ్య అక్షరం *
remember_copied (అక్షరం స్థిరరాశి *name, ino_t ino, dev_t dev)
{
  నిర్మాణం Src_గమ్యం_dest *ent;
  నిర్మాణం Src_గమ్యం_dest *ent_from_పట్టిక;

  ent = xmalloc (పరిమాణం *ent);
  ent->name = xstrdup (name);
  ent->st_ino = ino;
  ent->st_dev = dev;

  ent_from_పట్టిక = hash_చొప్పించు (src_గమ్యం_dest, ent);
  ఒకవేళ (ent_from_పట్టిక == ఖాళీ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  /* Determine whether there was already an entry in the table
     with a matching key.  If so, free ENT (it wasn't inserted) and
     return the 'name' from the table entry.  */
  ఒకవేళ (ent_from_పట్టిక != ent)
    {
      src_గమ్యం_dest_free (ent);
      తిరిగి ent_from_పట్టిక->name;
    }

  /* New key;  insertion succeeded.  */
  తిరిగి ఖాళీ;
}

/* Initialize the hash table.  */
బాహ్య శూన్య
hash_init (శూన్య)
{
  src_గమ్యం_dest = hash_initialize (INITIAL_పట్టిక_కొలత, ఖాళీ,
                                 src_గమ్యం_dest_hash,
                                 src_గమ్యం_dest_compare,
                                 src_గమ్యం_dest_free);
  ఒకవేళ (src_గమ్యం_dest == ఖాళీ)
    xalloc_die ();
}
