#include "NohGameMode.h"
#include "NohCharacter.h"
#include "NohHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ANohGameMode::ANohGameMode()
{
	//Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MainCharacter/Yasuke/BP_NohMann"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDBPClass(TEXT("/Game/UI/BP_NohHUD"));
	if (PlayerHUDBPClass.Class != NULL)
	{
		HUDClass = PlayerHUDBPClass.Class;
	}
}

void ANohGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::CreatePlayer(this, -1, true);
}
