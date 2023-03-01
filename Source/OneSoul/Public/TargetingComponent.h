// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONESOUL_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UTargetingComponent();

protected:

	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	void EnableLockOn();
	void DisableLockOn();
	void ToggleLockOn();
	void UpdateTargetingControlRotation();


	bool bIsTargeting=false;

	float TargetingDistance;
	float TargetingRadius;
	float TargetRotationInterpSpeed;
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;

	class AOnsSoulPlayer* OwnerCharacter;
	class AController* OwnerController;
	TSubclassOf<class UCameraComponent>FCamera;
	class UCameraComponent* FollowerCamera;
	class AActor* TargetActor;
		
};
