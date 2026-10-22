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

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ଅନ୍ତର୍ଭାଵ "blake2.h"

#ଯଦି 0
/* This will help compatibility with coreutils */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା blake2s_stream( FILE *stream, ଶୂନ୍ୟ *resstream, size_t outbytes )
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  ସ୍ଥିର ଅଚଳ size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ଯଦି( !buffer ) ଫେରନ୍ତୁ -1;

  blake2s_init( S, outbytes );

  ଯେତେବେଳେ( 1 )
  {
    sum = 0;

    ଯେତେବେଳେ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ଯଦି( buffer_length == sum )
        ଭାଙ୍ଗ;

      ଯଦି( 0 == n )
      {
        ଯଦି( ferror( stream ) )
          ଯାଅ cleanup_buffer;

        ଯାଅ final_process;
      }

      ଯଦି( feof( stream ) )
        ଯାଅ final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  ଯଦି( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  ମୁକ୍ତ( buffer );
  ଫେରନ୍ତୁ ret;
}
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା blake2b_stream( FILE *stream, ଶୂନ୍ୟ *resstream, size_t outbytes )
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  ସ୍ଥିର ଅଚଳ size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ଯଦି( !buffer ) ଫେରନ୍ତୁ -1;

  blake2b_init( S, outbytes );

  ଯେତେବେଳେ( 1 )
  {
    sum = 0;

    ଯେତେବେଳେ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ଯଦି( buffer_length == sum )
        ଭାଙ୍ଗ;

      ଯଦି( 0 == n )
      {
        ଯଦି( ferror( stream ) )
          ଯାଅ cleanup_buffer;

        ଯାଅ final_process;
      }

      ଯଦି( feof( stream ) )
        ଯାଅ final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  ଯଦି( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  ମୁକ୍ତ( buffer );
  ଫେରନ୍ତୁ ret;
}

#ଯଦି 0

#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>

ପୂର୍ଣ୍ଣସଂଖ୍ୟା blake2sp_stream( FILE *stream, ଶୂନ୍ୟ *resstream, size_t outbytes )
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  ସ୍ଥିର ଅଚଳ size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ଯଦି( !buffer ) ଫେରନ୍ତୁ -1;

  blake2sp_init( S, outbytes );

  ଯେତେବେଳେ( 1 )
  {
    sum = 0;

    ଯେତେବେଳେ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ଯଦି( buffer_length == sum )
        ଭାଙ୍ଗ;

      ଯଦି( 0 == n )
      {
        ଯଦି( ferror( stream ) )
          ଯାଅ cleanup_buffer;

        ଯାଅ final_process;
      }

      ଯଦି( feof( stream ) )
        ଯାଅ final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  ଯଦି( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  ମୁକ୍ତ( buffer );
  ଫେରନ୍ତୁ ret;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା blake2bp_stream( FILE *stream, ଶୂନ୍ୟ *resstream, size_t outbytes )
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  ସ୍ଥିର ଅଚଳ size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  ଯଦି( !buffer ) ଫେରନ୍ତୁ -1;

  blake2bp_init( S, outbytes );

  ଯେତେବେଳେ( 1 )
  {
    sum = 0;

    ଯେତେବେଳେ( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      ଯଦି( buffer_length == sum )
        ଭାଙ୍ଗ;

      ଯଦି( 0 == n )
      {
        ଯଦି( ferror( stream ) )
          ଯାଅ cleanup_buffer;

        ଯାଅ final_process;
      }

      ଯଦି( feof( stream ) )
        ଯାଅ final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  ଯଦି( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  ମୁକ୍ତ( buffer );
  ଫେରନ୍ତୁ ret;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା ( *blake2fn )( FILE *, ଶୂନ୍ୟ *, size_t );


ସ୍ଥିର ଶୂନ୍ୟ usage( ଅକ୍ଷର **argv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା errcode )
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


ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  ଅଚଳ ଅକ୍ଷର *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned ଅକ୍ଷର hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, i;
  opterr = 1;

  ଯେତେବେଳେ( 1 )
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା option_index = 0;
    ଅକ୍ଷର *end = ଖାଲି;
    unsigned long outbits;
    ସ୍ଥିର ସଂରଚନା option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { ଖାଲି, 0, ଖାଲି, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    ଯଦି( c == -1 ) ଭାଙ୍ଗ;
    ବାଛ( c )
    {
    ସ୍ଥିତି 'a':
      ଯଦି( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      ଅନ୍ୟଥା ଯଦି ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      ଅନ୍ୟଥା ଯଦି ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      ଅନ୍ୟଥା ଯଦି ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      ଅନ୍ୟଥା
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'l':
      outbits = strtoul(optarg, &end, 10);
      ଯଦି( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 0:
      ଯଦି( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      ଅନ୍ୟଥା ଯଦି( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି '?':
      usage( argv, 1 );
      ଭାଙ୍ଗ;
    }
  }

  ଯଦି(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  ଅନ୍ୟଥା ଯଦି( outbytes == 0 )
    outbytes = maxbytes;

  ଯଦି( optind == argc )
    argv[argc++] = (ଅକ୍ଷର *) "-";

  ପାଇଁ( i = optind; i < argc; ++i )
  {
    FILE *f = ଖାଲି;
    ଯଦି( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    ଅନ୍ୟଥା
      f = fopen( argv[i], "rb" );

    ଯଦି( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      ଜାରି;
    }

    ଯଦି( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    ଅନ୍ୟଥା
    {
      size_t j;
      ଯଦି( bsdstyle )
      {
        ଯଦି( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        ଅନ୍ୟଥା
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      ପାଇଁ( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      ଯଦି( bsdstyle )
        printf( "\n" );
      ଅନ୍ୟଥା
        printf( "  %s\n", argv[i] );
    }

    ଯଦି( f == stdin )
      clearerr( f );
    ଅନ୍ୟଥା ଯଦି( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  ଫେରନ୍ତୁ 0;
}
#endif
