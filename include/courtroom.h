#ifndef COURTROOM_H
#define COURTROOM_H

#include "aoapplication.h"
#include "aoblipplayer.h"
#include "aobutton.h"
#include "aocharbutton.h"
#include "aocharmovie.h"
#include "aoemotebutton.h"
#include "aoevidencebutton.h"
#include "aoevidencedisplay.h"
#include "aoimage.h"
#include "aolineedit.h"
#include "aomovie.h"
#include "aomusicplayer.h"
#include "aooptionsdialog.h"
#include "aopacket.h"
#include "aoscene.h"
#include "aosfxplayer.h"
#include "aotextarea.h"
#include "aotextedit.h"
#include "chatlogpiece.h"
#include "datatypes.h"
#include "debug_functions.h"
#include "file_functions.h"
#include "hardware_functions.h"
#include "lobby.h"
#include <QBrush>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QFont>
#include <QFuture>
#include <QHeaderView>
#include <QInputDialog>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMap>
#include <QMenu>
#include <QMetaObject>
#include <QMovie>
#include <QParallelAnimationGroup>
#include <QPlainTextEdit>
#include <QPropertyAnimation>
#include <QRegExp>
#include <QScrollBar>
#include <QSignalMapper>
#include <QSlider>
#include <QSpinBox>
#include <QTextBoundaryFinder>
#include <QTextBrowser>
#include <QTextCharFormat>
#include <QThread>
#include <QThreadPool>
#include <QTransform>
#include <QTreeWidget>
#include <QVector>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>

#include <stack>

class AOApplication;
class AOCharMovie;
class Courtroom : public QMainWindow {
  Q_OBJECT
public:
  explicit Courtroom(AOApplication *p_ao_app);

  void append_char(char_type p_char) { char_list.append(p_char); }
  void append_evidence(evi_type p_evi) { evidence_list.append(p_evi); }
  void append_music(QString f_music) { music_list.append(f_music); }
  void append_area(QString f_area) { area_list.append(f_area); }
  void clear_music() { music_list.clear(); }
  void clear_areas() { area_list.clear(); }
  void handle_failed_login();
  QString threading_sfx = "";
  QString threading_shake = "";
  QString threading_flash = "";
  QString threading_prefix = "";
  // cid and this may differ in cases of ini-editing
  QString current_char = "";
  int current_emote = 0;
  AOApplication *ao_app;
  // abstract widget to hold char buttons
  QWidget *ui_char_buttons;
  QVector<char_type> char_list;
  QVector<evi_type> evidence_list;
  QVector<QString> music_list;
  QVector<QString> area_list;
  QSignalMapper *char_button_mapper;
  QVector<AOCharButton *> ui_char_button_list;
  QVector<AOCharButton *> ui_char_button_list_filtered;
  QLineEdit *ui_char_search;
  QCheckBox *ui_char_passworded;
  QCheckBox *ui_char_taken;
  void mt_pre_framegetter(int frameNumber);
  void mt_framegetter(int frameNumber);
  void reset_music_list() { music_list.clear(); }

  void arup_append(int players, QString status, QString cm, QString locked)
  {
    arup_players.append(players);
    arup_statuses.append(status);
    arup_cms.append(cm);
    arup_locks.append(locked);
  }

  void arup_modify(int type, int place, QString value)
  {
    if (type == 0) {
      if (arup_players.size() > place)
        arup_players[place] = value.toInt();
    }
    else if (type == 1) {
      if (arup_statuses.size() > place)
        arup_statuses[place] = value;
    }
    else if (type == 2) {
      if (arup_cms.size() > place)
        arup_cms[place] = value;
    }
    else if (type == 3) {
      if (arup_locks.size() > place)
        arup_locks[place] = value;
    }
    list_areas();
  }

  void character_loading_finished();

  // sets position of widgets based on theme ini files
  void set_widgets();
  // sets font size based on theme ini files
  void set_font(QWidget *widget, QString p_identifier);
  // helper function that calls above function on the relevant widgets
  void set_fonts();

  void set_window_title(QString p_title);
  QPoint get_theme_pos(QString p_identifier);

  // reads theme inis and sets size and pos based on the identifier
  void set_size_and_pos(QWidget *p_widget, QString p_identifier);
  // sets status as taken on character with cid n_char and places proper shading
  // on charselect
  void set_taken(int n_char, bool p_taken);

