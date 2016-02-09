# gw2dps Raid Assist
Raid Assist is a tool to help players perform better at GW2 raids. It is based on @frifox's [gw2dps](https://github.com/frifox/gw2dps/releases). That, in turn, is based on rafi's [gw2lib](http://www.gamerevision.com/showthread.php?3691-Gw2lib&p=45709). See frifox's [project page](https://github.com/frifox/gw2dps) for initial usage/build instructions.

# How to Use
- Be inside a raid instance with all squad members
- Target the raid boss and activate Raid Assist (default: Alt-B)
- After an attempt ends (boss dies or squad wipes), stats will be output to your Gw2.exe folder. Raid Assist will automatically turn off.

# Features
- Intended for longer encounters, so DPS is shown over a longer (10s, 30s, and 60s) period of time.
- Tracks and displays internal state of players and bosses, which is intended to help learning and/or performing better at raids. The output is customized to be relevant for each boss, eg:
 - Vale guardian: cooldown for Magic Storm attack
 - Sabetha: compass and cannon timer
- Outputs stats for each raid attempt, which is intended to help players understand what went wrong, where/when errors occurred, and possible ways to improve.
- Player output columns:
 - DodgeSpeedAttainedCount: The number of times the player's movement speed suggested they were dodging. Clients are not sent endurance values of other agents, so dodges can't be directly counted (except for yourself). This is an attempted *estimate* at the number of times a player has dodged. As such, this number can both overestimate (eg: Superspeed) and underestimate (eg: dodging into a horizontal barrier).
 - HitsTaken: The number of times a player took a **heavy** hit. A heavy hit is any single amount of damage above a specific boss-dependent threshold. Usually, (at the time of this writing) damage values above this threshold can be avoided. Values lower than this is generally unavoidable periodic damage. The intention is to show how many avoidable hits each player takes.
 - TotalDamageTaken: The total damage a player has taken in a single raid attempt.

