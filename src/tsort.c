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

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "fadvise.h"
#ضمن "readtokens.h"
#ضمن "stdio--.h"
#ضمن "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
بنية successor
{
  بنية item *suc;
  بنية successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
بنية item
{
  حرف ثابت *str;
  بنية item *left, *right;
  signed حرف balance; /* -1, 0, or +1 */
  bool printed;
  size_t count;
  بنية item *qlink;
  بنية successor *top;
};

/* The head of the sorted list.  */
ساكن بنية item *head = عدم;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ساكن بنية item *zeros = عدم;

/* Used for loop detection.  */
ساكن بنية item *loop = عدم;

/* The number of strings to sort.  */
ساكن size_t n_strings = 0;

فراغ
usage (صحيح status)
{
  ܐܢ (status != EXIT_SUCCESS)
    emit_try_help ();
  ܐܚܪܢܐ
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
ساكن بنية item *
new_item (حرف ثابت *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  بنية item *k = xzalloc (حجم *k);
  ܐܢ (str)
    k->str = xstrdup (str);
  ܦܢܐ k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

ساكن بنية item *
search_item (بنية item *root, حرف ثابت *str)
{
  بنية item *p, *q, *r, *s, *t;
  صحيح a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  ܐܢ (root->right == عدم)
    ܦܢܐ (root->right = new_item (str));

  /* A1. Initialize.  */
  t = root;
  s = p = root->right;

  ܟܡܐ (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      ܐܢ (a == 0)
        ܦܢܐ p;

      /* A3 & A4.  Move left & right.  */
      ܐܢ (a < 0)
        q = p->left;
      ܐܚܪܢܐ
        q = p->right;

      ܐܢ (q == عدم)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          ܐܢ (a < 0)
            p->left = q;
          ܐܚܪܢܐ
            p->right = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          ܐܢ (a < 0)
            {
              r = p = s->left;
              a = -1;
            }
          ܐܚܪܢܐ
            {
              affirm (0 < a);
              r = p = s->right;
              a = 1;
            }

          ܟܡܐ (p != q)
            {
              صحيح cmp = strcmp (str, p->str);
              ܐܢ (cmp < 0)
                {
                  p->balance = -1;
                  p = p->left;
                }
              ܐܚܪܢܐ
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->right;
                }
            }

          /* A7. Balancing act.  */
          ܐܢ (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              ܦܢܐ q;
            }

          ܐܢ (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              ܐܢ (a < 0)
                {
                  s->left = r->right;
                  r->right = s;
                }
              ܐܚܪܢܐ
                {
                  s->right = r->left;
                  r->left = s;
                }
              s->balance = r->balance = 0;
            }
          ܐܚܪܢܐ
            {
              /* A9. Double rotation.  */
              ܐܢ (a < 0)
                {
                  p = r->right;
                  r->right = p->left;
                  p->left = r;
                  s->left = p->right;
                  p->right = s;
                }
              ܐܚܪܢܐ
                {
                  p = r->left;
                  r->left = p->right;
                  p->right = r;
                  s->right = p->left;
                  p->left = s;
                }

              s->balance = 0;
              r->balance = 0;
              ܐܢ (p->balance == a)
                s->balance = -a;
              ܐܚܪܢܐ ܐܢ (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          ܐܢ (s == t->right)
            t->right = p;
          ܐܚܪܢܐ
            t->left = p;

          ܦܢܐ q;
        }

      /* A3 & A4.  (continued).  */
      ܐܢ (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

ساكن فراغ
record_relation (بنية item *j, بنية item *k)
{
  بنية successor *p;

  ܐܢ (!streq (j->str, k->str))
    {
      k->count++;
      p = xmalloc (حجم *p);
      p->suc = k;
      p->next = j->top;
      j->top = p;
    }
}

ساكن bool
count_items (MAYBE_UNUSED بنية item *unused)
{
  n_strings++;
  ܦܢܐ false;
}

ساكن bool
scan_zeros (بنية item *k)
{
  /* Ignore strings that have already been printed.  */
  ܐܢ (k->count == 0 && !k->printed)
    {
      ܐܢ (head == عدم)
        head = k;
      ܐܚܪܢܐ
        zeros->qlink = k;

      zeros = k;
    }

  ܦܢܐ false;
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

ساكن bool
detect_loop (بنية item *k)
{
  ܐܢ (k->count > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      ܐܢ (loop == عدم)
        /* Start traversing the graph at K.  */
        loop = k;
      ܐܚܪܢܐ
        {
          بنية successor **p = &k->top;

          ܟܡܐ (*p)
            {
              ܐܢ ((*p)->suc == loop)
                {
                  ܐܢ (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      ܟܡܐ (loop)
                        {
                          بنية item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          ܐܢ (loop == k)
                            {
                              /* Remove relation.  */
                              بنية successor *s = *p;
                              s->suc->count--;
                              *p = s->next;
                              IF_LINT (حرر (s));
                              اكسر;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = عدم;
                          loop = tmp;
                        }

                      ܟܡܐ (loop)
                        {
                          بنية item *tmp = loop->qlink;

                          loop->qlink = عدم;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      ܦܢܐ true;
                    }
                  ܐܚܪܢܐ
                    {
                      k->qlink = loop;
                      loop = k;
                      اكسر;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  ܦܢܐ false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ساكن bool
recurse_tree (بنية item *root, bool (*action) (بنية item *))
{
  ܐܢ (root->left == عدم && root->right == عدم)
    ܦܢܐ (*action) (root);
  ܐܚܪܢܐ
    {
      ܐܢ (root->left != عدم)
        ܐܢ (recurse_tree (root->left, action))
          ܦܢܐ true;
      ܐܢ ((*action) (root))
        ܦܢܐ true;
      ܐܢ (root->right != عدم)
        ܐܢ (recurse_tree (root->right, action))
          ܦܢܐ true;
    }

  ܦܢܐ false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ساكن فراغ
walk_tree (بنية item *root, bool (*action) (بنية item *))
{
  ܐܢ (root->right)
    recurse_tree (root->right, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ساكن _Noreturn فراغ
tsort (حرف ثابت *file)
{
  bool ok = true;
  بنية item *j = عدم;
  بنية item *k = عدم;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  بنية item *root = new_item (عدم);

  ܐܢ (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  ܟܡܐ (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, حجم (DELIM) - 1, &tokenbuffer);
      ܐܢ (len == (size_t) -1)
        {
          ܐܢ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          اكسر;
        }

      affirm (len != 0);

      k = search_item (root, tokenbuffer.buffer);
      ܐܢ (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = عدم;
        }

      j = k;
    }

  ܐܢ (k != عدم)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_tree (root, count_items);

  ܟܡܐ (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_tree (root, scan_zeros);

      ܟܡܐ (head)
        {
          بنية successor *p = head->top;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          ܟܡܐ (p)
            {
              p->suc->count--;
              ܐܢ (p->suc->count == 0)
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
      ܐܢ (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          ܥܒܕ
            walk_tree (root, detect_loop);
          ܟܡܐ (loop);
        }
    }

  ܐܢ (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

صحيح
main (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ܟܡܐ (true)
    {
      ساكن بنية option ثابت long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {عدم, 0, عدم, 0}
        };
      صحيح c = getopt_long (argc, argv, "w", long_options, عدم);

      ܐܢ (c == -1)
        اكسر;

      بدل (c)
        {
        حالة 'w':
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  ܐܢ (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
