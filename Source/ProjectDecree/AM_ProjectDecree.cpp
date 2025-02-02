// Fill out your copyright notice in the Description page of Project Settings.


#include "AM_ProjectDecree.h"
#include "AbilitySystemGlobals.h"

void UAM_ProjectDecree::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}