/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* extern.h -- function declarations */

/* newrand.c */
unsigned long genrand(void);
void sgenrand(unsigned long seed);

/* stats.c function(s) */
void editstats( void );
void display_stats( void );
void display_stat_slot( int );

/* omega.c functions */

int main( int,char *[] );
int game_restore( char * );
void init_world( void );
void inititem( int );
void initrand( int, int );
void signalexit(int);
void signalsave(int);

/* abyss.c functions */

#ifdef SAVE_LEVELS
void kill_all_levels( void );
void kill_levels( char * );
void msdos_init( void );
plv msdos_changelevel( plv,int,int );
#endif
void load_abyss( void );

/* aux1.c functions */

char *levelname( int );
char *mstatus_string( Monster * );
char *trapid( int );
int getdir( void );
int goberserk( void );
int p_country_moveable( int,int );
int p_moveable( int,int );
int player_on_sanctuary( void );
void calc_melee( void );
int damage_item( pob );
void describe_player( void );
void fight_monster( Monster * );
void foodcheck( void );
void gain_experience( int );
void p_damage( int,int,char * );
void p_death( char * );
void roomcheck( void );
void searchat( int,int );
void setspot( int *,int * );
void showroom( int );
void surrender( Monster * );
void threaten( Monster * );
void tunnelcheck( void );

/* aux2.c functions */

long expval( int );
long item_value( pob );
long true_item_value( pob );
char *actionlocstr( char );
int p_immune( int );
int player_hit( int,char,Monster * );
int statmod( int );
void break_weapon( void );
void change_environment( char );
void drop_weapon( void );
void enter_site( Symbol );
void gain_level( void );
void minute_status_check( void );
void moon_check( void );
void movecursor( int *,int *,int,int );
void p_drown( void );
void p_fumble( int );
void p_hit ( Monster *,int,int );
void p_win( void );
void player_miss( Monster *,int );
void tacplayer( Monster * );
void tenminute_status_check( void );
void toggle_item_use( int );
void torch_check( void );
void weapon_use( int,pob,Monster * );

/* aux3.c functions */

char *citysiteid( int );
char *countryid( Symbol );
char getlocation( void );
int hostilemonstersnear( void );
int magic_resist( int );
int maneuvers( void );
int parsecitysite( void );
int stonecheck( int );
void alert_guards( void );
void countrysearch( void );
void default_maneuvers( void );
void destroy_order( void );
void hourly_check( void );
void indoors_random_event( void );
void outdoors_random_event( void );
void showknownsites( int, int );
void tenminute_check( void );
void terrain_check( int );

/* bank.c functions */

struct object * bank_create_card (int, int);
bank_account * bank_create_account (int, int, char *);
bank_account * bank_index_password (char *);
bank_account * bank_index_number (int);
int bank_random_account_number (void);
void bank_init(void);
void l_bank (void);

/* char.c functions */

long calcmana( void );
FILE *omegarc_check( void );
int competence_check( int );
int fixnpc( int );
bool initplayer( void );
int initstats( void );
void omegan_character_stats( void );
void save_omegarc( void );
void user_character_stats( void );

/* city.c functions */

void assign_city_function( int,int );
void load_city( int );
void make_justiciar( int,int );
void make_major_undead( int,int );
void make_minor_undead( int,int );
/*void mazesite( int,int,int );
void randommazesite( int,int,int );*/
void repair_jail( void );
void resurrect_guards( void );

/* command1.c functions */

void p_country_process( void );
void p_process( void );

/* command2.c functions */

void pickup( void );
void activate( void );
void bash_item( void );
void bash_location( void );
void callitem( void );
void closedoor( void );
void disarm( void );
void downstairs( void );
void drop( void );
void eat( void );
void floor_inv( void );
void give( void );
void magic( void );
void movepincountry( int,int );
void moveplayer( int,int );
void opendoor( void );
void peruse( void );
void quaff( void );
void rest( void );
void save( int );
void search( int * );
void setoptions( void );
void talk( void );
void upstairs( void );
void zapwand( void );

/* command3.c functions */