  // sets the current background to argument. also does some checks to see if
  // it's a legacy bg
  void set_background(QString p_background);

  // sets the evidence list member variable to argument
  void set_evidence_list(QVector<evi_type> &p_evi_list);

  // called when a DONE#% from the server was received
  void done_received();

  // sets the local mute list based on characters available on the server
  void set_mute_list();

  // Sets the local pair list based on the characters available on the server.
  void set_pair_list();

  // sets desk and bg based on pos in chatmessage
  void set_scene();

  // sets text color based on text color in chatmessage
  void set_text_color();

  // And gets the colour, too!
  QColor get_text_color(QString color);

  // takes in serverD-formatted IP list as prints a converted version to server
  // OOC admittedly poorly named
  void set_ip_list(QString p_list);

  // disables chat if current cid matches second argument
  // enables if p_muted is false
  void set_mute(bool p_muted, int p_cid);

  // send a message that the player is banned and quits the server
  void set_ban(int p_cid);

  // cid = character id, returns the cid of the currently selected character
  int get_cid() { return m_cid; }
  QString get_current_char() { return current_char; }
  QString get_current_background() { return current_background; }

  // set the character using an ID
  void set_character(int char_id);

  // properly sets up some varibles: resets user state
  void enter_courtroom(int p_cid);

  // helper function that populates ui_music_list with the contents of
  // music_list
  void list_music();
  void list_areas();

  // these are for OOC chat
  void append_ms_chatmessage(QString f_name, QString f_message);
  void append_server_chatmessage(QString p_name, QString p_message,
                                 QString p_colour);

  // check whether or not current theme has images for buttons with fallback
  // text
  void detect_fallback_text();

  // these functions handle chatmessages sequentially.
  // The process itself is very convoluted and merits separate documentation
  // But the general idea is objection animation->pre animation->talking->idle
  void handle_chatmessage(QStringList *p_contents);
  void handle_chatmessage_2();
  void handle_chatmessage_3();

  // This function filters out the common CC inline text trickery, for appending
  // to the IC chatlog.
  QString filter_ic_text(QString p_text, bool skip_filter, int chat_color);

  // adds text to the IC chatlog. p_name first as bold then p_text then a newlin
  // this function keeps the chatlog scrolled to the top unless there's text
  // selected
  // or the user isn't already scrolled to the top
  void append_ic_text(QString p_text, QString p_name = "",
                      bool is_songchange = false, bool force_filter = false,
                      bool skip_filter = false, int chat_color = 0);

  // prints who played the song to IC chat and plays said song(if found on local
  // filesystem) takes in a list where the first element is the song name and
  // the second is the char id of who played it
  void handle_song(QStringList *p_contents);

  void play_preanim(bool noninterrupting);

  // plays the witness testimony or cross examination animation based on
  // argument
  void handle_wtce(QString p_wtce, int variant);

  // sets the hp bar of defense(p_bar 1) or pro(p_bar 2)
  // state is an number between 0 and 10 inclusive
  void set_hp_bar(int p_bar, int p_state);

  // Toggles the judge buttons, whether they should appear or not.
  void toggle_judge_buttons(bool is_on);
  void doScreenShake();
  void doRealization();

  void announce_case(QString title, bool def, bool pro, bool jud, bool jur,
                     bool steno, bool wit);

  void check_connection_received();

  void refresh_iclog(bool skiplast);

  ~Courtroom();

private:
  int m_courtroom_width = 714;
  int m_courtroom_height = 668;

  int m_viewport_x = 0;
  int m_viewport_y = 0;

  int m_viewport_width = 256;
  int m_viewport_height = 192;

  bool first_message_sent = false;
  int maximumMessages = 0;
  QPropertyAnimation *screenshake_animation;
  QPropertyAnimation *chatbox_screenshake_animation;
  QParallelAnimationGroup *screenshake_group;
  QMovie *frame_emote_checker;
  // This is for inline message-colouring.

  enum INLINE_COLOURS { INLINE_BLUE, INLINE_GREEN, INLINE_ORANGE, INLINE_GREY };

  // A stack of inline colours.
  std::stack<INLINE_COLOURS> inline_colour_stack;

  bool next_character_is_not_special = false; // If true, write the
                                              // next character as it is.

  bool message_is_centered = false;

