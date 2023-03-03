// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Skeleton.h"
#include "Enemy_Skeleton_FSM.h"

AEnemy_Skeleton::AEnemy_Skeleton()
{
	//���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SKELETON.SKELETON'"));
	//������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//�������Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//�޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	//Į�޽� ����
	SwordMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwoedMeshComp"));

	//���̷��� �ȿ� �ٿ��ֱ�
	SwordMeshComp->SetupAttachment(GetMesh(), "Bip01-R-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSworodMesh(TEXT("/Script/Engine.StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SWORD.SWORD'"));
	if (TempSworodMesh.Succeeded())
	{
		//4-4.���̷�Ż�޽� �������Ҵ�
		SwordMeshComp->SetStaticMesh(TempSworodMesh.Object);

	}

	//���� �޽� ����
	ShieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMeshComp"));

	//���̷��� �ȿ� �ٿ��ֱ�
	ShieldMeshComp->SetupAttachment(GetMesh(), "Bip01-L-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempShieldMesh(TEXT("/Script/Engine.StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SHIELD.SHIELD'"));
	if (TempShieldMesh.Succeeded())
	{
		//4-4.���̷�Ż�޽� �������Ҵ�
		ShieldMeshComp->SetStaticMesh(TempShieldMesh.Object);

	}

	fsm = CreateDefaultSubobject<UEnemy_Skeleton_FSM>(TEXT("FSM"));
}
