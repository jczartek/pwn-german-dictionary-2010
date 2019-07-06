/* pwn-german-dictionary-2010-prefs.c
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
#include "pwn-german-dictionary-2010-prefs.h"

static void pwn_german_dictionary2010_prefs_iface_init (GyPrefsViewAddinInterface *iface);

struct _PwnGermanDictionary2010Prefs
{
  GObject parent_instance;
  guint ids[2];
};

G_DEFINE_TYPE_WITH_CODE (PwnGermanDictionary2010Prefs, pwn_german_dictionary2010_prefs, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GY_TYPE_PREFS_VIEW_ADDIN, pwn_german_dictionary2010_prefs_iface_init))


static void
pwn_german_dictionary2010_prefs_class_init (PwnGermanDictionary2010PrefsClass *klass)
{
}

static void
pwn_german_dictionary2010_prefs_init (PwnGermanDictionary2010Prefs *self)
{
}

static void
pwn_german_dictionary2010_prefs_load (GyPrefsViewAddin *self,
                                       GyPrefsView      *prefs)
{
  g_autofree gchar *path_angpol = NULL;
  g_autofree gchar *path_polang = NULL;
  GtkWidget *widget = NULL;
  GtkSizeGroup *path_group = NULL;

  path_angpol = g_strdup_printf ("/org/gtk/gydict/plugin/%s/", "pwn-german-polish-dictionary-2010");
  path_polang = g_strdup_printf ("/org/gtk/gydict/plugin/%s/", "pwn-polish-german-dictionary-2010");

  PWN_GERMAN_DICTIONARY2010_PREFS (self)->ids[0] =
    dzl_preferences_add_file_chooser (DZL_PREFERENCES (prefs), "dictionaries", "paths", "org.gtk.gydict.path",
                                      "path", path_angpol, "PWN German-Polish 2010", "Select niempol.win",
                                      GTK_FILE_CHOOSER_ACTION_OPEN, NULL, 0);
  path_group = gy_prefs_view_get_size_group (GY_PREFS_VIEW (prefs), "paths");
  widget = dzl_gtk_widget_find_child_typed (dzl_preferences_get_widget (DZL_PREFERENCES (prefs),
                                                                        PWN_GERMAN_DICTIONARY2010_PREFS (self)->ids[0]),
                                            GTK_TYPE_FILE_CHOOSER_BUTTON);
  if (widget) gtk_size_group_add_widget (path_group, widget);


  PWN_GERMAN_DICTIONARY2010_PREFS (self)->ids[1] =
    dzl_preferences_add_file_chooser (DZL_PREFERENCES (prefs), "dictionaries", "paths", "org.gtk.gydict.path",
                                      "path", path_polang, "PWN Polish-German 2010", "Select polniem.win",
                                      GTK_FILE_CHOOSER_ACTION_OPEN, NULL, 0);
  path_group = gy_prefs_view_get_size_group (GY_PREFS_VIEW (prefs), "paths");
  widget = dzl_gtk_widget_find_child_typed (dzl_preferences_get_widget (DZL_PREFERENCES (prefs),
                                                                        PWN_GERMAN_DICTIONARY2010_PREFS (self)->ids[1]),
                                            GTK_TYPE_FILE_CHOOSER_BUTTON);
  if (widget) gtk_size_group_add_widget (path_group, widget);

  gtk_widget_queue_draw (GTK_WIDGET (prefs));
}


static void
pwn_german_dictionary2010_prefs_unload (GyPrefsViewAddin *self,
                                         GyPrefsView      *prefs)
{
  dzl_preferences_remove_id (DZL_PREFERENCES (prefs),
                             PWN_GERMAN_DICTIONARY2010_PREFS (self)->ids[0]);
  dzl_preferences_remove_id (DZL_PREFERENCES (prefs),
                             PWN_GERMAN_DICTIONARY2010_PREFS (self)->ids[1]);
  gtk_widget_queue_draw (GTK_WIDGET (prefs));
}

static void
pwn_german_dictionary2010_prefs_iface_init (GyPrefsViewAddinInterface *iface)
{
  iface->load = pwn_german_dictionary2010_prefs_load;
  iface->unload = pwn_german_dictionary2010_prefs_unload;
}
