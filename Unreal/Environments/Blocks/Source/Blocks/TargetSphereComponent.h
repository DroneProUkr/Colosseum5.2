// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetSphereComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLOCKS_API UTargetSphereComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTargetSphereComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

private:

	void SetPosition(FVector position);
	void ResetPosition();

private:

	int m_currentWP;
	FVector m_flightDirection;
};
