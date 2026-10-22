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

#यदि_परिभाषितम् HAVE_CONFIG_H
# include <config.h>
#यदि_अन्तः

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "blake2.h"

#यदि 0
/* This will help compatibility with coreutils */
पूर्णाङ्क blake2s_प्रवाहः( FILE *प्रवाहः, शून्य *resstream, size_t outbytes )
{
  पूर्णाङ्क ret = -1;
  size_t योग, n;
  blake2s_state S[1];
  स्थिर ध्रुव size_t बफर_दीर्घता = 32768;
  uint8_t *बफर = ( uint8_t * )स्मृतिं_देहि( बफर_दीर्घता );

  यदि( !बफर ) प्रत्यागम -1;

  blake2s_init( S, outbytes );

  यावत्( 1 )
  {
    योग = 0;

    यावत्( 1 )
    {
      n = fread( बफर + योग, 1, बफर_दीर्घता - योग, प्रवाहः );
      योग += n;

      यदि( बफर_दीर्घता == योग )
        भङ्ग;

      यदि( 0 == n )
      {
        यदि( ferror( प्रवाहः ) )
          गच्छ cleanup_बफर;

        गच्छ final_process;
      }

      यदि( feof( प्रवाहः ) )
        गच्छ final_process;
    }

    blake2s_update( S, बफर, बफर_दीर्घता );
  }

final_process:;

  यदि( योग > 0 ) blake2s_update( S, बफर, योग );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_बफर:
  मुक्त( बफर );
  प्रत्यागम ret;
}
#यदि_अन्तः

पूर्णाङ्क blake2b_प्रवाहः( FILE *प्रवाहः, शून्य *resstream, size_t outbytes )
{
  पूर्णाङ्क ret = -1;
  size_t योग, n;
  blake2b_state S[1];
  स्थिर ध्रुव size_t बफर_दीर्घता = 32768;
  uint8_t *बफर = ( uint8_t * )स्मृतिं_देहि( बफर_दीर्घता );

  यदि( !बफर ) प्रत्यागम -1;

  blake2b_init( S, outbytes );

  यावत्( 1 )
  {
    योग = 0;

    यावत्( 1 )
    {
      n = fread( बफर + योग, 1, बफर_दीर्घता - योग, प्रवाहः );
      योग += n;

      यदि( बफर_दीर्घता == योग )
        भङ्ग;

      यदि( 0 == n )
      {
        यदि( ferror( प्रवाहः ) )
          गच्छ cleanup_बफर;

        गच्छ final_process;
      }

      यदि( feof( प्रवाहः ) )
        गच्छ final_process;
    }

    blake2b_update( S, बफर, बफर_दीर्घता );
  }

final_process:;

  यदि( योग > 0 ) blake2b_update( S, बफर, योग );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_बफर:
  मुक्त( बफर );
  प्रत्यागम ret;
}

#यदि 0

#अन्तर्भाव <errno.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>

पूर्णाङ्क blake2sp_प्रवाहः( FILE *प्रवाहः, शून्य *resstream, size_t outbytes )
{
  पूर्णाङ्क ret = -1;
  size_t योग, n;
  blake2sp_state S[1];
  स्थिर ध्रुव size_t बफर_दीर्घता = 16 * ( 1UL << 20 );
  uint8_t *बफर = ( uint8_t * )स्मृतिं_देहि( बफर_दीर्घता );

  यदि( !बफर ) प्रत्यागम -1;

  blake2sp_init( S, outbytes );

  यावत्( 1 )
  {
    योग = 0;

    यावत्( 1 )
    {
      n = fread( बफर + योग, 1, बफर_दीर्घता - योग, प्रवाहः );
      योग += n;

      यदि( बफर_दीर्घता == योग )
        भङ्ग;

      यदि( 0 == n )
      {
        यदि( ferror( प्रवाहः ) )
          गच्छ cleanup_बफर;

        गच्छ final_process;
      }

      यदि( feof( प्रवाहः ) )
        गच्छ final_process;
    }

    blake2sp_update( S, बफर, बफर_दीर्घता );
  }

final_process:;

  यदि( योग > 0 ) blake2sp_update( S, बफर, योग );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_बफर:
  मुक्त( बफर );
  प्रत्यागम ret;
}


