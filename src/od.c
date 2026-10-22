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
#परिभाषय क्रमादेशः_NAME "od"

#परिभाषय AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#परिभाषय DEFAULT_अष्टकानि_प्रत्येकम्_खण्डम् 16

#यदि FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  प्रकारनिर्देश _Float16 float16;
#अन्यथा
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  प्रकारनिर्देश भिन्न float16;
#यदि_अन्तः

#यदि BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  प्रकारनिर्देश __bf16 bfloat16;
#अन्यथा
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  प्रकारनिर्देश भिन्न bfloat16;
#यदि_अन्तः

गणना आकार_spec
  {
    NO_आकार = 0,
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
    N_आकार_SPECS
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

गणना { MAX_INTEGRAL_TYPE_विस्तारः = UINTMAX_विस्तारः };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
गणना
  {
    FMT_अष्टकानि_ALLOCATED =
      परिमाण "%*.%dlld" - परिमाण "%d" + INT_STRLEN_BOUND (पूर्णाङ्क) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_विस्तारः <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
प्रकारनिर्देश शून्य (*print_function_type)
  (idx_t fields, idx_t blank, शून्य ध्रुव *दत्त,
   वर्ण ध्रुव *fmt, पूर्णाङ्क विस्तारः, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
संरचना tspec
  {
    गणना output_format fmt;
    गणना आकार_spec आकार; /* Type of input object.  */
    print_function_type print_function;
    वर्ण fmt_string[FMT_अष्टकानि_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_बहुलकम्_trailer;
    पूर्णाङ्क क्षेत्रम्_विस्तारः; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_विस्तारः; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
स्थिर ध्रुव पूर्णाङ्क विस्तारः_अष्टकानि[] =
{
  -1,
  परिमाण (अचिह्नित वर्ण),
#यदि UCHAR_MAX < USHRT_MAX
  परिमाण (अचिह्नित ह्रस्व पूर्णाङ्क),
#यदि_अन्तः
#यदि USHRT_MAX < UINT_MAX
  परिमाण (अचिह्नित पूर्णाङ्क),
#यदि_अन्तः
#यदि UINT_MAX < ULONG_MAX
  परिमाण (अचिह्नित दीर्घ पूर्णाङ्क),
#यदि_अन्तः
#यदि ULONG_MAX < ULLONG_MAX
  परिमाण (अचिह्नित दीर्घ दीर्घ पूर्णाङ्क),
#यदि_अन्तः
#यदि ULLONG_MAX < UINTMAX_MAX
  परिमाण (uintmax_t),
#यदि_अन्तः
#यदि BF16_SUPPORTED
  परिमाण (bfloat16),
#नोचेद्यदि FLOAT16_SUPPORTED
  परिमाण (float16),
#यदि_अन्तः
#यदि FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  परिमाण (भिन्न),
#यदि_अन्तः
  परिमाण (द्विगुण),
#यदि DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  परिमाण (दीर्घ द्विगुण),
#यदि_अन्तः
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (विस्तारः_अष्टकानि) == N_आकार_SPECS);

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
स्थिर पूर्णाङ्क स्थानाङ्कः_आधारः = 8;

/* The number of octal digits required to represent the largest
   address value.  */
गणना { MAX_स्थानाङ्कः_दीर्घता = ((INTMAX_विस्तारः - 1) / 3
                             + ((INTMAX_विस्तारः - 1) % 3 != 0)) };

/* Width of a normal address.  */
स्थिर पूर्णाङ्क स्थानाङ्कः_pad_len = 7;

/* Minimum length when detecting --strings.  */
स्थिर idx_t string_min;

/* True when in --strings mode.  */
स्थिर bool flag_dump_वाक्यानि;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
स्थिर bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
स्थिर bool flag_pseudo_start;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
स्थिर intmax_t pseudo_विस्थापनम्;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
स्थिर शून्य format_स्थानाङ्कः_std (intmax_t, वर्ण);
स्थिर शून्य (*format_स्थानाङ्कः) (intmax_t, वर्ण) = format_स्थानाङ्कः_std;

/* The number of input bytes to skip before formatting and writing.  */
स्थिर intmax_t n_अष्टकानि_गन्तव्यम्_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
स्थिर intmax_t अन्तः_विस्थापनम् = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
स्थिर bool abbreviate_duplicate_खण्डानि = true;

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
स्थिर idx_t अष्टकानि_प्रत्येकम्_खण्डम्;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
स्थिर वर्ण ध्रुव *निवेशः_filename;

/* A null-terminated list of the file-arguments from the command line.  */
स्थिर वर्ण ध्रुव *ध्रुव *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
स्थिर वर्ण ध्रुव *ध्रुव default_file_list[] = {"-", रिक्त};

/* The input stream associated with the current file.  */
स्थिर FILE *अन्तर्_प्रवाहः;

/* If true, at least one of the files we read was standard input.  */
स्थिर bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
स्थिर गणना आकार_spec ध्रुव integral_type_आकार[] =
  {
#यदि UCHAR_MAX < USHRT_MAX
    [परिमाण (अचिह्नित वर्ण)] = CHAR,
#यदि_अन्तः
#यदि USHRT_MAX < UINT_MAX
    [परिमाण (अचिह्नित ह्रस्व पूर्णाङ्क)] = SHORT,
#यदि_अन्तः
    [परिमाण (अचिह्नित पूर्णाङ्क)] = INT,
#यदि UINT_MAX < ULONG_MAX
    [परिमाण (अचिह्नित दीर्घ पूर्णाङ्क)] = LONG,
#यदि_अन्तः
#यदि ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [परिमाण (अचिह्नित दीर्घ दीर्घ पूर्णाङ्क)] = LONG_LONG,
#यदि_अन्तः
#यदि ULONG_MAX < UINTMAX_MAX
    [परिमाण (uintmax_t)] = INTMAX,
#यदि_अन्तः
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
स्थिर गणना आकार_spec ध्रुव fp_type_आकार[] =
  {
#यदि FLOAT16_SUPPORTED
    [परिमाण (float16)] = FLOAT_HALF,
#नोचेद्यदि BF16_SUPPORTED
    [परिमाण (bfloat16)] = FLOAT_HALF,
#यदि_अन्तः
#यदि FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [परिमाण (भिन्न)] = FLOAT_SINGLE,
#यदि_अन्तः
    [परिमाण (द्विगुण)] = FLOAT_DOUBLE,
#यदि DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [परिमाण (दीर्घ द्विगुण)] = FLOAT_LONG_DOUBLE,
#यदि_अन्तः
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

स्थिर वर्ण ध्रुव *ध्रुव endian_तर्काः[] =
{
  "little", "big", रिक्त
};

स्थिर गणना endian_type ध्रुव endian_types[] =
{
  endian_little, endian_big
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"skip-bytes", required_तर्कः, रिक्त, 'j'},
  {"address-radix", required_तर्कः, रिक्त, 'A'},
  {"read-bytes", required_तर्कः, रिक्त, 'N'},
  {"format", required_तर्कः, रिक्त, 't'},
  {"output-duplicates", no_तर्कः, रिक्त, 'v'},
  {"strings", optional_तर्कः, रिक्त, 'S'},
  {"traditional", no_तर्कः, रिक्त, TRADITIONAL_OPTION},
  {"width", optional_तर्कः, रिक्त, 'w'},
  {"endian", required_तर्कः, रिक्त, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]विस्थापनम्_2[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]विस्थापनम्_2[.][b] [+][LABEL][.][b]]\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\n\
Write an unambiguous representation, octal अष्टकानि by स्वभाव,\n\
सम्बन्धि FILE गन्तव्यम् standard output.  With अधिकम् than one FILE तर्कः,\n\
concatenate them अन्तर् the listed क्रमः गन्तव्यम् form the निवेशः.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If first and second call formats both apply, the second format is assumed\n\
यदि the चरमः सङ्कार्यम्_2 begins with + or (यदि there are 2 operands) a digit.\n\
An विस्थापनम्_2 सङ्कार्यम्_2 means -j विस्थापनम्_2.  LABEL is the pseudo-स्थानाङ्कः\n\
at first byte printed, incremented when dump is progressing.\n\
For विस्थापनम्_2 and LABEL, a 0x or 0X उपसर्गः indicates hexadecimal;\n\
suffixes may be . प्रति octal and b प्रति multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --स्थानाङ्कः-radix=RADIX\n\
         output format प्रति file offsets;\n\
         RADIX is one सम्बन्धि [doxn], प्रति Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         swap निवेशः अष्टकानि according the specified क्रमः\n\
"));
      oputs (_("\
  -j, --skip-अष्टकानि=अष्टकानि_2\n\
         skip अष्टकानि_2 निवेशः अष्टकानि first\n\
"));
      oputs (_("\
  -N, --पठ-अष्टकानि=अष्टकानि_2\n\
         सीमा dump गन्तव्यम् अष्टकानि_2 निवेशः अष्टकानि\n\
"));
      oputs (_("\
  -S अष्टकानि_2, --वाक्यानि[=अष्टकानि_2]\n\
         show only NUL terminated वाक्यानि\n\
         सम्बन्धि at न्यूनतमम् अष्टकानि_2 (स्वभाव 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         कुरु not use * गन्तव्यम् mark पङ्क्तिः suppression\n\
"));
      oputs (_("\
  -w[अष्टकानि_2], --विस्तारः[=अष्टकानि_2]\n\
         output अष्टकानि_2 अष्टकानि प्रत्येकम् output पङ्क्तिः;\n\
         32 is implied when अष्टकानि_2 is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments अन्तर् third form above\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
\n\
Traditional format specifications may be intermixed; they accumulate:\n\
"), stdout);
      oputs (_("\
  -a   same as -t a,  select named characters, ignoring उच्चः-क्रमः bit\n\
"));
      oputs (_("\
  -b   same as -t o1, select octal अष्टकानि\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select अचिह्नित decimal 2-byte units\n\
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
TYPE is made ऊर्ध्वम् सम्बन्धि one or अधिकम् सम्बन्धि these specifications:\n\
  a          named character, ignoring उच्चः-क्रमः bit\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[आकार_2]    चिह्नित decimal, आकार_2 अष्टकानि प्रत्येकम् integer\n\
  f[आकार_2]    floating point, आकार_2 अष्टकानि प्रत्येकम् भिन्न\n\
  o[आकार_2]    octal, आकार_2 अष्टकानि प्रत्येकम् integer\n\
  u[आकार_2]    अचिह्नित decimal, आकार_2 अष्टकानि प्रत्येकम् integer\n\
  x[आकार_2]    hexadecimal, आकार_2 अष्टकानि प्रत्येकम् integer\n\
"), stdout);
      fputs (_("\
\n\
आकार_2 is a सङ्ख्या.  For TYPE अन्तर् [doux], आकार_2 may also be C प्रति\n\
परिमाण(वर्ण), S प्रति परिमाण(ह्रस्व), I प्रति परिमाण(पूर्णाङ्क) or L प्रति\n\
परिमाण(दीर्घ).  If TYPE is f, आकार_2 may also be B प्रति Brain 16 bit,\n\
H प्रति Half precision भिन्न, F प्रति परिमाण(भिन्न), D प्रति परिमाण(द्विगुण),\n\
or L प्रति परिमाण(दीर्घ द्विगुण).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix गन्तव्यम् any type displays printable characters at the अन्तः सम्बन्धि\n\
each output पङ्क्तिः.\n\
"), stdout);
      fputs (_("\
\n\
\n\
अष्टकानि_2 is hex with 0x or 0X उपसर्गः, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on प्रति G, T, P, E, Z, Y, R, Q.\n\
द्विमान_2 prefixes can be प्रयुक्तम्, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
स्थिर शून्य
print_n_spaces (intmax_t n)
{
  प्रति (; 0 < n; n--)
    अक्षरं_लिख (' ');
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
  प्रत्यागम pad / fields * i + m * i / fields;
}
स्थिर bool
pad_at_अतिप्रवाहः (idx_t fields)
{
  intmax_t गुणनफलम्;
  प्रत्यागम ckd_mul (&गुणनफलम्, fields - 1, fields - 1);
}

#परिभाषय PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
स्थिर शून्य                                                             \
N (idx_t fields, idx_t blank, शून्य ध्रुव *खण्डम्,			\
   FMT_STRING_DECL, पूर्णाङ्क विस्तारः, idx_t pad)				\
{                                                                       \
  T ध्रुव *p = खण्डम्;                                                   \
  idx_t pad_remaining = pad;						\
  प्रति (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t अग्र_pad = pad_at (fields, i - 1, pad);			\
      पूर्णाङ्क adjusted_विस्तारः = pad_remaining - अग्र_pad + विस्तारः;            \
      T x;                                                              \
      यदि (निवेशः_swap && परिमाण (T) > 1)                                 \
        {                                                               \
          सङ्घ {                                                       \
            T x;                                                        \
            वर्ण b[परिमाण (T)];                                         \
          } u;                                                          \
          प्रति (idx_t j = 0; j < परिमाण (T); j++)			\
            u.b[j] = ((वर्ण ध्रुव *) p)[परिमाण (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      अन्यथा                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_remaining = अग्र_pad;                                         \
    }                                                                   \
}

#परिभाषय PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, वर्ण ध्रुव *fmt_string,                           \
                xprintf (fmt_string, adjusted_विस्तारः, x))

#परिभाषय PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_अप्रयुक्तम् वर्ण ध्रुव *fmt_string,              \
                वर्ण buf[BUFSIZE];                                      \
                FTOASTR (buf, परिमाण buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_विस्तारः, buf))

PRINT_TYPE (print_s_char, चिह्नित वर्ण)
PRINT_TYPE (print_char, अचिह्नित वर्ण)
PRINT_TYPE (print_s_short, ह्रस्व पूर्णाङ्क)
PRINT_TYPE (print_short, अचिह्नित ह्रस्व पूर्णाङ्क)
PRINT_TYPE (print_int, अचिह्नित पूर्णाङ्क)
PRINT_TYPE (print_long, अचिह्नित दीर्घ पूर्णाङ्क)
PRINT_TYPE (print_long_long, अचिह्नित दीर्घ दीर्घ पूर्णाङ्क)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, भिन्न, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, द्विगुण, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, दीर्घ द्विगुण, ldtoastr, LDBL_BUFSIZE_BOUND)

#अपरिभाषय PRINT_TYPE
#अपरिभाषय PRINT_FLOATTYPE

स्थिर शून्य
dump_hexl_बहुलकम्_trailer (idx_t n_अष्टकानि, वर्ण ध्रुव *खण्डम्)
{
  fputs ("  >", stdout);
  प्रति (idx_t i = n_अष्टकानि; i > 0; i--)
    {
      अचिह्नित वर्ण c = *खण्डम्++;
      अचिह्नित वर्ण c2 = (isprint (c) ? c : '.');
      अक्षरं_लिख (c2);
    }
  अक्षरं_लिख ('<');
}

स्थिर शून्य
print_named_ascii (idx_t fields, idx_t blank, शून्य ध्रुव *खण्डम्,
                   MAYBE_अप्रयुक्तम् वर्ण ध्रुव *अप्रयुक्तम्_fmt_string,
                   पूर्णाङ्क विस्तारः, idx_t pad)
{
  अचिह्नित वर्ण ध्रुव *p = खण्डम्;
  idx_t pad_remaining = pad;
  प्रति (idx_t i = fields; blank < i; i--)
    {
      पूर्णाङ्क masked_c = *p++ & 0x7f;
      वर्ण ध्रुव *s;
      वर्ण buf[2];

      यदि (masked_c == 127)
        s = "del";
      अन्यथा यदि (masked_c <= 040)
        s = charname[masked_c];
      अन्यथा
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t अग्र_pad = pad_at (fields, i - 1, pad);
      पूर्णाङ्क adjusted_विस्तारः = pad_remaining - अग्र_pad + विस्तारः;
      xprintf ("%*s", adjusted_विस्तारः, s);
      pad_remaining = अग्र_pad;
    }
}

स्थिर शून्य
print_ascii (idx_t fields, idx_t blank, शून्य ध्रुव *खण्डम्,
             MAYBE_अप्रयुक्तम् वर्ण ध्रुव *अप्रयुक्तम्_fmt_string, पूर्णाङ्क विस्तारः,
             idx_t pad)
{
  अचिह्नित वर्ण ध्रुव *p = खण्डम्;
  idx_t pad_remaining = pad;
  प्रति (idx_t i = fields; blank < i; i--)
    {
      अचिह्नित वर्ण c = *p++;
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

      idx_t अग्र_pad = pad_at (fields, i - 1, pad);
      पूर्णाङ्क adjusted_विस्तारः = pad_remaining - अग्र_pad + विस्तारः;
      xprintf ("%*s", adjusted_विस्तारः, s);
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

स्थिर bool
simple_strtoi (वर्ण ध्रुव *s, वर्ण ध्रुव **p, पूर्णाङ्क *val)
{
  पूर्णाङ्क योग;

  प्रति (योग = 0; c_isdigit (*s); s++)
    यदि (ckd_mul (&योग, योग, 10) || ckd_सङ्कलय (&योग, योग, *s - '0'))
      प्रत्यागम false;
  *p = s;
  *val = योग;
  प्रत्यागम true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
स्थिर वर्ण ध्रुव *
ispec_गन्तव्यम्_format (गणना आकार_spec आकार_spec,
                 वर्ण ध्रुव *int_format,
                 वर्ण ध्रुव *long_format,
                 वर्ण ध्रुव *long_long_format,
                 वर्ण ध्रुव *intmax_format)
{
  प्रत्यागम (LONG < INTMAX && आकार_spec == INTMAX ? intmax_format
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

स्थिर bool ATTRIBUTE_NONNULL ()
decode_one_format (वर्ण ध्रुव *s_orig, वर्ण ध्रुव *s, वर्ण ध्रुव **अग्र,
                   संरचना tspec *tspec)
{
  गणना आकार_spec आकार_spec;
  पूर्णाङ्क आकार;
  गणना output_format fmt;
  print_function_type print_function;
  वर्ण ध्रुव *p;
  वर्ण c;
  पूर्णाङ्क क्षेत्रम्_विस्तारः;

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
          आकार = परिमाण (अचिह्नित वर्ण);
          भङ्ग;

        पक्ष 'S':
          ++s;
          आकार = परिमाण (अचिह्नित ह्रस्व पूर्णाङ्क);
          भङ्ग;

        पक्ष 'I':
          ++s;
          आकार = परिमाण (अचिह्नित पूर्णाङ्क);
          भङ्ग;

        पक्ष 'L':
          ++s;
          आकार = परिमाण (अचिह्नित दीर्घ पूर्णाङ्क);
          भङ्ग;

        स्वभाव:
          यदि (! simple_strtoi (s, &p, &आकार))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              प्रत्यागम false;
            }
          यदि (p == s)
            आकार = परिमाण (अचिह्नित पूर्णाङ्क);
          अन्यथा
            {
              यदि (countof (integral_type_आकार) <= आकार
                  || integral_type_आकार[आकार] == NO_आकार)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), आकार);
                  प्रत्यागम false;
                }
              s = p;
            }
          भङ्ग;
        }

      आकार_spec = integral_type_आकार[आकार];

      विकल्प (c)
        {
        पक्ष 'd':
          fmt = SIGNED_DECIMAL;
          क्षेत्रम्_विस्तारः = INT_द्वयङ्काः_STRLEN_BOUND (CHAR_BIT * आकार - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_गन्तव्यम्_format (आकार_spec, "d", "ld", "lld", "jd"));
          भङ्ग;

        पक्ष 'o':
          fmt = OCTAL;
          क्षेत्रम्_विस्तारः = (CHAR_BIT * आकार + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", क्षेत्रम्_विस्तारः,
                   ispec_गन्तव्यम्_format (आकार_spec, "o", "lo", "llo", "jo"));
          भङ्ग;

        पक्ष 'u':
          fmt = UNSIGNED_DECIMAL;
          क्षेत्रम्_विस्तारः = INT_द्वयङ्काः_STRLEN_BOUND (CHAR_BIT * आकार);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_गन्तव्यम्_format (आकार_spec, "u", "lu", "llu", "ju"));
          भङ्ग;

        पक्ष 'x':
          fmt = HEXADECIMAL;
          क्षेत्रम्_विस्तारः = (CHAR_BIT * आकार + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", क्षेत्रम्_विस्तारः,
                   ispec_गन्तव्यम्_format (आकार_spec, "x", "lx", "llx", "jx"));
          भङ्ग;

        स्वभाव:
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
          आकार = परिमाण (bfloat16);
          भङ्ग;

        पक्ष 'H':
          ++s;
          fmt = HFLOATING_POINT;
          आकार = परिमाण (float16);
          भङ्ग;

        पक्ष 'F':
          ++s;
          आकार = परिमाण (भिन्न);
          भङ्ग;

        पक्ष 'D':
          ++s;
          आकार = परिमाण (द्विगुण);
          भङ्ग;

        पक्ष 'L':
          ++s;
          आकार = परिमाण (दीर्घ द्विगुण);
          भङ्ग;

        स्वभाव:
          यदि (! simple_strtoi (s, &p, &आकार))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              प्रत्यागम false;
            }
          यदि (p == s)
            आकार = परिमाण (द्विगुण);
          अन्यथा
            {
              यदि (countof (fp_type_आकार) <= आकार
                  || fp_type_आकार[आकार] == NO_आकार
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && आकार == परिमाण (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), आकार);
                  प्रत्यागम false;
                }
              s = p;
            }
          भङ्ग;
        }
      आकार_spec = fp_type_आकार[आकार];

      यदि ((! FLOAT16_SUPPORTED && fmt == HFLOATING_POINT)
          || (! BF16_SUPPORTED && fmt == BFLOATING_POINT))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        प्रत्यागम false;
      }

      {
        संरचना lconv ध्रुव *locale = localeconv ();
        idx_t decimal_point_len =
          (locale->decimal_point[0] ? वाक्यदैर्घ्यम् (locale->decimal_point) : 1);

        यदि (आकार_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            क्षेत्रम्_विस्तारः = DBL_STRLEN_BOUND_L (decimal_point_len);
          }
        अन्यथा यदि (FLOAT_SINGLE < FLOAT_DOUBLE && आकार_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            क्षेत्रम्_विस्तारः = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        अन्यथा यदि (FLOAT_HALF < FLOAT_SINGLE && आकार_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_POINT
                             ? print_bfloat : print_halffloat;
            क्षेत्रम्_विस्तारः = FLT_STRLEN_BOUND_L (decimal_point_len);
          }
        अन्यथा यदि (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && आकार_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            क्षेत्रम्_विस्तारः = LDBL_STRLEN_BOUND_L (decimal_point_len);
          }
        अन्यथा
          affirm (false);

        भङ्ग;
      }

    पक्ष 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      आकार_spec = CHAR;
      print_function = print_named_ascii;
      क्षेत्रम्_विस्तारः = 3;
      भङ्ग;

    पक्ष 'c':
      ++s;
      fmt = CHARACTER;
      आकार_spec = CHAR;
      print_function = print_ascii;
      क्षेत्रम्_विस्तारः = 3;
      भङ्ग;

    स्वभाव:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      प्रत्यागम false;
    }

  tspec->आकार = आकार_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->क्षेत्रम्_विस्तारः = क्षेत्रम्_विस्तारः;
  tspec->hexl_बहुलकम्_trailer = (*s == 'z');
  यदि (tspec->hexl_बहुलकम्_trailer)
    s++;

  *अग्र = s;
  प्रत्यागम true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

स्थिर bool
open_अग्र_file (शून्य)
{
  bool ok = true;

  कुरु
    {
      निवेशः_filename = *file_list;
      यदि (निवेशः_filename == रिक्त)
        प्रत्यागम ok;
      ++file_list;

      यदि (streq (निवेशः_filename, "-"))
        {
          निवेशः_filename = _("standard input");
          अन्तर्_प्रवाहः = stdin;
          have_read_stdin = true;
          xset_द्विमान_बहुलकम् (STDIN_FILENO, O_द्विमान);
        }
      अन्यथा
        {
          अन्तर्_प्रवाहः = fopen (निवेशः_filename, (O_द्विमान ? "rb" : "r"));
          यदि (अन्तर्_प्रवाहः == रिक्त)
            {
              error (0, errno, "%s", quotef (निवेशः_filename));
              ok = false;
            }
        }
    }
  यावत् (अन्तर्_प्रवाहः == रिक्त);

  यदि (0 <= अन्तः_विस्थापनम् && !flag_dump_वाक्यानि)
    setvbuf (अन्तर्_प्रवाहः, रिक्त, _IONBF, 0);

  प्रत्यागम ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

स्थिर bool
check_and_close (पूर्णाङ्क अन्तर्_errno)
{
  bool ok = true;

  यदि (अन्तर्_प्रवाहः != रिक्त)
    {
      यदि (!ferror (अन्तर्_प्रवाहः))
        अन्तर्_errno = 0;
      यदि (streq (file_list[-1], "-"))
        clearerr (अन्तर्_प्रवाहः);
      अन्यथा यदि (fclose (अन्तर्_प्रवाहः) != 0 && !अन्तर्_errno)
        अन्तर्_errno = errno;
      यदि (अन्तर्_errno)
        {
          error (0, अन्तर्_errno, "%s", quotef (निवेशः_filename));
          ok = false;
        }

      अन्तर्_प्रवाहः = रिक्त;
    }

  यदि (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  प्रत्यागम ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

स्थिर bool ATTRIBUTE_NONNULL ()
decode_format_string (वर्ण ध्रुव *s)
{
  वर्ण ध्रुव *s_orig = s;

  यावत् (*s != '\0')
    {
      वर्ण ध्रुव *अग्र;

      यदि (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, परिमाण *spec);

      यदि (! decode_one_format (s_orig, s, &अग्र, &spec[n_specs]))
        प्रत्यागम false;

      affirm (s != अग्र);
      s = अग्र;
      ++n_specs;
    }

  प्रत्यागम true;
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
  पूर्णाङ्क अन्तर्_errno = 0;

  यदि (n_skip == 0)
    प्रत्यागम true;

  यावत् (अन्तर्_प्रवाहः != रिक्त)	/* EOF.  */
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

      यदि (fstat (fileno (अन्तर्_प्रवाहः), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          यदि (S_ISREG (file_stats.st_बहुलकम्)
              && STP_BLKSIZE (&file_stats) < file_stats.st_आकार)
            {
              यदि (file_stats.st_आकार < n_skip)
                n_skip -= file_stats.st_आकार;
              अन्यथा
                {
                  यदि (fseeko (अन्तर्_प्रवाहः, n_skip, SEEK_CUR) != 0)
                    {
                      अन्तर्_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          अन्यथा यदि (! (S_ISREG (file_stats.st_बहुलकम्)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (अन्तर्_प्रवाहः, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          अन्यथा
            {
              वर्ण buf[BUFSIZ];
              idx_t n_अष्टकानि_read, n_अष्टकानि_गन्तव्यम्_read = BUFSIZ;

              यावत् (0 < n_skip)
                {
                  यदि (n_skip < n_अष्टकानि_गन्तव्यम्_read)
                    n_अष्टकानि_गन्तव्यम्_read = n_skip;
                  n_अष्टकानि_read = fread (buf, 1, n_अष्टकानि_गन्तव्यम्_read, अन्तर्_प्रवाहः);
                  n_skip -= n_अष्टकानि_read;
                  यदि (n_अष्टकानि_read != n_अष्टकानि_गन्तव्यम्_read)
                    {
                      यदि (ferror (अन्तर्_प्रवाहः))
                        {
                          अन्तर्_errno = errno;
                          ok = false;
                          n_skip = 0;
                          भङ्ग;
                        }
                      यदि (feof (अन्तर्_प्रवाहः))
                        भङ्ग;
                    }
                }
            }

          यदि (n_skip == 0)
            भङ्ग;
        }

      अन्यथा   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (निवेशः_filename));
          ok = false;
        }

      ok &= check_and_close (अन्तर्_errno);

      ok &= open_अग्र_file ();
    }

  यदि (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  प्रत्यागम ok;
}

स्थिर शून्य
format_स्थानाङ्कः_none (MAYBE_अप्रयुक्तम् intmax_t स्थानाङ्कः,
                     MAYBE_अप्रयुक्तम् वर्ण c)
{
}

स्थिर शून्य
format_स्थानाङ्कः_std (intmax_t स्थानाङ्कः, वर्ण c)
{
  वर्ण buf[MAX_स्थानाङ्कः_दीर्घता + 2];
  वर्ण *p = buf + परिमाण buf;
  वर्ण ध्रुव *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - स्थानाङ्कः_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  विकल्प (स्थानाङ्कः_आधारः)
    {
    पक्ष 8:
      कुरु
        *--p = '0' + (स्थानाङ्कः & 7);
      यावत् ((स्थानाङ्कः >>= 3) != 0);
      भङ्ग;

    पक्ष 10:
      कुरु
        *--p = '0' + (स्थानाङ्कः % 10);
      यावत् ((स्थानाङ्कः /= 10) != 0);
      भङ्ग;

    पक्ष 16:
      कुरु
        *--p = "0123456789abcdef"[स्थानाङ्कः & 15];
      यावत् ((स्थानाङ्कः >>= 4) != 0);
      भङ्ग;
    }

  यावत् (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

स्थिर शून्य
format_स्थानाङ्कः_paren (intmax_t स्थानाङ्कः, वर्ण c)
{
  अक्षरं_लिख ('(');
  format_स्थानाङ्कः_std (स्थानाङ्कः, ')');
  यदि (c)
    अक्षरं_लिख (c);
}

स्थिर शून्य
format_स्थानाङ्कः_label (intmax_t स्थानाङ्कः, वर्ण c)
{
  format_स्थानाङ्कः_std (स्थानाङ्कः, ' ');

  intmax_t addr;
  यदि (ckd_सङ्कलय (&addr, स्थानाङ्कः, pseudo_विस्थापनम्))
    error (EXIT_FAILURE, 0, _("pseudo address too large for input"));

  format_स्थानाङ्कः_paren (addr, c);
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
write_खण्डम् (intmax_t current_विस्थापनम्, idx_t n_अष्टकानि,
             वर्ण ध्रुव *पूर्व_खण्डम्, वर्ण ध्रुव *curr_खण्डम्)
{
  स्थिर bool first = true;
  स्थिर bool पूर्व_pair_equal = false;

  यदि (abbreviate_duplicate_खण्डानि
      && !first && n_अष्टकानि == अष्टकानि_प्रत्येकम्_खण्डम्
      && memeq (पूर्व_खण्डम्, curr_खण्डम्, अष्टकानि_प्रत्येकम्_खण्डम्))
    {
      यदि (पूर्व_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      अन्यथा
        {
          प्रारूप_लिख ("*\n");
          पूर्व_pair_equal = true;
        }
    }
  अन्यथा
    {
      पूर्व_pair_equal = false;
      प्रति (idx_t i = 0; i < n_specs; i++)
        {
          पूर्णाङ्क datum_विस्तारः = विस्तारः_अष्टकानि[spec[i].आकार];
          idx_t fields_प्रत्येकम्_खण्डम् = अष्टकानि_प्रत्येकम्_खण्डम् / datum_विस्तारः;
          idx_t blank_fields = (अष्टकानि_प्रत्येकम्_खण्डम् - n_अष्टकानि) / datum_विस्तारः;
          यदि (i == 0)
            format_स्थानाङ्कः (current_विस्थापनम्, '\0');
          अन्यथा
            print_n_spaces (स्थानाङ्कः_pad_len);
          (*spec[i].print_function) (fields_प्रत्येकम्_खण्डम्, blank_fields,
                                     curr_खण्डम्, spec[i].fmt_string,
                                     spec[i].क्षेत्रम्_विस्तारः, spec[i].pad_विस्तारः);
          यदि (spec[i].hexl_बहुलकम्_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              पूर्णाङ्क क्षेत्रम्_विस्तारः = spec[i].क्षेत्रम्_विस्तारः;
              प्रति (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (क्षेत्रम्_विस्तारः);
              idx_t pad_विस्तारः = pad_at (fields_प्रत्येकम्_खण्डम्, blank_fields,
                                        spec[i].pad_विस्तारः);
              print_n_spaces (pad_विस्तारः);
              dump_hexl_बहुलकम्_trailer (n_अष्टकानि, curr_खण्डम्);
            }
          अक्षरं_लिख ('\n');
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

  यावत् (अन्तर्_प्रवाहः && (*c = getc (अन्तर्_प्रवाहः)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_अग्र_file ();
    }

  प्रत्यागम ok;
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
read_खण्डम् (idx_t n, वर्ण *खण्डम्, idx_t *n_अष्टकानि_अन्तर्_बफर)
{
  bool ok = true;

  affirm (0 < n && n <= अष्टकानि_प्रत्येकम्_खण्डम्);

  *n_अष्टकानि_अन्तर्_बफर = 0;

  यावत् (अन्तर्_प्रवाहः != रिक्त)	/* EOF.  */
    {
      idx_t n_needed = n - *n_अष्टकानि_अन्तर्_बफर;
      idx_t n_read = fread (खण्डम् + *n_अष्टकानि_अन्तर्_बफर,
                            1, n_needed, अन्तर्_प्रवाहः);

      *n_अष्टकानि_अन्तर्_बफर += n_read;

      यदि (n_read == n_needed)
        भङ्ग;

      ok &= check_and_close (errno);

      ok &= open_अग्र_file ();
    }

  प्रत्यागम ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_विशुद्धम्
स्थिर पूर्णाङ्क
get_lcm (शून्य)
{
  पूर्णाङ्क l_c_m = 1;

  प्रति (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, विस्तारः_अष्टकानि[spec[i].आकार]);
  प्रत्यागम l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
स्थिर strtol_error
xstr2nonneg (वर्ण ध्रुव *सीमित nptr, पूर्णाङ्क आधारः_2, intmax_t *val,
             वर्ण ध्रुव *सीमित valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, रिक्त, आधारः_2, val, valid_suffixes);
  प्रत्यागम s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

स्थिर bool
parse_old_विस्थापनम् (वर्ण *str, intmax_t *विस्थापनम्)
{
  /* Skip over any leading '+'.  */
  वर्ण *s = str + (str[0] == '+');

  यदि (! c_isdigit (s[0]))
    प्रत्यागम false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  वर्ण *dot = strchr (s, '.');
  यदि (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = रिक्त;
  पूर्णाङ्क radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  यदि (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  गणना strtol_error s_err = xstr2nonneg (s, radix, विस्थापनम्, "bB");

  यदि (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  यदि (s_err == LONGINT_अतिप्रवाहः)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  प्रत्यागम s_err == LONGINT_OK;
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
  वर्ण *खण्डम्[2];
  bool idx = false;
  bool ok = true;
  idx_t n_अष्टकानि_read;

  खण्डम्[0] = xinmalloc (1 + abbreviate_duplicate_खण्डानि, अष्टकानि_प्रत्येकम्_खण्डम्);
  खण्डम्[1] = खण्डम्[0] + abbreviate_duplicate_खण्डानि * अष्टकानि_प्रत्येकम्_खण्डम्;

  intmax_t current_विस्थापनम् = n_अष्टकानि_गन्तव्यम्_skip;

  कुरु
    {
      intmax_t needed_bound
        = अन्तः_विस्थापनम् < 0 ? INTMAX_MAX : अन्तः_विस्थापनम् - current_विस्थापनम्;
      यदि (needed_bound <= 0)
        {
          n_अष्टकानि_read = 0;
          भङ्ग;
        }
      idx_t n_needed = MIN (अष्टकानि_प्रत्येकम्_खण्डम्, needed_bound);
      ok &= read_खण्डम् (n_needed, खण्डम्[idx], &n_अष्टकानि_read);
      यदि (n_अष्टकानि_read < अष्टकानि_प्रत्येकम्_खण्डम्)
        भङ्ग;
      affirm (n_अष्टकानि_read == अष्टकानि_प्रत्येकम्_खण्डम्);
      write_खण्डम् (current_विस्थापनम्, n_अष्टकानि_read,
                   खण्डम्[!idx], खण्डम्[idx]);
      यदि (ferror (stdout))
        ok = false;
      current_विस्थापनम् += n_अष्टकानि_read;
      idx = !idx;
    }
  यावत् (ok);

  यदि (n_अष्टकानि_read > 0)
    {
      पूर्णाङ्क l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t अष्टकानि_गन्तव्यम्_write = (n_अष्टकानि_read + l_c_m - 1
                              - (n_अष्टकानि_read + l_c_m - 1) % l_c_m);

      स्मृतिं_पूरय (खण्डम्[idx] + n_अष्टकानि_read, 0, अष्टकानि_गन्तव्यम्_write - n_अष्टकानि_read);
      write_खण्डम् (current_विस्थापनम्, n_अष्टकानि_read, खण्डम्[!idx], खण्डम्[idx]);
      current_विस्थापनम् += n_अष्टकानि_read;
    }

  यदि (ok || current_विस्थापनम्)
    format_स्थानाङ्कः (current_विस्थापनम्, '\n');

  यदि (0 <= अन्तः_विस्थापनम् && अन्तः_विस्थापनम् <= current_विस्थापनम्)
    ok &= check_and_close (0);

  मुक्त (खण्डम्[0]);

  प्रत्यागम ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

स्थिर bool
dump_वाक्यानि (शून्य)
{
  idx_t bufsize = MAX (100, string_min + 1);
  वर्ण *buf = ximalloc (bufsize);
  intmax_t स्थानाङ्कः = n_अष्टकानि_गन्तव्यम्_skip;
  bool ok = true;

  यावत् (true)
    {
      idx_t i = 0;
      पूर्णाङ्क c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      यदि (0 <= अन्तः_विस्थापनम्
          && (अन्तः_विस्थापनम् < string_min || अन्तः_विस्थापनम् - string_min < स्थानाङ्कः))
        भङ्ग;

      /* Store consecutive printable characters to BUF.  */
      यावत् (! (0 <= अन्तः_विस्थापनम् && अन्तः_विस्थापनम् <= स्थानाङ्कः))
        {
          यदि (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, परिमाण *buf);
          ok &= read_char (&c);
          यदि (c < 0)
            {
              मुक्त (buf);
              प्रत्यागम ok;
            }
          स्थानाङ्कः++;
          buf[i++] = c;
          यदि (c == '\0')
            भङ्ग;		/* Print this string.  */
          यदि (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              भङ्ग;
            }
        }

      यदि (c < 0 || i - !c < string_min)
        अनुवर्त;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_स्थानाङ्कः (स्थानाङ्कः - i, ' ');

      प्रति (i = 0; (c = buf[i]); i++)
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
      अक्षरं_लिख ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  मुक्त (buf);

  ok &= check_and_close (0);
  प्रत्यागम ok;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क n_files;
  पूर्णाङ्क l_c_m;
  idx_t desired_विस्तारः = 0;
  bool modern = false;
  bool ok = true;
  idx_t विस्तारः_प्रत्येकम्_खण्डम् = 0;
  स्थिर वर्ण ध्रुव multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_अष्टकानि_गन्तव्यम्_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_start IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् (true)
    {
      intmax_t tmp;
      गणना strtol_error s_err;
      पूर्णाङ्क oi = -1;
      पूर्णाङ्क c = getopt_long (argc, argv, short_options, long_options, &oi);
      यदि (c == -1)
        भङ्ग;

      विकल्प (c)
        {
        पक्ष 'A':
          modern = true;
          विकल्प (optarg[0])
            {
            पक्ष 'd':
              format_स्थानाङ्कः = format_स्थानाङ्कः_std;
              स्थानाङ्कः_आधारः = 10;
              स्थानाङ्कः_pad_len = 7;
              भङ्ग;
            पक्ष 'o':
              format_स्थानाङ्कः = format_स्थानाङ्कः_std;
              स्थानाङ्कः_आधारः = 8;
              स्थानाङ्कः_pad_len = 7;
              भङ्ग;
            पक्ष 'x':
              format_स्थानाङ्कः = format_स्थानाङ्कः_std;
              स्थानाङ्कः_आधारः = 16;
              स्थानाङ्कः_pad_len = 6;
              भङ्ग;
            पक्ष 'n':
              format_स्थानाङ्कः = format_स्थानाङ्कः_none;
              स्थानाङ्कः_pad_len = 0;
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
          s_err = xstr2nonneg (optarg, 0, &n_अष्टकानि_गन्तव्यम्_skip, multipliers);
          यदि (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          भङ्ग;

        पक्ष 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_अष्टकानि_गन्तव्यम्_format, multipliers);
          यदि (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          भङ्ग;

        पक्ष 'S':
          modern = true;
          यदि (optarg == रिक्त)
            string_min = 3;
          अन्यथा
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              यदि (s_err == LONGINT_OK && ckd_सङ्कलय (&i, tmp, 1))
                s_err = LONGINT_अतिप्रवाहः;
              यदि (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_वाक्यानि = true;
          भङ्ग;

        पक्ष 't':
          modern = true;
          ok &= decode_format_string (optarg);
          भङ्ग;

        पक्ष 'v':
          modern = true;
          abbreviate_duplicate_खण्डानि = false;
          भङ्ग;

        पक्ष TRADITIONAL_OPTION:
          traditional = true;
          भङ्ग;

        पक्ष ENDIAN_OPTION:
          विकल्प (XARGMATCH ("--endian", optarg, endian_तर्काः, endian_types))
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

#परिभाषय CASE_OLD_ARG(old_char,new_string)		\
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

#अपरिभाषय CASE_OLD_ARG

        पक्ष 'w':
          modern = true;
          यदि (optarg == रिक्त)
            {
              desired_विस्तारः = 32;
            }
          अन्यथा
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              यदि (s_err == LONGINT_OK)
                {
                  यदि (ckd_सङ्कलय (&desired_विस्तारः, w_tmp, 0))
                    s_err = LONGINT_अतिप्रवाहः;
                  अन्यथा यदि (desired_विस्तारः == 0)
                    s_err = LONGINT_INVALID;
                }
              यदि (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
          भङ्ग;
        }
    }

  यदि (!ok)
    प्रत्यागम EXIT_FAILURE;

  यदि (flag_dump_वाक्यानि && n_specs > 0)
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

  यदि (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      विकल्प (n_files)
        {
        पक्ष 1:
          यदि ((traditional || argv[optind][0] == '+')
              && parse_old_विस्थापनम् (argv[optind], &o1))
            {
              n_अष्टकानि_गन्तव्यम्_skip = o1;
              --n_files;
              ++argv;
            }
          भङ्ग;

        पक्ष 2:
          यदि ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_विस्थापनम् (argv[optind + 1], &o2))
            {
              यदि (traditional && parse_old_विस्थापनम् (argv[optind], &o1))
                {
                  n_अष्टकानि_गन्तव्यम्_skip = o1;
                  flag_pseudo_start = true;
                  pseudo_start = o2;
                  argv += 2;
                  n_files -= 2;
                }
              अन्यथा
                {
                  n_अष्टकानि_गन्तव्यम्_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          भङ्ग;

        पक्ष 3:
          यदि (traditional
              && parse_old_विस्थापनम् (argv[optind + 1], &o1)
              && parse_old_विस्थापनम् (argv[optind + 2], &o2))
            {
              n_अष्टकानि_गन्तव्यम्_skip = o1;
              flag_pseudo_start = true;
              pseudo_start = o2;
              argv[optind + 2] = argv[optind];
              argv += 2;
              n_files -= 2;
            }
          भङ्ग;
        }

      यदि (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  यदि (flag_pseudo_start)
    {
      यदि (format_स्थानाङ्कः == format_स्थानाङ्कः_none)
        {
          स्थानाङ्कः_आधारः = 8;
          स्थानाङ्कः_pad_len = 7;
          format_स्थानाङ्कः = format_स्थानाङ्कः_paren;
        }
      अन्यथा
        format_स्थानाङ्कः = format_स्थानाङ्कः_label;
    }

  यदि (0 <= max_अष्टकानि_गन्तव्यम्_format
      && ckd_सङ्कलय (&अन्तः_विस्थापनम्, n_अष्टकानि_गन्तव्यम्_skip, max_अष्टकानि_गन्तव्यम्_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  यदि (n_specs == 0)
    decode_format_string ("oS");

  यदि (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (वर्ण ध्रुव *ध्रुव *) &argv[optind];
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
  यदि (अन्तर्_प्रवाहः == रिक्त)
    गच्छ cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_अष्टकानि_गन्तव्यम्_skip);
  यदि (अन्तर्_प्रवाहः == रिक्त)
    गच्छ cleanup;

  pseudo_विस्थापनम् = (flag_pseudo_start ? pseudo_start - n_अष्टकानि_गन्तव्यम्_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  यदि (desired_विस्तारः != 0)
    {
      यदि (desired_विस्तारः % l_c_m == 0)
        अष्टकानि_प्रत्येकम्_खण्डम् = desired_विस्तारः;
      अन्यथा
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_विस्तारः, l_c_m);
          अष्टकानि_प्रत्येकम्_खण्डम् = l_c_m;
        }
    }
  अन्यथा
    {
      यदि (l_c_m < DEFAULT_अष्टकानि_प्रत्येकम्_खण्डम्)
        अष्टकानि_प्रत्येकम्_खण्डम् = (DEFAULT_अष्टकानि_प्रत्येकम्_खण्डम्
                           - DEFAULT_अष्टकानि_प्रत्येकम्_खण्डम् % l_c_m);
      अन्यथा
        अष्टकानि_प्रत्येकम्_खण्डम् = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  प्रति (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_प्रत्येकम्_खण्डम् = अष्टकानि_प्रत्येकम्_खण्डम् / विस्तारः_अष्टकानि[spec[i].आकार];
      यदि (pad_at_अतिप्रवाहः (fields_प्रत्येकम्_खण्डम्))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_विस्तारः);
      idx_t खण्डम्_विस्तारः = (spec[i].क्षेत्रम्_विस्तारः + 1) * fields_प्रत्येकम्_खण्डम्;
      यदि (विस्तारः_प्रत्येकम्_खण्डम् < खण्डम्_विस्तारः)
        विस्तारः_प्रत्येकम्_खण्डम् = खण्डम्_विस्तारः;
    }
  प्रति (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_प्रत्येकम्_खण्डम् = अष्टकानि_प्रत्येकम्_खण्डम् / विस्तारः_अष्टकानि[spec[i].आकार];
      idx_t खण्डम्_विस्तारः = spec[i].क्षेत्रम्_विस्तारः * fields_प्रत्येकम्_खण्डम्;
      spec[i].pad_विस्तारः = विस्तारः_प्रत्येकम्_खण्डम् - खण्डम्_विस्तारः;
    }

#यदि_परिभाषितम् DEBUG
  प्रारूप_लिख ("lcm=%d, width_per_block=%td\n", l_c_m, विस्तारः_प्रत्येकम्_खण्डम्);
  प्रति (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_प्रत्येकम्_खण्डम् = अष्टकानि_प्रत्येकम्_खण्डम् / विस्तारः_अष्टकानि[spec[i].आकार];
      affirm (अष्टकानि_प्रत्येकम्_खण्डम् % विस्तारः_अष्टकानि[spec[i].आकार] == 0);
      affirm (1 <= spec[i].pad_विस्तारः / fields_प्रत्येकम्_खण्डम्);
      प्रारूप_लिख ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, विस्तारः_अष्टकानि[spec[i].आकार],
              spec[i].क्षेत्रम्_विस्तारः, spec[i].pad_विस्तारः);
    }
#यदि_अन्तः

  ok &= (flag_dump_वाक्यानि ? dump_वाक्यानि () : dump ());

cleanup:

  यदि (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
