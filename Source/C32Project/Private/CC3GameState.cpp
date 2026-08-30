// Fill out your copyright notice in the Description page of Project Settings.


#include "CC3GameState.h"
#include "CC3PlayerController.h"
#include "CC3GameInstance.h"
#include "PointItem.h"
#include "SpawnVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"


ACC3GameState::ACC3GameState() {
	Score = 0;
	SpawnedItemCount = 0;
	CollectedItemCount = 0;
	LevelDuration = 0.f;

	MaxLevels = 3;
	ItemToSpawn = 1;

	RemainingTime = 0;
}

void ACC3GameState::BeginPlay() {
	Super::BeginPlay();
	if (UGameInstance* GI = GetGameInstance()) {
		if (UCC3GameInstance* GameInstance = Cast<UCC3GameInstance>(GI)) {
			RemainingTime = GameInstance->TotalLevelDuration;
			GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &ACC3GameState::OnLevelTimeUp, RemainingTime, false);
			GetWorldTimerManager().SetTimer(HUDTimerHandle, this, &ACC3GameState::UpdateHUD, 0.1f, true);
			if (GameInstance->CurrentWave == 1) {
				GetWorldTimerManager().PauseTimer(LevelTimerHandle);
			}
		}
	}
	StartLevel();
}
void ACC3GameState::WaitUntilAnim() {
	GetWorldTimerManager().UnPauseTimer(LevelTimerHandle);
}
int32 ACC3GameState::GetScore() const {
	return Score;
}
void ACC3GameState::AddScore(int32 Amount) {
	if (UGameInstance* GI = GetGameInstance()) {
		UCC3GameInstance* CC3GameInstance = Cast<UCC3GameInstance>(GI);
		if (CC3GameInstance) {
			CC3GameInstance->AddScore(Amount);
		}
	}
}


void ACC3GameState::OnItemCollect() {}
void ACC3GameState::StartLevel() {
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
		if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(PC)) {
			PlayerController->ShowGameHUD();
		}
	}

	SpawnedItemCount = 0;
	CollectedItemCount = 0;
	TArray<AActor*> FoundVolume;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolume);
	
	for (int32 i = 0; i < ItemToSpawn; i++) {
		if (FoundVolume.Num() > 0) {
			for (int32 j = 0; j < FoundVolume.Num(); j++) {
				ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolume[j]);
				ItemToSpawn = SpawnVolume->ItemSpawnAmount;
					if (SpawnVolume) {
					AActor* SpawnActor = SpawnVolume->SpawnRandomItem();
					if (SpawnActor && SpawnActor->IsA(APointItem::StaticClass())) {
						SpawnedItemCount++;
					}
				}
			}
		}
	}
}
void ACC3GameState::EndWave() {
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	bool bIsEnd = false;
	float KeepTime = 0;
	if (UGameInstance* GI = GetGameInstance()) {
		UCC3GameInstance* GameInstance = Cast<UCC3GameInstance>(GI);
		if (GameInstance) {
			bool bHasScore = true;
			if (GameInstance->CurrentLevel == 1) {
				bHasScore = GameInstance->TotalScore >= GameInstance->Level1Score;
			}
			else if (GameInstance->CurrentLevel == 2) {
				bHasScore = GameInstance->TotalScore >= GameInstance->Level2Score;
			}
			else if (GameInstance->CurrentLevel == 3) {
				bHasScore = GameInstance->TotalScore >= GameInstance->Level3Score;
			}
			if (GameInstance->CurrentWave == 3 && !bHasScore) {
				if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
					if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(PC)) {
						PlayerController->ShowGameOver();
					}
				}
				return;
			}
			FWaveScores Scores;
			Scores.Wave = GameInstance->CurrentWave;
			Scores.Level = GameInstance->CurrentLevel;
			Scores.Score = GameInstance->CurrentScore;
			Scores.Time = GameInstance->TotalLevelDuration - RemainingTime;
			GameInstance->WaveScores.Add(Scores);

			if (GameInstance) {
				if (GameInstance->CurrentWave != 3) {
					GameInstance->CurrentScore = 0;
					GameInstance->CurrentWave++;

				}
				else if (GameInstance->CurrentWave == 3 && GameInstance->CurrentLevel != 3) {
					GameInstance->CurrentScore = 0;
					if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
						if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(PC)) {
							if (PlayerController) {
								PlayerController->EndLevel();
							}
						}
					}
					return;
				}
				else {
					bIsEnd = true;
					GameInstance->CurrentWave = 1;
					GameInstance->CurrentLevel = 1;
					if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
						if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(PC)) {
							if (PlayerController) {
								PlayerController->ResultLevel();
							}
						}
					}
				}
				if (!bIsEnd) {
					FString LevelName = FString::Printf(TEXT("Level%d_%d"), GameInstance->CurrentLevel, GameInstance->CurrentWave);
					UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
					GameInstance->TotalLevelDuration = RemainingTime + 40.f + (10.f * GameInstance->CurrentWave);
				}
			}
		}
	}
}
void ACC3GameState::OnLevelTimeUp() {
	EndWave();
}
void ACC3GameState::OnNextLevel() {
	if (UGameInstance* GI = GetGameInstance()) {
		if (UCC3GameInstance* GameInstance = Cast<UCC3GameInstance>(GI)) {
			GameInstance->CurrentWave = 1;
			GameInstance->CurrentLevel++;

			FString LevelName = FString::Printf(TEXT("Level%d_%d"), GameInstance->CurrentLevel, GameInstance->CurrentWave);
			UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
			GameInstance->TotalLevelDuration = RemainingTime + 40.f + (10.f * GameInstance->CurrentWave);
		}
	}
}
void ACC3GameState::UpdateHUD() {
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
		if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(PC)) {
			if (UUserWidget* HUD = PlayerController->GetHUDWidget()) {
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("Text_Time")))) {
					RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), RemainingTime)));
				}
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("Text_CurrentScore")))) {
					if (UGameInstance* GI = GetGameInstance()) {
						UCC3GameInstance* GameInstance = Cast<UCC3GameInstance>(GI);
						if (GameInstance) {
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GameInstance->CurrentScore)));
						}
					}	
				}
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("Text_TotalScore")))) {
					if (UGameInstance* GI = GetGameInstance()) {
						UCC3GameInstance* GameInstance = Cast<UCC3GameInstance>(GI);
						if (GameInstance) {
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GameInstance->TotalScore)));
						}
					}
				}
				if (UTextBlock* WaveText = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("Text_Wave")))) {
					if (UGameInstance* GI = GetGameInstance()) {
						UCC3GameInstance* GameInstance = Cast<UCC3GameInstance>(GI);
						if (GameInstance) {
							WaveText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GameInstance->CurrentWave)));
						}
					}
				}
				if (UTextBlock* WaveText = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("Text_Level")))) {
					if (UGameInstance* GI = GetGameInstance()) {
						UCC3GameInstance* GameInstance = Cast<UCC3GameInstance>(GI);
						if (GameInstance) {
							WaveText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GameInstance->CurrentLevel)));
						}
					}
				}

			}
		}
	}
}
