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
#ವ್ಯಾಖ್ಯಾನಿಸು ಬಫರ್_ಗಾತ್ರ (8)

#ವ್ಯಾಖ್ಯಾನಿಸು LEN(X, I) ((X)->p[(I)].one_past_ಕೊನೆ - (X)->p[(I)].ಆರಂಭ)
#ವ್ಯಾಖ್ಯಾನಿಸು ಖಾಲಿಗಳು_2(X) ((X)->n_bufs == 1 && LEN (X, 0) == 0)

#ವ್ಯಾಖ್ಯಾನಿಸು ONE_PAST_ಕೊನೆ(X, I) ((X)->p[(I)].one_past_ಕೊನೆ)

ರಚನೆ ಸಾಲು_ptr
{
  size_t i;
  ಅಕ್ಷರ *ptr;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಸಾಲು_ptr ಸಾಲು_ptr;

ರಚನೆ B_pair
{
  ಅಕ್ಷರ *ಆರಂಭ;
  ಅಕ್ಷರ *one_past_ಕೊನೆ;
};

ರಚನೆ Buf
{
  size_t n_bufs;
  ರಚನೆ obstack obs;
  ರಚನೆ B_pair *p;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ Buf Buf;

ಸ್ಥಿರ bool
buf_init_from_stdin (Buf *x, ಅಕ್ಷರ eol_byte)
{
  bool ಕೊನೆಯ_byte_is_eol_byte = true;
  bool ok = true;

#ವ್ಯಾಖ್ಯಾನಿಸು OBS (&(x->obs))
  obstack_init (OBS);

  ಆಗ (true)
    {
      ಅಕ್ಷರ *buf = ಸ್ಮರಣೆ_ಕೊಡು (ಬಫರ್_ಗಾತ್ರ);

      ಆದರೆ (buf == ಖಾಲಿ)
        {
          /* Fall back on the code that relies on a temporary file.
             Write all buffers to that file and free them.  */
          /* FIXME */
          ok = false;
          ನಿಲ್ಲಿಸು;
        }
      idx_t ಬೈಟುಗಳು_read = ತುಂಬಿದ_read (STDIN_FILENO, buf, ಬಫರ್_ಗಾತ್ರ);
      ಆದರೆ (ಬೈಟುಗಳು_read != ಬಫರ್_ಗಾತ್ರ && errno != 0)
        error (EXIT_FAILURE, errno, _("read error"));

      {
        ರಚನೆ B_pair bp;
        bp.ಆರಂಭ = buf;
        bp.one_past_ಕೊನೆ = buf + ಬೈಟುಗಳು_read;
        obstack_grow (OBS, &bp, ಗಾತ್ರ (bp));
      }

      ಆದರೆ (ಬೈಟುಗಳು_read != 0)
        ಕೊನೆಯ_byte_is_eol_byte = (buf[ಬೈಟುಗಳು_read - 1] == eol_byte);

      ಆದರೆ (ಬೈಟುಗಳು_read < ಬಫರ್_ಗಾತ್ರ)
        ನಿಲ್ಲಿಸು;
    }

  ಆದರೆ (ok)
    {
      /* If the file was non-empty and lacked an EOL_BYTE at its end,
         then add a buffer containing just that one byte.  */
      ಆದರೆ (!ಕೊನೆಯ_byte_is_eol_byte)
        {
          ಅಕ್ಷರ *buf = ಸ್ಮರಣೆ_ಕೊಡು (1);
          ಆದರೆ (buf == ಖಾಲಿ)
            {
              /* FIXME: just like above */
              ok = false;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ರಚನೆ B_pair bp;
              *buf = eol_byte;
              bp.ಆರಂಭ = buf;
              bp.one_past_ಕೊನೆ = buf + 1;
              obstack_grow (OBS, &bp, ಗಾತ್ರ (bp));
            }
        }
    }

  x->n_bufs = obstack_object_ಗಾತ್ರ (OBS) / ಗಾತ್ರ (x->p[0]);
  x->p = (ರಚನೆ B_pair *) obstack_ಮುಕ್ತಾಯ (OBS);

  /* If there are two or more buffers and the last buffer is empty,
     then free the last one and decrement the buffer count.  */
  ಆದರೆ (x->n_bufs >= 2
      && x->p[x->n_bufs - 1].ಆರಂಭ == x->p[x->n_bufs - 1].one_past_ಕೊನೆ)
    ಬಿಡುಗಡೆ (x->p[--(x->n_bufs)].ಆರಂಭ);

  ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ ಶೂನ್ಯ
buf_free (Buf *x)
{
  ಗಾಗಿ (size_t i = 0; i < x->n_bufs; i++)
    ಬಿಡುಗಡೆ (x->p[i].ಆರಂಭ);
  obstack_free (OBS, ಖಾಲಿ);
}

ಸಾಲು_ptr
ಸಾಲು_ptr_decrement (ಸ್ಥಿರಾಂಕ Buf *x, ಸ್ಥಿರಾಂಕ ಸಾಲು_ptr *lp)
{
  ಸಾಲು_ptr lp_new;

  ಆದರೆ (lp->ptr > x->p[lp->i].ಆರಂಭ)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr - 1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      affirm (lp->i > 0);
      lp_new.i = lp->i - 1;
      lp_new.ptr = ONE_PAST_ಕೊನೆ (x, lp->i - 1) - 1;
    }
  ಹಿಂತಿರುಗಿ lp_new;
}

ಸಾಲು_ptr
ಸಾಲು_ptr_increment (ಸ್ಥಿರಾಂಕ Buf *x, ಸ್ಥಿರಾಂಕ ಸಾಲು_ptr *lp)
{
  ಸಾಲು_ptr lp_new;

  affirm (lp->ptr <= ONE_PAST_ಕೊನೆ (x, lp->i) - 1);
  ಆದರೆ (lp->ptr < ONE_PAST_ಕೊನೆ (x, lp->i) - 1)
    {
      lp_new.i = lp->i;
      lp_new.ptr = lp->ptr + 1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      affirm (lp->i < x->n_bufs - 1);
      lp_new.i = lp->i + 1;
      lp_new.ptr = x->p[lp->i + 1].ಆರಂಭ;
    }
  ಹಿಂತಿರುಗಿ lp_new;
}

ಸ್ಥಿರ bool
ಕಂಡುಹಿಡಿ_bol (ಸ್ಥಿರಾಂಕ Buf *x,
          ಸ್ಥಿರಾಂಕ ಸಾಲು_ptr *ಕೊನೆಯ_bol, ಸಾಲು_ptr *new_bol, ಅಕ್ಷರ eol_byte)
{
  size_t i;
  ಸಾಲು_ptr tmp;
  ಅಕ್ಷರ *ಕೊನೆಯ_bol_ptr;

  ಆದರೆ (ಕೊನೆಯ_bol->ptr == x->p[0].ಆರಂಭ)
    ಹಿಂತಿರುಗಿ false;

  tmp = ಸಾಲು_ptr_decrement (x, ಕೊನೆಯ_bol);
  ಕೊನೆಯ_bol_ptr = tmp.ptr;
  i = tmp.i;
  ಆಗ (true)
    {
      ಅಕ್ಷರ *nl = memrchr (x->p[i].ಆರಂಭ, ಕೊನೆಯ_bol_ptr, eol_byte);
      ಆದರೆ (nl)
        {
          ಸಾಲು_ptr nl_pos;
          nl_pos.i = i;
          nl_pos.ptr = nl;
          *new_bol = ಸಾಲು_ptr_increment (x, &nl_pos);
          ಹಿಂತಿರುಗಿ true;
        }

      ಆದರೆ (i == 0)
        ನಿಲ್ಲಿಸು;

      --i;
      ಕೊನೆಯ_bol_ptr = ONE_PAST_ಕೊನೆ (x, i);
    }

  /* If last_bol->ptr didn't point at the first byte of X, then reaching
     this point means that we're about to return the line that is at the
     beginning of X.  */
  ಆದರೆ (ಕೊನೆಯ_bol->ptr != x->p[0].ಆರಂಭ)
    {
      new_bol->i = 0;
      new_bol->ptr = x->p[0].ಆರಂಭ;
      ಹಿಂತಿರುಗಿ true;
    }

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_ಸಾಲು (FILE *ಹೊರಹೋಗು_ಹರಿವು, ಸ್ಥಿರಾಂಕ Buf *x,
            ಸ್ಥಿರಾಂಕ ಸಾಲು_ptr *bol, ಸ್ಥಿರಾಂಕ ಸಾಲು_ptr *bol_ಮುಂದಿನ)
{
  ಗಾಗಿ (size_t i = bol->i; i <= bol_ಮುಂದಿನ->i; i++)
    {
      ಅಕ್ಷರ *a = (i == bol->i ? bol->ptr : x->p[i].ಆರಂಭ);
      ಅಕ್ಷರ *b = (i == bol_ಮುಂದಿನ->i ? bol_ಮುಂದಿನ->ptr : ONE_PAST_ಕೊನೆ (x, i));
      fwrite (a, 1, b - a, ಹೊರಹೋಗು_ಹರಿವು);
    }
}

ಸ್ಥಿರ bool
tac_mem ()
{
  Buf x;
  ಸಾಲು_ptr bol;
  ಅಕ್ಷರ eol_byte = '\n';

  ಆದರೆ (! buf_init_from_stdin (&x, eol_byte))
    {
      buf_free (&x);
      ಹಿಂತಿರುಗಿ false;
    }

  /* Special case the empty file.  */
  ಆದರೆ (ಖಾಲಿಗಳು_2 (&x))
    ಹಿಂತಿರುಗಿ true;

  /* Initially, point at one past the last byte of the file.  */
  bol.i = x.n_bufs - 1;
  bol.ptr = ONE_PAST_ಕೊನೆ (&x, bol.i);

  ಆಗ (true)
    {
      ಸಾಲು_ptr new_bol;
      ಆದರೆ (! ಕಂಡುಹಿಡಿ_bol (&x, &bol, &new_bol, eol_byte))
        ನಿಲ್ಲಿಸು;
      print_ಸಾಲು (stdout, &x, &new_bol, &bol);
      bol = new_bol;
    }
  ಹಿಂತಿರುಗಿ true;
}
