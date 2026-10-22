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

संरचना ओळ_ptr
{
  माप_प्रकार i;
  अक्षर *ptr;
};
प्रकारव्याख्या संरचना ओळ_ptr ओळ_ptr;

संरचना B_pair
{
  अक्षर *start;
  अक्षर *one_past_end;
};

संरचना Buf
{
  माप_प्रकार n_bufs;
  संरचना obstack obs;
  संरचना B_pair *p;
};
प्रकारव्याख्या संरचना Buf Buf;

स्थिर bool
buf_init_from_stdin (Buf *x, अक्षर eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  पर्यंत (true)
    {
      अक्षर *buf = दो_स्मृति (BUFFER_SIZE);

      जर (buf == रिक्त)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          थांबा;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      जर (bytes_read != BUFFER_SIZE && त्रुटि_सं != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

      {
        संरचना B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, आकार (bp));
      }

      जर (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      जर (bytes_read < BUFFER_SIZE)
        थांबा;
    }

  जर (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      जर (!last_byte_is_eol_byte)
        {
          अक्षर *buf = दो_स्मृति (1);
          जर (buf == रिक्त)
            {
              /* FIXME: just like above */
              ok = false;
            }
          अन्यथा
            {
              संरचना B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, आकार (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / आकार (x->p[0]);
  x->p = (संरचना B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  जर (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    मुक्त (x->p[--(x->n_bufs)].start);

  परत ok;
}

स्थिर शून्य
buf_free (Buf *x)
{
  साठी (माप_प्रकार i = 0; i < x->n_bufs; i++)
    मुक्त (x->p[i].start);
  obstack_free (OBS, रिक्त);
}

ओळ_ptr
ओळ_ptr_decrement (स्थिरांक Buf *x, स्थिरांक ओळ_ptr *lp)
{
  ओळ_ptr lp_new;

  जर (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  अन्यथा
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  परत lp_new;
}

ओळ_ptr
ओळ_ptr_increment (स्थिरांक Buf *x, स्थिरांक ओळ_ptr *lp)
{
  ओळ_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  जर (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  अन्यथा
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  परत lp_new;
}

स्थिर bool
find_bol (स्थिरांक Buf *x,
          स्थिरांक ओळ_ptr *last_bol, ओळ_ptr *new_bol, अक्षर eol_byte)
{
  माप_प्रकार i;
  ओळ_ptr tmp;
  अक्षर *last_bol_ptr;

  जर (last_bol->ptr == x->p[0].start)
    परत false;

  tmp = ओळ_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  पर्यंत (true)
    {
      अक्षर *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      जर (nl)
        {
          ओळ_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = ओळ_ptr_increment (x, &nl_pos);
          परत true;
        }

      जर (i == 0)
        थांबा;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  जर (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      परत true;
    }

  परत false;
}

स्थिर शून्य
print_ओळ (खाता *out_stream, स्थिरांक Buf *x,
            स्थिरांक ओळ_ptr *bol, स्थिरांक ओळ_ptr *bol_next)
{
  साठी (माप_प्रकार i = bol->i; i <= bol_next->i; i++)
    {
      अक्षर *a = (i == bol->i ? bol->ptr : x->p[i].start);
      अक्षर *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      ख_डालो (a, 1, b - a, out_stream);
    }
}

स्थिर bool
tac_mem ()
{
  Buf x;
  ओळ_ptr bol;
  अक्षर eol_byte = '\n';

  जर (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      परत false;
    }

  /* Special case the empty file.  */
  जर (EMPTY (&x))
    परत true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  पर्यंत (true)
    {
      ओळ_ptr new_bol;
      जर (! find_bol (&x, &bol, &new_bol, eol_byte))
        थांबा;
      print_ओळ (मानक_निकास, &x, &new_bol, &bol);
      bol = new_bol;
    }
  परत true;
}
