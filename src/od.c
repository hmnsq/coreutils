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

#समावेश <config.h>

#समावेश <endian.h>
#समावेश <float.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश "system.h"
#समावेश "argmatch.h"
#समावेश "assure.h"
#समावेश "c-ctype.h"
#समावेश "ftoastr.h"
#समावेश "quote.h"
#समावेश "stat-size.h"
#समावेश "xbinary-io.h"
#समावेश "xprintf.h"
#समावेश "xstrtol.h"
#समावेश "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "od"

#घोषणा AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#घोषणा DEFAULT_BYTES_PER_BLOCK 16

#अगर FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  प्रकार _Float16 float16;
#अन्यथा
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  प्रकार भग्न float16;
#पूर्ण_अगर

#अगर BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  प्रकार __bf16 bfloat16;
#अन्यथा
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  प्रकार भग्न bfloat16;
#पूर्ण_अगर

क्रमागत आकार_spec
  {
    NO_आकार = 0,
    CHAR,
    SHORT = CHAR + (अच_अक्षर_उच्च < अच_लघु_उच्च),
    INT = SHORT + (अच_लघु_उच्च < अच_पूर्णांक_उच्च),
    LONG = INT + (अच_पूर्णांक_उच्च < अच_दीर्घ_उच्च),
    LONG_LONG = LONG + (अच_दीर्घ_उच्च < ULLONG_MAX),
    INTMAX = LONG_LONG + (ULLONG_MAX < UINTMAX_MAX),
    FLOAT_HALF, /* Used only if (FLOAT16_SUPPORTED || BF16_SUPPORTED).  */
    FLOAT_SINGLE = FLOAT_HALF + (FLOAT16_SUPPORTED || BF16_SUPPORTED),
    FLOAT_DOUBLE = FLOAT_SINGLE + (भग्न_पूर्ण < द्विग_पूर्ण
                                   || FLT_MAX_EXP < DBL_MAX_EXP),
    FLOAT_LONG_DOUBLE = FLOAT_DOUBLE + (द्विग_पूर्ण < द_द्वि_पूर्ण
                                        || DBL_MAX_EXP < LDBL_MAX_EXP),
    N_आकार_SPECS
  };

क्रमागत output_format
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

