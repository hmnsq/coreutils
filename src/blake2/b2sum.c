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

#که_تعریف_شوی HAVE_CONFIG_H
# include <config.h>
#د_که_پای

#ضمن <stdio.h>
#ضمن <stdlib.h>

#ضمن "blake2.h"

#إذا 0
/* This will help compatibility with coreutils */
صحيح blake2s_بهیر( FILE *بهیر, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t مجموعه, n;
  blake2s_state S[1];
  ساكن ثابت size_t بفر_اوږدتیا = 32768;
  uint8_t *بفر = ( uint8_t * )حافظه_ورکړه( بفر_اوږدتیا );

  إذا( !بفر ) أرجع -1;

  blake2s_init( S, outbytes );

  طالما( 1 )
  {
    مجموعه = 0;

    طالما( 1 )
    {
      n = fread( بفر + مجموعه, 1, بفر_اوږدتیا - مجموعه, بهیر );
      مجموعه += n;

      إذا( بفر_اوږدتیا == مجموعه )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( بهیر ) )
          اذهب cleanup_بفر;

        اذهب final_process;
      }

      إذا( feof( بهیر ) )
        اذهب final_process;
    }

    blake2s_update( S, بفر, بفر_اوږدتیا );
  }

final_process:;

  إذا( مجموعه > 0 ) blake2s_update( S, بفر, مجموعه );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  حرر( بفر );
  أرجع ret;
}
#د_که_پای

صحيح blake2b_بهیر( FILE *بهیر, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t مجموعه, n;
  blake2b_state S[1];
  ساكن ثابت size_t بفر_اوږدتیا = 32768;
  uint8_t *بفر = ( uint8_t * )حافظه_ورکړه( بفر_اوږدتیا );

  إذا( !بفر ) أرجع -1;

  blake2b_init( S, outbytes );

  طالما( 1 )
  {
    مجموعه = 0;

    طالما( 1 )
    {
      n = fread( بفر + مجموعه, 1, بفر_اوږدتیا - مجموعه, بهیر );
      مجموعه += n;

      إذا( بفر_اوږدتیا == مجموعه )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( بهیر ) )
          اذهب cleanup_بفر;

        اذهب final_process;
      }

      إذا( feof( بهیر ) )
        اذهب final_process;
    }

    blake2b_update( S, بفر, بفر_اوږدتیا );
  }

final_process:;

  إذا( مجموعه > 0 ) blake2b_update( S, بفر, مجموعه );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  حرر( بفر );
  أرجع ret;
}

#إذا 0

#ضمن <errno.h>
#ضمن <getopt.h>
#ضمن <string.h>
#ضمن <unistd.h>

صحيح blake2sp_بهیر( FILE *بهیر, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t مجموعه, n;
  blake2sp_state S[1];
  ساكن ثابت size_t بفر_اوږدتیا = 16 * ( 1UL << 20 );
  uint8_t *بفر = ( uint8_t * )حافظه_ورکړه( بفر_اوږدتیا );

  إذا( !بفر ) أرجع -1;

  blake2sp_init( S, outbytes );

  طالما( 1 )
  {
    مجموعه = 0;

    طالما( 1 )
    {
      n = fread( بفر + مجموعه, 1, بفر_اوږدتیا - مجموعه, بهیر );
      مجموعه += n;

      إذا( بفر_اوږدتیا == مجموعه )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( بهیر ) )
          اذهب cleanup_بفر;

        اذهب final_process;
      }

      إذا( feof( بهیر ) )
        اذهب final_process;
    }

    blake2sp_update( S, بفر, بفر_اوږدتیا );
  }

final_process:;

  إذا( مجموعه > 0 ) blake2sp_update( S, بفر, مجموعه );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  حرر( بفر );
  أرجع ret;
}


