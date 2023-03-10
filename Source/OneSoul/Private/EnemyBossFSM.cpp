// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossFSM.h"
#include <Kismet/GameplayStatics.h>
#include "OneSoulCharacter.h"
#include "EnemyBoss.h"
#include "EnemyBossAnim.h"
#include <AIModule/Classes/AIController.h>
#include <NavigationSystem/Public/NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/CapsuleComponent.h>
#include "EnemyBossFireball.h"
#include "EnemyBossFireSpread.h"
#include "OnsSoulPlayer.h"

// Sets default values for this component's properties
UEnemyBossFSM::UEnemyBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UEnemyBossFSM::BeginPlay()
{
	Super::BeginPlay();

	//타겟 찾자
	target = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));	//나를 찾자
	me = Cast<AEnemyBoss>(GetOwner());

	//anim instance 찾자
	/*USkeletalMeshComponent* mesh = me->GetMesh();
	UAnimInstance animInstance = mesh->GetAnimInstance();
	anim = Cast<UEnemyAnim>(animInstance);*/
	anim = Cast<UEnemyBossAnim>(me->GetMesh()->GetAnimInstance());

	//ai controller 찾자
	ai = Cast<AAIController>(me->GetController());
	//ai = UAIBlueprintHelperLibrary::GetAIController(me);

	//나의 초기 체력을 셋팅하자
	currHP = maxHP;

	//나의 초기 쉴드게이지를 셋팅하자
	currShieldGauge = maxShieldGauge;

	//나의 초기 위치를 저장하자
	originPos = me->GetActorLocation();
	
}


// Called every frame
void UEnemyBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currState)
	{
	case EEnemyBossState::Idle:
		UpdateIdle();
		break;
	case EEnemyBossState::Move:
		UpdateMove();
		break;
	case EEnemyBossState::Attack:
		UpdateAttack();
		break;
	case EEnemyBossState::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemyBossState::Damaged:
		UpdateDamaged();
		break;
	case EEnemyBossState::Die:
		UpdateDie();
		break;
	case EEnemyBossState::ReturnPos:
		UpdateReturnPos();
		break;
	}
}

void UEnemyBossFSM::UpdateIdle()
{
	//만약에 플레이어를 쫓아 갈 수 있니?
	if (IsTargetTrace())
	{
		//상태를 Move 로 전환
		ChangeState(EEnemyBossState::Move);
	}
	else
	{
		//idleDelayTime 이 지나면	
		if (IsWaitComplete(idleDelayTime))
		{
			//현재상태를 Move 로 한다.
			ChangeState(EEnemyBossState::Move);
		}
	}
}

void UEnemyBossFSM::UpdateMove()
{
	// 시야에 들어왔는지 여부
	bool bTrace = IsTargetTrace();

	//1. 타겟을 향하는 방향을 구하고(target - me)
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//처음 위치, 나의 현재 위치의 거리
	float dist = FVector::Distance(originPos, me->GetActorLocation());

	//만약에 dist 가 moveRange 보다 커지면 (움직일 수 있는 반경을 넘어갔다면)
	if (dist > moveRange)
	{
		//상태를 ReturnPos 로 변경
		UE_LOG(LogTemp, Warning, TEXT("ReturnPos"));
		ChangeState(EEnemyBossState::ReturnPos);
	}
	//만약에 시야에 들어왔다면
	else if (bTrace)
	{
		UE_LOG(LogTemp, Warning, TEXT("bTrace"));

		//만약에 target - me 거리가 공격범위보다 작으면
		if (dir.Length() < attackRange)
		{
			//상태를 Attack 으로 변경
			ChangeState(EEnemyBossState::AttackDelay);
		}
		//그렇지 않으면
		else
		{
			//2. 그 방향으로 이동하고 싶다.
			//me->AddMovementInput(dir.GetSafeNormal());
			//ai 를 이용해서 목적지까지 이동하고 싶다.	
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//시야에 들어오지 않았다면
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("randPos"));
		// 랜덤한 위치까지 도착한 후 Idle 상태로 전환
		MoveToPos(randPos);
	}

}

