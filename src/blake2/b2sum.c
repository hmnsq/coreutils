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

#include <stdio.h>
#include <stdlib.h>

#include "blake2.h"

#만약 0
/* This will help compatibility with coreutils */
정수 blake2s_stream( FILE *stream, void *resstream, size_t outbytes )
{
  정수 ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  만약( !buffer ) 반환 -1;

  blake2s_init( S, outbytes );

  동안( 1 )
  {
    sum = 0;

    동안( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      만약( buffer_length == sum )
        break;

      만약( 0 == n )
      {
        만약( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      만약( feof( stream ) )
        goto final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  만약( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  반환 ret;
}
#endif

정수 blake2b_stream( FILE *stream, void *resstream, size_t outbytes )
{
  정수 ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  만약( !buffer ) 반환 -1;

  blake2b_init( S, outbytes );

  동안( 1 )
  {
    sum = 0;

    동안( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      만약( buffer_length == sum )
        break;

      만약( 0 == n )
      {
        만약( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      만약( feof( stream ) )
        goto final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  만약( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  반환 ret;
}

#만약 0

#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

정수 blake2sp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  정수 ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  만약( !buffer ) 반환 -1;

  blake2sp_init( S, outbytes );

  동안( 1 )
  {
    sum = 0;

    동안( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      만약( buffer_length == sum )
        break;

      만약( 0 == n )
      {
        만약( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      만약( feof( stream ) )
        goto final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  만약( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  반환 ret;
}


정수 blake2bp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  정수 ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  만약( !buffer ) 반환 -1;

  blake2bp_init( S, outbytes );

  동안( 1 )
  {
    sum = 0;

    동안( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      만약( buffer_length == sum )
        break;

      만약( 0 == n )
      {
        만약( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      만약( feof( stream ) )
        goto final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  만약( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  반환 ret;
}

typedef 정수 ( *blake2fn )( FILE *, void *, size_t );


static void usage( char **argv, 정수 errcode )
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


정수 main( 정수 argc, char **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  const char *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned char hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  정수 c, i;
  opterr = 1;

  동안( 1 )
  {
    정수 option_index = 0;
    char *end = NULL;
    unsigned long outbits;
    static struct option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { NULL, 0, NULL, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    만약( c == -1 ) break;
    switch( c )
    {
    case 'a':
      만약( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      아니면 만약 ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      아니면 만약 ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      아니면 만약 ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      아니면
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      break;

    case 'l':
      outbits = strtoul(optarg, &end, 10);
      만약( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      break;

    case 0:
      만약( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      아니면 만약( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      break;

    case '?':
      usage( argv, 1 );
      break;
    }
  }

  만약(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  아니면 만약( outbytes == 0 )
    outbytes = maxbytes;

  만약( optind == argc )
    argv[argc++] = (char *) "-";

  위해( i = optind; i < argc; ++i )
  {
    FILE *f = NULL;
    만약( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    아니면
      f = fopen( argv[i], "rb" );

    만약( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      continue;
    }

    만약( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    아니면
    {
      size_t j;
      만약( bsdstyle )
      {
        만약( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        아니면
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      위해( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      만약( bsdstyle )
        printf( "\n" );
      아니면
        printf( "  %s\n", argv[i] );
    }

    만약( f == stdin )
      clearerr( f );
    아니면 만약( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  반환 0;
}
#endif
