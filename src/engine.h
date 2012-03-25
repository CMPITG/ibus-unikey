#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <ibus.h>

#define IBUS_TYPE_UNIKEY_ENGINE (ibus_unikey_engine_get_type())

const gchar*          Unikey_IMNames[]   = { "Telex", "Vni", "STelex",
                                             "STelex2" };
const UkInputMethod   Unikey_IM[]        = { UkTelex, UkVni, UkSimpleTelex,
                                             UkSimpleTelex2 };
const unsigned int    NUM_INPUTMETHOD    =
    sizeof (Unikey_IM) / sizeof (Unikey_IM[0]);

const gchar*          Unikey_OCNames[]   = { "Unicode",
                                             "TCVN3",
                                             "VNI Win",
                                             "VIQR",
                                             "BK HCM 2",
                                             "CString",
                                             "NCR Decimal",
                                             "NCR Hex" };

const unsigned int    Unikey_OC[]        = { CONV_CHARSET_XUTF8,
                                             CONV_CHARSET_TCVN3,
                                             CONV_CHARSET_VNIWIN,
                                             CONV_CHARSET_VIQR,
                                             CONV_CHARSET_BKHCM2,
                                             CONV_CHARSET_UNI_CSTRING,
                                             CONV_CHARSET_UNIREF,
                                             CONV_CHARSET_UNIREF_HEX };
const unsigned int    NUM_OUTPUTCHARSET  =
    sizeof (Unikey_OC) / sizeof (Unikey_OC[0]);

/* Text and key processing functions */
static void ibus_unikey_engine_commit_string (IBusEngine *engine,
                                              const gchar *string);
static void ibus_unikey_engine_update_preedit_string2 (IBusEngine *engine,
                                                       const gchar *string,
                                                       gboolean visible);
static void ibus_unikey_engine_erase_chars (IBusEngine *engine, int num_chars);
static gboolean ibus_unikey_engine_process_key_event (IBusEngine* engine,
                                                      guint keyval,
                                                      guint keycode,
                                                      guint modifiers);
static gboolean ibus_unikey_engine_process_key_event_preedit (IBusEngine* engine,
                                                              guint keyval,
                                                              guint keycode,
                                                              guint modifiers);
static void ibus_unikey_engine_delete_a_char (IBusEngine *engine);
void adjustTonePosition ();
void processUkEngineData (IBusEngine *engine, guint keyval);
void processBackspace (IBusEngine *engine);

/* Helpers */
static string getPreeditStr ();
static bool isMacroEnabled ();
static bool isUsingTelex ();
bool isCombinationOfShift (guint keyval, guint modifiers);
static guint getIbusTextLength (string str);
static void addMenu (IBusPropList *engineProp,
                     const gchar *key,
                     IBusPropType type,
                     IBusText *label,
                     const gchar *icon,
                     IBusText *tooltip,
                     gboolean sensitive,
                     gboolean visible,
                     IBusPropState state,
                     IBusPropList *prop_list);

/* Engine setup functions */
void ibus_unikey_init (IBusBus* bus);
void ibus_unikey_exit ();
static void ibus_unikey_engine_class_init (IBusUnikeyEngineClass* klass);
static void ibus_unikey_engine_init (IBusUnikeyEngine* unikey);
static GObject* ibus_unikey_engine_constructor (GType type,
                                                guint n_construct_params,
                                                GObjectConstructParam* construct_params);
static void ibus_unikey_engine_destroy (IBusUnikeyEngine* unikey);
static void ibus_unikey_engine_focus_in (IBusEngine* engine);
static void ibus_unikey_engine_focus_out (IBusEngine* engine);
static void ibus_unikey_engine_reset (IBusEngine* engine);
static void ibus_unikey_engine_enable (IBusEngine* engine);
static void ibus_unikey_engine_disable (IBusEngine* engine);
static void ibus_unikey_engine_property_activate (IBusEngine* engine,
                                                  const gchar* prop_name,
                                                  guint prop_state);
static void ibus_unikey_engine_create_property_list (IBusUnikeyEngine* unikey);
static void* thread_mouse_capture (void* data);
static void* thread_run_setup (void* data);
GType ibus_unikey_engine_get_type (void);

#endif
