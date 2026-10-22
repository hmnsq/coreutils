/*
   BLAKE2 reference source code package - b2sum tool

   Copyright 2012, Samuel Neves <sneves@dei.uc.pt>.  You may use this under the
   terms of the CC0, the OpenSSL Licence, or the Apache Public License 2.0, at
   your option.  The terms of these licenses can be found at:

   - CC0 1.0 Universal : https://creativecommons.org/publicdomain/zero/1.0
   - OpenSSL license   : https://www.openssl.org/source/license.html
   - Apache 2.0        : https://www.apache.org/licenses/LICENSE-2.0

   More information about the BLAKE2 hash function can be found at
   https://blake2.net.
*/

#اگر_تعریف_شدہ HAVE_CONFIG_H
# include <config.h>
#اگر_ختم

#شامل <stdio.h>
#شامل <stdlib.h>

#شامل "blake2.h"

#اگر 0
/* This will help compatibility with coreutils */
صحیح blake2s_دھارا( FILE *دھارا, خالی *resstream, size_t outbytes )
{
  صحیح ret = -1;
  size_t مجموع, n;
  blake2s_state S[1];
  ساکن ثابت size_t بفر_لمبائی = 32768;
  uint8_t *بفر = ( uint8_t * )حافظہ_دو( بفر_لمبائی );

  اگر( !بفر ) واپس -1;

  blake2s_init( S, outbytes );

  جب( 1 )
  {
    مجموع = 0;

    جب( 1 )
    {
      n = fread( بفر + مجموع, 1, بفر_لمبائی - مجموع, دھارا );
      مجموع += n;

      اگر( بفر_لمبائی == مجموع )
        توڑو;

      اگر( 0 == n )
      {
        اگر( ferror( دھارا ) )
          جاؤ cleanup_بفر;

        جاؤ final_process;
      }

      اگر( feof( دھارا ) )
        جاؤ final_process;
    }

    blake2s_update( S, بفر, بفر_لمبائی );
  }

final_process:;

  اگر( مجموع > 0 ) blake2s_update( S, بفر, مجموع );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  آزاد( بفر );
  واپس ret;
}
#اگر_ختم

صحیح blake2b_دھارا( FILE *دھارا, خالی *resstream, size_t outbytes )
{
  صحیح ret = -1;
  size_t مجموع, n;
  blake2b_state S[1];
  ساکن ثابت size_t بفر_لمبائی = 32768;
  uint8_t *بفر = ( uint8_t * )حافظہ_دو( بفر_لمبائی );

  اگر( !بفر ) واپس -1;

  blake2b_init( S, outbytes );

  جب( 1 )
  {
    مجموع = 0;

    جب( 1 )
    {
      n = fread( بفر + مجموع, 1, بفر_لمبائی - مجموع, دھارا );
      مجموع += n;

      اگر( بفر_لمبائی == مجموع )
        توڑو;

      اگر( 0 == n )
      {
        اگر( ferror( دھارا ) )
          جاؤ cleanup_بفر;

        جاؤ final_process;
      }

      اگر( feof( دھارا ) )
        جاؤ final_process;
    }

    blake2b_update( S, بفر, بفر_لمبائی );
  }

final_process:;

  اگر( مجموع > 0 ) blake2b_update( S, بفر, مجموع );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  آزاد( بفر );
  واپس ret;
}

#اگر 0

#شامل <errno.h>
#شامل <getopt.h>
#شامل <string.h>
#شامل <unistd.h>

صحیح blake2sp_دھارا( FILE *دھارا, خالی *resstream, size_t outbytes )
{
  صحیح ret = -1;
  size_t مجموع, n;
  blake2sp_state S[1];
  ساکن ثابت size_t بفر_لمبائی = 16 * ( 1UL << 20 );
  uint8_t *بفر = ( uint8_t * )حافظہ_دو( بفر_لمبائی );

  اگر( !بفر ) واپس -1;

  blake2sp_init( S, outbytes );

  جب( 1 )
  {
    مجموع = 0;

    جب( 1 )
    {
      n = fread( بفر + مجموع, 1, بفر_لمبائی - مجموع, دھارا );
      مجموع += n;

      اگر( بفر_لمبائی == مجموع )
        توڑو;

      اگر( 0 == n )
      {
        اگر( ferror( دھارا ) )
          جاؤ cleanup_بفر;

        جاؤ final_process;
      }

      اگر( feof( دھارا ) )
        جاؤ final_process;
    }

    blake2sp_update( S, بفر, بفر_لمبائی );
  }

final_process:;

  اگر( مجموع > 0 ) blake2sp_update( S, بفر, مجموع );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  آزاد( بفر );
  واپس ret;
}


