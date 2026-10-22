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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#எனில் defined __linux__ && HAVE_SYS_MTIO_H
# include <sys/mtio.h>
#endif

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "alignalloc.h"
#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "xdectoint.h"
#அந்தர்பாவ "fcntl--.h"
#அந்தர்பாவ "gethrxtime.h"
#அந்தர்பாவ "human.h"
#அந்தர்பாவ "randint.h"
#அந்தர்பாவ "randread.h"
#அந்தர்பாவ "renameatu.h"
#அந்தர்பாவ "stat-size.h"

/* Default number of times to overwrite.  */
எண்ணிக்கை { DEFAULT_PASSES = 3 };

/* How many seconds to wait before checking whether to output another
   verbose output line.  */
எண்ணிக்கை { VERBOSE_UPDATE = 5 };

/* Sector size and corresponding mask, for recovering after write failures.
   The size must be a power of 2.  */
எண்ணிக்கை { SECTOR_SIZE = 512 };
எண்ணிக்கை { SECTOR_MASK = SECTOR_SIZE - 1 };
static_assert (0 < SECTOR_SIZE && (SECTOR_SIZE & SECTOR_MASK) == 0);

எண்ணிக்கை remove_method
{
  remove_none = 0,      /* the default: only wipe data.  */
  remove_unlink,        /* don't obfuscate name, just unlink.  */
  remove_wipe,          /* obfuscate name before unlink.  */
  remove_wipesync       /* obfuscate name, syncing each byte, before unlink.  */
};

நிலையான எழுத்து மாறிலி *மாறிலி remove_args[] =
{
  "unlink", "wipe", "wipesync", வெற்று
};

நிலையான எண்ணிக்கை remove_method மாறிலி remove_methods[] =
{
  remove_unlink, remove_wipe, remove_wipesync
};

