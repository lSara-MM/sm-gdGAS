extends RefCounted
class_name StressTestConfig

# -------------------------------------------------------------------
# Central configuration for the stress test.
# Keep ability IDs as the IDs used by AbilityData / AbilityContainer.
# -------------------------------------------------------------------

const ENTITY_COUNT := 10000
const RNG_SEED := 12345

const RACE_IDS := [
	Tags._Race_Beast,
	Tags._Race_Undead,
	Tags._Race_Machine,
	Tags._Race_Elemental,
	Tags._Race_Plant,
]

const TIER_IDS := [
	Tags._Tier_Lesser,
	Tags._Tier_Base,
	Tags._Tier_Elite,
]

const FACTION_IDS := [
	Tags._Faction_Blue,
	Tags._Faction_Red,
]

# Percentage chances for starting temporary states.
# These are deliberately small so the world remains mixed.
const START_POISON_CHANCE := 0.03
const START_BURN_CHANCE := 0.02
const START_FREEZE_CHANCE := 0.02
const START_INVULNERABLE_CHANCE := 0.01

# How many ability requests the random stress phase performs per tick.
const ACTIONS_PER_TICK := 100

# Delay between stress ticks.
const STRESS_TICK_INTERVAL := 0.05

# Number of random stress ticks.
const STRESS_TICKS := 1000
