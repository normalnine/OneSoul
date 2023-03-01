// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OnsSoulPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	OrienttoCamera,
	OrienttoMovement
};

UCLASS()
class ONESOUL_API AOnsSoulPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AOnsSoulPlayer();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Sprint")
	float SprintSpeed = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float WalkSpeed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MinStamina = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	bool bIsTargeting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetRotationInterpSpeed;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ERotationMode RotationMode = ERotationMode::OrienttoCamera;
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;

	void EnableLockOn();
	void DisableLockOn();
	void ToggleLockOnInput();
	void UpdateTargetingControlRotation();
	void StartSprint();
	void SetPlayerMaxSpeed(float MaxSpeed);
	void SprintTimer();
	void StopSprint();

protected:
	virtual void BeginPlay() override;
    void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void ToggleLockOn();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

UPROPERTY(EditAnywhere)
class AActor* TargetActor;

private:
UPROPERTY(VisibleAnywhere)
USpringArmComponent* SpringArm;

UPROPERTY(VisibleAnywhere)
UCameraComponent* Camera;

class AController* Controller;


};
