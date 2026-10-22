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

#شامل <config.h>

#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "assure.h"
#شامل "fadvise.h"
#شامل "readtokens.h"
#شامل "stdio--.h"
#شامل "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
ساختار successor
{
  ساختار item *suc;
  ساختار successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
ساختار item
{
  نویسه ثابت *str;
  ساختار item *چپ, *راست;
  signed نویسه balance; /* -1, 0, or +1 */
  bool printed;
  size_t شمار;
  ساختار item *qlink;
  ساختار successor *بالاترین;
};

/* The head of the sorted list.  */
ایستا ساختار item *head = پوچ;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ایستا ساختار item *zeros = پوچ;

/* Used for loop detection.  */
ایستا ساختار item *loop = پوچ;

/* The number of strings to sort.  */
ایستا size_t n_strings = 0;

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
ایستا ساختار item *
new_item (نویسه ثابت *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  ساختار item *k = xzalloc (اندازه *k);
  اگر (str)
    k->str = xstrdup (str);
  بازگشت k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

ایستا ساختار item *
جستجو_item (ساختار item *ریشه, نویسه ثابت *str)
{
  ساختار item *p, *q, *r, *s, *t;
  صحیح a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  اگر (ریشه->راست == پوچ)
    بازگشت (ریشه->راست = new_item (str));

  /* A1. Initialize.  */
  t = ریشه;
  s = p = ریشه->راست;

  وقتی (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      اگر (a == 0)
        بازگشت p;

      /* A3 & A4.  Move left & right.  */
      اگر (a < 0)
        q = p->چپ;
      وگرنه
        q = p->راست;

      اگر (q == پوچ)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          اگر (a < 0)
            p->چپ = q;
          وگرنه
            p->راست = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          اگر (a < 0)
            {
              r = p = s->چپ;
              a = -1;
            }
          وگرنه
            {
              affirm (0 < a);
              r = p = s->راست;
              a = 1;
            }

          وقتی (p != q)
            {
              صحیح cmp = strcmp (str, p->str);
              اگر (cmp < 0)
                {
                  p->balance = -1;
                  p = p->چپ;
                }
              وگرنه
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->راست;
                }
            }

          /* A7. Balancing act.  */
          اگر (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              بازگشت q;
            }

          اگر (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              اگر (a < 0)
                {
                  s->چپ = r->راست;
                  r->راست = s;
                }
              وگرنه
                {
                  s->راست = r->چپ;
                  r->چپ = s;
                }
              s->balance = r->balance = 0;
            }
          وگرنه
            {
              /* A9. Double rotation.  */
              اگر (a < 0)
                {
                  p = r->راست;
                  r->راست = p->چپ;
                  p->چپ = r;
                  s->چپ = p->راست;
                  p->راست = s;
                }
              وگرنه
                {
                  p = r->چپ;
                  r->چپ = p->راست;
                  p->راست = r;
                  s->راست = p->چپ;
                  p->چپ = s;
                }

              s->balance = 0;
              r->balance = 0;
              اگر (p->balance == a)
                s->balance = -a;
              وگرنه اگر (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          اگر (s == t->راست)
            t->راست = p;
          وگرنه
            t->چپ = p;

          بازگشت q;
        }

      /* A3 & A4.  (continued).  */
      اگر (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

ایستا تهی
record_relation (ساختار item *j, ساختار item *k)
{
  ساختار successor *p;

  اگر (!streq (j->str, k->str))
    {
      k->شمار++;
      p = xmalloc (اندازه *p);
      p->suc = k;
      p->next = j->بالاترین;
      j->بالاترین = p;
    }
}

ایستا bool
شمار_اقلام (MAYBE_UNUSED ساختار item *unused)
{
  n_strings++;
  بازگشت false;
}

ایستا bool
scan_zeros (ساختار item *k)
{
  /* Ignore strings that have already been printed.  */
  اگر (k->شمار == 0 && !k->printed)
    {
      اگر (head == پوچ)
        head = k;
      وگرنه
        zeros->qlink = k;

      zeros = k;
    }

  بازگشت false;
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

ایستا bool
detect_loop (ساختار item *k)
{
  اگر (k->شمار > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      اگر (loop == پوچ)
        /* Start traversing the graph at K.  */
        loop = k;
      وگرنه
        {
          ساختار successor **p = &k->بالاترین;

          وقتی (*p)
            {
              اگر ((*p)->suc == loop)
                {
                  اگر (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      وقتی (loop)
                        {
                          ساختار item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          اگر (loop == k)
                            {
                              /* Remove relation.  */
                              ساختار successor *s = *p;
                              s->suc->شمار--;
                              *p = s->next;
                              IF_LINT (آزاد (s));
                              بشکن;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = پوچ;
                          loop = tmp;
                        }

                      وقتی (loop)
                        {
                          ساختار item *tmp = loop->qlink;

                          loop->qlink = پوچ;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      بازگشت true;
                    }
                  وگرنه
                    {
                      k->qlink = loop;
                      loop = k;
                      بشکن;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  بازگشت false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ایستا bool
recurse_درخت (ساختار item *ریشه, bool (*action) (ساختار item *))
{
  اگر (ریشه->چپ == پوچ && ریشه->راست == پوچ)
    بازگشت (*action) (ریشه);
  وگرنه
    {
      اگر (ریشه->چپ != پوچ)
        اگر (recurse_درخت (ریشه->چپ, action))
          بازگشت true;
      اگر ((*action) (ریشه))
        بازگشت true;
      اگر (ریشه->راست != پوچ)
        اگر (recurse_درخت (ریشه->راست, action))
          بازگشت true;
    }

  بازگشت false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ایستا تهی
walk_درخت (ساختار item *ریشه, bool (*action) (ساختار item *))
{
  اگر (ریشه->راست)
    recurse_درخت (ریشه->راست, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ایستا _Noreturn تهی
tsort (نویسه ثابت *file)
{
  bool ok = true;
  ساختار item *j = پوچ;
  ساختار item *k = پوچ;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  ساختار item *ریشه = new_item (پوچ);

  اگر (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  وقتی (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, اندازه (DELIM) - 1, &tokenbuffer);
      اگر (len == (size_t) -1)
        {
          اگر (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          بشکن;
        }

      affirm (len != 0);

      k = جستجو_item (ریشه, tokenbuffer.buffer);
      اگر (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = پوچ;
        }

      j = k;
    }

  اگر (k != پوچ)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_درخت (ریشه, شمار_اقلام);

  وقتی (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_درخت (ریشه, scan_zeros);

      وقتی (head)
        {
          ساختار successor *p = head->بالاترین;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          وقتی (p)
            {
              p->suc->شمار--;
              اگر (p->suc->شمار == 0)
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
      اگر (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          انجامبده
            walk_درخت (ریشه, detect_loop);
          وقتی (loop);
        }
    }

  اگر (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی (true)
    {
      ایستا ساختار option ثابت long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {پوچ, 0, پوچ, 0}
        };
      صحیح c = getopt_long (argc, argv, "w", long_options, پوچ);

      اگر (c == -1)
        بشکن;

      انتخاب (c)
        {
        حالت 'w':
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
