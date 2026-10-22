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
  אם (status != EXIT_SUCCESS)
    emit_try_help ();
  אחרת
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
  אם (str)
    k->str = xstrdup (str);
  החזר k;
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
  אם (root->right == عدم)
    החזר (root->right = new_item (str));

  /* A1. Initialize.  */
  t = root;
  s = p = root->right;

  כלעוד (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      אם (a == 0)
        החזר p;

      /* A3 & A4.  Move left & right.  */
      אם (a < 0)
        q = p->left;
      אחרת
        q = p->right;

      אם (q == عدم)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          אם (a < 0)
            p->left = q;
          אחרת
            p->right = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          אם (a < 0)
            {
              r = p = s->left;
              a = -1;
            }
          אחרת
            {
              affirm (0 < a);
              r = p = s->right;
              a = 1;
            }

          כלעוד (p != q)
            {
              صحيح cmp = strcmp (str, p->str);
              אם (cmp < 0)
                {
                  p->balance = -1;
                  p = p->left;
                }
              אחרת
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->right;
                }
            }

          /* A7. Balancing act.  */
          אם (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              החזר q;
            }

          אם (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              אם (a < 0)
                {
                  s->left = r->right;
                  r->right = s;
                }
              אחרת
                {
                  s->right = r->left;
                  r->left = s;
                }
              s->balance = r->balance = 0;
            }
          אחרת
            {
              /* A9. Double rotation.  */
              אם (a < 0)
                {
                  p = r->right;
                  r->right = p->left;
                  p->left = r;
                  s->left = p->right;
                  p->right = s;
                }
              אחרת
                {
                  p = r->left;
                  r->left = p->right;
                  p->right = r;
                  s->right = p->left;
                  p->left = s;
                }

              s->balance = 0;
              r->balance = 0;
              אם (p->balance == a)
                s->balance = -a;
              אחרת אם (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          אם (s == t->right)
            t->right = p;
          אחרת
            t->left = p;

          החזר q;
        }

      /* A3 & A4.  (continued).  */
      אם (q->balance)
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

  אם (!streq (j->str, k->str))
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
  החזר false;
}

ساكن bool
scan_zeros (بنية item *k)
{
  /* Ignore strings that have already been printed.  */
  אם (k->count == 0 && !k->printed)
    {
      אם (head == عدم)
        head = k;
      אחרת
        zeros->qlink = k;

      zeros = k;
    }

  החזר false;
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
  אם (k->count > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      אם (loop == عدم)
        /* Start traversing the graph at K.  */
        loop = k;
      אחרת
        {
          بنية successor **p = &k->top;

          כלעוד (*p)
            {
              אם ((*p)->suc == loop)
                {
                  אם (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      כלעוד (loop)
                        {
                          بنية item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          אם (loop == k)
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

                      כלעוד (loop)
                        {
                          بنية item *tmp = loop->qlink;

                          loop->qlink = عدم;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      החזר true;
                    }
                  אחרת
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

  החזר false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ساكن bool
recurse_tree (بنية item *root, bool (*action) (بنية item *))
{
  אם (root->left == عدم && root->right == عدم)
    החזר (*action) (root);
  אחרת
    {
      אם (root->left != عدم)
        אם (recurse_tree (root->left, action))
          החזר true;
      אם ((*action) (root))
        החזר true;
      אם (root->right != عدم)
        אם (recurse_tree (root->right, action))
          החזר true;
    }

  החזר false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ساكن فراغ
walk_tree (بنية item *root, bool (*action) (بنية item *))
{
  אם (root->right)
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

  אם (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  כלעוד (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, حجم (DELIM) - 1, &tokenbuffer);
      אם (len == (size_t) -1)
        {
          אם (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          اكسر;
        }

      affirm (len != 0);

      k = search_item (root, tokenbuffer.buffer);
      אם (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = عدم;
        }

      j = k;
    }

  אם (k != عدم)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_tree (root, count_items);

  כלעוד (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_tree (root, scan_zeros);

      כלעוד (head)
        {
          بنية successor *p = head->top;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          כלעוד (p)
            {
              p->suc->count--;
              אם (p->suc->count == 0)
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
      אם (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          עשה
            walk_tree (root, detect_loop);
          כלעוד (loop);
        }
    }

  אם (fclose (stdin) != 0)
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

  כלעוד (true)
    {
      ساكن بنية option ثابت long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {عدم, 0, عدم, 0}
        };
      صحيح c = getopt_long (argc, argv, "w", long_options, عدم);

      אם (c == -1)
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

  אם (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
