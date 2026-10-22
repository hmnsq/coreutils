/* dd -- convert a file while copying it.
   Copyright (C) 1985-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Paul Rubin, David MacKenzie, and Stuart Kemp. */

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "alignalloc.h"
#ಅನ್ತರ್ಭಾವ "close-stream.h"
#ಅನ್ತರ್ಭಾವ "fd-reopen.h"
#ಅನ್ತರ್ಭಾವ "gethrxtime.h"
#ಅನ್ತರ್ಭಾವ "human.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "long-options.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"
#ಅನ್ತರ್ಭಾವ "xtime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "dd"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Stuart Kemp")

/* NonStop circa 2011 lacks SA_RESETHAND; see Bug#9076.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SA_RESETHAND
# define SA_RESETHAND 0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SIGINFO
# define SIGINFO SIGUSR1
#ಆದರೆ_ಅಂತ್ಯ

/* This may belong in GNULIB's fcntl module instead.
   Define O_CIO to 0 if it is not supported by this OS. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ O_CIO
# define O_CIO 0
#ಆದರೆ_ಅಂತ್ಯ

/* On AIX 5.1 and AIX 5.2, O_NOCACHE is defined via <fcntl.h>
   and would interfere with our use of that name, below.  */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು O_NOCACHE

#ವ್ಯಾಖ್ಯಾನಿಸು output_char(c)				\
  ಮಾಡು						\
    {						\
      obuf[oc++] = (c);				\
      ಆದರೆ (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  ಆಗ (0)

/* Default input and output blocksize. */
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
ಎಣಿಕೆ
  {
    C_ASCII = 01,

    C_EBCDIC = 02,
    C_IBM = 04,
    C_ಖಂಡ = 010,
    C_UNBLOCK = 020,
    C_LCASE = 040,
    C_UCASE = 0100,
    C_SWAB = 0200,
    C_NOERROR = 0400,
    C_NOTRUNC = 01000,
    C_SYNC = 02000,

    /* Use separate input and output buffers, and combine partial
       input blocks. */
    C_TWOBUFS = 04000,

    C_NOCREAT = 010000,
    C_EXCL = 020000,
    C_FDATASYNC = 040000,
    C_FSYNC = 0100000,

    C_SPARSE = 0200000
  };

/* Status levels.  */
ಎಣಿಕೆ
  {
    ಸ್ಥಿತಿ_NONE = 1,
    ಸ್ಥಿತಿ_NOXFER = 2,
    ಸ್ಥಿತಿ_DEFAULT = 3,
    ಸ್ಥಿತಿ_ಪ್ರಗತಿ = 4
  };

/* The name of the input file, or NULL for the standard input. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಒಳಹರಿವು_file = ಖಾಲಿ;

/* The name of the output file, or NULL for the standard output. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *output_file = ಖಾಲಿ;

/* The page size on this host.  */
ಸ್ಥಿರ idx_t ಪುಟ_ಗಾತ್ರ;

/* The number of bytes in which atomic reads are done. */
ಸ್ಥಿರ idx_t ಒಳಹರಿವು_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
ಸ್ಥಿರ idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
ಸ್ಥಿರ idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
ಸ್ಥಿರ intmax_t skip_ದಾಖಲೆಗಳು = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
ಸ್ಥಿರ idx_t skip_ಬೈಟುಗಳು = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
ಸ್ಥಿರ intmax_t seek_ದಾಖಲೆಗಳು = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
ಸ್ಥಿರ intmax_t seek_ಬೈಟುಗಳು = 0;

/* Whether the final output was done with a seek (rather than a write).  */
ಸ್ಥಿರ bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
ಸ್ಥಿರ intmax_t max_ದಾಖಲೆಗಳು = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
ಸ್ಥಿರ idx_t max_ಬೈಟುಗಳು = 0;

/* Bit vector of conversions to apply. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ conversions_ಮುಖವಾಡ = 0;

/* Open flags for the input and output files.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_flags = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ output_flags = 0;

/* Status flags for what is printed to stderr.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_level = ಸ್ಥಿತಿ_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
ಸ್ಥಿರ bool translation_needed = false;

/* Number of partial blocks written. */
ಸ್ಥಿರ intmax_t w_partial = 0;

/* Number of full blocks written. */
ಸ್ಥಿರ intmax_t w_ತುಂಬಿದ = 0;

/* Number of partial blocks read. */
ಸ್ಥಿರ intmax_t r_partial = 0;

/* Number of full blocks read. */
ಸ್ಥಿರ intmax_t r_ತುಂಬಿದ = 0;

/* Number of bytes written.  */
ಸ್ಥಿರ intmax_t w_ಬೈಟುಗಳು = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
ಸ್ಥಿರ intmax_t reported_w_ಬೈಟುಗಳು = -1;

/* Time that dd started.  */
ಸ್ಥಿರ xtime_t ಆರಂಭ_ಸಮಯ;

/* Next time to report periodic progress.  */
ಸ್ಥಿರ xtime_t ಮುಂದಿನ_ಸಮಯ;

/* If positive, the number of bytes output in the current progress line.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ_len;

/* True if input is seekable.  */
ಸ್ಥಿರ bool ಒಳಹರಿವು_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
ಸ್ಥಿರ off_t ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ;

/* True if a partial read should be diagnosed.  */
ಸ್ಥಿರ bool warn_partial_read;

/* Records truncated by conv=block. */
ಸ್ಥಿರ intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
ಸ್ಥಿರ ಅಕ್ಷರ newline_character = '\n';
ಸ್ಥಿರ ಅಕ್ಷರ space_character = ' ';

/* I/O buffers.  */
ಸ್ಥಿರ ಅಕ್ಷರ *ibuf;
ಸ್ಥಿರ ಅಕ್ಷರ *obuf;

/* Current index into 'obuf'. */
ಸ್ಥಿರ idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
ಸ್ಥಿರ idx_t ಲಂಬಸಾಲು = 0;

/* The set of signals that are caught.  */
ಸ್ಥಿರ sigset_t caught_ಸಂಕೇತಗಳು;

/* If nonzero, the value of the pending fatal signal.  */
ಸ್ಥಿರ sig_atomic_t ಅಸ್ಥಿರ ಅಡಚಣೆ_signal;

/* A count of the number of pending info signals that have been received.  */
ಸ್ಥಿರ sig_atomic_t ಅಸ್ಥಿರ info_signal_ಎಣಿಕೆ;

/* Whether to discard cache for input or output.  */
ಸ್ಥಿರ bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
ಸ್ಥಿರ bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
ಸ್ಥಿರ ssize_t (*iread_fnc) (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ *buf, idx_t ಗಾತ್ರ_2);

/* A longest symbol in the struct symbol_values tables below.  */
#ವ್ಯಾಖ್ಯಾನಿಸು LONGEST_ಚಿಹ್ನೆ "count_bytes"

/* A symbol and the corresponding integer value.  */
ರಚನೆ ಚಿಹ್ನೆ_ಮೌಲ್ಯ
{
  ಅಕ್ಷರ ಚಿಹ್ನೆ[ಗಾತ್ರ LONGEST_ಚಿಹ್ನೆ];
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
};

/* Conversion symbols, for conv="...".  */
ಸ್ಥಿರ ರಚನೆ ಚಿಹ್ನೆ_ಮೌಲ್ಯ ಸ್ಥಿರಾಂಕ conversions[] =
{
  {"ascii", C_ASCII | C_UNBLOCK | C_TWOBUFS},	/* EBCDIC to ASCII. */
  {"ebcdic", C_EBCDIC | C_ಖಂಡ | C_TWOBUFS},	/* ASCII to EBCDIC. */
  {"ibm", C_IBM | C_ಖಂಡ | C_TWOBUFS},	/* Different ASCII to EBCDIC. */
  {"block", C_ಖಂಡ | C_TWOBUFS},	/* Variable to fixed length records. */
  {"unblock", C_UNBLOCK | C_TWOBUFS},	/* Fixed to variable length records. */
  {"lcase", C_LCASE | C_TWOBUFS},	/* Translate upper to lower case. */
  {"ucase", C_UCASE | C_TWOBUFS},	/* Translate lower to upper case. */
  {"sparse", C_SPARSE},		/* Try to sparsely write output. */
  {"swab", C_SWAB | C_TWOBUFS},	/* Swap bytes of input. */
  {"noerror", C_NOERROR},	/* Ignore i/o errors. */
  {"nocreat", C_NOCREAT},	/* Do not create output file.  */
  {"excl", C_EXCL},		/* Fail if the output file already exists.  */
  {"notrunc", C_NOTRUNC},	/* Do not truncate output file. */
  {"sync", C_SYNC},		/* Pad input records to ibs with NULs. */
  {"fdatasync", C_FDATASYNC},	/* Synchronize output data before finishing.  */
  {"fsync", C_FSYNC},		/* Also synchronize output metadata.  */
  {"", 0}
};

#ವ್ಯಾಖ್ಯಾನಿಸು FFS_ಮುಖವಾಡ(x) ((x) ^ ((x) & ((x) - 1)))
ಎಣಿಕೆ
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_ದ್ವಿಮಾನ
          | O_CIO
          | O_ನೇರ
          | O_DIRECTORY
          | O_DSYNC
          | O_EXCL
          | O_NOATIME
          | O_NOCTTY
          | O_NOFOLLOW
          | O_NOLINKS
          | O_NONBLOCK
          | O_SYNC
          | O_ಪಠ್ಯ
          ),

    /* Use its lowest bits for private flags.  */
    O_FULLBLOCK = FFS_ಮುಖವಾಡ (v),
    v2 = v ^ O_FULLBLOCK,

    O_NOCACHE = FFS_ಮುಖವಾಡ (v2),
    v3 = v2 ^ O_NOCACHE,

    O_ಎಣಿಕೆ_ಬೈಟುಗಳು = FFS_ಮುಖವಾಡ (v3),
    v4 = v3 ^ O_ಎಣಿಕೆ_ಬೈಟುಗಳು,

    O_SKIP_ಬೈಟುಗಳು = FFS_ಮುಖವಾಡ (v4),
    v5 = v4 ^ O_SKIP_ಬೈಟುಗಳು,

    O_SEEK_ಬೈಟುಗಳು = FFS_ಮುಖವಾಡ (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_ಎಣಿಕೆ_ಬೈಟುಗಳು != 0);
static_assert (O_SKIP_ಬೈಟುಗಳು != 0);
static_assert (O_SEEK_ಬೈಟುಗಳು != 0);

#ವ್ಯಾಖ್ಯಾನಿಸು ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ (O_FULLBLOCK));
static_assert ( ! ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ (O_NOCACHE));
static_assert ( ! ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ (O_ಎಣಿಕೆ_ಬೈಟುಗಳು));
static_assert ( ! ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ (O_SKIP_ಬೈಟುಗಳು));
static_assert ( ! ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ (O_SEEK_ಬೈಟುಗಳು));

/* Flags, for iflag="..." and oflag="...".  */
ಸ್ಥಿರ ರಚನೆ ಚಿಹ್ನೆ_ಮೌಲ್ಯ ಸ್ಥಿರಾಂಕ flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_ದ್ವಿಮಾನ},
  {"cio",	  O_CIO},
  {"direct",	  O_ನೇರ},
  {"directory",   O_DIRECTORY},
  {"dsync",	  O_DSYNC},
  {"noatime",	  O_NOATIME},
  {"nocache",	  O_NOCACHE},   /* Discard cache.  */
  {"noctty",	  O_NOCTTY},
  {"nofollow",	  HAVE_WORKING_O_NOFOLLOW ? O_NOFOLLOW : 0},
  {"nolinks",	  O_NOLINKS},
  {"nonblock",	  O_NONBLOCK},
  {"sync",	  O_SYNC},
  {"text",	  O_ಪಠ್ಯ},
  {"fullblock",   O_FULLBLOCK}, /* Accumulate full blocks from input.  */
  {"count_bytes", O_ಎಣಿಕೆ_ಬೈಟುಗಳು},
  {"skip_bytes",  O_SKIP_ಬೈಟುಗಳು},
  {"seek_bytes",  O_SEEK_ಬೈಟುಗಳು},
  {"",		0}
};

