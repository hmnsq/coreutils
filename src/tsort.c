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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "readtokens.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
ಸಂರಚನಾ successor
{
  ಸಂರಚನಾ item *suc;
  ಸಂರಚನಾ successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
ಸಂರಚನಾ item
{
  ವರ್ಣ ಧ್ರುವ *str;
  ಸಂರಚನಾ item *ಎಡ, *ಬಲ;
  signed ವರ್ಣ balance; /* -1, 0, or +1 */
  bool printed;
  size_t ಎಣಿಕೆ;
  ಸಂರಚನಾ item *qlink;
  ಸಂರಚನಾ successor *ತುದಿ;
};

/* The head of the sorted list.  */
ಸ್ಥಿರ ಸಂರಚನಾ item *head = ರಿಕ್ತ;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ಸ್ಥಿರ ಸಂರಚನಾ item *zeros = ರಿಕ್ತ;

/* Used for loop detection.  */
ಸ್ಥಿರ ಸಂರಚನಾ item *loop = ರಿಕ್ತ;

/* The number of strings to sort.  */
ಸ್ಥಿರ size_t n_strings = 0;

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
ಸ್ಥಿರ ಸಂರಚನಾ item *
new_item (ವರ್ಣ ಧ್ರುವ *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  ಸಂರಚನಾ item *k = xzalloc (ಪರಿಮಾಣ *k);
  ಯದಿ (str)
    k->str = xstrdup (str);
  ಪ್ರತ್ಯಾಗಮ k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

ಸ್ಥಿರ ಸಂರಚನಾ item *
ಹುಡುಕು_item (ಸಂರಚನಾ item *ಬೇರು, ವರ್ಣ ಧ್ರುವ *str)
{
  ಸಂರಚನಾ item *p, *q, *r, *s, *t;
  ಪೂರ್ಣಾಙ್ಕ a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  ಯದಿ (ಬೇರು->ಬಲ == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ (ಬೇರು->ಬಲ = new_item (str));

  /* A1. Initialize.  */
  t = ಬೇರು;
  s = p = ಬೇರು->ಬಲ;

  ಯಾವತ್ (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      ಯದಿ (a == 0)
        ಪ್ರತ್ಯಾಗಮ p;

      /* A3 & A4.  Move left & right.  */
      ಯದಿ (a < 0)
        q = p->ಎಡ;
      ಅನ್ಯಥಾ
        q = p->ಬಲ;

      ಯದಿ (q == ರಿಕ್ತ)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          ಯದಿ (a < 0)
            p->ಎಡ = q;
          ಅನ್ಯಥಾ
            p->ಬಲ = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          ಯದಿ (a < 0)
            {
              r = p = s->ಎಡ;
              a = -1;
            }
          ಅನ್ಯಥಾ
            {
              affirm (0 < a);
              r = p = s->ಬಲ;
              a = 1;
            }

          ಯಾವತ್ (p != q)
            {
              ಪೂರ್ಣಾಙ್ಕ cmp = strcmp (str, p->str);
              ಯದಿ (cmp < 0)
                {
                  p->balance = -1;
                  p = p->ಎಡ;
                }
              ಅನ್ಯಥಾ
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->ಬಲ;
                }
            }

          /* A7. Balancing act.  */
          ಯದಿ (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              ಪ್ರತ್ಯಾಗಮ q;
            }

          ಯದಿ (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              ಯದಿ (a < 0)
                {
                  s->ಎಡ = r->ಬಲ;
                  r->ಬಲ = s;
                }
              ಅನ್ಯಥಾ
                {
                  s->ಬಲ = r->ಎಡ;
                  r->ಎಡ = s;
                }
              s->balance = r->balance = 0;
            }
          ಅನ್ಯಥಾ
            {
              /* A9. Double rotation.  */
              ಯದಿ (a < 0)
                {
                  p = r->ಬಲ;
                  r->ಬಲ = p->ಎಡ;
                  p->ಎಡ = r;
                  s->ಎಡ = p->ಬಲ;
                  p->ಬಲ = s;
                }
              ಅನ್ಯಥಾ
                {
                  p = r->ಎಡ;
                  r->ಎಡ = p->ಬಲ;
                  p->ಬಲ = r;
                  s->ಬಲ = p->ಎಡ;
                  p->ಎಡ = s;
                }

              s->balance = 0;
              r->balance = 0;
              ಯದಿ (p->balance == a)
                s->balance = -a;
              ಅನ್ಯಥಾ ಯದಿ (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          ಯದಿ (s == t->ಬಲ)
            t->ಬಲ = p;
          ಅನ್ಯಥಾ
            t->ಎಡ = p;

          ಪ್ರತ್ಯಾಗಮ q;
        }

      /* A3 & A4.  (continued).  */
      ಯದಿ (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

ಸ್ಥಿರ ಶೂನ್ಯ
record_relation (ಸಂರಚನಾ item *j, ಸಂರಚನಾ item *k)
{
  ಸಂರಚನಾ successor *p;

  ಯದಿ (!streq (j->str, k->str))
    {
      k->ಎಣಿಕೆ++;
      p = xmalloc (ಪರಿಮಾಣ *p);
      p->suc = k;
      p->next = j->ತುದಿ;
      j->ತುದಿ = p;
    }
}

ಸ್ಥಿರ bool
ಎಣಿಕೆ_ವಸ್ತುಗಳು (MAYBE_UNUSED ಸಂರಚನಾ item *unused)
{
  n_strings++;
  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
scan_zeros (ಸಂರಚನಾ item *k)
{
  /* Ignore strings that have already been printed.  */
  ಯದಿ (k->ಎಣಿಕೆ == 0 && !k->printed)
    {
      ಯದಿ (head == ರಿಕ್ತ)
        head = k;
      ಅನ್ಯಥಾ
        zeros->qlink = k;

      zeros = k;
    }

  ಪ್ರತ್ಯಾಗಮ false;
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

ಸ್ಥಿರ bool
detect_loop (ಸಂರಚನಾ item *k)
{
  ಯದಿ (k->ಎಣಿಕೆ > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      ಯದಿ (loop == ರಿಕ್ತ)
        /* Start traversing the graph at K.  */
        loop = k;
      ಅನ್ಯಥಾ
        {
          ಸಂರಚನಾ successor **p = &k->ತುದಿ;

          ಯಾವತ್ (*p)
            {
              ಯದಿ ((*p)->suc == loop)
                {
                  ಯದಿ (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      ಯಾವತ್ (loop)
                        {
                          ಸಂರಚನಾ item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          ಯದಿ (loop == k)
                            {
                              /* Remove relation.  */
                              ಸಂರಚನಾ successor *s = *p;
                              s->suc->ಎಣಿಕೆ--;
                              *p = s->next;
                              IF_LINT (ಮುಕ್ತ (s));
                              ಭಙ್ಗ;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = ರಿಕ್ತ;
                          loop = tmp;
                        }

                      ಯಾವತ್ (loop)
                        {
                          ಸಂರಚನಾ item *tmp = loop->qlink;

                          loop->qlink = ರಿಕ್ತ;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      ಪ್ರತ್ಯಾಗಮ true;
                    }
                  ಅನ್ಯಥಾ
                    {
                      k->qlink = loop;
                      loop = k;
                      ಭಙ್ಗ;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  ಪ್ರತ್ಯಾಗಮ false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ಸ್ಥಿರ bool
recurse_ವೃಕ್ಷ (ಸಂರಚನಾ item *ಬೇರು, bool (*action) (ಸಂರಚನಾ item *))
{
  ಯದಿ (ಬೇರು->ಎಡ == ರಿಕ್ತ && ಬೇರು->ಬಲ == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ (*action) (ಬೇರು);
  ಅನ್ಯಥಾ
    {
      ಯದಿ (ಬೇರು->ಎಡ != ರಿಕ್ತ)
        ಯದಿ (recurse_ವೃಕ್ಷ (ಬೇರು->ಎಡ, action))
          ಪ್ರತ್ಯಾಗಮ true;
      ಯದಿ ((*action) (ಬೇರು))
        ಪ್ರತ್ಯಾಗಮ true;
      ಯದಿ (ಬೇರು->ಬಲ != ರಿಕ್ತ)
        ಯದಿ (recurse_ವೃಕ್ಷ (ಬೇರು->ಬಲ, action))
          ಪ್ರತ್ಯಾಗಮ true;
    }

  ಪ್ರತ್ಯಾಗಮ false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ಸ್ಥಿರ ಶೂನ್ಯ
walk_ವೃಕ್ಷ (ಸಂರಚನಾ item *ಬೇರು, bool (*action) (ಸಂರಚನಾ item *))
{
  ಯದಿ (ಬೇರು->ಬಲ)
    recurse_ವೃಕ್ಷ (ಬೇರು->ಬಲ, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
tsort (ವರ್ಣ ಧ್ರುವ *file)
{
  bool ok = true;
  ಸಂರಚನಾ item *j = ರಿಕ್ತ;
  ಸಂರಚನಾ item *k = ರಿಕ್ತ;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  ಸಂರಚನಾ item *ಬೇರು = new_item (ರಿಕ್ತ);

  ಯದಿ (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  ಯಾವತ್ (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, ಪರಿಮಾಣ (DELIM) - 1, &tokenbuffer);
      ಯದಿ (len == (size_t) -1)
        {
          ಯದಿ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          ಭಙ್ಗ;
        }

      affirm (len != 0);

      k = ಹುಡುಕು_item (ಬೇರು, tokenbuffer.buffer);
      ಯದಿ (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = ರಿಕ್ತ;
        }

      j = k;
    }

  ಯದಿ (k != ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_ವೃಕ್ಷ (ಬೇರು, ಎಣಿಕೆ_ವಸ್ತುಗಳು);

  ಯಾವತ್ (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_ವೃಕ್ಷ (ಬೇರು, scan_zeros);

      ಯಾವತ್ (head)
        {
          ಸಂರಚನಾ successor *p = head->ತುದಿ;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          ಯಾವತ್ (p)
            {
              p->suc->ಎಣಿಕೆ--;
              ಯದಿ (p->suc->ಎಣಿಕೆ == 0)
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
      ಯದಿ (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          ಕುರು
            walk_ವೃಕ್ಷ (ಬೇರು, detect_loop);
          ಯಾವತ್ (loop);
        }
    }

  ಯದಿ (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ (true)
    {
      ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
        };
      ಪೂರ್ಣಾಙ್ಕ c = getopt_long (argc, argv, "w", long_options, ರಿಕ್ತ);

      ಯದಿ (c == -1)
        ಭಙ್ಗ;

      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'w':
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
