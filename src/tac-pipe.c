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
#समावेश "assure.h"

/* FIXME: this is small for testing */
#घोषणा बफर_आकार (8)

#घोषणा LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#घोषणा EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#घोषणा ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

काष्ठा पंक्ति_ptr
{
  माप_प्रकार i;
  अक्षर *ptr;
};
प्रकार काष्ठा पंक्ति_ptr पंक्ति_ptr;

काष्ठा B_pair
{
  अक्षर *start;
  अक्षर *one_past_end;
};

काष्ठा Buf
{
  माप_प्रकार n_bufs;
  काष्ठा obstack obs;
  काष्ठा B_pair *p;
};
प्रकार काष्ठा Buf Buf;

अटल bool
buf_init_from_stdin (Buf *x, अक्षर eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#घोषणा OBS (&(x->obs))
  obstack_init (OBS);

  जबतक (true)
    {
      अक्षर *buf = दो_स्मृति (बफर_आकार);

      अगर (buf == शून्य)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          अवरोध;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, बफर_आकार);
      अगर (bytes_read != बफर_आकार && त्रुटि_सं != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("read error"));

      {
        काष्ठा B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, माप (bp));
      }

      अगर (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      अगर (bytes_read < बफर_आकार)
        अवरोध;
    }

  अगर (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      अगर (!last_byte_is_eol_byte)
        {
          अक्षर *buf = दो_स्मृति (1);
          अगर (buf == शून्य)
            {
              /* FIXME: just like above */
              ok = false;
            }
          अन्यथा
            {
              काष्ठा B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, माप (bp));
            }
        }
    }

  x->n_bufs = obstack_object_आकार (OBS) / माप (x->p[0]);
  x->p = (काष्ठा B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  अगर (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    मुक्त (x->p[--(x->n_bufs)].start);

  वापस ok;
}

अटल व्योम
buf_free (Buf *x)
{
  क्रम (माप_प्रकार i = 0; i < x->n_bufs; i++)
    मुक्त (x->p[i].start);
  obstack_free (OBS, शून्य);
}

पंक्ति_ptr
पंक्ति_ptr_decrement (स्थिर Buf *x, स्थिर पंक्ति_ptr *lp)
{
  पंक्ति_ptr lp_new;

  अगर (lp->ptr > x->p[lp->i].start)
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
  वापस lp_new;
}

पंक्ति_ptr
पंक्ति_ptr_increment (स्थिर Buf *x, स्थिर पंक्ति_ptr *lp)
{
  पंक्ति_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  अगर (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
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
  वापस lp_new;
}

अटल bool
find_bol (स्थिर Buf *x,
          स्थिर पंक्ति_ptr *last_bol, पंक्ति_ptr *new_bol, अक्षर eol_byte)
{
  माप_प्रकार i;
  पंक्ति_ptr tmp;
  अक्षर *last_bol_ptr;

  अगर (last_bol->ptr == x->p[0].start)
    वापस false;

  tmp = पंक्ति_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  जबतक (true)
    {
      अक्षर *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      अगर (nl)
        {
          पंक्ति_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = पंक्ति_ptr_increment (x, &nl_pos);
          वापस true;
        }

      अगर (i == 0)
        अवरोध;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  अगर (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      वापस true;
    }

  वापस false;
}

अटल व्योम
print_पंक्ति (खाता *out_stream, स्थिर Buf *x,
            स्थिर पंक्ति_ptr *bol, स्थिर पंक्ति_ptr *bol_अग्र)
{
  क्रम (माप_प्रकार i = bol->i; i <= bol_अग्र->i; i++)
    {
      अक्षर *a = (i == bol->i ? bol->ptr : x->p[i].start);
      अक्षर *b = (i == bol_अग्र->i ? bol_अग्र->ptr : ONE_PAST_END (x, i));
      ख_डालो (a, 1, b - a, out_stream);
    }
}

अटल bool
tac_mem ()
{
  Buf x;
  पंक्ति_ptr bol;
  अक्षर eol_byte = '\n';

  अगर (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      वापस false;
    }

  /* Special case the empty file.  */
  अगर (EMPTY (&x))
    वापस true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  जबतक (true)
    {
      पंक्ति_ptr new_bol;
      अगर (! find_bol (&x, &bol, &new_bol, eol_byte))
        अवरोध;
      print_पंक्ति (मानक_निकास, &x, &new_bol, &bol);
      bol = new_bol;
    }
  वापस true;
}
