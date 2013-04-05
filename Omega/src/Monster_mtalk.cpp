/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* mtalk.c */
/* monster talk functions */

#include "glob.h"

/* The druid's altar is in the northern forest */
void Monster::m_talk_druid()
{
    int i;
    MonsterList* curr;

    if (! m_statusp(this,HOSTILE)) {
        print1("The Archdruid raises a hand in greeting.");
        if (State.getSpokeToDruid() == false) {
            State.setSpokeToDruid( true );
            morewait();
            print1("The Archdruid congratulates you on reaching his sanctum.");
            print2("You feel competent.");
            /* DAG gain_experience starts with a morewait as well... */
            /* morewait(); */
            gain_experience(300);
            if (Player.patron == DRUID) {
                print1("The Archdruid conveys to you the wisdom of nature....");
                print2("You feel like a sage.");
                morewait();
                for(i=0; i<NUMRANKS; i++) {
                    if (Player.guildxp[i] > 0)
                        Player.guildxp[i] += 300;
                }
            }
        }
        mprint("Do you request a ritual of neutralization? [yn] ");
        if (ynq() == 'y') {
            if (Phase/2 == 6 || Phase/2 == 0) {	/* full or new moon */
                mprint("\"Unfortunately, I cannot perform a ritual of balance on");
                if (Phase/2 == 6)
                    mprint("this lawful day.\"");
                else
                    mprint("this chaotic day.\"");
            }
            else if (Phase/2 == 3 || Phase/2 == 9) {	/* half moon */
                mprint("You take part in today's holy celebration of balance...");
                Player.alignment = 0;
                Player.mana = calcmana();
                if (Player.patron == DRUID)
                    gain_experience(200);	/* if a druid wants to spend 2 days */
                Time+=60;		/* celebrating for 1600 xp, why not? */
                hourly_check();
                Time+=60;
                hourly_check();
                Time+=60;
                hourly_check();
                Time+=60;
                hourly_check();
                Time+=60;
                hourly_check();
                Time+=60;
                hourly_check();
            }
            else {
                mprint("The ArchDruid conducts a sacred rite of balance...");
                if (Player.patron == DRUID) {
                    Player.alignment = 0;
                    Player.mana = calcmana();
                }
                else
                    Player.alignment -= Player.alignment*max(0, 10 - Player.level)/10;
                /* the higher level the character is, the more set in his/her ways */
                Time+=60;
                hourly_check();
            }
            dataprint();
        }
    }
    else {
        mprint("The ArchDruid looks at you and cries: 'Unclean! Unclean!'");
        disrupt(Player.x,Player.y,100);
        mprint("This seems to have satiated his desire for vengeance.");
        mprint("'Have you learned your lesson?' The ArchDruid asks. [yn] ");
        /* WDT: this didn't used to have a 'y'.  I'this not sure what was
         * intended. */
        if (ynq() != 'n') {
            mprint("'I certainly hope so!' says the ArchDruid.");
            for (curr = Level->mlist; curr; curr = curr->next)
                m_status_reset(curr->monster, HOSTILE);
            this->m_vanish();
        }
        else {
            mprint("'Idiot.' mutters the ArchDruid.");
            p_damage(500,UNSTOPPABLE,"the ArchDruid's Vengeance");
        }
    }
}

void Monster::m_talk_silent()
{
    int reply = random_range(8);
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    switch (reply) {
    case 0:
        strcat(Str2," does not reply. ");
        break;
    case 1:
        strcat(Str2," shrugs silently. ");
        break;
    case 2:
        strcat(Str2," holds a finger to his mouth. ");
        break;
    case 3:
        strcat(Str2," glares at you but says nothing. ");
        break;
    case 4:
        strcat(Str2," is not going to answer you. ");
        break;
    case 5:
        strcat(Str2," has taken a vow of silence. ");
        break;
    case 6:
        strcat(Str2," attempts sign language. ");
        break;
    case 7:
        strcat(Str2," fails to respond.");
        break;
    }
    mprint(Str2);
}

void Monster::m_talk_stupid()
{

    int reply = random_range(4);
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    switch (reply) {
    case 0:
        strcat(Str2," looks at you with mute incomprehension.");
        break;
    case 1:
        strcat(Str2," growls menacingly and ignores you.");
        break;
    case 2:
        strcat(Str2," does not seem to have heard you.");
        break;
    case 3:
        strcat(Str2," tries to pretend it didn't hear you.");
        break;
    }
    mprint(Str2);
}

