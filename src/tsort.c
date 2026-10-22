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

#include <config.h>

#include <getopt.h>
#include <sys/types.h>

#include "system.h"
#include "assure.h"
#include "fadvise.h"
#include "readtokens.h"
#include "stdio--.h"
#include "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
sa_nracanaa successor
{
  sa_nracanaa item *suc;
  sa_nracanaa successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
sa_nracanaa item
{
  var_na const *str;
  sa_nracanaa item *left, *right;
  signed var_na balance; /* -1, 0, or +1 */
  bool printed;
  size_t count;
  sa_nracanaa item *qlink;
  sa_nracanaa successor *top;
};

/* The head of the sorted list.  */
static sa_nracanaa item *head = NULL;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
static sa_nracanaa item *zeros = NULL;

/* Used for loop detection.  */
static sa_nracanaa item *loop = NULL;

/* The number of strings to sort.  */
static size_t n_strings = 0;

void
usage (puur_naa_nka status)
{
  ᨀᨘ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᨀᨘᨁ
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
static sa_nracanaa item *
new_item (var_na const *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  sa_nracanaa item *k = xzalloc (parimaa_na *k);
  ᨀᨘ (str)
    k->str = xstrdup (str);
  ᨀᨘᨒ k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

static sa_nracanaa item *
search_item (sa_nracanaa item *root, var_na const *str)
{
  sa_nracanaa item *p, *q, *r, *s, *t;
  puur_naa_nka a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  ᨀᨘ (root->right == NULL)
    ᨀᨘᨒ (root->right = new_item (str));

  /* A1. Initialize.  */
  t = root;
  s = p = root->right;

  ᨀᨛ (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      ᨀᨘ (a == 0)
        ᨀᨘᨒ p;

      /* A3 & A4.  Move left & right.  */
      ᨀᨘ (a < 0)
        q = p->left;
      ᨀᨘᨁ
        q = p->right;

      ᨀᨘ (q == NULL)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          ᨀᨘ (a < 0)
            p->left = q;
          ᨀᨘᨁ
            p->right = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          ᨀᨘ (a < 0)
            {
              r = p = s->left;
              a = -1;
            }
          ᨀᨘᨁ
            {
              affirm (0 < a);
              r = p = s->right;
              a = 1;
            }

          ᨀᨛ (p != q)
            {
              puur_naa_nka cmp = strcmp (str, p->str);
              ᨀᨘ (cmp < 0)
                {
                  p->balance = -1;
                  p = p->left;
                }
              ᨀᨘᨁ
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->right;
                }
            }

          /* A7. Balancing act.  */
          ᨀᨘ (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              ᨀᨘᨒ q;
            }

          ᨀᨘ (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              ᨀᨘ (a < 0)
                {
                  s->left = r->right;
                  r->right = s;
                }
              ᨀᨘᨁ
                {
                  s->right = r->left;
                  r->left = s;
                }
              s->balance = r->balance = 0;
            }
          ᨀᨘᨁ
            {
              /* A9. Double rotation.  */
              ᨀᨘ (a < 0)
                {
                  p = r->right;
                  r->right = p->left;
                  p->left = r;
                  s->left = p->right;
                  p->right = s;
                }
              ᨀᨘᨁ
                {
                  p = r->left;
                  r->left = p->right;
                  p->right = r;
                  s->right = p->left;
                  p->left = s;
                }

              s->balance = 0;
              r->balance = 0;
              ᨀᨘ (p->balance == a)
                s->balance = -a;
              ᨀᨘᨁ ᨀᨘ (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          ᨀᨘ (s == t->right)
            t->right = p;
          ᨀᨘᨁ
            t->left = p;

          ᨀᨘᨒ q;
        }

      /* A3 & A4.  (continued).  */
      ᨀᨘ (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

static void
record_relation (sa_nracanaa item *j, sa_nracanaa item *k)
{
  sa_nracanaa successor *p;

  ᨀᨘ (!streq (j->str, k->str))
    {
      k->count++;
      p = xmalloc (parimaa_na *p);
      p->suc = k;
      p->next = j->top;
      j->top = p;
    }
}

static bool
count_items (MAYBE_UNUSED sa_nracanaa item *unused)
{
  n_strings++;
  ᨀᨘᨒ false;
}

static bool
scan_zeros (sa_nracanaa item *k)
{
  /* Ignore strings that have already been printed.  */
  ᨀᨘ (k->count == 0 && !k->printed)
    {
      ᨀᨘ (head == NULL)
        head = k;
      ᨀᨘᨁ
        zeros->qlink = k;

      zeros = k;
    }

  ᨀᨘᨒ false;
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

static bool
detect_loop (sa_nracanaa item *k)
{
  ᨀᨘ (k->count > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      ᨀᨘ (loop == NULL)
        /* Start traversing the graph at K.  */
        loop = k;
      ᨀᨘᨁ
        {
          sa_nracanaa successor **p = &k->top;

          ᨀᨛ (*p)
            {
              ᨀᨘ ((*p)->suc == loop)
                {
                  ᨀᨘ (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      ᨀᨛ (loop)
                        {
                          sa_nracanaa item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          ᨀᨘ (loop == k)
                            {
                              /* Remove relation.  */
                              sa_nracanaa successor *s = *p;
                              s->suc->count--;
                              *p = s->next;
                              IF_LINT (free (s));
                              bha_nga;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = NULL;
                          loop = tmp;
                        }

                      ᨀᨛ (loop)
                        {
                          sa_nracanaa item *tmp = loop->qlink;

                          loop->qlink = NULL;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      ᨀᨘᨒ true;
                    }
                  ᨀᨘᨁ
                    {
                      k->qlink = loop;
                      loop = k;
                      bha_nga;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  ᨀᨘᨒ false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

static bool
recurse_tree (sa_nracanaa item *root, bool (*action) (sa_nracanaa item *))
{
  ᨀᨘ (root->left == NULL && root->right == NULL)
    ᨀᨘᨒ (*action) (root);
  ᨀᨘᨁ
    {
      ᨀᨘ (root->left != NULL)
        ᨀᨘ (recurse_tree (root->left, action))
          ᨀᨘᨒ true;
      ᨀᨘ ((*action) (root))
        ᨀᨘᨒ true;
      ᨀᨘ (root->right != NULL)
        ᨀᨘ (recurse_tree (root->right, action))
          ᨀᨘᨒ true;
    }

  ᨀᨘᨒ false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

static void
walk_tree (sa_nracanaa item *root, bool (*action) (sa_nracanaa item *))
{
  ᨀᨘ (root->right)
    recurse_tree (root->right, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

static _Noreturn void
tsort (var_na const *file)
{
  bool ok = true;
  sa_nracanaa item *j = NULL;
  sa_nracanaa item *k = NULL;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  sa_nracanaa item *root = new_item (NULL);

  ᨀᨘ (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  ᨀᨛ (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, parimaa_na (DELIM) - 1, &tokenbuffer);
      ᨀᨘ (len == (size_t) -1)
        {
          ᨀᨘ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          bha_nga;
        }

      affirm (len != 0);

      k = search_item (root, tokenbuffer.buffer);
      ᨀᨘ (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = NULL;
        }

      j = k;
    }

  ᨀᨘ (k != NULL)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_tree (root, count_items);

  ᨀᨛ (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_tree (root, scan_zeros);

      ᨀᨛ (head)
        {
          sa_nracanaa successor *p = head->top;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          ᨀᨛ (p)
            {
              p->suc->count--;
              ᨀᨘ (p->suc->count == 0)
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
      ᨀᨘ (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          do
            walk_tree (root, detect_loop);
          ᨀᨛ (loop);
        }
    }

  ᨀᨘ (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ᨀᨛ (true)
    {
      static sa_nracanaa option const long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {NULL, 0, NULL, 0}
        };
      puur_naa_nka c = getopt_long (argc, argv, "w", long_options, NULL);

      ᨀᨘ (c == -1)
        bha_nga;

      switch (c)
        {
        pak_sa 'w':
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ᨀᨘ (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
