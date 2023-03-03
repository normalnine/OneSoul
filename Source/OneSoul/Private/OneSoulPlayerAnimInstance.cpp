// Fill out your copyright notice in the Description page of Project Settings.


#include "OneSoulPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OnsSoulPlayer.h"
#include "Kismet/KismetMathLibrary.h"

void UOneSoulPlayerAnimInstance::NativeBeginPlay()
{
  Super::NativeBeginPlay();

  Montage_Play(LevelStart);
}

void UOneSoulPlayerAnimInstance::NativeInitializeAnimation()
{
 Super::NativeInitializeAnimation();

 OneSoulCharacter= Cast<AOnsSoulPlayer>(TryGetPawnOwner());

 if (OneSoulCharacter)
 {
	OneSoulCharacterMovement = OneSoulCharacter->GetCharacterMovement();
 }
}

void UOneSoulPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
 Super::NativeUpdateAnimation(DeltaSeconds);

 if (OneSoulCharacter == nullptr)
 {
  OneSoulCharacter = Cast<AOnsSoulPlayer>(TryGetPawnOwner());
 }
 
 if (OneSoulCharacter)
 {
	 //Set Speed
	 FVector Velocity{ OneSoulCharacter->GetVelocity() };
	 Velocity.Z = 0;
	 GroundSpeed= Velocity.Size();

	 //Set IsinAir
	 bIsInAir = OneSoulCharacterMovement->IsFalling();

	 FRotator AnimRotation = OneSoulCharacter->GetBaseAimRotation();
	 FRotator MovementRotation = OneSoulCharacter->GetActorRotation();

	 Roll = UKismetMathLibrary::NormalizedDeltaRotator(
		                       MovementRotation, AnimRotation).Roll;
	 Pitch = UKismetMathLibrary::NormalizedDeltaRotator(
		                       MovementRotation, AnimRotation).Pitch;
	 Yaw = UKismetMathLibrary::NormalizedDeltaRotator(
		                       MovementRotation, AnimRotation).Yaw;
     
	 FRotator RotationLastTick = OneSoulCharacter->GetActorRotation();
	 float Target = UKismetMathLibrary::NormalizedDeltaRotator(RotationLastTick, OneSoulCharacter->GetActorRotation()).Yaw;

	 YawDelta = UKismetMathLibrary::FInterpTo(
	                                YawDelta,
									Target/DeltaSeconds,
									DeltaSeconds,
									6.f
									);

	//Set is Accelerating
	 if (OneSoulCharacterMovement->GetCurrentAcceleration().Size() > 0)
	 {
		 bIsAcclerating = true;
	 }
	 else
	 {
		 bIsAcclerating = false;
	 }

	 
 }

}