void Monster::m_talk_greedy()
{

    int reply = random_range(4);
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    switch (reply) {
    case 0:
        strcat(Str2," says: Give me a treasure.... ");
        break;
    case 1:
        strcat(Str2," says: Stand and deliver, knave! ");
        break;
    case 2:
        strcat(Str2," says: Your money or your life! ");
        break;
    case 3:
        strcat(Str2," says: Yield or Die! ");
        break;
    }
    mprint(Str2);
}

void Monster::m_talk_hungry()
{

    int reply = random_range(4);
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    switch (reply) {
    case 0:
        strcat(Str2," says: I hunger, foolish adventurer! ");
        break;
    case 1:
        strcat(Str2," drools menacingly at you. ");
        break;
    case 2:
        strcat(Str2," says: You're invited to be lunch! ");
        break;
    case 3:
        strcat(Str2," says: Feeed Meee! ");
        break;
    }
    mprint(Str2);
}

void Monster::m_talk_guard()
{
    if (m_statusp(this,HOSTILE)) {
        print1("'Surrender in the name of the Law!'");
        print2("Do it? [yn] ");
        if (ynq2()=='y') {
            Player.alignment++;
            if (Current_Environment == E_CITY) {
                print1("Go directly to jail. Do not pass go, do not collect 200Au.");
                print2("You are taken to the city gaol.");
                morewait();
                send_to_jail();
                drawvision(Player.x,Player.y);
            }
            else {
                clearmsg();
                print1("Mollified, the guard disarms you and sends you away.");
                dispose_lost_objects(1,Player.possessions[O_WEAPON_HAND]);
                pacify_guards();
            }
        }
        else {
            clearmsg();
            print1("All right, you criminal scum, you asked for it!");
        }
    }
    else /* not hostile */
    {
        if ( (!CitySiteList[L_TOURIST-CITYSITEBASE][0]) &&
                (Current_Environment == E_CITY) )
        {
            print1("You should visit the tourist information booth and get their guidebook.");
            print2("The booth is located near the entrance to the city.");
            morewait();
            CitySiteList[L_TOURIST-CITYSITEBASE][0] = true;
        }
        else if (Player.rank[ORDER]>0)
            print1("'Greetings comrade! May you always tread the paths of Law.'");
        else {
            if (random_range(2))
                print1("Move it right along, stranger!");
            else print1("Nothing to see here.");
        }
    }
}

void Monster::m_talk_mp()
{
    mprint("The mendicant priest asks you to spare some treasure for the needy");
}

void Monster::m_talk_titter()
{

    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    strcat(Str2," titters obscenely at you.");
    mprint(Str2);
}

void Monster::m_talk_ninja()
{
    mprint("The black-garbed figure says apologetically:");
    mprint("'Situree simasita, wakarimasen.'");
}

void Monster::m_talk_thief()
{
    if (Player.rank[THIEVES]) {
        if (this->level == 2)
            this->name = "sneak thief";
        else this->name = "master thief";
        print1("The cloaked figure makes a gesture which you recognize...");
        print2("...the thieves' guild recognition signal!");
        print3("'Sorry, mate, thought you were a mark....'");
        morewait();
        this->m_vanish();
    }
    else m_talk_man();

}

void Monster::m_talk_assassin()
{
    this->name = "master assassin";
    print1("The ominous figure does not reply, but hands you an embossed card:");
    print2("'Guild of Assassins Ops are forbidden to converse with targets.'");
}

void Monster::m_talk_im()
{
    pol tmp;

    if (strcmp(this->name,"itinerant merchant") != 0) {
        this->name = "itinerant merchant";
    }
    if (this->possessions == NULL)
        mprint("The merchant says: Alas! I have nothing to sell!");
    else {
        this->possessions->thing->known = 2;
        clearmsg();
        mprint("I have a fine");
        mprint(itemid(this->possessions->thing));
        mprint("for only");
        mlongprint(max(10,4*true_item_value(this->possessions->thing)));
        mprint("Au.");
        mprint("Want it? [yn] ");
        if (ynq()=='y') {
            if (Player.cash < (max(10,4*true_item_value(this->possessions->thing)))) {
                if (Player.alignment > 10) {
                    mprint("Well, I'll let you have it for what you've got.");
                    Player.cash = 0;
                    gain_item(this->possessions->thing);
                }
                else mprint("Beat it, you deadbeat!");
            }
            else {
                mprint("Here you are. Have a good day.");
                Player.cash -= max(10,(4*item_value(this->possessions->thing)));
                gain_item(this->possessions->thing);
            }
        }
        else
        {
            mprint("Sorry I couldn't help you.");
            free_obj( this->possessions->thing, true );
        }
        tmp = this->possessions;
        this->possessions = tmp->next;
        free( tmp );
        if (NULL != this->possessions)
        {
            mprint("Well then, I must be off. Good day.");
            this->m_vanish();
        }
    }
}

