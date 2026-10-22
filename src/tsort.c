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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "tsort"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#ವ್ಯಾಖ್ಯಾನಿಸು DELIM " \t\n"

/* Members of the list of successors.  */
ರಚನೆ successor
{
  ರಚನೆ ಅಂಶ *suc;
  ರಚನೆ successor *ಮುಂದಿನ;
};

/* Each string is held in memory as the head of a list of successors.  */
ರಚನೆ ಅಂಶ
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str;
  ರಚನೆ ಅಂಶ *ಎಡ, *ಬಲ;
  ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ balance; /* -1, 0, or +1 */
  bool printed;
  size_t ಎಣಿಕೆ_2;
  ರಚನೆ ಅಂಶ *qlink;
  ರಚನೆ successor *ತುದಿ;
};

/* The head of the sorted list.  */
ಸ್ಥಿರ ರಚನೆ ಅಂಶ *ತಲೆ = ಖಾಲಿ;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ಸ್ಥಿರ ರಚನೆ ಅಂಶ *zeros = ಖಾಲಿ;

/* Used for loop detection.  */
ಸ್ಥಿರ ರಚನೆ ಅಂಶ *loop = ಖಾಲಿ;

/* The number of strings to sort.  */
ಸ್ಥಿರ size_t n_ಅಕ್ಷರಸರಣಿಗಳು = 0;

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION] [FILE]\n\
Write totally ordered list consistent with the partial ordering ಒಳ FILE.\n\
"), ಕಾರ್ಯಕ್ರಮ_name);

      emit_stdin_note ();

      fputs (_("\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }

  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Create a new item/node for STR.  */
ಸ್ಥಿರ ರಚನೆ ಅಂಶ *
new_ಅಂಶ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  ರಚನೆ ಅಂಶ *k = xzalloc (ಗಾತ್ರ *k);
  ಆದರೆ (str)
    k->str = xstrdup (str);
  ಹಿಂತಿರುಗಿ k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

ಸ್ಥಿರ ರಚನೆ ಅಂಶ *
ಹುಡುಕು_ಅಂಶ (ರಚನೆ ಅಂಶ *ಬೇರು, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  ರಚನೆ ಅಂಶ *p, *q, *r, *s, *t;
  ಪೂರ್ಣಾಂಕ a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  ಆದರೆ (ಬೇರು->ಬಲ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ (ಬೇರು->ಬಲ = new_ಅಂಶ (str));

  /* A1. Initialize.  */
  t = ಬೇರು;
  s = p = ಬೇರು->ಬಲ;

  ಆಗ (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      ಆದರೆ (a == 0)
        ಹಿಂತಿರುಗಿ p;

      /* A3 & A4.  Move left & right.  */
      ಆದರೆ (a < 0)
        q = p->ಎಡ;
      ಇಲ್ಲದಿದ್ದರೆ
        q = p->ಬಲ;

      ಆದರೆ (q == ಖಾಲಿ)
        {
          /* A5. Insert.  */
          q = new_ಅಂಶ (str);

          /* A3 & A4.  (continued).  */
          ಆದರೆ (a < 0)
            p->ಎಡ = q;
          ಇಲ್ಲದಿದ್ದರೆ
            p->ಬಲ = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          ಆದರೆ (a < 0)
            {
              r = p = s->ಎಡ;
              a = -1;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              affirm (0 < a);
              r = p = s->ಬಲ;
              a = 1;
            }

          ಆಗ (p != q)
            {
              ಪೂರ್ಣಾಂಕ cmp = strcmp (str, p->str);
              ಆದರೆ (cmp < 0)
                {
                  p->balance = -1;
                  p = p->ಎಡ;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->ಬಲ;
                }
            }

          /* A7. Balancing act.  */
          ಆದರೆ (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              ಹಿಂತಿರುಗಿ q;
            }

          ಆದರೆ (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              ಆದರೆ (a < 0)
                {
                  s->ಎಡ = r->ಬಲ;
                  r->ಬಲ = s;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  s->ಬಲ = r->ಎಡ;
                  r->ಎಡ = s;
                }
              s->balance = r->balance = 0;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* A9. Double rotation.  */
              ಆದರೆ (a < 0)
                {
                  p = r->ಬಲ;
                  r->ಬಲ = p->ಎಡ;
                  p->ಎಡ = r;
                  s->ಎಡ = p->ಬಲ;
                  p->ಬಲ = s;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  p = r->ಎಡ;
                  r->ಎಡ = p->ಬಲ;
                  p->ಬಲ = r;
                  s->ಬಲ = p->ಎಡ;
                  p->ಎಡ = s;
                }

              s->balance = 0;
              r->balance = 0;
              ಆದರೆ (p->balance == a)
                s->balance = -a;
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          ಆದರೆ (s == t->ಬಲ)
            t->ಬಲ = p;
          ಇಲ್ಲದಿದ್ದರೆ
            t->ಎಡ = p;

          ಹಿಂತಿರುಗಿ q;
        }

      /* A3 & A4.  (continued).  */
      ಆದರೆ (q->balance)
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
ದಾಖಲೆ_ಸಂಬಂಧ (ರಚನೆ ಅಂಶ *j, ರಚನೆ ಅಂಶ *k)
{
  ರಚನೆ successor *p;

  ಆದರೆ (!streq (j->str, k->str))
    {
      k->ಎಣಿಕೆ_2++;
      p = xmalloc (ಗಾತ್ರ *p);
      p->suc = k;
      p->ಮುಂದಿನ = j->ತುದಿ;
      j->ತುದಿ = p;
    }
}

ಸ್ಥಿರ bool
ಎಣಿಕೆ_ವಸ್ತುಗಳು (MAYBE_ಬಳಸದ ರಚನೆ ಅಂಶ *ಬಳಸದ)
{
  n_ಅಕ್ಷರಸರಣಿಗಳು++;
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
ಪರಿಶೀಲನೆ_zeros (ರಚನೆ ಅಂಶ *k)
{
  /* Ignore strings that have already been printed.  */
  ಆದರೆ (k->ಎಣಿಕೆ_2 == 0 && !k->printed)
    {
      ಆದರೆ (ತಲೆ == ಖಾಲಿ)
        ತಲೆ = k;
      ಇಲ್ಲದಿದ್ದರೆ
        zeros->qlink = k;

      zeros = k;
    }

  ಹಿಂತಿರುಗಿ false;
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
detect_loop (ರಚನೆ ಅಂಶ *k)
{
  ಆದರೆ (k->ಎಣಿಕೆ_2 > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      ಆದರೆ (loop == ಖಾಲಿ)
        /* Start traversing the graph at K.  */
        loop = k;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ರಚನೆ successor **p = &k->ತುದಿ;

          ಆಗ (*p)
            {
              ಆದರೆ ((*p)->suc == loop)
                {
                  ಆದರೆ (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      ಆಗ (loop)
                        {
                          ರಚನೆ ಅಂಶ *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          ಆದರೆ (loop == k)
                            {
                              /* Remove relation.  */
                              ರಚನೆ successor *s = *p;
                              s->suc->ಎಣಿಕೆ_2--;
                              *p = s->ಮುಂದಿನ;
                              IF_LINT (ಬಿಡುಗಡೆ (s));
                              ನಿಲ್ಲಿಸು;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = ಖಾಲಿ;
                          loop = tmp;
                        }

                      ಆಗ (loop)
                        {
                          ರಚನೆ ಅಂಶ *tmp = loop->qlink;

                          loop->qlink = ಖಾಲಿ;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      ಹಿಂತಿರುಗಿ true;
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    {
                      k->qlink = loop;
                      loop = k;
                      ನಿಲ್ಲಿಸು;
                    }
                }

              p = &(*p)->ಮುಂದಿನ;
            }
        }
    }

  ಹಿಂತಿರುಗಿ false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ಸ್ಥಿರ bool
recurse_ವೃಕ್ಷ (ರಚನೆ ಅಂಶ *ಬೇರು, bool (*action) (ರಚನೆ ಅಂಶ *))
{
  ಆದರೆ (ಬೇರು->ಎಡ == ಖಾಲಿ && ಬೇರು->ಬಲ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ (*action) (ಬೇರು);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (ಬೇರು->ಎಡ != ಖಾಲಿ)
        ಆದರೆ (recurse_ವೃಕ್ಷ (ಬೇರು->ಎಡ, action))
          ಹಿಂತಿರುಗಿ true;
      ಆದರೆ ((*action) (ಬೇರು))
        ಹಿಂತಿರುಗಿ true;
      ಆದರೆ (ಬೇರು->ಬಲ != ಖಾಲಿ)
        ಆದರೆ (recurse_ವೃಕ್ಷ (ಬೇರು->ಬಲ, action))
          ಹಿಂತಿರುಗಿ true;
    }

  ಹಿಂತಿರುಗಿ false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ಸ್ಥಿರ ಶೂನ್ಯ
walk_ವೃಕ್ಷ (ರಚನೆ ಅಂಶ *ಬೇರು, bool (*action) (ರಚನೆ ಅಂಶ *))
{
  ಆದರೆ (ಬೇರು->ಬಲ)
    recurse_ವೃಕ್ಷ (ಬೇರು->ಬಲ, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
tsort (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  bool ok = true;
  ರಚನೆ ಅಂಶ *j = ಖಾಲಿ;
  ರಚನೆ ಅಂಶ *k = ಖಾಲಿ;
  token_ಬಫರ್ tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  ರಚನೆ ಅಂಶ *ಬೇರು = new_ಅಂಶ (ಖಾಲಿ);

  ಆದರೆ (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  ಆಗ (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, ಗಾತ್ರ (DELIM) - 1, &tokenbuffer);
      ಆದರೆ (len == (size_t) -1)
        {
          ಆದರೆ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          ನಿಲ್ಲಿಸು;
        }

      affirm (len != 0);

      k = ಹುಡುಕು_ಅಂಶ (ಬೇರು, tokenbuffer.ಬಫರ್);
      ಆದರೆ (j)
        {
          /* T3. Record the relation.  */
          ದಾಖಲೆ_ಸಂಬಂಧ (j, k);
          k = ಖಾಲಿ;
        }

      j = k;
    }

  ಆದರೆ (k != ಖಾಲಿ)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_ವೃಕ್ಷ (ಬೇರು, ಎಣಿಕೆ_ವಸ್ತುಗಳು);

  ಆಗ (n_ಅಕ್ಷರಸರಣಿಗಳು > 0)
    {
      /* T4. Scan for zeros.  */
      walk_ವೃಕ್ಷ (ಬೇರು, ಪರಿಶೀಲನೆ_zeros);

      ಆಗ (ತಲೆ)
        {
          ರಚನೆ successor *p = ತಲೆ->ತುದಿ;

          /* T5. Output front of queue.  */
          puts (ತಲೆ->str);
          ತಲೆ->printed = true;
          n_ಅಕ್ಷರಸರಣಿಗಳು--;

          /* T6. Erase relations.  */
          ಆಗ (p)
            {
              p->suc->ಎಣಿಕೆ_2--;
              ಆದರೆ (p->suc->ಎಣಿಕೆ_2 == 0)
                {
                  zeros->qlink = p->suc;
                  zeros = p->suc;
                }

              p = p->ಮುಂದಿನ;
            }

          /* T7. Remove from queue.  */
          ತಲೆ = ತಲೆ->qlink;
        }

      /* T8.  End of process.  */
      ಆದರೆ (n_ಅಕ್ಷರಸರಣಿಗಳು > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          ಮಾಡು
            walk_ವೃಕ್ಷ (ಬೇರು, detect_loop);
          ಆಗ (loop);
        }
    }

  ಆದರೆ (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  ನಿರ್ಗಮಿಸು (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ (true)
    {
      ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
          {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
        };
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "w", long_options, ಖಾಲಿ);

      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'w':
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
