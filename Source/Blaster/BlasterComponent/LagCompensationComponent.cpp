// Fill out your copyright notice in the Description page of Project Settings.


#include "LagCompensationComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Components/BoxComponent.h"

ULagCompensationComponent::ULagCompensationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void ULagCompensationComponent::BeginPlay()
{
	Super::BeginPlay();

	FFramePackage FramePackage;
	SaveFramePackage(FramePackage);
	ShowFramePackage(FramePackage,FColor::Red);
}

void ULagCompensationComponent::SaveFramePackage(FFramePackage& Package)
{
	Character=Character==nullptr?Cast<ABlasterCharacter>(GetOwner()):Character;
	if (Character)
	{
		Package.Time = GetWorld()->GetTimeSeconds();
		for (auto BoxPair : Character->HitBoxes) 
		{
			FBoxInfomation BoxInfomation;
			BoxInfomation.Location = BoxPair.Value->GetComponentLocation();
			BoxInfomation.Rotation = BoxPair.Value->GetComponentRotation();
			BoxInfomation.BoxExtent = BoxPair.Value->GetScaledBoxExtent();
			Package.HitBoxInfo.Add(BoxPair.Key,BoxInfomation );
		}
	}
}

void ULagCompensationComponent::ShowFramePackage(const FFramePackage& Package, const FColor& Color)
{
	for (auto& BoxInfo : Package.HitBoxInfo)
	{
		DrawDebugBox(GetWorld(),BoxInfo.Value.Location,BoxInfo.Value.BoxExtent,FQuat(BoxInfo.Value.Rotation),Color,false,1.f);
	}
}

void ULagCompensationComponent::ServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize HitLocation, float HitTime)
{
	bool bReturn = HitCharacter == nullptr ||
		HitCharacter->GetLagCompensation() == nullptr ||
		HitCharacter->GetLagCompensation()->FrameHistory.GetHead() == nullptr ||
		HitCharacter->GetLagCompensation()->FrameHistory.GetTail() == nullptr;
	
	FFramePackage FrameToCheck;
	bool bShouldInterpolate = true;

	const TDoubleLinkedList<FFramePackage>* History = &HitCharacter->GetLagCompensation()->FrameHistory;
	const float OldestHistoryTime=History->GetTail()->GetValue().Time;
	const float NewestHistoryTime=History->GetHead()->GetValue().Time;

	if (OldestHistoryTime> HitTime)
	{
		return;
	}
	if (OldestHistoryTime==HitTime)
	{
		FrameToCheck = History->GetHead()->GetValue();
		bShouldInterpolate=false;
	}
	if (NewestHistoryTime<=HitTime)
	{
		FrameToCheck=History->GetHead()->GetValue();
		bShouldInterpolate=false;
	}

	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Younger=History->GetHead();
	TDoubleLinkedList<FFramePackage>::TDoubleLinkedListNode* Older = Younger;
	while (Older->GetValue().Time>HitTime)
	{
		//March back until: OlderTime < HitTime < YoungerTime
		if (Older->GetNextNode() == nullptr)break;
		Older=Older->GetNextNode();
		if (Older->GetValue().Time>HitTime)
		{
			Younger = Older;
		}
	}
	if (Older->GetValue().Time==HitTime) // hightly unlikely, but we found frame to check
	{
		FrameToCheck = Older->GetValue();
		bShouldInterpolate=false;
	}
	if (bShouldInterpolate)
	{
		//interpolate between Older and Younger
	}
	if (bReturn)	return;
}

void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (FrameHistory.Num()<=1)
	{
		FFramePackage ThisFrame;
		SaveFramePackage(ThisFrame);
		FrameHistory.AddHead(ThisFrame);
	}
	else
	{
		float HistoryLenth=FrameHistory.GetHead()->GetValue().Time-FrameHistory.GetTail()->GetValue().Time;
		while (HistoryLenth>MaxRecordTime)
		{
			FrameHistory.RemoveNode(FrameHistory.GetTail());
			HistoryLenth=FrameHistory.GetHead()->GetValue().Time-FrameHistory.GetTail()->GetValue().Time;
		}
		FFramePackage ThisFrame;
		SaveFramePackage(ThisFrame);
		FrameHistory.AddHead(ThisFrame);

		ShowFramePackage(ThisFrame,FColor::Red);
	}
}


