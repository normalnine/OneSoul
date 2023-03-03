// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossAnim.h"
#include "EnemyBoss.h"
#include "../OneSoulCharacter.h"
#include <Components/CapsuleComponent.h>
#include "EnemyBossFSM.h"

void UEnemyBossAnim::NativeBeginPlay()
{
	//GetOwningActor(); <<-------- Actor 를 가져온다.
	/*AActor* actor = GetOwningActor();
	APawn* p =Cast<APawn>(actor);*/

	//1. 내가 붙어 있는 Pawn 을 가져온다.
	APawn* pawn = TryGetPawnOwner();

	//2. AEnemy 로 형변환한다.
	enemy = Cast<AEnemyBoss>(pawn);
}

void UEnemyBossAnim::AnimNotify_DieEnd()
{
	//3. enemy 가 가지고 있는 fsm 을 이용해서 bDieMove 를 true
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
