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

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "readtokens.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
संरचना successor
{
  संरचना item *suc;
  संरचना successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
संरचना item
{
  वर्ण ध्रुव *str;
  संरचना item *वामः, *दक्षिणः;
  signed वर्ण balance; /* -1, 0, or +1 */
  bool printed;
  size_t गणनम्;
  संरचना item *qlink;
  संरचना successor *शिखरम्;
};

/* The head of the sorted list.  */
स्थिर संरचना item *head = रिक्त;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
स्थिर संरचना item *zeros = रिक्त;

/* Used for loop detection.  */
स्थिर संरचना item *loop = रिक्त;

/* The number of strings to sort.  */
स्थिर size_t n_strings = 0;

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
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
स्थिर संरचना item *
new_item (वर्ण ध्रुव *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  संरचना item *k = xzalloc (परिमाण *k);
  जइ (str)
    k->str = xstrdup (str);
  प्रत्यागम k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

स्थिर संरचना item *
अन्वेषणम्_item (संरचना item *मूलम्, वर्ण ध्रुव *str)
{
  संरचना item *p, *q, *r, *s, *t;
  पूर्णाङ्क a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  जइ (मूलम्->दक्षिणः == रिक्त)
    प्रत्यागम (मूलम्->दक्षिणः = new_item (str));

  /* A1. Initialize.  */
  t = मूलम्;
  s = p = मूलम्->दक्षिणः;

  जाव (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      जइ (a == 0)
        प्रत्यागम p;

      /* A3 & A4.  Move left & right.  */
      जइ (a < 0)
        q = p->वामः;
      अण्णहा
        q = p->दक्षिणः;

      जइ (q == रिक्त)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          जइ (a < 0)
            p->वामः = q;
          अण्णहा
            p->दक्षिणः = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          जइ (a < 0)
            {
              r = p = s->वामः;
              a = -1;
            }
          अण्णहा
            {
              affirm (0 < a);
              r = p = s->दक्षिणः;
              a = 1;
            }

          जाव (p != q)
            {
              पूर्णाङ्क cmp = strcmp (str, p->str);
              जइ (cmp < 0)
                {
                  p->balance = -1;
                  p = p->वामः;
                }
              अण्णहा
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->दक्षिणः;
                }
            }

          /* A7. Balancing act.  */
          जइ (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              प्रत्यागम q;
            }

          जइ (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              जइ (a < 0)
                {
                  s->वामः = r->दक्षिणः;
                  r->दक्षिणः = s;
                }
              अण्णहा
                {
                  s->दक्षिणः = r->वामः;
                  r->वामः = s;
                }
              s->balance = r->balance = 0;
            }
          अण्णहा
            {
              /* A9. Double rotation.  */
              जइ (a < 0)
                {
                  p = r->दक्षिणः;
                  r->दक्षिणः = p->वामः;
                  p->वामः = r;
                  s->वामः = p->दक्षिणः;
                  p->दक्षिणः = s;
                }
              अण्णहा
                {
                  p = r->वामः;
                  r->वामः = p->दक्षिणः;
                  p->दक्षिणः = r;
                  s->दक्षिणः = p->वामः;
                  p->वामः = s;
                }

              s->balance = 0;
              r->balance = 0;
              जइ (p->balance == a)
                s->balance = -a;
              अण्णहा जइ (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          जइ (s == t->दक्षिणः)
            t->दक्षिणः = p;
          अण्णहा
            t->वामः = p;

          प्रत्यागम q;
        }

      /* A3 & A4.  (continued).  */
      जइ (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

स्थिर शून्य
record_relation (संरचना item *j, संरचना item *k)
{
  संरचना successor *p;

  जइ (!streq (j->str, k->str))
    {
      k->गणनम्++;
      p = xmalloc (परिमाण *p);
      p->suc = k;
      p->next = j->शिखरम्;
      j->शिखरम् = p;
    }
}

स्थिर bool
गणनम्_वस्तूनि (MAYBE_UNUSED संरचना item *unused)
{
  n_strings++;
  प्रत्यागम false;
}

स्थिर bool
scan_zeros (संरचना item *k)
{
  /* Ignore strings that have already been printed.  */
  जइ (k->गणनम् == 0 && !k->printed)
    {
      जइ (head == रिक्त)
        head = k;
      अण्णहा
        zeros->qlink = k;

      zeros = k;
    }

  प्रत्यागम false;
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

स्थिर bool
detect_loop (संरचना item *k)
{
  जइ (k->गणनम् > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      जइ (loop == रिक्त)
        /* Start traversing the graph at K.  */
        loop = k;
      अण्णहा
        {
          संरचना successor **p = &k->शिखरम्;

          जाव (*p)
            {
              जइ ((*p)->suc == loop)
                {
                  जइ (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      जाव (loop)
                        {
                          संरचना item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          जइ (loop == k)
                            {
                              /* Remove relation.  */
                              संरचना successor *s = *p;
                              s->suc->गणनम्--;
                              *p = s->next;
                              IF_LINT (मुक्त (s));
                              भङ्ग;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = रिक्त;
                          loop = tmp;
                        }

                      जाव (loop)
                        {
                          संरचना item *tmp = loop->qlink;

                          loop->qlink = रिक्त;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      प्रत्यागम true;
                    }
                  अण्णहा
                    {
                      k->qlink = loop;
                      loop = k;
                      भङ्ग;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  प्रत्यागम false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

स्थिर bool
recurse_वृक्षः (संरचना item *मूलम्, bool (*action) (संरचना item *))
{
  जइ (मूलम्->वामः == रिक्त && मूलम्->दक्षिणः == रिक्त)
    प्रत्यागम (*action) (मूलम्);
  अण्णहा
    {
      जइ (मूलम्->वामः != रिक्त)
        जइ (recurse_वृक्षः (मूलम्->वामः, action))
          प्रत्यागम true;
      जइ ((*action) (मूलम्))
        प्रत्यागम true;
      जइ (मूलम्->दक्षिणः != रिक्त)
        जइ (recurse_वृक्षः (मूलम्->दक्षिणः, action))
          प्रत्यागम true;
    }

  प्रत्यागम false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

स्थिर शून्य
walk_वृक्षः (संरचना item *मूलम्, bool (*action) (संरचना item *))
{
  जइ (मूलम्->दक्षिणः)
    recurse_वृक्षः (मूलम्->दक्षिणः, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

स्थिर _Noreturn शून्य
tsort (वर्ण ध्रुव *file)
{
  bool ok = true;
  संरचना item *j = रिक्त;
  संरचना item *k = रिक्त;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  संरचना item *मूलम् = new_item (रिक्त);

  जइ (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  जाव (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, परिमाण (DELIM) - 1, &tokenbuffer);
      जइ (len == (size_t) -1)
        {
          जइ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          भङ्ग;
        }

      affirm (len != 0);

      k = अन्वेषणम्_item (मूलम्, tokenbuffer.buffer);
      जइ (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = रिक्त;
        }

      j = k;
    }

  जइ (k != रिक्त)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_वृक्षः (मूलम्, गणनम्_वस्तूनि);

  जाव (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_वृक्षः (मूलम्, scan_zeros);

      जाव (head)
        {
          संरचना successor *p = head->शिखरम्;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          जाव (p)
            {
              p->suc->गणनम्--;
              जइ (p->suc->गणनम् == 0)
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
      जइ (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          करेहि
            walk_वृक्षः (मूलम्, detect_loop);
          जाव (loop);
        }
    }

  जइ (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  जाव (true)
    {
      स्थिर संरचना option ध्रुव long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {रिक्त, 0, रिक्त, 0}
        };
      पूर्णाङ्क c = getopt_long (argc, argv, "w", long_options, रिक्त);

      जइ (c == -1)
        भङ्ग;

      विकल्प (c)
        {
        पक्ष 'w':
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  जइ (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
