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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <endian.h>
#অন্তর্ভাব <float.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "ftoastr.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "stat-size.h"
#অন্তর্ভাব "xbinary-io.h"
#অন্তর্ভাব "xprintf.h"
#অন্তর্ভাব "xstrtol.h"
#অন্তর্ভাব "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "od"

#define AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#define DEFAULT_BYTES_PER_BLOCK 16

#যদি FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  প্রকারলেখ _Float16 float16;
#অন্যথা
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  প্রকারলেখ দশমিক float16;
#endif

#যদি BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  প্রকারলেখ __bf16 bfloat16;
#অন্যথা
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  প্রকারলেখ দশমিক bfloat16;
#endif

গণনা আকার_spec
  {
    NO_আকার = 0,
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
    N_আকার_SPECS
  };

গণনা output_format
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

গণনা { MAX_INTEGRAL_TYPE_WIDTH = UINTMAX_WIDTH };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
গণনা
  {
    FMT_BYTES_ALLOCATED =
      আকার "%*.%dlld" - আকার "%d" + INT_STRLEN_BOUND (পূর্ণসংখ্যা) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_WIDTH <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
প্রকারলেখ শূন্য (*print_function_type)
  (idx_t fields, idx_t blank, শূন্য ধ্রুবক *তথ্য,
   অক্ষর ধ্রুবক *fmt, পূর্ণসংখ্যা width, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
কাঠামো tspec
  {
    গণনা output_format fmt;
    গণনা আকার_spec আকার_2; /* Type of input object.  */
    print_function_type print_function;
    অক্ষর fmt_string[FMT_BYTES_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_mode_trailer;
    পূর্ণসংখ্যা field_width; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_width; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
স্থির ধ্রুবক পূর্ণসংখ্যা width_bytes[] =
{
  -1,
  আকার (unsigned অক্ষর),
#যদি UCHAR_MAX < USHRT_MAX
  আকার (unsigned short পূর্ণসংখ্যা),
#endif
#যদি USHRT_MAX < UINT_MAX
  আকার (unsigned পূর্ণসংখ্যা),
#endif
#যদি UINT_MAX < ULONG_MAX
  আকার (unsigned long পূর্ণসংখ্যা),
#endif
#যদি ULONG_MAX < ULLONG_MAX
  আকার (unsigned long long পূর্ণসংখ্যা),
#endif
#যদি ULLONG_MAX < UINTMAX_MAX
  আকার (uintmax_t),
#endif
#যদি BF16_SUPPORTED
  আকার (bfloat16),
#নতুবা_যদি FLOAT16_SUPPORTED
  আকার (float16),
#endif
#যদি FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  আকার (দশমিক),
#endif
  আকার (দ্বিগুণ),
#যদি DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  আকার (long দ্বিগুণ),
#endif
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (width_bytes) == N_আকার_SPECS);

/* Names for some non-printing characters.  */
স্থির অক্ষর ধ্রুবক charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
স্থির পূর্ণসংখ্যা address_base = 8;

/* The number of octal digits required to represent the largest
   address value.  */
গণনা { MAX_ADDRESS_দৈর্ঘ্য = ((INTMAX_WIDTH - 1) / 3
                             + ((INTMAX_WIDTH - 1) % 3 != 0)) };

/* Width of a normal address.  */
স্থির পূর্ণসংখ্যা address_pad_len = 7;

/* Minimum length when detecting --strings.  */
স্থির idx_t string_min;

/* True when in --strings mode.  */
স্থির bool flag_dump_strings;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
স্থির bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
স্থির bool flag_pseudo_start;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
স্থির intmax_t pseudo_offset;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
স্থির শূন্য format_address_std (intmax_t, অক্ষর);
স্থির শূন্য (*format_address) (intmax_t, অক্ষর) = format_address_std;

/* The number of input bytes to skip before formatting and writing.  */
স্থির intmax_t n_bytes_to_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
স্থির intmax_t end_offset = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
স্থির bool abbreviate_duplicate_blocks = true;

/* An array of specs describing how to format each input block.  */
স্থির কাঠামো tspec *spec;

/* The number of format specs.  */
স্থির idx_t n_specs;

/* The allocated length of SPEC.  */
স্থির idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
স্থির idx_t bytes_per_block;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
স্থির অক্ষর ধ্রুবক *প্রবেশ_filename;

/* A null-terminated list of the file-arguments from the command line.  */
স্থির অক্ষর ধ্রুবক *ধ্রুবক *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
স্থির অক্ষর ধ্রুবক *ধ্রুবক default_file_list[] = {"-", ফাঁকা};

/* The input stream associated with the current file.  */
স্থির FILE *in_stream;

/* If true, at least one of the files we read was standard input.  */
স্থির bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
স্থির গণনা আকার_spec ধ্রুবক integral_type_আকার[] =
  {
#যদি UCHAR_MAX < USHRT_MAX
    [আকার (unsigned অক্ষর)] = CHAR,
#endif
#যদি USHRT_MAX < UINT_MAX
    [আকার (unsigned short পূর্ণসংখ্যা)] = SHORT,
#endif
    [আকার (unsigned পূর্ণসংখ্যা)] = INT,
#যদি UINT_MAX < ULONG_MAX
    [আকার (unsigned long পূর্ণসংখ্যা)] = LONG,
#endif
#যদি ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [আকার (unsigned long long পূর্ণসংখ্যা)] = LONG_LONG,
#endif
#যদি ULONG_MAX < UINTMAX_MAX
    [আকার (uintmax_t)] = INTMAX,
#endif
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
স্থির গণনা আকার_spec ধ্রুবক fp_type_আকার[] =
  {
#যদি FLOAT16_SUPPORTED
    [আকার (float16)] = FLOAT_HALF,
#নতুবা_যদি BF16_SUPPORTED
    [আকার (bfloat16)] = FLOAT_HALF,
#endif
#যদি FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [আকার (দশমিক)] = FLOAT_SINGLE,
#endif
    [আকার (দ্বিগুণ)] = FLOAT_DOUBLE,
#যদি DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [আকার (long দ্বিগুণ)] = FLOAT_LONG_DOUBLE,
#endif
  };

/* Use native endianness by default.  */
স্থির bool প্রবেশ_swap;

স্থির অক্ষর ধ্রুবক short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

গণনা endian_type
{
  endian_little,
  endian_big
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক endian_args[] =
{
  "little", "big", ফাঁকা
};

স্থির গণনা endian_type ধ্রুবক endian_types[] =
{
  endian_little, endian_big
};

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"skip-bytes", required_argument, ফাঁকা, 'j'},
  {"address-radix", required_argument, ফাঁকা, 'A'},
  {"read-bytes", required_argument, ফাঁকা, 'N'},
  {"format", required_argument, ফাঁকা, 't'},
  {"output-duplicates", no_argument, ফাঁকা, 'v'},
  {"strings", optional_argument, ফাঁকা, 'S'},
  {"traditional", no_argument, ফাঁকা, TRADITIONAL_OPTION},
  {"width", optional_argument, ফাঁকা, 'w'},
  {"endian", required_argument, ফাঁকা, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]OFFSET[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]OFFSET[.][b] [+][LABEL][.][b]]\n\
"),
              program_name, program_name, program_name);
      fputs (_("\n\
Write an unambiguous representation, octal bytes by পূর্বনির্ধারিত,\n\
of FILE to standard output.  With more than one FILE argument,\n\
concatenate them in the listed order to form the প্রবেশ.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If first and second call formats both apply, the second format is assumed\n\
যদি the last operand begins with + or (যদি there are 2 operands) a digit.\n\
An OFFSET operand means -j OFFSET.  LABEL is the pseudo-address\n\
at first byte printed, incremented when dump is progressing.\n\
For OFFSET and LABEL, a 0x or 0X prefix indicates hexadecimal;\n\
suffixes may be . জন্য octal and b জন্য multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --address-radix=RADIX\n\
         output format জন্য file offsets;\n\
         RADIX is one of [doxn], জন্য Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         swap প্রবেশ bytes according the specified order\n\
"));
      oputs (_("\
  -j, --skip-bytes=BYTES\n\
         skip BYTES প্রবেশ bytes first\n\
"));
      oputs (_("\
  -N, --পঠ-bytes=BYTES\n\
         limit dump to BYTES প্রবেশ bytes\n\
"));
      oputs (_("\
  -S BYTES, --strings[=BYTES]\n\
         show only NUL terminated strings\n\
         of at least BYTES (পূর্বনির্ধারিত 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         করো not use * to mark সারি suppression\n\
"));
      oputs (_("\
  -w[BYTES], --width[=BYTES]\n\
         output BYTES bytes per output সারি;\n\
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
  -a   same as -t a,  select named characters, ignoring উপরের-order bit\n\
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
  a          named character, ignoring উপরের-order bit\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[আকার_3]    signed decimal, আকার_3 bytes per integer\n\
  f[আকার_3]    floating point, আকার_3 bytes per দশমিক\n\
  o[আকার_3]    octal, আকার_3 bytes per integer\n\
  u[আকার_3]    unsigned decimal, আকার_3 bytes per integer\n\
  x[আকার_3]    hexadecimal, আকার_3 bytes per integer\n\
"), stdout);
      fputs (_("\
\n\
আকার_3 is a সংখ্যা.  For TYPE in [doux], আকার_3 may also be C জন্য\n\
আকার(অক্ষর), S জন্য আকার(short), I জন্য আকার(পূর্ণসংখ্যা) or L জন্য\n\
আকার(long).  If TYPE is f, আকার_3 may also be B জন্য Brain 16 bit,\n\
H জন্য Half precision দশমিক, F জন্য আকার(দশমিক), D জন্য আকার(দ্বিগুণ),\n\
or L জন্য আকার(long দ্বিগুণ).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix to any type displays printable characters at the end of\n\
each output সারি.\n\
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
and so on জন্য G, T, P, E, Z, Y, R, Q.\n\
দ্বিমিক_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
স্থির শূন্য
print_n_spaces (intmax_t n)
{
  জন্য (; 0 < n; n--)
    putchar (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
স্থির idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  ফেরত pad / fields * i + m * i / fields;
}
স্থির bool
pad_at_overflow (idx_t fields)
{
  intmax_t product;
  ফেরত ckd_mul (&product, fields - 1, fields - 1);
}

#define PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
স্থির শূন্য                                                             \
N (idx_t fields, idx_t blank, শূন্য ধ্রুবক *block,			\
   FMT_STRING_DECL, পূর্ণসংখ্যা width, idx_t pad)				\
{                                                                       \
  T ধ্রুবক *p = block;                                                   \
  idx_t pad_remaining = pad;						\
  জন্য (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t পরবর্তী_pad = pad_at (fields, i - 1, pad);			\
      পূর্ণসংখ্যা adjusted_width = pad_remaining - পরবর্তী_pad + width;            \
      T x;                                                              \
      যদি (প্রবেশ_swap && আকার (T) > 1)                                 \
        {                                                               \
          সংঘ {                                                       \
            T x;                                                        \
            অক্ষর b[আকার (T)];                                         \
          } u;                                                          \
          জন্য (idx_t j = 0; j < আকার (T); j++)			\
            u.b[j] = ((অক্ষর ধ্রুবক *) p)[আকার (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      অন্যথা                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_remaining = পরবর্তী_pad;                                         \
    }                                                                   \
}

#define PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, অক্ষর ধ্রুবক *fmt_string,                           \
                xprintf (fmt_string, adjusted_width, x))

#define PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_UNUSED অক্ষর ধ্রুবক *fmt_string,              \
                অক্ষর buf[BUFSIZE];                                      \
                FTOASTR (buf, আকার buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_width, buf))

PRINT_TYPE (print_s_char, signed অক্ষর)
PRINT_TYPE (print_char, unsigned অক্ষর)
PRINT_TYPE (print_s_short, short পূর্ণসংখ্যা)
PRINT_TYPE (print_short, unsigned short পূর্ণসংখ্যা)
PRINT_TYPE (print_int, unsigned পূর্ণসংখ্যা)
PRINT_TYPE (print_long, unsigned long পূর্ণসংখ্যা)
PRINT_TYPE (print_long_long, unsigned long long পূর্ণসংখ্যা)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, দশমিক, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, দ্বিগুণ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, long দ্বিগুণ, ldtoastr, LDBL_BUFSIZE_BOUND)

#undef PRINT_TYPE
#undef PRINT_FLOATTYPE

স্থির শূন্য
dump_hexl_mode_trailer (idx_t n_bytes, অক্ষর ধ্রুবক *block)
{
  fputs ("  >", stdout);
  জন্য (idx_t i = n_bytes; i > 0; i--)
    {
      unsigned অক্ষর c = *block++;
      unsigned অক্ষর c2 = (isprint (c) ? c : '.');
      putchar (c2);
    }
  putchar ('<');
}

স্থির শূন্য
print_named_ascii (idx_t fields, idx_t blank, শূন্য ধ্রুবক *block,
                   MAYBE_UNUSED অক্ষর ধ্রুবক *unused_fmt_string,
                   পূর্ণসংখ্যা width, idx_t pad)
{
  unsigned অক্ষর ধ্রুবক *p = block;
  idx_t pad_remaining = pad;
  জন্য (idx_t i = fields; blank < i; i--)
    {
      পূর্ণসংখ্যা masked_c = *p++ & 0x7f;
      অক্ষর ধ্রুবক *s;
      অক্ষর buf[2];

      যদি (masked_c == 127)
        s = "del";
      অন্যথা যদি (masked_c <= 040)
        s = charname[masked_c];
      অন্যথা
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t পরবর্তী_pad = pad_at (fields, i - 1, pad);
      পূর্ণসংখ্যা adjusted_width = pad_remaining - পরবর্তী_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = পরবর্তী_pad;
    }
}

স্থির শূন্য
print_ascii (idx_t fields, idx_t blank, শূন্য ধ্রুবক *block,
             MAYBE_UNUSED অক্ষর ধ্রুবক *unused_fmt_string, পূর্ণসংখ্যা width,
             idx_t pad)
{
  unsigned অক্ষর ধ্রুবক *p = block;
  idx_t pad_remaining = pad;
  জন্য (idx_t i = fields; blank < i; i--)
    {
      unsigned অক্ষর c = *p++;
      অক্ষর ধ্রুবক *s;
      অক্ষর buf[4];

      নির্বাচন (c)
        {
        ক্ষেত্র '\0':
          s = "\\0";
          বিরতি;

        ক্ষেত্র '\a':
          s = "\\a";
          বিরতি;

        ক্ষেত্র '\b':
          s = "\\b";
          বিরতি;

        ক্ষেত্র '\f':
          s = "\\f";
          বিরতি;

        ক্ষেত্র '\n':
          s = "\\n";
          বিরতি;

        ক্ষেত্র '\r':
          s = "\\r";
          বিরতি;

        ক্ষেত্র '\t':
          s = "\\t";
          বিরতি;

        ক্ষেত্র '\v':
          s = "\\v";
          বিরতি;

        পূর্বনির্ধারিত:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t পরবর্তী_pad = pad_at (fields, i - 1, pad);
      পূর্ণসংখ্যা adjusted_width = pad_remaining - পরবর্তী_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = পরবর্তী_pad;
    }
}

/* Convert a null-terminated (possibly zero-length) string S to an
   int value.  If S points to a non-digit set *P to S,
   *VAL to 0, and return true.  Otherwise, accumulate the integer value of
   the string of digits.  If the string of digits represents a value
   larger than INT_MAX, don't modify *VAL or *P and return false.
   Otherwise, advance *P to the first non-digit after S, set *VAL to
   the result of the conversion and return true.  */

স্থির bool
simple_strtoi (অক্ষর ধ্রুবক *s, অক্ষর ধ্রুবক **p, পূর্ণসংখ্যা *val)
{
  পূর্ণসংখ্যা যোগ;

  জন্য (যোগ = 0; c_isdigit (*s); s++)
    যদি (ckd_mul (&যোগ, যোগ, 10) || ckd_add (&যোগ, যোগ, *s - '0'))
      ফেরত false;
  *p = s;
  *val = যোগ;
  ফেরত true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
স্থির অক্ষর ধ্রুবক *
ispec_to_format (গণনা আকার_spec আকার_spec,
                 অক্ষর ধ্রুবক *int_format,
                 অক্ষর ধ্রুবক *long_format,
                 অক্ষর ধ্রুবক *long_long_format,
                 অক্ষর ধ্রুবক *intmax_format)
{
  ফেরত (LONG < INTMAX && আকার_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && আকার_spec == LONG_LONG ? long_long_format
          : INT < LONG && আকার_spec == LONG ? long_format
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

স্থির bool ATTRIBUTE_NONNULL ()
decode_one_format (অক্ষর ধ্রুবক *s_orig, অক্ষর ধ্রুবক *s, অক্ষর ধ্রুবক **পরবর্তী,
                   কাঠামো tspec *tspec)
{
  গণনা আকার_spec আকার_spec;
  পূর্ণসংখ্যা আকার_2;
  গণনা output_format fmt;
  print_function_type print_function;
  অক্ষর ধ্রুবক *p;
  অক্ষর c;
  পূর্ণসংখ্যা field_width;

  নির্বাচন (*s)
    {
    ক্ষেত্র 'd':
    ক্ষেত্র 'o':
    ক্ষেত্র 'u':
    ক্ষেত্র 'x':
      c = *s;
      ++s;
      নির্বাচন (*s)
        {
        ক্ষেত্র 'C':
          ++s;
          আকার_2 = আকার (unsigned অক্ষর);
          বিরতি;

        ক্ষেত্র 'S':
          ++s;
          আকার_2 = আকার (unsigned short পূর্ণসংখ্যা);
          বিরতি;

        ক্ষেত্র 'I':
          ++s;
          আকার_2 = আকার (unsigned পূর্ণসংখ্যা);
          বিরতি;

        ক্ষেত্র 'L':
          ++s;
          আকার_2 = আকার (unsigned long পূর্ণসংখ্যা);
          বিরতি;

        পূর্বনির্ধারিত:
          যদি (! simple_strtoi (s, &p, &আকার_2))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              ফেরত false;
            }
          যদি (p == s)
            আকার_2 = আকার (unsigned পূর্ণসংখ্যা);
          অন্যথা
            {
              যদি (countof (integral_type_আকার) <= আকার_2
                  || integral_type_আকার[আকার_2] == NO_আকার)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), আকার_2);
                  ফেরত false;
                }
              s = p;
            }
          বিরতি;
        }

      আকার_spec = integral_type_আকার[আকার_2];

      নির্বাচন (c)
        {
        ক্ষেত্র 'd':
          fmt = SIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * আকার_2 - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (আকার_spec, "d", "ld", "lld", "jd"));
          বিরতি;

        ক্ষেত্র 'o':
          fmt = OCTAL;
          field_width = (CHAR_BIT * আকার_2 + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (আকার_spec, "o", "lo", "llo", "jo"));
          বিরতি;

        ক্ষেত্র 'u':
          fmt = UNSIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * আকার_2);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (আকার_spec, "u", "lu", "llu", "ju"));
          বিরতি;

        ক্ষেত্র 'x':
          fmt = HEXADECIMAL;
          field_width = (CHAR_BIT * আকার_2 + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (আকার_spec, "x", "lx", "llx", "jx"));
          বিরতি;

        পূর্বনির্ধারিত:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (আকার_spec == INT
           ? print_int
           : SHORT < INT && আকার_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && আকার_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && আকার_spec == LONG
           ? print_long
           : LONG < INTMAX && আকার_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && আকার_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) ফাঁকা));
      বিরতি;

    ক্ষেত্র 'f':
      fmt = FLOATING_POINT;
      ++s;
      নির্বাচন (*s)
        {
        ক্ষেত্র 'B':
          ++s;
          fmt = BFLOATING_POINT;
          আকার_2 = আকার (bfloat16);
          বিরতি;

        ক্ষেত্র 'H':
          ++s;
          fmt = HFLOATING_POINT;
          আকার_2 = আকার (float16);
          বিরতি;

        ক্ষেত্র 'F':
          ++s;
          আকার_2 = আকার (দশমিক);
          বিরতি;

        ক্ষেত্র 'D':
          ++s;
          আকার_2 = আকার (দ্বিগুণ);
          বিরতি;

        ক্ষেত্র 'L':
          ++s;
          আকার_2 = আকার (long দ্বিগুণ);
          বিরতি;

        পূর্বনির্ধারিত:
          যদি (! simple_strtoi (s, &p, &আকার_2))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              ফেরত false;
            }
          যদি (p == s)
            আকার_2 = আকার (দ্বিগুণ);
          অন্যথা
            {
              যদি (countof (fp_type_আকার) <= আকার_2
                  || fp_type_আকার[আকার_2] == NO_আকার
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && আকার_2 == আকার (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), আকার_2);
                  ফেরত false;
                }
              s = p;
            }
          বিরতি;
        }
      আকার_spec = fp_type_আকার[আকার_2];

      যদি ((! FLOAT16_SUPPORTED && fmt == HFLOATING_POINT)
          || (! BF16_SUPPORTED && fmt == BFLOATING_POINT))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        ফেরত false;
      }

      {
        কাঠামো lconv ধ্রুবক *locale = localeconv ();
        idx_t decimal_point_len =
          (locale->decimal_point[0] ? strlen (locale->decimal_point) : 1);

        যদি (আকার_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            field_width = DBL_STRLEN_BOUND_L (decimal_point_len);
          }
        অন্যথা যদি (FLOAT_SINGLE < FLOAT_DOUBLE && আকার_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        অন্যথা যদি (FLOAT_HALF < FLOAT_SINGLE && আকার_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_POINT
                             ? print_bfloat : print_halffloat;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        অন্যথা যদি (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && আকার_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            field_width = LDBL_STRLEN_BOUND_L (decimal_point_len);
          }
        অন্যথা
          affirm (false);

        বিরতি;
      }

    ক্ষেত্র 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      আকার_spec = CHAR;
      print_function = print_named_ascii;
      field_width = 3;
      বিরতি;

    ক্ষেত্র 'c':
      ++s;
      fmt = CHARACTER;
      আকার_spec = CHAR;
      print_function = print_ascii;
      field_width = 3;
      বিরতি;

    পূর্বনির্ধারিত:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      ফেরত false;
    }

  tspec->আকার_2 = আকার_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->field_width = field_width;
  tspec->hexl_mode_trailer = (*s == 'z');
  যদি (tspec->hexl_mode_trailer)
    s++;

  *পরবর্তী = s;
  ফেরত true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

স্থির bool
open_পরবর্তী_file (শূন্য)
{
  bool ok = true;

  করো
    {
      প্রবেশ_filename = *file_list;
      যদি (প্রবেশ_filename == ফাঁকা)
        ফেরত ok;
      ++file_list;

      যদি (streq (প্রবেশ_filename, "-"))
        {
          প্রবেশ_filename = _("standard input");
          in_stream = stdin;
          have_read_stdin = true;
          xset_দ্বিমিক_mode (STDIN_FILENO, O_দ্বিমিক);
        }
      অন্যথা
        {
          in_stream = fopen (প্রবেশ_filename, (O_দ্বিমিক ? "rb" : "r"));
          যদি (in_stream == ফাঁকা)
            {
              error (0, errno, "%s", quotef (প্রবেশ_filename));
              ok = false;
            }
        }
    }
  যখন (in_stream == ফাঁকা);

  যদি (0 <= end_offset && !flag_dump_strings)
    setvbuf (in_stream, ফাঁকা, _IONBF, 0);

  ফেরত ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

স্থির bool
check_and_close (পূর্ণসংখ্যা in_errno)
{
  bool ok = true;

  যদি (in_stream != ফাঁকা)
    {
      যদি (!ferror (in_stream))
        in_errno = 0;
      যদি (streq (file_list[-1], "-"))
        clearerr (in_stream);
      অন্যথা যদি (fclose (in_stream) != 0 && !in_errno)
        in_errno = errno;
      যদি (in_errno)
        {
          error (0, in_errno, "%s", quotef (প্রবেশ_filename));
          ok = false;
        }

      in_stream = ফাঁকা;
    }

  যদি (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  ফেরত ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

স্থির bool ATTRIBUTE_NONNULL ()
decode_format_string (অক্ষর ধ্রুবক *s)
{
  অক্ষর ধ্রুবক *s_orig = s;

  যখন (*s != '\0')
    {
      অক্ষর ধ্রুবক *পরবর্তী;

      যদি (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, আকার *spec);

      যদি (! decode_one_format (s_orig, s, &পরবর্তী, &spec[n_specs]))
        ফেরত false;

      affirm (s != পরবর্তী);
      s = পরবর্তী;
      ++n_specs;
    }

  ফেরত true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

স্থির bool
skip (intmax_t n_skip)
{
  bool ok = true;
  পূর্ণসংখ্যা in_errno = 0;

  যদি (n_skip == 0)
    ফেরত true;

  যখন (in_stream != ফাঁকা)	/* EOF.  */
    {
      কাঠামো stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      যদি (fstat (fileno (in_stream), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          যদি (S_ISREG (file_stats.st_mode)
              && STP_BLKSIZE (&file_stats) < file_stats.st_আকার)
            {
              যদি (file_stats.st_আকার < n_skip)
                n_skip -= file_stats.st_আকার;
              অন্যথা
                {
                  যদি (fseeko (in_stream, n_skip, SEEK_CUR) != 0)
                    {
                      in_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          অন্যথা যদি (! (S_ISREG (file_stats.st_mode)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (in_stream, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          অন্যথা
            {
              অক্ষর buf[BUFSIZ];
              idx_t n_bytes_read, n_bytes_to_read = BUFSIZ;

              যখন (0 < n_skip)
                {
                  যদি (n_skip < n_bytes_to_read)
                    n_bytes_to_read = n_skip;
                  n_bytes_read = fread (buf, 1, n_bytes_to_read, in_stream);
                  n_skip -= n_bytes_read;
                  যদি (n_bytes_read != n_bytes_to_read)
                    {
                      যদি (ferror (in_stream))
                        {
                          in_errno = errno;
                          ok = false;
                          n_skip = 0;
                          বিরতি;
                        }
                      যদি (feof (in_stream))
                        বিরতি;
                    }
                }
            }

          যদি (n_skip == 0)
            বিরতি;
        }

      অন্যথা   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (প্রবেশ_filename));
          ok = false;
        }

      ok &= check_and_close (in_errno);

      ok &= open_পরবর্তী_file ();
    }

  যদি (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  ফেরত ok;
}

স্থির শূন্য
format_address_none (MAYBE_UNUSED intmax_t address,
                     MAYBE_UNUSED অক্ষর c)
{
}

স্থির শূন্য
format_address_std (intmax_t address, অক্ষর c)
{
  অক্ষর buf[MAX_ADDRESS_দৈর্ঘ্য + 2];
  অক্ষর *p = buf + আকার buf;
  অক্ষর ধ্রুবক *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - address_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  নির্বাচন (address_base)
    {
    ক্ষেত্র 8:
      করো
        *--p = '0' + (address & 7);
      যখন ((address >>= 3) != 0);
      বিরতি;

    ক্ষেত্র 10:
      করো
        *--p = '0' + (address % 10);
      যখন ((address /= 10) != 0);
      বিরতি;

    ক্ষেত্র 16:
      করো
        *--p = "0123456789abcdef"[address & 15];
      যখন ((address >>= 4) != 0);
      বিরতি;
    }

  যখন (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

স্থির শূন্য
format_address_paren (intmax_t address, অক্ষর c)
{
  putchar ('(');
  format_address_std (address, ')');
  যদি (c)
    putchar (c);
}

স্থির শূন্য
format_address_label (intmax_t address, অক্ষর c)
{
  format_address_std (address, ' ');

  intmax_t addr;
  যদি (ckd_add (&addr, address, pseudo_offset))
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

স্থির শূন্য
write_block (intmax_t current_offset, idx_t n_bytes,
             অক্ষর ধ্রুবক *পূর্ব_block, অক্ষর ধ্রুবক *curr_block)
{
  স্থির bool first = true;
  স্থির bool পূর্ব_pair_equal = false;

  যদি (abbreviate_duplicate_blocks
      && !first && n_bytes == bytes_per_block
      && memeq (পূর্ব_block, curr_block, bytes_per_block))
    {
      যদি (পূর্ব_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      অন্যথা
        {
          printf ("*\n");
          পূর্ব_pair_equal = true;
        }
    }
  অন্যথা
    {
      পূর্ব_pair_equal = false;
      জন্য (idx_t i = 0; i < n_specs; i++)
        {
          পূর্ণসংখ্যা datum_width = width_bytes[spec[i].আকার_2];
          idx_t fields_per_block = bytes_per_block / datum_width;
          idx_t blank_fields = (bytes_per_block - n_bytes) / datum_width;
          যদি (i == 0)
            format_address (current_offset, '\0');
          অন্যথা
            print_n_spaces (address_pad_len);
          (*spec[i].print_function) (fields_per_block, blank_fields,
                                     curr_block, spec[i].fmt_string,
                                     spec[i].field_width, spec[i].pad_width);
          যদি (spec[i].hexl_mode_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              পূর্ণসংখ্যা field_width = spec[i].field_width;
              জন্য (idx_t f = 0; f < blank_fields; f++)
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

স্থির bool
read_char (পূর্ণসংখ্যা *c)
{
  bool ok = true;

  *c = EOF;

  যখন (in_stream && (*c = getc (in_stream)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_পরবর্তী_file ();
    }

  ফেরত ok;
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

স্থির bool
read_block (idx_t n, অক্ষর *block, idx_t *n_bytes_in_বাফার)
{
  bool ok = true;

  affirm (0 < n && n <= bytes_per_block);

  *n_bytes_in_বাফার = 0;

  যখন (in_stream != ফাঁকা)	/* EOF.  */
    {
      idx_t n_needed = n - *n_bytes_in_বাফার;
      idx_t n_read = fread (block + *n_bytes_in_বাফার,
                            1, n_needed, in_stream);

      *n_bytes_in_বাফার += n_read;

      যদি (n_read == n_needed)
        বিরতি;

      ok &= check_and_close (errno);

      ok &= open_পরবর্তী_file ();
    }

  ফেরত ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_PURE
স্থির পূর্ণসংখ্যা
get_lcm (শূন্য)
{
  পূর্ণসংখ্যা l_c_m = 1;

  জন্য (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, width_bytes[spec[i].আকার_2]);
  ফেরত l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
স্থির strtol_error
xstr2nonneg (অক্ষর ধ্রুবক *restrict nptr, পূর্ণসংখ্যা base, intmax_t *val,
             অক্ষর ধ্রুবক *restrict valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, ফাঁকা, base, val, valid_suffixes);
  ফেরত s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

স্থির bool
parse_old_offset (অক্ষর *str, intmax_t *offset)
{
  /* Skip over any leading '+'.  */
  অক্ষর *s = str + (str[0] == '+');

  যদি (! c_isdigit (s[0]))
    ফেরত false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  অক্ষর *dot = strchr (s, '.');
  যদি (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = ফাঁকা;
  পূর্ণসংখ্যা radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  যদি (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  গণনা strtol_error s_err = xstr2nonneg (s, radix, offset, "bB");

  যদি (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  যদি (s_err == LONGINT_OVERFLOW)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  ফেরত s_err == LONGINT_OK;
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

স্থির bool
dump (শূন্য)
{
  অক্ষর *block[2];
  bool idx = false;
  bool ok = true;
  idx_t n_bytes_read;

  block[0] = xinmalloc (1 + abbreviate_duplicate_blocks, bytes_per_block);
  block[1] = block[0] + abbreviate_duplicate_blocks * bytes_per_block;

  intmax_t current_offset = n_bytes_to_skip;

  করো
    {
      intmax_t needed_bound
        = end_offset < 0 ? INTMAX_MAX : end_offset - current_offset;
      যদি (needed_bound <= 0)
        {
          n_bytes_read = 0;
          বিরতি;
        }
      idx_t n_needed = MIN (bytes_per_block, needed_bound);
      ok &= read_block (n_needed, block[idx], &n_bytes_read);
      যদি (n_bytes_read < bytes_per_block)
        বিরতি;
      affirm (n_bytes_read == bytes_per_block);
      write_block (current_offset, n_bytes_read,
                   block[!idx], block[idx]);
      যদি (ferror (stdout))
        ok = false;
      current_offset += n_bytes_read;
      idx = !idx;
    }
  যখন (ok);

  যদি (n_bytes_read > 0)
    {
      পূর্ণসংখ্যা l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t bytes_to_write = (n_bytes_read + l_c_m - 1
                              - (n_bytes_read + l_c_m - 1) % l_c_m);

      memset (block[idx] + n_bytes_read, 0, bytes_to_write - n_bytes_read);
      write_block (current_offset, n_bytes_read, block[!idx], block[idx]);
      current_offset += n_bytes_read;
    }

  যদি (ok || current_offset)
    format_address (current_offset, '\n');

  যদি (0 <= end_offset && end_offset <= current_offset)
    ok &= check_and_close (0);

  মুক্ত (block[0]);

  ফেরত ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

স্থির bool
dump_strings (শূন্য)
{
  idx_t bufsize = MAX (100, string_min + 1);
  অক্ষর *buf = ximalloc (bufsize);
  intmax_t address = n_bytes_to_skip;
  bool ok = true;

  যখন (true)
    {
      idx_t i = 0;
      পূর্ণসংখ্যা c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      যদি (0 <= end_offset
          && (end_offset < string_min || end_offset - string_min < address))
        বিরতি;

      /* Store consecutive printable characters to BUF.  */
      যখন (! (0 <= end_offset && end_offset <= address))
        {
          যদি (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, আকার *buf);
          ok &= read_char (&c);
          যদি (c < 0)
            {
              মুক্ত (buf);
              ফেরত ok;
            }
          address++;
          buf[i++] = c;
          যদি (c == '\0')
            বিরতি;		/* Print this string.  */
          যদি (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              বিরতি;
            }
        }

      যদি (c < 0 || i - !c < string_min)
        চালু;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_address (address - i, ' ');

      জন্য (i = 0; (c = buf[i]); i++)
        {
          নির্বাচন (c)
            {
            ক্ষেত্র '\a':
              fputs ("\\a", stdout);
              বিরতি;

            ক্ষেত্র '\b':
              fputs ("\\b", stdout);
              বিরতি;

            ক্ষেত্র '\f':
              fputs ("\\f", stdout);
              বিরতি;

            ক্ষেত্র '\n':
              fputs ("\\n", stdout);
              বিরতি;

            ক্ষেত্র '\r':
              fputs ("\\r", stdout);
              বিরতি;

            ক্ষেত্র '\t':
              fputs ("\\t", stdout);
              বিরতি;

            ক্ষেত্র '\v':
              fputs ("\\v", stdout);
              বিরতি;

            পূর্বনির্ধারিত:
              putc (c, stdout);
            }
        }
      putchar ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  মুক্ত (buf);

  ok &= check_and_close (0);
  ফেরত ok;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা n_files;
  পূর্ণসংখ্যা l_c_m;
  idx_t desired_width = 0;
  bool modern = false;
  bool ok = true;
  idx_t width_per_block = 0;
  স্থির অক্ষর ধ্রুবক multipliers[] = "bEGKkMmPQRTYZ0";

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

  যখন (true)
    {
      intmax_t tmp;
      গণনা strtol_error s_err;
      পূর্ণসংখ্যা oi = -1;
      পূর্ণসংখ্যা c = getopt_long (argc, argv, short_options, long_options, &oi);
      যদি (c == -1)
        বিরতি;

      নির্বাচন (c)
        {
        ক্ষেত্র 'A':
          modern = true;
          নির্বাচন (optarg[0])
            {
            ক্ষেত্র 'd':
              format_address = format_address_std;
              address_base = 10;
              address_pad_len = 7;
              বিরতি;
            ক্ষেত্র 'o':
              format_address = format_address_std;
              address_base = 8;
              address_pad_len = 7;
              বিরতি;
            ক্ষেত্র 'x':
              format_address = format_address_std;
              address_base = 16;
              address_pad_len = 6;
              বিরতি;
            ক্ষেত্র 'n':
              format_address = format_address_none;
              address_pad_len = 0;
              বিরতি;
            পূর্বনির্ধারিত:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              বিরতি;
            }
          বিরতি;

        ক্ষেত্র 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_bytes_to_skip, multipliers);
          যদি (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          বিরতি;

        ক্ষেত্র 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_bytes_to_format, multipliers);
          যদি (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          বিরতি;

        ক্ষেত্র 'S':
          modern = true;
          যদি (optarg == ফাঁকা)
            string_min = 3;
          অন্যথা
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              যদি (s_err == LONGINT_OK && ckd_add (&i, tmp, 1))
                s_err = LONGINT_OVERFLOW;
              যদি (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_strings = true;
          বিরতি;

        ক্ষেত্র 't':
          modern = true;
          ok &= decode_format_string (optarg);
          বিরতি;

        ক্ষেত্র 'v':
          modern = true;
          abbreviate_duplicate_blocks = false;
          বিরতি;

        ক্ষেত্র TRADITIONAL_OPTION:
          traditional = true;
          বিরতি;

        ক্ষেত্র ENDIAN_OPTION:
          নির্বাচন (XARGMATCH ("--endian", optarg, endian_args, endian_types))
            {
            ক্ষেত্র endian_big:
              প্রবেশ_swap = BYTE_ORDER != BIG_ENDIAN;
              বিরতি;
            ক্ষেত্র endian_little:
              প্রবেশ_swap = BYTE_ORDER != LITTLE_ENDIAN;
              বিরতি;
            }
          বিরতি;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#define CASE_OLD_ARG(old_char,new_string)		\
        ক্ষেত্র old_char:					\
          ok &= decode_format_string (new_string);	\
          বিরতি

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        ক্ষেত্র 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        ক্ষেত্র 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        ক্ষেত্র 'I': ক্ষেত্র 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        ক্ষেত্র 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        ক্ষেত্র 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#undef CASE_OLD_ARG

        ক্ষেত্র 'w':
          modern = true;
          যদি (optarg == ফাঁকা)
            {
              desired_width = 32;
            }
          অন্যথা
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              যদি (s_err == LONGINT_OK)
                {
                  যদি (ckd_add (&desired_width, w_tmp, 0))
                    s_err = LONGINT_OVERFLOW;
                  অন্যথা যদি (desired_width == 0)
                    s_err = LONGINT_INVALID;
                }
              যদি (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
          বিরতি;
        }
    }

  যদি (!ok)
    ফেরত EXIT_FAILURE;

  যদি (flag_dump_strings && n_specs > 0)
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

  যদি (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      নির্বাচন (n_files)
        {
        ক্ষেত্র 1:
          যদি ((traditional || argv[optind][0] == '+')
              && parse_old_offset (argv[optind], &o1))
            {
              n_bytes_to_skip = o1;
              --n_files;
              ++argv;
            }
          বিরতি;

        ক্ষেত্র 2:
          যদি ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_offset (argv[optind + 1], &o2))
            {
              যদি (traditional && parse_old_offset (argv[optind], &o1))
                {
                  n_bytes_to_skip = o1;
                  flag_pseudo_start = true;
                  pseudo_start = o2;
                  argv += 2;
                  n_files -= 2;
                }
              অন্যথা
                {
                  n_bytes_to_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          বিরতি;

        ক্ষেত্র 3:
          যদি (traditional
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
          বিরতি;
        }

      যদি (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  যদি (flag_pseudo_start)
    {
      যদি (format_address == format_address_none)
        {
          address_base = 8;
          address_pad_len = 7;
          format_address = format_address_paren;
        }
      অন্যথা
        format_address = format_address_label;
    }

  যদি (0 <= max_bytes_to_format
      && ckd_add (&end_offset, n_bytes_to_skip, max_bytes_to_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  যদি (n_specs == 0)
    decode_format_string ("oS");

  যদি (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (অক্ষর ধ্রুবক *ধ্রুবক *) &argv[optind];
    }
  অন্যথা
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_পরবর্তী_file ();
  যদি (in_stream == ফাঁকা)
    যাও cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_bytes_to_skip);
  যদি (in_stream == ফাঁকা)
    যাও cleanup;

  pseudo_offset = (flag_pseudo_start ? pseudo_start - n_bytes_to_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  যদি (desired_width != 0)
    {
      যদি (desired_width % l_c_m == 0)
        bytes_per_block = desired_width;
      অন্যথা
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_width, l_c_m);
          bytes_per_block = l_c_m;
        }
    }
  অন্যথা
    {
      যদি (l_c_m < DEFAULT_BYTES_PER_BLOCK)
        bytes_per_block = (DEFAULT_BYTES_PER_BLOCK
                           - DEFAULT_BYTES_PER_BLOCK % l_c_m);
      অন্যথা
        bytes_per_block = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  জন্য (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].আকার_2];
      যদি (pad_at_overflow (fields_per_block))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_width);
      idx_t block_width = (spec[i].field_width + 1) * fields_per_block;
      যদি (width_per_block < block_width)
        width_per_block = block_width;
    }
  জন্য (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].আকার_2];
      idx_t block_width = spec[i].field_width * fields_per_block;
      spec[i].pad_width = width_per_block - block_width;
    }

#ifdef DEBUG
  printf ("lcm=%d, width_per_block=%td\n", l_c_m, width_per_block);
  জন্য (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].আকার_2];
      affirm (bytes_per_block % width_bytes[spec[i].আকার_2] == 0);
      affirm (1 <= spec[i].pad_width / fields_per_block);
      printf ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, width_bytes[spec[i].আকার_2],
              spec[i].field_width, spec[i].pad_width);
    }
#endif

  ok &= (flag_dump_strings ? dump_strings () : dump ());

cleanup:

  যদি (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
