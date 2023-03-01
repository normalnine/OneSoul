// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"
#include "OnsSoulPlayer.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TargetingDistance = 900.f;
	TargetingRadius = 100.f;
	TargetRotationInterpSpeed = 9.f;
}


void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

  //플레이어 캐릭터 
  OwnerCharacter = Cast<AOnsSoulPlayer>(GetOwner());

  //플레이어 컨트롤러
  OwnerController= OwnerCharacter->GetController();

  //플레이어 카메라
  FollowerCamera=Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(FCamera));

}



void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsTargeting)
	{
	  UpdateTargetingControlRotation();
	}
	
}

void UTargetingComponent::EnableLockOn()
{
 FVector Start = GetOwner()->GetActorLocation();
 FVector End = FollowerCamera-> GetForwardVector() * TargetingDistance + Start;
 TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
 TargetObjectTypes.Add(Pawn);

 TArray<AActor*> IgnoreActors;
IgnoreActors.Add(GetOwner());

 FHitResult HitResult;

bool Result = UKismetSystemLibrary::SphereTraceSingleForObjects(
                       GetWorld(),
					   Start,
					   End,
					   TargetingRadius,
					   TargetObjectTypes,
					   false,
					   IgnoreActors,
					   EDrawDebugTrace::None,
					   HitResult,
					   true);

auto HitActor = UKismetSystemLibrary::IsValid(HitResult.GetActor());

if (Result==true && HitActor)
{
  TargetActor=HitResult.GetActor();
  bIsTargeting= true;

}

}

void UTargetingComponent::DisableLockOn()
{
 bIsTargeting=false;

}

void UTargetingComponent::ToggleLockOn()
{
	if (!bIsTargeting)
	{
	   EnableLockOn();
	}
	else
	{
		DisableLockOn();
	}
}

void UTargetingComponent::UpdateTargetingControlRotation()
{
 auto OChar = UKismetSystemLibrary::IsValid(OwnerCharacter);
 auto Target = UKismetSystemLibrary::IsValid(TargetActor);

	if (OChar && Target)
	{
	FVector Start = GetOwner()->GetActorLocation();
	FVector TargetR = TargetActor->GetActorLocation() - FVector(0.f,0.f,100.f);
	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(Start,TargetR);
	FRotator CurrentRot = OwnerCharacter->GetActorRotation();

	FRotator YZ = UKismetMathLibrary::RInterpTo(
	                                  CurrentRot,
						              TargetRot,
						              GetWorld()->DeltaTimeSeconds,
					                  TargetRotationInterpSpeed);
	 

	FRotator NewRotation = UKismetMathLibrary::MakeRotator(CurrentRot.Roll,YZ.Pitch,YZ.Yaw);

	 OwnerController->SetControlRotation(NewRotation);
    }
}