கட்டமைப்பு Options
{
  bool force;		/* -f flag: chmod files if necessary */
  size_t n_iterations;	/* -n flag: Number of iterations */
  off_t size;		/* -s flag: size of file */
  எண்ணிக்கை remove_method remove_file; /* -u flag: remove file after shredding */
  bool verbose;		/* -v flag: Print progress */
  bool exact;		/* -x flag: Do not round up file size */
  bool zero_fill;	/* -z flag: Add a final zero pass */
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி long_opts[] =
{
  {"exact", no_argument, வெற்று, 'x'},
  {"force", no_argument, வெற்று, 'f'},
  {"iterations", required_argument, வெற்று, 'n'},
  {"size", required_argument, வெற்று, 's'},
  {"random-source", required_argument, வெற்று, RANDOM_SOURCE_OPTION},
  {"remove", optional_argument, வெற்று, 'u'},
  {"verbose", no_argument, வெற்று, 'v'},
  {"zero", no_argument, வெற்று, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Overwrite the specified FILE(s) repeatedly, in order to make it harder\n\
க்கு even very expensive hardware probing to recover the data.\n\
"), stdout);
      fputs (_("\
\n\
If FILE is -, shred standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --force\n\
         change permissions to allow writing எனில் necessary\n\
"));
      oprintf (_("\
  -n, --iterations=N\n\
         overwrite N times instead of the இயல்பு (%d)\n\
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
         செய் not round file sizes up to the next full block;\n\
         this is the இயல்பு க்கு non-regular files\n\
"));
      oputs (_("\
  -z, --zero\n\
         add a final overwrite with zeros to hide shredding\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FILE will be skipped with a diagnostic message எனில் it is a FIFO, socket, or\n\
terminal, since its data does not reside on disk.\n\
"), stdout);
      fputs (_("\
\n\
Delete FILE(s) எனில் --remove (-u) is specified.  The இயல்பு is not to remove\n\
the files because it is common to operate on device files like /dev/hda,\n\
and those files usually should not be removed.\n\
The optional HOW parameter indicates how to remove a directory entry:\n\
'unlink' => use a standard unlink call.\n\
'wipe' => also first obfuscate bytes in the name.\n\
'wipesync' => also sync each obfuscated byte to the device.\n\
The இயல்பு mode is 'wipesync', but note it can be expensive.\n\
\n\
"), stdout);
      fputs (_("\
CAUTION: shred assumes the file system and hardware overwrite data in place.\n\
Although this is common, many platforms operate otherwise.  Also, backups\n\
and mirrors may contain unremovable copies that will let a shredded file\n\
be recovered later.  See the GNU coreutils manual க்கு details.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/*
 * Determine if pattern type is periodic or not.
 */
நிலையான bool
periodic_pattern (முழு type)
{
  எனில் (type <= 0)
    திரும்பு false;

  unsigned எழுத்து r[3];
  unsigned முழு bits = type & 0xfff;

  bits |= bits << 12;
  r[0] = (bits >> 4) & 255;
  r[1] = (bits >> 8) & 255;
  r[2] = bits & 255;

  திரும்பு (r[0] != r[1]) || (r[0] != r[2]);
}

/*
 * Fill a buffer with a fixed pattern.
 *
 * The buffer must be at least 3 bytes long, even if
 * size is less.  Larger sizes are filled exactly.
 */
நிலையான வெறுமை
fillpattern (முழு type, unsigned எழுத்து *r, size_t size)
{
  size_t i;
  unsigned முழு bits = type & 0xfff;

  bits |= bits << 12;
  r[0] = (bits >> 4) & 255;
  r[1] = (bits >> 8) & 255;
  r[2] = bits & 255;
  க்கு (i = 3; i <= size / 2; i *= 2)
    memcpy (r + i, r, i);
  எனில் (i < size)
    memcpy (r + i, r, size - i);

  /* Invert the first bit of every sector. */
  எனில் (type & 0x1000)
    க்கு (i = 0; i < size; i += SECTOR_SIZE)
      r[i] ^= 0x80;
}

/*
 * Generate a 6-character (+ nul) pass name string
 * FIXME: allow translation of "random".
 */
#define PASS_NAME_SIZE 7
நிலையான வெறுமை
passname (unsigned எழுத்து மாறிலி *data, எழுத்து name[PASS_NAME_SIZE])
{
  எனில் (data)
    sprintf (name, "%02x%02x%02x", data[0], data[1], data[2]);
  இல்லையெனில்
    memcpy (name, "random", PASS_NAME_SIZE);
}

/* Return true when it's ok to ignore an fsync or fdatasync
   failure that set errno to ERRNO_VAL.  */
நிலையான bool
ignorable_sync_errno (முழு errno_val)
{
  திரும்பு (errno_val == EINVAL
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
நிலையான முழு
dosync (முழு fd, எழுத்து மாறிலி *qname)
{
  முழு err;

  எனில் (fdatasync (fd) == 0)
    திரும்பு 0;
  err = errno;
  எனில் ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fdatasync failed"), qname);
      errno = err;
      திரும்பு -1;
    }

  எனில் (fsync (fd) == 0)
    திரும்பு 0;
  err = errno;
  எனில் ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fsync failed"), qname);
      errno = err;
      திரும்பு -1;
    }

  sync ();
  திரும்பு 0;
}

/* Turn on or off direct I/O mode for file descriptor FD, if possible.
   Try to turn it on if ENABLE is true.  Otherwise, try to turn it off.  */
நிலையான வெறுமை
direct_mode (முழு fd, bool enable)
{
  எனில் (O_DIRECT)
    {
      முழு fd_flags = fcntl (fd, F_GETFL);
      எனில் (0 < fd_flags)
        {
          முழு new_flags = (enable
                           ? (fd_flags | O_DIRECT)
                           : (fd_flags & ~O_DIRECT));
          எனில் (new_flags != fd_flags)
            fcntl (fd, F_SETFL, new_flags);
        }
    }

#எனில் HAVE_DIRECTIO && defined DIRECTIO_ON && defined DIRECTIO_OFF
  /* This is Solaris-specific.  */
  directio (fd, enable ? DIRECTIO_ON : DIRECTIO_OFF);
#endif
}

/* Rewind FD; its status is ST.  */
நிலையான bool
dorewind (முழு fd, கட்டமைப்பு stat மாறிலி *st)
{
  எனில் (S_ISCHR (st->st_mode))
    {
#எனில் defined __linux__ && HAVE_SYS_MTIO_H
      /* In the Linux kernel, lseek does not work on tape devices; it
         returns a randomish value instead.  Try the low-level tape
         rewind operation first.  */
      கட்டமைப்பு mtop op;
      op.mt_op = MTREW;
      op.mt_மொத்தம் = 1;
      எனில் (ioctl (fd, MTIOCTOP, &op) == 0)
        திரும்பு true;
#endif
    }
  off_t offset = lseek (fd, 0, SEEK_SET);
  எனில் (0 < offset)
    errno = EINVAL;
  திரும்பு offset == 0;
}

/* By convention, negative sizes represent unknown values.  */

நிலையான bool
known (off_t size)
{
  திரும்பு 0 <= size;
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
நிலையான முழு
dopass (முழு fd, கட்டமைப்பு stat மாறிலி *st, எழுத்து மாறிலி *qname, off_t *sizep,
        முழு type, கட்டமைப்பு randread_source *s,
        unsigned long முழு k, unsigned long முழு n)
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
  unsigned எழுத்து *pbuf = xalignalloc (page_size, FILLPATTERN_SIZE);

  எழுத்து pass_string[PASS_NAME_SIZE];	/* Name of current pass */
  bool write_error = false;
  bool other_error = false;

  /* Printable previous offset into the file */
  எழுத்து previous_offset_buf[LONGEST_HUMAN_READABLE + 1];
  எழுத்து மாறிலி *previous_human_offset;

  /* As a performance tweak, avoid direct I/O for small sizes,
     as it's just a performance rather than security consideration,
     and direct I/O can often be unsupported for small non aligned sizes.  */
  bool try_without_directio = 0 < size && size < output_size;
  எனில் (! try_without_directio)
    direct_mode (fd, true);

  எனில் (! dorewind (fd, st))
    {
      error (0, errno, _("%s: cannot rewind"), qname);
      other_error = true;
      செல் free_pattern_mem;
    }

  /* Constant fill patterns need only be set up once. */
  எனில் (type >= 0)
    {
      lim = known (size) && size < FILLPATTERN_SIZE ? size : FILLPATTERN_SIZE;
      fillpattern (type, pbuf, lim);
      passname (pbuf, pass_string);
    }
  இல்லையெனில்
    {
      passname (வெற்று, pass_string);
    }

  /* Set position if first status update */
  எனில் (n)
    {
      error (0, 0, _("%s: pass %lu/%lu (%s)..."), qname, k, n, pass_string);
      prev = gethrxtime ();
      previous_human_offset = "";
    }

  offset = 0;
  போது (true)
    {
      /* How much to write this time? */
      lim = output_size;
      எனில் (known (size) && size - offset < output_size)
        {
          எனில் (size < offset)
            நிறுத்து;
          lim = size - offset;
          எனில் (!lim)
            நிறுத்து;
        }
      எனில் (type < 0)
        randread (s, pbuf, lim);
      /* Loop to retry partial writes. */
      க்கு (soff = 0; soff < lim; soff += ssize)
        {
          ssize = லிக (fd, pbuf + soff, lim - soff);
          எனில் (ssize <= 0)
            {
              எனில் (! known (size) && (ssize == 0 || errno == ENOSPC))
                {
                  /* We have found the end of the file.  */
                  எனில் (soff <= OFF_T_MAX - offset)
                    *sizep = size = offset + soff;
                  நிறுத்து;
                }
              இல்லையெனில்
                {
                  முழு errnum = errno;

                  /* Retry without direct I/O since this may not be supported
                     at all on some (file) systems, or with the current size.
                     I.e., a specified --size that is not aligned, or when
                     dealing with slop at the end of a file with --exact.  */
                  எனில் (! try_without_directio && errno == EINVAL)
                    {
                      direct_mode (fd, false);
                      ssize = 0;
                      try_without_directio = true;
                      தொடர்;
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
                  எனில் (errnum == EIO && known (size)
                      && (soff | SECTOR_MASK) < lim)
                    {
                      size_t soff1 = (soff | SECTOR_MASK) + 1;
                      எனில் (lseek (fd, offset + soff1, SEEK_SET) != -1)
                        {
                          /* Arrange to skip this block. */
                          ssize = soff1 - soff;
                          write_error = true;
                          தொடர்;
                        }
                      error (0, errno, _("%s: lseek failed"), qname);
                    }
                  other_error = true;
                  செல் free_pattern_mem;
                }
            }
        }

      /* Okay, we have written "soff" bytes. */

      எனில் (ckd_add (&offset, offset, soff))
        {
          error (0, 0, _("%s: file too large"), qname);
          other_error = true;
          செல் free_pattern_mem;
        }

      bool done = offset == size;

      /* Time to print progress? */
      எனில் (n && ((done && *previous_human_offset)
                || VERBOSE_UPDATE <= xtime_sec ((now = gethrxtime ()) - prev)))
        {
          எழுத்து offset_buf[LONGEST_HUMAN_READABLE + 1];
          எழுத்து size_buf[LONGEST_HUMAN_READABLE + 1];
          முழு human_progress_opts = (human_autoscale | human_SI
                                     | human_base_1024 | human_B);
          எழுத்து மாறிலி *human_offset
            = human_readable (offset, offset_buf,
                              human_floor | human_progress_opts, 1, 1);

          எனில் (done || !streq (previous_human_offset, human_offset))
            {
              எனில் (! known (size))
                error (0, 0, _("%s: pass %lu/%lu (%s)...%s"),
                       qname, k, n, pass_string, human_offset);
              இல்லையெனில்
                {
                  uintmax_t off = offset;
                  முழு percent = (size == 0
                                 ? 100
                                 : (off <= UINTMAX_MAX / 100
                                    ? off * 100 / size
                                    : off / (size / 100)));
                  எழுத்து மாறிலி *human_size
                    = human_readable (size, size_buf,
                                      human_ceiling | human_progress_opts,
                                      1, 1);
                  எனில் (done)
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
              எனில் (dosync (fd, qname) != 0)
                {
                  எனில் (errno != EIO)
                    {
                      other_error = true;
                      செல் free_pattern_mem;
                    }
                  write_error = true;
                }
            }
        }
    }

  /* Force what we just wrote to hit the media. */
  எனில் (dosync (fd, qname) != 0)
    {
      எனில் (errno != EIO)
        {
          other_error = true;
          செல் free_pattern_mem;
        }
      write_error = true;
    }

free_pattern_mem:
  alignfree (pbuf);

  திரும்பு other_error ? -1 : write_error;
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
நிலையான முழு மாறிலி
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
நிலையான வெறுமை
genpattern (முழு *dest, size_t இலக்கம், கட்டமைப்பு randint_source *s)
{
  size_t randpasses;
  முழு மாறிலி *p;
  முழு *d;
  size_t n;
  size_t accum, உச்சி, swap;
  முழு k;

  எனில் (!இலக்கம்)
    திரும்பு;

  /* Stage 1: choose the passes to use */
  p = patterns;
  randpasses = 0;
  d = dest;			/* Destination for generated pass list */
  n = இலக்கம்;			/* Passes remaining to fill */

  போது (true)
    {
      k = *p++;			/* Block descriptor word */
      எனில் (!k)
        {			/* Loop back to the beginning */
          p = patterns;
        }
      இல்லையெனில் எனில் (k < 0)
        {			/* -k random passes */
          k = -k;
          எனில் ((size_t) k >= n)
            {
              randpasses += n;
              நிறுத்து;
            }
          randpasses += k;
          n -= k;
        }
      இல்லையெனில் எனில் ((size_t) k <= n)
        {			/* Full block of patterns */
          memcpy (d, p, k * அளவு (முழு));
          p += k;
          d += k;
          n -= k;
        }
      இல்லையெனில் எனில் (n < 2 || 3 * n < (size_t) k)
        {			/* Finish with random */
          randpasses += n;
          நிறுத்து;
        }
      இல்லையெனில்
        {			/* Pad out with n of the k available */
          செய்
            {
              எனில் (n == (size_t) k || randint_choose (s, k) < n)
                {
                  *d++ = *p;
                  n--;
                }
              p++;
              k--;
            }
          போது (n);
          நிறுத்து;
        }
    }
  உச்சி = இலக்கம் - randpasses;	/* Top of initialized data */
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
  க்கு (n = 0; n < இலக்கம்; n++)
    {
      எனில் (accum <= randpasses)
        {
          accum += இலக்கம் - 1;
          dest[உச்சி++] = dest[n];
          dest[n] = -1;
        }
      இல்லையெனில்
        {
          swap = n + randint_choose (s, உச்சி - n);
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
நிலையான bool
do_wipefd (முழு fd, எழுத்து மாறிலி *qname, கட்டமைப்பு randint_source *s,
           கட்டமைப்பு Options மாறிலி *flags)
{
  கட்டமைப்பு stat st;
  off_t size;		/* Size to write, size to read */
  off_t i_size = 0;	/* For small files, initial size to overwrite inode */
  unsigned long முழு n;	/* Number of passes for printing purposes */
  முழு *passarray;
  bool ok = true;
  கட்டமைப்பு randread_source *rs;

  n = 0;		/* dopass takes n == 0 to mean "don't print progress" */
  எனில் (flags->verbose)
    n = flags->n_iterations + flags->zero_fill;

  எனில் (fstat (fd, &st))
    {
      error (0, errno, _("%s: fstat failed"), qname);
      திரும்பு false;
    }

  /* If we know that we can't possibly shred the file, give up now.
     Otherwise, we may go into an infinite loop writing data before we
     find that we can't rewind the device.  */
  எனில் ((S_ISCHR (st.st_mode) && isatty (fd))
      || S_ISFIFO (st.st_mode)
      || S_ISSOCK (st.st_mode))
    {
      error (0, 0, _("%s: invalid file type"), qname);
      திரும்பு false;
    }
  இல்லையெனில் எனில் (S_ISREG (st.st_mode) && st.st_size < 0)
    {
      error (0, 0, _("%s: file has negative size"), qname);
      திரும்பு false;
    }

  /* Allocate pass array */
  passarray = xnmalloc (flags->n_iterations, அளவு *passarray);

  size = flags->size;
  எனில் (size == -1)
    {
      எனில் (S_ISREG (st.st_mode))
        {
          size = st.st_size;

          எனில் (! flags->exact)
            {
              /* Round up to the nearest block size to clear slack space.  */
              off_t remainder = size % STP_BLKSIZE (&st);
              எனில் (size && size < STP_BLKSIZE (&st))
                i_size = size;
              எனில் (remainder != 0)
                {
                  off_t size_incr = STP_BLKSIZE (&st) - remainder;
                  size += MIN (size_incr, OFF_T_MAX - size);
                }
            }
        }
      இல்லையெனில்
        {
          /* The behavior of lseek is unspecified, but in practice if
             it returns a positive number that's the size of this
             device.  */
          size = lseek (fd, 0, SEEK_END);
          எனில் (size <= 0)
            {
              /* We are unable to determine the length, up front.
                 Let dopass do that as part of its first iteration.  */
              size = -1;
            }
        }
    }
  இல்லையெனில் எனில் (S_ISREG (st.st_mode)
           && st.st_size < MIN (STP_BLKSIZE (&st), size))
    i_size = st.st_size;

  /* Schedule the passes in random order. */
  genpattern (passarray, flags->n_iterations, s);

  rs = randint_get_source (s);

  போது (true)
    {
      off_t pass_size;
      unsigned long முழு pn = n;

      எனில் (i_size)
        {
          pass_size = i_size;
          i_size = 0;
          pn = 0;
        }
      இல்லையெனில் எனில் (size)
        {
          pass_size = size;
          size = 0;
        }
      /* TODO: consider handling tail packing by
         writing the tail padding as a separate pass,
         (that would not rewind).  */
      இல்லையெனில்
        நிறுத்து;

      க்கு (size_t i = 0; i < flags->n_iterations + flags->zero_fill; i++)
        {
          முழு err = 0;
          முழு type = i < flags->n_iterations ? passarray[i] : 0;

          err = dopass (fd, &st, qname, &pass_size, type, rs, i + 1, pn);

          எனில் (err)
            {
              ok = false;
              எனில் (err < 0)
                செல் wipefd_out;
            }
        }
    }

  /* Now deallocate the data.  The effect of ftruncate is specified
     on regular files and shared memory objects (also directories, but
     they are not possible here); don't worry about errors reported
     for other file types.  */

  எனில் (flags->remove_file && ftruncate (fd, 0) != 0
      && (S_ISREG (st.st_mode) || S_TYPEISSHM (&st)))
    {
      error (0, errno, _("%s: error truncating"), qname);
      ok = false;
      செல் wipefd_out;
    }

wipefd_out:
  விடுவி (passarray);
  திரும்பு ok;
}

/* A wrapper with a little more checking for fds on the command line */
நிலையான bool
wipefd (முழு fd, எழுத்து மாறிலி *qname, கட்டமைப்பு randint_source *s,
        கட்டமைப்பு Options மாறிலி *flags)
{
  முழு fd_flags = fcntl (fd, F_GETFL);

  எனில் (fd_flags < 0)
    {
      error (0, errno, _("%s: fcntl failed"), qname);
      திரும்பு false;
    }
  எனில் (fd_flags & O_APPEND)
    {
      error (0, 0, _("%s: cannot shred append-only file descriptor"), qname);
      திரும்பு false;
    }
  திரும்பு do_wipefd (fd, qname, s, flags);
}

/* --- Name-wiping code --- */

/* Characters allowed in a file name - a safe universal set.  */
நிலையான எழுத்து மாறிலி nameset[] =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.";

/* Increment NAME (with LEN bytes).  NAME must be a big-endian base N
   number with the digits taken from nameset.  Return true if successful.
   Otherwise, (because NAME already has the greatest possible value)
   return false.  */

நிலையான bool
incname (எழுத்து *name, size_t len)
{
  போது (len--)
    {
      எழுத்து மாறிலி *p = strchr (nameset, name[len]);

      /* Given that NAME is composed of bytes from NAMESET,
         P will never be null here.  */

      /* If this character has a successor, use it.  */
      எனில் (p[1])
        {
          name[len] = p[1];
          திரும்பு true;
        }

      /* Otherwise, set this digit to 0 and increment the prefix.  */
      name[len] = nameset[0];
    }

  திரும்பு false;
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
நிலையான bool
wipename (எழுத்து *oldname, எழுத்து மாறிலி *qoldname, கட்டமைப்பு Options மாறிலி *flags)
{
  எழுத்து *newname = xstrdup (oldname);
  எழுத்து *base = last_component (newname);
  எழுத்து *dir = dir_name (newname);
  எழுத்து *qdir = xstrdup (quotef (dir));
  bool first = true;
  bool ok = true;
  முழு dir_fd = -1;

  எனில் (flags->remove_file == remove_wipesync)
    dir_fd = open (dir, O_RDONLY | O_DIRECTORY | O_NOCTTY | O_NONBLOCK);

  எனில் (flags->verbose)
    error (0, 0, _("%s: removing"), qoldname);

  எனில் (flags->remove_file != remove_unlink)
    க்கு (size_t len = base_len (base); len != 0; len--)
      {
        memset (base, nameset[0], len);
        base[len] = 0;
        bool rename_ok;
        போது (! (rename_ok = (renameatu (AT_FDCWD, oldname, AT_FDCWD, newname,
                                          RENAME_NOREPLACE)
                               == 0))
               && errno == EEXIST && incname (base, len))
          தொடர்;
        எனில் (rename_ok)
          {
            எனில் (0 <= dir_fd && dosync (dir_fd, qdir) != 0)
              ok = false;
            எனில் (flags->verbose)
              {
                /* People seem to understand this better than talking
                   about renaming OLDNAME.  NEWNAME doesn't need
                   quoting because we picked it.  OLDNAME needs to be
                   quoted only the first time.  */
                எழுத்து மாறிலி *old = first ? qoldname : oldname;
                error (0, 0,
                       _("%s: renamed to %s"), old, newname);
                first = false;
              }
            memcpy (oldname + (base - newname), base, len + 1);
          }
      }

  எனில் (unlink (oldname) != 0)
    {
      error (0, errno, _("%s: failed to remove"), qoldname);
      ok = false;
    }
  இல்லையெனில் எனில் (flags->verbose)
    error (0, 0, _("%s: removed"), qoldname);
  எனில் (0 <= dir_fd)
    {
      எனில் (dosync (dir_fd, qdir) != 0)
        ok = false;
      எனில் (close (dir_fd) != 0)
        {
          error (0, errno, _("%s: failed to close"), qdir);
          ok = false;
        }
    }
  விடுவி (newname);
  விடுவி (dir);
  விடுவி (qdir);
  திரும்பு ok;
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
நிலையான bool
wipefile (எழுத்து *name, எழுத்து மாறிலி *qname,
          கட்டமைப்பு randint_source *s, கட்டமைப்பு Options மாறிலி *flags)
{
  முழு fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_இருமம்);
  எனில் (fd < 0
      && (errno == EACCES && flags->force)
      && chmod (name, S_IWUSR) == 0)
    fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_இருமம்);
  எனில் (fd < 0)
    {
      கட்டமைப்பு stat st;
      மாறிலி முழு open_errno = errno;

      /* Try to give a more meaningful error message if we attempt to
         open a FIFO with no readers.  */
      எனில் (open_errno == ENXIO && 0 <= stat (name, &st)
          && S_ISFIFO (st.st_mode))
        error (0, 0, _("%s: invalid file type"), qname);
      இல்லையெனில்
        error (0, open_errno, _("%s: failed to open for writing"), qname);
      திரும்பு false;
    }

  /* We used O_NONBLOCK to prevent blocking when opening a FIFO.
     Reset that here.  */
  முழு fd_flags = fcntl (fd, F_GETFL);
  bool ok = 0 <= fd_flags && 0 <= fcntl (fd, F_SETFL, fd_flags & ~O_NONBLOCK);
  எனில் (ok)
    ok = do_wipefd (fd, qname, s, flags);
  இல்லையெனில்
    error (0, errno, _("couldn't reset non-blocking mode %s"), qname);

  எனில் (close (fd) != 0)
    {
      error (0, errno, _("%s: failed to close"), qname);
      ok = false;
    }
  எனில் (ok && flags->remove_file)
    ok = wipename (name, qname, flags);
  திரும்பு ok;
}


/* Buffers for random data.  */
நிலையான கட்டமைப்பு randint_source *randint_source;

/* Just on general principles, wipe buffers containing information
   that may be related to the possibly-pseudorandom values used during
   shredding.  */
நிலையான வெறுமை
clear_random_data (வெறுமை)
{
  randint_all_free (randint_source);
}


முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool ok = true;
  கட்டமைப்பு Options flags = {0};
  எழுத்து **file;
  முழு n_files;
  முழு c;
  எழுத்து மாறிலி *random_source = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  flags.n_iterations = DEFAULT_PASSES;
  flags.size = -1;

  போது ((c = getopt_long (argc, argv, "fn:s:uvxz", long_opts, வெற்று)) != -1)
    {
      தேர்வு (c)
        {
        நிலை 'f':
          flags.force = true;
          நிறுத்து;

        நிலை 'n':
          flags.n_iterations = xdectoumax (optarg, 0,
                                           MIN (ULONG_MAX,
                                                SIZE_MAX / அளவு (முழு)), "",
                                           _("invalid number of passes"), 0);
          நிறுத்து;

        நிலை RANDOM_SOURCE_OPTION:
          எனில் (random_source && !streq (random_source, optarg))
            error (EXIT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          நிறுத்து;

        நிலை 'u':
          எனில் (optarg == வெற்று)
            flags.remove_file = remove_wipesync;
          இல்லையெனில்
            flags.remove_file = XARGMATCH ("--remove", optarg,
                                           remove_args, remove_methods);
          நிறுத்து;

        நிலை 's':
          flags.size = xnumtoumax (optarg, 0, 0, OFF_T_MAX, "cbBkKMGTPEZYRQ0",
                                   _("invalid file size"), 0, 0);
          நிறுத்து;

        நிலை 'v':
          flags.verbose = true;
          நிறுத்து;

        நிலை 'x':
          flags.exact = true;
          நிறுத்து;

        நிலை 'z':
          flags.zero_fill = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  file = argv + optind;
  n_files = argc - optind;

  எனில் (n_files == 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  randint_source = randint_all_new (random_source, SIZE_MAX);
  எனில் (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));
  atexit (clear_random_data);

  க்கு (முழு i = 0; i < n_files; i++)
    {
      எழுத்து *qname = xstrdup (quotef (file[i]));
      எனில் (streq (file[i], "-"))
        {
          ok &= wipefd (STDOUT_FILENO, qname, randint_source, &flags);
        }
      இல்லையெனில்
        {
          /* Plain filename - Note that this overwrites *argv! */
          ok &= wipefile (file[i], qname, randint_source, &flags);
        }
      விடுவி (qname);
    }

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*
 * vim:sw=2:sts=2:
 */