क्रमागत { MAX_INTEGRAL_TYPE_WIDTH = UINTMAX_WIDTH };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
क्रमागत
  {
    FMT_BYTES_ALLOCATED =
      माप "%*.%dlld" - माप "%d" + INT_STRLEN_BOUND (पूर्णांक) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_WIDTH <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
प्रकार व्योम (*print_function_type)
  (idx_t fields, idx_t blank, व्योम स्थिर *तथ्य,
   अक्षर स्थिर *fmt, पूर्णांक width, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
काष्ठा tspec
  {
    क्रमागत output_format fmt;
    क्रमागत आकार_spec आकार; /* Type of input object.  */
    print_function_type print_function;
    अक्षर fmt_string[FMT_BYTES_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_mode_trailer;
    पूर्णांक field_width; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_width; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
अटल स्थिर पूर्णांक width_bytes[] =
{
  -1,
  माप (अचिन्हित अक्षर),
#अगर अच_अक्षर_उच्च < अच_लघु_उच्च
  माप (अचिन्हित लघु पूर्णांक),
#पूर्ण_अगर
#अगर अच_लघु_उच्च < अच_पूर्णांक_उच्च
  माप (अचिन्हित पूर्णांक),
#पूर्ण_अगर
#अगर अच_पूर्णांक_उच्च < अच_दीर्घ_उच्च
  माप (अचिन्हित दीर्घ पूर्णांक),
#पूर्ण_अगर
#अगर अच_दीर्घ_उच्च < ULLONG_MAX
  माप (अचिन्हित दीर्घ दीर्घ पूर्णांक),
#पूर्ण_अगर
#अगर ULLONG_MAX < UINTMAX_MAX
  माप (uintmax_t),
#पूर्ण_अगर
#अगर BF16_SUPPORTED
  माप (bfloat16),
#या_अगर FLOAT16_SUPPORTED
  माप (float16),
#पूर्ण_अगर
#अगर भग्न_पूर्ण < द्विग_पूर्ण || FLT_MAX_EXP < DBL_MAX_EXP
  माप (भग्न),
#पूर्ण_अगर
  माप (द्विगुन),
#अगर द्विग_पूर्ण < द_द्वि_पूर्ण || DBL_MAX_EXP < LDBL_MAX_EXP
  माप (दीर्घ द्विगुन),
#पूर्ण_अगर
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (width_bytes) == N_आकार_SPECS);

/* Names for some non-printing characters.  */
अटल अक्षर स्थिर charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
अटल पूर्णांक address_base = 8;

/* The number of octal digits required to represent the largest
   address value.  */
क्रमागत { MAX_ADDRESS_लंबाई = ((INTMAX_WIDTH - 1) / 3
                             + ((INTMAX_WIDTH - 1) % 3 != 0)) };

/* Width of a normal address.  */
अटल पूर्णांक address_pad_len = 7;

/* Minimum length when detecting --strings.  */
अटल idx_t string_min;

/* True when in --strings mode.  */
अटल bool flag_dump_strings;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
अटल bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
अटल bool flag_pseudo_start;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
अटल intmax_t pseudo_offset;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
अटल व्योम format_address_std (intmax_t, अक्षर);
अटल व्योम (*format_address) (intmax_t, अक्षर) = format_address_std;

/* The number of input bytes to skip before formatting and writing.  */
अटल intmax_t n_bytes_to_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
अटल intmax_t end_offset = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
अटल bool abbreviate_duplicate_blocks = true;

/* An array of specs describing how to format each input block.  */
अटल काष्ठा tspec *spec;

/* The number of format specs.  */
अटल idx_t n_specs;

/* The allocated length of SPEC.  */
अटल idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
अटल idx_t bytes_per_block;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
अटल अक्षर स्थिर *निवेश_filename;

/* A null-terminated list of the file-arguments from the command line.  */
अटल अक्षर स्थिर *स्थिर *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
अटल अक्षर स्थिर *स्थिर default_file_list[] = {"-", शून्य};

/* The input stream associated with the current file.  */
अटल खाता *in_stream;

/* If true, at least one of the files we read was standard input.  */
अटल bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
अटल क्रमागत आकार_spec स्थिर integral_type_आकार[] =
  {
#अगर अच_अक्षर_उच्च < अच_लघु_उच्च
    [माप (अचिन्हित अक्षर)] = CHAR,
#पूर्ण_अगर
#अगर अच_लघु_उच्च < अच_पूर्णांक_उच्च
    [माप (अचिन्हित लघु पूर्णांक)] = SHORT,
#पूर्ण_अगर
    [माप (अचिन्हित पूर्णांक)] = INT,
#अगर अच_पूर्णांक_उच्च < अच_दीर्घ_उच्च
    [माप (अचिन्हित दीर्घ पूर्णांक)] = LONG,
#पूर्ण_अगर
#अगर अच_दीर्घ_उच्च < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [माप (अचिन्हित दीर्घ दीर्घ पूर्णांक)] = LONG_LONG,
#पूर्ण_अगर
#अगर अच_दीर्घ_उच्च < UINTMAX_MAX
    [माप (uintmax_t)] = INTMAX,
#पूर्ण_अगर
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
अटल क्रमागत आकार_spec स्थिर fp_type_आकार[] =
  {
#अगर FLOAT16_SUPPORTED
    [माप (float16)] = FLOAT_HALF,
#या_अगर BF16_SUPPORTED
    [माप (bfloat16)] = FLOAT_HALF,
#पूर्ण_अगर
#अगर भग्न_पूर्ण < द्विग_पूर्ण || FLT_MAX_EXP < DBL_MAX_EXP
    [माप (भग्न)] = FLOAT_SINGLE,
#पूर्ण_अगर
    [माप (द्विगुन)] = FLOAT_DOUBLE,
#अगर द्विग_पूर्ण < द_द्वि_पूर्ण || DBL_MAX_EXP < LDBL_MAX_EXP
    [माप (दीर्घ द्विगुन)] = FLOAT_LONG_DOUBLE,
#पूर्ण_अगर
  };

/* Use native endianness by default.  */
अटल bool निवेश_swap;

अटल अक्षर स्थिर short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  TRADITIONAL_OPTION = अक्षर_उच्च + 1,
  ENDIAN_OPTION,
};

क्रमागत endian_type
{
  endian_little,
  endian_big
};

अटल अक्षर स्थिर *स्थिर endian_args[] =
{
  "little", "big", शून्य
};

अटल क्रमागत endian_type स्थिर endian_types[] =
{
  endian_little, endian_big
};

अटल काष्ठा option स्थिर long_options[] =
{
  {"skip-bytes", required_argument, शून्य, 'j'},
  {"address-radix", required_argument, शून्य, 'A'},
  {"read-bytes", required_argument, शून्य, 'N'},
  {"format", required_argument, शून्य, 't'},
  {"output-duplicates", no_argument, शून्य, 'v'},
  {"strings", optional_argument, शून्य, 'S'},
  {"traditional", no_argument, शून्य, TRADITIONAL_OPTION},
  {"width", optional_argument, शून्य, 'w'},
  {"endian", required_argument, शून्य, ENDIAN_OPTION },

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
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
  or:  %s [-abcdfilosx]... [खाता] [[+]OFFSET[.][b]]\n\
  or:  %s --traditional [OPTION]... [खाता] [[+]OFFSET[.][b] [+][LABEL][.][b]]\n\
"),
              program_name, program_name, program_name);
      ख_माला_दो (_("\n\
Write an unambiguous representation, octal bytes by शेष,\n\
of खाता to standard output.  With more than one खाता argument,\n\
concatenate them in the listed order to form the निवेश.\n\
"), मानक_निकास);

      emit_stdin_note ();

      ख_माला_दो (_("\
\n\
If first and second call formats both apply, the second format is assumed\n\
अगर the last operand begins with + or (अगर there are 2 operands) a digit.\n\
An OFFSET operand means -j OFFSET.  LABEL is the pseudo-address\n\
at first byte printed, incremented when dump is progressing.\n\
For OFFSET and LABEL, a 0x or 0X prefix indicates hexadecimal;\n\
suffixes may be . क्रम octal and b क्रम multiply by 512.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --address-radix=RADIX\n\
         output format क्रम file offsets;\n\
         RADIX is one of [doxn], क्रम Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         swap निवेश bytes according the specified order\n\
"));
      oputs (_("\
  -j, --skip-bytes=BYTES\n\
         skip BYTES निवेश bytes first\n\
"));
      oputs (_("\
  -N, --पढ़ो-bytes=BYTES\n\
         limit dump to BYTES निवेश bytes\n\
"));
      oputs (_("\
  -S BYTES, --strings[=BYTES]\n\
         show only NUL terminated strings\n\
         of at least BYTES (शेष 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         करो not use * to mark पंक्ति suppression\n\
"));
      oputs (_("\
  -w[BYTES], --width[=BYTES]\n\
         output BYTES bytes per output पंक्ति;\n\
         32 is implied when BYTES is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments in third form above\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
\n\
Traditional format specifications may be intermixed; they accumulate:\n\
"), मानक_निकास);
      oputs (_("\
  -a   same as -t a,  select named characters, ignoring ऊपरी-order bit\n\
"));
      oputs (_("\
  -b   same as -t o1, select octal bytes\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select अचिन्हित decimal 2-byte units\n\
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
      ख_माला_दो (_("\
\n\
\n\
TYPE is made up of one or more of these specifications:\n\
  a          named character, ignoring ऊपरी-order bit\n\
  c          printable character or backslash escape\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  d[आकार_2]    चिन्हित decimal, आकार_2 bytes per integer\n\
  f[आकार_2]    floating point, आकार_2 bytes per भग्न\n\
  o[आकार_2]    octal, आकार_2 bytes per integer\n\
  u[आकार_2]    अचिन्हित decimal, आकार_2 bytes per integer\n\
  x[आकार_2]    hexadecimal, आकार_2 bytes per integer\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
आकार_2 is a संख्या.  For TYPE in [doux], आकार_2 may also be C क्रम\n\
माप(अक्षर), S क्रम माप(लघु), I क्रम माप(पूर्णांक) or L क्रम\n\
माप(दीर्घ).  If TYPE is f, आकार_2 may also be B क्रम Brain 16 bit,\n\
H क्रम Half precision भग्न, F क्रम माप(भग्न), D क्रम माप(द्विगुन),\n\
or L क्रम माप(दीर्घ द्विगुन).\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Adding a z suffix to any type displays printable characters at the end of\n\
each output पंक्ति.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
\n\
BYTES is hex with 0x or 0X prefix, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on क्रम G, T, P, E, Z, Y, R, Q.\n\
द्विआधारी_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
अटल व्योम
print_n_spaces (intmax_t n)
{
  क्रम (; 0 < n; n--)
    अक्षर_दो (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
अटल idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  वापस pad / fields * i + m * i / fields;
}
अटल bool
pad_at_overflow (idx_t fields)
{
  intmax_t product;
  वापस ckd_mul (&product, fields - 1, fields - 1);
}

#घोषणा PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
अटल व्योम                                                             \
N (idx_t fields, idx_t blank, व्योम स्थिर *block,			\
   FMT_STRING_DECL, पूर्णांक width, idx_t pad)				\
{                                                                       \
  T स्थिर *p = block;                                                   \
  idx_t pad_remaining = pad;						\
  क्रम (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t अग्र_pad = pad_at (fields, i - 1, pad);			\
      पूर्णांक adjusted_width = pad_remaining - अग्र_pad + width;            \
      T x;                                                              \
      अगर (निवेश_swap && माप (T) > 1)                                 \
        {                                                               \
          जोड़ {                                                       \
            T x;                                                        \
            अक्षर b[माप (T)];                                         \
          } u;                                                          \
          क्रम (idx_t j = 0; j < माप (T); j++)			\
            u.b[j] = ((अक्षर स्थिर *) p)[माप (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      अन्यथा                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_remaining = अग्र_pad;                                         \
    }                                                                   \
}

#घोषणा PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, अक्षर स्थिर *fmt_string,                           \
                xprintf (fmt_string, adjusted_width, x))

#घोषणा PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_UNUSED अक्षर स्थिर *fmt_string,              \
                अक्षर buf[BUFSIZE];                                      \
                FTOASTR (buf, माप buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_width, buf))

PRINT_TYPE (print_s_char, चिन्हित अक्षर)
PRINT_TYPE (print_char, अचिन्हित अक्षर)
PRINT_TYPE (print_s_short, लघु पूर्णांक)
PRINT_TYPE (print_short, अचिन्हित लघु पूर्णांक)
PRINT_TYPE (print_int, अचिन्हित पूर्णांक)
PRINT_TYPE (print_long, अचिन्हित दीर्घ पूर्णांक)
PRINT_TYPE (print_long_long, अचिन्हित दीर्घ दीर्घ पूर्णांक)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, भग्न, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, द्विगुन, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, दीर्घ द्विगुन, ldtoastr, LDBL_BUFSIZE_BOUND)

#अघोषित PRINT_TYPE
#अघोषित PRINT_FLOATTYPE

अटल व्योम
dump_hexl_mode_trailer (idx_t n_bytes, अक्षर स्थिर *block)
{
  ख_माला_दो ("  >", मानक_निकास);
  क्रम (idx_t i = n_bytes; i > 0; i--)
    {
      अचिन्हित अक्षर c = *block++;
      अचिन्हित अक्षर c2 = (है_छाप (c) ? c : '.');
      अक्षर_दो (c2);
    }
  अक्षर_दो ('<');
}

अटल व्योम
print_named_ascii (idx_t fields, idx_t blank, व्योम स्थिर *block,
                   MAYBE_UNUSED अक्षर स्थिर *unused_fmt_string,
                   पूर्णांक width, idx_t pad)
{
  अचिन्हित अक्षर स्थिर *p = block;
  idx_t pad_remaining = pad;
  क्रम (idx_t i = fields; blank < i; i--)
    {
      पूर्णांक masked_c = *p++ & 0x7f;
      अक्षर स्थिर *s;
      अक्षर buf[2];

      अगर (masked_c == 127)
        s = "del";
      अन्यथा अगर (masked_c <= 040)
        s = charname[masked_c];
      अन्यथा
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t अग्र_pad = pad_at (fields, i - 1, pad);
      पूर्णांक adjusted_width = pad_remaining - अग्र_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = अग्र_pad;
    }
}

अटल व्योम
print_ascii (idx_t fields, idx_t blank, व्योम स्थिर *block,
             MAYBE_UNUSED अक्षर स्थिर *unused_fmt_string, पूर्णांक width,
             idx_t pad)
{
  अचिन्हित अक्षर स्थिर *p = block;
  idx_t pad_remaining = pad;
  क्रम (idx_t i = fields; blank < i; i--)
    {
      अचिन्हित अक्षर c = *p++;
      अक्षर स्थिर *s;
      अक्षर buf[4];

      चयन (c)
        {
        हाल '\0':
          s = "\\0";
          अवरोध;

        हाल '\a':
          s = "\\a";
          अवरोध;

        हाल '\b':
          s = "\\b";
          अवरोध;

        हाल '\f':
          s = "\\f";
          अवरोध;

        हाल '\n':
          s = "\\n";
          अवरोध;

        हाल '\r':
          s = "\\r";
          अवरोध;

        हाल '\t':
          s = "\\t";
          अवरोध;

        हाल '\v':
          s = "\\v";
          अवरोध;

        शेष:
          प्र_लिखो (buf, (है_छाप (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t अग्र_pad = pad_at (fields, i - 1, pad);
      पूर्णांक adjusted_width = pad_remaining - अग्र_pad + width;
      xprintf ("%*s", adjusted_width, s);
      pad_remaining = अग्र_pad;
    }
}

/* Convert a null-terminated (possibly zero-length) string S to an
   int value.  If S points to a non-digit set *P to S,
   *VAL to 0, and return true.  Otherwise, accumulate the integer value of
   the string of digits.  If the string of digits represents a value
   larger than INT_MAX, don't modify *VAL or *P and return false.
   Otherwise, advance *P to the first non-digit after S, set *VAL to
   the result of the conversion and return true.  */

अटल bool
simple_strtoi (अक्षर स्थिर *s, अक्षर स्थिर **p, पूर्णांक *val)
{
  पूर्णांक योग;

  क्रम (योग = 0; c_isdigit (*s); s++)
    अगर (ckd_mul (&योग, योग, 10) || ckd_add (&योग, योग, *s - '0'))
      वापस false;
  *p = s;
  *val = योग;
  वापस true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
अटल अक्षर स्थिर *
ispec_to_format (क्रमागत आकार_spec आकार_spec,
                 अक्षर स्थिर *int_format,
                 अक्षर स्थिर *long_format,
                 अक्षर स्थिर *long_long_format,
                 अक्षर स्थिर *intmax_format)
{
  वापस (LONG < INTMAX && आकार_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && आकार_spec == LONG_LONG ? long_long_format
          : INT < LONG && आकार_spec == LONG ? long_format
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

अटल bool ATTRIBUTE_NONNULL ()
decode_one_format (अक्षर स्थिर *s_orig, अक्षर स्थिर *s, अक्षर स्थिर **अग्र,
                   काष्ठा tspec *tspec)
{
  क्रमागत आकार_spec आकार_spec;
  पूर्णांक आकार;
  क्रमागत output_format fmt;
  print_function_type print_function;
  अक्षर स्थिर *p;
  अक्षर c;
  पूर्णांक field_width;

  चयन (*s)
    {
    हाल 'd':
    हाल 'o':
    हाल 'u':
    हाल 'x':
      c = *s;
      ++s;
      चयन (*s)
        {
        हाल 'C':
          ++s;
          आकार = माप (अचिन्हित अक्षर);
          अवरोध;

        हाल 'S':
          ++s;
          आकार = माप (अचिन्हित लघु पूर्णांक);
          अवरोध;

        हाल 'I':
          ++s;
          आकार = माप (अचिन्हित पूर्णांक);
          अवरोध;

        हाल 'L':
          ++s;
          आकार = माप (अचिन्हित दीर्घ पूर्णांक);
          अवरोध;

        शेष:
          अगर (! simple_strtoi (s, &p, &आकार))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              वापस false;
            }
          अगर (p == s)
            आकार = माप (अचिन्हित पूर्णांक);
          अन्यथा
            {
              अगर (countof (integral_type_आकार) <= आकार
                  || integral_type_आकार[आकार] == NO_आकार)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), आकार);
                  वापस false;
                }
              s = p;
            }
          अवरोध;
        }

      आकार_spec = integral_type_आकार[आकार];

      चयन (c)
        {
        हाल 'd':
          fmt = SIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (अक्षर_बिट * आकार - 1) + 1;
          प्र_लिखो (tspec->fmt_string, "%%*%s",
                   ispec_to_format (आकार_spec, "d", "ld", "lld", "jd"));
          अवरोध;

        हाल 'o':
          fmt = OCTAL;
          field_width = (अक्षर_बिट * आकार + 2) / 3;
          प्र_लिखो (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (आकार_spec, "o", "lo", "llo", "jo"));
          अवरोध;

        हाल 'u':
          fmt = UNSIGNED_DECIMAL;
          field_width = INT_BITS_STRLEN_BOUND (अक्षर_बिट * आकार);
          प्र_लिखो (tspec->fmt_string, "%%*%s",
                   ispec_to_format (आकार_spec, "u", "lu", "llu", "ju"));
          अवरोध;

        हाल 'x':
          fmt = HEXADECIMAL;
          field_width = (अक्षर_बिट * आकार + 3) / 4;
          प्र_लिखो (tspec->fmt_string, "%%*.%d%s", field_width,
                   ispec_to_format (आकार_spec, "x", "lx", "llx", "jx"));
          अवरोध;

        शेष:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (आकार_spec == INT
           ? print_int
           : SHORT < INT && आकार_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && आकार_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && आकार_spec == LONG
           ? print_long
           : LONG < INTMAX && आकार_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && आकार_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) शून्य));
      अवरोध;

    हाल 'f':
      fmt = FLOATING_POINT;
      ++s;
      चयन (*s)
        {
        हाल 'B':
          ++s;
          fmt = BFLOATING_POINT;
          आकार = माप (bfloat16);
          अवरोध;

        हाल 'H':
          ++s;
          fmt = HFLOATING_POINT;
          आकार = माप (float16);
          अवरोध;

        हाल 'F':
          ++s;
          आकार = माप (भग्न);
          अवरोध;

        हाल 'D':
          ++s;
          आकार = माप (द्विगुन);
          अवरोध;

        हाल 'L':
          ++s;
          आकार = माप (दीर्घ द्विगुन);
          अवरोध;

        शेष:
          अगर (! simple_strtoi (s, &p, &आकार))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              वापस false;
            }
          अगर (p == s)
            आकार = माप (द्विगुन);
          अन्यथा
            {
              अगर (countof (fp_type_आकार) <= आकार
                  || fp_type_आकार[आकार] == NO_आकार
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && आकार == माप (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), आकार);
                  वापस false;
                }
              s = p;
            }
          अवरोध;
        }
      आकार_spec = fp_type_आकार[आकार];

      अगर ((! FLOAT16_SUPPORTED && fmt == HFLOATING_POINT)
          || (! BF16_SUPPORTED && fmt == BFLOATING_POINT))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        वापस false;
      }

      {
        काष्ठा स्था_बदल स्थिर *locale = क्षेत्र_बदलो ();
        idx_t decimal_point_len =
          (locale->decimal_point[0] ? म_माप (locale->decimal_point) : 1);

        अगर (आकार_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            field_width = DBL_STRLEN_BOUND_L (decimal_point_len);
          }
        अन्यथा अगर (FLOAT_SINGLE < FLOAT_DOUBLE && आकार_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        अन्यथा अगर (FLOAT_HALF < FLOAT_SINGLE && आकार_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_POINT
                             ? print_bfloat : print_halffloat;
            field_width = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        अन्यथा अगर (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && आकार_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            field_width = LDBL_STRLEN_BOUND_L (decimal_point_len);
          }
        अन्यथा
          affirm (false);

        अवरोध;
      }

    हाल 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      आकार_spec = CHAR;
      print_function = print_named_ascii;
      field_width = 3;
      अवरोध;

    हाल 'c':
      ++s;
      fmt = CHARACTER;
      आकार_spec = CHAR;
      print_function = print_ascii;
      field_width = 3;
      अवरोध;

    शेष:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      वापस false;
    }

  tspec->आकार = आकार_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->field_width = field_width;
  tspec->hexl_mode_trailer = (*s == 'z');
  अगर (tspec->hexl_mode_trailer)
    s++;

  *अग्र = s;
  वापस true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

अटल bool
open_अग्र_file (व्योम)
{
  bool ok = true;

  करो
    {
      निवेश_filename = *file_list;
      अगर (निवेश_filename == शून्य)
        वापस ok;
      ++file_list;

      अगर (streq (निवेश_filename, "-"))
        {
          निवेश_filename = _("standard input");
          in_stream = मानक_निवेश;
          have_read_stdin = true;
          xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
        }
      अन्यथा
        {
          in_stream = ख_खोलो (निवेश_filename, (O_द्विआधारी ? "rb" : "r"));
          अगर (in_stream == शून्य)
            {
              error (0, त्रुटि_सं, "%s", quotef (निवेश_filename));
              ok = false;
            }
        }
    }
  जबतक (in_stream == शून्य);

  अगर (0 <= end_offset && !flag_dump_strings)
    रखो_भबफ (in_stream, शून्य, _IONBF, 0);

  वापस ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

अटल bool
check_and_close (पूर्णांक in_errno)
{
  bool ok = true;

  अगर (in_stream != शून्य)
    {
      अगर (!ख_त्रुटि (in_stream))
        in_errno = 0;
      अगर (streq (file_list[-1], "-"))
        साफ_त्रुटि (in_stream);
      अन्यथा अगर (ख_बंद (in_stream) != 0 && !in_errno)
        in_errno = त्रुटि_सं;
      अगर (in_errno)
        {
          error (0, in_errno, "%s", quotef (निवेश_filename));
          ok = false;
        }

      in_stream = शून्य;
    }

  अगर (ख_त्रुटि (मानक_निकास))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  वापस ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

अटल bool ATTRIBUTE_NONNULL ()
decode_format_string (अक्षर स्थिर *s)
{
  अक्षर स्थिर *s_orig = s;

  जबतक (*s != '\0')
    {
      अक्षर स्थिर *अग्र;

      अगर (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, माप *spec);

      अगर (! decode_one_format (s_orig, s, &अग्र, &spec[n_specs]))
        वापस false;

      affirm (s != अग्र);
      s = अग्र;
      ++n_specs;
    }

  वापस true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

अटल bool
skip (intmax_t n_skip)
{
  bool ok = true;
  पूर्णांक in_errno = 0;

  अगर (n_skip == 0)
    वापस true;

  जबतक (in_stream != शून्य)	/* EOF.  */
    {
      काष्ठा stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      अगर (ख_स्थिति (fileno (in_stream), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          अगर (S_ISREG (file_stats.st_mode)
              && STP_BLKSIZE (&file_stats) < file_stats.st_आकार)
            {
              अगर (file_stats.st_आकार < n_skip)
                n_skip -= file_stats.st_आकार;
              अन्यथा
                {
                  अगर (fseeko (in_stream, n_skip, प्रस्तुत_से) != 0)
                    {
                      in_errno = त्रुटि_सं;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          अन्यथा अगर (! (S_ISREG (file_stats.st_mode)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (in_stream, n_skip, प्रस्तुत_से) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          अन्यथा
            {
              अक्षर buf[बफ_मान];
              idx_t n_bytes_read, n_bytes_to_read = बफ_मान;

              जबतक (0 < n_skip)
                {
                  अगर (n_skip < n_bytes_to_read)
                    n_bytes_to_read = n_skip;
                  n_bytes_read = ख_पढ़ो (buf, 1, n_bytes_to_read, in_stream);
                  n_skip -= n_bytes_read;
                  अगर (n_bytes_read != n_bytes_to_read)
                    {
                      अगर (ख_त्रुटि (in_stream))
                        {
                          in_errno = त्रुटि_सं;
                          ok = false;
                          n_skip = 0;
                          अवरोध;
                        }
                      अगर (ख_पूर्ण (in_stream))
                        अवरोध;
                    }
                }
            }

          अगर (n_skip == 0)
            अवरोध;
        }

      अन्यथा   /* cannot fstat() file */
        {
          error (0, त्रुटि_सं, "%s", quotef (निवेश_filename));
          ok = false;
        }

      ok &= check_and_close (in_errno);

      ok &= open_अग्र_file ();
    }

  अगर (n_skip != 0)
    error (निकास_त्रुटि, 0, _("cannot skip past end of combined input"));

  वापस ok;
}

अटल व्योम
format_address_none (MAYBE_UNUSED intmax_t address,
                     MAYBE_UNUSED अक्षर c)
{
}

अटल व्योम
format_address_std (intmax_t address, अक्षर c)
{
  अक्षर buf[MAX_ADDRESS_लंबाई + 2];
  अक्षर *p = buf + माप buf;
  अक्षर स्थिर *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - address_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  चयन (address_base)
    {
    हाल 8:
      करो
        *--p = '0' + (address & 7);
      जबतक ((address >>= 3) != 0);
      अवरोध;

    हाल 10:
      करो
        *--p = '0' + (address % 10);
      जबतक ((address /= 10) != 0);
      अवरोध;

    हाल 16:
      करो
        *--p = "0123456789abcdef"[address & 15];
      जबतक ((address >>= 4) != 0);
      अवरोध;
    }

  जबतक (pbound < p)
    *--p = '0';

  ख_माला_दो (p, मानक_निकास);
}

अटल व्योम
format_address_paren (intmax_t address, अक्षर c)
{
  अक्षर_दो ('(');
  format_address_std (address, ')');
  अगर (c)
    अक्षर_दो (c);
}

अटल व्योम
format_address_label (intmax_t address, अक्षर c)
{
  format_address_std (address, ' ');

  intmax_t addr;
  अगर (ckd_add (&addr, address, pseudo_offset))
    error (निकास_त्रुटि, 0, _("pseudo address too large for input"));

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

अटल व्योम
write_block (intmax_t current_offset, idx_t n_bytes,
             अक्षर स्थिर *पूर्व_block, अक्षर स्थिर *curr_block)
{
  अटल bool first = true;
  अटल bool पूर्व_pair_equal = false;

  अगर (abbreviate_duplicate_blocks
      && !first && n_bytes == bytes_per_block
      && memeq (पूर्व_block, curr_block, bytes_per_block))
    {
      अगर (पूर्व_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      अन्यथा
        {
          म_लिखो ("*\n");
          पूर्व_pair_equal = true;
        }
    }
  अन्यथा
    {
      पूर्व_pair_equal = false;
      क्रम (idx_t i = 0; i < n_specs; i++)
        {
          पूर्णांक datum_width = width_bytes[spec[i].आकार];
          idx_t fields_per_block = bytes_per_block / datum_width;
          idx_t blank_fields = (bytes_per_block - n_bytes) / datum_width;
          अगर (i == 0)
            format_address (current_offset, '\0');
          अन्यथा
            print_n_spaces (address_pad_len);
          (*spec[i].print_function) (fields_per_block, blank_fields,
                                     curr_block, spec[i].fmt_string,
                                     spec[i].field_width, spec[i].pad_width);
          अगर (spec[i].hexl_mode_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              पूर्णांक field_width = spec[i].field_width;
              क्रम (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (field_width);
              idx_t pad_width = pad_at (fields_per_block, blank_fields,
                                        spec[i].pad_width);
              print_n_spaces (pad_width);
              dump_hexl_mode_trailer (n_bytes, curr_block);
            }
          अक्षर_दो ('\n');
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

अटल bool
read_char (पूर्णांक *c)
{
  bool ok = true;

  *c = खातापूर्ण;

  जबतक (in_stream && (*c = अ_लो (in_stream)) < 0)
    {
      ok &= check_and_close (त्रुटि_सं);
      ok &= open_अग्र_file ();
    }

  वापस ok;
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

अटल bool
read_block (idx_t n, अक्षर *block, idx_t *n_bytes_in_बफर)
{
  bool ok = true;

  affirm (0 < n && n <= bytes_per_block);

  *n_bytes_in_बफर = 0;

  जबतक (in_stream != शून्य)	/* EOF.  */
    {
      idx_t n_needed = n - *n_bytes_in_बफर;
      idx_t n_read = ख_पढ़ो (block + *n_bytes_in_बफर,
                            1, n_needed, in_stream);

      *n_bytes_in_बफर += n_read;

      अगर (n_read == n_needed)
        अवरोध;

      ok &= check_and_close (त्रुटि_सं);

      ok &= open_अग्र_file ();
    }

  वापस ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_PURE
अटल पूर्णांक
get_lcm (व्योम)
{
  पूर्णांक l_c_m = 1;

  क्रम (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, width_bytes[spec[i].आकार]);
  वापस l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
अटल strtol_error
xstr2nonneg (अक्षर स्थिर *restrict nptr, पूर्णांक base, intmax_t *val,
             अक्षर स्थिर *restrict valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, शून्य, base, val, valid_suffixes);
  वापस s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

अटल bool
parse_old_offset (अक्षर *str, intmax_t *offset)
{
  /* Skip over any leading '+'.  */
  अक्षर *s = str + (str[0] == '+');

  अगर (! c_isdigit (s[0]))
    वापस false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  अक्षर *dot = म_अक्षर (s, '.');
  अगर (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = शून्य;
  पूर्णांक radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  अगर (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  क्रमागत strtol_error s_err = xstr2nonneg (s, radix, offset, "bB");

  अगर (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  अगर (s_err == LONGINT_OVERFLOW)
    error (निकास_त्रुटि, दुस्फल, "%s", quotef (str));
  वापस s_err == LONGINT_OK;
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

अटल bool
dump (व्योम)
{
  अक्षर *block[2];
  bool idx = false;
  bool ok = true;
  idx_t n_bytes_read;

  block[0] = xinmalloc (1 + abbreviate_duplicate_blocks, bytes_per_block);
  block[1] = block[0] + abbreviate_duplicate_blocks * bytes_per_block;

  intmax_t current_offset = n_bytes_to_skip;

  करो
    {
      intmax_t needed_bound
        = end_offset < 0 ? INTMAX_MAX : end_offset - current_offset;
      अगर (needed_bound <= 0)
        {
          n_bytes_read = 0;
          अवरोध;
        }
      idx_t n_needed = MIN (bytes_per_block, needed_bound);
      ok &= read_block (n_needed, block[idx], &n_bytes_read);
      अगर (n_bytes_read < bytes_per_block)
        अवरोध;
      affirm (n_bytes_read == bytes_per_block);
      write_block (current_offset, n_bytes_read,
                   block[!idx], block[idx]);
      अगर (ख_त्रुटि (मानक_निकास))
        ok = false;
      current_offset += n_bytes_read;
      idx = !idx;
    }
  जबतक (ok);

  अगर (n_bytes_read > 0)
    {
      पूर्णांक l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t bytes_to_write = (n_bytes_read + l_c_m - 1
                              - (n_bytes_read + l_c_m - 1) % l_c_m);

      स_रखो (block[idx] + n_bytes_read, 0, bytes_to_write - n_bytes_read);
      write_block (current_offset, n_bytes_read, block[!idx], block[idx]);
      current_offset += n_bytes_read;
    }

  अगर (ok || current_offset)
    format_address (current_offset, '\n');

  अगर (0 <= end_offset && end_offset <= current_offset)
    ok &= check_and_close (0);

  मुक्त (block[0]);

  वापस ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

अटल bool
dump_strings (व्योम)
{
  idx_t bufsize = MAX (100, string_min + 1);
  अक्षर *buf = ximalloc (bufsize);
  intmax_t address = n_bytes_to_skip;
  bool ok = true;

  जबतक (true)
    {
      idx_t i = 0;
      पूर्णांक c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      अगर (0 <= end_offset
          && (end_offset < string_min || end_offset - string_min < address))
        अवरोध;

      /* Store consecutive printable characters to BUF.  */
      जबतक (! (0 <= end_offset && end_offset <= address))
        {
          अगर (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, माप *buf);
          ok &= read_char (&c);
          अगर (c < 0)
            {
              मुक्त (buf);
              वापस ok;
            }
          address++;
          buf[i++] = c;
          अगर (c == '\0')
            अवरोध;		/* Print this string.  */
          अगर (! है_छाप (c))
            {
              c = -1;		/* Give up on this string.  */
              अवरोध;
            }
        }

      अगर (c < 0 || i - !c < string_min)
        जारी;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_address (address - i, ' ');

      क्रम (i = 0; (c = buf[i]); i++)
        {
          चयन (c)
            {
            हाल '\a':
              ख_माला_दो ("\\a", मानक_निकास);
              अवरोध;

            हाल '\b':
              ख_माला_दो ("\\b", मानक_निकास);
              अवरोध;

            हाल '\f':
              ख_माला_दो ("\\f", मानक_निकास);
              अवरोध;

            हाल '\n':
              ख_माला_दो ("\\n", मानक_निकास);
              अवरोध;

            हाल '\r':
              ख_माला_दो ("\\r", मानक_निकास);
              अवरोध;

            हाल '\t':
              ख_माला_दो ("\\t", मानक_निकास);
              अवरोध;

            हाल '\v':
              ख_माला_दो ("\\v", मानक_निकास);
              अवरोध;

            शेष:
              अ_दो (c, मानक_निकास);
            }
        }
      अक्षर_दो ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  मुक्त (buf);

  ok &= check_and_close (0);
  वापस ok;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक n_files;
  पूर्णांक l_c_m;
  idx_t desired_width = 0;
  bool modern = false;
  bool ok = true;
  idx_t width_per_block = 0;
  अटल अक्षर स्थिर multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_bytes_to_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_start IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक (true)
    {
      intmax_t tmp;
      क्रमागत strtol_error s_err;
      पूर्णांक oi = -1;
      पूर्णांक c = getopt_long (argc, argv, short_options, long_options, &oi);
      अगर (c == -1)
        अवरोध;

      चयन (c)
        {
        हाल 'A':
          modern = true;
          चयन (optarg[0])
            {
            हाल 'd':
              format_address = format_address_std;
              address_base = 10;
              address_pad_len = 7;
              अवरोध;
            हाल 'o':
              format_address = format_address_std;
              address_base = 8;
              address_pad_len = 7;
              अवरोध;
            हाल 'x':
              format_address = format_address_std;
              address_base = 16;
              address_pad_len = 6;
              अवरोध;
            हाल 'n':
              format_address = format_address_none;
              address_pad_len = 0;
              अवरोध;
            शेष:
              error (निकास_त्रुटि, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              अवरोध;
            }
          अवरोध;

        हाल 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_bytes_to_skip, multipliers);
          अगर (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          अवरोध;

        हाल 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_bytes_to_format, multipliers);
          अगर (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          अवरोध;

        हाल 'S':
          modern = true;
          अगर (optarg == शून्य)
            string_min = 3;
          अन्यथा
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              अगर (s_err == LONGINT_OK && ckd_add (&i, tmp, 1))
                s_err = LONGINT_OVERFLOW;
              अगर (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_strings = true;
          अवरोध;

        हाल 't':
          modern = true;
          ok &= decode_format_string (optarg);
          अवरोध;

        हाल 'v':
          modern = true;
          abbreviate_duplicate_blocks = false;
          अवरोध;

        हाल TRADITIONAL_OPTION:
          traditional = true;
          अवरोध;

        हाल ENDIAN_OPTION:
          चयन (XARGMATCH ("--endian", optarg, endian_args, endian_types))
            {
            हाल endian_big:
              निवेश_swap = BYTE_ORDER != BIG_ENDIAN;
              अवरोध;
            हाल endian_little:
              निवेश_swap = BYTE_ORDER != LITTLE_ENDIAN;
              अवरोध;
            }
          अवरोध;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#घोषणा CASE_OLD_ARG(old_char,new_string)		\
        हाल old_char:					\
          ok &= decode_format_string (new_string);	\
          अवरोध

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        हाल 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        हाल 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        हाल 'I': हाल 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        हाल 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        हाल 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#अघोषित CASE_OLD_ARG

        हाल 'w':
          modern = true;
          अगर (optarg == शून्य)
            {
              desired_width = 32;
            }
          अन्यथा
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              अगर (s_err == LONGINT_OK)
                {
                  अगर (ckd_add (&desired_width, w_tmp, 0))
                    s_err = LONGINT_OVERFLOW;
                  अन्यथा अगर (desired_width == 0)
                    s_err = LONGINT_INVALID;
                }
              अगर (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
          अवरोध;
        }
    }

  अगर (!ok)
    वापस निकास_त्रुटि;

  अगर (flag_dump_strings && n_specs > 0)
    error (निकास_त्रुटि, 0,
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

  अगर (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      चयन (n_files)
        {
        हाल 1:
          अगर ((traditional || argv[optind][0] == '+')
              && parse_old_offset (argv[optind], &o1))
            {
              n_bytes_to_skip = o1;
              --n_files;
              ++argv;
            }
          अवरोध;

        हाल 2:
          अगर ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_offset (argv[optind + 1], &o2))
            {
              अगर (traditional && parse_old_offset (argv[optind], &o1))
                {
                  n_bytes_to_skip = o1;
                  flag_pseudo_start = true;
                  pseudo_start = o2;
                  argv += 2;
                  n_files -= 2;
                }
              अन्यथा
                {
                  n_bytes_to_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          अवरोध;

        हाल 3:
          अगर (traditional
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
          अवरोध;
        }

      अगर (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (निकास_त्रुटि);
        }
    }

  अगर (flag_pseudo_start)
    {
      अगर (format_address == format_address_none)
        {
          address_base = 8;
          address_pad_len = 7;
          format_address = format_address_paren;
        }
      अन्यथा
        format_address = format_address_label;
    }

  अगर (0 <= max_bytes_to_format
      && ckd_add (&end_offset, n_bytes_to_skip, max_bytes_to_format))
    error (निकास_त्रुटि, 0, _("skip-bytes + read-bytes is too large"));

  अगर (n_specs == 0)
    decode_format_string ("oS");

  अगर (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (अक्षर स्थिर *स्थिर *) &argv[optind];
    }
  अन्यथा
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_अग्र_file ();
  अगर (in_stream == शून्य)
    जाओ cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_bytes_to_skip);
  अगर (in_stream == शून्य)
    जाओ cleanup;

  pseudo_offset = (flag_pseudo_start ? pseudo_start - n_bytes_to_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  अगर (desired_width != 0)
    {
      अगर (desired_width % l_c_m == 0)
        bytes_per_block = desired_width;
      अन्यथा
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_width, l_c_m);
          bytes_per_block = l_c_m;
        }
    }
  अन्यथा
    {
      अगर (l_c_m < DEFAULT_BYTES_PER_BLOCK)
        bytes_per_block = (DEFAULT_BYTES_PER_BLOCK
                           - DEFAULT_BYTES_PER_BLOCK % l_c_m);
      अन्यथा
        bytes_per_block = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  क्रम (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].आकार];
      अगर (pad_at_overflow (fields_per_block))
        error (निकास_त्रुटि, 0, _("%td is too large"), desired_width);
      idx_t block_width = (spec[i].field_width + 1) * fields_per_block;
      अगर (width_per_block < block_width)
        width_per_block = block_width;
    }
  क्रम (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].आकार];
      idx_t block_width = spec[i].field_width * fields_per_block;
      spec[i].pad_width = width_per_block - block_width;
    }

#अगर_घोषित DEBUG
  म_लिखो ("lcm=%d, width_per_block=%td\n", l_c_m, width_per_block);
  क्रम (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_per_block = bytes_per_block / width_bytes[spec[i].आकार];
      affirm (bytes_per_block % width_bytes[spec[i].आकार] == 0);
      affirm (1 <= spec[i].pad_width / fields_per_block);
      म_लिखो ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, width_bytes[spec[i].आकार],
              spec[i].field_width, spec[i].pad_width);
    }
#पूर्ण_अगर

  ok &= (flag_dump_strings ? dump_strings () : dump ());

cleanup:

  अगर (have_read_stdin && ख_बंद (मानक_निवेश) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("standard input"));

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
