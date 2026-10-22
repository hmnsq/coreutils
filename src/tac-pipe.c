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
#અન્તર્ભાવ "assure.h"

/* FIXME: this is small for testing */
#define BUFFER_SIZE (8)

#define LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#define EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#define ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

સંરચના લીટી_ptr
{
  size_t i;
  અક્ષર *ptr;
};
પ્રકારવ્યાખ્યા સંરચના લીટી_ptr લીટી_ptr;

સંરચના B_pair
{
  અક્ષર *start;
  અક્ષર *one_past_end;
};

સંરચના Buf
{
  size_t n_bufs;
  સંરચના obstack obs;
  સંરચના B_pair *p;
};
પ્રકારવ્યાખ્યા સંરચના Buf Buf;

સ્થિર bool
buf_init_from_stdin (Buf *x, અક્ષર eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  જ્યારે (true)
    {
      અક્ષર *buf = malloc (BUFFER_SIZE);

      જો (buf == ખાલી)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          તોડો;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      જો (bytes_read != BUFFER_SIZE && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        સંરચના B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, કદ (bp));
      }

      જો (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      જો (bytes_read < BUFFER_SIZE)
        તોડો;
    }

  જો (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      જો (!last_byte_is_eol_byte)
        {
          અક્ષર *buf = malloc (1);
          જો (buf == ખાલી)
            {
              /* FIXME: just like above */
              ok = false;
            }
          નહીંતર
            {
              સંરચના B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, કદ (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / કદ (x->p[0]);
  x->p = (સંરચના B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  જો (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    મુક્ત (x->p[--(x->n_bufs)].start);

  પાછા ok;
}

સ્થિર શૂન્ય
buf_free (Buf *x)
{
  માટે (size_t i = 0; i < x->n_bufs; i++)
    મુક્ત (x->p[i].start);
  obstack_free (OBS, ખાલી);
}

લીટી_ptr
લીટી_ptr_decrement (અચલ Buf *x, અચલ લીટી_ptr *lp)
{
  લીટી_ptr lp_new;

  જો (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  નહીંતર
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  પાછા lp_new;
}

લીટી_ptr
લીટી_ptr_increment (અચલ Buf *x, અચલ લીટી_ptr *lp)
{
  લીટી_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  જો (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  નહીંતર
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  પાછા lp_new;
}

સ્થિર bool
find_bol (અચલ Buf *x,
          અચલ લીટી_ptr *last_bol, લીટી_ptr *new_bol, અક્ષર eol_byte)
{
  size_t i;
  લીટી_ptr tmp;
  અક્ષર *last_bol_ptr;

  જો (last_bol->ptr == x->p[0].start)
    પાછા false;

  tmp = લીટી_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  જ્યારે (true)
    {
      અક્ષર *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      જો (nl)
        {
          લીટી_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = લીટી_ptr_increment (x, &nl_pos);
          પાછા true;
        }

      જો (i == 0)
        તોડો;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  જો (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      પાછા true;
    }

  પાછા false;
}

સ્થિર શૂન્ય
print_લીટી (FILE *out_stream, અચલ Buf *x,
            અચલ લીટી_ptr *bol, અચલ લીટી_ptr *bol_next)
{
  માટે (size_t i = bol->i; i <= bol_next->i; i++)
    {
      અક્ષર *a = (i == bol->i ? bol->ptr : x->p[i].start);
      અક્ષર *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      fwrite (a, 1, b - a, out_stream);
    }
}

સ્થિર bool
tac_mem ()
{
  Buf x;
  લીટી_ptr bol;
  અક્ષર eol_byte = '\n';

  જો (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      પાછા false;
    }

  /* Special case the empty file.  */
  જો (EMPTY (&x))
    પાછા true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  જ્યારે (true)
    {
      લીટી_ptr new_bol;
      જો (! find_bol (&x, &bol, &new_bol, eol_byte))
        તોડો;
      print_લીટી (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  પાછા true;
}
