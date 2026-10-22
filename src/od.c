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

#అన్తర్భావ <config.h>

#అన్తర్భావ <endian.h>
#అన్తర్భావ <float.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "ftoastr.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stat-size.h"
#అన్తర్భావ "xbinary-io.h"
#అన్తర్భావ "xprintf.h"
#అన్తర్భావ "xstrtol.h"
#అన్తర్భావ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "od"

#నిర్వచించు AUTHORS proper_name ("Jim Meyering")

/* The default number of input bytes per output line.  */
#నిర్వచించు DEFAULT_బైట్లు_చొప్పున_ఖండం 16

#ఒకవేళ FLOAT16_SUPPORTED
  /* Available since clang 6 (2018), and gcc 7 (2017).  */
  రకనిర్వచనం _Float16 float16;
#లేకపోతే
# define FLOAT16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  రకనిర్వచనం దశాంశం float16;
#అయితే_ముగింపు

#ఒకవేళ BF16_SUPPORTED
  /* Available since clang 11 (2020), and gcc 13 (2023). */
  రకనిర్వచనం __bf16 bfloat16;
#లేకపోతే
# define BF16_SUPPORTED 0
  /* This is just a place-holder to avoid a few '#if' directives.
     In this case, the type isn't actually used.  */
  రకనిర్వచనం దశాంశం bfloat16;
#అయితే_ముగింపు

గణన కొలత_spec
  {
    NO_కొలత = 0,
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
    N_కొలత_SPECS
  };

గణన output_format
  {
    SIGNED_DECIMAL,
    UNSIGNED_DECIMAL,
    OCTAL,
    HEXADECIMAL,
    FLOATING_బిందువు,
    HFLOATING_బిందువు,
    BFLOATING_బిందువు,
    NAMED_CHARACTER,
    CHARACTER
  };

గణన { MAX_INTEGRAL_TYPE_వెడల్పు = UINTMAX_వెడల్పు };

/* The maximum number of bytes needed for a format string, including
   the trailing nul.  Each format string expects a variable amount of
   padding (guaranteed to be at least 1 plus the field width), then an
   element that will be formatted in the field.  */
గణన
  {
    FMT_బైట్లు_ALLOCATED =
      పరిమాణం "%*.%dlld" - పరిమాణం "%d" + INT_STRLEN_BOUND (పూర్ణాంకం) + 1
  };

/* Ensure that our choice for FMT_BYTES_ALLOCATED is reasonable.  */
static_assert (MAX_INTEGRAL_TYPE_వెడల్పు <= 3 * 99);

/* The type of a print function.  FIELDS is the number of fields per
   line, BLANK is the number of fields to leave blank.  WIDTH is width
   of one field, excluding leading space, and PAD is total pad to
   divide among FIELDS.  PAD is at least as large as FIELDS.  */
రకనిర్వచనం శూన్య (*print_function_type)
  (idx_t fields, idx_t blank, శూన్య స్థిరరాశి *దత్తాంశం,
   అక్షరం స్థిరరాశి *fmt, పూర్ణాంకం వెడల్పు, idx_t pad);

/* Each output format specification (from '-t spec' or from
   old-style options) is represented by one of these structures.  */
నిర్మాణం tspec
  {
    గణన output_format fmt;
    గణన కొలత_spec కొలత; /* Type of input object.  */
    print_function_type print_function;
    అక్షరం fmt_string[FMT_బైట్లు_ALLOCATED]; /* Of the style "%*d".  */
    bool hexl_బహుళకం_trailer;
    పూర్ణాంకం క్షేత్రం_వెడల్పు; /* Minimum width of a field, excluding leading space.  */
    idx_t pad_వెడల్పు; /* Total padding to be divided among fields.  */
  };

/* Convert enum size_spec to the size of the named type.  */
స్థిర స్థిరరాశి పూర్ణాంకం వెడల్పు_బైట్లు[] =
{
  -1,
  పరిమాణం (గుర్తులేని అక్షరం),
#ఒకవేళ UCHAR_MAX < USHRT_MAX
  పరిమాణం (గుర్తులేని పొట్టి పూర్ణాంకం),
#అయితే_ముగింపు
#ఒకవేళ USHRT_MAX < UINT_MAX
  పరిమాణం (గుర్తులేని పూర్ణాంకం),
#అయితే_ముగింపు
#ఒకవేళ UINT_MAX < ULONG_MAX
  పరిమాణం (గుర్తులేని దీర్ఘ పూర్ణాంకం),
#అయితే_ముగింపు
#ఒకవేళ ULONG_MAX < ULLONG_MAX
  పరిమాణం (గుర్తులేని దీర్ఘ దీర్ఘ పూర్ణాంకం),
#అయితే_ముగింపు
#ఒకవేళ ULLONG_MAX < UINTMAX_MAX
  పరిమాణం (uintmax_t),
#అయితే_ముగింపు
#ఒకవేళ BF16_SUPPORTED
  పరిమాణం (bfloat16),
#లేదావేళ FLOAT16_SUPPORTED
  పరిమాణం (float16),
#అయితే_ముగింపు
#ఒకవేళ FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
  పరిమాణం (దశాంశం),
#అయితే_ముగింపు
  పరిమాణం (ద్విగుణ),
#ఒకవేళ DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
  పరిమాణం (దీర్ఘ ద్విగుణ),
#అయితే_ముగింపు
};

/* Ensure that for each member of 'enum size_spec' there is an
   initializer in the width_bytes array.  */
static_assert (countof (వెడల్పు_బైట్లు) == N_కొలత_SPECS);

/* Names for some non-printing characters.  */
స్థిర అక్షరం స్థిరరాశి charname[33][4] =
{
  "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
  "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
  "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
  "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
  "sp"
};

/* Address base (8, 10 or 16).  */
స్థిర పూర్ణాంకం చిరునామా_పునాది = 8;

/* The number of octal digits required to represent the largest
   address value.  */
గణన { MAX_చిరునామా_నిడివి = ((INTMAX_వెడల్పు - 1) / 3
                             + ((INTMAX_వెడల్పు - 1) % 3 != 0)) };

/* Width of a normal address.  */
స్థిర పూర్ణాంకం చిరునామా_pad_len = 7;

/* Minimum length when detecting --strings.  */
స్థిర idx_t string_min;

/* True when in --strings mode.  */
స్థిర bool flag_dump_అక్షరశ్రేణులు;

/* True if we should recognize the older non-option arguments
   that specified at most one file and optional arguments specifying
   offset and pseudo-start address.  */
స్థిర bool traditional;

/* True if an old-style 'pseudo-address' was specified.  */
స్థిర bool flag_pseudo_ప్రారంభం;

/* The difference between the old-style pseudo starting address and
   the number of bytes to skip.  */
స్థిర intmax_t pseudo_స్థానభ్రంశం;

/* Function that accepts an address and an optional following char,
   and prints the address and char to stdout.  */
స్థిర శూన్య format_చిరునామా_std (intmax_t, అక్షరం);
స్థిర శూన్య (*format_చిరునామా) (intmax_t, అక్షరం) = format_చిరునామా_std;

/* The number of input bytes to skip before formatting and writing.  */
స్థిర intmax_t n_బైట్లు_గమ్యం_skip;

/* The offset of the first byte after the last byte to be formatted.
   If negative, there is no limit.  */
స్థిర intmax_t చివర_స్థానభ్రంశం = -1;

/* When true and two or more consecutive blocks are equal, format
   only the first block and output an asterisk alone on the following
   line to indicate that identical blocks have been elided.  */
స్థిర bool abbreviate_duplicate_ఖండాలు = true;

/* An array of specs describing how to format each input block.  */
స్థిర నిర్మాణం tspec *spec;

/* The number of format specs.  */
స్థిర idx_t n_specs;

/* The allocated length of SPEC.  */
స్థిర idx_t n_specs_allocated;

/* The number of input bytes formatted per output line.  It must be
   a multiple of the least common multiple of the sizes associated with
   the specified output types.  It should be as large as possible, but
   no larger than 16 -- unless specified with the -w option.  */
స్థిర idx_t బైట్లు_చొప్పున_ఖండం;

/* Human-readable representation of *file_list (for error messages).
   It differs from file_list[-1] only when file_list[-1] is "-".  */
స్థిర అక్షరం స్థిరరాశి *ఆదానం_filename;

/* A null-terminated list of the file-arguments from the command line.  */
స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి *file_list;

/* Initializer for file_list if no file-arguments
   were specified on the command line.  */
స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి default_file_list[] = {"-", ఖాళీ};

/* The input stream associated with the current file.  */
స్థిర FILE *లోపల_ప్రవాహం;

/* If true, at least one of the files we read was standard input.  */
స్థిర bool have_read_stdin;

/* Map the size in bytes to a type identifier.
   When two types have the same machine layout:
     - Prefer unsigned int to other types, as its format is shorter.
     - Prefer unsigned long to higher-ranked types, as it is older.
     - Prefer uintmax_t to unsigned long long int; this wins if %lld
       does not work but %jd does (e.g., MS-Windows).  */
