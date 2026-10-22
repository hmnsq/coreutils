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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "assure.h"
#অন্তর্ভাৱ "fadvise.h"
#অন্তর্ভাৱ "readtokens.h"
#অন্তর্ভাৱ "stdio--.h"
#অন্তর্ভাৱ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
সংরচনা successor
{
  সংরচনা item *suc;
  সংরচনা successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
সংরচনা item
{
  ৱর্ণ ধ্রুৱ *str;
  সংরচনা item *বাম, *ডান;
  signed ৱর্ণ balance; /* -1, 0, or +1 */
  bool printed;
  size_t গুনতি;
  সংরচনা item *qlink;
  সংরচনা successor *শীৰ্ষ;
};

/* The head of the sorted list.  */
স্থির সংরচনা item *head = রিক্ত;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
স্থির সংরচনা item *zeros = রিক্ত;

/* Used for loop detection.  */
স্থির সংরচনা item *loop = রিক্ত;

/* The number of strings to sort.  */
স্থির size_t n_strings = 0;

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
স্থির সংরচনা item *
new_item (ৱর্ণ ধ্রুৱ *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  সংরচনা item *k = xzalloc (পরিমাণ *k);
  যদি (str)
    k->str = xstrdup (str);
  ঘূৰাই k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

স্থির সংরচনা item *
অনুসন্ধান_item (সংরচনা item *মূল, ৱর্ণ ধ্রুৱ *str)
{
  সংরচনা item *p, *q, *r, *s, *t;
  পূর্ণাঙ্ক a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  যদি (মূল->ডান == রিক্ত)
    ঘূৰাই (মূল->ডান = new_item (str));

  /* A1. Initialize.  */
  t = মূল;
  s = p = মূল->ডান;

  যেতিয়ালৈ (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      যদি (a == 0)
        ঘূৰাই p;

      /* A3 & A4.  Move left & right.  */
      যদি (a < 0)
        q = p->বাম;
      অন্যথা
        q = p->ডান;

      যদি (q == রিক্ত)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          যদি (a < 0)
            p->বাম = q;
          অন্যথা
            p->ডান = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          যদি (a < 0)
            {
              r = p = s->বাম;
              a = -1;
            }
          অন্যথা
            {
              affirm (0 < a);
              r = p = s->ডান;
              a = 1;
            }

          যেতিয়ালৈ (p != q)
            {
              পূর্ণাঙ্ক cmp = strcmp (str, p->str);
              যদি (cmp < 0)
                {
                  p->balance = -1;
                  p = p->বাম;
                }
              অন্যথা
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->ডান;
                }
            }

          /* A7. Balancing act.  */
          যদি (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              ঘূৰাই q;
            }

          যদি (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              যদি (a < 0)
                {
                  s->বাম = r->ডান;
                  r->ডান = s;
                }
              অন্যথা
                {
                  s->ডান = r->বাম;
                  r->বাম = s;
                }
              s->balance = r->balance = 0;
            }
          অন্যথা
            {
              /* A9. Double rotation.  */
              যদি (a < 0)
                {
                  p = r->ডান;
                  r->ডান = p->বাম;
                  p->বাম = r;
                  s->বাম = p->ডান;
                  p->ডান = s;
                }
              অন্যথা
                {
                  p = r->বাম;
                  r->বাম = p->ডান;
                  p->ডান = r;
                  s->ডান = p->বাম;
                  p->বাম = s;
                }

              s->balance = 0;
              r->balance = 0;
              যদি (p->balance == a)
                s->balance = -a;
              অন্যথা যদি (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          যদি (s == t->ডান)
            t->ডান = p;
          অন্যথা
            t->বাম = p;

          ঘূৰাই q;
        }

      /* A3 & A4.  (continued).  */
      যদি (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

স্থির শূন্য
record_relation (সংরচনা item *j, সংরচনা item *k)
{
  সংরচনা successor *p;

  যদি (!streq (j->str, k->str))
    {
      k->গুনতি++;
      p = xmalloc (পরিমাণ *p);
      p->suc = k;
      p->next = j->শীৰ্ষ;
      j->শীৰ্ষ = p;
    }
}

স্থির bool
গুনতি_বস্তুসমূহ (MAYBE_UNUSED সংরচনা item *unused)
{
  n_strings++;
  ঘূৰাই false;
}

স্থির bool
scan_zeros (সংরচনা item *k)
{
  /* Ignore strings that have already been printed.  */
  যদি (k->গুনতি == 0 && !k->printed)
    {
      যদি (head == রিক্ত)
        head = k;
      অন্যথা
        zeros->qlink = k;

      zeros = k;
    }

  ঘূৰাই false;
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

স্থির bool
detect_loop (সংরচনা item *k)
{
  যদি (k->গুনতি > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      যদি (loop == রিক্ত)
        /* Start traversing the graph at K.  */
        loop = k;
      অন্যথা
        {
          সংরচনা successor **p = &k->শীৰ্ষ;

          যেতিয়ালৈ (*p)
            {
              যদি ((*p)->suc == loop)
                {
                  যদি (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      যেতিয়ালৈ (loop)
                        {
                          সংরচনা item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          যদি (loop == k)
                            {
                              /* Remove relation.  */
                              সংরচনা successor *s = *p;
                              s->suc->গুনতি--;
                              *p = s->next;
                              IF_LINT (মুক্ত (s));
                              ভঙ্গ;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = রিক্ত;
                          loop = tmp;
                        }

                      যেতিয়ালৈ (loop)
                        {
                          সংরচনা item *tmp = loop->qlink;

                          loop->qlink = রিক্ত;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      ঘূৰাই true;
                    }
                  অন্যথা
                    {
                      k->qlink = loop;
                      loop = k;
                      ভঙ্গ;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  ঘূৰাই false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

স্থির bool
recurse_বৃক্ষ (সংরচনা item *মূল, bool (*action) (সংরচনা item *))
{
  যদি (মূল->বাম == রিক্ত && মূল->ডান == রিক্ত)
    ঘূৰাই (*action) (মূল);
  অন্যথা
    {
      যদি (মূল->বাম != রিক্ত)
        যদি (recurse_বৃক্ষ (মূল->বাম, action))
          ঘূৰাই true;
      যদি ((*action) (মূল))
        ঘূৰাই true;
      যদি (মূল->ডান != রিক্ত)
        যদি (recurse_বৃক্ষ (মূল->ডান, action))
          ঘূৰাই true;
    }

  ঘূৰাই false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

স্থির শূন্য
walk_বৃক্ষ (সংরচনা item *মূল, bool (*action) (সংরচনা item *))
{
  যদি (মূল->ডান)
    recurse_বৃক্ষ (মূল->ডান, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

স্থির _Noreturn শূন্য
tsort (ৱর্ণ ধ্রুৱ *file)
{
  bool ok = true;
  সংরচনা item *j = রিক্ত;
  সংরচনা item *k = রিক্ত;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  সংরচনা item *মূল = new_item (রিক্ত);

  যদি (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  যেতিয়ালৈ (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, পরিমাণ (DELIM) - 1, &tokenbuffer);
      যদি (len == (size_t) -1)
        {
          যদি (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          ভঙ্গ;
        }

      affirm (len != 0);

      k = অনুসন্ধান_item (মূল, tokenbuffer.buffer);
      যদি (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = রিক্ত;
        }

      j = k;
    }

  যদি (k != রিক্ত)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_বৃক্ষ (মূল, গুনতি_বস্তুসমূহ);

  যেতিয়ালৈ (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_বৃক্ষ (মূল, scan_zeros);

      যেতিয়ালৈ (head)
        {
          সংরচনা successor *p = head->শীৰ্ষ;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          যেতিয়ালৈ (p)
            {
              p->suc->গুনতি--;
              যদি (p->suc->গুনতি == 0)
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
      যদি (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          কৰা
            walk_বৃক্ষ (মূল, detect_loop);
          যেতিয়ালৈ (loop);
        }
    }

  যদি (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ (true)
    {
      স্থির সংরচনা option ধ্রুৱ long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {রিক্ত, 0, রিক্ত, 0}
        };
      পূর্ণাঙ্ক c = getopt_long (argc, argv, "w", long_options, রিক্ত);

      যদি (c == -1)
        ভঙ্গ;

      ৱিকল্প (c)
        {
        পক্ষ 'w':
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  যদি (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
