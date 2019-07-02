#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class E_IDLEENTRYSTATE : uint8
{
	N_IDLE,
	LF_IDLE,
	C_IDLE
};
UENUM(BlueprintType)
enum class E_GAIT : uint8
{
	GT_WALKING,
	GT_RUNNING,
	GT_SPRINTING
};
UENUM(BlueprintType)
enum class E_MOVEMENTMODE : uint8
{
	MM_NONE,
	MM_GROUNDED,
	MM_FALLING,
	MM_RAGDOLL
};
UENUM(BlueprintType)
enum class E_STANCEMODE : uint8
{
	SM_STANDING,
	SM_CROUCHING
};
UENUM(BlueprintType)
enum class E_ROTATIONMODE : uint8
{
	RM_VELOCITYDIRECTION,
	RM_LOOKINGDIRECTION
};
UENUM(BlueprintType)
enum class E_CARDINALDIRECTION : uint8
{
	CD_NORTH,
	CD_EAST,
	CD_WEST,
	CD_SOUTH
};
UENUM(BlueprintType)
enum class E_MOVEMENTDIRECTION : uint8
{
	MD_FORWARDS,
	MD_BACKWARDS
};
UENUM(BlueprintType)
enum class E_ACTIVELOCOSTATE : uint8
{
	ALS_NONE,
	ALS_NOTMOVING,
	ALS_MOVING,
	ALS_PIVOT,
	ALS_STOPPING
};
UENUM(BlueprintType)
enum class E_FOOTSTEPTYPE : uint8
{
	FST_STEP,
	FST_WALK,
	FST_RUN,
	FST_SPRINT,
	FST_PIVOT,
	FST_JUMP,
	FST_LAND
};
UENUM(BlueprintType)
enum class E_ACTIVEWEAPON : uint8
{
	AW_NONE,
	AW_KATANA
};
UENUM(BlueprintType)
enum class E_KATANASTATE : uint8
{
	KS_KAMAE,
	KS_HIDARIJOUHOU,
	KS_MIGIJOUHOU,
	KS_HIDARIKESA,
	KS_MIGIKESA,
	KS_SHOMEN,
	KS_TSUKI,
	KS_CHIBURI,
	KS_NOTO
};