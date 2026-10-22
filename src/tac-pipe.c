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
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"

/* FIXME: this is small for testing */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਬਫਰ_ਮਾਪ (8)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LEN(X, I) ((X)->p[(I)].one_past_ਸਿਰਾ - (X)->p[(I)].ਸ਼ੁਰੂ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖਾਲੀ_ਥਾਵਾਂ_2(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ONE_PAST_ਸਿਰਾ(X, I) ((X)->p[(I)].one_past_ਸਿਰਾ)

ਬਣਤਰ ਸਤਰ_ptr
{
  size_t i;
  ਅੱਖਰ *ptr;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਸਤਰ_ptr ਸਤਰ_ptr;

ਬਣਤਰ B_pair
{
  ਅੱਖਰ *ਸ਼ੁਰੂ;
  ਅੱਖਰ *one_past_ਸਿਰਾ;
};

ਬਣਤਰ Buf
{
  size_t n_bufs;
  ਬਣਤਰ obstack obs;
  ਬਣਤਰ B_pair *p;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ Buf Buf;

ਸਥਿਰ bool
buf_init_from_stdin (Buf *x, ਅੱਖਰ eol_byte)
{
  bool ਆਖਰੀ_byte_is_eol_byte = true;
  bool ok = true;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OBS (&(x->obs))
  obstack_init (OBS);

  ਜਦੋਂ (true)
    {
      ਅੱਖਰ *buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);

      ਜੇ (buf == ਨਿਰਾ)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          ਰੋਕੋ;
        }
      idx_t ਬਾਈਟਾਂ_read = ਭਰਿਆ_read (STDIN_FILENO, buf, ਬਫਰ_ਮਾਪ);
      ਜੇ (ਬਾਈਟਾਂ_read != ਬਫਰ_ਮਾਪ && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        ਬਣਤਰ B_pair bp;
        bp.ਸ਼ੁਰੂ = buf;
        bp.one_past_ਸਿਰਾ = buf + ਬਾਈਟਾਂ_read;
        obstack_grow (OBS, &bp, ਆਕਾਰ (bp));
      }

      ਜੇ (ਬਾਈਟਾਂ_read != 0)
        ਆਖਰੀ_byte_is_eol_byte = (buf[ਬਾਈਟਾਂ_read - 1] == eol_byte);

      ਜੇ (ਬਾਈਟਾਂ_read < ਬਫਰ_ਮਾਪ)
        ਰੋਕੋ;
    }

  ਜੇ (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      ਜੇ (!ਆਖਰੀ_byte_is_eol_byte)
        {
          ਅੱਖਰ *buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ (1);
          ਜੇ (buf == ਨਿਰਾ)
            {
              /* FIXME: just like above */
              ok = false;
            }
          ਨਹੀਂਤਾਂ
            {
              ਬਣਤਰ B_pair bp;
              *buf = eol_byte;
              bp.ਸ਼ੁਰੂ = buf;
              bp.one_past_ਸਿਰਾ = buf + 1;
              obstack_grow (OBS, &bp, ਆਕਾਰ (bp));
            }
        }
    }

  x->n_bufs = obstack_object_ਮਾਪ (OBS) / ਆਕਾਰ (x->p[0]);
  x->p = (ਬਣਤਰ B_pair *) obstack_ਅੰਤ (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  ਜੇ (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].ਸ਼ੁਰੂ == x->p[x->n_bufs - 1].one_past_ਸਿਰਾ)
    ਮੁਕਤ (x->p[--(x->n_bufs)].ਸ਼ੁਰੂ);

  ਵਾਪਸ ok;
}

ਸਥਿਰ ਖਾਲੀ
buf_free (Buf *x)
{
  ਲਈ (size_t i = 0; i < x->n_bufs; i++)
    ਮੁਕਤ (x->p[i].ਸ਼ੁਰੂ);
  obstack_free (OBS, ਨਿਰਾ);
}

ਸਤਰ_ptr
ਸਤਰ_ptr_decrement (ਅਚਲ Buf *x, ਅਚਲ ਸਤਰ_ptr *lp)
{
  ਸਤਰ_ptr lp_new;

  ਜੇ (lp->ptr > x->p[lp->i].ਸ਼ੁਰੂ)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  ਨਹੀਂਤਾਂ
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_ਸਿਰਾ (x, lp->i - 1) - 1;
    }
  ਵਾਪਸ lp_new;
}

ਸਤਰ_ptr
ਸਤਰ_ptr_increment (ਅਚਲ Buf *x, ਅਚਲ ਸਤਰ_ptr *lp)
{
  ਸਤਰ_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_ਸਿਰਾ (x, lp->i) - 1);
  ਜੇ (lp->ptr < ONE_PAST_ਸਿਰਾ (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  ਨਹੀਂਤਾਂ
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].ਸ਼ੁਰੂ;
    }
  ਵਾਪਸ lp_new;
}