/* Status, for status="...".  */
ಸ್ಥಿರ ರಚನೆ ಚಿಹ್ನೆ_ಮೌಲ್ಯ ಸ್ಥಿರಾಂಕ statuses[] =
{
  {"none",	ಸ್ಥಿತಿ_NONE},
  {"noxfer",	ಸ್ಥಿತಿ_NOXFER},
  {"progress",	ಸ್ಥಿತಿ_ಪ್ರಗತಿ},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ trans_ಕೋಷ್ಟಕ[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ ascii_ಗಮ್ಯ_ebcdic[] =
{
  '\000', '\001', '\002', '\003', '\067', '\055', '\056', '\057',
  '\026', '\005', '\045', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\074', '\075', '\062', '\046',
  '\030', '\031', '\077', '\047', '\034', '\035', '\036', '\037',
  '\100', '\132', '\177', '\173', '\133', '\154', '\120', '\175',
  '\115', '\135', '\134', '\116', '\153', '\140', '\113', '\141',
  '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
  '\370', '\371', '\172', '\136', '\114', '\176', '\156', '\157',
  '\174', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
  '\310', '\311', '\321', '\322', '\323', '\324', '\325', '\326',
  '\327', '\330', '\331', '\342', '\343', '\344', '\345', '\346',
  '\347', '\350', '\351', '\255', '\340', '\275', '\232', '\155',
  '\171', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
  '\210', '\211', '\221', '\222', '\223', '\224', '\225', '\226',
  '\227', '\230', '\231', '\242', '\243', '\244', '\245', '\246',
  '\247', '\250', '\251', '\300', '\117', '\320', '\137', '\007',
  '\040', '\041', '\042', '\043', '\044', '\025', '\006', '\027',
  '\050', '\051', '\052', '\053', '\054', '\011', '\012', '\033',
  '\060', '\061', '\032', '\063', '\064', '\065', '\066', '\010',
  '\070', '\071', '\072', '\073', '\004', '\024', '\076', '\341',
  '\101', '\102', '\103', '\104', '\105', '\106', '\107', '\110',
  '\111', '\121', '\122', '\123', '\124', '\125', '\126', '\127',
  '\130', '\131', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\160', '\161', '\162', '\163', '\164', '\165',
  '\166', '\167', '\170', '\200', '\212', '\213', '\214', '\215',
  '\216', '\217', '\220', '\152', '\233', '\234', '\235', '\236',
  '\237', '\240', '\252', '\253', '\254', '\112', '\256', '\257',
  '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
  '\270', '\271', '\272', '\273', '\274', '\241', '\276', '\277',
  '\312', '\313', '\314', '\315', '\316', '\317', '\332', '\333',
  '\334', '\335', '\336', '\337', '\352', '\353', '\354', '\355',
  '\356', '\357', '\372', '\373', '\374', '\375', '\376', '\377'
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ ascii_ಗಮ್ಯ_ibm[] =
{
  '\000', '\001', '\002', '\003', '\067', '\055', '\056', '\057',
  '\026', '\005', '\045', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\074', '\075', '\062', '\046',
  '\030', '\031', '\077', '\047', '\034', '\035', '\036', '\037',
  '\100', '\132', '\177', '\173', '\133', '\154', '\120', '\175',
  '\115', '\135', '\134', '\116', '\153', '\140', '\113', '\141',
  '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
  '\370', '\371', '\172', '\136', '\114', '\176', '\156', '\157',
  '\174', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
  '\310', '\311', '\321', '\322', '\323', '\324', '\325', '\326',
  '\327', '\330', '\331', '\342', '\343', '\344', '\345', '\346',
  '\347', '\350', '\351', '\255', '\340', '\275', '\137', '\155',
  '\171', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
  '\210', '\211', '\221', '\222', '\223', '\224', '\225', '\226',
  '\227', '\230', '\231', '\242', '\243', '\244', '\245', '\246',
  '\247', '\250', '\251', '\300', '\117', '\320', '\241', '\007',
  '\040', '\041', '\042', '\043', '\044', '\025', '\006', '\027',
  '\050', '\051', '\052', '\053', '\054', '\011', '\012', '\033',
  '\060', '\061', '\032', '\063', '\064', '\065', '\066', '\010',
  '\070', '\071', '\072', '\073', '\004', '\024', '\076', '\341',
  '\101', '\102', '\103', '\104', '\105', '\106', '\107', '\110',
  '\111', '\121', '\122', '\123', '\124', '\125', '\126', '\127',
  '\130', '\131', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\160', '\161', '\162', '\163', '\164', '\165',
  '\166', '\167', '\170', '\200', '\212', '\213', '\214', '\215',
  '\216', '\217', '\220', '\232', '\233', '\234', '\235', '\236',
  '\237', '\240', '\252', '\253', '\254', '\255', '\256', '\257',
  '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
  '\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
  '\312', '\313', '\314', '\315', '\316', '\317', '\332', '\333',
  '\334', '\335', '\336', '\337', '\352', '\353', '\354', '\355',
  '\356', '\357', '\372', '\373', '\374', '\375', '\376', '\377'
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ ebcdic_ಗಮ್ಯ_ascii[] =
{
  '\000', '\001', '\002', '\003', '\234', '\011', '\206', '\177',
  '\227', '\215', '\216', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\235', '\205', '\010', '\207',
  '\030', '\031', '\222', '\217', '\034', '\035', '\036', '\037',
  '\200', '\201', '\202', '\203', '\204', '\012', '\027', '\033',
  '\210', '\211', '\212', '\213', '\214', '\005', '\006', '\007',
  '\220', '\221', '\026', '\223', '\224', '\225', '\226', '\004',
  '\230', '\231', '\232', '\233', '\024', '\025', '\236', '\032',
  '\040', '\240', '\241', '\242', '\243', '\244', '\245', '\246',
  '\247', '\250', '\325', '\056', '\074', '\050', '\053', '\174',
  '\046', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
  '\260', '\261', '\041', '\044', '\052', '\051', '\073', '\176',
  '\055', '\057', '\262', '\263', '\264', '\265', '\266', '\267',
  '\270', '\271', '\313', '\054', '\045', '\137', '\076', '\077',
  '\272', '\273', '\274', '\275', '\276', '\277', '\300', '\301',
  '\302', '\140', '\072', '\043', '\100', '\047', '\075', '\042',
  '\303', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\304', '\305', '\306', '\307', '\310', '\311',
  '\312', '\152', '\153', '\154', '\155', '\156', '\157', '\160',
  '\161', '\162', '\136', '\314', '\315', '\316', '\317', '\320',
  '\321', '\345', '\163', '\164', '\165', '\166', '\167', '\170',
  '\171', '\172', '\322', '\323', '\324', '\133', '\326', '\327',
  '\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
  '\340', '\341', '\342', '\343', '\344', '\135', '\346', '\347',
  '\173', '\101', '\102', '\103', '\104', '\105', '\106', '\107',
  '\110', '\111', '\350', '\351', '\352', '\353', '\354', '\355',
  '\175', '\112', '\113', '\114', '\115', '\116', '\117', '\120',
  '\121', '\122', '\356', '\357', '\360', '\361', '\362', '\363',
  '\134', '\237', '\123', '\124', '\125', '\126', '\127', '\130',
  '\131', '\132', '\364', '\365', '\366', '\367', '\370', '\371',
  '\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
  '\070', '\071', '\372', '\373', '\374', '\375', '\376', '\377'
};

/* True if we need to close the standard output *stream*.  */
ಸ್ಥಿರ bool close_stdout_required = true;

/* The only reason to close the standard output *stream* is if
   parse_long_options fails (as it does for --help or --version).
   In any other case, dd uses only the STDOUT_FILENO file descriptor,
   and the "cleanup" function calls "close (STDOUT_FILENO)".
   Closing the file descriptor and then letting the usual atexit-run
   close_stdout function call "fclose (stdout)" would result in a
   harmless failure of the close syscall (with errno EBADF).
   This function serves solely to avoid the unnecessary close_stdout
   call, once parse_long_options has succeeded.
   Meanwhile, we guarantee that the standard error stream is flushed,
   by inlining the last half of close_stdout as needed.  */
ಸ್ಥಿರ ಶೂನ್ಯ
maybe_close_stdout (ಶೂನ್ಯ)
{
  ಆದರೆ (close_stdout_required)
    close_stdout ();
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (close_ಹರಿವು (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
ಸ್ಥಿರ ಶೂನ್ಯ
diagnose (ಪೂರ್ಣಾಂಕ errnum, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt, ...)
{
  ಆದರೆ (0 < ಪ್ರಗತಿ_len)
    {
      fputc ('\n', stderr);
      ಪ್ರಗತಿ_len = 0;
    }

  va_list ap;
  va_ಆರಂಭ (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_ಕೊನೆ (ap);
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [ಕಾರ್ಯಾಂಶ]...\n\
  or:  %s OPTION\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Copy a file, converting and formatting according ಗಮ್ಯ the operands.\n\
\n\
"), stdout);
      oputs (_("\
  bs=ಬೈಟುಗಳು_2        ಪಠ and ಲಿಖ ಮೇಲೆ ಗಮ್ಯ ಬೈಟುಗಳು_2 ಬೈಟುಗಳು at a ಸಮಯ (ಪೂರ್ವನಿಯೋಜಿತ: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=ಬೈಟುಗಳು_2       convert ಬೈಟುಗಳು_2 ಬೈಟುಗಳು at a ಸಮಯ\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as ಪ್ರತಿ the comma separated ಚಿಹ್ನೆ list\n\
"));
      oputs (_("\
  ಎಣಿಕೆ_2=N         copy only N ಒಳಹರಿವು ಖಂಡಗಳು\n\
"));
      oputs (_("\
  ibs=ಬೈಟುಗಳು_2       ಪಠ ಮೇಲೆ ಗಮ್ಯ ಬೈಟುಗಳು_2 ಬೈಟುಗಳು at a ಸಮಯ (ಪೂರ್ವನಿಯೋಜಿತ: 512)\n\
"));
      oputs (_("\
  ಆದರೆ=FILE         ಪಠ from FILE instead ದ standard ಒಳಹರಿವು\n\
"));
      oputs (_("\
  iflag=FLAGS     ಪಠ as ಪ್ರತಿ the comma separated ಚಿಹ್ನೆ list\n\
"));
      oputs (_("\
  obs=ಬೈಟುಗಳು_2       ಲಿಖ ಬೈಟುಗಳು_2 ಬೈಟುಗಳು at a ಸಮಯ (ಪೂರ್ವನಿಯೋಜಿತ: 512)\n\
"));
      oputs (_("\
  ದ=FILE         ಲಿಖ ಗಮ್ಯ FILE instead ದ standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     ಲಿಖ as ಪ್ರತಿ the comma separated ಚಿಹ್ನೆ list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output ಖಂಡಗಳು\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized ಒಳಹರಿವು ಖಂಡಗಳು\n\
"));
      oputs (_("\
  ಸ್ಥಿತಿ_2=LEVEL    The LEVEL ದ information ಗಮ್ಯ print ಗಮ್ಯ standard error;\n\
                  'none' suppresses everything but error messages,\n\
                  'noxfer' suppresses the final transfer statistics,\n\
                  'progress' shows periodic transfer statistics\n\
"));
      fputs (_("\
\n\
N and ಬೈಟುಗಳು_2 may be followed by the following multiplicative suffixes:\n\
c=1, w=2, b=512, kB=1000, K=1024, MB=1000*1000, M=1024*1024, xM=M,\n\
GB=1000*1000*1000, G=1024*1024*1024, and so on ಗಾಗಿ T, P, E, Z, Y, R, Q.\n\
ದ್ವಿಮಾನ_2 prefixes can be ಬಳಸಿದ, too: KiB=K, MiB=M, and so on.\n\
If N ತುದಿಗಳು ಒಳ 'B', it counts ಬೈಟುಗಳು not ಖಂಡಗಳು.\n\
\n\
Each CONV ಚಿಹ್ನೆ may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC ಗಮ್ಯ ASCII\n\
  ebcdic    from ASCII ಗಮ್ಯ EBCDIC\n\
  ibm       from ASCII ಗಮ್ಯ alternate EBCDIC\n\
  ಖಂಡ     pad newline-terminated ದಾಖಲೆಗಳು with spaces ಗಮ್ಯ cbs-ಗಾತ್ರ_2\n\
  unblock   replace trailing spaces ಒಳ cbs-ಗಾತ್ರ_2 ದಾಖಲೆಗಳು with newline\n\
  lcase     change ಊರ್ಧ್ವ ಸ್ಥಿತಿ ಗಮ್ಯ ಅಧೋ ಸ್ಥಿತಿ\n\
  ucase     change ಅಧೋ ಸ್ಥಿತಿ ಗಮ್ಯ ಊರ್ಧ್ವ ಸ್ಥಿತಿ\n\
  sparse    try ಗಮ್ಯ seek rather than ಲಿಖ all-NUL output ಖಂಡಗಳು\n\
  swab      ಅದಲುಬದಲು every pair ದ ಒಳಹರಿವು ಬೈಟುಗಳು\n\
  sync      pad every ಒಳಹರಿವು ಖಂಡ with NULs ಗಮ್ಯ ibs-ಗಾತ್ರ_2; when ಬಳಸಿದ\n\
            with ಖಂಡ or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail ಆದರೆ the output file already exists\n\
  nocreat   ಮಾಡು not create the output file\n\
  notrunc   ಮಾಡು not truncate the output file\n\
  noerror   ಮುಂದುವರಿ after ಪಠ errors\n\
  fdatasync  physically ಲಿಖ output file ದತ್ತಾಂಶ before finishing\n\
  fsync     likewise, but also ಲಿಖ metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG ಚಿಹ್ನೆ may be:\n\
\n\
  append    append ಬಹುಲಕ_2 (makes sense only ಗಾಗಿ output; conv=notrunc suggested)\n\
"), stdout);
      ಆದರೆ (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      ಆದರೆ (O_ನೇರ)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      ಆದರೆ (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      ಆದರೆ (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      ಆದರೆ (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      ಆದರೆ (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#ಆದರೆ HAVE_POSIX_FADVISE
      ಆದರೆ (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#ಆದರೆ_ಅಂತ್ಯ
      ಆದರೆ (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      ಆದರೆ (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      ಆದರೆ (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      ಆದರೆ (O_ದ್ವಿಮಾನ)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      ಆದರೆ (O_ಪಠ್ಯ)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        ಬರೆ (_("\
\n\
Sending a %s ಸಂಕೇತ ಗಮ್ಯ a running 'dd' process makes it\n\
print I/O statistics ಗಮ್ಯ standard error and then resume copying.\n\
\n\
Options are:\n\
\n\
"), SIGINFO == SIGUSR1 ? "USR1" : "INFO");
      }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Common options to use when displaying sizes and rates.  */

ಎಣಿಕೆ { human_opts = (human_autoscale | human_ಸುತ್ತು_ಗಮ್ಯ_ಹತ್ತಿರದ
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

ಸ್ಥಿರ ಶೂನ್ಯ
alloc_ibuf (ಶೂನ್ಯ)
{
  ಆದರೆ (ibuf)
    ಹಿಂತಿರುಗಿ;

  bool extra_byte_for_swab = !!(conversions_ಮುಖವಾಡ & C_SWAB);
  ibuf = alignalloc (ಪುಟ_ಗಾತ್ರ, ಒಳಹರಿವು_blocksize + extra_byte_for_swab);
  ಆದರೆ (!ibuf)
    {
      ಅಕ್ಷರ hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             ಒಳಹರಿವು_blocksize,
             human_readable (ಒಳಹರಿವು_blocksize, hbuf,
                             human_opts | human_ತಳ_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

ಸ್ಥಿರ ಶೂನ್ಯ
alloc_obuf (ಶೂನ್ಯ)
{
  ಆದರೆ (obuf)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (conversions_ಮುಖವಾಡ & C_TWOBUFS)
    {
      obuf = alignalloc (ಪುಟ_ಗಾತ್ರ, output_blocksize);
      ಆದರೆ (!obuf)
        {
          ಅಕ್ಷರ hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_ತಳ_1024, 1, 1));
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
translate_charset (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *new_trans)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 256; i++)
    trans_ಕೋಷ್ಟಕ[i] = new_trans[trans_ಕೋಷ್ಟಕ[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ_2 (ಪೂರ್ಣಾಂಕ i)
{
  ಹಿಂತಿರುಗಿ ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ (i);
}

ಸ್ಥಿರ bool
abbreviation_lacks_ಪೂರ್ವಪ್ರತ್ಯಯ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_2)
{
  ಹಿಂತಿರುಗಿ ಸಂದೇಶ_2[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂದೇಶ_2) - 2] == ' ';
}

/* Print transfer statistics.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_xfer_stats (xtime_t ಪ್ರಗತಿ_ಸಮಯ)
{
  xtime_t now = ಪ್ರಗತಿ_ಸಮಯ ? ಪ್ರಗತಿ_ಸಮಯ : gethrxtime ();
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ slash_s[] = "/s";
  ಅಕ್ಷರ hbuf[3][LONGEST_HUMAN_READABLE + ಗಾತ್ರ slash_s];
  ದ್ವಿಗುಣ delta_s;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬೈಟುಗಳು_ಪ್ರತಿ_ಎರಡನೇ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *si = human_readable (w_ಬೈಟುಗಳು, hbuf[0], human_opts, 1, 1);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *iec = human_readable (w_ಬೈಟುಗಳು, hbuf[1],
                                    human_opts | human_ತಳ_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  ಅಕ್ಷರ *bpsbuf = hbuf[2];
  ಪೂರ್ಣಾಂಕ bpsbufsize = ಗಾತ್ರ hbuf[2];
  ಆದರೆ (ಆರಂಭ_ಸಮಯ < now)
    {
      ದ್ವಿಗುಣ XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - ಆರಂಭ_ಸಮಯ;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      ಬೈಟುಗಳು_ಪ್ರತಿ_ಎರಡನೇ = human_readable (w_ಬೈಟುಗಳು, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (ಬೈಟುಗಳು_ಪ್ರತಿ_ಎರಡನೇ - bpsbuf + bpsbuf, slash_s);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      ಬೈಟುಗಳು_ಪ್ರತಿ_ಎರಡನೇ = bpsbuf;
    }

  ಆದರೆ (ಪ್ರಗತಿ_ಸಮಯ)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  ಅಕ್ಷರ delta_s_buf[24];
  snprintf (delta_s_buf, ಗಾತ್ರ delta_s_buf,
            ಪ್ರಗತಿ_ಸಮಯ ? "%.0f s" : "%g s", delta_s);

  ಪೂರ್ಣಾಂಕ stats_len
    = (abbreviation_lacks_ಪೂರ್ವಪ್ರತ್ಯಯ (si)
       ? fprintf (stderr,
                  ngettext ("%jd byte copied, %s, %s",
                            "%jd bytes copied, %s, %s",
                            select_plural (w_ಬೈಟುಗಳು)),
                  w_ಬೈಟುಗಳು, delta_s_buf, ಬೈಟುಗಳು_ಪ್ರತಿ_ಎರಡನೇ)
       : abbreviation_lacks_ಪೂರ್ವಪ್ರತ್ಯಯ (iec)
       ? fprintf (stderr,
                  _("%jd bytes (%s) copied, %s, %s"),
                  w_ಬೈಟುಗಳು, si, delta_s_buf, ಬೈಟುಗಳು_ಪ್ರತಿ_ಎರಡನೇ)
       : fprintf (stderr,
                  _("%jd bytes (%s, %s) copied, %s, %s"),
                  w_ಬೈಟುಗಳು, si, iec, delta_s_buf, ಬೈಟುಗಳು_ಪ್ರತಿ_ಎರಡನೇ));

  ಆದರೆ (ಪ್ರಗತಿ_ಸಮಯ)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      ಆದರೆ (0 <= stats_len && stats_len < ಪ್ರಗತಿ_len)
        fprintf (stderr, "%*s", ಪ್ರಗತಿ_len - stats_len, "");
      ಪ್ರಗತಿ_len = stats_len;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    fputc ('\n', stderr);

  reported_w_ಬೈಟುಗಳು = w_ಬೈಟುಗಳು;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_stats (ಶೂನ್ಯ)
{
  ಆದರೆ (ಸ್ಥಿತಿ_level == ಸ್ಥಿತಿ_NONE)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (0 < ಪ್ರಗತಿ_len)
    {
      fputc ('\n', stderr);
      ಪ್ರಗತಿ_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_ತುಂಬಿದ, r_partial, w_ತುಂಬಿದ, w_partial);

  ಆದರೆ (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  ಆದರೆ (ಸ್ಥಿತಿ_level == ಸ್ಥಿತಿ_NOXFER)
    ಹಿಂತಿರುಗಿ;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಅಡಚಣೆ_handler (ಪೂರ್ಣಾಂಕ sig)
{
  ಆದರೆ (! SA_RESETHAND)
    ಸಂಕೇತ (sig, SIG_DFL);
  ಅಡಚಣೆ_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

ಸ್ಥಿರ ಶೂನ್ಯ
siginfo_handler (MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ sig)
{
  info_signal_ಎಣಿಕೆ++;
}

/* Install the signal handlers.  */

ಸ್ಥಿರ ಶೂನ್ಯ
install_signal_handlers (ಶೂನ್ಯ)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  ರಚನೆ sigaction act;
  sigemptyset (&caught_ಸಂಕೇತಗಳು);
  ಆದರೆ (catch_siginfo)
    sigaddset (&caught_ಸಂಕೇತಗಳು, SIGINFO);
  sigaction (SIGINT, ಖಾಲಿ, &act);
  ಆದರೆ (act.sa_handler != SIG_IGN)
    sigaddset (&caught_ಸಂಕೇತಗಳು, SIGINT);
  act.sa_mask = caught_ಸಂಕೇತಗಳು;

  ಆದರೆ (sigismember (&caught_ಸಂಕೇತಗಳು, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, ಖಾಲಿ);
    }

  ಆದರೆ (sigismember (&caught_ಸಂಕೇತಗಳು, SIGINT))
    {
      act.sa_handler = ಅಡಚಣೆ_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, ಖಾಲಿ);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
iclose (ಪೂರ್ಣಾಂಕ fd)
{
  ಆದರೆ (close (fd) != 0)
    ಮಾಡು
      ಆದರೆ (errno != EINTR)
        ಹಿಂತಿರುಗಿ -1;
    ಆಗ (close (fd) != 0 && errno != EBADF);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ synchronize_output (ಶೂನ್ಯ);

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup (ಶೂನ್ಯ)
{
  ಆದರೆ (!ಅಡಚಣೆ_signal)
    {
      ಪೂರ್ಣಾಂಕ sync_ಸ್ಥಿತಿ = synchronize_output ();
      ಆದರೆ (sync_ಸ್ಥಿತಿ)
        ನಿರ್ಗಮಿಸು (sync_ಸ್ಥಿತಿ);
    }

  ಆದರೆ (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (ಒಳಹರಿವು_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  ಆದರೆ (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

ಸ್ಥಿರ ಶೂನ್ಯ
process_ಸಂಕೇತಗಳು (ಶೂನ್ಯ)
{
  ಆಗ (ಅಡಚಣೆ_signal || info_signal_ಎಣಿಕೆ)
    {
      ಪೂರ್ಣಾಂಕ ಅಡಚಣೆ;
      ಪೂರ್ಣಾಂಕ infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_ಸಂಕೇತಗಳು, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      ಅಡಚಣೆ = ಅಡಚಣೆ_signal;
      infos = info_signal_ಎಣಿಕೆ;

      ಆದರೆ (infos)
        info_signal_ಎಣಿಕೆ = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, ಖಾಲಿ);

      ಆದರೆ (ಅಡಚಣೆ)
        cleanup ();
      print_stats ();
      ಆದರೆ (ಅಡಚಣೆ)
        ಎತ್ತು (ಅಡಚಣೆ);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಮುಕ್ತಾಯ_ಮೇಲೆ (ಶೂನ್ಯ)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_ಸಂಕೇತಗಳು ();
  cleanup ();
  print_stats ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
quit (ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2)
{
  ಮುಕ್ತಾಯ_ಮೇಲೆ ();
  ನಿರ್ಗಮಿಸು (ಸಂಕೇತ_2);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

ಸ್ಥಿರ off_t
cache_ಸುತ್ತು (ಪೂರ್ಣಾಂಕ fd, off_t len)
{
  ಸ್ಥಿರ off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  ಆದರೆ (len)
    {
      intmax_t c_pending;
      ಆದರೆ (ckd_ಕೂಡಿಸು (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      ಆದರೆ (c_pending > *pending)
        len = c_pending - *pending;
      ಇಲ್ಲದಿದ್ದರೆ
        len = 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    len = *pending;

  ಹಿಂತಿರುಗಿ len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

ಸ್ಥಿರ bool
invalidate_cache (ಪೂರ್ಣಾಂಕ fd, off_t len)
{
  ಪೂರ್ಣಾಂಕ adv_ret = -1;
  off_t ಸ್ಥಾನಾಂತರ;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_ಸುತ್ತು (fd, len);
  ಆದರೆ (len && !clen)
    ಹಿಂತಿರುಗಿ true; /* Don't advise this time.  */
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! len && ! clen && ! nocache_eof)
    ಹಿಂತಿರುಗಿ true;
  off_t pending = len ? cache_ಸುತ್ತು (fd, 0) : 0;

  ಆದರೆ (fd == STDIN_FILENO)
    {
      ಆದರೆ (ಒಳಹರಿವು_seekable)
        ಸ್ಥಾನಾಂತರ = ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಸ್ಥಾನಾಂತರ = -1;
          errno = ESPIPE;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರ off_t output_ಸ್ಥಾನಾಂತರ = -2;

      ಆದರೆ (output_ಸ್ಥಾನಾಂತರ != -1)
        {
          ಆದರೆ (output_ಸ್ಥಾನಾಂತರ < 0)
            output_ಸ್ಥಾನಾಂತರ = lseek (fd, 0, SEEK_CUR);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len)
            output_ಸ್ಥಾನಾಂತರ += clen + pending;
        }

      ಸ್ಥಾನಾಂತರ = output_ಸ್ಥಾನಾಂತರ;
    }

  ಆದರೆ (0 <= ಸ್ಥಾನಾಂತರ)
   {
     ಆದರೆ (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#ಆದರೆ HAVE_POSIX_FADVISE
     ಸ್ಥಾನಾಂತರ = ಸ್ಥಾನಾಂತರ - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     ಆದರೆ (clen == 0)
       ಸ್ಥಾನಾಂತರ -= ಸ್ಥಾನಾಂತರ % ಪುಟ_ಗಾತ್ರ;
     adv_ret = posix_fadvise (fd, ಸ್ಥಾನಾಂತರ, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#ಇಲ್ಲದಿದ್ದರೆ
     errno = ENOTSUP;
#ಆದರೆ_ಅಂತ್ಯ
   }

  ಹಿಂತಿರುಗಿ adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

ಸ್ಥಿರ ssize_t
iread (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ *buf, idx_t ಗಾತ್ರ_2)
{
  ssize_t nread;
  ಸ್ಥಿರ ssize_t prev_nread;

  ಮಾಡು
    {
      process_ಸಂಕೇತಗಳು ();
      nread = ಪಠ (fd, buf, ಗಾತ್ರ_2);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      ಆದರೆ (nread == -1 && errno == EINVAL
          && 0 < prev_nread && prev_nread < ಗಾತ್ರ_2
          && (ಒಳಹರಿವು_flags & O_ನೇರ))
        {
          errno = 0;
          nread = 0;
        }
    }
  ಆಗ (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  ಆದರೆ (0 < nread && nread < ಗಾತ್ರ_2)
    process_ಸಂಕೇತಗಳು ();

  ಆದರೆ (0 < nread && warn_partial_read)
    {
      ಆದರೆ (0 < prev_nread && prev_nread < ಗಾತ್ರ_2)
        {
          idx_t prev = prev_nread;
          ಆದರೆ (ಸ್ಥಿತಿ_level != ಸ್ಥಿತಿ_NONE)
            diagnose (0, ngettext (("warning: partial read (%td byte); "
                                    "suggest iflag=fullblock"),
                                   ("warning: partial read (%td bytes); "
                                    "suggest iflag=fullblock"),
                                   select_plural (prev)),
                      prev);
          warn_partial_read = false;
        }
    }

  prev_nread = nread;
  ಹಿಂತಿರುಗಿ nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
ಸ್ಥಿರ ssize_t
iread_fullblock (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ *buf, idx_t ಗಾತ್ರ_2)
{
  ssize_t nread = 0;

  ಆಗ (0 < ಗಾತ್ರ_2)
    {
      ssize_t ncurr = iread (fd, buf, ಗಾತ್ರ_2);
      ಆದರೆ (ncurr < 0)
        ಹಿಂತಿರುಗಿ ncurr;
      ಆದರೆ (ncurr == 0)
        ನಿಲ್ಲಿಸು;
      nread += ncurr;
      buf   += ncurr;
      ಗಾತ್ರ_2  -= ncurr;
    }

  ಹಿಂತಿರುಗಿ nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

ಸ್ಥಿರ idx_t
iwrite (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, idx_t ಗಾತ್ರ_2)
{
  idx_t ಒಟ್ಟು_written = 0;

  ಆದರೆ ((output_flags & O_ನೇರ) && ಗಾತ್ರ_2 < output_blocksize)
    {
      ಪೂರ್ಣಾಂಕ old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      ಆದರೆ (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_ನೇರ) != 0
          && ಸ್ಥಿತಿ_level != ಸ್ಥಿತಿ_NONE)
        diagnose (errno, _("failed to turn off O_DIRECT: %s"),
                  quotef (output_file));

      /* Since we have just turned off O_DIRECT for the final write,
         we try to preserve some of its semantics.  */

      /* Call invalidate_cache to setup the appropriate offsets
         for subsequent calls.  */
      o_nocache_eof = true;
      invalidate_cache (STDOUT_FILENO, 0);

      /* Attempt to ensure that the final block is committed
         to stable storage as quickly as possible.  */
      conversions_ಮುಖವಾಡ |= C_FSYNC;

      /* After the subsequent fsync we'll call invalidate_cache
         to attempt to clear all data from the page cache.  */
    }

  ಆಗ (ಒಟ್ಟು_written < ಗಾತ್ರ_2)
    {
      ssize_t nwritten = 0;
      process_ಸಂಕೇತಗಳು ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      ಆದರೆ ((conversions_ಮುಖವಾಡ & C_SPARSE) && is_nul (buf, ಗಾತ್ರ_2))
        {
          ಆದರೆ (lseek (fd, ಗಾತ್ರ_2, SEEK_CUR) < 0)
            {
              conversions_ಮುಖವಾಡ &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              final_op_was_seek = true;
              nwritten = ಗಾತ್ರ_2;
            }
        }

      ಆದರೆ (!nwritten)
        nwritten = ಲಿಖ (fd, buf + ಒಟ್ಟು_written, ಗಾತ್ರ_2 - ಒಟ್ಟು_written);

      ಆದರೆ (nwritten < 0)
        {
          ಆದರೆ (errno != EINTR)
            ನಿಲ್ಲಿಸು;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          ನಿಲ್ಲಿಸು;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಒಟ್ಟು_written += nwritten;
    }

  ಆದರೆ (o_nocache && ಒಟ್ಟು_written)
    invalidate_cache (fd, ಒಟ್ಟು_written);

  ಹಿಂತಿರುಗಿ ಒಟ್ಟು_written;
}

/* Write, then empty, the output buffer 'obuf'. */

ಸ್ಥಿರ ಶೂನ್ಯ
write_output (ಶೂನ್ಯ)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_ಬೈಟುಗಳು += nwritten;
  ಆದರೆ (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      ಆದರೆ (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    w_ತುಂಬಿದ++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ifdatasync (ಪೂರ್ಣಾಂಕ fd)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಮಾಡು
    {
      process_ಸಂಕೇತಗಳು ();
      ret = fdatasync (fd);
    }
  ಆಗ (ret < 0 && errno == EINTR);

  ಹಿಂತಿರುಗಿ ret;
}

/* Restart on EINTR from fd_reopen.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ifd_reopen (ಪೂರ್ಣಾಂಕ desired_fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ಪೂರ್ಣಾಂಕ flag, mode_t ಬಹುಲಕ_2)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಮಾಡು
    {
      process_ಸಂಕೇತಗಳು ();
      ret = fd_reopen (desired_fd, file, flag, ಬಹುಲಕ_2);
    }
  ಆಗ (ret < 0 && errno == EINTR);

  ಹಿಂತಿರುಗಿ ret;
}

/* Restart on EINTR from fstat.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ifstat (ಪೂರ್ಣಾಂಕ fd, ರಚನೆ stat *st)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಮಾಡು
    {
      process_ಸಂಕೇತಗಳು ();
      ret = fstat (fd, st);
    }
  ಆಗ (ret < 0 && errno == EINTR);

  ಹಿಂತಿರುಗಿ ret;
}

/* Restart on EINTR from fsync.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ifsync (ಪೂರ್ಣಾಂಕ fd)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಮಾಡು
    {
      process_ಸಂಕೇತಗಳು ();
      ret = fsync (fd);
    }
  ಆಗ (ret < 0 && errno == EINTR);

  ಹಿಂತಿರುಗಿ ret;
}

/* Restart on EINTR from ftruncate.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
iftruncate (ಪೂರ್ಣಾಂಕ fd, off_t ಉದ್ದಳತೆ)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಮಾಡು
    {
      process_ಸಂಕೇತಗಳು ();
      ret = ftruncate (fd, ಉದ್ದಳತೆ);
    }
  ಆಗ (ret < 0 && errno == EINTR);

  ಹಿಂತಿರುಗಿ ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
ಕಾರ್ಯಾಂಶ_matches (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ನಮೂನೆ_2, ಅಕ್ಷರ delim)
{
  ಆಗ (*ನಮೂನೆ_2)
    ಆದರೆ (*str++ != *ನಮೂನೆ_2++)
      ಹಿಂತಿರುಗಿ false;
  ಹಿಂತಿರುಗಿ !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
parse_symbols (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, ರಚನೆ ಚಿಹ್ನೆ_ಮೌಲ್ಯ ಸ್ಥಿರಾಂಕ *ಕೋಷ್ಟಕ,
               bool exclusive, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *error_msgid)
{
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = 0;

  ಆಗ (true)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *strcomma = strchr (str, ',');
      ರಚನೆ ಚಿಹ್ನೆ_ಮೌಲ್ಯ ಸ್ಥಿರಾಂಕ *ನಮೂದು;

      ಗಾಗಿ (ನಮೂದು = ಕೋಷ್ಟಕ;
           ! (ಕಾರ್ಯಾಂಶ_matches (str, ನಮೂದು->ಚಿಹ್ನೆ, ',') && ನಮೂದು->ಮೌಲ್ಯ);
           ನಮೂದು++)
        {
          ಆದರೆ (! ನಮೂದು->ಚಿಹ್ನೆ[0])
            {
              idx_t slen = strcomma ? strcomma - str : ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      ಆದರೆ (exclusive)
        ಮೌಲ್ಯ = ನಮೂದು->ಮೌಲ್ಯ;
      ಇಲ್ಲದಿದ್ದರೆ
        ಮೌಲ್ಯ |= ನಮೂದು->ಮೌಲ್ಯ;
      ಆದರೆ (!strcomma)
        ನಿಲ್ಲಿಸು;
      str = strcomma + 1;
    }

  ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

ಸ್ಥಿರ intmax_t
parse_integer (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  ಪೂರ್ಣಾಂಕ indeterminate = 0;
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t ಫಲಿತಾಂಶ = 1;
  bool ಅತಿಪ್ರವಾಹ = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  ಆಗ (true)
    {
      uintmax_t n = indeterminate;
      ಅಕ್ಷರ *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      ಆದರೆ ((e & ~LONGINT_ಅತಿಪ್ರವಾಹ) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          ಆದರೆ (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_ಅತಿಪ್ರವಾಹ) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      ಆದರೆ (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      ಆದರೆ ((e & ~LONGINT_ಅತಿಪ್ರವಾಹ) != LONGINT_OK)
        {
          ಆದರೆ (! (e & LONGINT_ಅತಿಪ್ರವಾಹ) && n <= INTMAX_MAX)
            {
              *invalid = e;
              ಹಿಂತಿರುಗಿ indeterminate;
            }
          e = LONGINT_ಅತಿಪ್ರವಾಹ;
        }

      ಆದರೆ (n == 0)
        ಫಲಿತಾಂಶ = 0;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಫಲಿತಾಂಶ != 0)
        {
          intmax_t ಗುಣಲಬ್ಧ;
          ಆದರೆ ((e & LONGINT_ಅತಿಪ್ರವಾಹ)
              || ಅತಿಪ್ರವಾಹ
              || ckd_mul (&ಗುಣಲಬ್ಧ, ಫಲಿತಾಂಶ, n))
            ಅತಿಪ್ರವಾಹ = true;
          ಇಲ್ಲದಿದ್ದರೆ
            ಫಲಿತಾಂಶ = ಗುಣಲಬ್ಧ;
        }

      ಆದರೆ (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      ಆದರೆ (! multiply)
        ನಿಲ್ಲಿಸು;
      str = suffix + 1;
    }

  ಆದರೆ (ಫಲಿತಾಂಶ == 0)
    {
      ಆದರೆ (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅತಿಪ್ರವಾಹ)
    {
      e = LONGINT_ಅತಿಪ್ರವಾಹ;
      ಫಲಿತಾಂಶ = INTMAX_MAX;
    }

  *invalid = e;
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
ಕಾರ್ಯಾಂಶ_is (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಾಂಶ_2, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ಹಿಂತಿರುಗಿ ಕಾರ್ಯಾಂಶ_matches (ಕಾರ್ಯಾಂಶ_2, name, '=');
}

ಸ್ಥಿರ ಶೂನ್ಯ
scanargs (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv)
{
  idx_t blocksize = 0;
  intmax_t ಎಣಿಕೆ_2 = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool ಎಣಿಕೆ_B = false, skip_B = false, seek_B = false;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = optind; i < argc; i++)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name = argv[i];
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *val = strchr (name, '=');

      ಆದರೆ (val == ಖಾಲಿ)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "if"))
        ಒಳಹರಿವು_file = val;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "of"))
        output_file = val;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "conv"))
        conversions_ಮುಖವಾಡ |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "iflag"))
        ಒಳಹರಿವು_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "status"))
        ಸ್ಥಿತಿ_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      ಇಲ್ಲದಿದ್ದರೆ
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = ಖಾಲಿ;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &ಒಳಹರಿವು_blocksize;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "skip") || ಕಾರ್ಯಾಂಶ_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕಾರ್ಯಾಂಶ_is (name, "count"))
            {
              ಎಣಿಕೆ_2 = n;
              ಎಣಿಕೆ_B = has_B;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          ಆದರೆ (n < n_min)
            invalid = LONGINT_INVALID;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (n_max < n)
            invalid = LONGINT_ಅತಿಪ್ರವಾಹ;

          ಆದರೆ (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_ಅತಿಪ್ರವಾಹ ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (converted_idx)
            *converted_idx = n;
        }
    }

  ಆದರೆ (blocksize)
    ಒಳಹರಿವು_blocksize = output_blocksize = blocksize;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_ಮುಖವಾಡ |= C_TWOBUFS;
    }

  ಆದರೆ (ಒಳಹರಿವು_blocksize == 0)
    ಒಳಹರಿವು_blocksize = DEFAULT_BLOCKSIZE;
  ಆದರೆ (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  ಆದರೆ (conversion_blocksize == 0)
    conversions_ಮುಖವಾಡ &= ~(C_ಖಂಡ | C_UNBLOCK);

  ಆದರೆ (ಒಳಹರಿವು_flags & (O_DSYNC | O_SYNC))
    ಒಳಹರಿವು_flags |= O_RSYNC;

  ಆದರೆ (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (skip_B)
    ಒಳಹರಿವು_flags |= O_SKIP_ಬೈಟುಗಳು;
  ಆದರೆ (ಒಳಹರಿವು_flags & O_SKIP_ಬೈಟುಗಳು && skip != 0)
    {
      skip_ದಾಖಲೆಗಳು = skip / ಒಳಹರಿವು_blocksize;
      skip_ಬೈಟುಗಳು = skip % ಒಳಹರಿವು_blocksize;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip != 0)
    skip_ದಾಖಲೆಗಳು = skip;

  ಆದರೆ (ಎಣಿಕೆ_B)
    ಒಳಹರಿವು_flags |= O_ಎಣಿಕೆ_ಬೈಟುಗಳು;
  ಆದರೆ (ಒಳಹರಿವು_flags & O_ಎಣಿಕೆ_ಬೈಟುಗಳು && ಎಣಿಕೆ_2 != INTMAX_MAX)
    {
      max_ದಾಖಲೆಗಳು = ಎಣಿಕೆ_2 / ಒಳಹರಿವು_blocksize;
      max_ಬೈಟುಗಳು = ಎಣಿಕೆ_2 % ಒಳಹರಿವು_blocksize;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಎಣಿಕೆ_2 != INTMAX_MAX)
    max_ದಾಖಲೆಗಳು = ಎಣಿಕೆ_2;

  ಆದರೆ (seek_B)
    output_flags |= O_SEEK_ಬೈಟುಗಳು;
  ಆದರೆ (output_flags & O_SEEK_ಬೈಟುಗಳು && seek != 0)
    {
      seek_ದಾಖಲೆಗಳು = seek / output_blocksize;
      seek_ಬೈಟುಗಳು = seek % output_blocksize;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (seek != 0)
    seek_ದಾಖಲೆಗಳು = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_ಮುಖವಾಡ & C_TWOBUFS) && ! (ಒಳಹರಿವು_flags & O_FULLBLOCK)
     && (skip_ದಾಖಲೆಗಳು
         || (0 < max_ದಾಖಲೆಗಳು && max_ದಾಖಲೆಗಳು < INTMAX_MAX)
         || (ಒಳಹರಿವು_flags | output_flags) & O_ನೇರ));

  iread_fnc = ((ಒಳಹರಿವು_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  ಒಳಹರಿವು_flags &= ~O_FULLBLOCK;

  ಆದರೆ (ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ_2 (conversions_ಮುಖವಾಡ & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  ಆದರೆ (ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ_2 (conversions_ಮುಖವಾಡ & (C_ಖಂಡ | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  ಆದರೆ (ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ_2 (conversions_ಮುಖವಾಡ & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  ಆದರೆ (ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ_2 (conversions_ಮುಖವಾಡ & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  ಆದರೆ (ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ_2 (ಒಳಹರಿವು_flags & (O_ನೇರ | O_NOCACHE))
      || ಅಪವರ್ತ್ಯ_ಬಿಟ್ಟುಗಳು_ಗಣ_2 (output_flags & (O_ನೇರ | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  ಆದರೆ (ಒಳಹರಿವು_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_ದಾಖಲೆಗಳು == 0 && max_ಬೈಟುಗಳು == 0);
      ಒಳಹರಿವು_flags &= ~O_NOCACHE;
    }
  ಆದರೆ (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_ದಾಖಲೆಗಳು == 0 && max_ಬೈಟುಗಳು == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

ಸ್ಥಿರ ಶೂನ್ಯ
apply_translations (ಶೂನ್ಯ)
{
  ಆದರೆ (conversions_ಮುಖವಾಡ & C_ASCII)
    translate_charset (ebcdic_ಗಮ್ಯ_ascii);

  ಆದರೆ (conversions_ಮುಖವಾಡ & C_UCASE)
    {
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 256; i++)
        trans_ಕೋಷ್ಟಕ[i] = ದೊಡ್ಡ_ಅಕ್ಷರ (trans_ಕೋಷ್ಟಕ[i]);
      translation_needed = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (conversions_ಮುಖವಾಡ & C_LCASE)
    {
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 256; i++)
        trans_ಕೋಷ್ಟಕ[i] = tolower (trans_ಕೋಷ್ಟಕ[i]);
      translation_needed = true;
    }

  ಆದರೆ (conversions_ಮುಖವಾಡ & C_EBCDIC)
    {
      translate_charset (ascii_ಗಮ್ಯ_ebcdic);
      newline_character = ascii_ಗಮ್ಯ_ebcdic['\n'];
      space_character = ascii_ಗಮ್ಯ_ebcdic[' '];
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (conversions_ಮುಖವಾಡ & C_IBM)
    {
      translate_charset (ascii_ಗಮ್ಯ_ibm);
      newline_character = ascii_ಗಮ್ಯ_ibm['\n'];
      space_character = ascii_ಗಮ್ಯ_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

ಸ್ಥಿರ ಶೂನ್ಯ
translate_ಬಫರ್ (ಅಕ್ಷರ *buf, idx_t nread)
{
  ಅಕ್ಷರ *cp = buf;
  ಗಾಗಿ (idx_t i = nread; i; i--, cp++)
    *cp = trans_ಕೋಷ್ಟಕ[ಗಮ್ಯ_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
swab_ಬಫರ್ (ಅಕ್ಷರ *buf, idx_t *nread, ಪೂರ್ಣಾಂಕ *saved_byte)
{
  ಆದರೆ (*nread == 0)
    ಹಿಂತಿರುಗಿ buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  ಪೂರ್ಣಾಂಕ prev_saved = *saved_byte;
  ಆದರೆ ((prev_saved < 0) == (*nread & 1))
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = buf[--*nread];
      *saved_byte = c;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  ಗಾಗಿ (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  ಆದರೆ (prev_saved < 0)
    ಹಿಂತಿರುಗಿ buf + 1;

  buf[1] = prev_saved;
  ++*nread;
  ಹಿಂತಿರುಗಿ buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

ಸ್ಥಿರ ಶೂನ್ಯ
advance_ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ (intmax_t ಸ್ಥಾನಾಂತರ)
{
  ಆದರೆ (0 <= ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ && ckd_ಕೂಡಿಸು (&ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ, ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ, ಸ್ಥಾನಾಂತರ))
    ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

ಸ್ಥಿರ intmax_t
skip (ಪೂರ್ಣಾಂಕ fdesc, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, intmax_t ದಾಖಲೆಗಳು, idx_t blocksize,
      idx_t *ಬೈಟುಗಳು)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t ಸ್ಥಾನಾಂತರ;
  ಆದರೆ (! ckd_mul (&ಸ್ಥಾನಾಂತರ, ದಾಖಲೆಗಳು, blocksize)
      && ! ckd_ಕೂಡಿಸು (&ಸ್ಥಾನಾಂತರ, ಸ್ಥಾನಾಂತರ, *ಬೈಟುಗಳು)
      && 0 <= lseek (fdesc, ಸ್ಥಾನಾಂತರ, SEEK_CUR))
    {
      ಆದರೆ (fdesc == STDIN_FILENO)
        {
           ರಚನೆ stat st;
           ಆದರೆ (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           ಆದರೆ (usable_st_ಗಾತ್ರ (&st) && 0 < st.st_ಗಾತ್ರ && 0 <= ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ
               && st.st_ಗಾತ್ರ - ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ < ಸ್ಥಾನಾಂತರ)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               ದಾಖಲೆಗಳು = ( ಸ್ಥಾನಾಂತರ - st.st_ಗಾತ್ರ ) / blocksize;
               ಸ್ಥಾನಾಂತರ = st.st_ಗಾತ್ರ - ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ;
             }
           ಇಲ್ಲದಿದ್ದರೆ
             ದಾಖಲೆಗಳು = 0;
           advance_ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ (ಸ್ಥಾನಾಂತರ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ದಾಖಲೆಗಳು = 0;
          *ಬೈಟುಗಳು = 0;
        }
      ಹಿಂತಿರುಗಿ ದಾಖಲೆಗಳು;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಪೂರ್ಣಾಂಕ lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      ಆದರೆ (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          ಆದರೆ (!lseek_errno)
            {
              /* The original seek was not attempted as offset > OFF_T_MAX.
                 We should error for write as can't get to the desired
                 location, even if OFF_T_MAX < max file size.
                 For read we're not going to read any data anyway,
                 so we should error for consistency.
                 It would be nice to not error for /dev/{zero,null}
                 for any offset, but that's not a significant issue.  */
              lseek_errno = EOVERFLOW;
            }

          diagnose (lseek_errno,
                    gettext (fdesc == STDIN_FILENO
                             ? N_("%s: cannot skip")
                             : N_("%s: cannot seek")),
                    quotef (file));
          /* If the file has a specific size and we've asked
             to skip/seek beyond the max allowable, then quit.  */
          quit (EXIT_FAILURE);
        }
      /* else file_size && offset > OFF_T_MAX or file ! seekable */

      ಅಕ್ಷರ *buf;
      ಆದರೆ (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          alloc_obuf ();
          buf = obuf;
        }

      ಮಾಡು
        {
          ssize_t nread = iread_fnc (fdesc, buf, ದಾಖಲೆಗಳು ? blocksize : *ಬೈಟುಗಳು);
          ಆದರೆ (nread < 0)
            {
              ಆದರೆ (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  ಆದರೆ (conversions_ಮುಖವಾಡ & C_NOERROR)
                    print_stats ();
                }
              ಇಲ್ಲದಿದ್ದರೆ
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nread == 0)
            ನಿಲ್ಲಿಸು;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fdesc == STDIN_FILENO)
            advance_ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ (nread);

          ಆದರೆ (ದಾಖಲೆಗಳು != 0)
            ದಾಖಲೆಗಳು--;
          ಇಲ್ಲದಿದ್ದರೆ
            *ಬೈಟುಗಳು = 0;
        }
      ಆಗ (ದಾಖಲೆಗಳು || *ಬೈಟುಗಳು);

      ಹಿಂತಿರುಗಿ ದಾಖಲೆಗಳು;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

ಸ್ಥಿರ bool
advance_ಒಳಹರಿವು_after_read_error (idx_t nbytes)
{
  ಆದರೆ (! ಒಳಹರಿವು_seekable)
    {
      ಆದರೆ (ಒಳಹರಿವು_seek_errno == ESPIPE)
        ಹಿಂತಿರುಗಿ true;
      errno = ಒಳಹರಿವು_seek_errno;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      off_t ಸ್ಥಾನಾಂತರ;
      advance_ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ (nbytes);
      ಆದರೆ (ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (ಒಳಹರಿವು_file));
          ಹಿಂತಿರುಗಿ false;
        }
      ಸ್ಥಾನಾಂತರ = lseek (STDIN_FILENO, 0, SEEK_CUR);
      ಆದರೆ (0 <= ಸ್ಥಾನಾಂತರ)
        {
          off_t diff;
          ಆದರೆ (ಸ್ಥಾನಾಂತರ == ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ)
            ಹಿಂತಿರುಗಿ true;
          diff = ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ - ಸ್ಥಾನಾಂತರ;
          ಆದರೆ (! (0 <= diff && diff <= nbytes) && ಸ್ಥಿತಿ_level != ಸ್ಥಿತಿ_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          ಆದರೆ (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            ಹಿಂತಿರುಗಿ true;
          ಆದರೆ (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (ಒಳಹರಿವು_file));
  ಹಿಂತಿರುಗಿ false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

ಸ್ಥಿರ ಶೂನ್ಯ
copy_simple (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, idx_t nread)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಆರಂಭ = buf;	/* First uncopied char in BUF.  */

  ಮಾಡು
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      ಸ್ಮರಣೆ_ನಕಲು (obuf + oc, ಆರಂಭ, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      ಆರಂಭ += nfree;
      oc += nfree;
      ಆದರೆ (oc >= output_blocksize)
        write_output ();
    }
  ಆಗ (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

ಸ್ಥಿರ ಶೂನ್ಯ
copy_with_ಖಂಡ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, idx_t nread)
{
  ಗಾಗಿ (idx_t i = nread; i; i--, buf++)
    {
      ಆದರೆ (*buf == newline_character)
        {
          ಆದರೆ (ಲಂಬಸಾಲು < conversion_blocksize)
            {
              ಗಾಗಿ (idx_t j = ಲಂಬಸಾಲು; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          ಲಂಬಸಾಲು = 0;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (ಲಂಬಸಾಲು == conversion_blocksize)
            r_truncate++;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಲಂಬಸಾಲು < conversion_blocksize)
            output_char (*buf);
          ಲಂಬಸಾಲು++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

ಸ್ಥಿರ ಶೂನ್ಯ
copy_with_unblock (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *buf, idx_t nread)
{
  ಸ್ಥಿರ idx_t pending_spaces = 0;

  ಗಾಗಿ (idx_t i = 0; i < nread; i++)
    {
      ಅಕ್ಷರ c = buf[i];

      ಆದರೆ (ಲಂಬಸಾಲು++ >= conversion_blocksize)
        {
          ಲಂಬಸಾಲು = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == space_character)
        pending_spaces++;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          ಆಗ (pending_spaces)
            {
              output_char (space_character);
              --pending_spaces;
            }
          output_char (c);
        }
    }
}

/* Set the file descriptor flags for FD that correspond to the nonzero bits
   in ADD_FLAGS.  The file's name is NAME.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_fd_flags (ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_flags, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  ಕೂಡಿಸು_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  ಆದರೆ (ಕೂಡಿಸು_flags)
    {
      ಪೂರ್ಣಾಂಕ old_flags = fcntl (fd, F_GETFL);
      ಪೂರ್ಣಾಂಕ new_flags = old_flags | ಕೂಡಿಸು_flags;
      bool ok = true;
      ಆದರೆ (old_flags < 0)
        ok = false;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (old_flags != new_flags)
        {
          ಆದರೆ (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              ರಚನೆ stat st;
              ಆದರೆ (ifstat (fd, &st) != 0)
                ok = false;
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_ಬಹುಲಕ))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          ಆದರೆ (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      ಆದರೆ (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
dd_copy (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = EXIT_SUCCESS;
  idx_t n_ಬೈಟುಗಳು_read;

  ಆದರೆ (skip_ದಾಖಲೆಗಳು != 0 || skip_ಬೈಟುಗಳು != 0)
    {
      intmax_t us_ಬೈಟುಗಳು;
      bool us_ಬೈಟುಗಳು_ಅತಿಪ್ರವಾಹ =
        (ckd_mul (&us_ಬೈಟುಗಳು, skip_ದಾಖಲೆಗಳು, ಒಳಹರಿವು_blocksize)
         || ckd_ಕೂಡಿಸು (&us_ಬೈಟುಗಳು, skip_ಬೈಟುಗಳು, us_ಬೈಟುಗಳು));
      off_t ಒಳಹರಿವು_offset0 = ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ;
      intmax_t us_ಖಂಡಗಳು = skip (STDIN_FILENO, ಒಳಹರಿವು_file,
                                 skip_ದಾಖಲೆಗಳು, ಒಳಹರಿವು_blocksize, &skip_ಬೈಟುಗಳು);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      ಆದರೆ ((us_ಖಂಡಗಳು
           || (0 <= ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ
               && (us_ಬೈಟುಗಳು_ಅತಿಪ್ರವಾಹ
                   || us_ಬೈಟುಗಳು != ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ - ಒಳಹರಿವು_offset0)))
          && ಸ್ಥಿತಿ_level != ಸ್ಥಿತಿ_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (ಒಳಹರಿವು_file));
        }
    }

  ಆದರೆ (seek_ದಾಖಲೆಗಳು != 0 || seek_ಬೈಟುಗಳು != 0)
    {
      idx_t ಬೈಟುಗಳು = seek_ಬೈಟುಗಳು;
      intmax_t write_ದಾಖಲೆಗಳು = skip (STDOUT_FILENO, output_file,
                                      seek_ದಾಖಲೆಗಳು, output_blocksize, &ಬೈಟುಗಳು);

      ಆದರೆ (write_ದಾಖಲೆಗಳು != 0 || ಬೈಟುಗಳು != 0)
        {
          memset (obuf, 0, write_ದಾಖಲೆಗಳು ? output_blocksize : ಬೈಟುಗಳು);

          ಮಾಡು
            {
              idx_t ಗಾತ್ರ_2 = write_ದಾಖಲೆಗಳು ? output_blocksize : ಬೈಟುಗಳು;
              ಆದರೆ (iwrite (STDOUT_FILENO, obuf, ಗಾತ್ರ_2) != ಗಾತ್ರ_2)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              ಆದರೆ (write_ದಾಖಲೆಗಳು != 0)
                write_ದಾಖಲೆಗಳು--;
              ಇಲ್ಲದಿದ್ದರೆ
                ಬೈಟುಗಳು = 0;
            }
          ಆಗ (write_ದಾಖಲೆಗಳು || ಬೈಟುಗಳು);
        }
    }

  ಆದರೆ (max_ದಾಖಲೆಗಳು == 0 && max_ಬೈಟುಗಳು == 0)
    ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;

  alloc_ibuf ();
  alloc_obuf ();
  ಪೂರ್ಣಾಂಕ saved_byte = -1;

  ಆಗ (true)
    {
      ಆದರೆ (ಸ್ಥಿತಿ_level == ಸ್ಥಿತಿ_ಪ್ರಗತಿ)
        {
          xtime_t ಪ್ರಗತಿ_ಸಮಯ = gethrxtime ();
          ಆದರೆ (ಮುಂದಿನ_ಸಮಯ <= ಪ್ರಗತಿ_ಸಮಯ)
            {
              print_xfer_stats (ಪ್ರಗತಿ_ಸಮಯ);
              ಮುಂದಿನ_ಸಮಯ += XTIME_PRECISION;
            }
        }

      ಆದರೆ (r_partial + r_ತುಂಬಿದ >= max_ದಾಖಲೆಗಳು + !!max_ಬೈಟುಗಳು)
        ನಿಲ್ಲಿಸು;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      ಆದರೆ ((conversions_ಮುಖವಾಡ & C_SYNC) && (conversions_ಮುಖವಾಡ & C_NOERROR))
        memset (ibuf,
                (conversions_ಮುಖವಾಡ & (C_ಖಂಡ | C_UNBLOCK)) ? ' ' : '\0',
                ಒಳಹರಿವು_blocksize);

      ಆದರೆ (r_partial + r_ತುಂಬಿದ >= max_ದಾಖಲೆಗಳು)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_ಬೈಟುಗಳು);
      ಇಲ್ಲದಿದ್ದರೆ
        nread = iread_fnc (STDIN_FILENO, ibuf, ಒಳಹರಿವು_blocksize);

      ಆದರೆ (nread > 0)
        {
          advance_ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ (nread);
          ಆದರೆ (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_ಮುಖವಾಡ & C_NOTRUNC);
          ನಿಲ್ಲಿಸು;			/* EOF.  */
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (!(conversions_ಮುಖವಾಡ & C_NOERROR) || ಸ್ಥಿತಿ_level != ಸ್ಥಿತಿ_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (ಒಳಹರಿವು_file));

          ಆದರೆ (conversions_ಮುಖವಾಡ & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = ಒಳಹರಿವು_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              ಆದರೆ (!advance_ಒಳಹರಿವು_after_read_error (bad_portion))
                {
                  exit_ಸ್ಥಿತಿ = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  ಒಳಹರಿವು_seekable = false;
                  ಒಳಹರಿವು_seek_errno = ESPIPE;
                }
              ಆದರೆ ((conversions_ಮುಖವಾಡ & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              ಇಲ್ಲದಿದ್ದರೆ
                ಮುಂದುವರಿ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Write any partial block. */
              exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
              ನಿಲ್ಲಿಸು;
            }
        }

      n_ಬೈಟುಗಳು_read = nread;

      ಆದರೆ (n_ಬೈಟುಗಳು_read < ಒಳಹರಿವು_blocksize)
        {
          r_partial++;
          partread = n_ಬೈಟುಗಳು_read;
          ಆದರೆ (conversions_ಮುಖವಾಡ & C_SYNC)
            {
              ಆದರೆ (!(conversions_ಮುಖವಾಡ & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_ಬೈಟುಗಳು_read,
                        (conversions_ಮುಖವಾಡ & (C_ಖಂಡ | C_UNBLOCK)) ? ' ' : '\0',
                        ಒಳಹರಿವು_blocksize - n_ಬೈಟುಗಳು_read);
              n_ಬೈಟುಗಳು_read = ಒಳಹರಿವು_blocksize;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          r_ತುಂಬಿದ++;
          partread = 0;
        }

      ಆದರೆ (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_ಬೈಟುಗಳು_read);
          w_ಬೈಟುಗಳು += nwritten;
          ಆದರೆ (nwritten != n_ಬೈಟುಗಳು_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              ಆದರೆ (nwritten != 0)
                w_partial++;
              ಹಿಂತಿರುಗಿ EXIT_FAILURE;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (n_ಬೈಟುಗಳು_read == ಒಳಹರಿವು_blocksize)
            w_ತುಂಬಿದ++;
          ಇಲ್ಲದಿದ್ದರೆ
            w_partial++;
          ಮುಂದುವರಿ;
        }

      /* Do any translations on the whole buffer at once.  */

      ಆದರೆ (translation_needed)
        translate_ಬಫರ್ (ibuf, n_ಬೈಟುಗಳು_read);

      ಆದರೆ (conversions_ಮುಖವಾಡ & C_SWAB)
        bufstart = swab_ಬಫರ್ (ibuf, &n_ಬೈಟುಗಳು_read, &saved_byte);
      ಇಲ್ಲದಿದ್ದರೆ
        bufstart = ibuf;

      ಆದರೆ (conversions_ಮುಖವಾಡ & C_ಖಂಡ)
        copy_with_ಖಂಡ (bufstart, n_ಬೈಟುಗಳು_read);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (conversions_ಮುಖವಾಡ & C_UNBLOCK)
        copy_with_unblock (bufstart, n_ಬೈಟುಗಳು_read);
      ಇಲ್ಲದಿದ್ದರೆ
        copy_simple (bufstart, n_ಬೈಟುಗಳು_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  ಆದರೆ (0 <= saved_byte)
    {
      ಅಕ್ಷರ saved_char = saved_byte;
      ಆದರೆ (conversions_ಮುಖವಾಡ & C_ಖಂಡ)
        copy_with_ಖಂಡ (&saved_char, 1);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (conversions_ಮುಖವಾಡ & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      ಇಲ್ಲದಿದ್ದರೆ
        output_char (saved_char);
    }

  ಆದರೆ ((conversions_ಮುಖವಾಡ & C_ಖಂಡ) && ಲಂಬಸಾಲು > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      ಗಾಗಿ (idx_t i = ಲಂಬಸಾಲು; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  ಆದರೆ (ಲಂಬಸಾಲು && (conversions_ಮುಖವಾಡ & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  ಆದರೆ (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_ಬೈಟುಗಳು += nwritten;
      ಆದರೆ (nwritten != 0)
        w_partial++;
      ಆದರೆ (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          ಹಿಂತಿರುಗಿ EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  ಆದರೆ (final_op_was_seek)
    {
      ರಚನೆ stat stdout_stat;
      ಆದರೆ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          ಹಿಂತಿರುಗಿ EXIT_FAILURE;
        }
      ಆದರೆ (S_ISREG (stdout_stat.st_ಬಹುಲಕ) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_ಸ್ಥಾನಾಂತರ = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          ಆದರೆ (0 <= output_ಸ್ಥಾನಾಂತರ && stdout_stat.st_ಗಾತ್ರ < output_ಸ್ಥಾನಾಂತರ)
            {
              ಆದರೆ (iftruncate (STDOUT_FILENO, output_ಸ್ಥಾನಾಂತರ) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_ಸ್ಥಾನಾಂತರ}, quoteaf (output_file));
                  ಹಿಂತಿರುಗಿ EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  ಆದರೆ (conversions_ಮುಖವಾಡ & (C_FDATASYNC | C_FSYNC)
      && ಸ್ಥಿತಿ_level == ಸ್ಥಿತಿ_ಪ್ರಗತಿ
      && 0 <= reported_w_ಬೈಟುಗಳು && reported_w_ಬೈಟುಗಳು < w_ಬೈಟುಗಳು)
    print_xfer_stats (0);

  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
synchronize_output (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = 0;
  ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ = conversions_ಮುಖವಾಡ;
  conversions_ಮುಖವಾಡ &= ~ (C_FDATASYNC | C_FSYNC);

  ಆದರೆ ((ಮುಖವಾಡ & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      ಆದರೆ (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
        }
      ಮುಖವಾಡ |= C_FSYNC;
    }

  ಆದರೆ ((ಮುಖವಾಡ & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      ಹಿಂತಿರುಗಿ EXIT_FAILURE;
    }

  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  off_t ಸ್ಥಾನಾಂತರ;

  install_signal_handlers ();

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if parse_long_options exits.  */
  atexit (maybe_close_stdout);

  ಪುಟ_ಗಾತ್ರ = getpagesize ();

  parse_gnu_standard_options_only (argc, argv, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE, ಆವೃತ್ತಿ,
                                   true, usage, AUTHORS,
                                   (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಖಾಲಿ);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 256; i++)
    trans_ಕೋಷ್ಟಕ[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  ಆದರೆ (ಒಳಹರಿವು_file == ಖಾಲಿ)
    {
      ಒಳಹರಿವು_file = _("standard input");
      ಗಣ_fd_flags (STDIN_FILENO, ಒಳಹರಿವು_flags, ಒಳಹರಿವು_file);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (ifd_reopen (STDIN_FILENO, ಒಳಹರಿವು_file, O_RDONLY | ಒಳಹರಿವು_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (ಒಳಹರಿವು_file));
    }

  ಸ್ಥಾನಾಂತರ = lseek (STDIN_FILENO, 0, SEEK_CUR);
  ಒಳಹರಿವು_seekable = (0 <= ಸ್ಥಾನಾಂತರ);
  ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ = MAX (0, ಸ್ಥಾನಾಂತರ);
  ಒಳಹರಿವು_seek_errno = errno;

  ಆದರೆ (output_file == ಖಾಲಿ)
    {
      output_file = _("standard output");
      ಗಣ_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      mode_t perms = ಬಹುಲಕ_RW_UGO;
      ಪೂರ್ಣಾಂಕ opts
        = (output_flags
           | (conversions_ಮುಖವಾಡ & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_ಮುಖವಾಡ & C_EXCL ? O_EXCL : 0)
           | (seek_ದಾಖಲೆಗಳು || (conversions_ಮುಖವಾಡ & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t ಗಾತ್ರ_2;
      ಆದರೆ ((ckd_mul (&ಗಾತ್ರ_2, seek_ದಾಖಲೆಗಳು, output_blocksize)
           || ckd_ಕೂಡಿಸು (&ಗಾತ್ರ_2, seek_ಬೈಟುಗಳು, ಗಾತ್ರ_2))
          && !(conversions_ಮುಖವಾಡ & C_NOTRUNC))
        error (EXIT_FAILURE, 0,
               _("offset too large: "
                 "cannot truncate to a length of seek=%jd"
                 " (%td-byte) blocks"),
               seek_ದಾಖಲೆಗಳು, output_blocksize);

      /* Open the output file with *read* access only if we might
         need to read to satisfy a 'seek=' request.  If we can't read
         the file, go ahead with write-only access; it might work.  */
      ಆದರೆ ((! seek_ದಾಖಲೆಗಳು
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      ಆದರೆ (seek_ದಾಖಲೆಗಳು != 0 && !(conversions_ಮುಖವಾಡ & C_NOTRUNC))
        {
          ಆದರೆ (iftruncate (STDOUT_FILENO, ಗಾತ್ರ_2) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              ಪೂರ್ಣಾಂಕ ftruncate_errno = errno;
              ರಚನೆ stat stdout_stat;
              ಆದರೆ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISREG (stdout_stat.st_ಬಹುಲಕ)
                       || S_ISDIR (stdout_stat.st_ಬಹುಲಕ)
                       || S_TYPEISSHM (&stdout_stat))
                {
                  intmax_t isize = ಗಾತ್ರ_2;
                  error (EXIT_FAILURE, ftruncate_errno,
                         _("failed to truncate to %jd bytes"
                           " in output file %s"),
                         isize, quoteaf (output_file));
                }
            }
        }
    }

  ಆರಂಭ_ಸಮಯ = gethrxtime ();
  ಮುಂದಿನ_ಸಮಯ = ಆರಂಭ_ಸಮಯ + XTIME_PRECISION;

  ಪೂರ್ಣಾಂಕ copy_ಸ್ಥಿತಿ = dd_copy ();
  ಪೂರ್ಣಾಂಕ sync_ಸ್ಥಿತಿ = synchronize_output ();
  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = copy_ಸ್ಥಿತಿ | sync_ಸ್ಥಿತಿ;

  ಆದರೆ (max_ದಾಖಲೆಗಳು == 0 && max_ಬೈಟುಗಳು == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      ಆದರೆ (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (ಒಳಹರಿವು_file));
          exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
        }
      ಆದರೆ (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      ಆದರೆ (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      ಆದರೆ (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  ಮುಕ್ತಾಯ_ಮೇಲೆ ();
  main_exit (exit_ಸ್ಥಿತಿ);
}
