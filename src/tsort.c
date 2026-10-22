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

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "readtokens.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tsort"

#define AUTHORS proper_name ("Mark Kettenis")

/* Token delimiters when reading from a file.  */
#define DELIM " \t\n"

/* Members of the list of successors.  */
संरचना successor
{
  संरचना item *suc;
  संरचना successor *next;
};

/* Each string is held in memory as the head of a list of successors.  */
संरचना item
{
  वर्ण अचर *str;
  संरचना item *बायाँ, *दायाँ;
  चिन्हित वर्ण balance; /* -1, 0, or +1 */
  bool printed;
  माप_प्रकार गन्ती;
  संरचना item *qlink;
  संरचना successor *शीर्ष;
};

/* The head of the sorted list.  */
स्थिर संरचना item *head = रिक्त;

/* The tail of the list of 'zeros', strings that have no predecessors.  */
स्थिर संरचना item *zeros = रिक्त;

/* Used for loop detection.  */
स्थिर संरचना item *loop = रिक्त;

/* The number of strings to sort.  */
स्थिर माप_प्रकार n_strings = 0;

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
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
स्थिर संरचना item *
new_item (वर्ण अचर *str)
{
  /* T1. Initialize (COUNT[k] <- 0 and TOP[k] <- ^).  */
  संरचना item *k = xzalloc (परिमाण *k);
  यदि (str)
    k->str = xstrdup (str);
  फर्काऊ k;
}

/* Search binary tree rooted at *ROOT for STR.  Allocate a new tree if
   *ROOT is null.  Insert a node/item for STR if not found.  Return
   the node/item found/created for STR.

   This is done according to Algorithm A (Balanced tree search and
   insertion) in Donald E. Knuth, The Art of Computer Programming,
   Volume 3/Searching and Sorting, pages 455--457.  */

स्थिर संरचना item *
खोज_item (संरचना item *जरा, वर्ण अचर *str)
{
  संरचना item *p, *q, *r, *s, *t;
  पूर्णाङ्क a;

  /* Make sure the tree is not empty, since that is what the algorithm
     below expects.  */
  यदि (जरा->दायाँ == रिक्त)
    फर्काऊ (जरा->दायाँ = new_item (str));

  /* A1. Initialize.  */
  t = जरा;
  s = p = जरा->दायाँ;

  जबसम्म (true)
    {
      /* A2. Compare.  */
      a = म_भेद (str, p->str);
      यदि (a == 0)
        फर्काऊ p;

      /* A3 & A4.  Move left & right.  */
      यदि (a < 0)
        q = p->बायाँ;
      अन्यथा
        q = p->दायाँ;

      यदि (q == रिक्त)
        {
          /* A5. Insert.  */
          q = new_item (str);

          /* A3 & A4.  (continued).  */
          यदि (a < 0)
            p->बायाँ = q;
          अन्यथा
            p->दायाँ = q;

          /* A6. Adjust balance factors.  */
          a = म_भेद (str, s->str);
          यदि (a < 0)
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

          जबसम्म (p != q)
            {
              पूर्णाङ्क cmp = म_भेद (str, p->str);
              यदि (cmp < 0)
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
          यदि (s->balance == 0 || s->balance == -a)
            {
              s->balance += a;
              फर्काऊ q;
            }

          यदि (r->balance == a)
            {
              /* A8. Single Rotation.  */
              p = r;
              यदि (a < 0)
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
              यदि (a < 0)
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
              यदि (p->balance == a)
                s->balance = -a;
              अन्यथा यदि (p->balance == -a)
                r->balance = a;
              p->balance = 0;
            }

          /* A10. Finishing touch.  */
          यदि (s == t->दायाँ)
            t->दायाँ = p;
          अन्यथा
            t->बायाँ = p;

          फर्काऊ q;
        }

      /* A3 & A4.  (continued).  */
      यदि (q->balance)
        {
          t = p;
          s = q;
        }

      p = q;
    }

  affirm (false);
}

/* Record the fact that J precedes K.  */

स्थिर शून्य
record_relation (संरचना item *j, संरचना item *k)
{
  संरचना successor *p;

  यदि (!streq (j->str, k->str))
    {
      k->गन्ती++;
      p = xmalloc (परिमाण *p);
      p->suc = k;
      p->next = j->शीर्ष;
      j->शीर्ष = p;
    }
}

स्थिर bool
गन्ती_वस्तुहरू (MAYBE_UNUSED संरचना item *unused)
{
  n_strings++;
  फर्काऊ false;
}

स्थिर bool
scan_zeros (संरचना item *k)
{
  /* Ignore strings that have already been printed.  */
  यदि (k->गन्ती == 0 && !k->printed)
    {
      यदि (head == रिक्त)
        head = k;
      अन्यथा
        zeros->qlink = k;

      zeros = k;
    }

  फर्काऊ false;
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

स्थिर bool
detect_loop (संरचना item *k)
{
  यदि (k->गन्ती > 0)
    {
      /* K does not have to be part of a cycle.  It is however part of
         a graph that contains a cycle.  */

      यदि (loop == रिक्त)
        /* Start traversing the graph at K.  */
        loop = k;
      अन्यथा
        {
          संरचना successor **p = &k->शीर्ष;

          जबसम्म (*p)
            {
              यदि ((*p)->suc == loop)
                {
                  यदि (k->qlink)
                    {
                      /* We have found a loop.  Retrace our steps.  */
                      जबसम्म (loop)
                        {
                          संरचना item *tmp = loop->qlink;

                          error (0, 0, "%s", (loop->str));

                          /* Until we encounter K again.  */
                          यदि (loop == k)
                            {
                              /* Remove relation.  */
                              संरचना successor *s = *p;
                              s->suc->गन्ती--;
                              *p = s->next;
                              IF_LINT (मुक्त (s));
                              रोक;
                            }

                          /* Tidy things up since we might have to
                             detect another loop.  */
                          loop->qlink = रिक्त;
                          loop = tmp;
                        }

                      जबसम्म (loop)
                        {
                          संरचना item *tmp = loop->qlink;

                          loop->qlink = रिक्त;
                          loop = tmp;
                        }

                      /* Since we have found the loop, stop walking
                         the tree.  */
                      फर्काऊ true;
                    }
                  अन्यथा
                    {
                      k->qlink = loop;
                      loop = k;
                      रोक;
                    }
                }

              p = &(*p)->next;
            }
        }
    }

  फर्काऊ false;
}

/* Recurse (sub)tree rooted at ROOT, calling ACTION for each node.
   Stop when ACTION returns true.  */

स्थिर bool
recurse_रूख (संरचना item *जरा, bool (*action) (संरचना item *))
{
  यदि (जरा->बायाँ == रिक्त && जरा->दायाँ == रिक्त)
    फर्काऊ (*action) (जरा);
  अन्यथा
    {
      यदि (जरा->बायाँ != रिक्त)
        यदि (recurse_रूख (जरा->बायाँ, action))
          फर्काऊ true;
      यदि ((*action) (जरा))
        फर्काऊ true;
      यदि (जरा->दायाँ != रिक्त)
        यदि (recurse_रूख (जरा->दायाँ, action))
          फर्काऊ true;
    }

  फर्काऊ false;
}

/* Walk the tree specified by the head ROOT, calling ACTION for
   each node.  */

स्थिर शून्य
walk_रूख (संरचना item *जरा, bool (*action) (संरचना item *))
{
  यदि (जरा->दायाँ)
    recurse_रूख (जरा->दायाँ, action);
}

/* Do a topological sort on FILE.  Exit with appropriate exit status.  */

स्थिर _Noreturn शून्य
tsort (वर्ण अचर *file)
{
  bool ok = true;
  संरचना item *j = रिक्त;
  संरचना item *k = रिक्त;
  token_buffer tokenbuffer;
  bool is_stdin = streq (file, "-");

  /* Initialize the head of the tree holding the strings we're sorting.  */
  संरचना item *जरा = new_item (रिक्त);

  यदि (!is_stdin && ! ख_व_खोलो (file, "r", मानक_निवेश))
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (file));

  fadvise (मानक_निवेश, FADVISE_SEQUENTIAL);

  init_tokenbuffer (&tokenbuffer);

  जबसम्म (true)
    {
      /* T2. Next Relation.  */
      माप_प्रकार len = readtoken (मानक_निवेश, DELIM, परिमाण (DELIM) - 1, &tokenbuffer);
      यदि (len == (माप_प्रकार) -1)
        {
          यदि (ख_त्रुटि (मानक_निवेश))
            error (निकास_त्रुटि, त्रुटि_सं, _("%s: read error"), quotef (file));
          रोक;
        }

      affirm (len != 0);

      k = खोज_item (जरा, tokenbuffer.buffer);
      यदि (j)
        {
          /* T3. Record the relation.  */
          record_relation (j, k);
          k = रिक्त;
        }

      j = k;
    }

  यदि (k != रिक्त)
    error (निकास_त्रुटि, 0, _("%s: input contains an odd number of tokens"),
           quotef (file));

  /* T1. Initialize (N <- n).  */
  walk_रूख (जरा, गन्ती_वस्तुहरू);

  जबसम्म (n_strings > 0)
    {
      /* T4. Scan for zeros.  */
      walk_रूख (जरा, scan_zeros);

      जबसम्म (head)
        {
          संरचना successor *p = head->शीर्ष;

          /* T5. Output front of queue.  */
          माला_दो (head->str);
          head->printed = true;
          n_strings--;

          /* T6. Erase relations.  */
          जबसम्म (p)
            {
              p->suc->गन्ती--;
              यदि (p->suc->गन्ती == 0)
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
      यदि (n_strings > 0)
        {
          /* The input contains a loop.  */
          error (0, 0, _("%s: input contains a loop:"), quotef (file));
          ok = false;

          /* Print the loop and remove a relation to break it.  */
          गर
            walk_रूख (जरा, detect_loop);
          जबसम्म (loop);
        }
    }

  यदि (ख_बंद (मानक_निवेश) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s",
           is_stdin ? _("standard input") : quotef (file));

  निकास (ok ? निकास_सफल : निकास_त्रुटि);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबसम्म (true)
    {
      स्थिर संरचना option अचर long_options[] =
        {
          {GETOPT_HELP_OPTION_DECL},
          {GETOPT_VERSION_OPTION_DECL},
          {रिक्त, 0, रिक्त, 0}
        };
      पूर्णाङ्क c = getopt_long (argc, argv, "w", long_options, रिक्त);

      यदि (c == -1)
        रोक;

      विकल्प (c)
        {
        पक्ष 'w':
          रोक;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  यदि (1 < argc - optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (निकास_त्रुटि);
    }

  tsort (optind == argc ? "-" : argv[optind]);
}