void abortshadowform( void );
void charid( void );
void city_move( void );
void dismount_steed( void );
void examine( void );
void fire( void );
void frobgamestatus( void );
void help( void );
void hunt( Symbol );
void nap( void );
void pickpocket( void );
void quit(void);
void rename_player( void );
void tacoptions( void );
void tunnel( void );
void vault( void );
void version( void );
void wizard( void );

/* country.c functions */

void load_country( void );
void load_dlair( int,int );
void load_misle( int,int );
void load_speak( int,int );
void load_temple( int,int );
void make_high_priest( int,int,int );
void random_temple_site( int,int,int,int );

/* effect1.c functions */

int random_item (void);
int stolen_item (void);
void acquire( int );
void ball( int,int,int,int,int,int );
void bless( int );
void bolt( int,int,int,int,int,int,int );
void enchant( int );
void fball( int,int,int,int,int );
void fbolt( int,int,int,int,int,int );
void heal( int );
void identify( int );
void lball( int,int,int,int,int );
void lbolt( int,int,int,int,int,int );
void manastorm( int,int,int );
void mondet( int );
void nbolt( int,int,int,int,int,int );
void icebolt( int,int,int,int,int,int );
void objdet( int );
void snowball( int,int,int,int,int );
void wish( int );

/* effect2.c functions */

void accuracy( int );
void alert( int );
void augment( int );
void breathe( int );
void displace( int );
void flux( int );
void haste( int );
void i_chaos( pob );
void i_law( pob );
void invisible( int );
void knowledge( int );
void recover_stat( int );
void regenerate( int );
void sanctify( int );
void warp( int );

/* effect3.c functions */

int itemlist( int,int );
int monsterlist( void );
void acid_cloud( void );
void aggravate( void );
void amnesia( void );
void annihilate( int );
void apport( int );
void clairvoyance( int );
void cleanse( int );
void cure( int );
void deflection( int );
void disease( int );
void disintegrate( int,int );
void dispel( int );
void disrupt( int,int,int );
void drain( int );
void drain_life( int );
void hellfire( int,int,int );
void hero( int );
void hide( int,int );
void illuminate( int );
void inflict_fear( int,int );
void learnspell( int );
void level_drain( int,char * );
void level_return( void );
void levitate( int );
void p_poison( int );
void p_teleport( int );
void polymorph( int );
void sanctuary( void );
void shadowform( void );
void sleep_monster( int );
void sleep_player( int );
void strategic_teleport( int );
void summon( int,int );
void truesight( int );

/* env.c functions */

void load_arena( void );
void load_circle( int );
void load_court( int );
void make_archmage( int,int );
void make_prime( int,int );

/* etc.c functions */

char *nameprint( void );
char *slotstr( int );
char *wordnum( int );
void hint( void );
void learnclericalspells( int,int );

/* file.c functions */

void lock_score_file( void );
void unlock_score_file( void );
FILE *checkfopen( char *,char * );
int filecheck( void );
int test_file_access( char *, char );
void abyss_file( void );
void adeptfile( void );
void checkhigh( char *,int );
void cityguidefile( void );
void combat_help( void );
void commandlist( void );
void copyfile( char * );
void displayfile( char * );
void displaycryptfile( char * );
void extendlog( char *,int );
void filescanstring( FILE *,char * );
void inv_help( void );
void save_hiscore_npc( int );
void show_license( void );
void showmotd( void );
void showscores( void );
void theologyfile( void );
void user_intro( void );
void wishfile( void );

/* gen1.c functions */

char *roomname( int );
plv findlevel( struct level *,char );
void build_room( int,int,int,char,int );
void build_square_room( int,int,int,char,int );
void cavern_level( void );
void change_level( char,char,char );
void clear_level( struct level * );
void corridor_crawl( int *,int *,int,int,int,Symbol,char );
void find_stairs( char,char );
void free_dungeon( void );
void install_specials( void );
void install_traps( void );
void makedoor( int,int );
void sewer_corridor( int,int,int,int,Symbol );
void sewer_level( void );
void straggle_corridor( int,int,int,int,Symbol,char );

/* gen2.c functions */

void make_country_screen( Symbol );
void make_general_map( char * );
void make_forest( void );
void make_jungle( void );
void make_mountains( void );
void make_plains( void );
void make_river( void );
void make_road( void );
void make_stairs( int );
void make_swamp( void );
void maze_corridor( int,int,int,int,char,char );
void maze_level( void );
void room_corridor( int,int,int,int,int );
void room_level( void );

