#include "Katana.h"
#include "NohCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

AKatana::AKatana()
{
	//Create Katana mesh
	mesh_weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = mesh_weapon;

	if (GetWorld())
	{
		//Current katana state
		m_katanastate = E_KATANASTATE::KS_IDLE;
		m_bKatanaRaised = false;
		m_bCanCancelKatanaAttack = false;

		//Current Animation Montage Time
		m_fCurrentAnimMontageTime = 0.0f;

		//Load Katana and Saya blueprint objects
		static ConstructorHelpers::FObjectFinder<UClass> KatanaBlueprint(TEXT("Class'/Game/Weapons/Katana/BP_Katana.BP_Katana_C'"));
		if (KatanaBlueprint.Object) {
			bp_katana = KatanaBlueprint.Object;
		}
		static ConstructorHelpers::FObjectFinder<UClass> SayaBlueprint(TEXT("Class'/Game/Weapons/Katana/BP_Saya.BP_Saya_C'"));
		if (SayaBlueprint.Object) {
			bp_saya = SayaBlueprint.Object;
		}

		//Load animation montages
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ch_animmontage_nbattouidle(TEXT("/Game/MainCharacter/Animations/Combat/Katana/N_Battou_Idle_Montage"));
		if (ch_animmontage_nbattouidle.Object) {
			animmontage_battou_idle = ch_animmontage_nbattouidle.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ch_animmontage_nbattoumoving(TEXT("/Game/MainCharacter/Animations/Combat/Katana/N_Battou_Moving_Montage"));
		if (ch_animmontage_nbattoumoving.Object) {
			animmontage_battou_moving = ch_animmontage_nbattoumoving.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ch_animmontage_nkamaenotoidle(TEXT("/Game/MainCharacter/Animations/Combat/Katana/Noto/N_Idle_Kamae_Noto"));
		if (ch_animmontage_nkamaenotoidle.Object) {
			animmontage_kamae_noto_idle = ch_animmontage_nkamaenotoidle.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ch_animmontage_nkamaenotomoving(TEXT("/Game/MainCharacter/Animations/Combat/Katana/Noto/N_Moving_Kamae_Noto"));
		if (ch_animmontage_nkamaenotomoving.Object) {
			animmontage_kamae_noto_moving = ch_animmontage_nkamaenotomoving.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ch_animmontage_nkamaechiburinotoidle(TEXT("/Game/MainCharacter/Animations/Combat/Katana/Noto/N_Idle_Kamae_ChiburiNoto"));
		if (ch_animmontage_nkamaechiburinotoidle.Object) {
			animmontage_kamae_chiburinoto_idle = ch_animmontage_nkamaechiburinotoidle.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ch_animmontage_nkamaechiburinotomoving(TEXT("/Game/MainCharacter/Animations/Combat/Katana/Noto/N_Moving_Kamae_ChiburiNoto"));
		if (ch_animmontage_nkamaechiburinotomoving.Object) {
			animmontage_kamae_chiburinoto_moving = ch_animmontage_nkamaechiburinotomoving.Object;
		}

		static ConstructorHelpers::FObjectFinder<UAnimMontage> ch_animmontage_raise_shomengiri(TEXT("/Game/MainCharacter/Animations/Combat/Katana/ShomenGiri/AM_RaiseShomenGiri"));
		if (ch_animmontage_raise_shomengiri.Object) {
			m_amRaiseShomengiri = ch_animmontage_raise_shomengiri.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> ch_animmontage_lower_shomengiri(TEXT("/Game/MainCharacter/Animations/Combat/Katana/ShomenGiri/AM_LowerShomenGiri"));
		if (ch_animmontage_lower_shomengiri.Object) {
			m_amLowerShomengiri = ch_animmontage_lower_shomengiri.Object;
		}
	}
}

//---Get Katana---//
AKatana* AKatana::GetWeapon(ANohCharacter*& nohref)
{
	AKatana* katana{ nullptr };
	if (nohref && bp_katana)
	{
		FActorSpawnParameters spawnparams;
		spawnparams.Owner = nohref;
		katana = GetWorld()->SpawnActor<AKatana>(bp_katana, FVector{0.0f, 0.0f, 0.0f}, FRotator{ 0.0f, 0.0f, 0.0f }, spawnparams);
		katana->mesh_weapon->SetCollisionResponseToChannel(nohref->GetMesh()->GetCollisionObjectType(), ECollisionResponse::ECR_Ignore);
		katana->mesh_weapon->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		//katana->mesh_weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AActor* saya{ GetWorld()->SpawnActor<AActor>(bp_saya, FVector{ 0.0f, 0.0f, 0.0f }, FRotator{ 0.0f, 0.0f, 0.0f }, spawnparams) };
		katana->AttachToComponent(nohref->GetMesh(), FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }, "socket_righthand");
		saya->AttachToComponent(nohref->GetMesh(), FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }, "socket_pelvis");
	}
	return katana;
}

UStaticMeshComponent* AKatana::GetKatanaMeshRef()
{
	return mesh_weapon;
}

//---Get Katanta State---//
E_KATANASTATE AKatana::GetKatanaState()
{
	return m_katanastate;
}

void AKatana::SetRaiseKatanaFinish(ANohCharacter*& nohref)
{
	if (nohref)
	{
		switch (m_katanastate)
		{
		case E_KATANASTATE::KS_KAMAE:
			break;
		case E_KATANASTATE::KS_HIDARIJOUHOU:
			break;
		case E_KATANASTATE::KS_MIGIJOUHOU:
			break;
		case E_KATANASTATE::KS_HIDARIKESA:
			break;
		case E_KATANASTATE::KS_MIGIKESA:
			break;
		case E_KATANASTATE::KS_SHOMEN:
			nohref->GetMesh()->GetAnimInstance()->Montage_Pause(m_amRaiseShomengiri);
			m_fCurrentAnimMontageTime = nohref->GetMesh()->GetAnimInstance()->Montage_GetPosition(m_amRaiseShomengiri);
			m_bKatanaRaised = true;
			break;
		case E_KATANASTATE::KS_TSUKI:
			break;
		}
	}
}

void AKatana::SetLowerkatanaFinish(ANohCharacter *& nohref)
{
	if (nohref)
	{
		switch (m_katanastate)
		{
		case E_KATANASTATE::KS_KAMAE:
			break;
		case E_KATANASTATE::KS_HIDARIJOUHOU:
			break;
		case E_KATANASTATE::KS_MIGIJOUHOU:
			break;
		case E_KATANASTATE::KS_HIDARIKESA:
			break;
		case E_KATANASTATE::KS_MIGIKESA:
			break;
		case E_KATANASTATE::KS_SHOMEN:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Back to normal"));
			m_katanastate = E_KATANASTATE::KS_KAMAE;
			break;
		case E_KATANASTATE::KS_TSUKI:
			break;
		}
	}
}
 
//---Unsheath Katana---//
void AKatana::Unsheath(ANohCharacter*& nohref)
{
	if (nohref)
	{
		m_katanastate = E_KATANASTATE::KS_KAMAE;
		if (!nohref->GetIsMoving())
		{
			nohref->GetMesh()->GetAnimInstance()->Montage_Play(animmontage_battou_idle, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
		else
		{
			nohref->GetMesh()->GetAnimInstance()->Montage_Play(animmontage_battou_moving, 1.5f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
	}
}

//---Sheath Katana---//
void AKatana::Sheath(ANohCharacter*& nohref)
{
	if (nohref)
	{
		switch (m_katanastate)
		{
		case E_KATANASTATE::KS_KAMAE:
			if (nohref->GetEnemyHit())
			{
				if (nohref->GetIsMoving())
				{
					nohref->GetMesh()->GetAnimInstance()->Montage_Play(animmontage_kamae_chiburinoto_moving, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
				else
				{
					nohref->GetMesh()->GetAnimInstance()->Montage_Play(animmontage_kamae_chiburinoto_idle, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
			}
			else
			{
				if (nohref->GetIsMoving())
				{
					nohref->GetMesh()->GetAnimInstance()->Montage_Play(animmontage_kamae_noto_moving, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
				else
				{
					nohref->GetMesh()->GetAnimInstance()->Montage_Play(animmontage_kamae_noto_idle, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
			}
			break;
		case E_KATANASTATE::KS_HIDARIJOUHOU:
			break;
		case E_KATANASTATE::KS_MIGIJOUHOU:
			break;
		case E_KATANASTATE::KS_HIDARIKESA:
			break;
		case E_KATANASTATE::KS_MIGIKESA:
			break;
		case E_KATANASTATE::KS_SHOMEN:
			break;
		case E_KATANASTATE::KS_TSUKI:
			break;
		}

		m_katanastate = E_KATANASTATE::KS_NOTO;
	}
}

void AKatana::SwapSkill(ANohCharacter*& nohref, E_KATANASTATE enumkatana)
{
	if (enumkatana == E_KATANASTATE::KS_IDLE)
	{
		m_katanastate = E_KATANASTATE::KS_IDLE;
	}
	else if (enumkatana == E_KATANASTATE::KS_KAMAE)
	{
		if (m_katanastate == E_KATANASTATE::KS_IDLE)
		{
			m_katanastate = E_KATANASTATE::KS_KAMAE;
		}
		else
		{
			CancelAttackSkill(nohref);
		}
	}
}

//---Choose Attack---//
void AKatana::ChooseAttackSkill(ANohCharacter*& nohref, float direction)
{
	if (nohref && m_katanastate == E_KATANASTATE::KS_KAMAE)
	{
		if (direction > -1.97f && direction < -1.18f)
		{
			m_katanastate = E_KATANASTATE::KS_SHOMEN;
			m_bCanCancelKatanaAttack = true;
			nohref->GetMesh()->GetAnimInstance()->Montage_Play(m_amRaiseShomengiri, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
	}
}

//---Cancel Attack---//
void AKatana::CancelAttackSkill(ANohCharacter *& nohref)
{
	if (m_bCanCancelKatanaAttack)
	{
		m_bCanCancelKatanaAttack = false;

		switch (m_katanastate)
		{
		case E_KATANASTATE::KS_SHOMEN:
			if (m_bKatanaRaised)
			{
				nohref->GetMesh()->GetAnimInstance()->Montage_Play(m_amLowerShomengiri, 1.0f, EMontagePlayReturnType::MontageLength, m_fCurrentAnimMontageTime, true);
			}
			else
			{

			}
			break;
		}
	}
}
