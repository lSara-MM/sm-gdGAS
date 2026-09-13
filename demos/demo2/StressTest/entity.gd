extends GAS_Entity

@export var attr_container : AttributeContainer
@export var tag_container : TagContainer
@export var ability_container : AbilityContainer
var race
var tier
var faction

func SetUp(rng: RandomNumberGenerator, abilities: Dictionary) -> Dictionary:
	var races = StressTestConfig.RACE_IDS
	var tiers = StressTestConfig.TIER_IDS
	var factions = StressTestConfig.FACTION_IDS

	race = races[rng.randi_range(0, races.size() - 1)]
	SetUpRace()
	
	tier = tiers[rng.randi_range(0, tiers.size() - 1)]
	SetUpTier()
	
	faction = factions[rng.randi_range(0, factions.size() - 1)]
	tag_container.add_tag(faction)
	
	SetUpAbilities(abilities)
	tag_container.add_tag(Tags._State)
	tag_container.add_tag(Tags._State_Alive)
	
	return {
	"race": race,
	"tier": tier,
	"faction": faction,
	"abilities":ability_container.abilities.size()
	}
	
func SetUpRace() -> void:
	tag_container.add_tag(race)
	
	match race:
		Tags._Race_Beast:
			pass
		Tags._Race_Undead:
			tag_container.add_tag(Tags._Immunity_Poison)
			pass
		Tags._Race_Machine:
			tag_container.add_tag(Tags._Immunity_Poison)
			tag_container.add_tag(Tags._Immunity_Stun)
			pass
		Tags._Race_Elemental:
			tag_container.add_tag(Tags._Immunity_Burn)
			tag_container.add_tag(Tags._Immunity_Freeze)
			pass
		Tags._Race_Plant:
			tag_container.add_tag(Tags._Immunity_Slow)
			tag_container.add_tag(Tags._Immunity_Stun)
			pass

func SetUpTier() -> void:
	var mod := ModifierData.new()
	mod.operation_type = ModifierData.PercentAdd
	match tier:
		Tags._Tier_Lesser:
			tag_container.add_tag(Tags._Tier_Lesser)
			mod.value = -25
		Tags._Tier_Base:
			tag_container.add_tag(Tags._Tier_Base)
			return
		Tags._Tier_Elite:
			tag_container.add_tag(Tags._Tier_Elite)
			mod.value = 25

	var attr_set = attr_container.get_attributes_set().attributes
	for attr in attr_set:
		if not attr.name.contains("Speed"):
			attr_container.add_base_modifier(attr.name, mod)
	pass

func SetUpAbilities(abilities: Dictionary) -> void:
	var num : int
	match tier:
		Tags._Tier_Lesser:
			num = 1
		Tags._Tier_Base:
			num = 2
		Tags._Tier_Elite:
			num = 3
	
	var race_abilities : Array = []
	#ability_container.abilities.push_back(abilities[Tags._Ability_BasicAttack])
	ability_container.grant_ability(abilities[Tags._Ability_BasicAttack])
		
	match race:
		Tags._Race_Beast:
			race_abilities = [
				abilities[Tags._Ability_Haste],
				abilities[Tags._Ability_Taunt],
				abilities[Tags._Ability_Slow]
			]
		Tags._Race_Undead:
			race_abilities = [
				abilities[Tags._Ability_PoisonDart],
				abilities[Tags._Ability_MassPurge],
				abilities[Tags._Ability_Regeneration]
			]
		Tags._Race_Machine:
			race_abilities = [
				abilities[Tags._Ability_Invulnerability],
				abilities[Tags._Ability_Execution],
				abilities[Tags._Ability_Haste]
			]
		Tags._Race_Elemental:
			race_abilities = [
				abilities[Tags._Ability_Freeze],
				abilities[Tags._Ability_FrostNova],
				abilities[Tags._Ability_Slow]
			]
		Tags._Race_Plant:
			race_abilities = [
				abilities[Tags._Ability_MassHeal],
				abilities[Tags._Ability_PoisonExplosion],
				abilities[Tags._Ability_Regeneration]
			]

	for ability in race_abilities.slice(0, num):
		ability_container.grant_ability(ability)
		#ability_container.abilities.push_back(ability)
	pass
