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

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>

#অন্তর্ভাৱ "blake2.h"

#যদি 0
/* This will help compatibility with coreutils */
পূর্ণাঙ্ক blake2s_stream( FILE *stream, শূন্য *resstream, size_t outbytes )
{
  পূর্ণাঙ্ক ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  স্থির ধ্রুৱ size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  যদি( !buffer ) ঘূৰাই -1;

  blake2s_init( S, outbytes );

  যেতিয়ালৈ( 1 )
  {
    sum = 0;

    যেতিয়ালৈ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      যদি( buffer_length == sum )
        ভঙ্গ;

      যদি( 0 == n )
      {
        যদি( ferror( stream ) )
          গচ্ছ cleanup_buffer;

        গচ্ছ final_process;
      }

      যদি( feof( stream ) )
        গচ্ছ final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  যদি( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  মুক্ত( buffer );
  ঘূৰাই ret;
}
#endif

পূর্ণাঙ্ক blake2b_stream( FILE *stream, শূন্য *resstream, size_t outbytes )
{
  পূর্ণাঙ্ক ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  স্থির ধ্রুৱ size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  যদি( !buffer ) ঘূৰাই -1;

  blake2b_init( S, outbytes );

  যেতিয়ালৈ( 1 )
  {
    sum = 0;

    যেতিয়ালৈ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      যদি( buffer_length == sum )
        ভঙ্গ;

      যদি( 0 == n )
      {
        যদি( ferror( stream ) )
          গচ্ছ cleanup_buffer;

        গচ্ছ final_process;
      }

      যদি( feof( stream ) )
        গচ্ছ final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  যদি( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  মুক্ত( buffer );
  ঘূৰাই ret;
}

#যদি 0

#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <unistd.h>

পূর্ণাঙ্ক blake2sp_stream( FILE *stream, শূন্য *resstream, size_t outbytes )
{
  পূর্ণাঙ্ক ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  স্থির ধ্রুৱ size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  যদি( !buffer ) ঘূৰাই -1;

  blake2sp_init( S, outbytes );

  যেতিয়ালৈ( 1 )
  {
    sum = 0;

    যেতিয়ালৈ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      যদি( buffer_length == sum )
        ভঙ্গ;

      যদি( 0 == n )
      {
        যদি( ferror( stream ) )
          গচ্ছ cleanup_buffer;

        গচ্ছ final_process;
      }

      যদি( feof( stream ) )
        গচ্ছ final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  যদি( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  মুক্ত( buffer );
  ঘূৰাই ret;
}


পূর্ণাঙ্ক blake2bp_stream( FILE *stream, শূন্য *resstream, size_t outbytes )
{
  পূর্ণাঙ্ক ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  স্থির ধ্রুৱ size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  যদি( !buffer ) ঘূৰাই -1;

  blake2bp_init( S, outbytes );

  যেতিয়ালৈ( 1 )
  {
    sum = 0;

    যেতিয়ালৈ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      যদি( buffer_length == sum )
        ভঙ্গ;

      যদি( 0 == n )
      {
        যদি( ferror( stream ) )
          গচ্ছ cleanup_buffer;

        গচ্ছ final_process;
      }

      যদি( feof( stream ) )
        গচ্ছ final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  যদি( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  মুক্ত( buffer );
  ঘূৰাই ret;
}

প্রকারনির্দেশ পূর্ণাঙ্ক ( *blake2fn )( FILE *, শূন্য *, size_t );


স্থির শূন্য usage( ৱর্ণ **argv, পূর্ণাঙ্ক errcode )
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


পূর্ণাঙ্ক main( পূর্ণাঙ্ক argc, ৱর্ণ **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  ধ্রুৱ ৱর্ণ *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned ৱর্ণ hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  পূর্ণাঙ্ক c, i;
  opterr = 1;

  যেতিয়ালৈ( 1 )
  {
    পূর্ণাঙ্ক option_index = 0;
    ৱর্ণ *end = রিক্ত;
    unsigned long outbits;
    স্থির সংরচনা option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { রিক্ত, 0, রিক্ত, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    যদি( c == -1 ) ভঙ্গ;
    ৱিকল্প( c )
    {
    পক্ষ 'a':
      যদি( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      অন্যথা যদি ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      অন্যথা যদি ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      অন্যথা যদি ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      অন্যথা
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      ভঙ্গ;

    পক্ষ 'l':
      outbits = strtoul(optarg, &end, 10);
      যদি( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      ভঙ্গ;

    পক্ষ 0:
      যদি( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      অন্যথা যদি( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      ভঙ্গ;

    পক্ষ '?':
      usage( argv, 1 );
      ভঙ্গ;
    }
  }

  যদি(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  অন্যথা যদি( outbytes == 0 )
    outbytes = maxbytes;

  যদি( optind == argc )
    argv[argc++] = (ৱর্ণ *) "-";

  বাবে( i = optind; i < argc; ++i )
  {
    FILE *f = রিক্ত;
    যদি( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    অন্যথা
      f = fopen( argv[i], "rb" );

    যদি( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      অনুৱর্ত;
    }

    যদি( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    অন্যথা
    {
      size_t j;
      যদি( bsdstyle )
      {
        যদি( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        অন্যথা
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      বাবে( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      যদি( bsdstyle )
        printf( "\n" );
      অন্যথা
        printf( "  %s\n", argv[i] );
    }

    যদি( f == stdin )
      clearerr( f );
    অন্যথা যদি( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  ঘূৰাই 0;
}
#endif
