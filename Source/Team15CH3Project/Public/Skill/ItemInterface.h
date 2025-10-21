// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAM15CH3PROJECT_API IItemInterface
{
	GENERATED_BODY()

public:
	virtual void OnItemOverlap(AActor* OverlapActor) = 0; //�ڿ� '= 0;' ������ �ݵ�� �������̵� �ʿ�
	virtual void OnItemEndOverlap(AActor* OverlapActor) = 0; //���� ���� �Լ�
	virtual void ActivateItem(AActor* Acivator) = 0;
	virtual FName GetIemType() const = 0; //FString ���� Ÿ�� �к��� FName�� �� ����
};
