/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#pragma once

#include "Channel3DProperties.h"

#include "VivoxChannel3DProperties.generated.h"

USTRUCT(BlueprintType)
struct FVivoxChannel3DProperties {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    int audibleDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    int conversationalDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    float audioFadeIntensityByDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	EAudioFadeModel audioFadeModel = EAudioFadeModel::InverseByDistance;

public:
Channel3DProperties toVivox() const {
        EAudioFadeModel vivoxAudioFadeModel = EAudioFadeModel::InverseByDistance;
        return Channel3DProperties(audibleDistance, conversationalDistance, (double)audioFadeIntensityByDistance, audioFadeModel);
    }

    FVivoxChannel3DProperties &fromVivox(const Channel3DProperties& vivoxChannel3DProperties) {
        this->audibleDistance = vivoxChannel3DProperties.AudibleDistance();
        this->conversationalDistance = vivoxChannel3DProperties.ConversationalDistance();
        this->audibleDistance = (float)vivoxChannel3DProperties.AudioFadeIntensityByDistance();
		this->audioFadeModel = vivoxChannel3DProperties.AudioFadeModel();

        return *this;
    }
};