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

#समावेश <config.h>

#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "fadvise.h"
#समावेश "readtokens.h"
#समावेश "stdio--.h"
#समावेश "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "tsort"

#घोषणा AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#घोषणा DELIM " \t\n"

/* Members of the list of successors.  */
काष्ठा successor
{
  काष्ठा item *suc;
  काष्ठा successor *अग्र;
};

/* Each string is held in memory as the head of a list of successors.  */
काष्ठा item
{
  अक्षर स्थिर *str;
  काष्ठा item *बायाँ, *दायाँ;
  चिन्हित अक्षर balance; /* -1, 0, or +1 */
  bool printed;
  माप_प्रकार गिनती;
  काष्ठा item *qlink;
  काष्ठा successor *शीर्ष_2;
};

/* The head of the sorted list.  */
अटल काष्ठा item *शीर्ष = शून्य;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
अटल काष्ठा item *zeros = शून्य;

/* Used for loop detection.  */
अटल काष्ठा item *loop = शून्य;

/* The number of strings to sort.  */
अटल माप_प्रकार n_strings = 0;

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION] [खाता]\n\
Write totally ordered list consistent with the partial ordering in खाता.\n\
"), program_name);

      emit_stdin_note ();

      ख_माला_दो (_("\
\n\
"), मानक_निकास);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

