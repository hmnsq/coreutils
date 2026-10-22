/* od -- dump files in octal and other formats
   Copyright (C) 1992-2026 Free Software Foundation, Inc.

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

/* Written by Jim Meyering.  */

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <endian.h>
#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "ftoastr.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "stat-size.h"
#અન્તર્ભાવ "xbinary-io.h"
#અન્તર્ભાવ "xprintf.h"
#અન્તર્ભાવ "xstrtol.h"
#અન્તર્ભાવ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "od"

#define AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#define DEFAULT_BYTES_PER_BLOCK 16

#જો FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  પ્રકારવ્યાખ્યા _Float16 float16;
#નહીંતર
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  પ્રકારવ્યાખ્યા દશાંશ float16;
#endif

#જો BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  પ્રકારવ્યાખ્યા __bf16 bfloat16;
#નહીંતર
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  પ્રકારવ્યાખ્યા દશાંશ bfloat16;
#endif

ગણના size_spec
  {
    NO_SIZE = 0,
    CHAR,
    SHORT = CHAR + (UCHAR_MAX < USHRT_MAX),
    INT = SHORT + (USHRT_MAX < UINT_MAX),
    LONG = INT + (UINT_MAX < ULONG_MAX),
    LONG_LONG = LONG + (ULONG_MAX < ULLONG_MAX),
    INTMAX = LONG_LONG + (ULLONG_MAX < UINTMAX_MAX),
    FLOAT_HALF, /* Used only if (FLOAT16_SUPPORTED || BF16_SUPPORTED).  */
    FLOAT_SINGLE = FLOAT_HALF + (FLOAT16_SUPPORTED || BF16_SUPPORTED),
    FLOAT_DOUBLE = FLOAT_SINGLE + (FLT_MANT_DIG < DBL_MANT_DIG
                                   || FLT_MAX_EXP < DBL_MAX_EXP),
    FLOAT_LONG_DOUBLE = FLOAT_DOUBLE + (DBL_MANT_DIG < LDBL_MANT_DIG
                                        || DBL_MAX_EXP < LDBL_MAX_EXP),
    N_SIZE_SPECS
  };

ગણના output_format
  {
    SIGNED_DECIMAL,
    UNSIGNED_DECIMAL,
    OCTAL,
    HEXADECIMAL,
    FLOATING_POINT,
    HFLOATING_POINT,
    BFLOATING_POINT,
    NAMED_CHARACTER,
    CHARACTER
  };

