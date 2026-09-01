


#include "CC3PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "CC3GameInstance.h"
#include "CC3GameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

ACC3PlayerController::ACC3PlayerController()
	: HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	DebuffWidgetClass(nullptr),
	DebuffLogWidgetClass(nullptr),
	DebuffLogWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr),
	PauseMenuWidgetClass(nullptr),
	PauseMenuWidgetInstance(nullptr)
{
	bOnTitle = true;
	bOnPause = false;
	bOnResult = false;
}

void ACC3PlayerController::BeginPlay() {
	Super::BeginPlay();
	bOnResult = false;
	APawn* CurrentPawn = GetPawn();
	if (!CurrentPawn) {
		return;
	}
	// FVector SpawnActorLocation = CurrentPawn ? CurrentPawn->GetActorLocation() : FVector::ZeroVector;
	// FRotator SpawnActorRotation = CurrentPawn ? CurrentPawn->GetActorRotation() : FRotator::ZeroRotator;

	FString CurrentMap = GetWorld()->GetMapName();

	if (CurrentMap.Contains("MenuLevel")) {
		SetPause(true);
		ShowMainMenu(false);
		return;
	}
	if (UCC3GameInstance* GI = Cast<UCC3GameInstance>(UGameplayStatics::GetGameInstance(this))) {
		if (GI->CurrentWave == 1) {
			ShowStartLevel();
			return;
		}
	}
	SetPause(false);
	ShowGameHUD();
	
}
UUserWidget* ACC3PlayerController::GetHUDWidget() const {
	return HUDWidgetInstance;
}
void ACC3PlayerController::ShowGameHUD() {
	if (HUDWidgetInstance) {
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance) {
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (HUDWidgetClass) {
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance) {
			HUDWidgetInstance->AddToViewport();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
			}
		ACC3GameState* GS = GetWorld() ? GetWorld()->GetGameState<ACC3GameState>() : nullptr;
		if (GS) {
			GS->UpdateHUD();
		}
	}

}
void ACC3PlayerController::ShowMainMenu(bool bIsRestart) {
	if (HUDWidgetInstance) {
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance) {
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (MainMenuWidgetClass) {
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance) {
			MainMenuWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}
void ACC3PlayerController::ShowStartLevel() {
	if (SetPause(false)) {
		SetPause(true);
	}
	if (StartLevelWidgetClass) {
		StartLevelWidgetInstance = CreateWidget<UUserWidget>(this, StartLevelWidgetClass);
		if (StartLevelWidgetInstance) {
			StartLevelWidgetInstance->AddToViewport();
			bShowMouseCursor = false;
			SetInputMode(FInputModeUIOnly());
		}
	}
	GetWorldTimerManager().SetTimer(AutoResumeTimerHandle, this, &ACC3PlayerController::EndStartLevel, 4.0f, false);
}

void ACC3PlayerController::StartGame() {
	bOnTitle = false;
	
	if (UCC3GameInstance* GI = Cast<UCC3GameInstance>(UGameplayStatics::GetGameInstance(this))) {
		GI->TotalLevelDuration = 60;
		GI->CurrentLevel = 1;
		GI->CurrentWave = 1;
		GI->TotalScore = 0;
		GI->CurrentScore = 0;
		GI->WaveScores.Empty();
	}
		UGameplayStatics::OpenLevel(GetWorld(), FName("Level1_1"));
		ShowStartLevel();
}
void ACC3PlayerController::EndStartLevel() {
	if (StartLevelWidgetInstance) {
		StartLevelWidgetInstance->RemoveFromParent();
		StartLevelWidgetInstance = nullptr;
	}
	SetInputMode(FInputModeGameOnly());
	SetPause(false);
	if (ACC3GameState* GS = GetWorld()->GetGameState<ACC3GameState>()) {
		GS->WaitUntilAnim();
	}
}
void ACC3PlayerController::ShowPauseMenu() {
	FString CurrentMap = GetWorld()->GetMapName();
	if (CurrentMap.Contains("MenuLevel") || bOnResult) {
		return;
	}
	bOnPause = !bOnPause;
	if (bOnPause) {
		SetPause(true);
		if (HUDWidgetInstance) {
			HUDWidgetInstance->RemoveFromParent();
			HUDWidgetInstance = nullptr;
		}
		if (PauseMenuWidgetClass) {
			PauseMenuWidgetInstance = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
			if (PauseMenuWidgetInstance) {
				PauseMenuWidgetInstance->AddToViewport();
				bShowMouseCursor = true;
				SetInputMode(FInputModeUIOnly());
			}
		}
	}
	else {
		if (PauseMenuWidgetInstance) {
			PauseMenuWidgetInstance->RemoveFromParent();
			PauseMenuWidgetInstance = nullptr;
		}
		ShowGameHUD();
		SetPause(false);
	}
}
void ACC3PlayerController::MoveToTitle() {
	if (HUDWidgetInstance) {
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (PauseMenuWidgetInstance) {
		PauseMenuWidgetInstance->RemoveFromParent();
		PauseMenuWidgetInstance = nullptr;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
	SetPause(true);
}
void ACC3PlayerController::ShowGameOver() {
	SetPause(true);
	if (HUDWidgetInstance) {
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	bOnResult = true;
	if (GameOverWidgetClass) {
		GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		if (GameOverWidgetInstance) {
			GameOverWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}
void ACC3PlayerController::EndLevel() {
	SetPause(true);
	if (HUDWidgetInstance) {
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (EndLevelWidgetClass) {
		EndLevelWidgetInstance = CreateWidget<UUserWidget>(this, EndLevelWidgetClass);
		if (EndLevelWidgetInstance) {
			EndLevelWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}
void ACC3PlayerController::ResultLevel() {
	bOnResult = true;
	SetPause(true);
	if (HUDWidgetInstance) {
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (ResultWidgetClass) {
		ResultWidgetInstance = CreateWidget<UUserWidget>(this, ResultWidgetClass);
		if (ResultWidgetInstance) {
			ResultWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
	// if ("MenuLevel" != GetWorld()->GetMapName()) {
	// 	MoveToTitle();
	// }
	
}
void ACC3PlayerController::UpdateDebuff(FName name, bool bIsOn) {
	if (!HUDWidgetInstance) {
		return;
	}
	UUserWidget* DebuffCanvas = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("WBP_Debuff")));
	if (!DebuffCanvas) {
		return;
	}
	if (UImage* SlowIcon = Cast<UImage>(DebuffCanvas->GetWidgetFromName(name))) {
		if (bIsOn) {
			SlowIcon->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			SlowIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
void ACC3PlayerController::UpdateDebuffLog(FString Log) {
	if (!HUDWidgetInstance) {
		return;
	}
	UUserWidget* DebuffLogBox = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("WBP_DebuffLog")));
	if (!DebuffLogBox) {
		return;
	}
	if (UTextBlock* DebuffLog = Cast<UTextBlock>(DebuffLogBox->GetWidgetFromName("Text_Debuff"))) {
		DebuffLog->SetText(FText::FromString(Log));
	}
	GetWorldTimerManager().SetTimer(LogTimerHandle, this, &ACC3PlayerController::RemoveDebuffLog, LogTimer, false);
		
}
void ACC3PlayerController::RemoveDebuffLog() {
	UUserWidget* DebuffLogBox = Cast<UUserWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("WBP_DebuffLog")));
	if (!DebuffLogBox) {
		return;
	}
	if (UTextBlock* DebuffLog = Cast<UTextBlock>(DebuffLogBox->GetWidgetFromName("Text_Debuff"))) {
		DebuffLog->SetText(FText::FromString(TEXT("")));
	}
}
