#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Skill/PassiveItem.h"
#include "Blueprint/UserWidget.h"

// HUD 위젯에서 사용되는 컴포넌트
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

// PlayerCharacter, StatsComponent
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"

#include "CommonUserWidget_BattleGameHUD.generated.h"


UCLASS()
class TEAM15CH3PROJECT_API UCommonUserWidget_BattleGameHUD : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
    //Image
    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_1;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_2;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_3;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_4;

    //Stack
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackCnt_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackCnt_2;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackCnt_3;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackCnt_4;

    UPROPERTY()
    EPassiveItemType SkillSlot_1_Type = EPassiveItemType::None;

    UPROPERTY()
    EPassiveItemType SkillSlot_2_Type = EPassiveItemType::None;

    UPROPERTY()
    EPassiveItemType SkillSlot_3_Type = EPassiveItemType::None;

    UPROPERTY()
    EPassiveItemType SkillSlot_4_Type = EPassiveItemType::None;

    UPROPERTY()
    TArray<EPassiveItemType> AssignedSkillTypes;



    //HP & MP 바인드
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HPText;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* MPBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MPText;

    //플레이어 캐릭터 참조
    UPROPERTY()
    APlayerCharacter* MyPlayerChar;

    //NativeTick 매프레임 업데이트 할 때 사용
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

    //초기화
    UFUNCTION(BlueprintCallable)
    void InitHUD();

};