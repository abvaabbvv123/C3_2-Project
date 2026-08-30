// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CC3SaveSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class C32PROJECT_API UCC3SaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CheckPoint")
	void SetSavePoint(const FTransform& NewTransform);

	UFUNCTION(BlueprintCallable, Category = "CheckPoint")
	void RespawnPawn(APawn* Respawn) const;

	UFUNCTION(BlueprintCallable, Category = "CheckPoint")
	bool HasSavePoint();

private:
	UPROPERTY()
	FTransform LastSavePoint;
	//bHasSavePoint false = go to player start
	UPROPERTY()
	bool bHasSavePoint = false;
};
