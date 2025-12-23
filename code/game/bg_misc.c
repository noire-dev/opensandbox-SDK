// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/js_local.h"

// clang-format off
item_t gameInfoItems[] = {
	{ NULL, 						NULL, 											NULL, 						NULL, 					0, 		IT_NULL, 		0 },
	{ "item_armor_shard", 			"models/powerups/armor/shard.md3", 				"icons/iconr_shard", 		"Armor Shard", 			5, 		IT_ARMOR, 		0 },
	{ "item_armor_combat", 			"models/powerups/armor/armor_yel.md3", 			"icons/iconr_yellow", 		"Armor 50", 			50, 	IT_ARMOR, 		0 },
	{ "item_armor_body", 			"models/powerups/armor/armor_red.md3",			"icons/iconr_red",			"Heavy Armor",			100,	IT_ARMOR,		0 },
	{ "item_health_small",			"models/powerups/health/small_cross.md3", 		"icons/iconh_green",		"Health 5",				5,		IT_HEALTH,		0 },
	{ "item_health",				"models/powerups/health/medium_cross.md3",		"icons/iconh_yellow",		"Health 25",			25,		IT_HEALTH,		0 },
	{ "item_health_large",			"models/powerups/health/large_cross.md3",		"icons/iconh_red",			"Health 50",			50,		IT_HEALTH,		0 },
	{ "item_health_mega",			"models/powerups/health/mega_cross.md3",		"icons/iconh_mega",			"Mega Health",			100,	IT_HEALTH,		0 },
	{ "weapon_gauntlet",			NULL,											"icons/iconw_gauntlet",		"Melee",				0,		IT_WEAPON,		WP_GAUNTLET },
	{ "weapon_machinegun",			"models/weapons2/machinegun/machinegun.md3",	"icons/iconw_machinegun",	"Machinegun",			40,		IT_WEAPON,		WP_MACHINEGUN },
	{ "weapon_shotgun",				"models/weapons2/shotgun/shotgun.md3",			"icons/iconw_shotgun",		"Shotgun",				10,		IT_WEAPON,		WP_SHOTGUN },
	{ "weapon_grenadelauncher",		"models/weapons2/grenadel/grenadel.md3",		"icons/iconw_grenade",		"Grenade Launcher",		10,		IT_WEAPON,		WP_GRENADE_LAUNCHER },
	{ "weapon_rocketlauncher",		"models/weapons2/rocketl/rocketl.md3",			"icons/iconw_rocket",		"Rocket Launcher",		10,		IT_WEAPON,		WP_ROCKET_LAUNCHER },
	{ "weapon_lightning",			"models/weapons2/lightning/lightning.md3",		"icons/iconw_lightning",	"Lightning Gun",		100,	IT_WEAPON,		WP_LIGHTNING },
	{ "weapon_railgun",				"models/weapons2/railgun/railgun.md3",			"icons/iconw_railgun",		"Railgun",				10,		IT_WEAPON,		WP_RAILGUN },
	{ "weapon_plasmagun",			"models/weapons2/plasma/plasma.md3",			"icons/iconw_plasma",		"Plasma Gun",			50,		IT_WEAPON,		WP_PLASMAGUN },
	{ "weapon_bfg",					"models/weapons2/bfg/bfg.md3",					"icons/iconw_bfg",			"BFG10K",				20,		IT_WEAPON,		WP_BFG },
	{ "weapon_grapplinghook",		"models/weapons2/grapple/grapple.md3",			"icons/iconw_grapple",		"Grappling Hook",		0,		IT_WEAPON,		WP_GRAPPLING_HOOK },
	{ "weapon_nailgun",				"models/weapons/nailgun/nailgun.md3",			"icons/iconw_nailgun",		"Nailgun",				10,		IT_WEAPON,		WP_NAILGUN },
	{ "weapon_prox_launcher",		"models/weapons/proxmine/proxmine.md3",			"icons/iconw_proxlauncher",	"Prox Launcher",		5,		IT_WEAPON,		WP_PROX_LAUNCHER },
	{ "weapon_chaingun",			"models/weapons/vulcan/vulcan.md3",				"icons/iconw_chaingun",		"Chaingun",				80,		IT_WEAPON,		WP_CHAINGUN },
	{ "weapon_flamethrower",		"models/weapons2/flamethrower/rocketl.md3",		"icons/iconw_flamethrower",	"Flamethrower",			100,	IT_WEAPON,		WP_FLAMETHROWER },
	{ "weapon_antimatter",			"models/weapons2/antimatter/plasma.md3",		"icons/iconw_voodoo",		"Dark Flare",			100,	IT_WEAPON,		WP_ANTIMATTER },
	{ "weapon_physgun", 			"models/weapons/physgun/physgun.md3",			"icons/iconw_physgun",		"Physgun",				0,		IT_WEAPON,		WP_PHYSGUN },
	{ "weapon_gravitygun", 			"models/weapons/physgun/physgun.md3",			"icons/iconw_gravitygun",	"Gravitygun",			0,		IT_WEAPON,		WP_GRAVITYGUN },
	{ "weapon_toolgun", 			"models/weapons/toolgun/toolgun.md3",			"icons/iconw_toolgun",		"Toolgun",				0,		IT_WEAPON,		WP_TOOLGUN },
	{ "weapon_thrower",				"models/weapons3/machinegun/machinegun.md3",	"icons/iconw_thrower",		"Thrower",				100,	IT_WEAPON,		WP_THROWER },
	{ "weapon_bouncer",				"models/weapons3/shotgun/shotgun.md3",			"icons/iconw_bouncer",		"Bouncer",				15,		IT_WEAPON,		WP_BOUNCER },
	{ "weapon_thunder",				"models/weapons3/grenadel/grenadel.md3",		"icons/iconw_thunder",		"Thunder",				10,		IT_WEAPON,		WP_THUNDER },
	{ "weapon_exploder",			"models/weapons3/rocketl/rocketl.md3",			"icons/iconw_exploder",		"Exploder",				10,		IT_WEAPON,		WP_EXPLODER },
	{ "weapon_knocker",				"models/weapons3/lightning/lightning.md3",		"icons/iconw_knocker",		"Knocker",				25,		IT_WEAPON,		WP_KNOCKER },
	{ "weapon_propgun",				"models/weapons3/railgun/railgun.md3",			"icons/iconw_propgun",		"Propgun",				15,		IT_WEAPON,		WP_PROPGUN },
	{ "weapon_regenerator",			"models/weapons3/plasma/plasma.md3",			"icons/iconw_regenerator",	"Regenerator",			15,		IT_WEAPON,		WP_REGENERATOR },
	{ "weapon_nuke",				"models/weapons3/bfg/bfg.md3",					"icons/iconw_nuke",			"Nuke",					2,		IT_WEAPON,		WP_NUKE },
	{ "ammo_bullets",				"models/powerups/ammo/machinegunam.md3",		"icons/icona_machinegun",	"Bullets",				50,		IT_AMMO,		WP_MACHINEGUN },
	{ "ammo_shells",				"models/powerups/ammo/shotgunam.md3",			"icons/icona_shotgun",		"Shells",				10,		IT_AMMO,		WP_SHOTGUN },
	{ "ammo_grenades",				"models/powerups/ammo/grenadeam.md3",			"icons/icona_grenade",		"Grenades",				5,		IT_AMMO,		WP_GRENADE_LAUNCHER },
	{ "ammo_cells",					"models/powerups/ammo/plasmaam.md3",			"icons/icona_plasma",		"Cells",				30,		IT_AMMO,		WP_PLASMAGUN },
	{ "ammo_lightning",				"models/powerups/ammo/lightningam.md3",			"icons/icona_lightning",	"Lightning",			60,		IT_AMMO,		WP_LIGHTNING },
	{ "ammo_rockets",				"models/powerups/ammo/rocketam.md3",			"icons/icona_rocket",		"Rockets",				5,		IT_AMMO,		WP_ROCKET_LAUNCHER },
	{ "ammo_slugs",					"models/powerups/ammo/railgunam.md3",			"icons/icona_railgun",		"Slugs",				10,		IT_AMMO,		WP_RAILGUN },
	{ "ammo_bfg",					"models/powerups/ammo/bfgam.md3",				"icons/icona_bfg",			"Bfg Ammo",				15,		IT_AMMO,		WP_BFG },
	{ "ammo_nails",					"models/powerups/ammo/nailgunam.md3",			"icons/icona_nailgun",		"Nails",				20,		IT_AMMO,		WP_NAILGUN },
	{ "ammo_mines",					"models/powerups/ammo/proxmineam.md3",			"icons/icona_proxlauncher",	"Proximity Mines",		10,		IT_AMMO,		WP_PROX_LAUNCHER },
	{ "ammo_belt",					"models/powerups/ammo/chaingunam.md3",			"icons/icona_chaingun",		"Chaingun Belt",		100,	IT_AMMO,		WP_CHAINGUN },
	{ "holdable_teleporter",		"models/powerups/holdable/teleporter.md3",		"icons/teleporter",			"Personal Teleporter",	1,		IT_HOLDABLE,	HI_TELEPORTER },
	{ "holdable_medkit", 			"models/powerups/holdable/medkit.md3",			"icons/medkit",				"Medkit", 				1,		IT_HOLDABLE,	HI_MEDKIT },
	{ "holdable_kamikaze", 			"models/powerups/kamikazi.md3",					"icons/kamikaze",			"Kamikaze",				1,		IT_HOLDABLE,	HI_KAMIKAZE },
	{ "holdable_portal", 			"models/powerups/holdable/porter.md3",			"icons/portal",				"Portal",				1,		IT_HOLDABLE,	HI_PORTAL },
	{ "holdable_invulnerability", 	"models/powerups/holdable/invulnerability.md3",	"icons/invulnerability",	"Invulnerability",		1,		IT_HOLDABLE,	HI_INVULNERABILITY },
	{ "item_quad",					"models/powerups/instant/quad.md3",				"icons/quad",				"Quad Damage",			30,		IT_POWERUP,		PW_QUAD },
	{ "item_enviro",				"models/powerups/instant/enviro.md3",			"icons/envirosuit",			"Battle Suit",			30,		IT_POWERUP,		PW_BATTLESUIT },
	{ "item_haste",					"models/powerups/instant/haste.md3",			"icons/haste",				"Speed",				30,		IT_POWERUP,		PW_HASTE },
	{ "item_invis",					"models/powerups/instant/invis.md3",			"icons/invis",				"Invisibility",			30,		IT_POWERUP,		PW_INVIS },
	{ "item_regen",					"models/powerups/instant/regen.md3",			"icons/regen",				"Regeneration",			30,		IT_POWERUP,		PW_REGEN },
	{ "item_flight",				"models/powerups/instant/flight.md3",			"icons/flight",				"Flight",				60,		IT_POWERUP,		PW_FLIGHT },
	{ "item_scout",					"models/powerups/scout.md3",					"icons/scout",				"Scout",				0,		IT_RUNE,		PW_SCOUT },
	{ "item_guard",					"models/powerups/guard.md3",					"icons/guard",				"Guard",				0,		IT_RUNE,		PW_GUARD },
	{ "item_doubler",				"models/powerups/doubler.md3",					"icons/doubler",			"Doubler",				0,		IT_RUNE,		PW_DOUBLER },
	{ "item_ammoregen",				"models/powerups/ammo.md3",						"icons/ammo_regen",			"Ammo Regen",			0,		IT_RUNE,		PW_AMMOREGEN },
	{ "team_CTF_redflag",			"models/flags/r_flag.md3",						"icons/iconf_red1",			"Red Flag",				0,		IT_TEAM,		PW_REDFLAG },
	{ "team_CTF_blueflag",			"models/flags/b_flag.md3",						"icons/iconf_blu1",			"Blue Flag",			0,		IT_TEAM,		PW_BLUEFLAG },
	{ "team_CTF_neutralflag",		"models/flags/n_flag.md3",						"icons/iconf_neutral1",		"Neutral Flag",			0,		IT_TEAM,		PW_NEUTRALFLAG },
	{ "item_redcube",				"models/powerups/orb/r_orb.md3",				"icons/iconh_rorb",			"Red Cube",				0,		IT_TEAM,		0 },
	{ "item_bluecube",				"models/powerups/orb/b_orb.md3",				"icons/iconh_borb",			"Blue Cube",			0,		IT_TEAM,		0 },
};
// clang-format on

