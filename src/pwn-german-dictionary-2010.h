/* pwn-german-dictionary-2007.h
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

#pragma once

#include <gydict.h>

G_BEGIN_DECLS

#define PWN_TYPE_GERMAN_DICTIONARY2010 (pwn_german_dictionary2010_get_type())

G_DECLARE_FINAL_TYPE (PwnGermanDictionary2010, pwn_german_dictionary2010, PWN, GERMAN_DICTIONARY2010, GyDict)

PwnGermanDictionary2010 *pwn_german_dictionary2010_new (const gchar *id);

G_END_DECLS
