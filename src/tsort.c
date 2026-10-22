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
#وصف_ڪريو پروگرام_NAME "tsort"

#وصف_ڪريو AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#وصف_ڪريو DELIM " \t\n"

/* Members of the list of successors.  */
بنية successor
{
  بنية شي *suc;
  بنية successor *ايندڙ;
};

/* Each string is held in memory as the head of a list of successors.  */
بنية شي
{
  حرف ثابت *str;
  بنية شي *کاٻو, *ساڄو;
  نشانوارو حرف balance; /* -1, 0, or +1 */
  bool printed;
  size_t ڳڻپ;
  بنية شي *qlink;
  بنية successor *چوٽي;
};

/* The head of the sorted list.  */
ساكن بنية شي *سر = عدم;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ساكن بنية شي *zeros = عدم;

/* Used for loop detection.  */
ساكن بنية شي *loop = عدم;

/* The number of strings to sort.  */
ساكن size_t n_لڙيون = 0;

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION] [FILE]\n\
Write totally ordered list consistent with the partial ordering اندريون FILE.\n\
"), پروگرام_name);

      emit_stdin_note ();

      fputs (_("\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }

  ٻاهر_نڪرو (حالت);
}

/* Create a new item/node for STR.  */
ساكن بنية شي *
new_شي (حرف ثابت *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  بنية شي *k = xzalloc (حجم *k);
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

ساكن بنية شي *
ڳولا_شي (بنية شي *پاڙ, حرف ثابت *str)
{
  بنية شي *p, *q, *r, *s, *t;
  صحيح a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  إذا (پاڙ->ساڄو == عدم)
    أرجع (پاڙ->ساڄو = new_شي (str));

  /* A1. Initialize.  */
  t = پاڙ;
  s = p = پاڙ->ساڄو;

  طالما (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      إذا (a == 0)
        أرجع p;

      /* A3 & A4.  Move left & right.  */
      إذا (a < 0)
        q = p->کاٻو;
      وإلا
        q = p->ساڄو;

      إذا (q == عدم)
        {
          /* A5. Insert.  */
          q = new_شي (str);

          /* A3 & A4.  (continued).  */
          إذا (a < 0)
            p->کاٻو = q;
          وإلا
            p->ساڄو = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          إذا (a < 0)
            {
              r = p = s->کاٻو;
              a = -1;
            }
          وإلا
            {
              affirm (0 < a);
              r = p = s->ساڄو;
              a = 1;
            }

          طالما (p != q)
            {
              صحيح cmp = strcmp (str, p->str);
              إذا (cmp < 0)
                {
                  p->balance = -1;
                  p = p->کاٻو;
                }
              وإلا
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->ساڄو;
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
                  s->کاٻو = r->ساڄو;
                  r->ساڄو = s;
                }
              وإلا
                {
                  s->ساڄو = r->کاٻو;
                  r->کاٻو = s;
                }
              s->balance = r->balance = 0;
            }
          وإلا
            {
              /* A9. Double rotation.  */
              إذا (a < 0)
                {
                  p = r->ساڄو;
                  r->ساڄو = p->کاٻو;
                  p->کاٻو = r;
                  s->کاٻو = p->ساڄو;
                  p->ساڄو = s;
                }
              وإلا
                {
                  p = r->کاٻو;
                  r->کاٻو = p->ساڄو;
                  p->ساڄو = r;
                  s->ساڄو = p->کاٻو;
                  p->کاٻو = s;
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
          إذا (s == t->ساڄو)
            t->ساڄو = p;
          وإلا
            t->کاٻو = p;

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
رڪارڊ_لاڳاپو (بنية شي *j, بنية شي *k)
{
  بنية successor *p;

  إذا (!streq (j->str, k->str))
    {
      k->ڳڻپ++;
      p = xmalloc (حجم *p);
      p->suc = k;
      p->ايندڙ = j->چوٽي;
      j->چوٽي = p;
    }
}

ساكن bool
ڳڻپ_شيون (MAYBE_اڻ_استعمال_ٿيل بنية شي *اڻ_استعمال_ٿيل)
{
  n_لڙيون++;
  أرجع false;
}

ساكن bool
جاچ_zeros (بنية شي *k)
{
  /* Ignore strings that have already been printed.  */
  إذا (k->ڳڻپ == 0 && !k->printed)
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
detect_loop (بنية شي *k)
{
  إذا (k->ڳڻپ > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      إذا (loop == عدم)
        /* Start traversing the graph at K.  */
        loop = k;
      وإلا
        {
          بنية successor **p = &k->چوٽي;

          طالما (*p)
            {
              إذا ((*p)->suc == loop)
                {
                  إذا (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      طالما (loop)
                        {
                          بنية شي *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          إذا (loop == k)
                            {
                              /* Remove relation.  */
                              بنية successor *s = *p;
                              s->suc->ڳڻپ--;
                              *p = s->ايندڙ;
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
                          بنية شي *tmp = loop->qlink;

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

              p = &(*p)->ايندڙ;
            }
        }
    }

  أرجع false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ساكن bool
recurse_وڻ (بنية شي *پاڙ, bool (*action) (بنية شي *))
{
  إذا (پاڙ->کاٻو == عدم && پاڙ->ساڄو == عدم)
    أرجع (*action) (پاڙ);
  وإلا
    {
      إذا (پاڙ->کاٻو != عدم)
        إذا (recurse_وڻ (پاڙ->کاٻو, action))
          أرجع true;
      إذا ((*action) (پاڙ))
        أرجع true;
      إذا (پاڙ->ساڄو != عدم)
        إذا (recurse_وڻ (پاڙ->ساڄو, action))
          أرجع true;
    }

  أرجع false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ساكن فراغ
walk_وڻ (بنية شي *پاڙ, bool (*action) (بنية شي *))
{
  إذا (پاڙ->ساڄو)
    recurse_وڻ (پاڙ->ساڄو, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ساكن _Noreturn فراغ
tsort (حرف ثابت *file)
{
  bool ok = true;
  بنية شي *j = عدم;
  بنية شي *k = عدم;
  token_بفر tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  بنية شي *پاڙ = new_شي (عدم);

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

      k = ڳولا_شي (پاڙ, tokenbuffer.بفر);
      إذا (j)
        {
          /* T3. Record the relation.  */
          رڪارڊ_لاڳاپو (j, k);
          k = عدم;
        }

      j = k;
    }

  إذا (k != عدم)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_وڻ (پاڙ, ڳڻپ_شيون);

  طالما (n_لڙيون > 0)
    {
      /* T4. Scan for zeros.  */
      walk_وڻ (پاڙ, جاچ_zeros);

      طالما (سر)
        {
          بنية successor *p = سر->چوٽي;

          /* T5. Output front of queue.  */
          puts (سر->str);
          سر->printed = true;
          n_لڙيون--;

          /* T6. Erase relations.  */
          طالما (p)
            {
              p->suc->ڳڻپ--;
              إذا (p->suc->ڳڻپ == 0)
                {
                  zeros->qlink = p->suc;
                  zeros = p->suc;
                }

              p = p->ايندڙ;
            }

          /* T7. Remove from queue.  */
          سر = سر->qlink;
        }

      /* T8.  End of process.  */
      إذا (n_لڙيون > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          افعل
            walk_وڻ (پاڙ, detect_loop);
          طالما (loop);
        }
    }

  إذا (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  ٻاهر_نڪرو (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما (true)
    {
      ساكن بنية option ثابت long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_نسخو_OPTION_DECL},
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

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

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
