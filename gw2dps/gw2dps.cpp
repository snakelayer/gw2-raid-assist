#include <inttypes.h>

#include "globals.h"

using namespace GW2LIB;
using namespace GW2LIB::GW2;
using namespace boost;

void ESP()
{
    //if (IsInterfaceHidden()) return;

    // Element Anchors
    Anchor aLeft, aTopLeft, aTop, aTopRight, aRight, aCenter, aBottom;
    Anchor bossDpsAnchor;

    bossDpsAnchor.x = round(GetWindowWidth() * 5 / 7);
    bossDpsAnchor.y = 8;

    aLeft.x = round(GetWindowWidth() / 6);
    aLeft.y = 75;

    //aTopLeft.x = round((GetWindowWidth() / 2 - 316 + 280) / 2 + 316);
    aTopLeft.x = round(GetWindowWidth() / 2);
    aTopLeft.y = 40;

    aTop.x = round(GetWindowWidth() / 2);
    aTop.y = 1;

    aTopRight.x = round((GetWindowWidth() / 2 - 294 - 179) / 2 + GetWindowWidth() / 2 + 179);
    aTopRight.y = 8;

    aRight.x = GetWindowWidth() - 10;
    aRight.y = 8;

    aCenter.x = round(GetWindowWidth() * float(0.5));
    aCenter.y = round(GetWindowHeight() * float(0.5));

    aBottom.x = round(GetWindowWidth() * float(0.5));
    aBottom.y = round(GetWindowHeight() - float(85));

    //AssistDrawer::get().drawFont(10, 10, AssistDrawer::WHITE, "delta: %i  down: %i  keys: %i  x: %i  y: %i  btn: %i\n%s", mouse_delta, mouse_down, mouse_keys, mouse_x, mouse_y, mouse_btn, chat.c_str());

    if (help)
    {
        stringstream ss;

        //ss << format("[%i] Exp Mode Crosshair (Alt H)\n") % expMode;
        //ss << format("\n");
        ss << format("[%i] Self Health Percent (%s)\n") % selfHealthPercent % get_key_description("Hotkeys.SELF_HEALTH_PERCENT");
        ss << format("\n");
        ss << format("[%i] Selected/Locked Target HP (%s)\n") % targetSelected % get_key_description("Hotkeys.TARGET_SELECTED");
        ss << format("[%i] Selected Target Details (%s)\n") % targetInfo % get_key_description("Hotkeys.TARGET_INFO");
        ss << format("[%i] Lock On Target (%s)\n") % targetLock % get_key_description("Hotkeys.TARGET_LOCK");
        ss << format("[%i] Allow Negative DPS (%s)\n") % dpsAllowNegative % get_key_description("Hotkeys.DPS_ALLOW_NEGATIVE");
        ss << format("\n");
        ss << format("[%i] DPS Meter (%s)\n") % logDps % get_key_description("Hotkeys.LOG_DPS");
        ss << format("[%i] DPS Meter History (%s)\n") % logDpsDetails % get_key_description("Hotkeys.LOG_DPS_DETAILS");
        ss << format("\n");
        ss << format("[%i] Kill Timer (%s)\n") % logKillTimer % get_key_description("Hotkeys.LOG_KILL_TIMER");
        ss << format("[%i] Kill Timer Details (%s)\n") % logKillTimerDetails % get_key_description("Hotkeys.LOG_KILL_TIMER_DETAILS");
        //ss << format("[%i] Kill Timer Writes to a File (Alt Num4)\n") % logKillTimerToFile;
        ss << format("\n");
        ss << format("[%i] Monitor Hits (%s)\n") % logHits % get_key_description("Hotkeys.LOG_HITS");
        ss << format("[%i] Show Hits History (%s)\n") % logHitsDetails % get_key_description("Hotkeys.LOG_HITS_DETAILS");
        ss << format("[%i] Record Hits to File (%s)\n") % logHitsToFile % get_key_description("Hotkeys.LOG_HITS_TO_FILE");
        ss << format("\n");
        ss << format("[%i] Monitor Attack Rate (%s)\n") % logAttackRate % get_key_description("Hotkeys.LOG_ATTACK_RATE");
        ss << format("[%i] Show Attack Rate History (%s)\n") % logAttackRateDetails % get_key_description("Hotkeys.LOG_ATTACK_RATE_DETAILS");
        ss << format("[%i] Record Attack Rate to File (%s)\n") % logAttackRateToFile % get_key_description("Hotkeys.LOG_ATTACK_RATE_TO_FILE");
        ss << format("[%i] Adjust Attack Rate Threshold +(%s) and -(%s)\n") % AttackRateChainHits % get_key_description("Hotkeys.ATTACKRATE_CHAIN_HITS_MORE") % get_key_description("Hotkeys.ATTACKRATE_CHAIN_HITS_LESS");
        ss << format("\n");
        ss << format("[%i] Nearby Ally Players List (%s)\n") % alliesList % get_key_description("Hotkeys.ALLIES_LIST");
        ss << format("[%i] Adjust WvW HP Bonus +(%s) and -(%s)\n") % wvwBonus % get_key_description("Hotkeys.WVW_BONUS_MORE") % get_key_description("Hotkeys.WVW_BONUS_LESS");
        ss << format("\n");
        ss << format("[%i] Count Ally NPCs (%s)\n") % floatAllyNpc % get_key_description("Hotkeys.FLOAT_ALLY_NPC");
        ss << format("[%i] Count Enemy NPCs (%s)\n") % floatEnemyNpc % get_key_description("Hotkeys.FLOAT_ENEMY_NPC");
        ss << format("[%i] Count Ally Players (%s)\n") % floatAllyPlayer % get_key_description("Hotkeys.FLOAT_ALLY_PLAYER");
        ss << format("[%i] Count Enemy Players (%s)\n") % floatEnemyPlayer % get_key_description("Hotkeys.FLOAT_ENEMY_PLAYER");
        //ss << format("[%i] Count Siege (Alt 5)\n") % floatSiege;
        ss << format("[%i] Show/Hide Floaters below counted (%s)\n") % floatCircles % get_key_description("Hotkeys.FLOAT_CIRCLES");
        ss << format("[%i] Floaters show Max HP / Distance (%s)\n") % floatType % get_key_description("Hotkeys.FLOAT_TYPE");
        ss << format("\n");
        ss << format("[%i] Speedometer (%s)\n") % logSpeedometer % get_key_description("Hotkeys.LOG_SPEEDOMETER");
        ss << format("[%i] Speedometer for Self/Enemy (%s)\n") % logSpeedometerEnemy % get_key_description("Hotkeys.LOG_SPEEDOMETER_ENEMY");
        ss << format("[%i] Measure Distance (%s)\n") % logDisplacement % get_key_description("Hotkeys.LOG_DISPLACEMENT");
        ss << format("[%i] Distance for Self/Enemy (%s)\n") % logDisplacementEnemy % get_key_description("Hotkeys.LOG_DISPLACEMENT_ENEMY");

        Vector2 strInfo = AssistDrawer::StringInfo(ss.str());
        float x = round(aCenter.x - strInfo.x / 2);
        float y = round(aCenter.y - strInfo.y / 2);

        AssistDrawer::get().drawBackground(x, y, strInfo);
        AssistDrawer::get().drawFont(x, y, (loopLimiter ? AssistDrawer::WHITE : AssistDrawer::CYAN), ss.str());
    }

    // JP Skills
    if (expMode)
    {
        float x = aCenter.x;
        float y = aCenter.y + 28;

        float box = 2;
        float line = 10;

        DrawLine(x - line, y, x + line, y, AssistDrawer::BORDER_COLOR);
        DrawLine(x, y - line, x, y + line, AssistDrawer::BORDER_COLOR);

        DrawRectFilled(x - box, y - box, box * 2, box * 2, 0xccFF0000);
        DrawRect(x - box, y - box, box * 2, box * 2, AssistDrawer::BORDER_COLOR);
    }

    // Targets & Agents //
    me = GetOwnCharacter();
    if (me.IsValid()){
        self.id = GetOwnAgent().GetAgentId();
        self.pos = GetOwnAgent().GetPos();

        self.cHealth = me.GetCurrentHealth();
        self.mHealth = me.GetMaxHealth();
        if (self.mHealth > 0)
            self.pHealth = 100.f * (self.cHealth / self.mHealth);
        else
            self.pHealth = 0;

        self.lvl = me.GetScaledLevel();
        self.lvlActual = me.GetLevel();
        self.alive = me.IsAlive();
    }

    if (selfFloat && GetOwnAgent().IsValid())
    {
        float rot = GetOwnAgent().GetRot();
        auto Char = GetOwnCharacter();
        Vector3 rotArrow = {
            self.pos.x + cos(rot) * 50.0f,
            self.pos.y + sin(rot) * 50.0f,
            self.pos.z
        };

        DWORD color = 0x4433ff00;
        float w = Char.GetCurrentHealth() / Char.GetMaxHealth() * 20;
        DrawCircleProjected(self.pos, 20.0f, color);
        DrawRectProjected(rotArrow, 20, 5, rot, color);
        DrawRectFilledProjected(rotArrow, w, 5, rot, color);
        DrawCircleFilledProjected(self.pos, 20.0f, color - AssistDrawer::FLOAT_MASK);
    }

    Agent agLocked = GetLockedSelection();
    if (agLocked.IsValid())
    {
        if (agLocked.GetAgentId() != selected.id)
            selected = {};

        int agType = agLocked.GetType();
        if (agType == GW2::AGENT_CATEGORY_CHAR) // char
        {
            selected.valid = true;
            selected.id = agLocked.GetAgentId();
            selected.type = agType;

            selected.pos = agLocked.GetPos();

            Character chLocked = agLocked.GetCharacter();
            selected.cHealth = chLocked.GetCurrentHealth();
            selected.mHealth = chLocked.GetMaxHealth();
            if (selected.mHealth > 0)
                selected.pHealth = 100.f * (selected.cHealth / selected.mHealth);
            else
                selected.pHealth = 0;
            selected.lvl = chLocked.GetScaledLevel();
            selected.lvlActual = chLocked.GetLevel();
            selected.breakbar = chLocked.GetBreakbarPercent() * 100;
        }
        else if (agType == GW2::AGENT_TYPE_GADGET) // structure
        {
            selected.valid = true;
            selected.id = agLocked.GetAgentId();
            selected.type = agType;

            selected.pos = agLocked.GetPos();

            Gadget gd = agLocked.GetGadget();
            selected.cHealth = gd.GetCurrentHealth();
            selected.mHealth = gd.GetMaxHealth();
            if (selected.mHealth > 0)
                selected.pHealth = 100.f * (selected.cHealth / selected.mHealth);
            else
                selected.pHealth = 0;
            //selected.lvl = chLocked.GetScaledLevel();
            //selected.lvlActual = chLocked.GetLevel();
        }
        else if (agType == GW2::AGENT_TYPE_GADGET_ATTACK_TARGET) // world boss
        {
            selected.valid = true;
            selected.id = agLocked.GetAgentId();
            selected.type = agType;

            selected.pos = agLocked.GetPos();

            AttackTarget tgt = agLocked.GetAttackTarget();
            selected.cHealth = tgt.GetCurrentHealth();
            selected.mHealth = tgt.GetMaxHealth();
            if (selected.mHealth > 0)
                selected.pHealth = 100.f * (selected.cHealth / selected.mHealth);
            else
                selected.pHealth = 0;

            //selected.lvl = chLocked.GetScaledLevel();
            //selected.lvlActual = chLocked.GetLevel();
        }
        else
            selected = {};

        if (selected.mHealth == 0)
            selected = {};
    }
    else
        selected = {};

    // Locked Target (ID)
    if (targetLock)
    {
        if (!locked.valid && selected.valid)
            targetLockID = selected.id;
    }
    else
    {
        if (!selected.valid)
        {
            locked = {};
            targetLockID = 0;
        }
        else
            targetLockID = selected.id;
    }

    // compile all agent data
    Floaters floaters;
    Allies allies;
    Agent ag;
    WBosses wbosses;
    while (ag.BeNext())
    {
        // Locked Target (Data)
        if (targetLockID == ag.GetAgentId())
        {
            int agType = ag.GetType();
            if (agType == GW2::AGENT_CATEGORY_CHAR) // char
            {
                locked.valid = true;
                locked.id = ag.GetAgentId();
                locked.type = agType;

                locked.pos = ag.GetPos();

                Character ch = ag.GetCharacter();
                locked.cHealth = ch.GetCurrentHealth();
                locked.mHealth = ch.GetMaxHealth();
                if (locked.mHealth > 0)
                    locked.pHealth = 100.f * (locked.cHealth / locked.mHealth);
                else
                    locked.pHealth = 0;
                locked.lvl = ch.GetScaledLevel();
                locked.lvlActual = ch.GetLevel();
            }
            else if (agType == GW2::AGENT_TYPE_GADGET) // structure
            {
                locked.valid = true;
                locked.id = ag.GetAgentId();
                locked.type = agType;

                locked.pos = ag.GetPos();

                Gadget gd = ag.GetGadget();
                locked.cHealth = gd.GetCurrentHealth();
                locked.mHealth = gd.GetMaxHealth();

                if (locked.mHealth > 0)
                    locked.pHealth = 100.f * (locked.cHealth / locked.mHealth);
                else
                    locked.pHealth = 0;
                //locked.lvl = ch.GetScaledLevel();
                //locked.lvlActual = ch.GetLevel();
            }
            else if (agType == GW2::AGENT_TYPE_GADGET_ATTACK_TARGET) // world boss
            {
                locked.valid = true;
                locked.id = ag.GetAgentId();
                locked.type = agType;

                locked.pos = ag.GetPos();

                AttackTarget tgt = ag.GetAttackTarget();
                locked.cHealth = tgt.GetCurrentHealth();
                locked.mHealth = tgt.GetMaxHealth();

                if (locked.mHealth > 0)
                    locked.pHealth = 100.f * (locked.cHealth / locked.mHealth);
                else
                    locked.pHealth = 0;
                //locked.lvl = ch.GetScaledLevel();
                //locked.lvlActual = ch.GetLevel();
            }

            if (locked.cHealth == 0 && locked.mHealth != 78870) // don't clear if 78870 (indestructible golem) or targetLocked
            {
                if (targetLock)
                    locked.alive = false;
                else
                    locked = {};
            }
        }

        // Wold Bosses
        if (woldBosses && ag.GetType() == GW2::AGENT_TYPE_GADGET_ATTACK_TARGET) {
            WBoss wboss;

            wboss.id = ag.GetAgentId();
            wboss.pos = ag.GetPos();

            AttackTarget tgt = ag.GetAttackTarget();
            wboss.cHealth = tgt.GetCurrentHealth();
            wboss.mHealth = tgt.GetMaxHealth();

            if (wboss.mHealth > 0)
                wboss.pHealth = 100.f * (wboss.cHealth / wboss.mHealth);
            else
                wboss.pHealth = 0;

            wbosses.list.push_back(wboss);
        }

        // Floaters
        if (floatAllyNpc || floatEnemyNpc || floatAllyPlayer || floatEnemyPlayer || floatSiege)
        {
            int agType = ag.GetType();

            // NPC & Player
            if (agType == GW2::AGENT_TYPE_CHAR)
            {
                Character ch = ag.GetCharacter();

                // gather data
                Vector3 pos = ag.GetPos();
                float rot = ag.GetRot();
                float cHealth = ch.GetCurrentHealth();
                float mHealth = ch.GetMaxHealth();
                int attitude = ch.GetAttitude();
                int prof = ch.GetProfession();
                string name = ch.GetName();

                // Filter the dead
                if (cHealth > 0 && mHealth > 1)
                {
                    // Filter those out of range
                    if (Dist(self.pos, pos) <= floatRadius)
                    {
                        Float floater;
                        floater.pos = pos;
                        floater.rot = rot;
                        floater.mHealth = mHealth;
                        floater.cHealth = cHealth;
                        floater.prof = prof;
                        floater.name = name;

                        // player vs npc
                        if (ch.IsPlayer() && !ch.IsControlled()) // (ignore self)
                        {
                            // allyPlayer
                            if (floatAllyPlayer && attitude == GW2::ATTITUDE_FRIENDLY)
                                floaters.allyPlayer.push_back(floater);

                            // enemyPlayer
                            if (floatEnemyPlayer && attitude == GW2::ATTITUDE_HOSTILE)
                                floaters.enemyPlayer.push_back(floater);
                        }

                        if (!ch.IsPlayer()){
                            // allyNpc
                            if (floatAllyNpc && (attitude == GW2::ATTITUDE_FRIENDLY || attitude == GW2::ATTITUDE_NEUTRAL))
                                floaters.allyNpc.push_back(floater);

                            // enemyNpc
                            if (floatEnemyNpc && (attitude == GW2::ATTITUDE_HOSTILE || attitude == GW2::ATTITUDE_INDIFFERENT))
                                floaters.enemyNpc.push_back(floater);
                        }
                    }
                }
            }
        }

        // Displacement
        if (logDisplacement)
        {
            if (logDisplacementEnemy)
            {
                if (locked.valid) {
                    if (locked.id != bufferDisplacement.id || (bufferDisplacement.start.x == 0 && bufferDisplacement.start.y == 0 && bufferDisplacement.start.z == 0))
                    {
                        bufferDisplacement.start = locked.pos;
                        bufferDisplacement.id = locked.id;
                    }
                    else
                    {
                        float displacement = Dist(bufferDisplacement.start, locked.pos);
                        bufferDisplacement.dist = int(displacement);
                    }
                }
                else
                {
                    // reset
                    bufferDisplacement = {};
                }
            }
            else
            {
                if (bufferDisplacement.start.x == 0 && bufferDisplacement.start.y == 0 && bufferDisplacement.start.z == 0)
                {
                    bufferDisplacement.start = self.pos;
                }
                else
                {
                    float displacement = Dist(bufferDisplacement.start, self.pos);
                    bufferDisplacement.dist = int(displacement);
                }
            }
        }
        else
        {
            // reset
            bufferDisplacement = {};
        }
    }

    // Left element
    {
        if (alliesList && (squad != nullptr)) {
            stringstream ss;
            squad->outputPlayerStats(ss);
            drawElementAt(ss, aLeft);
        
        }
    }

    if (showPing) {
        float ww = GetWindowWidth();
        float wh = GetWindowHeight();
        int ping = GetPing();
        int fps = GetFPS();
        int glide = (int)(me.GetGliderPercent()*100.0f);
        int rows = 3;

        stringstream ss;
        ss << format("%i\n%i\n%i") % fps % ping % glide;

        Vector2 strInfo = AssistDrawer::StringInfo(ss.str());
        float x = round(ww - strInfo.x - 2);
        float y = round(wh - AssistDrawer::lineHeight * rows - 4);

        AssistDrawer::get().drawBackground(x, y, strInfo);
        AssistDrawer::get().drawFont(x, y, (floatCircles ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());
    }

    // Bottom Element //
    {
        stringstream ss;

        if (selfHealthPercent && self.alive)
        {
            ss << format("%i") % (int)self.pHealth;

            Vector2 strInfo = AssistDrawer::StringInfo(ss.str());
            float x = round(aBottom.x - strInfo.x / 2);
            float y = round(aBottom.y - AssistDrawer::lineHeight);

            AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, ss.str());
        }
    }

    // TopLeft Element //
    {
        if (targetSelected)
        {
            //if (targetLock && selected.valid && locked.valid && selected.id == locked.id)
            if (targetLock && locked.valid)
            {

                stringstream ss;
                string formatString("Selected & Locked: %i / %i [%i%s]");
                ss << format(formatString) % (int)locked.cHealth % (int)locked.mHealth % (int)locked.pHealth % "%%";
                drawElementAt(ss, aTopLeft);

                // Prepare for Next Element
                aTopLeft.y += AssistDrawer::adjustYForNextElement(AssistDrawer::StringInfo(ss.str()).y);
            }
            else if (selected.valid)
            {
                stringstream ss;
                string formatString("Selected: %i / %i [%i%s]");
                ss << format(formatString) % (int)selected.cHealth % (int)selected.mHealth % (int)selected.pHealth % "%%";
                drawElementAt(ss, aTopLeft);

                // Prepare for Next Element
                aTopLeft.y += AssistDrawer::adjustYForNextElement(AssistDrawer::StringInfo(ss.str()).y);
            }

            if (targetInfo && selected.valid)
            {
                //aTopLeft.y += AssistDrawer::lineHeight;

                stringstream ss;

                if (targetInfoAlt)
                {
                    if (agLocked.GetType() == GW2::AGENT_TYPE_CHAR)
                    {
                        GW2::CharacterStats stats = agLocked.GetCharacter().GetStats();

                        ss << format("Power - %i") % stats.power;
                        ss << format("\nPrecision - %i") % stats.precision;
                        ss << format("\nToughness - %i") % stats.toughness;
                        ss << format("\nVitality - %i") % stats.vitality;
                        ss << format("\nFerocity - %i") % stats.ferocity;
                        ss << format("\nHealing - %i") % stats.healing;
                        ss << format("\nCondition - %i") % stats.condition;

                        ss << format("\n");
                        ss << format("\n(Agent: %p)") % (void**)agLocked.m_ptr;
                    }
                }
                else
                {
                    ss << format("Distance: %i\n") % int(Dist(self.pos, selected.pos));
                    if (agLocked.GetCharacter().GetBreakbarState() != GW2::BREAKBAR_STATE_NONE)
                        ss << format("BB: %3.05f%s") % selected.breakbar % "%%";
                }

                drawElementAt(ss, aTopLeft);

                // Prepare for Next Element
                aTopLeft.y += AssistDrawer::adjustYForNextElement(AssistDrawer::StringInfo(ss.str()).y);

                ss.str("");
            }
        }
    }

    // Top Elements (and floaters) //
    {
        if (floatAllyNpc || floatEnemyNpc || floatAllyPlayer || floatEnemyPlayer || floatSiege)
        {
            stringstream ss;

            ss << format("R: %i") % floatRadius;

            if (floatAllyNpc)
                ss << format(" | AllyNPCs: %i") % floaters.allyNpc.size();

            if (floatEnemyNpc)
                ss << format(" | FoeNPCs: %i") % floaters.enemyNpc.size();

            if (floatAllyPlayer)
                ss << format(" | Allies: %i") % floaters.allyPlayer.size();

            if (floatEnemyPlayer)
                ss << format(" | Foes: %i") % floaters.enemyPlayer.size();

            if (floatSiege)
                ss << format(" | Siege: %i") % floaters.siege.size();

            Vector2 strInfo = AssistDrawer::StringInfo(ss.str());
            float x = round(aTop.x - strInfo.x / 2);
            float y = round(aTop.y);

            AssistDrawer::get().drawBackground(x, y, strInfo);
            AssistDrawer::get().drawFont(x, y, (floatCircles ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

            aTop.y += AssistDrawer::adjustYForNextElement(strInfo.y);

            if (floatAllyPlayerProf)
            {
                int prof[10] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                for (auto & ally : floaters.allyPlayer) {
                    prof[ally.prof]++;
                }

                ss.str("");
                ss << format("War: %i") % prof[2];
                ss << format(" | Guard: %i") % prof[1];
                ss << format(" | Ele: %i") % prof[6];
                ss << format(" | Thief: %i") % prof[5];
                ss << format(" | Mes: %i") % prof[7];
                ss << format(" | Engi: %i") % prof[3];
                ss << format(" | Ranger: %i") % prof[4];
                ss << format(" | Necro: %i") % prof[8];
                ss << format(" | Rev: %i") % prof[9];

                strInfo = AssistDrawer::StringInfo(ss.str());
                float x = round(aTop.x - strInfo.x / 2);
                float y = round(aTop.y);

                AssistDrawer::get().drawBackground(x, y, strInfo);
                AssistDrawer::get().drawFont(x, y, (floatCircles ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

                aTop.y += AssistDrawer::adjustYForNextElement(strInfo.y);
            }

            if (floatCircles)
            {
                float x, y;
                if (floatAllyNpc && floaters.allyNpc.size() > 0)
                {
                    for (auto & floater : floaters.allyNpc) {
                        if (WorldToScreen(floater.pos, &x, &y))
                        {
                            stringstream fs;
                            //fs << floater.name << "\n";
                            if (floatType)
                                fs << format("%i") % int(Dist(self.pos, floater.pos));
                            else
                                fs << format("%i") % floater.mHealth;

                            AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, fs.str());

                            Vector3 rotArrow = {
                                floater.pos.x + cos(floater.rot) * 50.0f,
                                floater.pos.y + sin(floater.rot) * 50.0f,
                                floater.pos.z
                            };

                            DWORD color = 0x4433ff00;
                            float w = floater.cHealth / floater.mHealth * 20;
                            DrawCircleProjected(floater.pos, 20.0f, color);
                            DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
                            DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
                            DrawCircleFilledProjected(floater.pos, 20.0f, color - AssistDrawer::FLOAT_MASK);
                        }
                    }
                }

                if (floatEnemyNpc && floaters.enemyNpc.size() > 0)
                {
                    for (auto & floater : floaters.enemyNpc) {
                        if (WorldToScreen(floater.pos, &x, &y))
                        {
                            stringstream fs;
                            //fs << floater.name << "\n";
                            if (floatType)
                                fs << format("%i") % int(Dist(self.pos, floater.pos));
                            else
                                fs << format("%i") % floater.mHealth;

                            AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, fs.str());

                            Vector3 rotArrow = {
                                floater.pos.x + cos(floater.rot) * 50.0f,
                                floater.pos.y + sin(floater.rot) * 50.0f,
                                floater.pos.z
                            };

                            DWORD color = 0x44ff3300;
                            float w = floater.cHealth / floater.mHealth * 20;
                            DrawCircleProjected(floater.pos, 20.0f, color);
                            DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
                            DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
                            DrawCircleFilledProjected(floater.pos, 20.0f, color - AssistDrawer::FLOAT_MASK);
                        }
                    }
                }

                if (floatAllyPlayer && floaters.allyPlayer.size() > 0)
                {
                    for (auto & floater : floaters.allyPlayer) {
                        if (WorldToScreen(floater.pos, &x, &y))
                        {
                            float ww = GetWindowWidth() - 25;
                            float wh = GetWindowHeight() - 10;

                            if (x < 50) x = 50;
                            if (x > ww) x = ww;
                            if (y < 20) y = 20;
                            if (y > wh) y = wh;

                            stringstream fs;
                            //fs << floater.name << "\n";
                            if (floatType)
                                fs << format("%i") % int(Dist(self.pos, floater.pos));
                            else
                                fs << format("%i") % floater.mHealth;

                            AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, fs.str());

                            Vector3 rotArrow = {
                                floater.pos.x + cos(floater.rot) * 50.0f,
                                floater.pos.y + sin(floater.rot) * 50.0f,
                                floater.pos.z
                            };

                            DWORD color = 0x4433ff00;
                            float w = floater.cHealth / floater.mHealth * 20;
                            DrawCircleProjected(floater.pos, 20.0f, color);
                            DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
                            DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
                            DrawCircleFilledProjected(floater.pos, 20.0f, color - AssistDrawer::FLOAT_MASK);
                        }
                    }
                }

                if (floatEnemyPlayer && floaters.enemyPlayer.size() > 0)
                {
                    for (auto & floater : floaters.enemyPlayer) {
                        if (WorldToScreen(floater.pos, &x, &y))
                        {
                            stringstream fs;
                            //fs << floater.name << "\n";
                            if (floatType)
                                fs << format("%i") % int(Dist(self.pos, floater.pos));
                            else
                                fs << format("%i") % floater.mHealth;

                            AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, fs.str());

                            Vector3 rotArrow = {
                                floater.pos.x + cos(floater.rot) * 50.0f,
                                floater.pos.y + sin(floater.rot) * 50.0f,
                                floater.pos.z
                            };

                            DWORD color = 0x44ff3300;
                            float w = floater.cHealth / floater.mHealth * 20;
                            DrawCircleProjected(floater.pos, 20.0f, color);
                            DrawRectProjected(rotArrow, 20, 5, floater.rot, color);
                            DrawRectFilledProjected(rotArrow, w, 5, floater.rot, color);
                            DrawCircleFilledProjected(floater.pos, 20.0f, color - AssistDrawer::FLOAT_MASK);
                        }
                    }
                }

                if (floatSiege && floaters.siege.size() > 0)
                {
                    for (auto & floater : floaters.siege) {
                        //DWORD color = 0x44ff3300;
                        //DrawCircleProjected(floater.pos, 20.0f, color);
                        //DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
                    }
                }
            }
        }

        if (compDots) {
            compOverlay->RenderOverlay();
        }

        // World Boss
        if (woldBosses)
        {
            stringstream fs;
            fs << format("Wold Bosses: %i") % wbosses.list.size();

            Vector2 strInfo = AssistDrawer::StringInfo(fs.str());
            float lx = 12;
            float ly = 32;

            AssistDrawer::get().drawBackground(lx, ly, strInfo);
            AssistDrawer::get().drawFont(lx, ly, AssistDrawer::WHITE, fs.str());

            ly = ly + 12;

            if (wbosses.list.size() > 0) {
                float fx, fy;
                for (auto & wboss : wbosses.list) {
                    // floater
                    if (WorldToScreen(wboss.pos, &fx, &fy))
                    {
                        stringstream fs;
                        //fs << format("%i / %i") % wboss.cHealth % wboss.mHealth;
                        fs << format("[%i] %i") % wboss.id % int(Dist(self.pos, wboss.pos));

                        Vector2 strInfo = AssistDrawer::StringInfo(fs.str());
                        fx = round(fx - strInfo.x / 2);
                        fy = round(fy - 15);

                        AssistDrawer::get().drawBackground(fx, fy, strInfo);
                        AssistDrawer::get().drawFont(fx, fy, AssistDrawer::WHITE, fs.str());
                    }

                    // list
                    {
                        stringstream fs;
                        fs << format("[%i] %i / %i (%i)") % wboss.id % (int)wboss.cHealth % (int)wboss.mHealth % int(Dist(self.pos, wboss.pos));

                        Vector2 strInfo = AssistDrawer::StringInfo(fs.str());
                        ly = round(ly + strInfo.y + AssistDrawer::PADY);

                        AssistDrawer::get().drawBackground(lx, ly, strInfo);
                        AssistDrawer::get().drawFont(lx, ly, AssistDrawer::WHITE, fs.str());
                    }
                }
            }
        }

        if (logSpeedometer) {
            stringstream ss;

            ss << format("Speed: ");

            if (!bufferSpeedometer.empty())
            {
                double average[2] {};
                size_t samples = 0;

                // Speed ~ .5s
                samples = 5;
                if (samples > bufferSpeedometer.size())
                    samples = bufferSpeedometer.size();
                average[0] = 0;
                for (size_t i = 0; i < samples; i++)
                    average[0] += bufferSpeedometer[i];
                average[0] = average[0] / samples * (1000 / 100);

                // Speed ~ 3s
                samples = 30;
                if (samples > bufferSpeedometer.size())
                    samples = bufferSpeedometer.size();
                average[1] = 0;
                for (size_t i = 0; i < samples; i++)
                    average[1] += bufferSpeedometer[i];
                average[1] = average[1] / samples * (1000 / 100);

                // Prepare String
                ss << format("%i in/s, %i in/s") % int(average[0]) % int(average[1]);
            }
            else
            {
                ss << format("0 in/s, 0 in/s");
            }

            Vector2 strInfo = AssistDrawer::StringInfo(ss.str());
            float x = round(aTop.x - strInfo.x / 2);
            float y = round(aTop.y);

            AssistDrawer::get().drawBackground(x, y, strInfo);
            AssistDrawer::get().drawFont(x, y, (logSpeedometerEnemy ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

            aTop.y += AssistDrawer::adjustYForNextElement(strInfo.y);
        }

        if (logDisplacement)
        {
            stringstream ss;

            ss << format("Distance: %i") % bufferDisplacement.dist;

            Vector2 strInfo = AssistDrawer::StringInfo(ss.str());
            float x = round(aTop.x - strInfo.x / 2);
            float y = round(aTop.y);

            AssistDrawer::get().drawBackground(x, y, strInfo);
            AssistDrawer::get().drawFont(x, y, (logDisplacementEnemy ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

            aTop.y += AssistDrawer::adjustYForNextElement(strInfo.y);

        }
        else
        {
            if (logDisplacementStart.x != 0 || logDisplacementStart.y != 0 || logDisplacementStart.z != 0)
                logDisplacementStart = Vector3(0, 0, 0);
        }
    }

    // TopRight Elements //
    {
        if (logDps)
        {
            // separate ss vars
            stringstream ss;

            float aAdjustX = 120; // adjust anchor -120

            if (!bufferDps.empty())
            {
                averageDps[0] = computeAverage(10, bufferDps);
                averageDps[1] = computeAverage(30, bufferDps);
                averageDps[2] = computeAverage(60, bufferDps);

                if (boss == nullptr) {
                    ss << format("pDps: %d\n") % pc.getTotalDps();
                    ss << format("DPS(10s): %0.0f\n") % averageDps[0];
                    ss << format("DPS(30s): %0.0f\n") % averageDps[1];
                    ss << format("DPS(60s): %0.0f\n") % averageDps[2];
                }

                if (logDpsDetails)
                {
                    for (size_t i = 0; i < bufferDps.size(); i++)
                        ss << format("\nDP250ms: %i") % bufferDps[i];
                }
            }
            else
            {
                ss << format("(no target)\n");
            }

            Vector2 strInfo = AssistDrawer::StringInfo(ss.str());
            strInfo.x = aAdjustX; // box min-width with history stream
            float x = round(aTopRight.x - aAdjustX / 2); // perma anchor offset
            float y = round(aTopRight.y);

            AssistDrawer::get().drawBackground(x, y, strInfo);
            AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, ss.str());

            // Prepare for Next Element
            //ss.str("");
            //aTopRight.y += AssistDrawer::adjustYForNextElement(strInfo.y);
            aTopRight.x -= aAdjustX / 2 + AssistDrawer::PADX + 2;
        }
        else {
            pc.reset();
        }

        if (logKillTimer)
        {
            // separate ss vars
            stringstream ss;

            // Prepare String
            if (bufferKillTimer.time > 0)
            {
                ss << format("Timer: %s") % SecondsToString(bufferKillTimer.time);
                if (logKillTimerDetails)
                    ss << format("\nDPS: %0.2f") % bufferKillTimer.dps;
            }
            else
            {
                ss << format("Timer: 0.0s");
                if (logKillTimerDetails)
                    ss << format("\nDPS: 0.0");
            }

            Vector2 strInfo = AssistDrawer::StringInfo(ss.str());
            float x = 0;
            float y = round(aTopRight.y);
            if (logDps)
                x = round(aTopRight.x - strInfo.x - AssistDrawer::PADX); // perma anchor offset with logDps
            else
                x = round(aTopRight.x - strInfo.x / 2); // center otherwise

            AssistDrawer::get().drawBackground(x, y, strInfo);
            AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, ss.str());

            // Prepare for Next Element
            //ss.str("");
            aTopRight.y += AssistDrawer::adjustYForNextElement(strInfo.y);
            //aTopRight.x -= 0;
        }

    }

    // Right Elements //
    {
        if (logAttackRate)
        {
            stringstream ss;

            if (logAttackRateToFile)
                ss << format(".: Recording :.\n");
            else
                ss << format(".: Monitoring :.\n");
            ss << format(".: Attack Rate :.\n");
            ss << format("\n");

            if (!bufferAttackRate.empty())
            {
                //double min = *min_element(bufferAttackRate.begin(), bufferAttackRate.end());
                //double max = *max_element(bufferAttackRate.begin(), bufferAttackRate.end());
                double average = 0;
                for (size_t i = 0; i < bufferAttackRate.size(); i++)
                    average += bufferAttackRate[i];
                average = average / bufferAttackRate.size();

                ss << format("Counter: %i\n") % threadAttackRateCounter;
                //ss << format("Min: %0.3fs\n") % min;
                ss << format("Average: %0.3fs\n") % average;
                //ss << format("Max: %0.3fs\n") % max;

                if (logAttackRateDetails)
                {
                    ss << format("\n");
                    ss << format("History\n");
                    for (size_t i = 0; i < bufferAttackRate.size(); i++)
                        ss << format("\u2022 %0.3fs\n") % bufferAttackRate[i];
                }
            }
            else
            {
                ss << format("Counter: ...\n");
                //ss << format("Minimum: ...\n");
                ss << format("Average: ...\n");
                //ss << format("Maximum: ...\n");

                if (logAttackRateDetails)
                {
                    ss << format("\n");
                    ss << format("History\n");
                    ss << format("\u2022 ...\n");
                }
            }
            ss << format("\n");
            ss << format("Threshold: %i hits\n") % AttackRateChainHits;

            Vector2 strInfo = AssistDrawer::StringInfo(ss.str());

            float aAdjustX = 120; // adjust anchor -120
            if (strInfo.x < aAdjustX)
                strInfo.x = aAdjustX; // perma box min-width
            float x = round(aRight.x - strInfo.x);
            float y = round(aRight.y);

            AssistDrawer::get().drawBackground(x, y, strInfo);
            AssistDrawer::get().drawFont(x, y, (logAttackRateToFile ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

            // Prepare for Next Element
            //ss.str("");
            //aTopRight.y += AssistDrawer::adjustYForNextElement(strInfo.y);
            aRight.x = AssistDrawer::adjustXForNextElement(x);
        }

        if (logHits)
        {
            stringstream ss;

            if (logHitsToFile)
                ss << format(".: Recording :.\n");
            else
                ss << format(".: Monitoring :.\n");

            ss << format(".: Damage Hits :.\n");
            ss << format("\n");

            if (!bufferHits.empty())
            {
                //double min = *min_element(bufferHits.begin(), bufferHits.end());
                //double max = *max_element(bufferHits.begin(), bufferHits.end());
                double average = 0;
                for (size_t i = 0; i < bufferHits.size(); i++)
                    average += bufferHits[i];
                average = average / bufferHits.size();

                ss << format("Counter: %i\n") % threadHitsCounter;
                ss << format("Average: %0.1f\n") % average;

                if (logHitsDetails)
                {
                    ss << format("\n");
                    ss << format("History\n");
                    for (size_t i = 0; i < bufferHits.size(); i++)
                        ss << format("\u2022 %i\n") % bufferHits[i];
                }
            }
            else
            {
                ss << format("Counter: ...\n");
                ss << format("Average: ...\n");

                if (logHitsDetails)
                {
                    ss << format("\n");
                    ss << format("History\n");
                    ss << format("\u2022 ...\n");
                }
            }

            Vector2 strInfo = AssistDrawer::StringInfo(ss.str());


            float aAdjustX = 120; // adjust anchor -120
            if (strInfo.x < aAdjustX)
                strInfo.x = aAdjustX; // perma box min-width
            float x = round(aRight.x - strInfo.x);
            float y = round(aRight.y);

            AssistDrawer::get().drawBackground(x, y, strInfo);
            AssistDrawer::get().drawFont(x, y, (logHitsToFile ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

            // Prepare for Next Element
            //ss.str("");
            //aTopRight.y += AssistDrawer::adjustYForNextElement(strInfo.y);
            aRight.x = AssistDrawer::adjustXForNextElement(x);
        }

        if (logCrits)
        {
            stringstream ss;

            if (logCritsToFile)
                ss << format(".: Recording :.\n");
            else
                ss << format(".: Monitoring :.\n");

            ss << format(".: Crit Chance :.\n");
            ss << format("\n");

            ss << format("Samples: %i\n") % (logCritsGlances + logCritsNormals + logCritsCrits);
            ss << format("Glances: %i\n") % logCritsGlances;
            ss << format("Normals: %i\n") % logCritsNormals;
            ss << format("Crits: %i\n") % logCritsCrits;
            ss << format("\n");


            float critA = 0;
            if ((logCritsGlances + logCritsNormals + logCritsCrits) > 0)
                critA = logCritsCrits / (logCritsGlances + logCritsNormals + logCritsCrits + 0.f) * 100;

            float critB = 0;
            if ((logCritsNormals + logCritsCrits) > 0)
                critB = logCritsCrits / (logCritsNormals + logCritsCrits + 0.f) * 100;

            ss << format(".: Crit Chance :.\n");
            ss << format("GlIncl: %0.4f\n") % critA;
            ss << format("GlExcl: %0.4f\n") % critB;

            ss << format("\n");
            ss << format("Sample Hit: %i\n") % logCritsSample;


            Vector2 strInfo = AssistDrawer::StringInfo(ss.str());


            float aAdjustX = 120; // adjust anchor -120
            if (strInfo.x < aAdjustX)
                strInfo.x = aAdjustX; // perma box min-width
            float x = round(aRight.x - strInfo.x);
            float y = round(aRight.y);

            AssistDrawer::get().drawBackground(x, y, strInfo);
            AssistDrawer::get().drawFont(x, y, (logCritsToFile ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

            // Prepare for Next Element
            //ss.str("");
            //aTopRight.y += AssistDrawer::adjustYForNextElement(strInfo.y);
            aRight.x = AssistDrawer::adjustXForNextElement(x);
        }

        if (raid_debug) {
            displayDebug();
        }

        if (raid_boss_assist) {
            raid_boss_assist_was_on = true;

            if (boss != nullptr) {
                boss->drawAssistInfo();

                if (logDps) {					
                    stringstream ssDps;
                    ssDps << format("pDps: %d\n") % pc.getTotalDps();
                    boss->outputDps(ssDps);
                    drawElementAt(ssDps, bossDpsAnchor);
                }
            }
        }
    }
}

void displayDebug() {
    Anchor raidDebugAnchor;
    raidDebugAnchor.x = round(GetWindowWidth() * 3 / 4);
    raidDebugAnchor.y = 100;

    stringstream ss;
    ss << "Debug: \n";
    ss << format("mapId: %d\n") % GetCurrentMapId();
    ss << format("mapOpen: %s\n") % (IsMapOpen() ? "yes" : "no");
    ss << format("cameraPos: %f %f %f\n") % GetCameraPosition().x % GetCameraPosition().y % GetCameraPosition().z;
    ss << format("viewVec: %f %f %f\n") % GetViewVector().x % GetViewVector().y % GetViewVector().z;
    ss << format("FOV: %f\n") % GetFieldOfViewY();
    ss << format("mousePos: %f %f %f\n") % GetMouseInWorld().x % GetMouseInWorld().y % GetMouseInWorld().z;

    displayAgent("target", GetLockedSelection(), ss);
    displayAgent("self", GetOwnAgent(), ss);

    if (squad != nullptr) {
        ss << format("raid state: %d\n") % squad->getRaidState();
        ss << format("squad debugStr %s\n") % squad->getDebugStr();
    }

    if (boss != nullptr) {
        boss->outputDebug(ss);
    }

    drawElementAt(ss, raidDebugAnchor);
}

void displayAgent(string prefix, Agent &agent, stringstream &ss) {
    Character character = agent.GetCharacter();
    ss << format(prefix + " agentId: %d\n") % character.GetAgent().GetAgentId();
    ss << format(prefix + " location: x=%f, y=%f, z=%f\n") % agent.GetPos().x % agent.GetPos().y % agent.GetPos().z;
    ss << format(prefix + " selectable: %s\n") % (agent.IsSelectable() ? "yes" : "no");
    ss << format(prefix + " type: %d\n") % agent.GetType();
    ss << format(prefix + " gadget: %f / %f\n") % agent.GetGadget().GetCurrentHealth() % agent.GetGadget().GetMaxHealth();
    ss << format(prefix + " rot: %f\n") % agent.GetRot();
    ss << format(prefix + " token: %" PRIu64 "\n") % agent.GetToken();
    ss << format(prefix + " seq: %" PRIu64 "\n") % agent.GetSequence();
    ss << format(prefix + " speed: %.40f\n") % agent.GetSpeed();
    ss << format(prefix + " max speed: %.40f\n") % agent.GetMaxSpeed();
    ss << format(prefix + " name: %s\n") % agent.GetPlayer().GetName();
    ss << format(prefix + " alive: " + string(character.IsAlive() ? "yes" : "no") + "\n");
    ss << format(prefix + " downed: " + string(character.IsDowned() ? "yes" : "no") + "\n");
    ss << format(prefix + " monster: " + string(character.IsMonster() ? "yes" : "no") + "\n");
    ss << format(prefix + " monster player clone: " + string(character.IsClone() ? "yes" : "no") + "\n");
    ss << format(prefix + " valid: " + string(character.IsValid() ? "yes" : "no") + "\n");
    ss << format(prefix + " controlled: " + string(character.IsControlled() ? "yes" : "no") + "\n");
    ss << format(prefix + " downed: " + string(character.IsDowned() ? "yes" : "no") + "\n");
    ss << format(prefix + " player: " + string(character.IsPlayer() ? "yes" : "no") + "\n");
    ss << format(prefix + " pet: " + string(character.IsRangerPet() ? "yes" : "no") + "\n");
    ss << format(prefix + " gender: %d\n") % character.GetGender();
    ss << format(prefix + " breakbar state: %d\n") % character.GetBreakbarState();
    ss << format(prefix + " breakbar percent: %f\n") % character.GetBreakbarPercent();
    ss << format(prefix + " hp: %f/%f\n") % character.GetCurrentHealth() % character.GetMaxHealth();
    AttackTarget at = agent.GetAttackTarget();
    ss << format(prefix + " at hp: %f/%f\n") % at.GetCurrentHealth() % at.GetMaxHealth();
    ss << format(prefix + " endurance: %f/%f\n") % character.GetCurrentEndurance() % character.GetMaxEndurance();
    ss << format(prefix + " glide: %d\n") % character.GetGliderPercent();
    ss << format(prefix + " attitude: %d\n") % character.GetAttitude();
}

float computeAverage(size_t seconds, boost::circular_buffer<float> bufferDps) {
    size_t samples = seconds * 4;
    if (samples > bufferDps.size())
        samples = bufferDps.size();

    float sum = 0.0;
    for (size_t i = 0; i < samples; i++)
        sum += bufferDps[i];

    return sum / seconds;
}

void drawElementAt(stringstream &ss, Anchor &location) {
    Vector2 strInfo = AssistDrawer::StringInfo(ss.str());
    float x = round(location.x - strInfo.x / 2);
    float y = round(location.y);

    AssistDrawer::get().drawBackground(x, y, strInfo);
    AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, ss.str());
}

void chat_log(wchar_t *wtxt) {
    size_t len = wcslen(wtxt) + 1;

    char *txt = new char[len];
    memset(txt, 0, len);
    wcstombs(txt, wtxt, len);

    //font.Draw(10, 10, 0xffffffff, "%s", txt);
    //HL_LOG_DBG("%s\n", txt);
    chat = txt;
    delete txt;
}


bool mouse_move(int x, int y, int modkeys) {
    mouse_x = x;
    mouse_y = y;
    mouse_keys = modkeys;
    return true;
}

bool mouse_click(bool down, int button, int x, int y, int modkeys) {
    mouse_down = down;
    mouse_btn = button;
    mouse_x = x;
    mouse_y = y;
    mouse_keys = modkeys;
    return true;
}

bool mouse_wheel(int delta, int modkeys) {
    mouse_delta = delta;
    mouse_keys = modkeys;
    return true;
}

void dmg_log(Agent src, Agent tgt, int hit) {
    HL_LOG_DBG("srcId: %d, tgtId: %d, hit: %i\n", src.GetAgentId(), tgt.GetAgentId(), hit);
    // tgt isn't actually a valid Agent
    // if at some point it is, we can filter damage for the raid boss
    /*if ((squad != nullptr) && (boss != nullptr) && tgt.GetAgentId() == (boss->getAgentId())) {
        squad->updateDamage(src, hit);
    }*/
    if (squad != nullptr) {
        squad->updateDamage(src, hit);
    }
}

void combat_log(CombatLogType type, int hit, Agent agent) {
    switch (type) {
    case CL_CONDI_DMG_OUT:
    case CL_CRIT_DMG_OUT:
    case CL_GLANCE_DMG_OUT:
    case CL_PHYS_DMG_OUT:
        break;
    }

    pc.record(type, hit);
    HL_LOG_DBG("type: %i - hit: %i, agentId: %d\n", type, hit, agent.GetAgentId());
}

void GW2LIB::gw2lib_main()
{
    locale::global(locale("en-US"));

    init_keymap();
    init_config();
    load_preferences();

    compOverlay = new CompassOverlay();

    EnableEsp(ESP);
    SetGameHook(HOOK_CHAT, chat_log);
    SetGameHook(HOOK_MOUSE_MOVE, mouse_move);
    SetGameHook(HOOK_MOUSE_BUTTON, mouse_click);
    SetGameHook(HOOK_MOUSE_WHEEL, mouse_wheel);
    SetGameHook(HOOK_DAMAGE_LOG, dmg_log);
    SetGameHook(HOOK_COMBAT_LOG, combat_log);
    // TODO: new stuff to explore HOOK_ALLOCATOR and HOOK_LOGGER

    thread t1(threadHotKeys);
    thread t2(threadDps);
    thread t3(threadKillTimer);
    thread t4(threadHits);
    thread t5(threadAttackRate);
    thread t6(threadCrits);
    thread t7(threadSpeedometer);
    thread t8(threadRaidAssist);

    HMODULE dll = hl::GetCurrentModule();

    for (int i = 1; i < GW2::PROFESSION_NONE; i++) {
        stringstream res_id;
        res_id << "IDB_PNG" << i;
        HRSRC ires = FindResourceA(dll, res_id.str().c_str(), "PNG");
        if (ires && !profIcon[i].Init(LockResource(LoadResource(dll, ires)), SizeofResource(dll, ires))) {
            HL_LOG_ERR("Unable to load profession icon: %i\n", res_id);
        }
    }

    // wait for exit hotkey
    while (GetAsyncKeyState(VK_F12) >= 0)
        this_thread::sleep_for(chrono::milliseconds(25));

    close_config();

    // make threads clean-up first before interrupting
    PostThreadMessage(thread_id_hotkey, WM_USER, NULL, NULL);

    delete compOverlay;
    compOverlay = nullptr;

    // self destruct sequence
    t1.interrupt();
    t2.interrupt();
    t3.interrupt();
    t4.interrupt();
    t5.interrupt();
    t6.interrupt();
    t7.interrupt();
    t8.interrupt();

    this_thread::sleep_for(chrono::milliseconds(1000));
    return;
}