void UEnemyBossFSM::UpdateAttack()
{	
	// 머리 휘두르며 깨물기
	if (anim->bAttackPlay0)
	{
		
	}
	
	// 양쪽에서 총알 한 발씩 X2
	else if (anim->bAttackPlay1)
	{
		
	}

	// 브레스 발사체
	else if (anim->bAttackPlay2)
	{
		//gsdfgsdfZ
	}

	// 브레스 스프레드
	else if (anim->bAttackPlay3)
	{
		//gsdfgsdf
	}
	//2. 상태를 AttackDelay 로 전환	
	ChangeState(EEnemyBossState::AttackDelay);
}

void UEnemyBossFSM::UpdaetAttackDelay()
{
	//2. 만약에 현재시간이 attackDelayTime 보다 커지면
	if (IsWaitComplete(attackDelayTime))
	{
		//3. target 과 me 거리를 구하자.
		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();
		//4. 만약에 그거리가 attackRange보다 작으면
		if (dist < attackRange)
		{
			//5. Attack 상태로 전환
			DecideAttackPattern();
			ChangeState(EEnemyBossState::Attack);
		}
		else
		{
			//6. 그렇지 않으면 Idle 상태로 가자
			ChangeState(EEnemyBossState::Idle);
		}
	}
}

void UEnemyBossFSM::UpdateDamaged()
{
	//damageDelayTime 이 지나면
	if (IsWaitComplete(damageDelayTime))
	{
		//Move 상태
		ChangeState(EEnemyBossState::Idle);
	}
}

void UEnemyBossFSM::UpdateDie()
{
	//만약에 bDieMove 가 false 라면 함수를 나가라
	if (bDieMove == false) return;

	//P = P0 + vt
	//1. 아래로 내려가는 위치를 구한다.
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;

	//2. 만약에 p.Z 가 -200 보다 작으면 파괴한다
	if (p.Z < -200)
	{
		me->Destroy();

		//나를 비활성화
		//me->SetActive(false);
		////EnemyManager 찾자
		//AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass());
		//AEnemyManager* am = Cast<AEnemyManager>(actor);
		////찾은 놈에서 enemyArray 에 나를 다시 담자
		//am->enemyArray.Add(me);
		//currHP 를 maxHP
		//currHP = maxHP;
		//상태를 Idle
		//ChangeState(EEnemyBossState::Idle);
		//몽타주를 멈춰준다
		//me->StopAnimMontage(damageMontage);
		//bDieMove 를 false 로!
		//bDieMove = false;
	}
	//3. 그렇지 않으면 해당 위치로 셋팅한다
	else
	{
		me->SetActorLocation(p);
	}
}

void UEnemyBossFSM::UpdateReturnPos()
{
	// 처음 위치로 가서 도착하면 Idle 상태로 전환한다.
	MoveToPos(originPos);


	////1. 나 ----> 처음위치를 향하는 방향을 구한다.
	//FVector dir = originPos - me->GetActorLocation();

	////2. 만약에 나 --- 처음위치의 거리가 10보다 작으면
	//if (dir.Length() < 10)
	//{
	//	//3. Idle 상태로 전환
	//	ChangeState(EEnemyState::Idle);
	//}
	////4. 그렇지 않으면 
	//else
	//{
	//	//5. 계속 1번에서 구한 방향으로 이동한다
	//	me->AddMovementInput(dir.GetSafeNormal());
	//}
}

void UEnemyBossFSM::ChangeState(EEnemyBossState state)
{
	//상태 변경 로그를 출력하자
// 	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
// 	if (enumPtr != nullptr)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
// 			*enumPtr->GetNameStringByIndex((int32)currState),
// 			*enumPtr->GetNameStringByIndex((int32)state));
// 	}

	//현재 상태를 갱신
	currState = state;

	//anim 의 상태 갱신
	anim->state = state;

	//현재 시간 초기화
	currTime = 0;

	//ai 의 움직임 멈추자
	ai->StopMovement();

	//상태에 따른 초기설정
	switch (currState)
	{
	case EEnemyBossState::Attack:
		//currTime = attackDelayTime;
		break;
	case EEnemyBossState::Move:
	{
		//네비게이션 시스템 가져오자
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치를 얻오자
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyBossState::Damaged:
	{
		//1. 랜덤한 값을 뽑는다 (0, 1 중)
		//int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 이란 문자열을 만든다.
		//FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. 몽타주를 플레이한다.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Damage0")));
		
		FTimerHandle WaitHandle;
		float WaitTime = 20.0f; //시간을 설정하고
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				currShieldGauge = maxShieldGauge;
				ChangeState(EEnemyBossState::Idle);
			}), WaitTime, false);
	}
	break; 
	case EEnemyBossState::Die:
		//충돌안되게 설정
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Die 몽타주 실행
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));
		break;
	}
}

