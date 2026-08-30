// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CC3GameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWaveScores {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Score")
	int32 Wave = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Score")
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Score")
	int32 Score = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Score")
	float Time = 0.f;
};
UCLASS()
class C32PROJECT_API  UCC3GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCC3GameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Data")
	int32 CurrentLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Data")
	int32 CurrentWave;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Data")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Data")
	int32 CurrentScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Data")
	float TotalLevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Data")
	int32 Level1Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Data")
	int32 Level2Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Data")
	int32 Level3Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Data")
	TArray<FWaveScores> WaveScores;

	UFUNCTION(BlueprintCallable, Category = "Game|Data")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Game|Data")
	void AddLevel();
	UFUNCTION(BlueprintCallable, Category = "Game|Data")
	void ClearLevel();
	UFUNCTION(BlueprintCallable, Category = "Game|Data")
	void AddWave();
	UFUNCTION(BlueprintCallable, Category = "Game|Data")
	void ClearWave();
	UFUNCTION(BlueprintPure, Category = "Game|Data")
	int32 GetTotal();
	UFUNCTION(BlueprintPure, Category = "Game|Data")
	int32 GetLevel();
	UFUNCTION(BlueprintPure, Category = "Game|Data")
	int32 GetRequired();
	UFUNCTION(BlueprintPure, Category = "Game|Data")
	bool GetWaveScores(int32 Level, int32 Wave, FWaveScores& Scores) const;
};
