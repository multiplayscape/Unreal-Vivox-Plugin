/* Copyright (C) Siqi.Wu - All Rights Reserved

 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019

 */



#pragma once



#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "Engine/LatentActionManager.h"



#include "VivoxCore.h"



#include "VivoxConnectionState.h"



#include "VivoxChannelConnectionState.generated.h"



class UVivoxChannelSession;



UCLASS(BlueprintType)

class VIVOX_API UVivoxChannelConnectionState : public UObject

{

public:

    GENERATED_BODY()



public:

    static UVivoxChannelConnectionState *CreateVivoxChannelConnectionState(IChannelConnectionState *VivoxChannelConnectionState);



	virtual void BeginDestroy() override;



    /**

     * \brief The connection state of the resource.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    EVivoxConnectionState State() const;



    /**

     * \brief The ChannelSession in which this connection state change took place.

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    UVivoxChannelSession *ChannelSession() const;



    IChannelConnectionState *VivoxChannelConnectionState;

};

