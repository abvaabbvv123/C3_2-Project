// Fill out your copyright notice in the D1escription page of Project Settings.


#include "Portal.h"
#include "Engine/World.h"
#include "CC3GameState.h"
#include "GameFramework/RotatingMovementComponent.h"

APortal::APortal() {
	if (RotateComponent) {
		RotateComponent->DestroyComponent();
		RotateComponent = nullptr;
	}
}
void APortal::ActivateItem(AActor* Activator) { 
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player")) {
		if (UWorld* World = GetWorld()) {
			if (ACC3GameState* GameState = World->GetGameState<ACC3GameState>()) {
				GameState->EndWave();
			}
		}
		DestroyItem();
	}
}

