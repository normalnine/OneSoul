// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossAnim.h"
#include "EnemyBoss.h"
#include "../OneSoulCharacter.h"
#include <Components/CapsuleComponent.h>
#include "EnemyBossFSM.h"

void UEnemyBossAnim::NativeBeginPlay()
{
	//GetOwningActor(); <<-------- Actor �� �����´�.
	/*AActor* actor = GetOwningActor();
	APawn* p =Cast<APawn>(actor);*/

	//1. ���� �پ� �ִ� Pawn �� �����´�.
	APawn* pawn = TryGetPawnOwner();

	//2. AEnemy �� ����ȯ�Ѵ�.
	enemy = Cast<AEnemyBoss>(pawn);
}

void UEnemyBossAnim::AnimNotify_DieEnd()
{
	//3. enemy �� ������ �ִ� fsm �� �̿��ؼ� bDieMove �� true
	enemy->fsm->bDieMove = true;

	//Cast<AEnemy>(TryGetPawnOwner())->fsm->bDieMove = true;
}

void UEnemyBossAnim::AnimNotify_AttackStartHead()
{
	enemy->capsuleCompHead->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	enemy->capsuleCompHead->SetHiddenInGame(false);
}

void UEnemyBossAnim::AnimNotify_AttackEndHead()
{
	enemy->capsuleCompHead->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	enemy->capsuleCompHead->SetHiddenInGame(true);

}

void UEnemyBossAnim::AnimNotify_Fireball()
{
	enemy->fsm->SpawnFireball();
}

void UEnemyBossAnim::AnimNotify_FireSpread()
{
	enemy->fsm->SpawnFireSpread();
}
