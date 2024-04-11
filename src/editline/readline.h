/*    $NetBSD: readline.h,v 1.55 2023/04/25 17:51:32 christos Exp $    */

/*-
 * Copyright (c) 1997 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jaromir Dolecek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _READLINE_H_
#define _READLINE_H_

#if defined(_WIN32)
#ifdef LIBEDIT_BUILDING
#define LIBEDIT_DLL_API __declspec(dllexport) __cdecl
#else
 // we are building as static
 // https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-warning-lnk4217?view=msvc-170
#define LIBEDIT_DLL_API
//#define LIBEDIT_DLL_API __declspec(dllimport) __cdecl
#endif
#else
#define LIBEDIT_DLL_API
#endif

#include <sys/types.h>
#include <stdio.h>

/* list of readline stuff supported by editline library's readline wrapper */

/* typedefs */
typedef int       rl_linebuf_func_t(const char *, int);
typedef void      rl_voidfunc_t(void);
typedef void      rl_vintfunc_t(int);
typedef void      rl_vcpfunc_t(char *);
typedef char    **rl_completion_func_t(const char *, int, int);
typedef char     *rl_compentry_func_t(const char *, int);
typedef void      rl_compdisp_func_t(char **, int, int);
typedef int       rl_command_func_t(int, int);
typedef int       rl_hook_func_t(void);
typedef int       rl_icppfunc_t(char **);

/* only supports length */
typedef struct {
    int length;
} HISTORY_STATE;

typedef void *histdata_t;

typedef struct _hist_entry {
    const char    *line;
    histdata_t     data;
} HIST_ENTRY;

typedef struct _keymap_entry {
    char type;
#define ISFUNC    0
#define ISKMAP    1
#define ISMACR    2
    rl_linebuf_func_t *function;
} KEYMAP_ENTRY;

#define KEYMAP_SIZE    256

typedef KEYMAP_ENTRY KEYMAP_ENTRY_ARRAY[KEYMAP_SIZE];
typedef KEYMAP_ENTRY *Keymap;

#define control_character_threshold    0x20
#define control_character_bit        0x40

#ifndef CTRL
#if !defined(_WIN32)
#include <sys/ioctl.h>
#if !defined(__sun) && !defined(__hpux) && !defined(_AIX)
#include <sys/ttydefaults.h>
#endif
#endif
#ifndef CTRL
#define CTRL(c)        ((c) & 037)
#endif
#endif
#ifndef UNCTRL
#define UNCTRL(c)    (((c) - 'a' + 'A')|control_character_bit)
#endif

#define RUBOUT        0x7f
#define ABORT_CHAR    CTRL('G')
#define RL_READLINE_VERSION    0x0402
#define RL_PROMPT_START_IGNORE    '\1'
#define RL_PROMPT_END_IGNORE    '\2'

#define RL_STATE_NONE        0x000000
#define RL_STATE_DONE        0x000001

#define RL_SETSTATE(x)      (rl_readline_state |= ((unsigned long) x))
#define RL_UNSETSTATE(x)    (rl_readline_state &= ~((unsigned long) x))
#define RL_ISSTATE(x)       (rl_readline_state & ((unsigned long) x))

