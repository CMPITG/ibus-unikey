#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <libintl.h>

#include <ibus.h>
#include "ukengine.h"
#include "utils.h"
#include "engine_const.h"

#define _(str) gettext(str)

#define IU_DESC _("Vietnamese Input Method Engine for IBus using Unikey Engine\n\
Usage:\n\
  - Choose input method, output charset, options in language bar.\n\
  - There are 4 input methods: Telex, Vni, STelex (simple telex) \
and STelex2 (which same as STelex, the difference is it use w as ư).\n\
  - And 7 output charsets: Unicode (UTF-8), TCVN3, VNI Win, VIQR, CString, NCR Decimal and NCR Hex.\n\
  - Use <Shift>+<Space> or <Shift>+<Shift> to restore keystrokes.\n\
  - Use <Control> to commit a word.\
")

using namespace std;

static unsigned char endingLetters[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'b', 'c', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n',
    'p', 'q', 'r', 's', 't', 'v', 'x', 'z',
    'B', 'C', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N',
    'P', 'Q', 'R', 'S', 'T', 'V', 'X', 'Z'
};

static unsigned char WordBreakSyms[] = {
    ',', ';', ':', '.', '\"', '\'', '!', '?', ' ',
    '<', '>', '=', '+', '-', '*', '/', '\\',
    '_', '~', '`', '@', '#', '$', '%', '^', '&', '(', ')', '{', '}', '[', ']',
    '|'
};

IBusComponent* ibus_unikey_get_component () {
    IBusComponent* component;
    IBusEngineDesc* engine;

    component = ibus_component_new("org.freedesktop.IBus.Unikey",
                                   "Unikey",
                                   PACKAGE_VERSION,
                                   "GPLv3",
                                   "Lê Quốc Tuấn <mr.lequoctuan@gmail.com>",
                                   PACKAGE_BUGREPORT,
                                   "",
                                   PACKAGE_NAME);

#if IBUS_CHECK_VERSION(1,3,99)
    engine = ibus_engine_desc_new_varargs ("name",        "Unikey",
                                           "longname",    "Unikey",
                                           "description", IU_DESC,
                                           "language",    "vi",
                                           "license",     "GPLv3",
                                           "author",      "Lê Quốc Tuấn <mr.lequoctuan@gmail.com>",
                                           "icon",        PKGDATADIR"/icons/ibus-unikey.png",
                                           "layout",      "us",
                                           "rank",        99,
                                           NULL);
#else
    engine = ibus_engine_desc_new  ("Unikey",
                                    "Unikey",
                                    IU_DESC,
                                    "vi",
                                    "GPLv3",
                                    "Lê Quốc Tuấn <mr.lequoctuan@gmail.com>",
                                    PKGDATADIR"/icons/ibus-unikey.png",
                                    "us");
    engine->rank = 99;
#endif

    ibus_component_add_engine (component, engine);

    return component;
}

// code from x-unikey, for convert charset that not is XUtf-8
int latinToUtf (unsigned char* dst, unsigned char* src, int inSize,
                int* pOutSize) {
    int i;
    int outLeft;
    unsigned char ch;

    outLeft = *pOutSize;

    for (i=0; i<inSize; i++) {
        ch = *src++;
        if (ch < 0x80) {
            outLeft -= 1;
            if (outLeft >= 0)
                *dst++ = ch;
        } else {
            outLeft -= 2;
            if (outLeft >= 0) {
                *dst++ = (0xC0 | ch >> 6);
                *dst++ = (0x80 | (ch & 0x3F));
            }
        }
    }

    *pOutSize = outLeft;
    return (outLeft >= 0);
}

gboolean ibus_unikey_config_get_string (IBusConfig* config,
                                        const gchar* section,
                                        const gchar* name,
                                        gchar** result) {
#if IBUS_CHECK_VERSION(1,3,99)
    GVariant *value = NULL;
    value = ibus_config_get_value(config, section, name);
    if (value) {
        *result = g_strdup((gchar*)g_variant_get_string(value, NULL));
        g_variant_unref(value);
        return true;
    }
    return false;
#else
    GValue value = {0};
    if (ibus_config_get_value(config, section, name, &value)) {
        *result = g_strdup((gchar*)g_value_get_string(&value));
        g_value_unset(&value);
        return true;
    }
    return false;
#endif
}

void ibus_unikey_config_set_string (IBusConfig* config,
                                    const gchar* section,
                                    const gchar* name,
                                    const gchar* value) {
#if IBUS_CHECK_VERSION(1,3,99)
    ibus_config_set_value(config, section, name, g_variant_new_string(value));
#else
    GValue v = {0};
    g_value_init(&v, G_TYPE_STRING);
    g_value_set_string(&v, value);
    ibus_config_set_value(config, section, name, &v);
#endif
}

gboolean ibus_unikey_config_get_boolean(IBusConfig* config,
                                        const gchar* section,
                                        const gchar* name,
                                        gboolean* result) {
#if IBUS_CHECK_VERSION(1,3,99)
    GVariant *value = NULL;
    value = ibus_config_get_value(config, section, name);
    if (value) {
        *result = g_variant_get_boolean(value);
        g_variant_unref(value);
        return true;
    }
    return false;
#else
    GValue value = {0};
    if (ibus_config_get_value(config, section, name, &value)) {
        *result = g_value_get_boolean(&value);
        g_value_unset(&value);
        return true;
    }
    return false;
#endif
}

