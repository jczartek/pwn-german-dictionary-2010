/* pwn-german-dictionary-2010-private.h
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

#include <glib.h>

#pragma once

static const gchar *
G_GNUC_UNUSED get_superscript (gint n)
{
  static const gchar *superscript[] = { "⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹" };

  g_return_val_if_fail ((n >= 0 && n <= 9), NULL);

  return superscript[n];
}

static struct {
  const gchar *in;
  const gchar *out;
} entitles[] =
{
  {"&IPA502", "ˌ"},
  {"&inodot", "ɪ"},
  {"&##952", "θ"},
  {"&##8747", "ʃ"},
  {"&eng", "ŋ"},
  {"&square", "…"},
  {"&squareb", "•"},
  {"&pause", "―"},
  {"&##163", "£"},
  {"&dots", "…"},
  {"&rArr", "→"},
  {"&IPA118", "ɲ"},
  {"&##949", "ε"},
  {"&IPA413", " ̟"},
  {"&IPA424", " ̃"},
  {"&IPA505", " ̆"},
  {"&IPA135", "ʒ"},
  {"&IPA305", "ɑ"},
  {"&IPA306", "ɔ"},
  {"&IPA313", "ɒ"},
  {"&IPAa313", "ɒ"},
  {"&IPA314", "ʌ"},
  {"&IPA321", "ʊ"},
  {"&IPA326", "ɜ"},
  {"&IPA503", "ː"},
  {"&IPA146", "h"},
  {"&IPA170", "w"},
  {"&IPA128", "f"},
  {"&IPA325", "æ"},
  {"&IPA301", "i"},
  {"&IPA155", "l"},
  {"&IPA319", "ɪ"},
  {"&IPA114", "m"},
  {"&IPA134", "ʃ"},
  {"&IPA103", "t"},
  {"&IPA140", "x"},
  {"&IPA119", "ŋ"},
  {"&IPA131", "ð"},
  {"&IPA130", "θ"},
  {"&schwa.x", "ə"},
  {"&epsi", "ε"},
  {"&ldquor", "“"},
  {"&marker", "►"},
  {"&IPA101", "p"},
  {"&IPA102", "b"},
  {"&IPA104", "d"},
  {"&IPA109", "k"},
  {"&IPA110", "g"},
  {"&IPA116", "n"},
  {"&IPA122", "r"},
  {"&IPA129", "v"},
  {"&IPA132", "s"},
  {"&IPA133", "z"},
  {"&IPA153", "j"},
  {"&IPA182", "ɕ"},
  {"&IPA183", "ʑ"},
  {"&IPA302", "e"},
  {"&IPA304", "a"},
  {"&IPA307", "o"},
  {"&IPA308", "u"},
  {"&IPA309", "y"},
  {"&IPA322", "ə"},
  {"&IPA426", "ˡ"},
  {"&IPA491", "ǫ"},
  {"&IPA501", "ˈ"},
  {"&comma", ","},
  {"&squ", "•"},
  {"&ncaron", "ň"},
  {"&atildedotbl.x", "ã"},
  {"&rdquor", "”"},
  {"&verbar", "|"},
  {"&IPA405", " ̤"},
  {"&idot", "i"},
  {"&equals", "="},
  {"&lsqb", "["},
  {"&rsqb", "]"},
  {"&s224", "◊"},
  {"&karop", "◊"},
  {"&s225", "<"},
  {"&s241", ">"},
  {"&#!0,127", "▫"},
  {"&##37", "%"},
  {"&##9553", "║"},
  {"&oboczn", "║"},
  {"&##1100", "ь"},
  {"&##1098", "ъ"},
  {"&s172", "←"},
  {"&ytilde", "ỹ"},
  {"&estrok", "ě"},
  {"&ndotbl", "ṇ"},
  {"&yogh", "ȝ"},
  {"&ismutne", "i"},
  {"&usmutne", "u"},
  {"&middot_s", "⋅"},
  {"&##133", "…"},
  {"&semi", ";"},
  {"&zacute", "ź"},
  {"&dollar", "$"},
  {"&frac13", "⅓"},
  {"&frac15", "⅕"},
  {"&ldotbl.x", "ḷ"},
  {"&mdotbl.x", "ṃ"},
  {"&commat", "@"},
  {"&Lstrok", "Ł"},
  {"&Aogon", "Ą"},
  {"&ybreve.x", "Ў"},
  {"&IPA177", "ǀ"},
  {"&IPA324", "ɐ"},
  {"&IPA432", " ̯"},
  {"&IPA432i", "i̯"},
  {"&IPA303", "ɛ"},
  {"&IPA138", "ç"},
  {"&IPA320", "ʏ"},
  {"&IPA214", "ʤ"},
  {"&epsilontilde", "ε"},
  {"&auluk", "au"},
  {"&ailuk", "ai"},
  {"&apos", "'"},
  {"&brvbar", "|"},
  {"&reg", "®"},
  {"&rsquo", "’"},
  {"&lsquo", "‘"},
  {"&ccedil", "ç"},
  {"&eacute", "é"},
  {"&egrave", "è"},
  {"&amp", "&"},
  {"&ecirc", "ê"},
  {"&agrave", "à"},
  {"&iuml", "ï"},
  {"&ocirc", "ô"},
  {"&icirc", "î"},
  {"&para", "▹"},
  {"&mdash", "—"},
  {"&rdquo", "”"},
  {"&ldquo", "“"},
  {"&ndash", "–"},
  {"&asymp", "\u224d"},
  {"&ap", "≈"},
  {"&rarr", "↪"},
  {"&pound", "£"},
  {"&aelig", "æ"},
  {"&auml", "ä"},
  {"&dash", "-"},
  {"&uuml", "ü"},
  {"&ouml", "ö"},
  {"&szlig", "ß"},
  {"&Auml", "Ä"},
  {"&Ouml", "Ö"},
  {"&Uuml", "Ü"},
  {"&acirc", "â"},
  {"&ndotbl.x", "n̩"},
  {NULL, NULL}
};

static const gchar*
G_GNUC_UNUSED get_entitle (const gchar *in)
{
  for(guint i = 0; entitles[i].in != NULL; i++)
    {
      if (strncmp (in, entitles[i].in, strlen(entitles[i].in)) == 0)
        return entitles[i].out;
    }
  return NULL;
}

static gchar*
G_GNUC_UNUSED convert_to_roman (gint arabic_num)
{
  GString *roman_num = g_string_new (NULL);
  gint num[] = {1,4,5,9,10,40,50,90,100,400,500,900,1000};
  gchar* sym[] = {"I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M"};
  gint i = 12;

  while(arabic_num > 0)
    {
      gint div = arabic_num / num[i];
      arabic_num = arabic_num % num[i];

      while(div--)
      {
        g_string_append (roman_num, sym[i]);
      }
      i--;
    }

  return g_string_free (roman_num, FALSE);
} 
