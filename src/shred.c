/* shred.c - overwrite files and devices to make it harder to recover data

   Copyright (C) 1999-2026 Free Software Foundation, Inc.
   Copyright (C) 1997, 1998, 1999 Colin Plumb.

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

   Written by Colin Plumb.  */

/*
 * Do a more secure overwrite of given files or devices, to make it harder
 * for even very expensive hardware probing to recover the data.
 *
 * Although this process is also known as "wiping", I prefer the longer
 * name both because I think it is more evocative of what is happening and
 * because a longer name conveys a more appropriate sense of deliberateness.
 *
 * For the theory behind this, see "Secure Deletion of Data from Magnetic
 * and Solid-State Memory", on line at
 * https://www.cs.auckland.ac.nz/~pgut001/pubs/secure_del.html
 *
 * Just for the record, reversing one or two passes of disk overwrite
 * is not terribly difficult with hardware help.  Hook up a good-quality
 * digitizing oscilloscope to the output of the head preamplifier and copy
 * the high-res digitized data to a computer for some off-line analysis.
 * Read the "current" data and average all the pulses together to get an
 * "average" pulse on the disk.  Subtract this average pulse from all of
 * the actual pulses and you can clearly see the "echo" of the previous
 * data on the disk.
 *
 * Real hard drives have to balance the cost of the media, the head,
 * and the read circuitry.  They use better-quality media than absolutely
 * necessary to limit the cost of the read circuitry.  By throwing that
 * assumption out, and the assumption that you want the data processed
 * as fast as the hard drive can spin, you can do better.
 *
 * If asked to wipe a file, this also unlinks it, renaming it in a
 * clever way to try to leave no trace of the original filename.
 *
 * This was inspired by a desire to improve on some code titled:
 * Wipe V1.0-- Overwrite and delete files.  S. 2/3/96
 * but I've rewritten everything here so completely that no trace of
 * the original remains.
 *
 * Thanks to:
 * Bob Jenkins, for his good RNG work and patience with the FSF copyright
 * paperwork.
 * Jim Meyering, for his work merging this into the GNU fileutils while
 * still letting me feel a sense of ownership and pride.  Getting me to
 * tolerate the GNU brace style was quite a feat of diplomacy.
 * Paul Eggert, for lots of useful discussion and code.  I disagree with
 * an awful lot of his suggestions, but they're disagreements worth having.
 *
 * Things to think about:
 * - Security: Is there any risk to the race
 *   between overwriting and unlinking a file?  Will it do anything
 *   drastically bad if told to attack a named pipe or socket?
 */

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "shred"

#घोषणा AUTHORS proper_name ("Colin Plumb")

#समावेश <config.h>

#समावेश <getopt.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#अगर defined __linux__ && HAVE_SYS_MTIO_H
# include <sys/mtio.h>
#पूर्ण_अगर

#समावेश "system.h"
#समावेश "alignalloc.h"
#समावेश "argmatch.h"
#समावेश "assure.h"
#समावेश "xdectoint.h"
#समावेश "fcntl--.h"
#समावेश "gethrxtime.h"
#समावेश "human.h"
#समावेश "randint.h"
#समावेश "randread.h"
#समावेश "renameatu.h"
#समावेश "stat-size.h"

/* Default number of times to overwrite.  */
क्रमागत { DEFAULT_PASSES = 3 };

/* How many seconds to wait before checking whether to output another
   verbose output line.  */
क्रमागत { VERBOSE_UPDATE = 5 };

/* Sector size and corresponding mask, for recovering after write failures.
   The size must be a power of 2.  */
क्रमागत { SECTOR_आकार = 512 };
क्रमागत { SECTOR_MASK = SECTOR_आकार - 1 };
static_assert (0 < SECTOR_आकार && (SECTOR_आकार & SECTOR_MASK) == 0);

क्रमागत remove_method
{
  remove_none = 0,      /* the default: only wipe data.  */
  remove_unlink,        /* don't obfuscate name, just unlink.  */
  remove_wipe,          /* obfuscate name before unlink.  */
  remove_wipesync       /* obfuscate name, syncing each byte, before unlink.  */
};

अटल अक्षर स्थिर *स्थिर remove_args[] =
{
  "unlink", "wipe", "wipesync", शून्य
};

अटल क्रमागत remove_method स्थिर remove_methods[] =
{
  remove_unlink, remove_wipe, remove_wipesync
};

काष्ठा Options
{
  bool force;		/* -f flag: chmod files if necessary */
  माप_प्रकार n_iterations;	/* -n flag: Number of iterations */
  off_t आकार;		/* -s flag: size of file */
  क्रमागत remove_method remove_file; /* -u flag: remove file after shredding */
  bool verbose;		/* -v flag: Print progress */
  bool exact;		/* -x flag: Do not round up file size */
  bool zero_fill;	/* -z flag: Add a final zero pass */
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  RANDOM_SOURCE_OPTION = अक्षर_उच्च + 1
};

