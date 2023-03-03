// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"
#include "EnemyBossFSM.h"
#include "EnemyBossAnim.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AEnemyBoss::AEnemyBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleCompHead = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Head Collision"));
	//capsuleComp->SetupAttachment(GetMesh());
	capsuleCompHead->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MOUNTAIN_DRAGON_-HeadSocket"));
	capsuleCompHead->SetRelativeRotation(FRotator(0,30,90));
	capsuleCompHead->SetCapsuleSize(70.0f, 150.0f);
	capsuleCompHead->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/QuadrapedCreatures/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON'"));
	if (tempMesh.Succeeded())
	{

		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	fsm = CreateDefaultSubobject<UEnemyBossFSM>(TEXT("FSM"));

	// 애니메이션 블루프린트 할당하기
	ConstructorHelpers::FClassFinder<UEnemyBossAnim> tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KDH/Blueprints/ABP_MountainDragon.ABP_MountainDragon_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}

	// 월드에 배치되거나 스폰될 때 자동으로
	// AIController 부터 Process 될 수 있도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	capsuleCompHead->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss::BeginOverlapHead);
	capsuleCompHead->OnComponentEndOverlap.AddDynamic(this, &AEnemyBoss::EndOverlapHead);
}

void AEnemyBoss::SetActive(bool bActive)
{
	// 활성화
	if (bActive)
	{
		//충돌 활성
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//생성 위치 재설정
		fsm->originPos = GetActorLocation();
	}
	// 비활성화
	else
	{
		//충돌 비활성
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//탄창에 날 다시 넣어줘
		//dieDelegate.ExecuteIfBound(this);
	}

	//메쉬를 활성 / 비활성
	GetMesh()->SetActive(bActive);
	//메쉬를 보이고 / 안보이고
	GetMesh()->SetVisibility(bActive);
	//캐릭터 무브먼트 활성 / 비활성
	GetCharacterMovement()->SetActive(bActive);
	//fsm 활성 / 비활성
	fsm->SetActive(bActive);
}

void AEnemyBoss::BeginOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (bOverlap)
	{
		// 플레이어 데미지 함수 호출
		bOverlap = false;
	}
}

void AEnemyBoss::EndOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlap = true;
}

