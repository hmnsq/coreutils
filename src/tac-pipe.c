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
#تعریف_کن میانگیر_اندازه (8)

#تعریف_کن LEN(X, I) ((X)->p[(I)].one_past_انتها - (X)->p[(I)].شروع)
#تعریف_کن خالی_2(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#تعریف_کن ONE_PAST_انتها(X, I) ((X)->p[(I)].one_past_انتها)

بنية سطر_ptr
{
  size_t i;
  حرف *ptr;
};
تعريفنوع بنية سطر_ptr سطر_ptr;

بنية B_pair
{
  حرف *شروع;
  حرف *one_past_انتها;
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
  bool آخرین_byte_is_eol_byte = true;
  bool ok = true;

#تعریف_کن OBS (&(x->obs))
  obstack_init (OBS);

  طالما (true)
    {
      حرف *buf = حافظه_بده (میانگیر_اندازه);

      إذا (buf == عدم)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          اكسر;
        }
      idx_t بایتها_read = پر_read (STDIN_FILENO, buf, میانگیر_اندازه);
      إذا (بایتها_read != میانگیر_اندازه && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        بنية B_pair bp;
        bp.شروع = buf;
        bp.one_past_انتها = buf + بایتها_read;
        obstack_grow (OBS, &bp, حجم (bp));
      }

      إذا (بایتها_read != 0)
        آخرین_byte_is_eol_byte = (buf[بایتها_read - 1] == eol_byte);

      إذا (بایتها_read < میانگیر_اندازه)
        اكسر;
    }

  إذا (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      إذا (!آخرین_byte_is_eol_byte)
        {
          حرف *buf = حافظه_بده (1);
          إذا (buf == عدم)
            {
              /* FIXME: just like above */
              ok = false;
            }
          وإلا
            {
              بنية B_pair bp;
              *buf = eol_byte;
              bp.شروع = buf;
              bp.one_past_انتها = buf + 1;
              obstack_grow (OBS, &bp, حجم (bp));
            }
        }
    }

  x->n_bufs = obstack_object_اندازه (OBS) / حجم (x->p[0]);
  x->p = (بنية B_pair *) obstack_پایان (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  إذا (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].شروع == x->p[x->n_bufs - 1].one_past_انتها)
    حرر (x->p[--(x->n_bufs)].شروع);

  أرجع ok;
}

ساكن فراغ
buf_free (Buf *x)
{
  لكل (size_t i = 0; i < x->n_bufs; i++)
    حرر (x->p[i].شروع);
  obstack_free (OBS, عدم);
}

سطر_ptr
سطر_ptr_decrement (ثابت Buf *x, ثابت سطر_ptr *lp)
{
  سطر_ptr lp_new;

  إذا (lp->ptr > x->p[lp->i].شروع)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  وإلا
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_انتها (x, lp->i - 1) - 1;
    }
  أرجع lp_new;
}

سطر_ptr
سطر_ptr_increment (ثابت Buf *x, ثابت سطر_ptr *lp)
{
  سطر_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_انتها (x, lp->i) - 1);
  إذا (lp->ptr < ONE_PAST_انتها (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  وإلا
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].شروع;
    }
  أرجع lp_new;
}

ساكن bool
پیدا_کن_bol (ثابت Buf *x,
          ثابت سطر_ptr *آخرین_bol, سطر_ptr *new_bol, حرف eol_byte)
{
  size_t i;
  سطر_ptr tmp;
  حرف *آخرین_bol_ptr;

  إذا (آخرین_bol->ptr == x->p[0].شروع)
    أرجع false;

  tmp = سطر_ptr_decrement (x, آخرین_bol);
  آخرین_bol_ptr = tmp.ptr;
  i = tmp.i;
  طالما (true)
    {
      حرف *nl = memrchr (x->p[i].شروع, آخرین_bol_ptr, eol_byte);
      إذا (nl)
        {
          سطر_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = سطر_ptr_increment (x, &nl_pos);
          أرجع true;
        }

      إذا (i == 0)
        اكسر;

      --i;
      آخرین_bol_ptr = ONE_PAST_انتها (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  إذا (آخرین_bol->ptr != x->p[0].شروع)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].شروع;
      أرجع true;
    }

  أرجع false;
}

ساكن فراغ
print_سطر (FILE *بیرون_جریان, ثابت Buf *x,
            ثابت سطر_ptr *bol, ثابت سطر_ptr *bol_بعدی)
{
  لكل (size_t i = bol->i; i <= bol_بعدی->i; i++)
    {
      حرف *a = (i == bol->i ? bol->ptr : x->p[i].شروع);
      حرف *b = (i == bol_بعدی->i ? bol_بعدی->ptr : ONE_PAST_انتها (x, i));
      fwrite (a, 1, b - a, بیرون_جریان);
    }
}

ساكن bool
tac_mem ()
{
  Buf x;
  سطر_ptr bol;
  حرف eol_byte = '\n';

  إذا (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      أرجع false;
    }

  /* Special case the empty file.  */
  إذا (خالی_2 (&x))
    أرجع true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_انتها (&x, bol.i);

  طالما (true)
    {
      سطر_ptr new_bol;
      إذا (! پیدا_کن_bol (&x, &bol, &new_bol, eol_byte))
        اكسر;
      print_سطر (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  أرجع true;
}
