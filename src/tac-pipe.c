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
#شامل "assure.h"

/* FIXME: this is small for testing */
#تعریف_کرو بفر_سائز (8)

#تعریف_کرو LEN(X, I) ((X)->p[(I)].one_past_انتہا - (X)->p[(I)].آغاز)
#تعریف_کرو خالی_خانے_2(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#تعریف_کرو ONE_PAST_انتہا(X, I) ((X)->p[(I)].one_past_انتہا)

ساخت سطر_ptr
{
  size_t i;
  حرف *ptr;
};
قسمتعریف ساخت سطر_ptr سطر_ptr;

ساخت B_pair
{
  حرف *آغاز;
  حرف *one_past_انتہا;
};

ساخت Buf
{
  size_t n_bufs;
  ساخت obstack obs;
  ساخت B_pair *p;
};
قسمتعریف ساخت Buf Buf;

ساکن bool
buf_init_from_stdin (Buf *x, حرف eol_byte)
{
  bool آخری_byte_is_eol_byte = true;
  bool ok = true;

#تعریف_کرو OBS (&(x->obs))
  obstack_init (OBS);

  جب (true)
    {
      حرف *buf = حافظہ_دو (بفر_سائز);

      اگر (buf == تہی)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          توڑو;
        }
      idx_t بائٹیں_read = بھرا_read (STDIN_FILENO, buf, بفر_سائز);
      اگر (بائٹیں_read != بفر_سائز && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        ساخت B_pair bp;
        bp.آغاز = buf;
        bp.one_past_انتہا = buf + بائٹیں_read;
        obstack_grow (OBS, &bp, حجم (bp));
      }

      اگر (بائٹیں_read != 0)
        آخری_byte_is_eol_byte = (buf[بائٹیں_read - 1] == eol_byte);

      اگر (بائٹیں_read < بفر_سائز)
        توڑو;
    }

  اگر (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      اگر (!آخری_byte_is_eol_byte)
        {
          حرف *buf = حافظہ_دو (1);
          اگر (buf == تہی)
            {
              /* FIXME: just like above */
              ok = false;
            }
          ورنہ
            {
              ساخت B_pair bp;
              *buf = eol_byte;
              bp.آغاز = buf;
              bp.one_past_انتہا = buf + 1;
              obstack_grow (OBS, &bp, حجم (bp));
            }
        }
    }

  x->n_bufs = obstack_object_سائز (OBS) / حجم (x->p[0]);
  x->p = (ساخت B_pair *) obstack_اختتام (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  اگر (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].آغاز == x->p[x->n_bufs - 1].one_past_انتہا)
    آزاد (x->p[--(x->n_bufs)].آغاز);

  واپس ok;
}

ساکن خالی
buf_free (Buf *x)
{
  برائے (size_t i = 0; i < x->n_bufs; i++)
    آزاد (x->p[i].آغاز);
  obstack_free (OBS, تہی);
}

سطر_ptr
سطر_ptr_decrement (ثابت Buf *x, ثابت سطر_ptr *lp)
{
  سطر_ptr lp_new;

  اگر (lp->ptr > x->p[lp->i].آغاز)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  ورنہ
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_انتہا (x, lp->i - 1) - 1;
    }
  واپس lp_new;
}

سطر_ptr
سطر_ptr_increment (ثابت Buf *x, ثابت سطر_ptr *lp)
{
  سطر_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_انتہا (x, lp->i) - 1);
  اگر (lp->ptr < ONE_PAST_انتہا (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  ورنہ
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].آغاز;
    }
  واپس lp_new;
}

ساکن bool
ڈھونڈو_bol (ثابت Buf *x,
          ثابت سطر_ptr *آخری_bol, سطر_ptr *new_bol, حرف eol_byte)
{
  size_t i;
  سطر_ptr tmp;
  حرف *آخری_bol_ptr;

  اگر (آخری_bol->ptr == x->p[0].آغاز)
    واپس false;

  tmp = سطر_ptr_decrement (x, آخری_bol);
  آخری_bol_ptr = tmp.ptr;
  i = tmp.i;
  جب (true)
    {
      حرف *nl = memrchr (x->p[i].آغاز, آخری_bol_ptr, eol_byte);
      اگر (nl)
        {
          سطر_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = سطر_ptr_increment (x, &nl_pos);
          واپس true;
        }

      اگر (i == 0)
        توڑو;

      --i;
      آخری_bol_ptr = ONE_PAST_انتہا (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  اگر (آخری_bol->ptr != x->p[0].آغاز)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].آغاز;
      واپس true;
    }

  واپس false;
}

ساکن خالی
print_سطر (FILE *برآمد_دھارا, ثابت Buf *x,
            ثابت سطر_ptr *bol, ثابت سطر_ptr *bol_اگلا)
{
  برائے (size_t i = bol->i; i <= bol_اگلا->i; i++)
    {
      حرف *a = (i == bol->i ? bol->ptr : x->p[i].آغاز);
      حرف *b = (i == bol_اگلا->i ? bol_اگلا->ptr : ONE_PAST_انتہا (x, i));
      fwrite (a, 1, b - a, برآمد_دھارا);
    }
}

ساکن bool
tac_mem ()
{
  Buf x;
  سطر_ptr bol;
  حرف eol_byte = '\n';

  اگر (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      واپس false;
    }

  /* Special case the empty file.  */
  اگر (خالی_خانے_2 (&x))
    واپس true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_انتہا (&x, bol.i);

  جب (true)
    {
      سطر_ptr new_bol;
      اگر (! ڈھونڈو_bol (&x, &bol, &new_bol, eol_byte))
        توڑو;
      print_سطر (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  واپس true;
}