ગણના { MAX_INTEGRAL_TYPE_WIDTH = UINTMAX_WIDTH };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
ગણના
  {
    FMT_BYTES_ALLOCATED =
      કદ "%*.%dlld" - કદ "%d" + INT_STRLEN_BOUND (પૂર્ણાંક) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_WIDTH <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
પ્રકારવ્યાખ્યા શૂન્ય (*print_function_type)
  (idx_t fields, idx_t blank, શૂન્ય અચલ *data,
   અક્ષર અચલ *fmt, પૂર્ણાંક width, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
સંરચના tspec
  {
    ગણના output_format fmt;
    ગણના size_spec size; /* Type of input object.  */
    print_function_type print_function;
    અક્ષર fmt_string[FMT_BYTES_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_mode_trailer;
    પૂર્ણાંક field_width; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_width; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
સ્થિર અચલ પૂર્ણાંક width_bytes[] =
{
  -1,
  કદ (unsigned અક્ષર),
#જો UCHAR_MAX < USHRT_MAX
  કદ (unsigned short પૂર્ણાંક),
#endif
#જો USHRT_MAX < UINT_MAX
  કદ (unsigned પૂર્ણાંક),
#endif
#જો UINT_MAX < ULONG_MAX
  કદ (unsigned long પૂર્ણાંક),
#endif
#જો ULONG_MAX < ULLONG_MAX
  કદ (unsigned long long પૂર્ણાંક),
#endif
#જો ULLONG_MAX < UINTMAX_MAX
  કદ (uintmax_t),
#endif
#જો BF16_SUPPORTED
  કદ (bfloat16),
#અથવાજો FLOAT16_SUPPORTED
  કદ (float16),
#endif
#જો FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  કદ (દશાંશ),
#endif
  કદ (દ્વિગુણ),
#જો DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  કદ (long દ્વિગુણ),
#endif
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (width_bytes) == N_SIZE_SPECS);

/* Names for some non-printing characters.  */
સ્થિર અક્ષર અચલ charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
સ્થિર પૂર્ણાંક address_base = 8;

/* The number of octal digits required to represent the largest
   address value.  */
ગણના { MAX_ADDRESS_LENGTH = ((INTMAX_WIDTH - 1) / 3
                             + ((INTMAX_WIDTH - 1) % 3 != 0)) };

/* Width of a normal address.  */
સ્થિર પૂર્ણાંક address_pad_len = 7;

/* Minimum length when detecting --strings.  */
સ્થિર idx_t string_min;

/* True when in --strings mode.  */
સ્થિર bool flag_dump_strings;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
સ્થિર bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
સ્થિર bool flag_pseudo_start;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
સ્થિર intmax_t pseudo_offset;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
સ્થિર શૂન્ય format_address_std (intmax_t, અક્ષર);
સ્થિર શૂન્ય (*format_address) (intmax_t, અક્ષર) = format_address_std;

/* The number of input bytes to skip before formatting and writing.  */
સ્થિર intmax_t n_bytes_to_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
સ્થિર intmax_t end_offset = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
સ્થિર bool abbreviate_duplicate_blocks = true;

/* An array of specs describing how to format each input block.  */
સ્થિર સંરચના tspec *spec;

/* The number of format specs.  */
સ્થિર idx_t n_specs;

/* The allocated length of SPEC.  */
સ્થિર idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
સ્થિર idx_t bytes_per_block;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
સ્થિર અક્ષર અચલ *આગત_filename;

/* A null-terminated list of the file-arguments from the command line.  */
સ્થિર અક્ષર અચલ *અચલ *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
સ્થિર અક્ષર અચલ *અચલ default_file_list[] = {"-", ખાલી};

/* The input stream associated with the current file.  */
સ્થિર FILE *in_stream;

/* If true, at least one of the files we read was standard input.  */
સ્થિર bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
સ્થિર ગણના size_spec અચલ integral_type_size[] =
  {
#જો UCHAR_MAX < USHRT_MAX
    [કદ (unsigned અક્ષર)] = CHAR,
#endif
#જો USHRT_MAX < UINT_MAX
    [કદ (unsigned short પૂર્ણાંક)] = SHORT,
#endif
    [કદ (unsigned પૂર્ણાંક)] = INT,
#જો UINT_MAX < ULONG_MAX
    [કદ (unsigned long પૂર્ણાંક)] = LONG,
#endif
#જો ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [કદ (unsigned long long પૂર્ણાંક)] = LONG_LONG,
#endif
#જો ULONG_MAX < UINTMAX_MAX
    [કદ (uintmax_t)] = INTMAX,
#endif
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
સ્થિર ગણના size_spec અચલ fp_type_size[] =
  {
#જો FLOAT16_SUPPORTED
    [કદ (float16)] = FLOAT_HALF,
#અથવાજો BF16_SUPPORTED
    [કદ (bfloat16)] = FLOAT_HALF,
#endif
#જો FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [કદ (દશાંશ)] = FLOAT_SINGLE,
#endif
    [કદ (દ્વિગુણ)] = FLOAT_DOUBLE,
#જો DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [કદ (long દ્વિગુણ)] = FLOAT_LONG_DOUBLE,
#endif
  };

/* Use native endianness by default.  */
સ્થિર bool આગત_swap;

સ્થિર અક્ષર અચલ short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

ગણના endian_type
{
  endian_little,
  endian_big
};

સ્થિર અક્ષર અચલ *અચલ endian_args[] =
{
  "little", "big", ખાલી
};

સ્થિર ગણના endian_type અચલ endian_types[] =
{
  endian_little, endian_big
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"skip-bytes", required_argument, ખાલી, 'j'},
  {"address-radix", required_argument, ખાલી, 'A'},
  {"read-bytes", required_argument, ખાલી, 'N'},
  {"format", required_argument, ખાલી, 't'},
  {"output-duplicates", no_argument, ખાલી, 'v'},
  {"strings", optional_argument, ખાલી, 'S'},
  {"traditional", no_argument, ખાલી, TRADITIONAL_OPTION},
  {"width", optional_argument, ખાલી, 'w'},
  {"endian", required_argument, ખાલી, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]OFFSET[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]OFFSET[.][b] [+][LABEL][.][b]]\n\
"),
              program_name, program_name, program_name);
      fputs (_("\n\
Write an unambiguous representation, octal bytes by મૂળભૂત,\n\
of FILE to standard output.  With more than one FILE argument,\n\
concatenate them in the listed order to form the આગત.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If first and second call formats both apply, the second format is assumed\n\
જો the last operand begins with + or (જો there are 2 operands) a digit.\n\
An OFFSET operand means -j OFFSET.  LABEL is the pseudo-address\n\
at first byte printed, incremented when dump is progressing.\n\
For OFFSET and LABEL, a 0x or 0X prefix indicates hexadecimal;\n\
suffixes may be . માટે octal and b માટે multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --address-radix=RADIX\n\
         output format માટે file offsets;\n\
         RADIX is one of [doxn], માટે Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         swap આગત bytes according the specified order\n\
"));
      oputs (_("\
  -j, --skip-bytes=BYTES\n\
         skip BYTES આગત bytes first\n\
"));
      oputs (_("\
  -N, --પઠ-bytes=BYTES\n\
         limit dump to BYTES આગત bytes\n\
"));
      oputs (_("\
  -S BYTES, --strings[=BYTES]\n\
         show only NUL terminated strings\n\
         of at least BYTES (મૂળભૂત 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         કરો not use * to mark લીટી suppression\n\
"));
      oputs (_("\
  -w[BYTES], --width[=BYTES]\n\
         output BYTES bytes per output લીટી;\n\
         32 is implied when BYTES is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments in third form above\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
\n\
Traditional format specifications may be intermixed; they accumulate:\n\
"), stdout);
      oputs (_("\
  -a   same as -t a,  select named characters, ignoring ઉપલું-order bit\n\
"));
      oputs (_("\
  -b   same as -t o1, select octal bytes\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select unsigned decimal 2-byte units\n\
"));
      oputs (_("\
  -f   same as -t fF, select floats\n\
"));
      oputs (_("\
  -i   same as -t dI, select decimal ints\n\
"));
      oputs (_("\
  -l   same as -t dL, select decimal longs\n\
"));
      oputs (_("\
  -o   same as -t o2, select octal 2-byte units\n\
"));
      oputs (_("\
  -s   same as -t d2, select decimal 2-byte units\n\
"));
      oputs (_("\
  -x   same as -t x2, select hexadecimal 2-byte units\n\
"));
      fputs (_("\
\n\
\n\
TYPE is made up of one or more of these specifications:\n\
  a          named character, ignoring ઉપલું-order bit\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[SIZE]    signed decimal, SIZE bytes per integer\n\
  f[SIZE]    floating point, SIZE bytes per દશાંશ\n\
  o[SIZE]    octal, SIZE bytes per integer\n\
  u[SIZE]    unsigned decimal, SIZE bytes per integer\n\
  x[SIZE]    hexadecimal, SIZE bytes per integer\n\
"), stdout);
      fputs (_("\
\n\
SIZE is a સંખ્યા.  For TYPE in [doux], SIZE may also be C માટે\n\
કદ(અક્ષર), S માટે કદ(short), I માટે કદ(પૂર્ણાંક) or L માટે\n\
કદ(long).  If TYPE is f, SIZE may also be B માટે Brain 16 bit,\n\
H માટે Half precision દશાંશ, F માટે કદ(દશાંશ), D માટે કદ(દ્વિગુણ),\n\
or L માટે કદ(long દ્વિગુણ).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix to any type displays printable characters at the end of\n\
each output લીટી.\n\
"), stdout);
      fputs (_("\
\n\
\n\
BYTES is hex with 0x or 0X prefix, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on માટે G, T, P, E, Z, Y, R, Q.\n\
દ્વિઅંકી_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
સ્થિર શૂન્ય
print_n_spaces (intmax_t n)
{
  માટે (; 0 < n; n--)
    putchar (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
સ્થિર idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  પાછા pad / fields * i + m * i / fields;
}
સ્થિર bool
pad_at_overflow (idx_t fields)
{
  intmax_t product;
  પાછા ckd_mul (&product, fields - 1, fields - 1);
}

#define PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
સ્થિર શૂન્ય                                                             \
N (idx_t fields, idx_t blank, શૂન્ય અચલ *block,			\
   FMT_STRING_DECL, પૂર્ણાંક width, idx_t pad)				\
{                                                                       \
  T અચલ *p = block;                                                   \
  idx_t pad_remaining = pad;						\
  માટે (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t next_pad = pad_at (fields, i - 1, pad);			\
      પૂર્ણાંક adjusted_width = pad_remaining - next_pad + width;            \
      T x;                                                              \
      જો (આગત_swap && કદ (T) > 1)                                 \
        {                                                               \
          સંઘ {                                                       \
            T x;                                                        \
            અક્ષર b[કદ (T)];                                         \
          } u;                                                          \
          માટે (idx_t j = 0; j < કદ (T); j++)			\
            u.b[j] = ((અક્ષર અચલ *) p)[કદ (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      નહીંતર                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_remaining = next_pad;                                         \
    }                                                                   \
}

#define PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, અક્ષર અચલ *fmt_string,                           \
                xprintf (fmt_string, adjusted_width, x))

#define PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_UNUSED અક્ષર અચલ *fmt_string,              \
                અક્ષર buf[BUFSIZE];                                      \
                FTOASTR (buf, કદ buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_width, buf))

PRINT_TYPE (print_s_char, signed અક્ષર)
PRINT_TYPE (print_char, unsigned અક્ષર)
PRINT_TYPE (print_s_short, short પૂર્ણાંક)
PRINT_TYPE (print_short, unsigned short પૂર્ણાંક)
PRINT_TYPE (print_int, unsigned પૂર્ણાંક)
PRINT_TYPE (print_long, unsigned long પૂર્ણાંક)
PRINT_TYPE (print_long_long, unsigned long long પૂર્ણાંક)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, દશાંશ, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, દ્વિગુણ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, long દ્વિગુણ, ldtoastr, LDBL_BUFSIZE_BOUND)

#undef PRINT_TYPE
#undef PRINT_FLOATTYPE

સ્થિર શૂન્ય
dump_hexl_mode_trailer (idx_t n_bytes, અક્ષર અચલ *block)
{
  fputs ("  >", stdout);
  માટે (idx_t i = n_bytes; i > 0; i--)
    {
      unsigned અક્ષર c = *block++;
      unsigned અક્ષર c2 = (isprint (c) ? c : '.');
      putchar (c2);
    }
  putchar ('<');
}

સ્થિર શૂન્ય
print_named_ascii (idx_t fields, idx_t blank, શૂન્ય અચલ *block,
                   MAYBE_UNUSED અક્ષર અચલ *unused_fmt_string,
                   પૂર્ણાંક width, idx_t pad)
{
  unsigned અક્ષર અચલ *p = block;
  idx_t pad_remaining = pad;
  માટે (idx_t i = fields; blank < i; i--)
    {
      પૂર્ણાંક masked_c = *p++ & 0x7f;
      અક્ષર અચલ *s;
      અક્ષર buf[2];

      જો (masked_c == 127)
        s = "del";
      નહીંતર જો (masked_c <= 040)
        s = charname[masked_c];
      નહીંતર
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t next_pad = pad_at (fields, i - 1, pad);
      પૂર્ણાંક adjusted_width = pad_remaining - next_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = next_pad;
    }
}

સ્થિર શૂન્ય
print_ascii (idx_t fields, idx_t blank, શૂન્ય અચલ *block,
             MAYBE_UNUSED અક્ષર અચલ *unused_fmt_string, પૂર્ણાંક width,
             idx_t pad)
{
  unsigned અક્ષર અચલ *p = block;
  idx_t pad_remaining = pad;
  માટે (idx_t i = fields; blank < i; i--)
    {
      unsigned અક્ષર c = *p++;
      અક્ષર અચલ *s;
      અક્ષર buf[4];

      પસંદગી (c)
        {
        સ્થિતિ '\0':
          s = "\\0";
          તોડો;

        સ્થિતિ '\a':
          s = "\\a";
          તોડો;

        સ્થિતિ '\b':
          s = "\\b";
          તોડો;

        સ્થિતિ '\f':
          s = "\\f";
          તોડો;

        સ્થિતિ '\n':
          s = "\\n";
          તોડો;

        સ્થિતિ '\r':
          s = "\\r";
          તોડો;

        સ્થિતિ '\t':
          s = "\\t";
          તોડો;

        સ્થિતિ '\v':
          s = "\\v";
          તોડો;

        મૂળભૂત:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t next_pad = pad_at (fields, i - 1, pad);
      પૂર્ણાંક adjusted_width = pad_remaining - next_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = next_pad;
    }
}

/* Convert a null-terminated (possibly zero-length) string S to an
   int value.  If S points to a non-digit set *P to S,
   *VAL to 0, and return true.  Otherwise, accumulate the integer value of
   the string of digits.  If the string of digits represents a value
   larger than INT_MAX, don't modify *VAL or *P and return false.
   Otherwise, advance *P to the first non-digit after S, set *VAL to
   the result of the conversion and return true.  */

સ્થિર bool
simple_strtoi (અક્ષર અચલ *s, અક્ષર અચલ **p, પૂર્ણાંક *val)
{
  પૂર્ણાંક sum;

  માટે (sum = 0; c_isdigit (*s); s++)
    જો (ckd_mul (&sum, sum, 10) || ckd_add (&sum, sum, *s - '0'))
      પાછા false;
  *p = s;
  *val = sum;
  પાછા true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
સ્થિર અક્ષર અચલ *
ispec_to_format (ગણના size_spec size_spec,
                 અક્ષર અચલ *int_format,
                 અક્ષર અચલ *long_format,
                 અક્ષર અચલ *long_long_format,
                 અક્ષર અચલ *intmax_format)
{
  પાછા (LONG < INTMAX && size_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && size_spec == LONG_LONG ? long_long_format
          : INT < LONG && size_spec == LONG ? long_format
          : int_format);
}

/* If S points to a single valid modern od format string, put
   a description of that format in *TSPEC, make *NEXT point at the
   character following the just-decoded format (if *NEXT is non-null),
   and return true.  If S is not valid, don't modify *NEXT or *TSPEC,
   give a diagnostic, and return false.  For example, if S were
   "d4afL" *NEXT would be set to "afL" and *TSPEC would be
     {
       fmt = SIGNED_DECIMAL;
       size = INT or LONG; (whichever integral_type_size[4] resolves to)
       print_function = print_int; (assuming size == INT)
       field_width = 11;
       fmt_string = "%*d";
      }
   pad_width is determined later, but is at least as large as the
   number of fields printed per row.
   S_ORIG is solely for reporting errors.  It should be the full format
   string argument.
   */

સ્થિર bool ATTRIBUTE_NONNULL ()
decode_one_format (અક્ષર અચલ *s_orig, અક્ષર અચલ *s, અક્ષર અચલ **next,
                   સંરચના tspec *tspec)
{
  ગણના size_spec size_spec;
  પૂર્ણાંક size;
  ગણના output_format fmt;
  print_function_type print_function;
  અક્ષર અચલ *p;
  અક્ષર c;
  પૂર્ણાંક field_width;

  પસંદગી (*s)
    {
    સ્થિતિ 'd':
    સ્થિતિ 'o':
    સ્થિતિ 'u':
    સ્થિતિ 'x':
      c = *s;
      ++s;
      પસંદગી (*s)
        {
        સ્થિતિ 'C':
          ++s;
          size = કદ (unsigned અક્ષર);
          તોડો;

        સ્થિતિ 'S':
          ++s;
          size = કદ (unsigned short પૂર્ણાંક);
          તોડો;

        સ્થિતિ 'I':
          ++s;
          size = કદ (unsigned પૂર્ણાંક);
          તોડો;

        સ્થિતિ 'L':
          ++s;
          size = કદ (unsigned long પૂર્ણાંક);
          તોડો;

        મૂળભૂત:
          જો (! simple_strtoi (s, &p, &size))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              પાછા false;
            }
          જો (p == s)
            size = કદ (unsigned પૂર્ણાંક);
          નહીંતર
            {
              જો (countof (integral_type_size) <= size
                  || integral_type_size[size] == NO_SIZE)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), size);
                  પાછા false;
                }
              s = p;
            }
          તોડો;
        }

      size_spec = integral_type_size[size];

      પસંદગી (c)
        {
        સ્થિતિ 'd':
          fmt = SIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * size - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (size_spec, "d", "ld", "lld", "jd"));
          તોડો;

        સ્થિતિ 'o':
          fmt = OCTAL;
          field_width = (CHAR_BIT * size + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (size_spec, "o", "lo", "llo", "jo"));
          તોડો;

        સ્થિતિ 'u':
          fmt = UNSIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * size);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (size_spec, "u", "lu", "llu", "ju"));
          તોડો;

        સ્થિતિ 'x':
          fmt = HEXADECIMAL;
          field_width = (CHAR_BIT * size + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (size_spec, "x", "lx", "llx", "jx"));
          તોડો;

        મૂળભૂત:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (size_spec == INT
           ? print_int
           : SHORT < INT && size_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && size_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && size_spec == LONG
           ? print_long
           : LONG < INTMAX && size_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && size_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) ખાલી));
      તોડો;

    સ્થિતિ 'f':
      fmt = FLOATING_POINT;
      ++s;
      પસંદગી (*s)
        {
        સ્થિતિ 'B':
          ++s;
          fmt = BFLOATING_POINT;
          size = કદ (bfloat16);
          તોડો;

        સ્થિતિ 'H':
          ++s;
          fmt = HFLOATING_POINT;
          size = કદ (float16);
          તોડો;

        સ્થિતિ 'F':
          ++s;
          size = કદ (દશાંશ);
          તોડો;

        સ્થિતિ 'D':
          ++s;
          size = કદ (દ્વિગુણ);
          તોડો;

        સ્થિતિ 'L':
          ++s;
          size = કદ (long દ્વિગુણ);
          તોડો;

        મૂળભૂત:
          જો (! simple_strtoi (s, &p, &size))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              પાછા false;
            }
          જો (p == s)
            size = કદ (દ્વિગુણ);
          નહીંતર
            {
              જો (countof (fp_type_size) <= size
                  || fp_type_size[size] == NO_SIZE
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && size == કદ (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), size);
                  પાછા false;
                }
              s = p;
            }
          તોડો;
        }
      size_spec = fp_type_size[size];

      જો ((! FLOAT16_SUPPORTED && fmt == HFLOATING_POINT)
          || (! BF16_SUPPORTED && fmt == BFLOATING_POINT))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        પાછા false;
      }

      {
        સંરચના lconv અચલ *locale = localeconv ();
        idx_t decimal_point_len =
          (locale->decimal_point[0] ? strlen (locale->decimal_point) : 1);

        જો (size_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            field_width = DBL_STRLEN_BOUND_L (decimal_point_len);
          }
        નહીંતર જો (FLOAT_SINGLE < FLOAT_DOUBLE && size_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        નહીંતર જો (FLOAT_HALF < FLOAT_SINGLE && size_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_POINT
                             ? print_bfloat : print_halffloat;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        નહીંતર જો (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && size_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            field_width = LDBL_STRLEN_BOUND_L (decimal_point_len);
          }
        નહીંતર
          affirm (false);

        તોડો;
      }

    સ્થિતિ 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      size_spec = CHAR;
      print_function = print_named_ascii;
      field_width = 3;
      તોડો;

    સ્થિતિ 'c':
      ++s;
      fmt = CHARACTER;
      size_spec = CHAR;
      print_function = print_ascii;
      field_width = 3;
      તોડો;

    મૂળભૂત:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      પાછા false;
    }

  tspec->size = size_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->field_width = field_width;
  tspec->hexl_mode_trailer = (*s == 'z');
  જો (tspec->hexl_mode_trailer)
    s++;

  *next = s;
  પાછા true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

સ્થિર bool
open_next_file (શૂન્ય)
{
  bool ok = true;

  કરો
    {
      આગત_filename = *file_list;
      જો (આગત_filename == ખાલી)
        પાછા ok;
      ++file_list;

      જો (streq (આગત_filename, "-"))
        {
          આગત_filename = _("standard input");
          in_stream = stdin;
          have_read_stdin = true;
          xset_દ્વિઅંકી_mode (STDIN_FILENO, O_દ્વિઅંકી);
        }
      નહીંતર
        {
          in_stream = fopen (આગત_filename, (O_દ્વિઅંકી ? "rb" : "r"));
          જો (in_stream == ખાલી)
            {
              error (0, errno, "%s", quotef (આગત_filename));
              ok = false;
            }
        }
    }
  જ્યારે (in_stream == ખાલી);

  જો (0 <= end_offset && !flag_dump_strings)
    setvbuf (in_stream, ખાલી, _IONBF, 0);

  પાછા ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

સ્થિર bool
check_and_close (પૂર્ણાંક in_errno)
{
  bool ok = true;

  જો (in_stream != ખાલી)
    {
      જો (!ferror (in_stream))
        in_errno = 0;
      જો (streq (file_list[-1], "-"))
        clearerr (in_stream);
      નહીંતર જો (fclose (in_stream) != 0 && !in_errno)
        in_errno = errno;
      જો (in_errno)
        {
          error (0, in_errno, "%s", quotef (આગત_filename));
          ok = false;
        }

      in_stream = ખાલી;
    }

  જો (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  પાછા ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

સ્થિર bool ATTRIBUTE_NONNULL ()
decode_format_string (અક્ષર અચલ *s)
{
  અક્ષર અચલ *s_orig = s;

  જ્યારે (*s != '\0')
    {
      અક્ષર અચલ *next;

      જો (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, કદ *spec);

      જો (! decode_one_format (s_orig, s, &next, &spec[n_specs]))
        પાછા false;

      affirm (s != next);
      s = next;
      ++n_specs;
    }

  પાછા true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

સ્થિર bool
skip (intmax_t n_skip)
{
  bool ok = true;
  પૂર્ણાંક in_errno = 0;

  જો (n_skip == 0)
    પાછા true;

  જ્યારે (in_stream != ખાલી)	/* EOF.  */
    {
      સંરચના stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      જો (fstat (fileno (in_stream), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          જો (S_ISREG (file_stats.st_mode)
              && STP_BLKSIZE (&file_stats) < file_stats.st_size)
            {
              જો (file_stats.st_size < n_skip)
                n_skip -= file_stats.st_size;
              નહીંતર
                {
                  જો (fseeko (in_stream, n_skip, SEEK_CUR) != 0)
                    {
                      in_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          નહીંતર જો (! (S_ISREG (file_stats.st_mode)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (in_stream, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          નહીંતર
            {
              અક્ષર buf[BUFSIZ];
              idx_t n_bytes_read, n_bytes_to_read = BUFSIZ;

              જ્યારે (0 < n_skip)
                {
                  જો (n_skip < n_bytes_to_read)
                    n_bytes_to_read = n_skip;
                  n_bytes_read = fread (buf, 1, n_bytes_to_read, in_stream);
                  n_skip -= n_bytes_read;
                  જો (n_bytes_read != n_bytes_to_read)
                    {
                      જો (ferror (in_stream))
                        {
                          in_errno = errno;
                          ok = false;
                          n_skip = 0;
                          તોડો;
                        }
                      જો (feof (in_stream))
                        તોડો;
                    }
                }
            }

          જો (n_skip == 0)
            તોડો;
        }

      નહીંતર   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (આગત_filename));
          ok = false;
        }

      ok &= check_and_close (in_errno);

      ok &= open_next_file ();
    }

  જો (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  પાછા ok;
}

સ્થિર શૂન્ય
format_address_none (MAYBE_UNUSED intmax_t address,
                     MAYBE_UNUSED અક્ષર c)
{
}

સ્થિર શૂન્ય
format_address_std (intmax_t address, અક્ષર c)
{
  અક્ષર buf[MAX_ADDRESS_LENGTH + 2];
  અક્ષર *p = buf + કદ buf;
  અક્ષર અચલ *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - address_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  પસંદગી (address_base)
    {
    સ્થિતિ 8:
      કરો
        *--p = '0' + (address & 7);
      જ્યારે ((address >>= 3) != 0);
      તોડો;

    સ્થિતિ 10:
      કરો
        *--p = '0' + (address % 10);
      જ્યારે ((address /= 10) != 0);
      તોડો;

    સ્થિતિ 16:
      કરો
        *--p = "0123456789abcdef"[address & 15];
      જ્યારે ((address >>= 4) != 0);
      તોડો;
    }

  જ્યારે (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

સ્થિર શૂન્ય
format_address_paren (intmax_t address, અક્ષર c)
{
  putchar ('(');
  format_address_std (address, ')');
  જો (c)
    putchar (c);
}

સ્થિર શૂન્ય
format_address_label (intmax_t address, અક્ષર c)
{
  format_address_std (address, ' ');

  intmax_t addr;
  જો (ckd_add (&addr, address, pseudo_offset))
    error (EXIT_FAILURE, 0, _("pseudo address too large for input"));

  format_address_paren (addr, c);
}

/* Write N_BYTES bytes from CURR_BLOCK to standard output once for each
   of the N_SPEC format specs.  CURRENT_OFFSET is the byte address of
   CURR_BLOCK in the concatenation of input files, and it is printed
   (optionally) only before the output line associated with the first
   format spec.  When duplicate blocks are being abbreviated, the output
   for a sequence of identical input blocks is the output for the first
   block followed by an asterisk alone on a line.  It is valid to compare
   the blocks PREV_BLOCK and CURR_BLOCK only when N_BYTES == BYTES_PER_BLOCK.
   That condition may be false only for the last input block.  */

સ્થિર શૂન્ય
write_block (intmax_t current_offset, idx_t n_bytes,
             અક્ષર અચલ *prev_block, અક્ષર અચલ *curr_block)
{
  સ્થિર bool first = true;
  સ્થિર bool prev_pair_equal = false;

  જો (abbreviate_duplicate_blocks
      && !first && n_bytes == bytes_per_block
      && memeq (prev_block, curr_block, bytes_per_block))
    {
      જો (prev_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      નહીંતર
        {
          printf ("*\n");
          prev_pair_equal = true;
        }
    }
  નહીંતર
    {
      prev_pair_equal = false;
      માટે (idx_t i = 0; i < n_specs; i++)
        {
          પૂર્ણાંક datum_width = width_bytes[spec[i].size];
          idx_t fields_per_block = bytes_per_block / datum_width;
          idx_t blank_fields = (bytes_per_block - n_bytes) / datum_width;
          જો (i == 0)
            format_address (current_offset, '\0');
          નહીંતર
            print_n_spaces (address_pad_len);
          (*spec[i].print_function) (fields_per_block, blank_fields,
                                     curr_block, spec[i].fmt_string,
                                     spec[i].field_width, spec[i].pad_width);
          જો (spec[i].hexl_mode_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              પૂર્ણાંક field_width = spec[i].field_width;
              માટે (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (field_width);
              idx_t pad_width = pad_at (fields_per_block, blank_fields,
                                        spec[i].pad_width);
              print_n_spaces (pad_width);
              dump_hexl_mode_trailer (n_bytes, curr_block);
            }
          putchar ('\n');
        }
    }
  first = false;
}

/* Read a single byte into *C from the concatenation of the input files
   named in the global array FILE_LIST.  On the first call to this
   function, the global variable IN_STREAM is expected to be an open
   stream associated with the input file INPUT_FILENAME.  If IN_STREAM
   is at end-of-file, close it and update the global variables IN_STREAM
   and INPUT_FILENAME so they correspond to the next file in the list.
   Then try to read a byte from the newly opened file.  Repeat if
   necessary until EOF is reached for the last file in FILE_LIST, then
   set *C to EOF and return.  Subsequent calls do likewise.  Return
   true if successful.  */

સ્થિર bool
read_char (પૂર્ણાંક *c)
{
  bool ok = true;

  *c = EOF;

  જ્યારે (in_stream && (*c = getc (in_stream)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_next_file ();
    }

  પાછા ok;
}

/* Read N bytes into BLOCK from the concatenation of the input files
   named in the global array FILE_LIST.  On the first call to this
   function, the global variable IN_STREAM is expected to be an open
   stream associated with the input file INPUT_FILENAME.  If all N
   bytes cannot be read from IN_STREAM, close IN_STREAM and update
   the global variables IN_STREAM and INPUT_FILENAME.  Then try to
   read the remaining bytes from the newly opened file.  Repeat if
   necessary until EOF is reached for the last file in FILE_LIST.
   On subsequent calls, don't modify BLOCK and return true.  Set
   *N_BYTES_IN_BUFFER to the number of bytes read.  If an error occurs,
   it will be detected through ferror when the stream is about to be
   closed.  If there is an error, give a message but continue reading
   as usual and return false.  Otherwise return true.  */

સ્થિર bool
read_block (idx_t n, અક્ષર *block, idx_t *n_bytes_in_buffer)
{
  bool ok = true;

  affirm (0 < n && n <= bytes_per_block);

  *n_bytes_in_buffer = 0;

  જ્યારે (in_stream != ખાલી)	/* EOF.  */
    {
      idx_t n_needed = n - *n_bytes_in_buffer;
      idx_t n_read = fread (block + *n_bytes_in_buffer,
                            1, n_needed, in_stream);

      *n_bytes_in_buffer += n_read;

      જો (n_read == n_needed)
        તોડો;

      ok &= check_and_close (errno);

      ok &= open_next_file ();
    }

  પાછા ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_PURE
સ્થિર પૂર્ણાંક
get_lcm (શૂન્ય)
{
  પૂર્ણાંક l_c_m = 1;

  માટે (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, width_bytes[spec[i].size]);
  પાછા l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
સ્થિર strtol_error
xstr2nonneg (અક્ષર અચલ *restrict nptr, પૂર્ણાંક base, intmax_t *val,
             અક્ષર અચલ *restrict valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, ખાલી, base, val, valid_suffixes);
  પાછા s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

સ્થિર bool
parse_old_offset (અક્ષર *str, intmax_t *offset)
{
  /* Skip over any leading '+'.  */
  અક્ષર *s = str + (str[0] == '+');

  જો (! c_isdigit (s[0]))
    પાછા false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  અક્ષર *dot = strchr (s, '.');
  જો (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = ખાલી;
  પૂર્ણાંક radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  જો (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  ગણના strtol_error s_err = xstr2nonneg (s, radix, offset, "bB");

  જો (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  જો (s_err == LONGINT_OVERFLOW)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  પાછા s_err == LONGINT_OK;
}

/* Read a chunk of size BYTES_PER_BLOCK from the input files, write the
   formatted block to standard output, and repeat until the specified
   maximum number of bytes has been read or until all input has been
   processed.  If the last block read is smaller than BYTES_PER_BLOCK
   and its size is not a multiple of the size associated with a format
   spec, extend the input block with zero bytes until its length is a
   multiple of all format spec sizes.  Write the final block.  Finally,
   write on a line by itself the offset of the byte after the last byte
   read.  Accumulate return values from calls to read_block and
   check_and_close, and if any was false, return false.
   Otherwise, return true.  */

સ્થિર bool
dump (શૂન્ય)
{
  અક્ષર *block[2];
  bool idx = false;
  bool ok = true;
  idx_t n_bytes_read;

  block[0] = xinmalloc (1 + abbreviate_duplicate_blocks, bytes_per_block);
  block[1] = block[0] + abbreviate_duplicate_blocks * bytes_per_block;

  intmax_t current_offset = n_bytes_to_skip;

  કરો
    {
      intmax_t needed_bound
        = end_offset < 0 ? INTMAX_MAX : end_offset - current_offset;
      જો (needed_bound <= 0)
        {
          n_bytes_read = 0;
          તોડો;
        }
      idx_t n_needed = MIN (bytes_per_block, needed_bound);
      ok &= read_block (n_needed, block[idx], &n_bytes_read);
      જો (n_bytes_read < bytes_per_block)
        તોડો;
      affirm (n_bytes_read == bytes_per_block);
      write_block (current_offset, n_bytes_read,
                   block[!idx], block[idx]);
      જો (ferror (stdout))
        ok = false;
      current_offset += n_bytes_read;
      idx = !idx;
    }
  જ્યારે (ok);

  જો (n_bytes_read > 0)
    {
      પૂર્ણાંક l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t bytes_to_write = (n_bytes_read + l_c_m - 1
                              - (n_bytes_read + l_c_m - 1) % l_c_m);

      memset (block[idx] + n_bytes_read, 0, bytes_to_write - n_bytes_read);
      write_block (current_offset, n_bytes_read, block[!idx], block[idx]);
      current_offset += n_bytes_read;
    }

  જો (ok || current_offset)
    format_address (current_offset, '\n');

  જો (0 <= end_offset && end_offset <= current_offset)
    ok &= check_and_close (0);

  મુક્ત (block[0]);

  પાછા ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

સ્થિર bool
dump_strings (શૂન્ય)
{
  idx_t bufsize = MAX (100, string_min + 1);
  અક્ષર *buf = ximalloc (bufsize);
  intmax_t address = n_bytes_to_skip;
  bool ok = true;

  જ્યારે (true)
    {
      idx_t i = 0;
      પૂર્ણાંક c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      જો (0 <= end_offset
          && (end_offset < string_min || end_offset - string_min < address))
        તોડો;

      /* Store consecutive printable characters to BUF.  */
      જ્યારે (! (0 <= end_offset && end_offset <= address))
        {
          જો (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, કદ *buf);
          ok &= read_char (&c);
          જો (c < 0)
            {
              મુક્ત (buf);
              પાછા ok;
            }
          address++;
          buf[i++] = c;
          જો (c == '\0')
            તોડો;		/* Print this string.  */
          જો (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              તોડો;
            }
        }

      જો (c < 0 || i - !c < string_min)
        ચાલુ;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_address (address - i, ' ');

      માટે (i = 0; (c = buf[i]); i++)
        {
          પસંદગી (c)
            {
            સ્થિતિ '\a':
              fputs ("\\a", stdout);
              તોડો;

            સ્થિતિ '\b':
              fputs ("\\b", stdout);
              તોડો;

            સ્થિતિ '\f':
              fputs ("\\f", stdout);
              તોડો;

            સ્થિતિ '\n':
              fputs ("\\n", stdout);
              તોડો;

            સ્થિતિ '\r':
              fputs ("\\r", stdout);
              તોડો;

            સ્થિતિ '\t':
              fputs ("\\t", stdout);
              તોડો;

            સ્થિતિ '\v':
              fputs ("\\v", stdout);
              તોડો;

            મૂળભૂત:
              putc (c, stdout);
            }
        }
      putchar ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  મુક્ત (buf);

  ok &= check_and_close (0);
  પાછા ok;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક n_files;
  પૂર્ણાંક l_c_m;
  idx_t desired_width = 0;
  bool modern = false;
  bool ok = true;
  idx_t width_per_block = 0;
  સ્થિર અક્ષર અચલ multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_bytes_to_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_start IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે (true)
    {
      intmax_t tmp;
      ગણના strtol_error s_err;
      પૂર્ણાંક oi = -1;
      પૂર્ણાંક c = getopt_long (argc, argv, short_options, long_options, &oi);
      જો (c == -1)
        તોડો;

      પસંદગી (c)
        {
        સ્થિતિ 'A':
          modern = true;
          પસંદગી (optarg[0])
            {
            સ્થિતિ 'd':
              format_address = format_address_std;
              address_base = 10;
              address_pad_len = 7;
              તોડો;
            સ્થિતિ 'o':
              format_address = format_address_std;
              address_base = 8;
              address_pad_len = 7;
              તોડો;
            સ્થિતિ 'x':
              format_address = format_address_std;
              address_base = 16;
              address_pad_len = 6;
              તોડો;
            સ્થિતિ 'n':
              format_address = format_address_none;
              address_pad_len = 0;
              તોડો;
            મૂળભૂત:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              તોડો;
            }
          તોડો;

        સ્થિતિ 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_bytes_to_skip, multipliers);
          જો (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          તોડો;

        સ્થિતિ 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_bytes_to_format, multipliers);
          જો (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          તોડો;

        સ્થિતિ 'S':
          modern = true;
          જો (optarg == ખાલી)
            string_min = 3;
          નહીંતર
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              જો (s_err == LONGINT_OK && ckd_add (&i, tmp, 1))
                s_err = LONGINT_OVERFLOW;
              જો (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_strings = true;
          તોડો;

        સ્થિતિ 't':
          modern = true;
          ok &= decode_format_string (optarg);
          તોડો;

        સ્થિતિ 'v':
          modern = true;
          abbreviate_duplicate_blocks = false;
          તોડો;

        સ્થિતિ TRADITIONAL_OPTION:
          traditional = true;
          તોડો;

        સ્થિતિ ENDIAN_OPTION:
          પસંદગી (XARGMATCH ("--endian", optarg, endian_args, endian_types))
            {
            સ્થિતિ endian_big:
              આગત_swap = BYTE_ORDER != BIG_ENDIAN;
              તોડો;
            સ્થિતિ endian_little:
              આગત_swap = BYTE_ORDER != LITTLE_ENDIAN;
              તોડો;
            }
          તોડો;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#define CASE_OLD_ARG(old_char,new_string)		\
        સ્થિતિ old_char:					\
          ok &= decode_format_string (new_string);	\
          તોડો

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        સ્થિતિ 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        સ્થિતિ 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        સ્થિતિ 'I': સ્થિતિ 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        સ્થિતિ 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        સ્થિતિ 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#undef CASE_OLD_ARG

        સ્થિતિ 'w':
          modern = true;
          જો (optarg == ખાલી)
            {
              desired_width = 32;
            }
          નહીંતર
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              જો (s_err == LONGINT_OK)
                {
                  જો (ckd_add (&desired_width, w_tmp, 0))
                    s_err = LONGINT_OVERFLOW;
                  નહીંતર જો (desired_width == 0)
                    s_err = LONGINT_INVALID;
                }
              જો (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
          તોડો;
        }
    }

  જો (!ok)
    પાછા EXIT_FAILURE;

  જો (flag_dump_strings && n_specs > 0)
    error (EXIT_FAILURE, 0,
           _("no type may be specified when dumping strings"));

  n_files = argc - optind;

  /* If the --traditional option is used, there may be from
     0 to 3 remaining command line arguments;  handle each case
     separately.
        od [file] [[+]offset[.][b] [[+]label[.][b]]]
     The offset and label have the same syntax.

     If --traditional is not given, and if no modern options are
     given, and if the offset begins with + or (if there are two
     operands) a digit, accept only this form, as per POSIX:
        od [file] [[+]offset[.][b]]
  */

  જો (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      પસંદગી (n_files)
        {
        સ્થિતિ 1:
          જો ((traditional || argv[optind][0] == '+')
              && parse_old_offset (argv[optind], &o1))
            {
              n_bytes_to_skip = o1;
              --n_files;
              ++argv;
            }
          તોડો;

        સ્થિતિ 2:
          જો ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_offset (argv[optind + 1], &o2))
            {
              જો (traditional && parse_old_offset (argv[optind], &o1))
                {
                  n_bytes_to_skip = o1;
                  flag_pseudo_start = true;
                  pseudo_start = o2;
                  argv += 2;
                  n_files -= 2;
                }
              નહીંતર
                {
                  n_bytes_to_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          તોડો;

        સ્થિતિ 3:
          જો (traditional
              && parse_old_offset (argv[optind + 1], &o1)
              && parse_old_offset (argv[optind + 2], &o2))
            {
              n_bytes_to_skip = o1;
              flag_pseudo_start = true;
              pseudo_start = o2;
              argv[optind + 2] = argv[optind];
              argv += 2;
              n_files -= 2;
            }
          તોડો;
        }

      જો (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  જો (flag_pseudo_start)
    {
      જો (format_address == format_address_none)
        {
          address_base = 8;
          address_pad_len = 7;
          format_address = format_address_paren;
        }
      નહીંતર
        format_address = format_address_label;
    }

  જો (0 <= max_bytes_to_format
      && ckd_add (&end_offset, n_bytes_to_skip, max_bytes_to_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  જો (n_specs == 0)
    decode_format_string ("oS");

  જો (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (અક્ષર અચલ *અચલ *) &argv[optind];
    }
  નહીંતર
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_next_file ();
  જો (in_stream == ખાલી)
    જાઓ cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_bytes_to_skip);
  જો (in_stream == ખાલી)
    જાઓ cleanup;

  pseudo_offset = (flag_pseudo_start ? pseudo_start - n_bytes_to_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  જો (desired_width != 0)
    {
      જો (desired_width % l_c_m == 0)
        bytes_per_block = desired_width;
      નહીંતર
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_width, l_c_m);
          bytes_per_block = l_c_m;
        }
    }
  નહીંતર
    {
      જો (l_c_m < DEFAULT_BYTES_PER_BLOCK)
        bytes_per_block = (DEFAULT_BYTES_PER_BLOCK
                           - DEFAULT_BYTES_PER_BLOCK % l_c_m);
      નહીંતર
        bytes_per_block = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  માટે (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      જો (pad_at_overflow (fields_per_block))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_width);
      idx_t block_width = (spec[i].field_width + 1) * fields_per_block;
      જો (width_per_block < block_width)
        width_per_block = block_width;
    }
  માટે (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      idx_t block_width = spec[i].field_width * fields_per_block;
      spec[i].pad_width = width_per_block - block_width;
    }

#ifdef DEBUG
  printf ("lcm=%d, width_per_block=%td\n", l_c_m, width_per_block);
  માટે (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      affirm (bytes_per_block % width_bytes[spec[i].size] == 0);
      affirm (1 <= spec[i].pad_width / fields_per_block);
      printf ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, width_bytes[spec[i].size],
              spec[i].field_width, spec[i].pad_width);
    }
#endif

  ok &= (flag_dump_strings ? dump_strings () : dump ());

cleanup:

  જો (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
