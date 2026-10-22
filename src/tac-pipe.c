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
#define BUFFER_SIZE (8)

#define LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#define EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#define ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

ساختار سطر_ptr
{
  size_t i;
  نویسه *ptr;
};
تعریفنوع ساختار سطر_ptr سطر_ptr;

ساختار B_pair
{
  نویسه *start;
  نویسه *one_past_end;
};

ساختار Buf
{
  size_t n_bufs;
  ساختار obstack obs;
  ساختار B_pair *p;
};
تعریفنوع ساختار Buf Buf;

ایستا bool
buf_init_from_stdin (Buf *x, نویسه eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  وقتی (true)
    {
      نویسه *buf = malloc (BUFFER_SIZE);

      اگر (buf == پوچ)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          بشکن;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      اگر (bytes_read != BUFFER_SIZE && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        ساختار B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, اندازه (bp));
      }

      اگر (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      اگر (bytes_read < BUFFER_SIZE)
        بشکن;
    }

  اگر (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      اگر (!last_byte_is_eol_byte)
        {
          نویسه *buf = malloc (1);
          اگر (buf == پوچ)
            {
              /* FIXME: just like above */
              ok = false;
            }
          وگرنه
            {
              ساختار B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, اندازه (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / اندازه (x->p[0]);
  x->p = (ساختار B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  اگر (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    آزاد (x->p[--(x->n_bufs)].start);

  بازگشت ok;
}

ایستا تهی
buf_free (Buf *x)
{
  برای (size_t i = 0; i < x->n_bufs; i++)
    آزاد (x->p[i].start);
  obstack_free (OBS, پوچ);
}

سطر_ptr
سطر_ptr_decrement (ثابت Buf *x, ثابت سطر_ptr *lp)
{
  سطر_ptr lp_new;

  اگر (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  وگرنه
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  بازگشت lp_new;
}

سطر_ptr
سطر_ptr_increment (ثابت Buf *x, ثابت سطر_ptr *lp)
{
  سطر_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  اگر (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  وگرنه
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  بازگشت lp_new;
}

ایستا bool
find_bol (ثابت Buf *x,
          ثابت سطر_ptr *last_bol, سطر_ptr *new_bol, نویسه eol_byte)
{
  size_t i;
  سطر_ptr tmp;
  نویسه *last_bol_ptr;

  اگر (last_bol->ptr == x->p[0].start)
    بازگشت false;

  tmp = سطر_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  وقتی (true)
    {
      نویسه *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      اگر (nl)
        {
          سطر_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = سطر_ptr_increment (x, &nl_pos);
          بازگشت true;
        }

      اگر (i == 0)
        بشکن;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  اگر (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      بازگشت true;
    }

  بازگشت false;
}

ایستا تهی
print_سطر (FILE *out_stream, ثابت Buf *x,
            ثابت سطر_ptr *bol, ثابت سطر_ptr *bol_next)
{
  برای (size_t i = bol->i; i <= bol_next->i; i++)
    {
      نویسه *a = (i == bol->i ? bol->ptr : x->p[i].start);
      نویسه *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      fwrite (a, 1, b - a, out_stream);
    }
}

ایستا bool
tac_mem ()
{
  Buf x;
  سطر_ptr bol;
  نویسه eol_byte = '\n';

  اگر (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      بازگشت false;
    }

  /* Special case the empty file.  */
  اگر (EMPTY (&x))
    بازگشت true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  وقتی (true)
    {
      سطر_ptr new_bol;
      اگر (! find_bol (&x, &bol, &new_bol, eol_byte))
        بشکن;
      print_سطر (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  بازگشت true;
}
