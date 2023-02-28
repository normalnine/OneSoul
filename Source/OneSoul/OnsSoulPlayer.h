// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OnsSoulPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ONESOUL_API AOnsSoulPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AOnsSoulPlayer();

protected:
	virtual void BeginPlay() override;
    void MoveForward(float Value);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
UPROPERTY(VisibleAnywhere)
USpringArmComponent* SpringArm;

UPROPERTY(VisibleAnywhere)
UCameraComponent* Camera;

};
