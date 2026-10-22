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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "blake2.h"

#ಯದಿ 0
/* This will help compatibility with coreutils */
ಪೂರ್ಣಾಙ್ಕ blake2s_stream( FILE *stream, ಶೂನ್ಯ *resstream, size_t outbytes )
{
  ಪೂರ್ಣಾಙ್ಕ ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  ಸ್ಥಿರ ಧ್ರುವ size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ಯದಿ( !buffer ) ಪ್ರತ್ಯಾಗಮ -1;

  blake2s_init( S, outbytes );

  ಯಾವತ್( 1 )
  {
    sum = 0;

    ಯಾವತ್( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ಯದಿ( buffer_length == sum )
        ಭಙ್ಗ;

      ಯದಿ( 0 == n )
      {
        ಯದಿ( ferror( stream ) )
          ಗಚ್ಛ cleanup_buffer;

        ಗಚ್ಛ final_process;
      }

      ಯದಿ( feof( stream ) )
        ಗಚ್ಛ final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  ಯದಿ( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  ಮುಕ್ತ( buffer );
  ಪ್ರತ್ಯಾಗಮ ret;
}
#endif

ಪೂರ್ಣಾಙ್ಕ blake2b_stream( FILE *stream, ಶೂನ್ಯ *resstream, size_t outbytes )
{
  ಪೂರ್ಣಾಙ್ಕ ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  ಸ್ಥಿರ ಧ್ರುವ size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ಯದಿ( !buffer ) ಪ್ರತ್ಯಾಗಮ -1;

  blake2b_init( S, outbytes );

  ಯಾವತ್( 1 )
  {
    sum = 0;

    ಯಾವತ್( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ಯದಿ( buffer_length == sum )
        ಭಙ್ಗ;

      ಯದಿ( 0 == n )
      {
        ಯದಿ( ferror( stream ) )
          ಗಚ್ಛ cleanup_buffer;

        ಗಚ್ಛ final_process;
      }

      ಯದಿ( feof( stream ) )
        ಗಚ್ಛ final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  ಯದಿ( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  ಮುಕ್ತ( buffer );
  ಪ್ರತ್ಯಾಗಮ ret;
}

#ಯದಿ 0

#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>

ಪೂರ್ಣಾಙ್ಕ blake2sp_stream( FILE *stream, ಶೂನ್ಯ *resstream, size_t outbytes )
{
  ಪೂರ್ಣಾಙ್ಕ ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  ಸ್ಥಿರ ಧ್ರುವ size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ಯದಿ( !buffer ) ಪ್ರತ್ಯಾಗಮ -1;

  blake2sp_init( S, outbytes );

  ಯಾವತ್( 1 )
  {
    sum = 0;

    ಯಾವತ್( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ಯದಿ( buffer_length == sum )
        ಭಙ್ಗ;

      ಯದಿ( 0 == n )
      {
        ಯದಿ( ferror( stream ) )
          ಗಚ್ಛ cleanup_buffer;

        ಗಚ್ಛ final_process;
      }

      ಯದಿ( feof( stream ) )
        ಗಚ್ಛ final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  ಯದಿ( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  ಮುಕ್ತ( buffer );
  ಪ್ರತ್ಯಾಗಮ ret;
}


ಪೂರ್ಣಾಙ್ಕ blake2bp_stream( FILE *stream, ಶೂನ್ಯ *resstream, size_t outbytes )
{
  ಪೂರ್ಣಾಙ್ಕ ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  ಸ್ಥಿರ ಧ್ರುವ size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ಯದಿ( !buffer ) ಪ್ರತ್ಯಾಗಮ -1;

  blake2bp_init( S, outbytes );

  ಯಾವತ್( 1 )
  {
    sum = 0;

    ಯಾವತ್( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ಯದಿ( buffer_length == sum )
        ಭಙ್ಗ;

      ಯದಿ( 0 == n )
      {
        ಯದಿ( ferror( stream ) )
          ಗಚ್ಛ cleanup_buffer;

        ಗಚ್ಛ final_process;
      }

      ಯದಿ( feof( stream ) )
        ಗಚ್ಛ final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  ಯದಿ( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  ಮುಕ್ತ( buffer );
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ ( *blake2fn )( FILE *, ಶೂನ್ಯ *, size_t );


ಸ್ಥಿರ ಶೂನ್ಯ usage( ವರ್ಣ **argv, ಪೂರ್ಣಾಙ್ಕ errcode )
{
  FILE *out = errcode ? stderr : stdout;
  fprintf( out, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( out, "\n" );
  fprintf( out, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( out, "\n" );
  fprintf( out, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( out, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( out, "  --tag        create a BSD-style checksum\n" );
  fprintf( out, "  --help       display this help and exit\n" );
  exit( errcode );
}


ಪೂರ್ಣಾಙ್ಕ main( ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  ಧ್ರುವ ವರ್ಣ *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned ವರ್ಣ hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  ಪೂರ್ಣಾಙ್ಕ c, i;
  opterr = 1;

  ಯಾವತ್( 1 )
  {
    ಪೂರ್ಣಾಙ್ಕ option_index = 0;
    ವರ್ಣ *end = ರಿಕ್ತ;
    unsigned long outbits;
    ಸ್ಥಿರ ಸಂರಚನಾ option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { ರಿಕ್ತ, 0, ರಿಕ್ತ, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    ಯದಿ( c == -1 ) ಭಙ್ಗ;
    ವಿಕಲ್ಪ( c )
    {
    ಪಕ್ಷ 'a':
      ಯದಿ( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      ಅನ್ಯಥಾ ಯದಿ ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      ಅನ್ಯಥಾ ಯದಿ ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      ಅನ್ಯಥಾ ಯದಿ ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      ಅನ್ಯಥಾ
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      ಭಙ್ಗ;

    ಪಕ್ಷ 'l':
      outbits = strtoul(optarg, &end, 10);
      ಯದಿ( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      ಭಙ್ಗ;

    ಪಕ್ಷ 0:
      ಯದಿ( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      ಅನ್ಯಥಾ ಯದಿ( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ '?':
      usage( argv, 1 );
      ಭಙ್ಗ;
    }
  }

  ಯದಿ(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  ಅನ್ಯಥಾ ಯದಿ( outbytes == 0 )
    outbytes = maxbytes;

  ಯದಿ( optind == argc )
    argv[argc++] = (ವರ್ಣ *) "-";

  ಪ್ರತಿ( i = optind; i < argc; ++i )
  {
    FILE *f = ರಿಕ್ತ;
    ಯದಿ( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    ಅನ್ಯಥಾ
      f = fopen( argv[i], "rb" );

    ಯದಿ( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      ಅನುವರ್ತ;
    }

    ಯದಿ( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    ಅನ್ಯಥಾ
    {
      size_t j;
      ಯದಿ( bsdstyle )
      {
        ಯದಿ( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        ಅನ್ಯಥಾ
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      ಪ್ರತಿ( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      ಯದಿ( bsdstyle )
        printf( "\n" );
      ಅನ್ಯಥಾ
        printf( "  %s\n", argv[i] );
    }

    ಯದಿ( f == stdin )
      clearerr( f );
    ಅನ್ಯಥಾ ಯದಿ( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  ಪ್ರತ್ಯಾಗಮ 0;
}
#endif
