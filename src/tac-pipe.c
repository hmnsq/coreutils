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
#ಅನ್ತರ್ಭಾವ "assure.h"

/* FIXME: this is small for testing */
#define BUFFER_SIZE (8)

#define LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#define EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#define ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

ಸಂರಚನಾ ಸಾಲು_ptr
{
  size_t i;
  ವರ್ಣ *ptr;
};
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ಸಾಲು_ptr ಸಾಲು_ptr;

ಸಂರಚನಾ B_pair
{
  ವರ್ಣ *start;
  ವರ್ಣ *one_past_end;
};

ಸಂರಚನಾ Buf
{
  size_t n_bufs;
  ಸಂರಚನಾ obstack obs;
  ಸಂರಚನಾ B_pair *p;
};
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ Buf Buf;

ಸ್ಥಿರ bool
buf_init_from_stdin (Buf *x, ವರ್ಣ eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  ಯಾವತ್ (true)
    {
      ವರ್ಣ *buf = malloc (BUFFER_SIZE);

      ಯದಿ (buf == ರಿಕ್ತ)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          ಭಙ್ಗ;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      ಯದಿ (bytes_read != BUFFER_SIZE && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        ಸಂರಚನಾ B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, ಪರಿಮಾಣ (bp));
      }

      ಯದಿ (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      ಯದಿ (bytes_read < BUFFER_SIZE)
        ಭಙ್ಗ;
    }

  ಯದಿ (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      ಯದಿ (!last_byte_is_eol_byte)
        {
          ವರ್ಣ *buf = malloc (1);
          ಯದಿ (buf == ರಿಕ್ತ)
            {
              /* FIXME: just like above */
              ok = false;
            }
          ಅನ್ಯಥಾ
            {
              ಸಂರಚನಾ B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, ಪರಿಮಾಣ (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / ಪರಿಮಾಣ (x->p[0]);
  x->p = (ಸಂರಚನಾ B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  ಯದಿ (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    ಮುಕ್ತ (x->p[--(x->n_bufs)].start);

  ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ ಶೂನ್ಯ
buf_free (Buf *x)
{
  ಪ್ರತಿ (size_t i = 0; i < x->n_bufs; i++)
    ಮುಕ್ತ (x->p[i].start);
  obstack_free (OBS, ರಿಕ್ತ);
}

ಸಾಲು_ptr
ಸಾಲು_ptr_decrement (ಧ್ರುವ Buf *x, ಧ್ರುವ ಸಾಲು_ptr *lp)
{
  ಸಾಲು_ptr lp_new;

  ಯದಿ (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  ಅನ್ಯಥಾ
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  ಪ್ರತ್ಯಾಗಮ lp_new;
}

ಸಾಲು_ptr
ಸಾಲು_ptr_increment (ಧ್ರುವ Buf *x, ಧ್ರುವ ಸಾಲು_ptr *lp)
{
  ಸಾಲು_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  ಯದಿ (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  ಅನ್ಯಥಾ
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  ಪ್ರತ್ಯಾಗಮ lp_new;
}

ಸ್ಥಿರ bool
find_bol (ಧ್ರುವ Buf *x,
          ಧ್ರುವ ಸಾಲು_ptr *last_bol, ಸಾಲು_ptr *new_bol, ವರ್ಣ eol_byte)
{
  size_t i;
  ಸಾಲು_ptr tmp;
  ವರ್ಣ *last_bol_ptr;

  ಯದಿ (last_bol->ptr == x->p[0].start)
    ಪ್ರತ್ಯಾಗಮ false;

  tmp = ಸಾಲು_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  ಯಾವತ್ (true)
    {
      ವರ್ಣ *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      ಯದಿ (nl)
        {
          ಸಾಲು_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = ಸಾಲು_ptr_increment (x, &nl_pos);
          ಪ್ರತ್ಯಾಗಮ true;
        }

      ಯದಿ (i == 0)
        ಭಙ್ಗ;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  ಯದಿ (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      ಪ್ರತ್ಯಾಗಮ true;
    }

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_ಸಾಲು (FILE *out_stream, ಧ್ರುವ Buf *x,
            ಧ್ರುವ ಸಾಲು_ptr *bol, ಧ್ರುವ ಸಾಲು_ptr *bol_next)
{
  ಪ್ರತಿ (size_t i = bol->i; i <= bol_next->i; i++)
    {
      ವರ್ಣ *a = (i == bol->i ? bol->ptr : x->p[i].start);
      ವರ್ಣ *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      fwrite (a, 1, b - a, out_stream);
    }
}

ಸ್ಥಿರ bool
tac_mem ()
{
  Buf x;
  ಸಾಲು_ptr bol;
  ವರ್ಣ eol_byte = '\n';

  ಯದಿ (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      ಪ್ರತ್ಯಾಗಮ false;
    }

  /* Special case the empty file.  */
  ಯದಿ (EMPTY (&x))
    ಪ್ರತ್ಯಾಗಮ true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  ಯಾವತ್ (true)
    {
      ಸಾಲು_ptr new_bol;
      ಯದಿ (! find_bol (&x, &bol, &new_bol, eol_byte))
        ಭಙ್ಗ;
      print_ಸಾಲು (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  ಪ್ರತ್ಯಾಗಮ true;
}
