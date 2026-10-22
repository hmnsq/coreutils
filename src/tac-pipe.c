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
#ضمن "assure.h"

/* FIXME: this is small for testing */
#وصف_ڪريو بفر_سائيز (8)

#وصف_ڪريو LEN(X, I) ((X)->p[(I)].one_past_پڇاڙي - (X)->p[(I)].شروعات)
#وصف_ڪريو خالي_جايون_2(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#وصف_ڪريو ONE_PAST_پڇاڙي(X, I) ((X)->p[(I)].one_past_پڇاڙي)

بنية سٽ_ptr
{
  size_t i;
  حرف *ptr;
};
تعريفنوع بنية سٽ_ptr سٽ_ptr;

بنية B_pair
{
  حرف *شروعات;
  حرف *one_past_پڇاڙي;
};

بنية Buf
{
  size_t n_bufs;
  بنية obstack obs;
  بنية B_pair *p;
};
تعريفنوع بنية Buf Buf;

ساكن bool
buf_init_from_stdin (Buf *x, حرف eol_byte)
{
  bool آخري_byte_is_eol_byte = true;
  bool ok = true;

#وصف_ڪريو OBS (&(x->obs))
  obstack_init (OBS);

  طالما (true)
    {
      حرف *buf = يادگيري_ڏيو (بفر_سائيز);

      إذا (buf == عدم)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          اكسر;
        }
      idx_t بائيٽون_read = ڀريل_read (STDIN_FILENO, buf, بفر_سائيز);
      إذا (بائيٽون_read != بفر_سائيز && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        بنية B_pair bp;
        bp.شروعات = buf;
        bp.one_past_پڇاڙي = buf + بائيٽون_read;
        obstack_grow (OBS, &bp, حجم (bp));
      }

      إذا (بائيٽون_read != 0)
        آخري_byte_is_eol_byte = (buf[بائيٽون_read - 1] == eol_byte);

      إذا (بائيٽون_read < بفر_سائيز)
        اكسر;
    }

  إذا (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      إذا (!آخري_byte_is_eol_byte)
        {
          حرف *buf = يادگيري_ڏيو (1);
          إذا (buf == عدم)
            {
              /* FIXME: just like above */
              ok = false;
            }
          وإلا
            {
              بنية B_pair bp;
              *buf = eol_byte;
              bp.شروعات = buf;
              bp.one_past_پڇاڙي = buf + 1;
              obstack_grow (OBS, &bp, حجم (bp));
            }
        }
    }

  x->n_bufs = obstack_object_سائيز (OBS) / حجم (x->p[0]);
  x->p = (بنية B_pair *) obstack_پڄاڻي (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  إذا (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].شروعات == x->p[x->n_bufs - 1].one_past_پڇاڙي)
    حرر (x->p[--(x->n_bufs)].شروعات);

  أرجع ok;
}

ساكن فراغ
buf_free (Buf *x)
{
  لكل (size_t i = 0; i < x->n_bufs; i++)
    حرر (x->p[i].شروعات);
  obstack_free (OBS, عدم);
}

سٽ_ptr
سٽ_ptr_decrement (ثابت Buf *x, ثابت سٽ_ptr *lp)
{
  سٽ_ptr lp_new;

  إذا (lp->ptr > x->p[lp->i].شروعات)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  وإلا
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_پڇاڙي (x, lp->i - 1) - 1;
    }
  أرجع lp_new;
}

سٽ_ptr
سٽ_ptr_increment (ثابت Buf *x, ثابت سٽ_ptr *lp)
{
  سٽ_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_پڇاڙي (x, lp->i) - 1);
  إذا (lp->ptr < ONE_PAST_پڇاڙي (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  وإلا
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].شروعات;
    }
  أرجع lp_new;
}

ساكن bool
ڳوليو_bol (ثابت Buf *x,
          ثابت سٽ_ptr *آخري_bol, سٽ_ptr *new_bol, حرف eol_byte)
{
  size_t i;
  سٽ_ptr tmp;
  حرف *آخري_bol_ptr;

  إذا (آخري_bol->ptr == x->p[0].شروعات)
    أرجع false;

  tmp = سٽ_ptr_decrement (x, آخري_bol);
  آخري_bol_ptr = tmp.ptr;
  i = tmp.i;
  طالما (true)
    {
      حرف *nl = memrchr (x->p[i].شروعات, آخري_bol_ptr, eol_byte);
      إذا (nl)
        {
          سٽ_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = سٽ_ptr_increment (x, &nl_pos);
          أرجع true;
        }

      إذا (i == 0)
        اكسر;

      --i;
      آخري_bol_ptr = ONE_PAST_پڇاڙي (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  إذا (آخري_bol->ptr != x->p[0].شروعات)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].شروعات;
      أرجع true;
    }

  أرجع false;
}

ساكن فراغ
print_سٽ (FILE *ٻاهريون_وهڪرو, ثابت Buf *x,
            ثابت سٽ_ptr *bol, ثابت سٽ_ptr *bol_ايندڙ)
{
  لكل (size_t i = bol->i; i <= bol_ايندڙ->i; i++)
    {
      حرف *a = (i == bol->i ? bol->ptr : x->p[i].شروعات);
      حرف *b = (i == bol_ايندڙ->i ? bol_ايندڙ->ptr : ONE_PAST_پڇاڙي (x, i));
      fwrite (a, 1, b - a, ٻاهريون_وهڪرو);
    }
}

ساكن bool
tac_mem ()
{
  Buf x;
  سٽ_ptr bol;
  حرف eol_byte = '\n';

  إذا (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      أرجع false;
    }

  /* Special case the empty file.  */
  إذا (خالي_جايون_2 (&x))
    أرجع true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_پڇاڙي (&x, bol.i);

  طالما (true)
    {
      سٽ_ptr new_bol;
      إذا (! ڳوليو_bol (&x, &bol, &new_bol, eol_byte))
        اكسر;
      print_سٽ (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  أرجع true;
}