void ibus_unikey_config_set_boolean (IBusConfig* config,
                                     const gchar* section,
                                     const gchar* name,
                                     gboolean value) {
#if IBUS_CHECK_VERSION(1,3,99)
    ibus_config_set_value(config, section, name, g_variant_new_boolean(value));
#else
    GValue v = {0};
    g_value_init(&v, G_TYPE_BOOLEAN);
    g_value_set_boolean(&v, value);
    ibus_config_set_value(config, section, name, &v);
#endif
}

//
// cmpitg
//
// Helpers

// Reference:
// http://ibus.googlecode.com/svn/docs/ibus/ibus-ibustypes.html
string modifierNames (guint modifiers) {
    string names = " ";

    if ((modifiers & IBUS_SHIFT_MASK) == IBUS_SHIFT_MASK)
        names.append ("SHIFT ");
    if ((modifiers & IBUS_LOCK_MASK) == IBUS_LOCK_MASK)
        names.append ("LOCK ");
    if ((modifiers & IBUS_CONTROL_MASK) == IBUS_CONTROL_MASK)
        names.append ("CONTROL ");
    if ((modifiers & IBUS_MOD1_MASK) == IBUS_MOD1_MASK)
        names.append ("MOD1 ");
    if ((modifiers & IBUS_MOD2_MASK) == IBUS_MOD2_MASK)
        names.append ("MOD2 ");
    if ((modifiers & IBUS_MOD3_MASK) == IBUS_MOD3_MASK)
        names.append ("MOD3 ");
    if ((modifiers & IBUS_MOD4_MASK) == IBUS_MOD4_MASK)
        names.append ("MOD4 ");
    if ((modifiers & IBUS_MOD5_MASK) == IBUS_MOD5_MASK)
        names.append ("MOD5 ");
    if ((modifiers & IBUS_BUTTON1_MASK) == IBUS_BUTTON1_MASK)
        names.append ("BUTTON1 ");
    if ((modifiers & IBUS_BUTTON2_MASK) == IBUS_BUTTON2_MASK)
        names.append ("BUTTON2 ");
    if ((modifiers & IBUS_BUTTON3_MASK) == IBUS_BUTTON3_MASK)
        names.append ("BUTTON3 ");
    if ((modifiers & IBUS_BUTTON4_MASK) == IBUS_BUTTON4_MASK)
        names.append ("BUTTON4 ");
    if ((modifiers & IBUS_BUTTON5_MASK) == IBUS_BUTTON5_MASK)
        names.append ("BUTTON5 ");
    if ((modifiers & IBUS_SUPER_MASK) == IBUS_SUPER_MASK)
        names.append ("SUPER ");
    if ((modifiers & IBUS_HYPER_MASK) == IBUS_HYPER_MASK)
        names.append ("HYPER ");
    if ((modifiers & IBUS_META_MASK) == IBUS_META_MASK)
        names.append ("META ");
    if ((modifiers & IBUS_RELEASE_MASK) == IBUS_RELEASE_MASK)
        names.append ("RELEASE ");
    if ((modifiers & IBUS_HANDLED_MASK) == IBUS_HANDLED_MASK)
        names.append ("HANDLED_BY_IBUS ");
    if ((modifiers & IBUS_FORWARD_MASK) == IBUS_FORWARD_MASK)
        names.append ("FORWARD_BY_IBUS ");

    return names;
}

bool isKeyRelease (guint modifiers) {
    return modifierNames (modifiers).find ("RELEASE") != string::npos;
}

bool wordIsTerminated (guint keyval, guint modifiers) {
    return
        modifiers & IBUS_CONTROL_MASK
        || modifiers & IBUS_MOD1_MASK // alternate mask
        || keyval == IBUS_Control_L
        || keyval == IBUS_Control_R
        || keyval == IBUS_Tab
        || keyval == IBUS_Return
        || keyval == IBUS_Delete
        || keyval == IBUS_KP_Enter
        || (keyval >= IBUS_Home && keyval <= IBUS_Insert)
        || (keyval >= IBUS_KP_Home && keyval <= IBUS_KP_Delete);
}

bool isShiftPressed (guint keyval, guint modifiers) {
    return
        (keyval >= IBUS_Caps_Lock && keyval <= IBUS_Hyper_R)
        || (!(modifiers & IBUS_SHIFT_MASK)
            && (keyval == IBUS_Shift_L || keyval == IBUS_Shift_R));
}

bool isBackspacePressed (guint keyval) {
    return keyval == IBUS_BackSpace;
}

bool nothingToDelete (guint nBackspaces, string str) {
    return nBackspaces == 0 || str.empty ();
}

bool isOneCharToDelete (int nChars, int nBackspaces) {
    return nChars <= nBackspaces;
}

bool isNumpadKey (guint keyval) {
    return keyval >= IBUS_KP_Multiply && keyval <= IBUS_KP_9;
}

bool isCharacter (guint keyval) {
    return (keyval >= IBUS_space && keyval <= IBUS_asciitilde)
        || keyval == IBUS_Shift_L || keyval == IBUS_Shift_R;
}

bool isEndingLetter (guint keyval) {
    for (int i = 0; i < sizeof (endingLetters); i++)
        if (keyval == endingLetters[i])
            return true;
    return false;
}

bool isWordBreakSym (guint ch) {
    for (guint i = 0; i < sizeof (WordBreakSyms); i++)
        if (WordBreakSyms[i] == ch)
            return true;
    return false;
}