صحيح blake2bp_بهیر( FILE *بهیر, فراغ *resstream, size_t outbytes )
{
  صحيح ret = -1;
  size_t مجموعه, n;
  blake2bp_state S[1];
  ساكن ثابت size_t بفر_اوږدتیا = 16 * ( 1UL << 20 );
  uint8_t *بفر = ( uint8_t * )حافظه_ورکړه( بفر_اوږدتیا );

  إذا( !بفر ) أرجع -1;

  blake2bp_init( S, outbytes );

  طالما( 1 )
  {
    مجموعه = 0;

    طالما( 1 )
    {
      n = fread( بفر + مجموعه, 1, بفر_اوږدتیا - مجموعه, بهیر );
      مجموعه += n;

      إذا( بفر_اوږدتیا == مجموعه )
        اكسر;

      إذا( 0 == n )
      {
        إذا( ferror( بهیر ) )
          اذهب cleanup_بفر;

        اذهب final_process;
      }

      إذا( feof( بهیر ) )
        اذهب final_process;
    }

    blake2bp_update( S, بفر, بفر_اوږدتیا );
  }

final_process:;

  إذا( مجموعه > 0 ) blake2bp_update( S, بفر, مجموعه );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_بفر:
  حرر( بفر );
  أرجع ret;
}

تعريفنوع صحيح ( *blake2fn )( FILE *, فراغ *, size_t );


ساكن فراغ usage( حرف **argv, صحيح errcode )
{
  FILE *بهر = errcode ? stderr : stdout;
  fprintf( بهر, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( بهر, "\n" );
  fprintf( بهر, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( بهر, "\n" );
  fprintf( بهر, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( بهر, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( بهر, "  --tag        create a BSD-style checksum\n" );
  fprintf( بهر, "  --help       display this help and exit\n" );
  وتل( errcode );
}


صحيح اصلي( صحيح argc, حرف **argv )
{
  blake2fn blake2_بهیر = blake2b_بهیر;
  بینښه اوږد maxbytes = BLAKE2B_OUTBYTES;
  ثابت حرف *algorithm = "BLAKE2b";
  بینښه اوږد outbytes = 0;
  بینښه حرف hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  صحيح c, i;
  opterr = 1;

  طالما( 1 )
  {
    صحيح option_شاخص = 0;
    حرف *پای_ټکی = عدم;
    بینښه اوږد outbits;
    ساكن بنية option long_options[] = {
      { "help",  no_دلیل, 0,  0  },
      { "tag",   no_دلیل, 0,  0  },
      { عدم, 0, عدم, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_شاخص );
    إذا( c == -1 ) اكسر;
    بدل( c )
    {
    حالة 'a':
      إذا( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_بهیر = blake2b_بهیر;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_بهیر = blake2s_بهیر;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_بهیر = blake2bp_بهیر;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      وإلا إذا ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_بهیر = blake2sp_بهیر;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      وإلا
      {
        چاپ_کړه( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      اكسر;

    حالة 'l':
      outbits = strtoul(optarg, &پای_ټکی, 10);
      إذا( !پای_ټکی || *پای_ټکی != '\0' || outbits % 8 != 0)
      {
        چاپ_کړه( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      اكسر;

    حالة 0:
      إذا( 0 == strcmp( "help", long_options[option_شاخص].name ) )
        usage( argv, 0 );
      وإلا إذا( 0 == strcmp( "tag", long_options[option_شاخص].name ) )
        bsdstyle = true;
      اكسر;

    حالة '?':
      usage( argv, 1 );
      اكسر;
    }
  }

  إذا(outbytes > maxbytes)
  {
    چاپ_کړه( "Invalid length argument: %lu\n", outbytes * 8 );
    چاپ_کړه( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  وإلا إذا( outbytes == 0 )
    outbytes = maxbytes;

  إذا( optind == argc )
    argv[argc++] = (حرف *) "-";

  لكل( i = optind; i < argc; ++i )
  {
    FILE *f = عدم;
    إذا( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    وإلا
      f = fopen( argv[i], "rb" );

    إذا( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      استمر;
    }

    إذا( blake2_بهیر( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    وإلا
    {
      size_t j;
      إذا( bsdstyle )
      {
        إذا( outbytes < maxbytes )
          چاپ_کړه( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        وإلا
          چاپ_کړه( "%s (%s) = ", algorithm, argv[i] );
      }

      لكل( j = 0; j < outbytes; ++j )
        چاپ_کړه( "%02x", hash[j] );

      إذا( bsdstyle )
        چاپ_کړه( "\n" );
      وإلا
        چاپ_کړه( "  %s\n", argv[i] );
    }

    إذا( f == stdin )
      clearerr( f );
    وإلا إذا( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  أرجع 0;
}
#د_که_پای
