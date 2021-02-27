/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/LatentActionManager.h"

#include "VivoxCore.h"

#include "VivoxAccountId.h"
#include "VivoxParticipantProperties.h"

#include "VivoxParticipant.generated.h"

UCLASS(BlueprintType)
class VIVOX_API UVivoxParticipant : public UVivoxParticipantProperties
{
public:
    GENERATED_BODY()

public:
    static UVivoxParticipant *CreateVivoxParticipant(IParticipant *VivoxParticipant);

	virtual void BeginDestroy() override;

    /**
     * \brief True if this participant corresponds to the currently connected user.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    FString ParticipantId() const;

    /**
     * \brief The ChannelSession that owns this Participant

     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    UVivoxChannelSession *ParentChannelSession() const;

    /**
     * \brief The AccountId associated with this Participant
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    FVivoxAccountId Account() const;

    IParticipant *VivoxParticipant;
};

