// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Warp.generated.h"

/**
 * 
 */
UCLASS()
class U02_ACTION_API ACDoAction_Warp : public ACDoAction
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Tick(float DeltaTime) override;

private:
	//플레이어 Warp인지 확인
	class ACPlayer* UserCursorLocation(); 

	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator);

private:
	//데칼
	class UDecalComponent* Decal; 
	
	//마우스위치
	FVector Location;
};
