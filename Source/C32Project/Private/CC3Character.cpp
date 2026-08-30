// Fill out your copyright notice in the Description page of Project Settings.


#include "CC3Character.h"
#include "CC3PlayerController.h"
#include "CC3GameState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACC3Character::ACC3Character()
	:InputMappingContext(nullptr),MoveAction(nullptr),LookAction(nullptr),JumpAction(nullptr),SprintAction(nullptr),WalkAction(nullptr)
{
 	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->AirControl = 0.4f;
	GetCharacterMovement()->GravityScale = 3.2f;
	GetCharacterMovement()->JumpZVelocity = 780.0f;
	JumpMaxHoldTime = 0.25f;
	JumpMaxCount = 2;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 350.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	HeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("On Head Widget"));
	HeadWidget->SetupAttachment(GetMesh());
	HeadWidget->SetWidgetSpace(EWidgetSpace::Screen);


	MaxHealth = 100.f;
	Health = MaxHealth;
	MaxStamina = 100.f;
	Stamina = MaxStamina;
	NormalSpeed = 550.f;
	BasicSpeed = 550.f;
	SprintMultiplier = 1.6f;
	WalkMutiplier = 0.5f;
	SlowMultiplier = 0.5f;
	SprintSpeed = NormalSpeed * SprintMultiplier;
	WalkSpeed = NormalSpeed * WalkMutiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

// Called when the game starts or when spawned
void ACC3Character::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(Controller)) {
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
				if (InputMappingContext) {
					Subsystem->AddMappingContext(InputMappingContext, 0);
				}
			}
		}
	}
}

 
void ACC3Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetCharacterMovement()) return;

	const FVector HorizontalVelocity = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, 0.f);
	bIsMoving = HorizontalVelocity.SizeSquared() > FMath::Square(MovingThreshold);

}

// Called to bind functionality to input
void ACC3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Inputs = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (MoveAction) {
			Inputs->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACC3Character::Move);
		}
		if (JumpAction) {
			Inputs->BindAction(JumpAction, ETriggerEvent::Started, this, &ACC3Character::StartJump);
			Inputs->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACC3Character::StopJump);
		}
		if (SprintAction) {
			Inputs->BindAction(SprintAction, ETriggerEvent::Started, this, &ACC3Character::StartSprint);
			Inputs->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACC3Character::StopSprint);
		}
		if (WalkAction) {
			Inputs->BindAction(WalkAction, ETriggerEvent::Started, this, &ACC3Character::StartWalk);
			Inputs->BindAction(WalkAction, ETriggerEvent::Completed, this, &ACC3Character::StopWalk);
		}
		if (LookAction) {
			Inputs->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACC3Character::Look);
		}
		if (PauseAction) {
			Inputs->BindAction(PauseAction, ETriggerEvent::Started, this, &ACC3Character::Pause);
		}
	}
}
//Inputs
//
void ACC3Character::UpdateMovement() {
	if (!GetCharacterMovement()) {
		return;
	}
	if (bIsSprint) {
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * SprintMultiplier;
	}
	else if (bIsWalk) {
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * WalkMutiplier;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
} 
void ACC3Character::Move(const FInputActionValue& value) {
	if (!Controller) {
		return;
	}

	FVector2D MoveInput = value.Get<FVector2D>();
	bIsMoving = !MoveInput.IsNearlyZero();
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (bIsInputReverse) {
		MoveInput *= -1.f;
	}
		if (!FMath::IsNearlyZero(MoveInput.X)) {
		AddMovementInput(ForwardDirection, MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y)) {
		AddMovementInput(RightDirection, MoveInput.Y);
	}
}
void ACC3Character::StartJump(const FInputActionValue& value) {
	if (Stamina < StaminaCost) {
		return;
	}
	Jump();
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.0f, MaxStamina);
	OnStaminaChanged.Broadcast(Stamina);
	
}
void ACC3Character::StopJump(const FInputActionValue& value) {
	StopJumping();
	if (Stamina < MaxStamina) {
		GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &ACC3Character::RegenStamina, StaminaTickInterval, true);
	}
}
void ACC3Character::StartSprint(const FInputActionValue& value) {

	if (GetCharacterMovement() && Stamina > 0) {
		//GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * SprintMutiplier;
		bIsSprint = true;
		UpdateMovement();
	}
	GetWorldTimerManager().ClearTimer(StaminaTimerHandle);
	GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &ACC3Character::UseStamina, StaminaTickInterval, true);
}
void ACC3Character::StopSprint(const FInputActionValue& value) {
	if (GetCharacterMovement()) {
		//GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		bIsSprint = false;
		UpdateMovement();
	}
	GetWorldTimerManager().ClearTimer(StaminaTimerHandle);
	if (Stamina < MaxStamina) {
		GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &ACC3Character::RegenStamina, StaminaTickInterval, true);
	}
}
void ACC3Character::StartWalk(const FInputActionValue& value) {
	if (GetCharacterMovement()) {
		//GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		bIsWalk = true;
		UpdateMovement();
	}
}
void ACC3Character::StopWalk(const FInputActionValue& value) {
	if (GetCharacterMovement()) {
		//GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		bIsWalk = false;
		UpdateMovement();
	}
}
void ACC3Character::Look(const FInputActionValue& value) {
	if (bIsCamerafixed) {
		return;
	}
	FVector2D LookInput = value.Get<FVector2D>();
	if (bIsLookReverse) {
		LookInput *= -1;
	}
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}
void ACC3Character::Pause(const FInputActionValue& value) {
	if (ACC3PlayerController* PC = Cast<ACC3PlayerController>(GetWorld()->GetFirstPlayerController())) {
		PC->ShowPauseMenu();
	}
}
//Getters
//
float ACC3Character::GetHealth() const {
	return Health;
}
float ACC3Character::GetStamina() const {
	return Stamina;
}
float ACC3Character::GetSpeed() const {
	return NormalSpeed;
}
//Functions
//
void ACC3Character::AddHealth(float Amount) {
	Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}
