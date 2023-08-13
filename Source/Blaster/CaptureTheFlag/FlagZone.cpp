
#include "FlagZone.h"
#include "Components/BoxComponent.h"
#include "Blaster/Weapon/Flag.h"
#include "Blaster/Gamemode/CTFTeamGameMode.h"

AFlagZone::AFlagZone()
{
	PrimaryActorTick.bCanEverTick = false;
	Zone = CreateDefaultSubobject<UBoxComponent>(TEXT("Zone"));
	SetRootComponent(Zone);
}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();
	Zone->OnComponentBeginOverlap.AddDynamic(this, &AFlagZone::OnBoxOverlap);
}

void AFlagZone::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFlag* OverlappingFlag = Cast<AFlag>(OtherActor);
	if (OverlappingFlag && OverlappingFlag->GetTeam() != Team)
	{
		ACTFTeamGameMode* GameMode = GetWorld()->GetAuthGameMode<ACTFTeamGameMode>();
		if (GameMode)
		{
			GameMode->FlagCaptured(OverlappingFlag, this);
		}
		OverlappingFlag->ResetFlag();
	}
}
