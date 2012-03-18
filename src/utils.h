#if !defined (__UTILS_H)
#define __UTILS_H

#include "ukengine.h"
#include "engine_const.h"
#include <string>
#include <ibustypes.h>

using namespace std;

IBusComponent* ibus_unikey_get_component();
int latinToUtf(unsigned char* dst, unsigned char* src, int inSize, int* pOutSize);

#define get_macro_file() (g_build_filename(g_getenv("HOME"), UNIKEY_MACRO_FILE, NULL))

gboolean ibus_unikey_config_get_string (IBusConfig* config,
                                        const gchar* section,
                                        const gchar* name,
                                        gchar** result);

void ibus_unikey_config_set_string (IBusConfig* config,
                                    const gchar* section,
                                    const gchar* name,
                                    const gchar* value);

gboolean ibus_unikey_config_get_boolean (IBusConfig* config,
                                         const gchar* section,
                                         const gchar* name,
                                         gboolean* result);

void ibus_unikey_config_set_boolean (IBusConfig* config,
                                     const gchar* section,
                                     const gchar* name,
                                     gboolean value);

/* Getting the human-readable names from modifiers */
string modifierNames (guint modifiers);

/* Determine whether the key event is a key RELEASE */
bool isKeyRelease (guint modifiers);

bool wordIsTerminated (guint keyval, guint mod);

bool isShiftPressed (guint keyval, guint modifiers);

bool isBackspacePressed (guint keyval);

bool nothingToDelete (guint nBackspaces, string str);

bool isOneCharToDelete (int nChars, int nBackspaces);

bool isNumpadKey (guint keyval);

bool isCharacter (guint keyval);

bool isEndingLetter (guint keyval);

bool isWordBreakSym (guint ch);

#endif
