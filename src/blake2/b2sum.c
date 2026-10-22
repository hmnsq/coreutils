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

#నిర్వచితమైతే HAVE_CONFIG_H
# include <config.h>
#అయితే_ముగింపు

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>

#అన్తర్భావ "blake2.h"

#ఒకవేళ 0
/* This will help compatibility with coreutils */
పూర్ణాంకం blake2s_ప్రవాహం( FILE *ప్రవాహం, శూన్య *resstream, size_t outbytes )
{
  పూర్ణాంకం ret = -1;
  size_t కూడిక, n;
  blake2s_state S[1];
  స్థిర స్థిరరాశి size_t బఫరు_నిడివి = 32768;
  uint8_t *బఫరు = ( uint8_t * )జ్ఞాపకం_ఇవ్వు( బఫరు_నిడివి );

  ఒకవేళ( !బఫరు ) తిరిగి -1;

  blake2s_init( S, outbytes );

  అయితే( 1 )
  {
    కూడిక = 0;

    అయితే( 1 )
    {
      n = fread( బఫరు + కూడిక, 1, బఫరు_నిడివి - కూడిక, ప్రవాహం );
      కూడిక += n;

      ఒకవేళ( బఫరు_నిడివి == కూడిక )
        ఆపు;

      ఒకవేళ( 0 == n )
      {
        ఒకవేళ( ferror( ప్రవాహం ) )
          వెళ్ళు cleanup_బఫరు;

        వెళ్ళు final_process;
      }

      ఒకవేళ( feof( ప్రవాహం ) )
        వెళ్ళు final_process;
    }

    blake2s_update( S, బఫరు, బఫరు_నిడివి );
  }

final_process:;

  ఒకవేళ( కూడిక > 0 ) blake2s_update( S, బఫరు, కూడిక );

  blake2s_final( S, resstream, outbytes );
  ret = 0;
cleanup_బఫరు:
  విడుదల( బఫరు );
  తిరిగి ret;
}
#అయితే_ముగింపు

పూర్ణాంకం blake2b_ప్రవాహం( FILE *ప్రవాహం, శూన్య *resstream, size_t outbytes )
{
  పూర్ణాంకం ret = -1;
  size_t కూడిక, n;
  blake2b_state S[1];
  స్థిర స్థిరరాశి size_t బఫరు_నిడివి = 32768;
  uint8_t *బఫరు = ( uint8_t * )జ్ఞాపకం_ఇవ్వు( బఫరు_నిడివి );

  ఒకవేళ( !బఫరు ) తిరిగి -1;

  blake2b_init( S, outbytes );

  అయితే( 1 )
  {
    కూడిక = 0;

    అయితే( 1 )
    {
      n = fread( బఫరు + కూడిక, 1, బఫరు_నిడివి - కూడిక, ప్రవాహం );
      కూడిక += n;

      ఒకవేళ( బఫరు_నిడివి == కూడిక )
        ఆపు;

      ఒకవేళ( 0 == n )
      {
        ఒకవేళ( ferror( ప్రవాహం ) )
          వెళ్ళు cleanup_బఫరు;

        వెళ్ళు final_process;
      }

      ఒకవేళ( feof( ప్రవాహం ) )
        వెళ్ళు final_process;
    }

    blake2b_update( S, బఫరు, బఫరు_నిడివి );
  }

final_process:;

  ఒకవేళ( కూడిక > 0 ) blake2b_update( S, బఫరు, కూడిక );

  blake2b_final( S, resstream, outbytes );
  ret = 0;
cleanup_బఫరు:
  విడుదల( బఫరు );
  తిరిగి ret;
}

#ఒకవేళ 0

#అన్తర్భావ <errno.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <unistd.h>

