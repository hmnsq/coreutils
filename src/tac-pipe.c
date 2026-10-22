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
#അന്തര്ഭാവ "assure.h"

/* FIXME: this is small for testing */
#define BUFFER_SIZE (8)

#define LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#define EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#define ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

ഘടന വരി_ptr
{
  size_t i;
  അക്ഷരം *ptr;
};
തരനിർവചനം ഘടന വരി_ptr വരി_ptr;

ഘടന B_pair
{
  അക്ഷരം *start;
  അക്ഷരം *one_past_end;
};

ഘടന Buf
{
  size_t n_bufs;
  ഘടന obstack obs;
  ഘടന B_pair *p;
};
തരനിർവചനം ഘടന Buf Buf;

സ്ഥിതം bool
buf_init_from_stdin (Buf *x, അക്ഷരം eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  ആകുമ്പോൾ (true)
    {
      അക്ഷരം *buf = malloc (BUFFER_SIZE);

      എങ്കിൽ (buf == ശൂന്യ)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          നിർത്തുക;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      എങ്കിൽ (bytes_read != BUFFER_SIZE && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        ഘടന B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, വലുപ്പം (bp));
      }

      എങ്കിൽ (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      എങ്കിൽ (bytes_read < BUFFER_SIZE)
        നിർത്തുക;
    }

  എങ്കിൽ (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      എങ്കിൽ (!last_byte_is_eol_byte)
        {
          അക്ഷരം *buf = malloc (1);
          എങ്കിൽ (buf == ശൂന്യ)
            {
              /* FIXME: just like above */
              ok = false;
            }
          അല്ലെങ്കിൽ
            {
              ഘടന B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, വലുപ്പം (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / വലുപ്പം (x->p[0]);
  x->p = (ഘടന B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  എങ്കിൽ (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    സ്വതന്ത്രം (x->p[--(x->n_bufs)].start);

  തിരികെ ok;
}

സ്ഥിതം ശൂന്യം
buf_free (Buf *x)
{
  വേണ്ടി (size_t i = 0; i < x->n_bufs; i++)
    സ്വതന്ത്രം (x->p[i].start);
  obstack_free (OBS, ശൂന്യ);
}

വരി_ptr
വരി_ptr_decrement (സ്ഥിരം Buf *x, സ്ഥിരം വരി_ptr *lp)
{
  വരി_ptr lp_new;

  എങ്കിൽ (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  അല്ലെങ്കിൽ
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  തിരികെ lp_new;
}

വരി_ptr
വരി_ptr_increment (സ്ഥിരം Buf *x, സ്ഥിരം വരി_ptr *lp)
{
  വരി_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  എങ്കിൽ (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  അല്ലെങ്കിൽ
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  തിരികെ lp_new;
}

സ്ഥിതം bool
find_bol (സ്ഥിരം Buf *x,
          സ്ഥിരം വരി_ptr *last_bol, വരി_ptr *new_bol, അക്ഷരം eol_byte)
{
  size_t i;
  വരി_ptr tmp;
  അക്ഷരം *last_bol_ptr;

  എങ്കിൽ (last_bol->ptr == x->p[0].start)
    തിരികെ false;

  tmp = വരി_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  ആകുമ്പോൾ (true)
    {
      അക്ഷരം *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      എങ്കിൽ (nl)
        {
          വരി_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = വരി_ptr_increment (x, &nl_pos);
          തിരികെ true;
        }

      എങ്കിൽ (i == 0)
        നിർത്തുക;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  എങ്കിൽ (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      തിരികെ true;
    }

  തിരികെ false;
}

സ്ഥിതം ശൂന്യം
print_വരി (FILE *out_stream, സ്ഥിരം Buf *x,
            സ്ഥിരം വരി_ptr *bol, സ്ഥിരം വരി_ptr *bol_next)
{
  വേണ്ടി (size_t i = bol->i; i <= bol_next->i; i++)
    {
      അക്ഷരം *a = (i == bol->i ? bol->ptr : x->p[i].start);
      അക്ഷരം *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      fwrite (a, 1, b - a, out_stream);
    }
}

സ്ഥിതം bool
tac_mem ()
{
  Buf x;
  വരി_ptr bol;
  അക്ഷരം eol_byte = '\n';

  എങ്കിൽ (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      തിരികെ false;
    }

  /* Special case the empty file.  */
  എങ്കിൽ (EMPTY (&x))
    തിരികെ true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  ആകുമ്പോൾ (true)
    {
      വരി_ptr new_bol;
      എങ്കിൽ (! find_bol (&x, &bol, &new_bol, eol_byte))
        നിർത്തുക;
      print_വരി (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  തിരികെ true;
}
