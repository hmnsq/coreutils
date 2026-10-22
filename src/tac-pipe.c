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
#অন্তর্ভাৱ "assure.h"

/* FIXME: this is small for testing */
#define BUFFER_SIZE (8)

#define LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#define EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#define ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

সংরচনা সাৰি_ptr
{
  size_t i;
  ৱর্ণ *ptr;
};
প্রকারনির্দেশ সংরচনা সাৰি_ptr সাৰি_ptr;

সংরচনা B_pair
{
  ৱর্ণ *start;
  ৱর্ণ *one_past_end;
};

সংরচনা Buf
{
  size_t n_bufs;
  সংরচনা obstack obs;
  সংরচনা B_pair *p;
};
প্রকারনির্দেশ সংরচনা Buf Buf;

স্থির bool
buf_init_from_stdin (Buf *x, ৱর্ণ eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  যেতিয়ালৈ (true)
    {
      ৱর্ণ *buf = malloc (BUFFER_SIZE);

      যদি (buf == রিক্ত)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          ভঙ্গ;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      যদি (bytes_read != BUFFER_SIZE && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        সংরচনা B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, পরিমাণ (bp));
      }

      যদি (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      যদি (bytes_read < BUFFER_SIZE)
        ভঙ্গ;
    }

  যদি (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      যদি (!last_byte_is_eol_byte)
        {
          ৱর্ণ *buf = malloc (1);
          যদি (buf == রিক্ত)
            {
              /* FIXME: just like above */
              ok = false;
            }
          অন্যথা
            {
              সংরচনা B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, পরিমাণ (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / পরিমাণ (x->p[0]);
  x->p = (সংরচনা B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  যদি (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    মুক্ত (x->p[--(x->n_bufs)].start);

  ঘূৰাই ok;
}

স্থির শূন্য
buf_free (Buf *x)
{
  বাবে (size_t i = 0; i < x->n_bufs; i++)
    মুক্ত (x->p[i].start);
  obstack_free (OBS, রিক্ত);
}

সাৰি_ptr
সাৰি_ptr_decrement (ধ্রুৱ Buf *x, ধ্রুৱ সাৰি_ptr *lp)
{
  সাৰি_ptr lp_new;

  যদি (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  অন্যথা
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  ঘূৰাই lp_new;
}

সাৰি_ptr
সাৰি_ptr_increment (ধ্রুৱ Buf *x, ধ্রুৱ সাৰি_ptr *lp)
{
  সাৰি_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  যদি (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  অন্যথা
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  ঘূৰাই lp_new;
}

স্থির bool
find_bol (ধ্রুৱ Buf *x,
          ধ্রুৱ সাৰি_ptr *last_bol, সাৰি_ptr *new_bol, ৱর্ণ eol_byte)
{
  size_t i;
  সাৰি_ptr tmp;
  ৱর্ণ *last_bol_ptr;

  যদি (last_bol->ptr == x->p[0].start)
    ঘূৰাই false;

  tmp = সাৰি_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  যেতিয়ালৈ (true)
    {
      ৱর্ণ *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      যদি (nl)
        {
          সাৰি_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = সাৰি_ptr_increment (x, &nl_pos);
          ঘূৰাই true;
        }

      যদি (i == 0)
        ভঙ্গ;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  যদি (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      ঘূৰাই true;
    }

  ঘূৰাই false;
}

স্থির শূন্য
print_সাৰি (FILE *out_stream, ধ্রুৱ Buf *x,
            ধ্রুৱ সাৰি_ptr *bol, ধ্রুৱ সাৰি_ptr *bol_next)
{
  বাবে (size_t i = bol->i; i <= bol_next->i; i++)
    {
      ৱর্ণ *a = (i == bol->i ? bol->ptr : x->p[i].start);
      ৱর্ণ *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      fwrite (a, 1, b - a, out_stream);
    }
}

স্থির bool
tac_mem ()
{
  Buf x;
  সাৰি_ptr bol;
  ৱর্ণ eol_byte = '\n';

  যদি (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      ঘূৰাই false;
    }

  /* Special case the empty file.  */
  যদি (EMPTY (&x))
    ঘূৰাই true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  যেতিয়ালৈ (true)
    {
      সাৰি_ptr new_bol;
      যদি (! find_bol (&x, &bol, &new_bol, eol_byte))
        ভঙ্গ;
      print_সাৰি (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  ঘূৰাই true;
}
