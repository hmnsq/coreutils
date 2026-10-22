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
#ଅନ୍ତର୍ଭାଵ "assure.h"

/* FIXME: this is small for testing */
#define BUFFER_SIZE (8)

#define LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#define EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#define ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

ସଂରଚନା ଧାଡ଼ି_ptr
{
  size_t i;
  ଅକ୍ଷର *ptr;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ଧାଡ଼ି_ptr ଧାଡ଼ି_ptr;

ସଂରଚନା B_pair
{
  ଅକ୍ଷର *start;
  ଅକ୍ଷର *one_past_end;
};

ସଂରଚନା Buf
{
  size_t n_bufs;
  ସଂରଚନା obstack obs;
  ସଂରଚନା B_pair *p;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା Buf Buf;

ସ୍ଥିର bool
buf_init_from_stdin (Buf *x, ଅକ୍ଷର eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର *buf = malloc (BUFFER_SIZE);

      ଯଦି (buf == ଖାଲି)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          ଭାଙ୍ଗ;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      ଯଦି (bytes_read != BUFFER_SIZE && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        ସଂରଚନା B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, ଆକାର (bp));
      }

      ଯଦି (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      ଯଦି (bytes_read < BUFFER_SIZE)
        ଭାଙ୍ଗ;
    }

  ଯଦି (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      ଯଦି (!last_byte_is_eol_byte)
        {
          ଅକ୍ଷର *buf = malloc (1);
          ଯଦି (buf == ଖାଲି)
            {
              /* FIXME: just like above */
              ok = false;
            }
          ଅନ୍ୟଥା
            {
              ସଂରଚନା B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, ଆକାର (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / ଆକାର (x->p[0]);
  x->p = (ସଂରଚନା B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  ଯଦି (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    ମୁକ୍ତ (x->p[--(x->n_bufs)].start);

  ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର ଶୂନ୍ୟ
buf_free (Buf *x)
{
  ପାଇଁ (size_t i = 0; i < x->n_bufs; i++)
    ମୁକ୍ତ (x->p[i].start);
  obstack_free (OBS, ଖାଲି);
}

ଧାଡ଼ି_ptr
ଧାଡ଼ି_ptr_decrement (ଅଚଳ Buf *x, ଅଚଳ ଧାଡ଼ି_ptr *lp)
{
  ଧାଡ଼ି_ptr lp_new;

  ଯଦି (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  ଅନ୍ୟଥା
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  ଫେରନ୍ତୁ lp_new;
}

ଧାଡ଼ି_ptr
ଧାଡ଼ି_ptr_increment (ଅଚଳ Buf *x, ଅଚଳ ଧାଡ଼ି_ptr *lp)
{
  ଧାଡ଼ି_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  ଯଦି (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  ଅନ୍ୟଥା
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  ଫେରନ୍ତୁ lp_new;
}

ସ୍ଥିର bool
find_bol (ଅଚଳ Buf *x,
          ଅଚଳ ଧାଡ଼ି_ptr *last_bol, ଧାଡ଼ି_ptr *new_bol, ଅକ୍ଷର eol_byte)
{
  size_t i;
  ଧାଡ଼ି_ptr tmp;
  ଅକ୍ଷର *last_bol_ptr;

  ଯଦି (last_bol->ptr == x->p[0].start)
    ଫେରନ୍ତୁ false;

  tmp = ଧାଡ଼ି_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      ଯଦି (nl)
        {
          ଧାଡ଼ି_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = ଧାଡ଼ି_ptr_increment (x, &nl_pos);
          ଫେରନ୍ତୁ true;
        }

      ଯଦି (i == 0)
        ଭାଙ୍ଗ;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  ଯଦି (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      ଫେରନ୍ତୁ true;
    }

  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ
print_ଧାଡ଼ି (FILE *out_stream, ଅଚଳ Buf *x,
            ଅଚଳ ଧାଡ଼ି_ptr *bol, ଅଚଳ ଧାଡ଼ି_ptr *bol_next)
{
  ପାଇଁ (size_t i = bol->i; i <= bol_next->i; i++)
    {
      ଅକ୍ଷର *a = (i == bol->i ? bol->ptr : x->p[i].start);
      ଅକ୍ଷର *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      fwrite (a, 1, b - a, out_stream);
    }
}

ସ୍ଥିର bool
tac_mem ()
{
  Buf x;
  ଧାଡ଼ି_ptr bol;
  ଅକ୍ଷର eol_byte = '\n';

  ଯଦି (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      ଫେରନ୍ତୁ false;
    }

  /* Special case the empty file.  */
  ଯଦି (EMPTY (&x))
    ଫେରନ୍ତୁ true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  ଯେତେବେଳେ (true)
    {
      ଧାଡ଼ି_ptr new_bol;
      ଯଦି (! find_bol (&x, &bol, &new_bol, eol_byte))
        ଭାଙ୍ଗ;
      print_ଧାଡ଼ି (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  ଫେରନ୍ତୁ true;
}
