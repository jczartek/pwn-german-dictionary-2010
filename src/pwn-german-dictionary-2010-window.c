/* pwn-german-dictionary-2010-window.c
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

#include <gydict.h>
#include "pwn-german-dictionary-2010-window.h"
#include "pwn-german-dictionary-2010.h"

#define ID_GERMAN_POLISH "pwn-german-polish-dictionary-2010"
#define ID_POLISH_GERMAN "pwn-polish-german-dictionary-2010"

static void pwn_german_dictionary2010_window_iface_init (GyWindowAddinInterface *iface);

struct _PwnGermanDictionary2010Window
{
  GObject parent_instance;
  guint id_menu;
};

G_DEFINE_TYPE_WITH_CODE (PwnGermanDictionary2010Window, pwn_german_dictionary2010_window, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GY_TYPE_WINDOW_ADDIN, pwn_german_dictionary2010_window_iface_init))

static void
pwn_german_dictionary2010_window_class_init (PwnGermanDictionary2010WindowClass *klass)
{
}

static void
pwn_german_dictionary2010_window_init (PwnGermanDictionary2010Window *self)
{
}

static void
pwn_german_dictionary2010_window_load (GyWindowAddin *addin,
                                        GyWindow      *win)
{
  PwnGermanDictionary2010Window *self = PWN_GERMAN_DICTIONARY2010_WINDOW (addin);
  g_autoptr (GError) err = NULL;

  self->id_menu = dzl_menu_manager_add_resource (dzl_application_get_menu_manager (DZL_APPLICATION (g_application_get_default ())),
                                                 "/plugins/pwn-german-dictionary-2010/gtk/menus.ui", &err);
  if (err)
    g_critical ("%s", err->message);

  GyDictManager *manager = gy_window_get_dict_manager (win);

  GyDict *dict = GY_DICT (pwn_german_dictionary2010_new (ID_GERMAN_POLISH));
  gy_dict_manager_insert_dict (manager, dict, ID_GERMAN_POLISH);
  g_object_unref (dict);

  dict = GY_DICT (pwn_german_dictionary2010_new (ID_POLISH_GERMAN));
  gy_dict_manager_insert_dict (manager, dict, ID_POLISH_GERMAN);
  g_object_unref (dict);

}

static void
pwn_german_dictionary2010_window_unload (GyWindowAddin *addin,
                                          GyWindow      *win)
{
  PwnGermanDictionary2010Window *self = PWN_GERMAN_DICTIONARY2010_WINDOW (addin);

  dzl_menu_manager_remove (dzl_application_get_menu_manager (DZL_APPLICATION (g_application_get_default ())),
                           self->id_menu);
  self->id_menu = 0;

  GyDictManager *manager = gy_window_get_dict_manager (win);
  gy_dict_manager_remove_dict (manager, ID_GERMAN_POLISH);
  gy_dict_manager_remove_dict (manager, ID_POLISH_GERMAN);

}

static void
pwn_german_dictionary2010_window_iface_init (GyWindowAddinInterface *iface)
{
  iface->load = pwn_german_dictionary2010_window_load;
  iface->unload = pwn_german_dictionary2010_window_unload;
}