/* guild1.c functions */

void l_arena( void );
void l_castle( void );
void l_merc_guild( void );

/* guild2.c functions */

void l_college( void );
void l_monastery( void );
void l_order( void );
void l_sorcerors( void );
void l_thieves_guild( void );

/* house.c functions */

void load_house( int,int );
void make_house_npc( int,int );
void make_mansion_npc( int,int );

/* inv.c functions */

void do_inventory_control( void );
int  key_to_index( signed char );
signed char index_to_key( int );
long get_money( long );
char *cashstr( void );
char *itemid( pob );
int baditem( int );
int badobject( char );
int cursed( pob );
int find_and_remove_item( int,int );
int find_item( pob *,int,int );
int get_inventory_slot( void );
int get_item_number( pob );
int get_to_pack( pob );
int getitem( Symbol );
int getitem_prompt (char *, Symbol);
int item_useable( pob,int );
int objequal( struct object *,struct object * );
int slottable( pob,int );
int take_from_pack( int,int );
pob detach_money( long );
pob split_item( int,pob );
void add_to_pack( pob );
void conform_lost_object( pob );
void conform_lost_objects( int,pob );
void conform_unused_object( pob );
void dispose_lost_objects( int,pob );
void drop_at( int,int,pob );
void drop_from_slot( int );
void drop_money( void );
void fixpack( void );
void gain_item( pob );
void give_money( Monster * );
void givemonster( Monster *, struct object * );
void inventory_control( void );
void item_inventory( int );
void lose_all_items( void );
void merge_item( int );
void p_drop_at( int,int,int,pob );
void pack_extra_items( pob );
void pickup_at( int,int );
void put_to_pack( int );
void setchargestr( pob,char * );
void setnumstr( pob,char * );
void setplustr( pob,char * );
void show_inventory_slot( int,int );
void switch_to_slot( int );
void top_inventory_control( void );

/* item.c functions */

void shuffle( int [], int );
char *bootname( int );
char *cloakname( int );
char *grotname( void );
char *potionname( int );
char *ringname( int );
char *scrollname( int );
char *stickname( int );
int itemblessing( void );
int itemcharge( void );
int itemplus( void );
int twohandedp( int );
pob create_object( int );
void item_use( struct object * );
void make_armor( pob, int );
void make_artifact( pob, int );
void make_boots( pob, int );
void make_cash( pob, int );
void make_cloak( pob, int );
void make_corpse( pob, Monster * );
void make_food( pob, int );
void make_potion( pob, int );
void make_ring( pob, int );
void make_scroll( pob, int );
void make_shield( pob, int );
void make_stick( pob, int );
void make_thing( pob, int );
void make_weapon( pob, int );

/* itemf1.c functions */

void i_accuracy( pob );
void i_acquire( pob );
void i_alert( pob );
void i_augment( pob );
void i_azoth( pob );
void i_bless( pob );
void i_breathing( pob );
void i_charge( pob );
void i_clairvoyance( pob );
void i_corpse( pob );
void i_cure( pob );
void i_deflect( pob );
void i_displace( pob );
void i_enchant( pob );
void i_fear_resist( pob );
void i_flux( pob );
void i_food( pob );
void i_heal( pob );
void i_hero( pob );
void i_id( pob );
void i_illuminate( pob );
void i_immune( pob );
void i_invisible( pob );
void i_jane_t( pob );
void i_key( pob );
void i_knowledge( pob );
void i_lembas( pob );
void i_levitate( pob );
void i_mondet( pob );
void i_neutralize_poison( pob );
void i_no_op( pob );
void i_nothing( pob );
void i_objdet( pob );
void i_pepper_food( pob );
void i_perm_accuracy( pob );
void i_perm_agility( pob );
void i_perm_displace( pob );
void i_perm_hero( pob );
void i_perm_illuminate( pob );
void i_perm_invisible( pob );
void i_perm_levitate( pob );
void i_perm_negimmune( pob );
void i_perm_protection( pob );
void i_perm_speed( pob );
void i_perm_truesight( pob );
void i_pick( pob );
void i_poison_food( pob );
void i_pow( pob );
void i_raise_portcullis( pob );
void i_regenerate( pob );
void i_restore( pob );
void i_sleep_self( pob );
void i_speed( pob );
void i_spells( pob );
void i_stim( pob );
void i_teleport( pob );
void i_trap( pob );
void i_truesight( pob );
void i_warp( pob );
void i_wish( pob );

