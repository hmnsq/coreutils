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
#அந்தர்பாவ "assure.h"

/* FIXME: this is small for testing */
#define BUFFER_SIZE (8)

#define LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#define EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#define ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

கட்டமைப்பு வரி_ptr
{
  size_t i;
  எழுத்து *ptr;
};
வகைவரை கட்டமைப்பு வரி_ptr வரி_ptr;

கட்டமைப்பு B_pair
{
  எழுத்து *start;
  எழுத்து *one_past_end;
};

கட்டமைப்பு Buf
{
  size_t n_bufs;
  கட்டமைப்பு obstack obs;
  கட்டமைப்பு B_pair *p;
};
வகைவரை கட்டமைப்பு Buf Buf;

நிலையான bool
buf_init_from_stdin (Buf *x, எழுத்து eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  போது (true)
    {
      எழுத்து *buf = malloc (BUFFER_SIZE);

      எனில் (buf == வெற்று)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          நிறுத்து;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      எனில் (bytes_read != BUFFER_SIZE && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        கட்டமைப்பு B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, அளவு (bp));
      }

      எனில் (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      எனில் (bytes_read < BUFFER_SIZE)
        நிறுத்து;
    }

  எனில் (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      எனில் (!last_byte_is_eol_byte)
        {
          எழுத்து *buf = malloc (1);
          எனில் (buf == வெற்று)
            {
              /* FIXME: just like above */
              ok = false;
            }
          இல்லையெனில்
            {
              கட்டமைப்பு B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, அளவு (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / அளவு (x->p[0]);
  x->p = (கட்டமைப்பு B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  எனில் (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    விடுவி (x->p[--(x->n_bufs)].start);

  திரும்பு ok;
}

நிலையான வெறுமை
buf_free (Buf *x)
{
  க்கு (size_t i = 0; i < x->n_bufs; i++)
    விடுவி (x->p[i].start);
  obstack_free (OBS, வெற்று);
}

வரி_ptr
வரி_ptr_decrement (மாறிலி Buf *x, மாறிலி வரி_ptr *lp)
{
  வரி_ptr lp_new;

  எனில் (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  இல்லையெனில்
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  திரும்பு lp_new;
}

வரி_ptr
வரி_ptr_increment (மாறிலி Buf *x, மாறிலி வரி_ptr *lp)
{
  வரி_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  எனில் (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  இல்லையெனில்
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  திரும்பு lp_new;
}

நிலையான bool
find_bol (மாறிலி Buf *x,
          மாறிலி வரி_ptr *last_bol, வரி_ptr *new_bol, எழுத்து eol_byte)
{
  size_t i;
  வரி_ptr tmp;
  எழுத்து *last_bol_ptr;

  எனில் (last_bol->ptr == x->p[0].start)
    திரும்பு false;

  tmp = வரி_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  போது (true)
    {
      எழுத்து *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      எனில் (nl)
        {
          வரி_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = வரி_ptr_increment (x, &nl_pos);
          திரும்பு true;
        }

      எனில் (i == 0)
        நிறுத்து;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  எனில் (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      திரும்பு true;
    }

  திரும்பு false;
}

நிலையான வெறுமை
print_வரி (FILE *out_stream, மாறிலி Buf *x,
            மாறிலி வரி_ptr *bol, மாறிலி வரி_ptr *bol_next)
{
  க்கு (size_t i = bol->i; i <= bol_next->i; i++)
    {
      எழுத்து *a = (i == bol->i ? bol->ptr : x->p[i].start);
      எழுத்து *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      fwrite (a, 1, b - a, out_stream);
    }
}

நிலையான bool
tac_mem ()
{
  Buf x;
  வரி_ptr bol;
  எழுத்து eol_byte = '\n';

  எனில் (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      திரும்பு false;
    }

  /* Special case the empty file.  */
  எனில் (EMPTY (&x))
    திரும்பு true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  போது (true)
    {
      வரி_ptr new_bol;
      எனில் (! find_bol (&x, &bol, &new_bol, eol_byte))
        நிறுத்து;
      print_வரி (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  திரும்பு true;
}
