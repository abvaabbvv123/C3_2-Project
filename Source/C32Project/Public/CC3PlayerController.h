// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CC3PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class C32PROJECT_API ACC3PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACC3PlayerController();

	virtual void  BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> DebuffWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> DebuffLogWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* DebuffLogWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* MainMenuWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* PauseMenuWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> StartLevelWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* StartLevelWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> EndLevelWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* EndLevelWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> ResultWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* ResultWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* GameOverWidgetInstance;

	float LogTimer = 5.0f;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowPauseMenu();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void StartGame();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void MoveToTitle();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameOver();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowStartLevel();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void EndLevel();
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ResultLevel();
	UFUNCTION(BlueprintCallable)
	void EndStartLevel();

	void UpdateDebuffLog(FString Log);
	void RemoveDebuffLog();
	void UpdateDebuff(FName name, bool bIsOn);

	bool bOnTitle;
	bool bOnPause;
	bool bOnResult;

private:
	FTimerHandle AutoResumeTimerHandle;
	FTimerHandle LogTimerHandle;
};