/* itemf2.c functions */

void i_defend( pob );
void i_demonblade( pob );
void i_desecrate( pob );
void i_lightsabre( pob );
void i_mace_disrupt( pob );
void i_normal_armor( pob );
void i_normal_shield( pob );
void i_normal_weapon( pob );
void i_perm_breathing( pob );
void i_perm_burden( pob );
void i_perm_deflect( pob );
void i_perm_energy_resist( pob );
void i_perm_fear_resist( pob );
void i_perm_fire_resist( pob );
void i_perm_gaze_immune( pob );
void i_perm_knowledge( pob );
void i_perm_poison_resist( pob );
void i_perm_regenerate( pob );
void i_perm_strength( pob );
void i_victrix( pob );
void weapon_acidwhip( int, pob, Monster * );
void weapon_arrow( int, pob, Monster * );
void weapon_bare_hands( int, Monster * );
void weapon_bolt( int, pob, Monster * );
void weapon_defend( int, pob, Monster * );
void weapon_demonblade( int, pob, Monster * );
void weapon_desecrate( int, pob, Monster * );
void weapon_firestar( int, pob, Monster * );
void weapon_lightsabre( int, pob, Monster * );
void weapon_mace_disrupt( int, pob, Monster * );
void weapon_normal_hit( int, pob, Monster * );
void weapon_scythe( int, pob, Monster * );
void weapon_tangle( int, pob, Monster * );
void weapon_victrix( int, pob, Monster * );
void weapon_vorpal( int, pob, Monster * );

/* itemf3.c functions */

int orbcheck( char );
void i_antioch( pob );
void i_apport( pob );
void i_crystal( pob );
void i_death( pob );
void i_disintegrate( pob );
void i_dispel( pob );
void i_disrupt( pob );
void i_enchantment( pob );
void i_fear( pob );
void i_fireball( pob );
void i_firebolt( pob );
void i_helm( pob );
void i_hide( pob );
void i_juggernaut( pob );
void i_kolwynia( pob );
void i_holding( pob );
void i_lball( pob );
void i_lbolt( pob );
void i_life( pob );
void i_missile( pob );
void i_orbair( pob );
void i_orbdead( pob );
void i_orbearth( pob );
void i_orbfire( pob );
void i_orbmastery( pob );
void i_orbwater( pob );
void i_planes( pob );
void i_polymorph( pob );
void i_sceptre( pob );
void i_sleep_other( pob );
void i_snowball( pob );
void i_stargem( pob );
void i_summon( pob );
void i_symbol( pob );
void i_serenity( pob );

/* lev.c functions */

int difficulty( void );
Monster* m_create( int,int,int,int );
Monster* make_creature( int );
void make_country_monsters( Symbol );
void make_site_monster( int,int,int );
void make_site_treasure( int,int,int );
void make_specific_treasure( int,int,int );
void populate_level( int );
void stock_level( void );
void wandercheck( void );

/* map.c functions */
map *map_open(enum map_identifier);
void map_setLevel(map*,int);
int map_getWidth(map*);
int map_getLength(map*);
int map_getDepth(map*);
char map_getSiteChar(map*,int,int);
void map_close(map*);

/* mmelee.c functions */

char random_loc( void );
int monster_hit( Monster *,char,int );
void m_hit( Monster *,int );
void monster_melee( Monster *,char,int );
void tacmonster( Monster * );
void transcribe_monster_actions( Monster * );

/* mmove.c functions */

void m_confused_move( Monster * );
void m_flutter_move( Monster * );
void m_follow_move( Monster * );
void m_move_animal( Monster * );
void m_normal_move( Monster * );
void m_random_move( Monster * );
void m_scaredy_move( Monster * );
void m_simple_move( Monster * );
void m_smart_move( Monster * );
void m_spirit_move( Monster * );
void m_teleport( Monster * );
void m_vanish( Monster * );
void m_move_leash( Monster * );

