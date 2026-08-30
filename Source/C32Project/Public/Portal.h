// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Portal.generated.h"

/**
 * 
 */
UCLASS()
class C32PROJECT_API APortal : public ABaseItem
{
	GENERATED_BODY()
	
public:
	APortal();
	virtual void ActivateItem(AActor* Activator) override;
};