స్థిర గణన కొలత_spec స్థిరరాశి integral_type_కొలత[] =
  {
#ఒకవేళ UCHAR_MAX < USHRT_MAX
    [పరిమాణం (గుర్తులేని అక్షరం)] = CHAR,
#అయితే_ముగింపు
#ఒకవేళ USHRT_MAX < UINT_MAX
    [పరిమాణం (గుర్తులేని పొట్టి పూర్ణాంకం)] = SHORT,
#అయితే_ముగింపు
    [పరిమాణం (గుర్తులేని పూర్ణాంకం)] = INT,
#ఒకవేళ UINT_MAX < ULONG_MAX
    [పరిమాణం (గుర్తులేని దీర్ఘ పూర్ణాంకం)] = LONG,
#అయితే_ముగింపు
#ఒకవేళ ULONG_MAX < ULLONG_MAX && ULLONG_MAX < UINTMAX_MAX
    [పరిమాణం (గుర్తులేని దీర్ఘ దీర్ఘ పూర్ణాంకం)] = LONG_LONG,
#అయితే_ముగింపు
#ఒకవేళ ULONG_MAX < UINTMAX_MAX
    [పరిమాణం (uintmax_t)] = INTMAX,
#అయితే_ముగింపు
  };

/* Map the size in bytes to a floating type identifier.
   When two types have the same machine layout:
     - Prefer double to the other types, as its format is shorter.  */
స్థిర గణన కొలత_spec స్థిరరాశి fp_type_కొలత[] =
  {
#ఒకవేళ FLOAT16_SUPPORTED
    [పరిమాణం (float16)] = FLOAT_HALF,
#లేదావేళ BF16_SUPPORTED
    [పరిమాణం (bfloat16)] = FLOAT_HALF,
#అయితే_ముగింపు
#ఒకవేళ FLT_MANT_DIG < DBL_MANT_DIG || FLT_MAX_EXP < DBL_MAX_EXP
    [పరిమాణం (దశాంశం)] = FLOAT_SINGLE,
#అయితే_ముగింపు
    [పరిమాణం (ద్విగుణ)] = FLOAT_DOUBLE,
#ఒకవేళ DBL_MANT_DIG < LDBL_MANT_DIG || DBL_MAX_EXP < LDBL_MAX_EXP
    [పరిమాణం (దీర్ఘ ద్విగుణ)] = FLOAT_LONG_DOUBLE,
#అయితే_ముగింపు
  };

/* Use native endianness by default.  */
స్థిర bool ఆదానం_మార్పిడి;

స్థిర అక్షరం స్థిరరాశి short_options[] = "A:aBbcDdeFfHhIij:LlN:OoS:st:vw::Xx";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  TRADITIONAL_OPTION = CHAR_MAX + 1,
  ENDIAN_OPTION,
};

