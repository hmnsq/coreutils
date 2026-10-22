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
#تعریف_کړه پروګرام_NAME "tsort"

#تعریف_کړه AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#تعریف_کړه DELIM " \t\n"

/* Members of the list of successors.  */
بنية successor
{
  بنية توکی *suc;
  بنية successor *بل;
};

/* Each string is held in memory as the head of a list of successors.  */
بنية توکی
{
  حرف ثابت *str;
  بنية توکی *کیڼ, *ښی;
  نښهدار حرف balance; /* -1, 0, or +1 */
  bool printed;
  size_t شمېر;
  بنية توکی *qlink;
  بنية successor *سر_2;
};

/* The head of the sorted list.  */
ساكن بنية توکی *سر = عدم;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ساكن بنية توکی *zeros = عدم;

/* Used for loop detection.  */
ساكن بنية توکی *loop = عدم;

/* The number of strings to sort.  */
ساكن size_t n_مزي_ګانې = 0;

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION] [FILE]\n\
Write totally ordered list consistent with the partial ordering دننه FILE.\n\
"), پروګرام_name);

      emit_stdin_note ();

      fputs (_("\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (پروګرام_NAME);
    }

  وتل (حالت);
}

/* Create a new item/node for STR.  */
ساكن بنية توکی *
new_توکی (حرف ثابت *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  بنية توکی *k = xzalloc (حجم *k);
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

ساكن بنية توکی *
پلټنه_توکی (بنية توکی *ریښه, حرف ثابت *str)
{
  بنية توکی *p, *q, *r, *s, *t;
  صحيح a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  إذا (ریښه->ښی == عدم)
    أرجع (ریښه->ښی = new_توکی (str));

  /* A1. Initialize.  */
  t = ریښه;
  s = p = ریښه->ښی;

  طالما (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      إذا (a == 0)
        أرجع p;

      /* A3 & A4.  Move left & right.  */
      إذا (a < 0)
        q = p->کیڼ;
      وإلا
        q = p->ښی;

      إذا (q == عدم)
        {
          /* A5. Insert.  */
          q = new_توکی (str);

          /* A3 & A4.  (continued).  */
          إذا (a < 0)
            p->کیڼ = q;
          وإلا
            p->ښی = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          إذا (a < 0)
            {
              r = p = s->کیڼ;
              a = -1;
            }
          وإلا
            {
              affirm (0 < a);
              r = p = s->ښی;
              a = 1;
            }

          طالما (p != q)
            {
              صحيح cmp = strcmp (str, p->str);
              إذا (cmp < 0)
                {
                  p->balance = -1;
                  p = p->کیڼ;
                }
              وإلا
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->ښی;
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
                  s->کیڼ = r->ښی;
                  r->ښی = s;
                }
              وإلا
                {
                  s->ښی = r->کیڼ;
                  r->کیڼ = s;
                }
              s->balance = r->balance = 0;
            }
          وإلا
            {
              /* A9. Double rotation.  */
              إذا (a < 0)
                {
                  p = r->ښی;
                  r->ښی = p->کیڼ;
                  p->کیڼ = r;
                  s->کیڼ = p->ښی;
                  p->ښی = s;
                }
              وإلا
                {
                  p = r->کیڼ;
                  r->کیڼ = p->ښی;
                  p->ښی = r;
                  s->ښی = p->کیڼ;
                  p->کیڼ = s;
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
          إذا (s == t->ښی)
            t->ښی = p;
          وإلا
            t->کیڼ = p;

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
ریکارډ_اړیکه (بنية توکی *j, بنية توکی *k)
{
  بنية successor *p;

  إذا (!streq (j->str, k->str))
    {
      k->شمېر++;
      p = xmalloc (حجم *p);
      p->suc = k;
      p->بل = j->سر_2;
      j->سر_2 = p;
    }
}

ساكن bool
شمېر_توکي (MAYBE_ناکارول_شوی بنية توکی *ناکارول_شوی)
{
  n_مزي_ګانې++;
  أرجع false;
}

ساكن bool
لټون_zeros (بنية توکی *k)
{
  /* Ignore strings that have already been printed.  */
  إذا (k->شمېر == 0 && !k->printed)
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
detect_loop (بنية توکی *k)
{
  إذا (k->شمېر > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      إذا (loop == عدم)
        /* Start traversing the graph at K.  */
        loop = k;
      وإلا
        {
          بنية successor **p = &k->سر_2;

          طالما (*p)
            {
              إذا ((*p)->suc == loop)
                {
                  إذا (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      طالما (loop)
                        {
                          بنية توکی *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          إذا (loop == k)
                            {
                              /* Remove relation.  */
                              بنية successor *s = *p;
                              s->suc->شمېر--;
                              *p = s->بل;
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
                          بنية توکی *tmp = loop->qlink;

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

              p = &(*p)->بل;
            }
        }
    }

  أرجع false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ساكن bool
recurse_ونه (بنية توکی *ریښه, bool (*action) (بنية توکی *))
{
  إذا (ریښه->کیڼ == عدم && ریښه->ښی == عدم)
    أرجع (*action) (ریښه);
  وإلا
    {
      إذا (ریښه->کیڼ != عدم)
        إذا (recurse_ونه (ریښه->کیڼ, action))
          أرجع true;
      إذا ((*action) (ریښه))
        أرجع true;
      إذا (ریښه->ښی != عدم)
        إذا (recurse_ونه (ریښه->ښی, action))
          أرجع true;
    }

  أرجع false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ساكن فراغ
walk_ونه (بنية توکی *ریښه, bool (*action) (بنية توکی *))
{
  إذا (ریښه->ښی)
    recurse_ونه (ریښه->ښی, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ساكن _Noreturn فراغ
tsort (حرف ثابت *file)
{
  bool ok = true;
  بنية توکی *j = عدم;
  بنية توکی *k = عدم;
  token_بفر tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  بنية توکی *ریښه = new_توکی (عدم);

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

      k = پلټنه_توکی (ریښه, tokenbuffer.بفر);
      إذا (j)
        {
          /* T3. Record the relation.  */
          ریکارډ_اړیکه (j, k);
          k = عدم;
        }

      j = k;
    }

  إذا (k != عدم)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_ونه (ریښه, شمېر_توکي);

  طالما (n_مزي_ګانې > 0)
    {
      /* T4. Scan for zeros.  */
      walk_ونه (ریښه, لټون_zeros);

      طالما (سر)
        {
          بنية successor *p = سر->سر_2;

          /* T5. Output front of queue.  */
          puts (سر->str);
          سر->printed = true;
          n_مزي_ګانې--;

          /* T6. Erase relations.  */
          طالما (p)
            {
              p->suc->شمېر--;
              إذا (p->suc->شمېر == 0)
                {
                  zeros->qlink = p->suc;
                  zeros = p->suc;
                }

              p = p->بل;
            }

          /* T7. Remove from queue.  */
          سر = سر->qlink;
        }

      /* T8.  End of process.  */
      إذا (n_مزي_ګانې > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          افعل
            walk_ونه (ریښه, detect_loop);
          طالما (loop);
        }
    }

  إذا (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  وتل (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما (true)
    {
      ساكن بنية option ثابت long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_نسخه_OPTION_DECL},
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

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

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
