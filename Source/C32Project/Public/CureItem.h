// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CureItem.generated.h"

/**
 * 
 */
UCLASS()
class C32PROJECT_API ACureItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ACureItem();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	bool bIsHeal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float HealAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	bool bIsHealStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float StaminaAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	bool bIsCleanse;

	virtual void ActivateItem(AActor* Acitvator) override;
	
};
