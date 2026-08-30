// Fill out your copyright notice in the Description page of Project Settings.


#include "CC3GameInstance.h"

UCC3GameInstance::UCC3GameInstance() {
	CurrentLevel = 1;
	CurrentWave = 1;
	TotalScore = 0;
	CurrentScore = 0;
	Level1Score = 2600;
	Level2Score = 8100;
	Level3Score = 14400;

}
void UCC3GameInstance::AddScore(int32 Amount) {
	TotalScore += Amount;
	CurrentScore += Amount;
}

void UCC3GameInstance::AddLevel() {
	if (CurrentLevel >= 3) {
		return;
	}
	CurrentLevel++;
}
void UCC3GameInstance::ClearLevel() {
	CurrentLevel = 1;
}
void UCC3GameInstance::AddWave() {
	if (CurrentWave >= 3) {
		return;
	}
	CurrentWave++;
}
void UCC3GameInstance::ClearWave() {
	CurrentWave = 1;
}
int32 UCC3GameInstance::GetTotal() {
	return TotalScore;
}
int32 UCC3GameInstance::GetLevel() {
	return CurrentLevel;
}
int32 UCC3GameInstance::GetRequired() {
	int32 current_level = GetLevel();
	if (current_level == 1) {
		return Level1Score;
	}
	else if (current_level == 2) {
		return Level2Score;
	}
	else if (current_level == 3) {
		return Level3Score;
	}
	return 0;
}
bool UCC3GameInstance::GetWaveScores(int32 Level, int32 Wave, FWaveScores& Scores) const {
	for (const FWaveScores& Data : WaveScores) {
		if (Data.Level == Level && Data.Wave == Wave) {
			Scores = Data;
			return true;
		}
	}
	return false;
}