// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include "Kismet/GameplayStatics.h"
#include "CC3SaveSubsystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
// Sets default values
AKillZone::AKillZone()
{
	PrimaryActorTick.bCanEverTick = false;
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	OverlapVolume->SetCollisionProfileName(TEXT("Trigger"));
	OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnOverlapBegin);
}


void AKillZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn) {
		return;
	}
	if (UGameInstance* GameInstance = GetGameInstance()) {
		if (UCC3SaveSubsystem* Subsystem = GameInstance->GetSubsystem<UCC3SaveSubsystem>()) {
			Subsystem->RespawnPawn(Pawn);
		}
	}
	UGameplayStatics::ApplyDamage(Pawn, 20, nullptr, this, UDamageType::StaticClass());
}