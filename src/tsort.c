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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "readtokens.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "tsort"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DELIM " \t\n"

/* Members of the list of successors.  */
ਬਣਤਰ successor
{
  ਬਣਤਰ ਵਸਤੂ *suc;
  ਬਣਤਰ successor *ਅਗਲਾ;
};

/* Each string is held in memory as the head of a list of successors.  */
ਬਣਤਰ ਵਸਤੂ
{
  ਅੱਖਰ ਅਚਲ *str;
  ਬਣਤਰ ਵਸਤੂ *ਖੱਬਾ, *ਸੱਜਾ;
  ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ balance; /* -1, 0, or +1 */
  bool printed;
  size_t ਗਿਣਤੀ_2;
  ਬਣਤਰ ਵਸਤੂ *qlink;
  ਬਣਤਰ successor *ਸਿਖਰ;
};

/* The head of the sorted list.  */
ਸਥਿਰ ਬਣਤਰ ਵਸਤੂ *ਸਿਰ = ਨਿਰਾ;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ਸਥਿਰ ਬਣਤਰ ਵਸਤੂ *zeros = ਨਿਰਾ;

/* Used for loop detection.  */
ਸਥਿਰ ਬਣਤਰ ਵਸਤੂ *loop = ਨਿਰਾ;

/* The number of strings to sort.  */
ਸਥਿਰ size_t n_ਲੜੀਆਂ = 0;

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION] [FILE]\n\
Write totally ordered list consistent with the partial ordering ਵਿੱਚ FILE.\n\
"), ਪ੍ਰੋਗਰਾਮ_name);

      emit_stdin_note ();

      fputs (_("\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }

  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Create a new item/node for STR.  */
ਸਥਿਰ ਬਣਤਰ ਵਸਤੂ *
new_ਵਸਤੂ (ਅੱਖਰ ਅਚਲ *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  ਬਣਤਰ ਵਸਤੂ *k = xzalloc (ਆਕਾਰ *k);
  ਜੇ (str)
    k->str = xstrdup (str);
  ਵਾਪਸ k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

ਸਥਿਰ ਬਣਤਰ ਵਸਤੂ *
ਖੋਜ_ਵਸਤੂ (ਬਣਤਰ ਵਸਤੂ *ਜੜ੍ਹ, ਅੱਖਰ ਅਚਲ *str)
{
  ਬਣਤਰ ਵਸਤੂ *p, *q, *r, *s, *t;
  ਪੂਰਨ a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  ਜੇ (ਜੜ੍ਹ->ਸੱਜਾ == ਨਿਰਾ)
    ਵਾਪਸ (ਜੜ੍ਹ->ਸੱਜਾ = new_ਵਸਤੂ (str));

  /* A1. Initialize.  */
  t = ਜੜ੍ਹ;
  s = p = ਜੜ੍ਹ->ਸੱਜਾ;

  ਜਦੋਂ (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      ਜੇ (a == 0)
        ਵਾਪਸ p;

      /* A3 & A4.  Move left & right.  */
      ਜੇ (a < 0)
        q = p->ਖੱਬਾ;
      ਨਹੀਂਤਾਂ
        q = p->ਸੱਜਾ;

      ਜੇ (q == ਨਿਰਾ)
        {
          /* A5. Insert.  */
          q = new_ਵਸਤੂ (str);

          /* A3 & A4.  (continued).  */
          ਜੇ (a < 0)
            p->ਖੱਬਾ = q;
          ਨਹੀਂਤਾਂ
            p->ਸੱਜਾ = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          ਜੇ (a < 0)
            {
              r = p = s->ਖੱਬਾ;
              a = -1;
            }
          ਨਹੀਂਤਾਂ
            {
              affirm (0 < a);
              r = p = s->ਸੱਜਾ;
              a = 1;
            }

          ਜਦੋਂ (p != q)
            {
              ਪੂਰਨ cmp = strcmp (str, p->str);
              ਜੇ (cmp < 0)
                {
                  p->balance = -1;
                  p = p->ਖੱਬਾ;
                }
              ਨਹੀਂਤਾਂ
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->ਸੱਜਾ;
                }
            }

          /* A7. Balancing act.  */
          ਜੇ (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              ਵਾਪਸ q;
            }

          ਜੇ (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              ਜੇ (a < 0)
                {
                  s->ਖੱਬਾ = r->ਸੱਜਾ;
                  r->ਸੱਜਾ = s;
                }
              ਨਹੀਂਤਾਂ
                {
                  s->ਸੱਜਾ = r->ਖੱਬਾ;
                  r->ਖੱਬਾ = s;
                }
              s->balance = r->balance = 0;
            }
          ਨਹੀਂਤਾਂ
            {
              /* A9. Double rotation.  */
              ਜੇ (a < 0)
                {
                  p = r->ਸੱਜਾ;
                  r->ਸੱਜਾ = p->ਖੱਬਾ;
                  p->ਖੱਬਾ = r;
                  s->ਖੱਬਾ = p->ਸੱਜਾ;
                  p->ਸੱਜਾ = s;
                }
              ਨਹੀਂਤਾਂ
                {
                  p = r->ਖੱਬਾ;
                  r->ਖੱਬਾ = p->ਸੱਜਾ;
                  p->ਸੱਜਾ = r;
                  s->ਸੱਜਾ = p->ਖੱਬਾ;
                  p->ਖੱਬਾ = s;
                }

              s->balance = 0;
              r->balance = 0;
              ਜੇ (p->balance == a)
                s->balance = -a;
              ਨਹੀਂਤਾਂ ਜੇ (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          ਜੇ (s == t->ਸੱਜਾ)
            t->ਸੱਜਾ = p;
          ਨਹੀਂਤਾਂ
            t->ਖੱਬਾ = p;

          ਵਾਪਸ q;
        }

      /* A3 & A4.  (continued).  */
      ਜੇ (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

ਸਥਿਰ ਖਾਲੀ
ਰਿਕਾਰਡ_ਸਬੰਧ (ਬਣਤਰ ਵਸਤੂ *j, ਬਣਤਰ ਵਸਤੂ *k)
{
  ਬਣਤਰ successor *p;

  ਜੇ (!streq (j->str, k->str))
    {
      k->ਗਿਣਤੀ_2++;
      p = xmalloc (ਆਕਾਰ *p);
      p->suc = k;
      p->ਅਗਲਾ = j->ਸਿਖਰ;
      j->ਸਿਖਰ = p;
    }
}

ਸਥਿਰ bool
ਗਿਣਤੀ_ਚੀਜ਼ਾਂ (MAYBE_ਅਣਵਰਤਿਆ ਬਣਤਰ ਵਸਤੂ *ਅਣਵਰਤਿਆ)
{
  n_ਲੜੀਆਂ++;
  ਵਾਪਸ false;
}

ਸਥਿਰ bool
ਜਾਂਚ_zeros (ਬਣਤਰ ਵਸਤੂ *k)
{
  /* Ignore strings that have already been printed.  */
  ਜੇ (k->ਗਿਣਤੀ_2 == 0 && !k->printed)
    {
      ਜੇ (ਸਿਰ == ਨਿਰਾ)
        ਸਿਰ = k;
      ਨਹੀਂਤਾਂ
        zeros->qlink = k;

      zeros = k;
    }

  ਵਾਪਸ false;
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

ਸਥਿਰ bool
detect_loop (ਬਣਤਰ ਵਸਤੂ *k)
{
  ਜੇ (k->ਗਿਣਤੀ_2 > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      ਜੇ (loop == ਨਿਰਾ)
        /* Start traversing the graph at K.  */
        loop = k;
      ਨਹੀਂਤਾਂ
        {
          ਬਣਤਰ successor **p = &k->ਸਿਖਰ;

          ਜਦੋਂ (*p)
            {
              ਜੇ ((*p)->suc == loop)
                {
                  ਜੇ (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      ਜਦੋਂ (loop)
                        {
                          ਬਣਤਰ ਵਸਤੂ *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          ਜੇ (loop == k)
                            {
                              /* Remove relation.  */
                              ਬਣਤਰ successor *s = *p;
                              s->suc->ਗਿਣਤੀ_2--;
                              *p = s->ਅਗਲਾ;
                              IF_LINT (ਮੁਕਤ (s));
                              ਰੋਕੋ;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = ਨਿਰਾ;
                          loop = tmp;
                        }

                      ਜਦੋਂ (loop)
                        {
                          ਬਣਤਰ ਵਸਤੂ *tmp = loop->qlink;

                          loop->qlink = ਨਿਰਾ;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      ਵਾਪਸ true;
                    }
                  ਨਹੀਂਤਾਂ
                    {
                      k->qlink = loop;
                      loop = k;
                      ਰੋਕੋ;
                    }
                }

              p = &(*p)->ਅਗਲਾ;
            }
        }
    }

  ਵਾਪਸ false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ਸਥਿਰ bool
recurse_ਰੁੱਖ (ਬਣਤਰ ਵਸਤੂ *ਜੜ੍ਹ, bool (*action) (ਬਣਤਰ ਵਸਤੂ *))
{
  ਜੇ (ਜੜ੍ਹ->ਖੱਬਾ == ਨਿਰਾ && ਜੜ੍ਹ->ਸੱਜਾ == ਨਿਰਾ)
    ਵਾਪਸ (*action) (ਜੜ੍ਹ);
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (ਜੜ੍ਹ->ਖੱਬਾ != ਨਿਰਾ)
        ਜੇ (recurse_ਰੁੱਖ (ਜੜ੍ਹ->ਖੱਬਾ, action))
          ਵਾਪਸ true;
      ਜੇ ((*action) (ਜੜ੍ਹ))
        ਵਾਪਸ true;
      ਜੇ (ਜੜ੍ਹ->ਸੱਜਾ != ਨਿਰਾ)
        ਜੇ (recurse_ਰੁੱਖ (ਜੜ੍ਹ->ਸੱਜਾ, action))
          ਵਾਪਸ true;
    }

  ਵਾਪਸ false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ਸਥਿਰ ਖਾਲੀ
walk_ਰੁੱਖ (ਬਣਤਰ ਵਸਤੂ *ਜੜ੍ਹ, bool (*action) (ਬਣਤਰ ਵਸਤੂ *))
{
  ਜੇ (ਜੜ੍ਹ->ਸੱਜਾ)
    recurse_ਰੁੱਖ (ਜੜ੍ਹ->ਸੱਜਾ, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ਸਥਿਰ _Noreturn ਖਾਲੀ
tsort (ਅੱਖਰ ਅਚਲ *file)
{
  bool ok = true;
  ਬਣਤਰ ਵਸਤੂ *j = ਨਿਰਾ;
  ਬਣਤਰ ਵਸਤੂ *k = ਨਿਰਾ;
  token_ਬਫਰ tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  ਬਣਤਰ ਵਸਤੂ *ਜੜ੍ਹ = new_ਵਸਤੂ (ਨਿਰਾ);

  ਜੇ (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  ਜਦੋਂ (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, ਆਕਾਰ (DELIM) - 1, &tokenbuffer);
      ਜੇ (len == (size_t) -1)
        {
          ਜੇ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          ਰੋਕੋ;
        }

      affirm (len != 0);

      k = ਖੋਜ_ਵਸਤੂ (ਜੜ੍ਹ, tokenbuffer.ਬਫਰ);
      ਜੇ (j)
        {
          /* T3. Record the relation.  */
          ਰਿਕਾਰਡ_ਸਬੰਧ (j, k);
          k = ਨਿਰਾ;
        }

      j = k;
    }

  ਜੇ (k != ਨਿਰਾ)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_ਰੁੱਖ (ਜੜ੍ਹ, ਗਿਣਤੀ_ਚੀਜ਼ਾਂ);

  ਜਦੋਂ (n_ਲੜੀਆਂ > 0)
    {
      /* T4. Scan for zeros.  */
      walk_ਰੁੱਖ (ਜੜ੍ਹ, ਜਾਂਚ_zeros);

      ਜਦੋਂ (ਸਿਰ)
        {
          ਬਣਤਰ successor *p = ਸਿਰ->ਸਿਖਰ;

          /* T5. Output front of queue.  */
          puts (ਸਿਰ->str);
          ਸਿਰ->printed = true;
          n_ਲੜੀਆਂ--;

          /* T6. Erase relations.  */
          ਜਦੋਂ (p)
            {
              p->suc->ਗਿਣਤੀ_2--;
              ਜੇ (p->suc->ਗਿਣਤੀ_2 == 0)
                {
                  zeros->qlink = p->suc;
                  zeros = p->suc;
                }

              p = p->ਅਗਲਾ;
            }

          /* T7. Remove from queue.  */
          ਸਿਰ = ਸਿਰ->qlink;
        }

      /* T8.  End of process.  */
      ਜੇ (n_ਲੜੀਆਂ > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          ਕਰੋ
            walk_ਰੁੱਖ (ਜੜ੍ਹ, detect_loop);
          ਜਦੋਂ (loop);
        }
    }

  ਜੇ (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  ਬਾਹਰ_ਨਿਕਲੋ (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ (true)
    {
      ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
          {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
        };
      ਪੂਰਨ c = getopt_long (argc, argv, "w", long_options, ਨਿਰਾ);

      ਜੇ (c == -1)
        ਰੋਕੋ;

      ਚੋਣ (c)
        {
        ਹਾਲਤ 'w':
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