పూర్ణాంకం blake2sp_ప్రవాహం( FILE *ప్రవాహం, శూన్య *resstream, size_t outbytes )
{
  పూర్ణాంకం ret = -1;
  size_t కూడిక, n;
  blake2sp_state S[1];
  స్థిర స్థిరరాశి size_t బఫరు_నిడివి = 16 * ( 1UL << 20 );
  uint8_t *బఫరు = ( uint8_t * )జ్ఞాపకం_ఇవ్వు( బఫరు_నిడివి );

  ఒకవేళ( !బఫరు ) తిరిగి -1;

  blake2sp_init( S, outbytes );

  అయితే( 1 )
  {
    కూడిక = 0;

    అయితే( 1 )
    {
      n = fread( బఫరు + కూడిక, 1, బఫరు_నిడివి - కూడిక, ప్రవాహం );
      కూడిక += n;

      ఒకవేళ( బఫరు_నిడివి == కూడిక )
        ఆపు;

      ఒకవేళ( 0 == n )
      {
        ఒకవేళ( ferror( ప్రవాహం ) )
          వెళ్ళు cleanup_బఫరు;

        వెళ్ళు final_process;
      }

      ఒకవేళ( feof( ప్రవాహం ) )
        వెళ్ళు final_process;
    }

    blake2sp_update( S, బఫరు, బఫరు_నిడివి );
  }

final_process:;

  ఒకవేళ( కూడిక > 0 ) blake2sp_update( S, బఫరు, కూడిక );

  blake2sp_final( S, resstream, outbytes );
  ret = 0;
cleanup_బఫరు:
  విడుదల( బఫరు );
  తిరిగి ret;
}


పూర్ణాంకం blake2bp_ప్రవాహం( FILE *ప్రవాహం, శూన్య *resstream, size_t outbytes )
{
  పూర్ణాంకం ret = -1;
  size_t కూడిక, n;
  blake2bp_state S[1];
  స్థిర స్థిరరాశి size_t బఫరు_నిడివి = 16 * ( 1UL << 20 );
  uint8_t *బఫరు = ( uint8_t * )జ్ఞాపకం_ఇవ్వు( బఫరు_నిడివి );

  ఒకవేళ( !బఫరు ) తిరిగి -1;

  blake2bp_init( S, outbytes );

  అయితే( 1 )
  {
    కూడిక = 0;

    అయితే( 1 )
    {
      n = fread( బఫరు + కూడిక, 1, బఫరు_నిడివి - కూడిక, ప్రవాహం );
      కూడిక += n;

      ఒకవేళ( బఫరు_నిడివి == కూడిక )
        ఆపు;

      ఒకవేళ( 0 == n )
      {
        ఒకవేళ( ferror( ప్రవాహం ) )
          వెళ్ళు cleanup_బఫరు;

        వెళ్ళు final_process;
      }

      ఒకవేళ( feof( ప్రవాహం ) )
        వెళ్ళు final_process;
    }

    blake2bp_update( S, బఫరు, బఫరు_నిడివి );
  }

final_process:;

  ఒకవేళ( కూడిక > 0 ) blake2bp_update( S, బఫరు, కూడిక );

  blake2bp_final( S, resstream, outbytes );
  ret = 0;
cleanup_బఫరు:
  విడుదల( బఫరు );
  తిరిగి ret;
}

రకనిర్వచనం పూర్ణాంకం ( *blake2fn )( FILE *, శూన్య *, size_t );


స్థిర శూన్య usage( అక్షరం **argv, పూర్ణాంకం errcode )
{
  FILE *బహిర్గతం = errcode ? stderr : stdout;
  fprintf( బహిర్గతం, "Usage: %s [OPTION]... [FILE]...\n", argv[0] );
  fprintf( బహిర్గతం, "\n" );
  fprintf( బహిర్గతం, "With no FILE, or when FILE is -, read standard input.\n" );
  fprintf( బహిర్గతం, "\n" );
  fprintf( బహిర్గతం, "  -a <algo>    hash algorithm (blake2b is default): \n"
                "               [blake2b|blake2s|blake2bp|blake2sp]\n" );
  fprintf( బహిర్గతం, "  -l <length>  digest length in bits, must not exceed the maximum for\n"
                "               the selected algorithm and must be a multiple of 8\n" );
  fprintf( బహిర్గతం, "  --tag        create a BSD-style checksum\n" );
  fprintf( బహిర్గతం, "  --help       display this help and exit\n" );
  నిష్క్రమించు( errcode );
}


