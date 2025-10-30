#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Skill/PassiveItem.h"
#include "Blueprint/UserWidget.h"

// HUD �������� ���Ǵ� ������Ʈ
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



    //HP & MP ���ε�
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HPText;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* MPBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MPText;

    //�÷��̾� ĳ���� ����
    UPROPERTY()
    APlayerCharacter* MyPlayerChar;

    //NativeTick �������� ������Ʈ �� �� ���
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

    //�ʱ�ȭ
    UFUNCTION(BlueprintCallable)
    void InitHUD();

};