अटल काष्ठा option स्थिर long_opts[] =
{
  {"exact", no_argument, शून्य, 'x'},
  {"force", no_argument, शून्य, 'f'},
  {"iterations", required_argument, शून्य, 'n'},
  {"size", required_argument, शून्य, 's'},
  {"random-source", required_argument, शून्य, RANDOM_SOURCE_OPTION},
  {"remove", optional_argument, शून्य, 'u'},
  {"verbose", no_argument, शून्य, 'v'},
  {"zero", no_argument, शून्य, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      ख_माला_दो (_("\
Overwrite the specified खाता(s) repeatedly, in order to make it harder\n\
क्रम even very expensive hardware probing to recover the तथ्य.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
If खाता is -, shred standard output.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --force\n\
         change permissions to allow writing अगर necessary\n\
"));
      oprintf (_("\
  -n, --iterations=N\n\
         overwrite N बार instead of the शेष (%d)\n\
"), DEFAULT_PASSES);
      oputs (_("\
      --random-source=खाता\n\
         get random bytes from खाता\n\
"));
      oputs (_("\
  -s, --आकार=N\n\
         shred this many bytes (suffixes like K, M, G accepted)\n\
"));
      oputs (_("\
  -u\n\
         deallocate and हटाओ file after overwriting\n\
"));
      oputs (_("\
      --हटाओ[=HOW]\n\
         like -u but give control on HOW to delete;  See below\n\
"));
      oputs (_("\
  -v, --verbose\n\
         show details of तथ्य and metadata operations performed\n\
"));
      oputs (_("\
  -x, --exact\n\
         करो not round file sizes up to the अग्र full block;\n\
         this is the शेष क्रम non-regular files\n\
"));
      oputs (_("\
  -z, --zero\n\
         add a final overwrite with zeros to hide shredding\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
खाता will be skipped with a diagnostic message अगर it is a FIFO, socket, or\n\
terminal, since its तथ्य does not reside on disk.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Delete खाता(s) अगर --हटाओ (-u) is specified.  The शेष is not to हटाओ\n\
the files because it is common to operate on device files like /dev/hda,\n\
and those files usually should not be removed.\n\
The optional HOW parameter indicates how to हटाओ a directory entry:\n\
'unlink' => use a standard unlink call.\n\
'wipe' => also first obfuscate bytes in the name.\n\
'wipesync' => also sync each obfuscated byte to the device.\n\
The शेष mode is 'wipesync', but note it can be expensive.\n\
\n\
"), मानक_निकास);
      ख_माला_दो (_("\
CAUTION: shred assumes the file प्रणाली and hardware overwrite तथ्य in place.\n\
Although this is common, many platforms operate otherwise.  Also, backups\n\
and mirrors may contain unremovable copies that will let a shredded file\n\
be recovered later.  See the GNU coreutils manual क्रम details.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/*
 * Determine if pattern type is periodic or not.
 */
अटल bool
periodic_pattern (पूर्णांक type)
{
  अगर (type <= 0)
    वापस false;

  अचिन्हित अक्षर r[3];
  अचिन्हित पूर्णांक bits = type & 0xfff;

  bits |= bits << 12;
  r[0] = (bits >> 4) & 255;
  r[1] = (bits >> 8) & 255;
  r[2] = bits & 255;

  वापस (r[0] != r[1]) || (r[0] != r[2]);
}

/*
 * Fill a buffer with a fixed pattern.
 *
 * The buffer must be at least 3 bytes long, even if
 * size is less.  Larger sizes are filled exactly.
 */
अटल व्योम
fillpattern (पूर्णांक type, अचिन्हित अक्षर *r, माप_प्रकार आकार)
{
  माप_प्रकार i;
  अचिन्हित पूर्णांक bits = type & 0xfff;

  bits |= bits << 12;
  r[0] = (bits >> 4) & 255;
  r[1] = (bits >> 8) & 255;
  r[2] = bits & 255;
  क्रम (i = 3; i <= आकार / 2; i *= 2)
    स_नकल (r + i, r, i);
  अगर (i < आकार)
    स_नकल (r + i, r, आकार - i);

  /* Invert the first bit of every sector. */
  अगर (type & 0x1000)
    क्रम (i = 0; i < आकार; i += SECTOR_आकार)
      r[i] ^= 0x80;
}

/*
 * Generate a 6-character (+ nul) pass name string
 * FIXME: allow translation of "random".
 */
#घोषणा PASS_NAME_आकार 7
अटल व्योम
passname (अचिन्हित अक्षर स्थिर *तथ्य, अक्षर name[PASS_NAME_आकार])
{
  अगर (तथ्य)
    प्र_लिखो (name, "%02x%02x%02x", तथ्य[0], तथ्य[1], तथ्य[2]);
  अन्यथा
    स_नकल (name, "random", PASS_NAME_आकार);
}

/* Return true when it's ok to ignore an fsync or fdatasync
   failure that set errno to ERRNO_VAL.  */
अटल bool
ignorable_sync_errno (पूर्णांक errno_val)
{
  वापस (errno_val == EINVAL
          || errno_val == EBADF
          /* HP-UX does this */
          || errno_val == EISDIR);
}

/* Request that all data for FD be transferred to the corresponding
   storage device.  QNAME is the file name (quoted for colons).
   Report any errors found.  Return 0 on success, -1
   (setting errno) on failure.  It is not an error if fdatasync and/or
   fsync is not supported for this file, or if the file is not a
   writable file descriptor.  */
अटल पूर्णांक
dosync (पूर्णांक fd, अक्षर स्थिर *qname)
{
  पूर्णांक err;

  अगर (fdatasync (fd) == 0)
    वापस 0;
  err = त्रुटि_सं;
  अगर ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fdatasync failed"), qname);
      त्रुटि_सं = err;
      वापस -1;
    }

  अगर (fsync (fd) == 0)
    वापस 0;
  err = त्रुटि_सं;
  अगर ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fsync failed"), qname);
      त्रुटि_सं = err;
      वापस -1;
    }

  sync ();
  वापस 0;
}

/* Turn on or off direct I/O mode for file descriptor FD, if possible.
   Try to turn it on if ENABLE is true.  Otherwise, try to turn it off.  */
अटल व्योम
direct_mode (पूर्णांक fd, bool enable)
{
  अगर (O_DIRECT)
    {
      पूर्णांक fd_flags = fcntl (fd, F_GETFL);
      अगर (0 < fd_flags)
        {
          पूर्णांक new_flags = (enable
                           ? (fd_flags | O_DIRECT)
                           : (fd_flags & ~O_DIRECT));
          अगर (new_flags != fd_flags)
            fcntl (fd, F_SETFL, new_flags);
        }
    }

#अगर HAVE_DIRECTIO && defined DIRECTIO_ON && defined DIRECTIO_OFF
  /* This is Solaris-specific.  */
  directio (fd, enable ? DIRECTIO_ON : DIRECTIO_OFF);
#पूर्ण_अगर
}

/* Rewind FD; its status is ST.  */
अटल bool
dorewind (पूर्णांक fd, काष्ठा stat स्थिर *st)
{
  अगर (S_ISCHR (st->st_mode))
    {
#अगर defined __linux__ && HAVE_SYS_MTIO_H
      /* In the Linux kernel, lseek does not work on tape devices; it
         returns a randomish value instead.  Try the low-level tape
         rewind operation first.  */
      काष्ठा mtop op;
      op.mt_op = MTREW;
      op.mt_गिनती = 1;
      अगर (ioctl (fd, MTIOCTOP, &op) == 0)
        वापस true;
#पूर्ण_अगर
    }
  off_t offset = lseek (fd, 0, शुरू_से);
  अगर (0 < offset)
    त्रुटि_सं = EINVAL;
  वापस offset == 0;
}

/* By convention, negative sizes represent unknown values.  */

अटल bool
known (off_t आकार)
{
  वापस 0 <= आकार;
}

/*
 * Do pass number K of N, writing *SIZEP bytes of the given pattern TYPE
 * to the file descriptor FD.  K and N are passed in only for verbose
 * progress message purposes.  If N == 0, no progress messages are printed.
 *
 * If *SIZEP == -1, the size is unknown, and it will be filled in as soon
 * as writing fails with ENOSPC.
 *
 * Return 1 on write error, -1 on other error, 0 on success.
 */
अटल पूर्णांक
dopass (पूर्णांक fd, काष्ठा stat स्थिर *st, अक्षर स्थिर *qname, off_t *sizep,
        पूर्णांक type, काष्ठा randread_source *s,
        अचिन्हित दीर्घ पूर्णांक k, अचिन्हित दीर्घ पूर्णांक n)
{
  off_t आकार = *sizep;
  off_t offset;                /* Current file position */
  xtime_t पूर्व IF_LINT ( = 0); /* Time we printed the previous update.  */
  xtime_t now = 0;             /* Current time */
  माप_प्रकार lim;                  /* Amount of data to try writing */
  माप_प्रकार soff;                 /* Offset into buffer for next write */
  ssize_t ssize;               /* Return value from write */

  /* Fill pattern buffer.  Aligning it to a page so we can do direct I/O.  */
  माप_प्रकार page_आकार = getpagesize ();
#घोषणा PERIODIC_OUTPUT_आकार (60 * 1024)
#घोषणा NONPERIODIC_OUTPUT_आकार (64 * 1024)
  static_assert (PERIODIC_OUTPUT_आकार % 3 == 0);
  माप_प्रकार output_आकार = periodic_pattern (type)
                       ? PERIODIC_OUTPUT_आकार : NONPERIODIC_OUTPUT_आकार;
#घोषणा FILLPATTERN_आकार (((output_आकार + 2) / 3) * 3) /* Multiple of 3 */
  अचिन्हित अक्षर *pbuf = xalignalloc (page_आकार, FILLPATTERN_आकार);

  अक्षर pass_string[PASS_NAME_आकार];	/* Name of current pass */
  bool write_error = false;
  bool other_error = false;

  /* Printable previous offset into the file */
  अक्षर previous_offset_buf[LONGEST_HUMAN_READABLE + 1];
  अक्षर स्थिर *previous_human_offset;

  /* As a performance tweak, avoid direct I/O for small sizes,
     as it's just a performance rather than security consideration,
     and direct I/O can often be unsupported for small non aligned sizes.  */
  bool try_without_directio = 0 < आकार && आकार < output_आकार;
  अगर (! try_without_directio)
    direct_mode (fd, true);

  अगर (! dorewind (fd, st))
    {
      error (0, त्रुटि_सं, _("%s: cannot rewind"), qname);
      other_error = true;
      जाओ free_pattern_mem;
    }

  /* Constant fill patterns need only be set up once. */
  अगर (type >= 0)
    {
      lim = known (आकार) && आकार < FILLPATTERN_आकार ? आकार : FILLPATTERN_आकार;
      fillpattern (type, pbuf, lim);
      passname (pbuf, pass_string);
    }
  अन्यथा
    {
      passname (शून्य, pass_string);
    }

  /* Set position if first status update */
  अगर (n)
    {
      error (0, 0, _("%s: pass %lu/%lu (%s)..."), qname, k, n, pass_string);
      पूर्व = gethrxtime ();
      previous_human_offset = "";
    }

  offset = 0;
  जबतक (true)
    {
      /* How much to write this time? */
      lim = output_आकार;
      अगर (known (आकार) && आकार - offset < output_आकार)
        {
          अगर (आकार < offset)
            अवरोध;
          lim = आकार - offset;
          अगर (!lim)
            अवरोध;
        }
      अगर (type < 0)
        randread (s, pbuf, lim);
      /* Loop to retry partial writes. */
      क्रम (soff = 0; soff < lim; soff += ssize)
        {
          ssize = ग_लिखो (fd, pbuf + soff, lim - soff);
          अगर (ssize <= 0)
            {
              अगर (! known (आकार) && (ssize == 0 || त्रुटि_सं == ENOSPC))
                {
                  /* We have found the end of the file.  */
                  अगर (soff <= OFF_T_MAX - offset)
                    *sizep = आकार = offset + soff;
                  अवरोध;
                }
              अन्यथा
                {
                  पूर्णांक errnum = त्रुटि_सं;

                  /* Retry without direct I/O since this may not be supported
                     at all on some (file) systems, or with the current size.
                     I.e., a specified --size that is not aligned, or when
                     dealing with slop at the end of a file with --exact.  */
                  अगर (! try_without_directio && त्रुटि_सं == EINVAL)
                    {
                      direct_mode (fd, false);
                      ssize = 0;
                      try_without_directio = true;
                      जारी;
                    }
                  uintmax_t error_offset = offset + soff;
                  error (0, errnum, _("%s: error writing at offset %ju"),
                         qname, error_offset);

                  /* 'shred' is often used on bad media, before throwing it
                     out.  Thus, it shouldn't give up on bad blocks.  This
                     code works because lim is always a multiple of
                     SECTOR_SIZE, except at the end.  This size constraint
                     also enables direct I/O on some (file) systems.  */
                  static_assert (PERIODIC_OUTPUT_आकार % SECTOR_आकार == 0);
                  static_assert (NONPERIODIC_OUTPUT_आकार % SECTOR_आकार == 0);
                  अगर (errnum == EIO && known (आकार)
                      && (soff | SECTOR_MASK) < lim)
                    {
                      माप_प्रकार soff1 = (soff | SECTOR_MASK) + 1;
                      अगर (lseek (fd, offset + soff1, शुरू_से) != -1)
                        {
                          /* Arrange to skip this block. */
                          ssize = soff1 - soff;
                          write_error = true;
                          जारी;
                        }
                      error (0, त्रुटि_सं, _("%s: lseek failed"), qname);
                    }
                  other_error = true;
                  जाओ free_pattern_mem;
                }
            }
        }

      /* Okay, we have written "soff" bytes. */

      अगर (ckd_add (&offset, offset, soff))
        {
          error (0, 0, _("%s: file too large"), qname);
          other_error = true;
          जाओ free_pattern_mem;
        }

      bool done = offset == आकार;

      /* Time to print progress? */
      अगर (n && ((done && *previous_human_offset)
                || VERBOSE_UPDATE <= xtime_sec ((now = gethrxtime ()) - पूर्व)))
        {
          अक्षर offset_buf[LONGEST_HUMAN_READABLE + 1];
          अक्षर आकार_buf[LONGEST_HUMAN_READABLE + 1];
          पूर्णांक human_progress_opts = (human_autoscale | human_SI
                                     | human_base_1024 | human_B);
          अक्षर स्थिर *human_offset
            = human_readable (offset, offset_buf,
                              human_floor | human_progress_opts, 1, 1);

          अगर (done || !streq (previous_human_offset, human_offset))
            {
              अगर (! known (आकार))
                error (0, 0, _("%s: pass %lu/%lu (%s)...%s"),
                       qname, k, n, pass_string, human_offset);
              अन्यथा
                {
                  uintmax_t off = offset;
                  पूर्णांक percent = (आकार == 0
                                 ? 100
                                 : (off <= UINTMAX_MAX / 100
                                    ? off * 100 / आकार
                                    : off / (आकार / 100)));
                  अक्षर स्थिर *human_आकार
                    = human_readable (आकार, आकार_buf,
                                      human_ceiling | human_progress_opts,
                                      1, 1);
                  अगर (done)
                    human_offset = human_आकार;
                  error (0, 0, _("%s: pass %lu/%lu (%s)...%s/%s %d%%"),
                         qname, k, n, pass_string, human_offset, human_आकार,
                         percent);
                }

              म_नकल (previous_offset_buf, human_offset);
              previous_human_offset = previous_offset_buf;
              पूर्व = now;

              /*
               * Force periodic syncs to keep displayed progress accurate
               * FIXME: Should these be present even if -v is not enabled,
               * to keep the buffer cache from filling with dirty pages?
               * It's a common problem with programs that do lots of writes,
               * like mkfs.
               */
              अगर (dosync (fd, qname) != 0)
                {
                  अगर (त्रुटि_सं != EIO)
                    {
                      other_error = true;
                      जाओ free_pattern_mem;
                    }
                  write_error = true;
                }
            }
        }
    }

  /* Force what we just wrote to hit the media. */
  अगर (dosync (fd, qname) != 0)
    {
      अगर (त्रुटि_सं != EIO)
        {
          other_error = true;
          जाओ free_pattern_mem;
        }
      write_error = true;
    }

free_pattern_mem:
  alignfree (pbuf);

  वापस other_error ? -1 : write_error;
}

/*
 * The passes start and end with a random pass, and the passes in between
 * are done in random order.  The idea is to deprive someone trying to
 * reverse the process of knowledge of the overwrite patterns, so they
 * have the additional step of figuring out what was done to the device
 * before they can try to reverse or cancel it.
 *
 * First, all possible 1-bit patterns.  There are two of them.
 * Then, all possible 2-bit patterns.  There are four, but the two
 * which are also 1-bit patterns can be omitted.
 * Then, all possible 3-bit patterns.  Likewise, 8-2 = 6.
 * Then, all possible 4-bit patterns.  16-4 = 12.
 *
 * The basic passes are:
 * 1-bit: 0x000, 0xFFF
 * 2-bit: 0x555, 0xAAA
 * 3-bit: 0x249, 0x492, 0x924, 0x6DB, 0xB6D, 0xDB6 (+ 1-bit)
 *        100100100100         110110110110
 *           9   2   4            D   B   6
 * 4-bit: 0x111, 0x222, 0x333, 0x444, 0x666, 0x777,
 *        0x888, 0x999, 0xBBB, 0xCCC, 0xDDD, 0xEEE (+ 1-bit, 2-bit)
 * Adding three random passes at the beginning, middle and end
 * produces the default 25-pass structure.
 *
 * The next extension would be to 5-bit and 6-bit patterns.
 * There are 30 uncovered 5-bit patterns and 64-8-2 = 46 uncovered
 * 6-bit patterns, so they would increase the time required
 * significantly.  4-bit patterns are enough for most purposes.
 *
 * The main gotcha is that this would require a trickier encoding,
 * since lcm(2,3,4) = 12 bits is easy to fit into an int, but
 * lcm(2,3,4,5) = 60 bits is not.
 *
 * One extension that is included is to complement the first bit in each
 * 512-byte block, to alter the phase of the encoded data in the more
 * complex encodings.  This doesn't apply to MFM, so the 1-bit patterns
 * are considered part of the 3-bit ones and the 2-bit patterns are
 * considered part of the 4-bit patterns.
 *
 *
 * How does the generalization to variable numbers of passes work?
 *
 * Here's how...
 * Have an ordered list of groups of passes.  Each group is a set.
 * Take as many groups as will fit, plus a random subset of the
 * last partial group, and place them into the passes list.
 * Then shuffle the passes list into random order and use that.
 *
 * One extra detail: if we can't include a large enough fraction of the
 * last group to be interesting, then just substitute random passes.
 *
 * If you want more passes than the entire list of groups can
 * provide, just start repeating from the beginning of the list.
 */
अटल पूर्णांक स्थिर
  patterns[] =
{
  -2,				/* 2 random passes */
  2, 0x000, 0xFFF,		/* 1-bit */
  2, 0x555, 0xAAA,		/* 2-bit */
  -1,				/* 1 random pass */
  6, 0x249, 0x492, 0x6DB, 0x924, 0xB6D, 0xDB6,	/* 3-bit */
  12, 0x111, 0x222, 0x333, 0x444, 0x666, 0x777,
  0x888, 0x999, 0xBBB, 0xCCC, 0xDDD, 0xEEE,	/* 4-bit */
  -1,				/* 1 random pass */
        /* The following patterns have the first bit per block flipped */
  8, 0x1000, 0x1249, 0x1492, 0x16DB, 0x1924, 0x1B6D, 0x1DB6, 0x1FFF,
  14, 0x1111, 0x1222, 0x1333, 0x1444, 0x1555, 0x1666, 0x1777,
  0x1888, 0x1999, 0x1AAA, 0x1BBB, 0x1CCC, 0x1DDD, 0x1EEE,
  -1,				/* 1 random pass */
  0				/* End */
};

/*
 * Generate a random wiping pass pattern with num passes.
 * This is a two-stage process.  First, the passes to include
 * are chosen, and then they are shuffled into the desired
 * order.
 */
अटल व्योम
genpattern (पूर्णांक *dest, माप_प्रकार अंक, काष्ठा randint_source *s)
{
  माप_प्रकार randpasses;
  पूर्णांक स्थिर *p;
  पूर्णांक *d;
  माप_प्रकार n;
  माप_प्रकार accum, शीर्ष_2, swap;
  पूर्णांक k;

  अगर (!अंक)
    वापस;

  /* Stage 1: choose the passes to use */
  p = patterns;
  randpasses = 0;
  d = dest;			/* Destination for generated pass list */
  n = अंक;			/* Passes remaining to fill */

  जबतक (true)
    {
      k = *p++;			/* Block descriptor word */
      अगर (!k)
        {			/* Loop back to the beginning */
          p = patterns;
        }
      अन्यथा अगर (k < 0)
        {			/* -k random passes */
          k = -k;
          अगर ((माप_प्रकार) k >= n)
            {
              randpasses += n;
              अवरोध;
            }
          randpasses += k;
          n -= k;
        }
      अन्यथा अगर ((माप_प्रकार) k <= n)
        {			/* Full block of patterns */
          स_नकल (d, p, k * माप (पूर्णांक));
          p += k;
          d += k;
          n -= k;
        }
      अन्यथा अगर (n < 2 || 3 * n < (माप_प्रकार) k)
        {			/* Finish with random */
          randpasses += n;
          अवरोध;
        }
      अन्यथा
        {			/* Pad out with n of the k available */
          करो
            {
              अगर (n == (माप_प्रकार) k || randint_choose (s, k) < n)
                {
                  *d++ = *p;
                  n--;
                }
              p++;
              k--;
            }
          जबतक (n);
          अवरोध;
        }
    }
  शीर्ष_2 = अंक - randpasses;	/* Top of initialized data */
  /* affirm (d == dest + top); */

  /*
   * We now have fixed patterns in the dest buffer up to
   * "top", and we need to scramble them, with "randpasses"
   * random passes evenly spaced among them.
   *
   * We want one at the beginning, one at the end, and
   * evenly spaced in between.  To do this, we basically
   * use Bresenham's line draw (a.k.a DDA) algorithm
   * to draw a line with slope (randpasses-1)/(num-1).
   * (We use a positive accumulator and count down to
   * do this.)
   *
   * So for each desired output value, we do the following:
   * - If it should be a random pass, copy the pass type
   *   to top++, out of the way of the other passes, and
   *   set the current pass to -1 (random).
   * - If it should be a normal pattern pass, choose an
   *   entry at random between here and top-1 (inclusive)
   *   and swap the current entry with that one.
   */
  randpasses--;			/* To speed up later math */
  accum = randpasses;		/* Bresenham DDA accumulator */
  क्रम (n = 0; n < अंक; n++)
    {
      अगर (accum <= randpasses)
        {
          accum += अंक - 1;
          dest[शीर्ष_2++] = dest[n];
          dest[n] = -1;
        }
      अन्यथा
        {
          swap = n + randint_choose (s, शीर्ष_2 - n);
          k = dest[n];
          dest[n] = dest[swap];
          dest[swap] = k;
        }
      accum -= randpasses;
    }
  /* affirm (top == num); */
}

/*
 * The core routine to actually do the work.  This overwrites the first
 * size bytes of the given fd.  Return true if successful.
 */
अटल bool
do_wipefd (पूर्णांक fd, अक्षर स्थिर *qname, काष्ठा randint_source *s,
           काष्ठा Options स्थिर *flags)
{
  काष्ठा stat st;
  off_t आकार;		/* Size to write, size to read */
  off_t i_आकार = 0;	/* For small files, initial size to overwrite inode */
  अचिन्हित दीर्घ पूर्णांक n;	/* Number of passes for printing purposes */
  पूर्णांक *passarray;
  bool ok = true;
  काष्ठा randread_source *rs;

  n = 0;		/* dopass takes n == 0 to mean "don't print progress" */
  अगर (flags->verbose)
    n = flags->n_iterations + flags->zero_fill;

  अगर (ख_स्थिति (fd, &st))
    {
      error (0, त्रुटि_सं, _("%s: fstat failed"), qname);
      वापस false;
    }

  /* If we know that we can't possibly shred the file, give up now.
     Otherwise, we may go into an infinite loop writing data before we
     find that we can't rewind the device.  */
  अगर ((S_ISCHR (st.st_mode) && isatty (fd))
      || S_ISFIFO (st.st_mode)
      || S_ISSOCK (st.st_mode))
    {
      error (0, 0, _("%s: invalid file type"), qname);
      वापस false;
    }
  अन्यथा अगर (S_ISREG (st.st_mode) && st.st_आकार < 0)
    {
      error (0, 0, _("%s: file has negative size"), qname);
      वापस false;
    }

  /* Allocate pass array */
  passarray = xnmalloc (flags->n_iterations, माप *passarray);

  आकार = flags->आकार;
  अगर (आकार == -1)
    {
      अगर (S_ISREG (st.st_mode))
        {
          आकार = st.st_आकार;

          अगर (! flags->exact)
            {
              /* Round up to the nearest block size to clear slack space.  */
              off_t remainder = आकार % STP_BLKSIZE (&st);
              अगर (आकार && आकार < STP_BLKSIZE (&st))
                i_आकार = आकार;
              अगर (remainder != 0)
                {
                  off_t आकार_incr = STP_BLKSIZE (&st) - remainder;
                  आकार += MIN (आकार_incr, OFF_T_MAX - आकार);
                }
            }
        }
      अन्यथा
        {
          /* The behavior of lseek is unspecified, but in practice if
             it returns a positive number that's the size of this
             device.  */
          आकार = lseek (fd, 0, अंत_से);
          अगर (आकार <= 0)
            {
              /* We are unable to determine the length, up front.
                 Let dopass do that as part of its first iteration.  */
              आकार = -1;
            }
        }
    }
  अन्यथा अगर (S_ISREG (st.st_mode)
           && st.st_आकार < MIN (STP_BLKSIZE (&st), आकार))
    i_आकार = st.st_आकार;

  /* Schedule the passes in random order. */
  genpattern (passarray, flags->n_iterations, s);

  rs = randint_get_source (s);

  जबतक (true)
    {
      off_t pass_आकार;
      अचिन्हित दीर्घ पूर्णांक pn = n;

      अगर (i_आकार)
        {
          pass_आकार = i_आकार;
          i_आकार = 0;
          pn = 0;
        }
      अन्यथा अगर (आकार)
        {
          pass_आकार = आकार;
          आकार = 0;
        }
      /* TODO: consider handling tail packing by
         writing the tail padding as a separate pass,
         (that would not rewind).  */
      अन्यथा
        अवरोध;

      क्रम (माप_प्रकार i = 0; i < flags->n_iterations + flags->zero_fill; i++)
        {
          पूर्णांक err = 0;
          पूर्णांक type = i < flags->n_iterations ? passarray[i] : 0;

          err = dopass (fd, &st, qname, &pass_आकार, type, rs, i + 1, pn);

          अगर (err)
            {
              ok = false;
              अगर (err < 0)
                जाओ wipefd_out;
            }
        }
    }

  /* Now deallocate the data.  The effect of ftruncate is specified
     on regular files and shared memory objects (also directories, but
     they are not possible here); don't worry about errors reported
     for other file types.  */

  अगर (flags->remove_file && ftruncate (fd, 0) != 0
      && (S_ISREG (st.st_mode) || S_TYPEISSHM (&st)))
    {
      error (0, त्रुटि_सं, _("%s: error truncating"), qname);
      ok = false;
      जाओ wipefd_out;
    }

wipefd_out:
  मुक्त (passarray);
  वापस ok;
}

/* A wrapper with a little more checking for fds on the command line */
अटल bool
wipefd (पूर्णांक fd, अक्षर स्थिर *qname, काष्ठा randint_source *s,
        काष्ठा Options स्थिर *flags)
{
  पूर्णांक fd_flags = fcntl (fd, F_GETFL);

  अगर (fd_flags < 0)
    {
      error (0, त्रुटि_सं, _("%s: fcntl failed"), qname);
      वापस false;
    }
  अगर (fd_flags & O_APPEND)
    {
      error (0, 0, _("%s: cannot shred append-only file descriptor"), qname);
      वापस false;
    }
  वापस do_wipefd (fd, qname, s, flags);
}

/* --- Name-wiping code --- */

/* Characters allowed in a file name - a safe universal set.  */
अटल अक्षर स्थिर nameset[] =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.";

/* Increment NAME (with LEN bytes).  NAME must be a big-endian base N
   number with the digits taken from nameset.  Return true if successful.
   Otherwise, (because NAME already has the greatest possible value)
   return false.  */

अटल bool
incname (अक्षर *name, माप_प्रकार len)
{
  जबतक (len--)
    {
      अक्षर स्थिर *p = म_अक्षर (nameset, name[len]);

      /* Given that NAME is composed of bytes from NAMESET,
         P will never be null here.  */

      /* If this character has a successor, use it.  */
      अगर (p[1])
        {
          name[len] = p[1];
          वापस true;
        }

      /* Otherwise, set this digit to 0 and increment the prefix.  */
      name[len] = nameset[0];
    }

  वापस false;
}

/*
 * Repeatedly rename a file with shorter and shorter names,
 * to obliterate all traces of the file name (and length) on any system
 * that adds a trailing delimiter to on-device file names and reuses
 * the same directory slot.  Finally, unlink it.
 * The passed-in filename is modified in place to the new filename.
 * (Which is unlinked if this function succeeds, but is still present if
 * it fails for some reason.)
 *
 * The main loop is written carefully to not get stuck if all possible
 * names of a given length are occupied.  It counts down the length from
 * the original to 0.  While the length is non-zero, it tries to find an
 * unused file name of the given length.  It continues until either the
 * name is available and the rename succeeds, or it runs out of names
 * to try (incname wraps and returns 1).  Finally, it unlinks the file.
 *
 * The unlink is Unix-specific, as ANSI-standard remove has more
 * portability problems with C libraries making it "safe".  rename
 * is ANSI-standard.
 *
 * To force the directory data out, we try to open the directory and
 * invoke fdatasync and/or fsync on it.  This is non-standard, so don't
 * insist that it works: just fall back to a global sync in that case.
 * This is fairly significantly Unix-specific.  Of course, on any
 * file system with synchronous metadata updates, this is unnecessary.
 */
अटल bool
wipename (अक्षर *oldname, अक्षर स्थिर *qoldname, काष्ठा Options स्थिर *flags)
{
  अक्षर *newname = xstrdup (oldname);
  अक्षर *base = last_component (newname);
  अक्षर *dir = dir_name (newname);
  अक्षर *qdir = xstrdup (quotef (dir));
  bool first = true;
  bool ok = true;
  पूर्णांक dir_fd = -1;

  अगर (flags->remove_file == remove_wipesync)
    dir_fd = खोलो (dir, O_RDONLY | O_DIRECTORY | O_NOCTTY | O_NONBLOCK);

  अगर (flags->verbose)
    error (0, 0, _("%s: removing"), qoldname);

  अगर (flags->remove_file != remove_unlink)
    क्रम (माप_प्रकार len = base_len (base); len != 0; len--)
      {
        स_रखो (base, nameset[0], len);
        base[len] = 0;
        bool rename_ok;
        जबतक (! (rename_ok = (renameatu (AT_FDCWD, oldname, AT_FDCWD, newname,
                                          RENAME_NOREPLACE)
                               == 0))
               && त्रुटि_सं == EEXIST && incname (base, len))
          जारी;
        अगर (rename_ok)
          {
            अगर (0 <= dir_fd && dosync (dir_fd, qdir) != 0)
              ok = false;
            अगर (flags->verbose)
              {
                /* People seem to understand this better than talking
                   about renaming OLDNAME.  NEWNAME doesn't need
                   quoting because we picked it.  OLDNAME needs to be
                   quoted only the first time.  */
                अक्षर स्थिर *old = first ? qoldname : oldname;
                error (0, 0,
                       _("%s: renamed to %s"), old, newname);
                first = false;
              }
            स_नकल (oldname + (base - newname), base, len + 1);
          }
      }

  अगर (unlink (oldname) != 0)
    {
      error (0, त्रुटि_सं, _("%s: failed to remove"), qoldname);
      ok = false;
    }
  अन्यथा अगर (flags->verbose)
    error (0, 0, _("%s: removed"), qoldname);
  अगर (0 <= dir_fd)
    {
      अगर (dosync (dir_fd, qdir) != 0)
        ok = false;
      अगर (बंद (dir_fd) != 0)
        {
          error (0, त्रुटि_सं, _("%s: failed to close"), qdir);
          ok = false;
        }
    }
  मुक्त (newname);
  मुक्त (dir);
  मुक्त (qdir);
  वापस ok;
}

/*
 * Finally, the function that actually takes a filename and grinds
 * it into hamburger.
 *
 * FIXME
 * Detail to note: since we do not restore errno to EACCES after
 * a failed chmod, we end up printing the error code from the chmod.
 * This is actually the error that stopped us from proceeding, so
 * it's arguably the right one, and in practice it'll be either EACCES
 * again or EPERM, which both give similar error messages.
 * Does anyone disagree?
 */
अटल bool
wipefile (अक्षर *name, अक्षर स्थिर *qname,
          काष्ठा randint_source *s, काष्ठा Options स्थिर *flags)
{
  पूर्णांक fd = खोलो (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_द्विआधारी);
  अगर (fd < 0
      && (त्रुटि_सं == EACCES && flags->force)
      && chmod (name, S_IWUSR) == 0)
    fd = खोलो (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_द्विआधारी);
  अगर (fd < 0)
    {
      काष्ठा stat st;
      स्थिर पूर्णांक open_errno = त्रुटि_सं;

      /* Try to give a more meaningful error message if we attempt to
         open a FIFO with no readers.  */
      अगर (open_errno == ENXIO && 0 <= stat (name, &st)
          && S_ISFIFO (st.st_mode))
        error (0, 0, _("%s: invalid file type"), qname);
      अन्यथा
        error (0, open_errno, _("%s: failed to open for writing"), qname);
      वापस false;
    }

  /* We used O_NONBLOCK to prevent blocking when opening a FIFO.
     Reset that here.  */
  पूर्णांक fd_flags = fcntl (fd, F_GETFL);
  bool ok = 0 <= fd_flags && 0 <= fcntl (fd, F_SETFL, fd_flags & ~O_NONBLOCK);
  अगर (ok)
    ok = do_wipefd (fd, qname, s, flags);
  अन्यथा
    error (0, त्रुटि_सं, _("couldn't reset non-blocking mode %s"), qname);

  अगर (बंद (fd) != 0)
    {
      error (0, त्रुटि_सं, _("%s: failed to close"), qname);
      ok = false;
    }
  अगर (ok && flags->remove_file)
    ok = wipename (name, qname, flags);
  वापस ok;
}


/* Buffers for random data.  */
अटल काष्ठा randint_source *randint_source;

/* Just on general principles, wipe buffers containing information
   that may be related to the possibly-pseudorandom values used during
   shredding.  */
अटल व्योम
clear_random_तथ्य (व्योम)
{
  randint_all_free (randint_source);
}


पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool ok = true;
  काष्ठा Options flags = {0};
  अक्षर **file;
  पूर्णांक n_files;
  पूर्णांक c;
  अक्षर स्थिर *random_source = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  flags.n_iterations = DEFAULT_PASSES;
  flags.आकार = -1;

  जबतक ((c = getopt_long (argc, argv, "fn:s:uvxz", long_opts, शून्य)) != -1)
    {
      चयन (c)
        {
        हाल 'f':
          flags.force = true;
          अवरोध;

        हाल 'n':
          flags.n_iterations = xdectoumax (optarg, 0,
                                           MIN (अच_दीर्घ_उच्च,
                                                SIZE_MAX / माप (पूर्णांक)), "",
                                           _("invalid number of passes"), 0);
          अवरोध;

        हाल RANDOM_SOURCE_OPTION:
          अगर (random_source && !streq (random_source, optarg))
            error (निकास_त्रुटि, 0, _("multiple random sources specified"));
          random_source = optarg;
          अवरोध;

        हाल 'u':
          अगर (optarg == शून्य)
            flags.remove_file = remove_wipesync;
          अन्यथा
            flags.remove_file = XARGMATCH ("--remove", optarg,
                                           remove_args, remove_methods);
          अवरोध;

        हाल 's':
          flags.आकार = xnumtoumax (optarg, 0, 0, OFF_T_MAX, "cbBkKMGTPEZYRQ0",
                                   _("invalid file size"), 0, 0);
          अवरोध;

        हाल 'v':
          flags.verbose = true;
          अवरोध;

        हाल 'x':
          flags.exact = true;
          अवरोध;

        हाल 'z':
          flags.zero_fill = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  file = argv + optind;
  n_files = argc - optind;

  अगर (n_files == 0)
    {
      error (0, 0, _("missing file operand"));
      usage (निकास_त्रुटि);
    }

  randint_source = randint_all_new (random_source, SIZE_MAX);
  अगर (! randint_source)
    error (निकास_त्रुटि, त्रुटि_सं, "%s",
           quotef (random_source ? random_source : "getrandom"));
  निकास_पर (clear_random_तथ्य);

  क्रम (पूर्णांक i = 0; i < n_files; i++)
    {
      अक्षर *qname = xstrdup (quotef (file[i]));
      अगर (streq (file[i], "-"))
        {
          ok &= wipefd (STDOUT_FILENO, qname, randint_source, &flags);
        }
      अन्यथा
        {
          /* Plain filename - Note that this overwrites *argv! */
          ok &= wipefile (file[i], qname, randint_source, &flags);
        }
      मुक्त (qname);
    }

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
/*
 * vim:sw=2:sts=2:
 */
