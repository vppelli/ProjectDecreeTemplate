// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AM_ProjectDecree.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDECREE_API UAM_ProjectDecree : public UAssetManager
{
	GENERATED_BODY()
	
public:
	virtual void StartInitialLoading() override;
};
