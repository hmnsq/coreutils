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
#define PROGRAM_NAME "shred"

#define AUTHORS proper_name ("Colin Plumb")

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#যদি defined __linux__ && HAVE_SYS_MTIO_H
# include <sys/mtio.h>
#endif

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "alignalloc.h"
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "xdectoint.h"
#অন্তর্ভাব "fcntl--.h"
#অন্তর্ভাব "gethrxtime.h"
#অন্তর্ভাব "human.h"
#অন্তর্ভাব "randint.h"
#অন্তর্ভাব "randread.h"
#অন্তর্ভাব "renameatu.h"
#অন্তর্ভাব "stat-size.h"

/* Default number of times to overwrite.  */
গণনা { DEFAULT_PASSES = 3 };

/* How many seconds to wait before checking whether to output another
   verbose output line.  */
গণনা { VERBOSE_UPDATE = 5 };

/* Sector size and corresponding mask, for recovering after write failures.
   The size must be a power of 2.  */
গণনা { SECTOR_SIZE = 512 };
গণনা { SECTOR_MASK = SECTOR_SIZE - 1 };
static_assert (0 < SECTOR_SIZE && (SECTOR_SIZE & SECTOR_MASK) == 0);

গণনা remove_method
{
  remove_none = 0,      /* the default: only wipe data.  */
  remove_unlink,        /* don't obfuscate name, just unlink.  */
  remove_wipe,          /* obfuscate name before unlink.  */
  remove_wipesync       /* obfuscate name, syncing each byte, before unlink.  */
};

স্থির বর্ণ ধ্রুব *ধ্রুব remove_args[] =
{
  "unlink", "wipe", "wipesync", রিক্ত
};

স্থির গণনা remove_method ধ্রুব remove_methods[] =
{
  remove_unlink, remove_wipe, remove_wipesync
};

