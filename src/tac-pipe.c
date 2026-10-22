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
#تعریف_کړه بفر_اندازه (8)

#تعریف_کړه LEN(X, I) ((X)->p[(I)].one_past_پای_ټکی - (X)->p[(I)].پیل)
#تعریف_کړه تشه_2(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#تعریف_کړه ONE_PAST_پای_ټکی(X, I) ((X)->p[(I)].one_past_پای_ټکی)

بنية کرښه_ptr
{
  size_t i;
  حرف *ptr;
};
تعريفنوع بنية کرښه_ptr کرښه_ptr;

بنية B_pair
{
  حرف *پیل;
  حرف *one_past_پای_ټکی;
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
  bool وروستی_byte_is_eol_byte = true;
  bool ok = true;

#تعریف_کړه OBS (&(x->obs))
  obstack_init (OBS);

  طالما (true)
    {
      حرف *buf = حافظه_ورکړه (بفر_اندازه);

      إذا (buf == عدم)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          اكسر;
        }
      idx_t بایټونه_read = ډک_read (STDIN_FILENO, buf, بفر_اندازه);
      إذا (بایټونه_read != بفر_اندازه && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        بنية B_pair bp;
        bp.پیل = buf;
        bp.one_past_پای_ټکی = buf + بایټونه_read;
        obstack_grow (OBS, &bp, حجم (bp));
      }

      إذا (بایټونه_read != 0)
        وروستی_byte_is_eol_byte = (buf[بایټونه_read - 1] == eol_byte);

      إذا (بایټونه_read < بفر_اندازه)
        اكسر;
    }

  إذا (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      إذا (!وروستی_byte_is_eol_byte)
        {
          حرف *buf = حافظه_ورکړه (1);
          إذا (buf == عدم)
            {
              /* FIXME: just like above */
              ok = false;
            }
          وإلا
            {
              بنية B_pair bp;
              *buf = eol_byte;
              bp.پیل = buf;
              bp.one_past_پای_ټکی = buf + 1;
              obstack_grow (OBS, &bp, حجم (bp));
            }
        }
    }

  x->n_bufs = obstack_object_اندازه (OBS) / حجم (x->p[0]);
  x->p = (بنية B_pair *) obstack_پای (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  إذا (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].پیل == x->p[x->n_bufs - 1].one_past_پای_ټکی)
    حرر (x->p[--(x->n_bufs)].پیل);

  أرجع ok;
}

ساكن فراغ
buf_free (Buf *x)
{
  لكل (size_t i = 0; i < x->n_bufs; i++)
    حرر (x->p[i].پیل);
  obstack_free (OBS, عدم);
}

کرښه_ptr
کرښه_ptr_decrement (ثابت Buf *x, ثابت کرښه_ptr *lp)
{
  کرښه_ptr lp_new;

  إذا (lp->ptr > x->p[lp->i].پیل)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  وإلا
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_پای_ټکی (x, lp->i - 1) - 1;
    }
  أرجع lp_new;
}

کرښه_ptr
کرښه_ptr_increment (ثابت Buf *x, ثابت کرښه_ptr *lp)
{
  کرښه_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_پای_ټکی (x, lp->i) - 1);
  إذا (lp->ptr < ONE_PAST_پای_ټکی (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  وإلا
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].پیل;
    }
  أرجع lp_new;
}

ساكن bool
پیدا_کړه_bol (ثابت Buf *x,
          ثابت کرښه_ptr *وروستی_bol, کرښه_ptr *new_bol, حرف eol_byte)
{
  size_t i;
  کرښه_ptr tmp;
  حرف *وروستی_bol_ptr;

  إذا (وروستی_bol->ptr == x->p[0].پیل)
    أرجع false;

  tmp = کرښه_ptr_decrement (x, وروستی_bol);
  وروستی_bol_ptr = tmp.ptr;
  i = tmp.i;
  طالما (true)
    {
      حرف *nl = memrchr (x->p[i].پیل, وروستی_bol_ptr, eol_byte);
      إذا (nl)
        {
          کرښه_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = کرښه_ptr_increment (x, &nl_pos);
          أرجع true;
        }

      إذا (i == 0)
        اكسر;

      --i;
      وروستی_bol_ptr = ONE_PAST_پای_ټکی (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  إذا (وروستی_bol->ptr != x->p[0].پیل)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].پیل;
      أرجع true;
    }

  أرجع false;
}

ساكن فراغ
print_کرښه (FILE *بهر_بهیر, ثابت Buf *x,
            ثابت کرښه_ptr *bol, ثابت کرښه_ptr *bol_بل)
{
  لكل (size_t i = bol->i; i <= bol_بل->i; i++)
    {
      حرف *a = (i == bol->i ? bol->ptr : x->p[i].پیل);
      حرف *b = (i == bol_بل->i ? bol_بل->ptr : ONE_PAST_پای_ټکی (x, i));
      fwrite (a, 1, b - a, بهر_بهیر);
    }
}

ساكن bool
tac_mem ()
{
  Buf x;
  کرښه_ptr bol;
  حرف eol_byte = '\n';

  إذا (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      أرجع false;
    }

  /* Special case the empty file.  */
  إذا (تشه_2 (&x))
    أرجع true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_پای_ټکی (&x, bol.i);

  طالما (true)
    {
      کرښه_ptr new_bol;
      إذا (! پیدا_کړه_bol (&x, &bol, &new_bol, eol_byte))
        اكسر;
      print_کرښه (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  أرجع true;
}
