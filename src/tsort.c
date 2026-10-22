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
#تعریف_کرو پروگرام_NAME "tsort"

#تعریف_کرو AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#تعریف_کرو DELIM " \t\n"

/* Members of the list of successors.  */
بنية successor
{
  بنية شے *suc;
  بنية successor *اگلا;
};

/* Each string is held in memory as the head of a list of successors.  */
بنية شے
{
  حرف ثابت *str;
  بنية شے *بایاں, *دایاں;
  نشانیوالا حرف balance; /* -1, 0, or +1 */
  bool printed;
  size_t گنتی;
  بنية شے *qlink;
  بنية successor *چوٹی;
};

/* The head of the sorted list.  */
ساكن بنية شے *سر = عدم;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ساكن بنية شے *zeros = عدم;

/* Used for loop detection.  */
ساكن بنية شے *loop = عدم;

/* The number of strings to sort.  */
ساكن size_t n_لڑیاں = 0;

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION] [FILE]\n\
Write totally ordered list consistent with the partial ordering وچ FILE.\n\
"), پروگرام_name);

      emit_stdin_note ();

      fputs (_("\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }

  باہر_نکلو (حالت);
}

/* Create a new item/node for STR.  */
ساكن بنية شے *
new_شے (حرف ثابت *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  بنية شے *k = xzalloc (حجم *k);
  إذا (str)
    k->str = xstrdup (str);
  أرجع k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

ساكن بنية شے *
تلاش_شے (بنية شے *جڑ, حرف ثابت *str)
{
  بنية شے *p, *q, *r, *s, *t;
  صحيح a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  إذا (جڑ->دایاں == عدم)
    أرجع (جڑ->دایاں = new_شے (str));

  /* A1. Initialize.  */
  t = جڑ;
  s = p = جڑ->دایاں;

  طالما (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      إذا (a == 0)
        أرجع p;

      /* A3 & A4.  Move left & right.  */
      إذا (a < 0)
        q = p->بایاں;
      وإلا
        q = p->دایاں;

      إذا (q == عدم)
        {
          /* A5. Insert.  */
          q = new_شے (str);

          /* A3 & A4.  (continued).  */
          إذا (a < 0)
            p->بایاں = q;
          وإلا
            p->دایاں = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          إذا (a < 0)
            {
              r = p = s->بایاں;
              a = -1;
            }
          وإلا
            {
              affirm (0 < a);
              r = p = s->دایاں;
              a = 1;
            }

          طالما (p != q)
            {
              صحيح cmp = strcmp (str, p->str);
              إذا (cmp < 0)
                {
                  p->balance = -1;
                  p = p->بایاں;
                }
              وإلا
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->دایاں;
                }
            }

          /* A7. Balancing act.  */
          إذا (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              أرجع q;
            }

          إذا (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              إذا (a < 0)
                {
                  s->بایاں = r->دایاں;
                  r->دایاں = s;
                }
              وإلا
                {
                  s->دایاں = r->بایاں;
                  r->بایاں = s;
                }
              s->balance = r->balance = 0;
            }
          وإلا
            {
              /* A9. Double rotation.  */
              إذا (a < 0)
                {
                  p = r->دایاں;
                  r->دایاں = p->بایاں;
                  p->بایاں = r;
                  s->بایاں = p->دایاں;
                  p->دایاں = s;
                }
              وإلا
                {
                  p = r->بایاں;
                  r->بایاں = p->دایاں;
                  p->دایاں = r;
                  s->دایاں = p->بایاں;
                  p->بایاں = s;
                }

              s->balance = 0;
              r->balance = 0;
              إذا (p->balance == a)
                s->balance = -a;
              وإلا إذا (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          إذا (s == t->دایاں)
            t->دایاں = p;
          وإلا
            t->بایاں = p;

          أرجع q;
        }

      /* A3 & A4.  (continued).  */
      إذا (q->balance)
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
ریکارڈ_سمبندھ (بنية شے *j, بنية شے *k)
{
  بنية successor *p;

  إذا (!streq (j->str, k->str))
    {
      k->گنتی++;
      p = xmalloc (حجم *p);
      p->suc = k;
      p->اگلا = j->چوٹی;
      j->چوٹی = p;
    }
}

ساكن bool
گنتی_اشیاء (MAYBE_بن_ورتیا بنية شے *بن_ورتیا)
{
  n_لڑیاں++;
  أرجع false;
}

ساكن bool
جانچ_zeros (بنية شے *k)
{
  /* Ignore strings that have already been printed.  */
  إذا (k->گنتی == 0 && !k->printed)
    {
      إذا (سر == عدم)
        سر = k;
      وإلا
        zeros->qlink = k;

      zeros = k;
    }

  أرجع false;
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
detect_loop (بنية شے *k)
{
  إذا (k->گنتی > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      إذا (loop == عدم)
        /* Start traversing the graph at K.  */
        loop = k;
      وإلا
        {
          بنية successor **p = &k->چوٹی;

          طالما (*p)
            {
              إذا ((*p)->suc == loop)
                {
                  إذا (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      طالما (loop)
                        {
                          بنية شے *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          إذا (loop == k)
                            {
                              /* Remove relation.  */
                              بنية successor *s = *p;
                              s->suc->گنتی--;
                              *p = s->اگلا;
                              IF_LINT (حرر (s));
                              اكسر;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = عدم;
                          loop = tmp;
                        }

                      طالما (loop)
                        {
                          بنية شے *tmp = loop->qlink;

                          loop->qlink = عدم;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      أرجع true;
                    }
                  وإلا
                    {
                      k->qlink = loop;
                      loop = k;
                      اكسر;
                    }
                }

              p = &(*p)->اگلا;
            }
        }
    }

  أرجع false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ساكن bool
recurse_درخت (بنية شے *جڑ, bool (*action) (بنية شے *))
{
  إذا (جڑ->بایاں == عدم && جڑ->دایاں == عدم)
    أرجع (*action) (جڑ);
  وإلا
    {
      إذا (جڑ->بایاں != عدم)
        إذا (recurse_درخت (جڑ->بایاں, action))
          أرجع true;
      إذا ((*action) (جڑ))
        أرجع true;
      إذا (جڑ->دایاں != عدم)
        إذا (recurse_درخت (جڑ->دایاں, action))
          أرجع true;
    }

  أرجع false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ساكن فراغ
walk_درخت (بنية شے *جڑ, bool (*action) (بنية شے *))
{
  إذا (جڑ->دایاں)
    recurse_درخت (جڑ->دایاں, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ساكن _Noreturn فراغ
tsort (حرف ثابت *file)
{
  bool ok = true;
  بنية شے *j = عدم;
  بنية شے *k = عدم;
  token_بفر tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  بنية شے *جڑ = new_شے (عدم);

  إذا (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  طالما (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, حجم (DELIM) - 1, &tokenbuffer);
      إذا (len == (size_t) -1)
        {
          إذا (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          اكسر;
        }

      affirm (len != 0);

      k = تلاش_شے (جڑ, tokenbuffer.بفر);
      إذا (j)
        {
          /* T3. Record the relation.  */
          ریکارڈ_سمبندھ (j, k);
          k = عدم;
        }

      j = k;
    }

  إذا (k != عدم)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_درخت (جڑ, گنتی_اشیاء);

  طالما (n_لڑیاں > 0)
    {
      /* T4. Scan for zeros.  */
      walk_درخت (جڑ, جانچ_zeros);

      طالما (سر)
        {
          بنية successor *p = سر->چوٹی;

          /* T5. Output front of queue.  */
          puts (سر->str);
          سر->printed = true;
          n_لڑیاں--;

          /* T6. Erase relations.  */
          طالما (p)
            {
              p->suc->گنتی--;
              إذا (p->suc->گنتی == 0)
                {
                  zeros->qlink = p->suc;
                  zeros = p->suc;
                }

              p = p->اگلا;
            }

          /* T7. Remove from queue.  */
          سر = سر->qlink;
        }

      /* T8.  End of process.  */
      إذا (n_لڑیاں > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          افعل
            walk_درخت (جڑ, detect_loop);
          طالما (loop);
        }
    }

  إذا (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  باہر_نکلو (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما (true)
    {
      ساكن بنية option ثابت long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_نسخہ_OPTION_DECL},
          {عدم, 0, عدم, 0}
        };
      صحيح c = getopt_long (argc, argv, "w", long_options, عدم);

      إذا (c == -1)
        اكسر;

      بدل (c)
        {
        حالة 'w':
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
