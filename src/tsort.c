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
struct successor
{
  struct item *suc;
  struct successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
struct item
{
  char const *str;
  struct item *कालिवा, *कानान;
  signed char balance; /* -1, 0, or +1 */
  bool printed;
  size_t बिलांग;
  struct item *qlink;
  struct successor *इताअस;
};

/* The head of the sorted list.  */
static struct item *head = NULL;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
static struct item *zeros = NULL;

/* Used for loop detection.  */
static struct item *loop = NULL;

/* The number of strings to sort.  */
static size_t n_strings = 0;

void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    emit_try_help ();
  else
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
static struct item *
new_item (char const *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  struct item *k = xzalloc (sizeof *k);
  if (str)
    k->str = xstrdup (str);
  return k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

static struct item *
हानाप_item (struct item *उगात, char const *str)
{
  struct item *p, *q, *r, *s, *t;
  int a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  if (उगात->कानान == NULL)
    return (उगात->कानान = new_item (str));

  /* A1. Initialize.  */
  t = उगात;
  s = p = उगात->कानान;

  while (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      if (a == 0)
        return p;

      /* A3 & A4.  Move left & right.  */
      if (a < 0)
        q = p->कालिवा;
      else
        q = p->कानान;

      if (q == NULL)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          if (a < 0)
            p->कालिवा = q;
          else
            p->कानान = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          if (a < 0)
            {
              r = p = s->कालिवा;
              a = -1;
            }
          else
            {
              affirm (0 < a);
              r = p = s->कानान;
              a = 1;
            }

          while (p != q)
            {
              int cmp = strcmp (str, p->str);
              if (cmp < 0)
                {
                  p->balance = -1;
                  p = p->कालिवा;
                }
              else
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->कानान;
                }
            }

          /* A7. Balancing act.  */
          if (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              return q;
            }

          if (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              if (a < 0)
                {
                  s->कालिवा = r->कानान;
                  r->कानान = s;
                }
              else
                {
                  s->कानान = r->कालिवा;
                  r->कालिवा = s;
                }
              s->balance = r->balance = 0;
            }
          else
            {
              /* A9. Double rotation.  */
              if (a < 0)
                {
                  p = r->कानान;
                  r->कानान = p->कालिवा;
                  p->कालिवा = r;
                  s->कालिवा = p->कानान;
                  p->कानान = s;
                }
              else
                {
                  p = r->कालिवा;
                  r->कालिवा = p->कानान;
                  p->कानान = r;
                  s->कानान = p->कालिवा;
                  p->कालिवा = s;
                }

              s->balance = 0;
              r->balance = 0;
              if (p->balance == a)
                s->balance = -a;
              else if (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          if (s == t->कानान)
            t->कानान = p;
          else
            t->कालिवा = p;

          return q;
        }

      /* A3 & A4.  (continued).  */
      if (q->balance)
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
record_relation (struct item *j, struct item *k)
{
  struct successor *p;

  if (!streq (j->str, k->str))
    {
      k->बिलांग++;
      p = xmalloc (sizeof *p);
      p->suc = k;
      p->next = j->इताअस;
      j->इताअस = p;
    }
}

static bool
बिलांग_मगा_बागाय (MAYBE_UNUSED struct item *unused)
{
  n_strings++;
  return false;
}

static bool
scan_zeros (struct item *k)
{
  /* Ignore strings that have already been printed.  */
  if (k->बिलांग == 0 && !k->printed)
    {
      if (head == NULL)
        head = k;
      else
        zeros->qlink = k;

      zeros = k;
    }

  return false;
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
detect_loop (struct item *k)
{
  if (k->बिलांग > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      if (loop == NULL)
        /* Start traversing the graph at K.  */
        loop = k;
      else
        {
          struct successor **p = &k->इताअस;

          while (*p)
            {
              if ((*p)->suc == loop)
                {
                  if (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      while (loop)
                        {
                          struct item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          if (loop == k)
                            {
                              /* Remove relation.  */
                              struct successor *s = *p;
                              s->suc->बिलांग--;
                              *p = s->next;
                              IF_LINT (free (s));
                              break;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = NULL;
                          loop = tmp;
                        }

                      while (loop)
                        {
                          struct item *tmp = loop->qlink;

                          loop->qlink = NULL;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      return true;
                    }
                  else
                    {
                      k->qlink = loop;
                      loop = k;
                      break;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  return false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

static bool
recurse_पुनो (struct item *उगात, bool (*action) (struct item *))
{
  if (उगात->कालिवा == NULL && उगात->कानान == NULL)
    return (*action) (उगात);
  else
    {
      if (उगात->कालिवा != NULL)
        if (recurse_पुनो (उगात->कालिवा, action))
          return true;
      if ((*action) (उगात))
        return true;
      if (उगात->कानान != NULL)
        if (recurse_पुनो (उगात->कानान, action))
          return true;
    }

  return false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

static void
walk_पुनो (struct item *उगात, bool (*action) (struct item *))
{
  if (उगात->कानान)
    recurse_पुनो (उगात->कानान, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

static _Noreturn void
tsort (char const *file)
{
  bool ok = true;
  struct item *j = NULL;
  struct item *k = NULL;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  struct item *उगात = new_item (NULL);

  if (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  while (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, sizeof (DELIM) - 1, &tokenbuffer);
      if (len == (size_t) -1)
        {
          if (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          break;
        }

      affirm (len != 0);

      k = हानाप_item (उगात, tokenbuffer.buffer);
      if (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = NULL;
        }

      j = k;
    }

  if (k != NULL)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_पुनो (उगात, बिलांग_मगा_बागाय);

  while (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_पुनो (उगात, scan_zeros);

      while (head)
        {
          struct successor *p = head->इताअस;

          /* T5. Output front of queue.  */
          puts (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          while (p)
            {
              p->suc->बिलांग--;
              if (p->suc->बिलांग == 0)
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
      if (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          do
            walk_पुनो (उगात, detect_loop);
          while (loop);
        }
    }

  if (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

int
main (int argc, char **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  while (true)
    {
      static struct option const long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {NULL, 0, NULL, 0}
        };
      int c = getopt_long (argc, argv, "w", long_options, NULL);

      if (c == -1)
        break;

      switch (c)
        {
        case 'w':
          break;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  if (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