पूर्णाङ्क blake2bp_प्रवाहः( FILE *प्रवाहः, शून्य *resstream, size_t outbytes )
{
  पूर्णाङ्क ret = -1;
  size_t योग, n;
  blake2bp_state S[1];
  स्थिर ध्रुव size_t बफर_दीर्घता = 16 * ( 1UL << 20 );
  uint8_t *बफर = ( uint8_t * )स्मृतिं_देहि( बफर_दीर्घता );

  यदि( !बफर ) प्रत्यागम -1;

  blake2bp_init( S, outbytes );

  यावत्( 1 )
  {
    योग = 0;

    यावत्( 1 )
    {
      n = fread( बफर + योग, 1, बफर_दीर्घता - योग, प्रवाहः );
      योग += n;

      यदि( बफर_दीर्घता == योग )
        भङ्ग;

      यदि( 0 == n )
      {
        यदि( ferror( प्रवाहः ) )
          गच्छ cleanup_बफर;

        गच्छ final_process;
      }

      यदि( feof( प्रवाहः ) )
        गच्छ final_process;
    }

    blake2bp_update( S, बफर, बफर_दीर्घता );
  }

final_process:;

  यदि( योग > 0 ) blake2bp_update( S, बफर, योग );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_बफर:
  मुक्त( बफर );
  प्रत्यागम ret;
}

प्रकारनिर्देश पूर्णाङ्क ( *blake2fn )( FILE *, शून्य *, size_t );


स्थिर शून्य usage( वर्ण **argv, पूर्णाङ्क errcode )
{
  FILE *बहिर्गमनम् = errcode ? stderr : stdout;
  fprintf( बहिर्गमनम्, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( बहिर्गमनम्, "\n" );
  fprintf( बहिर्गमनम्, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( बहिर्गमनम्, "\n" );
  fprintf( बहिर्गमनम्, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( बहिर्गमनम्, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( बहिर्गमनम्, "  --tag        create a BSD-style checksum\n" );
  fprintf( बहिर्गमनम्, "  --help       display this help and exit\n" );
  निर्गच्छ( errcode );
}


पूर्णाङ्क मुख्यम्( पूर्णाङ्क argc, वर्ण **argv )
{
  blake2fn blake2_प्रवाहः = blake2b_प्रवाहः;
  अचिह्नित दीर्घ maxbytes = BLAKE2B_OUTBYTES;
  ध्रुव वर्ण *algorithm = "BLAKE2b";
  अचिह्नित दीर्घ outbytes = 0;
  अचिह्नित वर्ण hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  पूर्णाङ्क c, i;
  opterr = 1;

  यावत्( 1 )
  {
    पूर्णाङ्क option_सूचकांक = 0;
    वर्ण *अन्तः = रिक्त;
    अचिह्नित दीर्घ outbits;
    स्थिर संरचना option long_options[] = {
      { "help",  no_तर्कः, 0,  0  },
      { "tag",   no_तर्कः, 0,  0  },
      { रिक्त, 0, रिक्त, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_सूचकांक );
    यदि( c == -1 ) भङ्ग;
    विकल्प( c )
    {
    पक्ष 'a':
      यदि( 0 == वाक्यं_तुलय( optarg, "blake2b" ) )
      {
        blake2_प्रवाहः = blake2b_प्रवाहः;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      अन्यथा यदि ( 0 == वाक्यं_तुलय( optarg, "blake2s" ) )
      {
        blake2_प्रवाहः = blake2s_प्रवाहः;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      अन्यथा यदि ( 0 == वाक्यं_तुलय( optarg, "blake2bp" ) )
      {
        blake2_प्रवाहः = blake2bp_प्रवाहः;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      अन्यथा यदि ( 0 == वाक्यं_तुलय( optarg, "blake2sp" ) )
      {
        blake2_प्रवाहः = blake2sp_प्रवाहः;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      अन्यथा
      {
        प्रारूप_लिख( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      भङ्ग;

    पक्ष 'l':
      outbits = strtoul(optarg, &अन्तः, 10);
      यदि( !अन्तः || *अन्तः != '\0' || outbits % 8 != 0)
      {
        प्रारूप_लिख( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      भङ्ग;

    पक्ष 0:
      यदि( 0 == वाक्यं_तुलय( "help", long_options[option_सूचकांक].name ) )
        usage( argv, 0 );
      अन्यथा यदि( 0 == वाक्यं_तुलय( "tag", long_options[option_सूचकांक].name ) )
        bsdstyle = true;
      भङ्ग;

    पक्ष '?':
      usage( argv, 1 );
      भङ्ग;
    }
  }

  यदि(outbytes > maxbytes)
  {
    प्रारूप_लिख( "Invalid length argument: %lu\n", outbytes * 8 );
    प्रारूप_लिख( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
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

    यदि( blake2_प्रवाहः( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    अन्यथा
    {
      size_t j;
      यदि( bsdstyle )
      {
        यदि( outbytes < maxbytes )
          प्रारूप_लिख( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        अन्यथा
          प्रारूप_लिख( "%s (%s) = ", algorithm, argv[i] );
      }

      प्रति( j = 0; j < outbytes; ++j )
        प्रारूप_लिख( "%02x", hash[j] );

      यदि( bsdstyle )
        प्रारूप_लिख( "\n" );
      अन्यथा
        प्रारूप_लिख( "  %s\n", argv[i] );
    }

    यदि( f == stdin )
      clearerr( f );
    अन्यथा यदि( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  प्रत्यागम 0;
}
#यदि_अन्तः