int gameInfoItemsNum = ARRAY_SIZE(gameInfoItems);

// clang-format off
weaponProperties_t gameInfoWeapons[] = {
// 	wType 			mType 			mEffect 				classname 		delay 	count 	dmg 	sDmg 	sRadius range 	mod 					speed	sRandom spread	timeout guided	gravity	bounce	bMod 
	{ WT_NONE, 		MT_NONE, 		WP_NONE, 				NULL, 			0, 		0,		0, 		0, 		0,		0,		MOD_UNKNOWN, 			0, 		0, 		0, 		0, 		qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_MELEE, 	MT_NONE, 		WP_GAUNTLET, 			NULL, 			400,	1, 		50, 	0, 		0,		64,		WP_GAUNTLET, 			0, 		0, 		0, 		0, 		qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_BULLET, 	MT_NONE, 		WP_MACHINEGUN, 			NULL, 			100, 	1, 		7, 		0, 		0, 		131072, WP_MACHINEGUN, 			0, 		0, 		200, 	0, 		qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_SHOTGUN, 	MT_NONE, 		WP_SHOTGUN, 			NULL, 			1000, 	11, 	10, 	0, 		0,		8192,	WP_SHOTGUN, 			0, 		0, 		700, 	0, 		qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_GRENADE_LAUNCHER, 	"grenade", 		800, 	1, 		100, 	100, 	150,	0,		WP_GRENADE_LAUNCHER, 	700, 	0, 		0, 		2500, 	qfalse,	qtrue,	qtrue,	0.65 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_ROCKET_LAUNCHER, 	"rocket", 		800, 	1, 		100, 	100, 	120,	0,		WP_ROCKET_LAUNCHER, 	900, 	0, 		0, 		15000,	qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_LIGHTNING, MT_NONE, 		WP_LIGHTNING, 			NULL, 			50, 	1, 		8, 		0, 		0,		768,	WP_LIGHTNING, 			0, 		0, 		0, 		0, 		qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_RAILGUN, 	MT_NONE, 		WP_RAILGUN, 			NULL, 			1500, 	1, 		100, 	0, 		0,		16384,	WP_RAILGUN, 			0, 		0, 		0, 		0, 		qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_PLASMAGUN, 			"plasma", 		100, 	1, 		20, 	15, 	20,		0,		WP_PLASMAGUN, 			2000, 	0, 		0, 		10000, 	qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_BFG, 				"bfg", 			200, 	1, 		100, 	100, 	120,	0,		WP_BFG, 				2000, 	0, 		0, 		10000, 	qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_MISSILE, 	MT_HOOK, 		WP_GRAPPLING_HOOK, 		"hook", 		400, 	1, 		0, 		0, 		0,		0,		WP_GRAPPLING_HOOK, 		800, 	0, 		0, 		30000, 	qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_NAILGUN, 			"nail", 		1000, 	16, 	20, 	0, 		0,		0,		WP_NAILGUN, 			555, 	1800, 	500, 	10000, 	qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_MISSILE, 	MT_MINE, 		WP_PROX_LAUNCHER, 		"mine", 		800, 	1, 		0, 		100, 	150,	0,		WP_PROX_LAUNCHER, 		700, 	0, 		0, 		3000, 	qfalse,	qtrue,	qfalse,	0.0 },
	{ WT_BULLET, 	MT_GENERAL, 	WP_CHAINGUN, 			NULL, 			30, 	1, 		7, 		0, 		0,		131072,	WP_CHAINGUN, 			0, 		0, 		600, 	0, 		qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_FLAMETHROWER, 		"flame", 		40, 	1, 		20, 	15, 	30,		0,		WP_FLAMETHROWER, 		750, 	0, 		0, 		1500, 	qfalse,	qfalse,	qtrue, 	0.65 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_ANTIMATTER, 			"flare", 		40, 	1, 		6, 		6, 		20,		0,		WP_ANTIMATTER, 			1500, 	0, 		0, 		3500, 	qfalse,	qtrue,	qtrue, 	0.65 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_NAILGUN, 			"missile", 		150, 	1, 		25, 	0, 		0,		0,		WP_THROWER, 			3500, 	100, 	50, 	5000, 	qfalse,	qtrue,	qtrue,	0.65 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_NAILGUN, 			"missile", 		1000, 	11, 	16, 	0, 		0,		0,		WP_BOUNCER, 			2000, 	150, 	1000, 	5000, 	qfalse,	qtrue,	qtrue,	0.65 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_GRENADE_LAUNCHER, 	"missile", 		1000, 	6, 		100, 	100, 	150,	0,		WP_THUNDER, 			700, 	0, 		3000, 	2500, 	qfalse,	qtrue,	qtrue,	0.65 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_ROCKET_LAUNCHER, 	"missile", 		1600, 	1, 		150, 	150, 	300,	0,		WP_EXPLODER, 			200, 	0, 		0, 		30000, 	qtrue,	qfalse,	qfalse,	0.0 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_ANTIMATTER, 			"missile", 		500, 	1, 		2, 		0, 		0,		0,		WP_KNOCKER, 			3500, 	0, 		0, 		5000, 	qfalse,	qtrue,	qtrue,	0.65 },
	{ WT_MISSILE, 	MT_PROPGUN, 	WP_NONE, 				"sandbox_prop", 1000, 	1, 		0, 		0, 		0,		0,		WP_PROPGUN, 			2000, 	0, 		100, 	0, 		qfalse,	qfalse,	qfalse,	0.65 },
	{ WT_MISSILE, 	MT_GENERAL, 	WP_PLASMAGUN, 			"missile", 		1000, 	1, 		2, 		2, 		80,		0,		WP_REGENERATOR, 		1800, 	0, 		0, 		10000, 	qfalse,	qtrue,	qfalse,	0.0 },
	{ WT_MISSILE, 	MT_NUKE, 		WP_NONE, 				"hihihiha", 	3000, 	8, 		0, 		0, 		0,		0,		WP_NUKE, 				3000, 	0, 		777, 	0, 		qfalse,	qfalse,	qfalse,	0.65 },
	{ WT_EMPTY, 	MT_NONE, 		WP_PHYSGUN, 			NULL, 			100, 	1, 		0, 		0, 		0,		8192,	WP_PHYSGUN, 			0, 		0, 		0, 		0, 		qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_EMPTY, 	MT_NONE, 		WP_GRAVITYGUN, 			NULL, 			100, 	1, 		0, 		0, 		0,		1024,	WP_GRAVITYGUN, 			0, 		0, 		0, 		0, 		qfalse,	qfalse,	qfalse,	0.0 },
	{ WT_TOOLGUN, 	MT_NONE, 		WP_TOOLGUN, 			NULL, 			200, 	1, 		0, 		0, 		0,		4096,	WP_TOOLGUN, 			0, 		0, 		0, 		0, 		qfalse,	qfalse,	qfalse,	0.0 },
};
// clang-format on

