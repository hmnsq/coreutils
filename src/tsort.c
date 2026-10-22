/* tsort - topological sort.
   Copyright (C) 1998-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Mark Kettenis <kettenis@phys.uva.nl>.  */

/* The topological sort is done according to Algorithm T (Topological
   sort) in Donald E. Knuth, The Art of Computer Programming, Volume
   1/Fundamental Algorithms, page 262.  */

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "readtokens.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
ସଂରଚନା successor
{
  ସଂରଚନା item *suc;
  ସଂରଚନା successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
ସଂରଚନା item
{
  ଅକ୍ଷର ଅଚଳ *str;
  ସଂରଚନା item *ବାମ, *ଡାହାଣ;
  signed ଅକ୍ଷର balance; /* -1, 0, or +1 */
  bool printed;
  size_t ଗଣନା_2;
  ସଂରଚନା item *qlink;
  ସଂରଚନା successor *ଶୀର୍ଷ;
};

/* The head of the sorted list.  */
ସ୍ଥିର ସଂରଚନା item *head = ଖାଲି;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ସ୍ଥିର ସଂରଚନା item *zeros = ଖାଲି;

/* Used for loop detection.  */
ସ୍ଥିର ସଂରଚନା item *loop = ଖାଲି;

/* The number of strings to sort.  */
ସ୍ଥିର size_t n_strings = 0;

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION] [FILE]\n\
Write totally ordered list consistent with the partial ordering in FILE.\n\
"), program_name);

      emit_stdin_note ();

      fputs (_("\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* Create a new item/node for STR.  */
ସ୍ଥିର ସଂରଚନା item *
new_item (ଅକ୍ଷର ଅଚଳ *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  ସଂରଚନା item *k = xzalloc (ଆକାର *k);
  ଯଦି (str)
    k->str = xstrdup (str);
  ଫେରନ୍ତୁ k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

ସ୍ଥିର ସଂରଚନା item *
ସନ୍ଧାନ_item (ସଂରଚନା item *ମୂଳ, ଅକ୍ଷର ଅଚଳ *str)
{
  ସଂରଚନା item *p, *q, *r, *s, *t;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  ଯଦି (ମୂଳ->ଡାହାଣ == ଖାଲି)
    ଫେରନ୍ତୁ (ମୂଳ->ଡାହାଣ = new_item (str));

  /* A1. Initialize.  */
  t = ମୂଳ;
  s = p = ମୂଳ->ଡାହାଣ;

  ଯେତେବେଳେ (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      ଯଦି (a == 0)
        ଫେରନ୍ତୁ p;

      /* A3 & A4.  Move left & right.  */
      ଯଦି (a < 0)
        q = p->ବାମ;
      ଅନ୍ୟଥା
        q = p->ଡାହାଣ;

      ଯଦି (q == ଖାଲି)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          ଯଦି (a < 0)
            p->ବାମ = q;
          ଅନ୍ୟଥା
            p->ଡାହାଣ = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          ଯଦି (a < 0)
            {
              r = p = s->ବାମ;
              a = -1;
            }
          ଅନ୍ୟଥା
            {
              affirm (0 < a);
              r = p = s->ଡାହାଣ;
              a = 1;
            }

          ଯେତେବେଳେ (p != q)
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = strcmp (str, p->str);
              ଯଦି (cmp < 0)
                {
                  p->balance = -1;
                  p = p->ବାମ;
                }
              ଅନ୍ୟଥା
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->ଡାହାଣ;
                }
            }

          /* A7. Balancing act.  */
          ଯଦି (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              ଫେରନ୍ତୁ q;
            }

          ଯଦି (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              ଯଦି (a < 0)
                {
                  s->ବାମ = r->ଡାହାଣ;
                  r->ଡାହାଣ = s;
                }
              ଅନ୍ୟଥା
                {
                  s->ଡାହାଣ = r->ବାମ;
                  r->ବାମ = s;
                }
              s->balance = r->balance = 0;
            }
          ଅନ୍ୟଥା
            {
              /* A9. Double rotation.  */
              ଯଦି (a < 0)
                {
                  p = r->ଡାହାଣ;
                  r->ଡାହାଣ = p->ବାମ;
                  p->ବାମ = r;
                  s->ବାମ = p->ଡାହାଣ;
                  p->ଡାହାଣ = s;
                }
              ଅନ୍ୟଥା
                {
                  p = r->ବାମ;
                  r->ବାମ = p->ଡାହାଣ;
                  p->ଡାହାଣ = r;
                  s->ଡାହାଣ = p->ବାମ;
                  p->ବାମ = s;
                }

              s->balance = 0;
              r->balance = 0;
              ଯଦି (p->balance == a)
                s->balance = -a;
              ଅନ୍ୟଥା ଯଦି (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          ଯଦି (s == t->ଡାହାଣ)
            t->ଡାହାଣ = p;
          ଅନ୍ୟଥା
            t->ବାମ = p;

          ଫେରନ୍ତୁ q;
        }

      /* A3 & A4.  (continued).  */
      ଯଦି (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

ସ୍ଥିର ଶୂନ୍ୟ
record_relation (ସଂରଚନା item *j, ସଂରଚନା item *k)
{
  ସଂରଚନା successor *p;

  ଯଦି (!streq (j->str, k->str))
    {
      k->ଗଣନା_2++;
      p = xmalloc (ଆକାର *p);
      p->suc = k;
      p->next = j->ଶୀର୍ଷ;
      j->ଶୀର୍ଷ = p;
    }
}

ସ୍ଥିର bool
ଗଣନା_ବସ୍ତୁଗୁଡ଼ିକ (MAYBE_UNUSED ସଂରଚନା item *unused)
{
  n_strings++;
  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
scan_zeros (ସଂରଚନା item *k)
{
  /* Ignore strings that have already been printed.  */
  ଯଦି (k->ଗଣନା_2 == 0 && !k->printed)
    {
      ଯଦି (head == ଖାଲି)
        head = k;
      ଅନ୍ୟଥା
        zeros->qlink = k;

      zeros = k;
    }

  ଫେରନ୍ତୁ false;
}

/* Try to detect the loop.  If we have detected that K is part of a
   loop, print the loop on standard error, remove a relation to break
   the loop, and return true.

   The loop detection strategy is as follows: Realize that what we're
   dealing with is essentially a directed graph.  If we find an item
   that is part of a graph that contains a cycle we traverse the graph
   in backwards direction.  In general there is no unique way to do
   this, but that is no problem.  If we encounter an item that we have
   encountered before, we know that we've found a cycle.  All we have
   to do now is retrace our steps, printing out the items until we
   encounter that item again.  (This is not necessarily the item that
   we started from originally.)  Since the order in which the items
   are stored in the tree is not related to the specified partial
   ordering, we may need to walk the tree several times before the
   loop has completely been constructed.  If the loop was found, the
   global variable LOOP will be null.  */

ସ୍ଥିର bool
detect_loop (ସଂରଚନା item *k)
{
  ଯଦି (k->ଗଣନା_2 > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      ଯଦି (loop == ଖାଲି)
        /* Start traversing the graph at K.  */
        loop = k;
      ଅନ୍ୟଥା
        {
          ସଂରଚନା successor **p = &k->ଶୀର୍ଷ;

          ଯେତେବେଳେ (*p)
            {
              ଯଦି ((*p)->suc == loop)
                {
                  ଯଦି (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      ଯେତେବେଳେ (loop)
                        {
                          ସଂରଚନା item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          ଯଦି (loop == k)
                            {
                              /* Remove relation.  */
                              ସଂରଚନା successor *s = *p;
                              s->suc->ଗଣନା_2--;
                              *p = s->next;
                              IF_LINT (ମୁକ୍ତ (s));
                              ଭାଙ୍ଗ;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = ଖାଲି;
                          loop = tmp;
                        }

                      ଯେତେବେଳେ (loop)
                        {
                          ସଂରଚନା item *tmp = loop->qlink;

                          loop->qlink = ଖାଲି;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      ଫେରନ୍ତୁ true;
                    }
                  ଅନ୍ୟଥା
                    {
                      k->qlink = loop;
                      loop = k;
                      ଭାଙ୍ଗ;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  ଫେରନ୍ତୁ false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ସ୍ଥିର bool
recurse_ବୃକ୍ଷ (ସଂରଚନା item *ମୂଳ, bool (*action) (ସଂରଚନା item *))
{
  ଯଦି (ମୂଳ->ବାମ == ଖାଲି && ମୂଳ->ଡାହାଣ == ଖାଲି)
    ଫେରନ୍ତୁ (*action) (ମୂଳ);
  ଅନ୍ୟଥା
    {
      ଯଦି (ମୂଳ->ବାମ != ଖାଲି)
        ଯଦି (recurse_ବୃକ୍ଷ (ମୂଳ->ବାମ, action))
          ଫେରନ୍ତୁ true;
      ଯଦି ((*action) (ମୂଳ))
        ଫେରନ୍ତୁ true;
      ଯଦି (ମୂଳ->ଡାହାଣ != ଖାଲି)
        ଯଦି (recurse_ବୃକ୍ଷ (ମୂଳ->ଡାହାଣ, action))
          ଫେରନ୍ତୁ true;
    }

  ଫେରନ୍ତୁ false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ସ୍ଥିର ଶୂନ୍ୟ
walk_ବୃକ୍ଷ (ସଂରଚନା item *ମୂଳ, bool (*action) (ସଂରଚନା item *))
{
  ଯଦି (ମୂଳ->ଡାହାଣ)
    recurse_ବୃକ୍ଷ (ମୂଳ->ଡାହାଣ, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ସ୍ଥିର _Noreturn ଶୂନ୍ୟ
tsort (ଅକ୍ଷର ଅଚଳ *file)
{
  bool ok = true;
  ସଂରଚନା item *j = ଖାଲି;
  ସଂରଚନା item *k = ଖାଲି;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  ସଂରଚନା item *ମୂଳ = new_item (ଖାଲି);

  ଯଦି (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  ଯେତେବେଳେ (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, ଆକାର (DELIM) - 1, &tokenbuffer);
      ଯଦି (len == (size_t) -1)
        {
          ଯଦି (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          ଭାଙ୍ଗ;
        }

      affirm (len != 0);

      k = ସନ୍ଧାନ_item (ମୂଳ, tokenbuffer.buffer);
      ଯଦି (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = ଖାଲି;
        }

      j = k;
    }

  ଯଦି (k != ଖାଲି)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_ବୃକ୍ଷ (ମୂଳ, ଗଣନା_ବସ୍ତୁଗୁଡ଼ିକ);

  ଯେତେବେଳେ (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_ବୃକ୍ଷ (ମୂଳ, scan_zeros);

      ଯେତେବେଳେ (head)
        {
          ସଂରଚନା successor *p = head->ଶୀର୍ଷ;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          ଯେତେବେଳେ (p)
            {
              p->suc->ଗଣନା_2--;
              ଯଦି (p->suc->ଗଣନା_2 == 0)
                {
                  zeros->qlink = p->suc;
                  zeros = p->suc;
                }

              p = p->next;
            }

          /* T7. Remove from queue.  */
          head = head->qlink;
        }

      /* T8.  End of process.  */
      ଯଦି (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          କର
            walk_ବୃକ୍ଷ (ମୂଳ, detect_loop);
          ଯେତେବେଳେ (loop);
        }
    }

  ଯଦି (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ (true)
    {
      ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {ଖାଲି, 0, ଖାଲି, 0}
        };
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getopt_long (argc, argv, "w", long_options, ଖାଲି);

      ଯଦି (c == -1)
        ଭାଙ୍ଗ;

      ବାଛ (c)
        {
        ସ୍ଥିତି 'w':
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
