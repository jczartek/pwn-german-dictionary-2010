/* pwn-german-dictionary-2010.c
 *
 * Copyright 2019 Jakub Czartek <kuba@linux.pl>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <zlib.h>
#include <string.h>
#include "pwn-german-dictionary-2010.h"
#include "pwn-german-dictionary-2010-private.h"

#define SIZE_BUFFER  128
#define SIZE_ENTRY   64
#define DEFAULT_ENCODING "ISO8859-2"

struct _PwnGermanDictionary2010
{
  GyDict parent_instance;
  GFile *file;
  guint *offsets;
};

G_DEFINE_TYPE (PwnGermanDictionary2010, pwn_german_dictionary2010, GY_TYPE_DICT)

PwnGermanDictionary2010 *
pwn_german_dictionary2010_new (const gchar *id)
{
  return g_object_new (PWN_TYPE_GERMAN_DICTIONARY2010, "identifier", id, NULL);
}

static GQuark
pwn_german_dictionary2010_error_quark (void)
{
  return g_quark_from_static_string ("pwn-german-dictionary-2010-error");
}


static void
pwn_german_dictionary2010_map (GyDict  *dict,
                                GError **err)
{
  guint32 word_count = 0, index_base = 0, word_base = 0;
  g_autofree guint32 *offsets = NULL;
  g_autoptr(GFileInputStream) in = NULL;
  g_autoptr(GSettings) settings = NULL;
  g_autofree gchar *path = NULL;
  g_autofree gchar *settings_path = NULL;
  const gchar *id = NULL;
  gchar buf[SIZE_BUFFER];
  gchar entry[SIZE_ENTRY];
  guint16 magic;
  g_autoptr(GtkListStore) model = NULL;
  GtkTreeIter iter;
  PwnGermanDictionary2010 *self = PWN_GERMAN_DICTIONARY2010 (dict);

  g_return_if_fail (GY_IS_DICT (dict));

  g_object_get (dict, "identifier", &id, NULL);

  settings_path = g_strdup_printf ("/org/gtk/gydict/plugin/%s/", id);
  settings = g_settings_new_with_path ("org.gtk.gydict.path", settings_path);
  path = g_settings_get_string (settings, "path");

  if (self->file)
    g_object_unref (self->file);

  self->file = g_file_new_for_path (path);

  model = gtk_list_store_new (1, G_TYPE_STRING);

  if (!(in = g_file_read (self->file, NULL, err)))
    goto out;

  if (!g_seekable_seek (G_SEEKABLE (in), 0x68, G_SEEK_SET, NULL, err))
    goto out;

  if ((g_input_stream_read (G_INPUT_STREAM (in), &word_count, sizeof(word_count), NULL, err)) <= 0)
    goto out;

  if ((g_input_stream_read (G_INPUT_STREAM (in), &index_base, sizeof(index_base), NULL, err)) <= 0)
    goto out;

  if (!g_seekable_seek (G_SEEKABLE (in), 0x04, G_SEEK_CUR, NULL, err))
    goto out;

  if ((g_input_stream_read (G_INPUT_STREAM (in), &word_base, sizeof(word_base), NULL, err)) <= 0)
    goto out;

  offsets = (guint32 *) g_malloc0 ((word_count + 1) * sizeof (guint32));
  self->offsets = (guint32 *) g_malloc0 ((word_count + 1) * sizeof (guint32));

  if (!g_seekable_seek (G_SEEKABLE (in), index_base, G_SEEK_SET, NULL, err))
    goto out;

  if ((g_input_stream_read (G_INPUT_STREAM (in), offsets, (word_count * sizeof (guint32)), NULL, err)) <= 0)
    goto out;

	for (guint i = 0, j = 0; i < word_count; i++)
    {
#define MAGIC_OFFSET 0x03
#define OFFSET (12 - (MAGIC_OFFSET + sizeof (guint16)))

      magic = 0;
      offsets[i] &= 0x07ffffff;

      if (!g_seekable_seek (G_SEEKABLE (in), word_base+offsets[i]+MAGIC_OFFSET, G_SEEK_SET, NULL, err))
        goto out;

      if ((g_input_stream_read (G_INPUT_STREAM (in), &magic, sizeof (guint16), NULL, err)) <= 0)
        goto out;

      if (magic == 0x11dd || magic == 0x11d7)
        {
          g_autofree gchar *buf_conv = NULL;
          gchar *str = NULL;
          gsize len = 0;

          memset (entry, 0, SIZE_ENTRY);

          if ((g_input_stream_read (G_INPUT_STREAM (in), buf, SIZE_BUFFER, NULL, err)) <= 0)
            goto out;

          if (!(buf_conv = g_convert_with_fallback (buf+OFFSET, -1, "UTF-8", DEFAULT_ENCODING, NULL, NULL, NULL, err)))
            goto out;
          str = buf_conv;

          len = strcspn (str, "<&");
          strncat (entry, str,len);
          str = str + len;

          while (*str)
            {
              if (*str == '<')
                {
                  str = str + strcspn (str, ">") + 1;

                  if (g_ascii_isdigit (*str))
                    {
                      const gchar *sscript = get_superscript ((*str) - 48);
                      strcat (entry, sscript);
                    }

                  str = str + strcspn (str, ">") + 1;
                }
              else if (*str == '&')
                {
                  g_autofree gchar *entity = NULL;

                  len = strcspn (str, ";");
                  entity = g_strndup (str, len);

                  strcat (entry, get_entitle (entity));
                  str += len + 1;
                }
              else
                {
                  len = strcspn (str, "<&");
                  strncat (entry, str, len);
                  str = str + len;
                }
            }
          gtk_list_store_append (model, &iter);
          gtk_list_store_set (model, &iter, 0, entry, -1);
          self->offsets[j++] = offsets[i] + word_base;
        }
#undef MAGIC
#undef MAGIC_OFFSET
#undef OFFSET
    }
  g_object_set (dict, "model", model, NULL);
  g_object_set (dict, "is-mapped", TRUE, NULL);
  return;
out:
  g_object_set (dict, "is-mapped", FALSE, NULL);
  return;
}

static gchar *
pwn_german_dictionary2010_get_lexical_unit (GyDict  *dict,
                                             guint    index,
                                             GError **err)
{
  g_autoptr(GFileInputStream) in = NULL;
  gchar *in_buffer = NULL;
  g_autofree gchar *out_buffer = NULL;
  guint i = 0;
  PwnGermanDictionary2010 *self = PWN_GERMAN_DICTIONARY2010 (dict);

  g_return_val_if_fail (gy_dict_is_mapped (GY_DICT (self)), NULL);

#define MAXLEN 1024 * 90
#define OFFSET 12

  in_buffer = (gchar *) g_alloca (MAXLEN);

  if (!(in = g_file_read (self->file, NULL, err)))
    goto out;

  if (!g_seekable_seek (G_SEEKABLE (in), self->offsets[index], G_SEEK_SET, NULL, err))
    goto out;

  if ((g_input_stream_read (G_INPUT_STREAM (in), in_buffer, MAXLEN, NULL, err)) <= 0)
    goto out;

  i = OFFSET + strlen (in_buffer + OFFSET) + 2;

  if (in_buffer[i] < 20)
    {
      gint zerr;
      uLongf destlen;
      i += in_buffer[i]+1;
      destlen = MAXLEN;

      out_buffer = (gchar *) g_malloc0 (MAXLEN);

      if ((zerr = uncompress ((Bytef *)out_buffer, &destlen, (const Bytef *)in_buffer+i, MAXLEN)) != Z_OK)
        {
          switch (zerr)
            {
            case Z_BUF_ERROR:
              g_set_error_literal (err, pwn_german_dictionary2010_error_quark (), zerr,
                           "The buffer out_buffer was not large enough to hold the uncompressed data!");
              break;
            case Z_MEM_ERROR:
              g_set_error_literal (err, pwn_german_dictionary2010_error_quark (), zerr,
                           "Insufficient memory!");
              break;
            case Z_DATA_ERROR:
              g_set_error_literal (err, pwn_german_dictionary2010_error_quark (), zerr,
                           "The compressed data (referenced by in_buffer) was corrupted!");
              break;
            default:
              ;
            };
          goto out;
        }
    }
  else
    {
      out_buffer = g_strdup (in_buffer + i);
    }

  return g_convert_with_fallback (out_buffer, -1, "UTF-8", DEFAULT_ENCODING, NULL, NULL, NULL, err);
out:
  return NULL;
#undef MAXLEN
#undef OFFSET
}


static void
pwn_german_dictionary2010_finalize (GObject *object)
{
  PwnGermanDictionary2010 *self = (PwnGermanDictionary2010 *)object;

  if (self->file) g_clear_object (&self->file);

  if (self->offsets) g_clear_pointer (&self->offsets, g_free);

  G_OBJECT_CLASS (pwn_german_dictionary2010_parent_class)->finalize (object);
}

static inline const gchar *
skip_space (const gchar *s)
{
  while (*s == ' ' || *s == '\t')
    s++;
  return s;
}

static gboolean
pwn_german_dictionary2010_parse (GyDict         *self,
                                  const gchar    *raw_text,
                                  gint            length,
                                  GyTextAttrList **attr_list,
                                  gchar         **text,
                                  GError        **err)
{
  GString *s = g_string_new (NULL);
  GSList *stack = NULL;
  GyTextAttribute *attr = NULL;
  GyTextAttrList *to_apply = gy_text_attr_list_new ();

  const gchar *p, *q;
  p = q = raw_text;

  while (*q)
    {
      if (*q == '&')
        {
          p = q;

          while (';' != *q) // go to ';'
            q++;

          g_string_append (s, get_entitle (p));
          q++; // go behind ';'
          continue;
        }
      else if (*q == '<')
        {
          q++; // go behind '<'
          q = skip_space (q);

          if (*q != '/') // open tag
            {
              p = q;
              while (g_ascii_isalnum (*q))
                q++;

              if (strncmp (p, "B", q-p) == 0)
                {
                  attr = gy_text_attribute_weight_new (PANGO_WEIGHT_BOLD);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "BIG", q-p) == 0)
                {
                  attr = gy_text_attribute_scale_new (PANGO_SCALE_LARGE);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "P", q-p) == 0)
                {
                  if (s->len > 0 && s->str[s->len - 1] != '\n') // Prevent insert double '\n'
                    g_string_append_c (s, '\n');
                }
              else if (strncmp (p, "PH", q-p) == 0)
                {
                  PangoColor color = {0,0,0};
                  pango_color_parse (&color, "#a347ba");

                  attr = gy_text_attribute_foreground_new (color.red, color.green, color.blue);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;

                  attr = gy_text_attribute_weight_new (PANGO_WEIGHT_SEMIBOLD);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;

                  attr = gy_text_attribute_stretch_new (PANGO_STRETCH_SEMI_EXPANDED);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;
                }
              else if (strncmp(p, "DE", q-p) == 0)
                {
                  /* Nothing to do */
                }
              else if (strncmp(p, "PL", q-p) == 0)
                {
                  /* Nothing to do */
                }
              else if (strncmp (p, "I", q-p) == 0)
                {
                  attr = gy_text_attribute_style_new (PANGO_STYLE_ITALIC);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "SMALL", q-p) == 0)
                {
                  attr = gy_text_attribute_scale_new (PANGO_SCALE_SMALL);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "TEXTSECTION", q-p) == 0)
                {
                  /* Nothing to do */
                }
              else if (strncmp (p, "HANGINGPAR", q-p) == 0)
                {
                  /* Nothing to do */
                }
              else if (strncmp (p, "IMG", q-p) == 0)
                {
                  // atrr="val"
                  q = skip_space (q);

                  while (*q && *q != '"') // go to the first '"' of the value
                      q++;
                  q++; // go behind '"'

                  p = q;
                  while (g_ascii_isalnum (*q) || *q == '.')
                    q++;

                  q++; // go behind '"'

                  if (g_str_has_prefix (p, "rzym"))
                    {
                      p += 4;
                      if (g_ascii_isdigit (*p) && *(p+1) == '.')
                        {
                          g_autofree gchar *roman = convert_to_roman ((int) *p - 48);
                          g_string_append (s, roman);
                        }
                    }
                  else if (g_str_has_prefix (p, "idioms"))
                    {
                      g_string_append (s, "IDIOM");
                    }

                }
              else if (strncmp (p, "SUB", q-p) == 0)
                {
                  attr = gy_text_attribute_weight_new (PANGO_WEIGHT_BOLD);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;

                  attr = gy_text_attribute_scale_new (PANGO_SCALE_LARGE);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;

                  attr = gy_text_attribute_family_new ("serif");
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "ICON", q-p) == 0)
                {
                  /* Nothing to do */
                }
              else if (strncmp (p, "SUP", q-p) == 0)
                {
                  attr = gy_text_attribute_scale_new (PANGO_SCALE_X_SMALL);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;

                  attr = gy_text_attribute_rise_new (PANGO_SCALE * 6);
                  gy_text_attribute_set_start_index (attr, s->len);
                  stack = g_slist_append (stack, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "A", q-p) == 0)
                {
                }
              else if (strncmp (p, "N", q-p) == 0)
                {
                }
              else if (strncmp (p, "BR", q-p) == 0)
                {
                }
              else if (strncmp (p, "BOOKMARK", q-p) == 0)
                {
                }
              else if (strncmp (p, "GOBACK", q-p) == 0)
                {
                }
              else
                {
                  g_autofree gchar* tag = NULL;
                  tag = g_strndup (p, q-p);
                  g_message ("<UNKNOWN TAG: %s>", tag);
                }
            }
          else // close tag
            {
              q++; // go begind '/'
              q = skip_space (q);
              p = q;
              while (g_ascii_isalnum (*q))
                q++;

              if (strncmp (p, "B", q-p) == 0)
                {
                  GSList *last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "BIG", q-p) == 0)
                {
                  GSList *last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "P", q-p) == 0)
                {
                  /* Nothing to do */
                }
              else if (strncmp (p, "PH", q-p) == 0)
                {
                  GSList *last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;

                  last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;

                  last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;
                }
              else if (strncmp(p, "DE", q-p) == 0)
                {
                }
              else if (strncmp(p, "PL", q-p) == 0)
                {
                }
              else if (strncmp (p, "I", q-p) == 0)
                {
                  GSList *last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "SMALL", q-p) == 0)
                {
                  GSList *last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "SUB", q-p) == 0)
                {
                  GSList *last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;

                  last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;

                  last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "SUP", q-p) == 0)
                {
                  GSList *last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;

                  last = g_slist_last (stack);
                  attr = last->data;
                  stack = g_slist_remove_link (stack, last);
                  g_slist_free (last);
                  gy_text_attribute_set_end_index (attr, s->len);
                  gy_text_attr_list_insert_before (to_apply, attr);
                  attr = NULL;
                }
              else if (strncmp (p, "A", q-p) == 0)
                {
                }
              else if (strncmp (p, "N", q-p) == 0)
                {
                }
              else if (strncmp (p, "BOOKMARK", q-p) == 0)
                {
                }
              else
                {
                  g_autofree gchar* tag = NULL;
                  tag = g_strndup (p, q-p);
                  g_message ("<UNKNOWN TAG: /%s>", tag);
                }
            }

          while (*q && *q != '>')
            q++;
          q++; // go behind '>'
          continue;
        }
      else
        {
          p = q;
          while (*q && *q != '&' && *q != '<')
            q++;

          if (*p)
            {
              g_string_append_len (s, p, q-p);
            }
          continue;
        }
    }

  if (g_slist_length (stack) > 0)
    g_critical ("Stack with the pango attrs is not empty. Nth: %d", g_slist_length (stack));
  g_slist_free (stack);

  if (attr_list != NULL)
    *attr_list = to_apply;

  *text = g_string_free (s, FALSE);
  return TRUE;
}


static void
pwn_german_dictionary2010_class_init (PwnGermanDictionary2010Class *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GyDictClass *dict_class = GY_DICT_CLASS (klass);

  object_class->finalize = pwn_german_dictionary2010_finalize;

  dict_class->map = pwn_german_dictionary2010_map;
  dict_class->get_lexical_unit = pwn_german_dictionary2010_get_lexical_unit;
  dict_class->parse = pwn_german_dictionary2010_parse;
}

static void
pwn_german_dictionary2010_init (PwnGermanDictionary2010 *self)
{
}