int gameInfoWeaponsNum = ARRAY_SIZE(gameInfoWeapons);

// clang-format off
wPropProperties_t gameInfoWProps[] = {
// 	oType 				modelname 								mtMin 		mtMax 		health 	gravity 	scale 		colSize		solid
	{ OT_VANILLAQ3, 	"none", 								0, 			0, 			0, 		0.00,		0.00, 		0.00, 		qfalse }, //MT_NONE
	{ OT_VANILLAQ3, 	"none", 								0, 			0, 			0, 		0.00,		0.00, 		0.00, 		qfalse }, //MT_GENERAL
	{ OT_VANILLAQ3, 	"none", 								0, 			0, 			0, 		0.00,		0.00, 		0.00, 		qfalse }, //MT_HOOK
	{ OT_VANILLAQ3, 	"none", 								0, 			0, 			0, 		0.00,		0.00, 		0.00, 		qfalse }, //MT_MINE
	{ OT_VANILLAQ3, 	"none", 								0, 			0, 			0, 		0.00,		0.00, 		0.00, 		qfalse }, //MT_PROPS

	{ OT_BASIC, 		"props/cube", 							1, 			15, 		-1, 	1.00,		0.50, 		25, 		qtrue }, //MT_PROPGUN
	{ OT_NUKE, 			"models/ammo/rocket/rocket", 			0, 			0, 			1, 		1.00,		4.00, 		25, 		qfalse }, //MT_NUKE
};
// clang-format on