  int current_display_speed = 3;
  int message_display_speed_slow[7] = {30, 40, 50, 60, 75, 100, 120};
  int message_display_speed[7] = {10, 20, 30, 40, 50, 60, 75};

  // This is for checking if the character should start talking again
  // when an inline blue text ends.
  bool entire_message_is_blue = false;

  // And this is the inline 'talking checker'. Counts how 'deep' we are
  // in inline blues.
  int inline_blue_depth = 0;

  // The character ID of the character this user wants to appear alongside with.
  int other_charid = -1;

  // The offset this user has given if they want to appear alongside someone.
  int offset_with_pair = 0;

  QVector<int> arup_players;
  QVector<QString> arup_statuses;
  QVector<QString> arup_cms;
  QVector<QString> arup_locks;

  QVector<chatlogpiece> ic_chatlog_history;

  // These map music row items and area row items to their actual IDs.
  QVector<int> music_row_to_number;
  QVector<int> area_row_to_number;

  // triggers ping_server() every 60 seconds
  QTimer *keepalive_timer;

  // determines how fast messages tick onto screen
  QTimer *chat_tick_timer;
  // int chat_tick_interval = 60;
  // which tick position(character in chat message) we are at
  int tick_pos = 0;
  // used to determine how often blips sound
  int blip_pos = 0;
  int blip_rate = 1;
  int rainbow_counter = 0;
  bool rainbow_appended = false;
  bool blank_blip = false;

  // The cursor to write with in mirror mode
  QTextCursor *mirror_cursor;
  // Used for getting the current maximum blocks allowed in the IC chatlog.
  int log_maximum_blocks = 0;

  // True, if the log should go downwards.
  bool log_goes_downwards = false;

  // delay before chat messages starts ticking
  QTimer *text_delay_timer;

  // delay before sfx plays
  QTimer *sfx_delay_timer;

  // keeps track of how long realization is visible(it's just a white square and
  // should be visible less than a second)
  QTimer *realization_timer;

  // every time point in char.inis times this equals the final time
  const int time_mod = 40;

  // the amount of time non-animated objection/hold it/takethat images stay
  // onscreen for in ms
  const int shout_stay_time = 724;

  // the amount of time non-animated guilty/not guilty images stay onscreen for
  // in ms
  const int verdict_stay_time = 3000;

  // the amount of time non-animated witness testimony/cross-examination images
  // stay onscreen for in ms
  const int wtce_stay_time = 1500;

  static const int chatmessage_size = 28;
  QString m_chatmessage[chatmessage_size];
  bool chatmessage_is_empty = false;

  QString previous_ic_message = "";

  // char id, muted or not
  QMap<int, bool> mute_map;

  // QVector<int> muted_cids;

  bool is_muted = false;

  // state of animation, 0 = objecting, 1 = preanim, 2 = talking, 3 = idle, 4 =
  // noniterrupting preanim
  int anim_state = 3;

  // state of text ticking, 0 = not yet ticking, 1 = ticking in progress, 2 =
  // ticking done
  int text_state = 2;

  // characters we consider punctuation
  const QString punctuation_chars = ".,?!:;";

  // amount by which we multiply the delay when we parse punctuation chars
  int punctuation_modifier = 0;
  bool slower_blips = false;
  // character id, which index of the char_list the player is
  int m_cid = -1;
  // cid and this may differ in cases of ini-editing

  QString char_name = "";

  int objection_state = 0;
  bool keep_custom_objection = false;
  QString objection_custom = "";
  int realization_state = 0;
  int screenshake_state = 0;
  int text_color = 0;
  bool is_presenting_evidence = false;

  int defense_bar_state = 0;
  int prosecution_bar_state = 0;

  int current_char_page = 0;
  int char_columns = 10;
  int char_rows = 9;
  int max_chars_on_page = 90;

  const int button_width = 60;
  const int button_height = 60;

  int current_emote_page = 0;
  int emote_columns = 5;
  int emote_rows = 2;
  int max_emotes_on_page = 10;

  QVector<evi_type> local_evidence_list;

  int current_evidence_page = 0;
  int current_evidence = 0;
  int evidence_columns = 6;
  int evidence_rows = 3;
  int max_evidence_on_page = 18;

  // is set to true if the bg folder contains defensedesk.png,
  // prosecutiondesk.png and stand.png
  bool is_ao2_bg = false;

