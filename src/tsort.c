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
#تعریف_کرو پروگرام_NAME "tsort"

#تعریف_کرو AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#تعریف_کرو DELIM " \t\n"

/* Members of the list of successors.  */
ساخت successor
{
  ساخت شے *suc;
  ساخت successor *اگلا;
};

/* Each string is held in memory as the head of a list of successors.  */
ساخت شے
{
  حرف ثابت *str;
  ساخت شے *بایاں, *دایاں;
  علامتی حرف balance; /* -1, 0, or +1 */
  bool printed;
  size_t گنتی;
  ساخت شے *qlink;
  ساخت successor *چوٹی;
};

/* The head of the sorted list.  */
ساکن ساخت شے *سر = تہی;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
ساکن ساخت شے *zeros = تہی;

/* Used for loop detection.  */
ساکن ساخت شے *loop = تہی;

/* The number of strings to sort.  */
ساکن size_t n_لڑیاں = 0;

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION] [FILE]\n\
Write totally ordered list consistent with the partial ordering اندر FILE.\n\
"), پروگرام_name);

      emit_stdin_note ();

      fputs (_("\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }

  باہر_نکلو (حالت_2);
}

/* Create a new item/node for STR.  */
ساکن ساخت شے *
new_شے (حرف ثابت *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  ساخت شے *k = xzalloc (حجم *k);
  اگر (str)
    k->str = xstrdup (str);
  واپس k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

ساکن ساخت شے *
تلاش_شے (ساخت شے *جڑ, حرف ثابت *str)
{
  ساخت شے *p, *q, *r, *s, *t;
  صحیح a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  اگر (جڑ->دایاں == تہی)
    واپس (جڑ->دایاں = new_شے (str));

  /* A1. Initialize.  */
  t = جڑ;
  s = p = جڑ->دایاں;

  جب (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      اگر (a == 0)
        واپس p;

      /* A3 & A4.  Move left & right.  */
      اگر (a < 0)
        q = p->بایاں;
      ورنہ
        q = p->دایاں;

      اگر (q == تہی)
        {
          /* A5. Insert.  */
          q = new_شے (str);

          /* A3 & A4.  (continued).  */
          اگر (a < 0)
            p->بایاں = q;
          ورنہ
            p->دایاں = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          اگر (a < 0)
            {
              r = p = s->بایاں;
              a = -1;
            }
          ورنہ
            {
              affirm (0 < a);
              r = p = s->دایاں;
              a = 1;
            }

          جب (p != q)
            {
              صحیح cmp = strcmp (str, p->str);
              اگر (cmp < 0)
                {
                  p->balance = -1;
                  p = p->بایاں;
                }
              ورنہ
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->دایاں;
                }
            }

          /* A7. Balancing act.  */
          اگر (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              واپس q;
            }

          اگر (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              اگر (a < 0)
                {
                  s->بایاں = r->دایاں;
                  r->دایاں = s;
                }
              ورنہ
                {
                  s->دایاں = r->بایاں;
                  r->بایاں = s;
                }
              s->balance = r->balance = 0;
            }
          ورنہ
            {
              /* A9. Double rotation.  */
              اگر (a < 0)
                {
                  p = r->دایاں;
                  r->دایاں = p->بایاں;
                  p->بایاں = r;
                  s->بایاں = p->دایاں;
                  p->دایاں = s;
                }
              ورنہ
                {
                  p = r->بایاں;
                  r->بایاں = p->دایاں;
                  p->دایاں = r;
                  s->دایاں = p->بایاں;
                  p->بایاں = s;
                }

              s->balance = 0;
              r->balance = 0;
              اگر (p->balance == a)
                s->balance = -a;
              ورنہ اگر (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          اگر (s == t->دایاں)
            t->دایاں = p;
          ورنہ
            t->بایاں = p;

          واپس q;
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

ساکن خالی
ریکارڈ_رشتہ (ساخت شے *j, ساخت شے *k)
{
  ساخت successor *p;

  اگر (!streq (j->str, k->str))
    {
      k->گنتی++;
      p = xmalloc (حجم *p);
      p->suc = k;
      p->اگلا = j->چوٹی;
      j->چوٹی = p;
    }
}

ساکن bool
گنتی_اشیاء (MAYBE_غیر_مستعمل ساخت شے *غیر_مستعمل)
{
  n_لڑیاں++;
  واپس false;
}

ساکن bool
جائزہ_zeros (ساخت شے *k)
{
  /* Ignore strings that have already been printed.  */
  اگر (k->گنتی == 0 && !k->printed)
    {
      اگر (سر == تہی)
        سر = k;
      ورنہ
        zeros->qlink = k;

      zeros = k;
    }

  واپس false;
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

ساکن bool
detect_loop (ساخت شے *k)
{
  اگر (k->گنتی > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      اگر (loop == تہی)
        /* Start traversing the graph at K.  */
        loop = k;
      ورنہ
        {
          ساخت successor **p = &k->چوٹی;

          جب (*p)
            {
              اگر ((*p)->suc == loop)
                {
                  اگر (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      جب (loop)
                        {
                          ساخت شے *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          اگر (loop == k)
                            {
                              /* Remove relation.  */
                              ساخت successor *s = *p;
                              s->suc->گنتی--;
                              *p = s->اگلا;
                              IF_LINT (آزاد (s));
                              توڑو;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = تہی;
                          loop = tmp;
                        }

                      جب (loop)
                        {
                          ساخت شے *tmp = loop->qlink;

                          loop->qlink = تہی;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      واپس true;
                    }
                  ورنہ
                    {
                      k->qlink = loop;
                      loop = k;
                      توڑو;
                    }
                }

              p = &(*p)->اگلا;
            }
        }
    }

  واپس false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

ساکن bool
recurse_درخت (ساخت شے *جڑ, bool (*action) (ساخت شے *))
{
  اگر (جڑ->بایاں == تہی && جڑ->دایاں == تہی)
    واپس (*action) (جڑ);
  ورنہ
    {
      اگر (جڑ->بایاں != تہی)
        اگر (recurse_درخت (جڑ->بایاں, action))
          واپس true;
      اگر ((*action) (جڑ))
        واپس true;
      اگر (جڑ->دایاں != تہی)
        اگر (recurse_درخت (جڑ->دایاں, action))
          واپس true;
    }

  واپس false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

ساکن خالی
walk_درخت (ساخت شے *جڑ, bool (*action) (ساخت شے *))
{
  اگر (جڑ->دایاں)
    recurse_درخت (جڑ->دایاں, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

ساکن _Noreturn خالی
tsort (حرف ثابت *file)
{
  bool ok = true;
  ساخت شے *j = تہی;
  ساخت شے *k = تہی;
  token_بفر tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  ساخت شے *جڑ = new_شے (تہی);

  اگر (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  جب (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, حجم (DELIM) - 1, &tokenbuffer);
      اگر (len == (size_t) -1)
        {
          اگر (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          توڑو;
        }

      affirm (len != 0);

      k = تلاش_شے (جڑ, tokenbuffer.بفر);
      اگر (j)
        {
          /* T3. Record the relation.  */
          ریکارڈ_رشتہ (j, k);
          k = تہی;
        }

      j = k;
    }

  اگر (k != تہی)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_درخت (جڑ, گنتی_اشیاء);

  جب (n_لڑیاں > 0)
    {
      /* T4. Scan for zeros.  */
      walk_درخت (جڑ, جائزہ_zeros);

      جب (سر)
        {
          ساخت successor *p = سر->چوٹی;

          /* T5. Output front of queue.  */
          puts (سر->str);
          سر->printed = true;
          n_لڑیاں--;

          /* T6. Erase relations.  */
          جب (p)
            {
              p->suc->گنتی--;
              اگر (p->suc->گنتی == 0)
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
      اگر (n_لڑیاں > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          کرو
            walk_درخت (جڑ, detect_loop);
          جب (loop);
        }
    }

  اگر (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  باہر_نکلو (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب (true)
    {
      ساکن ساخت option ثابت long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_نسخہ_OPTION_DECL},
          {تہی, 0, تہی, 0}
        };
      صحیح c = getopt_long (argc, argv, "w", long_options, تہی);

      اگر (c == -1)
        توڑو;

      انتخاب (c)
        {
        حالت 'w':
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
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
