// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CC3GameState.generated.h"
class ASpawnVolume;
/**
 * 
 */
UCLASS()
class C32PROJECT_API ACC3GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ACC3GameState();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	int32 SpawnedItemCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	int32 CollectedItemCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	int32 MaxLevels;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	int32 ItemToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TArray<FName> LevelNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	ASpawnVolume* TargetSpawnActor;
	
	float RemainingTime = 0;
	FTimerHandle HUDTimerHandle;
	FTimerHandle LevelTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Game")
	void StartLevel();
	UFUNCTION(BlueprintCallable, Category = "Game")
	void EndWave();
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnNextLevel();
	UFUNCTION(BlueprintCallable, Category = "Game")
	void WaitUntilAnim();

	void OnItemCollect();
	void OnLevelTimeUp();
	void UpdateHUD();  
	//void UpdateDebuff(FName name, bool bIsOn);
};