  // whether or not to use text for buttons instead of images, true is text
  bool change_char_fallback = true;
  bool reload_theme_fallback = true;
  bool settings_fallback = true;
  bool call_mod_fallback = true;
  bool casing_fallback = true;
  bool amswap_fallback = true;
  bool ooc_toggle_fallback = true;

  // whether the ooc chat is server or master chat, true is server
  bool server_ooc = true;

  // Is AFK enabled
  bool isafk = false;
  QString current_background = "default";

  AOMusicPlayer *music_player;
  AOSfxPlayer *sfx_player;
  AOSfxPlayer *misc_sfx_player;
  AOSfxPlayer *frame_emote_sfx_player;
  AOSfxPlayer *pair_frame_emote_sfx_player;
  AOSfxPlayer *objection_player;
  AOBlipPlayer *blip_player;

  AOSfxPlayer *modcall_player;

  AOImage *ui_background;

  QWidget *ui_viewport;
  AOScene *ui_vp_background;
  AOMovie *ui_vp_speedlines;
  AOCharMovie *ui_vp_player_char;
  AOCharMovie *ui_vp_sideplayer_char;
  AOScene *ui_vp_desk;
  AOScene *ui_vp_legacy_desk;
  AOEvidenceDisplay *ui_vp_evidence_display;
  AOImage *ui_vp_chatbox;
  QLabel *ui_vp_showname;
  QTextEdit *ui_vp_message;
  AOMovie *ui_vp_realization;
  AOMovie *ui_vp_testimony;
  AOMovie *ui_vp_wtce;
  AOMovie *ui_vp_objection;
  void realization_done();

  bool colorf_iclog = false;
  bool mirror_iclog = false;
  bool colorf_limit = false;

  bool keep_evidence_display = false;

  QTextEdit *ui_ic_chatlog;

  AOTextArea *ui_ms_chatlog;
  AOTextArea *ui_server_chatlog;

  QListWidget *ui_mute_list;
  QListWidget *ui_area_list;
  QTreeWidget *ui_music_list;

  AOButton *ui_pair_button;
  QListWidget *ui_pair_list;
  QSpinBox *ui_pair_offset_spinbox;

  QLineEdit *ui_ic_chat_message;
  QLineEdit *ui_ic_chat_name;

  QLineEdit *ui_ooc_chat_message;
  QLineEdit *ui_ooc_chat_name;

  // QLineEdit *ui_area_password;
  QLineEdit *ui_music_search;
  QString music_search_par = "";
  QString area_search_par = "";

  QWidget *ui_emotes;
  QVector<AOEmoteButton *> ui_emote_list;
  AOButton *ui_emote_left;
  AOButton *ui_emote_right;

  QComboBox *ui_emote_dropdown;
  QComboBox *ui_pos_dropdown;

  AOImage *ui_defense_bar;
  AOImage *ui_prosecution_bar;

  QLabel *ui_music_label;
  QLabel *ui_sfx_label;
  QLabel *ui_blip_label;

  AOButton *ui_hold_it;
  AOButton *ui_objection;
  AOButton *ui_take_that;

  AOButton *ui_ooc_toggle;

  AOButton *ui_witness_testimony;
  AOButton *ui_cross_examination;
  AOButton *ui_guilty;
  AOButton *ui_not_guilty;

  AOButton *ui_change_character;
  AOButton *ui_reload_theme;
  AOButton *ui_call_mod;
  AOButton *ui_settings;
  AOButton *ui_announce_casing;
  AOButton *ui_switch_area_music;

  QCheckBox *ui_pre;
  QCheckBox *ui_flip;
  QCheckBox *ui_guard;
  QCheckBox *ui_casing;

  QCheckBox *ui_pre_non_interrupt;
  QCheckBox *ui_showname_enable;

  AOButton *ui_custom_objection;
  AOButton *ui_realization;
  AOButton *ui_screenshake;
  AOButton *ui_mute;

  QMenu *custom_obj_menu;

  AOButton *ui_defense_plus;
  AOButton *ui_defense_minus;

  AOButton *ui_prosecution_plus;
  AOButton *ui_prosecution_minus;

  QComboBox *ui_text_color;

  QSlider *ui_music_slider;
  QSlider *ui_sfx_slider;
  QSlider *ui_blip_slider;

  AOImage *ui_muted;

  QSpinBox *ui_log_limit_spinbox;
  QLabel *ui_log_limit_label;

