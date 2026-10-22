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
#تعریف_کن برنامه_NAME "tsort"

#تعریف_کن AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#تعریف_کن DELIM " \t\n"

/* Members of the list of successors.  */
بنية successor
{
  بنية قلم *suc;
  بنية successor *بعدی;
};

/* Each string is held in memory as the head of a list of successors.  */
بنية قلم
{
  حرف ثابت *str;
  بنية قلم *چپ, *راست;
  علامتدار حرف balance; /* -1, 0, or +1 */
  bool printed;
  size_t شمار;
  بنية قلم *qlink;
  بنية successor *بالاترین;
};

/* The head of the sorted list.  */
ساكن بنية قلم *سر = عدم;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ساكن بنية قلم *zeros = عدم;

/* Used for loop detection.  */
ساكن بنية قلم *loop = عدم;

/* The number of strings to sort.  */
ساكن size_t n_رشتهها = 0;

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION] [FILE]\n\
Write totally ordered list consistent with the partial ordering درون FILE.\n\
"), برنامه_name);

      emit_stdin_note ();

      fputs (_("\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }

  خروج (وضعیت);
}

/* Create a new item/node for STR.  */
ساكن بنية قلم *
new_قلم (حرف ثابت *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  بنية قلم *k = xzalloc (حجم *k);
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

ساكن بنية قلم *
جستجو_قلم (بنية قلم *ریشه, حرف ثابت *str)
{
  بنية قلم *p, *q, *r, *s, *t;
  صحيح a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  إذا (ریشه->راست == عدم)
    أرجع (ریشه->راست = new_قلم (str));

  /* A1. Initialize.  */
  t = ریشه;
  s = p = ریشه->راست;

  طالما (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      إذا (a == 0)
        أرجع p;

      /* A3 & A4.  Move left & right.  */
      إذا (a < 0)
        q = p->چپ;
      وإلا
        q = p->راست;

      إذا (q == عدم)
        {
          /* A5. Insert.  */
          q = new_قلم (str);

          /* A3 & A4.  (continued).  */
          إذا (a < 0)
            p->چپ = q;
          وإلا
            p->راست = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          إذا (a < 0)
            {
              r = p = s->چپ;
              a = -1;
            }
          وإلا
            {
              affirm (0 < a);
              r = p = s->راست;
              a = 1;
            }

          طالما (p != q)
            {
              صحيح cmp = strcmp (str, p->str);
              إذا (cmp < 0)
                {
                  p->balance = -1;
                  p = p->چپ;
                }
              وإلا
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->راست;
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
                  s->چپ = r->راست;
                  r->راست = s;
                }
              وإلا
                {
                  s->راست = r->چپ;
                  r->چپ = s;
                }
              s->balance = r->balance = 0;
            }
          وإلا
            {
              /* A9. Double rotation.  */
              إذا (a < 0)
                {
                  p = r->راست;
                  r->راست = p->چپ;
                  p->چپ = r;
                  s->چپ = p->راست;
                  p->راست = s;
                }
              وإلا
                {
                  p = r->چپ;
                  r->چپ = p->راست;
                  p->راست = r;
                  s->راست = p->چپ;
                  p->چپ = s;
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
          إذا (s == t->راست)
            t->راست = p;
          وإلا
            t->چپ = p;

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
رکورد_رابطه (بنية قلم *j, بنية قلم *k)
{
  بنية successor *p;

  إذا (!streq (j->str, k->str))
    {
      k->شمار++;
      p = xmalloc (حجم *p);
      p->suc = k;
      p->بعدی = j->بالاترین;
      j->بالاترین = p;
    }
}

ساكن bool
شمار_اقلام (MAYBE_استفادهنشده بنية قلم *استفادهنشده)
{
  n_رشتهها++;
  أرجع false;
}

ساكن bool
پیمایش_zeros (بنية قلم *k)
{
  /* Ignore strings that have already been printed.  */
  إذا (k->شمار == 0 && !k->printed)
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
detect_loop (بنية قلم *k)
{
  إذا (k->شمار > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      إذا (loop == عدم)
        /* Start traversing the graph at K.  */
        loop = k;
      وإلا
        {
          بنية successor **p = &k->بالاترین;

          طالما (*p)
            {
              إذا ((*p)->suc == loop)
                {
                  إذا (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      طالما (loop)
                        {
                          بنية قلم *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          إذا (loop == k)
                            {
                              /* Remove relation.  */
                              بنية successor *s = *p;
                              s->suc->شمار--;
                              *p = s->بعدی;
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
                          بنية قلم *tmp = loop->qlink;

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

              p = &(*p)->بعدی;
            }
        }
    }

  أرجع false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ساكن bool
recurse_درخت (بنية قلم *ریشه, bool (*action) (بنية قلم *))
{
  إذا (ریشه->چپ == عدم && ریشه->راست == عدم)
    أرجع (*action) (ریشه);
  وإلا
    {
      إذا (ریشه->چپ != عدم)
        إذا (recurse_درخت (ریشه->چپ, action))
          أرجع true;
      إذا ((*action) (ریشه))
        أرجع true;
      إذا (ریشه->راست != عدم)
        إذا (recurse_درخت (ریشه->راست, action))
          أرجع true;
    }

  أرجع false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ساكن فراغ
walk_درخت (بنية قلم *ریشه, bool (*action) (بنية قلم *))
{
  إذا (ریشه->راست)
    recurse_درخت (ریشه->راست, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ساكن _Noreturn فراغ
tsort (حرف ثابت *file)
{
  bool ok = true;
  بنية قلم *j = عدم;
  بنية قلم *k = عدم;
  token_میانگیر tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  بنية قلم *ریشه = new_قلم (عدم);

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

      k = جستجو_قلم (ریشه, tokenbuffer.میانگیر);
      إذا (j)
        {
          /* T3. Record the relation.  */
          رکورد_رابطه (j, k);
          k = عدم;
        }

      j = k;
    }

  إذا (k != عدم)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_درخت (ریشه, شمار_اقلام);

  طالما (n_رشتهها > 0)
    {
      /* T4. Scan for zeros.  */
      walk_درخت (ریشه, پیمایش_zeros);

      طالما (سر)
        {
          بنية successor *p = سر->بالاترین;

          /* T5. Output front of queue.  */
          puts (سر->str);
          سر->printed = true;
          n_رشتهها--;

          /* T6. Erase relations.  */
          طالما (p)
            {
              p->suc->شمار--;
              إذا (p->suc->شمار == 0)
                {
                  zeros->qlink = p->suc;
                  zeros = p->suc;
                }

              p = p->بعدی;
            }

          /* T7. Remove from queue.  */
          سر = سر->qlink;
        }

      /* T8.  End of process.  */
      إذا (n_رشتهها > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          افعل
            walk_درخت (ریشه, detect_loop);
          طالما (loop);
        }
    }

  إذا (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  خروج (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

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
