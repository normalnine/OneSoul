// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyBossFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyBossState : uint8
{
	Idle,
	Move,
	Attack,
	AttackDelay,
	Damaged,
	Die,
	ReturnPos
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONESOUL_API UEnemyBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	//�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEnemyBossState currState = EEnemyBossState::Idle;

	//Ÿ��
	UPROPERTY(EditAnywhere)
		class AOneSoulCharacter* target;
	//��
	UPROPERTY(EditAnywhere)
		class AEnemyBoss* me;

	//anim instance
	UPROPERTY(EditAnywhere)
		class UEnemyBossAnim* anim;

	//anim montage
	UPROPERTY(EditAnywhere)
		class UAnimMontage* damageMontage;

	//ai controller 
	UPROPERTY(EditAnywhere)
		class AAIController* ai;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyBossFireball> FireballFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyBossFireSpread> FireSpreadFactory;

	//����ð�
	float currTime = 0;
	//��� ���� ��ٸ��� �ð�
	float idleDelayTime = 2;

	//�Ѿ� �� �� �ִ� ����
	float traceRange = 1000;

	//���ݹ���
	float attackRange = 1000;
	//���� ��� �ð�
	float attackDelayTime = 2;

	//�ǰ� ��� �ð�
	float damageDelayTime = 2;

	//���� ü��
	float currHP;
	//�ִ� ü��
	float maxHP = 1;
	//���� ��ȣ��
	float currShieldGauge;
	//�ִ� ��ȣ��
	float maxShieldGauge = 100;

	//�׾��� �� �������� �ӷ�
	float dieSpeed = 100;
	//�׾��� �� �������� �ϴ� ����
	bool bDieMove = false;

	//�̵��� �� �ִ� �ݰ�
	float moveRange = 2000;
	//ó�� ��ġ�� ��Ƴ��� ����
	FVector originPos;

	//������ ��ġ
	FVector randPos;

public:
	//���
	void UpdateIdle();
	//�̵�
	void UpdateMove();
	//����
	void UpdateAttack();
	//���� ���
	void UpdaetAttackDelay();
	//�ǰ�
	void UpdateDamaged();
	//����
	void UpdateDie();
	//����
	void UpdateReturnPos();

	//���� ����� �ѹ���!!! ȣ�� �Ǵ� �Լ�
	void ChangeState(EEnemyBossState state);

	//���� �޾��� �� ó���ϴ� �Լ�
	void ReceiveDamage();

	//Delay �Լ�
	bool IsWaitComplete(float delayTime);

	//Ÿ���� �Ѿ� �� �� �ִ�?
	bool IsTargetTrace();

	//�ش� ��ġ���� �����ϸ� ���¸� Idle �� ��ȯ�ϴ� �Լ�
	void MoveToPos(FVector pos);

	//���� ���� ���ϱ�
	void DecideAttackPattern();

	//���� ���� �ʱ�ȭ
	void InitAttackPattern();

	//Fireball ��ȯ
	void SpawnFireball();

	//FireSpread ��ȯ
	void SpawnFireSpread();
};