void UEnemyBossFSM::ReceiveDamage()
{	
	if (currShieldGauge < 1)
	{
		//피를 줄이자
		currHP--;
		//hp 가 0보다 크면 Damage 상태로 전환
		if (currHP > 0)
		{
			//ChangeState(EEnemyBossState::Damaged);
		}
		//그렇지 않으면 Die 상태로 전환
		else
		{
			ChangeState(EEnemyBossState::Die);
		}
	}
	else
	{
		currShieldGauge--;
		if (currShieldGauge < 1)
		{
			ChangeState(EEnemyBossState::Damaged);
		}
	}
}

bool UEnemyBossFSM::IsWaitComplete(float delayTime)
{
	//1. 시간을 흐르게 한다.
	currTime += GetWorld()->DeltaTimeSeconds;

	//2. 만약에 현재시간이 delayTime보다 커지면 
	if (currTime > delayTime)
	{
		//3. 현재시간을 0으로 초기화
		currTime = 0;
		//4. true 반환
		return true;
	}

	//5. 그렇지 않으면 false 를 반환
	return false;
}


bool UEnemyBossFSM::IsTargetTrace()
{
	UE_LOG(LogTemp, Warning, TEXT("IsTargetTrace"));

	//1. 나 ----> 플레이어를 향하는 벡터
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//2. 나의 앞방향과 1번에 구한 벡터의 내적
	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dir.GetSafeNormal());

	//3. 2번에 구한 값을 Acos --> 두 벡터의 사이각
	float angle = UKismetMathLibrary::DegAcos(dotValue);

	//4. 만약에 3번에서 구한 값이 30보다 작고(시야각 60)
	//   나 - 타겟 과의 거리가 traceRange 보다 작으면
	if (angle < 30 && dir.Length() < traceRange)
	//if (dir.Length() < traceRange)
	{
		//Enemy -----> target LineTrace 쏘자!!
		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);
		DrawDebugLine(GetWorld(), me->GetActorLocation(), target->GetActorLocation(), FColor::Cyan);
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			hitInfo,
			me->GetActorLocation(),
			target->GetActorLocation(),
			ECollisionChannel::ECC_Visibility,
			param);

		//만약에 부딪힌 곳이 있다면
		if (bHit)
		{
			//만약에 부딪힌 놈의 이름이 Player 를 포함하고 있다면
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Character")))
			{
				//5. true 반환
				return true;
			}
		}
	}
	//6. 그렇지 않으면 false 반환
	return false;
}

void UEnemyBossFSM::MoveToPos(FVector pos)
{
	//해당 위치(pos) 로 간다
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle 상태로 전환
		ChangeState(EEnemyBossState::Idle);
	}
}

void UEnemyBossFSM::DecideAttackPattern()
{	
	InitAttackPattern();
	int32 randPattern = FMath::RandRange(0, 3);
	//UE_LOG(LogTemp, Warning, TEXT("attackNum : %d"), randPattern);
	
	switch (randPattern)
	{
	case 0:
		anim->bAttackPlay0 = true;
		break;
	case 1:
		anim->bAttackPlay1 = true;
		break;
	case 2:
		anim->bAttackPlay2 = true;
		break;
	case 3:
		anim->bAttackPlay3 = true;
		break;
	default:
		break;
	}
}

void UEnemyBossFSM::InitAttackPattern()
{
	anim->bAttackPlay0 = false;
	anim->bAttackPlay1 = false;
	anim->bAttackPlay2 = false;
	anim->bAttackPlay3 = false;
}

void UEnemyBossFSM::SpawnFireball()
{
	AEnemyBossFireball* fireball = GetWorld()->SpawnActor<AEnemyBossFireball>(FireballFactory, me->GetMesh()->GetSocketTransform(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));
}

void UEnemyBossFSM::SpawnFireSpread()
{
	AEnemyBossFireSpread* fireSpread = GetWorld()->SpawnActor<AEnemyBossFireSpread>(FireSpreadFactory, me->GetMesh()->GetSocketTransform(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));

}