/* mon.c functions */

char *mancorpse( void );
char *angeltype( int,int );
void make_hiscore_npc(Monster*, int);
void make_log_npc(Monster*);

/* move.c functions */

void l_abyss( void );
void l_air_station( void );
void l_arena_exit( void );
void l_balancestone( void );
void l_chaos( void );
void l_chaostone( void );
void l_circle_library( void );
void l_drop_every_portcullis( void );
void l_earth_station( void );
void l_enter_circle( void );
void l_enter_court( void );
void l_escalator( void );
void l_fire( void );
void l_fire_station( void );
void l_hedge( void );
void l_house_exit( void );
void l_lava( void );
void l_lawstone( void );
void l_lift( void );
void l_magic_pool( void );
void l_mindstone( void );
void l_no_op( void );
void l_portcullis_trap( void );
void l_raise_portcullis( void );
void l_rubble( void );
void l_sacrificestone( void );
void l_tactical_exit( void );
void l_temple_warning( void );
void l_throne( void );
void l_tome1( void );
void l_tome2( void );
void l_voice1( void );
void l_voice2( void );
void l_voice3( void );
void l_void( void );
void l_void_station( void );
void l_voidstone( void );
void l_water( void );
void l_water_station( void );
void l_whirlwind( void );
void stationcheck( void );

/* movef.c functions */

void m_movefunction( Monster *,int );
void p_movefunction( int );
void sign_print( int,int,int );

/* mspec.c functions */

void m_aggravate( Monster * );
void m_huge_sounds( Monster * );
void m_illusion( Monster * );
void m_sp_acid_cloud( Monster * );
void m_sp_angel( Monster * );
void m_sp_av( Monster * );
void m_sp_blackout( Monster * );
void m_sp_bogthing( Monster * );
void m_sp_court( Monster * );
void m_sp_demon( Monster * );
void m_sp_demonlover( Monster * );
void m_sp_dragonlord( Monster * );
void m_sp_eater( Monster * );
void m_sp_escape( Monster * );
void m_sp_explode( Monster * );
void m_sp_ghost( Monster * );
void m_sp_lair( Monster * );
void m_sp_lw( Monster * );
void m_sp_mb( Monster * );
void m_sp_merchant( Monster * );
void m_sp_mirror( Monster * );
void m_sp_mp( Monster * );
void m_sp_ng( Monster * );
void m_sp_poison_cloud( Monster * );
void m_sp_prime( Monster * );
void m_sp_raise( Monster * );
void m_sp_seductor( Monster * );
void m_sp_servant( Monster * );
void m_sp_spell( Monster * );
void m_sp_surprise( Monster * );
void m_sp_swarm( Monster * );
void m_sp_were( Monster * );
void m_sp_whistleblower( Monster * );
void m_summon( Monster * );
void m_thief_f( Monster * );

/* mstrike.c functions */

void m_blind_strike( Monster * );
void m_fireball( Monster * );
void m_firebolt( Monster * );
void m_lball( Monster * );
void m_nbolt( Monster * );
void m_snowball( Monster * );
void m_strike_sonic( Monster * );

/* mtalk.c functions */

void m_talk_animal( Monster * );
void m_talk_archmage( Monster * );
void m_talk_assassin( Monster * );
void m_talk_beg( Monster * );
void m_talk_burble( Monster * );
void m_talk_demonlover( Monster * );
void m_talk_druid( Monster * );
void m_talk_ef( Monster * );
void m_talk_evil( Monster * );
void m_talk_gf( Monster * );
void m_talk_greedy( Monster * );
void m_talk_guard( Monster * );
void m_talk_hint( Monster * );
void m_talk_horse( Monster * );
void m_talk_hungry( Monster * );
void m_talk_hyena( Monster * );
void m_talk_im( Monster * );
void m_talk_man( Monster * );
void m_talk_merchant( Monster * );
void m_talk_mimsy( Monster * );
void m_talk_mp( Monster * );
void m_talk_ninja( Monster * );
void m_talk_parrot( Monster * );
void m_talk_prime( Monster * );
void m_talk_robot( Monster * );
void m_talk_scream( Monster * );
void m_talk_seductor( Monster * );
void m_talk_servant( Monster * );
void m_talk_silent( Monster * );
void m_talk_slithy( Monster * );
void m_talk_stupid( Monster * );
void m_talk_thief( Monster * );
void m_talk_titter( Monster * );
void m_talk_maharaja( Monster * );