ਸਥਿਰ bool
ਲੱਭੋ_bol (ਅਚਲ Buf *x,
          ਅਚਲ ਸਤਰ_ptr *ਆਖਰੀ_bol, ਸਤਰ_ptr *new_bol, ਅੱਖਰ eol_byte)
{
  size_t i;
  ਸਤਰ_ptr tmp;
  ਅੱਖਰ *ਆਖਰੀ_bol_ptr;

  ਜੇ (ਆਖਰੀ_bol->ptr == x->p[0].ਸ਼ੁਰੂ)
    ਵਾਪਸ false;

  tmp = ਸਤਰ_ptr_decrement (x, ਆਖਰੀ_bol);
  ਆਖਰੀ_bol_ptr = tmp.ptr;
  i = tmp.i;
  ਜਦੋਂ (true)
    {
      ਅੱਖਰ *nl = memrchr (x->p[i].ਸ਼ੁਰੂ, ਆਖਰੀ_bol_ptr, eol_byte);
      ਜੇ (nl)
        {
          ਸਤਰ_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = ਸਤਰ_ptr_increment (x, &nl_pos);
          ਵਾਪਸ true;
        }

      ਜੇ (i == 0)
        ਰੋਕੋ;

      --i;
      ਆਖਰੀ_bol_ptr = ONE_PAST_ਸਿਰਾ (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  ਜੇ (ਆਖਰੀ_bol->ptr != x->p[0].ਸ਼ੁਰੂ)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].ਸ਼ੁਰੂ;
      ਵਾਪਸ true;
    }

  ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ
print_ਸਤਰ (FILE *ਬਾਹਰਲਾ_ਧਾਰਾ, ਅਚਲ Buf *x,
            ਅਚਲ ਸਤਰ_ptr *bol, ਅਚਲ ਸਤਰ_ptr *bol_ਅਗਲਾ)
{
  ਲਈ (size_t i = bol->i; i <= bol_ਅਗਲਾ->i; i++)
    {
      ਅੱਖਰ *a = (i == bol->i ? bol->ptr : x->p[i].ਸ਼ੁਰੂ);
      ਅੱਖਰ *b = (i == bol_ਅਗਲਾ->i ? bol_ਅਗਲਾ->ptr : ONE_PAST_ਸਿਰਾ (x, i));
      fwrite (a, 1, b - a, ਬਾਹਰਲਾ_ਧਾਰਾ);
    }
}

ਸਥਿਰ bool
tac_mem ()
{
  Buf x;
  ਸਤਰ_ptr bol;
  ਅੱਖਰ eol_byte = '\n';

  ਜੇ (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      ਵਾਪਸ false;
    }

  /* Special case the empty file.  */
  ਜੇ (ਖਾਲੀ_ਥਾਵਾਂ_2 (&x))
    ਵਾਪਸ true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_ਸਿਰਾ (&x, bol.i);

  ਜਦੋਂ (true)
    {
      ਸਤਰ_ptr new_bol;
      ਜੇ (! ਲੱਭੋ_bol (&x, &bol, &new_bol, eol_byte))
        ਰੋਕੋ;
      print_ਸਤਰ (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  ਵਾਪਸ true;
}
