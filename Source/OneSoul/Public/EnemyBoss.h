// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBoss.generated.h"

UCLASS()
class ONESOUL_API AEnemyBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
	class UEnemyBossFSM* fsm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEnemyBossAnim* anim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* capsuleCompHead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bOverlap = true;

	// 활성 / 비활성 하는 기능
	void SetActive(bool bActive);

	UFUNCTION()
	void BeginOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);
	
	UFUNCTION()
	void EndOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//죽었을 때 호출되는 함수를 담는 변수
	//FEnemyBossDieDelegate dieDelegate;
};
