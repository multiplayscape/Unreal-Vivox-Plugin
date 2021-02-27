/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/LatentActionManager.h"

#include "VivoxCore.h"

#include "VivoxTextMessage.h"

#include "VivoxDirectedTextMessage.generated.h"

class UVivoxLoginSession;

UCLASS(BlueprintType)
class VIVOX_API UVivoxDirectedTextMessage : public UVivoxTextMessage {
public:
    GENERATED_BODY()

public:
    static UVivoxDirectedTextMessage *CreateVivoxDirectedTextMessage(IDirectedTextMessage *VivoxDirectedTextMessage);

	virtual void BeginDestroy() override;

    /**
     * \brief The LoginSession that is the target of the message
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")
    UVivoxLoginSession *LoginSession();

    IDirectedTextMessage *VivoxDirectedTextMessage;
};

