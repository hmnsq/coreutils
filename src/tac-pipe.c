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
#अन्तर्भाव "assure.h"

/* FIXME: this is small for testing */
#define BUFFER_SIZE (8)

#define LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#define EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#define ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

संरचना पङ्क्तिः_ptr
{
  size_t i;
  वर्ण *ptr;
};
प्रकारनिर्देश संरचना पङ्क्तिः_ptr पङ्क्तिः_ptr;

संरचना B_pair
{
  वर्ण *start;
  वर्ण *one_past_end;
};

संरचना Buf
{
  size_t n_bufs;
  संरचना obstack obs;
  संरचना B_pair *p;
};
प्रकारनिर्देश संरचना Buf Buf;

स्थिर bool
buf_init_from_stdin (Buf *x, वर्ण eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  याव (true)
    {
      वर्ण *buf = malloc (BUFFER_SIZE);

      सचे (buf == रिक्त)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          भङ्ग;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      सचे (bytes_read != BUFFER_SIZE && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        संरचना B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, परिमाण (bp));
      }

      सचे (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      सचे (bytes_read < BUFFER_SIZE)
        भङ्ग;
    }

  सचे (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      सचे (!last_byte_is_eol_byte)
        {
          वर्ण *buf = malloc (1);
          सचे (buf == रिक्त)
            {
              /* FIXME: just like above */
              ok = false;
            }
          अञ्ञथा
            {
              संरचना B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, परिमाण (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / परिमाण (x->p[0]);
  x->p = (संरचना B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  सचे (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    मुक्त (x->p[--(x->n_bufs)].start);

  पटिनिवत्त ok;
}

स्थिर शून्य
buf_free (Buf *x)
{
  अत्थाय (size_t i = 0; i < x->n_bufs; i++)
    मुक्त (x->p[i].start);
  obstack_free (OBS, रिक्त);
}

पङ्क्तिः_ptr
पङ्क्तिः_ptr_decrement (ध्रुव Buf *x, ध्रुव पङ्क्तिः_ptr *lp)
{
  पङ्क्तिः_ptr lp_new;

  सचे (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  अञ्ञथा
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  पटिनिवत्त lp_new;
}

पङ्क्तिः_ptr
पङ्क्तिः_ptr_increment (ध्रुव Buf *x, ध्रुव पङ्क्तिः_ptr *lp)
{
  पङ्क्तिः_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  सचे (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  अञ्ञथा
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  पटिनिवत्त lp_new;
}

स्थिर bool
find_bol (ध्रुव Buf *x,
          ध्रुव पङ्क्तिः_ptr *last_bol, पङ्क्तिः_ptr *new_bol, वर्ण eol_byte)
{
  size_t i;
  पङ्क्तिः_ptr tmp;
  वर्ण *last_bol_ptr;

  सचे (last_bol->ptr == x->p[0].start)
    पटिनिवत्त false;

  tmp = पङ्क्तिः_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  याव (true)
    {
      वर्ण *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      सचे (nl)
        {
          पङ्क्तिः_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = पङ्क्तिः_ptr_increment (x, &nl_pos);
          पटिनिवत्त true;
        }

      सचे (i == 0)
        भङ्ग;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  सचे (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      पटिनिवत्त true;
    }

  पटिनिवत्त false;
}

स्थिर शून्य
print_पङ्क्तिः (FILE *out_stream, ध्रुव Buf *x,
            ध्रुव पङ्क्तिः_ptr *bol, ध्रुव पङ्क्तिः_ptr *bol_next)
{
  अत्थाय (size_t i = bol->i; i <= bol_next->i; i++)
    {
      वर्ण *a = (i == bol->i ? bol->ptr : x->p[i].start);
      वर्ण *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      fwrite (a, 1, b - a, out_stream);
    }
}

स्थिर bool
tac_mem ()
{
  Buf x;
  पङ्क्तिः_ptr bol;
  वर्ण eol_byte = '\n';

  सचे (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      पटिनिवत्त false;
    }

  /* Special case the empty file.  */
  सचे (EMPTY (&x))
    पटिनिवत्त true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  याव (true)
    {
      पङ्क्तिः_ptr new_bol;
      सचे (! find_bol (&x, &bol, &new_bol, eol_byte))
        भङ्ग;
      print_पङ्क्तिः (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  पटिनिवत्त true;
}
