---@meta

---@alias float number

---@class SkyLua
---@field log fun(msg: string)
---@field print fun(msg: string)
---@field debug fun(msg: string)
---@field notify fun(msg: string)
---@field get_player fun(): PlayerCharacter
---@field OnTick fun()
---@field Handle_ActionEvent fun(event: ActionEvent)
---@field Handle_MenuOpenCloseEvent fun(event: MenuOpenCloseEvent)
---@field Handle_ModCallbackEvent fun(event: ModCallbackEvent)
---@field Handle_CrosshairRefEvent fun(event: CrosshairRefEvent)
---@field Handle_TESHitEvent fun(event: TESHitEvent)
---@field Handle_TESActivateEvent fun(event: TESActivateEvent)
---@field ActorBoolFlags SkyLua.ActorBoolFlags
---@field ActorValue SkyLua.ActorValue
---@field HitEventFlag SkyLua.HitEventFlag
---@field QuestFlag SkyLua.QuestFlag
---@field ActorBaseDataFlag SkyLua.ActorBaseDataFlag
---@field ActionEventType SkyLua.ActionEventType
---@field QuestType SkyLua.QuestType
---@field ActionEventSlot SkyLua.ActionEventSlot

---@class SkyLua.ActorBoolFlags
---@enum
SkyLua.ActorBoolFlags = {
  kInBleedoutAnimation = 64,
  kMountPointClear = 4,
  kScenePackage = 1,
  kIsAMount = 2,
  kNone = 0,
  kGettingOnOffMount = 8,
  kInRandomScene = 16,
  kNoBleedoutRecovery = 32
}

---@class SkyLua.ActorValue
---@enum
SkyLua.ActorValue = {
  kEnchantingModifier = 113,
  kOneHandedSkillAdvance = 114,
  kTwoHandedSkillAdvance = 115,
  kMarksmanSkillAdvance = 116,
  kBlockSkillAdvance = 117,
  kSmithingSkillAdvance = 118,
  kHeavyArmorSkillAdvance = 119,
  kLightArmorSkillAdvance = 120,
  kPickpocketSkillAdvance = 121,
  kLockpickingSkillAdvance = 122,
  kSneakingSkillAdvance = 123,
  kAlchemySkillAdvance = 124,
  kSpeechcraftSkillAdvance = 125,
  kAlterationSkillAdvance = 126,
  kHeavyArmor = 11,
  kLightArmor = 12,
  kPickpocket = 13,
  kLockpicking = 14,
  kSneak = 15,
  kAlchemy = 16,
  kSpeech = 17,
  kAlteration = 18,
  kConjuration = 19,
  kDestruction = 20,
  kIllusion = 21,
  kRestoration = 22,
  kEnchanting = 23,
  kHealth = 24,
  kMagicka = 25,
  kStamina = 26,
  kHealRate = 27,
  kMagickaRate = 28,
  kStaminaRate = 29,
  kSpeedMult = 30,
  kInventoryWeight = 31,
  kCarryWeight = 32,
  kCriticalChance = 33,
  kMeleeDamage = 34,
  kUnarmedDamage = 35,
  kMass = 36,
  kVoicePoints = 37,
  kVoiceRate = 38,
  kDamageResist = 39,
  kPoisonResist = 40,
  kResistFire = 41,
  kResistShock = 42,
  kResistFrost = 43,
  kResistMagic = 44,
  kResistDisease = 45,
  kPerceptionCondition = 46,
  kEnduranceCondition = 47,
  kLeftAttackCondition = 48,
  kRightAttackCondition = 49,
  kLeftMobilityCondition = 50,
  kRightMobilityCondition = 51,
  kBrainCondition = 52,
  kParalysis = 53,
  kInvisibility = 54,
  kNightEye = 55,
  kDetectLifeRange = 56,
  kWaterBreathing = 57,
  kWaterWalking = 58,
  kIgnoreCrippledLimbs = 59,
  kFame = 60,
  kInfamy = 61,
  kJumpingBonus = 62,
  kWardPower = 63,
  kRightItemCharge = 64,
  kArmorPerks = 65,
  kShieldPerks = 66,
  kWardDeflection = 67,
  kVariable01 = 68,
  kVariable02 = 69,
  kVariable03 = 70,
  kVariable04 = 71,
  kVariable05 = 72,
  kVariable06 = 73,
  kVariable07 = 74,
  kVariable08 = 75,
  kVariable09 = 76,
  kVariable10 = 77,
  kBowSpeedBonus = 78,
  kFavorActive = 79,
  kFavorsPerDay = 80,
  kFavorsPerDayTimer = 81,
  kLeftItemCharge = 82,
  kAbsorbChance = 83,
  kBlindness = 84,
  kWeaponSpeedMult = 85,
  kShoutRecoveryMult = 86,
  kBowStaggerBonus = 87,
  kTelekinesis = 88,
  kFavorPointsBonus = 89,
  kLastBribedIntimidated = 90,
  kLastFlattered = 91,
  kMovementNoiseMult = 92,
  kBypassVendorStolenCheck = 93,
  kBypassVendorKeywordCheck = 94,
  kWaitingForPlayer = 95,
  kConjurationSkillAdvance = 127,
  kArchery = 8,
  kBlock = 9,
  kSmithing = 10,
  kEnergy = 2,
  kMorality = 3,
  kMood = 4,
  kAssistance = 5,
  kOneHanded = 6,
  kTwoHanded = 7,
  kOneHandedModifier = 96,
  kTwoHandedModifier = 97,
  kMarksmanModifier = 98,
  kBlockModifier = 99,
  kSmithingModifier = 100,
  kHeavyArmorModifier = 101,
  kLightArmorModifier = 102,
  kPickpocketModifier = 103,
  kLockpickingModifier = 104,
  kSneakingModifier = 105,
  kAlchemyModifier = 106,
  kSpeechcraftModifier = 107,
  kAlterationModifier = 108,
  kConjurationModifier = 109,
  kDestructionModifier = 110,
  kIllusionModifier = 111,
  kRestorationModifier = 112,
  kConfidence = 1,
  kAggression = 0,
  kNone = -1
}