పూర్ణాంకం ప్రధాన( పూర్ణాంకం argc, అక్షరం **argv )
{
  blake2fn blake2_ప్రవాహం = blake2b_ప్రవాహం;
  గుర్తులేని దీర్ఘ maxbytes = BLAKE2B_OUTBYTES;
  స్థిరరాశి అక్షరం *algorithm = "BLAKE2b";
  గుర్తులేని దీర్ఘ outbytes = 0;
  గుర్తులేని అక్షరం hash[BLAKE2B_OUTBYTES] = {0};
  bool bsdstyle = false;
  పూర్ణాంకం c, i;
  opterr = 1;

  అయితే( 1 )
  {
    పూర్ణాంకం option_సూచీ = 0;
    అక్షరం *చివర = ఖాళీ;
    గుర్తులేని దీర్ఘ outbits;
    స్థిర నిర్మాణం option long_options[] = {
      { "help",  no_వాదం, 0,  0  },
      { "tag",   no_వాదం, 0,  0  },
      { ఖాళీ, 0, ఖాళీ, 0 }
    };

    c = getopt_long( argc, argv, "a:l:", long_options, &option_సూచీ );
    ఒకవేళ( c == -1 ) ఆపు;
    ఎంపిక( c )
    {
    స్థితి 'a':
      ఒకవేళ( 0 == strcmp( optarg, "blake2b" ) )
      {
        blake2_ప్రవాహం = blake2b_ప్రవాహం;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2b";
      }
      లేకపోతే ఒకవేళ ( 0 == strcmp( optarg, "blake2s" ) )
      {
        blake2_ప్రవాహం = blake2s_ప్రవాహం;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2s";
      }
      లేకపోతే ఒకవేళ ( 0 == strcmp( optarg, "blake2bp" ) )
      {
        blake2_ప్రవాహం = blake2bp_ప్రవాహం;
        maxbytes = BLAKE2B_OUTBYTES;
        algorithm = "BLAKE2bp";
      }
      లేకపోతే ఒకవేళ ( 0 == strcmp( optarg, "blake2sp" ) )
      {
        blake2_ప్రవాహం = blake2sp_ప్రవాహం;
        maxbytes = BLAKE2S_OUTBYTES;
        algorithm = "BLAKE2sp";
      }
      లేకపోతే
      {
        రాయి( "Invalid function name: `%s'\n", optarg );
        usage( argv, 111 );
      }

      ఆపు;

    స్థితి 'l':
      outbits = strtoul(optarg, &చివర, 10);
      ఒకవేళ( !చివర || *చివర != '\0' || outbits % 8 != 0)
      {
        రాయి( "Invalid length argument: `%s'\n", optarg);
        usage( argv, 111 );
      }
      outbytes = outbits / 8;
      ఆపు;

    స్థితి 0:
      ఒకవేళ( 0 == strcmp( "help", long_options[option_సూచీ].name ) )
        usage( argv, 0 );
      లేకపోతే ఒకవేళ( 0 == strcmp( "tag", long_options[option_సూచీ].name ) )
        bsdstyle = true;
      ఆపు;

    స్థితి '?':
      usage( argv, 1 );
      ఆపు;
    }
  }

  ఒకవేళ(outbytes > maxbytes)
  {
    రాయి( "Invalid length argument: %lu\n", outbytes * 8 );
    రాయి( "Maximum digest length for %s is %lu\n", algorithm, maxbytes * 8 );
    usage( argv, 111 );
  }
  లేకపోతే ఒకవేళ( outbytes == 0 )
    outbytes = maxbytes;

  ఒకవేళ( optind == argc )
    argv[argc++] = (అక్షరం *) "-";

  కొరకు( i = optind; i < argc; ++i )
  {
    FILE *f = ఖాళీ;
    ఒకవేళ( argv[i][0] == '-' && argv[i][1] == '\0' )
      f = stdin;
    లేకపోతే
      f = fopen( argv[i], "rb" );

    ఒకవేళ( !f )
    {
      fprintf( stderr, "Could not open `%s': %s\n", argv[i], strerror( errno ) );
      కొనసాగు;
    }

    ఒకవేళ( blake2_ప్రవాహం( f, hash, outbytes ) < 0 )
    {
      fprintf( stderr, "Failed to hash `%s'\n", argv[i] );
    }
    లేకపోతే
    {
      size_t j;
      ఒకవేళ( bsdstyle )
      {
        ఒకవేళ( outbytes < maxbytes )
          రాయి( "%s-%lu (%s) = ", algorithm, outbytes * 8, argv[i] );
        లేకపోతే
          రాయి( "%s (%s) = ", algorithm, argv[i] );
      }

      కొరకు( j = 0; j < outbytes; ++j )
        రాయి( "%02x", hash[j] );

      ఒకవేళ( bsdstyle )
        రాయి( "\n" );
      లేకపోతే
        రాయి( "  %s\n", argv[i] );
    }

    ఒకవేళ( f == stdin )
      clearerr( f );
    లేకపోతే ఒకవేళ( fclose( f ) != 0 )
      fprintf( stderr, "Could not close `%s': %s\n", argv[i], strerror( errno ) );
  }

  తిరిగి 0;
}
#అయితే_ముగింపు