int gameInfoWPropsNum = ARRAY_SIZE(gameInfoWProps);

// clang-format off
NPCTypes_t gameInfoNPCTypes[] = {
//	name				faction				gravity		damage		speed		drop		push		frFire		pickup		chat
	{ "none",			FRAC_NONE,			1.00,		1.00,		1.00,		qfalse,		qfalse,		qtrue,		qtrue,		qtrue }, //NT_NONE
	{ "player",			FRAC_PLAYER,		1.00,		1.00,		1.00,		qtrue,		qfalse,		qtrue,		qtrue,		qtrue }, //NT_PLAYER
	{ "enemy",			FRAC_ENEMY,			1.00,		1.00,		1.00,		qfalse,		qfalse,		qfalse,		qfalse,		qfalse }, //NT_ENEMY
	{ "citizen",		FRAC_CITIZEN,		1.00,		1.00,		1.00,		qfalse,		qfalse,		qfalse,		qfalse,		qtrue }, //NT_CITIZEN
	{ "guard",			FRAC_GUARD,			1.00,		1.00,		1.00,		qfalse,		qfalse,		qfalse,		qfalse,		qfalse }, //NT_GUARD
	{ "partner",		FRAC_PARTNER,		1.00,		1.00,		1.00,		qtrue,		qfalse,		qfalse,		qtrue,		qtrue }, //NT_PARTNER
	{ "nextbot",		FRAC_NEXTBOT,		1.00,		5.00,		1.80,		qfalse,		qtrue,		qfalse,		qfalse,		qfalse }, //NT_NEXTBOT
};
// clang-format on