---@class SkyLua.HitEventFlag
---@enum
SkyLua.HitEventFlag = {
  kPowerAttack = 1,
  kSneakAttack = 2,
  kNone = 0,
  kHitBlocked = 8,
  kBashAttack = 4
}

---@class SkyLua.QuestFlag
---@enum
SkyLua.QuestFlag = {
  kNone = 0,
  kStopStart = -1,
  kEnabled = 1,
  kCompleted = 2,
  kAddIdleToHello = 4,
  kAllowRepeatStages = 8,
  kStartsEnabled = 16,
  kDisplayedInHUD = 32,
  kFailed = 64
}

---@class SkyLua.ActionEventType
---@enum
SkyLua.ActionEventType = {
  kWeaponSwing = 0,
  kSpellCast = 1,
  kSpellFire = 2,
  kVoiceCast = 3,
  kVoiceFire = 4,
  kBowDraw = 5,
  kBowRelease = 6,
  kBeginDraw = 7,
  kEndDraw = 8,
  kBeginSheathe = 9,
  kEndSheathe = 10
}

---@class SkyLua.QuestType
---@enum
SkyLua.QuestType = {
  kMainQuest = 1,
  kMagesGuild = 2,
  kThievesGuild = 3,
  kDarkBrotherhood = 4,
  kNone = 0,
  kMiscellaneous = 6,
  kDaedric = 7,
  kSideQuest = 8,
  kCivilWar = 9,
  kDLC01_Vampire = 10,
  kDLC02_Dragonborn = 11,
  kCompanionsQuest = 5
}

---@class SkyLua.ActionEventSlot
---@enum
SkyLua.ActionEventSlot = {
  kLeft = 0,
  kRight = 1,
  kVoice = 2
}

---@class TESObjectREFR
---@field get_name fun(self:TESObjectREFR): string
---@field as_actor fun(self:TESObjectREFR): Actor

---@class Actor
---@field as_actor_owner fun(self:Actor): ActorValueOwner

---@class PlayerCharacter

---@class ActorValueOwner
---@field get_actor_value fun(self:ActorValueOwner, arg: SkyLua.ActorValue): float
---@field set_actor_value fun(self:ActorValueOwner, arg: SkyLua.ActorValue, arg: float)

---@class ActionEvent
---@field get_type fun(self:ActionEvent): SkyLua.ActionEventType
---@field get_slot fun(self:ActionEvent): SkyLua.ActionEventSlot

---@class CrosshairRefEvent
---@field get_crosshair_ref fun(self:CrosshairRefEvent): TESObjectREFR

---@class ModCallbackEvent
---@field get_event_name fun(self:ModCallbackEvent): string

---@class MenuOpenCloseEvent
---@field get_menu_name fun(self:MenuOpenCloseEvent): string

---@class TESActivateEvent
---@field get_activated fun(self:TESActivateEvent): TESObjectREFR
---@field get_activator fun(self:TESActivateEvent): TESObjectREFR

---@class TESHitEvent
---@field get_cause fun(self:TESHitEvent): TESObjectREFR
---@field get_target fun(self:TESHitEvent): TESObjectREFR
---@field get_source fun(self:TESHitEvent): integer
---@field get_projectile fun(self:TESHitEvent): integer
---@field get_flags fun(self:TESHitEvent): SkyLua.HitEventFlag

---@type SkyLua
_G.SkyLua = _G.SkyLua
