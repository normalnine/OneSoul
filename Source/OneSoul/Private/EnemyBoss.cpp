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

	// �ִϸ��̼� �������Ʈ �Ҵ��ϱ�
	ConstructorHelpers::FClassFinder<UEnemyBossAnim> tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KDH/Blueprints/ABP_MountainDragon.ABP_MountainDragon_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}

	// ���忡 ��ġ�ǰų� ������ �� �ڵ�����
	// AIController ���� Process �� �� �ֵ��� ����
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
	// Ȱ��ȭ
	if (bActive)
	{
		//�浹 Ȱ��
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//���� ��ġ �缳��
		fsm->originPos = GetActorLocation();
	}
	// ��Ȱ��ȭ
	else
	{
		//�浹 ��Ȱ��
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//źâ�� �� �ٽ� �־���
		//dieDelegate.ExecuteIfBound(this);
	}

	//�޽��� Ȱ�� / ��Ȱ��
	GetMesh()->SetActive(bActive);
	//�޽��� ���̰� / �Ⱥ��̰�
	GetMesh()->SetVisibility(bActive);
	//ĳ���� �����Ʈ Ȱ�� / ��Ȱ��
	GetCharacterMovement()->SetActive(bActive);
	//fsm Ȱ�� / ��Ȱ��
	fsm->SetActive(bActive);
}

void AEnemyBoss::BeginOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (bOverlap)
	{
		// �÷��̾� ������ �Լ� ȣ��
		bOverlap = false;
	}
}

void AEnemyBoss::EndOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlap = true;
}

