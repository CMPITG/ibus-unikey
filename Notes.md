* After building, you will have some libs and excutables, containing
  `ibus-engine-unikey`, run this file to have the engine:

    ibus-engine-unikey --ibus

* Pay attention to *src/engine.cpp*.  The engine is initialized with:

    void ibus_unikey_init (IBusBus* bus)

* Attention: Backspace with changing diacritical marks: "hoạt" + Backspace

* Whenever you work with preeditstr, use std::string's functions.  Whenever
  you work with ibus text, use ibus_text_* functions.

* Goal:

  - Every button is a commit button
  - Save the content of `unikey->preeditstr` to some other place
  - Separating the *commit* action and the *clear* action
  - When would `preeditstr` is reset?  When is **word** is committed =>
    **Inside _reset ()**
  - When would `preeditstr` is committed but not reset?  Everytime a key is
    pressed.
  - Preventing `update_preeditstr ()` from calling!

* Subgoal:

  - Find every point where `unikey->preeditstr` is modified.

## engine.cpp

    void ibus_unikey_engine_reset         -> reset editing
    std::String unikey->preeditstr        -> information about preedit string

    void ibus_unikey_engine_commit_string -> commit string

    static gboolean ibus_unikey_engine_process_key_event_preedit ()

    thread_run_setup                      -> command line for the engine

    unikey->auto_commit -> whether or not the string is committed right away

Preedit string is

* BIG: when is it modified?
* Everytime backspace is pressed, delete a character

* Initialized in `ibus_unikey_engine_init ()`
* Destroyed in `ibus_unikey_engine_destroy ()`
* Hide with `ibus_engine_hide_preedit_text ()`
* Committed with `ibus_unikey_engine_commit_string ()`
* Cleared and committed in `ibus_unikey_engine_reset ()`
  - To commit it:
    + Hide it
    + Commit it
    + Clear it

* `ibus_unikey_engine_commit_string ()` is used to commit a Unikey string.

* `ibus_unikey_engine_erase_chars ()`?

  It is called whenever a character needs to be deleted or replaced, inside
  processing function.  E.g. when you type "đấy", you will get this log:

* `ibus_engine_update_preedit_text ()` is used to update *uncommitted text*
* `ibus_engine_hide_preedit_text ()` is used to hide text and is called when a
  word is done!

* `ibus_unikey_engine_update_preedit_string ()`?

  Update the text on the current input buffer:
  - Get the text from the preedit string
  - Underline it
  - Display it

* When is preedit string committed?

  - Inside `_reset ()` -> when is `_reset ()` called?
  - Receiving Backspaces

* `UnikeyPutChar ()`?

  Put a char to the screen?

* `UnikeyResetBuf ()`?

## To test

* Delete last build branch
* Add new build branch
* Gen & make
* Run with: `src/ibus-engine-unikey --ibus`
* Clean up

    git checkout master
    git branch -D build
    git branch build
    git checkout build
    ./autogen.sh
    make
    src/ibus-engine-unikey --ibus
