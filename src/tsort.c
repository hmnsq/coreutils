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

#అన్తర్భావ <config.h>

#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "readtokens.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "tsort"

#నిర్వచించు AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#నిర్వచించు DELIM " \t\n"

/* Members of the list of successors.  */
నిర్మాణం successor
{
  నిర్మాణం అంశం *suc;
  నిర్మాణం successor *తదుపరి;
};

/* Each string is held in memory as the head of a list of successors.  */
నిర్మాణం అంశం
{
  అక్షరం స్థిరరాశి *str;
  నిర్మాణం అంశం *ఎడమ, *కుడి;
  గుర్తుగల అక్షరం balance; /* -1, 0, or +1 */
  bool printed;
  size_t లెక్క;
  నిర్మాణం అంశం *qlink;
  నిర్మాణం successor *శిఖరం;
};

/* The head of the sorted list.  */
స్థిర నిర్మాణం అంశం *తల = ఖాళీ;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
స్థిర నిర్మాణం అంశం *zeros = ఖాళీ;

/* Used for loop detection.  */
స్థిర నిర్మాణం అంశం *loop = ఖాళీ;

/* The number of strings to sort.  */
స్థిర size_t n_అక్షరశ్రేణులు = 0;

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION] [FILE]\n\
Write totally ordered list consistent with the partial ordering లోపల FILE.\n\
"), కార్యక్రమం_name);

      emit_stdin_note ();

      fputs (_("\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }

  నిష్క్రమించు (స్థితి_2);
}

/* Create a new item/node for STR.  */
స్థిర నిర్మాణం అంశం *
new_అంశం (అక్షరం స్థిరరాశి *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  నిర్మాణం అంశం *k = xzalloc (పరిమాణం *k);
  ఒకవేళ (str)
    k->str = xstrdup (str);
  తిరిగి k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

స్థిర నిర్మాణం అంశం *
శోధన_అంశం (నిర్మాణం అంశం *మూలం, అక్షరం స్థిరరాశి *str)
{
  నిర్మాణం అంశం *p, *q, *r, *s, *t;
  పూర్ణాంకం a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  ఒకవేళ (మూలం->కుడి == ఖాళీ)
    తిరిగి (మూలం->కుడి = new_అంశం (str));

  /* A1. Initialize.  */
  t = మూలం;
  s = p = మూలం->కుడి;

  అయితే (true)
    {
      /* A2. Compare.  */
      a = strcmp (str, p->str);
      ఒకవేళ (a == 0)
        తిరిగి p;

      /* A3 & A4.  Move left & right.  */
      ఒకవేళ (a < 0)
        q = p->ఎడమ;
      లేకపోతే
        q = p->కుడి;

      ఒకవేళ (q == ఖాళీ)
        {
          /* A5. Insert.  */
          q = new_అంశం (str);

          /* A3 & A4.  (continued).  */
          ఒకవేళ (a < 0)
            p->ఎడమ = q;
          లేకపోతే
            p->కుడి = q;

          /* A6. Adjust balance factors.  */
          a = strcmp (str, s->str);
          ఒకవేళ (a < 0)
            {
              r = p = s->ఎడమ;
              a = -1;
            }
          లేకపోతే
            {
              affirm (0 < a);
              r = p = s->కుడి;
              a = 1;
            }

          అయితే (p != q)
            {
              పూర్ణాంకం cmp = strcmp (str, p->str);
              ఒకవేళ (cmp < 0)
                {
                  p->balance = -1;
                  p = p->ఎడమ;
                }
              లేకపోతే
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->కుడి;
                }
            }

          /* A7. Balancing act.  */
          ఒకవేళ (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              తిరిగి q;
            }

          ఒకవేళ (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              ఒకవేళ (a < 0)
                {
                  s->ఎడమ = r->కుడి;
                  r->కుడి = s;
                }
              లేకపోతే
                {
                  s->కుడి = r->ఎడమ;
                  r->ఎడమ = s;
                }
              s->balance = r->balance = 0;
            }
          లేకపోతే
            {
              /* A9. Double rotation.  */
              ఒకవేళ (a < 0)
                {
                  p = r->కుడి;
                  r->కుడి = p->ఎడమ;
                  p->ఎడమ = r;
                  s->ఎడమ = p->కుడి;
                  p->కుడి = s;
                }
              లేకపోతే
                {
                  p = r->ఎడమ;
                  r->ఎడమ = p->కుడి;
                  p->కుడి = r;
                  s->కుడి = p->ఎడమ;
                  p->ఎడమ = s;
                }

              s->balance = 0;
              r->balance = 0;
              ఒకవేళ (p->balance == a)
                s->balance = -a;
              లేకపోతే ఒకవేళ (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          ఒకవేళ (s == t->కుడి)
            t->కుడి = p;
          లేకపోతే
            t->ఎడమ = p;

          తిరిగి q;
        }

      /* A3 & A4.  (continued).  */
      ఒకవేళ (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

స్థిర శూన్య
రికార్డు_సంబంధం (నిర్మాణం అంశం *j, నిర్మాణం అంశం *k)
{
  నిర్మాణం successor *p;

  ఒకవేళ (!streq (j->str, k->str))
    {
      k->లెక్క++;
      p = xmalloc (పరిమాణం *p);
      p->suc = k;
      p->తదుపరి = j->శిఖరం;
      j->శిఖరం = p;
    }
}

స్థిర bool
లెక్క_వస్తువులు (MAYBE_వాడని నిర్మాణం అంశం *వాడని)
{
  n_అక్షరశ్రేణులు++;
  తిరిగి false;
}

స్థిర bool
పరిశీలన_zeros (నిర్మాణం అంశం *k)
{
  /* Ignore strings that have already been printed.  */
  ఒకవేళ (k->లెక్క == 0 && !k->printed)
    {
      ఒకవేళ (తల == ఖాళీ)
        తల = k;
      లేకపోతే
        zeros->qlink = k;

      zeros = k;
    }

  తిరిగి false;
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

స్థిర bool
detect_loop (నిర్మాణం అంశం *k)
{
  ఒకవేళ (k->లెక్క > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      ఒకవేళ (loop == ఖాళీ)
        /* Start traversing the graph at K.  */
        loop = k;
      లేకపోతే
        {
          నిర్మాణం successor **p = &k->శిఖరం;

          అయితే (*p)
            {
              ఒకవేళ ((*p)->suc == loop)
                {
                  ఒకవేళ (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      అయితే (loop)
                        {
                          నిర్మాణం అంశం *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          ఒకవేళ (loop == k)
                            {
                              /* Remove relation.  */
                              నిర్మాణం successor *s = *p;
                              s->suc->లెక్క--;
                              *p = s->తదుపరి;
                              IF_LINT (విడుదల (s));
                              ఆపు;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = ఖాళీ;
                          loop = tmp;
                        }

                      అయితే (loop)
                        {
                          నిర్మాణం అంశం *tmp = loop->qlink;

                          loop->qlink = ఖాళీ;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      తిరిగి true;
                    }
                  లేకపోతే
                    {
                      k->qlink = loop;
                      loop = k;
                      ఆపు;
                    }
                }

              p = &(*p)->తదుపరి;
            }
        }
    }

  తిరిగి false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

స్థిర bool
recurse_వృక్షం (నిర్మాణం అంశం *మూలం, bool (*action) (నిర్మాణం అంశం *))
{
  ఒకవేళ (మూలం->ఎడమ == ఖాళీ && మూలం->కుడి == ఖాళీ)
    తిరిగి (*action) (మూలం);
  లేకపోతే
    {
      ఒకవేళ (మూలం->ఎడమ != ఖాళీ)
        ఒకవేళ (recurse_వృక్షం (మూలం->ఎడమ, action))
          తిరిగి true;
      ఒకవేళ ((*action) (మూలం))
        తిరిగి true;
      ఒకవేళ (మూలం->కుడి != ఖాళీ)
        ఒకవేళ (recurse_వృక్షం (మూలం->కుడి, action))
          తిరిగి true;
    }

  తిరిగి false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

స్థిర శూన్య
walk_వృక్షం (నిర్మాణం అంశం *మూలం, bool (*action) (నిర్మాణం అంశం *))
{
  ఒకవేళ (మూలం->కుడి)
    recurse_వృక్షం (మూలం->కుడి, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

స్థిర _Noreturn శూన్య
tsort (అక్షరం స్థిరరాశి *file)
{
  bool ok = true;
  నిర్మాణం అంశం *j = ఖాళీ;
  నిర్మాణం అంశం *k = ఖాళీ;
  token_బఫరు tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  నిర్మాణం అంశం *మూలం = new_అంశం (ఖాళీ);

  ఒకవేళ (!is_stdin && ! freopen (file, "r", stdin))
    error (EXIT_FAILURE, errno, "%s", quotef (file));

  fadvise (stdin, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  అయితే (true)
    {
      /* T2. Next Relation.  */
      size_t len = readtoken (stdin, DELIM, పరిమాణం (DELIM) - 1, &tokenbuffer);
      ఒకవేళ (len == (size_t) -1)
        {
          ఒకవేళ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("%s: read error"), quotef (file));
          ఆపు;
        }

      affirm (len != 0);

      k = శోధన_అంశం (మూలం, tokenbuffer.బఫరు);
      ఒకవేళ (j)
        {
          /* T3. Record the relation.  */
          రికార్డు_సంబంధం (j, k);
          k = ఖాళీ;
        }

      j = k;
    }

  ఒకవేళ (k != ఖాళీ)
    error (EXIT_FAILURE, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_వృక్షం (మూలం, లెక్క_వస్తువులు);

  అయితే (n_అక్షరశ్రేణులు > 0)
    {
      /* T4. Scan for zeros.  */
      walk_వృక్షం (మూలం, పరిశీలన_zeros);

      అయితే (తల)
        {
          నిర్మాణం successor *p = తల->శిఖరం;

          /* T5. Output front of queue.  */
          puts (తల->str);
          తల->printed = true;
          n_అక్షరశ్రేణులు--;

          /* T6. Erase relations.  */
          అయితే (p)
            {
              p->suc->లెక్క--;
              ఒకవేళ (p->suc->లెక్క == 0)
                {
                  zeros->qlink = p->suc;
                  zeros = p->suc;
                }

              p = p->తదుపరి;
            }

          /* T7. Remove from queue.  */
          తల = తల->qlink;
        }

      /* T8.  End of process.  */
      ఒకవేళ (n_అక్షరశ్రేణులు > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          చేయి
            walk_వృక్షం (మూలం, detect_loop);
          అయితే (loop);
        }
    }

  ఒకవేళ (fclose (stdin) != 0)
    error (EXIT_FAILURE, errno, "%s",
           is_stdin ? _("standard input") : quotef (file));

  నిష్క్రమించు (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే (true)
    {
      స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_సంస్కరణ_OPTION_DECL},
          {ఖాళీ, 0, ఖాళీ, 0}
        };
      పూర్ణాంకం c = getopt_long (argc, argv, "w", long_options, ఖాళీ);

      ఒకవేళ (c == -1)
        ఆపు;

      ఎంపిక (c)
        {
        స్థితి 'w':
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
