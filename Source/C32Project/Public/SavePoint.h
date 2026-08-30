// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SavePoint.generated.h"

/**
 * 
 */
UCLASS()
class C32PROJECT_API ASavePoint : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASavePoint();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* SaveParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* SaveSound;

	virtual void ActivateItem(AActor* Activator) override;

};