/* priest.c functions */

int check_sacrilege( int );
void answer_prayer( void );
void hp_req_print( void );
void hp_req_test( void );
int increase_priest_rank( int );
void l_altar( void );
void make_hp( pob );

/* save.c functions */

int ok_outdated( int );
int restore_game( char * );
void restore_hiscore_npc( Monster*, int );
pob restore_item( FILE *, int );
pol restore_itemlist( FILE *, int );
void restore_country( FILE *, int );
void restore_level( FILE *, int );
void restore_monsters( FILE *,plv, int );
void restore_player( FILE *, int );
int save_game( char * );
int save_country( FILE * );
int save_item( FILE *,pob );
int save_itemlist( FILE *,pol );
int save_level( FILE *,plv );
int save_monsters( FILE *,MonsterList* );
int save_player( FILE * );

/* scr.c functions */

void omega_title( void );
int mcigetc( void );
long parsenum( char *message );
char *msgscanstring( void );
Symbol getspot( int,int,int );
char lgetc( void );
char menugetc( void );
char mgetc( void );
void cinema_hide (void);
void cinema_blank (void);
void cinema_print_line (int line, char * text);
int cinema_interact_line (int line, char * choices, char * text);
int cinema_ynq_line (int line, char * prompt);
int cinema_getnum_line (int line, char * prompt);
int cinema_confirm(char *action_description);
int cinema_ynq(char *action_description);
int cinema_interact(char *choices, char *line1, char *line2, char *line3);
void cinema_scene(char *line1, char *line2, char *line3);
int ynq( void );
int ynq1( void );
int ynq2( void );
int getnumber( int );
int litroom( int,int );
int move_slot( int,int,int );
int stillonblock( void );
void blankoutspot( int,int );
void blotspot( int,int );
void buffercycle( const char * );
int bufferappend( char * );
void bufferprint( void );
void checkclear( void );
void clear_if_necessary( void );
void clear_screen( void );
void clearmsg( void );
void clearmsg1( void );
void clearmsg3( void );
void colour_on( void );
void colour_off( void );
void commanderror( void );
void comwinprint( void );
void dataprint( void );
void deathprint( void );
void display_bigwin( void );
void display_death( char * );
void display_inventory_slot( int,int );
void display_option_slot( int );
void display_options( void );
void display_pack( void );
void display_possessions( void );
void display_quit( void );
void display_win( void );
void dobackspace( void );
void dodrawspot( int,int );
void draw_explosion( Symbol,int,int );
void drawmonsters( int );
void drawomega( void );
void drawplayer( void );
void drawscreen( void );
void drawspot( int,int );
void drawvision( int,int );
void endgraf( void );
void erase_level( void );
void erase_monster( Monster * );
void hide_line( int );
void initgraf( void );
void levelrefresh( void );
void lightspot( int,int );
void locprint( char * );
void maddch( char );
void menuaddch( char );
void menuclear( void );
void menulongprint( long );
void menunumprint( int );
void menuprint( char * );
void menuspellprint( int );
void showmenu( void );
void mlongprint( long );
void mnumprint( int );
void morewait( void );
void mprint( char * );
void nprint1( char * );
void nprint2( char * );
void nprint3( char * );
void phaseprint( void );
void bank_init( void );
void plotchar( Symbol,int,int );
void plotmon( Monster * );
void plotspot( int,int,int );
void print1( const char * );
void print2( char * );
void print3( char * );
void putspot( int,int,Symbol );
void redraw( void );
void screencheck( int,int );
void setlastxy( int, int );
void show_screen( void );
void omshowcursor( int,int );
void showflags( void );
void spreadroomdark( int,int,int );
void spreadroomlight( int,int,int );
void timeprint( void );
void xredraw( void );

/* site1.c functions */