  AOButton *ui_evidence_button;
  AOImage *ui_evidence;
  AOLineEdit *ui_evidence_name;
  QWidget *ui_evidence_buttons;
  QVector<AOEvidenceButton *> ui_evidence_list;
  AOButton *ui_evidence_left;
  AOButton *ui_evidence_right;
  AOButton *ui_evidence_present;
  AOImage *ui_evidence_overlay;
  AOButton *ui_evidence_delete;
  AOLineEdit *ui_evidence_image_name;
  AOButton *ui_evidence_image_button;
  AOButton *ui_evidence_x;
  AOTextEdit *ui_evidence_description;

  AOImage *ui_char_select_background;

  AOImage *ui_selector;

  AOButton *ui_back_to_lobby;

  QLineEdit *ui_char_password;

  AOButton *ui_char_select_left;
  AOButton *ui_char_select_right;

  AOButton *ui_spectator;

  void construct_char_select();
  void set_char_select();
  void set_char_select_page();
  void put_button_in_place(int starting, int chars_on_this_page);
  void filter_character_list();

  void construct_emotes();
  void set_emote_page();
  void set_emote_dropdown();

  void construct_evidence();
  void set_evidence_page();

public slots:
  void objection_done();
  void preanim_done();

  void mod_called(QString p_ip);

  void case_called(QString msg, bool def, bool pro, bool jud, bool jur,
                   bool steno, bool witness);

private slots:
  void start_chat_ticking();
  void play_sfx();

  void chat_tick();

  void on_mute_list_clicked(QModelIndex p_index);
  void on_pair_list_clicked(QModelIndex p_index);

  void on_chat_return_pressed();

  void on_ooc_return_pressed();

  void on_music_search_keypr();
  void on_music_search_edited(QString p_text);
  void on_music_list_double_clicked(QTreeWidgetItem *p_item, int column);
  void on_area_list_double_clicked(QModelIndex p_model);

  void select_emote(int p_id);

  void on_emote_clicked(int p_id);

  void on_emote_left_clicked();
  void on_emote_right_clicked();

  void on_emote_dropdown_changed(int p_index);
  void on_pos_dropdown_changed(int p_index);

  void on_evidence_name_edited();
  void on_evidence_image_name_edited();
  void on_evidence_image_button_clicked();
  void on_evidence_clicked(int p_id);
  void on_evidence_double_clicked(int p_id);

  void on_evidence_hover(int p_id, bool p_state);

  void on_evidence_left_clicked();
  void on_evidence_right_clicked();
  void on_evidence_present_clicked();

  void on_hold_it_clicked();
  void on_objection_clicked();
  void on_take_that_clicked();
  void on_custom_objection_clicked();
  void ShowContextMenu(const QPoint &pos);

  void on_realization_clicked();
  void on_screenshake_clicked();

  void on_mute_clicked();
  void on_pair_clicked();

  void on_defense_minus_clicked();
  void on_defense_plus_clicked();
  void on_prosecution_minus_clicked();
  void on_prosecution_plus_clicked();

  void on_text_color_changed(int p_color);

  void on_music_slider_moved(int p_value);
  void on_sfx_slider_moved(int p_value);
  void on_blip_slider_moved(int p_value);

  void on_log_limit_changed(int value);
  void on_pair_offset_changed(int value);

  void on_ooc_toggle_clicked();

  void on_witness_testimony_clicked();
  void on_cross_examination_clicked();
  void on_not_guilty_clicked();
  void on_guilty_clicked();

  void on_change_character_clicked();
  void on_reload_theme_clicked();
  void on_call_mod_clicked();
  void on_settings_clicked();
  void on_announce_casing_clicked();

  void on_pre_clicked();
  void on_flip_clicked();
  void on_guard_clicked();

  void on_showname_enable_clicked();

  void on_evidence_button_clicked();

  void on_evidence_delete_clicked();
  void on_evidence_x_clicked();

  void on_back_to_lobby_clicked();

  void on_char_select_left_clicked();
  void on_char_select_right_clicked();
  void on_char_search_changed();
  void on_char_taken_clicked();
  void on_char_passworded_clicked();

  void on_spectator_clicked();

  void char_clicked(int n_char);

  void on_switch_area_music_clicked();

  void on_casing_clicked();

  void ping_server();

#ifdef BASSAUDIO
  void load_bass_opus_plugin();
#endif
};

#endif // COURTROOM_H
