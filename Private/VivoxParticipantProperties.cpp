/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxParticipantProperties.h"

#include "Presence.h"
#include "VivoxCore.h"

#include "VivoxGlobals.h"

UVivoxParticipantProperties *UVivoxParticipantProperties::CreateVivoxParticipantProperties(IParticipantProperties *participantProperties) {
    UVivoxParticipantProperties *vivoxParticipantProperties = NewObject<UVivoxParticipantProperties>();
    vivoxParticipantProperties->VivoxParticipantProperties = participantProperties;

    return vivoxParticipantProperties;
}

void UVivoxParticipantProperties::BeginDestroy() {

	VivoxParticipantProperties = nullptr;

	this->UObject::BeginDestroy();

}

bool UVivoxParticipantProperties::IsSelf() const {
    return this->VivoxParticipantProperties->IsSelf();
}

bool UVivoxParticipantProperties::InAudio() const {
    return this->VivoxParticipantProperties->InAudio();
}

bool UVivoxParticipantProperties::InText() const {
    return this->VivoxParticipantProperties->InText();
}

bool UVivoxParticipantProperties::SpeechDetected() const {
    return this->VivoxParticipantProperties->SpeechDetected();
}

float UVivoxParticipantProperties::AudioEnergy() const {
    return (float) this->VivoxParticipantProperties->AudioEnergy();
}

int UVivoxParticipantProperties::LocalVolumeAdjustment() const {
    return this->VivoxParticipantProperties->LocalVolumeAdjustment();
}

EVivoxStatus UVivoxParticipantProperties::SetLocalVolumeAdjustment(int value) {
    return fromVivox(this->VivoxParticipantProperties->SetLocalVolumeAdjustment(value));
}

bool UVivoxParticipantProperties::LocalMute() const {
    return this->VivoxParticipantProperties->LocalMute();
}

void UVivoxParticipantProperties::SetLocalMute(bool value) {
    this->VivoxParticipantProperties->SetLocalMute(value);
}

bool UVivoxParticipantProperties::IsTyping() const {
    return this->VivoxParticipantProperties->IsTyping();
}

bool UVivoxParticipantProperties::IsMutedForAll() const {
    return this->VivoxParticipantProperties->IsMutedForAll();
}

class FBeginSetIsMutedForAllAction : public FPendingLatentAction {
private:
    // trigger for finish
    bool completed = false;
    IParticipantProperties *VivoxParticipantProperties;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

public:
    FBeginSetIsMutedForAllAction(
            IParticipantProperties *vivoxParticipantProperties,
            bool setMuted,
            const FString &accessToken,
            EVivoxStatus &status,
            const FLatentActionInfo &LatentInfo
    )
            : VivoxParticipantProperties(vivoxParticipantProperties),
              ExecutionFunction(LatentInfo.ExecutionFunction),
              OutputLink(LatentInfo.Linkage), CallbackTarget(LatentInfo.CallbackTarget) {

        IParticipantProperties::FOnBeginSetIsMutedForAllCompletedDelegate OnBeginSetIsMutedForAllCompleteCallback;
        OnBeginSetIsMutedForAllCompleteCallback.BindLambda(
                [this, &vivoxParticipantProperties, &status](VivoxCoreError Status) {
                    status = fromVivox(Status);
                    if (status != EVivoxStatus::Success) {
                        LOG_VIVOX_WARNING(FString::Printf(TEXT("Login failed: %s"),
                                                          ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status))));
                        completed = true;
                    } else {
                        completed = true;

                    }
                });

        status = fromVivox(
                VivoxParticipantProperties->BeginSetIsMutedForAll(setMuted, accessToken,
                                                                   OnBeginSetIsMutedForAllCompleteCallback));
    }

    void UpdateOperation(FLatentResponse &Response) override {
        Response.FinishAndTriggerIf(completed, ExecutionFunction, OutputLink, CallbackTarget);
    }

#if WITH_EDITOR

    // Returns a human readable description of the latent operation's current state
    FString GetDescription() const override {
        return *NSLOCTEXT("Begin Login Vivox", "server", " ").ToString();
    }

#endif
};

void UVivoxParticipantProperties::BeginSetIsMutedForAll(
        UObject *WorldContextObject,
        struct FLatentActionInfo LatentInfo,
        bool setMuted,
        const FString &accessToken,
        EVivoxStatus &status
) {
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)) {
        FLatentActionManager &LatentActionManager = World->GetLatentActionManager();
        if (LatentActionManager.FindExistingAction<FBeginSetIsMutedForAllAction>(LatentInfo.CallbackTarget,
                                                                                 LatentInfo.UUID) ==
            NULL) {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
                                             new FBeginSetIsMutedForAllAction(this->VivoxParticipantProperties,
                                                                              setMuted,
                                                                              accessToken,
                                                                              status,
                                                                              LatentInfo
                                             ));
        }
    }
}

FString UVivoxParticipantProperties::GetMuteForAllToken(const FString &tokenSigningKey,
                                                        FTimespan tokenExpirationDuration) const {
    return this->VivoxParticipantProperties->GetMuteForAllToken(tokenSigningKey, tokenExpirationDuration);
}