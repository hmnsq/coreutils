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

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "blake2.h"

#यदि 0
/* This will help compatibility with coreutils */
पूर्णाङ्क blake2s_stream( FILE *stream, शून्य *resstream, size_t outbytes )
{
  पूर्णाङ्क ret = -1;
  size_t sum, n;
  blake2s_state S[1];
  स्थिर ध्रुव size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  यदि( !buffer ) प्रत्यागम -1;

  blake2s_init( S, outbytes );

  यावत्( 1 )
  {
    sum = 0;

    यावत्( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      यदि( buffer_length == sum )
        भङ्ग;

      यदि( 0 == n )
      {
        यदि( ferror( stream ) )
          गच्छ cleanup_buffer;

        गच्छ final_process;
      }

      यदि( feof( stream ) )
        गच्छ final_process;
    }

    blake2s_update( S, buffer, buffer_length );
  }

final_process:;

  यदि( sum > 0 ) blake2s_update( S, buffer, sum );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  मुक्त( buffer );
  प्रत्यागम ret;
}
#endif

पूर्णाङ्क blake2b_stream( FILE *stream, शून्य *resstream, size_t outbytes )
{
  पूर्णाङ्क ret = -1;
  size_t sum, n;
  blake2b_state S[1];
  स्थिर ध्रुव size_t buffer_length = 32768;
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  यदि( !buffer ) प्रत्यागम -1;

  blake2b_init( S, outbytes );

  यावत्( 1 )
  {
    sum = 0;

    यावत्( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      यदि( buffer_length == sum )
        भङ्ग;

      यदि( 0 == n )
      {
        यदि( ferror( stream ) )
          गच्छ cleanup_buffer;

        गच्छ final_process;
      }

      यदि( feof( stream ) )
        गच्छ final_process;
    }

    blake2b_update( S, buffer, buffer_length );
  }

final_process:;

  यदि( sum > 0 ) blake2b_update( S, buffer, sum );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  मुक्त( buffer );
  प्रत्यागम ret;
}

#यदि 0

#अन्तर्भाव <errno.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>

पूर्णाङ्क blake2sp_stream( FILE *stream, शून्य *resstream, size_t outbytes )
{
  पूर्णाङ्क ret = -1;
  size_t sum, n;
  blake2sp_state S[1];
  स्थिर ध्रुव size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  यदि( !buffer ) प्रत्यागम -1;

  blake2sp_init( S, outbytes );

  यावत्( 1 )
  {
    sum = 0;

    यावत्( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      यदि( buffer_length == sum )
        भङ्ग;

      यदि( 0 == n )
      {
        यदि( ferror( stream ) )
          गच्छ cleanup_buffer;

        गच्छ final_process;
      }

      यदि( feof( stream ) )
        गच्छ final_process;
    }

    blake2sp_update( S, buffer, buffer_length );
  }

final_process:;

  यदि( sum > 0 ) blake2sp_update( S, buffer, sum );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  मुक्त( buffer );
  प्रत्यागम ret;
}


पूर्णाङ्क blake2bp_stream( FILE *stream, शून्य *resstream, size_t outbytes )
{
  पूर्णाङ्क ret = -1;
  size_t sum, n;
  blake2bp_state S[1];
  स्थिर ध्रुव size_t buffer_length = 16 * ( 1UL << 20 );
  uint8_t *buffer = ( uint8_t * )malloc( buffer_length );

  यदि( !buffer ) प्रत्यागम -1;

  blake2bp_init( S, outbytes );

  यावत्( 1 )
  {
    sum = 0;

    यावत्( 1 )
    {
      n = fread( buffer + sum, 1, buffer_length - sum, stream );
      sum += n;

      यदि( buffer_length == sum )
        भङ्ग;

      यदि( 0 == n )
      {
        यदि( ferror( stream ) )
          गच्छ cleanup_buffer;

        गच्छ final_process;
      }

      यदि( feof( stream ) )
        गच्छ final_process;
    }

    blake2bp_update( S, buffer, buffer_length );
  }

final_process:;

  यदि( sum > 0 ) blake2bp_update( S, buffer, sum );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_buffer:
  मुक्त( buffer );
  प्रत्यागम ret;
}

प्रकारनिर्देश पूर्णाङ्क ( *blake2fn )( FILE *, शून्य *, size_t );


स्थिर शून्य usage( वर्ण **argv, पूर्णाङ्क errcode )
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


पूर्णाङ्क main( पूर्णाङ्क argc, वर्ण **argv )
{
  blake2fn blake2_stream = blake2b_stream;
  unsigned long maxbytes = BLAKE2B_OUTBYTES;
  ध्रुव वर्ण *algorithm = "BLAKE2b";
  unsigned long outbytes = 0;
  unsigned वर्ण hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  पूर्णाङ्क c, i;
  opterr = 1;

  यावत्( 1 )
  {
    पूर्णाङ्क option_index = 0;
    वर्ण *end = रिक्त;
    unsigned long outbits;
    स्थिर संरचना option long_options[] = {
      { "help",  no_argument, 0,  0  },
      { "tag",   no_argument, 0,  0  },
      { रिक्त, 0, रिक्त, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_index );
    यदि( c == -1 ) भङ्ग;
    विकल्प( c )
    {
    पक्ष 'a':
      यदि( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_stream = blake2b_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      अन्यथा यदि ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_stream = blake2s_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      अन्यथा यदि ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_stream = blake2bp_stream;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      अन्यथा यदि ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_stream = blake2sp_stream;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      अन्यथा
      {
        printf( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      भङ्ग;

    पक्ष 'l':
      outbits = strtoul(optarg, &end, 10);
      यदि( !end || *end != '\0' || outbits % 8 != 0)
      {
        printf( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      भङ्ग;

    पक्ष 0:
      यदि( 0 == strcmp( "help", long_options[option_index].name ) )
        usage( argv, 0 );
      अन्यथा यदि( 0 == strcmp( "tag", long_options[option_index].name ) )
        bsdstyle = true;
      भङ्ग;

    पक्ष '?':
      usage( argv, 1 );
      भङ्ग;
    }
  }

  यदि(outbytes > maxbytes)
  {
    printf( "Invalid length argument: %lu\n", outbytes * 8 );
    printf( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  अन्यथा यदि( outbytes == 0 )
    outbytes = maxbytes;

  यदि( optind == argc )
    argv[argc++] = (वर्ण *) "-";

  प्रति( i = optind; i < argc; ++i )
  {
    FILE *f = रिक्त;
    यदि( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    अन्यथा
      f = fopen( argv[i], "rb" );

    यदि( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      अनुवर्त;
    }

    यदि( blake2_stream( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    अन्यथा
    {
      size_t j;
      यदि( bsdstyle )
      {
        यदि( outbytes < maxbytes )
          printf( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        अन्यथा
          printf( "%s (%s) = ", algorithm, argv[i] );
      }

      प्रति( j = 0; j < outbytes; ++j )
        printf( "%02x", hash[j] );

      यदि( bsdstyle )
        printf( "\n" );
      अन्यथा
        printf( "  %s\n", argv[i] );
    }

    यदि( f == stdin )
      clearerr( f );
    अन्यथा यदि( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  प्रत्यागम 0;
}
#endif