/* global variables used by readline enabled applications */
#ifdef __cplusplus
extern "C" {
#endif
extern const char           *rl_library_version;
extern int                   rl_readline_version;
extern const char           *rl_readline_name;
extern FILE                 *rl_instream;
extern FILE                 *rl_outstream;
extern char                 *rl_line_buffer;
extern int                   rl_point, rl_end;
extern const char           *rl_basic_quote_characters;
extern const char           *rl_basic_word_break_characters;
extern char                 *rl_completer_word_break_characters;
extern const char           *rl_completer_quote_characters;
extern rl_compentry_func_t  *rl_completion_entry_function;
extern char               *(*rl_completion_word_break_hook)(void);
extern rl_completion_func_t *rl_attempted_completion_function;
extern int                   rl_attempted_completion_over;
extern int                   rl_completion_type;
extern int                   rl_completion_query_items;
extern const char           *rl_special_prefixes;
extern int                   rl_completion_append_character;
extern int                   rl_inhibit_completion;
extern rl_hook_func_t       *rl_pre_input_hook;
extern rl_hook_func_t       *rl_startup_hook;
extern char                 *rl_terminal_name;
extern int                   rl_already_prompted;
extern char                 *rl_prompt;
extern int                   rl_done;
extern rl_vcpfunc_t         *rl_linefunc;
extern rl_hook_func_t       *rl_startup1_hook;
extern char                 *rl_prompt_saved;
extern int                   history_base, history_length;
extern int                   history_offset;
extern char                  history_expansion_char;
extern char                  history_subst_char;
extern char                 *history_no_expand_chars;
extern rl_linebuf_func_t    *history_inhibit_expansion_function;
extern int                   max_input_history;

/*
 * The following is not implemented
 */
extern unsigned long         rl_readline_state;
extern int                   rl_catch_signals;
extern int                   rl_catch_sigwinch;
extern KEYMAP_ENTRY_ARRAY    emacs_standard_keymap, emacs_meta_keymap, emacs_ctlx_keymap;
extern int                   rl_filename_completion_desired;
extern int                   rl_ignore_completion_duplicates;
extern int                 (*rl_getc_function)(FILE *);
extern rl_voidfunc_t        *rl_redisplay_function;
extern rl_compdisp_func_t   *rl_completion_display_matches_hook;
extern rl_vintfunc_t        *rl_prep_term_function;
extern rl_voidfunc_t        *rl_deprep_term_function;
extern rl_hook_func_t       *rl_event_hook;
extern int                   readline_echoing_p;
extern int                  _rl_print_completions_horizontally;
extern int                  _rl_complete_mark_directories;
extern rl_icppfunc_t        *rl_directory_completion_hook;
extern int                   rl_completion_suppress_append;
extern int                   rl_sort_completion_matches;
extern int                  _rl_completion_prefix_display_length;
extern int                  _rl_echoing_p;
extern int                   history_max_entries;
extern char                 *rl_display_prompt;
extern int                   rl_erase_empty_line;

/* supported functions */
LIBEDIT_DLL_API char         *readline(const char *);
LIBEDIT_DLL_API int           rl_initialize(void);

LIBEDIT_DLL_API void          using_history(void);
LIBEDIT_DLL_API int           add_history(const char *);
LIBEDIT_DLL_API void          clear_history(void);
LIBEDIT_DLL_API int           append_history(int, const char *);
LIBEDIT_DLL_API void          stifle_history(int);
LIBEDIT_DLL_API int           unstifle_history(void);
LIBEDIT_DLL_API int           history_is_stifled(void);
LIBEDIT_DLL_API int           where_history(void);
LIBEDIT_DLL_API HIST_ENTRY   *current_history(void);
LIBEDIT_DLL_API HIST_ENTRY   *history_get(int);
LIBEDIT_DLL_API HIST_ENTRY   *remove_history(int);
LIBEDIT_DLL_API HIST_ENTRY   *replace_history_entry(int, const char *, histdata_t);
LIBEDIT_DLL_API int           history_total_bytes(void);
LIBEDIT_DLL_API int           history_set_pos(int);
LIBEDIT_DLL_API HIST_ENTRY   *previous_history(void);
LIBEDIT_DLL_API HIST_ENTRY   *next_history(void);
LIBEDIT_DLL_API HIST_ENTRY  **history_list(void);
LIBEDIT_DLL_API int           history_search(const char *, int);
LIBEDIT_DLL_API int           history_search_prefix(const char *, int);
LIBEDIT_DLL_API int           history_search_pos(const char *, int, int);
LIBEDIT_DLL_API int           read_history(const char *);
LIBEDIT_DLL_API int           write_history(const char *);
LIBEDIT_DLL_API int           history_truncate_file(const char *, int);
LIBEDIT_DLL_API int           history_expand(char *, char **);
LIBEDIT_DLL_API char        **history_tokenize(const char *);
LIBEDIT_DLL_API const char   *get_history_event(const char *, int *, int);
LIBEDIT_DLL_API char         *history_arg_extract(int, int, const char *);

LIBEDIT_DLL_API char         *tilde_expand(char *);
LIBEDIT_DLL_API char         *filename_completion_function(const char *, int);
LIBEDIT_DLL_API char         *username_completion_function(const char *, int);
LIBEDIT_DLL_API int           rl_complete(int, int);
LIBEDIT_DLL_API int           rl_read_key(void);
LIBEDIT_DLL_API char        **completion_matches(/* const */ char *, rl_compentry_func_t *);
LIBEDIT_DLL_API void          rl_display_match_list(char **, int, int);

LIBEDIT_DLL_API int           rl_insert(int, int);
LIBEDIT_DLL_API int           rl_insert_text(const char *);
LIBEDIT_DLL_API int           rl_reset_terminal(const char *);
LIBEDIT_DLL_API void          rl_resize_terminal(void);
LIBEDIT_DLL_API int           rl_bind_key(int, rl_command_func_t *);
LIBEDIT_DLL_API int           rl_newline(int, int);
LIBEDIT_DLL_API void          rl_callback_read_char(void);
LIBEDIT_DLL_API void          rl_callback_handler_install(const char *, rl_vcpfunc_t *);
LIBEDIT_DLL_API void          rl_callback_handler_remove(void);
LIBEDIT_DLL_API void          rl_redisplay(void);
LIBEDIT_DLL_API int           rl_get_previous_history(int, int);
LIBEDIT_DLL_API void          rl_prep_terminal(int);
LIBEDIT_DLL_API void          rl_deprep_terminal(void);
LIBEDIT_DLL_API int           rl_read_init_file(const char *);
LIBEDIT_DLL_API int           rl_parse_and_bind(const char *);
LIBEDIT_DLL_API int           rl_variable_bind(const char *, const char *);
LIBEDIT_DLL_API int           rl_stuff_char(int);
LIBEDIT_DLL_API int           rl_add_defun(const char *, rl_command_func_t *, int);
LIBEDIT_DLL_API HISTORY_STATE *history_get_history_state(void);
LIBEDIT_DLL_API void           rl_get_screen_size(int *, int *);
LIBEDIT_DLL_API void           rl_set_screen_size(int, int);
LIBEDIT_DLL_API char          *rl_filename_completion_function(const char *, int);
LIBEDIT_DLL_API int           _rl_abort_internal(void);
LIBEDIT_DLL_API int           _rl_qsort_string_compare(char **, char **);
LIBEDIT_DLL_API char         **rl_completion_matches(const char *, rl_compentry_func_t *);
LIBEDIT_DLL_API void           rl_forced_update_display(void);
LIBEDIT_DLL_API int            rl_set_prompt(const char *);
LIBEDIT_DLL_API int            rl_on_new_line(void);
LIBEDIT_DLL_API void           rl_reset_after_signal(void);
LIBEDIT_DLL_API void           rl_echo_signal_char(int);
LIBEDIT_DLL_API int            rl_crlf(void);
LIBEDIT_DLL_API int            rl_ding(void);
LIBEDIT_DLL_API char          *rl_copy_text(int, int);
LIBEDIT_DLL_API void           rl_replace_line(const char *, int);
LIBEDIT_DLL_API int            rl_delete_text(int, int);
LIBEDIT_DLL_API void           rl_message(const char *format, ...) __attribute__((__format__(__printf__, 1, 2)));
LIBEDIT_DLL_API void           rl_save_prompt(void);
LIBEDIT_DLL_API void           rl_restore_prompt(void);

/*
 * The following are not implemented
 */
LIBEDIT_DLL_API int            rl_kill_text(int, int);
LIBEDIT_DLL_API Keymap         rl_get_keymap(void);
LIBEDIT_DLL_API void           rl_set_keymap(Keymap);
LIBEDIT_DLL_API Keymap         rl_make_bare_keymap(void);
LIBEDIT_DLL_API int            rl_generic_bind(int, const char *, const char *, Keymap);
LIBEDIT_DLL_API int            rl_bind_key_in_map(int, rl_command_func_t *, Keymap);
LIBEDIT_DLL_API int            rl_set_key(const char *, rl_command_func_t *, Keymap);
LIBEDIT_DLL_API void           rl_cleanup_after_signal(void);
LIBEDIT_DLL_API void           rl_free_line_state(void);
LIBEDIT_DLL_API int            rl_set_keyboard_input_timeout(int);
LIBEDIT_DLL_API int            rl_abort(int, int);
LIBEDIT_DLL_API int            rl_set_keymap_name(const char *, Keymap);
LIBEDIT_DLL_API histdata_t     free_history_entry(HIST_ENTRY *);
LIBEDIT_DLL_API void          _rl_erase_entire_line(void);

#ifdef __cplusplus
}
#endif

#endif /* _READLINE_H_ */
