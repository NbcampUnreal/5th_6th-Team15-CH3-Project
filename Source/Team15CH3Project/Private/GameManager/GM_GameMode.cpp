// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager/GM_GameMode.h"
#include "GameManager/GS_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/PlayerStart.h"
// #include "PlayerMade/Controller/PlayerMadeController.h"
// #include "PlayerMade/PlayerCharacter.h"

AGM_GameMode::AGM_GameMode()
{
    // Tick 활성화 
    PrimaryActorTick.bCanEverTick = true;

    /* 캐릭터와 플레이어 컨트롤 연동
    DefaultPawnClass = PlayerCharacter::StaticClass();
    PlayerControllerClass = PlayerMadeController::StaticClass(); */

    // 기본값 
    bIsGameOver = false;
    MissionSpawnDelay = 10.0f;

    // GameState 클래스 지정 
    GameStateClass = AGS_GameState::StaticClass();
}

void AGM_GameMode::BeginPlay()
{
    Super::BeginPlay();
    StartGame();
}

void AGM_GameMode::StartGame()
{
    bIsGameOver = false;

    // 게임스테이트 불러오기
    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        // 처음 생성시 초기화
        GS->EnemyKillCount= 0;
        GS->Stamina = 100.0f;
        GS->CurrentWave = 0;
        GS->bBossDefeated = false;
        GS->bMissionFailed = false;
    }

    ResetGame();

    // 일정 시간 후 미션 생성 
    GetWorldTimerManager().SetTimer(
        MissionTimerHandle,
        this,
        &AGM_GameMode::GenerateMission,
        MissionSpawnDelay,
        false
    );
    //첫번째 웨이브
    StartNextWave();
}

void AGM_GameMode::ResetGame()
{
    // 현재 월드 
    UWorld* World = GetWorld();
    if (!World) return;

    // 플레이어 컨트롤러 
    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC) return;

    // 플레이어 캐릭터 
    APawn* PlayerPawn = PC->GetPawn();
    if (PlayerPawn)
    {
        // 기존 캐릭터 제거
        PlayerPawn->Destroy();
    }

    // 플레이어 스폰 위치 
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), PlayerStarts);

    if (PlayerStarts.Num() > 0)
    {
        // 첫 번째 PlayerStart 위치 
        FVector SpawnLocation = PlayerStarts[0]->GetActorLocation();
        FRotator SpawnRotation = PlayerStarts[0]->GetActorRotation();

        // 캐릭터 스폰
        APawn* NewPawn = World->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation);
        if (NewPawn)
        {
            // 플레이어 컨트롤러에 소유권 설정
            PC->Possess(NewPawn);
        }
    }
}





void AGM_GameMode::StartNextWave()
{
    if (bIsGameOver) return;

    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        // 다음 웨이브
        GS->CurrentWave++;
    }
    // 적 스폰
    SpawnEnemiesForWave();
}

void AGM_GameMode::SpawnEnemiesForWave()
{
    // 1.적 스폰 로직 작성하기
}

void AGM_GameMode::GenerateMission()
{
    // 2.랜덤 미션 생성 로직 작성하기
}

void AGM_GameMode::MissionSuccess()
{
    // 미션 성공시 캐릭터 강화 로직 작성해주세요
}

void AGM_GameMode::MissionFail()
{
    // 미션 실패시 적 강화 로직 작성해주세요
}

void AGM_GameMode::CheckWinCondition()
{
    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        // 보스가 처치 시 게임 종료
        if (GS->bBossDefeated)
        {
            EndGame();
        }
    }
}

void AGM_GameMode::CheckLoseCondition()
{
    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        // 체력 0 이하일 시 게임 오버
        if (GS->Stamina <= 0)
        {
            EndGame();
        }
    }
}

void AGM_GameMode::EndGame()
{
    bIsGameOver = true;

    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        //게임 종료 UI 출력
    }
}

void AGM_GameMode::PrintCurrentWave()
{
    if (bIsGameOver) return;

    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        //UI출력으로 바꾸기
        UE_LOG(LogTemp, Log, TEXT("현재 웨이브: %d"), GS->CurrentWave);
    }
}


void AGM_GameMode::PlayerHit(float Damage)
{
    if (bIsGameOver) return;

    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        //공격 처리 후 패배 조건 확인
        GS->ModifyPlayerHealth(-Damage);
        CheckLoseCondition();
    }
}