void Monster::m_talk_man()
{

    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    switch (random_range(5)) {
    case 0:
        strcat(Str2," asks you for the way home.");
        break;
    case 1:
        strcat(Str2," wishes you a pleasant day.");
        break;
    case 2:
        strcat(Str2," sneers at you contemptuously.");
        break;
    case 3:
        strcat(Str2," smiles and nods.");
        break;
    case 4:
        strcat(Str2," tells you a joke.");
        break;
    }
    mprint(Str2);
}

void Monster::m_talk_evil()
{

    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    switch (random_range(14)) {
    case 0:
        strcat(Str2," says: 'THERE CAN BE ONLY ONE!'");
        break;
    case 1:
        strcat(Str2," says: 'Prepare to die, Buckwheat!'");
        break;
    case 2:
        strcat(Str2," says: 'Time to die!'");
        break;
    case 3:
        strcat(Str2," says: 'There will be no mercy.'");
        break;
    case 4:
        strcat(Str2," insults your mother-in-law.");
        break;
    case 5:
        strcat(Str2," says: 'Kurav tu ando mul!'");
        break;
    case 6:
        strcat(Str2," says: '!va al infierno!'");
        break;
    case 7:
        strcat(Str2," says: 'dame desu, nee.'");
        break;
    case 8:
        strcat(Str2," spits on your rug and calls your cat a bastard.");
        break;
    case 9:
        strcat(Str2," snickers malevolently and draws a weapon.");
        break;
    case 10:
        strcat(Str2," sends 'rm -r *' to your shell!");
        break;
    case 11:
        strcat(Str2," tweaks your nose and cackles evilly.");
        break;
    case 12:
        strcat(Str2," thumbs you in the eyes.");
        break;
    case 13:
        strcat(Str2," kicks you in the groin.");
        break;
    }
    mprint(Str2);
}

void Monster::m_talk_robot()
{
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    switch (random_range(4)) {
    case 0:
        strcat(Str2," says: 'exterminate...Exterminate...EXTERMINATE!!!'");
        break;
    case 1:
        strcat(Str2," says: 'Kill ... Crush ... Destroy'");
        break;
    case 2:
        strcat(Str2," says: 'Danger -- Danger'");
        break;
    case 3:
        strcat(Str2," says: 'Yo Mama -- core dumped.'");
        break;
    }
    mprint(Str2);
}

void Monster::m_talk_slithy()
{
    mprint("It can't talk -- it's too slithy!");
}

void Monster::m_talk_mimsy()
{
    mprint("It can't talk -- it's too mimsy!");
}

void Monster::m_talk_burble()
{

    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    strcat(Str2," burbles hatefully at you.");
    mprint(Str2);
}

void Monster::m_talk_beg()
{
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    strcat(Str2," asks you for alms.");
    mprint(Str2);
}

void Monster::m_talk_hint()
{
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    if (m_statusp(this,HOSTILE)) {
        strcat(Str2," only sneers at you. ");
        mprint(Str2);
    }
    else {
        strcat(Str2," whispers in your ear: ");
        mprint(Str2);
        hint();
        this->talkf = M_TALK_SILENT;
    }
}

void Monster::m_talk_gf()
{
    mprint("The good fairy glints: Would you like a wish?");
    if (ynq()=='y') {
        mprint("The good fairy glows: Are you sure?");
        if (ynq()=='y') {
            mprint("The good fairy radiates: Really really sure?");
            if (ynq()=='y') {
                mprint("The good fairy beams: I mean, like, sure as sure can be?");
                if (ynq()=='y') {
                    mprint("The good fairy dazzles: You don't want a wish, right?");
                    if (ynq()=='y') mprint("The good fairy laughs: I thought not.");
                    else wish(0);
                }
            }
        }
    }
    mprint("In a flash of sweet-smelling light, the fairy vanishes....");
    Player.hp = max(Player.hp,Player.maxhp);
    Player.mana = max(Player.mana,calcmana());
    mprint("You feel mellow.");
    this->m_vanish();
}