int gameInfoNPCTypesNum = ARRAY_SIZE(gameInfoNPCTypes);

// clang-format off
NPCFactions_t gameInfoFactions[] = {
//	faction				attackMask																				
	{ FRAC_NONE,		FRAC_NONE,																											}, //NT_NONE
	{ FRAC_PLAYER,		0xFFFFFFFF,																											}, //NT_PLAYER
	{ FRAC_ENEMY,		FRAC_PLAYER|FRAC_CITIZEN|FRAC_GUARD|FRAC_PARTNER,																	}, //NT_ENEMY
	{ FRAC_CITIZEN,		FRAC_NONE,																											}, //NT_CITIZEN
	{ FRAC_GUARD,		FRAC_ENEMY,																											}, //NT_GUARD
	{ FRAC_PARTNER,		FRAC_ENEMY,																											}, //NT_PARTNER
	{ FRAC_NEXTBOT,		FRAC_PLAYER|FRAC_ENEMY|FRAC_CITIZEN|FRAC_GUARD|FRAC_PARTNER,														}, //NT_NEXTBOT
};
// clang-format on

int gameInfoFactionsNum = ARRAY_SIZE(gameInfoFactions);

// clang-format off
char* gameInfoSandboxSpawns[] = {
	"none",
	"weapon_machinegun",
	"weapon_shotgun",
	"weapon_grenadelauncher",
	"weapon_rocketlauncher",
	"weapon_lightning",
	"weapon_railgun",
	"weapon_plasmagun",
	"weapon_bfg",
	"weapon_grapplinghook",
	"weapon_nailgun",
	"weapon_prox_launcher",
	"weapon_chaingun",
	"weapon_flamethrower",
	"weapon_antimatter",
	"weapon_physgun",
	"weapon_gravitygun",
	"weapon_toolgun",
	"weapon_thrower",
	"weapon_bouncer",
	"weapon_thunder",
	"weapon_exploder",
	"weapon_knocker",
	"weapon_propgun",
	"weapon_regenerator",
	"weapon_nuke",
	"ammo_bullets",
	"ammo_shells",
	"ammo_grenades",
	"ammo_cells",
	"ammo_lightning",
	"ammo_rockets",
	"ammo_slugs",
	"ammo_bfg",
	"ammo_nails",
	"ammo_mines",
	"ammo_belt",
	"item_armor_shard",
	"item_armor_combat",
	"item_armor_body",
	"item_health_small",
	"item_health",
	"item_health_large",
	"item_health_mega",
	"item_quad",
	"item_enviro",
	"item_haste",
	"item_invis",
	"item_regen",
	"item_flight",
	"item_scout",
	"item_doubler",
	"item_ammoregen",
	"item_guard",
	"holdable_teleporter",
	"holdable_medkit",
	"holdable_kamikaze",
	"holdable_invulnerability",
	"holdable_portal",

	"sb.shooter",
};
// clang-format on

int gameInfoSandboxSpawnsNum = ARRAY_SIZE(gameInfoSandboxSpawns);

// clang-format off
char* gametypes_names[] = {
	"Sandbox",
	"Free for All",
	"Team Deathmatch",
	"Capture the Flag",
	"One Flag Capture",
	"Overload",
	"Harvester",
};
// clang-format on

// clang-format off
char* gametypes_mapnames[] = {
	"sandbox",
	"ffa",
	"team",
	"ctf",
	"oneflag",
	"overload",
	"harvester",
};
// clang-format on

int BG_FindNPCTypeID(const char *name) {
	int i;

	for(i = 0; i < gameInfoNPCTypesNum; i++) {
		if(!Q_stricmp(gameInfoNPCTypes[i].name, name)) {
			if(i <= NT_PLAYER) return NT_CITIZEN;
			return i;
		}
	}
	return NT_CITIZEN;
}

qboolean BG_FactionShouldAttack(int attackerFaction, int targetFaction) {
	int i;

	for(i = 0; i < gameInfoFactionsNum; i++) {
		if(gameInfoFactions[i].faction == attackerFaction) {
			if(gameInfoFactions[i].attackMask & targetFaction) return qtrue;
			break;
		}
	}
	return qfalse;
}