void ACC3Character::AddStamina(float Amount) {
	Stamina = FMath::Clamp(Stamina + Amount, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(Stamina);
}
float ACC3Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCasuer) {
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCasuer);
	Health = FMath::Clamp(Health - ActualDamage, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	if (Health <= 0.f) {
		OnDeath();
	}
	return ActualDamage;
}
//Staminas
////
void ACC3Character::UseStamina() {
	const bool bOnAir = GetCharacterMovement() && GetCharacterMovement()->IsFalling();

	if (bIsMoving && !bOnAir) {
		Stamina = FMath::Clamp(Stamina - StaminaCost * StaminaTickInterval, 0.0f, MaxStamina);
		OnStaminaChanged.Broadcast(Stamina);
	}
	if (Stamina <= 0.f) {
		if (GetCharacterMovement()) {
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		}
		GetWorldTimerManager().ClearTimer(StaminaTimerHandle);
	}
}
void ACC3Character::RegenStamina() {
	if (GetCharacterMovement() && GetCharacterMovement()->IsFalling()) {
		return;
	}
	Stamina = FMath::Clamp(Stamina + StaminaRegen * StaminaTickInterval, 0.0f, MaxStamina);
	OnStaminaChanged.Broadcast(Stamina);

	if (Stamina >= MaxStamina) {
		GetWorldTimerManager().ClearTimer(StaminaTimerHandle);
	}
}

void ACC3Character::OnDeath() {
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->ShowGameOver();
	}
}
//debuffs
////
void ACC3Character::ApplySlow(float Strength, float Duration) {
	if (!GetCharacterMovement()) {
		return;
	}
	
	if (!bIsSlowed) {
		NormalSpeed = NormalSpeed * Strength;
	}
	bIsSlowed = true;
	UpdateMovement();
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_Slow", true);
	}
	
	GetWorldTimerManager().ClearTimer(SlowTimerHandle);
	GetWorldTimerManager().SetTimer(SlowTimerHandle, this, &ACC3Character::RevertSlow, Duration, false);
}
void ACC3Character::RevertSlow() {
	bIsSlowed = false;
	if (GetCharacterMovement()) {
		NormalSpeed = BasicSpeed;
	}
	UpdateMovement();
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_Slow", false);
	}
}
void ACC3Character::ApplyBlind(float Duration) {	
	bIsBlinded = true;
	if (BlindPostProcessMaterial && CameraComp) {
		if (!BlindDynamicMaterial) {
			BlindDynamicMaterial = UMaterialInstanceDynamic::Create(BlindPostProcessMaterial,this);
			FWeightedBlendable Blendable(1.0f, BlindDynamicMaterial);
			CameraComp->PostProcessSettings.bOverride_AutoExposureMethod = true;
			CameraComp->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
			CameraComp->PostProcessSettings.WeightedBlendables.Array.Add(Blendable);
			CameraComp->PostProcessSettings.AutoExposureMinBrightness = 1.f;
		}
		BlindDynamicMaterial->SetScalarParameterValue(TEXT("BlindAlpha"), 1.f);
	}
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_Blind", true);
	}
	GetWorldTimerManager().ClearTimer(BlindTimerHandle);
	GetWorldTimerManager().SetTimer(BlindTimerHandle, this, &ACC3Character::RevertBlind, Duration, false);
}
void ACC3Character::RevertBlind() {
	bIsBlinded = false;
	if (BlindDynamicMaterial) {
		CameraComp->PostProcessSettings.bOverride_AutoExposureMethod = false;
		CameraComp->PostProcessSettings.AutoExposureMinBrightness = false;
		BlindDynamicMaterial->SetScalarParameterValue(TEXT("BlindAlpha"), 0.f);
	}
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_Blind", false);
	}
}
void ACC3Character::ApplyCameraFix(float Duration) {
	bIsCamerafixed = true;
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_CameraLock", true);
	}
	GetWorldTimerManager().ClearTimer(CameraFixTimerHandle);
	GetWorldTimerManager().SetTimer(CameraFixTimerHandle, this, &ACC3Character::RevertCameraFix, Duration, false);
}
void ACC3Character::RevertCameraFix() {
	bIsCamerafixed = false;
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_CameraLock", false);
	}
}
void ACC3Character::ApplyMoveReverse(float Duration) {
	bIsInputReverse = true;
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_MoveReverse", true);
	}
	GetWorldTimerManager().ClearTimer(MoveReverseTimerHandle);
	GetWorldTimerManager().SetTimer(MoveReverseTimerHandle, this, &ACC3Character::RevertMoveReverse, Duration, false);
}
void ACC3Character::RevertMoveReverse() {
	bIsInputReverse = false;
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_MoveReverse", false);
	}
}
void ACC3Character::ApplyLookReverse(float Duration) {
	bIsLookReverse = true;
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_LookReverse", true);
	}
	GetWorldTimerManager().ClearTimer(LookReverseTimerHandle);
	GetWorldTimerManager().SetTimer(LookReverseTimerHandle, this, &ACC3Character::RevertLookReverse, Duration, false);
}
void ACC3Character::RevertLookReverse() {
	bIsLookReverse = false;
	if (ACC3PlayerController* PlayerController = Cast<ACC3PlayerController>(GetController())) {
		PlayerController->UpdateDebuff("Debuff_LookReverse", false);
	}
}
void ACC3Character::Cleanse() {
	RevertSlow();
	RevertBlind();
	RevertCameraFix();
	RevertMoveReverse();
	RevertLookReverse();
}