صحیح blake2bp_دھارا( FILE *دھارا, خالی *resstream, size_t outbytes )
{
  صحیح ret = -1;
  size_t مجموع, n;
  blake2bp_state S[1];
  ساکن ثابت size_t بفر_لمبائی = 16 * ( 1UL << 20 );
  uint8_t *بفر = ( uint8_t * )حافظہ_دو( بفر_لمبائی );

  اگر( !بفر ) واپس -1;

  blake2bp_init( S, outbytes );

  جب( 1 )
  {
    مجموع = 0;

    جب( 1 )
    {
      n = fread( بفر + مجموع, 1, بفر_لمبائی - مجموع, دھارا );
      مجموع += n;

      اگر( بفر_لمبائی == مجموع )
        توڑو;

      اگر( 0 == n )
      {
        اگر( ferror( دھارا ) )
          جاؤ cleanup_بفر;

        جاؤ final_process;
      }

      اگر( feof( دھارا ) )
        جاؤ final_process;
    }

    blake2bp_update( S, بفر, بفر_لمبائی );
  }

final_process:;

  اگر( مجموع > 0 ) blake2bp_update( S, بفر, مجموع );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  آزاد( بفر );
  واپس ret;
}

قسمتعریف صحیح ( *blake2fn )( FILE *, خالی *, size_t );


ساکن خالی usage( حرف **argv, صحیح errcode )
{
  FILE *برآمد = errcode ? stderr : stdout;
  fprintf( برآمد, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( برآمد, "\n" );
  fprintf( برآمد, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( برآمد, "\n" );
  fprintf( برآمد, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( برآمد, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( برآمد, "  --tag        create a BSD-style checksum\n" );
  fprintf( برآمد, "  --help       display this help and exit\n" );
  باہر_نکلو( errcode );
}


صحیح مین( صحیح argc, حرف **argv )
{
  blake2fn blake2_دھارا = blake2b_دھارا;
  غیرعلامتی طویل maxbytes = BLAKE2B_OUTBYTES;
  ثابت حرف *algorithm = "BLAKE2b";
  غیرعلامتی طویل outbytes = 0;
  غیرعلامتی حرف hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  صحیح c, i;
  opterr = 1;

  جب( 1 )
  {
    صحیح option_اشاریہ = 0;
    حرف *انتہا = تہی;
    غیرعلامتی طویل outbits;
    ساکن ساخت option long_options[] = {
      { "help",  no_دلیل, 0,  0  },
      { "tag",   no_دلیل, 0,  0  },
      { تہی, 0, تہی, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_اشاریہ );
    اگر( c == -1 ) توڑو;
    انتخاب( c )
    {
    حالت 'a':
      اگر( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_دھارا = blake2b_دھارا;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      ورنہ اگر ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_دھارا = blake2s_دھارا;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      ورنہ اگر ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_دھارا = blake2bp_دھارا;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      ورنہ اگر ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_دھارا = blake2sp_دھارا;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      ورنہ
      {
        لکھو( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      توڑو;

    حالت 'l':
      outbits = strtoul(optarg, &انتہا, 10);
      اگر( !انتہا || *انتہا != '\0' || outbits % 8 != 0)
      {
        لکھو( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      توڑو;

    حالت 0:
      اگر( 0 == strcmp( "help", long_options[option_اشاریہ].name ) )
        usage( argv, 0 );
      ورنہ اگر( 0 == strcmp( "tag", long_options[option_اشاریہ].name ) )
        bsdstyle = true;
      توڑو;

    حالت '?':
      usage( argv, 1 );
      توڑو;
    }
  }

  اگر(outbytes > maxbytes)
  {
    لکھو( "Invalid length argument: %lu\n", outbytes * 8 );
    لکھو( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  ورنہ اگر( outbytes == 0 )
    outbytes = maxbytes;

  اگر( optind == argc )
    argv[argc++] = (حرف *) "-";

  برائے( i = optind; i < argc; ++i )
  {
    FILE *f = تہی;
    اگر( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    ورنہ
      f = fopen( argv[i], "rb" );

    اگر( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      جاری;
    }

    اگر( blake2_دھارا( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    ورنہ
    {
      size_t j;
      اگر( bsdstyle )
      {
        اگر( outbytes < maxbytes )
          لکھو( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        ورنہ
          لکھو( "%s (%s) = ", algorithm, argv[i] );
      }

      برائے( j = 0; j < outbytes; ++j )
        لکھو( "%02x", hash[j] );

      اگر( bsdstyle )
        لکھو( "\n" );
      ورنہ
        لکھو( "  %s\n", argv[i] );
    }

    اگر( f == stdin )
      clearerr( f );
    ورنہ اگر( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  واپس 0;
}
#اگر_ختم