void Monster::m_talk_ef()
{
    mprint("The evil fairy roils: Eat my pixie dust!");
    mprint("She waves her black-glowing wand, which screams thinly....");
    this->movef=M_MOVE_SMART;
    this->meleef=M_MELEE_POISON;
    this->specialf=M_SP_THIEF;
    m_status_set(this,HOSTILE);
    acquire(-1);
    bless(-1);
    sleep_player(this->level/2);
    summon(-1,-1);
    summon(-1,-1);
    summon(-1,-1);
    summon(-1,-1);
}

void Monster::m_talk_seductor()
{
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    if (Player.preference == 'n')
    {
        strcat(Str2, " notices your disinterest and leaves with a pout.");
        mprint(Str2);
    }
    else
    {
        strcat(Str2," beckons seductively...");
        mprint(Str2);
        mprint("Flee? [yn] ");
        if (ynq()=='y') {
            mprint("You feel stupid.");
        }
        else {
            strcpy(Str2,"The ");
            strcat(Str2,this->name);
            strcat(Str2," shows you a good time....");
            mprint(Str2);
            gain_experience(500);
            Player.con++;
        }
    }
    this->m_vanish();
}

void Monster::m_talk_demonlover()
{
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    if (Player.preference == 'n')
    {
        strcat(Str2, " notices your disinterest and changes with a snarl...");
        mprint(Str2);
        morewait();
    }
    else
    {
        strcat(Str2," beckons seductively...");
        mprint(Str2);
        mprint("Flee? [yn] ");
        if (ynq()=='y')
            mprint("You feel fortunate....");
        else {
            if (this->uniqueness == COMMON) {
                strcpy(Str2,"The ");
                strcat(Str2,this->name);
            }
            else strcpy(Str2,this->name);
            strcat(Str2," shows you a good time....");
            mprint(Str2);
            morewait();
            mprint("You feel your life energies draining...");
            level_drain(random_range(3)+1,"a demon's kiss");
            morewait();
        }
    }
    this->talkf = M_TALK_EVIL;
    this->meleef = M_MELEE_SPIRIT;
    this->specialf = M_SP_DEMON;
    m_status_reset(this,NEEDY);
    m_status_set(this,HOSTILE);

    if ((this->symbol&0xff) == 's') {
        this->symbol = 'I'|CLR(RED);
        this->name = "incubus";
    }
    else {
        this->symbol = 'S'|CLR(RED);
        this->name = "succubus";
    }
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    strcat(Str2," laughs insanely.");
    mprint(Str2);
    mprint("You now notice the fangs, claws, batwings...");
}

void Monster::m_talk_horse()
{
    if (m_statusp(this,HOSTILE))
        mprint("The horse neighs angrily at you.");
    else if (m_statusp(this,HUNGRY))
        mprint("The horse noses curiously at your pack.");
    else if (State.getMounted())
        mprint("The horse and your steed don't seem to get along.");
    else if (Current_Environment == Current_Dungeon)
        mprint("The horse shies; maybe he doesn't like the dungeon air....");
    else {
        mprint("The horse lets you pat his nose. Want to ride him? [yn] ");
        if (ynq()=='y') {
            this->m_remove();
            State.setMounted( true );
            calc_melee();
            mprint("You are now equitating!");
        }
    }
}

void Monster::m_talk_hyena()
{
    mprint("The hyena only laughs at you...");
}

void Monster::m_talk_parrot()
{
    mprint("Polly wanna cracker?");
}

void Monster::m_talk_servant()
{
    int target,x=Player.x,y=Player.y;
    if (this->id == SERV_LAW) {
        target = SERV_CHAOS;
        mprint("The Servant of Law pauses in thought for a moment.");
        mprint("You are asked: Are there any Servants of Chaos hereabouts? [yn] ");
    }
    else {
        target = SERV_LAW;
        mprint("The Servant of Chaos grins mischievously at you.");
        mprint("You are asked: Are there any Servants of Law hereabouts? [yn] ");
    }
    if (ynq()=='y') {
        print1("Show me.");
        show_screen();
        drawmonsters(true);
        setspot(&x,&y);
        if (Level->site[x][y].creature != NULL) {
            if (Level->site[x][y].creature->id == target) {
                mprint("The Servant launches itself towards its enemy.");
                mprint("In a blaze of combat, the Servants annihilate each other!");
                gain_experience(this->xpv);
                Level->site[x][y].creature->m_death();
                Level->site[this->x][this->y].creature = NULL;
                this->x = x;
                this->y = y;
                Level->site[x][y].creature = this;
                Level->site[x][y].creature->m_death();
            }
            else mprint("Right. Tell me about it. Idiot!");
        }
        else mprint("Right. Tell me about it. Idiot!");
    }
    else mprint("The servant shrugs and turns away.");
}

