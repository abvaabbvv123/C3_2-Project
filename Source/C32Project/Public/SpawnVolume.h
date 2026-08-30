// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemRowTable.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;
UCLASS()
class C32PROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpawnVolume();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Spawn")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	UBoxComponent* Box;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	UDataTable* SpawnChanceDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 ItemSpawnAmount;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AActor* SpawnRandomItem();

	FVector GetRandomLocation() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FItemSpawnRow* GetRandomItem() const;
};
