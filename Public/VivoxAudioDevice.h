/* Copyright (C) Siqi.Wu - All Rights Reserved

 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019

 */



#pragma once



#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "Engine/LatentActionManager.h"



#include "VivoxCore.h"



#include "VivoxAudioDevice.generated.h"



UCLASS(BlueprintType)

class VIVOX_API UVivoxAudioDevice : public UObject

{

public:

    GENERATED_BODY()



public:

    static UVivoxAudioDevice *CreateVivoxAudioDevice(IAudioDevice *VivoxVoiceAudioDevice);



	virtual void BeginDestroy() override;



    /**

     * \brief User displayable device name

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    FString Name() const;



    /**

     * \brief Internal Device Identifier

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    FString Id() const;



    /**

     * \brief Returns true iff both the Name and Id properties are empty

     */

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "vivox")

    bool IsEmpty() const;



    IAudioDevice *VivoxVoiceAudioDevice;

};

