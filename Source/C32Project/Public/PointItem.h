// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"

#include "PointItem.generated.h"
UCLASS()
class C32PROJECT_API APointItem : public ABaseItem
{
	GENERATED_BODY()

public:
	APointItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;

};
