// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"
#include "CC3SaveSubsystem.h"
#include "CC3Character.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
// Sets default values

	ASavePoint::ASavePoint()
{
}

void ASavePoint::ActivateItem(AActor* Activator) {
	if (SaveParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SaveParticle, GetActorLocation(), GetActorRotation(), true);
	}
	if (SaveSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SaveSound, GetActorLocation());
	}
	//trigger set saveu
	if (UGameInstance* GameInstance = GetGameInstance()) {
		if (UCC3SaveSubsystem* Subsystem = GameInstance->GetSubsystem<UCC3SaveSubsystem>()) {
			Subsystem->SetSavePoint(GetActorTransform());
		}
	}
}