item_t *BG_FindItemForPowerup(powerup_t pw) {
	int i;

	for(i = 0; i < gameInfoItemsNum; i++) {
		if((gameInfoItems[i].giType == IT_POWERUP || gameInfoItems[i].giType == IT_TEAM || gameInfoItems[i].giType == IT_RUNE) && gameInfoItems[i].giTag == pw) {
			return &gameInfoItems[i];
		}
	}

	return NULL;
}

item_t *BG_FindItemForHoldable(holdable_t pw) {
	int i;

	for(i = 0; i < gameInfoItemsNum; i++) {
		if(gameInfoItems[i].giType == IT_HOLDABLE && gameInfoItems[i].giTag == pw) {
			return &gameInfoItems[i];
		}
	}

	return NULL;
}

item_t *BG_FindItemForWeapon(weapon_t weapon) {
	item_t *it;

	for(it = gameInfoItems + 1; it->classname; it++) {
		if(it->giType == IT_WEAPON && it->giTag == weapon) {
			return it;
		}
	}

	return NULL;
}

item_t *BG_FindItem(const char *pickupName) {
	item_t *it;

	for(it = gameInfoItems + 1; it->classname; it++) {
		if(!Q_stricmp(it->pickup_name, pickupName)) return it;
	}

	return NULL;
}

qboolean BG_CheckClassname(const char *classname) {
	item_t *it;

	for(it = gameInfoItems + 1; it->classname; it++) {
		if(!Q_stricmp(it->classname, classname)) return qtrue;
	}

	return qfalse;
}

item_t *BG_FindWeapon(int id) {
	item_t *it;

	for(it = gameInfoItems + 1; it->classname; it++) {
		if(it->giType == IT_WEAPON && it->giTag == id) {
			return it;
		}
	}

	return NULL;
}

item_t *BG_FindAmmo(int id) {
	item_t *it;

	for(it = gameInfoItems + 1; it->classname; it++) {
		if(it->giType == IT_AMMO && it->giTag == id) {
			return it;
		}
	}

	return NULL;
}

/*
============
BG_PlayerTouchesItem

Items can be picked up without actually touching their physical bounds to make
grabbing them easier
============
*/
qboolean BG_PlayerTouchesItem(playerState_t *ps, entityState_t *item, int atTime) {
	vec3_t origin;

	BG_EvaluateTrajectory(&item->pos, atTime, origin);

	// we are ignoring ducked differences here
	if(ps->origin[0] - origin[0] > 44 || ps->origin[0] - origin[0] < -50 || ps->origin[1] - origin[1] > 36 || ps->origin[1] - origin[1] < -36 || ps->origin[2] - origin[2] > 36 || ps->origin[2] - origin[2] < -36) {
		return qfalse;
	}

	return qtrue;
}