గణన endian_type
{
  endian_little,
  endian_big
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి endian_వాదనలు[] =
{
  "little", "big", ఖాళీ
};

స్థిర గణన endian_type స్థిరరాశి endian_types[] =
{
  endian_little, endian_big
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"skip-bytes", required_వాదం, ఖాళీ, 'j'},
  {"address-radix", required_వాదం, ఖాళీ, 'A'},
  {"read-bytes", required_వాదం, ఖాళీ, 'N'},
  {"format", required_వాదం, ఖాళీ, 't'},
  {"output-duplicates", no_వాదం, ఖాళీ, 'v'},
  {"strings", optional_వాదం, ఖాళీ, 'S'},
  {"traditional", no_వాదం, ఖాళీ, TRADITIONAL_OPTION},
  {"width", optional_వాదం, ఖాళీ, 'w'},
  {"endian", required_వాదం, ఖాళీ, ENDIAN_OPTION },

  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [-abcdfilosx]... [FILE] [[+]స్థానభ్రంశం_2[.][b]]\n\
  or:  %s --traditional [OPTION]... [FILE] [[+]స్థానభ్రంశం_2[.][b] [+][LABEL][.][b]]\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\n\
Write an unambiguous representation, octal బైట్లు by అప్రమేయ,\n\
యొక్క FILE గమ్యం standard output.  With మరిన్ని than one FILE వాదం,\n\
concatenate them లోపల the listed క్రమం గమ్యం form the ఆదానం.\n\
"), stdout);

      emit_stdin_note ();

      fputs (_("\
\n\
If మొదటి and రెండవ call formats both apply, the రెండవ format is assumed\n\
ఒకవేళ the చివరి నిర్వాహ్యం_2 begins with + or (ఒకవేళ there are 2 operands) a digit.\n\
An స్థానభ్రంశం_2 నిర్వాహ్యం_2 means -j స్థానభ్రంశం_2.  LABEL is the pseudo-చిరునామా\n\
at మొదటి byte printed, incremented when dump is progressing.\n\
For స్థానభ్రంశం_2 and LABEL, a 0x or 0X ఉపసర్గ indicates hexadecimal;\n\
suffixes may be . కొరకు octal and b కొరకు multiply by 512.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -A, --చిరునామా-radix=RADIX\n\
         output format కొరకు file offsets;\n\
         RADIX is one యొక్క [doxn], కొరకు Decimal, Octal, Hex or None\n\
"));
      oputs (_("\
      --endian={big|little}\n\
         మార్పిడి ఆదానం బైట్లు according the specified క్రమం\n\
"));
      oputs (_("\
  -j, --skip-బైట్లు=బైట్లు_2\n\
         skip బైట్లు_2 ఆదానం బైట్లు మొదటి\n\
"));
      oputs (_("\
  -N, --పఠ-బైట్లు=బైట్లు_2\n\
         హద్దు dump గమ్యం బైట్లు_2 ఆదానం బైట్లు\n\
"));
      oputs (_("\
  -S బైట్లు_2, --అక్షరశ్రేణులు[=బైట్లు_2]\n\
         show only NUL terminated అక్షరశ్రేణులు\n\
         యొక్క at కనిష్ఠ బైట్లు_2 (అప్రమేయ 3) printable characters\n\
"));
      oputs (_("\
  -t, --format=TYPE\n\
         select output format or formats\n\
"));
      oputs (_("\
  -v, --output-duplicates\n\
         చేయి not use * గమ్యం mark పంక్తి suppression\n\
"));
      oputs (_("\
  -w[బైట్లు_2], --వెడల్పు[=బైట్లు_2]\n\
         output బైట్లు_2 బైట్లు చొప్పున output పంక్తి;\n\
         32 is implied when బైట్లు_2 is not specified\n\
"));
      oputs (_("\
      --traditional\n\
         accept arguments లోపల మూడవ form above\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
\n\
Traditional format specifications may be intermixed; they accumulate:\n\
"), stdout);
      oputs (_("\
  -a   same as -t a,  select named characters, ignoring ఎగువ-క్రమం బిట్టు\n\
"));
      oputs (_("\
  -b   same as -t o1, select octal బైట్లు\n\
"));
      oputs (_("\
  -c   same as -t c,  select printable characters or backslash escapes\n\
"));
      oputs (_("\
  -d   same as -t u2, select గుర్తులేని decimal 2-byte units\n\
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
TYPE is made పైకి యొక్క one or మరిన్ని యొక్క these specifications:\n\
  a          named character, ignoring ఎగువ-క్రమం బిట్టు\n\
  c          printable character or backslash escape\n\
"), stdout);
      fputs (_("\
  d[కొలత_2]    గుర్తుగల decimal, కొలత_2 బైట్లు చొప్పున integer\n\
  f[కొలత_2]    floating బిందువు, కొలత_2 బైట్లు చొప్పున దశాంశం\n\
  o[కొలత_2]    octal, కొలత_2 బైట్లు చొప్పున integer\n\
  u[కొలత_2]    గుర్తులేని decimal, కొలత_2 బైట్లు చొప్పున integer\n\
  x[కొలత_2]    hexadecimal, కొలత_2 బైట్లు చొప్పున integer\n\
"), stdout);
      fputs (_("\
\n\
కొలత_2 is a సంఖ్య.  For TYPE లోపల [doux], కొలత_2 may also be C కొరకు\n\
పరిమాణం(అక్షరం), S కొరకు పరిమాణం(పొట్టి), I కొరకు పరిమాణం(పూర్ణాంకం) or L కొరకు\n\
పరిమాణం(దీర్ఘ).  If TYPE is f, కొలత_2 may also be B కొరకు Brain 16 బిట్టు,\n\
H కొరకు Half precision దశాంశం, F కొరకు పరిమాణం(దశాంశం), D కొరకు పరిమాణం(ద్విగుణ),\n\
or L కొరకు పరిమాణం(దీర్ఘ ద్విగుణ).\n\
"), stdout);
      fputs (_("\
\n\
Adding a z suffix గమ్యం any type displays printable characters at the చివర యొక్క\n\
each output పంక్తి.\n\
"), stdout);
      fputs (_("\
\n\
\n\
బైట్లు_2 is hex with 0x or 0X ఉపసర్గ, and may have a multiplier suffix:\n\
  b    512\n\
  KB   1000\n\
  K    1024\n\
  MB   1000*1000\n\
  M    1024*1024\n\
and so on కొరకు G, T, P, E, Z, Y, R, Q.\n\
ద్విమాన_2 prefixes can be వాడిన, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Define the print functions.  */

/* Print N spaces, where 0 <= N.
   Do not rely on printf ("%*s", N, "") as N may exceed INT_MAX.  */
స్థిర శూన్య
print_n_spaces (intmax_t n)
{
  కొరకు (; 0 < n; n--)
    అక్షరం_రాయి (' ');
}

/* If there are FIELDS fields, return the total padding up to the
   start of field I, where I < FIELDS.  PAD is the total padding for
   all fields.  The result equals (PAD * I) / FIELDS, except it does
   not suffer from internal overflow.  */
స్థిర idx_t
pad_at (idx_t fields, idx_t i, idx_t pad)
{
  /* This implementation assumes that (FIELDS - 1)^2 does not overflow
     intmax_t, an assumption checked by pad_at_overflow.  */
  intmax_t m = pad % fields;
  తిరిగి pad / fields * i + m * i / fields;
}
స్థిర bool
pad_at_అధికప్రవాహం (idx_t fields)
{
  intmax_t లబ్ధం;
  తిరిగి ckd_mul (&లబ్ధం, fields - 1, fields - 1);
}

#నిర్వచించు PRINT_FIELDS(N, T, FMT_STRING_DECL, ACTION)                     \
స్థిర శూన్య                                                             \
N (idx_t fields, idx_t blank, శూన్య స్థిరరాశి *ఖండం,			\
   FMT_STRING_DECL, పూర్ణాంకం వెడల్పు, idx_t pad)				\
{                                                                       \
  T స్థిరరాశి *p = ఖండం;                                                   \
  idx_t pad_మిగిలిన = pad;						\
  కొరకు (idx_t i = fields; blank < i; i--)				\
    {                                                                   \
      idx_t తదుపరి_pad = pad_at (fields, i - 1, pad);			\
      పూర్ణాంకం adjusted_వెడల్పు = pad_మిగిలిన - తదుపరి_pad + వెడల్పు;            \
      T x;                                                              \
      ఒకవేళ (ఆదానం_మార్పిడి && పరిమాణం (T) > 1)                                 \
        {                                                               \
          సంఘం {                                                       \
            T x;                                                        \
            అక్షరం b[పరిమాణం (T)];                                         \
          } u;                                                          \
          కొరకు (idx_t j = 0; j < పరిమాణం (T); j++)			\
            u.b[j] = ((అక్షరం స్థిరరాశి *) p)[పరిమాణం (T) - 1 - j];            \
          x = u.x;                                                      \
        }                                                               \
      లేకపోతే                                                              \
        x = *p;                                                         \
      p++;                                                              \
      ACTION;                                                           \
      pad_మిగిలిన = తదుపరి_pad;                                         \
    }                                                                   \
}

#నిర్వచించు PRINT_TYPE(N, T)                                                \
  PRINT_FIELDS (N, T, అక్షరం స్థిరరాశి *fmt_string,                           \
                xprintf (fmt_string, adjusted_వెడల్పు, x))

#నిర్వచించు PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
  PRINT_FIELDS (N, T, MAYBE_వాడని అక్షరం స్థిరరాశి *fmt_string,              \
                అక్షరం buf[BUFSIZE];                                      \
                FTOASTR (buf, పరిమాణం buf, 0, 0, x);                     \
                xprintf ("%*s", adjusted_వెడల్పు, buf))

PRINT_TYPE (print_s_char, గుర్తుగల అక్షరం)
PRINT_TYPE (print_char, గుర్తులేని అక్షరం)
PRINT_TYPE (print_s_short, పొట్టి పూర్ణాంకం)
PRINT_TYPE (print_short, గుర్తులేని పొట్టి పూర్ణాంకం)
PRINT_TYPE (print_int, గుర్తులేని పూర్ణాంకం)
PRINT_TYPE (print_long, గుర్తులేని దీర్ఘ పూర్ణాంకం)
PRINT_TYPE (print_long_long, గుర్తులేని దీర్ఘ దీర్ఘ పూర్ణాంకం)
PRINT_TYPE (print_intmax, uintmax_t)

PRINT_FLOATTYPE (print_bfloat, bfloat16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_halffloat, float16, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_float, దశాంశం, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_double, ద్విగుణ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_long_double, దీర్ఘ ద్విగుణ, ldtoastr, LDBL_BUFSIZE_BOUND)

#నిర్వచనం_రద్దు PRINT_TYPE
#నిర్వచనం_రద్దు PRINT_FLOATTYPE

స్థిర శూన్య
dump_hexl_బహుళకం_trailer (idx_t n_బైట్లు, అక్షరం స్థిరరాశి *ఖండం)
{
  fputs ("  >", stdout);
  కొరకు (idx_t i = n_బైట్లు; i > 0; i--)
    {
      గుర్తులేని అక్షరం c = *ఖండం++;
      గుర్తులేని అక్షరం c2 = (isprint (c) ? c : '.');
      అక్షరం_రాయి (c2);
    }
  అక్షరం_రాయి ('<');
}

స్థిర శూన్య
print_named_ascii (idx_t fields, idx_t blank, శూన్య స్థిరరాశి *ఖండం,
                   MAYBE_వాడని అక్షరం స్థిరరాశి *వాడని_fmt_string,
                   పూర్ణాంకం వెడల్పు, idx_t pad)
{
  గుర్తులేని అక్షరం స్థిరరాశి *p = ఖండం;
  idx_t pad_మిగిలిన = pad;
  కొరకు (idx_t i = fields; blank < i; i--)
    {
      పూర్ణాంకం masked_c = *p++ & 0x7f;
      అక్షరం స్థిరరాశి *s;
      అక్షరం buf[2];

      ఒకవేళ (masked_c == 127)
        s = "del";
      లేకపోతే ఒకవేళ (masked_c <= 040)
        s = charname[masked_c];
      లేకపోతే
        {
          buf[0] = masked_c;
          buf[1] = 0;
          s = buf;
        }

      idx_t తదుపరి_pad = pad_at (fields, i - 1, pad);
      పూర్ణాంకం adjusted_వెడల్పు = pad_మిగిలిన - తదుపరి_pad + వెడల్పు;
      xprintf ("%*s", adjusted_వెడల్పు, s);
      pad_మిగిలిన = తదుపరి_pad;
    }
}

స్థిర శూన్య
print_ascii (idx_t fields, idx_t blank, శూన్య స్థిరరాశి *ఖండం,
             MAYBE_వాడని అక్షరం స్థిరరాశి *వాడని_fmt_string, పూర్ణాంకం వెడల్పు,
             idx_t pad)
{
  గుర్తులేని అక్షరం స్థిరరాశి *p = ఖండం;
  idx_t pad_మిగిలిన = pad;
  కొరకు (idx_t i = fields; blank < i; i--)
    {
      గుర్తులేని అక్షరం c = *p++;
      అక్షరం స్థిరరాశి *s;
      అక్షరం buf[4];

      ఎంపిక (c)
        {
        స్థితి '\0':
          s = "\\0";
          ఆపు;

        స్థితి '\a':
          s = "\\a";
          ఆపు;

        స్థితి '\b':
          s = "\\b";
          ఆపు;

        స్థితి '\f':
          s = "\\f";
          ఆపు;

        స్థితి '\n':
          s = "\\n";
          ఆపు;

        స్థితి '\r':
          s = "\\r";
          ఆపు;

        స్థితి '\t':
          s = "\\t";
          ఆపు;

        స్థితి '\v':
          s = "\\v";
          ఆపు;

        అప్రమేయ:
          sprintf (buf, (isprint (c) ? "%c" : "%03o"), c);
          s = buf;
        }

      idx_t తదుపరి_pad = pad_at (fields, i - 1, pad);
      పూర్ణాంకం adjusted_వెడల్పు = pad_మిగిలిన - తదుపరి_pad + వెడల్పు;
      xprintf ("%*s", adjusted_వెడల్పు, s);
      pad_మిగిలిన = తదుపరి_pad;
    }
}

/* Convert a null-terminated (possibly zero-length) string S to an
   int value.  If S points to a non-digit set *P to S,
   *VAL to 0, and return true.  Otherwise, accumulate the integer value of
   the string of digits.  If the string of digits represents a value
   larger than INT_MAX, don't modify *VAL or *P and return false.
   Otherwise, advance *P to the first non-digit after S, set *VAL to
   the result of the conversion and return true.  */

స్థిర bool
simple_strtoi (అక్షరం స్థిరరాశి *s, అక్షరం స్థిరరాశి **p, పూర్ణాంకం *val)
{
  పూర్ణాంకం కూడిక;

  కొరకు (కూడిక = 0; c_isdigit (*s); s++)
    ఒకవేళ (ckd_mul (&కూడిక, కూడిక, 10) || ckd_కూడు (&కూడిక, కూడిక, *s - '0'))
      తిరిగి false;
  *p = s;
  *val = కూడిక;
  తిరిగి true;
}

/* Return a printf format for SPEC, given that remaining arguments are
   formats for int, long, long long, and intmax_t, respectively.  */
స్థిర అక్షరం స్థిరరాశి *
ispec_గమ్యం_format (గణన కొలత_spec కొలత_spec,
                 అక్షరం స్థిరరాశి *int_format,
                 అక్షరం స్థిరరాశి *long_format,
                 అక్షరం స్థిరరాశి *long_long_format,
                 అక్షరం స్థిరరాశి *intmax_format)
{
  తిరిగి (LONG < INTMAX && కొలత_spec == INTMAX ? intmax_format
          : LONG < LONG_LONG && కొలత_spec == LONG_LONG ? long_long_format
          : INT < LONG && కొలత_spec == LONG ? long_format
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

స్థిర bool ATTRIBUTE_NONNULL ()
decode_one_format (అక్షరం స్థిరరాశి *s_orig, అక్షరం స్థిరరాశి *s, అక్షరం స్థిరరాశి **తదుపరి,
                   నిర్మాణం tspec *tspec)
{
  గణన కొలత_spec కొలత_spec;
  పూర్ణాంకం కొలత;
  గణన output_format fmt;
  print_function_type print_function;
  అక్షరం స్థిరరాశి *p;
  అక్షరం c;
  పూర్ణాంకం క్షేత్రం_వెడల్పు;

  ఎంపిక (*s)
    {
    స్థితి 'd':
    స్థితి 'o':
    స్థితి 'u':
    స్థితి 'x':
      c = *s;
      ++s;
      ఎంపిక (*s)
        {
        స్థితి 'C':
          ++s;
          కొలత = పరిమాణం (గుర్తులేని అక్షరం);
          ఆపు;

        స్థితి 'S':
          ++s;
          కొలత = పరిమాణం (గుర్తులేని పొట్టి పూర్ణాంకం);
          ఆపు;

        స్థితి 'I':
          ++s;
          కొలత = పరిమాణం (గుర్తులేని పూర్ణాంకం);
          ఆపు;

        స్థితి 'L':
          ++s;
          కొలత = పరిమాణం (గుర్తులేని దీర్ఘ పూర్ణాంకం);
          ఆపు;

        అప్రమేయ:
          ఒకవేళ (! simple_strtoi (s, &p, &కొలత))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              తిరిగి false;
            }
          ఒకవేళ (p == s)
            కొలత = పరిమాణం (గుర్తులేని పూర్ణాంకం);
          లేకపోతే
            {
              ఒకవేళ (countof (integral_type_కొలత) <= కొలత
                  || integral_type_కొలత[కొలత] == NO_కొలత)
                {
                  error (0, 0, _("invalid type string %s;\nthis system"
                                 " doesn't provide a %d-byte integral type"),
                         quote (s_orig), కొలత);
                  తిరిగి false;
                }
              s = p;
            }
          ఆపు;
        }

      కొలత_spec = integral_type_కొలత[కొలత];

      ఎంపిక (c)
        {
        స్థితి 'd':
          fmt = SIGNED_DECIMAL;
          క్షేత్రం_వెడల్పు = INT_బిట్లు_STRLEN_BOUND (CHAR_BIT * కొలత - 1) + 1;
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_గమ్యం_format (కొలత_spec, "d", "ld", "lld", "jd"));
          ఆపు;

        స్థితి 'o':
          fmt = OCTAL;
          క్షేత్రం_వెడల్పు = (CHAR_BIT * కొలత + 2) / 3;
          sprintf (tspec->fmt_string, "%%*.%d%s", క్షేత్రం_వెడల్పు,
                   ispec_గమ్యం_format (కొలత_spec, "o", "lo", "llo", "jo"));
          ఆపు;

        స్థితి 'u':
          fmt = UNSIGNED_DECIMAL;
          క్షేత్రం_వెడల్పు = INT_బిట్లు_STRLEN_BOUND (CHAR_BIT * కొలత);
          sprintf (tspec->fmt_string, "%%*%s",
                   ispec_గమ్యం_format (కొలత_spec, "u", "lu", "llu", "ju"));
          ఆపు;

        స్థితి 'x':
          fmt = HEXADECIMAL;
          క్షేత్రం_వెడల్పు = (CHAR_BIT * కొలత + 3) / 4;
          sprintf (tspec->fmt_string, "%%*.%d%s", క్షేత్రం_వెడల్పు,
                   ispec_గమ్యం_format (కొలత_spec, "x", "lx", "llx", "jx"));
          ఆపు;

        అప్రమేయ:
          affirm (false);
        }

      /* Prefer INT, prefer LONG to longer types,
         and prefer INTMAX to LONG_LONG.  */
      print_function
        = (కొలత_spec == INT
           ? print_int
           : SHORT < INT && కొలత_spec == SHORT
           ? (fmt == SIGNED_DECIMAL ? print_s_short : print_short)
           : CHAR < SHORT && కొలత_spec == CHAR
           ? (fmt == SIGNED_DECIMAL ? print_s_char : print_char)
           : INT < LONG && కొలత_spec == LONG
           ? print_long
           : LONG < INTMAX && కొలత_spec == INTMAX
           ? print_intmax
           : LONG < LONG_LONG && LONG_LONG < INTMAX && కొలత_spec == LONG_LONG
           ? print_long_long
           : (affirm (false), (print_function_type) ఖాళీ));
      ఆపు;

    స్థితి 'f':
      fmt = FLOATING_బిందువు;
      ++s;
      ఎంపిక (*s)
        {
        స్థితి 'B':
          ++s;
          fmt = BFLOATING_బిందువు;
          కొలత = పరిమాణం (bfloat16);
          ఆపు;

        స్థితి 'H':
          ++s;
          fmt = HFLOATING_బిందువు;
          కొలత = పరిమాణం (float16);
          ఆపు;

        స్థితి 'F':
          ++s;
          కొలత = పరిమాణం (దశాంశం);
          ఆపు;

        స్థితి 'D':
          ++s;
          కొలత = పరిమాణం (ద్విగుణ);
          ఆపు;

        స్థితి 'L':
          ++s;
          కొలత = పరిమాణం (దీర్ఘ ద్విగుణ);
          ఆపు;

        అప్రమేయ:
          ఒకవేళ (! simple_strtoi (s, &p, &కొలత))
            {
              /* The integer at P in S would overflow an int.
                 A digit string that long is sufficiently odd looking
                 that the following diagnostic is sufficient.  */
              error (0, 0, _("invalid type string %s"), quote (s_orig));
              తిరిగి false;
            }
          ఒకవేళ (p == s)
            కొలత = పరిమాణం (ద్విగుణ);
          లేకపోతే
            {
              ఒకవేళ (countof (fp_type_కొలత) <= కొలత
                  || fp_type_కొలత[కొలత] == NO_కొలత
                  || (! FLOAT16_SUPPORTED && BF16_SUPPORTED
                      && కొలత == పరిమాణం (bfloat16)))
                {
                  error (0, 0,
                         _("invalid type string %s;\n"
                           "this system doesn't provide a %d-byte"
                           " floating point type"),
                         quote (s_orig), కొలత);
                  తిరిగి false;
                }
              s = p;
            }
          ఆపు;
        }
      కొలత_spec = fp_type_కొలత[కొలత];

      ఒకవేళ ((! FLOAT16_SUPPORTED && fmt == HFLOATING_బిందువు)
          || (! BF16_SUPPORTED && fmt == BFLOATING_బిందువు))
      {
        error (0, 0,
               _("this system doesn't provide a %s floating point type"),
               quote (s_orig));
        తిరిగి false;
      }

      {
        నిర్మాణం lconv స్థిరరాశి *locale = localeconv ();
        idx_t decimal_బిందువు_len =
          (locale->decimal_బిందువు[0] ? అక్షరశ్రేణి_పొడవు (locale->decimal_బిందువు) : 1);

        ఒకవేళ (కొలత_spec == FLOAT_DOUBLE)
          {
            print_function = print_double;
            క్షేత్రం_వెడల్పు = DBL_STRLEN_BOUND_L (decimal_బిందువు_len);
          }
        లేకపోతే ఒకవేళ (FLOAT_SINGLE < FLOAT_DOUBLE && కొలత_spec == FLOAT_SINGLE)
          {
            print_function = print_float;
            క్షేత్రం_వెడల్పు = FLT_STRLEN_BOUND_L (decimal_బిందువు_len);
          }
        లేకపోతే ఒకవేళ (FLOAT_HALF < FLOAT_SINGLE && కొలత_spec == FLOAT_HALF)
          {
            print_function = fmt == BFLOATING_బిందువు
                             ? print_bfloat : print_halffloat;
            క్షేత్రం_వెడల్పు = FLT_STRLEN_BOUND_L (decimal_బిందువు_len);
          }
        లేకపోతే ఒకవేళ (FLOAT_DOUBLE < FLOAT_LONG_DOUBLE
                 && కొలత_spec == FLOAT_LONG_DOUBLE)
          {
            print_function = print_long_double;
            క్షేత్రం_వెడల్పు = LDBL_STRLEN_BOUND_L (decimal_బిందువు_len);
          }
        లేకపోతే
          affirm (false);

        ఆపు;
      }

    స్థితి 'a':
      ++s;
      fmt = NAMED_CHARACTER;
      కొలత_spec = CHAR;
      print_function = print_named_ascii;
      క్షేత్రం_వెడల్పు = 3;
      ఆపు;

    స్థితి 'c':
      ++s;
      fmt = CHARACTER;
      కొలత_spec = CHAR;
      print_function = print_ascii;
      క్షేత్రం_వెడల్పు = 3;
      ఆపు;

    అప్రమేయ:
      error (0, 0, _("invalid character '%c' in type string %s"),
             *s, quote (s_orig));
      తిరిగి false;
    }

  tspec->కొలత = కొలత_spec;
  tspec->fmt = fmt;
  tspec->print_function = print_function;

  tspec->క్షేత్రం_వెడల్పు = క్షేత్రం_వెడల్పు;
  tspec->hexl_బహుళకం_trailer = (*s == 'z');
  ఒకవేళ (tspec->hexl_బహుళకం_trailer)
    s++;

  *తదుపరి = s;
  తిరిగి true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM and the global string INPUT_FILENAME to the
   first one that can be successfully opened. Modify FILE_LIST to
   reference the next filename in the list.  A file name of "-" is
   interpreted as standard input.  If any file open fails, give an error
   message and return false.  */

స్థిర bool
open_తదుపరి_file (శూన్య)
{
  bool ok = true;

  చేయి
    {
      ఆదానం_filename = *file_list;
      ఒకవేళ (ఆదానం_filename == ఖాళీ)
        తిరిగి ok;
      ++file_list;

      ఒకవేళ (streq (ఆదానం_filename, "-"))
        {
          ఆదానం_filename = _("standard input");
          లోపల_ప్రవాహం = stdin;
          have_read_stdin = true;
          xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
        }
      లేకపోతే
        {
          లోపల_ప్రవాహం = fopen (ఆదానం_filename, (O_ద్విమాన ? "rb" : "r"));
          ఒకవేళ (లోపల_ప్రవాహం == ఖాళీ)
            {
              error (0, errno, "%s", quotef (ఆదానం_filename));
              ok = false;
            }
        }
    }
  అయితే (లోపల_ప్రవాహం == ఖాళీ);

  ఒకవేళ (0 <= చివర_స్థానభ్రంశం && !flag_dump_అక్షరశ్రేణులు)
    setvbuf (లోపల_ప్రవాహం, ఖాళీ, _IONBF, 0);

  తిరిగి ok;
}

/* Test whether there have been errors on in_stream, and close it if
   it is not standard input.  Return false if there has been an error
   on in_stream or stdout; return true otherwise.  This function will
   report more than one error only if both a read and a write error
   have occurred.  IN_ERRNO, if nonzero, is the error number
   corresponding to the most recent action for IN_STREAM.  */

స్థిర bool
check_and_close (పూర్ణాంకం లోపల_errno)
{
  bool ok = true;

  ఒకవేళ (లోపల_ప్రవాహం != ఖాళీ)
    {
      ఒకవేళ (!ferror (లోపల_ప్రవాహం))
        లోపల_errno = 0;
      ఒకవేళ (streq (file_list[-1], "-"))
        clearerr (లోపల_ప్రవాహం);
      లేకపోతే ఒకవేళ (fclose (లోపల_ప్రవాహం) != 0 && !లోపల_errno)
        లోపల_errno = errno;
      ఒకవేళ (లోపల_errno)
        {
          error (0, లోపల_errno, "%s", quotef (ఆదానం_filename));
          ok = false;
        }

      లోపల_ప్రవాహం = ఖాళీ;
    }

  ఒకవేళ (ferror (stdout))
    {
      error (0, 0, _("write error"));
      ok = false;
    }

  తిరిగి ok;
}

/* Decode the modern od format string S.  Append the decoded
   representation to the global array SPEC, reallocating SPEC if
   necessary.  Return true if S is valid.  */

స్థిర bool ATTRIBUTE_NONNULL ()
decode_format_string (అక్షరం స్థిరరాశి *s)
{
  అక్షరం స్థిరరాశి *s_orig = s;

  అయితే (*s != '\0')
    {
      అక్షరం స్థిరరాశి *తదుపరి;

      ఒకవేళ (n_specs_allocated <= n_specs)
        spec = xpalloc (spec, &n_specs_allocated, 1, -1, పరిమాణం *spec);

      ఒకవేళ (! decode_one_format (s_orig, s, &తదుపరి, &spec[n_specs]))
        తిరిగి false;

      affirm (s != తదుపరి);
      s = తదుపరి;
      ++n_specs;
    }

  తిరిగి true;
}

/* Given a list of one or more input filenames FILE_LIST, set the global
   file pointer IN_STREAM to position N_SKIP in the concatenation of
   those files.  If any file operation fails or if there are fewer than
   N_SKIP bytes in the combined input, give an error message and return
   false.  When possible, use seek rather than read operations to
   advance IN_STREAM.  */

స్థిర bool
skip (intmax_t n_skip)
{
  bool ok = true;
  పూర్ణాంకం లోపల_errno = 0;

  ఒకవేళ (n_skip == 0)
    తిరిగి true;

  అయితే (లోపల_ప్రవాహం != ఖాళీ)	/* EOF.  */
    {
      నిర్మాణం stat file_stats;

      /* First try seeking.  For large offsets, this extra work is
         worthwhile.  If the offset is below some threshold it may be
         more efficient to move the pointer by reading.  There are two
         issues when trying to seek:
           - the file must be seekable.
           - before seeking to the specified position, make sure
             that the new position is in the current file.
             Try to do that by getting file's size using fstat.
             But that will work only for regular files.  */

      ఒకవేళ (fstat (fileno (లోపల_ప్రవాహం), &file_stats) == 0)
        {
          /* The st_size field is valid for regular files.
             If the number of bytes left to skip is larger than
             the size of the current file, we can decrement n_skip
             and go on to the next file.  Skip this optimization also
             when st_size is no greater than the block size, because
             some kernels report nonsense small file sizes for
             proc-like file systems.  */
          ఒకవేళ (S_ISREG (file_stats.st_బహుళకం)
              && STP_BLKSIZE (&file_stats) < file_stats.st_కొలత)
            {
              ఒకవేళ (file_stats.st_కొలత < n_skip)
                n_skip -= file_stats.st_కొలత;
              లేకపోతే
                {
                  ఒకవేళ (fseeko (లోపల_ప్రవాహం, n_skip, SEEK_CUR) != 0)
                    {
                      లోపల_errno = errno;
                      ok = false;
                    }
                  n_skip = 0;
                }
            }

          లేకపోతే ఒకవేళ (! (S_ISREG (file_stats.st_బహుళకం)
                      || S_TYPEISSHM (&file_stats)
                      || S_TYPEISTMO (&file_stats))
                   && fseeko (లోపల_ప్రవాహం, n_skip, SEEK_CUR) == 0)
            n_skip = 0;

          /* If it's neither a seekable file with unusable size, nor a
             regular file so large it can't be in a proc-like file system,
             position the file pointer by reading.  */

          లేకపోతే
            {
              అక్షరం buf[BUFSIZ];
              idx_t n_బైట్లు_read, n_బైట్లు_గమ్యం_read = BUFSIZ;

              అయితే (0 < n_skip)
                {
                  ఒకవేళ (n_skip < n_బైట్లు_గమ్యం_read)
                    n_బైట్లు_గమ్యం_read = n_skip;
                  n_బైట్లు_read = fread (buf, 1, n_బైట్లు_గమ్యం_read, లోపల_ప్రవాహం);
                  n_skip -= n_బైట్లు_read;
                  ఒకవేళ (n_బైట్లు_read != n_బైట్లు_గమ్యం_read)
                    {
                      ఒకవేళ (ferror (లోపల_ప్రవాహం))
                        {
                          లోపల_errno = errno;
                          ok = false;
                          n_skip = 0;
                          ఆపు;
                        }
                      ఒకవేళ (feof (లోపల_ప్రవాహం))
                        ఆపు;
                    }
                }
            }

          ఒకవేళ (n_skip == 0)
            ఆపు;
        }

      లేకపోతే   /* cannot fstat() file */
        {
          error (0, errno, "%s", quotef (ఆదానం_filename));
          ok = false;
        }

      ok &= check_and_close (లోపల_errno);

      ok &= open_తదుపరి_file ();
    }

  ఒకవేళ (n_skip != 0)
    error (EXIT_FAILURE, 0, _("cannot skip past end of combined input"));

  తిరిగి ok;
}

స్థిర శూన్య
format_చిరునామా_none (MAYBE_వాడని intmax_t చిరునామా,
                     MAYBE_వాడని అక్షరం c)
{
}

స్థిర శూన్య
format_చిరునామా_std (intmax_t చిరునామా, అక్షరం c)
{
  అక్షరం buf[MAX_చిరునామా_నిడివి + 2];
  అక్షరం *p = buf + పరిమాణం buf;
  అక్షరం స్థిరరాశి *pbound;

  *--p = '\0';
  *--p = c;
  pbound = p - చిరునామా_pad_len;

  /* Use a special case of the code for each base.  This is measurably
     faster than generic code.  */
  ఎంపిక (చిరునామా_పునాది)
    {
    స్థితి 8:
      చేయి
        *--p = '0' + (చిరునామా & 7);
      అయితే ((చిరునామా >>= 3) != 0);
      ఆపు;

    స్థితి 10:
      చేయి
        *--p = '0' + (చిరునామా % 10);
      అయితే ((చిరునామా /= 10) != 0);
      ఆపు;

    స్థితి 16:
      చేయి
        *--p = "0123456789abcdef"[చిరునామా & 15];
      అయితే ((చిరునామా >>= 4) != 0);
      ఆపు;
    }

  అయితే (pbound < p)
    *--p = '0';

  fputs (p, stdout);
}

స్థిర శూన్య
format_చిరునామా_paren (intmax_t చిరునామా, అక్షరం c)
{
  అక్షరం_రాయి ('(');
  format_చిరునామా_std (చిరునామా, ')');
  ఒకవేళ (c)
    అక్షరం_రాయి (c);
}

స్థిర శూన్య
format_చిరునామా_label (intmax_t చిరునామా, అక్షరం c)
{
  format_చిరునామా_std (చిరునామా, ' ');

  intmax_t addr;
  ఒకవేళ (ckd_కూడు (&addr, చిరునామా, pseudo_స్థానభ్రంశం))
    error (EXIT_FAILURE, 0, _("pseudo address too large for input"));

  format_చిరునామా_paren (addr, c);
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

స్థిర శూన్య
write_ఖండం (intmax_t current_స్థానభ్రంశం, idx_t n_బైట్లు,
             అక్షరం స్థిరరాశి *prev_ఖండం, అక్షరం స్థిరరాశి *curr_ఖండం)
{
  స్థిర bool మొదటి = true;
  స్థిర bool prev_pair_equal = false;

  ఒకవేళ (abbreviate_duplicate_ఖండాలు
      && !మొదటి && n_బైట్లు == బైట్లు_చొప్పున_ఖండం
      && memeq (prev_ఖండం, curr_ఖండం, బైట్లు_చొప్పున_ఖండం))
    {
      ఒకవేళ (prev_pair_equal)
        {
          /* The two preceding blocks were equal, and the current
             block is the same as the last one, so print nothing.  */
        }
      లేకపోతే
        {
          రాయి ("*\n");
          prev_pair_equal = true;
        }
    }
  లేకపోతే
    {
      prev_pair_equal = false;
      కొరకు (idx_t i = 0; i < n_specs; i++)
        {
          పూర్ణాంకం datum_వెడల్పు = వెడల్పు_బైట్లు[spec[i].కొలత];
          idx_t fields_చొప్పున_ఖండం = బైట్లు_చొప్పున_ఖండం / datum_వెడల్పు;
          idx_t blank_fields = (బైట్లు_చొప్పున_ఖండం - n_బైట్లు) / datum_వెడల్పు;
          ఒకవేళ (i == 0)
            format_చిరునామా (current_స్థానభ్రంశం, '\0');
          లేకపోతే
            print_n_spaces (చిరునామా_pad_len);
          (*spec[i].print_function) (fields_చొప్పున_ఖండం, blank_fields,
                                     curr_ఖండం, spec[i].fmt_string,
                                     spec[i].క్షేత్రం_వెడల్పు, spec[i].pad_వెడల్పు);
          ఒకవేళ (spec[i].hexl_బహుళకం_trailer)
            {
              /* Space-pad out to full line width, then dump the trailer.  */
              పూర్ణాంకం క్షేత్రం_వెడల్పు = spec[i].క్షేత్రం_వెడల్పు;
              కొరకు (idx_t f = 0; f < blank_fields; f++)
                print_n_spaces (క్షేత్రం_వెడల్పు);
              idx_t pad_వెడల్పు = pad_at (fields_చొప్పున_ఖండం, blank_fields,
                                        spec[i].pad_వెడల్పు);
              print_n_spaces (pad_వెడల్పు);
              dump_hexl_బహుళకం_trailer (n_బైట్లు, curr_ఖండం);
            }
          అక్షరం_రాయి ('\n');
        }
    }
  మొదటి = false;
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

స్థిర bool
read_char (పూర్ణాంకం *c)
{
  bool ok = true;

  *c = EOF;

  అయితే (లోపల_ప్రవాహం && (*c = getc (లోపల_ప్రవాహం)) < 0)
    {
      ok &= check_and_close (errno);
      ok &= open_తదుపరి_file ();
    }

  తిరిగి ok;
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

స్థిర bool
read_ఖండం (idx_t n, అక్షరం *ఖండం, idx_t *n_బైట్లు_లోపల_బఫరు)
{
  bool ok = true;

  affirm (0 < n && n <= బైట్లు_చొప్పున_ఖండం);

  *n_బైట్లు_లోపల_బఫరు = 0;

  అయితే (లోపల_ప్రవాహం != ఖాళీ)	/* EOF.  */
    {
      idx_t n_needed = n - *n_బైట్లు_లోపల_బఫరు;
      idx_t n_read = fread (ఖండం + *n_బైట్లు_లోపల_బఫరు,
                            1, n_needed, లోపల_ప్రవాహం);

      *n_బైట్లు_లోపల_బఫరు += n_read;

      ఒకవేళ (n_read == n_needed)
        ఆపు;

      ok &= check_and_close (errno);

      ok &= open_తదుపరి_file ();
    }

  తిరిగి ok;
}

/* Return the least common multiple of the sizes associated
   with the format specs.  */

ATTRIBUTE_శుద్ధ
స్థిర పూర్ణాంకం
get_lcm (శూన్య)
{
  పూర్ణాంకం l_c_m = 1;

  కొరకు (idx_t i = 0; i < n_specs; i++)
    l_c_m = lcm (l_c_m, వెడల్పు_బైట్లు[spec[i].కొలత]);
  తిరిగి l_c_m;
}

/* Act like xstrtoimax (NPTR, NULL, BASE, VAL, VALID_SUFFIXES),
   except reject negative values, and *VAL may be set if
   LONGINT_INVALID is returned.  */
స్థిర strtol_error
xstr2nonneg (అక్షరం స్థిరరాశి *పరిమిత nptr, పూర్ణాంకం పునాది_2, intmax_t *val,
             అక్షరం స్థిరరాశి *పరిమిత valid_suffixes)
{
  strtol_error s_err = xstrtoimax (nptr, ఖాళీ, పునాది_2, val, valid_suffixes);
  తిరిగి s_err != LONGINT_INVALID && *val < 0 ? LONGINT_INVALID : s_err;
}

/* If STR is a valid traditional offset specification with an optional
   leading '+', and can be represented in intmax_t, return true and
   set *OFFSET to the offset it denotes.  If it is valid but cannot be
   represented, diagnose the problem and exit.  Otherwise return false
   and possibly set *OFFSET.  */

స్థిర bool
parse_old_స్థానభ్రంశం (అక్షరం *str, intmax_t *స్థానభ్రంశం)
{
  /* Skip over any leading '+'.  */
  అక్షరం *s = str + (str[0] == '+');

  ఒకవేళ (! c_isdigit (s[0]))
    తిరిగి false;

  /* Determine the radix for S.  If there is a '.', followed first by
     optional 'b' or (undocumented) 'B' and then by end of string,
     it's decimal, otherwise, if the string begins with '0X' or '0x',
     it's hexadecimal, else octal.  */
  అక్షరం *dot = strchr (s, '.');
  ఒకవేళ (dot && dot[(dot[1] == 'b' || dot[1] == 'B') + 1])
    dot = ఖాళీ;
  పూర్ణాంకం radix = dot ? 10 : s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ? 16 : 8;

  ఒకవేళ (dot)
    {
      /* Temporarily remove the '.' from the decimal string.  */
      dot[0] = dot[1];
      dot[1] = '\0';
    }

  గణన strtol_error s_err = xstr2nonneg (s, radix, స్థానభ్రంశం, "bB");

  ఒకవేళ (dot)
    {
      /* Restore the decimal string's original value.  */
      dot[1] = dot[0];
      dot[0] = '.';
    }

  ఒకవేళ (s_err == LONGINT_అధికప్రవాహం)
    error (EXIT_FAILURE, ERANGE, "%s", quotef (str));
  తిరిగి s_err == LONGINT_OK;
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

స్థిర bool
dump (శూన్య)
{
  అక్షరం *ఖండం[2];
  bool idx = false;
  bool ok = true;
  idx_t n_బైట్లు_read;

  ఖండం[0] = xinmalloc (1 + abbreviate_duplicate_ఖండాలు, బైట్లు_చొప్పున_ఖండం);
  ఖండం[1] = ఖండం[0] + abbreviate_duplicate_ఖండాలు * బైట్లు_చొప్పున_ఖండం;

  intmax_t current_స్థానభ్రంశం = n_బైట్లు_గమ్యం_skip;

  చేయి
    {
      intmax_t needed_bound
        = చివర_స్థానభ్రంశం < 0 ? INTMAX_MAX : చివర_స్థానభ్రంశం - current_స్థానభ్రంశం;
      ఒకవేళ (needed_bound <= 0)
        {
          n_బైట్లు_read = 0;
          ఆపు;
        }
      idx_t n_needed = MIN (బైట్లు_చొప్పున_ఖండం, needed_bound);
      ok &= read_ఖండం (n_needed, ఖండం[idx], &n_బైట్లు_read);
      ఒకవేళ (n_బైట్లు_read < బైట్లు_చొప్పున_ఖండం)
        ఆపు;
      affirm (n_బైట్లు_read == బైట్లు_చొప్పున_ఖండం);
      write_ఖండం (current_స్థానభ్రంశం, n_బైట్లు_read,
                   ఖండం[!idx], ఖండం[idx]);
      ఒకవేళ (ferror (stdout))
        ok = false;
      current_స్థానభ్రంశం += n_బైట్లు_read;
      idx = !idx;
    }
  అయితే (ok);

  ఒకవేళ (n_బైట్లు_read > 0)
    {
      పూర్ణాంకం l_c_m = get_lcm ();

      /* Ensure zero-byte padding up to the smallest multiple of l_c_m that
         is at least as large as n_bytes_read.  */
      idx_t బైట్లు_గమ్యం_write = (n_బైట్లు_read + l_c_m - 1
                              - (n_బైట్లు_read + l_c_m - 1) % l_c_m);

      memset (ఖండం[idx] + n_బైట్లు_read, 0, బైట్లు_గమ్యం_write - n_బైట్లు_read);
      write_ఖండం (current_స్థానభ్రంశం, n_బైట్లు_read, ఖండం[!idx], ఖండం[idx]);
      current_స్థానభ్రంశం += n_బైట్లు_read;
    }

  ఒకవేళ (ok || current_స్థానభ్రంశం)
    format_చిరునామా (current_స్థానభ్రంశం, '\n');

  ఒకవేళ (0 <= చివర_స్థానభ్రంశం && చివర_స్థానభ్రంశం <= current_స్థానభ్రంశం)
    ok &= check_and_close (0);

  విడుదల (ఖండం[0]);

  తిరిగి ok;
}

/* STRINGS mode.  Find each "string constant" in the input.
   A string constant is a run of at least STRING_MIN
   printable characters terminated by a NUL or END_OFFSET.
   Based on a function written by Richard Stallman for a
   traditional version of od.  Return true if successful.  */

స్థిర bool
dump_అక్షరశ్రేణులు (శూన్య)
{
  idx_t bufsize = MAX (100, string_min + 1);
  అక్షరం *buf = ximalloc (bufsize);
  intmax_t చిరునామా = n_బైట్లు_గమ్యం_skip;
  bool ok = true;

  అయితే (true)
    {
      idx_t i = 0;
      పూర్ణాంకం c = 1;  /* Init to 1 so can distinguish if NUL read.  */

      ఒకవేళ (0 <= చివర_స్థానభ్రంశం
          && (చివర_స్థానభ్రంశం < string_min || చివర_స్థానభ్రంశం - string_min < చిరునామా))
        ఆపు;

      /* Store consecutive printable characters to BUF.  */
      అయితే (! (0 <= చివర_స్థానభ్రంశం && చివర_స్థానభ్రంశం <= చిరునామా))
        {
          ఒకవేళ (i == bufsize - 1)
            buf = xpalloc (buf, &bufsize, 1, -1, పరిమాణం *buf);
          ok &= read_char (&c);
          ఒకవేళ (c < 0)
            {
              విడుదల (buf);
              తిరిగి ok;
            }
          చిరునామా++;
          buf[i++] = c;
          ఒకవేళ (c == '\0')
            ఆపు;		/* Print this string.  */
          ఒకవేళ (! isprint (c))
            {
              c = -1;		/* Give up on this string.  */
              ఆపు;
            }
        }

      ఒకవేళ (c < 0 || i - !c < string_min)
        కొనసాగు;

      buf[i] = 0;

      /* If we get here, the string is all printable, so print it.  */

      format_చిరునామా (చిరునామా - i, ' ');

      కొరకు (i = 0; (c = buf[i]); i++)
        {
          ఎంపిక (c)
            {
            స్థితి '\a':
              fputs ("\\a", stdout);
              ఆపు;

            స్థితి '\b':
              fputs ("\\b", stdout);
              ఆపు;

            స్థితి '\f':
              fputs ("\\f", stdout);
              ఆపు;

            స్థితి '\n':
              fputs ("\\n", stdout);
              ఆపు;

            స్థితి '\r':
              fputs ("\\r", stdout);
              ఆపు;

            స్థితి '\t':
              fputs ("\\t", stdout);
              ఆపు;

            స్థితి '\v':
              fputs ("\\v", stdout);
              ఆపు;

            అప్రమేయ:
              putc (c, stdout);
            }
        }
      అక్షరం_రాయి ('\n');
    }

  /* We reach this point only if we search through
     (max_bytes_to_format - string_min) bytes before reaching EOF.  */

  విడుదల (buf);

  ok &= check_and_close (0);
  తిరిగి ok;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం n_files;
  పూర్ణాంకం l_c_m;
  idx_t desired_వెడల్పు = 0;
  bool modern = false;
  bool ok = true;
  idx_t వెడల్పు_చొప్పున_ఖండం = 0;
  స్థిర అక్షరం స్థిరరాశి multipliers[] = "bEGKkMmPQRTYZ0";

  /* The maximum number of bytes that will be formatted.
     If negative, there is no limit.  */
  intmax_t max_బైట్లు_గమ్యం_format = -1;

  /* The old-style 'pseudo starting address' to be printed in parentheses
     after any true address.  */
  intmax_t pseudo_ప్రారంభం IF_LINT ( = 0);

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే (true)
    {
      intmax_t tmp;
      గణన strtol_error s_err;
      పూర్ణాంకం oi = -1;
      పూర్ణాంకం c = getopt_long (argc, argv, short_options, long_options, &oi);
      ఒకవేళ (c == -1)
        ఆపు;

      ఎంపిక (c)
        {
        స్థితి 'A':
          modern = true;
          ఎంపిక (optarg[0])
            {
            స్థితి 'd':
              format_చిరునామా = format_చిరునామా_std;
              చిరునామా_పునాది = 10;
              చిరునామా_pad_len = 7;
              ఆపు;
            స్థితి 'o':
              format_చిరునామా = format_చిరునామా_std;
              చిరునామా_పునాది = 8;
              చిరునామా_pad_len = 7;
              ఆపు;
            స్థితి 'x':
              format_చిరునామా = format_చిరునామా_std;
              చిరునామా_పునాది = 16;
              చిరునామా_pad_len = 6;
              ఆపు;
            స్థితి 'n':
              format_చిరునామా = format_చిరునామా_none;
              చిరునామా_pad_len = 0;
              ఆపు;
            అప్రమేయ:
              error (EXIT_FAILURE, 0,
                     _("invalid output address radix '%c';"
                       " it must be one character from [doxn]"),
                     optarg[0]);
              ఆపు;
            }
          ఆపు;

        స్థితి 'j':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &n_బైట్లు_గమ్యం_skip, multipliers);
          ఒకవేళ (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          ఆపు;

        స్థితి 'N':
          modern = true;
          s_err = xstr2nonneg (optarg, 0, &max_బైట్లు_గమ్యం_format, multipliers);
          ఒకవేళ (s_err != LONGINT_OK)
            xstrtol_fatal (s_err, oi, c, long_options, optarg);
          ఆపు;

        స్థితి 'S':
          modern = true;
          ఒకవేళ (optarg == ఖాళీ)
            string_min = 3;
          లేకపోతే
            {
              s_err = xstr2nonneg (optarg, 0, &tmp, multipliers);
              /* The minimum string length + 1 must fit in idx_t,
                 since we may allocate a buffer of this size + 1.  */
              idx_t i;
              ఒకవేళ (s_err == LONGINT_OK && ckd_కూడు (&i, tmp, 1))
                s_err = LONGINT_అధికప్రవాహం;
              ఒకవేళ (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
              string_min = tmp;
            }
          flag_dump_అక్షరశ్రేణులు = true;
          ఆపు;

        స్థితి 't':
          modern = true;
          ok &= decode_format_string (optarg);
          ఆపు;

        స్థితి 'v':
          modern = true;
          abbreviate_duplicate_ఖండాలు = false;
          ఆపు;

        స్థితి TRADITIONAL_OPTION:
          traditional = true;
          ఆపు;

        స్థితి ENDIAN_OPTION:
          ఎంపిక (XARGMATCH ("--endian", optarg, endian_వాదనలు, endian_types))
            {
            స్థితి endian_big:
              ఆదానం_మార్పిడి = BYTE_ORDER != BIG_ENDIAN;
              ఆపు;
            స్థితి endian_little:
              ఆదానం_మార్పిడి = BYTE_ORDER != LITTLE_ENDIAN;
              ఆపు;
            }
          ఆపు;

          /* The next several cases map the traditional format
             specification options to the corresponding modern format
             specs.  GNU od accepts any combination of old- and
             new-style options.  Format specification options accumulate.
             The obsolescent and undocumented formats are compatible
             with FreeBSD 4.10 od.  */

#నిర్వచించు CASE_OLD_ARG(old_char,new_string)		\
        స్థితి old_char:					\
          ok &= decode_format_string (new_string);	\
          ఆపు

          CASE_OLD_ARG ('a', "a");
          CASE_OLD_ARG ('b', "o1");
          CASE_OLD_ARG ('c', "c");
          CASE_OLD_ARG ('D', "u4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('d', "u2");
        స్థితి 'F': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('e', "fD"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('f', "fF");
        స్థితి 'X': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('H', "x4"); /* obsolescent and undocumented */
          CASE_OLD_ARG ('i', "dI");
        స్థితి 'I': స్థితి 'L': /* obsolescent and undocumented aliases */
          CASE_OLD_ARG ('l', "dL");
          CASE_OLD_ARG ('O', "o4"); /* obsolescent and undocumented */
        స్థితి 'B': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('o', "o2");
          CASE_OLD_ARG ('s', "d2");
        స్థితి 'h': /* obsolescent and undocumented alias */
          CASE_OLD_ARG ('x', "x2");

#నిర్వచనం_రద్దు CASE_OLD_ARG

        స్థితి 'w':
          modern = true;
          ఒకవేళ (optarg == ఖాళీ)
            {
              desired_వెడల్పు = 32;
            }
          లేకపోతే
            {
              intmax_t w_tmp;
              s_err = xstr2nonneg (optarg, 10, &w_tmp, "");
              ఒకవేళ (s_err == LONGINT_OK)
                {
                  ఒకవేళ (ckd_కూడు (&desired_వెడల్పు, w_tmp, 0))
                    s_err = LONGINT_అధికప్రవాహం;
                  లేకపోతే ఒకవేళ (desired_వెడల్పు == 0)
                    s_err = LONGINT_INVALID;
                }
              ఒకవేళ (s_err != LONGINT_OK)
                xstrtol_fatal (s_err, oi, c, long_options, optarg);
            }
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
          ఆపు;
        }
    }

  ఒకవేళ (!ok)
    తిరిగి EXIT_FAILURE;

  ఒకవేళ (flag_dump_అక్షరశ్రేణులు && n_specs > 0)
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

  ఒకవేళ (!modern || traditional)
    {
      intmax_t o1;
      intmax_t o2;

      ఎంపిక (n_files)
        {
        స్థితి 1:
          ఒకవేళ ((traditional || argv[optind][0] == '+')
              && parse_old_స్థానభ్రంశం (argv[optind], &o1))
            {
              n_బైట్లు_గమ్యం_skip = o1;
              --n_files;
              ++argv;
            }
          ఆపు;

        స్థితి 2:
          ఒకవేళ ((traditional || argv[optind + 1][0] == '+'
               || c_isdigit (argv[optind + 1][0]))
              && parse_old_స్థానభ్రంశం (argv[optind + 1], &o2))
            {
              ఒకవేళ (traditional && parse_old_స్థానభ్రంశం (argv[optind], &o1))
                {
                  n_బైట్లు_గమ్యం_skip = o1;
                  flag_pseudo_ప్రారంభం = true;
                  pseudo_ప్రారంభం = o2;
                  argv += 2;
                  n_files -= 2;
                }
              లేకపోతే
                {
                  n_బైట్లు_గమ్యం_skip = o2;
                  --n_files;
                  argv[optind + 1] = argv[optind];
                  ++argv;
                }
            }
          ఆపు;

        స్థితి 3:
          ఒకవేళ (traditional
              && parse_old_స్థానభ్రంశం (argv[optind + 1], &o1)
              && parse_old_స్థానభ్రంశం (argv[optind + 2], &o2))
            {
              n_బైట్లు_గమ్యం_skip = o1;
              flag_pseudo_ప్రారంభం = true;
              pseudo_ప్రారంభం = o2;
              argv[optind + 2] = argv[optind];
              argv += 2;
              n_files -= 2;
            }
          ఆపు;
        }

      ఒకవేళ (traditional && 1 < n_files)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
          error (0, 0, "%s",
                 _("compatibility mode supports at most one file"));
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (flag_pseudo_ప్రారంభం)
    {
      ఒకవేళ (format_చిరునామా == format_చిరునామా_none)
        {
          చిరునామా_పునాది = 8;
          చిరునామా_pad_len = 7;
          format_చిరునామా = format_చిరునామా_paren;
        }
      లేకపోతే
        format_చిరునామా = format_చిరునామా_label;
    }

  ఒకవేళ (0 <= max_బైట్లు_గమ్యం_format
      && ckd_కూడు (&చివర_స్థానభ్రంశం, n_బైట్లు_గమ్యం_skip, max_బైట్లు_గమ్యం_format))
    error (EXIT_FAILURE, 0, _("skip-bytes + read-bytes is too large"));

  ఒకవేళ (n_specs == 0)
    decode_format_string ("oS");

  ఒకవేళ (n_files > 0)
    {
      /* Set the global pointer FILE_LIST so that it
         references the first file-argument on the command-line.  */

      file_list = (అక్షరం స్థిరరాశి *స్థిరరాశి *) &argv[optind];
    }
  లేకపోతే
    {
      /* No files were listed on the command line.
         Set the global pointer FILE_LIST so that it
         references the null-terminated list of one name: "-".  */

      file_list = default_file_list;
    }

  /* open the first input file */
  ok = open_తదుపరి_file ();
  ఒకవేళ (లోపల_ప్రవాహం == ఖాళీ)
    వెళ్ళు cleanup;

  /* skip over any unwanted header bytes */
  ok &= skip (n_బైట్లు_గమ్యం_skip);
  ఒకవేళ (లోపల_ప్రవాహం == ఖాళీ)
    వెళ్ళు cleanup;

  pseudo_స్థానభ్రంశం = (flag_pseudo_ప్రారంభం ? pseudo_ప్రారంభం - n_బైట్లు_గమ్యం_skip : 0);

  /* Compute output block length.  */
  l_c_m = get_lcm ();

  ఒకవేళ (desired_వెడల్పు != 0)
    {
      ఒకవేళ (desired_వెడల్పు % l_c_m == 0)
        బైట్లు_చొప్పున_ఖండం = desired_వెడల్పు;
      లేకపోతే
        {
          error (0, 0, _("warning: invalid width %td; using %d instead"),
                 desired_వెడల్పు, l_c_m);
          బైట్లు_చొప్పున_ఖండం = l_c_m;
        }
    }
  లేకపోతే
    {
      ఒకవేళ (l_c_m < DEFAULT_బైట్లు_చొప్పున_ఖండం)
        బైట్లు_చొప్పున_ఖండం = (DEFAULT_బైట్లు_చొప్పున_ఖండం
                           - DEFAULT_బైట్లు_చొప్పున_ఖండం % l_c_m);
      లేకపోతే
        బైట్లు_చొప్పున_ఖండం = l_c_m;
    }

  /* Compute padding necessary to align output block.  */
  కొరకు (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_చొప్పున_ఖండం = బైట్లు_చొప్పున_ఖండం / వెడల్పు_బైట్లు[spec[i].కొలత];
      ఒకవేళ (pad_at_అధికప్రవాహం (fields_చొప్పున_ఖండం))
        error (EXIT_FAILURE, 0, _("%td is too large"), desired_వెడల్పు);
      idx_t ఖండం_వెడల్పు = (spec[i].క్షేత్రం_వెడల్పు + 1) * fields_చొప్పున_ఖండం;
      ఒకవేళ (వెడల్పు_చొప్పున_ఖండం < ఖండం_వెడల్పు)
        వెడల్పు_చొప్పున_ఖండం = ఖండం_వెడల్పు;
    }
  కొరకు (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_చొప్పున_ఖండం = బైట్లు_చొప్పున_ఖండం / వెడల్పు_బైట్లు[spec[i].కొలత];
      idx_t ఖండం_వెడల్పు = spec[i].క్షేత్రం_వెడల్పు * fields_చొప్పున_ఖండం;
      spec[i].pad_వెడల్పు = వెడల్పు_చొప్పున_ఖండం - ఖండం_వెడల్పు;
    }

#నిర్వచితమైతే DEBUG
  రాయి ("lcm=%d, width_per_block=%td\n", l_c_m, వెడల్పు_చొప్పున_ఖండం);
  కొరకు (idx_t i = 0; i < n_specs; i++)
    {
      idx_t fields_చొప్పున_ఖండం = బైట్లు_చొప్పున_ఖండం / వెడల్పు_బైట్లు[spec[i].కొలత];
      affirm (బైట్లు_చొప్పున_ఖండం % వెడల్పు_బైట్లు[spec[i].కొలత] == 0);
      affirm (1 <= spec[i].pad_వెడల్పు / fields_చొప్పున_ఖండం);
      రాయి ("%td: fmt=\"%s\" in_width=%d out_width=%d pad=%td\n",
              i, spec[i].fmt_string, వెడల్పు_బైట్లు[spec[i].కొలత],
              spec[i].క్షేత్రం_వెడల్పు, spec[i].pad_వెడల్పు);
    }
#అయితే_ముగింపు

  ok &= (flag_dump_అక్షరశ్రేణులు ? dump_అక్షరశ్రేణులు () : dump ());

cleanup:

  ఒకవేళ (have_read_stdin && fclose (stdin) < 0)
    error (EXIT_FAILURE, errno, _("standard input"));

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
