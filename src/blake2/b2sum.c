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

#ถ้า 0
/* This will help compatibility with coreutils */
จำนวนเต็ม blake2s_stream( FILE *stream, void *resstream, size_t outbytes )
{
  จำนวนเต็ม ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ถ้า( !buffer ) คืน -1;

  blake2s_init( S, outbytes );

  ขณะ( 1 )
  {
    sum = 0;

    ขณะ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ถ้า( buffer_length == sum )
        bha_nga;

      ถ้า( 0 == n )
      {
        ถ้า( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ถ้า( feof( stream ) )
        goto final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  ถ้า( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  คืน ret;
}
#endif

จำนวนเต็ม blake2b_stream( FILE *stream, void *resstream, size_t outbytes )
{
  จำนวนเต็ม ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  static const size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ถ้า( !buffer ) คืน -1;

  blake2b_init( S, outbytes );

  ขณะ( 1 )
  {
    sum = 0;

    ขณะ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ถ้า( buffer_length == sum )
        bha_nga;

      ถ้า( 0 == n )
      {
        ถ้า( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ถ้า( feof( stream ) )
        goto final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  ถ้า( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  คืน ret;
}

#ถ้า 0

#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

จำนวนเต็ม blake2sp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  จำนวนเต็ม ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ถ้า( !buffer ) คืน -1;

  blake2sp_init( S, outbytes );

  ขณะ( 1 )
  {
    sum = 0;

    ขณะ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ถ้า( buffer_length == sum )
        bha_nga;

      ถ้า( 0 == n )
      {
        ถ้า( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ถ้า( feof( stream ) )
        goto final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  ถ้า( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  คืน ret;
}


จำนวนเต็ม blake2bp_stream( FILE *stream, void *resstream, size_t outbytes )
{
  จำนวนเต็ม ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  static const size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ถ้า( !buffer ) คืน -1;

  blake2bp_init( S, outbytes );

  ขณะ( 1 )
  {
    sum = 0;

    ขณะ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ถ้า( buffer_length == sum )
        bha_nga;

      ถ้า( 0 == n )
      {
        ถ้า( ferror( stream ) )
          goto cleanup_buffer;

        goto final_process;
      }

      ถ้า( feof( stream ) )
        goto final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  ถ้า( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  free( buffer );
  คืน ret;
}

typedef จำนวนเต็ม ( *blake2fn )( FILE *, void *, size_t );


static void usage( var_na **argv, จำนวนเต็ม errcode )
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


จำนวนเต็ม หลัก( จำนวนเต็ม argc, var_na **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  const var_na *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned var_na hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  จำนวนเต็ม c, i;
  opterr = 1;

  ขณะ( 1 )
  {
    จำนวนเต็ม option_index = 0;
    var_na *end = NULL;
    unsigned long outbits;
    static sa_nracanaa option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { NULL, 0, NULL, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    ถ้า( c == -1 ) bha_nga;
    switch( c )
    {
    pak_sa 'a':
      ถ้า( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      มิฉะนั้น ถ้า ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      มิฉะนั้น ถ้า ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      มิฉะนั้น ถ้า ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      มิฉะนั้น
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      bha_nga;

    pak_sa 'l':
      outbits = strtoul(optarg, &end, 10);
      ถ้า( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      bha_nga;

    pak_sa 0:
      ถ้า( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      มิฉะนั้น ถ้า( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      bha_nga;

    pak_sa '?':
      usage( argv, 1 );
      bha_nga;
    }
  }

  ถ้า(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  มิฉะนั้น ถ้า( outbytes == 0 )
    outbytes = maxbytes;

  ถ้า( optind == argc )
    argv[argc++] = (var_na *) "-";

  สำหรับ( i = optind; i < argc; ++i )
  {
    FILE *f = NULL;
    ถ้า( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    มิฉะนั้น
      f = fopen( argv[i], "rb" );

    ถ้า( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      continue;
    }

    ถ้า( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    มิฉะนั้น
    {
      size_t j;
      ถ้า( bsdstyle )
      {
        ถ้า( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        มิฉะนั้น
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      สำหรับ( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      ถ้า( bsdstyle )
        printf( "\n" );
      มิฉะนั้น
        printf( "  %s\n", argv[i] );
    }

    ถ้า( f == stdin )
      clearerr( f );
    มิฉะนั้น ถ้า( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  คืน 0;
}
#endif