/*
================
BG_CanItemBeGrabbed

Returns false if the item should not be picked up.
This needs to be the same for client side prediction and server use.
================
*/
qboolean BG_CanItemBeGrabbed(int gametype, const entityState_t *ent, const playerState_t *ps) {
	item_t *item;
	int upperBound;

	if(ent->modelindex < 1 || ent->modelindex >= gameInfoItemsNum) return qfalse;

	item = &gameInfoItems[ent->modelindex];

	switch(item->giType) {
	case IT_WEAPON: return qtrue; // weapons are always picked up

	case IT_AMMO:
		if(ps->stats[STAT_AMMO] >= 9000) {
			return qfalse; // can't hold any more
		}
		return qtrue;

	case IT_ARMOR:
		if(gameInfoItems[ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT) {
			return qfalse;
		}

		if(gameInfoItems[ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD) {
			upperBound = ps->stats[STAT_MAX_HEALTH];
		} else {
			upperBound = ps->stats[STAT_MAX_HEALTH] * 2;
		}

		if(ps->stats[STAT_ARMOR] >= upperBound) {
			return qfalse;
		}
		return qtrue;

	case IT_HEALTH:
		if(gameInfoItems[ps->stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD) {
			upperBound = ps->stats[STAT_MAX_HEALTH];
		} else if(item->quantity == 5 || item->quantity == 100) {
			if(ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] * 2) {
				return qfalse;
			}
			return qtrue;
		}

		if(ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH]) {
			return qfalse;
		}
		return qtrue;

	case IT_POWERUP: return qtrue; // powerups are always picked up

	case IT_RUNE:
		// can only hold one item at a time
		if(ps->stats[STAT_PERSISTANT_POWERUP]) {
			return qfalse;
		}

		// check team only
		if((ent->generic1 & 2) && (ps->persistant[PERS_TEAM] != TEAM_RED)) {
			return qfalse;
		}
		if((ent->generic1 & 4) && (ps->persistant[PERS_TEAM] != TEAM_BLUE)) {
			return qfalse;
		}

		return qtrue;

	case IT_TEAM: // team items, such as flags
		if(gametype == GT_1FCTF) {
			// neutral flag can always be picked up
			if(item->giTag == PW_NEUTRALFLAG) {
				return qtrue;
			}
			if(ps->persistant[PERS_TEAM] == TEAM_RED) {
				if(item->giTag == PW_BLUEFLAG && ps->powerups[PW_NEUTRALFLAG]) {
					return qtrue;
				}
			} else if(ps->persistant[PERS_TEAM] == TEAM_BLUE) {
				if(item->giTag == PW_REDFLAG && ps->powerups[PW_NEUTRALFLAG]) {
					return qtrue;
				}
			}
		}
		if(gametype == GT_CTF) {
			if(ps->persistant[PERS_TEAM] == TEAM_RED) {
				if(item->giTag == PW_BLUEFLAG || (item->giTag == PW_REDFLAG && ent->modelindex2) || (item->giTag == PW_REDFLAG && ps->powerups[PW_BLUEFLAG])) return qtrue;
			} else if(ps->persistant[PERS_TEAM] == TEAM_BLUE) {
				if(item->giTag == PW_REDFLAG || (item->giTag == PW_BLUEFLAG && ent->modelindex2) || (item->giTag == PW_BLUEFLAG && ps->powerups[PW_REDFLAG])) return qtrue;
			}
		}
		if(gametype == GT_HARVESTER) {
			return qtrue;
		}
		return qfalse;

	case IT_HOLDABLE:
		if(ps->stats[STAT_HOLDABLE_ITEM]) {
			return qfalse;
		}
		return qtrue;

	case IT_NULL: return qfalse;

	default: Com_Printf("BG_CanItemBeGrabbed: unknown enum %d\n", item->giType); break;
	}

	return qfalse;
}

void BG_EvaluateTrajectory(const trajectory_t *tr, int atTime, vec3_t result) {
	float deltaTime;
	float phase;

	switch(tr->trType) {
	case TR_STATIONARY:
	case TR_INTERPOLATE: VectorCopy(tr->trBase, result); break;
	case TR_LINEAR:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		break;
	case TR_SINE:
		deltaTime = (atTime - tr->trTime) / (float)tr->trDuration;
		phase = sin(deltaTime * M_PI * 2);
		VectorMA(tr->trBase, phase, tr->trDelta, result);
		break;
	case TR_LINEAR_STOP:
		if(atTime > tr->trTime + tr->trDuration) {
			atTime = tr->trTime + tr->trDuration;
		}
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		if(deltaTime < 0) {
			deltaTime = 0;
		}
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		break;
	case TR_GRAVITY:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		result[2] -= 0.5 * mod_gravity * deltaTime * deltaTime;
		break;
	case TR_ROTATING:
		if(tr->trTime > 0)
			deltaTime = tr->trTime * 0.001; // milliseconds to seconds
		else if(tr->trTime < 0)
			deltaTime = (atTime + tr->trTime) * 0.001;
		else
			deltaTime = (atTime - tr->trTime) * 0.001;
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		break;
	case TR_GRAVITY_WATER:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		result[2] -= 0.5 * (mod_gravity * 0.50) * deltaTime * deltaTime;
		break;
	default: break;
	}
}

/*
================
BG_EvaluateTrajectoryDelta

For determining velocity at a given time
================
*/
void BG_EvaluateTrajectoryDelta(const trajectory_t *tr, int atTime, vec3_t result) {
	float deltaTime;
	float phase;

	switch(tr->trType) {
	case TR_STATIONARY:
	case TR_INTERPOLATE: VectorClear(result); break;
	case TR_ROTATING:
	case TR_LINEAR: VectorCopy(tr->trDelta, result); break;
	case TR_SINE:
		deltaTime = (atTime - tr->trTime) / (float)tr->trDuration;
		phase = cos(deltaTime * M_PI * 2); // derivative of sin = cos
		phase *= 0.5;
		VectorScale(tr->trDelta, phase, result);
		break;
	case TR_LINEAR_STOP:
		if(atTime > tr->trTime + tr->trDuration) {
			VectorClear(result);
			return;
		}
		VectorCopy(tr->trDelta, result);
		break;
	case TR_GRAVITY:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorCopy(tr->trDelta, result);
		result[2] -= mod_gravity * deltaTime;
		break;
	case TR_GRAVITY_WATER:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorCopy(tr->trDelta, result);
		result[2] -= (mod_gravity * 0.50) * deltaTime;
		break;
	default: break;
	}
}

void ST_EvaluateTrajectory(const trajectory_t *tr, int atTime, vec3_t result, float mass) {
	float deltaTime;
	float phase;

	switch(tr->trType) {
	case TR_STATIONARY:
	case TR_INTERPOLATE: VectorCopy(tr->trBase, result); break;
	case TR_LINEAR:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		break;
	case TR_SINE:
		deltaTime = (atTime - tr->trTime) / (float)tr->trDuration;
		phase = sin(deltaTime * M_PI * 2);
		VectorMA(tr->trBase, phase, tr->trDelta, result);
		break;
	case TR_LINEAR_STOP:
		if(atTime > tr->trTime + tr->trDuration) {
			atTime = tr->trTime + tr->trDuration;
		}
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		if(deltaTime < 0) {
			deltaTime = 0;
		}
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		break;
	case TR_GRAVITY:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		result[2] -= 0.5 * (mod_gravity * mass) * deltaTime * deltaTime;
		break;
	case TR_GRAVITY_WATER:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		result[2] -= 0.5 * (mod_gravity * (mass * 0.50)) * deltaTime * deltaTime;
		break;
	case TR_ROTATING:
		if(tr->trTime > 0)
			deltaTime = tr->trTime * 0.001; // milliseconds to seconds
		else if(tr->trTime < 0)
			deltaTime = (atTime + tr->trTime) * 0.001;
		else
			deltaTime = (atTime - tr->trTime) * 0.001;
		VectorMA(tr->trBase, deltaTime, tr->trDelta, result);
		break;
	default:
		// Com_Error( ERR_DROP, "ST_EvaluateTrajectory: unknown trType: %i", tr->trTime );
		break;
	}
}

/*
================
ST_EvaluateTrajectoryDelta

For determining velocity at a given time
================
*/
void ST_EvaluateTrajectoryDelta(const trajectory_t *tr, int atTime, vec3_t result, float mass) {
	float deltaTime;
	float phase;

	switch(tr->trType) {
	case TR_STATIONARY:
	case TR_INTERPOLATE: VectorClear(result); break;
	case TR_ROTATING:
	case TR_LINEAR: VectorCopy(tr->trDelta, result); break;
	case TR_SINE:
		deltaTime = (atTime - tr->trTime) / (float)tr->trDuration;
		phase = cos(deltaTime * M_PI * 2); // derivative of sin = cos
		phase *= 0.5;
		VectorScale(tr->trDelta, phase, result);
		break;
	case TR_LINEAR_STOP:
		if(atTime > tr->trTime + tr->trDuration) {
			VectorClear(result);
			return;
		}
		VectorCopy(tr->trDelta, result);
		break;
	case TR_GRAVITY:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorCopy(tr->trDelta, result);
		result[2] -= (mod_gravity * mass) * deltaTime;
		break;
	case TR_GRAVITY_WATER:
		deltaTime = (atTime - tr->trTime) * 0.001; // milliseconds to seconds
		VectorCopy(tr->trDelta, result);
		result[2] -= (mod_gravity * (mass * 0.50)) * deltaTime;
		break;
	default:
		// Com_Error( ERR_DROP, "ST_EvaluateTrajectoryDelta: unknown trType: %i", tr->trTime );
		break;
	}
}

/*
===============
BG_AddPredictableEventToPlayerstate

Handles the sequence numbers
===============
*/
void BG_AddPredictableEventToPlayerstate(int newEvent, int eventParm, playerState_t *ps) {
	ps->events[ps->eventSequence & (MAX_PS_EVENTS - 1)] = newEvent;
	ps->eventParms[ps->eventSequence & (MAX_PS_EVENTS - 1)] = eventParm;
	ps->eventSequence++;
}

void BG_TouchJumpPad(playerState_t *ps, entityState_t *jumppad) {
	vec3_t angles;
	float p;
	int effectNum;

	// spectators don't use jump pads
	if(ps->pm_type != PM_NORMAL) {
		return;
	}

	// flying characters don't hit bounce pads
	if(ps->powerups[PW_FLIGHT]) {
		return;
	}

	// if we didn't hit this same jumppad the previous frame
	// then don't play the event sound again if we are in a fat trigger
	if(ps->jumppad_ent != jumppad->number) {
		vectoangles(jumppad->origin2, angles);
		p = fabs(AngleNormalize180(angles[PITCH]));
		if(p < 45) {
			effectNum = 0;
		} else {
			effectNum = 1;
		}
		BG_AddPredictableEventToPlayerstate(EV_JUMP_PAD, effectNum, ps);
	}
	// remember hitting this jumppad this frame
	ps->jumppad_ent = jumppad->number;
	ps->jumppad_frame = ps->pmove_framecount;
	// give the player the velocity from the jumppad
	VectorCopy(jumppad->origin2, ps->velocity);
}

/*
========================
BG_PlayerStateToEntityState

This is done after each set of usercmd_t on the server,
and after local prediction on the client
========================
*/
void BG_PlayerStateToEntityState(playerState_t *ps, entityState_t *s, qboolean snap) {
	int i;

	if(ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR) {
		s->eType = ET_INVISIBLE;
	} else if(ps->stats[STAT_HEALTH] <= GIB_HEALTH) {
		s->eType = ET_INVISIBLE;
	} else {
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

	s->pos.trType = TR_INTERPOLATE;
	VectorCopy(ps->origin, s->pos.trBase);
	if(snap) {
		SnapVector(s->pos.trBase);
	}
	// set the trDelta for flag direction
	VectorCopy(ps->velocity, s->pos.trDelta);

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy(ps->viewangles, s->apos.trBase);
	if(snap) {
		SnapVector(s->apos.trBase);
	}

	s->angles2[YAW] = ps->movementDir;
	s->legsAnim = ps->legsAnim;
	s->torsoAnim = ps->torsoAnim;
	s->clientNum = ps->clientNum; // ET_PLAYER looks here instead of at number
	                              // so corpses can also reference the proper config
	s->eFlags = ps->eFlags;
	if(ps->stats[STAT_HEALTH] <= 0) {
		s->eFlags |= EF_DEAD;
	} else {
		s->eFlags &= ~EF_DEAD;
	}

	if(ps->externalEvent) {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else if(ps->entityEventSequence < ps->eventSequence) {
		int seq;

		if(ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
		}
		seq = ps->entityEventSequence & (MAX_PS_EVENTS - 1);
		s->event = ps->events[seq] | ((ps->entityEventSequence & 3) << 8);
		s->eventParm = ps->eventParms[seq];
		ps->entityEventSequence++;
	}

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for(i = 0; i < MAX_POWERUPS; i++) {
		if(ps->powerups[i]) {
			s->powerups |= 1 << i;
		}
	}

	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;
}

qboolean BG_InVehicle(int id) {
	if(id) return qtrue;

	return 0;
}
