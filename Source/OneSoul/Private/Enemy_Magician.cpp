// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Magician.h"
AEnemy_Magician::AEnemy_Magician()
{
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
	StaffMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwoedMeshComp"));

	//���̷��� �ȿ� �ٿ��ֱ�
	StaffMeshComp->SetupAttachment(GetMesh(), "Bip01-R-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempStaffMesh(TEXT("/Script/Engine.StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/Cartoon_Weapon_Pack/FBX/Staff_03.Staff_03'"));
	if (TempStaffMesh.Succeeded())
	{
		//4-4.���̷�Ż�޽� �������Ҵ�
		StaffMeshComp->SetStaticMesh(TempStaffMesh.Object);

	}
}