// Fill out your copyright notice in the Description page of Project Settings.


#include "PointItem.h"
#include "Engine/World.h"
#include "CC3GameState.h"

APointItem::APointItem() {
	PointValue = 0;
	ItemType = "Default";
}

void APointItem::ActivateItem(AActor* Activator) {
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player")) {
		if (UWorld* World = GetWorld()) {
			if (ACC3GameState* GameState = World->GetGameState<ACC3GameState>()) {
				GameState->AddScore(PointValue);
			}
		}
		DestroyItem();
	}
}

