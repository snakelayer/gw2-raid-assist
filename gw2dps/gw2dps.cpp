#include "gw2lib.h"
#include "gw2dps.h"
#include "config.h"
#include "keymap.h"
#include "hotkey.h"
#include "preferences.h"
#include "assist_drawer.h"
#include "raid/squad.h"
#include "boss/unknown_boss.h"
#include "boss/vale_guardian.h"
#include "boss/raid_boss_factory.h"

// Settings //
bool killApp = false;

bool help = false;
bool expMode = false;
bool selfFloat = false;
bool selfHealthPercent = true;
bool loopLimiter = true;

bool woldBosses = false;

bool targetSelected = true;
bool targetInfo = false;
bool targetInfoAlt = false;
bool targetLock = false;

bool dpsAllowNegative = false;

bool logDps = true;
bool logDpsDetails = false;
string logDpsFile = "gw2dpsLog-Dps.txt";

float averageDps[3] = { 0.0f, 0.0f, 0.0f };
float secondsToDeath = 0.0f;
bool logRaidAssistToFile = true;

bool logKillTimer = false;
bool logKillTimerDetails = false;
bool logKillTimerToFile = false;

bool logHits = false;
bool logHitsDetails = false;
bool logHitsToFile = false;
string logHitsFile = "gw2dpsLog-Hits.txt";
int threadHitsCounter = 0;

bool logAttackRate = false;
bool logAttackRateDetails = false;
bool logAttackRateToFile = false;
int AttackRateChainHits = 1;
int AttackRateChainTime = 0; // not used atm
string logAttackRateFile = "gw2dpsLog-AttackRate.txt";
int threadAttackRateCounter = 0;

bool logCrits = false;
bool logCritsDetails = true;
int logCritsSample = 0;
int logCritsGlances = 0;
int logCritsNormals = 0;
int logCritsCrits = 0;
bool logCritsToFile = false;
string logCritsFile = "gw2dpsLog-Crits.txt";

bool alliesList = false;
int wvwBonus = 0;

bool floatAllyNpc = false;
bool floatEnemyNpc = false;
bool floatAllyPlayer = false;
bool floatAllyPlayerProf = false;
bool floatEnemyPlayer = false;
bool floatSiege = false;
int floatRadius = 7000;
bool floatCircles = false;
bool floatType = true; // false = HP; true = Dist;

bool logSpeedometer = false;
bool logSpeedometerEnemy = false;
int logDisplacementValue = 0;
bool logDisplacement = false;
bool logDisplacementEnemy = false;
Vector3 logDisplacementStart = Vector3(0, 0, 0);

bool raid_debug = false;
bool raid_boss_assist = false;

Squad *squad;
RaidBoss *boss;

#ifdef ARCH_64BIT
uintptr_t hp_shift1 = 0x58;
uintptr_t hp_shift2 = 0x1c0;
uintptr_t hp_shift3 = 0x50;
uintptr_t hp_shift4 = 0x1e8;
uintptr_t hp_shift_cur = 0xc;
uintptr_t hp_shift_max = 0x10;
#else
uintptr_t hp_shift1 = 0x34;
uintptr_t hp_shift2 = 0x178;
uintptr_t hp_shift3 = 0x2c;
uintptr_t hp_shift4 = 0x18c;
uintptr_t hp_shift_cur = 0x8;
uintptr_t hp_shift_max = 0xc;
#endif

DWORD thread_id_hotkey = 0;

// Threads //
#include "thread.Hotkeys.cpp"
#include "thread.Dps.cpp"
#include "thread.KillTimer.cpp"
#include "thread.Hits.cpp"
#include "thread.AttackRate.cpp"
#include "thread.Crits.cpp"
#include "thread.Speedometer.cpp"
#include "thread.RaidAssist.cpp"
#include "thread.SquadSpeedometer.cpp"

// Self
Character me;
Agent meAg = me.GetAgent();

