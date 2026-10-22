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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CONFIG_H
# include <config.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "blake2.h"

#ಆದರೆ 0
/* This will help compatibility with coreutils */
ಪೂರ್ಣಾಂಕ blake2s_ಹರಿವು( FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, size_t outbytes )
{
  ಪೂರ್ಣಾಂಕ ret = -1;
  size_t ಮೊತ್ತ, n;
  blake2s_state S[1];
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t ಬಫರ್_ಉದ್ದಳತೆ = 32768;
  uint8_t *ಬಫರ್ = ( uint8_t * )ಸ್ಮರಣೆ_ಕೊಡು( ಬಫರ್_ಉದ್ದಳತೆ );

  ಆದರೆ( !ಬಫರ್ ) ಹಿಂತಿರುಗಿ -1;

  blake2s_init( S, outbytes );

  ಆಗ( 1 )
  {
    ಮೊತ್ತ = 0;

    ಆಗ( 1 )
    {
      n = fread( ಬಫರ್ + ಮೊತ್ತ, 1, ಬಫರ್_ಉದ್ದಳತೆ - ಮೊತ್ತ, ಹರಿವು );
      ಮೊತ್ತ += n;

      ಆದರೆ( ಬಫರ್_ಉದ್ದಳತೆ == ಮೊತ್ತ )
        ನಿಲ್ಲಿಸು;

      ಆದರೆ( 0 == n )
      {
        ಆದರೆ( ferror( ಹರಿವು ) )
          ಹೋಗು cleanup_ಬಫರ್;

        ಹೋಗು final_process;
      }

      ಆದರೆ( feof( ಹರಿವು ) )
        ಹೋಗು final_process;
    }

    blake2s_update( S, ಬಫರ್, ಬಫರ್_ಉದ್ದಳತೆ );
  }

final_process:;

  ಆದರೆ( ಮೊತ್ತ > 0 ) blake2s_update( S, ಬಫರ್, ಮೊತ್ತ );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_ಬಫರ್:
  ಬಿಡುಗಡೆ( ಬಫರ್ );
  ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ

ಪೂರ್ಣಾಂಕ blake2b_ಹರಿವು( FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, size_t outbytes )
{
  ಪೂರ್ಣಾಂಕ ret = -1;
  size_t ಮೊತ್ತ, n;
  blake2b_state S[1];
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t ಬಫರ್_ಉದ್ದಳತೆ = 32768;
  uint8_t *ಬಫರ್ = ( uint8_t * )ಸ್ಮರಣೆ_ಕೊಡು( ಬಫರ್_ಉದ್ದಳತೆ );

  ಆದರೆ( !ಬಫರ್ ) ಹಿಂತಿರುಗಿ -1;

  blake2b_init( S, outbytes );

  ಆಗ( 1 )
  {
    ಮೊತ್ತ = 0;

    ಆಗ( 1 )
    {
      n = fread( ಬಫರ್ + ಮೊತ್ತ, 1, ಬಫರ್_ಉದ್ದಳತೆ - ಮೊತ್ತ, ಹರಿವು );
      ಮೊತ್ತ += n;

      ಆದರೆ( ಬಫರ್_ಉದ್ದಳತೆ == ಮೊತ್ತ )
        ನಿಲ್ಲಿಸು;

      ಆದರೆ( 0 == n )
      {
        ಆದರೆ( ferror( ಹರಿವು ) )
          ಹೋಗು cleanup_ಬಫರ್;

        ಹೋಗು final_process;
      }

      ಆದರೆ( feof( ಹರಿವು ) )
        ಹೋಗು final_process;
    }

    blake2b_update( S, ಬಫರ್, ಬಫರ್_ಉದ್ದಳತೆ );
  }

final_process:;

  ಆದರೆ( ಮೊತ್ತ > 0 ) blake2b_update( S, ಬಫರ್, ಮೊತ್ತ );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_ಬಫರ್:
  ಬಿಡುಗಡೆ( ಬಫರ್ );
  ಹಿಂತಿರುಗಿ ret;
}

#ಆದರೆ 0

#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>

ಪೂರ್ಣಾಂಕ blake2sp_ಹರಿವು( FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, size_t outbytes )
{
  ಪೂರ್ಣಾಂಕ ret = -1;
  size_t ಮೊತ್ತ, n;
  blake2sp_state S[1];
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t ಬಫರ್_ಉದ್ದಳತೆ = 16 * ( 1UL << 20 );
  uint8_t *ಬಫರ್ = ( uint8_t * )ಸ್ಮರಣೆ_ಕೊಡು( ಬಫರ್_ಉದ್ದಳತೆ );

  ಆದರೆ( !ಬಫರ್ ) ಹಿಂತಿರುಗಿ -1;

  blake2sp_init( S, outbytes );

  ಆಗ( 1 )
  {
    ಮೊತ್ತ = 0;

    ಆಗ( 1 )
    {
      n = fread( ಬಫರ್ + ಮೊತ್ತ, 1, ಬಫರ್_ಉದ್ದಳತೆ - ಮೊತ್ತ, ಹರಿವು );
      ಮೊತ್ತ += n;

      ಆದರೆ( ಬಫರ್_ಉದ್ದಳತೆ == ಮೊತ್ತ )
        ನಿಲ್ಲಿಸು;

      ಆದರೆ( 0 == n )
      {
        ಆದರೆ( ferror( ಹರಿವು ) )
          ಹೋಗು cleanup_ಬಫರ್;

        ಹೋಗು final_process;
      }

      ಆದರೆ( feof( ಹರಿವು ) )
        ಹೋಗು final_process;
    }

    blake2sp_update( S, ಬಫರ್, ಬಫರ್_ಉದ್ದಳತೆ );
  }

final_process:;

  ಆದರೆ( ಮೊತ್ತ > 0 ) blake2sp_update( S, ಬಫರ್, ಮೊತ್ತ );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_ಬಫರ್:
  ಬಿಡುಗಡೆ( ಬಫರ್ );
  ಹಿಂತಿರುಗಿ ret;
}


ಪೂರ್ಣಾಂಕ blake2bp_ಹರಿವು( FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, size_t outbytes )
{
  ಪೂರ್ಣಾಂಕ ret = -1;
  size_t ಮೊತ್ತ, n;
  blake2bp_state S[1];
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t ಬಫರ್_ಉದ್ದಳತೆ = 16 * ( 1UL << 20 );
  uint8_t *ಬಫರ್ = ( uint8_t * )ಸ್ಮರಣೆ_ಕೊಡು( ಬಫರ್_ಉದ್ದಳತೆ );

  ಆದರೆ( !ಬಫರ್ ) ಹಿಂತಿರುಗಿ -1;

  blake2bp_init( S, outbytes );

  ಆಗ( 1 )
  {
    ಮೊತ್ತ = 0;

    ಆಗ( 1 )
    {
      n = fread( ಬಫರ್ + ಮೊತ್ತ, 1, ಬಫರ್_ಉದ್ದಳತೆ - ಮೊತ್ತ, ಹರಿವು );
      ಮೊತ್ತ += n;

      ಆದರೆ( ಬಫರ್_ಉದ್ದಳತೆ == ಮೊತ್ತ )
        ನಿಲ್ಲಿಸು;

      ಆದರೆ( 0 == n )
      {
        ಆದರೆ( ferror( ಹರಿವು ) )
          ಹೋಗು cleanup_ಬಫರ್;

        ಹೋಗು final_process;
      }

      ಆದರೆ( feof( ಹರಿವು ) )
        ಹೋಗು final_process;
    }

    blake2bp_update( S, ಬಫರ್, ಬಫರ್_ಉದ್ದಳತೆ );
  }

final_process:;

  ಆದರೆ( ಮೊತ್ತ > 0 ) blake2bp_update( S, ಬಫರ್, ಮೊತ್ತ );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_ಬಫರ್:
  ಬಿಡುಗಡೆ( ಬಫರ್ );
  ಹಿಂತಿರುಗಿ ret;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ ( *blake2fn )( FILE *, ಶೂನ್ಯ *, size_t );


ಸ್ಥಿರ ಶೂನ್ಯ usage( ಅಕ್ಷರ **argv, ಪೂರ್ಣಾಂಕ errcode )
{
  FILE *ಹೊರಹೋಗು = errcode ? stderr : stdout;
  fprintf( ಹೊರಹೋಗು, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( ಹೊರಹೋಗು, "\n" );
  fprintf( ಹೊರಹೋಗು, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( ಹೊರಹೋಗು, "\n" );
  fprintf( ಹೊರಹೋಗು, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( ಹೊರಹೋಗು, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( ಹೊರಹೋಗು, "  --tag        create a BSD-style checksum\n" );
  fprintf( ಹೊರಹೋಗು, "  --help       display this help and exit\n" );
  ನಿರ್ಗಮಿಸು( errcode );
}


ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ( ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv )
{
  blake2fn blake2_ಹರಿವು = blake2b_ಹರಿವು;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ maxbytes = BLAKE2B_OUTBYTES;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *algorithm = "BLAKE2b";
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ outbytes = 0;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  ಪೂರ್ಣಾಂಕ c, i;
  opterr = 1;

  ಆಗ( 1 )
  {
    ಪೂರ್ಣಾಂಕ option_ಸೂಚ್ಯಂಕ = 0;
    ಅಕ್ಷರ *ಕೊನೆ = ಖಾಲಿ;
    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ outbits;
    ಸ್ಥಿರ ರಚನೆ option long_options[] = {
      { "help",  no_ವಾದ, 0,  0  },
      { "tag",   no_ವಾದ, 0,  0  },
      { ಖಾಲಿ, 0, ಖಾಲಿ, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_ಸೂಚ್ಯಂಕ );
    ಆದರೆ( c == -1 ) ನಿಲ್ಲಿಸು;
    ಆಯ್ಕೆ( c )
    {
    ಸ್ಥಿತಿ 'a':
      ಆದರೆ( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_ಹರಿವು = blake2b_ಹರಿವು;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_ಹರಿವು = blake2s_ಹರಿವು;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_ಹರಿವು = blake2bp_ಹರಿವು;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_ಹರಿವು = blake2sp_ಹರಿವು;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      ಇಲ್ಲದಿದ್ದರೆ
      {
        ಬರೆ( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'l':
      outbits = strtoul(optarg, &ಕೊನೆ, 10);
      ಆದರೆ( !ಕೊನೆ || *ಕೊನೆ != '\0' || outbits % 8 != 0)
      {
        ಬರೆ( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 0:
      ಆದರೆ( 0 == strcmp( "help", long_options[option_ಸೂಚ್ಯಂಕ].name ) )
        usage( argv, 0 );
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( 0 == strcmp( "tag", long_options[option_ಸೂಚ್ಯಂಕ].name ) )
        bsdstyle = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ '?':
      usage( argv, 1 );
      ನಿಲ್ಲಿಸು;
    }
  }

  ಆದರೆ(outbytes > maxbytes)
  {
    ಬರೆ( "Invalid length argument: %lu\n", outbytes * 8 );
    ಬರೆ( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( outbytes == 0 )
    outbytes = maxbytes;

  ಆದರೆ( optind == argc )
    argv[argc++] = (ಅಕ್ಷರ *) "-";

  ಗಾಗಿ( i = optind; i < argc; ++i )
  {
    FILE *f = ಖಾಲಿ;
    ಆದರೆ( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    ಇಲ್ಲದಿದ್ದರೆ
      f = fopen( argv[i], "rb" );

    ಆದರೆ( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      ಮುಂದುವರಿ;
    }

    ಆದರೆ( blake2_ಹರಿವು( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
      size_t j;
      ಆದರೆ( bsdstyle )
      {
        ಆದರೆ( outbytes < maxbytes )
          ಬರೆ( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        ಇಲ್ಲದಿದ್ದರೆ
          ಬರೆ( "%s (%s) = ", algorithm, argv[i] );
      }

      ಗಾಗಿ( j = 0; j < outbytes; ++j )
        ಬರೆ( "%02x", hash[j] );

      ಆದರೆ( bsdstyle )
        ಬರೆ( "\n" );
      ಇಲ್ಲದಿದ್ದರೆ
        ಬರೆ( "  %s\n", argv[i] );
    }

    ಆದರೆ( f == stdin )
      clearerr( f );
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ
