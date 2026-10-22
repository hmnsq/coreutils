/* tac from a pipe.

   Copyright (C) 1997-2026 Free Software Foundation, Inc.

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

/* FIXME */
#అన్తర్భావ "assure.h"

/* FIXME: this is small for testing */
#నిర్వచించు బఫరు_కొలత (8)

#నిర్వచించు LEN(X, I) ((X)->p[(I)].one_past_చివర - (X)->p[(I)].ప్రారంభం)
#నిర్వచించు ఖాళీలు_2(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#నిర్వచించు ONE_PAST_చివర(X, I) ((X)->p[(I)].one_past_చివర)

నిర్మాణం పంక్తి_ptr
{
  size_t i;
  అక్షరం *ptr;
};
రకనిర్వచనం నిర్మాణం పంక్తి_ptr పంక్తి_ptr;

నిర్మాణం B_pair
{
  అక్షరం *ప్రారంభం;
  అక్షరం *one_past_చివర;
};

నిర్మాణం Buf
{
  size_t n_bufs;
  నిర్మాణం obstack obs;
  నిర్మాణం B_pair *p;
};
రకనిర్వచనం నిర్మాణం Buf Buf;

స్థిర bool
buf_init_from_stdin (Buf *x, అక్షరం eol_byte)
{
  bool చివరి_byte_is_eol_byte = true;
  bool ok = true;

#నిర్వచించు OBS (&(x->obs))
  obstack_init (OBS);

  అయితే (true)
    {
      అక్షరం *buf = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);

      ఒకవేళ (buf == ఖాళీ)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          ఆపు;
        }
      idx_t బైట్లు_read = నిండిన_read (STDIN_FILENO, buf, బఫరు_కొలత);
      ఒకవేళ (బైట్లు_read != బఫరు_కొలత && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        నిర్మాణం B_pair bp;
        bp.ప్రారంభం = buf;
        bp.one_past_చివర = buf + బైట్లు_read;
        obstack_grow (OBS, &bp, పరిమాణం (bp));
      }

      ఒకవేళ (బైట్లు_read != 0)
        చివరి_byte_is_eol_byte = (buf[బైట్లు_read - 1] == eol_byte);

      ఒకవేళ (బైట్లు_read < బఫరు_కొలత)
        ఆపు;
    }

  ఒకవేళ (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      ఒకవేళ (!చివరి_byte_is_eol_byte)
        {
          అక్షరం *buf = జ్ఞాపకం_ఇవ్వు (1);
          ఒకవేళ (buf == ఖాళీ)
            {
              /* FIXME: just like above */
              ok = false;
            }
          లేకపోతే
            {
              నిర్మాణం B_pair bp;
              *buf = eol_byte;
              bp.ప్రారంభం = buf;
              bp.one_past_చివర = buf + 1;
              obstack_grow (OBS, &bp, పరిమాణం (bp));
            }
        }
    }

  x->n_bufs = obstack_object_కొలత (OBS) / పరిమాణం (x->p[0]);
  x->p = (నిర్మాణం B_pair *) obstack_ముగింపు (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  ఒకవేళ (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].ప్రారంభం == x->p[x->n_bufs - 1].one_past_చివర)
    విడుదల (x->p[--(x->n_bufs)].ప్రారంభం);

  తిరిగి ok;
}

స్థిర శూన్య
buf_free (Buf *x)
{
  కొరకు (size_t i = 0; i < x->n_bufs; i++)
    విడుదల (x->p[i].ప్రారంభం);
  obstack_free (OBS, ఖాళీ);
}

పంక్తి_ptr
పంక్తి_ptr_decrement (స్థిరరాశి Buf *x, స్థిరరాశి పంక్తి_ptr *lp)
{
  పంక్తి_ptr lp_new;

  ఒకవేళ (lp->ptr > x->p[lp->i].ప్రారంభం)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  లేకపోతే
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_చివర (x, lp->i - 1) - 1;
    }
  తిరిగి lp_new;
}

పంక్తి_ptr
పంక్తి_ptr_increment (స్థిరరాశి Buf *x, స్థిరరాశి పంక్తి_ptr *lp)
{
  పంక్తి_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_చివర (x, lp->i) - 1);
  ఒకవేళ (lp->ptr < ONE_PAST_చివర (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  లేకపోతే
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].ప్రారంభం;
    }
  తిరిగి lp_new;
}

స్థిర bool
కనుగొను_bol (స్థిరరాశి Buf *x,
          స్థిరరాశి పంక్తి_ptr *చివరి_bol, పంక్తి_ptr *new_bol, అక్షరం eol_byte)
{
  size_t i;
  పంక్తి_ptr tmp;
  అక్షరం *చివరి_bol_ptr;

  ఒకవేళ (చివరి_bol->ptr == x->p[0].ప్రారంభం)
    తిరిగి false;

  tmp = పంక్తి_ptr_decrement (x, చివరి_bol);
  చివరి_bol_ptr = tmp.ptr;
  i = tmp.i;
  అయితే (true)
    {
      అక్షరం *nl = memrchr (x->p[i].ప్రారంభం, చివరి_bol_ptr, eol_byte);
      ఒకవేళ (nl)
        {
          పంక్తి_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = పంక్తి_ptr_increment (x, &nl_pos);
          తిరిగి true;
        }

      ఒకవేళ (i == 0)
        ఆపు;

      --i;
      చివరి_bol_ptr = ONE_PAST_చివర (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  ఒకవేళ (చివరి_bol->ptr != x->p[0].ప్రారంభం)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].ప్రారంభం;
      తిరిగి true;
    }

  తిరిగి false;
}

స్థిర శూన్య
print_పంక్తి (FILE *బహిర్గతం_ప్రవాహం, స్థిరరాశి Buf *x,
            స్థిరరాశి పంక్తి_ptr *bol, స్థిరరాశి పంక్తి_ptr *bol_తదుపరి)
{
  కొరకు (size_t i = bol->i; i <= bol_తదుపరి->i; i++)
    {
      అక్షరం *a = (i == bol->i ? bol->ptr : x->p[i].ప్రారంభం);
      అక్షరం *b = (i == bol_తదుపరి->i ? bol_తదుపరి->ptr : ONE_PAST_చివర (x, i));
      fwrite (a, 1, b - a, బహిర్గతం_ప్రవాహం);
    }
}

స్థిర bool
tac_mem ()
{
  Buf x;
  పంక్తి_ptr bol;
  అక్షరం eol_byte = '\n';

  ఒకవేళ (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      తిరిగి false;
    }

  /* Special case the empty file.  */
  ఒకవేళ (ఖాళీలు_2 (&x))
    తిరిగి true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_చివర (&x, bol.i);

  అయితే (true)
    {
      పంక్తి_ptr new_bol;
      ఒకవేళ (! కనుగొను_bol (&x, &bol, &new_bol, eol_byte))
        ఆపు;
      print_పంక్తి (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  తిరిగి true;
}
