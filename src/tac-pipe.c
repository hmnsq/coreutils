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
#include "assure.h"

/* FIXME: this is small for testing */
#define BUFFER_SIZE (8)

#define LEN(X, I) ((X)->p[(I)].one_past_end - (X)->p[(I)].start)
#define EMPTY(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#define ONE_PAST_END(X, I) ((X)->p[(I)].one_past_end)

sa_nracanaa Line_ptr
{
  size_t i;
  var_na *ptr;
};
typedef sa_nracanaa Line_ptr Line_ptr;

sa_nracanaa B_pair
{
  var_na *start;
  var_na *one_past_end;
};

sa_nracanaa Buf
{
  size_t n_bufs;
  sa_nracanaa obstack obs;
  sa_nracanaa B_pair *p;
};
typedef sa_nracanaa Buf Buf;

static bool
buf_init_from_stdin (Buf *x, var_na eol_byte)
{
  bool last_byte_is_eol_byte = true;
  bool ok = true;

#define OBS (&(x->obs))
  obstack_init (OBS);

  ໃນຂະນະທີ່ (true)
    {
      var_na *buf = malloc (BUFFER_SIZE);

      ຖ້າ (buf == NULL)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          bha_nga;
        }
      idx_t bytes_read = full_read (STDIN_FILENO, buf, BUFFER_SIZE);
      ຖ້າ (bytes_read != BUFFER_SIZE && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        sa_nracanaa B_pair bp;
        bp.start = buf;
        bp.one_past_end = buf + bytes_read;
        obstack_grow (OBS, &bp, parimaa_na (bp));
      }

      ຖ້າ (bytes_read != 0)
        last_byte_is_eol_byte = (buf[bytes_read - 1] == eol_byte);

      ຖ້າ (bytes_read < BUFFER_SIZE)
        bha_nga;
    }

  ຖ້າ (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      ຖ້າ (!last_byte_is_eol_byte)
        {
          var_na *buf = malloc (1);
          ຖ້າ (buf == NULL)
            {
              /* FIXME: just like above */
              ok = false;
            }
          ບໍ່ດັ່ງນັ້ນ
            {
              sa_nracanaa B_pair bp;
              *buf = eol_byte;
              bp.start = buf;
              bp.one_past_end = buf + 1;
              obstack_grow (OBS, &bp, parimaa_na (bp));
            }
        }
    }

  x->n_bufs = obstack_object_size (OBS) / parimaa_na (x->p[0]);
  x->p = (sa_nracanaa B_pair *) obstack_finish (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  ຖ້າ (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].start == x->p[x->n_bufs - 1].one_past_end)
    free (x->p[--(x->n_bufs)].start);

  ສົ່ງຄືນ ok;
}

static void
buf_free (Buf *x)
{
  ສຳລັບ (size_t i = 0; i < x->n_bufs; i++)
    free (x->p[i].start);
  obstack_free (OBS, NULL);
}

Line_ptr
line_ptr_decrement (const Buf *x, const Line_ptr *lp)
{
  Line_ptr lp_new;

  ຖ້າ (lp->ptr > x->p[lp->i].start)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_END (x, lp->i - 1) - 1;
    }
  ສົ່ງຄືນ lp_new;
}

Line_ptr
line_ptr_increment (const Buf *x, const Line_ptr *lp)
{
  Line_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_END (x, lp->i) - 1);
  ຖ້າ (lp->ptr < ONE_PAST_END (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].start;
    }
  ສົ່ງຄືນ lp_new;
}

static bool
find_bol (const Buf *x,
          const Line_ptr *last_bol, Line_ptr *new_bol, var_na eol_byte)
{
  size_t i;
  Line_ptr tmp;
  var_na *last_bol_ptr;

  ຖ້າ (last_bol->ptr == x->p[0].start)
    ສົ່ງຄືນ false;

  tmp = line_ptr_decrement (x, last_bol);
  last_bol_ptr = tmp.ptr;
  i = tmp.i;
  ໃນຂະນະທີ່ (true)
    {
      var_na *nl = memrchr (x->p[i].start, last_bol_ptr, eol_byte);
      ຖ້າ (nl)
        {
          Line_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = line_ptr_increment (x, &nl_pos);
          ສົ່ງຄືນ true;
        }

      ຖ້າ (i == 0)
        bha_nga;

      --i;
      last_bol_ptr = ONE_PAST_END (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  ຖ້າ (last_bol->ptr != x->p[0].start)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].start;
      ສົ່ງຄືນ true;
    }

  ສົ່ງຄືນ false;
}

static void
print_line (FILE *out_stream, const Buf *x,
            const Line_ptr *bol, const Line_ptr *bol_next)
{
  ສຳລັບ (size_t i = bol->i; i <= bol_next->i; i++)
    {
      var_na *a = (i == bol->i ? bol->ptr : x->p[i].start);
      var_na *b = (i == bol_next->i ? bol_next->ptr : ONE_PAST_END (x, i));
      fwrite (a, 1, b - a, out_stream);
    }
}

static bool
tac_mem ()
{
  Buf x;
  Line_ptr bol;
  var_na eol_byte = '\n';

  ຖ້າ (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      ສົ່ງຄືນ false;
    }

  /* Special case the empty file.  */
  ຖ້າ (EMPTY (&x))
    ສົ່ງຄືນ true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_END (&x, bol.i);

  ໃນຂະນະທີ່ (true)
    {
      Line_ptr new_bol;
      ຖ້າ (! find_bol (&x, &bol, &new_bol, eol_byte))
        bha_nga;
      print_line (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  ສົ່ງຄືນ true;
}
