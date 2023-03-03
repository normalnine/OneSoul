// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OnsSoulPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wing")
    bool SetParachute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swimming")
	bool IsSwimming = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ERotationMode RotationMode = ERotationMode::OrienttoCamera;
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;
	

	UFUNCTION(BlueprintCallable,Category="Targeting")
	void EnableLockOn();
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void DisableLockOn();
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void ToggleLockOnInput();
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void UpdateTargetingControlRotation();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprint();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void SetPlayerMaxSpeed(float MaxSpeed);
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void SprintTimer();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprint();
	UFUNCTION(BlueprintCallable, Category = "Wing")
	void Wings();
	
protected:
	virtual void BeginPlay() override;
    void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void TurnAtRate(float Rate);
	void LookUpRate(float Rate);
	void ToggleLockOn();


public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

UPROPERTY(EditAnywhere)
class AActor* TargetActor;
class AController* Controller;
UPROPERTY(VisibleAnywhere)
USkeletalMeshComponent* Wing;

private:

UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera, meta = (AllowPrivateAccess = "true"))
USpringArmComponent* SpringArm;
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
UCameraComponent* Camera;
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
float BaseTurnRate;
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
float BaseLookUpRate;


};
