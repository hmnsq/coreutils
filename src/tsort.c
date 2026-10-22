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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "readtokens.h"
#അന്തര്ഭാവ "stdio--.h"
#അന്തര്ഭാവ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
ഘടന successor
{
  ഘടന item *suc;
  ഘടന successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
ഘടന item
{
  അക്ഷരം സ്ഥിരം *str;
  ഘടന item *ഇടത്, *വലത്;
  signed അക്ഷരം balance; /* -1, 0, or +1 */
  bool printed;
  size_t എണ്ണം;
  ഘടന item *qlink;
  ഘടന successor *മുകൾ;
};

/* The head of the sorted list.  */
സ്ഥിതം ഘടന item *head = ശൂന്യ;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
സ്ഥിതം ഘടന item *zeros = ശൂന്യ;

/* Used for loop detection.  */
സ്ഥിതം ഘടന item *loop = ശൂന്യ;

/* The number of strings to sort.  */
സ്ഥിതം size_t n_strings = 0;

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
സ്ഥിതം ഘടന item *
new_item (അക്ഷരം സ്ഥിരം *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  ഘടന item *k = xzalloc (വലുപ്പം *k);
  എങ്കിൽ (str)
    k->str = xstrdup (str);
  തിരികെ k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

സ്ഥിതം ഘടന item *
തിരയൽ_item (ഘടന item *വേര്, അക്ഷരം സ്ഥിരം *str)
{
  ഘടന item *p, *q, *r, *s, *t;
  പൂർണ്ണസംഖ്യ a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  എങ്കിൽ (വേര്->വലത് == ശൂന്യ)
    തിരികെ (വേര്->വലത് = new_item (str));

  /* A1. Initialize.  */
  t = വേര്;
  s = p = വേര്->വലത്;

  ആകുമ്പോൾ (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      എങ്കിൽ (a == 0)
        തിരികെ p;

      /* A3 & A4.  Move left & right.  */
      എങ്കിൽ (a < 0)
        q = p->ഇടത്;
      അല്ലെങ്കിൽ
        q = p->വലത്;

      എങ്കിൽ (q == ശൂന്യ)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          എങ്കിൽ (a < 0)
            p->ഇടത് = q;
          അല്ലെങ്കിൽ
            p->വലത് = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          എങ്കിൽ (a < 0)
            {
              r = p = s->ഇടത്;
              a = -1;
            }
          അല്ലെങ്കിൽ
            {
              affirm (0 < a);
              r = p = s->വലത്;
              a = 1;
            }

          ആകുമ്പോൾ (p != q)
            {
              പൂർണ്ണസംഖ്യ cmp = strcmp (str, p->str);
              എങ്കിൽ (cmp < 0)
                {
                  p->balance = -1;
                  p = p->ഇടത്;
                }
              അല്ലെങ്കിൽ
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->വലത്;
                }
            }

          /* A7. Balancing act.  */
          എങ്കിൽ (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              തിരികെ q;
            }

          എങ്കിൽ (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              എങ്കിൽ (a < 0)
                {
                  s->ഇടത് = r->വലത്;
                  r->വലത് = s;
                }
              അല്ലെങ്കിൽ
                {
                  s->വലത് = r->ഇടത്;
                  r->ഇടത് = s;
                }
              s->balance = r->balance = 0;
            }
          അല്ലെങ്കിൽ
            {
              /* A9. Double rotation.  */
              എങ്കിൽ (a < 0)
                {
                  p = r->വലത്;
                  r->വലത് = p->ഇടത്;
                  p->ഇടത് = r;
                  s->ഇടത് = p->വലത്;
                  p->വലത് = s;
                }
              അല്ലെങ്കിൽ
                {
                  p = r->ഇടത്;
                  r->ഇടത് = p->വലത്;
                  p->വലത് = r;
                  s->വലത് = p->ഇടത്;
                  p->ഇടത് = s;
                }

              s->balance = 0;
              r->balance = 0;
              എങ്കിൽ (p->balance == a)
                s->balance = -a;
              അല്ലെങ്കിൽ എങ്കിൽ (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          എങ്കിൽ (s == t->വലത്)
            t->വലത് = p;
          അല്ലെങ്കിൽ
            t->ഇടത് = p;

          തിരികെ q;
        }

      /* A3 & A4.  (continued).  */
      എങ്കിൽ (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

സ്ഥിതം ശൂന്യം
record_relation (ഘടന item *j, ഘടന item *k)
{
  ഘടന successor *p;

  എങ്കിൽ (!streq (j->str, k->str))
    {
      k->എണ്ണം++;
      p = xmalloc (വലുപ്പം *p);
      p->suc = k;
      p->next = j->മുകൾ;
      j->മുകൾ = p;
    }
}

സ്ഥിതം bool
എണ്ണം_ഇനങ്ങൾ (MAYBE_UNUSED ഘടന item *unused)
{
  n_strings++;
  തിരികെ false;
}

സ്ഥിതം bool
scan_zeros (ഘടന item *k)
{
  /* Ignore strings that have already been printed.  */
  എങ്കിൽ (k->എണ്ണം == 0 && !k->printed)
    {
      എങ്കിൽ (head == ശൂന്യ)
        head = k;
      അല്ലെങ്കിൽ
        zeros->qlink = k;

      zeros = k;
    }

  തിരികെ false;
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

സ്ഥിതം bool
detect_loop (ഘടന item *k)
{
  എങ്കിൽ (k->എണ്ണം > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      എങ്കിൽ (loop == ശൂന്യ)
        /* Start traversing the graph at K.  */
        loop = k;
      അല്ലെങ്കിൽ
        {
          ഘടന successor **p = &k->മുകൾ;

          ആകുമ്പോൾ (*p)
            {
              എങ്കിൽ ((*p)->suc == loop)
                {
                  എങ്കിൽ (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      ആകുമ്പോൾ (loop)
                        {
                          ഘടന item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          എങ്കിൽ (loop == k)
                            {
                              /* Remove relation.  */
                              ഘടന successor *s = *p;
                              s->suc->എണ്ണം--;
                              *p = s->next;
                              IF_LINT (സ്വതന്ത്രം (s));
                              നിർത്തുക;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = ശൂന്യ;
                          loop = tmp;
                        }

                      ആകുമ്പോൾ (loop)
                        {
                          ഘടന item *tmp = loop->qlink;

                          loop->qlink = ശൂന്യ;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      തിരികെ true;
                    }
                  അല്ലെങ്കിൽ
                    {
                      k->qlink = loop;
                      loop = k;
                      നിർത്തുക;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  തിരികെ false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

സ്ഥിതം bool
recurse_വൃക്ഷം (ഘടന item *വേര്, bool (*action) (ഘടന item *))
{
  എങ്കിൽ (വേര്->ഇടത് == ശൂന്യ && വേര്->വലത് == ശൂന്യ)
    തിരികെ (*action) (വേര്);
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (വേര്->ഇടത് != ശൂന്യ)
        എങ്കിൽ (recurse_വൃക്ഷം (വേര്->ഇടത്, action))
          തിരികെ true;
      എങ്കിൽ ((*action) (വേര്))
        തിരികെ true;
      എങ്കിൽ (വേര്->വലത് != ശൂന്യ)
        എങ്കിൽ (recurse_വൃക്ഷം (വേര്->വലത്, action))
          തിരികെ true;
    }

  തിരികെ false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

സ്ഥിതം ശൂന്യം
walk_വൃക്ഷം (ഘടന item *വേര്, bool (*action) (ഘടന item *))
{
  എങ്കിൽ (വേര്->വലത്)
    recurse_വൃക്ഷം (വേര്->വലത്, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

സ്ഥിതം _Noreturn ശൂന്യം
tsort (അക്ഷരം സ്ഥിരം *file)
{
  bool ok = true;
  ഘടന item *j = ശൂന്യ;
  ഘടന item *k = ശൂന്യ;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  ഘടന item *വേര് = new_item (ശൂന്യ);

  എങ്കിൽ (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  ആകുമ്പോൾ (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, വലുപ്പം (DELIM) - 1, &tokenbuffer);
      എങ്കിൽ (len == (size_t) -1)
        {
          എങ്കിൽ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          നിർത്തുക;
        }

      affirm (len != 0);

      k = തിരയൽ_item (വേര്, tokenbuffer.buffer);
      എങ്കിൽ (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = ശൂന്യ;
        }

      j = k;
    }

  എങ്കിൽ (k != ശൂന്യ)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_വൃക്ഷം (വേര്, എണ്ണം_ഇനങ്ങൾ);

  ആകുമ്പോൾ (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_വൃക്ഷം (വേര്, scan_zeros);

      ആകുമ്പോൾ (head)
        {
          ഘടന successor *p = head->മുകൾ;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          ആകുമ്പോൾ (p)
            {
              p->suc->എണ്ണം--;
              എങ്കിൽ (p->suc->എണ്ണം == 0)
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
      എങ്കിൽ (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          ചെയ്യുക
            walk_വൃക്ഷം (വേര്, detect_loop);
          ആകുമ്പോൾ (loop);
        }
    }

  എങ്കിൽ (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ (true)
    {
      സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {ശൂന്യ, 0, ശൂന്യ, 0}
        };
      പൂർണ്ണസംഖ്യ c = getopt_long (argc, argv, "w", long_options, ശൂന്യ);

      എങ്കിൽ (c == -1)
        നിർത്തുക;

      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'w':
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
