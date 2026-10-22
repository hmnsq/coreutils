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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "shred"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Colin Plumb")

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ defined __linux__ && HAVE_SYS_MTIO_H
# include <sys/mtio.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "alignalloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fcntl--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gethrxtime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "human.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "randint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "randread.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "renameatu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-size.h"

/* Default number of times to overwrite.  */
ਗਿਣਤੀ { DEFAULT_PASSES = 3 };

/* How many seconds to wait before checking whether to output another
   verbose output line.  */
ਗਿਣਤੀ { VERBOSE_UPDATE = 5 };

/* Sector size and corresponding mask, for recovering after write failures.
   The size must be a power of 2.  */
ਗਿਣਤੀ { SECTOR_ਮਾਪ = 512 };
ਗਿਣਤੀ { SECTOR_ਨਕਾਬ = SECTOR_ਮਾਪ - 1 };
static_assert (0 < SECTOR_ਮਾਪ && (SECTOR_ਮਾਪ & SECTOR_ਨਕਾਬ) == 0);

ਗਿਣਤੀ remove_ਵਿਧੀ
{
  remove_none = 0,      /* the default: only wipe data.  */
  remove_unlink,        /* don't obfuscate name, just unlink.  */
  remove_wipe,          /* obfuscate name before unlink.  */
  remove_wipesync       /* obfuscate name, syncing each byte, before unlink.  */
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ remove_ਦਲੀਲਾਂ[] =
{
  "unlink", "wipe", "wipesync", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ remove_ਵਿਧੀ ਅਚਲ remove_methods[] =
{
  remove_unlink, remove_wipe, remove_wipesync
};

ਬਣਤਰ Options
{
  bool force;		/* -f flag: chmod files if necessary */
  size_t n_iterations;	/* -n flag: Number of iterations */
  off_t ਮਾਪ;		/* -s flag: size of file */
  ਗਿਣਤੀ remove_ਵਿਧੀ remove_file; /* -u flag: remove file after shredding */
  bool verbose;		/* -v flag: Print progress */
  bool exact;		/* -x flag: Do not round up file size */
  bool zero_fill;	/* -z flag: Add a final zero pass */
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  RANDOM_ਸਰੋਤ_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_opts[] =
{
  {"exact", no_ਦਲੀਲ, ਨਿਰਾ, 'x'},
  {"force", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"iterations", required_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"size", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"random-source", required_ਦਲੀਲ, ਨਿਰਾ, RANDOM_ਸਰੋਤ_OPTION},
  {"remove", optional_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"zero", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... FILE...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Overwrite the specified FILE(s) repeatedly, ਵਿੱਚ ਕ੍ਰਮ ਮੰਜ਼ਿਲ make it harder\n\
ਲਈ even very expensive hardware probing ਮੰਜ਼ਿਲ recover the ਅੰਕੜੇ.\n\
"), stdout);
      fputs (_("\
\n\
If FILE is -, shred standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -f, --force\n\
         change permissions ਮੰਜ਼ਿਲ allow writing ਜੇ necessary\n\
"));
      oprintf (_("\
  -n, --iterations=N\n\
         overwrite N times instead ਦਾ the ਮੂਲ (%d)\n\
"), DEFAULT_PASSES);
      oputs (_("\
      --random-ਸਰੋਤ_2=FILE\n\
         get random ਬਾਈਟਾਂ from FILE\n\
"));
      oputs (_("\
  -s, --ਮਾਪ=N\n\
         shred this many ਬਾਈਟਾਂ (suffixes like K, M, G accepted)\n\
"));
      oputs (_("\
  -u\n\
         deallocate and remove file after overwriting\n\
"));
      oputs (_("\
      --remove[=HOW]\n\
         like -u but give control on HOW ਮੰਜ਼ਿਲ delete;  See ਹੇਠਾਂ\n\
"));
      oputs (_("\
  -v, --verbose\n\
         show details ਦਾ ਅੰਕੜੇ and metadata operations performed\n\
"));
      oputs (_("\
  -x, --exact\n\
         ਕਰੋ not ਗੇੜਾ_2 file sizes ਉੱਪਰ ਮੰਜ਼ਿਲ the ਅਗਲਾ ਭਰਿਆ ਖੰਡ;\n\
         this is the ਮੂਲ ਲਈ non-regular files\n\
"));
      oputs (_("\
  -z, --zero\n\
         ਜੋੜੋ a final overwrite with zeros ਮੰਜ਼ਿਲ hide shredding\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
FILE will be skipped with a diagnostic ਸੁਨੇਹਾ_2 ਜੇ it is a FIFO, socket, or\n\
terminal, since its ਅੰਕੜੇ does not reside on disk.\n\
"), stdout);
      fputs (_("\
\n\
Delete FILE(s) ਜੇ --remove (-u) is specified.  The ਮੂਲ is not ਮੰਜ਼ਿਲ remove\n\
the files because it is common ਮੰਜ਼ਿਲ ਚਲਾਓ on device files like /dev/hda,\n\
and those files usually should not be removed.\n\
The optional HOW parameter indicates how ਮੰਜ਼ਿਲ remove a directory ਇੰਦਰਾਜ:\n\
'unlink' => use a standard unlink call.\n\
'wipe' => also ਪਹਿਲਾ obfuscate ਬਾਈਟਾਂ ਵਿੱਚ the name.\n\
'wipesync' => also sync each obfuscated byte ਮੰਜ਼ਿਲ the device.\n\
The ਮੂਲ ਬਹੁਲਕ_2 is 'wipesync', but note it can be expensive.\n\
\n\
"), stdout);
      fputs (_("\
CAUTION: shred assumes the file system and hardware overwrite ਅੰਕੜੇ ਵਿੱਚ ਰੱਖੋ.\n\
Although this is common, many platforms ਚਲਾਓ otherwise.  Also, backups\n\
and mirrors may contain unremovable copies that will let a shredded file\n\
be recovered later.  See the GNU coreutils manual ਲਈ details.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/*
 * Determine if pattern type is periodic or not.
 */
ਸਥਿਰ bool
periodic_ਨਮੂਨਾ (ਪੂਰਨ type)
{
  ਜੇ (type <= 0)
    ਵਾਪਸ false;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ r[3];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੱਟ_2 = type & 0xfff;

  ਬਿੱਟ_2 |= ਬਿੱਟ_2 << 12;
  r[0] = (ਬਿੱਟ_2 >> 4) & 255;
  r[1] = (ਬਿੱਟ_2 >> 8) & 255;
  r[2] = ਬਿੱਟ_2 & 255;

  ਵਾਪਸ (r[0] != r[1]) || (r[0] != r[2]);
}

/*
 * Fill a buffer with a fixed pattern.
 *
 * The buffer must be at least 3 bytes long, even if
 * size is less.  Larger sizes are filled exactly.
 */
ਸਥਿਰ ਖਾਲੀ
fillpattern (ਪੂਰਨ type, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *r, size_t ਮਾਪ)
{
  size_t i;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੱਟ_2 = type & 0xfff;

  ਬਿੱਟ_2 |= ਬਿੱਟ_2 << 12;
  r[0] = (ਬਿੱਟ_2 >> 4) & 255;
  r[1] = (ਬਿੱਟ_2 >> 8) & 255;
  r[2] = ਬਿੱਟ_2 & 255;
  ਲਈ (i = 3; i <= ਮਾਪ / 2; i *= 2)
    ਸਮ੍ਰਿਤੀ_ਨਕਲ (r + i, r, i);
  ਜੇ (i < ਮਾਪ)
    ਸਮ੍ਰਿਤੀ_ਨਕਲ (r + i, r, ਮਾਪ - i);

  /* Invert the first bit of every sector. */
  ਜੇ (type & 0x1000)
    ਲਈ (i = 0; i < ਮਾਪ; i += SECTOR_ਮਾਪ)
      r[i] ^= 0x80;
}

/*
 * Generate a 6-character (+ nul) pass name string
 * FIXME: allow translation of "random".
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PASS_NAME_ਮਾਪ 7
ਸਥਿਰ ਖਾਲੀ
passname (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅਚਲ *ਅੰਕੜੇ, ਅੱਖਰ name[PASS_NAME_ਮਾਪ])
{
  ਜੇ (ਅੰਕੜੇ)
    sprintf (name, "%02x%02x%02x", ਅੰਕੜੇ[0], ਅੰਕੜੇ[1], ਅੰਕੜੇ[2]);
  ਨਹੀਂਤਾਂ
    ਸਮ੍ਰਿਤੀ_ਨਕਲ (name, "random", PASS_NAME_ਮਾਪ);
}

/* Return true when it's ok to ignore an fsync or fdatasync
   failure that set errno to ERRNO_VAL.  */
ਸਥਿਰ bool
ignorable_sync_errno (ਪੂਰਨ errno_val)
{
  ਵਾਪਸ (errno_val == EINVAL
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
ਸਥਿਰ ਪੂਰਨ
dosync (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *qname)
{
  ਪੂਰਨ err;

  ਜੇ (fdatasync (fd) == 0)
    ਵਾਪਸ 0;
  err = errno;
  ਜੇ ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fdatasync failed"), qname);
      errno = err;
      ਵਾਪਸ -1;
    }

  ਜੇ (fsync (fd) == 0)
    ਵਾਪਸ 0;
  err = errno;
  ਜੇ ( ! ignorable_sync_errno (err))
    {
      error (0, err, _("%s: fsync failed"), qname);
      errno = err;
      ਵਾਪਸ -1;
    }

  sync ();
  ਵਾਪਸ 0;
}

/* Turn on or off direct I/O mode for file descriptor FD, if possible.
   Try to turn it on if ENABLE is true.  Otherwise, try to turn it off.  */
ਸਥਿਰ ਖਾਲੀ
ਸਿੱਧਾ_ਬਹੁਲਕ (ਪੂਰਨ fd, bool enable)
{
  ਜੇ (O_ਸਿੱਧਾ)
    {
      ਪੂਰਨ fd_flags = fcntl (fd, F_GETFL);
      ਜੇ (0 < fd_flags)
        {
          ਪੂਰਨ new_flags = (enable
                           ? (fd_flags | O_ਸਿੱਧਾ)
                           : (fd_flags & ~O_ਸਿੱਧਾ));
          ਜੇ (new_flags != fd_flags)
            fcntl (fd, F_SETFL, new_flags);
        }
    }

#ਜੇ HAVE_DIRECTIO && defined DIRECTIO_ON && defined DIRECTIO_OFF
  /* This is Solaris-specific.  */
  directio (fd, enable ? DIRECTIO_ON : DIRECTIO_OFF);
#ਜੇ_ਅੰਤ
}

/* Rewind FD; its status is ST.  */
ਸਥਿਰ bool
dorewind (ਪੂਰਨ fd, ਬਣਤਰ stat ਅਚਲ *st)
{
  ਜੇ (S_ISCHR (st->st_ਬਹੁਲਕ))
    {
#ਜੇ defined __linux__ && HAVE_SYS_MTIO_H
      /* In the Linux kernel, lseek does not work on tape devices; it
         returns a randomish value instead.  Try the low-level tape
         rewind operation first.  */
      ਬਣਤਰ mtop op;
      op.mt_op = MTREW;
      op.mt_ਗਿਣਤੀ = 1;
      ਜੇ (ioctl (fd, MTIOCTOP, &op) == 0)
        ਵਾਪਸ true;
#ਜੇ_ਅੰਤ
    }
  off_t ਵਿੱਥ = lseek (fd, 0, SEEK_SET);
  ਜੇ (0 < ਵਿੱਥ)
    errno = EINVAL;
  ਵਾਪਸ ਵਿੱਥ == 0;
}

/* By convention, negative sizes represent unknown values.  */

ਸਥਿਰ bool
ਗਿਆਤ (off_t ਮਾਪ)
{
  ਵਾਪਸ 0 <= ਮਾਪ;
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
ਸਥਿਰ ਪੂਰਨ
dopass (ਪੂਰਨ fd, ਬਣਤਰ stat ਅਚਲ *st, ਅੱਖਰ ਅਚਲ *qname, off_t *sizep,
        ਪੂਰਨ type, ਬਣਤਰ randread_ਸਰੋਤ *s,
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ k, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ n)
{
  off_t ਮਾਪ = *sizep;
  off_t ਵਿੱਥ;                /* Current file position */
  xtime_t prev IF_LINT ( = 0); /* Time we printed the previous update.  */
  xtime_t now = 0;             /* Current time */
  size_t lim;                  /* Amount of data to try writing */
  size_t soff;                 /* Offset into buffer for next write */
  ssize_t ssize;               /* Return value from write */

  /* Fill pattern buffer.  Aligning it to a page so we can do direct I/O.  */
  size_t ਪੰਨਾ_ਮਾਪ = getpagesize ();
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PERIODIC_OUTPUT_ਮਾਪ (60 * 1024)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NONPERIODIC_OUTPUT_ਮਾਪ (64 * 1024)
  static_assert (PERIODIC_OUTPUT_ਮਾਪ % 3 == 0);
  size_t output_ਮਾਪ = periodic_ਨਮੂਨਾ (type)
                       ? PERIODIC_OUTPUT_ਮਾਪ : NONPERIODIC_OUTPUT_ਮਾਪ;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FILLPATTERN_ਮਾਪ (((output_ਮਾਪ + 2) / 3) * 3) /* Multiple of 3 */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *pbuf = xalignalloc (ਪੰਨਾ_ਮਾਪ, FILLPATTERN_ਮਾਪ);

  ਅੱਖਰ pass_string[PASS_NAME_ਮਾਪ];	/* Name of current pass */
  bool write_error = false;
  bool other_error = false;

  /* Printable previous offset into the file */
  ਅੱਖਰ ਪਿਛਲਾ_ਵਿੱਥ_buf[LONGEST_HUMAN_READABLE + 1];
  ਅੱਖਰ ਅਚਲ *ਪਿਛਲਾ_human_ਵਿੱਥ;

  /* As a performance tweak, avoid direct I/O for small sizes,
     as it's just a performance rather than security consideration,
     and direct I/O can often be unsupported for small non aligned sizes.  */
  bool try_without_directio = 0 < ਮਾਪ && ਮਾਪ < output_ਮਾਪ;
  ਜੇ (! try_without_directio)
    ਸਿੱਧਾ_ਬਹੁਲਕ (fd, true);

  ਜੇ (! dorewind (fd, st))
    {
      error (0, errno, _("%s: cannot rewind"), qname);
      other_error = true;
      ਜਾਓ free_ਨਮੂਨਾ_mem;
    }

  /* Constant fill patterns need only be set up once. */
  ਜੇ (type >= 0)
    {
      lim = ਗਿਆਤ (ਮਾਪ) && ਮਾਪ < FILLPATTERN_ਮਾਪ ? ਮਾਪ : FILLPATTERN_ਮਾਪ;
      fillpattern (type, pbuf, lim);
      passname (pbuf, pass_string);
    }
  ਨਹੀਂਤਾਂ
    {
      passname (ਨਿਰਾ, pass_string);
    }

  /* Set position if first status update */
  ਜੇ (n)
    {
      error (0, 0, _("%s: pass %lu/%lu (%s)..."), qname, k, n, pass_string);
      prev = gethrxtime ();
      ਪਿਛਲਾ_human_ਵਿੱਥ = "";
    }

  ਵਿੱਥ = 0;
  ਜਦੋਂ (true)
    {
      /* How much to write this time? */
      lim = output_ਮਾਪ;
      ਜੇ (ਗਿਆਤ (ਮਾਪ) && ਮਾਪ - ਵਿੱਥ < output_ਮਾਪ)
        {
          ਜੇ (ਮਾਪ < ਵਿੱਥ)
            ਰੋਕੋ;
          lim = ਮਾਪ - ਵਿੱਥ;
          ਜੇ (!lim)
            ਰੋਕੋ;
        }
      ਜੇ (type < 0)
        randread (s, pbuf, lim);
      /* Loop to retry partial writes. */
      ਲਈ (soff = 0; soff < lim; soff += ssize)
        {
          ssize = ਲਿਖ (fd, pbuf + soff, lim - soff);
          ਜੇ (ssize <= 0)
            {
              ਜੇ (! ਗਿਆਤ (ਮਾਪ) && (ssize == 0 || errno == ENOSPC))
                {
                  /* We have found the end of the file.  */
                  ਜੇ (soff <= OFF_T_MAX - ਵਿੱਥ)
                    *sizep = ਮਾਪ = ਵਿੱਥ + soff;
                  ਰੋਕੋ;
                }
              ਨਹੀਂਤਾਂ
                {
                  ਪੂਰਨ errnum = errno;

                  /* Retry without direct I/O since this may not be supported
                     at all on some (file) systems, or with the current size.
                     I.e., a specified --size that is not aligned, or when
                     dealing with slop at the end of a file with --exact.  */
                  ਜੇ (! try_without_directio && errno == EINVAL)
                    {
                      ਸਿੱਧਾ_ਬਹੁਲਕ (fd, false);
                      ssize = 0;
                      try_without_directio = true;
                      ਜਾਰੀ;
                    }
                  uintmax_t error_ਵਿੱਥ = ਵਿੱਥ + soff;
                  error (0, errnum, _("%s: error writing at offset %ju"),
                         qname, error_ਵਿੱਥ);

                  /* 'shred' is often used on bad media, before throwing it
                     out.  Thus, it shouldn't give up on bad blocks.  This
                     code works because lim is always a multiple of
                     SECTOR_SIZE, except at the end.  This size constraint
                     also enables direct I/O on some (file) systems.  */
                  static_assert (PERIODIC_OUTPUT_ਮਾਪ % SECTOR_ਮਾਪ == 0);
                  static_assert (NONPERIODIC_OUTPUT_ਮਾਪ % SECTOR_ਮਾਪ == 0);
                  ਜੇ (errnum == EIO && ਗਿਆਤ (ਮਾਪ)
                      && (soff | SECTOR_ਨਕਾਬ) < lim)
                    {
                      size_t soff1 = (soff | SECTOR_ਨਕਾਬ) + 1;
                      ਜੇ (lseek (fd, ਵਿੱਥ + soff1, SEEK_SET) != -1)
                        {
                          /* Arrange to skip this block. */
                          ssize = soff1 - soff;
                          write_error = true;
                          ਜਾਰੀ;
                        }
                      error (0, errno, _("%s: lseek failed"), qname);
                    }
                  other_error = true;
                  ਜਾਓ free_ਨਮੂਨਾ_mem;
                }
            }
        }

      /* Okay, we have written "soff" bytes. */

      ਜੇ (ckd_ਜੋੜੋ (&ਵਿੱਥ, ਵਿੱਥ, soff))
        {
          error (0, 0, _("%s: file too large"), qname);
          other_error = true;
          ਜਾਓ free_ਨਮੂਨਾ_mem;
        }

      bool ਪੂਰਾ = ਵਿੱਥ == ਮਾਪ;

      /* Time to print progress? */
      ਜੇ (n && ((ਪੂਰਾ && *ਪਿਛਲਾ_human_ਵਿੱਥ)
                || VERBOSE_UPDATE <= xtime_sec ((now = gethrxtime ()) - prev)))
        {
          ਅੱਖਰ ਵਿੱਥ_buf[LONGEST_HUMAN_READABLE + 1];
          ਅੱਖਰ ਮਾਪ_buf[LONGEST_HUMAN_READABLE + 1];
          ਪੂਰਨ human_ਤਰੱਕੀ_opts = (human_autoscale | human_SI
                                     | human_ਅਧਾਰ_1024 | human_B);
          ਅੱਖਰ ਅਚਲ *human_ਵਿੱਥ
            = human_readable (ਵਿੱਥ, ਵਿੱਥ_buf,
                              human_floor | human_ਤਰੱਕੀ_opts, 1, 1);

          ਜੇ (ਪੂਰਾ || !streq (ਪਿਛਲਾ_human_ਵਿੱਥ, human_ਵਿੱਥ))
            {
              ਜੇ (! ਗਿਆਤ (ਮਾਪ))
                error (0, 0, _("%s: pass %lu/%lu (%s)...%s"),
                       qname, k, n, pass_string, human_ਵਿੱਥ);
              ਨਹੀਂਤਾਂ
                {
                  uintmax_t off = ਵਿੱਥ;
                  ਪੂਰਨ percent = (ਮਾਪ == 0
                                 ? 100
                                 : (off <= UINTMAX_MAX / 100
                                    ? off * 100 / ਮਾਪ
                                    : off / (ਮਾਪ / 100)));
                  ਅੱਖਰ ਅਚਲ *human_ਮਾਪ
                    = human_readable (ਮਾਪ, ਮਾਪ_buf,
                                      human_ceiling | human_ਤਰੱਕੀ_opts,
                                      1, 1);
                  ਜੇ (ਪੂਰਾ)
                    human_ਵਿੱਥ = human_ਮਾਪ;
                  error (0, 0, _("%s: pass %lu/%lu (%s)...%s/%s %d%%"),
                         qname, k, n, pass_string, human_ਵਿੱਥ, human_ਮਾਪ,
                         percent);
                }

              strcpy (ਪਿਛਲਾ_ਵਿੱਥ_buf, human_ਵਿੱਥ);
              ਪਿਛਲਾ_human_ਵਿੱਥ = ਪਿਛਲਾ_ਵਿੱਥ_buf;
              prev = now;

              /*
               * Force periodic syncs to keep displayed progress accurate
               * FIXME: Should these be present even if -v is not enabled,
               * to keep the buffer cache from filling with dirty pages?
               * It's a common problem with programs that do lots of writes,
               * like mkfs.
               */
              ਜੇ (dosync (fd, qname) != 0)
                {
                  ਜੇ (errno != EIO)
                    {
                      other_error = true;
                      ਜਾਓ free_ਨਮੂਨਾ_mem;
                    }
                  write_error = true;
                }
            }
        }
    }

  /* Force what we just wrote to hit the media. */
  ਜੇ (dosync (fd, qname) != 0)
    {
      ਜੇ (errno != EIO)
        {
          other_error = true;
          ਜਾਓ free_ਨਮੂਨਾ_mem;
        }
      write_error = true;
    }

free_ਨਮੂਨਾ_mem:
  alignfree (pbuf);

  ਵਾਪਸ other_error ? -1 : write_error;
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
ਸਥਿਰ ਪੂਰਨ ਅਚਲ
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
  0				/*ਸਿਰਾ*/
};

/*
 * Generate a random wiping pass pattern with num passes.
 * This is a two-stage process.  First, the passes to include
 * are chosen, and then they are shuffled into the desired
 * order.
 */
ਸਥਿਰ ਖਾਲੀ
genpattern (ਪੂਰਨ *dest, size_t ਅੰਕ, ਬਣਤਰ randint_ਸਰੋਤ *s)
{
  size_t randpasses;
  ਪੂਰਨ ਅਚਲ *p;
  ਪੂਰਨ *d;
  size_t n;
  size_t accum, ਸਿਖਰ, ਅਦਲਾ_ਬਦਲੀ;
  ਪੂਰਨ k;

  ਜੇ (!ਅੰਕ)
    ਵਾਪਸ;

  /* Stage 1: choose the passes to use */
  p = patterns;
  randpasses = 0;
  d = dest;			/* Destination for generated pass list */
  n = ਅੰਕ;			/* Passes remaining to fill */

  ਜਦੋਂ (true)
    {
      k = *p++;			/* Block descriptor word */
      ਜੇ (!k)
        {			/* Loop back to the beginning */
          p = patterns;
        }
      ਨਹੀਂਤਾਂ ਜੇ (k < 0)
        {			/* -k random passes */
          k = -k;
          ਜੇ ((size_t) k >= n)
            {
              randpasses += n;
              ਰੋਕੋ;
            }
          randpasses += k;
          n -= k;
        }
      ਨਹੀਂਤਾਂ ਜੇ ((size_t) k <= n)
        {			/* Full block of patterns */
          ਸਮ੍ਰਿਤੀ_ਨਕਲ (d, p, k * ਆਕਾਰ (ਪੂਰਨ));
          p += k;
          d += k;
          n -= k;
        }
      ਨਹੀਂਤਾਂ ਜੇ (n < 2 || 3 * n < (size_t) k)
        {			/* Finish with random */
          randpasses += n;
          ਰੋਕੋ;
        }
      ਨਹੀਂਤਾਂ
        {			/* Pad out with n of the k available */
          ਕਰੋ
            {
              ਜੇ (n == (size_t) k || randint_ਚੁਣੋ (s, k) < n)
                {
                  *d++ = *p;
                  n--;
                }
              p++;
              k--;
            }
          ਜਦੋਂ (n);
          ਰੋਕੋ;
        }
    }
  ਸਿਖਰ = ਅੰਕ - randpasses;	/* Top of initialized data */
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
  ਲਈ (n = 0; n < ਅੰਕ; n++)
    {
      ਜੇ (accum <= randpasses)
        {
          accum += ਅੰਕ - 1;
          dest[ਸਿਖਰ++] = dest[n];
          dest[n] = -1;
        }
      ਨਹੀਂਤਾਂ
        {
          ਅਦਲਾ_ਬਦਲੀ = n + randint_ਚੁਣੋ (s, ਸਿਖਰ - n);
          k = dest[n];
          dest[n] = dest[ਅਦਲਾ_ਬਦਲੀ];
          dest[ਅਦਲਾ_ਬਦਲੀ] = k;
        }
      accum -= randpasses;
    }
  /* affirm (top == num); */
}

/*
 * The core routine to actually do the work.  This overwrites the first
 * size bytes of the given fd.  Return true if successful.
 */
ਸਥਿਰ bool
do_wipefd (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *qname, ਬਣਤਰ randint_ਸਰੋਤ *s,
           ਬਣਤਰ Options ਅਚਲ *flags)
{
  ਬਣਤਰ stat st;
  off_t ਮਾਪ;		/* Size to write, size to read */
  off_t i_ਮਾਪ = 0;	/* For small files, initial size to overwrite inode */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ n;	/* Number of passes for printing purposes */
  ਪੂਰਨ *passarray;
  bool ok = true;
  ਬਣਤਰ randread_ਸਰੋਤ *rs;

  n = 0;		/* dopass takes n == 0 to mean "don't print progress" */
  ਜੇ (flags->verbose)
    n = flags->n_iterations + flags->zero_fill;

  ਜੇ (fstat (fd, &st))
    {
      error (0, errno, _("%s: fstat failed"), qname);
      ਵਾਪਸ false;
    }

  /* If we know that we can't possibly shred the file, give up now.
     Otherwise, we may go into an infinite loop writing data before we
     find that we can't rewind the device.  */
  ਜੇ ((S_ISCHR (st.st_ਬਹੁਲਕ) && isatty (fd))
      || S_ISFIFO (st.st_ਬਹੁਲਕ)
      || S_ISSOCK (st.st_ਬਹੁਲਕ))
    {
      error (0, 0, _("%s: invalid file type"), qname);
      ਵਾਪਸ false;
    }
  ਨਹੀਂਤਾਂ ਜੇ (S_ISREG (st.st_ਬਹੁਲਕ) && st.st_ਮਾਪ < 0)
    {
      error (0, 0, _("%s: file has negative size"), qname);
      ਵਾਪਸ false;
    }

  /* Allocate pass array */
  passarray = xnmalloc (flags->n_iterations, ਆਕਾਰ *passarray);

  ਮਾਪ = flags->ਮਾਪ;
  ਜੇ (ਮਾਪ == -1)
    {
      ਜੇ (S_ISREG (st.st_ਬਹੁਲਕ))
        {
          ਮਾਪ = st.st_ਮਾਪ;

          ਜੇ (! flags->exact)
            {
              /* Round up to the nearest block size to clear slack space.  */
              off_t remainder = ਮਾਪ % STP_BLKSIZE (&st);
              ਜੇ (ਮਾਪ && ਮਾਪ < STP_BLKSIZE (&st))
                i_ਮਾਪ = ਮਾਪ;
              ਜੇ (remainder != 0)
                {
                  off_t ਮਾਪ_incr = STP_BLKSIZE (&st) - remainder;
                  ਮਾਪ += MIN (ਮਾਪ_incr, OFF_T_MAX - ਮਾਪ);
                }
            }
        }
      ਨਹੀਂਤਾਂ
        {
          /* The behavior of lseek is unspecified, but in practice if
             it returns a positive number that's the size of this
             device.  */
          ਮਾਪ = lseek (fd, 0, SEEK_END);
          ਜੇ (ਮਾਪ <= 0)
            {
              /* We are unable to determine the length, up front.
                 Let dopass do that as part of its first iteration.  */
              ਮਾਪ = -1;
            }
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (S_ISREG (st.st_ਬਹੁਲਕ)
           && st.st_ਮਾਪ < MIN (STP_BLKSIZE (&st), ਮਾਪ))
    i_ਮਾਪ = st.st_ਮਾਪ;

  /* Schedule the passes in random order. */
  genpattern (passarray, flags->n_iterations, s);

  rs = randint_get_ਸਰੋਤ (s);

  ਜਦੋਂ (true)
    {
      off_t pass_ਮਾਪ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ pn = n;

      ਜੇ (i_ਮਾਪ)
        {
          pass_ਮਾਪ = i_ਮਾਪ;
          i_ਮਾਪ = 0;
          pn = 0;
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਮਾਪ)
        {
          pass_ਮਾਪ = ਮਾਪ;
          ਮਾਪ = 0;
        }
      /* TODO: consider handling tail packing by
         writing the tail padding as a separate pass,
         (that would not rewind).  */
      ਨਹੀਂਤਾਂ
        ਰੋਕੋ;

      ਲਈ (size_t i = 0; i < flags->n_iterations + flags->zero_fill; i++)
        {
          ਪੂਰਨ err = 0;
          ਪੂਰਨ type = i < flags->n_iterations ? passarray[i] : 0;

          err = dopass (fd, &st, qname, &pass_ਮਾਪ, type, rs, i + 1, pn);

          ਜੇ (err)
            {
              ok = false;
              ਜੇ (err < 0)
                ਜਾਓ wipefd_ਬਾਹਰਲਾ;
            }
        }
    }

  /* Now deallocate the data.  The effect of ftruncate is specified
     on regular files and shared memory objects (also directories, but
     they are not possible here); don't worry about errors reported
     for other file types.  */

  ਜੇ (flags->remove_file && ftruncate (fd, 0) != 0
      && (S_ISREG (st.st_ਬਹੁਲਕ) || S_TYPEISSHM (&st)))
    {
      error (0, errno, _("%s: error truncating"), qname);
      ok = false;
      ਜਾਓ wipefd_ਬਾਹਰਲਾ;
    }

wipefd_ਬਾਹਰਲਾ:
  ਮੁਕਤ (passarray);
  ਵਾਪਸ ok;
}

/* A wrapper with a little more checking for fds on the command line */
ਸਥਿਰ bool
wipefd (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *qname, ਬਣਤਰ randint_ਸਰੋਤ *s,
        ਬਣਤਰ Options ਅਚਲ *flags)
{
  ਪੂਰਨ fd_flags = fcntl (fd, F_GETFL);

  ਜੇ (fd_flags < 0)
    {
      error (0, errno, _("%s: fcntl failed"), qname);
      ਵਾਪਸ false;
    }
  ਜੇ (fd_flags & O_APPEND)
    {
      error (0, 0, _("%s: cannot shred append-only file descriptor"), qname);
      ਵਾਪਸ false;
    }
  ਵਾਪਸ do_wipefd (fd, qname, s, flags);
}

/* --- Name-wiping code --- */

/* Characters allowed in a file name - a safe universal set.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ nameset[] =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.";

/* Increment NAME (with LEN bytes).  NAME must be a big-endian base N
   number with the digits taken from nameset.  Return true if successful.
   Otherwise, (because NAME already has the greatest possible value)
   return false.  */

ਸਥਿਰ bool
incname (ਅੱਖਰ *name, size_t len)
{
  ਜਦੋਂ (len--)
    {
      ਅੱਖਰ ਅਚਲ *p = strchr (nameset, name[len]);

      /* Given that NAME is composed of bytes from NAMESET,
         P will never be null here.  */

      /* If this character has a successor, use it.  */
      ਜੇ (p[1])
        {
          name[len] = p[1];
          ਵਾਪਸ true;
        }

      /* Otherwise, set this digit to 0 and increment the prefix.  */
      name[len] = nameset[0];
    }

  ਵਾਪਸ false;
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
ਸਥਿਰ bool
wipename (ਅੱਖਰ *oldname, ਅੱਖਰ ਅਚਲ *qoldname, ਬਣਤਰ Options ਅਚਲ *flags)
{
  ਅੱਖਰ *newname = xstrdup (oldname);
  ਅੱਖਰ *ਅਧਾਰ_2 = ਆਖਰੀ_component (newname);
  ਅੱਖਰ *dir = dir_name (newname);
  ਅੱਖਰ *qdir = xstrdup (quotef (dir));
  bool ਪਹਿਲਾ = true;
  bool ok = true;
  ਪੂਰਨ dir_fd = -1;

  ਜੇ (flags->remove_file == remove_wipesync)
    dir_fd = open (dir, O_RDONLY | O_DIRECTORY | O_NOCTTY | O_NONBLOCK);

  ਜੇ (flags->verbose)
    error (0, 0, _("%s: removing"), qoldname);

  ਜੇ (flags->remove_file != remove_unlink)
    ਲਈ (size_t len = ਅਧਾਰ_len (ਅਧਾਰ_2); len != 0; len--)
      {
        memset (ਅਧਾਰ_2, nameset[0], len);
        ਅਧਾਰ_2[len] = 0;
        bool rename_ok;
        ਜਦੋਂ (! (rename_ok = (renameatu (AT_FDCWD, oldname, AT_FDCWD, newname,
                                          RENAME_NOREPLACE)
                               == 0))
               && errno == EEXIST && incname (ਅਧਾਰ_2, len))
          ਜਾਰੀ;
        ਜੇ (rename_ok)
          {
            ਜੇ (0 <= dir_fd && dosync (dir_fd, qdir) != 0)
              ok = false;
            ਜੇ (flags->verbose)
              {
                /* People seem to understand this better than talking
                   about renaming OLDNAME.  NEWNAME doesn't need
                   quoting because we picked it.  OLDNAME needs to be
                   quoted only the first time.  */
                ਅੱਖਰ ਅਚਲ *old = ਪਹਿਲਾ ? qoldname : oldname;
                error (0, 0,
                       _("%s: renamed to %s"), old, newname);
                ਪਹਿਲਾ = false;
              }
            ਸਮ੍ਰਿਤੀ_ਨਕਲ (oldname + (ਅਧਾਰ_2 - newname), ਅਧਾਰ_2, len + 1);
          }
      }

  ਜੇ (unlink (oldname) != 0)
    {
      error (0, errno, _("%s: failed to remove"), qoldname);
      ok = false;
    }
  ਨਹੀਂਤਾਂ ਜੇ (flags->verbose)
    error (0, 0, _("%s: removed"), qoldname);
  ਜੇ (0 <= dir_fd)
    {
      ਜੇ (dosync (dir_fd, qdir) != 0)
        ok = false;
      ਜੇ (close (dir_fd) != 0)
        {
          error (0, errno, _("%s: failed to close"), qdir);
          ok = false;
        }
    }
  ਮੁਕਤ (newname);
  ਮੁਕਤ (dir);
  ਮੁਕਤ (qdir);
  ਵਾਪਸ ok;
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
ਸਥਿਰ bool
wipefile (ਅੱਖਰ *name, ਅੱਖਰ ਅਚਲ *qname,
          ਬਣਤਰ randint_ਸਰੋਤ *s, ਬਣਤਰ Options ਅਚਲ *flags)
{
  ਪੂਰਨ fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_ਦੁਆਧਾਰੀ);
  ਜੇ (fd < 0
      && (errno == EACCES && flags->force)
      && chmod (name, S_IWUSR) == 0)
    fd = open (name, O_WRONLY | O_NOCTTY | O_NONBLOCK | O_ਦੁਆਧਾਰੀ);
  ਜੇ (fd < 0)
    {
      ਬਣਤਰ stat st;
      ਅਚਲ ਪੂਰਨ open_errno = errno;

      /* Try to give a more meaningful error message if we attempt to
         open a FIFO with no readers.  */
      ਜੇ (open_errno == ENXIO && 0 <= stat (name, &st)
          && S_ISFIFO (st.st_ਬਹੁਲਕ))
        error (0, 0, _("%s: invalid file type"), qname);
      ਨਹੀਂਤਾਂ
        error (0, open_errno, _("%s: failed to open for writing"), qname);
      ਵਾਪਸ false;
    }

  /* We used O_NONBLOCK to prevent blocking when opening a FIFO.
     Reset that here.  */
  ਪੂਰਨ fd_flags = fcntl (fd, F_GETFL);
  bool ok = 0 <= fd_flags && 0 <= fcntl (fd, F_SETFL, fd_flags & ~O_NONBLOCK);
  ਜੇ (ok)
    ok = do_wipefd (fd, qname, s, flags);
  ਨਹੀਂਤਾਂ
    error (0, errno, _("couldn't reset non-blocking mode %s"), qname);

  ਜੇ (close (fd) != 0)
    {
      error (0, errno, _("%s: failed to close"), qname);
      ok = false;
    }
  ਜੇ (ok && flags->remove_file)
    ok = wipename (name, qname, flags);
  ਵਾਪਸ ok;
}


/* Buffers for random data.  */
ਸਥਿਰ ਬਣਤਰ randint_ਸਰੋਤ *randint_ਸਰੋਤ;

/* Just on general principles, wipe buffers containing information
   that may be related to the possibly-pseudorandom values used during
   shredding.  */
ਸਥਿਰ ਖਾਲੀ
clear_random_ਅੰਕੜੇ (ਖਾਲੀ)
{
  randint_all_free (randint_ਸਰੋਤ);
}


ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool ok = true;
  ਬਣਤਰ Options flags = {0};
  ਅੱਖਰ **file;
  ਪੂਰਨ n_files;
  ਪੂਰਨ c;
  ਅੱਖਰ ਅਚਲ *random_ਸਰੋਤ = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  flags.n_iterations = DEFAULT_PASSES;
  flags.ਮਾਪ = -1;

  ਜਦੋਂ ((c = getopt_long (argc, argv, "fn:s:uvxz", long_opts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'f':
          flags.force = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'n':
          flags.n_iterations = xdectoumax (optarg, 0,
                                           MIN (ULONG_MAX,
                                                SIZE_MAX / ਆਕਾਰ (ਪੂਰਨ)), "",
                                           _("invalid number of passes"), 0);
          ਰੋਕੋ;

        ਹਾਲਤ RANDOM_ਸਰੋਤ_OPTION:
          ਜੇ (random_ਸਰੋਤ && !streq (random_ਸਰੋਤ, optarg))
            error (EXIT_FAILURE, 0, _("multiple random sources specified"));
          random_ਸਰੋਤ = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          ਜੇ (optarg == ਨਿਰਾ)
            flags.remove_file = remove_wipesync;
          ਨਹੀਂਤਾਂ
            flags.remove_file = XARGMATCH ("--remove", optarg,
                                           remove_ਦਲੀਲਾਂ, remove_methods);
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          flags.ਮਾਪ = xnumtoumax (optarg, 0, 0, OFF_T_MAX, "cbBkKMGTPEZYRQ0",
                                   _("invalid file size"), 0, 0);
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          flags.verbose = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'x':
          flags.exact = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          flags.zero_fill = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  file = argv + optind;
  n_files = argc - optind;

  ਜੇ (n_files == 0)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  randint_ਸਰੋਤ = randint_all_new (random_ਸਰੋਤ, SIZE_MAX);
  ਜੇ (! randint_ਸਰੋਤ)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_ਸਰੋਤ ? random_ਸਰੋਤ : "getrandom"));
  atexit (clear_random_ਅੰਕੜੇ);

  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    {
      ਅੱਖਰ *qname = xstrdup (quotef (file[i]));
      ਜੇ (streq (file[i], "-"))
        {
          ok &= wipefd (STDOUT_FILENO, qname, randint_ਸਰੋਤ, &flags);
        }
      ਨਹੀਂਤਾਂ
        {
          /* Plain filename - Note that this overwrites *argv! */
          ok &= wipefile (file[i], qname, randint_ਸਰੋਤ, &flags);
        }
      ਮੁਕਤ (qname);
    }

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
/*
 * vim:sw=2:sts=2:
 */
