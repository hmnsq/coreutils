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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "readtokens.h"
#அந்தர்பாவ "stdio--.h"
#அந்தர்பாவ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
கட்டமைப்பு successor
{
  கட்டமைப்பு item *suc;
  கட்டமைப்பு successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
கட்டமைப்பு item
{
  எழுத்து மாறிலி *str;
  கட்டமைப்பு item *இடது, *வலது;
  signed எழுத்து balance; /* -1, 0, or +1 */
  bool printed;
  size_t மொத்தம்;
  கட்டமைப்பு item *qlink;
  கட்டமைப்பு successor *உச்சி;
};

/* The head of the sorted list.  */
நிலையான கட்டமைப்பு item *head = வெற்று;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
நிலையான கட்டமைப்பு item *zeros = வெற்று;

/* Used for loop detection.  */
நிலையான கட்டமைப்பு item *loop = வெற்று;

/* The number of strings to sort.  */
நிலையான size_t n_strings = 0;

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
நிலையான கட்டமைப்பு item *
new_item (எழுத்து மாறிலி *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  கட்டமைப்பு item *k = xzalloc (அளவு *k);
  எனில் (str)
    k->str = xstrdup (str);
  திரும்பு k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

நிலையான கட்டமைப்பு item *
தேடல்_item (கட்டமைப்பு item *வேர், எழுத்து மாறிலி *str)
{
  கட்டமைப்பு item *p, *q, *r, *s, *t;
  முழு a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  எனில் (வேர்->வலது == வெற்று)
    திரும்பு (வேர்->வலது = new_item (str));

  /* A1. Initialize.  */
  t = வேர்;
  s = p = வேர்->வலது;

  போது (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      எனில் (a == 0)
        திரும்பு p;

      /* A3 & A4.  Move left & right.  */
      எனில் (a < 0)
        q = p->இடது;
      இல்லையெனில்
        q = p->வலது;

      எனில் (q == வெற்று)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          எனில் (a < 0)
            p->இடது = q;
          இல்லையெனில்
            p->வலது = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          எனில் (a < 0)
            {
              r = p = s->இடது;
              a = -1;
            }
          இல்லையெனில்
            {
              affirm (0 < a);
              r = p = s->வலது;
              a = 1;
            }

          போது (p != q)
            {
              முழு cmp = strcmp (str, p->str);
              எனில் (cmp < 0)
                {
                  p->balance = -1;
                  p = p->இடது;
                }
              இல்லையெனில்
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->வலது;
                }
            }

          /* A7. Balancing act.  */
          எனில் (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              திரும்பு q;
            }

          எனில் (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              எனில் (a < 0)
                {
                  s->இடது = r->வலது;
                  r->வலது = s;
                }
              இல்லையெனில்
                {
                  s->வலது = r->இடது;
                  r->இடது = s;
                }
              s->balance = r->balance = 0;
            }
          இல்லையெனில்
            {
              /* A9. Double rotation.  */
              எனில் (a < 0)
                {
                  p = r->வலது;
                  r->வலது = p->இடது;
                  p->இடது = r;
                  s->இடது = p->வலது;
                  p->வலது = s;
                }
              இல்லையெனில்
                {
                  p = r->இடது;
                  r->இடது = p->வலது;
                  p->வலது = r;
                  s->வலது = p->இடது;
                  p->இடது = s;
                }

              s->balance = 0;
              r->balance = 0;
              எனில் (p->balance == a)
                s->balance = -a;
              இல்லையெனில் எனில் (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          எனில் (s == t->வலது)
            t->வலது = p;
          இல்லையெனில்
            t->இடது = p;

          திரும்பு q;
        }

      /* A3 & A4.  (continued).  */
      எனில் (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

நிலையான வெறுமை
record_relation (கட்டமைப்பு item *j, கட்டமைப்பு item *k)
{
  கட்டமைப்பு successor *p;

  எனில் (!streq (j->str, k->str))
    {
      k->மொத்தம்++;
      p = xmalloc (அளவு *p);
      p->suc = k;
      p->next = j->உச்சி;
      j->உச்சி = p;
    }
}

நிலையான bool
மொத்தம்_பொருட்கள் (MAYBE_UNUSED கட்டமைப்பு item *unused)
{
  n_strings++;
  திரும்பு false;
}

நிலையான bool
scan_zeros (கட்டமைப்பு item *k)
{
  /* Ignore strings that have already been printed.  */
  எனில் (k->மொத்தம் == 0 && !k->printed)
    {
      எனில் (head == வெற்று)
        head = k;
      இல்லையெனில்
        zeros->qlink = k;

      zeros = k;
    }

  திரும்பு false;
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

நிலையான bool
detect_loop (கட்டமைப்பு item *k)
{
  எனில் (k->மொத்தம் > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      எனில் (loop == வெற்று)
        /* Start traversing the graph at K.  */
        loop = k;
      இல்லையெனில்
        {
          கட்டமைப்பு successor **p = &k->உச்சி;

          போது (*p)
            {
              எனில் ((*p)->suc == loop)
                {
                  எனில் (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      போது (loop)
                        {
                          கட்டமைப்பு item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          எனில் (loop == k)
                            {
                              /* Remove relation.  */
                              கட்டமைப்பு successor *s = *p;
                              s->suc->மொத்தம்--;
                              *p = s->next;
                              IF_LINT (விடுவி (s));
                              நிறுத்து;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = வெற்று;
                          loop = tmp;
                        }

                      போது (loop)
                        {
                          கட்டமைப்பு item *tmp = loop->qlink;

                          loop->qlink = வெற்று;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      திரும்பு true;
                    }
                  இல்லையெனில்
                    {
                      k->qlink = loop;
                      loop = k;
                      நிறுத்து;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  திரும்பு false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

நிலையான bool
recurse_மரம் (கட்டமைப்பு item *வேர், bool (*action) (கட்டமைப்பு item *))
{
  எனில் (வேர்->இடது == வெற்று && வேர்->வலது == வெற்று)
    திரும்பு (*action) (வேர்);
  இல்லையெனில்
    {
      எனில் (வேர்->இடது != வெற்று)
        எனில் (recurse_மரம் (வேர்->இடது, action))
          திரும்பு true;
      எனில் ((*action) (வேர்))
        திரும்பு true;
      எனில் (வேர்->வலது != வெற்று)
        எனில் (recurse_மரம் (வேர்->வலது, action))
          திரும்பு true;
    }

  திரும்பு false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

நிலையான வெறுமை
walk_மரம் (கட்டமைப்பு item *வேர், bool (*action) (கட்டமைப்பு item *))
{
  எனில் (வேர்->வலது)
    recurse_மரம் (வேர்->வலது, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

நிலையான _Noreturn வெறுமை
tsort (எழுத்து மாறிலி *file)
{
  bool ok = true;
  கட்டமைப்பு item *j = வெற்று;
  கட்டமைப்பு item *k = வெற்று;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  கட்டமைப்பு item *வேர் = new_item (வெற்று);

  எனில் (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  போது (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, அளவு (DELIM) - 1, &tokenbuffer);
      எனில் (len == (size_t) -1)
        {
          எனில் (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          நிறுத்து;
        }

      affirm (len != 0);

      k = தேடல்_item (வேர், tokenbuffer.buffer);
      எனில் (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = வெற்று;
        }

      j = k;
    }

  எனில் (k != வெற்று)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_மரம் (வேர், மொத்தம்_பொருட்கள்);

  போது (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_மரம் (வேர், scan_zeros);

      போது (head)
        {
          கட்டமைப்பு successor *p = head->உச்சி;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          போது (p)
            {
              p->suc->மொத்தம்--;
              எனில் (p->suc->மொத்தம் == 0)
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
      எனில் (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          செய்
            walk_மரம் (வேர், detect_loop);
          போது (loop);
        }
    }

  எனில் (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது (true)
    {
      நிலையான கட்டமைப்பு option மாறிலி long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {வெற்று, 0, வெற்று, 0}
        };
      முழு c = getopt_long (argc, argv, "w", long_options, வெற்று);

      எனில் (c == -1)
        நிறுத்து;

      தேர்வு (c)
        {
        நிலை 'w':
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