সংরচনা Options
{
  bool force;		/* -f flag: chmod files if necessary */
  size_t n_iterations;	/* -n flag: Number of iterations */
  off_t size;		/* -s flag: size of file */
  গণনা remove_method remove_file; /* -u flag: remove file after shredding */
  bool verbose;		/* -v flag: Print progress */
  bool exact;		/* -x flag: Do not round up file size */
  bool zero_fill;	/* -z flag: Add a final zero pass */
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

স্থির সংরচনা option ধ্রুব long_opts[] =
{
  {"exact", no_argument, রিক্ত, 'x'},
  {"force", no_argument, রিক্ত, 'f'},
  {"iterations", required_argument, রিক্ত, 'n'},
  {"size", required_argument, রিক্ত, 's'},
  {"random-source", required_argument, রিক্ত, RANDOM_SOURCE_OPTION},
  {"remove", optional_argument, রিক্ত, 'u'},
  {"verbose", no_argument, রিক্ত, 'v'},
  {"zero", no_argument, রিক্ত, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Overwrite the specified FILE(s) repeatedly, in order to make it harder\n\
প্রতি even very expensive hardware probing to recover the data.\n\
"), stdout);
      fputs (_("\
\n\
If FILE is -, shred standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --force\n\
         change permissions to allow writing যদি necessary\n\
"));
      oprintf (_("\
  -n, --iterations=N\n\
         overwrite N times instead of the স্বভাব (%d)\n\
"), DEFAULT_PASSES);
      oputs (_("\
      --random-source=FILE\n\
         get random bytes from FILE\n\
"));
      oputs (_("\
  -s, --size=N\n\
         shred this many bytes (suffixes like K, M, G accepted)\n\
"));
      oputs (_("\
  -u\n\
         deallocate and remove file after overwriting\n\
"));
      oputs (_("\
      --remove[=HOW]\n\
         like -u but give control on HOW to delete;  See below\n\
"));
      oputs (_("\
  -v, --verbose\n\
         show details of data and metadata operations performed\n\
"));
      oputs (_("\
  -x, --exact\n\
         কুরু not round file sizes up to the next full block;\n\
         this is the স্বভাব প্রতি non-regular files\n\
"));
      oputs (_("\
  -z, --zero\n\
         add a final overwrite with zeros to hide shredding\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FILE will be skipped with a diagnostic message যদি it is a FIFO, socket, or\n\
terminal, since its data does not reside on disk.\n\
"), stdout);
      fputs (_("\
\n\
Delete FILE(s) যদি --remove (-u) is specified.  The স্বভাব is not to remove\n\
the files because it is common to operate on device files like /dev/hda,\n\
and those files usually should not be removed.\n\
The optional HOW parameter indicates how to remove a directory entry:\n\
'unlink' => use a standard unlink call.\n\
'wipe' => also first obfuscate bytes in the name.\n\
'wipesync' => also sync each obfuscated byte to the device.\n\
The স্বভাব mode is 'wipesync', but note it can be expensive.\n\
\n\
"), stdout);
      fputs (_("\
CAUTION: shred assumes the file system and hardware overwrite data in place.\n\
Although this is common, many platforms operate otherwise.  Also, backups\n\
and mirrors may contain unremovable copies that will let a shredded file\n\
be recovered later.  See the GNU coreutils manual প্রতি details.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/*
 * Determine if pattern type is periodic or not.
 */
স্থির bool
periodic_pattern (পূর্ণাঙ্ক type)
{
  যদি (type <= 0)
    প্রত্যাগম false;

  unsigned বর্ণ r[3];
  unsigned পূর্ণাঙ্ক bits = type & 0xfff;

  bits |= bits << 12;
  r[0] = (bits >> 4) & 255;
  r[1] = (bits >> 8) & 255;
  r[2] = bits & 255;

  প্রত্যাগম (r[0] != r[1]) || (r[0] != r[2]);
}

/*
 * Fill a buffer with a fixed pattern.
 *
 * The buffer must be at least 3 bytes long, even if
 * size is less.  Larger sizes are filled exactly.
 */
স্থির শূন্য
fillpattern (পূর্ণাঙ্ক type, unsigned বর্ণ *r, size_t size)
{
  size_t i;
  unsigned পূর্ণাঙ্ক bits = type & 0xfff;

  bits |= bits << 12;
  r[0] = (bits >> 4) & 255;
  r[1] = (bits >> 8) & 255;
  r[2] = bits & 255;
  প্রতি (i = 3; i <= size / 2; i *= 2)
    memcpy (r + i, r, i);
  যদি (i < size)
    memcpy (r + i, r, size - i);

  /* Invert the first bit of every sector. */
  যদি (type & 0x1000)
    প্রতি (i = 0; i < size; i += SECTOR_SIZE)
      r[i] ^= 0x80;
}

/*
 * Generate a 6-character (+ nul) pass name string
 * FIXME: allow translation of "random".
 */
#define PASS_NAME_SIZE 7
স্থির শূন্য
passname (unsigned বর্ণ ধ্রুব *data, বর্ণ name[PASS_NAME_SIZE])
{
  যদি (data)
    sprintf (name, "%02x%02x%02x", data[0], data[1], data[2]);
  অন্যথা
    memcpy (name, "random", PASS_NAME_SIZE);
}

/* Return true when it's ok to ignore an fsync or fdatasync
   failure that set errno to ERRNO_VAL.  */
স্থির bool
ignorable_sync_errno (পূর্ণাঙ্ক errno_val)
{
  প্রত্যাগম (errno_val == EINVAL
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
স্থির পূর্ণাঙ্ক
dosync (পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *qname)
{
  পূর্ণাঙ্ক err;

  যদি (fdatasync (fd) == 0)
    প্রত্যাগম 0;
  err = errno;
  যদি ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fdatasync failed"), qname);
      errno = err;
      প্রত্যাগম -1;
    }

  যদি (fsync (fd) == 0)
    প্রত্যাগম 0;
  err = errno;
  যদি ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fsync failed"), qname);
      errno = err;
      প্রত্যাগম -1;
    }

  sync ();
  প্রত্যাগম 0;
}

/* Turn on or off direct I/O mode for file descriptor FD, if possible.
   Try to turn it on if ENABLE is true.  Otherwise, try to turn it off.  */
স্থির শূন্য
direct_mode (পূর্ণাঙ্ক fd, bool enable)
{
  যদি (O_DIRECT)
    {
      পূর্ণাঙ্ক fd_flags = fcntl (fd, F_GETFL);
      যদি (0 < fd_flags)
        {
          পূর্ণাঙ্ক new_flags = (enable
                           ? (fd_flags | O_DIRECT)
                           : (fd_flags & ~O_DIRECT));
          যদি (new_flags != fd_flags)
            fcntl (fd, F_SETFL, new_flags);
        }
    }

#যদি HAVE_DIRECTIO && defined DIRECTIO_ON && defined DIRECTIO_OFF
  /* This is Solaris-specific.  */
  directio (fd, enable ? DIRECTIO_ON : DIRECTIO_OFF);
#endif
}

/* Rewind FD; its status is ST.  */
স্থির bool
dorewind (পূর্ণাঙ্ক fd, সংরচনা stat ধ্রুব *st)
{
  যদি (S_ISCHR (st->st_mode))
    {
#যদি defined __linux__ && HAVE_SYS_MTIO_H
      /* In the Linux kernel, lseek does not work on tape devices; it
         returns a randomish value instead.  Try the low-level tape
         rewind operation first.  */
      সংরচনা mtop op;
      op.mt_op = MTREW;
      op.mt_গুনতি = 1;
      যদি (ioctl (fd, MTIOCTOP, &op) == 0)
        প্রত্যাগম true;
#endif
    }
  off_t offset = lseek (fd, 0, SEEK_SET);
  যদি (0 < offset)
    errno = EINVAL;
  প্রত্যাগম offset == 0;
}

/* By convention, negative sizes represent unknown values.  */

স্থির bool
known (off_t size)
{
  প্রত্যাগম 0 <= size;
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
স্থির পূর্ণাঙ্ক
dopass (পূর্ণাঙ্ক fd, সংরচনা stat ধ্রুব *st, বর্ণ ধ্রুব *qname, off_t *sizep,
        পূর্ণাঙ্ক type, সংরচনা randread_source *s,
        unsigned long পূর্ণাঙ্ক k, unsigned long পূর্ণাঙ্ক n)
{
  off_t size = *sizep;
  off_t offset;                /* Current file position */
  xtime_t prev IF_LINT ( = 0); /* Time we printed the previous update.  */
  xtime_t now = 0;             /* Current time */
  size_t lim;                  /* Amount of data to try writing */
  size_t soff;                 /* Offset into buffer for next write */
  ssize_t ssize;               /* Return value from write */

  /* Fill pattern buffer.  Aligning it to a page so we can do direct I/O.  */
  size_t page_size = getpagesize ();
#define PERIODIC_OUTPUT_SIZE (60 * 1024)
#define NONPERIODIC_OUTPUT_SIZE (64 * 1024)
  static_assert (PERIODIC_OUTPUT_SIZE % 3 == 0);
  size_t output_size = periodic_pattern (type)
                       ? PERIODIC_OUTPUT_SIZE : NONPERIODIC_OUTPUT_SIZE;
#define FILLPATTERN_SIZE (((output_size + 2) / 3) * 3) /* Multiple of 3 */
  unsigned বর্ণ *pbuf = xalignalloc (page_size, FILLPATTERN_SIZE);

  বর্ণ pass_string[PASS_NAME_SIZE];	/* Name of current pass */
  bool write_error = false;
  bool other_error = false;

  /* Printable previous offset into the file */
  বর্ণ previous_offset_buf[LONGEST_HUMAN_READABLE + 1];
  বর্ণ ধ্রুব *previous_human_offset;

  /* As a performance tweak, avoid direct I/O for small sizes,
     as it's just a performance rather than security consideration,
     and direct I/O can often be unsupported for small non aligned sizes.  */
  bool try_without_directio = 0 < size && size < output_size;
  যদি (! try_without_directio)
    direct_mode (fd, true);

  যদি (! dorewind (fd, st))
    {
      error (0, errno, _("%s: cannot rewind"), qname);
      other_error = true;
      গচ্ছ free_pattern_mem;
    }

  /* Constant fill patterns need only be set up once. */
  যদি (type >= 0)
    {
      lim = known (size) && size < FILLPATTERN_SIZE ? size : FILLPATTERN_SIZE;
      fillpattern (type, pbuf, lim);
      passname (pbuf, pass_string);
    }
  অন্যথা
    {
      passname (রিক্ত, pass_string);
    }

  /* Set position if first status update */
  যদি (n)
    {
      error (0, 0, _("%s: pass %lu/%lu (%s)..."), qname, k, n, pass_string);
      prev = gethrxtime ();
      previous_human_offset = "";
    }

  offset = 0;
  যাবত্ (true)
    {
      /* How much to write this time? */
      lim = output_size;
      যদি (known (size) && size - offset < output_size)
        {
          যদি (size < offset)
            ভঙ্গ;
          lim = size - offset;
          যদি (!lim)
            ভঙ্গ;
        }
      যদি (type < 0)
        randread (s, pbuf, lim);
      /* Loop to retry partial writes. */
      প্রতি (soff = 0; soff < lim; soff += ssize)
        {
          ssize = লিখ (fd, pbuf + soff, lim - soff);
          যদি (ssize <= 0)
            {
              যদি (! known (size) && (ssize == 0 || errno == ENOSPC))
                {
                  /* We have found the end of the file.  */
                  যদি (soff <= OFF_T_MAX - offset)
                    *sizep = size = offset + soff;
                  ভঙ্গ;
                }
              অন্যথা
                {
                  পূর্ণাঙ্ক errnum = errno;

                  /* Retry without direct I/O since this may not be supported
                     at all on some (file) systems, or with the current size.
                     I.e., a specified --size that is not aligned, or when
                     dealing with slop at the end of a file with --exact.  */
                  যদি (! try_without_directio && errno == EINVAL)
                    {
                      direct_mode (fd, false);
                      ssize = 0;
                      try_without_directio = true;
                      অনুবর্ত;
                    }
                  uintmax_t error_offset = offset + soff;
                  error (0, errnum, _("%s: error writing at offset %ju"),
                         qname, error_offset);

                  /* 'shred' is often used on bad media, before throwing it
                     out.  Thus, it shouldn't give up on bad blocks.  This
                     code works because lim is always a multiple of
                     SECTOR_SIZE, except at the end.  This size constraint
                     also enables direct I/O on some (file) systems.  */
                  static_assert (PERIODIC_OUTPUT_SIZE % SECTOR_SIZE == 0);
                  static_assert (NONPERIODIC_OUTPUT_SIZE % SECTOR_SIZE == 0);
                  যদি (errnum == EIO && known (size)
                      && (soff | SECTOR_MASK) < lim)
                    {
                      size_t soff1 = (soff | SECTOR_MASK) + 1;
                      যদি (lseek (fd, offset + soff1, SEEK_SET) != -1)
                        {
                          /* Arrange to skip this block. */
                          ssize = soff1 - soff;
                          write_error = true;
                          অনুবর্ত;
                        }
                      error (0, errno, _("%s: lseek failed"), qname);
                    }
                  other_error = true;
                  গচ্ছ free_pattern_mem;
                }
            }
        }

      /* Okay, we have written "soff" bytes. */

      যদি (ckd_add (&offset, offset, soff))
        {
          error (0, 0, _("%s: file too large"), qname);
          other_error = true;
          গচ্ছ free_pattern_mem;
        }

      bool done = offset == size;

      /* Time to print progress? */
      যদি (n && ((done && *previous_human_offset)
                || VERBOSE_UPDATE <= xtime_sec ((now = gethrxtime ()) - prev)))
        {
          বর্ণ offset_buf[LONGEST_HUMAN_READABLE + 1];
          বর্ণ size_buf[LONGEST_HUMAN_READABLE + 1];
          পূর্ণাঙ্ক human_progress_opts = (human_autoscale | human_SI
                                     | human_base_1024 | human_B);
          বর্ণ ধ্রুব *human_offset
            = human_readable (offset, offset_buf,
                              human_floor | human_progress_opts, 1, 1);

          যদি (done || !streq (previous_human_offset, human_offset))
            {
              যদি (! known (size))
                error (0, 0, _("%s: pass %lu/%lu (%s)...%s"),
                       qname, k, n, pass_string, human_offset);
              অন্যথা
                {
                  uintmax_t off = offset;
                  পূর্ণাঙ্ক percent = (size == 0
                                 ? 100
                                 : (off <= UINTMAX_MAX / 100
                                    ? off * 100 / size
                                    : off / (size / 100)));
                  বর্ণ ধ্রুব *human_size
                    = human_readable (size, size_buf,
                                      human_ceiling | human_progress_opts,
                                      1, 1);
                  যদি (done)
                    human_offset = human_size;
                  error (0, 0, _("%s: pass %lu/%lu (%s)...%s/%s %d%%"),
                         qname, k, n, pass_string, human_offset, human_size,
                         percent);
                }

              strcpy (previous_offset_buf, human_offset);
              previous_human_offset = previous_offset_buf;
              prev = now;

              /*
               * Force periodic syncs to keep displayed progress accurate
               * FIXME: Should these be present even if -v is not enabled,
               * to keep the buffer cache from filling with dirty pages?
               * It's a common problem with programs that do lots of writes,
               * like mkfs.
               */
              যদি (dosync (fd, qname) != 0)
                {
                  যদি (errno != EIO)
                    {
                      other_error = true;
                      গচ্ছ free_pattern_mem;
                    }
                  write_error = true;
                }
            }
        }
    }

  /* Force what we just wrote to hit the media. */
  যদি (dosync (fd, qname) != 0)
    {
      যদি (errno != EIO)
        {
          other_error = true;
          গচ্ছ free_pattern_mem;
        }
      write_error = true;
    }

free_pattern_mem:
  alignfree (pbuf);

  প্রত্যাগম other_error ? -1 : write_error;
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
স্থির পূর্ণাঙ্ক ধ্রুব
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
স্থির শূন্য
genpattern (পূর্ণাঙ্ক *dest, size_t অঙ্ক, সংরচনা randint_source *s)
{
  size_t randpasses;
  পূর্ণাঙ্ক ধ্রুব *p;
  পূর্ণাঙ্ক *d;
  size_t n;
  size_t accum, শীর্ষ, swap;
  পূর্ণাঙ্ক k;

  যদি (!অঙ্ক)
    প্রত্যাগম;

  /* Stage 1: choose the passes to use */
  p = patterns;
  randpasses = 0;
  d = dest;			/* Destination for generated pass list */
  n = অঙ্ক;			/* Passes remaining to fill */

  যাবত্ (true)
    {
      k = *p++;			/* Block descriptor word */
      যদি (!k)
        {			/* Loop back to the beginning */
          p = patterns;
        }
      অন্যথা যদি (k < 0)
        {			/* -k random passes */
          k = -k;
          যদি ((size_t) k >= n)
            {
              randpasses += n;
              ভঙ্গ;
            }
          randpasses += k;
          n -= k;
        }
      অন্যথা যদি ((size_t) k <= n)
        {			/* Full block of patterns */
          memcpy (d, p, k * পরিমাণ (পূর্ণাঙ্ক));
          p += k;
          d += k;
          n -= k;
        }
      অন্যথা যদি (n < 2 || 3 * n < (size_t) k)
        {			/* Finish with random */
          randpasses += n;
          ভঙ্গ;
        }
      অন্যথা
        {			/* Pad out with n of the k available */
          কুরু
            {
              যদি (n == (size_t) k || randint_choose (s, k) < n)
                {
                  *d++ = *p;
                  n--;
                }
              p++;
              k--;
            }
          যাবত্ (n);
          ভঙ্গ;
        }
    }
  শীর্ষ = অঙ্ক - randpasses;	/* Top of initialized data */
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
  প্রতি (n = 0; n < অঙ্ক; n++)
    {
      যদি (accum <= randpasses)
        {
          accum += অঙ্ক - 1;
          dest[শীর্ষ++] = dest[n];
          dest[n] = -1;
        }
      অন্যথা
        {
          swap = n + randint_choose (s, শীর্ষ - n);
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
স্থির bool
do_wipefd (পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *qname, সংরচনা randint_source *s,
           সংরচনা Options ধ্রুব *flags)
{
  সংরচনা stat st;
  off_t size;		/* Size to write, size to read */
  off_t i_size = 0;	/* For small files, initial size to overwrite inode */
  unsigned long পূর্ণাঙ্ক n;	/* Number of passes for printing purposes */
  পূর্ণাঙ্ক *passarray;
  bool ok = true;
  সংরচনা randread_source *rs;

  n = 0;		/* dopass takes n == 0 to mean "don't print progress" */
  যদি (flags->verbose)
    n = flags->n_iterations + flags->zero_fill;

  যদি (fstat (fd, &st))
    {
      error (0, errno, _("%s: fstat failed"), qname);
      প্রত্যাগম false;
    }

  /* If we know that we can't possibly shred the file, give up now.
     Otherwise, we may go into an infinite loop writing data before we
     find that we can't rewind the device.  */
  যদি ((S_ISCHR (st.st_mode) && isatty (fd))
      || S_ISFIFO (st.st_mode)
      || S_ISSOCK (st.st_mode))
    {
      error (0, 0, _("%s: invalid file type"), qname);
      প্রত্যাগম false;
    }
  অন্যথা যদি (S_ISREG (st.st_mode) && st.st_size < 0)
    {
      error (0, 0, _("%s: file has negative size"), qname);
      প্রত্যাগম false;
    }

  /* Allocate pass array */
  passarray = xnmalloc (flags->n_iterations, পরিমাণ *passarray);

  size = flags->size;
  যদি (size == -1)
    {
      যদি (S_ISREG (st.st_mode))
        {
          size = st.st_size;

          যদি (! flags->exact)
            {
              /* Round up to the nearest block size to clear slack space.  */
              off_t remainder = size % STP_BLKSIZE (&st);
              যদি (size && size < STP_BLKSIZE (&st))
                i_size = size;
              যদি (remainder != 0)
                {
                  off_t size_incr = STP_BLKSIZE (&st) - remainder;
                  size += MIN (size_incr, OFF_T_MAX - size);
                }
            }
        }
      অন্যথা
        {
          /* The behavior of lseek is unspecified, but in practice if
             it returns a positive number that's the size of this
             device.  */
          size = lseek (fd, 0, SEEK_END);
          যদি (size <= 0)
            {
              /* We are unable to determine the length, up front.
                 Let dopass do that as part of its first iteration.  */
              size = -1;
            }
        }
    }
  অন্যথা যদি (S_ISREG (st.st_mode)
           && st.st_size < MIN (STP_BLKSIZE (&st), size))
    i_size = st.st_size;

  /* Schedule the passes in random order. */
  genpattern (passarray, flags->n_iterations, s);

  rs = randint_get_source (s);

  যাবত্ (true)
    {
      off_t pass_size;
      unsigned long পূর্ণাঙ্ক pn = n;

      যদি (i_size)
        {
          pass_size = i_size;
          i_size = 0;
          pn = 0;
        }
      অন্যথা যদি (size)
        {
          pass_size = size;
          size = 0;
        }
      /* TODO: consider handling tail packing by
         writing the tail padding as a separate pass,
         (that would not rewind).  */
      অন্যথা
        ভঙ্গ;

      প্রতি (size_t i = 0; i < flags->n_iterations + flags->zero_fill; i++)
        {
          পূর্ণাঙ্ক err = 0;
          পূর্ণাঙ্ক type = i < flags->n_iterations ? passarray[i] : 0;

          err = dopass (fd, &st, qname, &pass_size, type, rs, i + 1, pn);

          যদি (err)
            {
              ok = false;
              যদি (err < 0)
                গচ্ছ wipefd_out;
            }
        }
    }

  /* Now deallocate the data.  The effect of ftruncate is specified
     on regular files and shared memory objects (also directories, but
     they are not possible here); don't worry about errors reported
     for other file types.  */

  যদি (flags->remove_file && ftruncate (fd, 0) != 0
      && (S_ISREG (st.st_mode) || S_TYPEISSHM (&st)))
    {
      error (0, errno, _("%s: error truncating"), qname);
      ok = false;
      গচ্ছ wipefd_out;
    }

wipefd_out:
  মুক্ত (passarray);
  প্রত্যাগম ok;
}

/* A wrapper with a little more checking for fds on the command line */
স্থির bool
wipefd (পূর্ণাঙ্ক fd, বর্ণ ধ্রুব *qname, সংরচনা randint_source *s,
        সংরচনা Options ধ্রুব *flags)
{
  পূর্ণাঙ্ক fd_flags = fcntl (fd, F_GETFL);

  যদি (fd_flags < 0)
    {
      error (0, errno, _("%s: fcntl failed"), qname);
      প্রত্যাগম false;
    }
  যদি (fd_flags & O_APPEND)
    {
      error (0, 0, _("%s: cannot shred append-only file descriptor"), qname);
      প্রত্যাগম false;
    }
  প্রত্যাগম do_wipefd (fd, qname, s, flags);
}

/* --- Name-wiping code --- */

/* Characters allowed in a file name - a safe universal set.  */
স্থির বর্ণ ধ্রুব nameset[] =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.";

/* Increment NAME (with LEN bytes).  NAME must be a big-endian base N
   number with the digits taken from nameset.  Return true if successful.
   Otherwise, (because NAME already has the greatest possible value)
   return false.  */

স্থির bool
incname (বর্ণ *name, size_t len)
{
  যাবত্ (len--)
    {
      বর্ণ ধ্রুব *p = strchr (nameset, name[len]);

      /* Given that NAME is composed of bytes from NAMESET,
         P will never be null here.  */

      /* If this character has a successor, use it.  */
      যদি (p[1])
        {
          name[len] = p[1];
          প্রত্যাগম true;
        }

      /* Otherwise, set this digit to 0 and increment the prefix.  */
      name[len] = nameset[0];
    }

  প্রত্যাগম false;
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
স্থির bool
wipename (বর্ণ *oldname, বর্ণ ধ্রুব *qoldname, সংরচনা Options ধ্রুব *flags)
{
  বর্ণ *newname = xstrdup (oldname);
  বর্ণ *base = last_component (newname);
  বর্ণ *dir = dir_name (newname);
  বর্ণ *qdir = xstrdup (quotef (dir));
  bool first = true;
  bool ok = true;
  পূর্ণাঙ্ক dir_fd = -1;

  যদি (flags->remove_file == remove_wipesync)
    dir_fd = open (dir, O_RDONLY | O_DIRECTORY | O_NOCTTY | O_NONBLOCK);

  যদি (flags->verbose)
    error (0, 0, _("%s: removing"), qoldname);

  যদি (flags->remove_file != remove_unlink)
    প্রতি (size_t len = base_len (base); len != 0; len--)
      {
        memset (base, nameset[0], len);
        base[len] = 0;
        bool rename_ok;
        যাবত্ (! (rename_ok = (renameatu (AT_FDCWD, oldname, AT_FDCWD, newname,
                                          RENAME_NOREPLACE)
                               == 0))
               && errno == EEXIST && incname (base, len))
          অনুবর্ত;
        যদি (rename_ok)
          {
            যদি (0 <= dir_fd && dosync (dir_fd, qdir) != 0)
              ok = false;
            যদি (flags->verbose)
              {
                /* People seem to understand this better than talking
                   about renaming OLDNAME.  NEWNAME doesn't need
                   quoting because we picked it.  OLDNAME needs to be
                   quoted only the first time.  */
                বর্ণ ধ্রুব *old = first ? qoldname : oldname;
                error (0, 0,
                       _("%s: renamed to %s"), old, newname);
                first = false;
              }
            memcpy (oldname + (base - newname), base, len + 1);
          }
      }

  যদি (unlink (oldname) != 0)
    {
      error (0, errno, _("%s: failed to remove"), qoldname);
      ok = false;
    }
  অন্যথা যদি (flags->verbose)
    error (0, 0, _("%s: removed"), qoldname);
  যদি (0 <= dir_fd)
    {
      যদি (dosync (dir_fd, qdir) != 0)
        ok = false;
      যদি (close (dir_fd) != 0)
        {
          error (0, errno, _("%s: failed to close"), qdir);
          ok = false;
        }
    }
  মুক্ত (newname);
  মুক্ত (dir);
  মুক্ত (qdir);
  প্রত্যাগম ok;
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
স্থির bool
wipefile (বর্ণ *name, বর্ণ ধ্রুব *qname,
          সংরচনা randint_source *s, সংরচনা Options ধ্রুব *flags)
{
  পূর্ণাঙ্ক fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_দ্বিমিক);
  যদি (fd < 0
      && (errno == EACCES && flags->force)
      && chmod (name, S_IWUSR) == 0)
    fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_দ্বিমিক);
  যদি (fd < 0)
    {
      সংরচনা stat st;
      ধ্রুব পূর্ণাঙ্ক open_errno = errno;

      /* Try to give a more meaningful error message if we attempt to
         open a FIFO with no readers.  */
      যদি (open_errno == ENXIO && 0 <= stat (name, &st)
          && S_ISFIFO (st.st_mode))
        error (0, 0, _("%s: invalid file type"), qname);
      অন্যথা
        error (0, open_errno, _("%s: failed to open for writing"), qname);
      প্রত্যাগম false;
    }

  /* We used O_NONBLOCK to prevent blocking when opening a FIFO.
     Reset that here.  */
  পূর্ণাঙ্ক fd_flags = fcntl (fd, F_GETFL);
  bool ok = 0 <= fd_flags && 0 <= fcntl (fd, F_SETFL, fd_flags & ~O_NONBLOCK);
  যদি (ok)
    ok = do_wipefd (fd, qname, s, flags);
  অন্যথা
    error (0, errno, _("couldn't reset non-blocking mode %s"), qname);

  যদি (close (fd) != 0)
    {
      error (0, errno, _("%s: failed to close"), qname);
      ok = false;
    }
  যদি (ok && flags->remove_file)
    ok = wipename (name, qname, flags);
  প্রত্যাগম ok;
}


/* Buffers for random data.  */
স্থির সংরচনা randint_source *randint_source;

/* Just on general principles, wipe buffers containing information
   that may be related to the possibly-pseudorandom values used during
   shredding.  */
স্থির শূন্য
clear_random_data (শূন্য)
{
  randint_all_free (randint_source);
}


পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  bool ok = true;
  সংরচনা Options flags = {0};
  বর্ণ **file;
  পূর্ণাঙ্ক n_files;
  পূর্ণাঙ্ক c;
  বর্ণ ধ্রুব *random_source = রিক্ত;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  flags.n_iterations = DEFAULT_PASSES;
  flags.size = -1;

  যাবত্ ((c = getopt_long (argc, argv, "fn:s:uvxz", long_opts, রিক্ত)) != -1)
    {
      বিকল্প (c)
        {
        পক্ষ 'f':
          flags.force = true;
          ভঙ্গ;

        পক্ষ 'n':
          flags.n_iterations = xdectoumax (optarg, 0,
                                           MIN (ULONG_MAX,
                                                SIZE_MAX / পরিমাণ (পূর্ণাঙ্ক)), "",
                                           _("invalid number of passes"), 0);
          ভঙ্গ;

        পক্ষ RANDOM_SOURCE_OPTION:
          যদি (random_source && !streq (random_source, optarg))
            error (EXIT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          ভঙ্গ;

        পক্ষ 'u':
          যদি (optarg == রিক্ত)
            flags.remove_file = remove_wipesync;
          অন্যথা
            flags.remove_file = XARGMATCH ("--remove", optarg,
                                           remove_args, remove_methods);
          ভঙ্গ;

        পক্ষ 's':
          flags.size = xnumtoumax (optarg, 0, 0, OFF_T_MAX, "cbBkKMGTPEZYRQ0",
                                   _("invalid file size"), 0, 0);
          ভঙ্গ;

        পক্ষ 'v':
          flags.verbose = true;
          ভঙ্গ;

        পক্ষ 'x':
          flags.exact = true;
          ভঙ্গ;

        পক্ষ 'z':
          flags.zero_fill = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  file = argv + optind;
  n_files = argc - optind;

  যদি (n_files == 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  randint_source = randint_all_new (random_source, SIZE_MAX);
  যদি (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));
  atexit (clear_random_data);

  প্রতি (পূর্ণাঙ্ক i = 0; i < n_files; i++)
    {
      বর্ণ *qname = xstrdup (quotef (file[i]));
      যদি (streq (file[i], "-"))
        {
          ok &= wipefd (STDOUT_FILENO, qname, randint_source, &flags);
        }
      অন্যথা
        {
          /* Plain filename - Note that this overwrites *argv! */
          ok &= wipefile (file[i], qname, randint_source, &flags);
        }
      মুক্ত (qname);
    }

  প্রত্যাগম ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*
 * vim:sw=2:sts=2:
 */
