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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "readtokens.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
સંરચના successor
{
  સંરચના item *suc;
  સંરચના successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
સંરચના item
{
  અક્ષર અચલ *str;
  સંરચના item *ડાબું, *જમણું;
  signed અક્ષર balance; /* -1, 0, or +1 */
  bool printed;
  size_t ગણતરી;
  સંરચના item *qlink;
  સંરચના successor *ટોચ;
};

/* The head of the sorted list.  */
સ્થિર સંરચના item *head = ખાલી;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
સ્થિર સંરચના item *zeros = ખાલી;

/* Used for loop detection.  */
સ્થિર સંરચના item *loop = ખાલી;

/* The number of strings to sort.  */
સ્થિર size_t n_strings = 0;

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
સ્થિર સંરચના item *
new_item (અક્ષર અચલ *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  સંરચના item *k = xzalloc (કદ *k);
  જો (str)
    k->str = xstrdup (str);
  પાછા k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

સ્થિર સંરચના item *
શોધ_item (સંરચના item *મૂળ, અક્ષર અચલ *str)
{
  સંરચના item *p, *q, *r, *s, *t;
  પૂર્ણાંક a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  જો (મૂળ->જમણું == ખાલી)
    પાછા (મૂળ->જમણું = new_item (str));

  /* A1. Initialize.  */
  t = મૂળ;
  s = p = મૂળ->જમણું;

  જ્યારે (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      જો (a == 0)
        પાછા p;

      /* A3 & A4.  Move left & right.  */
      જો (a < 0)
        q = p->ડાબું;
      નહીંતર
        q = p->જમણું;

      જો (q == ખાલી)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          જો (a < 0)
            p->ડાબું = q;
          નહીંતર
            p->જમણું = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          જો (a < 0)
            {
              r = p = s->ડાબું;
              a = -1;
            }
          નહીંતર
            {
              affirm (0 < a);
              r = p = s->જમણું;
              a = 1;
            }

          જ્યારે (p != q)
            {
              પૂર્ણાંક cmp = strcmp (str, p->str);
              જો (cmp < 0)
                {
                  p->balance = -1;
                  p = p->ડાબું;
                }
              નહીંતર
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->જમણું;
                }
            }

          /* A7. Balancing act.  */
          જો (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              પાછા q;
            }

          જો (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              જો (a < 0)
                {
                  s->ડાબું = r->જમણું;
                  r->જમણું = s;
                }
              નહીંતર
                {
                  s->જમણું = r->ડાબું;
                  r->ડાબું = s;
                }
              s->balance = r->balance = 0;
            }
          નહીંતર
            {
              /* A9. Double rotation.  */
              જો (a < 0)
                {
                  p = r->જમણું;
                  r->જમણું = p->ડાબું;
                  p->ડાબું = r;
                  s->ડાબું = p->જમણું;
                  p->જમણું = s;
                }
              નહીંતર
                {
                  p = r->ડાબું;
                  r->ડાબું = p->જમણું;
                  p->જમણું = r;
                  s->જમણું = p->ડાબું;
                  p->ડાબું = s;
                }

              s->balance = 0;
              r->balance = 0;
              જો (p->balance == a)
                s->balance = -a;
              નહીંતર જો (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          જો (s == t->જમણું)
            t->જમણું = p;
          નહીંતર
            t->ડાબું = p;

          પાછા q;
        }

      /* A3 & A4.  (continued).  */
      જો (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

સ્થિર શૂન્ય
record_relation (સંરચના item *j, સંરચના item *k)
{
  સંરચના successor *p;

  જો (!streq (j->str, k->str))
    {
      k->ગણતરી++;
      p = xmalloc (કદ *p);
      p->suc = k;
      p->next = j->ટોચ;
      j->ટોચ = p;
    }
}

સ્થિર bool
ગણતરી_વસ્તુઓ (MAYBE_UNUSED સંરચના item *unused)
{
  n_strings++;
  પાછા false;
}

સ્થિર bool
scan_zeros (સંરચના item *k)
{
  /* Ignore strings that have already been printed.  */
  જો (k->ગણતરી == 0 && !k->printed)
    {
      જો (head == ખાલી)
        head = k;
      નહીંતર
        zeros->qlink = k;

      zeros = k;
    }

  પાછા false;
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

સ્થિર bool
detect_loop (સંરચના item *k)
{
  જો (k->ગણતરી > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      જો (loop == ખાલી)
        /* Start traversing the graph at K.  */
        loop = k;
      નહીંતર
        {
          સંરચના successor **p = &k->ટોચ;

          જ્યારે (*p)
            {
              જો ((*p)->suc == loop)
                {
                  જો (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      જ્યારે (loop)
                        {
                          સંરચના item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          જો (loop == k)
                            {
                              /* Remove relation.  */
                              સંરચના successor *s = *p;
                              s->suc->ગણતરી--;
                              *p = s->next;
                              IF_LINT (મુક્ત (s));
                              તોડો;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = ખાલી;
                          loop = tmp;
                        }

                      જ્યારે (loop)
                        {
                          સંરચના item *tmp = loop->qlink;

                          loop->qlink = ખાલી;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      પાછા true;
                    }
                  નહીંતર
                    {
                      k->qlink = loop;
                      loop = k;
                      તોડો;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  પાછા false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

સ્થિર bool
recurse_વૃક્ષ (સંરચના item *મૂળ, bool (*action) (સંરચના item *))
{
  જો (મૂળ->ડાબું == ખાલી && મૂળ->જમણું == ખાલી)
    પાછા (*action) (મૂળ);
  નહીંતર
    {
      જો (મૂળ->ડાબું != ખાલી)
        જો (recurse_વૃક્ષ (મૂળ->ડાબું, action))
          પાછા true;
      જો ((*action) (મૂળ))
        પાછા true;
      જો (મૂળ->જમણું != ખાલી)
        જો (recurse_વૃક્ષ (મૂળ->જમણું, action))
          પાછા true;
    }

  પાછા false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

સ્થિર શૂન્ય
walk_વૃક્ષ (સંરચના item *મૂળ, bool (*action) (સંરચના item *))
{
  જો (મૂળ->જમણું)
    recurse_વૃક્ષ (મૂળ->જમણું, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

સ્થિર _Noreturn શૂન્ય
tsort (અક્ષર અચલ *file)
{
  bool ok = true;
  સંરચના item *j = ખાલી;
  સંરચના item *k = ખાલી;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  સંરચના item *મૂળ = new_item (ખાલી);

  જો (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  જ્યારે (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, કદ (DELIM) - 1, &tokenbuffer);
      જો (len == (size_t) -1)
        {
          જો (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          તોડો;
        }

      affirm (len != 0);

      k = શોધ_item (મૂળ, tokenbuffer.buffer);
      જો (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = ખાલી;
        }

      j = k;
    }

  જો (k != ખાલી)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_વૃક્ષ (મૂળ, ગણતરી_વસ્તુઓ);

  જ્યારે (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_વૃક્ષ (મૂળ, scan_zeros);

      જ્યારે (head)
        {
          સંરચના successor *p = head->ટોચ;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          જ્યારે (p)
            {
              p->suc->ગણતરી--;
              જો (p->suc->ગણતરી == 0)
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
      જો (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          કરો
            walk_વૃક્ષ (મૂળ, detect_loop);
          જ્યારે (loop);
        }
    }

  જો (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે (true)
    {
      સ્થિર સંરચના option અચલ long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {ખાલી, 0, ખાલી, 0}
        };
      પૂર્ણાંક c = getopt_long (argc, argv, "w", long_options, ખાલી);

      જો (c == -1)
        તોડો;

      પસંદગી (c)
        {
        સ્થિતિ 'w':
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