/* Create a new item/node for STR.  */
अटल काष्ठा item *
new_item (अक्षर स्थिर *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  काष्ठा item *k = xzalloc (माप *k);
  अगर (str)
    k->str = xstrdup (str);
  वापस k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

अटल काष्ठा item *
खोज_item (काष्ठा item *मूल, अक्षर स्थिर *str)
{
  काष्ठा item *p, *q, *r, *s, *t;
  पूर्णांक a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  अगर (मूल->दायाँ == शून्य)
    वापस (मूल->दायाँ = new_item (str));

  /* A1. Initialize.  */
  t = मूल;
  s = p = मूल->दायाँ;

  जबतक (true)
    {
      /* A2. Compare.  */
      a = म_भेद (str, p->str);
      अगर (a == 0)
        वापस p;

      /* A3 & A4.  Move left & right.  */
      अगर (a < 0)
        q = p->बायाँ;
      अन्यथा
        q = p->दायाँ;

      अगर (q == शून्य)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          अगर (a < 0)
            p->बायाँ = q;
          अन्यथा
            p->दायाँ = q;

          /* A6. Adjust balance factors.  */
          a = म_भेद (str, s->str);
          अगर (a < 0)
            {
              r = p = s->बायाँ;
              a = -1;
            }
          अन्यथा
            {
              affirm (0 < a);
              r = p = s->दायाँ;
              a = 1;
            }

          जबतक (p != q)
            {
              पूर्णांक cmp = म_भेद (str, p->str);
              अगर (cmp < 0)
                {
                  p->balance = -1;
                  p = p->बायाँ;
                }
              अन्यथा
                {
                  affirm (0 < cmp);
                  p->balance = 1;
                  p = p->दायाँ;
                }
            }

          /* A7. Balancing act.  */
          अगर (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              वापस q;
            }

          अगर (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              अगर (a < 0)
                {
                  s->बायाँ = r->दायाँ;
                  r->दायाँ = s;
                }
              अन्यथा
                {
                  s->दायाँ = r->बायाँ;
                  r->बायाँ = s;
                }
              s->balance = r->balance = 0;
            }
          अन्यथा
            {
              /* A9. Double rotation.  */
              अगर (a < 0)
                {
                  p = r->दायाँ;
                  r->दायाँ = p->बायाँ;
                  p->बायाँ = r;
                  s->बायाँ = p->दायाँ;
                  p->दायाँ = s;
                }
              अन्यथा
                {
                  p = r->बायाँ;
                  r->बायाँ = p->दायाँ;
                  p->दायाँ = r;
                  s->दायाँ = p->बायाँ;
                  p->बायाँ = s;
                }

              s->balance = 0;
              r->balance = 0;
              अगर (p->balance == a)
                s->balance = -a;
              अन्यथा अगर (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          अगर (s == t->दायाँ)
            t->दायाँ = p;
          अन्यथा
            t->बायाँ = p;

          वापस q;
        }

      /* A3 & A4.  (continued).  */
      अगर (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

अटल व्योम
record_relation (काष्ठा item *j, काष्ठा item *k)
{
  काष्ठा successor *p;

  अगर (!streq (j->str, k->str))
    {
      k->गिनती++;
      p = xmalloc (माप *p);
      p->suc = k;
      p->अग्र = j->शीर्ष_2;
      j->शीर्ष_2 = p;
    }
}

अटल bool
गिनती_वस्तुएँ (MAYBE_UNUSED काष्ठा item *unused)
{
  n_strings++;
  वापस false;
}

अटल bool
scan_zeros (काष्ठा item *k)
{
  /* Ignore strings that have already been printed.  */
  अगर (k->गिनती == 0 && !k->printed)
    {
      अगर (शीर्ष == शून्य)
        शीर्ष = k;
      अन्यथा
        zeros->qlink = k;

      zeros = k;
    }

  वापस false;
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

अटल bool
detect_loop (काष्ठा item *k)
{
  अगर (k->गिनती > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      अगर (loop == शून्य)
        /* Start traversing the graph at K.  */
        loop = k;
      अन्यथा
        {
          काष्ठा successor **p = &k->शीर्ष_2;

          जबतक (*p)
            {
              अगर ((*p)->suc == loop)
                {
                  अगर (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      जबतक (loop)
                        {
                          काष्ठा item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          अगर (loop == k)
                            {
                              /* Remove relation.  */
                              काष्ठा successor *s = *p;
                              s->suc->गिनती--;
                              *p = s->अग्र;
                              IF_LINT (मुक्त (s));
                              अवरोध;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = शून्य;
                          loop = tmp;
                        }

                      जबतक (loop)
                        {
                          काष्ठा item *tmp = loop->qlink;

                          loop->qlink = शून्य;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      वापस true;
                    }
                  अन्यथा
                    {
                      k->qlink = loop;
                      loop = k;
                      अवरोध;
                    }
                }

              p = &(*p)->अग्र;
            }
        }
    }

  वापस false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

अटल bool
recurse_वृक्ष (काष्ठा item *मूल, bool (*action) (काष्ठा item *))
{
  अगर (मूल->बायाँ == शून्य && मूल->दायाँ == शून्य)
    वापस (*action) (मूल);
  अन्यथा
    {
      अगर (मूल->बायाँ != शून्य)
        अगर (recurse_वृक्ष (मूल->बायाँ, action))
          वापस true;
      अगर ((*action) (मूल))
        वापस true;
      अगर (मूल->दायाँ != शून्य)
        अगर (recurse_वृक्ष (मूल->दायाँ, action))
          वापस true;
    }

  वापस false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

अटल व्योम
walk_वृक्ष (काष्ठा item *मूल, bool (*action) (काष्ठा item *))
{
  अगर (मूल->दायाँ)
    recurse_वृक्ष (मूल->दायाँ, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

अटल _Noreturn व्योम
tsort (अक्षर स्थिर *file)
{
  bool ok = true;
  काष्ठा item *j = शून्य;
  काष्ठा item *k = शून्य;
  token_बफर tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  काष्ठा item *मूल = new_item (शून्य);

  अगर (!is_stdin && ! ख_व_खोलो (file, "r", मानक_निवेश))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (file));

  fadvise (मानक_निवेश, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  जबतक (true)
    {
      /* T2. Next Relation.  */
      माप_प्रकार len = readtoken (मानक_निवेश, DELIM, माप (DELIM) - 1, &tokenbuffer);
      अगर (len == (माप_प्रकार) -1)
        {
          अगर (ख_त्रुटि (मानक_निवेश))
            error (निकास_त्रुटि, त्रुटि_सं, _("%s: read error"), quotef (file));
          अवरोध;
        }

      affirm (len != 0);

      k = खोज_item (मूल, tokenbuffer.बफर);
      अगर (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = शून्य;
        }

      j = k;
    }

  अगर (k != शून्य)
    error (निकास_त्रुटि, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_वृक्ष (मूल, गिनती_वस्तुएँ);

  जबतक (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_वृक्ष (मूल, scan_zeros);

      जबतक (शीर्ष)
        {
          काष्ठा successor *p = शीर्ष->शीर्ष_2;

          /* T5. Output front of queue.  */
          माला_दो (शीर्ष->str);
          शीर्ष->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          जबतक (p)
            {
              p->suc->गिनती--;
              अगर (p->suc->गिनती == 0)
                {
                  zeros->qlink = p->suc;
                  zeros = p->suc;
                }

              p = p->अग्र;
            }

          /* T7. Remove from queue.  */
          शीर्ष = शीर्ष->qlink;
        }

      /* T8.  End of process.  */
      अगर (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          करो
            walk_वृक्ष (मूल, detect_loop);
          जबतक (loop);
        }
    }

  अगर (ख_बंद (मानक_निवेश) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s",
           is_stdin ? _("standard input") : quotef (file));

  निकास (ok ? निकास_सफल : निकास_त्रुटि);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक (true)
    {
      अटल काष्ठा option स्थिर long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {शून्य, 0, शून्य, 0}
        };
      पूर्णांक c = getopt_long (argc, argv, "w", long_options, शून्य);

      अगर (c == -1)
        अवरोध;

      चयन (c)
        {
        हाल 'w':
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (निकास_त्रुटि);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