void Monster::m_talk_animal()
{
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    mprint(Str2);
    mprint("shows you a scholarly paper by Dolittle, D. Vet.");
    mprint("which demonstrates that animals don't have speech centers");
    mprint("complex enough to communicate in higher languages.");
    mprint("It giggles softly to itself and takes back the paper.");
}

void Monster::m_talk_scream()
{
    mprint("A thinly echoing scream reaches your ears....");
    morewait();
    mprint("You feel doomed....");
    morewait();
    mprint("A bird appears and flies three times widdershins around your head.");
    summon(-1,QUAIL);
    this->talkf = M_TALK_EVIL;
}

void Monster::m_talk_archmage()
{
    if (m_statusp(this,HOSTILE)) {
        mprint("The Archmage ignores your attempt at conversation");
        mprint("and concentrates on his spellcasting....");
    }
    else if (Current_Environment == E_COURT) {
        mprint("The Archmage congratulates you on getting this far.");
        mprint("He invites you to attempt the Throne of High Magic");
        mprint("but warns you that it is important to wield the Sceptre");
        mprint("before sitting on the throne.");
        if (Level->site[this->x][this->y].p_locf == L_THRONE) {
            mprint("The Archmage smiles and makes an arcane gesture....");
            this->m_vanish();
        }
    }
    else {
        mprint("The Archmage tells you to find him again in his");
        mprint("Magical Court at the base of his castle in the mountains");
        mprint("of the far North-East; if you do he will give you some");
        mprint("important information.");
    }
}

void Monster::m_talk_maharaja()
{
    if (m_statusp(this,HOSTILE)) {
        switch (random_range(4)) {
        case 0:
            mprint("The Maharaja says: 'So Mote it be!'");
            break;
        case 1:
            mprint("The Maharaja says: 'Let it be written!'");
            break;
        case 2:
            mprint("The Maharaja says: 'Let it be done!'");
            break;
        case 3:
            mprint("The Maharaja offers you goat-eyeball soup.'");
            break;
        }
    }
    else if (Current_Environment == E_PALACE) {
        mprint("The Maharaja insists that you leave. Or die.");
    }
    else {
        mprint("Where did I put those dungeons?");
    }
}

void Monster::m_talk_merchant()
{
    if (! m_statusp(this,HOSTILE)) {
        if (Current_Environment == E_VILLAGE) {
            mprint("The merchant asks you if you want to buy a horse for 250GP.");
            mprint("Pay the merchant? [yn] ");
            if (ynq()=='y') {
                if (Player.cash < 250)
                    mprint("The merchant says: 'Come back when you've got the cash!'");
                else {
                    Player.cash -= 250;
                    mprint("The merchant takes your money and tells you to select");
                    mprint("any horse you want in the stables.");
                    mprint("He says: 'You'll want to get to know him before trying to");
                    mprint("ride him. By the way, food for the horse is not included.'");
                    mprint("The merchant runs off toward the bank, cackling gleefully.");
                    this->m_vanish();
                }
            }
            else mprint("The merchant tells you to stop wasting his time.");
        }
        else {
            mprint("The merchant tells you to visit his stables at his village");
            mprint("for a great deal on a horse.");
        }
    }
    else {
        mprint("The merchant ignores you and screams:");
        mprint("'Help! Help! I'this being oppressed!'");
    }
}

void Monster::m_talk_prime()
{
    if (!m_statusp(this,HOSTILE)) {
        if (Current_Environment == E_CIRCLE) {
            print1("The Prime nods brusquely at you, removes a gem from his");
            print2("sleeve, places it on the floor, and vanishes wordlessly.");
            morewait();
            this->m_dropstuff();
            this->m_vanish();
        }
        else {
            print1("The Prime makes an intricate gesture, which leaves behind");
            print2("glowing blue sparks... He winks mischievously at you....");
            if (Player.rank[CIRCLE] > 0) {
                morewait();
                print1("The blue sparks strike you! You feel enhanced!");
                print2("You feel more experienced....");
                Player.pow+=Player.rank[CIRCLE];
                Player.mana += calcmana();
                gain_experience(1000);
                this->m_vanish();
            }
        }
    }
    else m_talk_evil();
}