void buyfromstock( int,int );
void l_alchemist( void );
void l_armorer( void );
void l_casino( void );
void l_club( void );
void l_commandant( void );
void l_crap( void );
void l_diner( void );
void l_dpw( void );
void l_gym( void );
void l_healer( void );
void l_library( void );
void l_pawn_shop( void );
void l_statue_wake( void );
void l_tavern( void );
void statue_random( int,int );
void wake_statue( int,int,int );

/* site2.c functions */

void cureforpay( void );
int gymtrain( int *,int * );
void healforpay( void );
void l_adept( void );
void l_brothel( void );
void l_cartographer( void );
void l_charity( void );
void l_condo( void );
void l_countryside( void );
void l_house( void );
void l_hovel( void );
void l_mansion( void );
void l_oracle( void );
void l_safe( void );
void l_trifid( void );
void l_vault( void );
void pacify_guards( void );
void send_to_jail( void );
void l_tourist( void );

/* spell.c functions */

char *spellid( int );
int getspell( void );
int spellparse( void );
void cast_spell( int );
void initspells( void );
void s_accuracy( void );
void s_alert( void );
void s_apport( void );
void s_bless( void );
void s_breathe( void );
void s_clairvoyance( void );
void s_cure( void );
void s_desecrate( void );
void s_disintegrate( void );
void s_dispel( void );
void s_disrupt( void );
void s_drain( void );
void s_enchant( void );
void s_fear( void );
void s_firebolt( void );
void s_haste( void );
void s_heal( void );
void s_hellfire( void );
void s_hero( void );
void s_identify( void );
void s_invisible( void );
void s_knowledge( void );
void s_lball( void );
void s_levitate( void );
void s_missile( void );
void s_mondet( void );
void s_objdet( void );
void s_polymorph( void );
void s_regenerate( void );
void s_restore( void );
void s_return( void );
void s_ritual( void );
void s_sanctify( void );
void s_sanctuary( void );
void s_shadowform( void );
void s_sleep( void );
void s_summon( void );
void s_teleport( void );
void s_TRUESIGHT( void );
void s_warp( void );
void s_wish( void );
void showknownspells( int, int );

/* time.c functions */

void fix_phantom( Monster * );
void time_clock( int );

/* trap.c functions */

void l_trap_abyss( void );
void l_trap_acid( void );
void l_trap_blade( void );
void l_trap_dart( void );
void l_trap_disintegrate( void );
void l_trap_door( void );
void l_trap_fire( void );
void l_trap_manadrain( void );
void l_trap_pit( void );
void l_trap_siren( void );
void l_trap_sleepgas( void );
void l_trap_snare( void );
void l_trap_teleport( void );

/* util.c functions */

void setPlayerXY( int,int );
long calc_points( void );
char *getarticle( char * );
char *month( void );
char *ordinal( int );
char *salloc( char * );
char inversedir( int );
void init_perms( void );
int confirmation( void );
char cryptkey( char* );
int day( void );
int distance( int,int,int,int );
int hitp( int,int );
int hour( void );
int inbounds( int,int );
int los_p( int,int,int,int );
int m_unblocked( Monster *,int,int );
int nighttime( void );
int offscreen( int,int );
int ok_to_free( plv );
void free_level( plv );
void free_mons_and_objs( MonsterList* );
void free_obj( pob, int );
pob copy_obj( pob );
void free_objlist( pol );
void *checkmalloc( unsigned int );
int random_range( unsigned int );
int screenmody( int );
int screenmodx( int );
int showhour( void );
int showminute( void );
int strmem( char,char * );
char *strjoin(char*,char*);

/* pdump.c functions */

void player_dump( void );
int strprefix( char *,char * );
int unblocked( int,int );
int view_los_p( int,int,int,int );
int view_unblocked( int,int );
void calc_weight( void );
void change_to_game_perms( void );
void change_to_user_perms( void );
void do_los( Symbol,int *,int *,int,int );
void do_object_los (Symbol, int *, int *, int, int);
void findspace( int *,int *,int );

/* village.c functions */

void assign_village_function( int,int,int );
void load_village( int, int );
void make_food_bin( int,int );
void make_guard( int,int );
void make_horse( int,int );
void make_merchant( int,int );
void make_sheep( int,int );
void special_village_site( int,int,int );

void getOmegaRCPath();

