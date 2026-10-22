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

#अन्तर्भाव <config.h>

#अन्तर्भाव <endian.h>
#अन्तर्भाव <float.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "ftoastr.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stat-size.h"
#अन्तर्भाव "xbinary-io.h"
#अन्तर्भाव "xprintf.h"
#अन्तर्भाव "xstrtol.h"
#अन्तर्भाव "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "od"

#define AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#define DEFAULT_BYTES_PER_BLOCK 16

#सचे FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  प्रकारनिर्देश _Float16 float16;
#अञ्ञथा
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  प्रकारनिर्देश भिन्न float16;
#endif

#सचे BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  प्रकारनिर्देश __bf16 bfloat16;
#अञ्ञथा
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  प्रकारनिर्देश भिन्न bfloat16;
#endif

गणना size_spec
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

गणना output_format
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

गणना { MAX_INTEGRAL_TYPE_WIDTH = UINTMAX_WIDTH };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
गणना
  {
    FMT_BYTES_ALLOCATED =
      परिमाण "%*.%dlld" - परिमाण "%d" + INT_STRLEN_BOUND (पूर्णाङ्क) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_WIDTH <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
प्रकारनिर्देश शून्य (*print_function_type)
  (idx_t fields, idx_t blank, शून्य ध्रुव *data,
   वर्ण ध्रुव *fmt, पूर्णाङ्क width, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
संरचना tspec
  {
    गणना output_format fmt;
    गणना size_spec size; /* Type of input object.  */
    print_function_type print_function;
    वर्ण fmt_string[FMT_BYTES_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_mode_trailer;
    पूर्णाङ्क field_width; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_width; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
स्थिर ध्रुव पूर्णाङ्क width_bytes[] =
{
  -1,
  परिमाण (unsigned वर्ण),
#सचे UCHAR_MAX < USHRT_MAX
  परिमाण (unsigned short पूर्णाङ्क),
#endif
#सचे USHRT_MAX < UINT_MAX
  परिमाण (unsigned पूर्णाङ्क),
#endif
#सचे UINT_MAX < ULONG_MAX
  परिमाण (unsigned long पूर्णाङ्क),
#endif
#सचे ULONG_MAX < ULLONG_MAX
  परिमाण (unsigned long long पूर्णाङ्क),
#endif
#सचे ULLONG_MAX < UINTMAX_MAX
  परिमाण (uintmax_t),
#endif
#सचे BF16_SUPPORTED
  परिमाण (bfloat16),
#नोचेद्यदि FLOAT16_SUPPORTED
  परिमाण (float16),
#endif
#सचे FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  परिमाण (भिन्न),
#endif
  परिमाण (द्विगुण),
#सचे DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  परिमाण (long द्विगुण),
#endif
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (width_bytes) == N_SIZE_SPECS);

/* Names for some non-printing characters.  */
स्थिर वर्ण ध्रुव charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
स्थिर पूर्णाङ्क address_base = 8;

/* The number of octal digits required to represent the largest
   address value.  */
गणना { MAX_ADDRESS_LENGTH = ((INTMAX_WIDTH - 1) / 3
                             + ((INTMAX_WIDTH - 1) % 3 != 0)) };

/* Width of a normal address.  */
स्थिर पूर्णाङ्क address_pad_len = 7;

/* Minimum length when detecting --strings.  */
स्थिर idx_t string_min;

/* True when in --strings mode.  */
स्थिर bool flag_dump_strings;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
स्थिर bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
स्थिर bool flag_pseudo_start;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
स्थिर intmax_t pseudo_offset;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
स्थिर शून्य format_address_std (intmax_t, वर्ण);
स्थिर शून्य (*format_address) (intmax_t, वर्ण) = format_address_std;

/* The number of input bytes to skip before formatting and writing.  */
स्थिर intmax_t n_bytes_to_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
स्थिर intmax_t end_offset = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
स्थिर bool abbreviate_duplicate_blocks = true;

/* An array of specs describing how to format each input block.  */
स्थिर संरचना tspec *spec;

/* The number of format specs.  */
स्थिर idx_t n_specs;

/* The allocated length of SPEC.  */
स्थिर idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
स्थिर idx_t bytes_per_block;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
स्थिर वर्ण ध्रुव *निवेशः_filename;

/* A null-terminated list of the file-arguments from the command line.  */
स्थिर वर्ण ध्रुव *ध्रुव *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
स्थिर वर्ण ध्रुव *ध्रुव default_file_list[] = {"-", रिक्त};

/* The input stream associated with the current file.  */
स्थिर FILE *in_stream;

/* If true, at least one of the files we read was standard input.  */
स्थिर bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
स्थिर गणना size_spec ध्रुव integral_type_size[] =
  {
#सचे UCHAR_MAX < USHRT_MAX
    [परिमाण (unsigned वर्ण)] = CHAR,
#endif
#सचे USHRT_MAX < UINT_MAX
    [परिमाण (unsigned short पूर्णाङ्क)] = SHORT,
#endif
    [परिमाण (unsigned पूर्णाङ्क)] = INT,
#सचे UINT_MAX < ULONG_MAX
    [परिमाण (unsigned long पूर्णाङ्क)] = LONG,
#endif
#सचे ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [परिमाण (unsigned long long पूर्णाङ्क)] = LONG_LONG,
#endif
#सचे ULONG_MAX < UINTMAX_MAX
    [परिमाण (uintmax_t)] = INTMAX,
#endif
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
स्थिर गणना size_spec ध्रुव fp_type_size[] =
  {
#सचे FLOAT16_SUPPORTED
    [परिमाण (float16)] = FLOAT_HALF,
#नोचेद्यदि BF16_SUPPORTED
    [परिमाण (bfloat16)] = FLOAT_HALF,
#endif
#सचे FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [परिमाण (भिन्न)] = FLOAT_SINGLE,
#endif
    [परिमाण (द्विगुण)] = FLOAT_DOUBLE,
#सचे DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [परिमाण (long द्विगुण)] = FLOAT_LONG_DOUBLE,
#endif
  };

/* Use native endianness by default.  */
स्थिर bool निवेशः_swap;

स्थिर वर्ण ध्रुव short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

गणना endian_type
{
  endian_little,
  endian_big
};

स्थिर वर्ण ध्रुव *ध्रुव endian_args[] =
{
  "little", "big", रिक्त
};

स्थिर गणना endian_type ध्रुव endian_types[] =
{
  endian_little, endian_big
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"skip-bytes", required_argument, रिक्त, 'j'},
  {"address-radix", required_argument, रिक्त, 'A'},
  {"read-bytes", required_argument, रिक्त, 'N'},
  {"format", required_argument, रिक्त, 't'},
  {"output-duplicates", no_argument, रिक्त, 'v'},
  {"strings", optional_argument, रिक्त, 'S'},
  {"traditional", no_argument, रिक्त, TRADITIONAL_OPTION},
  {"width", optional_argument, रिक्त, 'w'},
  {"endian", required_argument, रिक्त, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]OFFSET[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]OFFSET[.][b] [+][LABEL][.][b]]\n\
"),
              program_name, program_name, program_name);
      fputs (_("\n\
Write an unambiguous representation, octal bytes by स्वभाव,\n\
of FILE to standard output.  With more than one FILE argument,\n\
concatenate them in the listed order to form the निवेशः.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If first and second call formats both apply, the second format is assumed\n\
सचे the last operand begins with + or (सचे there are 2 operands) a digit.\n\
An OFFSET operand means -j OFFSET.  LABEL is the pseudo-address\n\
at first byte printed, incremented when dump is progressing.\n\
For OFFSET and LABEL, a 0x or 0X prefix indicates hexadecimal;\n\
suffixes may be . अत्थाय octal and b अत्थाय multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --address-radix=RADIX\n\
         output format अत्थाय file offsets;\n\
         RADIX is one of [doxn], अत्थाय Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         swap निवेशः bytes according the specified order\n\
"));
      oputs (_("\
  -j, --skip-bytes=BYTES\n\
         skip BYTES निवेशः bytes first\n\
"));
      oputs (_("\
  -N, --पठ-bytes=BYTES\n\
         limit dump to BYTES निवेशः bytes\n\
"));
      oputs (_("\
  -S BYTES, --strings[=BYTES]\n\
         show only NUL terminated strings\n\
         of at least BYTES (स्वभाव 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         करोहि not use * to mark पङ्क्तिः suppression\n\
"));
      oputs (_("\
  -w[BYTES], --width[=BYTES]\n\
         output BYTES bytes per output पङ्क्तिः;\n\
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
  -a   same as -t a,  select named characters, ignoring उच्चः-order bit\n\
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
  a          named character, ignoring उच्चः-order bit\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[SIZE]    signed decimal, SIZE bytes per integer\n\
  f[SIZE]    floating point, SIZE bytes per भिन्न\n\
  o[SIZE]    octal, SIZE bytes per integer\n\
  u[SIZE]    unsigned decimal, SIZE bytes per integer\n\
  x[SIZE]    hexadecimal, SIZE bytes per integer\n\
"), stdout);
      fputs (_("\
\n\
SIZE is a सङ्ख्या.  For TYPE in [doux], SIZE may also be C अत्थाय\n\
परिमाण(वर्ण), S अत्थाय परिमाण(short), I अत्थाय परिमाण(पूर्णाङ्क) or L अत्थाय\n\
परिमाण(long).  If TYPE is f, SIZE may also be B अत्थाय Brain 16 bit,\n\
H अत्थाय Half precision भिन्न, F अत्थाय परिमाण(भिन्न), D अत्थाय परिमाण(द्विगुण),\n\
or L अत्थाय परिमाण(long द्विगुण).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix to any type displays printable characters at the end of\n\
each output पङ्क्तिः.\n\
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
and so on अत्थाय G, T, P, E, Z, Y, R, Q.\n\
द्विमान_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
स्थिर शून्य
print_n_spaces (intmax_t n)
{
  अत्थाय (; 0 < n; n--)
    putchar (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
स्थिर idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  पटिनिवत्त pad / fields * i + m * i / fields;
}
स्थिर bool
pad_at_overflow (idx_t fields)
{
  intmax_t product;
  पटिनिवत्त ckd_mul (&product, fields - 1, fields - 1);
}

#define PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
स्थिर शून्य                                                             \
N (idx_t fields, idx_t blank, शून्य ध्रुव *block,			\
   FMT_STRING_DECL, पूर्णाङ्क width, idx_t pad)				\
{                                                                       \
  T ध्रुव *p = block;                                                   \
  idx_t pad_remaining = pad;						\
  अत्थाय (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t next_pad = pad_at (fields, i - 1, pad);			\
      पूर्णाङ्क adjusted_width = pad_remaining - next_pad + width;            \
      T x;                                                              \
      सचे (निवेशः_swap && परिमाण (T) > 1)                                 \
        {                                                               \
          सङ्घ {                                                       \
            T x;                                                        \
            वर्ण b[परिमाण (T)];                                         \
          } u;                                                          \
          अत्थाय (idx_t j = 0; j < परिमाण (T); j++)			\
            u.b[j] = ((वर्ण ध्रुव *) p)[परिमाण (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      अञ्ञथा                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_remaining = next_pad;                                         \
    }                                                                   \
}

#define PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, वर्ण ध्रुव *fmt_string,                           \
                xprintf (fmt_string, adjusted_width, x))

#define PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_UNUSED वर्ण ध्रुव *fmt_string,              \
                वर्ण buf[BUFSIZE];                                      \
                FTOASTR (buf, परिमाण buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_width, buf))

PRINT_TYPE (print_s_char, signed वर्ण)
PRINT_TYPE (print_char, unsigned वर्ण)
PRINT_TYPE (print_s_short, short पूर्णाङ्क)
PRINT_TYPE (print_short, unsigned short पूर्णाङ्क)
PRINT_TYPE (print_int, unsigned पूर्णाङ्क)
PRINT_TYPE (print_long, unsigned long पूर्णाङ्क)
PRINT_TYPE (print_long_long, unsigned long long पूर्णाङ्क)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, भिन्न, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, द्विगुण, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, long द्विगुण, ldtoastr, LDBL_BUFSIZE_BOUND)

#undef PRINT_TYPE
#undef PRINT_FLOATTYPE

स्थिर शून्य
dump_hexl_mode_trailer (idx_t n_bytes, वर्ण ध्रुव *block)
{
  fputs ("  >", stdout);
  अत्थाय (idx_t i = n_bytes; i > 0; i--)
    {
      unsigned वर्ण c = *block++;
      unsigned वर्ण c2 = (isprint (c) ? c : '.');
      putchar (c2);
    }
  putchar ('<');
}

स्थिर शून्य
print_named_ascii (idx_t fields, idx_t blank, शून्य ध्रुव *block,
                   MAYBE_UNUSED वर्ण ध्रुव *unused_fmt_string,
                   पूर्णाङ्क width, idx_t pad)
{
  unsigned वर्ण ध्रुव *p = block;
  idx_t pad_remaining = pad;
  अत्थाय (idx_t i = fields; blank < i; i--)
    {
      पूर्णाङ्क masked_c = *p++ & 0x7f;
      वर्ण ध्रुव *s;
      वर्ण buf[2];

      सचे (masked_c == 127)
        s = "del";
      अञ्ञथा सचे (masked_c <= 040)
        s = charname[masked_c];
      अञ्ञथा
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t next_pad = pad_at (fields, i - 1, pad);
      पूर्णाङ्क adjusted_width = pad_remaining - next_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = next_pad;
    }
}

स्थिर शून्य
print_ascii (idx_t fields, idx_t blank, शून्य ध्रुव *block,
             MAYBE_UNUSED वर्ण ध्रुव *unused_fmt_string, पूर्णाङ्क width,
             idx_t pad)
{
  unsigned वर्ण ध्रुव *p = block;
  idx_t pad_remaining = pad;
  अत्थाय (idx_t i = fields; blank < i; i--)
    {
      unsigned वर्ण c = *p++;
      वर्ण ध्रुव *s;
      वर्ण buf[4];

      विकल्प (c)
        {
        पक्ष '\0':
          s = "\\0";
          भङ्ग;

        पक्ष '\a':
          s = "\\a";
          भङ्ग;

        पक्ष '\b':
          s = "\\b";
          भङ्ग;

        पक्ष '\f':
          s = "\\f";
          भङ्ग;

        पक्ष '\n':
          s = "\\n";
          भङ्ग;

        पक्ष '\r':
          s = "\\r";
          भङ्ग;

        पक्ष '\t':
          s = "\\t";
          भङ्ग;

        पक्ष '\v':
          s = "\\v";
          भङ्ग;

        स्वभाव:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t next_pad = pad_at (fields, i - 1, pad);
      पूर्णाङ्क adjusted_width = pad_remaining - next_pad + width;
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

स्थिर bool
simple_strtoi (वर्ण ध्रुव *s, वर्ण ध्रुव **p, पूर्णाङ्क *val)
{
  पूर्णाङ्क sum;

  अत्थाय (sum = 0; c_isdigit (*s); s++)
    सचे (ckd_mul (&sum, sum, 10) || ckd_add (&sum, sum, *s - '0'))
      पटिनिवत्त false;
  *p = s;
  *val = sum;
  पटिनिवत्त true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
स्थिर वर्ण ध्रुव *
ispec_to_format (गणना size_spec size_spec,
                 वर्ण ध्रुव *int_format,
                 वर्ण ध्रुव *long_format,
                 वर्ण ध्रुव *long_long_format,
                 वर्ण ध्रुव *intmax_format)
{
  पटिनिवत्त (LONG < INTMAX && size_spec == INTMAX ? intmax_format
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

स्थिर bool ATTRIBUTE_NONNULL ()
decode_one_format (वर्ण ध्रुव *s_orig, वर्ण ध्रुव *s, वर्ण ध्रुव **next,
                   संरचना tspec *tspec)
{
  गणना size_spec size_spec;
  पूर्णाङ्क size;
  गणना output_format fmt;
  print_function_type print_function;
  वर्ण ध्रुव *p;
  वर्ण c;
  पूर्णाङ्क field_width;

  विकल्प (*s)
    {
    पक्ष 'd':
    पक्ष 'o':
    पक्ष 'u':
    पक्ष 'x':
      c = *s;
      ++s;
      विकल्प (*s)
        {
        पक्ष 'C':
          ++s;
          size = परिमाण (unsigned वर्ण);
          भङ्ग;

        पक्ष 'S':
          ++s;
          size = परिमाण (unsigned short पूर्णाङ्क);
          भङ्ग;

        पक्ष 'I':
          ++s;
          size = परिमाण (unsigned पूर्णाङ्क);
          भङ्ग;

        पक्ष 'L':
          ++s;
          size = परिमाण (unsigned long पूर्णाङ्क);
          भङ्ग;

        स्वभाव:
          सचे (! simple_strtoi (s, &p, &size))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              पटिनिवत्त false;
            }
          सचे (p == s)
            size = परिमाण (unsigned पूर्णाङ्क);
          अञ्ञथा
            {
              सचे (countof (integral_type_size) <= size
                  || integral_type_size[size] == NO_SIZE)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), size);
                  पटिनिवत्त false;
                }
              s = p;
            }
          भङ्ग;
        }

      size_spec = integral_type_size[size];

      विकल्प (c)
        {
        पक्ष 'd':
          fmt = SIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * size - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (size_spec, "d", "ld", "lld", "jd"));
          भङ्ग;

        पक्ष 'o':
          fmt = OCTAL;
          field_width = (CHAR_BIT * size + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (size_spec, "o", "lo", "llo", "jo"));
          भङ्ग;

        पक्ष 'u':
          fmt = UNSIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (CHAR_BIT * size);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_to_format (size_spec, "u", "lu", "llu", "ju"));
          भङ्ग;

        पक्ष 'x':
          fmt = HEXADECIMAL;
          field_width = (CHAR_BIT * size + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (size_spec, "x", "lx", "llx", "jx"));
          भङ्ग;

        स्वभाव:
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
           : (affirm (false), (print_function_type) रिक्त));
      भङ्ग;

    पक्ष 'f':
      fmt = FLOATING_POINT;
      ++s;
      विकल्प (*s)
        {
        पक्ष 'B':
          ++s;
          fmt = BFLOATING_POINT;
          size = परिमाण (bfloat16);
          भङ्ग;

        पक्ष 'H':
          ++s;
          fmt = HFLOATING_POINT;
          size = परिमाण (float16);
          भङ्ग;

        पक्ष 'F':
          ++s;
          size = परिमाण (भिन्न);
          भङ्ग;

        पक्ष 'D':
          ++s;
          size = परिमाण (द्विगुण);
          भङ्ग;

        पक्ष 'L':
          ++s;
          size = परिमाण (long द्विगुण);
          भङ्ग;

        स्वभाव:
          सचे (! simple_strtoi (s, &p, &size))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              पटिनिवत्त false;
            }
          सचे (p == s)
            size = परिमाण (द्विगुण);
          अञ्ञथा
            {
              सचे (countof (fp_type_size) <= size
                  || fp_type_size[size] == NO_SIZE
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && size == परिमाण (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), size);
                  पटिनिवत्त false;
                }
              s = p;
            }
          भङ्ग;
        }
      size_spec = fp_type_size[size];

      सचे ((! FLOAT16_SUPPORTED && fmt == HFLOATING_POINT)
          || (! BF16_SUPPORTED && fmt == BFLOATING_POINT))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        पटिनिवत्त false;
      }

      {
        संरचना lconv ध्रुव *locale = localeconv ();
        idx_t decimal_point_len =
          (locale->decimal_point[0] ? strlen (locale->decimal_point) : 1);

        सचे (size_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            field_width = DBL_STRLEN_BOUND_L (decimal_point_len);
          }
        अञ्ञथा सचे (FLOAT_SINGLE < FLOAT_DOUBLE && size_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        अञ्ञथा सचे (FLOAT_HALF < FLOAT_SINGLE && size_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_POINT
                             ? print_bfloat : print_halffloat;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        अञ्ञथा सचे (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && size_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            field_width = LDBL_STRLEN_BOUND_L (decimal_point_len);
          }
        अञ्ञथा
          affirm (false);

        भङ्ग;
      }

    पक्ष 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      size_spec = CHAR;
      print_function = print_named_ascii;
      field_width = 3;
      भङ्ग;

    पक्ष 'c':
      ++s;
      fmt = CHARACTER;
      size_spec = CHAR;
      print_function = print_ascii;
      field_width = 3;
      भङ्ग;

    स्वभाव:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      पटिनिवत्त false;
    }

  tspec->size = size_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->field_width = field_width;
  tspec->hexl_mode_trailer = (*s == 'z');
  सचे (tspec->hexl_mode_trailer)
    s++;

  *next = s;
  पटिनिवत्त true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

स्थिर bool
open_next_file (शून्य)
{
  bool ok = true;

  करोहि
    {
      निवेशः_filename = *file_list;
      सचे (निवेशः_filename == रिक्त)
        पटिनिवत्त ok;
      ++file_list;

      सचे (streq (निवेशः_filename, "-"))
        {
          निवेशः_filename = _("standard input");
          in_stream = stdin;
          have_read_stdin = true;
          xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
        }
      अञ्ञथा
        {
          in_stream = fopen (निवेशः_filename, (O_द्विमान ? "rb" : "r"));
          सचे (in_stream == रिक्त)
            {
              error (0, errno, "%s", quotef (निवेशः_filename));
              ok = false;
            }
        }
    }
  याव (in_stream == रिक्त);

  सचे (0 <= end_offset && !flag_dump_strings)
    setvbuf (in_stream, रिक्त, _IONBF, 0);

  पटिनिवत्त ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

स्थिर bool
check_and_close (पूर्णाङ्क in_errno)
{
  bool ok = true;

  सचे (in_stream != रिक्त)
    {
      सचे (!ferror (in_stream))
        in_errno = 0;
      सचे (streq (file_list[-1], "-"))
        clearerr (in_stream);
      अञ्ञथा सचे (fclose (in_stream) != 0 && !in_errno)
        in_errno = errno;
      सचे (in_errno)
        {
          error (0, in_errno, "%s", quotef (निवेशः_filename));
          ok = false;
        }

      in_stream = रिक्त;
    }

  सचे (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  पटिनिवत्त ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

स्थिर bool ATTRIBUTE_NONNULL ()
decode_format_string (वर्ण ध्रुव *s)
{
  वर्ण ध्रुव *s_orig = s;

  याव (*s != '\0')
    {
      वर्ण ध्रुव *next;

      सचे (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, परिमाण *spec);

      सचे (! decode_one_format (s_orig, s, &next, &spec[n_specs]))
        पटिनिवत्त false;

      affirm (s != next);
      s = next;
      ++n_specs;
    }

  पटिनिवत्त true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

स्थिर bool
skip (intmax_t n_skip)
{
  bool ok = true;
  पूर्णाङ्क in_errno = 0;

  सचे (n_skip == 0)
    पटिनिवत्त true;

  याव (in_stream != रिक्त)	/* EOF.  */
    {
      संरचना stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      सचे (fstat (fileno (in_stream), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          सचे (S_ISREG (file_stats.st_mode)
              && STP_BLKSIZE (&file_stats) < file_stats.st_size)
            {
              सचे (file_stats.st_size < n_skip)
                n_skip -= file_stats.st_size;
              अञ्ञथा
                {
                  सचे (fseeko (in_stream, n_skip, SEEK_CUR) != 0)
                    {
                      in_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          अञ्ञथा सचे (! (S_ISREG (file_stats.st_mode)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (in_stream, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          अञ्ञथा
            {
              वर्ण buf[BUFSIZ];
              idx_t n_bytes_read, n_bytes_to_read = BUFSIZ;

              याव (0 < n_skip)
                {
                  सचे (n_skip < n_bytes_to_read)
                    n_bytes_to_read = n_skip;
                  n_bytes_read = fread (buf, 1, n_bytes_to_read, in_stream);
                  n_skip -= n_bytes_read;
                  सचे (n_bytes_read != n_bytes_to_read)
                    {
                      सचे (ferror (in_stream))
                        {
                          in_errno = errno;
                          ok = false;
                          n_skip = 0;
                          भङ्ग;
                        }
                      सचे (feof (in_stream))
                        भङ्ग;
                    }
                }
            }

          सचे (n_skip == 0)
            भङ्ग;
        }

      अञ्ञथा   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (निवेशः_filename));
          ok = false;
        }

      ok &= check_and_close (in_errno);

      ok &= open_next_file ();
    }

  सचे (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  पटिनिवत्त ok;
}

स्थिर शून्य
format_address_none (MAYBE_UNUSED intmax_t address,
                     MAYBE_UNUSED वर्ण c)
{
}

स्थिर शून्य
format_address_std (intmax_t address, वर्ण c)
{
  वर्ण buf[MAX_ADDRESS_LENGTH + 2];
  वर्ण *p = buf + परिमाण buf;
  वर्ण ध्रुव *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - address_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  विकल्प (address_base)
    {
    पक्ष 8:
      करोहि
        *--p = '0' + (address & 7);
      याव ((address >>= 3) != 0);
      भङ्ग;

    पक्ष 10:
      करोहि
        *--p = '0' + (address % 10);
      याव ((address /= 10) != 0);
      भङ्ग;

    पक्ष 16:
      करोहि
        *--p = "0123456789abcdef"[address & 15];
      याव ((address >>= 4) != 0);
      भङ्ग;
    }

  याव (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

स्थिर शून्य
format_address_paren (intmax_t address, वर्ण c)
{
  putchar ('(');
  format_address_std (address, ')');
  सचे (c)
    putchar (c);
}

स्थिर शून्य
format_address_label (intmax_t address, वर्ण c)
{
  format_address_std (address, ' ');

  intmax_t addr;
  सचे (ckd_add (&addr, address, pseudo_offset))
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

स्थिर शून्य
write_block (intmax_t current_offset, idx_t n_bytes,
             वर्ण ध्रुव *prev_block, वर्ण ध्रुव *curr_block)
{
  स्थिर bool first = true;
  स्थिर bool prev_pair_equal = false;

  सचे (abbreviate_duplicate_blocks
      && !first && n_bytes == bytes_per_block
      && memeq (prev_block, curr_block, bytes_per_block))
    {
      सचे (prev_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      अञ्ञथा
        {
          printf ("*\n");
          prev_pair_equal = true;
        }
    }
  अञ्ञथा
    {
      prev_pair_equal = false;
      अत्थाय (idx_t i = 0; i < n_specs; i++)
        {
          पूर्णाङ्क datum_width = width_bytes[spec[i].size];
          idx_t fields_per_block = bytes_per_block / datum_width;
          idx_t blank_fields = (bytes_per_block - n_bytes) / datum_width;
          सचे (i == 0)
            format_address (current_offset, '\0');
          अञ्ञथा
            print_n_spaces (address_pad_len);
          (*spec[i].print_function) (fields_per_block, blank_fields,
                                     curr_block, spec[i].fmt_string,
                                     spec[i].field_width, spec[i].pad_width);
          सचे (spec[i].hexl_mode_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              पूर्णाङ्क field_width = spec[i].field_width;
              अत्थाय (idx_t f = 0; f < blank_fields; f++)
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

स्थिर bool
read_char (पूर्णाङ्क *c)
{
  bool ok = true;

  *c = EOF;

  याव (in_stream && (*c = getc (in_stream)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_next_file ();
    }

  पटिनिवत्त ok;
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

स्थिर bool
read_block (idx_t n, वर्ण *block, idx_t *n_bytes_in_buffer)
{
  bool ok = true;

  affirm (0 < n && n <= bytes_per_block);

  *n_bytes_in_buffer = 0;

  याव (in_stream != रिक्त)	/* EOF.  */
    {
      idx_t n_needed = n - *n_bytes_in_buffer;
      idx_t n_read = fread (block + *n_bytes_in_buffer,
                            1, n_needed, in_stream);

      *n_bytes_in_buffer += n_read;

      सचे (n_read == n_needed)
        भङ्ग;

      ok &= check_and_close (errno);

      ok &= open_next_file ();
    }

  पटिनिवत्त ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
get_lcm (शून्य)
{
  पूर्णाङ्क l_c_m = 1;

  अत्थाय (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, width_bytes[spec[i].size]);
  पटिनिवत्त l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
स्थिर strtol_error
xstr2nonneg (वर्ण ध्रुव *restrict nptr, पूर्णाङ्क base, intmax_t *val,
             वर्ण ध्रुव *restrict valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, रिक्त, base, val, valid_suffixes);
  पटिनिवत्त s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

स्थिर bool
parse_old_offset (वर्ण *str, intmax_t *offset)
{
  /* Skip over any leading '+'.  */
  वर्ण *s = str + (str[0] == '+');

  सचे (! c_isdigit (s[0]))
    पटिनिवत्त false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  वर्ण *dot = strchr (s, '.');
  सचे (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = रिक्त;
  पूर्णाङ्क radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  सचे (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  गणना strtol_error s_err = xstr2nonneg (s, radix, offset, "bB");

  सचे (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  सचे (s_err == LONGINT_OVERFLOW)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  पटिनिवत्त s_err == LONGINT_OK;
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

स्थिर bool
dump (शून्य)
{
  वर्ण *block[2];
  bool idx = false;
  bool ok = true;
  idx_t n_bytes_read;

  block[0] = xinmalloc (1 + abbreviate_duplicate_blocks, bytes_per_block);
  block[1] = block[0] + abbreviate_duplicate_blocks * bytes_per_block;

  intmax_t current_offset = n_bytes_to_skip;

  करोहि
    {
      intmax_t needed_bound
        = end_offset < 0 ? INTMAX_MAX : end_offset - current_offset;
      सचे (needed_bound <= 0)
        {
          n_bytes_read = 0;
          भङ्ग;
        }
      idx_t n_needed = MIN (bytes_per_block, needed_bound);
      ok &= read_block (n_needed, block[idx], &n_bytes_read);
      सचे (n_bytes_read < bytes_per_block)
        भङ्ग;
      affirm (n_bytes_read == bytes_per_block);
      write_block (current_offset, n_bytes_read,
                   block[!idx], block[idx]);
      सचे (ferror (stdout))
        ok = false;
      current_offset += n_bytes_read;
      idx = !idx;
    }
  याव (ok);

  सचे (n_bytes_read > 0)
    {
      पूर्णाङ्क l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t bytes_to_write = (n_bytes_read + l_c_m - 1
                              - (n_bytes_read + l_c_m - 1) % l_c_m);

      memset (block[idx] + n_bytes_read, 0, bytes_to_write - n_bytes_read);
      write_block (current_offset, n_bytes_read, block[!idx], block[idx]);
      current_offset += n_bytes_read;
    }

  सचे (ok || current_offset)
    format_address (current_offset, '\n');

  सचे (0 <= end_offset && end_offset <= current_offset)
    ok &= check_and_close (0);

  मुक्त (block[0]);

  पटिनिवत्त ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

स्थिर bool
dump_strings (शून्य)
{
  idx_t bufsize = MAX (100, string_min + 1);
  वर्ण *buf = ximalloc (bufsize);
  intmax_t address = n_bytes_to_skip;
  bool ok = true;

  याव (true)
    {
      idx_t i = 0;
      पूर्णाङ्क c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      सचे (0 <= end_offset
          && (end_offset < string_min || end_offset - string_min < address))
        भङ्ग;

      /* Store consecutive printable characters to BUF.  */
      याव (! (0 <= end_offset && end_offset <= address))
        {
          सचे (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, परिमाण *buf);
          ok &= read_char (&c);
          सचे (c < 0)
            {
              मुक्त (buf);
              पटिनिवत्त ok;
            }
          address++;
          buf[i++] = c;
          सचे (c == '\0')
            भङ्ग;		/* Print this string.  */
          सचे (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              भङ्ग;
            }
        }

      सचे (c < 0 || i - !c < string_min)
        अनुवर्त;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_address (address - i, ' ');

      अत्थाय (i = 0; (c = buf[i]); i++)
        {
          विकल्प (c)
            {
            पक्ष '\a':
              fputs ("\\a", stdout);
              भङ्ग;

            पक्ष '\b':
              fputs ("\\b", stdout);
              भङ्ग;

            पक्ष '\f':
              fputs ("\\f", stdout);
              भङ्ग;

            पक्ष '\n':
              fputs ("\\n", stdout);
              भङ्ग;

            पक्ष '\r':
              fputs ("\\r", stdout);
              भङ्ग;

            पक्ष '\t':
              fputs ("\\t", stdout);
              भङ्ग;

            पक्ष '\v':
              fputs ("\\v", stdout);
              भङ्ग;

            स्वभाव:
              putc (c, stdout);
            }
        }
      putchar ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  मुक्त (buf);

  ok &= check_and_close (0);
  पटिनिवत्त ok;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क n_files;
  पूर्णाङ्क l_c_m;
  idx_t desired_width = 0;
  bool modern = false;
  bool ok = true;
  idx_t width_per_block = 0;
  स्थिर वर्ण ध्रुव multipliers[] = "bEGKkMmPQRTYZ0";

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

  याव (true)
    {
      intmax_t tmp;
      गणना strtol_error s_err;
      पूर्णाङ्क oi = -1;
      पूर्णाङ्क c = getopt_long (argc, argv, short_options, long_options, &oi);
      सचे (c == -1)
        भङ्ग;

      विकल्प (c)
        {
        पक्ष 'A':
          modern = true;
          विकल्प (optarg[0])
            {
            पक्ष 'd':
              format_address = format_address_std;
              address_base = 10;
              address_pad_len = 7;
              भङ्ग;
            पक्ष 'o':
              format_address = format_address_std;
              address_base = 8;
              address_pad_len = 7;
              भङ्ग;
            पक्ष 'x':
              format_address = format_address_std;
              address_base = 16;
              address_pad_len = 6;
              भङ्ग;
            पक्ष 'n':
              format_address = format_address_none;
              address_pad_len = 0;
              भङ्ग;
            स्वभाव:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              भङ्ग;
            }
          भङ्ग;

        पक्ष 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_bytes_to_skip, multipliers);
          सचे (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          भङ्ग;

        पक्ष 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_bytes_to_format, multipliers);
          सचे (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          भङ्ग;

        पक्ष 'S':
          modern = true;
          सचे (optarg == रिक्त)
            string_min = 3;
          अञ्ञथा
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              सचे (s_err == LONGINT_OK && ckd_add (&i, tmp, 1))
                s_err = LONGINT_OVERFLOW;
              सचे (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_strings = true;
          भङ्ग;

        पक्ष 't':
          modern = true;
          ok &= decode_format_string (optarg);
          भङ्ग;

        पक्ष 'v':
          modern = true;
          abbreviate_duplicate_blocks = false;
          भङ्ग;

        पक्ष TRADITIONAL_OPTION:
          traditional = true;
          भङ्ग;

        पक्ष ENDIAN_OPTION:
          विकल्प (XARGMATCH ("--endian", optarg, endian_args, endian_types))
            {
            पक्ष endian_big:
              निवेशः_swap = BYTE_ORDER != BIG_ENDIAN;
              भङ्ग;
            पक्ष endian_little:
              निवेशः_swap = BYTE_ORDER != LITTLE_ENDIAN;
              भङ्ग;
            }
          भङ्ग;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#define CASE_OLD_ARG(old_char,new_string)		\
        पक्ष old_char:					\
          ok &= decode_format_string (new_string);	\
          भङ्ग

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        पक्ष 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        पक्ष 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        पक्ष 'I': पक्ष 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        पक्ष 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        पक्ष 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#undef CASE_OLD_ARG

        पक्ष 'w':
          modern = true;
          सचे (optarg == रिक्त)
            {
              desired_width = 32;
            }
          अञ्ञथा
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              सचे (s_err == LONGINT_OK)
                {
                  सचे (ckd_add (&desired_width, w_tmp, 0))
                    s_err = LONGINT_OVERFLOW;
                  अञ्ञथा सचे (desired_width == 0)
                    s_err = LONGINT_INVALID;
                }
              सचे (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
          भङ्ग;
        }
    }

  सचे (!ok)
    पटिनिवत्त EXIT_FAILURE;

  सचे (flag_dump_strings && n_specs > 0)
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

  सचे (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      विकल्प (n_files)
        {
        पक्ष 1:
          सचे ((traditional || argv[optind][0] == '+')
              && parse_old_offset (argv[optind], &o1))
            {
              n_bytes_to_skip = o1;
              --n_files;
              ++argv;
            }
          भङ्ग;

        पक्ष 2:
          सचे ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_offset (argv[optind + 1], &o2))
            {
              सचे (traditional && parse_old_offset (argv[optind], &o1))
                {
                  n_bytes_to_skip = o1;
                  flag_pseudo_start = true;
                  pseudo_start = o2;
                  argv += 2;
                  n_files -= 2;
                }
              अञ्ञथा
                {
                  n_bytes_to_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          भङ्ग;

        पक्ष 3:
          सचे (traditional
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
          भङ्ग;
        }

      सचे (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  सचे (flag_pseudo_start)
    {
      सचे (format_address == format_address_none)
        {
          address_base = 8;
          address_pad_len = 7;
          format_address = format_address_paren;
        }
      अञ्ञथा
        format_address = format_address_label;
    }

  सचे (0 <= max_bytes_to_format
      && ckd_add (&end_offset, n_bytes_to_skip, max_bytes_to_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  सचे (n_specs == 0)
    decode_format_string ("oS");

  सचे (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (वर्ण ध्रुव *ध्रुव *) &argv[optind];
    }
  अञ्ञथा
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_next_file ();
  सचे (in_stream == रिक्त)
    गच्छ cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_bytes_to_skip);
  सचे (in_stream == रिक्त)
    गच्छ cleanup;

  pseudo_offset = (flag_pseudo_start ? pseudo_start - n_bytes_to_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  सचे (desired_width != 0)
    {
      सचे (desired_width % l_c_m == 0)
        bytes_per_block = desired_width;
      अञ्ञथा
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_width, l_c_m);
          bytes_per_block = l_c_m;
        }
    }
  अञ्ञथा
    {
      सचे (l_c_m < DEFAULT_BYTES_PER_BLOCK)
        bytes_per_block = (DEFAULT_BYTES_PER_BLOCK
                           - DEFAULT_BYTES_PER_BLOCK % l_c_m);
      अञ्ञथा
        bytes_per_block = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  अत्थाय (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      सचे (pad_at_overflow (fields_per_block))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_width);
      idx_t block_width = (spec[i].field_width + 1) * fields_per_block;
      सचे (width_per_block < block_width)
        width_per_block = block_width;
    }
  अत्थाय (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].size];
      idx_t block_width = spec[i].field_width * fields_per_block;
      spec[i].pad_width = width_per_block - block_width;
    }

#ifdef DEBUG
  printf ("lcm=%d, width_per_block=%td\n", l_c_m, width_per_block);
  अत्थाय (idx_t i = 0; i < n_specs; i++)
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

  सचे (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  पटिनिवत्त ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
