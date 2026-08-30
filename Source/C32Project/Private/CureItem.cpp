// Fill out your copyright notice in the Description page of Project Settings.


#include "CureItem.h"
#include "CC3Character.h"

ACureItem::ACureItem() {
	HealAmount = 0;
}

void ACureItem::ActivateItem(AActor* Activator) {
	Super::ActivateItem(Activator);
	if (ACC3Character* Player = Cast<ACC3Character>(Activator)) {
		if (bIsHeal) {
			Player->AddHealth(HealAmount);
		}
		if (bIsHealStamina) {
			Player->AddStamina(StaminaAmount);
		}
		if (bIsCleanse) {
			Player->Cleanse();
		}	
		DestroyItem();
	}
}