void ESP()
{
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

		StrInfo strInfo;
		strInfo = AssistDrawer::StringInfo(ss.str());
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

	if (selfFloat)
	{
		DWORD color = 0x4433ff00;
		DrawCircleProjected(self.pos, 20.0f, color);
		DrawCircleFilledProjected(self.pos, 20.0f, color - 0x30000000);
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

			unsigned long shift = *(unsigned long*)agLocked.m_ptr;
			shift = *(unsigned long*)(shift + hp_shift1);
			shift = *(unsigned long*)(shift + hp_shift2);
			if (shift)
			{
				selected.cHealth = *(float*)(shift + hp_shift_cur);
				selected.mHealth = *(float*)(shift + hp_shift_max);
			}
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

			unsigned long shift = *(unsigned long*)agLocked.m_ptr;
			shift = *(unsigned long*)(shift + hp_shift1);
			shift = *(unsigned long*)(shift + hp_shift3);
			shift = *(unsigned long*)(shift + hp_shift4);
			if (shift)
			{
				selected.cHealth = *(float*)(shift + hp_shift_cur);
				selected.mHealth = *(float*)(shift + hp_shift_max);
			}
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

				unsigned long shift = *(unsigned long*)ag.m_ptr;
				shift = *(unsigned long*)(shift + hp_shift1);
				shift = *(unsigned long*)(shift + hp_shift2);
				if (shift)
				{
					locked.cHealth = *(float*)(shift + hp_shift_cur);
					locked.mHealth = *(float*)(shift + hp_shift_max);
				}
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

				unsigned long shift = *(unsigned long*)ag.m_ptr;
				shift = *(unsigned long*)(shift + hp_shift1);
				shift = *(unsigned long*)(shift + hp_shift3);
				shift = *(unsigned long*)(shift + hp_shift4);
				if (shift)
				{
					locked.cHealth = *(float*)(shift + hp_shift_cur);
					locked.mHealth = *(float*)(shift + hp_shift_max);
				}
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

			unsigned long shift = *(unsigned long*)ag.m_ptr;
			shift = *(unsigned long*)(shift + hp_shift1);
			shift = *(unsigned long*)(shift + hp_shift3);
			shift = *(unsigned long*)(shift + hp_shift4);
			if (shift)
			{
				wboss.cHealth = *(float*)(shift + hp_shift_cur);
				wboss.mHealth = *(float*)(shift + hp_shift_max);
			}
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
				float cHealth = ch.GetCurrentHealth();
				float mHealth = ch.GetMaxHealth();
				int attitude = ch.GetAttitude();
				int prof = ch.GetProfession();

				// Filter the dead
				if (cHealth > 0 && mHealth > 1)
				{
					// Filter those out of range
					if (Dist(self.pos, pos) <= floatRadius)
					{
						Float floater;
						floater.pos = pos;
						floater.mHealth = mHealth;
						floater.prof = prof;

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

	// Bottom Element //
	{
		stringstream ss;
		StrInfo strInfo;

		if (selfHealthPercent && self.alive)
		{
			ss << format("%i") % (int)self.pHealth;

			strInfo = AssistDrawer::StringInfo(ss.str());
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
				ss << format(formatString) % (int)selected.cHealth % (int)selected.mHealth % (int)selected.pHealth % "%%";
				drawElementAt(ss, aTopLeft);

				// Prepare for Next Element
				aTopLeft.y += AssistDrawer::adjustYForNextElementByLines(AssistDrawer::StringInfo(ss.str()).lineCount);
			}
			else if (selected.valid)
			{
				stringstream ss;
				string formatString("Selected: %i / %i [%i%s]");
				ss << format(formatString) % (int)selected.cHealth % (int)selected.mHealth % (int)selected.pHealth % "%%";
				drawElementAt(ss, aTopLeft);

				// Prepare for Next Element
				aTopLeft.y += AssistDrawer::adjustYForNextElementByLines(AssistDrawer::StringInfo(ss.str()).lineCount);
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
				aTopLeft.y += AssistDrawer::adjustYForNextElementByLines(AssistDrawer::StringInfo(ss.str()).lineCount);

				ss.str("");
			}
		}
	}

	// Top Elements (and floaters) //
	{
		if (floatAllyNpc || floatEnemyNpc || floatAllyPlayer || floatEnemyPlayer || floatSiege)
		{
			stringstream ss;
			StrInfo strInfo;

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

			strInfo = AssistDrawer::StringInfo(ss.str());
			float x = round(aTop.x - strInfo.x / 2);
			float y = round(aTop.y);

			AssistDrawer::get().drawBackground(x, y, strInfo);
			AssistDrawer::get().drawFont(x, y, (floatCircles ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

			aTop.y += AssistDrawer::adjustYForNextElementByPos(strInfo.y);

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

				aTop.y += AssistDrawer::adjustYForNextElementByPos(strInfo.y);
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
							if (floatType)
								fs << format("%i") % int(Dist(self.pos, floater.pos));
							else
								fs << format("%i") % floater.mHealth;

							StrInfo fsInfo = AssistDrawer::StringInfo(fs.str());
							AssistDrawer::get().drawFont(x - fsInfo.x / 2, y - 15, AssistDrawer::WHITE, fs.str());

							DWORD color = 0x4433ff00;
							DrawCircleProjected(floater.pos, 20.0f, color);
							DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
						}
					}
				}

				if (floatEnemyNpc && floaters.enemyNpc.size() > 0)
				{
					for (auto & floater : floaters.enemyNpc) {
						if (WorldToScreen(floater.pos, &x, &y))
						{
							stringstream fs;
							if (floatType)
								fs << format("%i") % int(Dist(self.pos, floater.pos));
							else
								fs << format("%i") % floater.mHealth;

							StrInfo fsInfo = AssistDrawer::StringInfo(fs.str());
							AssistDrawer::get().drawFont(x - fsInfo.x / 2, y - 15, AssistDrawer::WHITE, fs.str());

							DWORD color = 0x44ff3300;
							DrawCircleProjected(floater.pos, 20.0f, color);
							DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
						}
					}
				}

				if (floatAllyPlayer && floaters.allyPlayer.size() > 0)
				{
					for (auto & floater : floaters.allyPlayer) {
						if (WorldToScreen(floater.pos, &x, &y))
						{
							stringstream fs;
							if (floatType)
								fs << format("%i") % int(Dist(self.pos, floater.pos));
							else
								fs << format("%i") % floater.mHealth;

							StrInfo fsInfo = AssistDrawer::StringInfo(fs.str());
							AssistDrawer::get().drawFont(x - fsInfo.x / 2, y - 15, AssistDrawer::WHITE, fs.str());

							DWORD color = 0x4433ff00;
							DrawCircleProjected(floater.pos, 20.0f, color);
							DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
						}
					}
				}

				if (floatEnemyPlayer && floaters.enemyPlayer.size() > 0)
				{
					for (auto & floater : floaters.enemyPlayer) {
						if (WorldToScreen(floater.pos, &x, &y))
						{
							stringstream fs;
							if (floatType)
								fs << format("%i") % int(Dist(self.pos, floater.pos));
							else
								fs << format("%i") % floater.mHealth;

							StrInfo fsInfo = AssistDrawer::StringInfo(fs.str());
							AssistDrawer::get().drawFont(x - fsInfo.x / 2, y - 15, AssistDrawer::WHITE, fs.str());

							DWORD color = 0x44ff3300;
							DrawCircleProjected(floater.pos, 20.0f, color);
							DrawCircleFilledProjected(floater.pos, 20.0f, color - 0x30000000);
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

		// World Boss
		if (woldBosses)
		{
			stringstream fs;
			fs << format("Wold Bosses: %i") % wbosses.list.size();

			StrInfo strInfo = AssistDrawer::StringInfo(fs.str());
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

						StrInfo strInfo = AssistDrawer::StringInfo(fs.str());
						fx = round(fx - strInfo.x / 2);
						fy = round(fy - 15);

						AssistDrawer::get().drawBackground(fx, fy, strInfo);
						AssistDrawer::get().drawFont(fx, fy, AssistDrawer::WHITE, fs.str());
					}

					// list
					{
						stringstream fs;
						fs << format("[%i] %i / %i (%i)") % wboss.id % (int)wboss.cHealth % (int)wboss.mHealth % int(Dist(self.pos, wboss.pos));

						StrInfo strInfo = AssistDrawer::StringInfo(fs.str());
						ly = round(ly + strInfo.y + AssistDrawer::PADY);

						AssistDrawer::get().drawBackground(lx, ly, strInfo);
						AssistDrawer::get().drawFont(lx, ly, AssistDrawer::WHITE, fs.str());
					}
				}
			}
		}

		if (logSpeedometer) {
			stringstream ss;
			StrInfo strInfo;

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

			strInfo = AssistDrawer::StringInfo(ss.str());
			float x = round(aTop.x - strInfo.x / 2);
			float y = round(aTop.y);

			AssistDrawer::get().drawBackground(x, y, strInfo);
			AssistDrawer::get().drawFont(x, y, (logSpeedometerEnemy ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

			aTop.y += AssistDrawer::adjustYForNextElementByPos(strInfo.y);
		}

		if (logDisplacement)
		{
			stringstream ss;
			StrInfo strInfo;

			ss << format("Distance: %i") % bufferDisplacement.dist;

			strInfo = AssistDrawer::StringInfo(ss.str());
			float x = round(aTop.x - strInfo.x / 2);
			float y = round(aTop.y);

			AssistDrawer::get().drawBackground(x, y, strInfo);
			AssistDrawer::get().drawFont(x, y, (logDisplacementEnemy ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

			aTop.y += AssistDrawer::adjustYForNextElementByPos(strInfo.y);

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
			StrInfo strInfo;

			float aAdjustX = 120; // adjust anchor -120

			if (!bufferDps.empty())
			{
				averageDps[0] = computeAverage(10, bufferDps);
				averageDps[1] = computeAverage(30, bufferDps);
				averageDps[2] = computeAverage(60, bufferDps);

				if (boss == nullptr) {
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

			strInfo = AssistDrawer::StringInfo(ss.str());
			strInfo.x = aAdjustX; // box min-width with history stream
			float x = round(aTopRight.x - aAdjustX / 2); // perma anchor offset
			float y = round(aTopRight.y);

			AssistDrawer::get().drawBackground(x, y, strInfo);
			AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, ss.str());

			// Prepare for Next Element
			//ss.str("");
			//aTopRight.y += AssistDrawer::adjustYForNextElementByLines(strInfo.lineCount);
			aTopRight.x -= aAdjustX / 2 + AssistDrawer::PADX + 2;
		}

		if (logKillTimer)
		{
			// separate ss vars
			stringstream ss;
			StrInfo strInfo;

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

			strInfo = AssistDrawer::StringInfo(ss.str());
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
			aTopRight.y += AssistDrawer::adjustYForNextElementByLines(strInfo.lineCount);
			//aTopRight.x -= 0;
		}

	}

	// Right Elements //
	{
		if (logAttackRate)
		{
			stringstream ss;
			StrInfo strInfo;

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

			strInfo = AssistDrawer::StringInfo(ss.str());

			float aAdjustX = 120; // adjust anchor -120
			if (strInfo.x < aAdjustX)
				strInfo.x = aAdjustX; // perma box min-width
			float x = round(aRight.x - strInfo.x);
			float y = round(aRight.y);

			AssistDrawer::get().drawBackground(x, y, strInfo);
			AssistDrawer::get().drawFont(x, y, (logAttackRateToFile ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

			// Prepare for Next Element
			//ss.str("");
			//aTopRight.y += AssistDrawer::adjustYForNextElementByLines(strInfo.lineCount);
			aRight.x = AssistDrawer::adjustXForNextElement(x);
		}

		if (logHits)
		{
			stringstream ss;
			StrInfo strInfo;

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

			strInfo = AssistDrawer::StringInfo(ss.str());


			float aAdjustX = 120; // adjust anchor -120
			if (strInfo.x < aAdjustX)
				strInfo.x = aAdjustX; // perma box min-width
			float x = round(aRight.x - strInfo.x);
			float y = round(aRight.y);

			AssistDrawer::get().drawBackground(x, y, strInfo);
			AssistDrawer::get().drawFont(x, y, (logHitsToFile ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

			// Prepare for Next Element
			//ss.str("");
			//aTopRight.y += AssistDrawer::adjustYForNextElementByLines(strInfo.lineCount);
			aRight.x = AssistDrawer::adjustXForNextElement(x);
		}

		if (logCrits)
		{
			stringstream ss;
			StrInfo strInfo;

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


			strInfo = AssistDrawer::StringInfo(ss.str());


			float aAdjustX = 120; // adjust anchor -120
			if (strInfo.x < aAdjustX)
				strInfo.x = aAdjustX; // perma box min-width
			float x = round(aRight.x - strInfo.x);
			float y = round(aRight.y);

			AssistDrawer::get().drawBackground(x, y, strInfo);
			AssistDrawer::get().drawFont(x, y, (logCritsToFile ? AssistDrawer::CYAN : AssistDrawer::WHITE), ss.str());

			// Prepare for Next Element
			//ss.str("");
			//aTopRight.y += AssistDrawer::adjustYForNextElementByLines(strInfo.lineCount);
			aRight.x = AssistDrawer::adjustXForNextElement(x);
		}

		if (raid_debug) {
			displayDebug();
		}

		if (raid_boss_assist) {
			if (boss != nullptr) {
				boss->drawAssistInfo();

				if (logDps) {					
					stringstream ssDps;
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
	ss << format("cameraPos: %f %f %f\n") % GetCameraPosition().x % GetCameraPosition().y % GetCameraPosition().z;
	ss << format("viewVec: %f %f %f\n") % GetViewVector().x % GetViewVector().y % GetViewVector().z;
	ss << format("FOV: %f\n") % GetFieldOfViewY();
	ss << format("mousePos: %f %f %f\n") % GetMouseInWorld().x % GetMouseInWorld().y % GetMouseInWorld().z;

	displayAgent("target", GetLockedSelection(), ss);
	displayAgent("self", GetOwnAgent(), ss);

	if (squad != nullptr) {
		ss << format("raid state: %d\n") % squad->getRaidState();
		ss << format("raid log file: %s\n") % squad->getLogFileName();
		ss << format("squad debugStr %s\n") % squad->getDebugStr();
	}

	if (boss != nullptr) {
		boss->outputDebug(ss);
	}

	drawElementAt(ss, raidDebugAnchor);
}

void displayAgent(string prefix, Agent &agent, stringstream &ss) {
	GW2LIB::Character character = agent.GetCharacter();
	ss << format(prefix + " agentId: %d ptr: %p\n") % character.GetAgent().GetAgentId() % &character.GetAgent();
	ss << format(prefix + " location: x=%f, y=%f, z=%f\n") % agent.GetPos().x % agent.GetPos().y % agent.GetPos().z;
	ss << format(prefix + " type: %d\n") % agent.GetType();
	ss << format(prefix + " gadget: %f / %f\n") % agent.GetGadget().GetCurrentHealth() % agent.GetGadget().GetMaxHealth();
	ss << format(prefix + " rot: %f\n") % agent.GetRot();
	ss << format(prefix + " name: %s\n") % character.GetName();
	ss << format(prefix + " alive: " + string(character.IsAlive() ? "yes" : "no") + "\n");
	ss << format(prefix + " downed: " + string(character.IsDowned() ? "yes" : "no") + "\n");
	ss << format(prefix + " monster: " + string(character.IsMonster() ? "yes" : "no") + "\n");
	ss << format(prefix + " valid: " + string(character.IsValid() ? "yes" : "no") + "\n");
	ss << format(prefix + " controlled: " + string(character.IsControlled() ? "yes" : "no") + "\n");
	ss << format(prefix + " downed: " + string(character.IsDowned() ? "yes" : "no") + "\n");
	ss << format(prefix + " player: " + string(character.IsPlayer() ? "yes" : "no") + "\n");
	ss << format(prefix + " breakbar state: %d\n") % character.GetBreakbarState();
	ss << format(prefix + " breakbar percent: %f\n") % character.GetBreakbarPercent();
	ss << format(prefix + " hp: %f/%f\n") % character.GetCurrentHealth() % character.GetMaxHealth();
	ss << format(prefix + " endurance: %f/%f\n") % character.GetCurrentEndurance() % character.GetMaxEndurance();
	ss << format(prefix + " glide: %d\n") % character.GetGliderPercent();
	GW2LIB::GW2::CharacterStats stats = character.GetStats();
	ss << format(prefix + " pow:%d, prec:%d, fero:%d, cond:%d, tough:%d, vit:%d, heal:%d\n") % stats.power % stats.precision % stats.ferocity % stats.condition % stats.toughness % stats.vitality % stats.healing;
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
	StrInfo strInfo = AssistDrawer::StringInfo(ss.str());
	float x = round(location.x - strInfo.x / 2);
	float y = round(location.y);

	AssistDrawer::get().drawBackground(x, y, strInfo);
	AssistDrawer::get().drawFont(x, y, AssistDrawer::WHITE, ss.str());
}


void load_preferences() {
	expMode = Str2Bool(read_config_value("Preferences.EXP_MODE"));
	selfFloat = Str2Bool(read_config_value("Preferences.SELF_FLOAT"));
	loopLimiter = Str2Bool(read_config_value("Preferences.LOOP_LIMITER"), true);
	selfHealthPercent = Str2Bool(read_config_value("Preferences.SELF_HEALTH_PERCENT"), true);
	woldBosses = Str2Bool(read_config_value("Preferences.WORLD_BOSSES"));
	targetSelected = Str2Bool(read_config_value("Preferences.TARGET_SELECTED"), true);
	targetInfo = Str2Bool(read_config_value("Preferences.TARGET_INFO"));
	targetInfoAlt = Str2Bool(read_config_value("Preferences.TARGET_INFO_ALT"));
	targetLock = Str2Bool(read_config_value("Preferences.TARGET_LOCK"));
	dpsAllowNegative = Str2Bool(read_config_value("Preferences.DPS_ALLOW_NEGATIVE"));
	logDps = Str2Bool(read_config_value("Preferences.LOG_DPS"), true);
	logDpsDetails = Str2Bool(read_config_value("Preferences.LOG_DPS_DETAILS"));
	logKillTimer = Str2Bool(read_config_value("Preferences.LOG_KILL_TIMER"));
	logKillTimerDetails = Str2Bool(read_config_value("Preferences.LOG_KILL_TIMER_DETAILS"));
	logKillTimerToFile = Str2Bool(read_config_value("Preferences.LOG_KILL_TIMER_TO_FILE"));
	logHits = Str2Bool(read_config_value("Preferences.LOG_HITS"));
	logHitsDetails = Str2Bool(read_config_value("Preferences.LOG_HITS_DETAILS"));
	logHitsToFile = Str2Bool(read_config_value("Preferences.LOG_HITS_TO_FILE"));
	logAttackRate = Str2Bool(read_config_value("Preferences.LOG_ATTACK_RATE"));
	logAttackRateDetails = Str2Bool(read_config_value("Preferences.LOG_ATTACK_RATE_DETAILS"));
	logAttackRateToFile = Str2Bool(read_config_value("Preferences.LOG_ATTACK_RATE_TO_FILE"));
	logCrits = Str2Bool(read_config_value("Preferences.LOG_CRITS"));
	alliesList = Str2Bool(read_config_value("Preferences.ALLIES_LIST"));
	floatCircles = Str2Bool(read_config_value("Preferences.FLOAT_CIRCLES"));
	floatType = Str2Bool(read_config_value("Preferences.FLOAT_TYPE"), true);
	floatAllyNpc = Str2Bool(read_config_value("Preferences.FLOAT_ALLY_NPC"));
	floatEnemyNpc = Str2Bool(read_config_value("Preferences.FLOAT_ENEMY_NPC"));
	floatAllyPlayer = Str2Bool(read_config_value("Preferences.FLOAT_ALLY_PLAYER"));
	floatAllyPlayerProf = Str2Bool(read_config_value("Preferences.FLOAT_ALLY_PLAYER_PROF"));
	floatEnemyPlayer = Str2Bool(read_config_value("Preferences.FLOAT_ENEMY_PLAYER"));
	floatSiege = Str2Bool(read_config_value("Preferences.FLOAT_SIEGE"));
	logSpeedometer = Str2Bool(read_config_value("Preferences.LOG_SPEEDOMETER"));
	logSpeedometerEnemy = Str2Bool(read_config_value("Preferences.LOG_SPEEDOMETER_ENEMY"));
	logDisplacement = Str2Bool(read_config_value("Preferences.LOG_DISPLACEMENT"));
	logDisplacementEnemy = Str2Bool(read_config_value("Preferences.LOG_DISPLACEMENT_ENEMY"));
	floatRadius = Str2Int(read_config_value("Preferences.FLOAT_RADIUS"), 7000);
	wvwBonus = Str2Int(read_config_value("Preferences.WVW_BONUS"));
	AttackRateChainHits = Str2Int(read_config_value("Preferences.ATTACKRATE_CHAIN_HITS"), 1);
	logCritsSample = Str2Int(read_config_value("Preferences.LOG_CRITS_SAMPLE"));
}

void save_preferences() {
	write_config_value("Preferences.EXP_MODE", Bool2Str(expMode));
	write_config_value("Preferences.SELF_FLOAT", Bool2Str(selfFloat));
	write_config_value("Preferences.LOOP_LIMITER", Bool2Str(loopLimiter));
	write_config_value("Preferences.SELF_HEALTH_PERCENT", Bool2Str(selfHealthPercent));
	write_config_value("Preferences.WORLD_BOSSES", Bool2Str(woldBosses));
	write_config_value("Preferences.TARGET_SELECTED", Bool2Str(targetSelected));
	write_config_value("Preferences.TARGET_INFO", Bool2Str(targetInfo));
	write_config_value("Preferences.TARGET_INFO_ALT", Bool2Str(targetInfoAlt));
	write_config_value("Preferences.TARGET_LOCK", Bool2Str(targetLock));
	write_config_value("Preferences.DPS_ALLOW_NEGATIVE", Bool2Str(dpsAllowNegative));
	write_config_value("Preferences.LOG_DPS", Bool2Str(logDps));
	write_config_value("Preferences.LOG_DPS_DETAILS", Bool2Str(logDpsDetails));
	write_config_value("Preferences.LOG_KILL_TIMER", Bool2Str(logKillTimer));
	write_config_value("Preferences.LOG_KILL_TIMER_DETAILS", Bool2Str(logKillTimerDetails));
	write_config_value("Preferences.LOG_KILL_TIMER_TO_FILE", Bool2Str(logKillTimerToFile));
	write_config_value("Preferences.LOG_HITS", Bool2Str(logHits));
	write_config_value("Preferences.LOG_HITS_DETAILS", Bool2Str(logHitsDetails));
	write_config_value("Preferences.LOG_HITS_TO_FILE", Bool2Str(logHitsToFile));
	write_config_value("Preferences.LOG_ATTACK_RATE", Bool2Str(logAttackRate));
	write_config_value("Preferences.LOG_ATTACK_RATE_DETAILS", Bool2Str(logAttackRateDetails));
	write_config_value("Preferences.LOG_ATTACK_RATE_TO_FILE", Bool2Str(logAttackRateToFile));
	write_config_value("Preferences.LOG_CRITS", Bool2Str(logCrits));
	write_config_value("Preferences.ALLIES_LIST", Bool2Str(alliesList));
	write_config_value("Preferences.FLOAT_CIRCLES", Bool2Str(floatCircles));
	write_config_value("Preferences.FLOAT_TYPE", Bool2Str(floatType));
	write_config_value("Preferences.FLOAT_ALLY_NPC", Bool2Str(floatAllyNpc));
	write_config_value("Preferences.FLOAT_ENEMY_NPC", Bool2Str(floatEnemyNpc));
	write_config_value("Preferences.FLOAT_ALLY_PLAYER", Bool2Str(floatAllyPlayer));
	write_config_value("Preferences.FLOAT_ALLY_PLAYER_PROF", Bool2Str(floatAllyPlayerProf));
	write_config_value("Preferences.FLOAT_ENEMY_PLAYER", Bool2Str(floatEnemyPlayer));
	write_config_value("Preferences.FLOAT_SIEGE", Bool2Str(floatSiege));
	write_config_value("Preferences.LOG_SPEEDOMETER", Bool2Str(logSpeedometer));
	write_config_value("Preferences.LOG_SPEEDOMETER_ENEMY", Bool2Str(logSpeedometerEnemy));
	write_config_value("Preferences.LOG_DISPLACEMENT", Bool2Str(logDisplacement));
	write_config_value("Preferences.LOG_DISPLACEMENT_ENEMY", Bool2Str(logDisplacementEnemy));
	write_config_value("Preferences.FLOAT_RADIUS", Int2Str(floatRadius));
	write_config_value("Preferences.WVW_BONUS", Int2Str(wvwBonus));
	write_config_value("Preferences.ATTACKRATE_CHAIN_HITS", Int2Str(AttackRateChainHits));
	write_config_value("Preferences.LOG_CRITS_SAMPLE", Int2Str(logCritsSample));
	save_config();
}


void chat_log(wchar_t *wtxt) {
    size_t len = wcslen(wtxt) + 1;

    char *txt = new char[len];
    memset(txt, 0, len);
    wcstombs(txt, wtxt, len);

    //font.Draw(10, 10, 0xffffffff, "%s", txt);

    delete txt;
}


void GW2LIB::gw2lib_main()
{
	locale::global(locale("en-US"));

	init_keymap();
	init_config();
	load_preferences();

	EnableEsp(ESP);
    SetGameHook(ChatHook, chat_log);

	thread t1(&threadHotKeys);
	thread t2(&threadDps);
	thread t3(&threadKillTimer);
	thread t4(&threadHits);
	thread t5(&threadAttackRate);
	thread t6(&threadCrits);
	thread t7(&threadSpeedometer);
	thread t8(&threadRaidAssist);
	thread t9(&threadSquadSpeedometer);

	// wait for exit hotkey
	while (GetAsyncKeyState(VK_F12) >= 0)
		Sleep(1);

	close_config();

	// make threads clean-up first before interrupting
	PostThreadMessage(thread_id_hotkey, WM_USER, NULL, NULL);

	// self destruct sequence
	t1.interrupt(); //t1.join();
	t2.interrupt(); //t2.join();
	t3.interrupt(); //t3.join();
	t4.interrupt(); //t4.join();
	t5.interrupt(); //t5.join();
	t6.interrupt(); //t6.join();
	t7.interrupt(); //t7.join();
	t8.interrupt();
	t9.interrupt();

	Sleep(1000);
	return;
}
