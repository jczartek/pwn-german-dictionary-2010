/* pwn-english-dictionary-2007.c
 *
 * Copyright 2019 Jakub Czartek
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
 */

#include <gydict.h>
#include <libpeas/peas.h>
#include "pwn-german-dictionary-2010-prefs.h"
#include "pwn-german-dictionary-2010-window.h"

void
peas_register_types (PeasObjectModule *module)
{
  peas_object_module_register_extension_type (module,
                                              GY_TYPE_WINDOW_ADDIN,
                                              PWN_TYPE_GERMAN_DICTIONARY2010_WINDOW);

  peas_object_module_register_extension_type (module,
                                              GY_TYPE_PREFS_VIEW_ADDIN,
                                              PWN_TYPE_ENGLISH_DICTIONARY2010_PREFS);
}
